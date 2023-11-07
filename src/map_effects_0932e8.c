#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"
#include "units.h"
#include "battle.h"

void Evtf657_Map38_Floodgate(EvtData *);
void Map38_RaiseFloodgate(void);
void Evtf655_Map38_RaiseFloodgate(EvtData *);
void Map38_SetWater(void);
void Evtf658_Map38_Floodwater(EvtData *);
void Evtf659_Splash(EvtData *);
void Evtf654_Map38_WashAwayUnit(EvtData *);

#undef EVTF
#define EVTF 657
void Evtf657_Map38_Floodgate(EvtData *evt) {
   EvtData *evt_a1;
   s32 iz, ix;
   s32 casualties;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 != 0) {
         DepressButton(21, 4);
         Map38_SetWater();
         Map38_RaiseFloodgate();
         evt->functionIndex = EVTF_NULL;
         return;
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 != 0) {
         gCameraRotation.vy &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         evt->state++;
         evt->state3 = 0;
         gPlayerControlSuppressed = 1;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         if (++evt->state3 >= 0) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 1:
         evt_a1 = Evt_GetUnused();
         evt_a1->functionIndex = EVTF_BUTTON_DEPRESS;
         evt_a1->x1.n = CV(21.0);
         evt_a1->z1.n = CV(4.0);
         evt_a1->y1.n = CV(2.0);
         EVT.child = evt_a1;
         evt->state2++;

      // fallthrough
      case 2:
         evt_a1 = EVT.child;
         if (evt_a1->functionIndex != EVTF_BUTTON_DEPRESS) {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(79));
            evt->state2++;
         }
         break;

      case 3:
         PanCamera(CV(26.0), CV(2.0), CV(7.0), 3);
         if (++evt->state3 >= 32) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 4:
         evt_a1 = Evt_GetUnused();
         evt_a1->functionIndex = EVTF_MAP38_FLOODWATER;
         EVT.child = evt_a1;
         evt_a1 = Evt_GetUnused();
         evt_a1->functionIndex = EVTF_MAP38_RAISE_FLOODGATE;
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(79));
         EVT.casualty = NULL;
         evt->state2++;
         break;

      case 5:
         gCameraZoom.vz += 1;
         if (evt->state3 == 30) {
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_2_1);
         }
         if (++evt->state3 >= 256) {
            evt->state2++;
         }
         break;

      case 6:
         evt->x1.s.hi = 25;
         evt->z1.s.hi = 5;
         evt->x3.s.hi = 27;
         evt->z3.s.hi = 15;
         evt->state2++;

      // fallthrough
      case 7:
         if (OBJ_MAP_UNIT(evt).raw != 0 &&
             gUnits[OBJ_MAP_UNIT(evt).s.unitIdx].class != CLASS_AIRMAN) {
            gState.msgFinished = 0;
            OBJ_TILE_STATE(evt).action = TA_X20;
            evt->state2++;
         } else {
            evt->state2 += 2;
         }
         break;

      case 8:
         if (gState.msgFinished) {
            evt->state2++;
         }
         break;

      case 9:
         evt->x1.s.hi += 1;
         if (evt->x1.s.hi > 27) {
            evt->state2++;
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(103));
            evt->state3 = 0;
         } else {
            evt->state2 -= 2;
         }
         break;

      case 10:
         if (++evt->state3 >= 8) {
            evt->x1.s.hi = 25;
            evt->z1.s.hi += 1;
            if (evt->z1.s.hi >= 16) {
               casualties = 0;
               for (ix = 25; ix <= 27; ix++) {
                  for (iz = 5; iz <= 15; iz++) {
                     if (gMapUnitsPtr[iz][ix].raw != 0 &&
                         gUnits[gMapUnitsPtr[iz][ix].s.unitIdx].class != CLASS_AIRMAN) {
                        evt_a1 = Evt_GetUnused();
                        evt_a1->functionIndex = EVTF_MAP38_WASH_AWAY_UNIT;
                        evt_a1->x1.n = ix * CV(1.0) + CV(0.3125);
                        evt_a1->z1.n = iz * CV(1.0) + CV(0.3125);
                        if (EVT.casualty == NULL) {
                           EVT.casualty = evt_a1;
                        }
                        casualties++;
                     }
                  }
               }
               if (casualties != 0) {
                  PerformAudioCommand(AUDIO_CMD_PLAY_XA(103));
               }
               evt->state3 = 0;
               evt->state2++;
            } else {
               evt->state2 -= 3;
            }
         }
         break;

      case 11:
         evt_a1 = EVT.casualty;
         if (evt_a1 != NULL && evt_a1->functionIndex == EVTF_MAP38_WASH_AWAY_UNIT) {
            PanCamera(evt_a1->x1.n, evt_a1->y1.n, evt_a1->z1.n + evt->state3 * 8, 3);
         } else {
            PanCamera(CV(26.0), CV(2.0), CV(5.0), 3);
         }
         gCameraZoom.vz += (400 - gCameraZoom.vz) >> 3;
         if (++evt->state3 >= 128) {
            evt->state2++;
         }
         break;

      case 12:
         evt_a1 = EVT.child;
         evt_a1->state2++;
         evt->state2++;
         break;

      case 13:
         evt_a1 = EVT.child;
         if (evt_a1->functionIndex != EVTF_MAP38_FLOODWATER) {
            evt->state3 = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 3:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         evt->state++;
      }
      break;

   case 4:
      AssignToMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 0;
      gState.signal = 0;
      evt->state++;
      break;

   case 5:
      break;
   }
}

