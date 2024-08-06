import sys
import re
import os
import argparse
from os.path import exists
from typing import Optional
from makeelf.elf import *
from makeelf.elf import _Strtab, _Symtab

R_MIPS_32 = 2
R_MIPS_26 = 4
R_MIPS_HI16 = 5
R_MIPS_LO16 = 6

#TODO: sync changes (strtab etc)
#TODO: don't rely on sym names, do look-ups;

def colorize(msg, color, fmt=""):
    return f"\033[{fmt}{color}m{msg}\033[0m"

def error(msg, res=1):
    color = 40 + res
    smolreset()
    print(colorize(msg, color, fmt="1;"), file=sys.stderr)
    sys.exit(res)

smolprintLen = 0
smolprintMax = 100
def smolprint(s, d="   ", color=None):
    global smolprintLen
    if smolprintMax <= 0: return
    newln = (smolprintLen == 0)
    s = str(s) + d
    smolprintLen += len(s)
    if smolprintLen > smolprintMax:
        if not newln: print()
        smolprintLen = len(s)
    if color is not None:
        s = colorize(s, color)
    print(s, end="")
        
def smolreset():
    global smolprintLen
    if smolprintLen > 0:
        print()
        smolprintLen = 0

def smolprintln(s):
    smolreset()
    smolprint(s)
    smolreset()

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


class ElfSection:
    
    def __init__(self, obj, idx, hdr, dat):
        self.obj = obj
        self.idx = idx
        self.hdr = hdr
        self.dat = dat

    def assign(self, newBytes):
        assert len(newBytes) == len(self.dat)
        self.dat = bytes(newBytes)
        self.obj.elf.sections[self.idx] = self.dat

