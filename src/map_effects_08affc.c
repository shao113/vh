#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

static MapTileModel sMapTileModel_80100514 = {
    .vertices =
        {
            [0] = {-32, 0, 16, 0},   [1] = {0, 16, 16, 0},    [2] = {0, 16, -16, 0},
            [3] = {-32, 16, -16, 0}, [4] = {-32, 16, 16, 0},  [5] = {0, 0, 16, 0},
            [6] = {0, 0, -16, 0},    [7] = {-32, 0, -16, 0},  [8] = {-64, 16, -16, 0},
            [9] = {-64, 16, 16, 0},  [10] = {-64, 0, -16, 0}, [11] = {-64, 0, 16, 0},
            [12] = {-96, 0, -16, 0}, [13] = {-96, 0, 16, 0},  [14] = {-96, 16, -16, 0},
            [15] = {-96, 16, 16, 0}, [16] = {0, 0, 0, 0},     [17] = {0, 0, 0, 0},
            [18] = {0, 0, 0, 0},     [19] = {0, 0, 0, 0},     [20] = {0, 0, 0, 0},
            [21] = {0, 0, 0, 0},
        },
    .gfx =
        {
            [0] = 152,
            [1] = 147,
            [2] = 142,
            [3] = 142,
            [4] = 147,
            [5] = 152,
            [6] = 0,
            [7] = 0,
            [8] = 0,
            [9] = 0,
            [10] = 0,
            [11] = 0,
            [12] = 0,
            [13] = 0,
            [14] = 0,
            [15] = 0,
            [16] = 0,
            [17] = 0,
        },
    .faces =
        {
            [0] = {7, 0, 5, 6},
            [1] = {10, 11, 0, 7},
            [2] = {12, 13, 11, 10},
            [3] = {2, 1, 4, 3},
            [4] = {3, 4, 9, 8},
            [5] = {8, 9, 15, 14},
            [6] = {6, 5, 1, 2},
            [7] = {5, 0, 4, 1},
            [8] = {0, 11, 9, 4},
            [9] = {11, 13, 15, 9},
            [10] = {13, 12, 14, 15},
            [11] = {12, 10, 8, 14},
            [12] = {10, 7, 3, 8},
            [13] = {7, 6, 2, 3},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 14,
    .height = 16,
};

static MapTileModel sMapTileModel_80100644 = {
    .vertices =
        {
            [0] = {-32, -16, 16, 0},   [1] = {0, 0, 16, 0},       [2] = {0, 0, -16, 0},
            [3] = {-32, 0, -16, 0},    [4] = {-32, 0, 16, 0},     [5] = {0, -16, 16, 0},
            [6] = {0, -16, -16, 0},    [7] = {-32, -16, -16, 0},  [8] = {-64, 0, -16, 0},
            [9] = {-64, 0, 16, 0},     [10] = {-64, -16, -16, 0}, [11] = {-64, -16, 16, 0},
            [12] = {-96, -16, -16, 0}, [13] = {-96, -16, 16, 0},  [14] = {-96, 0, -16, 0},
            [15] = {-96, 0, 16, 0},    [16] = {0, 0, 0, 0},       [17] = {0, 0, 0, 0},
            [18] = {0, 0, 0, 0},       [19] = {0, 0, 0, 0},       [20] = {0, 0, 0, 0},
            [21] = {0, 0, 0, 0},
        },
    .gfx =
        {
            [0] = 152,
            [1] = 147,
            [2] = 142,
            [3] = 142,
            [4] = 147,
            [5] = 152,
            [6] = 218,
            [7] = 218,
            [8] = 218,
            [9] = 218,
            [10] = 218,
            [11] = 218,
            [12] = 218,
            [13] = 218,
            [14] = 0,
            [15] = 0,
            [16] = 0,
            [17] = 0,
        },
    .faces =
        {
            [0] = {7, 0, 5, 6},
            [1] = {10, 11, 0, 7},
            [2] = {12, 13, 11, 10},
            [3] = {2, 1, 4, 3},
            [4] = {3, 4, 9, 8},
            [5] = {8, 9, 15, 14},
            [6] = {6, 5, 1, 2},
            [7] = {5, 0, 4, 1},
            [8] = {0, 11, 9, 4},
            [9] = {11, 13, 15, 9},
            [10] = {13, 12, 14, 15},
            [11] = {12, 10, 8, 14},
            [12] = {10, 7, 3, 8},
            [13] = {7, 6, 2, 3},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 14,
    .height = 16,
};

static s16 sGfxSets_80100774[9][18] = {
    {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 142,
     GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 152,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 153, GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 143,
     GFX_MAP_TEXTURES_OFS + 143, GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 153,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 142,
     GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 152,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 221,
     GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 221,
     GFX_MAP_TEXTURES_OFS + 251, GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 254,
     GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 250, GFX_MAP_TEXTURES_OFS + 254,
     GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 254, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 220, GFX_MAP_TEXTURES_OFS + 220, GFX_MAP_TEXTURES_OFS + 220,
     GFX_MAP_TEXTURES_OFS + 220, GFX_MAP_TEXTURES_OFS + 220, GFX_MAP_TEXTURES_OFS + 220,
     GFX_MAP_TEXTURES_OFS + 250, GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 254,
     GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 251, GFX_MAP_TEXTURES_OFS + 254,
     GFX_MAP_TEXTURES_OFS + 254, GFX_MAP_TEXTURES_OFS + 254, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 142,
     GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 152, GFX_NULL,
     GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL, GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 142,
     GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 152,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 153, GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 143,
     GFX_MAP_TEXTURES_OFS + 143, GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 153,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219,
     GFX_MAP_TEXTURES_OFS + 219, GFX_MAP_TEXTURES_OFS + 219, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL},
    {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 142,
     GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147, GFX_MAP_TEXTURES_OFS + 152,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218,
     GFX_MAP_TEXTURES_OFS + 218, GFX_MAP_TEXTURES_OFS + 218, GFX_NULL, GFX_NULL, GFX_NULL,
     GFX_NULL}};

void Map11_LowerDrawbridge(void) {
   s16 textures[6] = {GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 147,
                      GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 143,
                      GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 153};

   s32 ix, iz, idx;
   MapTileModel *tileModel;

   idx = 0;
   for (iz = 3; iz <= 4; iz++) {
      for (ix = 6; ix <= 8; ix++) {
         tileModel = &gMapRowPointers[iz][ix];
         AdjustFaceElevation(tileModel, 0, -32);
         AdjustFaceElevation(tileModel, 10, -16);
         gMapRowPointers[iz][ix].gfx[0] = textures[idx++];
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_PLAINS;
      }
   }
}

void Map26_LowerDrawbridge(void) {
   s16 textures[6] = {GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 221,
                      GFX_MAP_TEXTURES_OFS + 221, GFX_MAP_TEXTURES_OFS + 220,
                      GFX_MAP_TEXTURES_OFS + 220, GFX_MAP_TEXTURES_OFS + 220};

   s32 ix, iz, idx;
   MapTileModel *tileModel;

   idx = 0;
   for (iz = 7; iz <= 8; iz++) {
      for (ix = 38; ix <= 40; ix++) {
         tileModel = &gMapRowPointers[iz][ix];
         AdjustFaceElevation(tileModel, 0, -32);
         AdjustFaceElevation(tileModel, 1, -16);
         gMapRowPointers[iz][ix].gfx[0] = textures[idx++];
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_PLAINS;
      }
   }

   gTerrainPtr[7][37].s.terrain = TERRAIN_PLAINS;
   gTerrainPtr[8][37].s.terrain = TERRAIN_PLAINS;
}

void Map29_LowerDrawbridge(void) {
   s16 textures[6] = {GFX_MAP_TEXTURES_OFS + 152, GFX_MAP_TEXTURES_OFS + 147,
                      GFX_MAP_TEXTURES_OFS + 142, GFX_MAP_TEXTURES_OFS + 153,
                      GFX_MAP_TEXTURES_OFS + 148, GFX_MAP_TEXTURES_OFS + 143};

   s32 ix, iz, idx;
   MapTileModel *tileModel;

   idx = 0;
   for (ix = 9; ix <= 10; ix++) {
      for (iz = 15; iz <= 17; iz++) {
         tileModel = &gMapRowPointers[iz][ix];
         AdjustFaceElevation(tileModel, 0, -32);
         AdjustFaceElevation(tileModel, 1, -16);
         gMapRowPointers[iz][ix].gfx[0] = textures[idx++];
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_PLAINS;
      }
   }
}

#undef EVTF
#define EVTF 362
void Evtf362_DrawbridgeButton(EvtData *evt) {
   extern MapTileModel s_tileModels_80124314[2];

   static VECTOR translation = {288, -48, 112, 0};

   s32 i;
   s32 gfxSetIdx;
   MapTileModel *tileModel;
   SVECTOR rot;
   MATRIX mtx;
   VECTOR rotated;
   s32 flag;
   EvtData *dust;
   s32 signedTileSize;

   switch (evt->state) {
   case 0:
      if (gState.mapNum == 26) {
         evt->x1.n = CV(38.0);
         evt->y1.n = CV(1.0);
         evt->z1.n = CV(7.5);
         evt->mem = DEG(180);
         evt->y3.n = DEG(0.703125);
         EVT.gfxSetIdx = 3;
         EVT.battleNum = 17; // i.e. mapNum - 9 (same as displayed in debug menu)
         EVT.tileModel = &sMapTileModel_80100644;
         gTerrainPtr[7][37].s.terrain = TERRAIN_NO_ENTRY;
         gTerrainPtr[8][37].s.terrain = TERRAIN_NO_ENTRY;
         gMapRowPointers[7][37].height = 8;
         gMapRowPointers[8][37].height = 8;
      } else if (gState.mapNum == 11) {
         evt->x1.n = CV(9.0);
         evt->y1.n = CV(1.5);
         evt->z1.n = CV(3.5);
         evt->mem = DEG(0);
         evt->y3.n = DEG(0.703125);
         EVT.gfxSetIdx = 0;
         EVT.battleNum = 2;
         EVT.tileModel = &sMapTileModel_80100514;
      } else if (gState.mapNum == 29) {
         evt->x1.n = CV(9.5);
         evt->y1.n = CV(1.5);
         evt->z1.n = CV(15.0);
         evt->mem = DEG(90);
         evt->y3.n = DEG(0.703125);
         EVT.gfxSetIdx = 6;
         EVT.battleNum = 20;
         EVT.tileModel = &sMapTileModel_80100514;
         gTerrainPtr[15][9].s.terrain = TERRAIN_NO_ENTRY;
         gTerrainPtr[15][10].s.terrain = TERRAIN_NO_ENTRY;
         gMapRowPointers[15][9].height = 9;
         gMapRowPointers[15][10].height = 9;
      }

      if (gState.mapState.s.field_0x0 != 0) {
         switch (EVT.battleNum) {
         case 2: // Map 11
            DepressButton(11, 1);
            Map11_LowerDrawbridge();
            break;
         case 17: // Map 26
            DepressButton(35, 8);
            Map26_LowerDrawbridge();
            break;
         case 20: // Map 29
            DepressButton(18, 12);
            Map29_LowerDrawbridge();
            break;
         }
         evt->state = 12;
         return;
      }

      s_tileModels_80124314[0] = sMapTileModel_80100514;
      s_tileModels_80124314[1] = sMapTileModel_80100514;
      gfxSetIdx = EVT.gfxSetIdx;

      for (i = 0; i < 18; i++) {
         s_tileModels_80124314[0].gfx[i] = sGfxSets_80100774[gfxSetIdx][i];
         s_tileModels_80124314[1].gfx[i] = sGfxSets_80100774[gfxSetIdx + 1][i];
      }

      evt->z3.n = 0;
      evt->state3 = DEG(90);
      evt->state++;

   // fallthrough
   case 1:
      tileModel = EVT.tileModel;
      PushMatrix();
      rot.vx = 0;
      rot.vz = DEG(90);
      rot.vy = evt->mem;
      translation.vx = evt->x1.n >> 3;
      translation.vz = evt->z1.n >> 3;
      translation.vy = -evt->y1.n >> 3;
      RotMatrix(&rot, &mtx);
      TransMatrix(&mtx, &translation);
      SetRotMatrix(&mtx);
      SetTransMatrix(&mtx);

      for (i = 0; i < ARRAY_COUNT(tileModel->vertices); i++) {
         RotTrans(&tileModel->vertices[i], &rotated, &flag);
         s_tileModels_80124314[0].vertices[i].vx = rotated.vx;
         s_tileModels_80124314[0].vertices[i].vy = rotated.vy;
         s_tileModels_80124314[0].vertices[i].vz = rotated.vz;
         s_tileModels_80124314[1].vertices[i].vx = rotated.vx;
         s_tileModels_80124314[1].vertices[i].vy = rotated.vy;
         s_tileModels_80124314[1].vertices[i].vz = rotated.vz;
         if (EVT.battleNum != 20) {
            s_tileModels_80124314[1].vertices[i].vz = s_tileModels_80124314[0].vertices[i].vz + 32;
         } else {
            s_tileModels_80124314[1].vertices[i].vx = s_tileModels_80124314[0].vertices[i].vx + 32;
         }
      }

      PopMatrix();
      UpdateMapTileLighting(&s_tileModels_80124314[0]);
      UpdateMapTileLighting(&s_tileModels_80124314[1]);
      evt->state++;
      break;

   case 2:
      if (gState.mapState.s.field_0x0 == 2) {
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(34));
         evt->state++;
      }
      if (gOverheadMapState != 0) {
         RenderOverheadMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], 128);
         RenderOverheadMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], 128);
      } else {
         RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], GRID_COLOR_NONE);
         RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], GRID_COLOR_NONE);
      }
      break;

   case 3:
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], GRID_COLOR_NONE);
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], GRID_COLOR_NONE);
      EVT.timer = 64;
      EVT.dstCamPosY = -(s_tileModels_80124314[0].vertices[12].vy << 3);
      gCameraRotation.vy &= 0xfff;
      EVT.dstCamRotY = func_800A96A8(gCameraRotation.vy, DEG(315));
      evt->state++;
      break;

   case 4:
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], GRID_COLOR_NONE);
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], GRID_COLOR_NONE);
      if (evt->mem != DEG(0)) {
         PanCamera(evt->x1.n + CV(2.0), EVT.dstCamPosY, evt->z1.n, 2);
      } else {
         PanCamera(evt->x1.n - CV(2.0), EVT.dstCamPosY, evt->z1.n, 2);
      }
      if (--EVT.timer > 0) {
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 3;
         gCameraZoom.vz += (384 - gCameraZoom.vz) >> 3;
      } else {
         EVT.timer = 64;
         evt->state++;
      }
      break;

   case 5:
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], GRID_COLOR_NONE);
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], GRID_COLOR_NONE);
      if (--evt->state2 <= 0) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(34));
         evt->state++;
      }
      break;

   case 6:
      tileModel = EVT.tileModel;
      gCameraPos.vy = -s_tileModels_80124314[0].vertices[12].vy;
      evt->y3.n += DEG(0.3515625);
      evt->state3 -= evt->y3.n;
      if (evt->state3 < DEG(0)) {
         if (evt->z3.n == 0) {
            for (i = 0; i < 2; i++) {
               dust = Evt_GetUnused();
               dust->functionIndex = EVTF_DUST_CLOUD_SPAWNER;
               signedTileSize = (evt->mem != DEG(0)) ? CV(+1.0) : CV(-1.0);
               dust->x1.n = evt->x1.s.hi * CV(1.0) + 3 * signedTileSize;
               dust->z1.n = (evt->z1.s.hi + i) * CV(1.0) + CV(0.5);
               dust->y1.n = evt->y1.n;
            }
         }
         evt->state3 = DEG(0);
         evt->z3.n++;
         evt->y3.n = -(evt->y3.n >> 1);
         if (evt->z3.n > 5) {
            switch (EVT.battleNum) {
            case 2: // Map 11
               Map11_LowerDrawbridge();
               break;
            case 17: // Map 26
               Map26_LowerDrawbridge();
               break;
            case 20: // Map 29
               Map29_LowerDrawbridge();
               break;
            }
            evt->state++;
         }
      }
      PushMatrix();
      rot.vx = 0;
      rot.vz = evt->state3;
      rot.vy = evt->mem;
      RotMatrix(&rot, &mtx);
      TransMatrix(&mtx, &translation);
      SetRotMatrix(&mtx);
      SetTransMatrix(&mtx);

      for (i = 0; i < ARRAY_COUNT(tileModel->vertices); i++) {
         RotTrans(&tileModel->vertices[i], &rotated, &flag);
         s_tileModels_80124314[0].vertices[i].vx = rotated.vx;
         s_tileModels_80124314[0].vertices[i].vy = rotated.vy;
         s_tileModels_80124314[0].vertices[i].vz = rotated.vz;
         s_tileModels_80124314[1].vertices[i].vx = rotated.vx;
         s_tileModels_80124314[1].vertices[i].vy = rotated.vy;
         s_tileModels_80124314[1].vertices[i].vz = rotated.vz;
         if (EVT.battleNum != 20) {
            s_tileModels_80124314[1].vertices[i].vz = s_tileModels_80124314[0].vertices[i].vz + 32;
         } else {
            s_tileModels_80124314[1].vertices[i].vx = s_tileModels_80124314[0].vertices[i].vx + 32;
         }
      }

      PopMatrix();
      UpdateMapTileLighting(&s_tileModels_80124314[0]);
      UpdateMapTileLighting(&s_tileModels_80124314[1]);
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[0], GRID_COLOR_NONE);
      RenderMapTile(gGraphicsPtr->ot, &s_tileModels_80124314[1], GRID_COLOR_NONE);
      break;

   case 7:
      evt->state2 = 32;
      evt->state++;

   // fallthrough
   case 8:
      if (--evt->state2 <= 0) {
         evt->state2 = 16;
         evt->state++;
         gState.mapState.s.field_0x0 = 3;
      }
      break;

   case 9:
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 10:
      evt->state++;
      break;

   case 11:
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 12:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}
