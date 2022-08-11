import sys
import os
from os.path import exists
from makeelf.elf import *
from makeelf.elf import _Strtab, _Symtab

# TODO: don't rely on sym names, do look-ups; mod sym names?

R_MIPS_26 = 4

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

def processDataSymbols(sec):
    # fix-up (r)data symbol relocations
    modified = False
    for i, sym in enumerate(symtab.lst):
        name = symbolNames[i]
        assocIdx = swapHalfWord(sym.st_shndx)
        if sym.st_info == 0x3 and assocIdx == sec.idx:
            smolprint(name)
            sym.st_info = 0x0
            modified = True
    if modified:
        elf.sections[symtabSec.idx] = symtab
    return modified

def processInstruction(data, offset, symName):
    # fix-up reloc addend for j/jal instruction;
    # just extracting offset from symbol name for now (FIXME)
    newOfs = int(symName[13:], 16)
    instruction = int.from_bytes(data[offset:offset+4], 'little')
    op = (instruction & 0xfc000000) >> 26
    oldOfs = (instruction & 0x3ffffff) << 2
    if op != 0x2 and op != 0x3:
        error('!! unexpected op')
    if newOfs != oldOfs:
        smolprint(f"@{offset:06X}: {oldOfs:06X} -> {newOfs:06X}")
        newInstruction = (op << 26) | (newOfs >> 2)
        data[offset:offset+4] = newInstruction.to_bytes(4, 'little')
        return True
    # no patching was necessary
    return False

def processTextRelocs():
    modified = False
    newBytes = bytearray(textSec.dat)
    for i in range(0, len(relTSec.dat), 8):
        reloc = relTSec.dat[i:i+8]
        offset = getInt(reloc[:4])
        type = getInt(reloc[4:5])
        symIdx = getInt(reloc[5:])
        symName = symbolNames[symIdx]
        #print(hex(offset),type,symName)
        if type != R_MIPS_26 or not symName.startswith("$.rel.text@"):
            continue
        if processInstruction(newBytes, offset, symName):
            modified = True
    if modified:
        elf.sections[textSec.idx] = newBytes
    return modified


if len(sys.argv) != 1+1:
    error("args: obj.o", 0)

objFilename = sys.argv[1]
assertFilesExist(objFilename)
modified = False

with open(objFilename, "rb") as f:
    elf, b = Elf32.from_bytes(f.read())

sectionNames = getSectionNames()
#print(sectionNames)

if not sectionsExist('.symtab', '.strtab', report=True):
    error("missing an expected section, exiting", 0)

symtabSec = getSectionByName('.symtab')
strtabSec = getSectionByName('.strtab')
symtab = _Symtab(symtabSec.dat)
strtab = _Strtab(strtabSec.dat)
symbolNames = getSymbolNames(symtab, strtab)

if sectionsExist('.text', '.rel.text'):
    relTSec = getSectionByName('.rel.text')
    textSec = getSectionByName('.text')
    sanityCheck(relTSec, textSec)

    if processTextRelocs():
        modified = True

if sectionExists('.rdata'):    
    if processDataSymbols(getSectionByName('.rdata')):
        modified = True

if sectionExists('.data'):        
    if processDataSymbols(getSectionByName('.data')):
        modified = True

if sectionExists('.sdata'):        
    if processDataSymbols(getSectionByName('.sdata')):
        modified = True

if modified:
    fd = os.open(objFilename, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
    os.write(fd, bytes(elf))
    os.close(fd)

if smolprintLen > 0:
    print()
