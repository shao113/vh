#ifndef COMMON_H
#define COMMON_H

#define NULL 0
#define ASCII_DIGIT 0x30

#include "sys/types.h"
#include "PsyQ/libetc.h"

#include "types.h"
/*#include "include_asm.h"*/

typedef u16 BigInt[8];

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

extern u8 gScratch1_801317c0[];
extern u8 gScratch2_8013e054[];
extern u8 gScratch3_80180210[];

extern s32 gPadStateNewPresses;

#endif
