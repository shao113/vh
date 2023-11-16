#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "graphics.h"

// TODO: I've been procrastinating on this, but I really need to re-evaluate the "evt" naming, since
// they drive the entire game; substituting evt->obj seems reasonable and should be mostly painless;
// Also need to start cleaning up a bunch of other naming inconsistencies.

#define EVT_DATA_CT 350
#define EVT_DATA_LAST_IDX 349

#define EVT_HELPER_2(x) evt->d.evtf##x
#define EVT_HELPER_1(x) EVT_HELPER_2(x)
#define EVT EVT_HELPER_1(EVTF)
#define EVTF 000

typedef enum EvtFunctionIdx {
   EVTF_NULL = 0,
   EVTF_NOOP = 1, // Used for sprites, data stores, ...
   EVTF_MENU_CHOICE = 2,
   EVTF_BATTLE_ACTIONS = 3,
   EVTF_WINDOW_TBD_004 = 4,
   EVTF_WINDOW_TBD_005 = 5,
   EVTF_LOGO = 6,
   EVTF_APPLY_POISON = 7,
   EVTF_BATTLE_PORTRAIT = 8,
   EVTF_BATTLE_MGR = 13,
   EVTF_BATTLE_UNIT = 14,
   EVTF_TARGETING_ATTACK = 15,
   EVTF_CHOOSE_DONE_DIRECTION = 16,
   EVTF_CAMERA_TBD_017 = 17,
   EVTF_COMPASS = 19,
   EVTF_PUSHED_BOULDER = 20,
   EVTF_UNIT_ATTACKING = 21,
   EVTF_PROJECTILE = 22,
   EVTF_RANGED_TARGET_CAMERA = 23,
   EVTF_BOUNCE_ZOOM = 24,
   EVTF_OVERHEAD_MAP_VIEW = 25,
   EVTF_CAMERA_TBD_026 = 26,
   EVTF_TARGETING_SPELL = 27,
   EVTF_UNIT_CASTING = 28,
   EVTF_PROJECTILE_INDIRECT = 29, // Unused? maybe to open chest w/ ranged attacker?
   EVTF_FIELD_INFO = 30,
   EVTF_BATTLE_SPELLS_LIST = 31,
   EVTF_DISPLAY_DAMAGE = 32,
   EVTF_DISPLAY_DAMAGE_2 = 33,
   EVTF_MAP_OBJECT_TREE = 35,
   EVTF_MAP_OBJECT_GRAVE_MARKER = 36,
   EVTF_MAP_OBJECT_FOUNTAIN = 37,
   EVTF_MAP_OBJECT_LAMP_POST = 38,
   EVTF_MAP_OBJECT_FLAG = 39,
   EVTF_MAP_OBJECT_CHEST = 40,
   EVTF_MAP_OBJECT_FLOWING_SAND = 42,
   EVTF_SETUP_MAP_OBJECTS = 43,
   EVTF_BLOOD_SPURT_PARTICLE_OFFSET = 45,
   EVTF_MAP_OBJECT_CRATE = 46,
   EVTF_BATTLE_MAP_CURSOR = 47,
   EVTF_PUSH = 48,
   EVTF_BATTLE_MAP_CURSOR_CONTROL = 49,
   EVTF_UNIT_SPRITES_DECODER = 50,
   EVTF_FLOATING_DAMAGE_TEXT = 51,
   EVTF_ATTACK_INFO_MARKER = 52,
   EVTF_STRETCH_WARP_SPRITE = 62,
   EVTF_CIRCLE = 77,
   EVTF_DAMAGE_FX2 = 78,
   EVTF_SLAY_FX3 = 79,
   EVTF_ROMAN_FIRE_FLAME = 81,
   EVTF_AVALANCHE_DUST_CLOUD = 84,
   EVTF_MAP13_EXPLOSION_PILLAR = 85,
   EVTF_MAP15_HULL_SPLASH = 86,
   EVTF_DAGGER_STORM_FX2 = 90,
   EVTF_DAGGER_STORM_DAGGER = 91,
   EVTF_MOOD_RING_RING = 95,
   EVTF_MOOD_RING_FX2 = 96,
   EVTF_MAP20_SCN30_ARROW = 98,
   EVTF_HEALING_FX2 = 100,
   EVTF_HEALING_SPARKLE = 101,
   EVTF_POISON_FX2 = 102,
   EVTF_POISON_BUBBLES = 103,
   EVTF_MAGIC_CHARGE_GLYPH_RING = 107,
   EVTF_HARMFUL_WAVE_RING = 109,
   EVTF_CASTING_STAT_BUFF = 110,
   EVTF_FAERIE_SPARKLE = 116,
   EVTF_FAERIE_SPARKLE_TRAIL = 117,
   EVTF_FAERIE_TARGET = 118,
   EVTF_FX_TBD_119 = 119,
   EVTF_SLAY_UNIT = 131,
   EVTF_FX_TBD_132 = 132,
   EVTF_FX_TBD_133 = 133,
   EVTF_FX_TBD_134 = 134,
   EVTF_FX_TBD_136 = 136,
   EVTF_FX_TBD_137 = 137,
   EVTF_FX_TBD_138 = 138,
   EVTF_FX_TBD_139 = 139,
   EVTF_FX_TBD_140 = 140,
   EVTF_FX_TBD_141 = 141,
   EVTF_FX_TBD_142 = 142,
   EVTF_HOLY_LIGHTNING_FX2 = 144,
   EVTF_HOLY_LIGHTNING_ELECTRIC_ORB = 145,
   EVTF_FX_TBD_147 = 147,
   EVTF_FX_TBD_148 = 148,
   EVTF_FLASHING_UNIT_SPRITE = 149,
   EVTF_SPELLBIND_GLYPH = 155,
   EVTF_DELTA_MIRAGE_RAY = 157,
   EVTF_EXPLOSION_RAYS = 159,
   EVTF_ICE_STORM_TARGET = 160,
   EVTF_ICE_STORM_SPLASH = 162,
   EVTF_STONE_SHOWER_ROCK = 164,
   EVTF_STONE_SHOWER_FX2 = 165,
   EVTF_RAINBOW_SWIRL = 167,
   EVTF_RAINBOW_STROKE_RAINBOW_SWIRL = 168,
   EVTF_HOMING_RAY = 171,
   EVTF_HOLY_PRESSURE_CUBE = 172,
   EVTF_FIRE_GEM_BEAM = 173,
   EVTF_THUNDER_FLASH_RAY = 179,
   EVTF_FX_TBD_183 = 183,
   EVTF_AVALANCHE_ROCK = 185,
   EVTF_FX_TBD_186 = 186,
   EVTF_DARK_FIRE_RAY = 188,
   EVTF_MAGIC_ARROW_ARROW = 191,
   EVTF_DARK_STAR_FX2 = 193,
   EVTF_ROLLING_THUNDER_FX2 = 195,
   EVTF_ROLLING_THUNDER_ORB_PAIR = 196,
   EVTF_ROLLING_THUNDER_CASTING_BOLT = 198,
   EVTF_UNIT_STRUCK = 201,
   EVTF_UNIT_BLOCKING = 202,
   EVTF_BLOCKING_IMPACT_PARTICLE = 203,
   EVTF_SUMMON_CREST = 204,
   EVTF_BLOOD_SPURT = 205,
   EVTF_BLOOD_SPURT_PARTICLE = 206,
   EVTF_AVALANCHE_BOULDER = 211,
   EVTF_HOLY_LIGHTNING_CASTING_BOLT = 212,
   EVTF_DUST_CLOUD_SPAWNER = 213,
   EVTF_DUST_CLOUD = 214,
   EVTF_CLOUD = 215,
   EVTF_EXPLOSION_FX2 = 220,
   EVTF_FX_TBD_223 = 223,
   EVTF_THUNDER_BALL_INITIAL_ORB = 225,
   EVTF_THUNDER_BALL_CHILD_ORB = 226,
   EVTF_MAP36_LEENA = 256,
   EVTF_MAP36_VILLAGER = 258,
   EVTF_STAT_RAISED = 272,
   EVTF_OUTWARD_RAY = 273,
   EVTF_CAMERA_TBD_277 = 277,
   EVTF_FAINT_SPARKLES = 278,
   EVTF_ICE_STORM_CAMERA = 279,
   EVTF_DARK_HURRICANE_TARGET = 280,
   EVTF_DARK_HURRICANE_FX2 = 281,
   EVTF_DARK_HURRICANE_FX3 = 282,
   EVTF_DARK_HURRICANE_CLOUD = 283,
   EVTF_CASTING_FX = 285,
   EVTF_MAP13_BRIDGE_EXPLOSION_IN_BATTLE = 288,
   EVTF_CHIMNEY_SMOKE_RING = 289,
   EVTF_REVEAL_CHEST_ITEM = 290,
   EVTF_CHEST_IMPACT = 291,
   EVTF_BLUE_ITEM_SPARKLE = 292,
   EVTF_REVEAL_HIDDEN_ITEM = 294,
   EVTF_SMOKE = 295, //?
   EVTF_MAP17_SPRAY_PARTICLE = 296,
   EVTF_SPLASH_DROPLETS = 297,
   EVTF_SPLASH_WITH_DROPLETS = 298,
   EVTF_MAP15_OCEAN = 299,
   EVTF_MAP32_SMOKESTACK = 300,
   EVTF_MAP32_SMOKESTACK_PARTICLE = 301,
   EVTF_CHIMNEY_SMOKE = 302,
   EVTF_MAP31_SCN61_XENO_FLAME = 304,
   EVTF_EXTRA_HEALING_FX2 = 306, // + Hyper Healing
   EVTF_EVIL_STREAM_FX3 = 307,
   EVTF_EXPLOSION = 309,
   EVTF_LIFE_ORB_BEAM = 310,
   EVTF_BUBBLE_SWIRL_BUBBLE = 312,
   EVTF_INWARD_RAY = 314,
   EVTF_VERTICAL_RAY = 315,
   EVTF_AVALANCHE_FX3 = 317,
   EVTF_FX_TBD_323 = 323,
   EVTF_EVIL_STREAM_FX2 = 324,
   EVTF_SALAMANDER_HEAD = 335,
   EVTF_SALAMANDER_SEGMENT = 336,
   EVTF_DAGGER_STORM_BLOOD_SPLATTER = 337,
   EVTF_AVALANCHE_RUBBLE = 339,
   EVTF_FILE_SAVE_DIALOG = 341,
   EVTF_FILE_SAVE_DIALOG_IBS = 342,
   EVTF_ROMAN_FIRE_FX2 = 344,
   EVTF_ROMAN_FIRE_FX3 = 345,
   EVTF_BUTTON_DEPRESS = 346,
   EVTF_MAP26 = 347,
   EVTF_RUBBLE = 349,
   EVTF_MSGBOX_TEXT = 351,
   EVTF_MAP29 = 352,
   EVTF_MAP19_ELEVATOR_355 = 355,
   EVTF_MAP19_ELEVATOR_356 = 356,
   EVTF_MAP19 = 357,
   EVTF_MAP19_ELEVATOR = 358,
   EVTF_PHASE_SHIFT_MAP_SCALER = 359,
   EVTF_FILE_LOAD_DIALOG_360 = 360,
   EVTF_MAP13_BRIDGE_EXPLOSION_IN_SCENE = 361,
   EVTF_DRAWBRIDGE_BUTTON = 362,
   EVTF_MAP15_PLANK = 364,
   EVTF_MAP17_FLOODGATE = 365,
   EVTF_MAP17_BUTTON = 366,
   EVTF_MAP17_DRAINING_WATER = 368,
   EVTF_SCREEN_EFFECT = 369,
   EVTF_MAGE_OIL_FX2 = 370,
   EVTF_RESTORE_8_MP_UNK_FX2 = 371,
   EVTF_MAGIC_CHARGE_FX2 = 372, // + Mage Gem
   EVTF_FILE_LOAD_DIALOG = 373,
   EVTF_FILE_LOAD_DIALOG_IBS = 374,
   EVTF_FLAME_BREATH_PARTICLE = 375,
   EVTF_FILE_LOAD_DIALOG_376 = 376,
   EVTF_EVIL_STREAM_ROCK = 379,
   EVTF_LEVEL_UP_FX = 380,
   EVTF_FLAME_BREATH = 382,
   EVTF_SPARKLE = 383,
   EVTF_REVEAL_MIMIC = 385,
   EVTF_HEALING_SPARKLE_386 = 386,
   EVTF_FULLSCREEN_IMAGE = 387,
   EVTF_DARK_HURRICANE_VORTEX = 389,
   EVTF_DARK_HURRICANE_VORTEX_LAYER = 390,
   EVTF_DYNAMO_HUM_ELECTRIC_ORB = 395,
   EVTF_DYNAMO_HUM_ORB_ELECTRICITY = 396,
   EVTF_FX_TBD_398 = 398,
   EVTF_MAP11 = 399,
   EVTF_AI_TBD_400 = 400,
   EVTF_AI_TBD_401 = 401,
   EVTF_AI_TBD_402 = 402,
   EVTF_AI_TBD_403 = 403,
   EVTF_AI_TBD_404 = 404,
   EVTF_PANORAMA = 405,
   EVTF_NOOP_407 = 407,
   EVTF_CLOSED_WINDOW = 408,
   EVTF_EVENT_ENTITY = 409,
   EVTF_EVENT_ZOOM = 410,
   EVTF_MAP_OBJECT_VILE_BOG = 411,
   EVTF_EVENT_CAMERA = 412,
   EVTF_MSGBOX_PORTRAIT = 413,
   EVTF_DEBUG_MENU = 414,
   EVTF_MAP_OBJECT_TORCH = 415,
   EVTF_BATTLE_VICTORY = 420,
   EVTF_UPPER_MSGBOX_TAIL = 421,
   EVTF_LOWER_MSGBOX_TAIL = 422,
   EVTF_BATTLE_DEFEAT = 423,
   EVTF_BATTLE_ENDER = 424,
   EVTF_BATTLE_OPTIONS = 425,
   EVTF_EVALUATE_BATTLE_10 = 426,
   EVTF_EVALUATE_BATTLE_11 = 427,
   EVTF_EVALUATE_BATTLE_12 = 428,
   EVTF_EVALUATE_BATTLE_13 = 429,
   EVTF_EVALUATE_BATTLE_14 = 430,
   EVTF_EVALUATE_BATTLE_15 = 431,
   EVTF_EVALUATE_BATTLE_16 = 432,
   EVTF_EVALUATE_BATTLE_17 = 433,
   EVTF_EVALUATE_BATTLE_STD = 434,
   EVTF_EVALUATE_BATTLE_19 = 435,
   EVTF_EVALUATE_BATTLE_20 = 436,
   EVTF_EVALUATE_BATTLE_21 = 437,
   EVTF_EVALUATE_BATTLE_08 = 438,
   EVTF_EVALUATE_BATTLE_23 = 439,
   EVTF_EVALUATE_BATTLE_26 = 442,
   EVTF_EVALUATE_BATTLE_27 = 443,
   EVTF_EVALUATE_BATTLE_28 = 444,
   EVTF_EVALUATE_BATTLE_29 = 445,
   EVTF_BATTLE_VICTORY_PARTICLE = 446,
   EVTF_MAP_OBJECT_FLOWING_WATER = 449,
   EVTF_EVALUATE_BATTLE_32 = 552,
   EVTF_EVALUATE_BATTLE_33 = 553,
   EVTF_EVALUATE_BATTLE_35 = 555,
   EVTF_EVALUATE_BATTLE_37 = 557,
   EVTF_EVALUATE_BATTLE_38 = 558,
   EVTF_EVALUATE_BATTLE_39 = 559,
   EVTF_EVALUATE_BATTLE_40 = 560,
   EVTF_EVALUATE_BATTLE_42 = 562,
   EVTF_EVALUATE_BATTLE_43 = 563,
   EVTF_MAP_OBJECT_WATER_1 = 564,
   EVTF_MAP_OBJECT_WATER_2 = 565,
   EVTF_MAP_OBJECT_LAVA_1 = 566,
   EVTF_OPENING_CHEST = 567,
   EVTF_MAP_OBJECT_RAIL = 568,
   EVTF_MAP_OBJECT_LAVA_2 = 569,
   EVTF_AI_TBD_570 = 570,
   EVTF_LEVEL_UP = 571,
   EVTF_MAP_OBJECT_LAVA_3 = 572,
   EVTF_BATTLE_ITEMS_LIST = 573,
   EVTF_DISPLAY_ICON = 574,
   EVTF_STATUS_PORTRAIT = 575,
   EVTF_TAVERN = 576,
   EVTF_AUDIO_CMD = 581,
   EVTF_MAIN_MENU_JPN = 582,
   EVTF_BATTLE_PLAYER_EVENT = 585,
   EVTF_BATTLE_MSGBOX = 586,
   EVTF_BATTLE_ENEMY_EVENT = 587,
   EVTF_CAMERA_TBD_588 = 588,
   EVTF_AI_TBD_589 = 589,
   EVTF_BATTLE_TURN_TICKER = 590,
   EVTF_MAP_OBJECT_BOULDER = 591,
   EVTF_BATTLE_TURN_START = 592,
   EVTF_BATTLE_RESULTS_UNIT = 593,
   EVTF_BATTLE_RESULTS = 594,
   EVTF_STATUS_WINDOW = 595,
   EVTF_STATUS_WINDOW_MGR = 596,
   EVTF_BATTLE_INTRO = 597,
   EVTF_MAP32_CAR_RELEASE = 650,
   EVTF_MAP33_LAVA_PIT_PLATFORM = 651,
   EVTF_MAP35_BUTTON = 652,
   EVTF_EXPLODING_TILE = 653,
   EVTF_MAP38_WASH_AWAY_UNIT = 654,
   EVTF_MAP38_RAISE_FLOODGATE = 655,
   EVTF_MAP39 = 656,
   EVTF_MAP38_FLOODGATE = 657,
   EVTF_MAP38_FLOODWATER = 658,
   EVTF_SPLASH = 659,
   EVTF_FLAME = 661,
   EVTF_MAP28_OPEN_DOOR = 662,
   EVTF_MAP28_BUTTON = 663,
   EVTF_MAP27_OPEN_CELL_DOOR = 664,
   EVTF_MAP27_BUTTONS = 665,
   EVTF_MAP14_LOWER_SAND_MOUND = 666,
   EVTF_MAP14_LOWER_SAND_TILE = 667,
   EVTF_MAP14_RAISE_SAND_MOUND = 668,
   EVTF_MAP14_RAISE_SAND_TILE = 669,
   EVTF_MAP14_SAND = 670,
   EVTF_MAP39_SPLASHING_TILE = 672,
   EVTF_HEAVY_RAINFALL = 676,
   EVTF_RAINFALL_DROP = 677,
   EVTF_RIPPLE = 678,
   EVTF_STAT_BUFF_FX = 681,
   EVTF_ADJUST_FACE_ELEVATION = 683,
   EVTF_SLIDING_FACE = 684,
   EVTF_ROCK_SPURT = 685,
   EVTF_ROCK_SPURT_PARTICLE_2 = 686,
   EVTF_LIGHT_RAINFALL = 687,
   EVTF_DYNAMO_HUM_COLORED_BOLT = 700,
   EVTF_FLAMING_ROCK = 702,
   EVTF_MAP40_BARRICADE = 703,
   EVTF_MAP14_UNUSED_708 = 708,
   EVTF_MAP14_UNUSED_709 = 709,
   EVTF_PARTICLE_710 = 710, // Generic animated particle?
   EVTF_SPELLBIND_FX2 = 715,
   EVTF_SPELLBIND_FX3 = 716,
   EVTF_TBD_717 = 717,
   EVTF_TBD_718 = 718,
   EVTF_TBD_732 = 732,
   EVTF_STAT_BUFF_ICON = 733,
   EVTF_SPARKLE_DUST = 735,
   EVTF_REMOVE_PARALYSIS_BUBBLE = 736,
   EVTF_REMOVE_PARALYSIS = 737,
   EVTF_MAP40_LOWER_BARRICADE = 738,
   EVTF_PARTICLE_739 = 739,
   EVTF_REMOVE_PARALYSIS_SPARKLES = 740,
   EVTF_ENTITY_BLOCKING = 746,
   EVTF_WYRMFANG_FLAMES_CW = 747,
   EVTF_WYRMFANG_FLAMES_CCW = 748,
   EVTF_WYRMFANG_FLAME = 749,
   EVTF_MAP33_SCN65_LOWER_PLATFORM = 750,
   EVTF_MAP33_LOWER_PLATFORM = 751,
   EVTF_MAP15_PIRATE_STAND_IN = 755,
   EVTF_PUSHED_OBJECT_SPLASH = 757,
   EVTF_ROCK_SPURT_PARTICLE = 759,
   EVTF_ELITE_MELEE_SPARKLES = 760,
   EVTF_REVEAL_USED_ITEM = 761,
   EVTF_BOULDER_RUBBLE = 763,
   EVTF_PROJECTILE_TRAIL_POISON = 764,
   EVTF_PROJECTILE_TRAIL_EXPLOSION = 765,
   EVTF_PROJECTILE_TRAIL_SMOKE = 766,
   EVTF_PROJECTILE_TRAIL_SPARKLES = 767,
   EVTF_ITEM_SPELL = 770,
   EVTF_HEALING_PLUS_FX2 = 791,
   EVTF_ULTRA_HEALING_FX2 = 792,
   EVTF_SUPREME_HEALING_FX2 = 793, // + Holy H2O
   EVTF_FADE = 795,
   EVTF_FX_TBD_799 = 799,
   EVTF_FX_TBD_800 = 800,
   EVTF_FX_TBD_801 = 801,
   EVTF_FX_TBD_802 = 802,
   EVTF_FX_TBD_803 = 803,
} EvtFunctionIdx;

