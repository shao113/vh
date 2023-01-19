#ifndef COMMON_H
#define COMMON_H

#include "sys/types.h"
#include "PsyQ/libetc.h"

#include "types.h"
/*#include "include_asm.h"*/

#define NULL 0
#define ASCII_DIGIT 0x30
#define ANGLE_45_DEGREES 0x200
#define ANGLE_90_DEGREES 0x400
#define ANGLE_180_DEGREES 0x800
#define ANGLE_360_DEGREES 0x1000

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
#define UNIT_DB_CT 144

//#define ABS(x) (((x)>=0)?(x):(-(x)))
//#define ABS(arg) abs(arg)
s32 abs(s32);
#define SET_ABS(x)                                                                                 \
   do {                                                                                            \
      s32 tmp = x;                                                                                 \
      if (tmp < 0)                                                                                 \
         x = -tmp;                                                                                 \
   } while (0)
#define CLAMP(x, min, max) ((x) < (min) ? (min) : (x) > (max) ? (max) : (x))
#define LO(arg) ((s8 *)&arg)[0]
#define HI(arg) ((s8 *)&arg)[1]
#define LO_H(arg) ((s16 *)&arg)[0]
#define HI_H(arg) ((s16 *)&arg)[1]
#define TO_TILE(arg) (*((s8 *)&arg + 1))

typedef u16 BigInt[8];

struct EvtData;

// Note: On-screen compass vs internal coords: -N...S+  ;  -E...W+ (flipped)
typedef enum Direction { DIR_SOUTH = 0, DIR_WEST = 1, DIR_NORTH = 2, DIR_EAST = 3 } Direction;
typedef enum CameraDirection {
   CAM_DIR_SOUTH = 0,
   CAM_DIR_EAST = 1,
   CAM_DIR_NORTH = 2,
   CAM_DIR_WEST = 3
} CameraDirection;

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
extern s32 gSignal3; // ?: Used for signaling completion/state of various battle fx/mechanics

#endif
