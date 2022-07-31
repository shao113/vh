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

typedef struct DynamicIcon {
   u16 gfxIdx;
   s16 x;
   s16 y;
} DynamicIcon;

extern Graphics *gGraphicsPtr;
extern SVECTOR gLightRotation;
extern s16 gLightRotation_vy, gLightRotation_vz; /* FIXME */
extern SVECTOR gCameraRotation;
extern s16 gCameraRotation_vy, gCameraRotation_vz; /* FIXME */

void DecodeUnitSprites(void);

#endif
