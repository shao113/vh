#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"
#include "units.h"
#include "battle.h"

void Map35_RemoveBridgeTile(s32, s32);
void Map35_RemoveBridge(void);
void Evtf652_Map35_Button(EvtData *);
void Evtf702_FlamingRock(EvtData *);

static MapTileModel sMapTileModel_80100e9c = {
#include "assets/80100e9c.inc"
};

void Map35_RemoveBridgeTile(s32 z, s32 x) {
   MapTileModel *tileModel;

   tileModel = &gMapRowPointers[z][x];
   AdjustFaceElevation(tileModel, 0, 64);
   AdjustFaceElevation(tileModel, 1, 48);
   tileModel->gfx[0] = GFX_MAP_TEXTURE_5;
   gTerrainPtr[z][x].s.elevation = -(tileModel->vertices[0].vy >> 4);
   gTerrainPtr[z][x].s.terrain = TERRAIN_WATER;
   UpdateTileElevation(z, x);
}

void Map35_RemoveBridge(void) {
   s32 iz, ix;

   for (ix = 9; ix <= 13; ix++) {
      for (iz = 7; iz <= 8; iz++) {
         Map35_RemoveBridgeTile(iz, ix);
      }
   }
}

#undef EVTF
#define EVTF 652
void Evtf652_Map35_Button(EvtData *evt) {
   extern MapTileModel s_tileModel_801246a4;

   EvtData *evt_s0;
   s32 i, j, k;
   s32 avail;
   s32 alt;
   MapTileModel *pTileModel;
   s8 unused[16];
   s32 casualties;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 != 0) {
         DepressButton(18, 8);
         Map35_RemoveBridge();
         evt->functionIndex = EVTF_NULL;
         return;
      }
      evt->state++;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 != 0) {
         gPlayerControlSuppressed = 1;
         evt->state++;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         gCameraRotation.vy &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_BUTTON_DEPRESS;
         evt_s0->z1.s.hi = 8;
         evt_s0->x1.s.hi = 18;
         EVT.buttonDepress = evt_s0;
         casualties = 0;
         EVT.hitPlayerUnit = 0;
         for (j = 9; j <= 13; j++) {
            for (i = 7; i <= 8; i++) {
               if (gMapUnitsPtr[i][j].s.team == TEAM_PLAYER) {
                  casualties++;
               }
            }
         }
         if (casualties != 0) {
            EVT.hitPlayerUnit = 1;
         }
         evt->state2++;
         break;

      case 1:
         evt_s0 = EVT.buttonDepress;
         if (evt_s0->state == 99) {
            evt->state2++;
         }
         break;

      case 2:
         evt->state++;
         evt->state2 = 0;
         if (EVT.hitPlayerUnit) {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(19));
         } else {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(37));
         }
         break;
      }

      break;

   case 3:

      switch (evt->state2) {
      case 0:
         evt->x1.n = CV(11.0);
         evt->z1.n = CV(8.0);
         evt->y1.n = CV(3.5);
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
         evt_s0->d.evtf026.target = evt;
         evt_s0->d.evtf026.zoom = 512;
         evt->state3 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (++evt->state3 >= 32) {
            evt->state2 = 0;
            evt->state3 = 0;
            evt->state++;
         }
         break;

      case 2:
         break;
      }

      break;

   case 4:
      if (!EVT.hitPlayerUnit) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(37));
      }

      s_tileModel_801246a4 = sMapTileModel_80100e9c;
      pTileModel = &gMapRowPointers[15][32]; //?
      for (j = 0; j < 10; j++) {
         pTileModel[j] = sMapTileModel_80100e9c;
      }

      alt = 0;
      if (!EVT.hitPlayerUnit) {
         for (j = 9; j <= 13; j++) {
            for (i = 7; i <= 8; i++) {
               if (gMapUnitsPtr[i][j].raw != 0) {
                  gTileStateGridPtr[i][j].action = TA_X16;
               }
               if (alt != 0) {
                  evt_s0 = Evt_GetUnused();
                  evt_s0->functionIndex = EVTF_EXPLODING_TILE;
                  evt_s0->d.evtf653.translation.vx = j * CV(1.0);
                  evt_s0->d.evtf653.translation.vy = CV(2.5);
                  evt_s0->d.evtf653.translation.vz = i * CV(1.0);
                  evt_s0->d.evtf653.tileModel = &sMapTileModel_80100e9c;
                  evt_s0->d.evtf653.riverVelocity = CV(0.0625);
                  evt_s0->state3 = (rand() >> 2) % 48;
                  alt = 0;
               } else {
                  alt++;
               }
            }
         }
      }

      evt->state++;
      break;

   case 5:
      if (EVT.hitPlayerUnit) {
         evt->state++;
         EVT.todo_x5a = 0;
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(115));
      } else {
         gCameraRotation.vy -= DEG(1.40625);
         if (++evt->state3 >= 64) {
            Map35_RemoveBridge();
            evt->state3 = 0;
            gCameraRotation.vy &= 0xfff;
            evt->state2 = 0;
            if (EVT.hitPlayerUnit) {
               evt->state++;
               EVT.todo_x5a = 0;
            } else {
               evt->state += 2;
            }
         }
      }

      break;

   case 6:
      EVT.todo_x5a++;
      if (--EVT.todo_x54 <= 0 && evt->state2 < 7) {
         for (j = 9; j <= 13; j++) {
            for (i = 7; i <= 8; i++) {
               evt_s0 = Evt_GetUnused();
               evt_s0->functionIndex = EVTF_SMOKE;
               evt_s0->x1.n = j * CV(1.0);
               evt_s0->z1.n = i * CV(1.0) + CV(0.5);
               evt_s0->y1.n = GetTerrainElevation(evt_s0->z1.s.hi, evt_s0->x1.s.hi);
               evt_s0->d.sprite.semiTrans = 2;
               evt_s0->x2.n = -rand() % CV(0.25);
               evt_s0->z2.n = rand() % CV(0.25) - CV(0.125);
               evt_s0->y2.n = rand() % CV(0.25);
            }
         }
         EVT.todo_x54 = 16 - (EVT.todo_x5a >> 4);
         if (EVT.todo_x54 < 4) {
            EVT.todo_x54 = 4;
         }
      }

      switch (evt->state2) {
      case 0:
         EVT.tileX = 9;
         evt->state2++;

      // fallthrough
      case 1:
         EVT.tileZ = 7;
         evt->state2++;

      // fallthrough
      case 2:
         j = EVT.tileX;
         i = EVT.tileZ;
         if (gMapUnitsPtr[i][j].raw != 0) {
            gState.msgFinished = 0;
            gTileStateGridPtr[i][j].action = TA_X20;
            evt->state2++;
         } else {
            evt->state2 += 2;
         }
         break;

      case 3:
         if (gState.msgFinished) {
            evt->state2++;
         }
         break;

      case 4:
         EVT.tileZ++;
         if (EVT.tileZ >= 9) {
            evt->state2++;
         } else {
            evt->state2 = 2;
         }
         break;

      case 5:
         EVT.tileX++;
         if (EVT.tileX >= 14) {
            evt->state2++;
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(37));
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_8_8);
            evt->state3 = 0;
         } else {
            evt->state2 = 1;
         }
         break;

      case 6:
         if (++evt->state3 >= 64) {
            k = 2;
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(37));
            for (j = 9; j <= 13; j++) {
               for (i = 7; i <= 8; i++) {
                  if (gMapUnitsPtr[i][j].raw != 0) {
                     gTileStateGridPtr[i][j].action = TA_X16;
                  }
                  avail = Evt_CountUnused();
                  if (--k <= 0 && avail >= 50) {
                     evt_s0 = Evt_GetUnused();
                     evt_s0->functionIndex = EVTF_EXPLODING_TILE;
                     evt_s0->d.evtf653.translation.vx = j * CV(1.0);
                     evt_s0->d.evtf653.translation.vy = CV(2.5);
                     evt_s0->d.evtf653.translation.vz = i * CV(1.0);
                     evt_s0->d.evtf653.tileModel = &sMapTileModel_80100e9c;
                     evt_s0->d.evtf653.riverVelocity = CV(0.0625);
                     evt_s0->state3 = (rand() >> 2) % 48;
                     k = 1;
                  }
               }
            }
            Map35_RemoveBridge();
            evt->state3 = 0;
            evt->state2++;
            PerformAudioCommand(AUDIO_CMD_FADE_IN_8_8);
         }
         break;

      case 7:
         gCameraZoom.vz += 10;
         gCameraRotation.vx += 1;
         if (++evt->state3 >= 96) {
            evt->state++;
            evt->state3 = 0;
         }
         break;
      }

      break;

   case 7:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         AssignToMainCamera(&EVT.camera);
         evt->state++;
      }
      break;

   case 8:
      gPlayerControlSuppressed = 0;
      gState.signal = 0;
      evt->state++;
      break;

   case 9:
      break;
   }
}

