#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"
#include "battle.h"
#include "units.h"

static s16 sCarXPositions[4] = {CV(10.0), CV(20.0), CV(32.0), CV(42.0)};

// TODO: Integrate autogen
static MapTileModel sMapTileModels_801009bc[4] = {
#include "assets/801009bc.inc"
};

#undef EVTF
#define EVTF 673
void Evtf673_Map32_Scn63_Cinematic(EvtData *evt) {
   EvtData *focus;

   focus = EVT.focus;

   switch (evt->state) {
   case 0:
      AssignFromMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 1;
      evt->state++;
      evt->x1.n = CV(108.0);
      evt->z1.n = CV(4.0);
      evt->y1.n = CV(8.0);
      evt->x2.n = CV(60.0);
      evt->z2.n = CV(4.0);
      evt->y2.n = CV(2.5);
      PerformAudioCommand(AUDIO_CMD_PLAY_XA(84));
      focus = Evt_GetUnused();
      focus->functionIndex = EVTF_NOOP;
      EVT.focus = focus;
      focus->x1.n = evt->x2.n;
      focus->z1.n = evt->z2.n;
      focus->y1.n = evt->y2.n;
      gState.focus = focus;
      func_800985F0(evt);
      break;

   case 1:
      func_800986F0(evt);
      focus->x1.n = evt->x2.n;
      focus->z1.n = evt->z2.n;
      focus->y1.n = evt->y2.n;
      evt->state++;

   // fallthrough
   case 2:
      evt->x1.n -= CV(0.5);
      evt->y1.n += CV(0.0625);
      func_800986F0(evt);
      focus->x1.n = evt->x2.n;
      focus->z1.n = evt->z2.n;
      focus->y1.n = evt->y2.n;

      if (++evt->mem >= 64) {
         evt->state++;
         evt->z3.n = CV(16.0);
         evt->state3 = 0;
      }
      break;

   case 3:
      evt->x1.n = evt->x2.n + (evt->z3.n * rcos(evt->state3) >> 12);
      evt->z1.n = evt->z2.n + (evt->z3.n * rsin(evt->state3) >> 12);
      evt->y1.n -= CV(0.015625);
      evt->x2.n -= CV(0.25);

      if (evt->x2.n < CV(7.0)) {
         evt->x2.n = CV(7.0);
      }

      switch (evt->state2) {
      case 0:
         evt->z3.n -= CV(0.25);
         if (evt->z3.n <= CV(5.5)) {
            EVT.unused_0x3C = 0;
            evt->state2++;
         }
         break;

      case 1:
         evt->z3.n += CV(0.375);
         if (evt->z3.n >= CV(24.0)) {
            evt->state2++;
         }
         break;

      case 2:
         evt->z3.n -= CV(0.0625);
         if (evt->z3.n <= CV(16.5)) {
            evt->state2++;
         }
         break;
      }

      func_800986F0((EvtData *)evt);
      focus->x1.n = evt->x2.n;
      focus->z1.n = evt->z2.n;
      focus->y1.n = evt->y2.n;
      evt->state3 -= 8;

      if (++evt->mem >= 512) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 4:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->mem >= 64) {
         evt->state++;
      }
      break;

   case 5:
      AssignToMainCamera(&EVT.camera);
      gState.focus = NULL;
      evt->state++;
      break;

   case 6:
      evt->functionIndex = EVTF_NULL;
      break;
   }

   gState.eventCameraRot = gCameraRotation.vy;
}

void Map32_RemoveCar(s32 idx) {
   extern void ShiftTile(MapTileModel *, s16, s16);

   s32 x;
   s32 ix, iz;
   MapTileModel *tileModel;
   MapTileModel *railTileModel;

   x = sCarXPositions[idx] / CV(1.0);

   for (ix = 2; ix < x; ix++) {
      for (iz = 0; iz <= 7; iz++) {
         // Replace tiles w/ empty rail, and shift into place
         tileModel = &gMapRowPointers[iz][ix];
         railTileModel = &gMapRowPointers[iz][ix % 2];
         *tileModel = *railTileModel;
         ShiftTile(tileModel, ix / 2 * 64, 0);
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_NO_ENTRY;
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
      }
   }
}

