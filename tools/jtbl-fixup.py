import sys
import re
from os.path import exists
from makeelf.elf import *
from makeelf.elf import _Strtab, _Symtab

R_MIPS_32 = 2

def error(msg, res=1):
    color = 40 + res
    msg = f"\033[1;{color}m" + msg + "\033[0m"
    print(msg, file=sys.stderr)
    sys.exit(res)

smolprintLen = 0
def smolprint(s, d="   ", n=100):
    global smolprintLen
    s += d
    smolprintLen += len(s)
    if smolprintLen > n:
        print()
        smolprintLen = len(s)
    print(s, end="")

def assertFilesExist(*args):
    for fn in args:
        if not exists(fn):
            error(f"!! file not found: {fn}")

def getInt(b):
    return int.from_bytes(b, 'little')

def swapWord(s):
    return int.from_bytes(s.to_bytes(4, 'little'), 'big')

def swapHalfWord(s):
    return ((s & 0x00ff) << 8) | ((s & 0xff00) >> 8)

def getNameFromSymbol(sym, strtab):
    start = swapWord(sym.st_name)
    end = strtab.find(b'\0', start)
    return strtab.blob[start:end].decode('ascii')

def getSymbolNames(symtab, strtab):
    lst = []
    for sym in symtab.lst:
        lst.append(getNameFromSymbol(sym, strtab))
    return lst

def getSectionNames():
    #shstrhdr = elf.Shdr_table[1]
    shstrtab = _Strtab(elf.sections[1])
    lst = []
    for s in elf.Shdr_table:
        start = s.sh_name
        end = shstrtab.find(b'\0', start)
        lst.append(shstrtab.blob[start:end].decode('ascii'))
    return lst

class Section:
    def __init__(self, idx, hdr, dat):
        self.idx = idx
        self.hdr = hdr
        self.dat = dat

def getSectionByName(name):
    if sectionNames.count(name):
        idx = sectionNames.index(name)
        hdr = elf.Shdr_table[idx]
        data = elf.sections[idx]
        return Section(idx, hdr, data)
    return None

def sectionExists(name):
    return sectionNames.count(name) > 0

def sectionsExist(*args, report=False):
    res = True
    missing = []
    for name in args:
        if not sectionExists(name):
            missing.append(name)
            res = False
    if report and not res:
        print("missing sections: " + str(missing))
    return res

# this was tuned specifically for cc1_v258; may need reworking for v263
def parseAssembly(filename):
    
    def seekJtblLabel(reg, lines, start):
        for i in range(start, -1, -1):
            a = lines[i].split()
            if len(a) == 0 or a[0] in [".set", "nop", "#nop"]:
                continue
            elif a[0] == "lw":
                #e.g.     lw    $2,$L20($2)
                match = re.search(r"^(\$\d+),(\$L\w+)\((\$\d+)\)$", a[1])
                if match and match.group(1) == reg and match.group(3) == reg:
                    return match.group(2)
            else:
                return False
                
    def buildJtblList(lines, start):
        lst = []
        for i in range(start, len(lines)):
            a = lines[i].split()
            if len(a) == 2 and a[0] == ".word" and a[1].startswith("$L"):
                lst.append(a[1])
            else:
                break
        return lst
               
    def cmp(s, parts):
        return s.split() == parts
    
    info = []
    function = None

    with open(filename) as file:
        lines = file.readlines()

    for i in range(0, len(lines)):
        line = lines[i]
        if line.find(".ent") != -1:
            if function:
                error("asm: !! unexpected .ent")
            a = line.split()
            function = a[1]
        elif line.find(".end") != -1:
            a = line.split()
            if a[1] != function:
                error("asm: !! unexpected .end")
            function = None
        elif line.find(".rdata") != -1:
            entry = {"isJtbl": False}
            info.append(entry)
            if (line.strip() != ".rdata"):
                error("asm: !! unexpected .rdata")
            # expecting some surrounding lines
            if (i < 5) or (i+4 >= len(lines)):
                continue
            a = lines[i-1].split()
            # e.g.     j    $2
            if len(a) != 2 or a[0] != "j":
                continue

            reg = a[1]
            label = seekJtblLabel(reg, lines, i-2)
            
            if not label:
                continue
            if not cmp(lines[i+1], [".align", "3"]):
                continue
            
            if cmp(lines[i+2], [".align", "2"]):
                #cc1_v258
                start = i+3
            else:
                #cc1_v263
                start = i+2

            if lines[start].strip() != label+":":
                continue
            
            lst = buildJtblList(lines, start+1)
            entry["isJtbl"] = True
            entry["label"] = label
            entry["count"] = len(lst)
            entry["entries"] = lst
            entry["function"] = function
            
    return info