typedef struct EvtData_Sprite {
   /* :0x24 */ s8 hidden;
   /* :0x25 */ s8 facingLeft;
   /* :0x26 */ s8 animInitialized;
   /* :0x27 */ s8 finishedMoving;
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ s16 direction; /* s->w->n->e, 0x400 increments */
   /* :0x2C */ s16 clut;
   /* :0x2E */ s8 currentFrameDelay;
   /* :0x2F */ s8 animDataIdx;
   /* :0x30 */ s8 animFinished;
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ s16 stripIdx;
   /* :0x38 */ void *animData;
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s8 facingFront;
   /* :0x55 */ u8 unk_0x55[3];
   /* :0x58 */ s16 animRelativeOfs;
   /* :0x5A */ s16 animYOfs;
   /* :0x5C */ s8 animSingleAxis;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_Sprite;

/* TBD - Sprites passed to AddEvtPrim2 use a different coord layout (xy), and sometimes use other
 * fields for their own data. */
typedef struct EvtData_Sprite2 {
   /* :0x24 */ s8 hidden;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[3];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ SVectorXY coords[4];
   /* :0x4C */ u8 unk_0x4C[20];
} EvtData_Sprite2;

/* Data Store (Union) */
typedef union EvtData_DataStore {
   s8 bytes[60];
   s16 shorts[30];
   s32 ints[15];
   struct EvtData *evts[15];
   void *pointers[15];
   Cylinder cylinder;
   SVECTOR svectors[7];
} EvtData_DataStore;

/* Battle - Actions */
typedef struct EvtData_003 {
   /* :0x24 */ s8 range;
   /* :0x25 */ s8 performedSubaction; // Whether unit has committed to a turn by using examine/push
   /* :0x26 */ u8 unk_0x26;
   /* :0x27 */ s8 remainingRange;
   /* :0x28 */ struct UnitStatus *unit;
   /* :0x2C */ u8 unk_0x2C;
   /* :0x2D */ s8 cursorUnitIdx;
   /* :0x2E */ u8 unk_0x2E[30];
   /* :0x4C */ s8 timer;
   /* :0x4D */ u8 unk_0x4D[19];
} EvtData_003;

/* Window (incomplete) */
typedef struct EvtData_004_005_408 {
   /* :0x24 */ s32 effectPhase;
   /* :0x28 */ s16 clut;
   /* :0x2A */ s16 choicesTopMargin;
   /* :0x2C */ /*s16 todo_x2c;*/ s32 todo_x2c;
   ///* :0x2E */ s16 highlightYOffset;
   /* :0x30 */ s32 todo_x30;
   /* :0x34 */ s8 highlightHeight;
   /* :0x35 */ s8 choiceCt;
   /* :0x36 */ s8 choiceHeight;
   /* :0x37 */ s8 windowId;
   /* :0x38 */ s16 halfHeight;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ s16 halfWidth;
   /* :0x3E */ u8 unk_0x3E[2];
   /* :0x40 */ s16 relQuadX0;
   /* :0x42 */ s16 relQuadY0;
   /* :0x44 */ s16 relQuadX1;
   /* :0x46 */ s16 relQuadY1;
   /* :0x48 */ s16 relQuadX2;
   /* :0x4A */ s16 relQuadY2;
   /* :0x4C */ s16 relQuadX3;
   /* :0x4E */ s16 relQuadY3;
   /* :0x50 */ struct EvtData *highlight;
   /* :0x54 */ struct EvtData *window;
   /* :0x58 */ s16 effectX;
   /* :0x5A */ s16 effectY;
   /* :0x5C */ s8 effect;
   /* :0x5D */ s8 translucentHighlight;
   /* :0x5E */ s8 otOfs;
   /* :0x5F */ s8 disableWraparound;
} EvtData_004_005_408;

/* Apply Poison */
typedef struct EvtData_007 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 timer;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_007;

/* Battle Portrait */
typedef struct EvtData_008 {
   /* :0x24 */ s8 windowId;
   /* :0x25 */ s8 flipped;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ struct EvtData *window;
   /* :0x2C */ struct EvtData *sprite;
   /* :0x30 */ s16 portraitId;
   /* :0x32 */ u8 unk_0x32[46];
} EvtData_008;

/* Battle Manager */
typedef struct EvtData_013 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ struct EvtData *unitSprite;
   /* :0x2C */ s8 timer;
   /* :0x2D */ s8 todo_x2d;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_013;

/* Battle Unit */
typedef struct EvtData_014 {
   /* :0x24 */ u8 team;
   /* :0x25 */ u8 unitIdx;
   /* :0x26 */ s8 pathIdx;
   /* :0x27 */ s8 animIdx;
   /* :0x28 */ struct UnitStatus *unit;
   /* :0x2C */ u8 **animSet;
   /* :0x30 */ struct EvtData *sprite;
   /* :0x34 */ s8 timer;
   /* :0x35 */ u8 unk_0x35;
   /* :0x36 */ s16 direction;
   /* :0x38 */ u8 unk_0x38[40];
} EvtData_014;

/* Choose Done Direction */
typedef struct EvtData_016 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s16 angle;
   /* :0x2A */ u8 unk_0x2A[54];
} EvtData_016;

/* Camera - TBD */
typedef struct EvtData_017 {
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s16 camSavedX;
   /* :0x2A */ s16 camSavedZ;
   /* :0x2C */ s16 camSavedY;
   /* :0x2E */ s16 camSavedRotX;
   /* :0x30 */ s16 camSavedRotZ;
   /* :0x32 */ s16 camSavedRotY;
   /* :0x34 */ s16 camSavedZoom;
   /* :0x36 */ s16 dstZoom;
   /* :0x38 */ s16 dstCamRotY;
   /* :0x3A */ s16 timer;
   /* :0x3C */ s16 savedGeomOfsY;
   /* :0x3E */ s16 geomOfsY;
   /* :0x40 */ s16 dstGeomOfsY;
   /* :0x42 */ u8 unk_0x42[30];
} EvtData_017;

