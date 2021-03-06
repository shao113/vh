#ifndef UNITS_H
#define UNITS_H

#include "common.h"
#include "evt.h"

#define UNIT_CT 40

typedef enum SpellIdx {
   SPELL_NULL = 0,
   SPELL_FAERIE_LIGHT = 1,
   SPELL_ICE_STORM = 2,
   SPELL_ROLLING_FIRE = 3,
   SPELL_FAERIE_STAR = 4,
   SPELL_DELTA_MIRAGE = 5,
   SPELL_DARK_STAR = 6,
   SPELL_SPELLBIND = 7,
   SPELL_PIERCING_RAY = 8,
   SPELL_ENVENOM = 9,
   SPELL_PHASE_SHIFT = 10,
   SPELL_ROMAN_FIRE = 11,
   SPELL_POISON_CLOUD = 12,
   SPELL_SPREAD_FORCE = 13,
   SPELL_AVALANCHE = 14,
   SPELL_SALAMANDER = 15,
   SPELL_HEALING = 16,
   SPELL_MYSTIC_SHIELD = 17,
   SPELL_CURE = 18,
   SPELL_HEALING_PLUS = 19,
   SPELL_BLESS_WEAPON = 20,
   SPELL_HOLY_LIGHTNING = 21,
   SPELL_ULTRA_HEALING = 22,
   SPELL_MAGIC_CHARGE = 23,
   SPELL_HOLY_PRESSURE = 24,
   SPELL_SUPREME_HEALING = 25,
   SPELL_STONE_SHOWER = 26,
   SPELL_CURE_WIDE = 27,
   SPELL_HEALING_CIRCLE = 28,
   SPELL_PERFECT_GUARD = 29,
   SPELL_THUNDER_FLASH = 30,
   SPELL_HEALING_WAVE = 31,
   SPELL_MYSTIC_ENERGY = 32,
   SPELL_SELF_HEALING = 33,
   SPELL_HEALING_2 = 34,
   SPELL_EXTRA_HEALING = 35,
   SPELL_HYPER_HEALING = 36,
   SPELL_HEALING_CIRCLE_2 = 37,
   SPELL_PIERCING_LIGHT = 38,
   SPELL_RAINBOW_STROKE = 39,
   SPELL_MAGIC_ARROW = 40,
   SPELL_THUNDER_BALL = 41,
   SPELL_DARK_HURRICANE = 42,
   SPELL_RAINBOW_STORM = 43,
   SPELL_DAGGER_STORM = 44,
   SPELL_PLASMA_WAVE = 45,
   SPELL_DARK_FIRE = 46,
   SPELL_EXPLOSION = 47,
   SPELL_DYNAMO_HUM = 48,
   SPELL_HERB = 49,
   SPELL_MEGAHERB = 50,
   SPELL_ELIXIR = 51,
   SPELL_MAGE_OIL = 52,
   SPELL_MAGE_GEM = 53,
   SPELL_LIFE_ORB = 54,
   SPELL_HOLY_H2O = 55,
   SPELL_FIRE_GEM = 56,
   SPELL_MOODRING = 57,
   SPELL_AURA_GEM = 58,
   SPELL_WYRMFANG = 59,
   SPELL_ROLLING_THUNDER = 60,
   SPELL_HARMFUL_WAVE = 61,
   SPELL_EVIL_STREAM = 62,
   SPELL_MAD_BOOK = 63,
   SPELL_MUSHROOM = 64,
   SPELL_MOON_PIE = 65,
   SPELL_IRONBOOT = 66,
   SPELL_UNICORN = 67,
   SPELL_KINGFOIL = 68,
   SPELL_HELSTONE = 69,
   SPELL_SHIVBOOK = 70,
   SPELL_NCKLACE = 71
} SpellIdx;

