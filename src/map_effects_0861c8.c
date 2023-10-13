#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "units.h"
#include "battle.h"
#include "audio.h"

//...
void func_800985F0(EvtData *evt);
//...

// The tile models so far are just boxes; that should be ok to embed, but anything more complex
// should be kept separate and #include'd instead.
static MapTileModel sMapTileModel_800ffd50 = {
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

static u8 sUnkBytes_800ffe80[264] = {
    0x0e, 0x04, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x7e, 0x00, 0x7e, 0x00, 0x07, 0x00, 0x0e, 0x05, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x08, 0x00, 0x0e, 0x06, 0x06, 0x00,
    0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x03, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00,
    0x09, 0x00, 0x0f, 0x04, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0f, 0x05, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07,
    0x0a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x0f, 0x06,
    0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x06, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x10, 0x04, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x04, 0x00, 0x10, 0x05, 0x06, 0x00, 0x04, 0x05,
    0x06, 0x07, 0x0a, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00,
    0x10, 0x06, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x06, 0x00, 0x7e, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x11, 0x04, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x11, 0x05, 0x06, 0x00,
    0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x11, 0x06, 0x06, 0x00, 0x04, 0x05, 0x06, 0x07, 0x0a, 0x00, 0x09, 0x00, 0x7e, 0x00,
    0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00};

static MapTileModel sMapTileModel_800fff88 = {
    .vertices =
        {
            [0] = {-16, -16, 16, 0}, [1] = {-16, 0, -16, 0},   [2] = {16, 0, 16, 0},
            [3] = {16, 0, -16, 0},   [4] = {-16, -16, -16, 0}, [5] = {-16, 0, 16, 0},
            [6] = {16, -16, 16, 0},  [7] = {16, -16, -16, 0},  [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},      [10] = {0, 0, 0, 0},      [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},     [13] = {0, 0, 0, 0},      [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},     [16] = {0, -64, 48, 0},   [17] = {0, -64, 48, 0},
            [18] = {-8, -64, 32, 0}, [19] = {8, -64, 32, 0},   [20] = {0, -64, 48, 0},
            [21] = {0, -112, 48, 0},
        },
    .gfx =
        {
            [0] = GFX_MAP_TEXTURE_166,
            [1] = GFX_MAP_TEXTURE_166,
            [2] = GFX_MAP_TEXTURE_167,
            [3] = GFX_MAP_TEXTURE_167,
            [4] = GFX_MAP_TEXTURE_167,
            [5] = GFX_MAP_TEXTURE_167,
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
            [0] = {0, 6, 7, 4},
            [1] = {3, 2, 5, 1},
            [2] = {6, 0, 5, 2},
            [3] = {7, 6, 2, 3},
            [4] = {4, 7, 3, 1},
            [5] = {0, 4, 1, 5},
            [6] = {0, 0, 0, 0},
            [7] = {0, 0, 0, 0},
            [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},
            [10] = {0, 0, 0, 0},
            [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},
            [13] = {0, 0, 0, 0},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 6,
    .height = 0,
};

static MapTileModel sMapTileModel_801000b8 = {
    .vertices =
        {
            [0] = {-16, -32, 16, 0}, [1] = {-16, 0, -16, 0},   [2] = {16, 0, 16, 0},
            [3] = {16, 0, -16, 0},   [4] = {-16, -32, -16, 0}, [5] = {-16, 0, 16, 0},
            [6] = {16, -32, 16, 0},  [7] = {16, -32, -16, 0},  [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},      [10] = {0, 0, 0, 0},      [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},     [13] = {0, 0, 0, 0},      [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},     [16] = {0, 0, 0, 0},      [17] = {0, 0, 0, 0},
            [18] = {-8, -64, 32, 0}, [19] = {8, -64, 32, 0},   [20] = {0, -64, 48, 0},
            [21] = {0, -112, 48, 0},
        },
    .gfx =
        {
            [0] = GFX_MAP_TEXTURE_188,
            [1] = GFX_MAP_TEXTURE_188,
            [2] = GFX_MAP_TEXTURE_167,
            [3] = GFX_MAP_TEXTURE_167,
            [4] = GFX_MAP_TEXTURE_167,
            [5] = GFX_MAP_TEXTURE_184,
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
            [0] = {0, 6, 7, 4},
            [1] = {3, 2, 5, 1},
            [2] = {7, 6, 2, 3},
            [3] = {4, 7, 3, 1},
            [4] = {0, 4, 1, 5},
            [5] = {6, 0, 5, 2},
            [6] = {0, 0, 0, 0},
            [7] = {0, 0, 0, 0},
            [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},
            [10] = {0, 0, 0, 0},
            [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},
            [13] = {0, 0, 0, 0},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 6,
    .height = 0,
};

static MapTileModel sMapTileModel_801001e8 = {
    .vertices =
        {
            [0] = {16, -32, -16, 0},  [1] = {16, 0, 16, 0},    [2] = {-16, 0, -16, 0},
            [3] = {-16, 0, 16, 0},    [4] = {16, -32, 16, 0},  [5] = {16, 0, -16, 0},
            [6] = {-16, -32, -16, 0}, [7] = {-16, -32, 16, 0}, [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},       [10] = {0, 0, 0, 0},     [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},      [13] = {0, 0, 0, 0},     [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},      [16] = {0, 0, 0, 0},     [17] = {0, 0, 0, 0},
            [18] = {-8, -64, 32, 0},  [19] = {8, -64, 32, 0},  [20] = {0, -64, 48, 0},
            [21] = {0, -112, 48, 0},
        },
    .gfx =
        {
            [0] = GFX_MAP_TEXTURE_188,
            [1] = GFX_MAP_TEXTURE_188,
            [2] = GFX_MAP_TEXTURE_167,
            [3] = GFX_MAP_TEXTURE_167,
            [4] = GFX_MAP_TEXTURE_167,
            [5] = GFX_MAP_TEXTURE_184,
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
            [0] = {0, 6, 7, 4},
            [1] = {3, 2, 5, 1},
            [2] = {7, 6, 2, 3},
            [3] = {4, 7, 3, 1},
            [4] = {0, 4, 1, 5},
            [5] = {6, 0, 5, 2},
            [6] = {0, 0, 0, 0},
            [7] = {0, 0, 0, 0},
            [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},
            [10] = {0, 0, 0, 0},
            [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},
            [13] = {0, 0, 0, 0},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 6,
    .height = 0,
};

void Map13_RemoveBridgeSection(s32 x) {
   typedef struct DamagedBridgeTileInfo {
      u8 z;
      u8 faceIdx;
      u8 elevation;
   } DamagedBridgeTileInfo;

   static DamagedBridgeTileInfo tileInfo[8] = {{2, 0, 4}, {2, 1, 2}, {5, 0, 4}, {5, 1, 2},
                                               {3, 0, 3}, {3, 1, 2}, {4, 0, 3}, {4, 1, 2}};

   s32 i;
   DamagedBridgeTileInfo *p;
   MapTileModel *pTileModel;

   p = &tileInfo[0];

   for (i = 0; i < 8; i++) {
      pTileModel = &gMapRowPointers[p->z][x];
      AdjustFaceElevation(pTileModel, p->faceIdx, p->elevation * 16);
      pTileModel->gfx[0] = GFX_MAP_TEXTURE_52; // Water texture
      p++;
   }

   if (x > 4) {
      gMapRowPointers[2][x - 1].gfx[4] = GFX_MAP_TEXTURE_167;
      gMapRowPointers[3][x - 1].gfx[5] = GFX_MAP_TEXTURE_213;
      gMapRowPointers[4][x - 1].gfx[5] = GFX_MAP_TEXTURE_213;
      gMapRowPointers[5][x - 1].gfx[4] = GFX_MAP_TEXTURE_167;
   }

   for (i = 2; i <= 5; i++) {
      gTerrainPtr[i][x].s.elevation = -gMapRowPointers[i][x].vertices[0].vy >> 4;
      gTerrainPtr[i][x].s.terrain = TERRAIN_WATER;
   }
}

void Map13_ExplodeBridgeTile(s32 z, s32 x, s32 idx) {
   extern MapTileModel s_tileModels_80123e54[4]; //?

   static MapTileModel *baseTileModels[4] = {&sMapTileModel_800fff88, &sMapTileModel_801000b8,
                                             &sMapTileModel_801000b8, &sMapTileModel_800fff88};

   EvtData *explodingTile;

   idx &= 3;
   s_tileModels_80123e54[idx] = *baseTileModels[idx];

   explodingTile = Evt_GetUnused();
   explodingTile->functionIndex = EVTF_EXPLODING_TILE;
   // Unused? It looks like Evtf653_ExplodingTile maintains its own working copy.
   explodingTile->d.evtf653.tileModel_unused = &s_tileModels_80123e54[idx];
   explodingTile->d.evtf653.tileModel = baseTileModels[idx];
   explodingTile->d.evtf653.riverVelocity = -16;
   explodingTile->d.evtf653.translation.vx = x * CV(1.0) + CV(0.5);
   explodingTile->d.evtf653.translation.vy = CV(2.0);
   explodingTile->d.evtf653.translation.vz = z * CV(1.0) + CV(0.5);
   explodingTile->d.evtf653.translation.pad = 0;
}

#undef EVTF
#define EVTF 361
void Evtf361_Map13_BridgeExplosion_Scene(EvtData *evt) {
   s32 i;
   s32 end;
   EvtData *explosion;
   s8 unused[8];

   switch (evt->state) {
   case 0:
      EVT.currentX = 21;
      evt->x1.n = CV(21.5);
      evt->z1.n = CV(4.5);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->mem = 0;

      if (gState.mapState.s.field_0x0 != 0) {
         end = gState.mapState.s.field_0x0;
         for (i = 0; i < end; i++) {
            Map13_RemoveBridgeSection(EVT.currentX--);
            evt->mem++;
            evt->x1.n -= CV(1.0);
         }
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 > evt->mem) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 2:
      evt->state++;
      break;

   case 3:
      evt->state++;

   // fallthrough
   case 4:
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 5:
      evt->state++;
      evt->z1.n = CV(0.5);

      for (i = 0; i < 4; i++) {
         if ((rand() & 0xfff) < 0x800) {
            Map13_ExplodeBridgeTile(i + 2, EVT.currentX, i);
         }
      }

      for (i = 0; i < 4; i++) {
         if (gTerrainPtr[i + 2][EVT.currentX].raw != 0) {
            gTileStateGridPtr[i + 2][EVT.currentX].action = TA_X16;
         }
      }

      for (i = 0; i < 4; i++) {
         explosion = Evt_GetUnused();
         explosion->functionIndex = EVTF_MAP13_EXPLOSION_PILLAR;
         explosion->x1.n = EVT.currentX * CV(1.0) + CV(0.5);
         explosion->y1.n = CV(2.0);
         explosion->z1.n = (i + 2) * CV(1.0) + CV(0.5);
      }

      Map13_RemoveBridgeSection(EVT.currentX);

   // fallthrough
   case 6:
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 7:
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 8:
      EVT.currentX--;
      evt->mem++;
      evt->x1.n -= CV(1.0);
      evt->state3 = 256;
      gState.field_0x96 = 0;
      evt->state = 1;
      break;
   }
}

#undef EVTF
#define EVTF 288
void Evtf288_Map13_BridgeExplosion_Battle(EvtData *evt) {
   s32 i;
   s32 end;
   s32 numHit;
   EvtData *explosion;
   s8 unused[8];

   switch (evt->state) {
   case 0:
      EVT.currentX = 21;
      evt->x1.n = CV(21.5);
      evt->z1.n = CV(4.5);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->mem = 0;

      if (gState.mapState.s.field_0x0 != 0) {
         end = gState.mapState.s.field_0x0;

         for (i = 0; i < end; i++) {
            Map13_RemoveBridgeSection(EVT.currentX--);
            evt->mem++;
            evt->x1.n -= CV(1.0);

            if (EVT.currentX < 4) {
               break;
            }
         }
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 > evt->mem) {
         if (EVT.currentX < 4) {
            gState.field_0x96 = 0;
            break;
         }

         evt->state3 = 64;
         evt->state++;
         numHit = 0;
         EVT.hitPlayerUnit = 0;

         for (i = 0; i < 4; i++) {
            if (gMapUnitsPtr[i + 2][EVT.currentX].s.team == TEAM_PLAYER) {
               numHit++;
            }
         }

         if (numHit != 0) {
            EVT.hitPlayerUnit = 1;
         }

         if (EVT.hitPlayerUnit) {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(18));
         } else {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(19));
         }
      }

      break;

   case 2:
      EVT.dstRotY_unused = GetBestViewOfTarget(evt->z1.s.hi, evt->x1.s.hi, 1);
      gCameraRotation.vy &= 0xfff;
      AssignFromMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 1;
      evt->state3 = 48;
      evt->state++;

   // fallthrough
   case 3:
      gCameraRotation.vy += (EVT.camera.rotX - gCameraRotation.vy) >> 2;
      gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
      gCameraZoom.vz += (768 - gCameraZoom.vz) >> 2;
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 2);

      if (EVT.hitPlayerUnit && evt->state3 == 16) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(18));
      }
      if (!EVT.hitPlayerUnit && evt->state3 == 16) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(19));
      }

      if (--evt->state3 <= 0) {
         PositionCamera(evt->x1.n, evt->y1.n, evt->z1.n);
         gCameraRotation.vx = CV(1.5);
         gCameraRotation.vy = EVT.camera.rotX;
         gCameraZoom.vz = 768;

         if (EVT.hitPlayerUnit) {
            evt->state++;

            for (i = 0; i < 4; i++) {
               explosion = Evt_GetUnused();
               explosion->functionIndex = EVTF_MAP13_EXPLOSION_PILLAR;
               explosion->x1.n = EVT.currentX * CV(1.0) + CV(0.5);
               explosion->y1.n = CV(2.0);
               explosion->z1.n = (i + 2) * CV(1.0) + CV(0.5);

               explosion = Evt_GetUnused();
               explosion->functionIndex = EVTF_MAP13_EXPLOSION_PILLAR;
               explosion->x1.n = EVT.currentX * CV(1.0) + CV(0.5);
               explosion->y1.n = CV(2.0);
               explosion->z1.n = (i + 2) * CV(1.0) + CV(0.5);
            }
            evt->state3 = 32;
         } else {
            for (i = 0; i < 4; i++) {
               Map13_ExplodeBridgeTile(i + 2, EVT.currentX, i);
               if (gMapUnitsPtr[i + 2][EVT.currentX].s.unitIdx != UNIT_NULL) {
                  gTileStateGridPtr[i + 2][EVT.currentX].action = TA_X16;
               }
            }
            Map13_RemoveBridgeSection(EVT.currentX);
            evt->state += 3;
            evt->state3 = 96;
         }
      }

      break;

   case 4:
      if (--evt->state3 <= 0) {
         evt->state2 = 0;
         evt->state++;
      }
      break;

   case 5:

      switch (evt->state2) {
      case 0:
         EVT.iterator = 0;
         evt->state2++;

      // fallthrough
      case 1:
         i = EVT.iterator;
         if (gMapUnitsPtr[i + 2][EVT.currentX].raw != 0) {
            gState.msgFinished = 0;
            gTileStateGridPtr[i + 2][EVT.currentX].action = TA_X20;
            evt->state2 += 2;
         } else {
            evt->state2++;
         }
         break;

      case 2:
         if (++EVT.iterator >= 4) {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(19));
            evt->state2 += 3;
            evt->state3 = 0;
         } else {
            evt->state2 = 1;
         }
         break;

      case 3:
         if (gState.msgFinished) {
            evt->state2++;
            if (++EVT.iterator >= 4) {
               PerformAudioCommand(AUDIO_CMD_PREPARE_XA(19));
               evt->state2 = 5;
               evt->state3 = 0;
            }
         }
         break;

      case 4:
         evt->state2 = 1;
         break;

      case 5:
         if (++evt->state3 >= 45) {
            evt->state2++;
         }
         if (evt->state3 == 40) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(19));
         }
         break;

      case 6:
         for (i = 0; i < 4; i++) {
            if (gMapUnitsPtr[i + 2][EVT.currentX].raw != 0) {
               gTileStateGridPtr[i + 2][EVT.currentX].action = TA_X16;
            }
            Map13_ExplodeBridgeTile(i + 2, EVT.currentX, i);
         }
         Map13_RemoveBridgeSection(EVT.currentX);
         evt->state++;
         evt->state3 = 96;
         break;
      } // switch (evt->state2) (via state:5)

      break;

   case 6:
      if (--evt->state3 <= 0) {
         evt->state3 = 48;
         evt->state++;
      }
      break;

   case 7:
      EaseOutCamera(&EVT.camera, 3);

      if (--evt->state3 <= 0) {
         AssignToMainCamera(&EVT.camera);
         evt->state++;
      }
      break;

   case 8:
      EVT.currentX--;
      evt->mem++;
      evt->x1.n -= CV(1.0);
      evt->state3 = 256;
      gState.field_0x96 = 0;
      evt->state = 1;
      break;
   }
}