/* Pushed Boulder */
typedef struct EvtData_020 {
   /* :0x24 */ struct EvtData *unitSprite;
   /* :0x28 */ s16 savedCamX;
   /* :0x2A */ s16 savedCamZ;
   /* :0x2C */ s16 savedCamY;
   /* :0x2E */ s16 savedCamRotX;
   /* :0x30 */ u8 unk_0x30[4];
   /* :0x34 */ s16 savedZoom;
   /* :0x36 */ u8 unk_0x36[4];
   /* :0x3A */ s16 timer;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_020;

/* Unit Attacking */
typedef struct EvtData_021 {
   /* :0x24 */ struct UnitStatus *attacker;
   /* :0x28 */ struct UnitStatus *defender;
   /* :0x2C */ struct EvtData *attackerSprite;
   /* :0x30 */ struct EvtData *defenderSprite;
   /* :0x34 */ s8 countering;
   /* :0x35 */ u8 unk_0x35[3];
   /* :0x38 */ s8 timer;
   /* :0x39 */ s8 mapSizeX;
   /* :0x3A */ s8 mapSizeZ;
   /* :0x3B */ u8 unk_0x3B[37];
} EvtData_021;

/* Projectile */
typedef struct EvtData_022_029 {
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s16 xMid;
   /* :0x2A */ s16 zMid;
   /* :0x2C */ s16 xzMidDist;
   /* :0x2E */ s16 zMidDist;
   /* :0x30 */ s16 yRotOfs;
   /* :0x32 */ s16 zRotOfs;
   /* :0x34 */ u8 unk_0x34[4];
   /* :0x38 */ s16 zRot;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_022_029;

/* Camera - Ranged Target */
typedef struct EvtData_023 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ s16 yRotDst;
   /* :0x2A */ s16 delay;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_023;

/* Camera - Bounce Zoom */
typedef struct EvtData_024 {
   /* :0x24 */ s16 soft;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 savedZoom;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_024;

/* Overhead Map View */
typedef struct EvtData_025 {
   /* :0x24 */ s16 camSavedRotX;
   /* :0x26 */ s16 camSavedRotY;
   /* :0x28 */ s16 camSavedZoom;
   /* :0x2A */ s16 camSavedX;
   /* :0x2C */ s16 camSavedY;
   /* :0x2E */ s16 camSavedZ;
   /* :0x30 */ s8 delay;
   /* :0x31 */ u8 unk_0x31[47];
} EvtData_025;

/* Camera - TBD */
typedef struct EvtData_026_588 {
   /* :0x24 */ struct EvtData *target;
   /* :0x28 */ s8 type;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s16 zoom;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[8];
   /* :0x36 */ s16 dstZoom;
   /* :0x38 */ s16 dstCamRotY;
   /* :0x3A */ s16 timer;
   /* :0x3C */ s16 savedGeomOfsY;
   /* :0x3E */ s16 geomOfsY;
   /* :0x40 */ s16 dstGeomOfsY;
   /* :0x42 */ u8 unk_0x42[2];
   /* :0x44 */ s8 todo_x44;
   /* :0x45 */ u8 unk_0x45[27];
} EvtData_026_588;

/* Targeting Spell */
typedef struct EvtData_027 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ struct UnitStatus *caster;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_027;

/* Unit Casting Spell */
typedef struct EvtData_028 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ s8 mapSizeX;
   /* :0x26 */ s8 mapSizeZ;
   /* :0x27 */ u8 unk_0x27[57];
} EvtData_028;

/* Unit/Field Info */
typedef struct EvtData_030 {
   /* :0x24 */ s8 previousTerrain;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s8 unitIdx;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_030;

/* Battle Spells List */
typedef struct EvtData_031 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 spell;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_031;

/* Display Damage */
typedef struct EvtData_032_033 {
   /* :0x24 */ struct EvtData *barFg;
   /* :0x28 */ struct EvtData *barBg;
   /* :0x2C */ s16 barDstX3;
   /* :0x2E */ s16 timer;
   /* :0x30 */ s32 maxHp;
   /* :0x34 */ s32 prevHp;
   /* :0x38 */ s32 curHp;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_032_033;

/* Map Object - Chest */
typedef struct EvtData_040 {
   /* :0x24 */ s8 item;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s16 lidAngle;
   /* :0x2A */ s16 lidAngleVel;
   /* :0x2C */ s8 facing;
   /* :0x2D */ s8 terrain;
   /* :0x2E */ u8 unk_0x2E[34];
   /* :0x50 */ s16 lid_todo_x50;
   /* :0x52 */ s16 lid_todo_x52;
   /* :0x54 */ s16 lid_todo_x54;
   /* :0x56 */ s16 lid_todo_x56;
   /* :0x58 */ s16 lid_todo_x58;
   /* :0x5A */ s16 lid_todo_x5a;
   /* :0x5C */ s16 lid_todo_x5c;
   /* :0x5E */ s16 lid_todo_x5e;
} EvtData_040;

/* Map Object - Crate */
typedef struct EvtData_046 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s8 stack;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s16 elevation;
   /* :0x2C */ u8 unk_0x2C[48];
   /* :0x5C */ s8 terrain;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_046;

/* Push */
typedef struct EvtData_048 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 stack;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s16 direction;
   /* :0x2C */ s8 targetX;
   /* :0x2D */ s8 targetZ;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 osc;
   /* :0x32 */ u8 unk_0x32[42];
   /* :0x5C */ s8 timer;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_048;

/* Unit Sprites Decoder */
typedef struct EvtData_050 {
   /* :0x24 */ u8 unk_0x24[24];
   /* :0x3C */ s32 remainingBytes;
   /* :0x40 */ u16 encodingBits;
   /* :0x42 */ u16 cacheOfs;
   /* :0x44 */ void *src;
   /* :0x48 */ void *dst;
   /* :0x4C */ u8 unk_0x4C[4];
   /* :0x50 */ void *baseSrcDataPtr;
   /* :0x54 */ void *baseDstDataPtr;
   /* :0x58 */ s16 vramX;
   /* :0x5A */ s16 vramY;
   /* :0x5C */ u8 unk_0x5C[4];
} EvtData_050;

/* Floating Damage Text */
/* TODO: Rename stuff to also account for restoration. */
typedef struct EvtData_051 {
   /* :0x24 */ s16 damage;
   /* :0x26 */ s16 clut;
   /* :0x28 */ s16 phase;
   /* :0x2A */ s16 timer;
   /* :0x2C */ u8 unk_0x2C[32];
   /* :0x4C */ s16 hundreds;
   /* :0x4E */ s16 tens;
   /* :0x50 */ s16 ones;
   /* :0x52 */ s16 numDigits;
   /* :0x54 */ u8 unk_0x54[4];
   /* :0x58 */ struct EvtData *unitSprite;
   /* :0x5C */ struct EvtData *digitSprite;
} EvtData_051;

/* Attack Info Marker */
typedef struct EvtData_052 {
   /* :0x24 */ s16 type;
   /* :0x26 */ s16 clut;
   /* :0x28 */ s16 angle;
   /* :0x2A */ s16 timer;
   /* :0x2C */ u8 unk_0x2C[44];
   /* :0x58 */ struct EvtData *unitSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_052;

/* Stretch Warp Sprite */
typedef struct EvtData_062 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ s16 phase;
   /* :0x28 */ u8 unk_0x28[40];
   /* :0x50 */ struct EvtData *sprite;
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_062;

/* Fade From Black */
typedef struct EvtData_070 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_070;

/* Fade To Black */
typedef struct EvtData_071 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_071;

/* Fade From White */
typedef struct EvtData_072 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_072;

/* Fade To White */
typedef struct EvtData_073 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_073;

/* Fade In Sprite */
typedef struct EvtData_074 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 fade;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_074;

/* Fade Out Sprite */
typedef struct EvtData_075 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 fade;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_075;

/* FX - Circles (TBD) */
typedef struct EvtData_076 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[40];
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_076;

/* FX - Circle (TBD) */
typedef struct EvtData_077 {
   /* :0x24 */ s16 fade;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 angle;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_077;

/* Damage - FX2 */
typedef struct EvtData_078 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_078;

/* Slay - FX3 */
typedef struct EvtData_079 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_079;

/* Roman Fire - FX1 */
typedef struct EvtData_080 {
   /* :0x24 */ u8 unk_0x24[48];
   /* :0x54 */ struct EvtData *target;
   /* :0x58 */ struct EvtData *cam;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_080;

/* FX - TBD */
typedef struct EvtData_082 {
   /* :0x24 */ s16 radius;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 theta1;
   /* :0x2A */ s16 theta2;
   /* :0x2C */ s16 theta3;
   /* :0x2E */ s16 theta4;
   /* :0x30 */ u8 unk_0x30[40];
   /* :0x58 */ struct EvtData *todo_x58;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_082;

/* Map 20 - Scene 30 - Arrow Spawner */
typedef struct EvtData_087 {
   /* :0x24 */ union {
      struct EvtData *entitySpriteParam;
      struct {
         s16 z, y;
      } velocity;
   } variant_0x24;
   /* :0x28 */ s16 yAccel;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *entitySprite;
} EvtData_087;

/* Map 15 - Scene 17 - Cinematic */
typedef struct EvtData_089 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ Camera camera;
   /* :0x36 */ u8 unk_0x36[26];
   /* :0x50 */ struct EvtData *focus;
   /* :0x54 */ struct EvtData *ocean;
   /* :0x58 */ s16 dstRotY;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *plank;
} EvtData_089;

/* Dagger Storm - FX2 */
typedef struct EvtData_090 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ s16 fatal;
   /* :0x5E */ u8 unk_0x5E[2];
} EvtData_090;

/* Dagger Storm - Dagger */
typedef struct EvtData_091 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[18];
   /* :0x3C */ SVectorXZY positions[4];
   /* :0x54 */ u8 unk_0x54[4];
   /* :0x58 */ struct EvtData *targetSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_091;

/* Mood Ring - FX1 */
typedef struct EvtData_094 {
   /* :0x24 */ u8 unk_0x24[52];
   /* :0x58 */ s8 launchingOutward;
   /* :0x59 */ u8 unk_0x59[7];
} EvtData_094;

/* Mood Ring - Ring */
typedef struct EvtData_095 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx_unused;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx_unused;
   /* :0x36 */ u8 unk_0x36[14];
   /* :0x44 */ s32 unused_0x44;
   /* :0x48 */ u8 unk_0x48[12];
   /* :0x54 */ s16 theta;
   /* :0x56 */ s16 radius;
   /* :0x58 */ u8 unk_0x58[4];
   /* :0x5C */ struct EvtData *parent; // evtf094
} EvtData_095;

/* Mood Ring - FX2 */
typedef struct EvtData_096 {
   /* :0x24 */ s16 fatal;
   /* :0x26 */ u8 unk_0x26[54];
   /* :0x5C */ struct EvtData *targetSprite;
} EvtData_096;

/* Mood Ring - FX3 */
typedef struct EvtData_097 {
   /* :0x24 */ s16 fatal;
   /* :0x26 */ u8 unk_0x26[54];
   /* :0x5C */ struct EvtData *targetSprite;
} EvtData_097;

/* Map 20 - Scene 30 - Arrow */
typedef struct EvtData_098 {
   /* :0x24 */ s8 hidden;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[6];
   /* :0x30 */ s8 animFinished;
   /* :0x31 */ u8 unk_0x31[11];
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ SVectorXZY pos;
   /* :0x5A */ u8 unk_0x5A[6];
} EvtData_098;

/* Healing - FX2 */
// Used for: Healing, Self Healing, Herb, Megaherb, Moon Pie
typedef struct EvtData_100 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ u8 unk_0x3C[32];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_100;

/* Healing Sparkle */
typedef struct EvtData_101 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ u8 unk_0x2C[44];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_101;

/* Poison - FX2 */
typedef struct EvtData_102_227 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ u8 unk_0x3C[32];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_102_227;

/* Poison Bubbles */
typedef struct EvtData_103 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ u8 unk_0x2C[44];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_103;

/* Cure - FX2 */
typedef struct EvtData_104 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[24];
   /* :0x40 */ s16 todo_x40;
   /* :0x42 */ u8 unk_0x42[2];
   /* :0x44 */ struct EvtData *ringSprite;
   /* :0x48 */ u8 unk_0x48[20];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_104;

/* Magic Charge - FX3 */
typedef struct EvtData_106 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *glyphRing;
} EvtData_106;

/* Magic Charge - Glyph Ring */
typedef struct EvtData_107 {
   /* :0x24 */ u8 unk_0x24[12];
   /* :0x30 */ s16 fade;
   /* :0x32 */ s16 fadeTicker;
   /* :0x34 */ u8 unk_0x34[24];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ u8 unk_0x4E[10];
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ u8 unk_0x5A[6];
} EvtData_107;

/* Harmful Wave - FX2 */
typedef struct EvtData_108 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_108;

/* Harmful Wave - Ring */
typedef struct EvtData_109 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 height;
   /* :0x4E */ u8 unk_0x4E[14];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_109;

/* Casting Stat Buff */
typedef struct EvtData_110 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[10];
   /* :0x30 */ s16 glyphsFade;
   /* :0x32 */ s16 crestFade;
   /* :0x34 */ u8 unk_0x34[24];
   /* :0x4C */ s16 clut;
   /* :0x4E */ u8 unk_0x4E[10];
   /* :0x58 */ struct EvtData *buffFx;
   /* :0x5C */ u8 unk_0x5C[4];
} EvtData_110;

/* Faerie - FX2 */
typedef struct EvtData_115 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 timer;
   /* :0x2E */ u8 unk_0x2E[42];
   /* :0x58 */ struct EvtData *targetSprite;
   /* :0x5C */ struct EvtData *faerieSprite;
} EvtData_115;

/* Faerie - Sparkle */
typedef struct EvtData_116 {
   /* :0x24 */ s16 xOfs;
   /* :0x26 */ s16 zOfs;
   /* :0x28 */ s16 yOfs;
   /* :0x2A */ s16 fade;
   /* :0x2C */ u8 unk_0x2C[48];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_116;

/* Faerie - Sparkle Trail */
typedef struct EvtData_117 {
   /* :0x24 */ u8 unk_0x24[6];
   /* :0x2A */ s16 fade;
   /* :0x2C */ u8 unk_0x2C[48];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_117;

/* Faerie - Target */
typedef struct EvtData_118 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ s16 timer;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 unused_0x2C;
   /* :0x2E */ u8 unk_0x2E[30];
   /* :0x4C */ s16 clut;
   /* :0x4E */ s16 fade;
   /* :0x50 */ u8 unk_0x50[16];
} EvtData_118;

/* FX - TBD */
typedef struct EvtData_119 {
   /* :0x24 */ s16 type;
   /* :0x26 */ s16 boxIdx;
   /* :0x28 */ s16 clut;
   /* :0x2A */ s16 semiTrans;
   /* :0x2C */ s16 once;
   /* :0x2E */ s16 radius;
   /* :0x30 */ s16 theta;
   /* :0x32 */ s16 h;
   /* :0x34 */ u8 unk_0x34[24];
   /* :0x4C */ s16 timer;
   /* :0x4E */ u8 unk_0x4E[14];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_119;

/* Dark Fire - FX1 */
typedef struct EvtData_122 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ s16 theta;
   /* :0x28 */ u8 unk_0x28[16];
   /* :0x38 */ s16 savedLightRotZ;
   /* :0x3A */ s16 savedLightRotY;
   /* :0x3C */ u8 unk_0x3C[28];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *target;
} EvtData_122;

/* FX - TBD */
typedef struct EvtData_130 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[38];
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_130;

/* Slay Unit */
typedef struct EvtData_131 {
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[54];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_131;

/* FX - TBD */
/* 132, 134, 136, 138, 140, 142, 799, 800 */
typedef struct EvtData_132_Etc {
   /* :0x24 */ s16 phase; // also used as a timer
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 increasing;
   /* :0x30 */ u8 unk_0x30[28];
   /* :0x4C */ s16 clut;
   /* :0x4E */ u8 unk_0x4E[2];
   /* :0x50 */ struct EvtData *unitSprite;
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_132_Etc;

/* FX - TBD */
typedef struct EvtData_133_Etc {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 semiTrans;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a; // increments x34
   /* :0x2C */ u8 unk_0x2C[2];
   /* :0x2E */ s16 radius;
   /* :0x30 */ s16 theta;
   /* :0x32 */ s16 h;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ u8 unk_0x36[22];
   /* :0x4C */ s16 timer;
   /* :0x4E */ u8 unk_0x4E[14];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_133_Etc;

/* Holy Lightning - FX2 */
typedef struct EvtData_144 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ s16 endingFxType;
   /* :0x28 */ u8 unk_0x28[48];
   /* :0x58 */ struct EvtData *targetFlashingFx;
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_144;

/* Holy Lightning - Electric Orb */
typedef struct EvtData_145 {
   /* :0x24 */ s16 fade;
   /* :0x26 */ u8 unk_0x26[14];
   /* :0x34 */ s16 x_1; // TBD
   /* :0x36 */ s16 y_1;
   /* :0x38 */ s16 z_1;
   /* :0x3A */ s16 x_2;
   /* :0x3C */ s16 y_2;
   /* :0x3E */ s16 z_2;
   /* :0x40 */ s16 todo_x40; //
   /* :0x42 */ s16 todo_x42; //? Randomized but never used?
   /* :0x44 */ s16 todo_x44; //
   /* :0x46 */ u8 unk_0x46[6];
   /* :0x4C */ s16 x_3;
   /* :0x4E */ s16 y_3;
   /* :0x50 */ s16 z_3;
   /* :0x52 */ u8 unk_0x52[2];
   /* :0x54 */ s16 x_4;
   /* :0x56 */ s16 z_4;
   /* :0x58 */ struct EvtData *orbSprite;
   /* :0x5C */ struct EvtData *lightningSprite;
} EvtData_145;

/* FX - TBD */
typedef struct EvtData_147 {
   /* :0x24 */ s16 clut;
   /* :0x26 */ u8 unk_0x26[14];
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ s16 todo_x3c;
   /* :0x3E */ s16 todo_x3e;
   /* :0x40 */ s16 todo_x40;
   /* :0x42 */ s16 todo_x42;
   /* :0x44 */ s16 todo_x44;
   /* :0x46 */ u8 unk_0x46[6];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[10];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_147;

/* FX - TBD */
typedef struct EvtData_148 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ s16 endingFxType;
   /* :0x28 */ u8 unk_0x28[36];
   /* :0x4C */ s16 clut;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_148;

/* Flashing Unit Sprite */
typedef struct EvtData_149 {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 fadeSpeed;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 increasing;
   /* :0x30 */ u8 unk_0x30[44];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_149;

/* Spellbind - Glyph */
typedef struct EvtData_155 {
   /* :0x24 */ s16 initialIdx;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[7];
   /* :0x31 */ u8 todo_x31;
   /* :0x32 */ u8 unk_0x32[10];
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s16 idx;
   /* :0x56 */ s16 theta;
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ s16 todo_x5c;
   /* :0x5E */ s16 todo_x5e;
} EvtData_155;

/* Delta Mirage - FX1 */
typedef struct EvtData_156 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ u8 unk_0x2C[4];
   /* :0x30 */ s16 timer;
   /* :0x32 */ u8 unk_0x32[4];
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ u8 unk_0x38[40];
} EvtData_156;

/* Delta Mirage - Ray */
typedef struct EvtData_157 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ u8 unk_0x38[36];
   /* :0x5C */ struct EvtData *parent;
} EvtData_157;

/* Explosion - FX1 */
typedef struct EvtData_158 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[54];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_158;

/* Explosion - Rays */
typedef struct EvtData_159 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ s16 todo_x3c;
   /* :0x3E */ s16 todo_x3e;
   /* :0x40 */ s16 todo_x40;
   /* :0x42 */ s16 todo_x42;
   /* :0x44 */ s16 todo_x44;
   /* :0x46 */ s16 todo_x46;
   /* :0x48 */ s16 todo_x48;
   /* :0x4A */ s16 todo_x4a;
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ s16 todo_x52;
   /* :0x54 */ u8 unk_0x54[4];
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_159;

/* Ice Storm - Target */
typedef struct EvtData_160 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ s16 timer;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[30];
   /* :0x4C */ s16 clut;
   /* :0x4E */ s16 fade;
   /* :0x50 */ u8 unk_0x50[16];
} EvtData_160;

/* Piercing Light - FX1 */
typedef struct EvtData_161 {
   /* :0x24 */ u8 unk_0x24[20];
   /* :0x38 */ s16 clut;
   /* :0x3A */ u8 unk_0x3A[34];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_161;

/* Ice Storm - Splash */
typedef struct EvtData_162 {
   /* :0x24 */ s16 xOfs;
   /* :0x26 */ s16 zOfs;
   /* :0x28 */ u8 unk_0x28[2];
   /* :0x2A */ s16 fade;
   /* :0x2C */ u8 unk_0x2C[48];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_162;

/* Stone Shower - FX1 */
typedef struct EvtData_163 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 timer;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_163;

/* Stone Shower - Rock */
typedef struct EvtData_164 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[6];
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ u8 unk_0x34[24];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ u8 unk_0x50[4];
   /* :0x54 */ s16 downward;
   /* :0x56 */ u8 unk_0x56[2];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_164;

/* Stone Shower - FX2 */
typedef struct EvtData_165 {
   /* :0x24 */ s16 endingFxType;
   /* :0x26 */ s16 timer;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_165;

/* Rainbow Swirl */
typedef struct EvtData_167 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ s16 todo_x3c;
   /* :0x3E */ s16 todo_x3e;
   /* :0x40 */ s16 todo_x40;
   /* :0x42 */ s16 todo_x42;
   /* :0x44 */ s16 todo_x44;
   /* :0x46 */ s16 todo_x46;
   /* :0x48 */ s16 todo_x48;
   /* :0x4A */ s16 todo_x4a;
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ s16 todo_x52;
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ s16 todo_x56;
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_167;

/* Rainbow Stroke - Rainbow Swirl */
typedef struct EvtData_168 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ s16 todo_x3c;
   /* :0x3E */ s16 todo_x3e;
   /* :0x40 */ s16 todo_x40;
   /* :0x42 */ s16 todo_x42;
   /* :0x44 */ s16 todo_x44;
   /* :0x46 */ s16 todo_x46;
   /* :0x48 */ s16 todo_x48;
   /* :0x4A */ s16 todo_x4a;
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ s16 todo_x52;
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ s16 todo_x56;
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_168;

/* Evil Stream - FX1 */
typedef struct EvtData_169 {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[2];
   /* :0x2A */ s16 fade;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_169;

/* Piercing Ray, Plasma Wave - FX1 */
typedef struct EvtData_170 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_170;

/* Homing Ray */
typedef struct EvtData_171 {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ u8 unk_0x38[40];
} EvtData_171;

/* Holy Pressure - Cube */
typedef struct EvtData_172 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 timer;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_172;

/* Fire Gem - Beam */
typedef struct EvtData_173 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ u8 unk_0x32[42];
   /* :0x5C */ struct EvtData *fx;
} EvtData_173;

/* Rainbow Storm - FX1 */
typedef struct EvtData_175 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ s16 iterator;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_175;

/* Rainbow Stroke - FX1 */
typedef struct EvtData_176 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ struct EvtData *cam;
   /* :0x5C */ struct EvtData *fx;
} EvtData_176;

/* Holy Pressure - FX1 */
typedef struct EvtData_177 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ s16 iterator;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_177;

/* Thunder Flash - FX1 */
typedef struct EvtData_178 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 fade;
   /* :0x2E */ u8 unk_0x2E[8];
   /* :0x36 */ s16 timer;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ u8 unk_0x3A[36];
   /* :0x5E */ s16 todo_x5e;
} EvtData_178;

/* Thunder Flash - Ray */
typedef struct EvtData_179 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ u8 unk_0x34[2];
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ u8 unk_0x38[36];
   /* :0x5C */ struct EvtData *parent;
} EvtData_179;

/* Spread Force - FX1 */
typedef struct EvtData_180 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 fade;
   /* :0x2E */ u8 unk_0x2E[8];
   /* :0x36 */ s16 timer;
   /* :0x38 */ u8 unk_0x38[32];
   /* :0x58 */ s16 iterator;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *dataStore;
} EvtData_180;

/* FX - TBD */
typedef struct EvtData_182 {
   /* :0x24 */ s16 savedZoom;
   /* :0x26 */ s16 theta;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *cam; // evtf026
} EvtData_182;

/* FX - TBD */
typedef struct EvtData_183 {
   /* :0x24 */ u8 unk_0x24[2];
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[4];
   /* :0x36 */ s16 timer;
   /* :0x38 */ u8 unk_0x38[36];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_183;

/* Avalanche - FX1 */
typedef struct EvtData_184 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[2];
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 dx;
   /* :0x32 */ s16 dz;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 todo_x3a;
   /* :0x3C */ u8 unk_0x3C[4];
   /* :0x40 */ struct EvtData *cam;      // evtf026
   /* :0x44 */ struct EvtData *boulder;  // evtf211
   /* :0x48 */ struct EvtData *rocks[6]; // evtf185
} EvtData_184;

/* Avalanche - Rock */
typedef struct EvtData_185 {
   /* :0x24 */ s16 height;
   /* :0x26 */ s16 theta;
   /* :0x28 */ s16 velocity;
   /* :0x2A */ u8 unk_0x2A[46];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_185;

/* FX - TBD */
typedef struct EvtData_186 {
   /* :0x24 */ u8 unk_0x24[2];
   /* :0x26 */ s16 fxType;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_186;

/* Dark Fire - Ray */
typedef struct EvtData_188 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ s16 todo_x32;
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ u8 unk_0x38[36];
   /* :0x5C */ struct EvtData *target;
} EvtData_188;

/* Ice Storm - FX1 */
typedef struct EvtData_189 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[46];
   /* :0x5C */ s16 todo_x5c;
   /* :0x5E */ u8 unk_0x5E[2];
} EvtData_189;

/* Magic Arrow - Arrow */
typedef struct EvtData_191 {
   /* :0x24 */ s16 clut_unused;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 timer;
   /* :0x36 */ u8 unk_0x36[42];
} EvtData_191;

/* Perfect Guard - FX2 */
typedef struct EvtData_192 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ s16 todo_x5c;
   /* :0x5E */ s16 todo_x5e;
} EvtData_192;

/* Dark Star - FX3 */
typedef struct EvtData_193 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[40];
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[8];
   /* :0x5A */ s16 endingFxType;
   /* :0x5C */ struct EvtData *targetSprite;
} EvtData_193;

/* Rolling Thunder - FX2 */
typedef struct EvtData_195 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ s16 endingFxType;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_195;

/* Rolling Thunder - Orb Pair */
typedef struct EvtData_196 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[6];
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ u8 unk_0x36[22];
   /* :0x4C */ s16 clut;
   /* :0x4E */ u8 unk_0x4E[6];
   /* :0x54 */ struct EvtData *lightningSprite;
   /* :0x58 */ struct EvtData *orbSprite;
   /* :0x5C */ struct EvtData *targetSprite;
} EvtData_196;

/* Rolling Thunder - FX1 */
typedef struct EvtData_197 {
   /* :0x24 */ union {
      struct EvtData *unitSpriteParam;
      s16 timer;
   } variant_0x24;
   /* :0x28 */ u8 unk_0x28[16];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ struct EvtData *bolts[9];
} EvtData_197;

/* Rolling Thunder - Casting Bolt */
typedef struct EvtData_198 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ u8 unk_0x30[40];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_198;

/* Magic Arrow - FX1 */
typedef struct EvtData_199 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_199;

/* Unit Struck */
typedef struct EvtData_201 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 flashState;
   /* :0x30 */ u8 unk_0x30[40];
   /* :0x58 */ struct EvtData *impactSprite;
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_201;

/* Unit Blocking */
typedef struct EvtData_202_746 {
   /* :0x24 */ union {
      struct EvtData *entitySpriteParam;
      s16 timer;
   } variant_0x24;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 flashState;
   /* :0x30 */ u8 unk_0x30[40];
   /* :0x58 */ struct EvtData *unusedSprite;
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_202_746;

/* Blocking Impact */
typedef struct EvtData_203 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[34];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[6];
   /* :0x58 */ s16 direction;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_203;

/* Summon Crest */
typedef struct EvtData_204 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 maskClut;
   /* :0x4E */ s16 clut;
   /* :0x50 */ s16 vramSrcX;
   /* :0x52 */ s16 vramSrcY;
   /* :0x54 */ u8 unk_0x54[8];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_204;

/* Blood Spurt */
typedef struct EvtData_205 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 flashState;
   /* :0x30 */ u8 unk_0x30[44];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_205;

/* Blood Spurt Particle */
typedef struct EvtData_206 {
   /* :0x24 */ SVectorXZY position1;
   /* :0x2A */ s16 position7_x;
   /* :0x2C */ SVectorXZY position2;
   /* :0x32 */ s16 position7_z;
   /* :0x34 */ SVectorXZY position3;
   /* :0x3A */ s16 position7_y;
   /* :0x3C */ SVectorXZY position4;
   /* :0x42 */ s16 position8_x;
   /* :0x44 */ SVectorXZY position5;
   /* :0x4A */ s16 position8_z;
   /* :0x4C */ SVectorXZY position6;
   /* :0x52 */ s16 position8_y;
   /* :0x54 */ u8 unk_0x54[4];
   /* :0x58 */ s16 theta;
   /* :0x5A */ s16 shade;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_206;

/* Holy Lightning - FX1 */
typedef struct EvtData_208 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ struct EvtData *bolts[9];
} EvtData_208;

/* Avalanche - Boulder */
typedef struct EvtData_211 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 rotY;
   /* :0x50 */ s16 rotZ;
   /* :0x52 */ s16 rotX;
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_211;

/* Holy Lightning - Casting Bolt */
typedef struct EvtData_212 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ u8 unk_0x30[28];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ u8 unk_0x4E[10];
   /* :0x58 */ struct EvtData *sprite;
   /* :0x5C */ struct EvtData *parent;
} EvtData_212;

/* Dust Cloud Spawner */
typedef struct EvtData_213 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_213;

/* Dust Cloud */
typedef struct EvtData_214 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 halfSize;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ struct EvtData *sprite;
   /* :0x30 */ SVectorXZY positions[8];
} EvtData_214;

