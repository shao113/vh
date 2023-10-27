#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

static BVectorZX sPlatformLocations[5] = {{11, 10}, {10, 11}, {12, 11}, {11, 12}, {11, 11}};

void Map33_SavePlatform(void) {
   MapTileModel *pSavedTile;
   MapTileModel *pMapTile;
   BVectorZX *pLoc;
   s32 i;

   pSavedTile = (MapTileModel *)gScratch1_801317c0;
   pLoc = &sPlatformLocations[0];

   for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
      pMapTile = &gMapRowPointers[pLoc[i].z][pLoc[i].x];
      *pSavedTile = *pMapTile;
      pSavedTile++;
   }
}

void Map33_RestorePlatform(void) {
   MapTileModel *pSavedTile;
   MapTileModel *pMapTile;
   s32 i;

   pSavedTile = (MapTileModel *)gScratch1_801317c0;

   for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
      pMapTile = &gMapRowPointers[sPlatformLocations[i].z][sPlatformLocations[i].x];
      *pMapTile = *pSavedTile;
      pSavedTile++;
   }
}

void Map33_LowerPlatform(s32 steps) {
   s32 i;
   MapTileModel *pSavedTile;

   Map33_SavePlatform();
   pSavedTile = (MapTileModel *)gScratch1_801317c0;

   for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
      AdjustFaceElevation(pSavedTile, 0, steps * 16);
      AdjustFaceElevation(pSavedTile, 1, steps * 16);
      if (i < 4) {
         pSavedTile->vertices[pSavedTile->faces[2][1]].vy += steps * 16;
         pSavedTile->vertices[pSavedTile->faces[2][3]].vy += steps * 16;
      }
      pSavedTile++;
   }

   // The saved copy is modified above, and those changes will be adopted here:
   Map33_RestorePlatform();
}

#undef EVTF
#define EVTF 651
void Evtf651_Map33_LavaPitPlatform(EvtData *evt) {
   s32 i;
   EvtData *unitSprite;
   EvtData *evt_v1;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 != 0) {
         Map33_LowerPlatform(gState.mapState.s.field_0x0);
         for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
            UpdateTileElevation(sPlatformLocations[i].z, sPlatformLocations[i].x);
         }
         EVT.step = gState.mapState.s.field_0x0;
      }
      evt->x2.n = CV(11.5);
      evt->z2.n = CV(11.5);
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 > EVT.step && EVT.step < 10) {
         evt->state2 = 0;
         evt->state++;
         evt->state3 = 0;
         evt->mem = 128;
         evt->y3.n = 4;
         evt->y2.n = 24;
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(39));
         gPlayerControlSuppressed = 1;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         gCameraRotation.vy &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         unitSprite = GetUnitSpriteAtPosition(evt->z2.s.hi, evt->x2.s.hi);
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
         evt_v1->d.evtf026.target = unitSprite;
         *(s16 *)(&evt_v1->d.evtf026.type) = 1; //?
         evt_v1->d.evtf026.zoom = 512;
         evt->state3 = 0;
         evt->state2++;
         break;

      case 1:
         if (++evt->state3 >= 32) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 2:
         evt->state2 = 0;
         evt->state++;
         break;
      }

      break;

   case 3:
      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_MAP33_LOWER_PLATFORM;
      evt->state++;
      break;

   case 4:
      if (--evt->mem <= 0) {
         EVT.step++;
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 5:
      evt->state++;

   // fallthrough
   case 6:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         AssignToMainCamera(&EVT.camera);
         evt->state++;
      }
      break;

   case 7:
      evt->state = 1;
      gPlayerControlSuppressed = 0;
      gState.field_0x96 = 0;
      break;
   }
}

