#ifndef EVT_H
#define EVT_H

#include "common.h"
#include "PsyQ/libgte.h"

#define EVT_DATA_CT 350

typedef struct SVectorXZY {
   s16 x;
   s16 z;
   s16 y;
} SVectorXZY;

/* TBD Not necessary? Can just cast to s8? */
typedef struct CoordinateParts {
   s8 frac;
   s8 tile;
} CoordinateParts;

typedef union Coordinate {
   s16 n;
   CoordinateParts c;
} Coordinate;

typedef enum {
   EVTF_NULL = 0,
   EVTF_NOOP = 1, /* TBD Used only for sprites? */
   EVTF_DISPLAY_ICON = 574,
   EVTF_AUDIO_CMD = 581
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

/* Audio Command */
typedef struct EvtData_581 {
   /* :0x10 */ u8 unk_0x0E[20];
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
      EvtData_Sprite sprite;
      EvtData_581 evtf581;
   } d;
   //} d  __attribute__((__packed__));
} EvtData;

extern EvtData gEvtDataArray[EVT_DATA_CT];

#endif
