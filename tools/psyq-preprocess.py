#
# Trimmed to just parse+display, set up for pre-processing hacks.
#

"""
Original C++ code is from psyq-obj-parser by the PCSX-Redux authors.

/***************************************************************************
 *   Copyright (C) 2020 PCSX-Redux authors                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.           *
 ***************************************************************************/
 """

import sys
import os
from enum import IntEnum
from os.path import exists

class PsyqOpcode(IntEnum):
    END = 0
    BYTES = 2
    SWITCH = 6
    ZEROES = 8
    RELOCATION = 10
    EXPORTED_SYMBOL = 12
    IMPORTED_SYMBOL = 14
    SECTION = 16
    LOCAL_SYMBOL = 18
    FILENAME = 28
    PROGRAMTYPE = 46
    UNINITIALIZED = 48
    INC_SLD_LINENUM = 50
    INC_SLD_LINENUM_BY_BYTE = 52
    SET_SLD_LINENUM = 56
    SET_SLD_LINENUM_FILE = 58
    END_SLD = 60
    FUNCTION = 74
    FUNCTION_END = 76
    SECTION_DEF = 82
    SECTION_DEF2 = 84
    
class PsyqRelocType(IntEnum):
    REL32 = 16
    UNK_30 = 30
    REL26 = 74
    HI16 = 82
    LO16 = 84
    GPREL16 = 100
    
class PsyqExprOpcode(IntEnum):
    VALUE = 0
    SYMBOL = 2
    SECTION_BASE = 4
    SECTION_START = 12
    SECTION_END = 22
    ADD = 44
    SUB = 46
    DIV = 50
    
def vprint(msg):
    print(msg, end="")
    
def readUInt32(file):
    b = file.read(4)
    if len(b) != 4:
        return None
    return int.from_bytes(b, "little", signed=False)
        
def readUInt16(file):
    b = file.read(2)
    if len(b) != 2:
        return None
    return int.from_bytes(b, "little", signed=False)
    
def readUInt8(file):
    b = file.read(1)
    if len(b) != 1:
        return None
    return int.from_bytes(b, "little", signed=False)
    
def readString(file, n):
    b = file.read(n)
    if len(b) != n:
        return None
    return b.decode("ascii")
    
def readPsyqString(file):
    return readString(file, readUInt8(file))
    
def isprint(c):
    return c > 0x1f and c < 0x7f

def toHexString(blob):
    ret = ""
    size = len(blob)
    for lineOffset in range(0, size, 16):
        ret += f"{lineOffset:06x}: "
        text = ""
        for offset in range(16):
            if lineOffset + offset < size:
                b = blob[lineOffset + offset]
                ret += f"{b:02x} "
                if isprint(b):
                    text += chr(b)
                else:
                    text += "."
            else:
                ret += "   "
        ret += f" {text}\n"
    return ret
    
class PsyqSymbol:
    class Type(IntEnum):
        LOCAL = 0
        EXPORTED = 1
        IMPORTED = 2
        UNINITIALIZED = 3
        
    def __init__(self):
        self.offset = 0
        self.size = 0
    
    def getOffset(self, lnk):
        if self.symbolType == PsyqSymbol.Type.UNINITIALIZED:
            section = lnk.sections.get(self.sectionIndex)
            assert section is not None
            return len(section.data) + section.zeroes + self.offset
        else:
            return self.offset
    
    def display(self, lnk, key):
        if self.symbolType == PsyqSymbol.Type.EXPORTED:
            section = lnk.sections.get(self.sectionIndex)
            if section is None:
                print(f"** BROKEN SYMBOL AT INDEX {key:04x} **")
            else:
                print(f"    {key:04x}   {'EXPORT':6}   ({self.sectionIndex:04x})  {section.name:12}   {self.getOffset(lnk):08x}   {'':8}   {self.name}")
        elif self.symbolType == PsyqSymbol.Type.IMPORTED:
            print(f"    {key:04x}   {'IMPORT':6}   {'':6}  {'':12}   {'':8}   {'':8}   {self.name}")
        else:
            section = lnk.sections.get(self.sectionIndex)
            if section is None:
                print(f"** BROKEN SYMBOL AT INDEX {key:04x} **")
            else:
                print(f"    {key:04x}   {'UNDEFN':6}   ({self.sectionIndex:04x})  {section.name:12}  ({self.getOffset(lnk):08x})  {self.size:08x}   {self.name}")

