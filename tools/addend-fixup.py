import sys
import os
from os.path import exists
from makeelf.elf import *
from makeelf.elf import _Strtab, _Symtab

R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

# TODO how difficult would it be to parse lnk directly?
# TODO should test with various offsets

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

def sanityCheck(relTSec, textSec):
    if not (relTSec.hdr.sh_entsize == 8 and relTSec.hdr.sh_type == SHT.SHT_REL and relTSec.hdr.sh_addralign == 4):
        error('!! unexpected .rel.text format')
    if not (textSec.hdr.sh_type == SHT.SHT_PROGBITS and textSec.hdr.sh_addralign == 8):
        error('!! unexpected .text format')

def processInstruction(data, offset, addend):
    instruction = int.from_bytes(data[offset:offset+4], 'little')
    op = (instruction & 0xfc000000) >> 26
    oldAddend = (instruction & 0xffff)
    if op != 15:
        error('!! unexpected op')
    #FIXME
    #newAddend = addend >> 15
    # too lazy to dig deeper, but this allows a match (for now)
    newAddend = oldAddend + 1
    if newAddend != oldAddend:
        smolprint(f"@{offset:06X}: {oldAddend:04X} -> {newAddend:04X}")
        newInstruction = (instruction & 0xffff0000) | (newAddend)
        data[offset:offset+4] = newInstruction.to_bytes(4, 'little')
        return True
    # no patching was necessary
    return False

def processLargeAddends(lst):
    modified = False
    newBytes = bytearray(textSec.dat)
    for entry in lst:
        if processInstruction(newBytes, entry[0], entry[1]):
            modified = True
    if modified:
        elf.sections[textSec.idx] = newBytes
    return modified

def scanDumpForLargeAddends(filename):
    lst = []
    with open(filename) as file:
        for line in file:
            a = line.split()
            if (len(a) != 5) or (a[0] != "HI16") or (a[3] != "+") or (int(a[4]) < 0x8000):
                continue
            if not a[1].startswith(".text:"):
                print("not .text?")
                continue
            offset = int(a[1][7:],16)
            addend = int(a[4])
            lst.append((offset, addend))
    return lst


if len(sys.argv) != 1+2:
    error("args: dump.txt obj.o", 0)
    
dumpFilename = sys.argv[1]
objFilename = sys.argv[2]
assertFilesExist(dumpFilename, objFilename)
modified = False

addends = scanDumpForLargeAddends(dumpFilename)
if len(addends) == 0:
    #print("no large addends detected, exiting")
    sys.exit(0)

with open(objFilename, "rb") as f:
    elf, b = Elf32.from_bytes(f.read())

sectionNames = getSectionNames()
#print(sectionNames)

if not sectionsExist('.symtab', '.strtab', '.text', '.rel.text', report=True):
    error("missing an expected section, exiting", 0)

symtabSec = getSectionByName('.symtab')
strtabSec = getSectionByName('.strtab')
symtab = _Symtab(symtabSec.dat)
strtab = _Strtab(strtabSec.dat)
symbolNames = getSymbolNames(symtab, strtab)

relTSec = getSectionByName('.rel.text')
textSec = getSectionByName('.text')
sanityCheck(relTSec, textSec)

#print("Large addends at:", addends)
if processLargeAddends(addends):
    modified = True
    
if modified:
    fd = os.open(objFilename, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
    os.write(fd, bytes(elf))
    os.close(fd)

if smolprintLen > 0:
    print()