typedef enum ItemIdx {
   ITEM_NULL = 0,
   ITEM_HERB = 1,
   ITEM_MEGAHERB = 2,
   ITEM_MAGE_OIL = 3,
   ITEM_MAGE_GEM = 4,
   ITEM_ELIXIR = 5,
   ITEM_HOLY_H2O = 6,
   ITEM_LIFE_ORB = 7,
   ITEM_FIRE_GEM = 8,
   ITEM_MOODRING = 9,
   ITEM_AURA_GEM = 10,
   ITEM_WYRMFANG = 11,
   ITEM_S_SWORD = 12,
   ITEM_L_SWORD = 13,
   ITEM_I_SWORD = 14,
   ITEM_G_SWORD = 15,
   ITEM_M_SWORD = 16,
   ITEM_CALIBURN = 17,
   ITEM_V_HEART = 18,
   ITEM_H_BOW = 19,
   ITEM_L_BOW = 20,
   ITEM_IRON_BOW = 21,
   ITEM_STL_BOW = 22,
   ITEM_KILL_BOW = 23,
   ITEM_GRT_BOW = 24,
   ITEM_RUNE_BOW = 25,
   ITEM_STAFF = 26,
   ITEM_L_STAFF = 27,
   ITEM_I_STAFF = 28,
   ITEM_M_STAFF = 29,
   ITEM_S_STAFF = 30,
   ITEM_ARKSTAFF = 31,
   ITEM_RUNEWAND = 32,
   ITEM_IRON_AXE = 33,
   ITEM_BATTLEAX = 34,
   ITEM_BLOODAXE = 35,
   ITEM_GRT_AXE = 36,
   ITEM_RAGNAROK = 37,
   ITEM_I_SPEAR = 38,
   ITEM_S_LANCE = 39,
   ITEM_HALBERD = 40,
   ITEM_D_SPEAR = 41,
   ITEM_IRONCLAW = 42,
   ITEM_STL_CLAW = 43,
   ITEM_P_CLAWS = 44,
   ITEM_D_CLAWS = 45,
   ITEM_LTHRHELM = 46,
   ITEM_LGHTHELM = 47,
   ITEM_I_HELM = 48,
   ITEM_STL_MASK = 49,
   ITEM_GRIMHELM = 50,
   ITEM_DRGNHELM = 51,
   ITEM_BANDANNA = 52,
   ITEM_L_BAND = 53,
   ITEM_IRON_CAP = 54,
   ITEM_STL_BAND = 55,
   ITEM_L_MASK = 56,
   ITEM_R_CROWN = 57,
   ITEM_LEATHER = 58,
   ITEM_PLEATHER = 59,
   ITEM_CHAIN = 60,
   ITEM_SCALE = 61,
   ITEM_PLATE = 62,
   ITEM_KEVLAR = 63,
   ITEM_CAPE = 64,
   ITEM_WOOLROBE = 65,
   ITEM_MSTRROBE = 66,
   ITEM_MYSTROBE = 67,
   ITEM_ZEN_ROBE = 68,
   ITEM_MOJOROBE = 69,
   ITEM_NOVA_KEY = 70,
   ITEM_EARTH_K_ = 71,
   ITEM_MANA_KEY = 72,
   ITEM_CHAOS_K_ = 73,
   ITEM_LOGOS_K_ = 74,
   ITEM_HEAVEN_K = 75,
   ITEM_NOVA_P_ = 76,
   ITEM_ICE_P_ = 77,
   ITEM_MANA_P_ = 78,
   ITEM_CHAOS_P_ = 79,
   ITEM_LOGO_P_ = 80,
   ITEM_HEAVEN_P = 81,
   ITEM_V_HELM = 82,
   ITEM_V_ARMOR = 83,
   ITEM_V_HEART_2 = 84,
   ITEM_GOLDCOIN = 85,
   ITEM_MITHRIL = 86,
   ITEM_GOLD_AXE = 87,
   ITEM_MAD_BOOK = 88,
   ITEM_MUSHROOM = 89,
   ITEM_MOON_PIE = 90,
   ITEM_IRONBOOT = 91,
   ITEM_UNICORN = 92,
   ITEM_KINGFOIL = 93,
   ITEM_HELSTONE = 94,
   ITEM_SHIVBOOK = 95,
   ITEM_NCKLACE = 96,
   ITEM_BANANA = 97,
   ITEM_MACROMAN = 98,
   ITEM_TAROT = 99,
   ITEM_RAMEN = 100,
   ITEM_THIEF_SWORD = 101,
   ITEM_THIEF_HELM = 102,
   ITEM_THIEF_MAIL = 103,
   ITEM_IMP_PANTS = 104,
   ITEM_LARGE_SPEAR = 105,
   ITEM_CAPTAIN_SUIT = 106,
   ITEM_RED_BANDANNA = 107,
   ITEM_PIRATE_SUIT = 108,
   ITEM_FARM_TOOL = 109,
   ITEM_CLOTHES = 110,
   ITEM_DARK_HELM = 111,
   ITEM_MAD_ARMOR = 112,
   ITEM_DEATH_MASK = 113,
   ITEM_LORD_ARMOR = 114,
   ITEM_EMPIRE_HELM = 115,
   ITEM_EMPIRE_MAIL = 116,
   ITEM_BOMB = 117,
   ITEM_EMPIRE_HOOD = 118,
   ITEM_EMPIRE_ROBE = 119,
   ITEM_COMMAND_HELM = 120,
   ITEM_COMMANDARMOR = 121,
   ITEM_CRIMSON_BAND = 122,
   ITEM_CRIMSON_MAIL = 123,
   ITEM_GUARD_HELM = 124,
   ITEM_GUARD_MAIL = 125,
   ITEM_VORPAL_CHAIN = 126,
   ITEM_COMMAND_HELM_2 = 127,
   ITEM_CRIMSON_HOOD = 128,
   ITEM_CRIMSON_CAPE = 129,
   ITEM_BLACK_HELM = 130,
   ITEM_BLACK_ARMOR = 131,
   ITEM_R_CROWN_2 = 132,
   ITEM_DOOM_SWORD = 133,
   ITEM_DOOM_CROWN = 134,
   ITEM_DOOM_MAIL = 135,
   ITEM_SAINT_BUSTER = 136,
   ITEM_HOLY_HELM = 137,
   ITEM_VALHALA_MAIL = 138,
} ItemIdx;

