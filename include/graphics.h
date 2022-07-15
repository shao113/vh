#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"
#include "PsyQ/libgpu.h"
//#include "PsyQ/libgte.h"

typedef struct Graphics {
   DRAWENV drawEnv;
   DISPENV dispEnv;
   POLY_FT4 quads[1300];
   u32 ot[1024];
} Graphics;

extern Graphics *gGraphicsPtr;

#endif
