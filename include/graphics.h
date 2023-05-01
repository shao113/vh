#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "common.h"
#include "PsyQ/libgte.h"
#include "PsyQ/libgpu.h"

#define OT_SIZE 1024
#define GFX_CT 870

typedef enum GfxIdx {
   GFX_NULL = 0,
   GFX_ENEMY_CIRCLE = 2,
   GFX_PLAYER_CIRCLE = 3,
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
   GFX_PORTRAIT_A_FACE = 17,
   GFX_PORTRAIT_A_SPEAK_1 = 18,
   GFX_PORTRAIT_A_SPEAK_2 = 19,
   GFX_PORTRAIT_A_BLINK = 20,
   GFX_PORTRAIT_B_FACE = 21,
   GFX_PORTRAIT_B_SPEAK_1 = 22,
   GFX_PORTRAIT_B_SPEAK_2 = 23,
   GFX_PORTRAIT_B_BLINK = 24,
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
   GFX_SELECTION_ARROW = 65,
   GFX_IMPACT_1 = 67,
   GFX_IMPACT_2 = 68,
   GFX_IMPACT_3 = 69,
   GFX_IMPACT_4 = 70,
   GFX_IMPACT_5 = 71,
   GFX_IMPACT_6 = 72,
   GFX_IMPACT_7 = 73,
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
   GFX_SPARKLE_1 = 100,
   GFX_SPARKLE_2 = 101,
   GFX_SPARKLE_3 = 102,
   GFX_SPARKLE_4 = 103,
   GFX_SPARKLE_5 = 104,
   GFX_FLAME_1 = 105,
   GFX_FLAME_2 = 106,
   GFX_FLAME_3 = 107,
   GFX_FLAME_4 = 108,
   GFX_FLAME_5 = 109,
   GFX_FLAME_6 = 110,
   GFX_FLAME_7 = 111,
   GFX_FLAME_8 = 112,
   GFX_PUFF_1 = 113,
   GFX_PUFF_2 = 114,
   GFX_PUFF_3 = 115,
   GFX_PUFF_4 = 116,
   GFX_PUFF_5 = 117,
   GFX_PUFF_6 = 118,
   GFX_PUFF_7 = 119,
   GFX_PUFF_8 = 120,
   GFX_PUFF_9 = 121,
   GFX_PUFF_10 = 122,
   GFX_DOTS = 123,
   GFX_BOULDER_1 = 124,
   GFX_BOULDER_2 = 125,
   GFX_ROCK_1 = 126,
   GFX_ROCK_2 = 127,
   GFX_ROCK_3 = 128,
   GFX_ROCK_4 = 129,
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
   GFX_TREE_SNOWY = 302,
   GFX_TREE_EVERGREEN = 303,
   GFX_TREE_PALM = 304,
   GFX_FAIRY_1 = 305,
   GFX_FAIRY_2 = 306,
   GFX_PANORAMA_DEEP_BLUE = 311,
   GFX_TREE_BARE = 312,
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
   GFX_STRIP_SPRITES_OFS = 347,
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
   GFX_TBD_685 = 685,
   GFX_WINDOW_TBD_707 = 707,
   GFX_WINDOW_TBD_708 = 708,
   GFX_ITEM_ICONS_OFS = 727,
} GfxIdx;

typedef enum AnimIdx {
   ANIM_IDLE_B = 0,
   ANIM_IDLE_F = 1,
   ANIM_STRUCK_B = 2,
   ANIM_STRUCK_F = 3,
   ANIM_BLOCKING_B = 4,
   ANIM_BLOCKING_F = 5,
   ANIM_WALKING_B = 6,
   ANIM_WALKING_F = 7,
   ANIM_JUMPING_B = 8,
   ANIM_JUMPING_F = 9,
   ANIM_CROUCHING_B = 10,
   ANIM_CROUCHING_F = 11,
   ANIM_ATTACKING_B = 12,
   ANIM_ATTACKING_F = 13,
   ANIM_ATTACKING_END_B = 14,
   ANIM_ATTACKING_END_F = 15,
   ANIM_CASTING_B = 16,
   ANIM_CASTING_F = 17,
   ANIM_STRUCK_END_B = 18,
   ANIM_STRUCK_END_F = 19,
   ANIM_BLOCKING_END_B = 20,
   ANIM_BLOCKING_END_F = 21,
   ANIM_HOP_B = 22,
   ANIM_HOP_F = 23,
   ANIM_HIGH_STEP_B = 24,
   ANIM_HIGH_STEP_F = 25,
   ANIM_ATTACKING_FAR_B = 26,
   ANIM_ATTACKING_FAR_F = 27,
   ANIM_ATTACKING_FAR_END_B = 28,
   ANIM_ATTACKING_FAR_END_F = 29
} AnimIdx;

