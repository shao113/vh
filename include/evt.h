#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "graphics.h"

#define EVT_DATA_CT 350
#define EVT_DATA_LAST_IDX 349

#define TO_TILE(arg) (*((s8 *)&arg + 1))
//#define TO_TILE(arg) ((s8)arg)

typedef enum {
   EVTF_NULL = 0,
   EVTF_NOOP = 1, /* TBD Used only for sprites? */
   EVTF_MAP_OBJECT_CHEST = 40,
   EVTF_MAP_OBJECT_CRATE = 46,
   EVTF_UNIT_SPRITES_DECODER = 50,
   EVTF_FILE_SAVE_DIALOG = 341,
   EVTF_FILE_LOAD_DIALOG = 373,
   EVTF_FULLSCREEN_IMAGE = 387,
   EVTF_EVALUATE_BATTLE_10 = 426,
   EVTF_EVALUATE_BATTLE_08 = 438,
   EVTF_DISPLAY_ICON = 574,
   EVTF_AUDIO_CMD = 581,
   EVTF_MAP_OBJECT_BOULDER = 591,
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
   /* :0x28 */ u16 gfxIdx;
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

/* Evaluate Battle 08 */
typedef struct EvtData_438 {
   /* :0x10 */ u8 unk_0x10[20];
   /* :0x24 */ s8 delay;
   /* :0x25 */ u8 unk_0x25[59];
} EvtData_438;

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
   /* 0x0A */ u16 mem;
   /* 0x0C */ s16 state;
   /* 0x0E */ s16 state2;
   /* 0x10: */
   union {
      u8 bytes[80];
      EvtData_Sprite sprite;
      EvtData_438 evtf438;
      EvtData_581 evtf581;
   } d;
} EvtData;

typedef void (*EvtFunction)(EvtData *evt);

extern EvtData gEvtDataArray[EVT_DATA_CT];
extern EvtFunction gEvtFunctionPointers[804];
extern EvtData gEvtData050_UnitSpritesDecoder;
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
