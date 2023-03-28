#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "graphics.h"

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
   EVTF_TARGETING_ATTACK = 15,
   EVTF_CHOOSE_DONE_DIRECTION = 16,
   EVTF_CAMERA_TBD_017 = 17,
   EVTF_COMPASS = 19,
   EVTF_UNIT_ATTACKING = 21,
   EVTF_PROJECTILE = 22,
   EVTF_BOUNCE_ZOOM = 24,
   EVTF_OVERHEAD_MAP_VIEW = 25,
   EVTF_CAMERA_TBD_026 = 26,
   EVTF_TARGETING_SPELL = 27,
   EVTF_UNIT_CASTING = 28,
   EVTF_PROJECTILE_INDIRECT = 29, /* Unused? maybe to open chest w/ ranged attacker? */
   EVTF_BATTLE_SPELLS_LIST = 31,
   EVTF_DISPLAY_DAMAGE = 32,
   EVTF_DISPLAY_DAMAGE_2 = 33,
   EVTF_MAP_OBJECT_CHEST = 40,
   EVTF_MAP_OBJECT_CRATE = 46,
   EVTF_PUSH = 48,
   EVTF_BATTLE_MAP_CURSOR_CONTROL = 49,
   EVTF_UNIT_SPRITES_DECODER = 50,
   EVTF_FLOATING_DAMAGE_TEXT = 51,
   EVTF_ATTACK_INFO_MARKER = 52,
   EVTF_STRETCH_WARP_SPRITE = 62,
   EVTF_CIRCLE = 77,
   EVTF_SPELL_FX2_HEALING = 100,
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
   EVTF_BLOOD_SPURT = 205,
   EVTF_DUST_CLOUD = 214,
   EVTF_CLOUD = 215,
   EVTF_REVEAL_CHEST_ITEM = 290,
   EVTF_REVEAL_HIDDEN_ITEM = 294,
   EVTF_FILE_SAVE_DIALOG = 341,
   EVTF_FILE_SAVE_DIALOG_IBS = 342,
   EVTF_FILE_LOAD_DIALOG_360 = 360,
   EVTF_SCREEN_EFFECT = 369,
   EVTF_FILE_LOAD_DIALOG = 373,
   EVTF_FILE_LOAD_DIALOG_IBS = 374,
   EVTF_FULLSCREEN_IMAGE = 387,
   EVTF_PANORAMA = 405,
   EVTF_NOOP_407 = 407,
   EVTF_CLOSED_WINDOW = 408,
   EVTF_EVENT_CAMERA = 412,
   EVTF_MSGBOX_PORTRAIT = 413,
   EVTF_DEBUG_MENU = 414,
   EVTF_BATTLE_VICTORY = 420,
   EVTF_UPPER_MSGBOX_TAIL = 421,
   EVTF_LOWER_MSGBOX_TAIL = 422,
   EVTF_BATTLE_ENDER = 424,
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
   EVTF_MAP_OBJECT_LAVA_2 = 569,
   EVTF_AI_TBD_570 = 570,
   EVTF_LEVEL_UP = 571,
   EVTF_MAP_OBJECT_LAVA_3 = 572,
   EVTF_BATTLE_ITEMS_LIST = 573,
   EVTF_DISPLAY_ICON = 574,
   EVTF_TAVERN = 576,
   EVTF_AUDIO_CMD = 581,
   EVTF_MAIN_MENU_JPN = 582,
   EVTF_BATTLE_PLAYER_EVENT = 585,
   EVTF_BATTLE_MSGBOX = 586,
   EVTF_BATTLE_ENEMY_EVENT = 587,
   EVTF_CAMERA_TBD_588 = 588,
   EVTF_BATTLE_TURN_TICKER = 590,
   EVTF_MAP_OBJECT_BOULDER = 591,
   EVTF_BATTLE_TURN_START = 592,
   EVTF_BATTLE_RESULTS = 594,
   EVTF_STATUS_WINDOW = 595,
   EVTF_STATUS_WINDOW_MGR = 596,
   EVTF_BATTLE_INTRO = 597,
   EVTF_TBD_732 = 732,
   EVTF_SPARKLE_DUST = 735,
   EVTF_REMOVE_PARALYSIS = 737,
   EVTF_REVEAL_USED_ITEM = 761,
   EVTF_PROJECTILE_TRAIL_POISON = 764,
   EVTF_PROJECTILE_TRAIL_EXPLOSION = 765,
   EVTF_PROJECTILE_TRAIL_SMOKE = 766,
   EVTF_PROJECTILE_TRAIL_SPARKLES = 767,
   EVTF_ITEM_SPELL = 770,
   EVTF_FX_TBD_799 = 799,
   EVTF_FX_TBD_800 = 800,
   EVTF_FX_TBD_801 = 801,
   EVTF_FX_TBD_802 = 802,
   EVTF_FX_TBD_803 = 803,
} EvtFunctionIdx;

