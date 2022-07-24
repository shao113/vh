#ifndef COMMON_H
#define COMMON_H

#define NULL 0

#include "sys/types.h"
#include "PsyQ/libetc.h"

#include "types.h"
/*#include "include_asm.h"*/

typedef u16 BigInt[8];

typedef struct State {
   s32 primary, secondary, field2_0x8, field3_0xc, field4_0x10, field5_0x14, field6_0x18,
       field7_0x1c;
} State;

extern void *gScratch1_801317c0;
extern void *gScratch2_8013e054;
extern void *gScratch3_80180210;

extern s32 gPadStateNewPresses;
extern State gState;

#endif