#undef EVTF
#define EVTF 650
void Evtf650_Map32_CarRelease(EvtData *evt) {
   extern void ShiftTile(MapTileModel *, s16, s16);

   static s32 audioCommands[5] = {AUDIO_CMD_PLAY_XA(85), AUDIO_CMD_PLAY_XA(85),
                                  AUDIO_CMD_PLAY_XA(85), AUDIO_CMD_PLAY_XA(86),
                                  AUDIO_CMD_PLAY_XA(86)};

   s32 i, j, k;
   s16 startX, endX;
   EvtData *evt_s0;
   EvtData *unitSprite;
   UnitStatus *unit;
   SVECTOR translation;
   SVECTOR rotation = {0, 0, 0};
   MapTileModel tileModels[4];
   MapTileModel *tileModel;
   MapTileModel *tileModel2;
   s16 savedCamPosX, savedCamPosY;
   s32 flag;

   startX = EVT.startX / CV(1.0);
   endX = EVT.endX / CV(1.0);

   switch (evt->state) {
   case 0:
      for (i = 0; i < ARRAY_COUNT(sMapTileModels_801009bc); i++) {
         UpdateMapTileLighting(&sMapTileModels_801009bc[i]);
      }
      if (gState.mapState.s.field_0x0 != 0) {
         Map32_RemoveCar(gState.mapState.s.field_0x0 - 1);
         EVT.startX = sCarXPositions[gState.mapState.s.field_0x0 - 1];
         EVT.carIdx = gState.mapState.s.field_0x0;
      } else {
         EVT.startX = CV(2.0);
         EVT.carIdx = 0;
         EVT.endX = 0;
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 > EVT.carIdx && EVT.carIdx <= 3) {
         AssignFromMainCamera(&EVT.camera);
         evt->x1.n = sCarXPositions[EVT.carIdx];
         evt->z1.n = CV(6.0);
         evt->y1.n = CV(4.0);

         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
         evt_s0->d.evtf026.target = evt;
         evt_s0->d.evtf026.zoom = 384;

         evt->state2 = 0;
         evt->state++;
         EVT.endX = sCarXPositions[EVT.carIdx];
         evt->x2.n = 0;
         evt->x3.n = -3;
         EVT.todo_x26 = 0;

         startX = EVT.startX / CV(1.0);
         endX = EVT.endX / CV(1.0);
         EVT.hasCasualties = 0;
         k = 0;
         for (i = startX; i < endX; i++) {
            for (j = 0; j < 8; j++) {
               if (gMapUnitsPtr[j][i].s.unitIdx != 0) {
                  k++;
               }
            }
         }
         if (k != 0) {
            EVT.hasCasualties = 1;
         }
         if (EVT.hasCasualties) {
            PerformAudioCommand(0x1000 + audioCommands[0]);
         } else {
            PerformAudioCommand(0x1000 + audioCommands[EVT.carIdx]);
         }
         gPlayerControlSuppressed = 1;
      }
      break;

   case 2:
      switch (evt->state2) {
      case 0:
         if (++evt->state3 >= 32) {
            evt_s0 = Evt_GetUnused();
            evt_s0->functionIndex = EVTF_BOUNCE_ZOOM;
            if (EVT.hasCasualties) {
               PerformAudioCommand(audioCommands[0]);
            } else {
               PerformAudioCommand(audioCommands[EVT.carIdx]);
            }
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 1:
         if (++evt->state3 >= 4) {
            for (i = 0; i < 16; i++) {
               evt_s0 = CreatePositionedEvt(evt, EVTF_MAP32_SMOKESTACK_PARTICLE);
               evt_s0->z2.n = rand() % CV(0.37890625) - CV(0.1875);
               evt_s0->x2.n = rand() % CV(0.015625) + CV(0.109375);
               evt_s0->y2.n = rand() % CV(0.015625) + CV(0.0078125);
               evt_s0->z3.n = evt_s0->z2.n >> 2;
               evt_s0->x3.n = -(rand() % CV(0.015625) + CV(0.125));
               evt_s0->z1.n = i * CV(0.125) + CV(4.0);
            }
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 2:
         if (++evt->state3 >= 30) {
            startX = EVT.startX / CV(1.0);
            endX = EVT.endX / CV(1.0);
            k = 0;
            for (i = startX; i < endX; i++) {
               for (j = 0; j < 8; j++) {
                  if (gMapUnitsPtr[j][i].s.unitIdx != 0) {
                     k++;
                  }
               }
            }
            if (k != 0) {
               PerformAudioCommand(AUDIO_CMD_FADE_OUT_128_1);
               evt->state2 += 2;
               evt->state3 = 0;
            } else {
               evt->state2++;
               evt->state3 = 0;
            }
         }
         break;

      case 3:
         if (++evt->state3 >= 15) {
            evt->state3 = 0;
            evt->state2 += 3;
         }
         break;

      case 4:
         if (++evt->state3 >= 0) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 5:
         startX = EVT.startX / CV(1.0);
         endX = EVT.endX / CV(1.0);

         switch (evt->state3) {
         case 0:
            EVT.tileX = startX;
            evt->state3++;

         // fallthrough
         case 1:
            EVT.tileZ = 0;
            evt->state3++;

         // fallthrough
         case 2:
            i = EVT.tileX;
            j = EVT.tileZ;
            if (gMapUnitsPtr[j][i].s.unitIdx != 0) {
               gState.msgFinished = 0;
               gTileStateGridPtr[j][i].action = TA_X20;
               evt->state3++;
            } else {
               evt->state3 += 3;
            }
            break;

         case 3:
            if (gState.msgFinished) {
               evt->state3++;
            }
            break;

         case 4:
            evt->state3++;

         // fallthrough
         case 5:
            EVT.tileZ++;
            if (EVT.tileZ >= 8) {
               evt->state3++;
            } else {
               evt->state3 = 2;
            }
            break;

         case 6:
            EVT.tileX++;
            if (EVT.tileX >= endX) {
               EVT.timer = 0;
               evt->state3++;
               evt_s0 = Evt_GetUnused();
               evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
               evt_s0->d.evtf026.target = evt;
               evt_s0->d.evtf026.zoom = 384;
               PerformAudioCommand(0x1000 + audioCommands[EVT.carIdx]); // Prepare XA
            } else {
               evt->state3 = 1;
            }
            break;

         case 7:
            if (++EVT.timer >= 32) {
               PerformAudioCommand(audioCommands[EVT.carIdx]); // Play XA
               for (i = 0; i < 16; i++) {
                  evt_s0 = CreatePositionedEvt(evt, EVTF_MAP32_SMOKESTACK_PARTICLE);
                  evt_s0->z2.n = rand() % CV(0.37890625) - CV(0.1875);
                  evt_s0->x2.n = rand() % CV(0.015625) + CV(0.109375);
                  evt_s0->y2.n = rand() % CV(0.015625) + CV(0.0078125);
                  evt_s0->z3.n = evt_s0->z2.n >> 2;
                  evt_s0->x3.n = -(rand() % CV(0.015625) + CV(0.125));
                  evt_s0->z1.n = i * CV(0.125) + CV(4.0);
               }
               EVT.timer = 0;
               evt->state3++;
            }
            break;

         case 8:
            if (++EVT.timer >= 60) {
               evt->state3 = 0;
               evt->state2++;
            }
            break;
         } // switch (evt->state3) (via state:2->state2:5)

         break;

      case 6:
         startX = EVT.startX / CV(1.0);
         endX = EVT.endX / CV(1.0);

         for (i = startX; i < endX; i++) {
            for (j = 0; j < 8; j++) {
               tileModel = &gMapRowPointers[j][i];
               for (k = 0; k < ARRAY_COUNT(tileModel->gfx); k++) {
                  if (tileModel->gfx[k] >= GFX_MAP_TEXTURE_7 &&
                      tileModel->gfx[k] <= GFX_MAP_TEXTURE_18) {
                     tileModel->gfx[k] = GFX_TRANSPARENT;
                  }
                  if (tileModel->gfx[k] >= GFX_MAP_TEXTURE_23 &&
                      tileModel->gfx[k] <= GFX_MAP_TEXTURE_34) {
                     tileModel->gfx[k] = GFX_TRANSPARENT;
                  }
               }
            }
         }

         for (i = startX / 2; i < endX / 2; i++) {
            translation.vx = (i + 1) * 512;
            translation.vy = 0;
            translation.vz = -512;
            for (j = 0; j < ARRAY_COUNT(tileModels); j++) {
               tileModels[j] = sMapTileModels_801009bc[j];
               RotTransMapTile(&sMapTileModels_801009bc[j], &rotation, &translation,
                               &tileModels[j]);
            }
         }

         gState.fieldRenderingDisabled = 1;
         evt->state3 = 0;
         evt->state2 = 0;
         evt->state++;

         break;
      } // switch (evt->state2) (via state:2)

      break;

   case 3:
      gCameraZoom.vz += 8;
      startX = EVT.startX / CV(1.0);
      endX = EVT.endX / CV(1.0);

      k = endX + 16;
      if (k > 64) {
         k = 64;
      }

      for (i = D_80122E28; i < startX; i++) {
         for (j = 1; j < 7; j++) {
            tileModel = &gMapRowPointers[j][i];
            RenderMapTile(gGraphicsPtr->ot, tileModel, GRID_COLOR_NONE);
         }
         RenderEdgeMapTile(gGraphicsPtr->ot, &gMapRowPointers[0][i], GRID_COLOR_NONE);
         RenderEdgeMapTile(gGraphicsPtr->ot, &gMapRowPointers[7][i], GRID_COLOR_NONE);
      }

      for (i = endX; i < k; i++) {
         for (j = 1; j < 7; j++) {
            tileModel = &gMapRowPointers[j][i];
            RenderMapTile(gGraphicsPtr->ot, tileModel, GRID_COLOR_NONE);
         }
         RenderEdgeMapTile(gGraphicsPtr->ot, &gMapRowPointers[0][i], GRID_COLOR_NONE);
         RenderEdgeMapTile(gGraphicsPtr->ot, &gMapRowPointers[7][i], GRID_COLOR_NONE);
      }

      for (i = (startX - 10) / 2; i < endX / 2; i++) {
         translation.vx = (i + 1) * 512;
         translation.vy = 0;
         translation.vz = -512;
         for (j = 0; j < ARRAY_COUNT(tileModels); j++) {
            tileModels[j] = sMapTileModels_801009bc[j];
            RotTransMapTile(&sMapTileModels_801009bc[j], &rotation, &translation, &tileModels[j]);
            RenderEdgeMapTile(gGraphicsPtr->ot, &tileModels[j], GRID_COLOR_NONE);
         }
      }

      savedCamPosY = gCameraPos.vy;
      savedCamPosX = gCameraPos.vx;
      PushMatrix();
      RotMatrix(&gCameraRotation, &gCameraMatrix);
      TransMatrix(&gCameraMatrix, &gCameraZoom);
      SetRotMatrix(&gCameraMatrix);
      SetTransMatrix(&gCameraMatrix);
      gCameraPos.vx += EVT.todo_x24;
      RotTrans(&gCameraPos, (VECTOR *)gCameraMatrix.t, &flag);
      SetTransMatrix(&gCameraMatrix);

      for (i = startX; i < endX; i++) {
         for (j = 0; j < 8; j++) {
            tileModel = &gMapRowPointers[j][i];
            RenderMapTile(gGraphicsPtr->ot, tileModel, GRID_COLOR_NONE);
         }
      }

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;

      for (i = 0; i < UNIT_CT; i++) {
         if (gUnits[i].idx != 0) {
            unit = &gUnits[i];
            unitSprite = unit->sprite;
            if (unitSprite->x1.n < EVT.endX) {
               unitSprite->d.sprite.hidden = 1;
               CopyEvtData(unitSprite, evt_s0);
               evt_s0->d.sprite.hidden = 0;
               RenderUnitSprite(gGraphicsPtr->ot, evt_s0, 0);
            }
         }
      }

      evt_s0->functionIndex = EVTF_NULL;
      PopMatrix();
      gCameraPos.vx = savedCamPosX;
      EVT.todo_x26 += evt->x2.n;
      evt->x2.n += evt->x3.n;
      EVT.todo_x24 = EVT.todo_x26 >> 3;
      if (EVT.todo_x24 > -0x400) {
         break;
      }

      evt->state++;

   // fallthrough
   case 4:
      for (i = startX; i < endX; i++) {
         for (j = 0; j < 8; j++) {
            tileModel2 = &gMapRowPointers[j][i];
            gMapRowPointers[j][i] = gMapRowPointers[j][i % 2];
            ShiftTile(tileModel2, i / 2 * 64, 0);
            if (gMapUnitsPtr[j][i].s.unitIdx != 0) {
               gTileStateGridPtr[j][i].action = TA_X16;
            }
            gTerrainPtr[j][i].s.terrain = TERRAIN_NO_ENTRY;
            gTerrainPtr[j][i].s.elevation = -gMapRowPointers[j][i].vertices[0].vy >> 4;
         }
      }
      gState.fieldRenderingDisabled = 0;
      evt->state++;

   // fallthrough
   case 5:
      EaseOutCamera(&EVT.camera, 2);
      if (++evt->state3 >= 32) {
         AssignToMainCamera(&EVT.camera);
         EVT.todo_x24 = 0;
         EVT.carIdx++;
         EVT.startX = EVT.endX;
         evt->state = 1;
         evt->state3 = 0;
         gPlayerControlSuppressed = 0;
         gState.field_0x96 = 0;
         if (EVT.carIdx > 2) {
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_4_1);
         }
      }
      break;
   }
}