/* Cloud (Sand, Dust, etc.) */
typedef struct EvtData_215 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ SVectorXZY position1;
   /* :0x2E */ s16 position7_x;
   /* :0x30 */ SVectorXZY position2;
   /* :0x36 */ s16 position7_z;
   /* :0x38 */ SVectorXZY position3;
   /* :0x3E */ s16 position7_y;
   /* :0x40 */ SVectorXZY position4;
   /* :0x46 */ s16 position8_x;
   /* :0x48 */ SVectorXZY position5;
   /* :0x4E */ s16 position8_z;
   /* :0x50 */ SVectorXZY position6;
   /* :0x56 */ s16 position8_y;
   /* :0x58 */ s16 halfSize;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_215;

/* Explosion - FX2 */
typedef struct EvtData_220 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[50];
   /* :0x5A */ s16 endingFxType;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_220;

/* FX - TBD */
typedef struct EvtData_222 {
   /* :0x24 */ s16 savedZoom;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ s16 fade;
   /* :0x2E */ u8 unk_0x2E[42];
   /* :0x58 */ struct EvtData *fx;  // evtf223
   /* :0x5C */ struct EvtData *cam; // evtf026
} EvtData_222;

/* FX - TBD */
typedef struct EvtData_223 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[4];
   /* :0x36 */ s16 timer;
   /* :0x38 */ u8 unk_0x38[32];
   /* :0x58 */ struct EvtData *lightningSprite;
   /* :0x5C */ struct EvtData *orbSprite;
} EvtData_223;