/*typedef enum SpriteBoxIdx {
   SPRITE_BOX_0 = 0,
   SPRITE_BOX_1 = 1,
   SPRITE_BOX_2 = 2,
   SPRITE_BOX_3 = 3,
   SPRITE_BOX_4 = 4,
   SPRITE_BOX_5 = 5,
   SPRITE_BOX_6 = 6,
   SPRITE_BOX_7 = 7,
   SPRITE_BOX_8 = 8,
   SPRITE_BOX_9 = 9,
   SPRITE_BOX_10 = 10,
   SPRITE_BOX_11 = 11,
   SPRITE_BOX_12 = 12,
   SPRITE_BOX_13 = 13,
   SPRITE_BOX_14 = 14,
   SPRITE_BOX_15 = 15,
   SPRITE_BOX_16 = 16,
   SPRITE_BOX_17 = 17,
   SPRITE_BOX_18 = 18
} SpriteBoxIdx;*/

typedef enum GpuCode {
   GPU_CODE_SEMI_TRANS = 0x02,
   GPU_CODE_POLY_FT4 = 0x2c,
} GpuCode;

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

/* Partial; defining as needed */
// neutral,injured,happy,angry,upset
typedef enum PortraitId {
   PORTRAIT_ASH = 1,
   PORTRAIT_ASH_ANGRY = 4,
   PORTRAIT_ASH_UPSET = 5,
   PORTRAIT_ASH_6 = 6,
   PORTRAIT_CLINT = 8,
   PORTRAIT_CLINT_UPSET = 12,
   PORTRAIT_DIEGO = 15,
   PORTRAIT_ELENI_25 = 25,
   PORTRAIT_KIRA = 36,
   PORTRAIT_KIRA_ANGRY = 39,
   PORTRAIT_DOLAN = 50,
   PORTRAIT_AMON_HAPPY = 59,
   PORTRAIT_AMON_ANGRY = 60,
   PORTRAIT_SARA = 64,
   PORTRAIT_SARA_HAPPY = 66,
   PORTRAIT_ZOHAR = 71,
   PORTRAIT_ZOOT_449 = 449,
   PORTRAIT_C_KNIGHT = 463,
   PORTRAIT_HASSAN_INJURED = 478,
   PORTRAIT_KANE = 480,
   PORTRAIT_KANE_INJURED = 481,
   PORTRAIT_KANE_HAPPY = 482,
   PORTRAIT_KANE_ANGRY = 483,
   PORTRAIT_KANE_BLK_KNIGHT = 485,
   PORTRAIT_KANE_BLK_KNIGHT_INJURED = 486,
   PORTRAIT_MAGNUS_DARK_LORD_492 = 492,
   PORTRAIT_DOLF_495 = 495,
   PORTRAIT_SABINA = 506,
   PORTRAIT_SABINA_508 = 508,
   PORTRAIT_SABINA_509 = 509,
   PORTRAIT_KURTZ_511 = 511,
   PORTRAIT_LANDO_514 = 514,
   PORTRAIT_LANDO_INJURED = 515,
   PORTRAIT_DALLAS_516 = 516,
   PORTRAIT_DALLAS_517 = 517,
   PORTRAIT_DALLAS_518 = 518,
   PORTRAIT_ZOOT_559 = 559,
   PORTRAIT_JUGGERNAUT = 560,
   PORTRAIT_DUMAS = 561,
   PORTRAIT_DUMAS_562 = 562,
   PORTRAIT_LEENA = 571,
   PORTRAIT_LEENA_573 = 573,
   PORTRAIT_DOLF_581 = 581,
   PORTRAIT_XENO = 585,
   PORTRAIT_XENO_INJURED = 586,
   PORTRAIT_590 = 590,
   PORTRAIT_591 = 591,
   PORTRAIT_592 = 592,
   PORTRAIT_593 = 593,
   PORTRAIT_595 = 595,
   PORTRAIT_596 = 596,
   PORTRAIT_597 = 597,
   PORTRAIT_598 = 598,
   PORTRAIT_599 = 599,
   PORTRAIT_600 = 600,
   PORTRAIT_601 = 601,
   PORTRAIT_602 = 602,
   PORTRAIT_605 = 605,
   PORTRAIT_606 = 606,
   PORTRAIT_607 = 607,
   PORTRAIT_609 = 609,
   PORTRAIT_610 = 610,
   PORTRAIT_611 = 611,
   PORTRAIT_612 = 612,
   PORTRAIT_614 = 614,
   PORTRAIT_615 = 615,
   PORTRAIT_616 = 616,
   PORTRAIT_618 = 618,
   PORTRAIT_619 = 619,
   PORTRAIT_620 = 620,
   PORTRAIT_622 = 622,
   PORTRAIT_623 = 623,
   PORTRAIT_624 = 624,
   PORTRAIT_626 = 626,
   PORTRAIT_627 = 627,
   PORTRAIT_628 = 628,
   PORTRAIT_630 = 630,
   PORTRAIT_631 = 631,
   PORTRAIT_632 = 632,
   PORTRAIT_633 = 633,
   PORTRAIT_634 = 634,
   PORTRAIT_635 = 635,
   PORTRAIT_636 = 636,
   PORTRAIT_637 = 637,
   PORTRAIT_639 = 639,
   PORTRAIT_SALAMANDER = 655,
   PORTRAIT_APPARITION_675 = 675,
   PORTRAIT_CLIVE_INJURED = 677,
   PORTRAIT_KANE_APPARITION = 678,
   PORTRAIT_APPARITION_681 = 681,
   PORTRAIT_682 = 682,
   PORTRAIT_KANE_CURSED = 684,
   PORTRAIT_JUGGERNAUT_INJURED = 686,
   PORTRAIT_687 = 687,
   PORTRAIT_688 = 688,
   PORTRAIT_689 = 689,
   PORTRAIT_690 = 690,
} PortraitId;

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
extern CVECTOR gLightColor;
extern MATRIX gCameraMatrix;
extern SVECTOR gCameraPos;
extern SVECTOR gCameraRotation;
/* FIXME - static inline getters? */
// extern s16 gCameraRotation_vx, gCameraRotation_vy, gCameraRotation_vz;
extern VECTOR gCameraZoom;
extern s32 gGeomOffsetX, gGeomOffsetY;
extern s32 gQuadIndex;
extern s16 gOscillation;
extern u8 gGridColorOscillation;
extern s32 gDecodingSprites;
extern RECT gTempRect;