#undef EVTF
#define EVTF 702
void Evtf702_FlamingRock(EvtData *evt) {
   extern s16 gExplosionAnimData_800ffa80[26];

   static s16 rockAnimData[12] = {4, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3,
                                  2, GFX_ROCK_4, 2, GFX_NULL,   1, GFX_NULL};

   s16 x, y, z;
   EvtData *particle;
   s32 i, skip;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         evt->state3 = rand() & 7;
         evt->state2++;
      // fallthrough
      case 1:
         if (--evt->state3 <= 0) {
            evt->state3 = 0;
            evt->state2 = 0;
            evt->state++;
            evt->d.sprite.animData = rockAnimData;
         }
         break;
      }

      break;

   case 1:
      x = evt->x1.n;
      y = evt->y1.n;
      z = evt->z1.n;

      evt->y1.n += evt->y2.n;
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y2.n += evt->y3.n;

      particle = CreatePositionedEvt(evt, EVTF_PARTICLE_710);
      particle->x1.n = evt->x1.n;
      particle->z1.n = evt->z1.n;
      particle->y1.n = evt->y1.n;
      particle->state2 = rand() % 8 + 10;
      particle->d.sprite.animData = gExplosionAnimData_800ffa80;
      particle->mem = 16;
      skip = rand() & 7;
      for (i = 0; i < skip; i++) {
         UpdateEvtAnimation(particle);
      }

      particle->x2.n = -((x - evt->x1.n) >> 2) + ((rand() >> 2) % 10 - 5);
      particle->z2.n = -((z - evt->z1.n) >> 2) + ((rand() >> 2) % 10 - 5);
      particle->y2.n = -((y - evt->y1.n) >> 2);

      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 240, 128, 128);

      if (evt->y1.n < GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi)) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}