#undef EVTF
#define EVTF 750
void Evtf750_751_Map33_LowerPlatform(EvtData *evt) {
   s32 i, j;
   MapTileModel *pMapTile;
   MapTileModel *pSavedTile;
   u8 *pMapTileFaceVerts;
   u8 *pSavedTileFaceVerts;
   s16 dx, dy, dz;
   EvtData *clonedSprite;
   EvtData *unitSprite;

   switch (evt->state) {
   case 0:
      evt->x2.n = CV(11.5);
      evt->z2.n = CV(11.5);
      evt->mem = 128;
      evt->y3.n = CV(0.015625);
      evt->y2.n = CV(0.09375);
      PerformAudioCommand(AUDIO_CMD_PLAY_XA(39));
      Map33_SavePlatform();
      evt->state++;
      break;

   case 1:
      dx = (evt->mem >> 4) * rcos(evt->state3 * 16) >> 12;
      dz = (evt->mem >> 5) * rcos(evt->state3 * 8) >> 12;
      EVT.ydrop += evt->y2.n;
      if (EVT.ydrop > CV(0.5)) {
         EVT.ydrop = CV(0.5);
         evt->y2.n = -(evt->y2.n / 2);
      }
      evt->y2.n += evt->y3.n;
      dy = EVT.ydrop >> 3;

      for (i = 0, pSavedTile = (MapTileModel *)gScratch1_801317c0;
           i < ARRAY_COUNT(sPlatformLocations); i++) {
         pMapTile = &gMapRowPointers[sPlatformLocations[i].z][sPlatformLocations[i].x];
         pMapTileFaceVerts = &pMapTile->faces[0][0];
         pSavedTileFaceVerts = &pSavedTile->faces[0][0];
         for (j = 0; j < 4; j++) {
            pMapTile->vertices[pMapTileFaceVerts[j]].vx =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vx + dx;
            pMapTile->vertices[pMapTileFaceVerts[j]].vz =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vz + dz;
            pMapTile->vertices[pMapTileFaceVerts[j]].vy =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vy + dy;
         }
         pMapTileFaceVerts = &pMapTile->faces[1][0];
         pSavedTileFaceVerts = &pSavedTile->faces[1][0];
         for (j = 0; j < 4; j++) {
            pMapTile->vertices[pMapTileFaceVerts[j]].vx =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vx + dx;
            pMapTile->vertices[pMapTileFaceVerts[j]].vz =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vz + dz;
            pMapTile->vertices[pMapTileFaceVerts[j]].vy =
                pSavedTile->vertices[pSavedTileFaceVerts[j]].vy + dy;
         }
         if (i < 4) {
            pMapTile->vertices[pMapTile->faces[2][1]].vx =
                pSavedTile->vertices[pSavedTile->faces[2][1]].vx + dx;
            pMapTile->vertices[pMapTile->faces[2][1]].vz =
                pSavedTile->vertices[pSavedTile->faces[2][1]].vz + dz;
            pMapTile->vertices[pMapTile->faces[2][1]].vy =
                pSavedTile->vertices[pSavedTile->faces[2][1]].vy + dy;
            pMapTile->vertices[pMapTile->faces[2][3]].vx =
                pSavedTile->vertices[pSavedTile->faces[2][3]].vx + dx;
            pMapTile->vertices[pMapTile->faces[2][3]].vz =
                pSavedTile->vertices[pSavedTile->faces[2][3]].vz + dz;
            pMapTile->vertices[pMapTile->faces[2][3]].vy =
                pSavedTile->vertices[pSavedTile->faces[2][3]].vy + dy;
         }
         pSavedTile++;
      }

      clonedSprite = Evt_GetUnused();
      clonedSprite->functionIndex = EVTF_NOOP;
      if (evt->functionIndex == EVTF_MAP33_SCN65_LOWER_PLATFORM) {
         unitSprite = EVT.variant_0x24.entitySpriteParam;
      } else {
         unitSprite = GetUnitSpriteAtPosition(evt->z2.s.hi, evt->x2.s.hi);
      }
      CopyEvtData(unitSprite, clonedSprite);
      unitSprite->d.sprite.hidden = 1;
      clonedSprite->d.sprite.hidden = 0;
      clonedSprite->x1.n = unitSprite->x1.n + (dx << 3);
      clonedSprite->z1.n = unitSprite->z1.n + (dz << 3);
      clonedSprite->y1.n = unitSprite->y1.n - (dy << 3);
      RenderUnitSprite(gGraphicsPtr->ot, clonedSprite, 1);
      clonedSprite->functionIndex = EVTF_NULL;

      evt->state3 += 8;
      if (--evt->mem <= 0) {
         EVT.variant_0x24.unk++;
         evt->state3 = 0;
         unitSprite->d.sprite.hidden = 0;
         evt->state++;
      }
      break;

   case 2:
      pSavedTile = (MapTileModel *)gScratch1_801317c0;
      for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
         AdjustFaceElevation(pSavedTile, 0, 16);
         AdjustFaceElevation(pSavedTile, 1, 16);
         if (i < 4) {
            pSavedTile->vertices[pSavedTile->faces[2][1]].vy += 16;
            pSavedTile->vertices[pSavedTile->faces[2][3]].vy += 16;
         }
         pSavedTile++;
      }
      Map33_RestorePlatform();
      for (i = 0; i < ARRAY_COUNT(sPlatformLocations); i++) {
         gTerrainPtr[sPlatformLocations[i].z][sPlatformLocations[i].x].s.elevation =
             -gMapRowPointers[sPlatformLocations[i].z][sPlatformLocations[i].x].vertices[0].vy >> 4;
      }
      evt->state++;

   // fallthrough
   case 3:
      evt->state++;
      break;

   case 4:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 653
void Evtf653_ExplodingTile(EvtData *evt) {
   MapTileModel transformedModel;
   MapTileModel *pBaseModel;
   s16 y, z, x;
   s16 elevation;
   EvtData *evt_s1;
   s32 theta;
   SVECTOR *pRot;
   SVECTOR *pTrans;

   memset(&transformedModel, 0, sizeof(MapTileModel));
   pBaseModel = EVT.tileModel;
   transformedModel = *pBaseModel;

   switch (evt->state) {
   case 0:
      pTrans = &EVT.translation;
      evt->x1.n = pTrans->vx;
      evt->y1.n = pTrans->vy;
      evt->z1.n = pTrans->vz;
      // memcpy(&EVT.savedTranslation, &EVT.translation, sizeof(SVECTOR));
      ((u32 *)&EVT.savedTranslation)[0] = ((u32 *)pTrans)[0];
      ((u32 *)&EVT.savedTranslation)[1] = ((u32 *)pTrans)[1];
      evt->state++;

   // fallthrough
   case 1:
      pTrans = &EVT.translation;
      // memcpy(&EVT.translation, &EVT.savedTranslation, sizeof(SVECTOR));
      ((u32 *)pTrans)[0] = ((u32 *)&EVT.savedTranslation)[0];
      ((u32 *)pTrans)[1] = ((u32 *)&EVT.savedTranslation)[1];
      evt->x1.n = pTrans->vx;
      evt->y1.n = pTrans->vy;
      evt->z1.n = pTrans->vz;
      theta = (rand() >> 2) % DEG(360);
      evt->x2.n = CV(0.0625) * rcos(theta) >> 12;
      evt->z2.n = CV(0.0625) * rsin(theta) >> 12;
      evt->y2.n = CV(0.625);
      evt->y3.n = CV(-0.046875);
      evt->x3.n = 0;
      evt->z3.n = 0;
      evt->state2 = 3;
      evt->mem = 0;
      evt->state++;

   // fallthrough
   case 2:
      if (--evt->state3 <= 0) {
         evt->state3 = 0;
         evt->state++;
      }
      pBaseModel = EVT.tileModel;
      pTrans = &EVT.translation;
      pRot = &EVT.rotation;
      pTrans->vx = evt->x1.n;
      pTrans->vy = evt->y1.n;
      pTrans->vz = evt->z1.n;
      RotTransMapTile(pBaseModel, pRot, pTrans, &transformedModel);
      RenderMapTile(gGraphicsPtr->ot, &transformedModel, GRID_COLOR_NONE);
      break;

   case 3:
      pBaseModel = EVT.tileModel;
      pTrans = &EVT.translation;
      pRot = &EVT.rotation;
      evt->state3++;

      if (evt->state2 != 0) {
         pRot->vy = evt->state3 << 8; //* DEG(22.5);
         pRot->vz = evt->state3 << 8;
      }

      x = evt->x1.n;
      y = evt->y1.n;
      z = evt->z1.n;

      evt->y1.n += evt->y2.n;
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y2.n += evt->y3.n;

      if (evt->mem != 0) {
         elevation = evt->y1.n;
      } else {
         elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) - CV(0.5);
      }

      if (elevation > evt->y1.n) {
         if (OBJ_TERRAIN(evt).s.terrain != TERRAIN_WATER) {
            evt->y1.n = elevation;
            evt->y2.n = -(evt->y2.n >> 1);
            if (evt->state2 != 0) {
               evt->state2--;
            } else {
               evt->x2.n = 0;
               evt->y2.n = 0;
               evt->z2.n = 0;
               evt->y3.n = 0;
               evt->state3 = 224;
            }
         } else {
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_SPLASH_WITH_DROPLETS;
            evt_s1->x1.n = evt->x1.n;
            evt_s1->z1.n = evt->z1.n;
            evt_s1->y1.n = elevation;
            evt_s1->y2.n = -evt->y2.n;
            evt->y1.n = elevation;
            evt->y2.n = 0;
            evt->y3.n = 0;
            evt->x2.n = 0;
            evt->z2.n = EVT.riverVelocity;
            evt->state2 = 0;
            evt->state3 = 224;
            evt->mem = 1;
            return;
         }
      }

      if (evt->state3 >= 256) {
         evt->functionIndex = EVTF_NULL;
      }

      if ((evt->x1.s.hi < gMapMinX || evt->x1.s.hi > gMapMaxX) ||
          (evt->z1.s.hi < gMapMinZ || evt->z1.s.hi > gMapMaxZ)) {
         evt->functionIndex = EVTF_NULL;
      } else {
         pTrans->vx = evt->x1.n;
         pTrans->vy = evt->y1.n;
         pTrans->vz = evt->z1.n;
         RotTransMapTile(pBaseModel, pRot, &EVT.translation, &transformedModel);
         RenderMapTile(gGraphicsPtr->ot, &transformedModel, GRID_COLOR_NONE);
         if (evt->state2 != 0) {
            evt_s1 = CreatePositionedEvt(evt, EVTF_EXPLOSION);
            evt_s1->mem = 1;
            evt_s1->x2.n = (x - evt->x1.n) >> 2;
            evt_s1->z2.n = (z - evt->z1.n) >> 2;
            evt_s1->y2.n = ((y - evt->y1.n) >> 2) + (rand() >> 2) % 16;
         }
      }
      break;
   }
}