void Map38_RaiseFloodgate(void) {
   s32 i;
   MapTileModel *tileModel;

   for (i = 0; i < 3; i++) {
      tileModel = &gMapRowPointers[4][25 + i];
      AdjustFaceElevation(tileModel, 1, -48);
      AdjustFaceElevation(tileModel, 2, -48);
      AdjustFaceElevation(tileModel, 3, -16);
      UpdateTileElevation(4, 25 + i);
   }
}

#undef EVTF
#define EVTF 655
void Evtf655_Map38_RaiseFloodgate(EvtData *evt) {
   s32 i;
   MapTileModel *tileModel;

   switch (evt->state) {
   case 0:
      tileModel = &gMapRowPointers[4][25];
      evt->y1.n = -tileModel->vertices[tileModel->faces[1][0]].vy << 4; // top
      evt->y2.n = -tileModel->vertices[tileModel->faces[2][0]].vy << 4; // bottom
      EVT.waterFaceY = -tileModel->vertices[tileModel->faces[3][0]].vy << 4;
      evt->mem = 0;
      evt->y3.n = 2;
      evt->state++;

   // fallthrough
   case 1:
      for (i = 0; i < 3; i++) {
         tileModel = &gMapRowPointers[4][25 + i];
         SetFaceElevation(tileModel, 1, -(evt->y1.n + evt->mem) >> 4);
         SetFaceElevation(tileModel, 2, -(evt->y2.n + evt->mem) >> 4);
         if (evt->mem <= 256) {
            // Continue raising attached water face until level
            SetFaceElevation(tileModel, 3, -(EVT.waterFaceY + evt->mem) >> 4);
         }
      }
      evt->mem += evt->y3.n;
      if (evt->mem >= 768) {
         for (i = 0; i < 3; i++) {
            UpdateTileElevation(4, 25 + i);
         }
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void Map38_SetWater(void) {
   s32 iz, ix;
   MapTileModel *tileModel;

   for (ix = 25; ix <= 27; ix++) {
      for (iz = 5; iz <= 15; iz++) {
         tileModel = &gMapRowPointers[iz][ix];
         AdjustFaceElevation(tileModel, 0, -16);
         tileModel->gfx[0] = GFX_MAP_TEXTURE_52;
         gTerrainPtr[iz][ix].s.terrain = TERRAIN_WATER;
         gTerrainPtr[iz][ix].s.elevation = -(tileModel->vertices[0].vy >> 4);
      }
   }
}

#undef EVTF
#define EVTF 658
void Evtf658_Map38_Floodwater(EvtData *evt) {
   // FIXME: Fake match (do-while)

   Quad quad = {
       {-128, 0, -128, 0},
       {128, 0, -128, 0},
       {-128, 0, 128, 0},
       {128, 0, 128, 0},
   };

   s32 i, j, k;
   s32 iVar4;
   u8 fade;
   s16 r;
   s16 elevation;
   POLY_FT4 *poly;
   EvtData *evt_s0;
   EvtData *evt_s2;
   SVectorXZY *pCoord;
   s32 iVar7;
   s16 theta;
   s8 unused[32];

   switch (evt->state) {
   case 0:
      evt->state++;

   // fallthrough
   case 1:
      evt->state++;
      evt->state2 = 0;
      evt->state3 = 16;
      evt->mem = 4;
      evt->z1.n = 0;
      evt->z2.n = CV(0.25);
      EVT.theta = 0;
      EVT.timer = 0;
      EVT.todo_x30 = 0;
      break;

   case 2:
      evt->state++;

   // fallthrough
   case 3:
      if (evt->state2 < 2 && --EVT.timer <= 0) {
         for (iVar4 = 0; iVar4 < 8; iVar4++) {
            evt_s0 = Evt_GetUnused();
            evt_s0->functionIndex = EVTF_SPLASH;
            evt_s0->x1.n = CV(25.125) + iVar4 * CV(0.375);
            evt_s0->z1.n = CV(4.5);
            evt_s0->y1.n = GetTerrainElevation(evt_s0->z1.s.hi, evt_s0->x1.s.hi) + CV(0.5);
            evt_s0->z2.n = CV(0.125);
            evt_s0->d.sprite.semiTrans = 2;
         }
         EVT.timer = rand() % 3 + 6;
      }
      evt->mem = (evt->state3 >> 5) + 1;
      if (evt->mem > 4) {
         evt->mem = 4;
      }
      evt->z1.n += evt->z2.n;
      if (evt->z1.n >= CV(11.0)) {
         evt->z1.n = CV(11.0);
      }

      switch (evt->state2) {
      case 0:
         EVT.theta += DEG(22.5);
         EVT.todo_x30 += 2;
         r = EVT.todo_x30 >> 3;
         if (r >= CV(0.1875)) {
            EVT.todo_x30 = 0;
            r = CV(0.1875);
            evt->state2++;
         }
         break;

      case 1:
         EVT.theta += DEG(22.5);
         r = CV(0.1875);
         break;

      case 2:
         EVT.theta += DEG(22.5);
         EVT.todo_x30 -= 2;
         r = EVT.todo_x30 >> 3;
         if (r <= 0) {
            EVT.todo_x30 = 0;
            r = 0;
            evt->state2++;
         }
         break;

      default:
         EVT.todo_x30 += 1;
         r = 0;
         if (EVT.todo_x30 >= 32) {
            evt->state++;
         }
         break;
      }

      if (evt->state3 <= 128) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_NOOP;
         evt_s2->d.sprite.gfxIdx = GFX_MAP_TEXTURE_52;
         elevation = GetTerrainElevation(5, 26);
         theta = EVT.theta;
         quad[3].vy = quad[2].vy = abs(r * rsin(theta) >> 12);

         for (i = 5; i < evt->z1.s.hi + 5; i++) {
            if (i == 5) {
               quad[0].vy = 0;
               quad[1].vy = 0;
            } else {
               quad[0].vy = quad[2].vy;
               quad[1].vy = quad[3].vy;
               theta += DEG(33.75);
               iVar7 = r * rsin(theta) >> 12;
               quad[2].vy = iVar7;
               quad[3].vy = iVar7;
            }
            for (j = 25; j <= 27; j++) {
               pCoord = &evt_s2->d.sprite.coords[0];
               for (iVar4 = 0; iVar4 < 4; iVar4++) {
                  pCoord->x = j * CV(1.0) + quad[iVar4].vx + CV(0.5);
                  pCoord->z = i * CV(1.0) + quad[iVar4].vz + CV(0.5);
                  pCoord->y = elevation + evt->state3 + quad[iVar4].vy;
                  pCoord++;
               }
               pCoord = &evt_s2->d.sprite.coords[0];
               evt_s2->x1.n = (pCoord[0].x + pCoord[1].x + pCoord[2].x + pCoord[3].x) / 4;
               evt_s2->z1.n = (pCoord[0].z + pCoord[1].z + pCoord[2].z + pCoord[3].z) / 4;
               evt_s2->y1.n = (pCoord[0].y + pCoord[1].y + pCoord[2].y + pCoord[3].y) / 4;
               evt_s2->d.sprite.semiTrans = 1;
               evt_s2->d.sprite.otOfs = (4 - evt->mem) * 3 - 5;

               for (k = 0; k < evt->mem; k++) {
                  evt_s2->d.sprite.gfxIdx = GFX_MAP_TEXTURE_52;
                  AddEvtPrim5(gGraphicsPtr->ot, evt_s2);
                  poly = &gGraphicsPtr->quads[gQuadIndex - 1];
                  fade = 256 - evt->state3;
                  setRGB0(poly, fade, fade, fade);
               }
               for (k = evt->mem; k < 2; k++) {
                  evt_s2->d.sprite.semiTrans = 2;
                  evt_s2->d.sprite.gfxIdx = GFX_MAP_TEXTURE_52;
                  AddEvtPrim5(gGraphicsPtr->ot, evt_s2);
                  poly = &gGraphicsPtr->quads[gQuadIndex - 1];
                  fade = evt->state3;
                  setRGB0(poly, fade, fade, fade);
               }
            }
         }

         i = evt->z1.s.hi + 5;
         for (j = 25; j <= 27; j++) {
            evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y =
                evt_s2->d.sprite.coords[2].y = evt_s2->d.sprite.coords[3].y =
                    evt->state3 + elevation;

            evt_s2->d.sprite.coords[0].x = j * CV(1.0) + CV(1.0);
            evt_s2->d.sprite.coords[1].x = j * CV(1.0);
            evt_s2->d.sprite.coords[2].x = j * CV(1.0) + CV(1.0);
            evt_s2->d.sprite.coords[3].x = j * CV(1.0);

            evt_s2->d.sprite.coords[0].z = i * CV(1.0);
            evt_s2->d.sprite.coords[1].z = i * CV(1.0);
            evt_s2->d.sprite.coords[2].z = i * CV(1.0) + (u8)evt->z1.s.lo;
            evt_s2->d.sprite.coords[3].z = i * CV(1.0) + (u8)evt->z1.s.lo;
            evt_s2->d.sprite.semiTrans = 1;

            do {
               iVar7 = evt->mem * 3;
               iVar7 -= 5;
               evt_s2->d.sprite.otOfs = iVar7;
            } while (0);

            for (k = 0; k < evt->mem; k++) {
               evt_s2->d.sprite.gfxIdx = GFX_MAP_TEXTURE_52;
               AddEvtPrim4(gGraphicsPtr->ot, evt_s2);
               poly = &gGraphicsPtr->quads[gQuadIndex - 1];
               fade = 256 - evt->state3;
               setRGB0(poly, fade, fade, fade);
            }
            for (k = evt->mem; k < 2; k++) {
               evt_s2->d.sprite.semiTrans = 2;
               evt_s2->d.sprite.gfxIdx = GFX_MAP_TEXTURE_52;
               AddEvtPrim5(gGraphicsPtr->ot, evt_s2);
               poly = &gGraphicsPtr->quads[gQuadIndex - 1];
               fade = evt->state3;
               setRGB0(poly, fade, fade, fade);
            }
         }
         evt_s2->functionIndex = EVTF_NULL;
      }
      if (++evt->state3 >= 128) {
         evt->state3 = 128;
      }
      if (evt->state == 4) {
         Map38_SetWater();
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
#define EVTF 659
void Evtf659_Splash(EvtData *evt) {
   static s16 splashAnimData[20] = {
       7, GFX_SPLASH_1, 2, GFX_SPLASH_2, 2, GFX_SPLASH_3, 2, GFX_SPLASH_4, 2, GFX_SPLASH_5,
       2, GFX_SPLASH_6, 2, GFX_SPLASH_7, 2, GFX_SPLASH_8, 2, GFX_NULL,     0, GFX_NULL};
   static Quad quad = {{-16, -16, 0, 0}, {16, -16, 0, 0}, {-16, 16, 0, 0}, {16, 16, 0, 0}};

   s32 i;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = splashAnimData;
      evt->d.sprite.semiTrans = 4;
      evt->state++;
      // TODO: EvtData?
      if (evt->d.sprite.coords[0].x == 0) {
         evt->d.sprite.coords[0].x = rand() % 64;
      }
      evt->d.sprite.coords[0].z = 128;
      evt->mem = 0;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i] = quad[i];
      }
      gQuad_800fe63c[0].vy -= (evt->d.sprite.coords[0].x * rsin(evt->mem) >> 12);
      gQuad_800fe63c[1].vy -= (evt->d.sprite.coords[0].x * rsin(evt->mem) >> 12);
      evt->mem += evt->d.sprite.coords[0].z;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 654
void Evtf654_Map38_WashAwayUnit(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *clonedSprite;

   switch (evt->state) {
   case 0:
      EVT.tileX = evt->x1.s.hi;
      EVT.tileZ = evt->z1.s.hi;
      evt->state++;

   // fallthrough
   case 1:
      evt->state++;
      break;

   case 2:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;
      EVT.unitSprite = unitSprite;
      unitSprite->d.sprite.hidden = 1;

      clonedSprite = Evt_GetUnused();
      CopyEvtData(unitSprite, clonedSprite);
      clonedSprite->functionIndex = EVTF_NOOP;
      clonedSprite->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, clonedSprite, 0);
      clonedSprite->functionIndex = EVTF_NULL;

      evt->state3 = 0;
      evt->state++;
      break;

   case 3:
      unitSprite = EVT.unitSprite;

      switch (evt->state2) {
      case 0:
         evt->x2.n = 0;
         evt->z2.n = CV(0.03125);
         evt->z3.n = CV(0.00390625);
         if (++evt->state3 >= 32) {
            evt->state2++;
         }
         break;

      case 1:
         evt->x2.n = 0;
         evt->z3.n = CV(0.03125);
         if (evt->z1.s.hi >= gMapMaxZ) {
            gTileStateGridPtr[EVT.tileZ][EVT.tileX].action = TA_X16;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->z2.n += evt->z3.n;

      clonedSprite = Evt_GetUnused();
      CopyEvtData(unitSprite, clonedSprite);
      clonedSprite->d.sprite.hidden = 0;
      clonedSprite->x1.n = evt->x1.n;
      clonedSprite->z1.n = evt->z1.n;
      RenderUnitSprite(gGraphicsPtr->ot, clonedSprite, 0);
      clonedSprite->functionIndex = EVTF_NULL;

      if (--evt->mem <= 0) {
         CreatePositionedEvt(evt, EVTF_SPLASH);
         evt->mem = rand() % 5;
      }
      break;
   }
}
