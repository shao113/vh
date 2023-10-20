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
   MapTileModel *tileModel;
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

         tileModel = &gMapRowPointers[15][6];
         EVT.face1Init = EVT.face1Curr = tileModel->vertices[tileModel->faces[1][0]].vy << 3;
         EVT.face2Init = EVT.face2Curr = tileModel->vertices[tileModel->faces[2][0]].vy << 3;
         EVT.face3Init = EVT.face3Curr = tileModel->vertices[tileModel->faces[3][0]].vy << 3;

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

         tileModel = &gMapRowPointers[15][6];
         SetFaceElevation(tileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(tileModel, 2, EVT.face2Curr >> 3);

         tileModel = &gMapRowPointers[15][7];
         SetFaceElevation(tileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(tileModel, 2, EVT.face2Curr >> 3);

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

         tileModel = &gMapRowPointers[15][6];
         SetFaceElevation(tileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(tileModel, 2, EVT.face2Curr >> 3);
         SetFaceElevation(tileModel, 3, EVT.face3Curr >> 3);

         tileModel = &gMapRowPointers[15][7];
         SetFaceElevation(tileModel, 1, EVT.face1Curr >> 3);
         SetFaceElevation(tileModel, 2, EVT.face2Curr >> 3);
         SetFaceElevation(tileModel, 3, EVT.face3Curr >> 3);

         if (((EVT.face1Curr - EVT.face1Init) >> 3) >= 48) {
            tileModel = &gMapRowPointers[15][6];
            SetFaceElevation(tileModel, 1, (EVT.face1Init >> 3) + 48);
            SetFaceElevation(tileModel, 2, (EVT.face2Init >> 3) + 48);
            SetFaceElevation(tileModel, 3, (EVT.face3Init >> 3) + 16);

            tileModel = &gMapRowPointers[15][7];
            SetFaceElevation(tileModel, 1, (EVT.face1Init >> 3) + 48);
            SetFaceElevation(tileModel, 2, (EVT.face2Init >> 3) + 48);
            SetFaceElevation(tileModel, 3, (EVT.face3Init >> 3) + 16);

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

#undef EVTF
#define EVTF 358
void Evtf358_Map19_Elevator(EvtData *evt) {
   MapTileModel *tileModel;
   s8 sign;
   s16 diff;

   switch (evt->state) {
   case 0:
      evt->state++;

   // fallthrough
   case 1:
      if (evt->state2 != 1) {
         return;
      }
      evt->state++;

   // fallthrough
   case 2:
      evt->state3 = 0x100;
      EVT.todo_x24 = 0;
      EVT.todo_x26 = 0;
      EVT.todo_x28 = 0;
      EVT.todo_x2c = 0x100;
      EVT.todo_x2e = 0x100;
      EVT.todo_x30 = 0x100;
      EVT.todo_x34 = 0;
      EVT.todo_x36 = 0;
      EVT.todo_x38 = 0;
      evt->state++;

   // fallthrough
   case 3:
      tileModel = &OBJ_TILE_MODEL(evt);
      sign = (evt->mem != 0) ? +1 : -1;

      EVT.todo_x24 += EVT.todo_x2c;
      diff = (EVT.todo_x24 >> 8) - (EVT.todo_x34 >> 8);
      if (diff > 0) {
         AdjustFaceElevation(tileModel, 0, diff * sign);
         EVT.todo_x34 += diff * 0x100;
      }

      EVT.todo_x26 += EVT.todo_x2e;
      diff = (EVT.todo_x26 >> 8) - (EVT.todo_x36 >> 8);
      if (diff > 0) {
         AdjustFaceElevation(tileModel, 1, diff * sign);
         EVT.todo_x36 += diff * 0x100;
      }

      EVT.todo_x28 += EVT.todo_x30;
      diff = (EVT.todo_x28 >> 8) - (EVT.todo_x38 >> 8);
      if (diff > 0) {
         tileModel->vertices[tileModel->faces[2][0]].vy += diff * sign;
         tileModel->vertices[tileModel->faces[2][1]].vy += diff * sign;
         tileModel->vertices[tileModel->faces[2][2]].vy += diff * sign;
         EVT.todo_x38 += diff * 0x100;
         OBJ_TERRAIN(evt).s.elevation = -tileModel->vertices[0].vy >> 4;
      }

      if (EVT.todo_x24 >= 0x3000) {
         evt->state++;
      }
      break;

   case 4:
      OBJ_TERRAIN(evt).s.elevation = -OBJ_TILE_MODEL(evt).vertices[0].vy >> 4;
      evt->state2 = 0;
      evt->state++;
      break;

   case 5:
      if (evt->mem != 0) {
         evt->mem = 0;
      } else {
         evt->mem = 1;
      }
      evt->state = 1;
      break;

   case 6:
      break;
   }
}

#undef EVTF
#define EVTF 355
void Evtf355_356_Map19_Elevator(EvtData *evt) {
   EvtData *elevatorA;
   EvtData *elevatorB;

   switch (evt->state) {
   case 0:
      if (evt->functionIndex == EVTF_MAP19_ELEVATOR_356) {
         EVT.elevator2 = EVT.elevator2Param;
         EVT.elevator1 = EVT.variant_0x24.elevator1Param;
      } else if (evt->functionIndex == EVTF_MAP19_ELEVATOR_355) {
         Evt_GetUnused(); //?
         EVT.elevator2 = EVT.elevator2Param;
         EVT.elevator1 = EVT.variant_0x24.elevator1Param;
      }
      AssignFromMainCamera(&EVT.camera);
      elevatorB = EVT.elevator1;
      elevatorA = EVT.elevator2;
      if (elevatorB->z1.n < elevatorA->z1.n) {
         EVT.variant_0x24.dstCamRotY.a = DEG(45);
         EVT.variant_0x24.dstCamRotY.b = DEG(135);
      } else {
         EVT.variant_0x24.dstCamRotY.a = DEG(135);
         EVT.variant_0x24.dstCamRotY.b = DEG(45);
      }
      gPlayerControlSuppressed = 1;
      evt->state3 = 0;
      evt->state2 = 0;
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(38));
      evt->state++;
      break;

   case 1:
      elevatorB = EVT.elevator1;

      switch (evt->state3) {
      case 0:
         PanCamera(elevatorB->x1.n + CV(0.5), elevatorB->y1.n + CV(0.5), elevatorB->z1.n + CV(0.5),
                   2);
         gCameraRotation.vy += (EVT.variant_0x24.dstCamRotY.a - gCameraRotation.vy) >> 2;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         gCameraZoom.vz += (384 - gCameraZoom.vz) >> 2;
         if (++evt->state2 >= 48) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(38));
            evt->state3++;
         }
         break;

      case 1:
         gCameraRotation.vy = EVT.variant_0x24.dstCamRotY.a;
         gCameraRotation.vx = DEG(33.75);
         gCameraZoom.vz = 384;
         evt->state3++;
         break;

      case 2:
         elevatorB = EVT.elevator1;
         if (elevatorB->state2 == 0) {
            elevatorB->state2 = 1;
            evt->state2 = 0;
            evt->state3 = 0;
            evt->state++;
         }
         break;
      } // switch (evt->state3) (via state:1)

      break;

   case 2:
      elevatorB = EVT.elevator1;
      if (elevatorB->state2 == 0) {
         evt->state++;
      }
      break;

   case 3:
      elevatorB = EVT.elevator2;

      switch (evt->state3) {
      case 0:
         evt->state2 = 0;
         evt->state3++;

      // fallthrough
      case 1:
         if (++evt->state2 >= 32) {
            evt->state2 = 0;
            evt->state3++;
         }
         break;

      case 2:
         gCameraZoom.vz += (768 - gCameraZoom.vz) >> 3;
         if (++evt->state2 >= 32) {
            evt->state2 = 0;
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(38));
            evt->state3++;
         }
         break;

      case 3:
         PanCamera(elevatorB->x1.n + CV(0.5), elevatorB->y1.n + CV(0.5), elevatorB->z1.n + CV(0.5),
                   4);
         gCameraRotation.vy += (EVT.variant_0x24.dstCamRotY.b - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 4;
         gCameraZoom.vz += (384 - gCameraZoom.vz) >> 4;
         if (++evt->state2 >= 64) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(38));
            evt->state3++;
         }
         break;

      case 4:
         gCameraRotation.vy = EVT.variant_0x24.dstCamRotY.b;
         gCameraRotation.vx = DEG(33.75);
         gCameraZoom.vz = 384;
         evt->state3++;
         break;

      case 5:
         elevatorB = EVT.elevator2;
         if (elevatorB->state2 == 0) {
            elevatorB->state2 = 1;
            evt->state2 = 0;
            evt->state3 = 0;
            evt->state++;
         }
      } // switch (evt->state3) (via state:3)

      break;

   case 4:
      elevatorB = EVT.elevator2;
      if (elevatorB->state2 == 0) {
         evt->state++;
      }
      break;

   case 5:

      switch (evt->state3) {
      case 0:
         evt->state2 = 0;
         evt->state3++;

      // fallthrough
      case 1:
         if (++evt->state2 >= 32) {
            evt->state2 = 0;
            evt->state3++;
         }
         break;

      case 2:
         EaseOutCamera(&EVT.camera, 2);
         if (++evt->state2 >= 48) {
            evt->state3++;
         }
         break;

      case 3:
         AssignToMainCamera(&EVT.camera);
         evt->state3++;
         break;

      case 4:
         evt->functionIndex = EVTF_NULL;
         gPlayerControlSuppressed = 0;
         gState.mapState.s.field_0x0 = 0;
         gState.signal = 0;
         break;
      } // switch (evt->state3) (via state:5)

      break;
   }
}

