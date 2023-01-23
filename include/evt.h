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
   EVTF_WINDOW_TBD_004 = 4,
   EVTF_WINDOW_TBD_005 = 5,
   EVTF_PROJECTILE = 22,
   EVTF_PROJECTILE_INDIRECT = 29, /* Unused? maybe to open chest w/ ranged attacker? */
   EVTF_MAP_OBJECT_CHEST = 40,
   EVTF_MAP_OBJECT_CRATE = 46,
   EVTF_UNIT_SPRITES_DECODER = 50,
   EVTF_FILE_SAVE_DIALOG = 341,
   EVTF_FILE_LOAD_DIALOG_360 = 360,
   EVTF_SCREEN_EFFECT = 369,
   EVTF_FILE_LOAD_DIALOG = 373,
   EVTF_FULLSCREEN_IMAGE = 387,
   EVTF_PANORAMA = 405,
   EVTF_NOOP_407 = 407,
   EVTF_CLOSED_WINDOW = 408,
   EVTF_EVENT_CAMERA = 412,
   EVTF_DEBUG_MENU = 414,
   EVTF_UPPER_MSGBOX_TAIL = 421,
   EVTF_LOWER_MSGBOX_TAIL = 422,
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
   EVTF_MAP_OBJECT_LAVA_2 = 569,
   EVTF_MAP_OBJECT_LAVA_3 = 572,
   EVTF_DISPLAY_ICON = 574,
   EVTF_AUDIO_CMD = 581,
   EVTF_MAIN_MENU_JPN = 582,
   EVTF_MAP_OBJECT_BOULDER = 591,
   EVTF_SPARKLE_DUST = 735,
   EVTF_PROJECTILE_TRAIL_POISON = 764,
   EVTF_PROJECTILE_TRAIL_EXPLOSION = 765,
   EVTF_PROJECTILE_TRAIL_SMOKE = 766,
   EVTF_PROJECTILE_TRAIL_SPARKLES = 767,

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
   /* :0x34 */ s16 vecIdx;
   /* :0x36 */ s16 stripIdx;
   /* :0x38 */ u8 *animData;
   /* :0x3C */ SVectorXZY coords[4];
   /* :0x54 */ s8 facingFront;
   /* :0x55 */ u8 unk_0x55[3];
   /* :0x58 */ s16 animRelativeOfs;
   /* :0x5A */ s16 animYOfs;
   /* :0x5C */ u8 animSingleAxis;
   /* :0x5D */ u8 unk_0x5D[3];
} EvtData_Sprite;

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

/* Battle Spells List */
typedef struct EvtData_031 {
   /* :0x10 */ s16 drawState;
   /* :0x12 */ u8 unk_0x12[18];
   /* :0x24 */ struct UnitStatus *unit;
   /* :0x28 */ s8 spell;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_031;

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

/* Evaluate Battle 08 */
typedef struct EvtData_438 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 delay;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_438;

/* Map Object - Rippling Water/Lava (animated texture) */
typedef struct EvtData_564_565_566 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ u16 phase;
   /* :0x26 */ u8 unk_0x26[58];
} EvtData_564_565_566;

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
   /* :0x3A */ s16 delay;
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

/* Audio Command */
typedef struct EvtData_581 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s16 cmd;
   /* :0x26 */ u8 unk_0x26[2];
   /* :0x28 */ s8 delay;
   /* :0x29 */ u8 unk_0x29[55];
} EvtData_581;

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
      EvtData_004_005_408 evtf004;
      EvtData_022_029 evtf022;
      EvtData_023 evtf023;
      EvtData_025 evtf025;
      EvtData_031 evtf031;
      EvtData_405 evtf405;
      EvtData_421_422 evtf421;
      EvtData_438 evtf438;
      EvtData_564_565_566 evtf564;
      EvtData_571 evtf571;
      EvtData_573 evtf573;
      EvtData_581 evtf581;
   } d;
} EvtData;

typedef void (*EvtFunction)(EvtData *evt);

extern EvtData gEvtDataArray[EVT_DATA_CT];
extern EvtFunction gEvtFunctionPointers[804];
extern EvtData gEvtData050_UnitSpritesDecoder;
extern EvtData *gTempGfxEvt;
extern EvtData *gTempEvt;

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

#endif
