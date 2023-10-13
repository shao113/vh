#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

//...
void Map17_RemoveWater(void);
//...

void Map17_LowerFloodgate(void) {
   s32 ix;

   for (ix = 6; ix <= 7; ix++) {
      AdjustFaceElevation(&gMapRowPointers[15][ix], 1, 48);
      AdjustFaceElevation(&gMapRowPointers[15][ix], 2, 48);
      AdjustFaceElevation(&gMapRowPointers[15][ix], 3, 16);
      gTerrainPtr[15][ix].s.elevation = -gMapRowPointers[15][ix].vertices[0].vy >> 4;
      UpdateTileElevation(15, ix);
   }
}

#undef EVTF
#define EVTF 366
void Evtf366_Map17_Button(EvtData *evt) {
   EvtData *buttonDepress;

   switch (evt->state) {
   case 0:
      evt->x1.s.hi = 10;
      evt->x1.s.lo = CV(0.5);
      evt->z1.s.hi = 16;
      evt->z1.s.lo = CV(0.5);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);

      if (gState.mapState.s.field_0x0 != 0) {
         Map17_RemoveWater();
         DepressButton(10, 16);
         Map17_LowerFloodgate();
         evt->functionIndex = EVTF_NULL;
         return;
      }

      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 1) {
         evt->state2 = 48;
         AssignFromMainCamera(&EVT.camera);
         EVT.unused_0x24 = 0x200;

         buttonDepress = Evt_GetUnused();
         buttonDepress->functionIndex = EVTF_BUTTON_DEPRESS;
         EVT.buttonDepress = buttonDepress;
         buttonDepress->x1.n = evt->x1.n;
         buttonDepress->z1.n = evt->z1.n;
         buttonDepress->y1.n = evt->y1.n;

         gPlayerControlSuppressed = 1;
         evt->state += 2;
      }
      break;

   case 3:
      buttonDepress = EVT.buttonDepress;
      if (buttonDepress->state == 99) {
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(35));
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 4:
      if (--evt->state3 <= 0) {
         gState.mapState.s.field_0x0 = 2;
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 5:
      if (++evt->state3 == 16) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(35));
      }
      if (gState.mapState.s.field_0x0 == 4) {
         evt->state3 = 48;
         evt->state++;
      }
      break;

   case 6:
      if (--evt->state3 <= 0) {
         evt->state3 = 8;
         evt->state++;
      }
      EaseOutCamera(&EVT.camera, 2);
      break;

   case 7:
      if (--evt->state3 <= 0) {
         evt->functionIndex = EVTF_NULL;
         AssignToMainCamera(&EVT.camera);
         gState.signal = 0;
         gPlayerControlSuppressed = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 296
void Evtf296_Map17_SprayParticle(EvtData *evt) {
   Quad unusedQuad = {{-2, -2, 0, 0}, {2, -2, 0, 0}, {-2, 2, 0, 0}, {2, 2, 0, 0}};

   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_DOTS;
      evt->d.sprite.boxIdx = 5;
      evt->d.sprite.clut = 3;
      evt->d.sprite.semiTrans = 4;

      evt->state3 = rand() % DEG(90) + DEG(225);
      evt->x2.n = CV(0.25) * rcos(evt->state3) >> 12;
      evt->z2.n = CV(0.21875) * rsin(evt->state3) >> 12;
      evt->y2.n = CV(0.25) + (CV(0.0625) * rsin(rand() % DEG(360)) >> 12);
      evt->y3.n = CV(-0.015625);

      evt->state2 = 0;
      evt->state++;

   // fallthrough
   case 1:
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->y2.n += evt->y3.n;

      if (++evt->state2 >= 32) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 365
void Evtf365_Map17_Floodgate(EvtData *evt) {
   s32 i;
   MapTileModel *pTileModel;
   EvtData *drainage;
   EvtData *particle;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 != 0) {
         evt->functionIndex = EVTF_NULL;
         return;
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 2) {
         evt->state++;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         evt->state3 = 0;
         evt->x1.n = CV(6.5);
         evt->z1.n = CV(15.5);
         evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
         EVT.dstCamRotY = DEG(-45);

         pTileModel = &gMapRowPointers[15][6];
         EVT.face1Init = EVT.face1Curr = pTileModel->vertices[pTileModel->faces[1][0]].vy << 3;
         EVT.face2Init = EVT.face2Curr = pTileModel->vertices[pTileModel->faces[2][0]].vy << 3;
         EVT.face3Init = EVT.face3Curr = pTileModel->vertices[pTileModel->faces[3][0]].vy << 3;

         evt->y3.n = 8;
         evt->state2++;

      // fallthrough
      case 1:
         PanCamera(CV(6.5), evt->y1.n, CV(15.5), 3);
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 2;

         if (++evt->state3 >= 48) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 2:
         EVT.face1Curr += evt->y3.n;
         EVT.face2Curr += evt->y3.n;

         pTileModel = &gMapRowPointers[15][6];
         SetFaceElevation(pTileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(pTileModel, 2, EVT.face2Curr >> 3);

         pTileModel = &gMapRowPointers[15][7];
         SetFaceElevation(pTileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(pTileModel, 2, EVT.face2Curr >> 3);

         if (++evt->state3 >= 32) {
            drainage = Evt_GetUnused();
            drainage->functionIndex = EVTF_MAP17_DRAINING_WATER;
            evt->y3.n = 1;
            evt->state2++;
         }
         break;

      case 3:
         EVT.face1Curr += evt->y3.n;
         EVT.face2Curr += evt->y3.n;
         EVT.face3Curr += evt->y3.n;

         pTileModel = &gMapRowPointers[15][6];
         SetFaceElevation(pTileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(pTileModel, 2, EVT.face2Curr >> 3);
         SetFaceElevation(pTileModel, 3, EVT.face3Curr >> 3);

         pTileModel = &gMapRowPointers[15][7];
         SetFaceElevation(pTileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(pTileModel, 2, EVT.face2Curr >> 3);
         SetFaceElevation(pTileModel, 3, EVT.face3Curr >> 3);

         if (((EVT.face1Curr - EVT.face1Init) >> 3) >= 48) {
            pTileModel = &gMapRowPointers[15][6];
            SetFaceElevation(pTileModel, 1, (EVT.face1Init >> 3) + 48);
            SetFaceElevation(pTileModel, 2, (EVT.face2Init >> 3) + 48);
            SetFaceElevation(pTileModel, 3, (EVT.face3Init >> 3) + 16);

            pTileModel = &gMapRowPointers[15][7];
            SetFaceElevation(pTileModel, 1, (EVT.face1Init >> 3) + 48);
            SetFaceElevation(pTileModel, 2, (EVT.face2Init >> 3) + 48);
            SetFaceElevation(pTileModel, 3, (EVT.face3Init >> 3) + 16);

            UpdateTileElevation(15, 6);
            UpdateTileElevation(15, 7);
            evt->state2++;
         }

         if (--evt->mem <= 0) {
            for (i = 0; i < 4; i++) {
               particle = Evt_GetUnused();
               particle->functionIndex = EVTF_MAP17_SPRAY_PARTICLE;
               particle->x1.n = i * CV(0.25) + CV(6.0);
               particle->z1.n = i * CV(0.25) + CV(15.0);
               particle->y1.n = GetTerrainElevation(14, 6 + i / 2);

               particle = Evt_GetUnused();
               particle->functionIndex = EVTF_MAP17_SPRAY_PARTICLE;
               particle->x1.n = i * CV(0.25) + CV(7.0);
               particle->z1.n = CV(15.0);
               particle->y1.n = GetTerrainElevation(14, 6 + i * 2);
            }
            evt->mem = (rand() >> 2) % 3 + 3;
         }
         break;

      case 4:
         evt->state3 = 0;
         evt->state++;
         break;
      } // switch (evt->state2) (via state:2)

      break;

   case 3:
      gTerrainPtr[15][6].s.elevation = -gMapRowPointers[15][6].vertices[0].vy >> 4;
      gTerrainPtr[15][7].s.elevation = -gMapRowPointers[15][7].vertices[0].vy >> 4;
      evt->state++;
      break;

   case 4:
      if (gState.mapState.s.field_0x0 == 3) {
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 5:
      if (--evt->state3 <= 0) {
         gState.mapState.s.field_0x0 = 4;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void Map17_RemoveWater(void) {
   s32 iz, ix;

   for (iz = 0; iz <= 1; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         AdjustFaceElevation(&gMapRowPointers[iz][ix], 0, 16);
      }
   }
   for (iz = 4; iz <= 14; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         AdjustFaceElevation(&gMapRowPointers[iz][ix], 0, 16);
      }
   }
   for (iz = 2; iz <= 3; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         AdjustFaceElevation(&gMapRowPointers[iz][ix], 1, 16);
      }
   }
   for (iz = 0; iz <= 1; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         gMapRowPointers[iz][ix].gfx[0] = GFX_MAP_TEXTURE_192;
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_PLAINS;
      }
   }
   for (iz = 4; iz <= 14; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         gMapRowPointers[iz][ix].gfx[0] = GFX_MAP_TEXTURE_192;
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_PLAINS;
      }
   }
   for (iz = 2; iz <= 3; iz++) {
      for (ix = 6; ix <= 7; ix++) {
         gMapRowPointers[iz][ix].gfx[1] = GFX_MAP_TEXTURE_192;
      }
   }
}

#undef EVTF
#define EVTF 368
void Evtf368_Map17_DrainingWater(EvtData *evt) {
   static Quad quad = {{-128, 0, 128, 0}, {128, 0, 128, 0}, {-128, 0, -128, 0}, {128, 0, -128, 0}};

   s32 iz, ix, i, j;
   s16 elevation;
   EvtData *sprite;
   SVectorXZY *pCoord;
   POLY_FT4 *poly;
   u8 fade;
   s8 unused[32];

   switch (evt->state) {
   case 0:
      evt->state++;

   // fallthrough
   case 1:
      evt->state++;
      evt->state2 = 1;
      evt->state3 = 128;
      evt->mem = 4;
      break;

   case 2:
      Map17_RemoveWater();
      evt->state++;

   // fallthrough
   case 3:
      evt->mem = evt->state3 >> 5;

      if (evt->state3 != 0) {
         sprite = Evt_GetUnused();
         sprite->functionIndex = EVTF_NOOP;
         sprite->d.sprite.gfxIdx = GFX_MAP_TEXTURE_51; // Flowing water
         elevation = GetTerrainElevation(0, 6);

         for (iz = 0; iz <= 14; iz++) {
            for (ix = 6; ix <= 7; ix++) {
               pCoord = &sprite->d.sprite.coords[0];
               for (i = 0; i < 4; i++) {
                  pCoord->x = ix * CV(1.0) + quad[i].vx + CV(0.5);
                  pCoord->z = iz * CV(1.0) + quad[i].vz + CV(0.5);
                  pCoord->y = elevation + evt->state3;
                  pCoord++;
               }

               pCoord = &sprite->d.sprite.coords[0];
               sprite->x1.n = (pCoord[0].x + pCoord[1].x + pCoord[2].x + pCoord[3].x) / 4;
               sprite->z1.n = (pCoord[0].z + pCoord[1].z + pCoord[2].z + pCoord[3].z) / 4;
               sprite->y1.n = (pCoord[0].y + pCoord[1].y + pCoord[2].y + pCoord[3].y) / 4;

               sprite->d.sprite.semiTrans = 1;

               for (j = 0; j < evt->mem; j++) {
                  sprite->d.sprite.gfxIdx = GFX_MAP_TEXTURE_51;
                  AddEvtPrim5(gGraphicsPtr->ot, sprite);
                  poly = &gGraphicsPtr->quads[gQuadIndex - 1];
                  fade = 0x100 - evt->state3;
                  setRGB0(poly, fade, fade, fade);
               }

               for (j = evt->mem; j < 2; j++) {
                  sprite->d.sprite.semiTrans = 2;
                  sprite->d.sprite.gfxIdx = GFX_MAP_TEXTURE_51;
                  poly = &gGraphicsPtr->quads[gQuadIndex];
                  AddEvtPrim5(gGraphicsPtr->ot, sprite);

                  fade = evt->state3;
                  if (fade < 0x20) {
                     fade = 0x20;
                  }
                  setRGB0(poly, fade, fade, fade);
               }
            }
         }

         sprite->functionIndex = EVTF_NULL;
      }

      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 4:
      evt->state++;
      break;

   case 5:
      gState.mapState.s.field_0x0 = 3;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}