class PsyqSection:  
    def __init__(self, group=-1, alignment=-1, name=""):
        self.group = group
        self.alignment = alignment
        self.name = name
        self.zeroes = 0
        self.uninitializedOffset = 0
        self.data = b""
        self.relocations = []
        self.pointer = 0
        #TODO
        
    def getFullSize(self):
        return len(self.data) + self.zeroes + self.uninitializedOffset
    
    def isBss(self):
        return self.name == ".bss" or self.name == ".sbss"
    
    def isText(self):
        return self.name == ".text"
        
    def display(self, lnk, key):
        print(f"    {key:04x}   {self.group:04x}   {self.alignment:8}   {self.getFullSize():08x}   {len(self.data):08x}   {self.zeroes:08x}   {self.uninitializedOffset:08x}   {self.name}")
        
    def displayRelocs(self, lnk):
        for reloc in self.relocations:
            reloc.display(lnk, self)
            print()
                  
class PsyqExpression:
    def __init__(self):
        self.left = None
        self.right = None
        
    @staticmethod
    def parse(file, verbose, level=0):
        ret = PsyqExpression()
        exprOp = readUInt8(file)
        ret.type = PsyqExprOpcode(exprOp)
        vprint("    ")
        vprint("  " * level)
        if exprOp == PsyqExprOpcode.VALUE:
            value = readUInt32(file)
            vprint(f"Value: {value:08x}\n")
            ret.value = value
        elif exprOp == PsyqExprOpcode.SYMBOL:
            import_ = readUInt16(file)
            vprint(f"Import: {import_}\n")
            ret.symbolIndex = import_
        elif exprOp == PsyqExprOpcode.SECTION_BASE:
            sectionIndex = readUInt16(file)
            vprint(f"Base of section {sectionIndex}\n")
            ret.sectionIndex = sectionIndex
        elif exprOp == PsyqExprOpcode.SECTION_START:
            sectionIndex = readUInt16(file)
            vprint(f"Start of section {sectionIndex}\n")
            ret.sectionIndex = sectionIndex
        elif exprOp == PsyqExprOpcode.SECTION_END:
            sectionIndex = readUInt16(file)
            vprint(f"End of section {sectionIndex}\n")
            ret.sectionIndex = sectionIndex
        elif exprOp == PsyqExprOpcode.ADD:
            vprint("Add:\n")
            ret.right = PsyqExpression.parse(file, verbose, level + 1)
            ret.left = PsyqExpression.parse(file, verbose, level + 1)
            if ret.right.type == PsyqExprOpcode.ADD and ret.left.type == PsyqExprOpcode.VALUE:
                addend = ret.left.value
                if ret.right.right.type == PsyqExprOpcode.VALUE:
                    ret.right.right.value += addend
                    return ret.right
                elif ret.right.left.type == PsyqExprOpcode.VALUE:
                    ret.right.left.value += addend
                    return ret.right
            elif ret.left.type == PsyqExprOpcode.ADD and ret.right.type == PsyqExprOpcode.VALUE:
                addend = ret.right.value
                if ret.left.right.type == PsyqExprOpcode.VALUE:
                    ret.left.right.value += addend
                    return ret.left
                elif ret.left.left.type == PsyqExprOpcode.VALUE:
                    ret.left.left.value += addend
                    return ret.left
            if (not ret.left) or (not ret.right):
                return None
        elif exprOp == PsyqExprOpcode.SUB:
            vprint("Sub:\n")
            ret.right = PsyqExpression.parse(file, verbose, level + 1)
            ret.left = PsyqExpression.parse(file, verbose, level + 1)
            if (not ret.left) or (not ret.right):
                return None
        elif exprOp == PsyqExprOpcode.DIV:
            vprint("Div:\n")
            ret.right = PsyqExpression.parse(file, verbose, level + 1)
            ret.left = PsyqExpression.parse(file, verbose, level + 1)
            if (not ret.left) or (not ret.right):
                return None
        else:
            print(f"Unknown expression type {exprOp}")
            return None
        return ret
        
    def display(self, lnk, top=False):
        if self.type == PsyqExprOpcode.VALUE:
            print(self.value, end='')
        elif self.type == PsyqExprOpcode.SYMBOL:
            symbol = lnk.symbols.get(self.symbolIndex)
            print("**ERR**" if symbol is None else symbol.name, end='')
        elif self.type == PsyqExprOpcode.SECTION_BASE:
            section = lnk.sections.get(self.sectionIndex)
            print("{}__base".format("**ERR**" if section is None else section.name), end='')
        elif self.type == PsyqExprOpcode.SECTION_START:
            section = lnk.sections.get(self.sectionIndex)
            print("{}__start".format("**ERR**" if section is None else section.name), end='')
        elif self.type == PsyqExprOpcode.SECTION_END:
            section = lnk.sections.get(self.sectionIndex)
            print("{}__end".format("**ERR**" if section is None else section.name), end='')
        elif self.type == PsyqExprOpcode.ADD:
            if self.left.type == PsyqExprOpcode.VALUE:
                if self.left.value == 0:
                    self.right.display(lnk)
                else:
                    top or print("(", end='')
                    self.right.display(lnk)
                    print(" + ", end='')
                    self.left.display(lnk)
                    top or print(")", end='')
            else:
                top or print("(", end='')
                self.left.display(lnk)
                print(" + ", end='')
                self.right.display(lnk)
                top or print(")", end='')
        elif self.type == PsyqExprOpcode.SUB:
            top or print("(", end='')
            self.left.display(lnk)
            print(" - ", end='')
            self.right.display(lnk)
            top or print(")", end='')
        elif self.type == PsyqExprOpcode.DIV:
            top or print("(", end='')
            self.left.display(lnk)
            print(" / ", end='')
            self.right.display(lnk)
            top or print(")", end='')
            
