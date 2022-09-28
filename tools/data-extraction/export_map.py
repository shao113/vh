import os.path
from itertools import chain

from vh.config import *
from vh.util import readClut, loadImageRect

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

def loadTextureInfo(battleNum):
    #LAND_DT.DAT (69 * 1797 bytes)
    RECORD_SIZE = 1797 #1028+257+512
    NUM_TEXTURES = 257
    CLUT_IDX_OFFSET = 32
    textures = []
    with open(DATA_DIR + "LAND_DT.DAT", "rb") as f:
        f.seek(battleNum * RECORD_SIZE)
        # texture windows: 1028 bytes
        for i in range(NUM_TEXTURES):
            texture = TextureInfo(f.read(4))
            textures.append(texture)
        # clut ids: 257 bytes
        for i in range(NUM_TEXTURES):
            clutId = ord(f.read(1))
            if clutId != 0: clutId -= CLUT_IDX_OFFSET
            textures[i].clutId = clutId
            assert 0 <= clutId < 16
        # cluts: 512 bytes
        cluts = [readClut(f) for i in range(16)]
    return textures, cluts

def generateMtl(textureFilename, battleNum, mapTiles):
    mtlFilename = f"b{battleNum}.mtl"
    uniqueGfx = set()
    for mapTile in mapTiles:
        uniqueGfx.update(mapTile.faceGfx)
    #print(uniqueGfx)
    assert max(uniqueGfx) < 257 and min(uniqueGfx) >= 0

    textures, cluts = loadTextureInfo(battleNum)
    with open(textureFilename, "rb") as textureFile, open(EXPORT_DIR + mtlFilename, "w") as mtlFile:
        for gfx in uniqueGfx:
            tex = textures[gfx]
            assert tex.w > 0 and tex.h > 0
            img = loadImageRect(textureFile, cluts[tex.clutId], tex.x, tex.y, tex.w, tex.h, stride=128)
            #TBD texture alignment? 1px?
            img = img.crop(box=(0, 0, tex.w-1, tex.h-1))
            imgFilename = f"b{battleNum}_g{gfx}.png"
            print(imgFilename)
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

def exportMap(objFilename, mapFilename, battleNum, textureFilename, xCount=28, zCount=22):
    with open(mapFilename, "rb") as mapFile, open(EXPORT_DIR + objFilename, "w") as objFile:
        tiles = []
        for z in range(zCount):
            for x in range(xCount):
                tile = MapTile(x, z)
                tile.read(mapFile)
                tiles.append(tile)

        mtlFilename = generateMtl(textureFilename, battleNum, tiles)
        #mtlFilename = "b10.mtl" 
        objFile.write(f"mtllib {mtlFilename}\n")
        objFile.write("vt 0.0 1.0 0.0\nvt 1.0 1.0 0.0\nvt 1.0 0.0 0.0\nvt 0.0 0.0 0.0\n\n")

        vertexOffset = 0
        for tile in tiles:
            tile.writeToObj(objFile, vertexOffset, separate=True)
            vertexOffset += len(tile.vertices)


# Normally texture gfx indices would be offset by 400; just zero-indexing "local" cluts/gfxidx for now;
# TBD if anything references any of the global COL_DAT.DAT cluts or previously loaded GFX
# TBD: unpacked development versions? e.g. M_MAP01.DAT; any interesting differences?
# TODO generalize "battleNum" to include event maps?

unpackMap(
    prsFilename=DATA_DIR + "M_MAP02.PRS", 
    outFilename="unpacked-map.tmp")

exportMap(
    objFilename="test3d-11.obj",
    #mapFilename=DATA_DIR + "M_MAP01.DAT", 
    mapFilename="unpacked-map.tmp", 
    battleNum=11,
    textureFilename=DATA_DIR + "F_TEX01.DAT",
    xCount=28,
    zCount=12)