def getCombinedJtblCount(lst):
    total = 0
    for entry in lst:
        if entry["isJtbl"]:
            total += entry["count"]
    return total

def parseRdata():
    relRdSec = getSectionByName('.rel.rdata')
    rdataSec = getSectionByName('.rdata')
    
    if not (relRdSec.hdr.sh_entsize == 8 and relRdSec.hdr.sh_type == SHT.SHT_REL and relRdSec.hdr.sh_addralign == 4):
        error('!! unexpected .rel.rdata format')
    if not (rdataSec.hdr.sh_type == SHT.SHT_PROGBITS and rdataSec.hdr.sh_addralign == 8):
        error('!! unexpected .rdata format')
        
    relocs = []
    sectionSize = rdataSec.hdr.sh_size
    sectionOffset = rdataSec.hdr.sh_offset
    
    if sectionSize == 0:
        error("rdata: !! sectionSize")
        
    for i in range(0, len(relRdSec.dat), 8):
        reloc = relRdSec.dat[i:i+8]
        offset = getInt(reloc[:4])
        type = getInt(reloc[4:5])
        symIdx = getInt(reloc[5:])
        symName = symbolNames[symIdx]
        if type == R_MIPS_32 and symName.startswith("$.rel.text@"):
            # just extracting the target offset from sym name for now
            value = int(symName[11:], 16)
            relocs.append((offset, value))
    
    return {"size": sectionSize, "offset": sectionOffset, "relocs": relocs}



if len(sys.argv) != 1+2:
    error("args: asm.s obj.o", 0)

asmFilename = sys.argv[1]
objFilename = sys.argv[2]

if not (asmFilename.endswith(".s") and objFilename.endswith(".o")):
    print("warning: args?")
    
assertFilesExist(asmFilename, objFilename)

with open(objFilename, "rb") as f:
    elf, b = Elf32.from_bytes(f.read())

sectionNames = getSectionNames()

if not sectionsExist('.symtab', '.strtab', '.rdata', '.rel.rdata', '.text', report=True):
    error('missing an expected section, exiting', 0)

symtabSec = getSectionByName('.symtab')
strtabSec = getSectionByName('.strtab')
symtab = _Symtab(symtabSec.dat)
strtab = _Strtab(strtabSec.dat)
symbolNames = getSymbolNames(symtab, strtab)

# parsing the assembly is probably not necessary, but nice to have for checking consistency
asm = parseAssembly(asmFilename)
rdata = parseRdata()

#print(asm)
#print(rdata)

if len(rdata["relocs"]) != getCombinedJtblCount(asm):
    error("!! diff counts")
    
if len(rdata["relocs"]) == 0:
    # nothing to do
    sys.exit(0)
    
rdataBaseOfs = rdata["offset"]
with open(objFilename, "r+b") as file:
    for reloc in rdata["relocs"]:
        offset = rdataBaseOfs + reloc[0]
        file.seek(offset)
        if file.read(4) != b'\0\0\0\0':
            error("!! original bytes not zero")
        value = reloc[1]
        smolprint(f"@{offset:04X}:{value:04X}", d=" ")
        patch = value.to_bytes(4, 'little')
        file.seek(offset)
        file.write(patch)

if smolprintLen > 0:
    print()
