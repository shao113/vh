import os.path
from itertools import chain

from vh.config import *
from vh.util import readClut, loadImageRect
from vh.misc_data import MiscData

EXPORT_DIR = "exported-maps/"

class TextureInfo:
    def __init__(self, data):
        self.x = data[0]
        self.y = data[1]
        self.w = data[2]
        self.h = data[3]
        if self.w != 0: self.w += 1
        if self.h != 0: self.h += 1
        self.clutId = -1

    #def __repr__(self):
    #	return f"<x:{self.x}, y:{self.y}, w:{self.w}, h:{self.h}, c:{self.clutId}>"

class MapTile:
    def __init__(self, x, z):
        self.x = x
        self.z = z
        self.vertices = []
        self.faceGfx = []
        self.faces = []
        self.shade = []
        self.faceCt = 0
        self.height = 0

    def writeToObj(self, f, vertexOffset=0, separate=True):
        if self.faceCt == 0:
            #print(f"{self.x},{self.z}: zero faces")
            return
        f.write(f"{'o' if separate else 'g'} tile_{self.x:0>2}_{self.z:0>2}\n\n")
        for vtx in self.vertices:
            f.write(f"v {vtx[0]} {vtx[1]} {vtx[2]}\n")
        f.write("\n")
        gfx = -1
        for i, face in enumerate(self.faces):
            if self.faceGfx[i] != gfx:
                gfx = self.faceGfx[i]
                f.write(f"usemtl g{gfx}\n")
            if face[2] == face[3]:
                # Triangle
                f.write(f"f {face[0]+vertexOffset+1}/1 {face[1]+vertexOffset+1}/2 {face[2]+vertexOffset+1}/4\n")
            else:
                # Quad
                f.write(f"f {face[0]+vertexOffset+1}/1 {face[1]+vertexOffset+1}/2 {face[2]+vertexOffset+1}/3\n")
                f.write(f"f {face[0]+vertexOffset+1}/1 {face[2]+vertexOffset+1}/3 {face[3]+vertexOffset+1}/4\n")
        f.write("\n")

    def read(self, f):
        MAX_FACES = 18
        MAX_VERTS = 22
        self.vertices = [createVertex(readSVECTOR(f)) for i in range(MAX_VERTS)]
        self.faceGfx = [int.from_bytes(f.read(2), "little") for i in range(MAX_FACES)]
        self.faces = [readFace(f) for i in range(MAX_FACES)]
        # TBD
        self.shade = [ord(f.read(1)) for i in range(MAX_FACES)]
        self.faceCt = ord(f.read(1))
        self.height = ord(f.read(1))

        # Discard extraneous entries
        if self.faceCt == 0:
            self.faces.clear()
            self.faceGfx.clear()
            self.vertices.clear()
        else:
            self.faces = self.faces[:self.faceCt]
            self.faceGfx = self.faceGfx[:self.faceCt]
            maxVertexIdx = max(chain.from_iterable(self.faces))
            self.vertices = self.vertices[:maxVertexIdx+1]

def readSVECTOR(f):
    return [int.from_bytes(f.read(2), "little", signed=True) for i in range(4)]
    
def createVertex(v):
    # Flip on y and z axes, and scale down for easier viewing in Blender
    return [v[0] / 100, -(v[1] / 100), -(v[2] / 100)]
    
def readFace(f):
    return [ord(f.read(1)) for i in range(4)]

def loadTextureInfo(mapNum):
    #LAND_DT.DAT (69 * 1797 bytes)
    RECORD_SIZE = 1797 #1028+257+512
    NUM_TEXTURES = 257
    textures = []
    with open(DATA_DIR + "LAND_DT.DAT", "rb") as f:
        f.seek(mapNum * RECORD_SIZE)
        # texture windows: 1028 bytes
        for i in range(NUM_TEXTURES):
            texture = TextureInfo(f.read(4))
            textures.append(texture)
        # clut ids: 257 bytes
        for i in range(NUM_TEXTURES):
            clutId = ord(f.read(1))
            textures[i].clutId = clutId
        # cluts: 512 bytes
        # a few maps (e.g. 56-58) do access the global cluts
        cluts = globalCluts.copy()
        cluts[32:48] = [readClut(f) for i in range(16)]
    return textures, cluts

