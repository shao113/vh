#include "common.h"
#include "evt.h"
#include "field.h"
#include "units.h"
#include "graphics.h"
#include "state.h"
#include "field.h"
#include "battle.h"
#include "window.h"

void Evtf035_MapObject_Tree(EvtData *);
void Evtf036_MapObject_GraveMarker(EvtData *);
void Evtf415_MapObject_Torch(EvtData *);
void Evtf037_MapObject_Fountain(EvtData *);
void Evtf038_MapObject_LampPost(EvtData *);
void Evtf039_MapObject_Flag(EvtData *);
void Evtf045_BloodSpurtOffset(EvtData *);
void Evtf591_MapObject_Boulder(EvtData *);
void Evtf046_MapObject_Crate(EvtData *);
s32 ScanForCrateDestination(EvtData *);
s32 ScanForBoulderDestination(EvtData *);
void UpdateCrateElevation(EvtData *);
void Evtf020_PushedBoulder(EvtData *);
void Evtf048_Push(EvtData *);
void TiltChestLid(EvtData *, s16, u8);
void Evtf040_MapObject_Chest(EvtData *);

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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_NO_ENTRY;
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
         if (gState.mapNum == 40 && gState.mapState.s.field_0x0 == 0) {
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
      if (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain == TERRAIN_WATER ||
          gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain == TERRAIN_LAVA) {
         newEvt->functionIndex = EVTF_PUSHED_OBJECT_SPLASH;
         PerformAudioCommand(0x5ea);
      } else {
         newEvt->functionIndex = EVTF_BOULDER_RUBBLE;
         if (!(gState.mapNum == 40 && gState.mapState.s.field_0x0 == 0)) {
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
      if (gState.mapNum == 40 && gState.mapState.s.field_0x0 == 0) {
         // Destroyed barricade
         gState.mapState.s.field_0x0 = 1;
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
      EVT.y += (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.elevation * 0x80 - EVT.y) >> 2;
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
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.elevation += 2;
      EVT.terrain = gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_PLAINS;
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
               gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.elevation -= 2;
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
               gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = EVT.terrain;
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
      if (gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain == TERRAIN_WATER ||
          gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain == TERRAIN_LAVA) {
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
         EVT.terrain = gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain;
         gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_PLAINS;
         EVT.stack = ++gCrateGrid_Ptr[HI(EVT.z)][HI(EVT.x)];
         gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.elevation += 2;
         evt->state = 1;
      }
      break;
   }
}

s32 ScanForCrateDestination(EvtData *evt) {
   s32 dx, dz;
   s32 unitHeight;

   dx = 0;
   dz = 0;

   switch (gTileStateGridPtr[HI(evt->d.evtf046.z)][HI(evt->d.evtf046.x)].cachedShort >> 10) {
   case DIR_SOUTH:
      dz = 1;
      dx = 0;
      break;
   case DIR_WEST:
      dz = 0;
      dx = 1;
      break;
   case DIR_NORTH:
      dz = -1;
      dx = 0;
      break;
   case DIR_EAST:
      dz = 0;
      dx = -1;
      break;
   }

   evt->d.evtf046.dstX = evt->d.evtf046.x;
   evt->d.evtf046.dstZ = evt->d.evtf046.z;
   HI(evt->d.evtf046.dstX) = dx + HI(evt->d.evtf046.dstX);
   HI(evt->d.evtf046.dstZ) = dz + HI(evt->d.evtf046.dstZ);

   if (gMapUnitsPtr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].raw != 0) {
      unitHeight = 0x100;
   } else {
      unitHeight = 0;
   }

   if (gTerrainPtr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].s.terrain ==
       TERRAIN_NO_ENTRY) {
      HI(evt->d.evtf046.dstX) -= dx;
      HI(evt->d.evtf046.dstZ) -= dz;
      evt->d.evtf046.dstY = evt->d.evtf046.y;
      return 0;
   } else {
      if (evt->d.evtf046.y >
          -(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy << 3) +
              (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8) +
              unitHeight) {
         evt->d.evtf046.dstY =
             -(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy
               << 3) +
             (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8);
      } else if (evt->d.evtf046.y <
                 -(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy
                   << 3) +
                     (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8) +
                     unitHeight) {
         HI(evt->d.evtf046.dstX) -= dx;
         HI(evt->d.evtf046.dstZ) -= dz;
         evt->d.evtf046.dstY = evt->d.evtf046.y;
         return 0;
      } else if (evt->d.evtf046.y <=
                 (-(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy
                    << 3) +
                  (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8))) {
         evt->d.evtf046.dstY =
             -(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy
               << 3) +
             (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8);
      } else {
         evt->d.evtf046.dstY =
             -(gMapRowPointers[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)].vertices[0].vy
               << 3) +
             (gCrateGrid_Ptr[HI(evt->d.evtf046.dstZ)][HI(evt->d.evtf046.dstX)] << 8);
      }
      return 1;
   }
}

