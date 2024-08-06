from vh.config import *
from vh.util import toRomAddr, loadImageRect, loadStrings, readCString
from vh.misc_data import MiscData

class ItemData:
    typeNames = ["SWORD","BOW","STAFF","AXE","SPEAR","CLAWS","V_HEART","HELM","BAND","V_HELM","ARMOR","ROBE","V_ARMOR","ITEM"]
    
    ready = False
    names = None
    costs = None
    descriptions = None
    power = None
    displayPower = None
    spells = None
    types = None

    class Item:
        pass

    def __init__(self, rom):
        if not ItemData.ready:
            ItemData.initializeStatics(rom)

    def __getitem__(self, key):
        assert 0 <= key <= 100
        item = ItemData.Item()
        item.type = ItemData.types[key]
        item.cost = ItemData.costs[key]
        item.power = ItemData.power[key]
        item.displayPower = ItemData.displayPower[key]
        item.spell = ItemData.spells[key]
        item.description = ItemData.descriptions[key]
        return item

    @classmethod
    def initializeStatics(cls, rom):
        #itemNames = loadStrings(rom, 0x800eed20, 101, 17, "shift_jis")
        cls.names = loadStrings(rom, 0x800eb404, 139, 13)
        cls.costs = cls.loadItemCosts(rom)
        cls.descriptions = cls.loadItemDescriptions(rom)
        cls.power = cls.loadItemPower(rom)
        cls.displayPower = cls.loadItemDisplayPower(rom)
        cls.spells = cls.loadItemSpells(rom)
        cls.types = cls.loadItemTypes(rom)
        cls.ready = True

    @staticmethod
    def loadIcon(n, clut=None):
        if clut is None:
            ICONS_STARTING_GFX_IDX = 0x2d7
            clut = MiscData.cluts[MiscData.gfxClutIndices[ICONS_STARTING_GFX_IDX+n]]
        with open(DATA_DIR + "F_ITEM.DAT", "rb") as f:
            img = loadImageRect(f, clut, (n - 1) % 16 * 16, (n - 1) // 16 * 16, 16, 16, stride=128)
            return img

    @staticmethod
    def loadItemCosts(f):
        f.seek(toRomAddr(0x800eeb18))
        return [int.from_bytes(f.read(2), "little") for i in range(101)]

    @staticmethod
    def loadItemDescriptions(f):
        lst = []
        f.seek(toRomAddr(0x800ef3d8))
        addresses = [int.from_bytes(f.read(4), "little") for i in range(101)]
        for address in addresses:
            f.seek(toRomAddr(address))
            lst.append(readCString(f))
        return lst

    @staticmethod
    def loadItemPower(f):
        f.seek(toRomAddr(0x800eec4c))
        return [ord(f.read(1)) for i in range(101)]

    @staticmethod
    def loadItemDisplayPower(f):
        # ?: as shown to player
        f.seek(toRomAddr(0x800eebe4))
        return [ord(f.read(1)) for i in range(101)]

    @staticmethod
    def loadItemSpells(f):
        f.seek(toRomAddr(0x800eecb4))
        return [ord(f.read(1)) for i in range(101)]

    @staticmethod
    def loadItemTypes(f):
        f.seek(toRomAddr(0x800ef780))
        return [ItemData.typeNames[ord(f.read(1))] for i in range(101)]

