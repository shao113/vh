#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"
#include "PsyQ/libgte.h"
#include "PsyQ/libgpu.h"

#define OT_SIZE 1024
#define GFX_CT 870

typedef enum GfxIdx {
   GFX_PLAYER_CIRCLE = 2,
   GFX_ENEMY_CIRCLE = 3,
   GFX_GRAVE_MARKER = 4,
   GFX_TORCH_1 = 5,
   GFX_TORCH_2 = 6,
   GFX_TORCH_3 = 7,
   GFX_TORCH_4 = 8,
   GFX_YOU_1 = 9,
   GFX_YOU_2 = 10,
   GFX_WIN_1 = 11,
   GFX_WIN_2 = 12,
   GFX_LOSE_1 = 13,
   GFX_LOSE_2 = 14,
   GFX_DIGIT_0 = 30,
   GFX_DIGIT_1 = 31,
   GFX_DIGIT_2 = 32,
   GFX_DIGIT_3 = 33,
   GFX_DIGIT_4 = 34,
   GFX_DIGIT_5 = 35,
   GFX_DIGIT_6 = 36,
   GFX_DIGIT_7 = 37,
   GFX_DIGIT_8 = 38,
   GFX_DIGIT_9 = 39,
   GFX_PARALYZED = 40,
   GFX_MISS = 41,
   GFX_MAP_CURSOR_1 = 46,
   GFX_MAP_CURSOR_2 = 47,
   GFX_MAP_CURSOR_3 = 48,
   GFX_MAP_CURSOR_4 = 49,
   GFX_MAP_CURSOR_5 = 50,
   GFX_MAP_CURSOR_6 = 51,
   GFX_DIRECTION_ARROW_FILLED = 52,
   GFX_DIRECTION_ARROW_EMPTY = 53,
   GFX_BLUE_GEM = 54,
   GFX_RED_GEM = 55,
   GFX_YELLOW_GEM = 56,
   GFX_GRAY_GEM = 57,
   GFX_COMPASS = 58,
   GFX_E = 59,
   GFX_W = 60,
   GFX_S = 61,
   GFX_N = 62,
   GFX_RED_X = 64,
   GFX_COLOR_1 = 74,
   GFX_COLOR_2 = 75,
   GFX_COLOR_3 = 76,
   GFX_COLOR_4 = 77,
   GFX_COLOR_5 = 78,
   GFX_COLOR_6 = 79,
   GFX_COLOR_7 = 80,
   GFX_COLOR_8 = 81,
   GFX_COLOR_9 = 82,
   GFX_COLOR_10 = 83,
   GFX_COLOR_11 = 84,
   GFX_COLOR_12 = 85,
   GFX_COLOR_13 = 86,
   GFX_COLOR_14 = 87,
   GFX_COLOR_15 = 88,
   GFX_EXPLOSION_1 = 89,
   GFX_EXPLOSION_2 = 90,
   GFX_EXPLOSION_3 = 91,
   GFX_EXPLOSION_4 = 92,
   GFX_EXPLOSION_5 = 93,
   GFX_EXPLOSION_6 = 94,
   GFX_EXPLOSION_7 = 95,
   GFX_EXPLOSION_8 = 96,
   GFX_EXPLOSION_9 = 97,
   GFX_EXPLOSION_10 = 98,
   GFX_EXPLOSION_11 = 99,
   GFX_DOTS = 123,
   GFX_SWIRLY_RING = 131,
   GFX_SWIRLY_RING_TOP = 132,
   GFX_SWIRLY_RING_BTM = 133,
   GFX_RING = 134,
   GFX_RING_TOP = 135,
   GFX_RING_BTM = 136,
   GFX_RUNE_1 = 137,
   GFX_RUNE_2 = 138,
   GFX_RUNE_3 = 139,
   GFX_RUNE_4 = 140,
   GFX_RUNE_5 = 141,
   GFX_RUNE_6 = 142,
   GFX_RUNE_7 = 143,
   GFX_RUNE_8 = 144,
   GFX_RUNE_9 = 145,
   GFX_RUNE_10 = 146,
   GFX_RUNE_11 = 147,
   GFX_RUNE_12 = 148,
   GFX_LIGHTNING_1 = 149,
   GFX_LIGHTNING_2 = 150,
   GFX_LIGHTNING_3 = 151,
   GFX_LIGHTNING_4 = 152,
   GFX_LIGHTNING_5 = 153,
   GFX_LIGHTNING_6 = 154,
   GFX_LIGHTNING_7 = 155,
   GFX_LIGHTNING_8 = 156,
   GFX_ARROW = 212,
   GFX_FOUNTAIN_1 = 219,
   GFX_FOUNTAIN_2 = 220,
   GFX_FLAG_1 = 221,
   GFX_FLAG_2 = 222,
   GFX_FLAG_3 = 223,
   GFX_FLAG_4 = 224,
   GFX_LAMP_POST = 225,
   GFX_TREE = 226,
   GFX_MAP_MARKER = 227,
   GFX_SOLID_CIRCLE = 235,
   GFX_CREST = 247,
   GFX_DAGGER = 263,
   GFX_CRATE = 264,
   GFX_SWORD = 265,
   GFX_SHIELD = 266,
   GFX_BLUE_CRYSTAL = 267,
   GFX_BLUE_SHARD = 268,
   GFX_SUPPORT = 269,
   GFX_PANORAMA_DEEP_BLUE = 311,
   GFX_MSGBOX_TAIL_DOWN = 313,
   GFX_MSGBOX_TAIL_RIGHT = 314,
   GFX_MSGBOX_TAIL_LEFT = 315,
   GFX_CHEST_1 = 320,
   GFX_CHEST_2 = 321,
   GFX_CHEST_3 = 322,
   GFX_CHEST_4 = 323,
   GFX_PANORAMA_2 = 324,
   GFX_PANORAMA_3 = 325,
   GFX_PANORAMA_4 = 326,
   GFX_PANORAMA_RED = 327,
   GFX_PANORAMA_6 = 328,
   GFX_PANORAMA_7 = 329,
   GFX_PANORAMA_8 = 330,
   GFX_CAMPFIRE_1 = 331,
   GFX_CAMPFIRE_2 = 332,
   GFX_CAMPFIRE_3 = 333,
   GFX_CAMPFIRE_4 = 334,
   GFX_BOMB = 335,
   GFX_NOW_LOADING = 336,
   GFX_VANDAL_HEARTS = 337,
   GFX_LV = 338,
   GFX_UP = 339,
   GFX_POISONED = 340,
   GFX_AT = 341,
   GFX_DF = 342,
   GFX_MV = 343,
   GFX_SCROLL_INDICATOR = 344,
   GFX_COLORS = 346,
   GFX_MAP_TEXTURES_OFS = 400,
   GFX_MAP_TEXTURE_1 = 401,
   GFX_MAP_TEXTURE_4 = 404,
   GFX_MAP_TEXTURE_5 = 405,
   GFX_MAP_TEXTURE_7 = 407,
   GFX_MAP_TEXTURE_21 = 421,
   GFX_MAP_TEXTURE_23 = 423,
   GFX_MAP_TEXTURE_51 = 451,
   GFX_MAP_TEXTURE_52 = 452,
   GFX_MAP_TEXTURE_90 = 490,
   GFX_MAP_TEXTURE_128 = 528,
   GFX_MAP_TEXTURE_198 = 598,
   GFX_WINDOW_TBD_657 = 657,
   GFX_WINDOW_TBD_658 = 658,
   GFX_WINDOW_TBD_707 = 707,
   GFX_WINDOW_TBD_708 = 708,
   GFX_ITEM_ICONS_OFS = 727,
} GfxIdx;