class PsyqRelocation:
    typeStr = {
        PsyqRelocType.REL32: "REL32",
        PsyqRelocType.REL26: "REL26",
        PsyqRelocType.HI16: "HI16",
        PsyqRelocType.LO16: "LO16",
        PsyqRelocType.GPREL16: "GPREL16",
        PsyqRelocType.UNK_30: "UNK_30"
    }
    
    def __init__(self, type, offset, expression):
        self.type = type
        self.offset = offset
        self.expression = expression
    
    def display(self, lnk, sec):
        print(f"    {PsyqRelocation.typeStr[self.type]:8}   {sec.name:>12}::{self.offset:08x}  ", end='')
        self.expression.display(lnk, True)
                
        
class PsyqLnkFile:
    def __init__(self):
        self.sections = {}
        self.symbols = {}
        self.currentSection = 0xffff
        self.twoPartsReloc = None
        self.localIndex = 0xffff
        
    def getCurrentSection(self):
        return self.sections.get(self.currentSection)
    
    @staticmethod
    def parse(filename, verbose, blob=None, patches=None):
        ret = PsyqLnkFile()
        gotProgramSeven = False
        
        with open(filename, "rb") as file:
            vprint(":: Reading signature.\n")
            signature = readString(file, 3)
            if signature != "LNK":
                print(f"Wrong signature: {signature}")
                return None
            vprint(" --> Signature ok.\n")
            
            vprint(":: Reading version: ")
            version = readUInt8(file)
            vprint(f"{version}\n")
            if (version != 2):
                print(f"Unknown version {version}")
                return None
            
            vprint(":: Parsing file...\n")
            while True:
                opcode = readUInt8(file)
                if opcode is None:
                    break
                vprint(f"  :: Read opcode {opcode} --> ")
                #vprint(f"{file.tell():06x}  :: Read opcode {opcode} --> ")
                #print(PsyqOpcode(opcode))
                if opcode == PsyqOpcode.END:
                    vprint("EOF\n")
                    return ret
                elif opcode == PsyqOpcode.BYTES:
                    size = readUInt16(file)
                    vprint(f"Bytes ({size:04x})\n")
                    b = file.read(size)
                    vprint(toHexString(b) + "\n")
                    section = ret.getCurrentSection()
                    if not section:
                        print(f"Section {ret.currentSection} not found")
                        return None
                    section.pointer = section.getFullSize()
                    if section.zeroes:
                        section.data += b"\0" * section.zeroes
                    section.data += b
                    continue
                elif opcode == PsyqOpcode.SWITCH:
                    sectionIndex = readUInt16(file)
                    vprint(f"Switch to section {sectionIndex}\n")
                    ret.currentSection = sectionIndex
                    continue
                elif opcode == PsyqOpcode.ZEROES:
                    size = readUInt32(file)
                    vprint(f"Zeroes ({size:04x})\n")
                    section = ret.getCurrentSection()
                    if not section:
                        print(f"Section {ret.currentSection} not found")
                        return None
                    section.zeroes += size
                    continue
                elif opcode == PsyqOpcode.RELOCATION:
                    relocType = readUInt8(file)
                    vprint(f"Relocation {relocType} ")
                    if relocType == PsyqRelocType.REL32:
                        vprint("(REL32), ")
                    elif relocType == PsyqRelocType.REL26:
                        vprint("(REL26), ")
                    elif relocType == PsyqRelocType.HI16:
                        vprint("(HI16), ")
                    elif relocType == PsyqRelocType.LO16:
                        vprint("(LO16), ")
                    elif relocType == PsyqRelocType.GPREL16:
                        vprint("(GPREL16), ")
                    elif relocType == PsyqRelocType.UNK_30:
                        vprint("(UNK_30), ")
                    else:
                        print(f"Unknown relocation type {relocType}")
                        return None
                    offset = readUInt16(file)
                    section = ret.getCurrentSection()
                    if not section:
                        print(f"Section {ret.currentSection} not found")
                        return None
                    vprint(f"offset {offset:04x}+{section.pointer:08x}, expression: \n")
                    
                    ## HACK
                    if relocType == PsyqRelocType.UNK_30:
                        assert patches is not None and blob is not None
                        expression = HACK_PatchReloc30(file, blob, patches)
                    else:
                        expression = PsyqExpression.parse(file, verbose)
                    if not expression:
                        return None
                    section.relocations.append(PsyqRelocation(relocType, offset + section.pointer, expression))
                    continue    
                elif opcode == PsyqOpcode.EXPORTED_SYMBOL:
                    symbolIndex = readUInt16(file)
                    sectionIndex = readUInt16(file)
                    offset = readUInt32(file)
                    name = readPsyqString(file)
                    vprint(f"Export: id {symbolIndex}, section {sectionIndex}, offset {offset:08x}, name {name}\n")
                    symbol = PsyqSymbol()
                    symbol.symbolType = PsyqSymbol.Type.EXPORTED
                    symbol.sectionIndex = sectionIndex
                    symbol.offset = offset
                    symbol.name = name
                    ret.symbols[symbolIndex] = symbol
                    continue
                elif opcode == PsyqOpcode.IMPORTED_SYMBOL:
                    symbolIndex = readUInt16(file)
                    name = readPsyqString(file)
                    vprint(f"Import: id {symbolIndex}, name {name}\n")
                    symbol = PsyqSymbol()
                    symbol.symbolType = PsyqSymbol.Type.IMPORTED
                    symbol.name = name
                    ret.symbols[symbolIndex] = symbol
                    continue
                elif opcode == PsyqOpcode.SECTION:
                    sectionIndex = readUInt16(file)
                    group = readUInt16(file)
                    alignment = readUInt8(file)
                    name = readPsyqString(file)
                    vprint(f"Section: id {sectionIndex}, group {group}, alignment {alignment}, name {name}\n")
                    section = PsyqSection(group, alignment, name)
                    ret.sections[sectionIndex] = section
                    if (alignment - 1) & alignment:
                        print(f"Section alignment {alignment} isn't a power of two.")
                        return None
                    continue
                elif opcode == PsyqOpcode.LOCAL_SYMBOL:
                    sectionIndex = readUInt16(file)
                    offset = readUInt32(file)
                    name = readPsyqString(file)
                    vprint(f"Local: section {sectionIndex}, offset {offset}, name {name}\n")
                    symbol = PsyqSymbol()
                    symbol.symbolType = PsyqSymbol.Type.LOCAL
                    symbol.sectionIndex = sectionIndex
                    symbol.offset = offset
                    symbol.name = name
                    ret.symbols[ret.localIndex] = symbol
                    ret.localIndex -= 1
                    continue
                elif opcode == PsyqOpcode.FILENAME:
                    index = readUInt16(file)
                    name = readPsyqString(file)
                    vprint(f"File {index}: {name}\n")
                    continue
                elif opcode == PsyqOpcode.PROGRAMTYPE:
                    type = readUInt8(file)
                    vprint(f"Program type {type}\n")
                    if type != 7:
                        print(f"Unknown program type {type}.")
                        return None
                    if gotProgramSeven:
                        print("Already got program type.")
                        return None
                    gotProgramSeven = True
                    continue
                elif opcode == PsyqOpcode.UNINITIALIZED:
                    symbolIndex = readUInt16(file)
                    sectionIndex = readUInt16(file)
                    size = readUInt32(file)
                    name = readPsyqString(file)
                    
                    symbol = PsyqSymbol()
                    symbol.symbolType = PsyqSymbol.Type.UNINITIALIZED
                    symbol.sectionIndex = sectionIndex
                    symbol.size = size
                    symbol.name = name
                    section = ret.sections.get(sectionIndex)
                    if section is None:
                        print(f"Section {sectionIndex} not found for {name}.")
                        
                    sizeToUse = symbol.size
                    if sizeToUse > section.alignment:
                        sizeToUse = section.alignment
                    align = sizeToUse - 1
                    section.uninitializedOffset += align
                    section.uninitializedOffset &= ~align
                    symbol.offset = section.uninitializedOffset
                    
                    vprint(f"Uninitialized: id {symbolIndex}, section {sectionIndex}, offset {symbol.offset:08x}, size {size:08x}, name {name}\n")
                    
                    section.uninitializedOffset += size
                    ret.symbols[symbolIndex] = symbol
                    continue
                else:
                    print(f"Unknown opcode {opcode}.")
                    return None

                print("Got actual end of file before EOF command.")
                return None
    
    def display(self):
        print("  :: Symbols\n")
        print("    indx    type    sectn                   offset      size     name")
        print("    -----------------------------------------------------------------")

        for key, symbol in sorted(self.symbols.items()):
            symbol.display(self, key)
            
        print(f"\n\n\n  :: Sections\n")
        print("    indx   grp    alignmnt   size       data       zeroes     alloc      name")
        print("    -------------------------------------------------------------------------")
        
        for key, section in sorted(self.sections.items()):
            section.display(self, key)
            
        print("\n\n\n  :: Relocations\n")
        print("    type            section::offset    expression")
        print("    ------------------------------------------")
            
        for key, section in sorted(self.sections.items()):
            section.displayRelocs(self)