s32 ScanForBoulderDestination(EvtData *evt) {
   s32 dx, dz;
   s16 elevation;
   s32 boulderHeight;
   s32 hasRoom;

   dx = 0;
   dz = 0;

   switch (gTileStateGridPtr[HI(evt->d.evtf591.z)][HI(evt->d.evtf591.x)].cachedShort >> 10) {
   case DIR_SOUTH:
      dz = 1;
      dx = 0;
      break;
   case DIR_WEST:
      dz = 0;
      dx = 1;
      break;
   case DIR_NORTH:
      dz = -1;
      dx = 0;
      break;
   case DIR_EAST:
      dz = 0;
      dx = -1;
      break;
   }

   evt->d.evtf591.dstX = evt->d.evtf591.x;
   evt->d.evtf591.dstZ = evt->d.evtf591.z;
   elevation = evt->d.evtf591.y;
   hasRoom = 0;

   while (1) {
      HI(evt->d.evtf591.dstX) = dx + HI(evt->d.evtf591.dstX);
      HI(evt->d.evtf591.dstZ) = dz + HI(evt->d.evtf591.dstZ);

      if (gMapUnitsPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.team == TEAM_BOULDER) {
         boulderHeight = 0x100;
      } else {
         boulderHeight = 0;
      }

      if (elevation <
          ((gMapRowPointers[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].height * 0x80) +
           (gCrateGrid_Ptr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)] * 0x100) +
           boulderHeight)) {
         HI(evt->d.evtf591.dstX) -= dx;
         HI(evt->d.evtf591.dstZ) -= dz;
         return hasRoom;
      }

      if (gMapUnitsPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.team != TEAM_NULL &&
          gMapUnitsPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.unitIdx == 0) {
         HI(evt->d.evtf591.dstX) -= dx;
         HI(evt->d.evtf591.dstZ) -= dz;
         return hasRoom;
      }

      if (gTerrainPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.terrain ==
              TERRAIN_WATER ||
          gTerrainPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.terrain == TERRAIN_LAVA) {
         return 1;
      }

      elevation = gTerrainPtr[HI(evt->d.evtf591.dstZ)][HI(evt->d.evtf591.dstX)].s.elevation * 128;
      hasRoom = 1;
   }
}

void UpdateCrateElevation(EvtData *evt) {
   evt->d.evtf046.y =
       (-gMapRowPointers[HI(evt->d.evtf046.z)][HI(evt->d.evtf046.x)].vertices[0].vy << 3);
}