typedef struct EvtData_Sprite {
   /* :0x10 */ s16 animState;
   /* :0x12 */ s16 x1;
   /* :0x14 */ s16 y1;
   /* :0x16 */ s16 z1;
   /* :0x18 */ s16 x2;
   /* :0x1A */ s16 y2;
   /* :0x1C */ s16 z2;
   /* :0x1E */ s16 x3;
   /* :0x20 */ s16 y3;
   /* :0x22 */ s16 z3;
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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

/* Battle - Actions */
typedef struct EvtData_003 {
   /* :0x10 */ s16 cursorState;
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[6];
   /* :0x1E */ s16 unitX;
   /* :0x20 */ s16 unitY;
   /* :0x22 */ s16 unitZ;
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
   /* :0x10 */ s16 effectState;
   /* :0x12 */ s16 x; /* Center point */
   /* :0x14 */ s16 y;
   /* :0x16 */ u8 unk_0x16[8];
   /* :0x1E */ s16 destX;
   /* :0x20 */ s16 destY;
   /* :0x22 */ u8 unk_0x22[2];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 timer;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_007;

/* Battle Portrait */
typedef struct EvtData_008 {
   /* :0x10 */ u8 unk_0x10[20];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ struct EvtData *unitSprite;
   /* :0x2C */ s8 timer;
   /* :0x2D */ s8 todo_x2d;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_013;

/* Battle Unit (Incomplete) */
typedef struct EvtData_014 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ u8 team;
   /* :0x25 */ u8 unitIdx;
   /* :0x26 */ u8 pathIdx;
   /* :0x27 */ u8 animIdx;
   /* :0x28 */ struct UnitStatus *unit;
   /* :0x2C */ u8 **animSet;
   /* :0x30 */ struct EvtData *sprite;
   /* :0x34 */ s8 timer;
   /* :0x35 */ u8 unk_0x35[43];
} EvtData_014;

/* Targeting Attack */
typedef struct EvtData_015 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[72];
} EvtData_015;

/* Choose Done Direction */
typedef struct EvtData_016 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s16 angle;
   /* :0x2A */ u8 unk_0x2A[54];
} EvtData_016;