#undef EVTF
#define EVTF 357
void Evtf357_Map19(EvtData *evt) {
   EvtData *evt_s3;
   EvtData *evt_v1;
   EvtData *evt_s1;
   EvtData *evt_s0;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x1 != 0) {
         AdjustFaceElevation2(&gMapRowPointers[3][19], 0, -48);
         AdjustFaceElevation2(&gMapRowPointers[3][19], 1, -48);
         AdjustFaceElevation2(&gMapRowPointers[3][19], 2, -48);
         UpdateTileElevation(3, 19);
         AdjustFaceElevation2(&gMapRowPointers[12][19], 0, 48);
         AdjustFaceElevation2(&gMapRowPointers[12][19], 1, 48);
         AdjustFaceElevation2(&gMapRowPointers[12][19], 2, 48);
         UpdateTileElevation(12, 19);
      }

      evt_s3 = Evt_GetUnused();
      evt_s3->functionIndex = EVTF_MAP19_ELEVATOR;
      evt_s3->z1.s.hi = 3;
      evt_s3->x1.s.hi = 19;
      evt_s3->y1.n = CV(2.0);
      EVT.elevator1 = evt_s3;

      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_MAP19_ELEVATOR;
      evt_v1->z1.s.hi = 12;
      evt_v1->x1.s.hi = 19;
      evt_v1->y1.n = CV(2.0);
      EVT.elevator2 = evt_v1;

      if (gState.mapState.s.field_0x1 == 0) {
         evt_s3->mem = 0;
         evt_v1->mem = 1;
         gState.mapState.s.field_0x2 = 1;
      } else {
         evt_s3->mem = 1;
         evt_v1->mem = 0;
         gState.mapState.s.field_0x1 = 1;
         gState.mapState.s.field_0x2 = 0;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = EVT.elevator1;
      evt_s0 = EVT.elevator2;

      // Set to 1 or 2 by Evtf435_EvaluateBattle19, depending on which side is examined.
      if (gState.mapState.s.field_0x0 == 1) {
         evt_s3 = Evt_GetUnused();
         evt_s3->functionIndex = EVTF_MAP19_ELEVATOR_356;
         evt_s3->d.evtf356.variant_0x24.elevator1Param = evt_s1;
         evt_s3->d.evtf356.elevator2Param = evt_s0;
         evt->state++;
      } else if (gState.mapState.s.field_0x0 == 2) {
         evt_s3 = Evt_GetUnused();
         evt_s3->functionIndex = EVTF_MAP19_ELEVATOR_355;
         evt_s3->d.evtf356.variant_0x24.elevator1Param = evt_s0;
         evt_s3->d.evtf356.elevator2Param = evt_s1;
         evt->state++;
      }

      break;

   case 2:
      if (gState.mapState.s.field_0x0 == 0) {
         gState.mapState.s.field_0x1++;
         gState.mapState.s.field_0x1 &= 1;
         gState.mapState.s.field_0x2++;
         gState.mapState.s.field_0x2 &= 1;
         evt->state = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 354
void Evtf354_Map19_DebugElevators(EvtData *evt) {
   // Unused? (Name chosen based on location and behavior.)

   switch (evt->state) {
   case 0:
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 0 && (gSavedPad2StateNewPresses & PAD_L2)) {
         gState.mapState.s.field_0x0 = 1;
         evt->state++;
      } else if (gState.mapState.s.field_0x0 == 0 && (gSavedPad2StateNewPresses & PAD_R2)) {
         gState.mapState.s.field_0x0 = 2;
         evt->state++;
      }
      break;

   case 2:
      if (gState.mapState.s.field_0x0 == 0) {
         evt->state = 1;
      }
      break;
   }
}
