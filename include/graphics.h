#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"
#include "PsyQ/libgte.h"
#include "PsyQ/libgpu.h"

typedef struct Graphics {
   DRAWENV drawEnv;
   DISPENV dispEnv;
   POLY_FT4 quads[1300];
   u32 ot[1024];
} Graphics;

extern Graphics *gGraphicsPtr;
extern SVECTOR gLightRotation;
extern s16 gLightRotation_vy, gLightRotation_vz; /* FIXME */

void DecodeUnitSprites(void);

#endif