/* Camera - TBD */
typedef struct EvtData_017 {
   /* :0x10 */ u8 unk_0x10[20];
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

/* Unit Attacking */
typedef struct EvtData_021 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ struct EvtData *targetSprite;
   /* :0x28 */ s16 yRotDst;
   /* :0x2A */ s16 delay;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_023;

/* Camera - Bounce Zoom */
typedef struct EvtData_024 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 soft;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ s16 todo_x28;
   /* :0x2A */ s16 todo_x2a;
   /* :0x2C */ s16 savedZoom;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_024;

/* Overhead Map View */
typedef struct EvtData_025 {
   /* :0x10 */ u8 unk_0x10[20];
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
   /* :0x10 */ u8 unk_0x10[20];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[16];
   /* :0x28 */ struct UnitStatus *caster;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_027;

/* Unit Casting Spell */
typedef struct EvtData_028 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[7];
   /* :0x1F */ s8 targetX;
   /* :0x20 */ u8 unk_0x20[3];
   /* :0x23 */ s8 targetZ;
   /* :0x24 */ s8 timer;
   /* :0x25 */ s8 mapSizeX;
   /* :0x26 */ s8 mapSizeZ;
   /* :0x27 */ u8 unk_0x27[57];
} EvtData_028;

/* Unit/Field Info */
typedef struct EvtData_030 {
   /* :0x10 */ s16 terrainInfoState;
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s8 previousTerrain;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s8 unitIdx;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_030;

/* Battle Spells List */
typedef struct EvtData_031 {
   /* :0x10 */ s16 drawState;
   /* :0x12 */ u8 unk_0x12[18];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 spell;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_031;

/* Display Damage */
typedef struct EvtData_032_033 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ struct EvtData *barFg;
   /* :0x28 */ struct EvtData *barBg;
   /* :0x2C */ s16 barDstX3;
   /* :0x2E */ s16 timer;
   /* :0x30 */ s32 maxHp;
   /* :0x34 */ s32 prevHp;
   /* :0x38 */ s32 curHp;
   /* :0x3C */ u8 unk_0x3C[36];
} EvtData_032_033;

/* Unit Sprites Decoder */
typedef struct EvtData_050 {
   /* :0x10 */ u8 unk_0x10[44];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 speed;
   /* :0x26 */ s16 phase;
   /* :0x28 */ u8 unk_0x28[40];
   /* :0x50 */ struct EvtData *sprite;
   /* :0x54 */ u8 unk_0x54[12];
} EvtData_062;

/* Fade From Black */
typedef struct EvtData_070 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_070;

/* Fade To Black */
typedef struct EvtData_071 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_071;

/* Fade From White */
typedef struct EvtData_072 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_072;

/* Fade To White */
typedef struct EvtData_073 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 fade;
   /* :0x4E */ u8 unk_0x4E[18];
} EvtData_073;

/* Fade In Sprite */
typedef struct EvtData_074 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 fade;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_074;

/* Fade Out Sprite */
typedef struct EvtData_075 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 speed;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 fade;
   /* :0x2A */ u8 unk_0x2A[50];
   /* :0x5C */ struct EvtData *sprite;
} EvtData_075;

/* FX - Circles (TBD) */
typedef struct EvtData_076 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[40];
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_076;

/* FX - Circle (TBD) */
typedef struct EvtData_077 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 fade;
   /* :0x26 */ u8 unk_0x26[38];
   /* :0x4C */ s16 angle;
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_077;

/* Spell FX2 - Damage */
typedef struct EvtData_078 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_078;

/* Spell FX3 - Slay */
typedef struct EvtData_079 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_079;

/* FX - TBD */
typedef struct EvtData_119 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 todo_x24;
   /* :0x26 */ s16 todo_x26;
   /* :0x28 */ u8 unk_0x28[38];
   /* :0x4E */ s16 todo_x4e;
   /* :0x50 */ s16 todo_x50;
   /* :0x52 */ u8 unk_0x52[14];
} EvtData_130;

/* FX - TBD */
/* 132, 134, 136, 138, 140, 142, 799, 800 */
typedef struct EvtData_132_Etc {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
typedef struct EvtData_149 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 clut;
   /* :0x26 */ s16 fadeSpeed;
   /* :0x28 */ u8 unk_0x28[4];
   /* :0x2C */ s16 fade;
   /* :0x2E */ s16 increasing;
   /* :0x30 */ u8 unk_0x30[44];
   /* :0x5C */ struct EvtData *unitSprite;
} EvtData_149;

/* Dust Cloud Spawner */
typedef struct EvtData_213 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 timer;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_213;