/* Thunder Ball - FX1 */
typedef struct EvtData_224 {
   /* :0x24 */ s16 savedZoom;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_224;

/* Thunder Ball - Initial Orb */
typedef struct EvtData_225 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[4];
   /* :0x36 */ s16 timer;
   /* :0x38 */ u8 unk_0x38[32];
   /* :0x58 */ struct EvtData *lightningSprite;
   /* :0x5C */ struct EvtData *orbSprite;
} EvtData_225;

/* Thunder Ball - Child Orb */
typedef struct EvtData_226 {
   /* :0x24 */ u8 unk_0x24[2];
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[2];
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 dx;
   /* :0x2E */ s16 dz;
   /* :0x30 */ s16 dy;
   /* :0x32 */ u8 unk_0x32[4];
   /* :0x36 */ s16 timer;
   /* :0x38 */ u8 unk_0x38[32];
   /* :0x58 */ struct EvtData *lightningSprite;
   /* :0x5C */ struct EvtData *orbSprite;
} EvtData_226;

/* Stat Raised */
typedef struct EvtData_272 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[22];
   /* :0x4C */ s16 type;
   /* :0x4E */ u8 unk_0x4E[6];
   /* :0x54 */ s16 statGfxIdx;
   /* :0x56 */ s16 upGfxIdx;
   /* :0x58 */ s16 animRelativeOfs;
   /* :0x5A */ s16 animYOfs;
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_272;

/* Zoom - TBD */
typedef struct EvtData_277 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 dstZoom;
   /* :0x2A */ u8 unk_0x2A[14];
   /* :0x38 */ s16 smoothness;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_277;

/* Faint Sparkles */
typedef struct EvtData_278 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[46];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_278;

/* Ice Storm - Camera */
typedef struct EvtData_279 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ u8 unk_0x28[2];
   /* :0x2A */ s16 dstZoom;
   /* :0x2C */ s16 dstCamRotX;
   /* :0x2E */ s16 delayType;
   /* :0x30 */ u8 unk_0x30[8];
   /* :0x38 */ s16 dstCamRotY;
   /* :0x3A */ s16 timer;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_279;

/* Dark Hurricane - Target */
typedef struct EvtData_280 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[46];
   /* :0x54 */ struct EvtData *parent; // evtf389
   /* :0x58 */ struct EvtData *unitSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_280;

/* Dark Hurricane - FX2 / FX3 */
typedef struct EvtData_281_282 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ struct EvtData *targetSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_281_282;

/* Dark Hurricane - Cloud */
typedef struct EvtData_283 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ void *animData;
   /* :0x3C */ u8 unk_0x3C[32];
   /* :0x5C */ struct EvtData *parent; // evtf389
} EvtData_283;

/* FX - TBD */
typedef struct EvtData_284 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[6];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ void *animData;
   /* :0x3C */ Quad quad;
   /* :0x5C */ u8 unk_0x5C[4];
} EvtData_284;

/* FX - TBD */
typedef struct EvtData_287 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_287;

/* Map 13 - Bridge Explosion (Battle) */
typedef struct EvtData_288 {
   /* :0x24 */ s16 currentX;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ s16 dstRotY_unused;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ Camera camera;
   /* :0x3E */ u8 unk_0x3E[22];
   /* :0x54 */ s16 hitPlayerUnit;
   /* :0x56 */ u8 unk_0x56[6];
   /* :0x5C */ s16 iterator;
   /* :0x5E */ u8 unk_0x5E[2];
} EvtData_288;

/* Reveal Item */
typedef struct EvtData_290_294_761 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 theta;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 gfxIdx;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_290_294_761;

/* Blue Item Sparkles */
typedef struct EvtData_292 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_292;

/* Map 31 - Scene 61 - Xeno's Flames */
typedef struct EvtData_303 {
   /* :0x24 */ struct EvtData *entitySprite;
   /* :0x28 */ s16 theta;
   /* :0x2A */ u8 unk_0x2A[54];
} EvtData_303;

/* Flame */
typedef struct EvtData_304_661 {
   /* :0x24 */ u8 unk_0x24[13];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ u8 unk_0x32[6];
   /* :0x38 */ void *animData;
   /* :0x3C */ u8 unk_0x3C[28];
   /* :0x58 */ s16 radius;
   /* :0x5A */ s16 theta;
   /* :0x5C */ Quad *quadp;
} EvtData_304_661;

/* Healing - FX2 */
// TODO: Disambiguate from EvtData_100?
// 306: Extra Healing, Hyper Healing
// 791: Healing Plus
// 792: Ultra Healing
// 793: Supreme Healing, Holy H2O
typedef struct EvtData_306_Etc {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_306_Etc;

/* Evil Stream - FX2 / FX3 */
typedef struct EvtData_307_324 {
   /* :0x24 */ Cylinder cylinder;
   /* :0x48 */ u8 unk_0x48[16];
   /* :0x58 */ struct EvtData *targetSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_307_324;

/* Life Orb - Beam */
typedef struct EvtData_310 {
   /* :0x24 */ s16 height;
   /* :0x26 */ s16 unused_0x26;
   /* :0x28 */ s16 radius;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_310;

/* Bubble Swirl - Bubble */
typedef struct EvtData_312 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[6];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[6];
   /* :0x3C */ s16 theta;
   /* :0x3E */ s16 yVelocity;
   /* :0x40 */ s16 rVelocity;
   /* :0x42 */ s16 radius;
   /* :0x44 */ u8 unk_0x44[28];
} EvtData_312;

/* Life Orb - FX1 */
typedef struct EvtData_313 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *beam; // evtf310
} EvtData_313;

/* Inward Ray */
typedef struct EvtData_314 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[4];
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ s16 theta1;
   /* :0x36 */ s16 theta2;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ s16 unused_0x3A;
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ u8 unk_0x56[10];
} EvtData_314;

/* Vertical Ray */
typedef struct EvtData_315 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[4];
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ s16 theta;
   /* :0x36 */ s16 radius;
   /* :0x38 */ s16 top;
   /* :0x3A */ s16 height;
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s16 rotationSpeed;
   /* :0x56 */ s16 speed;
   /* :0x58 */ u8 unk_0x58[8];
} EvtData_315;

/* Avalanche - FX2 / FX3 */
typedef struct EvtData_317_338 {
   /* :0x24 */ s32 availableSlots;
   /* :0x28 */ s32 rubbleAmount;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_317_338;

/* FX - TBD */
typedef struct EvtData_321 {
   /* :0x24 */ s16 thetas[8];
   /* :0x34 */ s16 radius;
   /* :0x36 */ u8 unk_0x36[38];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_321;

/* Magic Restoration (Magic Charge, etc.) - FX2 */
typedef struct EvtData_322_Etc {
   /* :0x24 */ s16 amount;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_322_Etc;

/* FX - TBD */
typedef struct EvtData_323_713 {
   /* :0x24 */ struct EvtData *entitySprite;
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[7];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ u8 unk_0x32[10];
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ u8 unk_0x54[8];
   /* :0x5C */ struct EvtData *dataStore; // cylinder
} EvtData_323_713;

/* FX - TBD */
typedef struct EvtData_326 {
   /* :0x24 */ s16 height;
   /* :0x26 */ s16 speed;
   /* :0x28 */ s16 radius;
   /* :0x2A */ u8 unk_0x2A[46];
   /* :0x58 */ struct EvtData *dataStore;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_326;

/* Healing Circle - FX2 */
typedef struct EvtData_327 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *targetSprite;
} EvtData_327;

/* Magic Restoration (Magic Charge, etc.) - FX1 */
typedef struct EvtData_330 {
   /* :0x24 */ s16 theta1;
   /* :0x26 */ s16 theta2;
   /* :0x28 */ s16 startingTheta1;
   /* :0x2A */ s16 startingTheta2;
   /* :0x2C */ u8 unk_0x2C[4];
   /* :0x30 */ s16 ringSize;
   /* :0x32 */ u8 unk_0x32[34];
   /* :0x54 */ struct EvtData *dataStore;
   /* :0x58 */ struct EvtData *ringBottom;
   /* :0x5C */ struct EvtData *ringTop;
} EvtData_330;

/* FX - TBD */
typedef struct EvtData_331 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[38];
   /* :0x5C */ struct EvtData *parent; // (Maybe)
} EvtData_331;

/* Rolling Fire - FX1 */
typedef struct EvtData_332 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[14];
   /* :0x38 */ void *animData;
   /* :0x3C */ u8 unk_0x3C[32];
   /* :0x5C */ struct EvtData *dataStore; // cylinder
} EvtData_332;

/* Salamander - FX1 */
typedef struct EvtData_334 {
   /* :0x24 */ s32 todo_x24;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_334;

/* Salamander - Head / Segment */
typedef struct EvtData_335_336 {
   /* :0x24 */ s16 theta1;
   /* :0x26 */ s16 theta2;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ SVectorXYZ position1;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ SVectorXYZ position2;
   /* :0x3E */ u8 unk_0x3E[2];
   /* :0x40 */ SVectorXYZ position3;
   /* :0x46 */ u8 unk_0x46[2];
   /* :0x48 */ SVectorXYZ position4;
   /* :0x4E */ s16 unused_0x4E;
   /* :0x50 */ struct EvtData *parent; // evtf334 (fx1)
   /* :0x54 */ struct EvtData *sprite;
   /* :0x58 */ u8 unk_0x58[4];
   /* :0x5C */ struct EvtData *link; // evtf334/evtf335/evtf336
} EvtData_335_336;

/* Button Depress */
typedef struct EvtData_346 {
   /* :0x24 */ s16 dstCamRotY;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_346;

/* Map 26 */
typedef struct EvtData_347 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ Camera camera;
   /* :0x3A */ u8 unk_0x3A[34];
   /* :0x5C */ struct EvtData *buttonDepress; // evtf346
} EvtData_347;

/* FX - TBD */
typedef struct EvtData_348 {
   /* :0x24 */ s16 todo_x24[8];
   /* :0x34 */ s16 todo_x34[8];
   /* :0x44 */ s16 todo_x44[8];
   /* :0x54 */ u8 unk_0x54[8];
   /* :0x5C */ struct EvtData *dataStore; // cylinder
} EvtData_348;

/* MsgBox Text */
typedef struct EvtData_351 {
   /* :0x24 */ s16 maxCharsPerLine;
   /* :0x26 */ s16 lineSpacing;
   /* :0x28 */ s16 maxRows;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 textSpeedAccum;
   /* :0x2E */ s16 textSpeed;
   /* :0x30 */ s16 type;
   /* :0x32 */ s16 textPtrIdx;
   /* :0x34 */ s16 pregapChars;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ s16 readingFromStringTable;
   /* :0x3A */ s16 indentChars;
   /* :0x3C */ RECT rect;
   /* :0x44 */ s8 todo_x44;
   /* :0x45 */ s8 todo_x45;
   /* :0x46 */ u8 unk_0x46[2];
   /* :0x48 */ s16 todo_x48;
   /* :0x4A */ u8 unk_0x4A[10];
   /* :0x54 */ u8 *textResumePtr;
   /* :0x58 */ struct EvtData *buttonIcon;
   /* :0x5C */ u8 *textPtr;
} EvtData_351;

/* Map 29 */
typedef struct EvtData_352 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ Camera camera;
   /* :0x3A */ u8 unk_0x3A[34];
   /* :0x5C */ struct EvtData *buttonDepress; // evtf346
} EvtData_352;

/* Map 19 - Elevator (TBD) */
typedef struct EvtData_355_356 {
   /* :0x24 */ union {
      struct EvtData *elevator1Param;
      struct {
         s16 a;
         s16 b;
      } dstCamRotY;
   } variant_0x24;
   /* :0x28 */ struct EvtData *elevator2Param;
   /* :0x2C */ Camera camera;
   /* :0x3A */ u8 unk_0x3A[30];
   /* :0x58 */ struct EvtData *elevator2;
   /* :0x5C */ struct EvtData *elevator1;
} EvtData_355_356;

/* Map 19 */
typedef struct EvtData_357 {
   /* :0x24 */ u8 unk_0x24[52];
   /* :0x58 */ struct EvtData *elevator2;
   /* :0x5C */ struct EvtData *elevator1;
} EvtData_357;

/* Map 19 - Elevator (TBD) */
typedef struct EvtData_358 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ s16 todo_x2e;
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ s16 todo_x36;
   /* :0x38 */ s16 todo_x38;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_358;

/* Map 13 - Bridge Explosion (Scene) */
typedef struct EvtData_361 {
   /* :0x24 */ s16 currentX;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_361;

/* Drawbridge Button */
typedef struct EvtData_362 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 gfxSetIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 battleNum; // i.e. mapNum - 9 (same as displayed in debug menu)
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ struct MapTileModel *tileModel;
   /* :0x34 */ s16 dstCamPosY;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ s16 dstCamRotY;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_362;

/* Wyrmfang - FX1 */
typedef struct EvtData_363 {
   /* :0x24 */ u8 unk_0x24[52];
   /* :0x58 */ struct EvtData *ccwFlames;
   /* :0x5C */ struct EvtData *cwFlames;
} EvtData_363;

/* Map 17 - Floodgate */
typedef struct EvtData_365 {
   /* :0x24 */ s16 dstCamRotY;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ s16 face1Init;
   /* :0x2E */ s16 face2Init;
   /* :0x30 */ s16 face3Init;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 face1Curr;
   /* :0x36 */ s16 face2Curr;
   /* :0x38 */ s16 face3Curr;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_365;

/* Map 17 - Button */
typedef struct EvtData_366 {
   /* :0x24 */ s16 unused_0x24;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ Camera camera;
   /* :0x3A */ u8 unk_0x3A[34];
   /* :0x5C */ struct EvtData *buttonDepress;
} EvtData_366;

/* Screen Effect (Incomplete) */
typedef struct EvtData_369 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 speed;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 intensity;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 unused_0x30;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 otOfs;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ CVECTOR color;
   /* :0x3C */ s16 rmax;
   /* :0x3E */ s16 gmax;
   /* :0x40 */ s16 bmax;
   /* :0x42 */ u8 unk_0x42[2];
   /* :0x44 */ s16 rd;
   /* :0x46 */ s16 gd;
   /* :0x48 */ s16 bd;
   /* :0x4A */ u8 unk_0x4A[2];
   /* :0x4C */ s16 semiTrans;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_369;

/* FX - TBD */
typedef struct EvtData_377 {
   /* :0x24 */ s16 theta1;
   /* :0x26 */ s16 theta2;
   /* :0x28 */ s16 theta3;
   /* :0x2A */ s16 theta4;
   /* :0x2C */ s16 radius;
   /* :0x2E */ s16 theta5;
   /* :0x30 */ u8 unk_0x30[36];
   /* :0x54 */ struct EvtData *sprite;
   /* :0x58 */ u8 unk_0x58[4];
   /* :0x5C */ struct EvtData *todo_x5c;
} EvtData_377;

/* Phase Shift - FX1 */
typedef struct EvtData_378 {
   /* :0x24 */ SVectorXY positions[8];
   /* :0x44 */ u8 unk_0x44[20];
   /* :0x58 */ struct EvtData *runeSprite;
   /* :0x5C */ struct EvtData *crestSprite;
} EvtData_378;

/* Evil Stream - Rock */
typedef struct EvtData_379 {
   /* :0x24 */ u8 unk_0x24[16];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ void *animData;
   /* :0x3C */ s16 maxHeight;
   /* :0x3E */ u8 unk_0x3E[34];
} EvtData_379;

/* Level Up - FX */
typedef struct EvtData_380 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[50];
   /* :0x58 */ struct EvtData *unitSprite;
   /* :0x5C */ struct EvtData *sprite;
} EvtData_380;

/* Flame Breath */
typedef struct EvtData_382 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ s16 direction;
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[6];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[6];
   /* :0x3C */ s16 theta;
   /* :0x3E */ s16 yTheta;
   /* :0x40 */ s16 unused_0x40;
   /* :0x42 */ u8 unk_0x42[30];
} EvtData_382;

/* Healing Sparkle */
// TODO: Disambiguate from EvtData_101?
typedef struct EvtData_386 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[10];
   /* :0x38 */ void *animData;
   /* :0x3C */ s16 timer;
   /* :0x3E */ u8 unk_0x3E[34];
} EvtData_386;