#undef EVTF
#define EVTF 020
void Evtf020_PushedBoulder(EvtData *evt) {
   EvtData *unitSprite;

   unitSprite = EVT.unitSprite;

   switch (evt->state) {
   case 0:
      gPlayerControlSuppressed = 1;
      if (gState.mapNum == 40 && gState.mapState.s.field_0x0 == 0) {
         PerformAudioCommand(0x1370);
      } else {
         PerformAudioCommand(0x136f);
      }
      gSignal5 = 0;
      EVT.savedCamX = gCameraPos.vx;
      EVT.savedCamZ = gCameraPos.vz;
      EVT.savedCamY = gCameraPos.vy;
      EVT.savedCamRotX = gCameraRotation.vx;
      EVT.savedZoom = gCameraZoom.vz;
      EVT.timer = 30;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         gCameraPos.vx += (-(unitSprite->d.sprite.x1 >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(unitSprite->d.sprite.z1 >> 3) - gCameraPos.vz) >> 2;
         gCameraPos.vy += ((unitSprite->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;
         gCameraRotation.vx += (0x180 - gCameraRotation.vx) >> 2;
         gCameraZoom.vz += (250 - gCameraZoom.vz) >> 2;
      } else {
         gSignal5 = 1;
         evt->state++;
      }
      break;

   case 2:
      if (gSignal5 == 2) {
         evt->state++;
      }
      break;

   case 3:
      if (gSignal5 != 3) {
         gCameraZoom.vz += (512 - gCameraZoom.vz) >> 3;
         EVT.timer = 20;
      } else {
         evt->state++;
      }
      break;

   case 4:
      if (--EVT.timer != 0) {
         gCameraRotation.vx += (EVT.savedCamRotX - gCameraRotation.vx) >> 1;
         gCameraZoom.vz += (EVT.savedZoom - gCameraZoom.vz) >> 1;
         gCameraPos.vx += (EVT.savedCamX - gCameraPos.vx) >> 2;
         gCameraPos.vz += (EVT.savedCamZ - gCameraPos.vz) >> 2;
         gCameraPos.vy += (EVT.savedCamY - gCameraPos.vy) >> 2;
      } else {
         gCameraZoom.vz = EVT.savedZoom;
         gCameraRotation.vx = ((gCameraRotation.vx - 64) >> 8) * 0x100 + 0x80;
         gSignal2 = 99;
         evt->functionIndex = EVTF_NULL;
      }
   }
}

#undef EVTF
#define EVTF 048
void Evtf048_Push(EvtData *evt) {
   EvtData *sprite;
   EvtData *evt1;
   s16 dir;
   s16 fInner, fOuter, nfInner, nfOuter;
   s16 targetZ, targetX;
   s32 i;
   s32 j;
   s32 stack;
   s32 tmp2;
   // s16 crateElevation;
   // s16 srcElevation;
   s32 targetElevation;

   sprite = EVT.unit->evtBattler;
   sprite = sprite->d.evtf014.sprite;
   dir = 0;

   switch (evt->state) {
   case 0:
      EVT.x = sprite->d.sprite.x1;
      EVT.z = sprite->d.sprite.z1;
      EVT.y = sprite->d.sprite.y1 + 0x80;
      gSignal4 = sprite->d.sprite.direction;
      evt->state++;

   // fallthrough
   case 1:

      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         dir = ANGLE_0_DEGREES;
         break;
      case CAM_DIR_EAST:
         dir = ANGLE_270_DEGREES;
         break;
      case CAM_DIR_NORTH:
         dir = ANGLE_180_DEGREES;
         break;
      case CAM_DIR_WEST:
         dir = ANGLE_90_DEGREES;
         break;
      }

      if (gPadStateNewPresses & PAD_UP) {
         gSignal4 = dir + ANGLE_0_DEGREES;
      }
      if (gPadStateNewPresses & PAD_RIGHT) {
         gSignal4 = dir + ANGLE_90_DEGREES;
      }
      if (gPadStateNewPresses & PAD_DOWN) {
         gSignal4 = dir + ANGLE_180_DEGREES;
      }
      if (gPadStateNewPresses & PAD_LEFT) {
         gSignal4 = dir + ANGLE_270_DEGREES;
      }
      gSignal4 &= 0xfff;

      if (gPadStateNewPresses & PAD_X) {
         gSignal2 = 2;
         evt->functionIndex = EVTF_NULL;
         return;
      }

      EVT.osc += ANGLE_45_DEGREES;

      fInner = (rcos(EVT.osc & 0xfff) * 50 >> 12) + 192;
      fOuter = (rcos(EVT.osc & 0xfff) * 130 >> 12) + 512;
      nfInner = (rcos(EVT.osc & 0xfff) * 25 >> 12) + 192;
      nfOuter = (rcos(EVT.osc & 0xfff) * 50 >> 12) + 384;
      evt1 = Evt_GetUnused();

      RenderDirectionArrow(evt, evt1, 0, gSignal4 == 0, fInner, fOuter, nfInner, nfOuter);
      RenderDirectionArrow(evt, evt1, 0x400, gSignal4 == 0x400, fInner, fOuter, nfInner, nfOuter);
      RenderDirectionArrow(evt, evt1, 0x800, gSignal4 == 0x800, fInner, fOuter, nfInner, nfOuter);
      RenderDirectionArrow(evt, evt1, 0xc00, gSignal4 == 0xc00, fInner, fOuter, nfInner, nfOuter);

      targetX = HI(sprite->d.sprite.x1);
      targetZ = HI(sprite->d.sprite.z1);

      switch (gSignal4 >> 10) {
      case DIR_SOUTH:
         targetZ++;
         break;
      case DIR_WEST:
         targetX++;
         break;
      case DIR_NORTH:
         targetZ--;
         break;
      case DIR_EAST:
         targetX--;
         break;
      }

      EVT.direction = gSignal4;
      EVT.targetX = targetX;
      EVT.targetZ = targetZ;

      if ((gPadStateNewPresses & PAD_CIRCLE) &&
          gMapUnitsPtr[targetZ][targetX].s.team == TEAM_BOULDER) {

         i = gTerrainPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].s.elevation -
             gTerrainPtr[targetZ][targetX].s.elevation;
         //?
         if (i + 1U < 3) {
            gTileStateGridPtr[targetZ][targetX].cachedShort = gSignal4;

            evt1 = gEvtDataArray;
            for (i = 0; i < EVT_DATA_CT; i++) {
               if (evt1->functionIndex == EVTF_MAP_OBJECT_BOULDER &&
                   HI(evt1->d.evtf591.x) == targetX && HI(evt1->d.evtf591.z) == targetZ) {
                  if (ScanForBoulderDestination(evt1)) {
                     gSignal2 = 1;
                     evt->state = 5;
                     return;
                  }
                  break;
               }
               evt1++;
            }
         }
      } // CIRCLE (Boulders)

      if ((gPadStateNewPresses & PAD_CIRCLE) && gCrateGrid_Ptr[targetZ][targetX] != 0 &&
          gCrateGrid_Ptr[targetZ][targetX] != 0x7f) {
         // TODO: Clean-up.
         //@2a68
         tmp2 = gTerrainPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].s.elevation + 2;
         targetElevation = gTerrainPtr[targetZ][targetX].s.elevation;

         if (gTerrainPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].s.elevation >=
             targetElevation - gCrateGrid_Ptr[targetZ][targetX] * 2 - 1) {

            if (tmp2 - 1 <= targetElevation) {
               j = targetElevation - gCrateGrid_Ptr[targetZ][targetX] * 2;
               tmp2 = tmp2 - 1;
               stack = 0;

               do {
                  j += 2; //@2ae4
                  stack++;
               } while (j < tmp2);

               //@2af4
               EVT.stack = stack;
               gTileStateGridPtr[targetZ][targetX].cachedByte = stack;
               gTileStateGridPtr[targetZ][targetX].cachedShort = gSignal4;

               evt1 = gEvtDataArray;
               for (i = 0; i < EVT_DATA_CT; i++) {
                  if (evt1->functionIndex == EVTF_MAP_OBJECT_CRATE &&
                      stack == evt1->d.evtf046.stack &&
                      (HI(evt1->d.evtf046.x) == targetX && HI(evt1->d.evtf046.z) == targetZ)) {

                     if (!ScanForCrateDestination(evt1)) {
                        return;
                     }

                     break;
                  }
                  evt1++;
               }

               gSignal2 = 1;
               evt->state++;
            }
         }
      } // CIRCLE (Crates)
      break;

   case 2:
      gTileStateGridPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].action = TA_PUSHING_CRATE;
      gSignal3 = 0;
      evt->state++;

   // fallthrough
   case 3:
      if (gSignal3 != 0) {
         gTileStateGridPtr[EVT.targetZ][EVT.targetX].action = TA_CRATE_PUSHED_PRE;
         gTileStateGridPtr[EVT.targetZ][EVT.targetX].cachedByte = EVT.stack;
         gTileStateGridPtr[EVT.targetZ][EVT.targetX].cachedShort = EVT.direction;
         EVT.timer = 20;
         evt->state++;
      }
      break;

   case 4:
      if (--EVT.timer == 0) {
         gSignal2 = 99;
         evt->functionIndex = EVTF_NULL;
      }
      break;

   case 5:
      gTileStateGridPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].action =
          TA_PUSHING_BOULDER;
      gSignal3 = 0;
      evt->state++;

   // fallthrough
   case 6:
      if (gSignal3 != 0) {
         gTileStateGridPtr[EVT.targetZ][EVT.targetX].action = TA_BOULDER_PUSHED;
         gTileStateGridPtr[EVT.targetZ][EVT.targetX].cachedShort = EVT.direction;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void TiltChestLid(EvtData *chest, s16 angle, u8 facing) {
   if (facing == 0) {
      chest->d.evtf040.lid_todo_x5c =
          chest->d.evtf040.lid_todo_x58 + (rcos(angle & 0xfff) * 0xe0 >> 12);
      chest->d.evtf040.lid_todo_x5e =
          chest->d.evtf040.lid_todo_x5a + (rcos((angle + 0x400U) & 0xfff) * 0xe0 >> 12);
      chest->d.evtf040.lid_todo_x50 =
          chest->d.evtf040.lid_todo_x58 + (rcos((angle - 0x280U) & 0xfff) * 0x60 >> 12);
      chest->d.evtf040.lid_todo_x52 =
          chest->d.evtf040.lid_todo_x5a + (rcos((angle + 0x180U) & 0xfff) * 0x60 >> 12);
      chest->d.evtf040.lid_todo_x54 =
          chest->d.evtf040.lid_todo_x58 + (rcos((angle - 0x100U) & 0xfff) * 0xc0 >> 12);
      chest->d.evtf040.lid_todo_x56 =
          chest->d.evtf040.lid_todo_x5a + (rcos((angle + 0x300U) & 0xfff) * 0xc0 >> 12);
   } else {
      chest->d.evtf040.lid_todo_x58 =
          chest->d.evtf040.lid_todo_x5c - (rcos(angle & 0xfff) * 0xe0 >> 12);
      chest->d.evtf040.lid_todo_x5a =
          chest->d.evtf040.lid_todo_x5e + (rcos((angle + 0x400U) & 0xfff) * 0xe0 >> 12);
      chest->d.evtf040.lid_todo_x54 =
          chest->d.evtf040.lid_todo_x5c - (rcos((angle - 0x280U) & 0xfff) * 0x60 >> 12);
      chest->d.evtf040.lid_todo_x56 =
          chest->d.evtf040.lid_todo_x5e + (rcos((angle + 0x180U) & 0xfff) * 0x60 >> 12);
      chest->d.evtf040.lid_todo_x50 =
          chest->d.evtf040.lid_todo_x5c - (rcos((angle - 0x100U) & 0xfff) * 0xc0 >> 12);
      chest->d.evtf040.lid_todo_x52 =
          chest->d.evtf040.lid_todo_x5e + (rcos((angle + 0x300U) & 0xfff) * 0xc0 >> 12);
   }
}

#undef EVTF
#define EVTF 040
void Evtf040_MapObject_Chest(EvtData *evt) {
   static s16 chestGfx[10] = {GFX_CHEST_4, GFX_CHEST_4, GFX_CHEST_2, GFX_CHEST_2, GFX_CHEST_3,
                              GFX_CHEST_3, GFX_CHEST_2, GFX_CHEST_2, GFX_CHEST_1, GFX_CHEST_1};
   // static u8 mimicLevels[40];
   // static u8 mimicExpMulti[40];
   static u8 mimicData[80] = {
       10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 1, 1, 1, 1, 1,  1, 1,  1, 1, 1,
       1,  1,  1,  1,  1,  1,  1,  18, 1,  1,  1, 1, 1, 1, 21, 1, 23, 1, 1, 1,

       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1, 1, 1, 1, 1,  1, 1,  1, 1, 1,
       1,  1,  1,  1,  1,  1,  1,  8,  1,  1,  1, 1, 1, 1, 12, 1, 15, 1, 1, 0,
   };

   EvtData *face;
   EvtData *newEvt;
   s32 i;
   s16 idx;
   u8 *pMimicExpMulti; // Starts halfway into mimicData

   face = gTempGfxEvt;
   //?: Maybe a remnant of mapObj.param = 0?
   *(s16 *)(&face->d.sprite.hidden) = 0;
   idx = 0;
   if (gRedAttackGridPtr[HI(EVT.z)][HI(EVT.x)] == PATH_STEP_UNSET) {
      face->d.sprite.clut = 0;
   } else {
      face->d.sprite.clut = 3;
   }
   face->d.sprite.otOfs = 2;

   switch (evt->state) {
   case 0:
      EVT.item = EVT.item;
      gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.team = TEAM_CHEST;
      EVT.terrain = gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain;
      gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = TERRAIN_OBSTACLE;
      LO(EVT.x) = 0x80;
      LO(EVT.z) = 0x80;
      EVT.y = GetTerrainElevation(HI(EVT.z), HI(EVT.x));
      EVT.lid_todo_x50 = EVT.z - 0x40;
      EVT.lid_todo_x54 = EVT.z + 0x40;
      EVT.lid_todo_x58 = EVT.z - 0x70;
      EVT.lid_todo_x5c = EVT.z + 0x70;
      EVT.lid_todo_x52 = EVT.y + 0xd0;
      EVT.lid_todo_x56 = EVT.y + 0xd0;
      EVT.lid_todo_x5a = EVT.y + 0x80;
      EVT.lid_todo_x5e = EVT.y + 0x80;
      evt->state++;
      break;

   case 1:
      if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action == TA_CHEST_1) {
         gTerrainPtr[HI(EVT.z)][HI(EVT.x)].s.terrain = EVT.terrain;
         gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.team = TEAM_NULL;
         EVT.lidAngleVel = -0xc0;
         evt->mem = 3;
         gSignal3 = 0;
         newEvt = Evt_GetUnused();
         newEvt->d.sprite.x1 = EVT.x;
         newEvt->d.sprite.z1 = EVT.z;
         newEvt->d.sprite.y1 = EVT.y;
         if (EVT.item != 0) {
            newEvt->functionIndex = EVTF_REVEAL_CHEST_ITEM;
            newEvt->d.evtf290.gfxIdx = GFX_ITEM_ICONS_OFS + EVT.item;
            PerformAudioCommand(0x324);
         } else {
            newEvt->functionIndex = EVTF_REVEAL_MIMIC;
            gState.D_8014053E = 0;
            PerformAudioCommand(0x384);
         }
         evt->state++;
      }
      if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action == TA_CHEST_2) {
         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].cachedByte = EVT.item;
      }
      if (gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action == TA_CHEST_3) {
         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
         EVT.facing = gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].cachedByte;
         if (EVT.facing == 2 && gMapUnitsPtr[gTargetZ - 1][gTargetX].s.team == TEAM_NULL) {
            EVT.facing = 0;
         }
      }
      break;

   case 2:
      if (--evt->mem == 0) {
         evt->mem = 75;
         evt->state++;
      }
      break;

   case 3:
      if (--evt->mem != 0) {
         EVT.lidAngleVel -= 0x10;
         EVT.lidAngle += EVT.lidAngleVel;
         if (EVT.lidAngle < -0x600) {
            EVT.lidAngle = -0x600;
            EVT.lidAngleVel = (-EVT.lidAngleVel >> 1);
         }
         TiltChestLid(evt, EVT.lidAngle, EVT.facing);
         break;
      }
      if (EVT.item == 0) {
         // Spawn a mimic
         evt->state = 6;
         break;
      }
      if (gTileStateGridPtr[gTargetZ][gTargetX].cachedByte == 0) {
         ShowReceivedItemDialog(EVT.item, 0x3c, 0);
      } else {
         ShowDepotReceivedItemDialog(EVT.item, 0x3c, 0);
      }
      evt->mem = 50;
      evt->state++;
      break;

   case 4:
      if ((--evt->mem == 0) || (gPadStateNewPresses & PAD_CIRCLE)) {
         CloseWindow(0x3c);
         evt->state = 5;
      }
      break;

   case 5:
      gSignal3 = 1;
      gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
      evt->functionIndex = EVTF_NULL;
      break;

   case 6:
      evt->mem = 30;
      i = 0;
      while (gSpriteStripUnitIds[i] != UNIT_ID_MIMIC) {
         i++;
      }
      pMimicExpMulti = &mimicData[40 + gState.mapNum];
      SetupBattleUnit(i, HI(EVT.z), HI(EVT.x), mimicData[gState.mapNum], TEAM_ENEMY,
                      gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].cachedShort, 99, *pMimicExpMulti, 99);
      evt->state++;
      break;

   case 7:
      if (--evt->mem == 0) {
         gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_NONE;
         gState.D_8014053E = 1;
         gSignal3 = 1;
         evt->functionIndex = EVTF_NULL;
      }
      return;
   }

   if (!IsSpriteOutsideVisibleRange(evt)) {
      face->d.sprite.coords[0].x = EVT.x - 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = EVT.x + 0x70;
      face->d.sprite.coords[3].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[0].z = EVT.z - 0x70;
      face->d.sprite.coords[1].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[2].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[3].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[0].y = EVT.y + 0x80;
      face->d.sprite.coords[1].y = face->d.sprite.coords[0].y;
      face->d.sprite.coords[2].y = EVT.y;
      face->d.sprite.coords[3].y = face->d.sprite.coords[2].y;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[0]

      face->d.sprite.coords[0].z = EVT.z + 0x70;
      face->d.sprite.coords[1].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[2].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[3].z = face->d.sprite.coords[0].z;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[1]

      face->d.sprite.coords[0].x = EVT.x + 0x70;
      face->d.sprite.coords[1].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[3].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[0].z = EVT.z - 0x70;
      face->d.sprite.coords[2].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[1].z = EVT.z + 0x70;
      face->d.sprite.coords[3].z = face->d.sprite.coords[1].z;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[2]

      face->d.sprite.coords[0].x = EVT.x - 0x70;
      face->d.sprite.coords[1].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[3].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[0].z = EVT.z - 0x70;
      face->d.sprite.coords[2].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[1].z = EVT.z + 0x70;
      face->d.sprite.coords[3].z = face->d.sprite.coords[1].z;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[3]

      face->d.sprite.coords[0].x = EVT.x - 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = EVT.x + 0x70;
      face->d.sprite.coords[3].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[0].z = EVT.lid_todo_x50;
      face->d.sprite.coords[1].z = EVT.lid_todo_x50;
      face->d.sprite.coords[2].z = EVT.lid_todo_x58;
      face->d.sprite.coords[3].z = EVT.lid_todo_x58;
      face->d.sprite.coords[0].y = EVT.lid_todo_x52;
      face->d.sprite.coords[1].y = EVT.lid_todo_x52;
      face->d.sprite.coords[2].y = EVT.lid_todo_x5a;
      face->d.sprite.coords[3].y = EVT.lid_todo_x5a;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[4]

      face->d.sprite.coords[0].z = EVT.lid_todo_x54;
      face->d.sprite.coords[1].z = EVT.lid_todo_x54;
      face->d.sprite.coords[2].z = EVT.lid_todo_x5c;
      face->d.sprite.coords[3].z = EVT.lid_todo_x5c;
      face->d.sprite.coords[0].y = EVT.lid_todo_x56;
      face->d.sprite.coords[1].y = EVT.lid_todo_x56;
      face->d.sprite.coords[2].y = EVT.lid_todo_x5e;
      face->d.sprite.coords[3].y = EVT.lid_todo_x5e;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[5]

      face->d.sprite.coords[0].x = EVT.x + 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[3].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[0].z = EVT.lid_todo_x50;
      face->d.sprite.coords[1].z = EVT.lid_todo_x54;
      face->d.sprite.coords[2].z = EVT.lid_todo_x58;
      face->d.sprite.coords[3].z = EVT.lid_todo_x5c;
      face->d.sprite.coords[0].y = EVT.lid_todo_x52;
      face->d.sprite.coords[1].y = EVT.lid_todo_x56;
      face->d.sprite.coords[2].y = EVT.lid_todo_x5a;
      face->d.sprite.coords[3].y = EVT.lid_todo_x5e;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[6]

      face->d.sprite.coords[0].x = EVT.x - 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[3].x = face->d.sprite.coords[0].x;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[7]

      face->d.sprite.coords[0].x = EVT.x + 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = EVT.x - 0x70;
      face->d.sprite.coords[3].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[0].z = EVT.lid_todo_x54;
      face->d.sprite.coords[1].z = EVT.lid_todo_x54;
      face->d.sprite.coords[2].z = EVT.lid_todo_x50;
      face->d.sprite.coords[3].z = EVT.lid_todo_x50;
      face->d.sprite.coords[0].y = EVT.lid_todo_x56;
      face->d.sprite.coords[1].y = EVT.lid_todo_x56;
      face->d.sprite.coords[2].y = EVT.lid_todo_x52;
      face->d.sprite.coords[3].y = EVT.lid_todo_x52;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[8]

      face->d.sprite.coords[0].x = EVT.x - 0x70;
      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[1].x = EVT.x + 0x70;
      face->d.sprite.coords[3].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[0].z = EVT.z - 0x70;
      face->d.sprite.coords[1].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[2].z = EVT.z + 0x70;
      face->d.sprite.coords[3].z = face->d.sprite.coords[2].z;
      face->d.sprite.coords[0].y = EVT.y;
      face->d.sprite.coords[1].y = face->d.sprite.coords[0].y;
      face->d.sprite.coords[2].y = face->d.sprite.coords[0].y;
      face->d.sprite.coords[3].y = face->d.sprite.coords[0].y;
      face->d.sprite.otOfs = 2;
      face->d.sprite.gfxIdx = chestGfx[idx++];
      AddEvtPrim4(gGraphicsPtr->ot, face); //[9]
   }
}