class ElfObject:
    
    def __init__(self, filename):
        with open(filename, "rb") as f:
            self.elf, b = Elf32.from_bytes(f.read())

        self.sectionNames = self.getSectionNames()
        assert self.sectionsExist('.symtab', '.strtab')

        self.symtabSec = self.getSectionByName('.symtab')
        self.strtabSec = self.getSectionByName('.strtab')
        self.symtab = _Symtab(self.symtabSec.dat)
        self.strtab = _Strtab(self.strtabSec.dat)

        self.symbolNames = self.getSymbolNames()

    def save(self, filename):
        fd = os.open(filename, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
        os.write(fd, bytes(self.elf))
        os.close(fd)
  
    def getSectionNames(self):
        #shstrhdr = elf.Shdr_table[1]
        shstrtab = _Strtab(self.elf.sections[1])
        lst = []
        for s in self.elf.Shdr_table:
            start = s.sh_name
            end = shstrtab.find(b'\0', start)
            lst.append(shstrtab.blob[start:end].decode('ascii'))
        return lst

    def getSectionByName(self, name) -> Optional[ElfSection]:
        if self.sectionNames.count(name) > 0:
            idx = self.sectionNames.index(name)
            hdr = self.elf.Shdr_table[idx]
            data = self.elf.sections[idx]
            return ElfSection(self, idx, hdr, data)
        return None

    def sectionExists(self, name):
        return self.sectionNames.count(name) > 0

    def sectionsExist(self, *args, missing=[]):
        res = True
        for name in args:
            if not self.sectionExists(name):
                missing.append(name)
                res = False
        return res

    def getNameFromSymbol(self, sym):
        start = swapWord(sym.st_name)
        end = self.strtab.find(b'\0', start)
        return self.strtab.blob[start:end].decode('ascii')

    def getSymbolNames(self):
        return [self.getNameFromSymbol(sym) for sym in self.symtab.lst]



def fixupTextRelocs(obj: ElfObject):

    def sanityCheck():
        if not (relTSec.hdr.sh_entsize == 8 and relTSec.hdr.sh_type == SHT.SHT_REL and relTSec.hdr.sh_addralign == 4):
            error('!! unexpected .rel.text format')
        if not (textSec.hdr.sh_type == SHT.SHT_PROGBITS and textSec.hdr.sh_addralign == 8):
            error('!! unexpected .text format')

    def processInstruction(data, offset, symName, relocType):
        # just extracting offset from symbol name for now (FIXME)
        newOfs = int(symName[13:], 16)
        instruction = int.from_bytes(data[offset:offset+4], 'little')
        if relocType == R_MIPS_26:
            # fix-up reloc addend for j/jal instruction;
            op = (instruction & 0xfc000000) >> 26
            oldOfs = (instruction & 0x3ffffff) << 2
            if op != 0x2 and op != 0x3:
                error('!! unexpected op')
            if newOfs != oldOfs:
                smolprint(f"@{offset:06X}: {oldOfs:06X} -> {newOfs:06X}")
                newInstruction = (op << 26) | (newOfs >> 2)
                data[offset:offset+4] = newInstruction.to_bytes(4, 'little')
                return True
        elif relocType in {R_MIPS_HI16, R_MIPS_LO16}:
            # fix-up reloc for e.g. function pointer
            if relocType == R_MIPS_HI16:
                newOfs >>= 16
            elif relocType == R_MIPS_LO16:
                newOfs &= 0xffff
            oldOfs = (instruction & 0x0000ffff)
            if newOfs != oldOfs:
                smolprint(f"@{offset:06X}: {oldOfs:06X} -> {newOfs:06X} (TYPE {relocType})")
                newInstruction = (instruction & 0xffff0000) | newOfs
                data[offset:offset+4] = newInstruction.to_bytes(4, 'little')
                return True
        # no patching was necessary
        return False

    if not obj.sectionsExist('.text', '.rel.text'):
        smolprintln("Skipping.")
        return False
        
    relTSec = obj.getSectionByName('.rel.text')
    textSec = obj.getSectionByName('.text')
    sanityCheck()
    
    modified = False
    newBytes = bytearray(textSec.dat)

    for i in range(0, len(relTSec.dat), 8):
        reloc = relTSec.dat[i:i+8]
        offset = getInt(reloc[:4])
        type = getInt(reloc[4:5])
        symIdx = getInt(reloc[5:])
        symName = obj.symbolNames[symIdx]
        #print(hex(offset),type,symName)
        if type in {R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16} and symName.startswith("$.rel.text@"):
            if processInstruction(newBytes, offset, symName, type):
                modified = True

    if modified:
        textSec.assign(newBytes)
    return modified


def fixupDataSymbols(obj: ElfObject):

    def processDataSymbols(sec):
        # fix-up (r)data symbol relocations
        modified = False
        for i, sym in enumerate(obj.symtab.lst):
            name = obj.symbolNames[i]
            assocIdx = swapHalfWord(sym.st_shndx)
            if sym.st_info == 0x3 and assocIdx == sec.idx:
                smolprint(name)
                sym.st_info = 0x0
                modified = True
        if modified:
            obj.symtabSec.assign(obj.symtab)
            #elf.sections[symtabSec.idx] = symtab
        return modified

    modified = False

    if obj.sectionExists('.rdata') and processDataSymbols(obj.getSectionByName('.rdata')):
        modified = True
    if obj.sectionExists('.data') and processDataSymbols(obj.getSectionByName('.data')):
        modified = True
    if obj.sectionExists('.sdata') and processDataSymbols(obj.getSectionByName('.sdata')):
        modified = True

    return modified


def fixupJtbls(obj: ElfObject, asmFilename):

    def parseAssembly(filename):
        
        def seekJtblLabel(reg, lines, start):
            for i in range(start, -1, -1):
                a = lines[i].split()
                if len(a) == 0 or a[0] in {".set", "nop", "#nop"}:
                    continue
                elif a[0] == "lw":
                    #e.g.     lw    $2,$L20($2)
                    match = re.search(r"^(\$\d+),(\$L\w+)\((\$\d+)\)$", a[1])
                    if match and match.group(1) == reg and match.group(3) == reg:
                        return match.group(2)
                else:
                    break
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

    def sanityCheck():
        if not (relRdSec.hdr.sh_entsize == 8 and relRdSec.hdr.sh_type == SHT.SHT_REL and relRdSec.hdr.sh_addralign == 4):
            error('!! unexpected .rel.rdata format')
        if not (rdataSec.hdr.sh_type == SHT.SHT_PROGBITS and rdataSec.hdr.sh_addralign == 8):
            error('!! unexpected .rdata format')

    def parseRdata():         
        relocs = []
        sectionSize = rdataSec.hdr.sh_size
        sectionOffset = rdataSec.hdr.sh_offset
                    
        for i in range(0, len(relRdSec.dat), 8):
            reloc = relRdSec.dat[i:i+8]
            offset = getInt(reloc[:4])
            type = getInt(reloc[4:5])
            symIdx = getInt(reloc[5:])
            symName = obj.symbolNames[symIdx]
            if type == R_MIPS_32 and symName.startswith("$.rel.text@"):
                # just extracting the target offset from sym name for now
                value = int(symName[11:], 16)
                relocs.append((offset, value))
        
        return {"size": sectionSize, "offset": sectionOffset, "relocs": relocs}

    missing = []
    if not obj.sectionsExist('.rdata', '.rel.rdata', '.text', '.rel.text', missing=missing):
        smolprintln(f"Skipping - missing sections: {missing}")
        return False

    relRdSec = obj.getSectionByName('.rel.rdata')
    rdataSec = obj.getSectionByName('.rdata')
    sanityCheck()

    # parsing the assembly is probably not necessary, but nice to have for checking consistency
    asm = parseAssembly(asmFilename)
    rdata = parseRdata()

    if len(rdata["relocs"]) != getCombinedJtblCount(asm):
        error("jtbl !! diff counts")

    if len(rdata["relocs"]) == 0:
        # nothing to do
        return False

    rdataBaseOfs = rdata["offset"]
    newBytes = bytearray(rdataSec.dat)

    for reloc in rdata["relocs"]:
        prev = newBytes[reloc[0]:reloc[0]+4]
        if prev != b'\0\0\0\0':
            error("jtbl !! original bytes not zero")
        offset = rdataBaseOfs + reloc[0]
        value = reloc[1]
        smolprint(f"@{offset:04X}:{value:04X}", d=" ")
        newBytes[reloc[0]:reloc[0]+4] = value.to_bytes(4, 'little')

    rdataSec.assign(newBytes)
    return True


def fixupLargeAddends(obj: ElfObject, dumpFilename):

    def sanityCheck():
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
        # too lazy to dig deeper, but this allows a match (for now)
        newAddend = (addend + 0x8000) >> 16
        newAddend &= 0xffff
        if newAddend != oldAddend:
            smolprint(f"@{offset:06X}: {oldAddend:04X} -> {newAddend:04X}")
            newInstruction = (instruction & 0xffff0000) | (newAddend)
            data[offset:offset+4] = newInstruction.to_bytes(4, 'little')
            return True
        # no patching was necessary
        return False

    def scanDumpForLargeAddends(filename):
        lst = []
        with open(filename) as file:
            for line in file:
                a = line.split()
                if (len(a) != 5):
                    continue
                if a[0] == "HI16" and a[3] == "+" and int(a[4]) >= 0x8000:
                    # large addend
                    offset = int(a[1][7:],16)
                    addend = int(a[4])
                elif a[0] == "HI16" and a[3] == "-":
                    # negative addend
                    offset = int(a[1][7:],16)
                    addend = -int(a[4])
                else:
                    continue
                if not a[1].startswith(".text:"):
                    print("not .text?")
                    continue
                lst.append((offset, addend))
        return lst

    addends = scanDumpForLargeAddends(dumpFilename)
    if len(addends) == 0:
        return False

    missing = []
    if not obj.sectionsExist('.text', '.rel.text', missing=missing):
        smolprintln(f"Skipping - missing sections: {missing}")
        return False

    relTSec = obj.getSectionByName('.rel.text')
    textSec = obj.getSectionByName('.text')
    sanityCheck()

    modified = False
    newBytes = bytearray(textSec.dat)

    for entry in addends:
        if processInstruction(newBytes, entry[0], entry[1]):
            modified = True

    if modified:
        textSec.assign(newBytes)
    return modified

def markupSymNames(obj: ElfObject):

    def modifyName(rsym, fsym):
        #to keep it simple, new names padded/trunc'd to same len
        #TODO clashing names?
        funcName = obj.getNameFromSymbol(fsym)
        oldName = obj.getNameFromSymbol(rsym)
        assert len(oldName) == 19
        assert oldName.startswith("$") and not funcName.startswith("$")
        newName = ('$' + funcName[:18]).ljust(19, "_")
        if newName in newNames:
            smolprint(f"!! {newName}", color=33)
        newNames.add(newName)
        #smolprint(f"{oldName} -> {newName}")
        start = swapWord(rsym.st_name)
        newBytes[start:start+19] = newName.encode('ascii')
            
    if not obj.sectionExists('.text'):
        smolprintln("Skipping.")
        return False

    textSec = obj.getSectionByName('.text')
    modified = False
    newBytes = bytearray(obj.strtabSec.dat)
    newNames = set()

    # offset:sym
    unresolved = {}
    functions = {}

    for i in range(2, len(obj.symtab.lst)):
        sym = obj.symtab.lst[i]
        if swapHalfWord(sym.st_shndx) == textSec.idx:
            if sym.st_info == 3:
                unresolved[swapWord(sym.st_value)] = sym
            elif sym.st_info == 18:
                functions[swapWord(sym.st_value)] = sym  
    for ofs in unresolved:
        if ofs in functions:
            modifyName(unresolved[ofs], functions[ofs])
            modified = True

    if modified:
        obj.strtabSec.assign(newBytes)
    return modified


parser = argparse.ArgumentParser()
parser.add_argument("objFilename")
parser.add_argument("--asmFilename")
parser.add_argument("--dumpFilename")
parser.add_argument("--output")
parser.add_argument("--smolprintMax", type=int)

parser.add_argument("--fixupTextRelocs", action="store_true")
parser.add_argument("--fixupDataSymbols", action="store_true")
parser.add_argument("--fixupJtbls", action="store_true")
parser.add_argument("--fixupLargeAddends", action="store_true")
parser.add_argument("--markupSymNames", action="store_true")

args = parser.parse_args()

if args.smolprintMax is not None:
    smolprintMax = args.smolprintMax


#objFilename = "main.c.o"
objFilename = args.objFilename
assertFilesExist(objFilename)

obj = ElfObject(objFilename)
modified = False

#print(obj.sectionNames)
#print(obj.symbolNames)


if args.fixupTextRelocs:
    smolprintln(f":: fixupTextRelocs ({objFilename})...")
    if fixupTextRelocs(obj):
        modified = True

if args.fixupDataSymbols:
    smolprintln(f":: fixupDataSymbols ({objFilename})...")
    if fixupDataSymbols(obj):
        modified = True

if args.fixupJtbls:
    smolprintln(f":: fixupJtbls ({objFilename})...")
    if args.asmFilename is None or not exists(args.asmFilename):
        error("!! missing asm")
    if fixupJtbls(obj, asmFilename=args.asmFilename):
        modified = True

if args.fixupLargeAddends:
    smolprintln(f":: fixupLargeAddends ({objFilename})...")
    if args.dumpFilename is None or not exists(args.dumpFilename):
        error("!! missing dump")    
    if fixupLargeAddends(obj, dumpFilename=args.dumpFilename):
        modified = True

if args.markupSymNames:
    smolprintln(f":: markupSymNames ({objFilename})...") 
    if markupSymNames(obj):
        modified = True

if modified:
    fn = args.output or objFilename
    obj.save(fn)
    smolprintln(f":: Saved: {fn}.")

smolreset()
