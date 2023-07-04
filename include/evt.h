#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "graphics.h"

// TODO: I've been procrastinating on this, but I really need to re-evaluate the "evt" naming, since
// they drive the entire game; substituting evt->obj seems reasonable and should be mostly painless;
// Also need to start cleaning up a bunch of other naming inconsistencies.

#define EVT_DATA_CT 350
#define EVT_DATA_LAST_IDX 349

// Can't decide if this is an improvement over just typing it out; maybe for big functions?
#define EVT_HELPER_2(x) evt->d.evtf##x
#define EVT_HELPER_1(x) EVT_HELPER_2(x)
#define EVT EVT_HELPER_1(EVTF)
#define EVTF 000

typedef enum EvtFunctionIdx {
   EVTF_NULL = 0,
   EVTF_NOOP = 1, /* TBD Used only for sprites? Sometimes used as a data store? */
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
   EVTF_PROJECTILE_INDIRECT = 29, /* Unused? maybe to open chest w/ ranged attacker? */
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
   EVTF_SPELL_FX2_HEALING = 100,
   EVTF_HEALING_SPARKLE = 101,
   EVTF_SPELL_FX2_POISON = 102,
   EVTF_POISON_BUBBLES = 103,
   EVTF_SPELL_FX_MAGIC_CHARGE_GLYPH_RING = 107,
   EVTF_SPELL_FX_HARMFUL_WAVE_RING = 109,
   EVTF_CASTING_STAT_BUFF = 110,
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
   EVTF_FX_TBD_149 = 149,
   EVTF_SPELL_FX_DELTA_MIRAGE_RAY = 157,
   EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT = 198,
   EVTF_UNIT_STRUCK = 201,
   EVTF_UNIT_BLOCKING = 202,
   EVTF_BLOCKING_IMPACT_PARTICLE = 203,
   EVTF_SUMMON_CREST = 204,
   EVTF_BLOOD_SPURT = 205,
   EVTF_BLOOD_SPURT_PARTICLE = 206,
   EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT = 212,
   EVTF_DUST_CLOUD_SPAWNER = 213,
   EVTF_DUST_CLOUD = 214,
   EVTF_CLOUD = 215,
   EVTF_STAT_RAISED = 272,
   EVTF_CASTING_FX = 285,
   EVTF_REVEAL_CHEST_ITEM = 290,
   EVTF_REVEAL_HIDDEN_ITEM = 294,
   EVTF_FILE_SAVE_DIALOG = 341,
   EVTF_FILE_SAVE_DIALOG_IBS = 342,
   EVTF_FILE_LOAD_DIALOG_360 = 360,
   EVTF_SCREEN_EFFECT = 369,
   EVTF_FILE_LOAD_DIALOG = 373,
   EVTF_FILE_LOAD_DIALOG_IBS = 374,
   EVTF_FILE_LOAD_DIALOG_376 = 376,
   EVTF_LEVEL_UP_FX = 380,
   EVTF_FLAME_BREATH = 382,
   EVTF_REVEAL_MIMIC = 385,
   EVTF_FULLSCREEN_IMAGE = 387,
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
   EVTF_STAT_BUFF_FX = 681,
   EVTF_ADJUST_FACE_ELEVATION = 683,
   EVTF_SLIDING_FACE = 684,
   EVTF_ROCK_SPURT = 685,
   EVTF_ROCK_SPURT_PARTICLE_2 = 686,
   EVTF_TBD_732 = 732,
   EVTF_STAT_BUFF_ICON = 733,
   EVTF_SPARKLE_DUST = 735,
   EVTF_REMOVE_PARALYSIS = 737,
   EVTF_ENTITY_BLOCKING = 746,
   EVTF_PUSHED_OBJECT_SPLASH = 757,
   EVTF_ELITE_MELEE_SPARKLES = 760,
   EVTF_REVEAL_USED_ITEM = 761,
   EVTF_BOULDER_RUBBLE = 763,
   EVTF_PROJECTILE_TRAIL_POISON = 764,
   EVTF_PROJECTILE_TRAIL_EXPLOSION = 765,
   EVTF_PROJECTILE_TRAIL_SMOKE = 766,
   EVTF_PROJECTILE_TRAIL_SPARKLES = 767,
   EVTF_ITEM_SPELL = 770,
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
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s8 type;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s16 zoom;
   /* :0x2C */ u8 unk_0x2C[10];
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

/* Spell FX2 - Damage */
typedef struct EvtData_078 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_078;

/* Spell FX3 - Slay */
typedef struct EvtData_079 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_079;

/* Spell FX3 - Healing */
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

/* Spell FX2 - Poison */
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

/* Spell FX2 - Cure */
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

/* Spell FX3 - Magic Charge */
typedef struct EvtData_106 {
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *glyphRing;
} EvtData_106;

/* Spell FX - Magic Charge - Glyph Ring */
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

/* Spell FX2 - Harmful Wave */
typedef struct EvtData_108 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_108;

/* Spell FX - Harmful Wave - Ring */
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

/* FX - TBD */
typedef struct EvtData_119 {
   /* :0x24 */ s16 type;
   /* :0x26 */ s16 boxIdx;
   /* :0x28 */ s16 clut;
   /* :0x2A */ s8 semiTrans;
   /* :0x2B */ u8 unk_0x2B;
   /* :0x2C */ s16 once;
   /* :0x2E */ s16 radius;
   /* :0x30 */ s16 theta;
   /* :0x32 */ s16 h;
   /* :0x34 */ u8 unk_0x34[24];
   /* :0x4C */ s16 timer;
   /* :0x4E */ u8 unk_0x4E[14];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_119;

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
typedef struct EvtData_149 {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 fadeSpeed;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 increasing;
   /* :0x30 */ u8 unk_0x30[44];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_149;

/* Spell FX1 - Delta Mirage */
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

/* Spell FX - Delta Mirage - Ray */
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

/* Spell FX - Fire Gem - Beam */
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

/* Spell FX1 - Rolling Thunder */
typedef struct EvtData_197 {
   /* :0x24 */ union {
      struct EvtData *unitSpriteParam;
      s16 timer;
   } variant_0x24;
   /* :0x28 */ u8 unk_0x28[16];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ struct EvtData *bolts[9];
} EvtData_197;

/* Spell FX - Rolling Thunder - Casting Bolt */
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

/* Spell FX1 - Holy Lightning */
typedef struct EvtData_208 {
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[18];
   /* :0x38 */ struct EvtData *unitSprite;
   /* :0x3C */ struct EvtData *bolts[9];
} EvtData_208;

/* Spell FX - Holy Lightning - Casting Bolt */
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

/* Stat Raised */
typedef struct EvtData_272 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[10];
   /* :0x34 */ s16 todo_x34;
   /* :0x36 */ u8 unk_0x36[22];
   /* :0x4C */ s16 type;
   /* :0x4E */ u8 unk_0x4E[6];
   /* :0x54 */ s16 todo_x54;
   /* :0x56 */ s16 todo_x56;
   /* :0x58 */ s16 todo_x58;
   /* :0x5A */ s16 todo_x5a;
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_272;

/* Reveal Item */
typedef struct EvtData_290_294_761 {
   /* :0x24 */ u8 unk_0x24[8];
   /* :0x2C */ u16 gfxIdx;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_290_294_761;

/* Screen Effect (Incomplete) */
typedef struct EvtData_369 {
   /* :0x24 */ u8 unk_0x24[4];
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ s16 todo_x2c;
   /* :0x2E */ u8 unk_0x2E[6];
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
   /* :0x2a */ s16 yOffset;
   /* :0x2c */ u8 unk_0x2c[52];
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

typedef struct EvtData_Unk_8006183c {
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 scale;
   /* :0x28 */ u8 unk_0x28[52];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_Unk_8006183c;

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
      EvtData_078 evtf078;         /* Spell FX2 - Damage */
      EvtData_079 evtf079;         /* Spell FX3 - Slay */
      EvtData_100 evtf100;         /* Spell FX3 - Healing */
      EvtData_101 evtf101;         /* Healing Sparkle */
      EvtData_102_227 evtf102;     /* Spell FX2 - Poison */
      EvtData_103 evtf103;         /* Poison Bubbles */
      EvtData_104 evtf104;         /* Spell FX2 - Cure */
      EvtData_106 evtf106;         /* Spell FX3 - Magic Charge */
      EvtData_107 evtf107;         /* Spell FX - Magic Charge - Glyph Ring */
      EvtData_108 evtf108;         /* Spell FX2 - Harmful Wave */
      EvtData_109 evtf109;         /* Spell FX - Harmful Wave - Ring */
      EvtData_110 evtf110;         /* Casting Stat Buff */
      EvtData_119 evtf119;         /* FX - TBD */
      EvtData_149 evtf149;         /* FX - TBD */
      EvtData_130 evtf130;         /* FX - TBD */
      EvtData_131 evtf131;         /* Slay Unit */
      EvtData_132_Etc evtf132;     /* FX - TBD */
      EvtData_133_Etc evtf133;     /* FX - TBD */
      EvtData_133_Etc evtf137;     /* FX - TBD */
      EvtData_133_Etc evtf141;     /* FX - TBD */
      EvtData_147 evtf147;         /* FX - TBD */
      EvtData_156 evtf156;         /* Spell FX1 - Delta Mirage */
      EvtData_157 evtf157;         /* Spell FX - Delta Mirage - Ray */
      EvtData_173 evtf173;         /* Spell FX - Fire Gem - Beam */
      EvtData_197 evtf197;         /* Spell FX1 - Rolling Thunder */
      EvtData_198 evtf198;         /* Spell FX - Rolling Thunder - Casting Bolt */
      EvtData_201 evtf201;         /* Unit Struck */
      EvtData_202_746 evtf202;     /* Unit Blocking */
      EvtData_203 evtf203;         /* Blocking Impact */
      EvtData_204 evtf204;         /* Summon Crest */
      EvtData_205 evtf205;         /* Blood Spurt */
      EvtData_206 evtf206;         /* Blood Spurt Particle */
      EvtData_208 evtf208;         /* Spell FX1 - Holy Lightning */
      EvtData_212 evtf212;         /* Spell FX - Holy Lightning - Casting Bolt */
      EvtData_213 evtf213;         /* Dust Cloud Spawner */
      EvtData_214 evtf214;         /* Dust Cloud */
      EvtData_215 evtf215;         /* Cloud (Sand, Dust, etc.) */
      EvtData_272 evtf272;         /* Stat Raised */
      EvtData_290_294_761 evtf290; /* Reveal Chest Item */
      EvtData_290_294_761 evtf294; /* Reveal Item */
      EvtData_369 evtf369;         /* Screen Effect */
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
      EvtData_681 evtf681;         /* Stat Buff FX */
      EvtData_733 evtf733;         /* Stat Buff Icon */
      EvtData_133_Etc evtf801;     /* FX - TBD */
      EvtData_133_Etc evtf802;     /* FX - TBD */
      EvtData_133_Etc evtf803;     /* FX - TBD */
      EvtData_MapObject mapObj;    /* Map Object - Generic */
      EvtData_Unk_8006183c evtfUnk8006183c;
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

EvtData *CreatePositionedEvt(EvtData *, s32);

#endif
