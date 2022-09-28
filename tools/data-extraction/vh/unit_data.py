from vh.config import *
from vh.util import toRomAddr, loadStrings, readImage
from vh.misc_data import MiscData

class UnitData:
    classNames = ["Knight","Armor","Monk","Archer","Airman","Mage","Priest","?: 7","Vandalier"]
    
    ready = False
    travelRanges = None
    typeNames = None
    characterNames = None
    unitInfo = None
    portraitClutIndices = None
    unitPortaitIds = None

    class Unit:
        pass
    
    def __init__(self, rom):
        if not UnitData.ready:
            UnitData.initializeStatics(rom)

    def __getitem__(self, key):
        assert 0 <= key <= 143
        assert len(UnitData.unitInfo) == 144
        return UnitData.unitInfo[key]

    @classmethod
    def initializeStatics(cls, rom):
        cls.travelRanges = cls.loadTravelRanges(rom)
        cls.typeNames = loadStrings(rom, 0x800eb050, 86, 11)
        cls.characterNames = loadStrings(rom, 0x800eaf58, 35, 7)
        cls.unitInfo = cls.loadUnitInfo(rom)
        cls.portraitClutIndices = cls.loadPortraitClutIndices(rom)
        cls.unitPortaitIds = cls.loadUnitPortraitIds(rom)
        if ADJUST_PORTRAIT_IDS:
            cls.adjustPortraitIds(cls.unitPortaitIds)
        cls.ready = True

    @staticmethod
    def loadUnitPortrait(unitId, clut=None):
        portraitId = UnitData.unitPortaitIds[unitId]
        if clut is None:
            clut = MiscData.cluts[UnitData.portraitClutIndices[portraitId]]
        return UnitData.loadPortrait(portraitId, clut)

    @staticmethod
    def loadPortraitClutIndices(f):
        f.seek(toRomAddr(0x800f7974))
        return f.read(700)

    @staticmethod
    def loadUnitPortraitIds(f):
        f.seek(toRomAddr(0x800ea5c8))
        return [int.from_bytes(f.read(2), "little") for i in range(144)]

    @staticmethod
    def loadTravelRanges(f):
        f.seek(toRomAddr(0x800ea5b8))
        return f.read(14)

    @staticmethod
    def loadUnitInfo(f):
        unitInfo = []

        for unitId in range(144):
            f.seek(toRomAddr(0x800ea6e8 + unitId * 15))
            unit = UnitData.Unit()
            unit.nameIdx = ord(f.read(1))
            unit.type = ord(f.read(1))
            unit.advantage = ord(f.read(1))
            unit.cls = ord(f.read(1))
            unit.step = ord(f.read(1))
            unit.mgcSus = ord(f.read(1))
            unit.ailSus = ord(f.read(1))
            unit.atkRange = ord(f.read(1))
            unit.equipment = [ord(f.read(1)) for i in range(4)]
            unit.mp = ord(f.read(1))
            unit.spells = [ord(f.read(1)) for i in range(2)]

            unit.travelRange = UnitData.travelRanges[unit.step]
            unit.name = UnitData.characterNames[unit.nameIdx]
            if len(unit.name) == 0:
                unit.name = f'?: {unit.nameIdx}'
            unit.typeName = UnitData.typeNames[unit.type]
            unit.className = UnitData.classNames[unit.cls]

            unitInfo.append(unit)
        return unitInfo

    @staticmethod
    def loadPortrait(n, clut):
        BYTES_PER_PORTRAIT = 1536 #extra bytes are for blinking/speaking subimages
        PORTRAITS_PER_FILE = 144
        PORTRAIT_FILENAMES = ['F_CLAS01.DAT', 'F_CLAS02.DAT', 'F_CLAS03.DAT', 'F_FACE01.DAT', 'F_FACE02.DAT', 'F_FACE03.DAT']
        
        fileIdx = n // PORTRAITS_PER_FILE
        idxWithinFile = n % PORTRAITS_PER_FILE
        assert(0 <= fileIdx <= 5)
        
        with open(DATA_DIR + PORTRAIT_FILENAMES[fileIdx], "rb") as f:
            f.seek(idxWithinFile * BYTES_PER_PORTRAIT)
            img = readImage(f, clut, 48, 48)
            return img

    @staticmethod 
    def adjustPortraitIds(lst):
        PARTY_CT = 12
        EXPRESSION_CT = 7
        ADV_CT = 5
        for adv in range(0, ADV_CT):
            for partyIdx in range(PARTY_CT):
                if partyIdx == 0 and adv == 2:
                    # no suitable advancement for ash, so skip
                    continue
                idx = 1 + adv * PARTY_CT + partyIdx
                lst[idx] += adv * (PARTY_CT * EXPRESSION_CT)
        
