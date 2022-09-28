from operator import attrgetter
from io import BytesIO
from os.path import exists
from PIL import Image, ImagePalette, ImageDraw

from vh.config import *
from vh.util import toRomAddr, readImage, loadImageRect
from vh.misc_data import MiscData

class SpriteLoader:
    ready = False

    rects = None
    animSetAddrs = None
    uniqAnimSetAddrs = None
    unitClutIndices = None
    unitAnimCounts = {}
    
    def __init__(self, rom):
        if not SpriteLoader.ready:
            SpriteLoader.initializeStatics(rom)
         
        self.rom = rom
        self.unitId = -1
        self.strip = None
        self.spritesheet = None
        self.frames = {}
        self.unitAnimAddrs = []
        
    def load(self, unitId):
        self.frames.clear()
        self.unitId = unitId
        self.unitAnimAddrs = SpriteLoader.loadUnitAnimAddrs(self.rom, unitId)
        SpriteLoader.unitAnimCounts[unitId] = len(self.unitAnimAddrs)

        clut = MiscData.cluts[SpriteLoader.unitClutIndices[unitId]]
        self.strip, self.spritesheet = SpriteLoader.loadUnitSprites(unitId, clut)
        
    def getCachedFrame(self, num):
        if num in self.frames:
            return self.frames[num]
        if (1 <= num <= 34) or (45 <= num <= 64):
            img = self.spritesheet.crop(SpriteLoader.rects[num])
            self.frames[num] = img
            return img
        elif 35 <= num <= 44:
            img = self.strip.crop(SpriteLoader.rects[num])
            self.frames[num] = img
            return img
        else:
            print(f"Unknown frame type: {num}")
            assert False

    def getFrame(self, num):
        return self.getCachedFrame(num).copy()
        
    def exportUnitAnimation(self, n, *, b64=False, filename=None):
        # TODO correct frame logic; positioning? special bytes eg 0x3c
        MAX_LEN = 100 #arbitrary
        durations = []
        frames = []
        positionings = []
        positioning = None
        loop = False
        animAddr = self.unitAnimAddrs[n]
        if animAddr == 0:
            print(f"null anim: {n}")
            return
        self.rom.seek(toRomAddr(animAddr))
        
        # Frame logic not accurate, but should work to start with:    
        for i in range(MAX_LEN):
            assert i < 50
            b1, b2 = ord(self.rom.read(1)), ord(self.rom.read(1)) 
            if b1 == 0:
                loop = (b2 == 1)
                break
            elif b1 == 0x3c or b1 == 0x3b:
                xzoffset = int.from_bytes(b2.to_bytes(1, "little"), "little", signed=True)
                yoffset = int.from_bytes(self.rom.read(1), "little", signed=True)
                positioning = (xzoffset, yoffset)
                continue
            else:
                frames.append(b1)
                durations.append(b2)
                positionings.append(positioning)
                positioning = None
        #print(list(zip(frames,durations,positionings)), loop)  
        frames = [self.getCachedFrame(x) for x in frames]
        maxWidth = max(frames, key=attrgetter('width')).width
        paddedFrames = []

        for i, frame in enumerate(frames):
            if frame.width < maxWidth:
                # I must be using palettes wrong, since certain functions mess up the colors;
                # But using getdata() and raw() seems to avoid the issue.
                mode, data = frame.palette.getdata()
                paddedFrame = Image.new("P", (maxWidth, frame.height))
                paddedFrame.paste(frame)
                paddedFrame.palette = ImagePalette.raw("RGB", data)
                paddedFrame.info['transparency'] = frame.info['transparency']
                paddedFrames.append(paddedFrame)
            else:
                paddedFrames.append(frame)

            if VISUALIZE_ANIM_POS and positionings[i] is not None:
                paddedFrames[i] = paddedFrames[i].copy()
                dc = ImageDraw.Draw(paddedFrames[i])
                p = positionings[i][0]
                dc.line((24,1, 24+p,1), fill=15, width=3)
                dc.line((24,1, 24+p,1), fill=13, width=1)
                p = positionings[i][1]
                dc.line((1,24, 1,24+p), fill=15, width=3)
                dc.line((1,24, 1,24+p), fill=13, width=1)

        firstFrame = paddedFrames.pop(0)
        durations = [round(x / 60 * 1000) for x in durations]
        #loop = 0 if loop else 1

        # For easier viewing, slow down and always loop
        durations = [x * 3 for x in durations]
        if not loop:
            durations[-1] = 1000
        loop = 0

        assert not b64
        #    with BytesIO() as mem:
        #        firstFrame.save(mem, "WEBP", save_all=True, append_images=paddedFrames, duration=durations, loop=loop, lossless=True, method=4)
        #        return str(b64encode(mem.getvalue()), "ascii")  

        firstFrame.save(filename, save_all=True, append_images=paddedFrames, duration=durations, loop=loop, lossless=True, method=4)
           
    @staticmethod
    def loadUnitSprites(n, clut):
        # Each UNIT*.DAT file will generally contain six compressed 256x352 sprite sheets, followed
        # by another six uncompressed 512x48 sprite strips 
        # TODO: UNIT_16.DAT contains only 5 sub-units
        # TODO: ENEMY_00.DAT?
        assert 1 <= n <= 870
        UNITS_PER_FILE = 6
        BYTES_PER_STRIP = 0x3000
        SIZE_OF_DECOMPRESSED_DATA = 0xb000
        SPRITESTRIP_W, SPRITESTRIP_H = 512, 48
        SPRITESHEET_W, SPRITESHEET_H = 256, 352
            
        def writeByte(b):
            nonlocal dataOfs, cacheOfs
            cache[cacheOfs] = b
            cacheOfs = (cacheOfs + 1) & 0x3ff
            data[dataOfs] = b
            dataOfs += 1
        
        if n == 137 or n == 138:
            print("TODO")
            n -= 1

        n -= 1
        fileIdx = n // UNITS_PER_FILE
        subUnitIdx = n % UNITS_PER_FILE

        filename = f"{DATA_DIR}UNIT_{fileIdx:02X}.DAT"
        assert exists(filename)
        #print(fileIdx,subUnitIdx,filename)
        
        with open(filename, "rb") as f:
            assert f.read(2) == b"KG" # maybe kyara graphics?
            subUnitCt = int.from_bytes(f.read(2), "little")
            offset = 4 + subUnitCt * 4
            offsets = []
            for i in range(subUnitCt):
                # Calculate offsets of the compressed spritesheets
                offsets.append(offset)
                offset += int.from_bytes(f.read(4), "little")
            strip = loadImageRect(f, clut, 0, 0, SPRITESTRIP_W, SPRITESTRIP_H, stride=SPRITESTRIP_W//2, baseOfs=offset+subUnitIdx*BYTES_PER_STRIP)
            f.seek(offsets[subUnitIdx])
            f.read(4) # currently unused; size of current sub-unit data?
            data = bytearray(b"\xaa" * (SIZE_OF_DECOMPRESSED_DATA + 64))
            dataOfs = 0
            cache = bytearray(0x400)
            cacheOfs = 0x3ff - 64 - 1
            remainingBytes = SIZE_OF_DECOMPRESSED_DATA
            mode = 0
            
            while remainingBytes > 0:
                if mode & 0x100 == 0:
                    # Ran out of sentinel bits; read a byte from input and OR a sentinel hi-byte onto it
                    mode = ord(f.read(1)) | 0xff00
                    
                if mode & 1 == 0:
                    # Mode lo-bit is clear; next two bytes decode to cache idx and length
                    b1 = ord(f.read(1))
                    b2 = ord(f.read(1))
                    
                    # b1: cccc_cccc; b2: CCnn_nnnn;
                    # CCcccccccc: 10-bit cache index
                    # nnnnnn + 3: num bytes to read from cache (3..66)
                    
                    cacheIdx = b1 | ((b2 & 0b1100_0000) << 2)
                    cacheBytesToRead = (b2 & 0b0011_1111) + 3
                    
                    for i in range(cacheBytesToRead):
                        writeByte(cache[(cacheIdx + i) & 0x3ff])
                            
                    remainingBytes -= cacheBytesToRead
                else:
                    # Mode lo-bit is set; read and cache one byte from input
                    writeByte(ord(f.read(1)))
                    remainingBytes -= 1    
                    
                # Next mode bit
                mode >>= 1
                    
            with BytesIO(data) as tmp:
                spritesheet = readImage(tmp, clut, SPRITESHEET_W, SPRITESHEET_H)
            
            return strip, spritesheet
    
    @classmethod
    def initializeStatics(cls, rom):
        assert not cls.ready
        cls.rects = cls.createSpriteRects()
        cls.animSetAddrs = cls.loadUnitAnimSetAddrs(rom)
        cls.uniqAnimSetAddrs = set(cls.animSetAddrs)
        cls.unitClutIndices = cls.loadUnitClutIndices(rom)
        cls.unitAnimCounts = {}
        cls.ready = True
        
    @staticmethod
    def loadUnitClutIndices(f):
        f.seek(toRomAddr(0x800ea3cc))
        return f.read(492)
    
    @staticmethod
    def loadUnitAnimSetAddrs(f):
        f.seek(toRomAddr(0x800ea18c))
        addresses = [int.from_bytes(f.read(4), "little") for i in range(144)]
        return addresses

    @staticmethod
    def loadUnitAnimAddrs(f, unitId):
        assert 0 < unitId < 144
        END_ADDR = 0x800ea18c
        MAX_LEN = 100 #arbitrary
        addresses = []
        currentAddress = SpriteLoader.animSetAddrs[unitId]
        f.seek(toRomAddr(currentAddress))
        for i in range(MAX_LEN):
            addresses.append(int.from_bytes(f.read(4), "little"))
            currentAddress += 4
            if (currentAddress in SpriteLoader.uniqAnimSetAddrs) or (currentAddress >= END_ADDR):
                #reached another set's start address, or exceeded designated region
                break
        return addresses

    @staticmethod
    def getUnitAnimationCount(unitId):
        return SpriteLoader.unitAnimCounts[unitId]
        
    @staticmethod
    def createSpriteRects():
        SPRITE_W, SPRITE_H = 48, 48
        WIDE_SPRITE_W, WIDE_SPRITE_H = 64, 48
        XWIDE_SPRITE_W, XWIDE_SPRITE_H = 96, 48
        
        def batch(x, y, w, h, first, last, perRow, lst):
            for i in range(first, last+1):
                ix = x + ((i - first) % perRow) * w
                iy = y + ((i - first) // perRow) * h
                lst[i] = (ix, iy, ix + w, iy + h)
            
        rects = [(0,0,0,0)] * 65
        #spritesheet sprites
        batch(0, 0, SPRITE_W, SPRITE_H, first=1, last=20, perRow=4, lst=rects)
        batch(192, 0, WIDE_SPRITE_W, WIDE_SPRITE_H, first=21, last=25, perRow=1, lst=rects)
        batch(192, 256, WIDE_SPRITE_W, WIDE_SPRITE_H, first=26, last=26, perRow=1, lst=rects)
        batch(0, 304, WIDE_SPRITE_W, WIDE_SPRITE_H, first=27, last=30, perRow=4, lst=rects)
        batch(0, 256, SPRITE_W, SPRITE_H, first=31, last=34, perRow=4, lst=rects)
        #sprite strip:
        batch(0, 0, SPRITE_W, SPRITE_H, first=35, last=39, perRow=5, lst=rects)
        batch(256, 0, SPRITE_W, SPRITE_H, first=40, last=44, perRow=5, lst=rects)
        #double-width (spritesheet)
        batch(0, 0, XWIDE_SPRITE_W, XWIDE_SPRITE_H, first=45, last=54, perRow=2, lst=rects)
        batch(0, 256,  XWIDE_SPRITE_W, XWIDE_SPRITE_H, first=55, last=64, perRow=2, lst=rects)
       
        return rects