/* Dark Hurricane - FX1 */
typedef struct EvtData_388 {
   /* :0x24 */ u8 unk_0x24[44];
   /* :0x50 */ struct EvtData *vortex; // evtf389
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_388;

/* Dark Hurricane - Vortex */
typedef struct EvtData_389 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ s16 radius;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 theta;
   /* :0x32 */ u8 unk_0x32[34];
   /* :0x54 */ struct EvtData *sprite;
   /* :0x58 */ struct EvtData *parent; // evtf388
   /* :0x5C */ u8 unk_0x5C[4];
} EvtData_389;

/* Dark Hurricane - Vortex Layer */
typedef struct EvtData_390 {
   /* :0x24 */ s16 radius2;
   /* :0x26 */ s16 theta2;
   /* :0x28 */ s16 height;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 radius;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 theta;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 maxRadius;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ s16 unused_0x38;
   /* :0x3A */ u8 unk_0x3A[26];
   /* :0x54 */ struct EvtData *sprite;
   /* :0x58 */ struct EvtData *parent; // evtf389/evtf390
   /* :0x5C */ u8 unk_0x5C[4];
} EvtData_390;

/* FX - TBD */
typedef struct EvtData_392 {
   /* :0x24 */ struct EvtData *todo_x24;
   /* :0x28 */ struct EvtData *todo_x28;
   /* :0x2C */ struct EvtData *todo_x2c;
   /* :0x30 */ struct EvtData *todo_x30;
   /* :0x34 */ u8 unk_0x34[40];
   /* :0x5C */ struct EvtData *todo_x5c;
} EvtData_392;

/* Dynamo Hum - FX1 */
typedef struct EvtData_394 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *orb; // evtf395
} EvtData_394;

/* Dynamo Hum - Electric Orb */
typedef struct EvtData_395 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s8 animFinished;
   /* :0x31 */ u8 unk_0x31[3];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ void *animData;
   /* :0x3C */ s16 theta;
   /* :0x3E */ s16 size;
   /* :0x40 */ u8 unk_0x40[28];
   /* :0x5C */ struct EvtData *parent; // evtf394
} EvtData_395;

/* Dynamo Hum - Orb Electricity */
typedef struct EvtData_396 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[10];
   /* :0x38 */ void *animData;
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s32 length;
   /* :0x58 */ struct EvtData *links[2];
} EvtData_396;

/* Map 11 */
typedef struct EvtData_399 {
   /* :0x24 */ s16 dstRotY;
   /* :0x26 */ u8 unk_0x26[6];
   /* :0x2C */ Camera camera;
   /* :0x3A */ u8 unk_0x3A[38];
} EvtData_399;

/* AI - TBD */
typedef struct EvtData_400 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s16 resumeZ;
   /* :0x2A */ s16 resumeX;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_400;