def HACK_PatchReloc30(file, blob, patches):
    before = file.tell()
    expression = PsyqExpression.parse(file, verbose)
    after = file.tell()

    assert expression.type == PsyqExprOpcode.SUB
    assert blob[before-3] == PsyqRelocType.UNK_30
    newBytes = bytearray()
    newBytes.append(PsyqRelocType.GPREL16)
    newBytes += blob[before-2:before]

    if after - before == 13:
        assert expression.left.type == PsyqExprOpcode.ADD and expression.right.type == PsyqExprOpcode.SECTION_START
        assert expression.left.left.type == PsyqExprOpcode.VALUE and expression.left.right.type == PsyqExprOpcode.SECTION_BASE
        value = expression.left.left.value
        baseSection = expression.right.sectionIndex
        newBytes.append(PsyqExprOpcode.ADD)
        newBytes.append(PsyqExprOpcode.SECTION_BASE)
        newBytes += baseSection.to_bytes(2, 'little', signed=False)
        newBytes.append(PsyqExprOpcode.VALUE)
        newBytes += value.to_bytes(4, 'little', signed=False)
    elif after - before == 7:
        assert expression.left.type == PsyqExprOpcode.SYMBOL and expression.right.type == PsyqExprOpcode.SECTION_START
        newBytes.append(PsyqExprOpcode.SYMBOL)
        newBytes += expression.left.symbolIndex.to_bytes(2, 'little', signed=False)
    else:
        assert False

    patches.append((before-3, after, newBytes))
    return expression
    