/* Dust Cloud */
typedef struct EvtData_214 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ s16 theta;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s16 halfSize;
   /* :0x2A */ u8 unk_0x2A[2];
   /* :0x2C */ struct EvtData *sprite;
   /* :0x30 */ SVectorXZY positions[8];
} EvtData_214;

/* Cloud (Sand, Dust, etc.) */
typedef struct EvtData_215 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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

/* Reveal Item */
typedef struct EvtData_290_294_761 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[20];
   /* :0x2C */ u16 gfxIdx;
   /* :0x2E */ u8 unk_0x2E[50];
} EvtData_290_294_761;

/* Panorama */
typedef struct EvtData_405 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 pan;
   /* :0x14 */ u8 unk_0x14[16];
   /* :0x24 */ s16 yRot;
   /* :0x26 */ s16 xRot;
   /* :0x28 */ s16 xOffset;
   /* :0x2a */ s16 yOffset;
   /* :0x2c */ u8 unk_0x2c[52];
} EvtData_405;

/* Camera - Event Zoom */
typedef struct EvtData_410 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 zoom;
   /* :0x26 */ s16 timer;
   /* :0x28 */ u8 unk_0x28[56];
} EvtData_410;

/* MsgBox Portrait */
typedef struct EvtData_413 {
   /* :0x10 */ s16 blinkState;
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_420_423;

/* MsgBox Tail */
typedef struct EvtData_421_422 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ struct EvtData *sprite;
   /* :0x28 */ s16 left;
   /* :0x2A */ s16 top;
   /* :0x2C */ s16 right;
   /* :0x2E */ s16 bottom;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_421_422;

/* Battle - Options */
typedef struct EvtData_425 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_425;

/* Evaluate Battle 08 */
typedef struct EvtData_438 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 delay;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_438;

/* Battle - Victory/Defeat Particle */
typedef struct EvtData_446 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 windowId;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ struct EvtData *window;
   /* :0x2C */ struct EvtData *sprite;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_447;

/* Map Object - Rippling Water/Lava (animated texture) */
typedef struct EvtData_564_565_566 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ u16 phase;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_564_565_566;

/* Opening Chest */
typedef struct EvtData_567 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
   /* :0x24 */ struct UnitStatus *opener;
   /* :0x28 */ u8 sendToDepot;
   /* :0x29 */ u8 unk_0x29[3];
   /* :0x2C */ struct EvtData *openerSprite;
   /* :0x30 */ u8 unk_0x30[48];
} EvtData_567;

