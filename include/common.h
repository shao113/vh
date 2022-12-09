#ifndef COMMON_H
#define COMMON_H

#include "sys/types.h"
#include "PsyQ/libetc.h"

#include "types.h"
/*#include "include_asm.h"*/

#define NULL 0
#define ASCII_DIGIT 0x30

#define PAD_UP PADLup
#define PAD_DOWN PADLdown
#define PAD_LEFT PADLleft
#define PAD_RIGHT PADLright
#define PAD_TRIANGLE PADRup
#define PAD_X PADRdown
#define PAD_SQUARE PADRleft
#define PAD_CIRCLE PADRright
#define PAD_L1 PADL1
#define PAD_L2 PADL2
#define PAD_R1 PADR1
#define PAD_R2 PADR2
#define PAD_START PADstart
#define PAD_SELECT PADselect

#define UNIT_CT 40
#define PARTY_CT 13
#define DEPOT_CT 150
#define BATTLE_CT 44

#define H_LO(arg) ((s8 *)&arg)[0]
#define H_HI(arg) ((s8 *)&arg)[1]
#define TO_TILE(arg) (*((s8 *)&arg + 1))

typedef u16 BigInt[8];

struct EvtData;

typedef enum Direction { DIR_SOUTH = 0, DIR_WEST = 1, DIR_NORTH = 2, DIR_EAST = 3 } Direction;

typedef struct SVectorXZY {
   s16 x;
   s16 z;
   s16 y;
} SVectorXZY;

typedef struct SVectorXYZ {
   s16 x;
   s16 y;
   s16 z;
} SVectorXYZ;

typedef struct BVectorZXY {
   u8 z;
   u8 x;
   u8 y;
} BVectorZXY;

extern u8 gScratch1_801317c0[];
extern u8 gScratch2_8013e054[];
extern u8 gScratch3_80180210[];

extern s32 gSavedPadState;
extern s32 gSavedPad2State;
extern s32 gSavedPadStateNewPresses;
extern s32 gSavedPad2StateNewPresses;
extern s32 gPadState;
extern s32 gPad2State;
extern s32 gPadStateNewPresses;
extern s32 gPad2StateNewPresses;
extern s32 gClearSavedPadState;

extern u8 gPlayerControlSuppressed;
extern u8 gMapCursorSuppressed;
extern s32 gSignal1, gSignal2;

#endif
