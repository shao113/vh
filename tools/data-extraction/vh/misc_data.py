from vh.config import *
from vh.util import toRomAddr, loadStrings, readClut

class MiscData:
    ready = False
    spellNames = None
    gfxClutIndices = None
    cluts = None

    def __init__(self, rom):
        if not MiscData.ready:
            MiscData.initializeStatics(rom)

    @classmethod
    def initializeStatics(cls, rom):
        cls.spellNames = loadStrings(rom, 0x800ee410, 72, 21)
        cls.gfxClutIndices = cls.loadGfxClutIds(rom)
        cls.cluts = cls.loadCluts()
        cls.ready = True

    @staticmethod
    def loadCluts():
        cluts = []
        with open(DATA_DIR + "COL_DAT.DAT", "rb") as f:
            for i in range(0, 13 * 16):
                cluts.append(readClut(f))
        return cluts

    @staticmethod
    def loadGfxClutIds(f):
        f.seek(toRomAddr(0x800fde70))
        return [int.from_bytes(f.read(2), "little") for i in range(870)]