def generateMtl(textureFilename, mapNum, mapTiles):
    mtlFilename = f"m{mapNum:0>2}.mtl"
    uniqueGfx = set()
    for mapTile in mapTiles:
        uniqueGfx.update(mapTile.faceGfx)
    #print(uniqueGfx)
    assert max(uniqueGfx) < 257 and min(uniqueGfx) >= 0

    textures, cluts = loadTextureInfo(mapNum)
    with open(textureFilename, "rb") as textureFile, open(EXPORT_DIR + mtlFilename, "w") as mtlFile:
        for gfx in uniqueGfx:
            tex = textures[gfx]
            assert tex.w > 0 and tex.h > 0
            img = loadImageRect(textureFile, cluts[tex.clutId], tex.x, tex.y, tex.w, tex.h, stride=128)
            #TBD texture alignment? 1px?
            img = img.crop(box=(0, 0, tex.w-1, tex.h-1))
            imgFilename = f"m{mapNum:0>2}_g{gfx}.png"
            #print(imgFilename)
            img.save(EXPORT_DIR + imgFilename)
            mtlFile.write(f"newmtl g{gfx}\n")
            mtlFile.write(f"map_Kd {imgFilename}\n\n")
    return mtlFilename

def unpackMap(prsFilename, outFilename):

    maxwrites = 1024 * 1024
    def writeByte(b):
        nonlocal cacheOfs, maxwrites
        cache[cacheOfs] = b
        outFile.write(cache[cacheOfs:cacheOfs+1])
        cacheOfs = (cacheOfs + 1) & 0x3ff
        maxwrites -= 1
        if maxwrites < 1:
            print("too many writes, bailing out!")
            exit()

    def readByte():
        nonlocal remainingBytes
        remainingBytes -= 1
        return ord(prsFile.read(1)) 

    def getCached(idx):
        return cache[idx & 0x3ff]

    with open(prsFilename, "rb") as prsFile, open(outFilename, "wb") as outFile:
        cache = bytearray(0x400)
        cacheOfs = 0x3ff - 32 - 1
        mode = 0
        # Size of compressed input data
        remainingBytes = int.from_bytes(prsFile.read(4), "little")
        assert remainingBytes == os.path.getsize(prsFilename) - 4

        while remainingBytes > 0:
            if mode & 0x100 == 0:
                # Ran out of sentinel bits; read a byte from input and OR a sentinel hi-byte onto it
                mode = readByte() | 0xff00

            if mode & 1 == 0:
                # Mode lo-bit is clear; read and cache one byte from input
                writeByte(readByte())
            else:
                # Mode lo-bit is set; the interpretation of the next byte depends on its hi-bits
                b1 = readByte()

                if b1 & 0b1000_0000 == 0:
                    # Hi-bit is clear; read from cache
                    b2 = readByte()

                    # b1: 0nnn_nnCC; b2: cccc_cccc
                    # CCcccccccc: 10-bit cache index
                    # nnnnn + 3: num bytes to read from cache (3..34)
                    cacheIdx =         ((b1 & 0b0000_0011) << 8) | b2
                    cacheBytesToRead = ((b1 & 0b0111_1100) >> 2) + 3

                    for i in range(cacheBytesToRead):
                        writeByte(getCached(cacheIdx + i))

                elif b1 & 0b0100_0000 == 0:
                    # Second-highest bit is clear; read from cache (relative)
                    # b1: 10nn_cccc
                    cacheIdx =          (b1 & 0b0000_1111)
                    cacheBytesToRead = ((b1 & 0b0011_0000) >> 4) + 2

                    for i in range(cacheBytesToRead):
                        writeByte(getCached(cacheOfs - cacheIdx))
                else:
                    # Both high bits set; lower six bits determine how many bytes to read from input
                    # b1: 11nn_nnnn
                    bytesToRead = (b1 & 0b0011_1111) + 8

                    for i in range(bytesToRead):
                        writeByte(readByte())
            # Next mode bit
            mode >>= 1	     