typedef enum UnitNameIdx {
   UNIT_ASH = 1,
   UNIT_CLINT = 2,
   UNIT_DIEGO = 3,
   UNIT_ELENI = 4,
   UNIT_HUXLEY = 5,
   UNIT_KIRA = 6,
   UNIT_GROG = 7,
   UNIT_DOLAN = 8,
   UNIT_AMON = 9,
   UNIT_SARA = 10,
   UNIT_ZOHAR = 11,
   UNIT_DARIUS = 12,
   UNIT_ZOOT = 13,
   UNIT_HASSAN = 15,
   UNIT_MAGNUS = 17,
   UNIT_LANDO = 19,
   UNIT_DUMAS = 20,
   UNIT_KANE = 21,
   UNIT_DALLAS = 22,
   UNIT_KURTZ = 23,
   UNIT_SABINA = 24,
   UNIT_DOLF = 29,
   UNIT_LEENA = 30,
   UNIT_CLIVE = 31,
   UNIT_KIRA_2 = 34
} UnitNameIdx;

typedef enum UnitTeam { TEAM_NULL = 0, TEAM_PLAYER = 1, TEAM_ENEMY = 2 } UnitTeam;

typedef enum UnitType {
   UNIT_TYPE_NULL = 0,
   UNIT_TYPE_HERO = 1,
   UNIT_TYPE_CHAMPION = 2,
   UNIT_TYPE_PARAGON = 3,
   UNIT_TYPE_SOLDIER = 4,
   UNIT_TYPE_SWORDSMAN = 5,
   UNIT_TYPE_DUELIST = 6,
   UNIT_TYPE_GUARDSMAN = 7,
   UNIT_TYPE_DRAGOON = 8,
   UNIT_TYPE_ARCHER = 9,
   UNIT_TYPE_BOWMAN = 10,
   UNIT_TYPE_SNIPER = 11,
   UNIT_TYPE_HAWKNIGHT = 12,
   UNIT_TYPE_SKY_LORD = 13,
   UNIT_TYPE_MAGE = 14,
   UNIT_TYPE_SORCERER = 15,
   UNIT_TYPE_ENCHANTER = 16,
   UNIT_TYPE_MONK = 17,
   UNIT_TYPE_NINJA = 18,
   UNIT_TYPE_HEALER = 19,
   UNIT_TYPE_BISHOP = 20,
   UNIT_TYPE_ARCHBISHOP = 21,
   UNIT_TYPE_THIEF_LORD = 22,
   UNIT_TYPE_BRIGAND = 23,
   UNIT_TYPE_HUNTER = 24,
   UNIT_TYPE_HUNTERIMP = 25,
   UNIT_TYPE_WAR_GHOST = 26,
   UNIT_TYPE_CLAY_GOLEM = 27,
   UNIT_TYPE_HELL_BAT = 28,
   UNIT_TYPE_DEATH_ANT = 29,
   UNIT_TYPE_ANT_ARM = 30,
   UNIT_TYPE_MADCAPTAIN = 31,
   UNIT_TYPE_EVILSTATUE = 32,
   UNIT_TYPE_VILLAGER = 33,
   UNIT_TYPE_MADSOLDIER = 34,
   UNIT_TYPE_GUARDDOG = 35,
   UNIT_TYPE_SHOOTERIMP = 36,
   UNIT_TYPE_DARK_GOLEM = 37,
   UNIT_TYPE_BLOODGHOST = 38,
   UNIT_TYPE_DARK_LORD = 39,
   UNIT_TYPE_SPARKIE = 40,
   UNIT_TYPE_CENTURION = 41,
   UNIT_TYPE_GRENADIER = 42,
   UNIT_TYPE_BLOOD_BAT = 43,
   UNIT_TYPE_DEACON = 44,
   UNIT_TYPE_HITMAN = 45,
   UNIT_TYPE_C_ARCHER = 46,
   UNIT_TYPE_JUGGERNAUT = 47,
   UNIT_TYPE_MIMIC = 48,
   UNIT_TYPE_JAILMASTER = 49,
   UNIT_TYPE_ASSASSIN = 50,
   UNIT_TYPE_GREATARMOR = 51,
   UNIT_TYPE_WAR_MASTER = 52,
   UNIT_TYPE_VALKYRIE = 53,
   UNIT_TYPE_C_SHOOTER = 54,
   UNIT_TYPE_C_KNIGHT = 55,
   UNIT_TYPE_C_HAWK = 56,
   UNIT_TYPE_BUGABOO = 57,
   UNIT_TYPE_WIGHT = 58,
   UNIT_TYPE_SKELETON = 59,
   UNIT_TYPE_EGGWORM = 60,
   UNIT_TYPE_GOLD_GOLEM = 61,
   UNIT_TYPE_ACID_GHOST = 62,
   UNIT_TYPE_DEATHANGEL = 63,
   UNIT_TYPE_C_WARLOCK = 64,
   UNIT_TYPE_C_PRIEST = 65,
   UNIT_TYPE_SALAMANDER = 66,
   UNIT_TYPE_GUARDIAN = 67,
   UNIT_TYPE_BASILISK = 68,
   UNIT_TYPE_BLK_KNIGHT = 69,
   UNIT_TYPE_XENO = 70,
   UNIT_TYPE_HELLKNIGHT = 71,
   UNIT_TYPE_BAHAMUT = 72,
   UNIT_TYPE_DARK_MAGE = 73,
   UNIT_TYPE_DOOM_LORD = 74,
   UNIT_TYPE_DARK_ANGEL = 75,
   UNIT_TYPE_MAGE_TOWER = 76,
   UNIT_TYPE_DEATH_DEV_ = 77,
   UNIT_TYPE_VANDALIER = 78,
   UNIT_TYPE_NPC = 79,
   UNIT_TYPE_BUCCANEER = 80,
   UNIT_TYPE_CORSAIR = 81,
   UNIT_TYPE_WARLOCK = 82,
   UNIT_TYPE_C_ARMOR = 83,
   UNIT_TYPE_MEGA_GUARD = 84,
   UNIT_TYPE_M_CANNON = 85
} UnitType;

