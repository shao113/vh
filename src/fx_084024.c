#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "units.h"

#undef EVTF
#define EVTF 087
void Evtf087_Map20_Scn30_ArrowSpawner(EvtData *evt) {
   EvtData *evt_v1;

   switch (evt->state) {
   case 0:
      evt_v1 = EVT.variant_0x24.entitySpriteParam;
      evt->x1.n = evt_v1->x1.n;
      evt->z1.n = evt_v1->z1.n;
      evt->y1.n = CV(1.0);
      EVT.entitySprite = evt_v1;
      EVT.variant_0x24.velocity.z = CV(0.46875);
      EVT.variant_0x24.velocity.y = CV(0.28125);
      EVT.yAccel = CV(-0.046875);
      evt->y1.n += CV(0.5);
      evt->state++;

   // fallthrough
   case 1:
      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_MAP20_SCN30_ARROW;
      evt_v1->x1.n = evt->x1.n;
      evt_v1->z1.n = evt->z1.n;
      evt_v1->y1.n = evt->y1.n;
      evt_v1->z2.n = EVT.variant_0x24.velocity.z;
      evt_v1->y2.n = EVT.variant_0x24.velocity.y;
      evt_v1->y3.n = EVT.yAccel;
      evt_v1->mem = 0x80;
      evt->state++;
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 098
void Evtf098_Map20_Scn30_Arrow(EvtData *evt) {
   static const Quad baseQuad = {{0, CV(-0.125), CV(-0.5), 0},
                                 {0, CV(-0.125), CV(+0.5), 0},
                                 {0, CV(+0.125), CV(-0.5), 0},
                                 {0, CV(+0.125), CV(+0.5), 0}};

   Quad rotatedQuad;
   s32 i;
   s16 dy, dz;
   s16 theta;
   SVectorXZY *pPosition;

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_ARROW;

      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;

      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;

      if (EVT.animFinished) {
         EVT.hidden = 1;
      }

      dz = evt->z1.n - EVT.pos.z;
      dy = evt->y1.n - EVT.pos.y;
      theta = -ratan2(dy, dz);
      RotateQuadYZ(baseQuad, rotatedQuad, theta);

      for (i = 0; i < 4; i++) {
         rotatedQuad[i].vx = 0;
      }

      pPosition = &EVT.coords[0];
      for (i = 0; i < 4; i++) {
         pPosition->x = evt->x1.n + rotatedQuad[i].vx;
         pPosition->z = evt->z1.n + rotatedQuad[i].vz;
         pPosition->y = evt->y1.n + rotatedQuad[i].vy;
         pPosition++;
      }

      AddEvtPrim3(gGraphicsPtr->ot, evt);
      EVT.pos.x = evt->x1.n;
      EVT.pos.z = evt->z1.n;
      EVT.pos.y = evt->y1.n;

      if (evt->y2.n <= 0 &&
          evt->y1.n <= GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + CV(0.5)) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

static s16 sSmokeAnimData_800ffa08[24] = {
    7, GFX_PUFF_1, 2, GFX_PUFF_2, 2, GFX_PUFF_3, 2, GFX_PUFF_4,  2, GFX_PUFF_5, 2, GFX_PUFF_6,
    2, GFX_PUFF_7, 2, GFX_PUFF_8, 2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   0, GFX_NULL};

static s16 sPoisonAnimData_800ffa38[36] = {
    7, GFX_POISON_1, 2, GFX_POISON_2, 2, GFX_POISON_3, 2, GFX_POISON_4, 2, GFX_POISON_5,
    2, GFX_POISON_6, 2, GFX_POISON_7, 2, GFX_POISON_8, 2, GFX_POISON_1, 2, GFX_POISON_2,
    2, GFX_POISON_3, 2, GFX_POISON_4, 2, GFX_POISON_5, 2, GFX_POISON_6, 2, GFX_POISON_7,
    2, GFX_POISON_8, 2, GFX_NULL,     0, GFX_NULL};

static s16 sExplosionAnimData_800ffa80[26] = {
    7, GFX_EXPLOSION_1, 2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,  2, GFX_EXPLOSION_4,
    2, GFX_EXPLOSION_5, 2, GFX_EXPLOSION_6,  2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,
    2, GFX_EXPLOSION_9, 2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
    0, GFX_NULL};

s16 gSparkleAnimData_800ffab4[36] = {
    7, GFX_SPARKLE_1, 2, GFX_SPARKLE_2, 2, GFX_SPARKLE_3, 2, GFX_SPARKLE_4, 2, GFX_SPARKLE_5,
    2, GFX_SPARKLE_4, 2, GFX_SPARKLE_3, 2, GFX_SPARKLE_2, 2, GFX_SPARKLE_1, 2, GFX_SPARKLE_2,
    2, GFX_SPARKLE_3, 2, GFX_SPARKLE_4, 2, GFX_SPARKLE_5, 2, GFX_SPARKLE_4, 2, GFX_SPARKLE_3,
    2, GFX_SPARKLE_2, 2, GFX_NULL,      0, GFX_NULL};

#undef EVTF
#define EVTF 764
void Evtf764_to_769_ProjectileTrail(EvtData *evt) {
   EvtData *particle;
   s32 i;
   s32 skip;

   particle = CreatePositionedEvt(evt, EVTF_PARTICLE_710);

   switch (evt->functionIndex) {
   case EVTF_PROJECTILE_TRAIL_POISON:
      particle->d.sprite.animData = sPoisonAnimData_800ffa38;
      particle->state2 = rand() % 4 + 2;
      particle->y2.n = -4;
      break;
   case EVTF_PROJECTILE_TRAIL_EXPLOSION:
      particle->state2 = rand() % 6 + 6;
      particle->d.sprite.animData = sExplosionAnimData_800ffa80;
      break;
   case EVTF_PROJECTILE_TRAIL_SMOKE:
      particle->state2 = rand() % 4 + 6;
      particle->d.sprite.animData = sSmokeAnimData_800ffa08;
      particle->d.sprite.semiTrans = 2;
      break;
   default: // EVTF_PROJECTILE_TRAIL_SPARKLES
      particle->state2 = rand() % 5;
      particle->d.sprite.animData = gSparkleAnimData_800ffab4;
      particle->d.sprite.semiTrans = 2;
      break;
   }

   particle->mem = 32;

   skip = rand() & 7;
   for (i = 0; i < skip; i++) {
      UpdateEvtAnimation(particle);
   }

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 790
void Evtf790_Fx_TBD(EvtData *evt) {
   static s16 explosionAnimData[26] = {3, GFX_EXPLOSION_1,  1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,
                                       1, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

   EvtData *evt_s2;
   s32 theta;
   s32 radius;
   s32 i;
   s32 skip;

   if (--evt->mem <= 0) {
      evt_s2 = CreatePositionedEvt(evt, EVTF_PARTICLE_739);
      radius = rand() & 3;
      theta = rand() & 0xfff;
      evt_s2->x2.n = radius * rcos(theta) >> 12;
      evt_s2->z2.n = radius * rsin(theta) >> 12;
      evt_s2->y2.n = CV(0.24609375);
      evt_s2->d.sprite.animData = explosionAnimData;
      evt_s2->mem = 48;

      skip = rand() & 3;
      for (i = 0; i < skip; i++) {
         UpdateEvtAnimation(evt_s2);
      }

      evt->mem = rand() & 7;
   }
}

static Quad sQuad_Size1 = {{-1, -1, 0, 0}, {1, -1, 0, 0}, {-1, 1, 0, 0}, {1, 1, 0, 0}};
static Quad sQuad_Size2 = {{-2, -2, 0, 0}, {2, -2, 0, 0}, {-2, 2, 0, 0}, {2, 2, 0, 0}};
static Quad sQuad_Size3 = {{-3, -3, 0, 0}, {3, -3, 0, 0}, {-3, 3, 0, 0}, {3, 3, 0, 0}};
static Quad sQuad_Size4 = {{-4, -4, 0, 0}, {4, -4, 0, 0}, {-4, 4, 0, 0}, {4, 4, 0, 0}};
static Quad sQuad_Size5 = {{-5, -5, 0, 0}, {5, -5, 0, 0}, {-5, 5, 0, 0}, {5, 5, 0, 0}};
static Quad sQuad_Size6 = {{-6, -6, 0, 0}, {6, -6, 0, 0}, {-6, 6, 0, 0}, {6, 6, 0, 0}};
static Quad sQuad_Size7 = {{-7, -7, 0, 0}, {7, -7, 0, 0}, {-7, 7, 0, 0}, {7, 7, 0, 0}};
static Quad sQuad_Size8 = {{-8, -8, 0, 0}, {8, -8, 0, 0}, {-8, 8, 0, 0}, {8, 8, 0, 0}};
static Quad sQuad_Size9 = {{-9, -9, 0, 0}, {9, -9, 0, 0}, {-9, 9, 0, 0}, {9, 9, 0, 0}};
static Quad sQuad_Size10 = {{-10, -10, 0, 0}, {10, -10, 0, 0}, {-10, 10, 0, 0}, {10, 10, 0, 0}};
static Quad sQuad_Size11 = {{-11, -11, 0, 0}, {11, -11, 0, 0}, {-11, 11, 0, 0}, {11, 11, 0, 0}};
static Quad sQuad_Size12 = {{-12, -12, 0, 0}, {12, -12, 0, 0}, {-12, 12, 0, 0}, {12, 12, 0, 0}};
static Quad sQuad_Size13 = {{-13, -13, 0, 0}, {13, -13, 0, 0}, {-13, 13, 0, 0}, {13, 13, 0, 0}};
static Quad sQuad_Size14 = {{-14, -14, 0, 0}, {14, -14, 0, 0}, {-14, 14, 0, 0}, {14, 14, 0, 0}};
static Quad sQuad_Size15 = {{-15, -15, 0, 0}, {15, -15, 0, 0}, {-15, 15, 0, 0}, {15, 15, 0, 0}};

static Quad *sSquareQuads[16] = {&sQuad_Size1,  &sQuad_Size1,  &sQuad_Size2,  &sQuad_Size3,
                                 &sQuad_Size4,  &sQuad_Size5,  &sQuad_Size6,  &sQuad_Size7,
                                 &sQuad_Size8,  &sQuad_Size9,  &sQuad_Size10, &sQuad_Size11,
                                 &sQuad_Size12, &sQuad_Size13, &sQuad_Size14, &sQuad_Size15};

#undef EVTF
#define EVTF 710
void Evtf710_Particle(EvtData *evt) {
   evt->x1.n += evt->x2.n;
   evt->z1.n += evt->z2.n;
   evt->y1.n += evt->y2.n;

   evt->x2.n += evt->x3.n;
   evt->z2.n += evt->z3.n;
   evt->y2.n += evt->y3.n;

   UpdateEvtAnimation(evt);

   if (evt->d.sprite.animFinished) {
      evt->d.sprite.hidden = 1;
   }

   evt->state2 = evt->state2 & 0xf;
   gSpriteBoxQuads[7] = sSquareQuads[evt->state2];
   AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
   gSpriteBoxQuads[7] = &gQuad_800fe63c;

   if (--evt->mem <= 0) {
      evt->functionIndex = EVTF_NULL;
   }
}

#undef EVTF
#define EVTF 797
void Evtf797_Map47_Scn14_Dusk(EvtData *evt) {
   // Spawned by EVDATA14.DAT (Grog's house) to indicate passage of time while waiting
   gLightColor.r = 0x80;
   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 378
void Evtf378_PhaseShift_FX1(EvtData *evt) {
   const s16 cornerThetas[4] = {DEG(45), DEG(135), DEG(-45), DEG(-135)};

   EvtData *runeSprite;  // sprite2 (xy)
   EvtData *crestSprite; // sprite2 (xy)
   EvtData *mapScaler;
   SVectorXY *pPosition;
   s32 i, j;
   s16 radius;
   s32 iRing;
   s16 quarterRadius;
   s32 startingTheta;
   s16 theta;

   switch (evt->state) {
   case 0:
      runeSprite = Evt_GetUnused();
      runeSprite->functionIndex = EVTF_NOOP;
      EVT.runeSprite = runeSprite;

      crestSprite = Evt_GetUnused();
      crestSprite->functionIndex = EVTF_NOOP;
      crestSprite->d.sprite2.gfxIdx = GFX_CREST;

      evt->x2.n = CV(0.625);
      evt->y2.n = CV(0.46875);
      evt->state3 = 96;

      pPosition = &EVT.positions[0];
      for (j = 0; j < ARRAY_COUNT(EVT.positions); j++) {
         pPosition->x = 137 + j;
         pPosition->y = j * 512;
         pPosition++;
      }

      EVT.crestSprite = crestSprite;
      evt->state++;

   // fallthrough
   case 1:
      crestSprite = EVT.crestSprite;
      runeSprite = EVT.runeSprite;

      switch (evt->state2) {
      case 0:
         radius = CV(0.25) * rsin(evt->mem * DEG(2.8125)) >> 12;
         if (evt->mem >= 32) {
            evt->state2++;
         }
         gLightColor.r += (0x20 - gLightColor.r) >> 4;
         gLightColor.g += (0x20 - gLightColor.g) >> 4;
         gLightColor.b += (0x20 - gLightColor.b) >> 4;
         break;

      case 1:
         radius = CV(0.25);
         if (evt->mem >= 96) {
            evt->state2++;
         }
         break;

      case 2:
         gLightColor.r += (0x80 - gLightColor.r) >> 4;
         gLightColor.g += (0x80 - gLightColor.g) >> 4;
         gLightColor.b += (0x80 - gLightColor.b) >> 4;
         radius = CV(0.25) * rsin(-evt->mem * DEG(2.8125)) >> 12;
         break;

      default:
         radius = CV(0.25);
         break;
      }

      startingTheta = (evt->mem * DEG(5.625));
      startingTheta &= 0xfff;
      pPosition = &crestSprite->d.sprite2.coords[0];
      for (j = 0; j < ARRAY_COUNT(cornerThetas); j++) {
         pPosition->x = evt->x2.n + (radius * rcos(startingTheta + cornerThetas[j]) >> 12);
         pPosition->y = evt->y2.n + (radius * rsin(startingTheta + cornerThetas[j]) >> 12);
         pPosition++;
      }

      AddEvtPrim2(gGraphicsPtr->ot, crestSprite);

      for (iRing = 0; iRing < 3; iRing++) {
         startingTheta = -startingTheta;
         quarterRadius = radius >> 2;

         for (i = 0; i < 16; i++) {
            theta = i * (DEG(360) / 16);
            runeSprite->x1.n = evt->x2.n + (radius * rcos(startingTheta + theta) >> 12);
            runeSprite->y1.n = evt->y2.n + (radius * rsin(startingTheta + theta) >> 12);

            pPosition = &runeSprite->d.sprite2.coords[0];
            for (j = 0; j < ARRAY_COUNT(cornerThetas); j++) {
               pPosition->x = runeSprite->x1.n +
                              (quarterRadius * rcos(startingTheta + theta + cornerThetas[j]) >> 12);
               pPosition->y = runeSprite->y1.n +
                              (quarterRadius * rsin(startingTheta + theta + cornerThetas[j]) >> 12);
               pPosition++;
            }

            runeSprite->d.sprite2.gfxIdx = GFX_RUNE_1 + (i % 8);
            AddEvtPrim2(gGraphicsPtr->ot, runeSprite);
         }

         radius += (radius >> 1);
      }

      gCameraZoom.vz += (512 - gCameraZoom.vz) >> 4;

      if (evt->mem == 96) {
         mapScaler = Evt_GetUnused();
         mapScaler->functionIndex = EVTF_PHASE_SHIFT_MAP_SCALER;
      }
      if (++evt->mem >= 128) {
         evt->state++;
      }
      break;

   case 2:
      crestSprite = EVT.crestSprite;
      crestSprite->functionIndex = EVTF_NULL;
      runeSprite = EVT.runeSprite;
      runeSprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 363
void Evtf363_Wyrmfang_FX1(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      EVT.cwFlames = CreatePositionedEvt(evt, EVTF_WYRMFANG_FLAMES_CW);
      EVT.ccwFlames = CreatePositionedEvt(evt, EVTF_WYRMFANG_FLAMES_CCW);
      evt->state++;

   // fallthrough
   case 1:
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 4);
      gCameraZoom.vz += (768 - gCameraZoom.vz) >> 4;
      gCameraRotation.vy -= 8;

      gLightColor.r += (0x40 - gLightColor.r) >> 4;
      gLightColor.g += (0x40 - gLightColor.g) >> 4;
      gLightColor.b += (0x40 - gLightColor.b) >> 4;

      if (++evt->mem >= 32) {
         evt->state++;
         evt->state3 = 0;
         evt->mem = 0;
      }
      break;

   case 2:
      evt->state3 += 2;
      if (evt->state3 >= 0xff) {
         evt->state3 = 0xff;
      }

      gLightColor.r = evt->state3;

      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 4);
      gCameraZoom.vz += (768 - gCameraZoom.vz) >> 4;
      gCameraRotation.vy -= 8;

      if (++evt->mem >= 160) {
         evt->state++;
      }
      break;

   case 3:
      gLightColor.r = 0x80;
      gLightColor.g = 0x80;
      gLightColor.b = 0x80;

      EVT.cwFlames->functionIndex = EVTF_NULL;
      EVT.ccwFlames->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;

      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 359
void Evtf359_PhaseShift_MapScaler(EvtData *evt) {
   s16 radius;

   switch (evt->state) {
   case 0:
      evt->y2.n = gCameraPos.vy;
      gState.enableMapScaling = 1;
      evt->state++;

   // fallthrough
   case 1:
      evt->state2++;
      radius = abs(FIXED(0.5) * rsin(evt->mem * DEG(360.0 / 128)) >> 12);

      gMapScale.vy = FIXED(0.5) + (radius * rsin(evt->mem * DEG(11.25)) >> 12);
      gMapScale.vx = FIXED(0.5) + (radius * rsin(evt->mem * DEG(22.5)) >> 12);
      gMapScale.vz = FIXED(0.5) + (radius * rcos(evt->mem * DEG(22.5)) >> 12);

      gCameraZoom.vz += (512 - gCameraZoom.vz) >> 4;
      gCameraPos.vy = evt->y2.n + (gMapScale.vy >> 8);
      gCameraRotation.vy += DEG(360.0 / 128);

      if (++evt->mem >= 128) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 2:
      gMapScale.vy += (FIXED(1.0) - gMapScale.vy) >> 3;
      gMapScale.vx += (FIXED(1.0) - gMapScale.vx) >> 3;
      gMapScale.vz += (FIXED(1.0) - gMapScale.vz) >> 3;
      gCameraPos.vy += (evt->y2.n - gCameraPos.vy) >> 3;

      if (++evt->mem >= 32) {
         evt->state++;
      }
      break;

   case 3:
      gMapScale.vx = FIXED(1.0);
      gMapScale.vz = FIXED(1.0);
      gMapScale.vy = FIXED(1.0);
      gState.enableMapScaling = 0;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

s32 InEventScene(void) {
   if (gState.primary == STATE_4 || gState.primary == STATE_25 || gState.primary == STATE_16 ||
       gState.primary == STATE_17) {
      return 1;
   } else {
      return 0;
   }
}

void SetupMapExtras(void) {
   extern EvtData *D_801233CC;
   EvtData *evt;
   s32 i;

   evt = Evt_GetUnused();
   evt->functionIndex = EVTF_CAMERA_TBD_277;
   D_801233CC = evt;

   if (gState.mapNum == 11) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP11;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_HEAVY_RAINFALL;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(18.375);
      evt->z1.n = CV(2.375);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(13.375);
      evt->z1.n = CV(10.625);
   }

   if (gState.mapNum == 13) {
      if (InEventScene() == 1) {
         evt = Evt_GetUnused();
         evt->functionIndex = EVTF_MAP13_BRIDGE_EXPLOSION_IN_SCENE;
      } else {
         evt = Evt_GetUnused();
         evt->functionIndex = EVTF_MAP13_BRIDGE_EXPLOSION_IN_BATTLE;
      }
   }

   if (!InEventScene() && gState.mapNum == 14) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP14_SAND;
   }

   if (gState.mapNum == 15 && !InEventScene()) {
      Map15_RaisePlank(7, 11);
      Map15_RaisePlank(14, 11);
   }

   if (gState.mapNum == 17) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP17_BUTTON;
   }

   if (gState.mapNum == 17) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP17_FLOODGATE;
   }

   if (gState.mapNum == 19) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP19;
   }

   if (gState.mapNum == 26) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP26;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;
   }

   if (gState.mapNum == 27) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP27;
   }

   if (gState.mapNum == 28) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP28_BUTTON;

      for (i = 194; i < 197; i++) {
         gGfxSubTextures[GFX_MAP_TEXTURES_OFS + i].h = 24;
      }

      if (gState.primary == STATE_4 && gState.scene == 50) {
         gState.mapState.s.field_0x0 = 1;
      }
   }

   if (gState.mapNum == 29) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP29;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;
   }

   if (gState.mapNum == 32) {
      evt = Evt_GetUnused();
      evt->x1.n = CV(58.0);
      evt->z1.n = CV(4.0);
      evt->y1.n = CV(6.75);
      evt->functionIndex = EVTF_MAP32_SMOKESTACK;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP32_CAR_RELEASE;
   }

   if (gState.mapNum == 33) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP33;
   }

   if (gState.mapNum == 35) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP35_BUTTON;
   }

   if (gState.mapNum == 36 && !InEventScene()) {
      // Leena shielding villager
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP36_LEENA;
      evt->x1.n = CV(7.5);
      evt->z1.n = CV(6.5);
      gMapUnitsPtr[6][7].s.team = TEAM_NEUTRAL;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP36_VILLAGER;
      evt->x1.n = CV(7.5);
      evt->z1.n = CV(7.5);
      gMapUnitsPtr[7][7].s.team = TEAM_NEUTRAL;
   }

   if (gState.mapNum == 39 && !InEventScene()) {
      // Leena raising submerged platforms
      Map39_Setup();
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP39;
   }

   if (gState.mapNum == 40) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP40_BARRICADE;
      gGfxSubTextures[GFX_MAP_TEXTURE_232].h = 40;
      gGfxSubTextures[GFX_MAP_TEXTURE_233].h = 40;
   }

   if (gState.mapNum == 38) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP38_FLOODGATE;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(20.5);
      evt->z1.n = CV(11.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(18.5);
      evt->z1.n = CV(12.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(17.5);
      evt->z1.n = CV(12.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(13.0);
      evt->z1.n = CV(2.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(11.0);
      evt->z1.n = CV(3.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(3.0);
      evt->z1.n = CV(13.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(1.0);
      evt->z1.n = CV(14.0);
      evt->y1.n = CV(3.0);
   }

   if (gState.mapNum == 42) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(2.375);
      evt->z1.n = CV(9.625);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(19.375);
      evt->z1.n = CV(3.375);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(17.625);
      evt->z1.n = CV(14.625);
   }
}
