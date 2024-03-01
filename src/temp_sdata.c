#include "common.h"
#include "object.h"
#include "state.h"
#include "graphics.h"
#include "audio.h"

static u8 D_80122E10[] = " ";
static u8 D_80122E12 = 0;
static u8 D_80122E13 = 0;
static u8 D_80122E14 = 0;
u8 gOverheadMapState = 0;
static u8 D_80122E16 = 0;
static u8 D_80122e17 = 0;

SVECTOR gCameraRotation = {0};
SVECTOR gLightRotation = {0};

/*s16 gCameraRotation_vx = 0;
s16 gCameraRotation_vy = 0;
s16 gCameraRotation_vz = 0;
*/