typedef enum UnitClass {
   CLASS_KNIGHT = 0,
   CLASS_ARMOR = 1,
   CLASS_MONK = 2,
   CLASS_ARCHER = 3,
   CLASS_AIRMAN = 4,
   CLASS_MAGE = 5,
   CLASS_PRIEST = 6,
   CLASS_7 = 7,
   CLASS_VANDALIER = 8
} UnitClass;

typedef struct UnitStatus {
   u8 idx;
   s8 level;
   s8 team;
   u8 field3_0x3;
   EvtData *evtBattler;
   EvtData *evtSprite;
   BigInt expLevel;
   u8 field7_0x1c;
   u8 field8_0x1d;
   u8 expMulti;
   u8 field10_0x1f;
   s16 hpFrac;
   u8 field12_0x22;
   u8 field13_0x23;
   u32 atkVar10000;
   u32 defVar10000;
   u32 agiVar10000;
   s16 hp;
   s16 maxHp;
   s16 attack;
   s16 defense;
   s16 agility;
   s16 exp;
   s16 mp;
   s16 maxMp;
   s16 hpVar100;
   s16 atkVar100;
   s16 defVar100;
   s16 agiVar100;
   s16 direction;
   u8 stripIdx;
   u8 spells[10];
   u8 done;
   u8 field33_0x56;
   u8 field34_0x57;
   u16 item1;
   u16 item2;
   u16 droppedItem;
   u16 helmet;
   u16 armor;
   u16 weapon;
   s16 unitId; // ?: 0..143
   u8 travelRange;
   u8 attackRange;
   u8 name;
   u8 unitType;  // See: {@sym 800eb050}
   u8 advantage; // See: {@sym 800f16e4}
   u8 class;
   u8 step;
   u8 magicSusceptibility;
   u8 ailmentSusceptibility;
   u8 hideMarker;
   u8 poisoned;
   u8 paralyzed;
   u8 atkBoosted;
   u8 defBoosted;
   u8 aglBoosted;
   u8 tileX;
   u8 tileZ;
   u8 field59_0x77;
} UnitStatus;

extern s8 gCharacterNames[35][7];
extern s8 gUnitTypeNames[86][11];
extern s8 gItemNames[139][13];
extern UnitStatus gUnits[UNIT_CT];

#endif
