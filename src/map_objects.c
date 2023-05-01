#include "common.h"
#include "evt.h"
#include "field.h"
#include "units.h"
#include "graphics.h"
#include "state.h"
#include "field.h"
#include "battle.h"

#undef EVTF
#define EVTF 035
void Evtf035_MapObject_Tree(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.boxIdx = 10;
      EVT.gfxIdx = EVT.param;
      EVT.param = 0;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[HI(EVT.z)][HI(EVT.x)].height =
          (-gMapRowPointers[HI(EVT.z)][HI(EVT.x)].vertices[0].vy >> 4) + 4;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, (gState.mapNum == 14));
   }
}

#undef EVTF
#define EVTF 036
void Evtf036_MapObject_GraveMarker(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.boxIdx = 18;
      EVT.gfxIdx = GFX_GRAVE_MARKER;
      EVT.param = 0;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[HI(EVT.z)][HI(EVT.x)].height =
          (-gMapRowPointers[HI(EVT.z)][HI(EVT.x)].vertices[0].vy >> 4) + 3;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, 0);
   }
}

#undef EVTF
#define EVTF 415
void Evtf415_MapObject_Torch(EvtData *evt) {
   static s16 torchAnimData[] = {18, GFX_TORCH_1, 3, GFX_TORCH_2, 3, GFX_TORCH_3,
                                 3,  GFX_TORCH_4, 3, GFX_NULL,    3, GFX_NULL};

   switch (evt->state) {
   case 0:
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      EVT.animData = torchAnimData;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[HI(EVT.z)][HI(EVT.x)].height =
          (-gMapRowPointers[HI(EVT.z)][HI(EVT.x)].vertices[0].vy >> 4) + 3;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   UpdateEvtAnimation(evt);

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, 0);
   }
}

#undef EVTF
#define EVTF 037
void Evtf037_MapObject_Fountain(EvtData *evt) {
   static s16 fountainAnimData[] = {12, GFX_FOUNTAIN_1, 1, GFX_FOUNTAIN_2,
                                    1,  GFX_NULL,       1, GFX_NULL};

   switch (evt->state) {
   case 0:
      EVT.animData = fountainAnimData;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   UpdateEvtAnimation(evt);

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, 0);
   }
}

#undef EVTF
#define EVTF 038
void Evtf038_MapObject_LampPost(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.boxIdx = 13;
      EVT.gfxIdx = GFX_LAMP_POST;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[HI(EVT.z)][HI(EVT.x)].height =
          (-gMapRowPointers[HI(EVT.z)][HI(EVT.x)].vertices[0].vy >> 4) + 5;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, 0);
   }
}

#undef EVTF
#define EVTF 039
void Evtf039_MapObject_Flag(EvtData *evt) {
   static s16 flagAnimData[] = {14, GFX_FLAG_1, 2, GFX_FLAG_2, 2, GFX_FLAG_3,
                                2,  GFX_FLAG_4, 2, GFX_NULL,   1, GFX_NULL};

   switch (evt->state) {
   case 0:
      EVT.animData = flagAnimData;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[HI(EVT.z)][HI(EVT.x)].height =
          (-gMapRowPointers[HI(EVT.z)][HI(EVT.x)].vertices[0].vy >> 4) + 6;
      SetElevationFromTerrain(evt);
      evt->state++;
      break;
   }

   UpdateEvtAnimation(evt);

   if (gOverheadMapState == 0) {
      AddEvtPrim8(gGraphicsPtr->ot, evt, 0);
   }
}

void Evtf045_BloodSpurtOffset(EvtData *evt) {
   // Used during battle 32 to simulate train movement
   // todo replace
   evt->d.sprite.x1 -= 0x30;
   Evtf206_686_BloodSpurtParticle(evt);
}