#undef EVTF
#define EVTF 085
void Evtf085_Map13_ExplosionPillar(EvtData *evt) {
   static s16 explosionAnimData[26] = {0, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                                       2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

   s32 i;
   EvtData *explosion;

   switch (evt->state) {
   case 0:
      for (i = 0; i < 5; i++) {
         explosion = Evt_GetUnused();
         explosion->functionIndex = EVTF_EXPLOSION;
         explosion->x1.n = evt->x1.n;
         explosion->z1.n = evt->z1.n;
         explosion->y1.n = evt->y1.n;
         explosion->y2.n = i * CV(0.375);
         explosion->y3.n = i * CV(-0.03125);
         explosion->d.sprite.animData = explosionAnimData;
         explosion->d.sprite.boxIdx = 6;
         explosion->state = 1;
      }
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 297
void Evtf297_SplashDroplets(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_DOTS;
      evt->d.sprite.boxIdx = 4;
      evt->state2 = 32;
      evt->state++;

   // fallthrough
   case 1:
      evt->y1.n += evt->y2.n;
      evt->y2.n += evt->y3.n;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      if (evt->y1.n < GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi)) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 298
void Evtf298_SplashWithDroplets(EvtData *evt) {
   static s16 splashAnimData[20] = {
       0, GFX_SPLASH_1, 2, GFX_SPLASH_2, 2, GFX_SPLASH_3, 2, GFX_SPLASH_4, 2, GFX_SPLASH_5,
       2, GFX_SPLASH_6, 2, GFX_SPLASH_7, 2, GFX_SPLASH_8, 2, GFX_NULL,     0, GFX_NULL};

   s32 i;
   EvtData *droplets;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = splashAnimData;
      evt->d.sprite.boxIdx = 4;
      evt->y3.n = CV(-0.015625);

      for (i = 0; i < 8; i++) {
         droplets = Evt_GetUnused();
         droplets->functionIndex = EVTF_SPLASH_DROPLETS;
         droplets->x1.n = evt->x1.n;
         droplets->z1.n = evt->z1.n;
         droplets->y1.n = evt->y1.n;
         droplets->y2.n = (evt->y2.n / 16) * (i + 1);
         droplets->y3.n = CV(-0.03125);
      }

      evt->d.sprite.semiTrans = 2;
      evt->state++;

   // fallthrough
   case 1:
      evt->y2.n += evt->y3.n;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void Map15_RaisePlank(s32 x, s32 z) {
   s32 i;
   MapTileModel *pTileModel;

   z = 11;

   for (i = 0; i < 4; i++) {
      pTileModel = &gMapRowPointers[z - i][x];
      pTileModel->gfx[0] = GFX_MAP_TEXTURE_14; // Plank texture
      AdjustFaceElevation(pTileModel, 0, -48);
      AdjustFaceElevation(pTileModel, 1, -32);
      gTerrainPtr[z - i][x].s.elevation = -gMapRowPointers[z - i][x].vertices[0].vy >> 4;
      gTerrainPtr[z - i][x].s.terrain = TERRAIN_PLAINS;
   }
}

void Map15_HideWaterTiles(void) {
   s16 *pSavedGfx;
   MapTileModel *pTileModel;
   s32 iz, ix, iGfx;

   pSavedGfx = (s16 *)gScratch1_801317c0;

   for (ix = 0; ix < 21; ix++) {
      for (iz = 0; iz < 21; iz++) {
         pTileModel = &gMapRowPointers[iz][ix];
         for (iGfx = 0; iGfx < ARRAY_COUNT(pTileModel->gfx); iGfx++) {
            *pSavedGfx++ = pTileModel->gfx[iGfx];
            if (pTileModel->gfx[iGfx] == GFX_MAP_TEXTURE_6) {
               pTileModel->gfx[iGfx] = GFX_NULL;
            }
         }
      }
   }
}

void Map15_RestoreWaterTiles(void) {
   s16 *pSavedGfx;
   MapTileModel *pTileModel;
   s32 iz, ix, iGfx;

   pSavedGfx = (s16 *)gScratch1_801317c0;

   for (ix = 0; ix < 21; ix++) {
      for (iz = 0; iz < 21; iz++) {
         pTileModel = &gMapRowPointers[iz][ix];
         for (iGfx = 0; iGfx < ARRAY_COUNT(pTileModel->gfx); iGfx++) {
            pTileModel->gfx[iGfx] = *pSavedGfx++;
         }
      }
   }
}

#undef EVTF
#define EVTF 089
void Evtf089_Map15_Scn17_Cinematic(EvtData *evt) {
   // Spawned by EVDATA17.DAT
   EvtData *evt_s1;
   EvtData *evt_a0;
   EvtData *pEvt;
   s32 i, j;
   s16 camPosX, camPosY, camRotY;
   s32 flag;

   switch (evt->state) {
   case 0:
      if (++evt->state3 >= 0) {
         evt->state++;
         gState.fieldRenderingDisabled = 1;

         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_MAP15_OCEAN;
         EVT.ocean = evt_s1;
         evt->state3 = 0;
         gPlayerControlSuppressed = 1;

         evt_a0 = Evt_GetUnused();
         evt_a0->functionIndex = EVTF_NOOP;
         EVT.focus = evt_a0;
      }
      break;

   case 1:
      evt->x1.n = CV(60.0);
      evt->z1.n = CV(30.0);
      evt->y1.n = CV(14.0);
      evt->x2.n = evt->x1.n + CV(16.0);
      evt->z2.n = CV(12.0);
      evt->y2.n = CV(2.0);

      AssignFromMainCamera(&EVT.camera);
      EVT.todo_x24 = 0xc00;
      evt->state++;
      break;

   case 2:
      // Player's ship
      for (i = gMapMinZ; i < 12; i++) {
         for (j = gMapMinX; j <= gMapMaxX; j++) {
            RenderMapTile(gGraphicsPtr->ot, &gMapRowPointers[i][j], GRID_COLOR_NONE);
         }
      }

      camPosY = gCameraPos.vy;
      camPosX = gCameraPos.vx;
      camRotY = gCameraRotation.vy;

      PushMatrix();
      RotMatrix(&gCameraRotation, &gCameraMatrix);
      TransMatrix(&gCameraMatrix, &gCameraZoom);
      SetRotMatrix(&gCameraMatrix);
      SetTransMatrix(&gCameraMatrix);
      gCameraPos.vx += EVT.todo_x24;

      if (EVT.todo_x24 >= 0x200) {
         EVT.todo_x24 -= 8;
      } else if (EVT.todo_x24 >= 0x40) {
         EVT.todo_x24 -= 4;
      } else if (EVT.todo_x24 >= 1) {
         EVT.todo_x24 -= 1;
         EVT.todo_x24 += -(EVT.todo_x24) >> 4;
      } else {
         evt->state++;
      }

      RotTrans(&gCameraPos, (VECTOR *)gCameraMatrix.t, &flag);
      SetTransMatrix(&gCameraMatrix);

      // Temporary sprite used to render the stand-ins at their faux positions
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;

      pEvt = &gEvtDataArray[0];
      for (i = 0; i < EVT_DATA_CT; i++) {
         if (pEvt->functionIndex == EVTF_MAP15_PIRATE_STAND_IN) {
            evt_a0 = pEvt->d.entitySpawn.entitySpriteParam;
            evt_a0->d.sprite.hidden = 1;
            CopyEvtData(evt_a0, evt_s1);
            evt_s1->functionIndex = EVTF_NOOP;
            evt_s1->d.sprite.hidden = 0;
            RenderUnitSprite(gGraphicsPtr->ot, evt_s1, 0);
         }
         pEvt++;
      }

      evt_s1->functionIndex = EVTF_NULL;

      // Pirate's ship
      for (i = 12; i <= gMapMaxZ; i++) {
         for (j = gMapMinX; j <= gMapMaxX; j++) {
            RenderMapTile(gGraphicsPtr->ot, &gMapRowPointers[i][j], GRID_COLOR_NONE);
         }
      }

      PopMatrix();

      gCameraPos.vx = camPosX;
      gCameraRotation.vy = camRotY;

      evt->x3.n = 0x1000 - EVT.todo_x24;
      evt->x2.n = (EVT.todo_x24 << 3) + evt->x3.n;
      evt->x1.n -= CV(0.0625);
      evt->z1.n -= CV(0.046875);
      evt->y1.n -= CV(0.015625);

      evt_a0 = EVT.focus;
      evt_a0->x1.n = evt->x2.n;
      evt_a0->y1.n = evt->y2.n;
      evt_a0->z1.n = evt->z2.n;

      switch (evt->state2) {
      case 0:
         evt->state3 = 0;
         evt_a0->x1.n = evt->x2.n;
         evt_a0->y1.n = evt->y2.n;
         evt_a0->z1.n = evt->z2.n;
         gState.focus = evt_a0;
         evt->state2++;
         break;
      case 1:
         func_800985F0(evt);
         if (++evt->state3 >= 48) {
            evt->state2++;
         }
         break;
      case 2:
         func_800985F0(evt);
         break;
      }

      if (--evt->mem <= 0) {
         for (i = 0; i < 1; i++) {
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_MAP15_HULL_SPLASH;
            evt_s1->x1.n = (i + 4) * CV(1.0) + (EVT.todo_x24 << 3);
            evt_s1->z1.n = (i + 15) * CV(1.0);
            evt_s1->y1.n = CV(0.5);
            evt_s1->x2.n = CV(-0.1875);
            evt_s1->x3.n = CV(0.03125);
            evt_s1->z2.n = CV(0.25);
            evt_s1->z3.n = i - 3;
         }
         evt->mem = 4;
      }

      if (evt->state == 3) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_NOOP;

         pEvt = &gEvtDataArray[0];
         for (i = 0; i < EVT_DATA_CT; i++) {
            if (pEvt->functionIndex == EVTF_MAP15_PIRATE_STAND_IN) {
               evt_a0 = pEvt->d.entitySpawn.entitySpriteParam;
               evt_a0->d.sprite.hidden = 0;
               pEvt->functionIndex = EVTF_NULL;
            }
            pEvt++;
         }
         evt_s1->functionIndex = EVTF_NULL;
      }

      break;

   case 3:
      for (i = 0; i <= gMapMaxZ; i++) {
         for (j = gMapMinX; j <= gMapMaxX; j++) {
            RenderMapTile(gGraphicsPtr->ot, &gMapRowPointers[i][j], GRID_COLOR_NONE);
         }
      }
      evt->state2 = 0;
      evt->state++;
      break;

   case 4:
      if (gState.fieldRenderingDisabled) {
         for (i = 0; i <= gMapMaxZ; i++) {
            for (j = gMapMinX; j <= gMapMaxX; j++) {
               RenderMapTile(gGraphicsPtr->ot, &gMapRowPointers[i][j], GRID_COLOR_NONE);
            }
         }
      }

      switch (evt->state2) {
      case 0:
         evt->state3 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (++evt->state3 >= 48) {
            evt->state2++;
         }
         break;

      case 2:
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_MAP15_PLANK;
         // Tile-based coords
         evt_s1->x1.n = 7;
         evt_s1->z1.n = 11;
         EVT.plank = evt_s1;
         EVT.dstRotY = GetBestViewOfTarget(11, 7, 1);
         evt->state3 = 0;
         evt->state2++;
         break;

      case 3:
         evt_a0 = EVT.focus;
         evt_a0->x1.n = CV(7.0);
         evt_a0->y1.n = CV(2.0);
         evt_a0->z1.n = CV(11.0);

         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         gCameraZoom.vz += (384 - gCameraZoom.vz) >> 3;

         if (evt->state3 == 16) {
            gState.fieldRenderingDisabled = 0;
            Map15_RestoreWaterTiles();
         }

         if (evt->state3 == 17) {
            evt_s1 = EVT.ocean;
            evt_s1->functionIndex = EVTF_NULL;
         }

         if (++evt->state3 >= 48) {
            evt->state2++;
         }

         break;

      case 4:
         evt_s1 = EVT.plank;
         if (evt_s1->functionIndex != EVTF_MAP15_PLANK) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 5:
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_MAP15_PLANK;
         // Tile-based coords
         evt_s1->x1.n = 14;
         evt_s1->z1.n = 11;
         EVT.plank = evt_s1;
         EVT.dstRotY += DEG(90);
         evt->state3 = 0;
         evt->state2++;
         break;

      case 6:
         evt_a0 = EVT.focus;
         evt_a0->x1.n = CV(14.0);
         evt_a0->y1.n = CV(2.0);
         evt_a0->z1.n = CV(11.0);

         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 3;
         gCameraZoom.vz += (384 - gCameraZoom.vz) >> 3;
         if (++evt->state3 >= 48) {
            evt->state2++;
         }
         break;

      case 7:
         evt_s1 = EVT.plank;
         if (evt_s1->functionIndex != EVTF_MAP15_PLANK) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 8:
         if (++evt->state3 >= 48) {
            evt->state2 = 0;
            evt->state++;
         }
         break;
      } // switch (evt->state2) (via state:4)

      break;

   case 5:
      gState.fieldRenderingDisabled = 0;
      gState.signal = 0;
      evt_a0 = EVT.focus;
      gState.eventCameraPan.x = evt_a0->x1.n;
      gState.eventCameraPan.y = evt_a0->y1.n;
      gState.eventCameraPan.z = evt_a0->z1.n;
      evt_a0->functionIndex = EVTF_NULL;
      gState.focus = NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }

   gState.eventCameraRot = gCameraRotation.vy;
}

void Map15_GetChunkBrightness(s32 nearness, s16 sideIdx, CVECTOR *dstColor) {
   s16 r, g, b;
   s16 n;
   s8 unused[8];

   n = nearness - 150;
   if (n < 0) {
      n = 0;
   } else if (n > 256) {
      n = 256;
   }

   r = gSideLightColor[sideIdx].r;
   g = gSideLightColor[sideIdx].g;
   b = gSideLightColor[sideIdx].b;
   r *= n;
   g *= n;
   b *= n;
   dstColor->r = r >> 8;
   dstColor->g = g >> 8;
   dstColor->b = b >> 8;
}

void Map15_RenderOceanChunk(s32 x, s32 y, s32 z, s32 xsize, s32 zsize) {
   EvtData *chunk;
   CVECTOR color;
   s32 otz;
   s32 sxy, p, flag;
   POLY_FT4 *poly;

   chunk = Evt_GetUnused();
   chunk->functionIndex = EVTF_NOOP;

   chunk->x1.n = x;
   chunk->z1.n = -z;
   chunk->y1.n = y;

   chunk->d.dataStore.svectors[3].vx = x / 8;
   chunk->d.dataStore.svectors[3].vz = z / 8;
   chunk->d.dataStore.svectors[3].vy = -y / 8;
   chunk->d.dataStore.svectors[4].vx = x / 8;
   chunk->d.dataStore.svectors[4].vz = (z + zsize) / 8;
   chunk->d.dataStore.svectors[4].vy = -y / 8;
   chunk->d.dataStore.svectors[5].vx = (x + xsize) / 8;
   chunk->d.dataStore.svectors[5].vz = z / 8;
   chunk->d.dataStore.svectors[5].vy = -y / 8;
   chunk->d.dataStore.svectors[6].vx = (x + xsize) / 8;
   chunk->d.dataStore.svectors[6].vz = (z + zsize) / 8;
   chunk->d.dataStore.svectors[6].vy = -y / 8;

   // Presumably gfxIdx; but since it's unused anyway, this will do
   chunk->d.dataStore.svectors[0].vz = GFX_MAP_TEXTURE_6;

   chunk->vec.vx = chunk->x1.n >> 3;
   chunk->vec.vz = chunk->z1.n >> 3;
   chunk->vec.vy = chunk->y1.n >> 3;

   otz = RotTransPers(&chunk->vec, &sxy, &p, &flag);
   Map15_GetChunkBrightness(OT_SIZE + 6 - otz, 0, &color);

   poly = &gGraphicsPtr->quads[gQuadIndex++];
   SetPolyFT4(poly);
   RotTransPers4(&chunk->d.dataStore.svectors[3], &chunk->d.dataStore.svectors[4],
                 &chunk->d.dataStore.svectors[5], &chunk->d.dataStore.svectors[6], &poly->x0,
                 &poly->x1, &poly->x2, &poly->x3, &p, &flag);
   poly->r0 = color.r;
   poly->g0 = color.g;
   poly->b0 = color.b;
   poly->tpage = GetTPage(0, 0, 576, 256);
   poly->clut = gGfxClutIds[GFX_MAP_TEXTURE_6];
   setUVWH(poly, 0, 0, 255, 255);
   addPrim(&gGraphicsPtr->ot[2], poly);

   chunk->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 299
void Evtf299_Map15_Ocean(EvtData *evt) {
   extern DR_MODE s_drawModes_80123e24[2][2];

   RECT rect;
   u16 tpage;
   s32 ix, iz;
   s8 unused[8];

   switch (evt->state) {
   case 0:
      Map15_HideWaterTiles();
      evt->state++;

   // fallthrough
   case 1:
      evt->state2++;
      evt->state2 %= 2;

      rect.x = 0;
      rect.y = 0;
      rect.w = 0;
      rect.h = 0;
      tpage = GetTPage(0, 0, 576, 256);
      SetDrawMode(&s_drawModes_80123e24[0][evt->state2], 0, 0, tpage, &rect);
      AddPrim(&gGraphicsPtr->ot[2], &s_drawModes_80123e24[0][evt->state2]);

      for (iz = -1; iz < 4; iz++) {
         for (ix = -1; ix < 15; ix++) {
            Map15_RenderOceanChunk(ix * 0x800, 0x80, iz * 0x800, 0x800, 0x800);
         }
      }

      // Set up repeating texture
      rect.x = 0;
      rect.y = 0;
      rect.w = 32;
      rect.h = 32;
      tpage = GetTPage(0, 0, 576, 256);
      SetDrawMode(&s_drawModes_80123e24[1][evt->state2], 0, 0, tpage, &rect);
      AddPrim(&gGraphicsPtr->ot[2], &s_drawModes_80123e24[1][evt->state2]);

      // Copy the animated water drawn by EVTF_MAP_OBJECT_WATER_2
      rect.x = 332;
      rect.y = 0;
      rect.w = 32 >> 2;
      rect.h = 32;
      MoveImage(&rect, 576, 256);
      break;
   }
}

static MapTileModel sMapTileModel_8010039c = {
    .vertices =
        {
            [0] = {-16, -16, 16, 0}, [1] = {16, 0, 16, 0},     [2] = {16, 0, -16, 0},
            [3] = {-16, 0, -16, 0},  [4] = {-16, 0, 16, 0},    [5] = {16, -16, 16, 0},
            [6] = {16, -16, -16, 0}, [7] = {-16, -16, -16, 0}, [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},      [10] = {0, 0, 0, 0},      [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},     [13] = {0, 0, 0, 0},      [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},     [16] = {0, 0, 0, 0},      [17] = {0, 0, 0, 0},
            [18] = {0, 0, 0, 0},     [19] = {0, 0, 0, 0},      [20] = {0, 0, 0, 0},
            [21] = {0, 0, 0, 0},
        },
    .gfx =
        {
            [0] = GFX_MAP_TEXTURE_14,
            [1] = GFX_MAP_TEXTURE_153,
            [2] = 0,
            [3] = GFX_MAP_TEXTURE_153,
            [4] = 0,
            [5] = GFX_MAP_TEXTURE_14,
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
            [1] = {6, 5, 1, 2},
            [2] = {7, 6, 2, 3},
            [3] = {0, 7, 3, 4},
            [4] = {5, 0, 4, 1},
            [5] = {4, 3, 2, 1},
            [6] = {0, 0, 0, 0},
            [7] = {0, 0, 0, 0},
            [8] = {0, 0, 0, 0},
            [9] = {0, 0, 0, 0},
            [10] = {0, 0, 0, 0},
            [11] = {0, 0, 0, 0},
            [12] = {0, 0, 0, 0},
            [13] = {0, 0, 0, 0},
            [14] = {0, 0, 0, 0},
            [15] = {0, 0, 0, 0},
            [16] = {0, 0, 0, 0},
            [17] = {0, 0, 0, 0},
        },
    .shades = {0},
    .faceCt = 6,
    .height = 8,
};

#undef EVTF
#define EVTF 364
void Evtf364_Map15_Plank(EvtData *evt) {
   static const s16 textureGfx[4] = {GFX_MAP_TEXTURE_14, GFX_MAP_TEXTURE_153, GFX_MAP_TEXTURE_153,
                                     GFX_NULL};
   s32 i, j, k;
   MapTileModel mapTile1;
   MapTileModel mapTile2;
   SVECTOR rotation;
   SVECTOR position;
   MapTileModel *pTileModel;

   switch (evt->state) {
   case 0:
      for (i = 0; i < 3; i++) {
         j = textureGfx[i];

         for (k = 0; k < 4; k++) {
            gGfxSubTextures[GFX_TILED_DYN_163 + i][k] = gGfxSubTextures[j][k];
         }

         gGfxClutIds[GFX_TILED_DYN_163 + i] = gGfxClutIds[j];
         gGfxTPageIds[GFX_TILED_DYN_163 + i] = gGfxTPageIds[j];
      }

      evt->z3.n = 16;
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->z3.n < 0) {
         mapTile1 = sMapTileModel_8010039c;
         pTileModel = &mapTile1;

         for (i = 0; i < 8; i++) {
            pTileModel->vertices[i].vx += evt->x1.n * 32 + 16;
            pTileModel->vertices[i].vz += evt->z1.n * 32 + 16;
            pTileModel->vertices[i].vy -= 48;
         }

         for (i = 0; i < evt->state3; i++) {
            RenderMapTile(gGraphicsPtr->ot, pTileModel, GRID_COLOR_NONE);
            for (k = 0; k < 8; k++) {
               pTileModel->vertices[k].vz -= 32;
            }
         }

         mapTile2 = sMapTileModel_8010039c;

         j = 0;
         for (i = 0; i < 4; i++) {
            mapTile2.vertices[mapTile2.faces[2][i]].vz += 32 - (evt->state2 - j);
         }

         rotation.vx = 0;
         rotation.vz = rand() % 15 - 7;
         rotation.vy = 0;
         position.vx = evt->x1.n * CV(1.0) + CV(0.5);
         position.vy = CV(1.5);
         position.vz = (evt->z1.n - evt->state3) * CV(1.0) + CV(0.5);
         RotTransMapTile(&mapTile2, &rotation, &position, &mapTile2);

         mapTile2.gfx[5] = GFX_TILED_DYN_163;
         mapTile2.gfx[0] = GFX_TILED_DYN_163;
         mapTile2.gfx[4] = GFX_MAP_TEXTURE_153;
         mapTile2.gfx[2] = GFX_MAP_TEXTURE_153;
         mapTile2.gfx[1] = GFX_TILED_DYN_164;
         mapTile2.gfx[3] = GFX_TILED_DYN_165;

         gGfxSubTextures[GFX_TILED_DYN_163][0] =
             gGfxSubTextures[GFX_MAP_TEXTURE_14][0] - ((evt->state2 >> 1) - 16);
         gGfxSubTextures[GFX_TILED_DYN_163][2] = evt->state2 >> 1;
         gGfxSubTextures[GFX_TILED_DYN_164][0] =
             gGfxSubTextures[GFX_MAP_TEXTURE_153][0] - ((evt->state2 >> 1) - 16);
         gGfxSubTextures[GFX_TILED_DYN_164][2] = evt->state2 >> 1;
         gGfxSubTextures[GFX_TILED_DYN_165][2] = evt->state2 >> 1;

         RenderMapTile(gGraphicsPtr->ot, &mapTile2, GRID_COLOR_NONE);

         evt->state2 += 2;
         if (evt->state2 >= 33) {
            evt->state2 = 0;
            if (++evt->state3 >= 4) {
               Map15_RaisePlank(evt->x1.n, evt->z1.n);
               evt->state3 = 16;
               evt->state++;
            }
         }
      }

      break;

   case 2:
      if (--evt->state3 <= 0) {
         evt->state++;
      } else {
         i = rand() % evt->state3;
         i -= evt->state3 / 2;
         gCameraPos.vz += i;
      }
      break;

   case 3:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 086
void Evtf086_Map15_HullSplash(EvtData *evt) {
   static s16 splashAnimData[20] = {
       3, GFX_SPLASH_1, 2, GFX_SPLASH_2, 2, GFX_SPLASH_3, 2, GFX_SPLASH_4, 2, GFX_SPLASH_5,
       2, GFX_SPLASH_6, 2, GFX_SPLASH_7, 2, GFX_SPLASH_8, 2, GFX_NULL,     0, GFX_NULL};

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = splashAnimData;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->x2.n += evt->x3.n;
      evt->z1.n += evt->z2.n;
      evt->z2.n += evt->z3.n;

      if (evt->z2.n <= 0) {
         evt->z3.n = 0;
      }

      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 755
void Evtf755_Map15_PirateStandIn(void) {
   // Spawned by EVDATA17.DAT as stand-ins for pirate sprites
}

void func_800985F0(EvtData *evt) {
   s32 dx, dy, dz;
   s32 dist;
   s16 zoom;

   PositionCamera(evt->x2.n, evt->y2.n, evt->z2.n);
   gCameraRotation.vy = ratan2(evt->z2.n - evt->z1.n, evt->x2.n - evt->x1.n) + DEG(270);

   dx = evt->x1.n - evt->x2.n;
   dz = evt->z1.n - evt->z2.n;
   dist = SquareRoot0(dx * dx + dz * dz);
   gCameraRotation.vx = -ratan2(evt->y2.n - evt->y1.n, dist);

   dy = evt->y1.n - evt->y2.n;
   zoom = SquareRoot0(dist * dist + dy * dy) >> 3;
   gCameraZoom.vz = zoom;
}

void func_800986F0(EvtData *evt) {
   s32 dx, dy, dz;
   s32 dist;
   s16 zoom;
   s16 rot;

   PanCamera(evt->x2.n, evt->y2.n, evt->z2.n, 2);
   rot = ratan2(evt->z2.n - evt->z1.n, evt->x2.n - evt->x1.n) + DEG(270);
   gCameraRotation.vy += (rot - gCameraRotation.vy) >> 2;

   dx = evt->x1.n - evt->x2.n;
   dz = evt->z1.n - evt->z2.n;
   dist = SquareRoot0(dx * dx + dz * dz);

   rot = -ratan2(evt->y2.n - evt->y1.n, dist);
   gCameraRotation.vx += (rot - gCameraRotation.vx) >> 2;

   dy = evt->y1.n - evt->y2.n;
   zoom = SquareRoot0(dist * dist + dy * dy) >> 3;
   gCameraZoom.vz += (zoom - gCameraZoom.vz) >> 2;
}