/* Level Up - Camera Control, Sound */
typedef struct EvtData_571 {
   /* :0x10 */ u8 unk_0x10[20];
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
   /* :0x10 */ s16 drawState;
   /* :0x12 */ u8 unk_0x12[18];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 item;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_573;

/* Status Portrait */
typedef struct EvtData_575 {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x1;
   /* :0x14 */ s16 y1;
   /* :0x16 */ u8 unk_0x16[8];
   /* :0x1E */ s16 x3;
   /* :0x20 */ s16 y3;
   /* :0x22 */ u8 unk_0x22[6];
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
   /* :0x10 */ s16 state3;
   /* :0x12 */ u8 unk_0x12[18];
   /* :0x24 */ s8 needSpeak[4];
   /* :0x28 */ s8 textPtrIdx;
   /* :0x29 */ u8 unk_0x29[35];
   /* :0x4C */ s8 timer;
   /* :0x4D */ u8 unk_0x4D[19];
} EvtData_576;

/* Audio Command */
typedef struct EvtData_581 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 cmd;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s8 delay;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_581;

/* Battle - Player Event */
typedef struct EvtData_585 {
   /* :0x10 */ u8 unk_0x10[76];
   /* :0x5C */ s8 timer;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_585;

/* Battle - MsgBox */
typedef struct EvtData_586 {
   /* :0x10 */ u8 unk_0x10[20];
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
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[3];
   /* :0x28 */ s8 xenoSpawnZ;
   /* :0x29 */ u8 unk_0x29;
   /* :0x2A */ s8 spawnZ;
   /* :0x2B */ s8 spawnX;
   /* :0x2C */ u8 unk_0x2C[52];
} EvtData_587;

/* Battle - Turn Start */
typedef struct EvtData_592 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 timer;
   /* :0x25 */ s8 team;
   /* :0x26 */ s8 targets;
   /* :0x27 */ u8 unk_0x27[57];
} EvtData_592;

/* Battle - Intro */
typedef struct EvtData_597 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 timer;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_597;

typedef struct EvtData_Unk_8006183c {
   /* :0x10 */ u8 unk_0x10[2];
   /* :0x12 */ s16 x;
   /* :0x14 */ s16 y;
   /* :0x16 */ s16 z;
   /* :0x18 */ u8 unk_0x18[12];
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
   /* 0x10: */
   union {
      u8 bytes[80];
      EvtData_Sprite sprite;
      EvtData_Sprite2 sprite2;
      EvtData_003 evtf003;         /* Battle - Actions */
      EvtData_004_005_408 evtf004; /* Window */
      EvtData_007 evtf007;         /* Apply Poison */
      EvtData_008 evtf008;         /* Battle Portrait */
      EvtData_013 evtf013;         /* Battle Manager */
      EvtData_014 evtf014;         /* Battle Unit */
      EvtData_015 evtf015;         /* Targeting Attack */
      EvtData_016 evtf016;         /* Choose Done Direction */
      EvtData_017 evtf017;         /* Camera - TBD */
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
      EvtData_119 evtf119;         /* FX - TBD */
      EvtData_149 evtf149;         /* FX - TBD */
      EvtData_130 evtf130;         /* FX - TBD */
      EvtData_132_Etc evtf132;     /* FX - TBD */
      EvtData_133_Etc evtf133;     /* FX - TBD */
      EvtData_133_Etc evtf137;     /* FX - TBD */
      EvtData_133_Etc evtf141;     /* FX - TBD */
      EvtData_213 evtf213;         /* Dust Cloud Spawner */
      EvtData_214 evtf214;         /* Dust Cloud */
      EvtData_215 evtf215;         /* Cloud (Sand, Dust, etc.) */
      EvtData_290_294_761 evtf294; /* Reveal Item */
      EvtData_405 evtf405;         /* Panorama */
      EvtData_410 evtf410;         /* Camera - Event Zoom */
      EvtData_413 evtf413;         /* MsgBox Portrait */
      EvtData_420_423 evtf420;     /* Battle Victory/Defeat */
      EvtData_421_422 evtf421;     /* MsgBox Tail (Upper) */
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
      EvtData_592 evtf592;         /* Battle - Turn Start */
      EvtData_597 evtf597;         /* Battle - Intro */
      EvtData_133_Etc evtf801;     /* FX - TBD */
      EvtData_133_Etc evtf802;     /* FX - TBD */
      EvtData_133_Etc evtf803;     /* FX - TBD */
      EvtData_Unk_8006183c evtfUnk8006183c;
   } d;
} EvtData;

typedef void (*EvtFunction)(EvtData *evt);

extern EvtData gEvtDataArray[EVT_DATA_CT];
extern EvtFunction gEvtFunctionPointers[804];
extern EvtData gEvtData050_UnitSpritesDecoder;
extern EvtData *gTempGfxEvt;
extern EvtData *gTempEvt;

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

void Evtf001_Noop(EvtData *evt);
void Evtf426_EvaluateBattle10(EvtData *);
void Evtf427_EvaluateBattle11(EvtData *);
void Evtf438_EvaluateBattle08(EvtData *);
void Evtf574_DisplayIcon(EvtData *);
void Evtf581_AudioCommand(EvtData *);

EvtData *CreatePositionedEvt(EvtData *, s32);

#endif