#undef EVTF
#define EVTF 591
void Evtf591_MapObject_Boulder(EvtData *evt) {
   // TBD: Are the static inlines necessary? GetCamRotY_U etc.
   s32 shouldDraw;
   s16 diff;
   EvtData *newEvt;
   u8 unused[40];

   shouldDraw = !IsSpriteOutsideVisibleRange(evt);
   if (gOverheadMapState != 0) {
      shouldDraw = 1;
   }

   switch (evt->state) {
   case 0:
      EVT.boxIdx = 15;
      EVT.gfxIdx = GFX_BOULDER_1;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      EVT.dstX = EVT.x;
      EVT.dstZ = EVT.z;
      EVT.gridZ = HI(EVT.z);
      EVT.gridX = HI(EVT.x);
      SetElevationFromTerrain(evt);
      gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.team = TEAM_BOULDER;
      evt->state++;

   // fallthrough
   case 1:
      if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action == TA_BOULDER_PUSHED) {
         if (gState.mapNum == 40 && gState.mapState.n.field_0x0 == 0) {
            PerformAudioCommand(0x1370);
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_AUDIO_CMD;
            newEvt->d.evtf581.cmd = 0x370;
            newEvt->d.evtf581.delay = 10;
         } else {
            PerformAudioCommand(0x136f);
         }
         evt->state++;
         evt->state2 = 0;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         EVT.savedCamRotY = gCameraRotation.vy = GetCamRotY_U() & 0xfff;
         gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.team = TEAM_NULL;
         EVT.dstCamRotY = GetBestViewOfTarget(HI(EVT.dstZ), HI(EVT.dstX), 1);
         diff = EVT.dstCamRotY - GetCamRotY_U();
         if (diff > 0) {
            if (diff > ANGLE_180_DEGREES) {
               EVT.dstCamRotY -= ANGLE_360_DEGREES;
            }
         } else if (diff < -ANGLE_180_DEGREES) {
            EVT.dstCamRotY += ANGLE_360_DEGREES;
         }

         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
         ScanForBoulderDestination(evt);

         EVT.dx = 0;
         EVT.dz = 0;
         if (HI(EVT.dstX) > HI(EVT.x)) {
            EVT.dx = 0x40;
         }
         if (HI(EVT.dstX) < HI(EVT.x)) {
            EVT.dx = -0x40;
         }
         if (HI(EVT.dstZ) > HI(EVT.z)) {
            EVT.dz = 0x40;
         }
         if (HI(EVT.dstZ) < HI(EVT.z)) {
            EVT.dz = -0x40;
         }
         evt->state2++;

      // fallthrough
      case 1:
         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_CLOUD;
         newEvt->d.evtf215.x = EVT.x;
         newEvt->d.evtf215.z = EVT.z;
         newEvt->d.evtf215.y = EVT.y;
         EVT.x += EVT.dx;
         EVT.z += EVT.dz;
         if (EVT.gridZ != HI(EVT.z) || EVT.gridX != HI(EVT.x)) {
            EVT.gridZ = HI(EVT.z);
            EVT.gridX = HI(EVT.x);
            if (gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx != 0) {
               gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_BOULDER_HIT;
            }
         }
         if (EVT.x == EVT.dstX && EVT.z == EVT.dstZ) {
            evt->state++;
         }
         break;
      } // switch (evt->state2) (via state:2)

      OffsetCamPosX((-(EVT.x >> 3) - GetCamPosX()) >> 2);
      OffsetCamPosZ((-(EVT.z >> 3) - GetCamPosZ()) >> 2);
      OffsetCamPosY(((EVT.y >> 3) - GetCamPosY()) >> 2);
      OffsetCamRotY((EVT.dstCamRotY - GetCamRotY()) >> 3);
      break;

   case 3:
      newEvt = Evt_GetUnused();
      if (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain == TERRAIN_WATER ||
          gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain == TERRAIN_LAVA) {
         newEvt->functionIndex = EVTF_PUSHED_OBJECT_SPLASH;
         PerformAudioCommand(0x5ea);
      } else {
         newEvt->functionIndex = EVTF_BOULDER_RUBBLE;
         if (!(gState.mapNum == 40 && gState.mapState.n.field_0x0 == 0)) {
            PerformAudioCommand(0x36f);
         }
      }
      newEvt->d.sprite.x1 = EVT.x;
      newEvt->d.sprite.z1 = EVT.z;
      newEvt->d.sprite.y1 = EVT.y;
      EVT.hidden = 1;
      EVT.timer = 70;
      evt->state++;

   // fallthrough
   case 4:
      if (gState.mapNum == 40 && gState.mapState.n.field_0x0 == 0) {
         // Destroyed barricade
         gState.mapState.n.field_0x0 = 1;
         gState.field_0x96 = 1;
         EVT.timer = 120;
         evt->state++;
      }
      if (--EVT.timer == 0) {
         EVT.timer = 20;
         gSignal5 = 3;
         evt->state += 2;
      }
      break;

   case 5:
      if (--EVT.timer == 0) {
         EVT.timer = 20;
         gSignal5 = 3;
         evt->state++;
      }
      break;

   case 6:
      if (--EVT.timer == 0) {
         gCameraRotation.vy = GetCamDir() * ANGLE_90_DEGREES + ANGLE_45_DEGREES;
         gPlayerControlSuppressed = 0;
         evt->functionIndex = EVTF_NULL;
         return;
      }

      OffsetCamRotY((EVT.savedCamRotY - GetCamRotY()) >> 2);
      break;
   }

   if (evt->state != 1) {
      EVT.y += (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].elevation * 0x80 - EVT.y) >> 2;
   }

   if (shouldDraw) {
      AddEvtPrim7(gGraphicsPtr->ot, evt, 0);
   }
}