def exportMap(objFilename, mapFilename, mapNum, textureFilename, xCount=28, zCount=22, guessSize=False):
    if guessSize:
        xCount, zCount = measureMap(mapFilename)
        print(f"guessed size: {xCount}, {zCount}")
    with open(mapFilename, "rb") as mapFile, open(EXPORT_DIR + objFilename, "w") as objFile:
        tiles = []
        for z in range(zCount):
            for x in range(xCount):
                tile = MapTile(x, z)
                tile.read(mapFile)
                tiles.append(tile)

        mtlFilename = generateMtl(textureFilename, mapNum, tiles)
        objFile.write(f"mtllib {mtlFilename}\n")
        objFile.write("vt 0.0 1.0 0.0\nvt 1.0 1.0 0.0\nvt 1.0 0.0 0.0\nvt 0.0 0.0 0.0\n\n")

        vertexOffset = 0
        for tile in tiles:
            tile.writeToObj(objFile, vertexOffset, separate=True)
            vertexOffset += len(tile.vertices)

def measureMap(mapFilename):
    RECORD_SIZE = 304
    def getx(v): return v[0]
    with open(mapFilename, "rb") as mapFile:
        filesize = os.path.getsize(mapFilename)
        assert filesize % RECORD_SIZE == 0
        tileCount = filesize // RECORD_SIZE
        prevCenter = -999
        for i in range(tileCount):
            tile = MapTile(0, 0)
            tile.read(mapFile)
            if tile.faceCt != 0:
                centerx = (max(tile.vertices, key=getx)[0] + min(tile.vertices, key=getx)[0]) / 2
                if centerx < prevCenter:
                    # (presumably) hit the beginning of the second row
                    rowSize = i
                    break
                prevCenter = centerx
        if tileCount % rowSize == 0:
            return rowSize, tileCount // rowSize
        else:
            # fails if second row starts with blanks (e.g. map 45)
            print("failed to guess size, falling back to default")
            return 28, 22