smolprintLen = 0
def smolprint(s, d="   ", n=100):
    global smolprintLen
    s += d
    smolprintLen += len(s)
    if smolprintLen > n:
        print()
        smolprintLen = len(s)
    print(s, end="")


if len(sys.argv) == 1 + 1:
    lnkFilename = sys.argv[1]
    fixup = False
    verbose = True
    display = True
elif len(sys.argv) == 1 + 2:
    lnkFilename = sys.argv[1]
    outFilename = sys.argv[2]
    fixup = True
    verbose = False
    display = False
else:
    print("args: input.lnk [output.lnk]")
    sys.exit()

#verbose = True
#display = True

if not verbose:
    vprint = lambda msg: None
   
if not exists(lnkFilename):
    print(f"Unable to open file: {lnkFilename}")
    sys.exit(-1)
   
with open(lnkFilename, "rb") as file:
    blob = bytearray(file.read())
    
patches = []
lnk = PsyqLnkFile.parse(lnkFilename, verbose, blob, patches)
if lnk is None:
    sys.exit(-1)
   
if display:
    print(f":: Displaying {lnkFilename}")
    lnk.display()
   
if fixup and len(patches) > 0:
    #print("Patching: ")
    for p in reversed(patches):
        smolprint(f"@{p[0]:06x}", d="  ")
        blob[p[0]:p[1]] = p[2]
    print()    
    fd = os.open(outFilename, os.O_WRONLY | os.O_CREAT | os.O_TRUNC)
    os.write(fd, blob)
    os.close(fd)