typedef struct CubeSideOrdering {
   s16 west;
   s16 east;
   s16 north;
   s16 south;
} CubeSideOrdering;

#undef EVTF
#define EVTF 046
void Evtf046_MapObject_Crate(EvtData *evt) {
   static CubeSideOrdering sideOrdering[4] = {
       {-1, 0, 2, 0}, {2, 0, 0, -1}, {0, -1, 0, 2}, {0, 2, -1, 0}};

   EvtData *face;
   EvtData *evt1;
   s32 dir;
   s16 x1, x2, z1, z2;
   s32 shouldDraw;
   POLY_FT4 *poly;
   s32 i;

   dir = (gCameraRotation.vy & 0xfff) >> 10;
   face = gTempGfxEvt;
   //?
   *(s16 *)(&face->d.sprite.hidden) = 0;
   face->d.sprite.gfxIdx = GFX_CRATE;
   face->d.sprite.clut = 0;
   face->d.sprite.otOfs = -6;

   x1 = EVT.x + 0x80;
   x2 = EVT.x - 0x80;
   z1 = EVT.z + 0x80;
   z2 = EVT.z - 0x80;

   // Top face:
   face->d.sprite.coords[0].x = x2;
   face->d.sprite.coords[2].x = x2;
   face->d.sprite.coords[1].x = x1 - 1;
   face->d.sprite.coords[3].x = x1 - 1;
   face->d.sprite.coords[0].z = z2;
   face->d.sprite.coords[2].z = z1 - 1;
   face->d.sprite.coords[1].z = z2;
   face->d.sprite.coords[3].z = z1 - 1;
   face->d.sprite.coords[0].y = EVT.y + 0x100;
   face->d.sprite.coords[1].y = EVT.y + 0x100;
   face->d.sprite.coords[2].y = EVT.y + 0x100;
   face->d.sprite.coords[3].y = EVT.y + 0x100;
   face->d.sprite.y1 = EVT.y + 0x100;
   face->d.sprite.x1 = EVT.x;
   face->d.sprite.z1 = EVT.z;

   shouldDraw = !IsSpriteOutsideVisibleRange(evt);
   if (gOverheadMapState != 0) {
      shouldDraw = 1;
   }

   if (shouldDraw) {
      AddEvtPrim5(gGraphicsPtr->ot, face);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      if (gShowBlueMovementGrid && gBlueMovementGridPtr[HI(EVT.z)][HI(EVT.x)] != PATH_STEP_UNSET &&
          gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)] == EVT.stack) {
         poly->r0 = 40;
         poly->g0 = 40;
         poly->b0 = gGridColorOscillation;
      } else if (gYellowTargetGridPtr[HI(EVT.z)][HI(EVT.x)] != PATH_STEP_UNSET) {
         poly->r0 = gGridColorOscillation;
         poly->g0 = gGridColorOscillation;
         poly->b0 = 0;
      } else if (gRedAttackGridPtr[HI(EVT.z)][HI(EVT.x)] != PATH_STEP_UNSET) {
         poly->r0 = gGridColorOscillation;
         poly->g0 = 40;
         poly->b0 = 40;
      }

      // West face:
      face->d.sprite.coords[0].x = x2 + 0xff;
      face->d.sprite.coords[2].x = x2 + 0xff;
      face->d.sprite.coords[1].y = EVT.y;
      face->d.sprite.coords[3].y = EVT.y;
      if (sideOrdering[dir].west != 0) {
         face->d.sprite.otOfs = sideOrdering[dir].west - 7;
         AddEvtPrim5(gGraphicsPtr->ot, face);
      }

      // East face:
      face->d.sprite.coords[0].x = x2;
      face->d.sprite.coords[2].x = x2;
      face->d.sprite.coords[1].x = x2;
      face->d.sprite.coords[3].x = x2;
      if (sideOrdering[dir].east != 0) {
         face->d.sprite.otOfs = sideOrdering[dir].east - 7;
         AddEvtPrim5(gGraphicsPtr->ot, face);
      }

      // North face:
      face->d.sprite.coords[1].x = x2 + 0xff;
      face->d.sprite.coords[3].x = x2 + 0xff;
      face->d.sprite.coords[2].z = z2;
      face->d.sprite.coords[3].z = z2;
      face->d.sprite.coords[0].y = EVT.y;
      face->d.sprite.coords[1].y = EVT.y;
      face->d.sprite.coords[2].y = EVT.y + 0x100;
      face->d.sprite.coords[3].y = EVT.y + 0x100;
      if (sideOrdering[dir].north != 0) {
         face->d.sprite.otOfs = sideOrdering[dir].north - 7;
         AddEvtPrim5(gGraphicsPtr->ot, face);
      }

      // South face:
      face->d.sprite.coords[0].z = z2 + 0xff;
      face->d.sprite.coords[2].z = z2 + 0xff;
      face->d.sprite.coords[1].z = z2 + 0xff;
      face->d.sprite.coords[3].z = z2 + 0xff;
      if (sideOrdering[dir].south != 0) {
         face->d.sprite.otOfs = sideOrdering[dir].south - 7;
         AddEvtPrim5(gGraphicsPtr->ot, face);
      }
   }

   switch (evt->state) {
   case 0:
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      UpdateCrateElevation(evt);
      EVT.y += gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)] * 0x100;
      EVT.stack = ++gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)];
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].elevation += 2;
      EVT.terrain = gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_PLAINS;
      evt->state++;

   // fallthrough
   case 1:
      if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action != TA_CRATE_PUSHED ||
          gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].cachedByte >= EVT.stack) {
         if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action == TA_CRATE_PUSHED_PRE &&
             gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].cachedByte == EVT.stack) {
            if (ScanForCrateDestination(evt) == 0) {
               // Can't be pushed
               gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
            } else {
               gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_CRATE_PUSHED;
               gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)]--;
               gTerrainPtr[HI(EVT.z)][HI(EVT.x)].elevation -= 2;
               evt->state += 2;
               evt->state2 = 0;
            }
         }
      } else {
         evt->state++;
         evt->state2 = 0;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         EVT.elevation = EVT.y - 0x100;
         EVT.dy = 0;
         evt->state2++;
         break;

      case 1:
         EVT.y -= EVT.dy;
         EVT.dy += 0x20;
         if (EVT.y <= EVT.elevation) {
            EVT.y = EVT.elevation;
            EVT.stack--;
            evt->state = 1;
            PerformAudioCommand(0x5eb);
            if (EVT.stack == 1) {
               evt1 = Evt_GetUnused();
               evt1->functionIndex = EVTF_DUST_CLOUD_SPAWNER;
               evt1->d.evtf213.x = EVT.x;
               evt1->d.evtf213.z = EVT.z;
               evt1->d.evtf213.y = EVT.y;
            }
         }
         break;
      }

      break;

   case 3:

      switch (evt->state2) {
      case 0:
         if (EVT.terrain != TERRAIN_PLAINS) {
            if (gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)] == 0) {
               gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = EVT.terrain;
            } else {
               evt1 = gEvtDataArray;
               for (i = 0; i < EVT_DATA_CT; i++) {
                  if (evt1->functionIndex == EVTF_MAP_OBJECT_CRATE && evt1 != evt) {
                     break;
                  }
                  evt1++;
               }
               evt1->d.evtf046.terrain = EVT.terrain;
            }
         }

         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
         ScanForCrateDestination(evt);

         EVT.dx = 0;
         EVT.dz = 0;
         if (HI(EVT.dstX) > HI(EVT.x)) {
            EVT.dx = 0x40;
         }
         if (HI(EVT.dstX) < HI(EVT.x)) {
            EVT.dx = -0x40;
         }
         if (HI(EVT.dstZ) > HI(EVT.z)) {
            EVT.dz = 0x40;
         }
         if (HI(EVT.dstZ) < HI(EVT.z)) {
            EVT.dz = -0x40;
         }
         evt->state2++;

      // fallthrough
      case 1:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_CLOUD;
         evt1->d.evtf215.x = EVT.x;
         evt1->d.evtf215.z = EVT.z;
         evt1->d.evtf215.y = EVT.y;

         EVT.x += EVT.dx;
         EVT.z += EVT.dz;

         if (EVT.x == EVT.dstX && EVT.z == EVT.dstZ) {
            if (EVT.dstY == EVT.y) {
               evt->state += 2;
            } else {
               evt->state++;
               evt->state2 = 0;
            }
         }
         break;
      } // switch (evt->state2) (via state:3)

      break;

   case 4:

      switch (evt->state2) {
      case 0:
         EVT.elevation = EVT.dstY;
         EVT.dy = 0x20;
         evt->state2++;

      // fallthrough
      case 1:
         EVT.y -= EVT.dy;
         EVT.dy += 0x20;
         if (EVT.y <= EVT.elevation) {
            EVT.y = EVT.elevation;
            evt->state++;
            PerformAudioCommand(0x5eb);
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_DUST_CLOUD_SPAWNER;
            evt1->d.evtf213.x = EVT.x;
            evt1->d.evtf213.z = EVT.z;
            evt1->d.evtf213.y = EVT.y;
         }
         break;
      }

      break;

   case 5:
      if (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain == TERRAIN_WATER ||
          gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain == TERRAIN_LAVA) {
         PerformAudioCommand(0x5ea);
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_PUSHED_OBJECT_SPLASH;
         // todo replace
         evt1->d.sprite.x1 = EVT.x;
         evt1->d.sprite.z1 = EVT.z;
         evt1->d.sprite.y1 = EVT.y;
         evt1->d.sprite.y2 = 0x100;
         evt->functionIndex = EVTF_NULL;
      } else {
         EVT.terrain = gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain;
         gTerrainPtr[HI(EVT.z)][HI(EVT.x)].terrain = TERRAIN_PLAINS;
         EVT.stack = ++gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)];
         gTerrainPtr[HI(EVT.z)][HI(EVT.x)].elevation += 2;
         evt->state = 1;
      }
      break;
   }
}