def batchExport(first=0, last=68):
    presets = [
        ("M_KEY01.PRS", "F_TEX14.DAT"), #0
        ("M_KEY02.PRS", "F_TEX15.DAT"), #1
        ("M_KEY03.PRS", "F_TEX13.DAT"), #2
        ("M_KEY04.PRS", "F_TEX02.DAT"), #3
        ("M_KEY05.PRS", "F_TEX05.DAT"), #4
        ("M_KEY06.PRS", "F_TEX02.DAT"), #5
        None,                           #6
        None,                           #7
        ("M_SHOW.PRS", "F_TEX02.DAT"),  #8
        ("IVENT.PRS", "F_TEX01.DAT"),   #9
        ("M_MAP01.PRS", "F_TEX01.DAT"), #10
        ("M_MAP02.PRS", "F_TEX01.DAT"), #11
        ("M_MAP03.PRS", "F_TEX02.DAT"), #12
        ("M_MAP04.PRS", "F_TEX01.DAT"), #13
        ("M_MAP05.PRS", "F_TEX03.DAT"), #14
        ("M_MAP06.PRS", "F_TEX04.DAT"), #15
        ("M_MAP07.PRS", "F_TEX02.DAT"), #16
        ("M_MAP08.PRS", "F_TEX02.DAT"), #17
        ("M_MAP09.PRS", "F_TEX05.DAT"), #18
        ("M_MAP10.PRS", "F_TEX05.DAT"), #19
        ("M_MAP11.PRS", "F_TEX06.DAT"), #20
        ("M_MAP12.PRS", "F_TEX02.DAT"), #21
        ("M_MAP13.PRS", "F_TEX07.DAT"), #22
        ("M_MAP14.PRS", "F_TEX07.DAT"), #23
        ("M_MAP15.PRS", "F_TEX02.DAT"), #24
        ("M_MAP16.PRS", "F_TEX05.DAT"), #25
        ("M_MAP17.PRS", "F_TEX02.DAT"), #26
        ("M_MAP18.PRS", "F_TEX08.DAT"), #27
        ("M_MAP19.PRS", "F_TEX09.DAT"), #28
        ("M_MAP20.PRS", "F_TEX10.DAT"), #29
        ("M_MAP21.PRS", "F_TEX09.DAT"), #30
        ("M_MAP22.PRS", "F_TEX08.DAT"), #31
        ("M_MAP23.PRS", "F_TEX11.DAT"), #32
        ("M_MAP24.PRS", "F_TEX12.DAT"), #33
        ("M_MAP25.PRS", "F_TEX04.DAT"), #34
        ("M_MAP26.PRS", "F_TEX13.DAT"), #35
        ("M_MAP27.PRS", "F_TEX14.DAT"), #36
        ("M_MAP28.PRS", "F_TEX15.DAT"), #37
        ("M_MAP29.PRS", "F_TEX16.DAT"), #38
        ("M_MAP30.PRS", "F_TEX17.DAT"), #39
        ("M_MAP31.PRS", "F_TEX05.DAT"), #40
        ("M_MAP32.PRS", "F_TEX01.DAT"), #41
        ("M_MAP33.PRS", "F_TEX18.DAT"), #42
        ("M_MAP34.PRS", "F_TEX15.DAT"), #43
        ("M_IVE01.PRS", "F_TEX06.DAT"), #44
        ("M_IVE02.PRS", "F_IVE01.DAT"), #45
        ("M_IVE03.PRS", "F_TEX08.DAT"), #46
        ("M_IVE04.PRS", "F_IVE02.DAT"), #47
        ("M_IVE05.PRS", "F_IVE02.DAT"), #48
        ("M_IVE06.PRS", "F_TEX08.DAT"), #49
        ("M_IVE07.PRS", "F_IVE02.DAT"), #50
        ("M_IVE08.PRS", "F_IVE02.DAT"), #51
        ("M_IVE09.PRS", "F_IVE03.DAT"), #52
        ("M_IVE10.PRS", "F_IVE03.DAT"), #53
        ("M_IVE11.PRS", "F_TEX08.DAT"), #54
        ("M_IVE12.PRS", "F_IVE03.DAT"), #55
        ("M_IVE13.PRS", "F_IVE04.DAT"), #56
        ("M_IVE14.PRS", "F_IVE04.DAT"), #57
        ("M_IVE15.PRS", "F_IVE04.DAT"), #58
        ("M_IVE16.PRS", "F_IVE05.DAT"), #59
        ("M_IVE17.PRS", "F_IVE05.DAT"), #60
        ("M_IVE18.PRS", "F_TEX06.DAT"), #61
        ("M_IVE19.PRS", "F_IVE04.DAT"), #62
        ("M_IVE20.PRS", "F_IVE06.DAT"), #63
        ("M_IVE21.PRS", "F_TEX01.DAT"), #64
        ("M_MAP04.PRS", "F_TEX01.DAT"), #65
        ("M_MAP09.PRS", "F_TEX05.DAT"), #66
        ("M_MAP12.PRS", "F_TEX05.DAT"), #67
        ("M_MAP24.PRS", "F_TEX12.DAT")  #68
    ]

    for i in range(first, last+1):
        print(f"map {i}")
        if presets[i] is None:
            print("skipping")
            continue
        
        prsFilename, textureFilename = presets[i]
        objFilename = f"m{i:0>2}_{prsFilename}.obj"
        tmpFilename = "unpacked-map.tmp"
        
        unpackMap(
            prsFilename=DATA_DIR + prsFilename, 
            outFilename=tmpFilename)

        exportMap(
            objFilename=objFilename, 
            mapFilename=tmpFilename, 
            mapNum=i,
            textureFilename=DATA_DIR + textureFilename,
            guessSize=True)

        print(f"exported: {objFilename}")


# TBD if anything references any of the previously loaded GFX
# TBD: unpacked development versions? e.g. M_MAP01.DAT; any interesting differences?
# TODO: polygon winding?

# a few maps (e.g. 56-58) do access the global cluts
globalCluts = MiscData.loadCluts()

batchExport()