typedef struct Graphics {
   DRAWENV drawEnv;
   DISPENV dispEnv;
   POLY_FT4 quads[1300];
   u32 ot[OT_SIZE];
} Graphics;

typedef SVECTOR Quad[4];

typedef struct TextureWindow {
   u8 x, y, w, h;
} TextureWindow;

typedef struct PortraitOverlayOffsets {
   u8 blinkX, blinkY, speakX, speakY;
} PortraitOverlayOffsets;

typedef struct PortraitsDb {
   u8 cluts[700];
   s16 sceneSets[103][26];
} PortraitsDb;

extern Graphics gGraphicBuffers[2];
extern Graphics *gGraphicsPtr;
extern SVECTOR gLightRotation;
// extern s16 gLightRotation_vy, gLightRotation_vz; /* FIXME */
extern MATRIX gCameraMatrix;
extern SVECTOR gCameraPos;
extern SVECTOR gCameraRotation;
/* FIXME - static inline getters? */
// extern s16 gCameraRotation_vx, gCameraRotation_vy, gCameraRotation_vz;
extern VECTOR gCameraZoom;
extern s32 gGeomOffsetX, gGeomOffsetY;
extern s32 gQuadIndex;
extern u16 gOscillation;
extern u8 gGridColorOscillation;
extern s32 gDecodingSprites;

// Hard-coded values start as indices into gClutIds & are replaced w/ actual CLUT IDs in SetupGfx()
extern u16 gGfxClutIds[GFX_CT];
extern u16 gGfxTPageIds[GFX_CT];
extern s16 gGfxTPageCells[GFX_CT];
extern TextureWindow gGfxSubTextures[GFX_CT];
extern u16 gTPageIds[4][32];

extern PortraitOverlayOffsets gPortraitOverlayOffsetsDb[692];
extern PortraitsDb gPortraitsDb;

extern Quad gQuad_800fe63c;

void DecodeUnitSprites(void);
void StartUnitSpritesDecoder(u8);
struct EvtData *GetUnitSpriteAtPosition(u8, u8);

void AddEvtPrim_Gui(u32 *ot, struct EvtData *evt);

static inline s16 GetCamRotX() { return gCameraRotation.vx; }
static inline s16 GetCamRotY() { return gCameraRotation.vy; }
static inline u16 GetLightRotY() { return gLightRotation.vy; }
static inline s16 GetCamPosY() { return gCameraPos.vy; }
static inline s32 GetCamZoom() { return gCameraZoom.vz; }

#endif