// For each idx of gCurrentUnitSet, stores the corresponding idx of gEncodedUnitSpriteData
extern s16 gUnitSetEncodedSpriteDataIdx[20];
extern void *gEncodedUnitSpriteData[20];
extern u16 gSpriteStripClutIds[25];
extern s16 gSpriteStripTPageCells[50];
extern s16 gSpriteStripTPageIds[50];
extern s16 gSpriteStripUnitIds[23];
extern u8 **gSpriteStripAnimSets[25];
extern u8 gUnitClutIds[492];

// Mix of 48x48 and 64x48 px frames (tpages 11 & 27); wider frames allow for e.g. slash animations
extern TextureWindow gTexwSpriteSetFrames[35];
// 48x48 px frames
extern TextureWindow gTexwSpriteStripFrames[25];
// 96x48 px frames (tpages 11 & 27)
extern TextureWindow gTexwWideSpriteSetFrames[10];

// Hard-coded values start as indices into gClutIds & are replaced w/ actual CLUT IDs in SetupGfx()
extern u16 gGfxClutIds[GFX_CT];
extern u16 gGfxTPageIds[GFX_CT];
extern s16 gGfxTPageCells[GFX_CT];
extern TextureWindow gGfxSubTextures[GFX_CT];
// extern u16 gTPageIds[4][32];
extern u16 gTPageIds[128];

// ?: Going off of the loop at {@addr 0x8005cd94}, this should
// have 128 elements, but the last few bytes ({@addr 0x8014014c})
// are used in a completely unrelated manner: {@addr 800c471c}
extern u16 gClutIds[124];

extern PortraitOverlayOffsets gPortraitOverlayOffsetsDb[692];
extern PortraitsDb gPortraitsDb;
extern PortraitOverlayOffsets gPortraitOverlayOffsets[50];
extern u8 gPortraitClutIds[75];

extern Quad gQuad_800fe53c;
extern Quad gQuad_800fe63c;
extern Quad gQuad_800fe65c;
extern Quad *gSpriteBoxQuads[19];

void DecodeUnitSprites(void);
void StartUnitSpritesDecoder(u8);
struct EvtData *GetUnitSpriteAtPosition(u8, u8);

void AddEvtPrim_Gui(u32 *ot, struct EvtData *evt);

static inline u16 GetLightRotY() { return gLightRotation.vy; }
static inline s32 GetCamZoom() { return gCameraZoom.vz; }

static inline s16 GetCamPosX() { return gCameraPos.vx; }
static inline s16 GetCamPosY() { return gCameraPos.vy; }
static inline s16 GetCamPosZ() { return gCameraPos.vz; }

static inline s16 GetCamRotX() { return gCameraRotation.vx; }
static inline s16 GetCamRotY() { return gCameraRotation.vy; }
static inline s16 GetCamDir() { return gCameraRotation.vy >> 10; }
static inline u16 GetCamRotY_U() { return gCameraRotation.vy; }

/*static inline void SetCamPosX(s32 value) { gCameraPos.vx = value; }
static inline void SetCamPosY(s32 value) { gCameraPos.vy = value; }
static inline void SetCamPosZ(s32 value) { gCameraPos.vz = value; }
static inline void SetCamRotY(s32 value) { gCameraRotation.vy = value; }*/

static inline void OffsetCamPosX(s32 value) { gCameraPos.vx = GetCamPosX() + value; }
static inline void OffsetCamPosY(s32 value) { gCameraPos.vy = GetCamPosY() + value; }
static inline void OffsetCamPosZ(s32 value) { gCameraPos.vz = GetCamPosZ() + value; }
static inline void OffsetCamRotY(s32 value) { gCameraRotation.vy = GetCamRotY() + value; }

#endif