/* AI - TBD */
typedef struct EvtData_401 {
   /* :0x24 */ s8 team;
   /* :0x25 */ s8 unitIdx;
   /* :0x26 */ s16 unitIter;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_401;

/* AI - TBD */
typedef struct EvtData_402_Etc {
   /* :0x24 */ s16 resumeZ;
   /* :0x26 */ s16 resumeX;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_402_Etc;

/* Panorama */
typedef struct EvtData_405 {
   /* :0x24 */ s16 yRot;
   /* :0x26 */ s16 xRot;
   /* :0x28 */ s16 xOffset;
   /* :0x2A */ s16 yOffset;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_405;

/* Event Entity */
typedef struct EvtData_409 {
   /* :0x24 */ u8 unk_0x24[3];
   /* :0x27 */ s8 animIdx;
   /* :0x28 */ u8 unk_0x28[8];
   /* :0x30 */ struct EvtData *sprite;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ u8 **baseAnimSet;
   /* :0x40 */ s8 commandState;
   /* :0x41 */ u8 unk_0x41;
   /* :0x42 */ s16 timer;
   /* :0x44 */ s16 *pNextCommand;
   /* :0x48 */ s16 stripIdxA;
   /* :0x4A */ s16 stripIdxB;
   /* :0x4C */ s8 maintainDirection;
   /* :0x4D */ s8 usingAltAnimSet;
   /* :0x4E */ s8 elevationType; // TBD
   /* :0x4F */ u8 unk_0x4F;
   /* :0x50 */ u8 **altAnimSet;
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_409;

/* Camera - Event Zoom */
typedef struct EvtData_410 {
   /* :0x24 */ s16 zoom;
   /* :0x26 */ s16 timer;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_410;

/* MsgBox Portrait */
typedef struct EvtData_413 {
   /* :0x24 */ union {
      s16 id;
      s8 idx;
   } portrait;
   /* :0x26 */ s8 flipped;
   /* :0x27 */ u8 unk_0x27;
   /* :0x28 */ s8 speakTimer;
   /* :0x29 */ s8 blinkTimer;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ struct EvtData *faceSprite;
   /* :0x30 */ struct EvtData *speakSprite;
   /* :0x34 */ struct EvtData *blinkSprite;
   /* :0x38 */ struct EvtData *anchor;
   /* :0x3C */ s16 anchorOfsX;
   /* :0x3E */ s16 anchorOfsY;
   /* :0x40 */ u8 unk_0x40[32];
} EvtData_413;

/* Battle Victory/Defeat */
typedef struct EvtData_420_423 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_420_423;

/* MsgBox Tail */
typedef struct EvtData_421_422 {
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s16 left;
   /* :0x2A */ s16 top;
   /* :0x2C */ s16 right;
   /* :0x2E */ s16 bottom;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_421_422;

/* Battle Ender */
typedef struct EvtData_424 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_424;

/* Battle - Options */
typedef struct EvtData_425 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_425;

/* Evaluate Battle 08 */
typedef struct EvtData_438 {
   /* :0x24 */ s8 delay;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_438;

/* Battle - Victory/Defeat Particle */
typedef struct EvtData_446 {
   /* :0x24 */ s8 hidden;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[3];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ SVectorXY coords[4];
   /* :0x4C */ s16 todo_x4c;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ s16 todo_x52;
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ s16 todo_x56;
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ s16 todo_x5c;
   /* :0x5E */ s16 delay;
} EvtData_446;

/* Unit Portrait (in depot, dojo, etc.) */
typedef struct EvtData_447 {
   /* :0x24 */ s8 windowId;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ struct EvtData *window;
   /* :0x2C */ struct EvtData *sprite;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_447;

/* Map Object - Rippling Water/Lava (animated texture) */
typedef struct EvtData_564_565_566 {
   /* :0x24 */ u16 phase;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_564_565_566;

/* Opening Chest */
typedef struct EvtData_567 {
   /* :0x24 */ struct UnitStatus *opener;
   /* :0x28 */ u8 sendToDepot;
   /* :0x29 */ u8 unk_0x29[3];
   /* :0x2C */ struct EvtData *openerSprite;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_567;

/* Level Up - Camera Control, Sound */
typedef struct EvtData_571 {
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s16 camSavedX;
   /* :0x2A */ s16 camSavedZ;
   /* :0x2C */ s16 camSavedY;
   /* :0x2E */ s16 camSavedRotX;
   /* :0x30 */ s16 camSavedRotZ;
   /* :0x32 */ s16 camSavedRotY;
   /* :0x34 */ s16 camSavedZoom;
   /* :0x36 */ s16 dstZoom;
   /* :0x38 */ s16 dstRotY;
   /* :0x3A */ s16 timer;
   /* :0x3C */ s16 savedGeomOfsY;
   /* :0x3E */ s16 geomOfsY;
   /* :0x40 */ s16 dstGeomOfsY;
   /* :0x42 */ u8 unk_0x42[30];
} EvtData_571;

/* Battle Items List */
typedef struct EvtData_573 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 item;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_573;

/* Status Portrait */
typedef struct EvtData_575 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[4];
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ s16 portraitId;
   /* :0x3E */ u8 unk_0x3E[34];
} EvtData_575;

/* Tavern */
typedef struct EvtData_576 {
   /* :0x24 */ s8 needSpeak[4];
   /* :0x28 */ s8 textPtrIdx;
   /* :0x29 */ u8 unk_0x29[35];
   /* :0x4C */ s8 timer;
   /* :0x4D */ u8 unk_0x4D[19];
} EvtData_576;

/* Audio Command */
typedef struct EvtData_581 {
   /* :0x24 */ s16 cmd;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s8 delay;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_581;

/* Battle - Player Event */
typedef struct EvtData_585 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ s8 timer;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_585;

/* Battle - MsgBox */
typedef struct EvtData_586 {
   /* :0x24 */ s16 nameIdx;
   /* :0x26 */ s16 portrait;
   /* :0x28 */ s16 textIdx;
   /* :0x2A */ s16 omitTail;
   /* :0x2C */ u8 unk_0x2C[48];
   /* :0x5C */ s8 timer;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_586;

/* Battle - Enemy Event */
typedef struct EvtData_587 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s8 xenoSpawnZ;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s8 spawnZ;
   /* :0x2B */ s8 spawnX;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_587;

/* Map Object - Boulder */
typedef struct EvtData_591 {
   /* :0x24 */ s8 hidden;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[22];
   /* :0x4C */ s16 dstCamRotY;
   /* :0x4E */ s16 savedCamRotY;
   /* :0x50 */ s8 timer;
   /* :0x51 */ u8 unk_0x51[3];
   /* :0x54 */ s8 gridZ;
   /* :0x55 */ s8 gridX;
   /* :0x56 */ u8 unk_0x56[10];
} EvtData_591;

/* Battle - Turn Start */
typedef struct EvtData_592 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ s8 team;
   /* :0x26 */ s8 targets;
   /* :0x27 */ u8 unk_0x27[57];
} EvtData_592;

/* Battle - Results - Unit (Reward or Penalty) */
typedef struct EvtData_593 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[4];
   /* :0x32 */ s16 otOfs;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ u8 unitId;
   /* :0x3D */ s8 slot;
   /* :0x3E */ s8 isPenalty;
   /* :0x3F */ u8 unk_0x3F;
   /* :0x40 */ s16 unitGfxIdx;
   /* :0x42 */ s16 unitClut;
   /* :0x44 */ u8 unk_0x44[28];
} EvtData_593;

/* Status Window Manager */
typedef struct EvtData_595 {
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 multiple;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_595;

/* Battle - Intro */
typedef struct EvtData_597 {
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_597;

/* Map 32 - Car Release */
typedef struct EvtData_650 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 startX;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 carIdx;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 endX;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[10];
   /* :0x4C */ s16 tileX;
   /* :0x4E */ s16 tileZ;
   /* :0x50 */ s16 timer;
   /* :0x52 */ u8 unk_0x52[10];
   /* :0x5C */ s16 hasCasualties;
   /* :0x5E */ u8 unk_0x5E[2];
} EvtData_650;

/* Map 33 - Lava Pit Platform */
typedef struct EvtData_651 {
   /* :0x24 */ s16 step;
   /* :0x26 */ u8 unk_0x26[14];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[30];
} EvtData_651;

/* Map 35 - Button */
typedef struct EvtData_652 {
   /* :0x24 */ u8 unk_0x24[16];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[10];
   /* :0x4C */ s16 tileX;
   /* :0x4E */ s16 tileZ;
   /* :0x50 */ u8 unk_0x50[4];
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ u8 unk_0x56[2];
   /* :0x58 */ s16 hitPlayerUnit;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ struct EvtData *buttonDepress;
} EvtData_652;

/* Exploding Tile */
typedef struct EvtData_653 {
   /* :0x24 */ SVECTOR translation;
   /* :0x2C */ SVECTOR rotation;
   /* :0x34 */ u8 unk_0x34[8];
   /* :0x3C */ SVECTOR savedTranslation;
   /* :0x44 */ u8 unk_0x44[16];
   /* :0x54 */ s16 riverVelocity;
   /* :0x56 */ u8 unk_0x56[2];
   /* :0x58 */ struct MapTileModel *tileModel;
   /* :0x5C */ struct MapTileModel *tileModel_unused;
} EvtData_653;

/* Map 38 - Wash Away Unit */
typedef struct EvtData_654 {
   /* :0x24 */ s16 tileX;
   /* :0x26 */ s16 tileZ;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_654;

/* Map 38 - Raise Floodgate */
typedef struct EvtData_655 {
   /* :0x24 */ s16 waterFaceY;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_655;

/* Map 39 */
typedef struct EvtData_656 {
   /* :0x24 */ u8 unk_0x24[16];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[30];
} EvtData_656;

/* Map 38 - Floodgate */
typedef struct EvtData_657 {
   /* :0x24 */ u8 unk_0x24[16];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[22];
   /* :0x58 */ struct EvtData *casualty;
   /* :0x5C */ struct EvtData *child; // button or water
} EvtData_657;

/* Map 38 - Floodwater */
typedef struct EvtData_658 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 theta;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 timer;
   /* :0x2E */ u8 unk_0x2E[2];
   /* :0x30 */ s16 todo_x30;
   /* :0x32 */ u8 unk_0x32[46];
} EvtData_658;

/* Map 28 - Button */
typedef struct EvtData_663 {
   /* :0x24 */ u8 unk_0x24[12];
   /* :0x30 */ Camera camera;
   /* :0x3E */ u8 unk_0x3E[30];
   /* :0x5C */ struct EvtData *child; // button or door
} EvtData_663;

/* Map 27 - Buttons */
typedef struct EvtData_665 {
   /* :0x24 */ s16 buttonPressed[4];
   /* :0x2C */ u8 unk_0x2C[4];
   /* :0x30 */ Camera camera;
   /* :0x3E */ u8 unk_0x3E[30];
   /* :0x5C */ struct EvtData *child; // button or door
} EvtData_665;

/* Map 14 - Lower Sand Mound */
typedef struct EvtData_666 {
   /* :0x24 */ s16 count;
   /* :0x26 */ s16 offset;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_666;

/* Map 14 - Raise Sand Mound */
typedef struct EvtData_668 {
   /* :0x24 */ s16 count;
   /* :0x26 */ s16 offset;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_668;

/* Map 14 - Sand */
typedef struct EvtData_670 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *mound; // evtf666/evtf668
} EvtData_670;

/* Map 39 - Splashing Tile */
typedef struct EvtData_672 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct MapTileModel *tileModel;
} EvtData_672;

/* Map 32 - Scene 63 - Cinematic */
typedef struct EvtData_673 {
   /* :0x24 */ Camera camera;
   /* :0x32 */ u8 unk_0x32[10];
   /* :0x3C */ s16 unused_0x3C;
   /* :0x3E */ u8 unk_0x3E[30];
   /* :0x5C */ struct EvtData *focus;
} EvtData_673;

/* Leena's Forcefield */
typedef struct EvtData_675 {
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[3];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[6];
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_675;

/* Stat Buff FX */
typedef struct EvtData_681 {
   /* :0x24 */ struct EvtData *unitSprite;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[34];
   /* :0x4C */ s16 clut;
   /* :0x4E */ s16 fade;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_681;

/* Campfire */
typedef struct EvtData_692 {
   /* :0x24 */ struct EvtData *entitySprite;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_692;

/* Spellbind - FX2 / FX3 */
typedef struct EvtData_715_to_718 {
   /* :0x24 */ u8 unk_0x24[2];
   /* :0x26 */ s16 unused_0x26;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_715_to_718;

/* Stat Buff Icon */
typedef struct EvtData_733 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[34];
   /* :0x4C */ s16 type;
   /* :0x4E */ s16 clut;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[10];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_733;

/* Wyrmfang - Flames */
typedef struct EvtData_747_748 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ s16 radius;
   /* :0x28 */ s16 amplitude;
   /* :0x2A */ s16 timer;
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_747_748;

/* Wyrmfang - Flame */
typedef struct EvtData_749 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ s16 radius;
   /* :0x28 */ s16 yTheta;
   /* :0x2A */ s16 amplitude;
   /* :0x2C */ u8 unk_0x2C[32];
   /* :0x4C */ struct EvtData *sprite;
   /* :0x50 */ u8 unk_0x50[16];
} EvtData_749;

/* Map 33 - Lower Platform */
typedef struct EvtData_750_751 {
   /* :0x24 */ union {
      struct EvtData *entitySpriteParam;
      s16 unk;
   } variant_0x24;
   /* :0x28 */ s16 ydrop;
   /* :0x2A */ u8 unk_0x2A[54];
} EvtData_750_751;

/* Map 39 - Scene 82 */
typedef struct EvtData_754 {
   /* :0x24 */ u8 unk_0x24[16];
   /* :0x34 */ Camera camera;
   /* :0x42 */ u8 unk_0x42[26];
   /* :0x5C */ struct EvtData *focus;
} EvtData_754;

/* Elite Melee Sparkles */
typedef struct EvtData_760 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_760;

/* Event Fade */
typedef struct EvtData_795 {
   /* :0x24 */ s16 fade;
   /* :0x26 */ u8 unk_0x26[14];
   /* :0x34 */ s16 delta;
   /* :0x36 */ u8 unk_0x36[14];
   /* :0x44 */ s16 max;
   /* :0x46 */ u8 unk_0x46[26];
} EvtData_795;

/* Map Object - Generic */
typedef struct EvtData_MapObject {
   /* :0x24 */ s16 param;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[2];
   /* :0x38 */ void *animData;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_MapObject;

/* Entity Spawn - Generic */
// When an event entity spawns an object, it passes it the entity's sprite
typedef struct EvtData_EntitySpawn {
   /* :0x24 */ struct EvtData *entitySpriteParam;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_EntitySpawn;

typedef struct EvtData_Unk_8006183c {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 scale;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_Unk_8006183c;

typedef struct EvtData_Unk_80080924 {
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ SVectorXZY position1;
   /* :0x2E */ s16 position7_x;
   /* :0x30 */ SVectorXZY position2;
   /* :0x36 */ s16 position7_z;
   /* :0x38 */ SVectorXZY position3;
   /* :0x3E */ s16 position7_y;
   /* :0x40 */ SVectorXZY position4;
   /* :0x46 */ s16 position8_x;
   /* :0x48 */ SVectorXZY position5;
   /* :0x4E */ s16 position8_z;
   /* :0x50 */ SVectorXZY position6;
   /* :0x56 */ s16 position8_y;
   /* :0x58 */ s16 halfSize;
   /* :0x5A */ u8 unk_0x5A[2];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_Unk_80080924;

typedef struct EvtData_Unk_80087b58 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *dataStore; // cylinder
} EvtData_Unk_80087b58;

typedef struct EvtData_Unk_8008a364 {
   /* :0x24 */ u8 unk_0x24[56];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_Unk_8008a364;

typedef struct EvtData_Unk_8008d1f0 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 gfxIdx;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 clut;
   /* :0x2E */ u8 unk_0x2E[3];
   /* :0x31 */ s8 semiTrans;
   /* :0x32 */ u8 unk_0x32[2];
   /* :0x34 */ s16 boxIdx;
   /* :0x36 */ u8 unk_0x36[6];
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ u8 unk_0x54[8];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_Unk_8008d1f0;

typedef struct EvtData_Unk_UsedBy392 {
   /* :0x24 */ s32 todo_x24;
   /* :0x28 */ s32 todo_x28;
   /* :0x2C */ s16 theta;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_Unk_UsedBy392;

typedef struct EvtData {
   /* 0x00 */ SVECTOR vec;
   /* 0x08 */ s16 functionIndex;
   /* 0x0A */ s16 mem;
   /* 0x0C */ s16 state;
   /* 0x0E */ s16 state2;
   /* 0x10 */ s16 state3;
   /* 0x12 */ CoordinateValue x1;
   /* 0x14 */ CoordinateValue y1;
   /* 0x16 */ CoordinateValue z1;
   /* 0x18 */ CoordinateValue x2;
   /* 0x1A */ CoordinateValue y2;
   /* 0x1C */ CoordinateValue z2;
   /* 0x1E */ CoordinateValue x3;
   /* 0x20 */ CoordinateValue y3;
   /* 0x22 */ CoordinateValue z3;
   /* 0x24: */
   union {
      s8 bytes[60];
      EvtData_Sprite sprite;
      EvtData_Sprite2 sprite2;
      EvtData_DataStore dataStore;
      EvtData_003 evtf003;         /* Battle - Actions */
      EvtData_004_005_408 evtf004; /* Window */
      EvtData_007 evtf007;         /* Apply Poison */
      EvtData_008 evtf008;         /* Battle Portrait */
      EvtData_013 evtf013;         /* Battle Manager */
      EvtData_014 evtf014;         /* Battle Unit */
      EvtData_016 evtf016;         /* Choose Done Direction */
      EvtData_017 evtf017;         /* Camera - TBD */
      EvtData_020 evtf020;         /* Pushed Boulder */
      EvtData_021 evtf021;         /* Unit Attacking */
      EvtData_022_029 evtf022;     /* Projectile */
      EvtData_023 evtf023;         /* Camera - Ranged Target */
      EvtData_024 evtf024;         /* Camera - Bounce Zoom */
      EvtData_025 evtf025;         /* Overhead Map View */
      EvtData_026_588 evtf026;     /* Camera - TBD */
      EvtData_027 evtf027;         /* Targeting Spell */
      EvtData_028 evtf028;         /* Unit Casting Spell */
      EvtData_030 evtf030;         /* Unit/Field Info */
      EvtData_031 evtf031;         /* Battle Spells List */
      EvtData_032_033 evtf032;     /* Display Damage */
      EvtData_MapObject evtf035;   /* Map Object - Tree */
      EvtData_MapObject evtf036;   /* Map Object - Grave Marker */
      EvtData_MapObject evtf037;   /* Map Object - Fountain */
      EvtData_MapObject evtf038;   /* Map Object - Lamp Post */
      EvtData_MapObject evtf039;   /* Map Object - Flag */
      EvtData_040 evtf040;         /* Map Object - Chest */
      EvtData_046 evtf046;         /* Map Object - Crate */
      EvtData_048 evtf048;         /* Push */
      EvtData_050 evtf050;         /* Unit Sprites Decoder */
      EvtData_051 evtf051;         /* Floating Damage Text */
      EvtData_052 evtf052;         /* Attack Info Marker */
      EvtData_062 evtf062;         /* Stretch Warp Sprite */
      EvtData_070 evtf070;         /* Fade From Black */
      EvtData_071 evtf071;         /* Fade To Black */
      EvtData_072 evtf072;         /* Fade From White */
      EvtData_073 evtf073;         /* Fade To White */
      EvtData_074 evtf074;         /* Fade In Sprite */
      EvtData_075 evtf075;         /* Fade Out Sprite */
      EvtData_076 evtf076;         /* FX - Circles (TBD) */
      EvtData_077 evtf077;         /* FX - Circle (TBD) */
      EvtData_078 evtf078;         /* Damage - FX2 */
      EvtData_079 evtf079;         /* Slay - FX3 */
      EvtData_080 evtf080;         /* Roman Fire - FX1 */
      EvtData_082 evtf082;         /* FX - TBD */
      EvtData_087 evtf087;         /* Map 20 - Scene 30 - Arrow Spawner */
      EvtData_089 evtf089;         /* Map 15 - Scene 17 - Cinematic */
      EvtData_090 evtf090;         /* Dagger Storm - FX2 */
      EvtData_091 evtf091;         /* Dagger Storm - Dagger */
      EvtData_094 evtf094;         /* Mood Ring - FX1 */
      EvtData_095 evtf095;         /* Mood Ring - Ring */
      EvtData_096 evtf096;         /* Mood Ring - FX2 */
      EvtData_097 evtf097;         /* Mood Ring - FX3 */
      EvtData_098 evtf098;         /* Map 20 - Scene 30 - Arrow */
      EvtData_100 evtf100;         /* Healing - FX2 */
      EvtData_101 evtf101;         /* Healing Sparkle */
      EvtData_102_227 evtf102;     /* Poison - FX2 */
      EvtData_103 evtf103;         /* Poison Bubbles */
      EvtData_104 evtf104;         /* Cure - FX2 */
      EvtData_106 evtf106;         /* Magic Charge - FX3 */
      EvtData_107 evtf107;         /* Magic Charge - Glyph Ring */
      EvtData_108 evtf108;         /* Harmful Wave - FX2 */
      EvtData_109 evtf109;         /* Harmful Wave - Ring */
      EvtData_110 evtf110;         /* Casting Stat Buff */
      EvtData_115 evtf115;         /* Faerie - FX2 */
      EvtData_116 evtf116;         /* Faerie - Sparkle */
      EvtData_117 evtf117;         /* Faerie - Sparkle Trail */
      EvtData_118 evtf118;         /* Faerie - Target */
      EvtData_119 evtf119;         /* FX - TBD */
      EvtData_122 evtf122;         /* Dark Fire - FX1 */
      EvtData_149 evtf149;         /* Flashing Unit Sprite */
      EvtData_130 evtf130;         /* FX - TBD */
      EvtData_131 evtf131;         /* Slay Unit */
      EvtData_132_Etc evtf132;     /* FX - TBD */
      EvtData_133_Etc evtf133;     /* FX - TBD */
      EvtData_132_Etc evtf134;     /* FX - TBD */
      EvtData_132_Etc evtf136;     /* FX - TBD */
      EvtData_133_Etc evtf137;     /* FX - TBD */
      EvtData_132_Etc evtf140;     /* FX - TBD */
      EvtData_133_Etc evtf141;     /* FX - TBD */
      EvtData_144 evtf144;         /* Holy Lightning - FX2 */
      EvtData_145 evtf145;         /* Holy Lightning - Electric Orb */
      EvtData_147 evtf147;         /* FX - TBD */
      EvtData_148 evtf148;         /* FX - TBD */
      EvtData_155 evtf155;         /* Spellbind - Glyph */
      EvtData_156 evtf156;         /* Delta Mirage - FX1 */
      EvtData_157 evtf157;         /* Delta Mirage - Ray */
      EvtData_158 evtf158;         /* Explosion - FX1 */
      EvtData_159 evtf159;         /* Explosion - Rays */
      EvtData_160 evtf160;         /* Ice Storm - Target */
      EvtData_161 evtf161;         /* Piercing Light - FX1 */
      EvtData_162 evtf162;         /* Ice Storm - Splash */
      EvtData_163 evtf163;         /* Stone Shower - FX1 */
      EvtData_164 evtf164;         /* Stone Shower - Rock */
      EvtData_165 evtf165;         /* Stone Shower - FX2 */
      EvtData_167 evtf167;         /* Rainbow Swirl */
      EvtData_168 evtf168;         /* Rainbow Stroke - Rainbow Swirl */
      EvtData_169 evtf169;         /* Evil Stream - FX1 */
      EvtData_170 evtf170;         /* Piercing Ray, Plasma Wave - FX1 */
      EvtData_171 evtf171;         /* Homing Ray */
      EvtData_172 evtf172;         /* Holy Pressure - Cube */
      EvtData_173 evtf173;         /* Fire Gem - Beam */
      EvtData_175 evtf175;         /* Rainbow Storm - FX1 */
      EvtData_176 evtf176;         /* Rainbow Stroke - FX1 */
      EvtData_177 evtf177;         /* Holy Pressure - FX1 */
      EvtData_178 evtf178;         /* Thunder Flash - FX1 */
      EvtData_179 evtf179;         /* Thunder Flash - Ray */
      EvtData_180 evtf180;         /* Spread Force - FX1 */
      EvtData_182 evtf182;         /* FX - TBD */
      EvtData_183 evtf183;         /* FX - TBD */
      EvtData_184 evtf184;         /* Avalanche - FX1 */
      EvtData_185 evtf185;         /* Avalanche - Rock */
      EvtData_186 evtf186;         /* FX - TBD */
      EvtData_188 evtf188;         /* Dark Fire - Ray */
      EvtData_189 evtf189;         /* Ice Storm - FX1 */
      EvtData_191 evtf191;         /* Magic Arrow - Arrow */
      EvtData_192 evtf192;         /* Perfect Guard - FX2 */
      EvtData_193 evtf193;         /* Dark Star - FX3 */
      EvtData_195 evtf195;         /* Rolling Thunder - FX2 */
      EvtData_196 evtf196;         /* Rolling Thunder - Orb Pair */
      EvtData_197 evtf197;         /* Rolling Thunder - FX1 */
      EvtData_198 evtf198;         /* Rolling Thunder - Casting Bolt */
      EvtData_199 evtf199;         /* Magic Arrow - FX1 */
      EvtData_201 evtf201;         /* Unit Struck */
      EvtData_202_746 evtf202;     /* Unit Blocking */
      EvtData_203 evtf203;         /* Blocking Impact */
      EvtData_204 evtf204;         /* Summon Crest */
      EvtData_205 evtf205;         /* Blood Spurt */
      EvtData_206 evtf206;         /* Blood Spurt Particle */
      EvtData_208 evtf208;         /* Holy Lightning - FX1 */
      EvtData_211 evtf211;         /* Avalanche - Boulder */
      EvtData_212 evtf212;         /* Holy Lightning - Casting Bolt */
      EvtData_213 evtf213;         /* Dust Cloud Spawner */
      EvtData_214 evtf214;         /* Dust Cloud */
      EvtData_215 evtf215;         /* Cloud (Sand, Dust, etc.) */
      EvtData_220 evtf220;         /* Explosion - FX2 */
      EvtData_222 evtf222;         /* FX - TBD */
      EvtData_223 evtf223;         /* FX - TBD */
      EvtData_224 evtf224;         /* Thunder Ball - FX1 */
      EvtData_225 evtf225;         /* Thunder Ball - Initial Orb */
      EvtData_226 evtf226;         /* Thunder Ball - Child Orb */
      EvtData_272 evtf272;         /* Stat Raised */
      EvtData_277 evtf277;         /* Zoom - TBD */
      EvtData_278 evtf278;         /* Faint Sparkles */
      EvtData_279 evtf279;         /* Ice Storm - Camera */
      EvtData_280 evtf280;         /* Dark Hurricane - Target */
      EvtData_281_282 evtf281;     /* Dark Hurricane - FX2 / FX3 */
      EvtData_283 evtf283;         /* Dark Hurricane - Cloud */
      EvtData_284 evtf284;         /* FX - TBD */
      EvtData_287 evtf287;         /* FX - TBD */
      EvtData_288 evtf288;         /* Map 13 - Bridge Explosion (Battle) */
      EvtData_290_294_761 evtf290; /* Reveal Chest Item */
      EvtData_290_294_761 evtf294; /* Reveal Item */
      EvtData_292 evtf292;         /* Blue Item Sparkles */
      EvtData_303 evtf303;         /* Map 31 - Scene 61 - Xeno's Flames */
      EvtData_304_661 evtf304;     /* Flame */
      EvtData_306_Etc evtf306;     /* Healing - FX2 */
      EvtData_307_324 evtf307;     /* Evil Stream - FX2 / FX3 */
      EvtData_310 evtf310;         /* Life Orb - Beam */
      EvtData_312 evtf312;         /* Bubble Swirl - Bubble */
      EvtData_313 evtf313;         /* Life Orb - FX1 */
      EvtData_314 evtf314;         /* Inward Ray */
      EvtData_315 evtf315;         /* Vertical Ray */
      EvtData_317_338 evtf317;     /* Avalanche - FX2 / FX3 */
      EvtData_321 evtf321;         /* FX - TBD */
      EvtData_322_Etc evtf322;     /* Magic Restoration (Magic Charge, etc.) - FX2 */
      EvtData_323_713 evtf323;     /* FX - TBD */
      EvtData_326 evtf326;         /* FX - TBD */
      EvtData_327 evtf327;         /* Healing Circle - FX2 */
      EvtData_330 evtf330;         /* Magic Restoration (Magic Charge, etc.) - FX1 */
      EvtData_331 evtf331;         /* FX - TBD */
      EvtData_332 evtf332;         /* Rolling Fire - FX1 */
      EvtData_334 evtf334;         /* Salamander - FX1 */
      EvtData_335_336 evtf335;     /* Salamander - Head */
      EvtData_335_336 evtf336;     /* Salamander - Segment */
      EvtData_132_Etc evtf344;     /* Roman Fire - FX2 / FX3 */
      EvtData_346 evtf346;         /* Button Depress */
      EvtData_347 evtf347;         /* Map 26 */
      EvtData_348 evtf348;         /* FX - TBD */
      EvtData_351 evtf351;         /* MsgBox Text */
      EvtData_352 evtf352;         /* Map 26 */
      EvtData_355_356 evtf355;     /* Map 19 - Elevator (TBD) */
      EvtData_355_356 evtf356;     /* Map 19 - Elevator (TBD) */
      EvtData_357 evtf357;         /* Map 19 */
      EvtData_358 evtf358;         /* Map 19 - Elevator (TBD) */
      EvtData_361 evtf361;         /* Map 13 - Bridge Explosion (Scene) */
      EvtData_362 evtf362;         /* Drawbridge Button */
      EvtData_363 evtf363;         /* Wyrmfang - FX1 */
      EvtData_365 evtf365;         /* Map 17 - Floodgate */
      EvtData_366 evtf366;         /* Map 17 - Button */
      EvtData_369 evtf369;         /* Screen Effect */
      EvtData_377 evtf377;         /* FX - TBD */
      EvtData_378 evtf378;         /* Phase Shift - FX1 */
      EvtData_379 evtf379;         /* Evil Stream - Rock */
      EvtData_380 evtf380;         /* Level Up - FX */
      EvtData_382 evtf382;         /* Flame Breath */
      EvtData_386 evtf386;         /* Healing Sparkle */
      EvtData_388 evtf388;         /* Dark Hurricane - FX1 */
      EvtData_389 evtf389;         /* Dark Hurricane - Vortex */
      EvtData_390 evtf390;         /* Dark Hurricane - Vortex Layer */
      EvtData_392 evtf392;         /* FX - TBD */
      EvtData_394 evtf394;         /* Dynamo Hum - FX1 */
      EvtData_395 evtf395;         /* Dynamo Hum - Electric Orb */
      EvtData_399 evtf399;         /* Map 11 */
      EvtData_396 evtf396;         /* Dynamo Hum - Orb Electricity */
      EvtData_400 evtf400;         /* AI - TBD */
      EvtData_401 evtf401;         /* AI - TBD */
      EvtData_402_Etc evtf402;     /* AI - TBD */
      EvtData_402_Etc evtf403;     /* AI - TBD */
      EvtData_402_Etc evtf404;     /* AI - TBD */
      EvtData_405 evtf405;         /* Panorama */
      EvtData_409 evtf409;         /* Event Entity */
      EvtData_410 evtf410;         /* Camera - Event Zoom */
      EvtData_413 evtf413;         /* MsgBox Portrait */
      EvtData_MapObject evtf415;   /* Map Object - Torch */
      EvtData_420_423 evtf420;     /* Battle Victory/Defeat */
      EvtData_421_422 evtf421;     /* MsgBox Tail (Upper) */
      EvtData_424 evtf424;         /* Battle Ender */
      EvtData_425 evtf425;         /* Battle - Options */
      EvtData_438 evtf438;         /* Evaluate Battle 08 */
      EvtData_446 evtf446;         /* Battle - Victory/Defeat Particle */
      EvtData_447 evtf447;         /* Unit Portrait (in depot, dojo, etc.) */
      EvtData_564_565_566 evtf564; /* Map Object - Rippling Water */
      EvtData_567 evtf567;         /* Opening Chest */
      EvtData_571 evtf571;         /* Level Up - Camera Control, Sound */
      EvtData_573 evtf573;         /* Battle Items List */
      EvtData_575 evtf575;         /* Status Portrait */
      EvtData_576 evtf576;         /* Tavern */
      EvtData_581 evtf581;         /* Audio Command */
      EvtData_585 evtf585;         /* Battle - Player Event */
      EvtData_586 evtf586;         /* Battle - MsgBox */
      EvtData_587 evtf587;         /* Battle - Enemy Event */
      EvtData_026_588 evtf588;     /* Camera - TBD */
      EvtData_402_Etc evtf589;     /* AI - TBD */
      EvtData_591 evtf591;         /* Map Object - Boulder */
      EvtData_592 evtf592;         /* Battle - Turn Start */
      EvtData_593 evtf593;         /* Battle - Results - Unit (Reward or Penalty) */
      EvtData_595 evtf595;         /* Status Window Manager */
      EvtData_597 evtf597;         /* Battle - Intro */
      EvtData_650 evtf650;         /* Map 32 - Car Release */
      EvtData_651 evtf651;         /* Map 33 - Lava Pit Platform */
      EvtData_652 evtf652;         /* Map 35 - Button */
      EvtData_653 evtf653;         /* Exploding Tile */
      EvtData_654 evtf654;         /* Map 38 - Wash Away Unit */
      EvtData_655 evtf655;         /* Map 38 - Raise Floodgate */
      EvtData_656 evtf656;         /* Map 39 */
      EvtData_657 evtf657;         /* Map 38 - Floodgate */
      EvtData_658 evtf658;         /* Map 38 - Floodwater */
      EvtData_663 evtf663;         /* Map 28 - Button */
      EvtData_665 evtf665;         /* Map 27 - Buttons */
      EvtData_666 evtf666;         /* Map 14 - Lower Sand Mound */
      EvtData_668 evtf668;         /* Map 14 - Raise Sand Mound */
      EvtData_670 evtf670;         /* Map 14 - Sand */
      EvtData_672 evtf672;         /* Map 39 - Splashing Tile */
      EvtData_673 evtf673;         /* Map 32 - Scene 63 - Cinematic */
      EvtData_675 evtf675;         /* Leena's Forcefield */
      EvtData_681 evtf681;         /* Stat Buff FX */
      EvtData_692 evtf692;         /* Campfire */
      EvtData_715_to_718 evtf715;  /* Spellbind - FX2 / FX3 */
      EvtData_733 evtf733;         /* Stat Buff Icon */
      EvtData_747_748 evtf747;     /* Wyrmfang - Flames */
      EvtData_749 evtf749;         /* Wyrmfang - Flame */
      EvtData_750_751 evtf750;     /* Map 33 - Lower Platform */
      EvtData_754 evtf754;         /* Map 39 - Scene 82 */
      EvtData_760 evtf760;         /* Elite Melee Sparkles */
      EvtData_290_294_761 evtf761; /* Reveal Used Item */
      EvtData_795 evtf795;         /* Event Fade */
      EvtData_133_Etc evtf801;     /* FX - TBD */
      EvtData_133_Etc evtf802;     /* FX - TBD */
      EvtData_133_Etc evtf803;     /* FX - TBD */
      EvtData_MapObject mapObj;    /* Map Object - Generic */
      EvtData_EntitySpawn entitySpawn;
      EvtData_Unk_8006183c evtfUnk8006183c;
      EvtData_Unk_80080924 evtfUnk80080924;
      EvtData_Unk_80087b58 evtfUnk80087b58;
      EvtData_Unk_8008a364 evtfUnk8008a364;
      EvtData_Unk_8008d1f0 evtfUnk8008d1f0;
      EvtData_Unk_UsedBy392 evtfUnkUsedBy392;
   } d;
} EvtData;

typedef void (*EvtFunction)(EvtData *evt);

extern EvtData gEvtDataArray[EVT_DATA_CT];
extern EvtFunction gEvtFunctionPointers[804];
extern EvtData gEvtData050_UnitSpritesDecoder;
extern EvtData *gTempGfxEvt;
extern EvtData *gTempEvt;
extern EvtData *gScreenFade;

void Evt_ResetByFunction(s16);
EvtData *Evt_FindByFunction(s16);
void Evt_Execute(void);
void Evt_ResetFromIdx10(void);
void Evt_ResetAll(void);
EvtData *Evt_GetUnused(void);
EvtData *Evt_GetFirstUnused(void);
EvtData *Evt_GetLastUnused(void);
EvtData *Evt_GetLastUnusedSkippingTail(s32);
s32 Evt_CountUnused(void);

EvtData *CreatePositionedEvt(EvtData *, s16);
EvtData *SnapToUnit(EvtData *);

#endif
