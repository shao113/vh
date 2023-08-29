#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

// TODO: Figure out if this data should be split or continued. Some of this is accessed pretty far
// apart, so it will probably need to be global.
//
// From the ordering and xref locations, and the interspersing of probable static data, I'm guessing
// these were defined (probably as global) as they were needed, and then occasionally re-used later.
// For now, I'll continue defining them as static (function-scoped where reasonable), making global
// as needed, and not worry about precise placement until later when I begin finer file splits.

// The values offset by 0x100 will be flipped.
static s16 sDarkFireSphereAnimData_800ff344[36] = {7, GFX_TILED_GRAY_SPARKLES_DYN_1, // Marker?
                                                   1, GFX_LIT_SPHERE_1,
                                                   1, GFX_LIT_SPHERE_2,
                                                   1, GFX_LIT_SPHERE_3,
                                                   1, GFX_LIT_SPHERE_4,
                                                   1, GFX_LIT_SPHERE_5,
                                                   1, GFX_LIT_SPHERE_6,
                                                   1, GFX_LIT_SPHERE_7,
                                                   1, GFX_LIT_SPHERE_8,
                                                   1, GFX_LIT_SPHERE_7 + 0x100,
                                                   1, GFX_LIT_SPHERE_6 + 0x100,
                                                   1, GFX_LIT_SPHERE_5 + 0x100,
                                                   1, GFX_LIT_SPHERE_4 + 0x100,
                                                   1, GFX_LIT_SPHERE_3 + 0x100,
                                                   1, GFX_LIT_SPHERE_2 + 0x100,
                                                   1, GFX_LIT_SPHERE_1 + 0x100,
                                                   1, GFX_NULL,
                                                   1, GFX_NULL};

s16 gSparkleAnimData_800ff38c[14] = {4, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};

static s16 sExplosionAnimData_800ff3a8[26] = {
    0, GFX_EXPLOSION_1, 2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,  2, GFX_EXPLOSION_4,
    2, GFX_EXPLOSION_5, 2, GFX_EXPLOSION_6,  2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,
    2, GFX_EXPLOSION_9, 2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
    1, GFX_NULL};

s16 gExplosionAnimData_800ff3dc[26] = {0, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                                       2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

static s16 sFlameAnimData_800ff410[20] = {
    0, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
    2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    1, GFX_NULL};

static s16 sSmokeAnimData_800ff438[24] = {
    0, GFX_PUFF_1, 2, GFX_PUFF_2, 2, GFX_PUFF_3, 2, GFX_PUFF_4,  2, GFX_PUFF_5, 2, GFX_PUFF_6,
    2, GFX_PUFF_7, 2, GFX_PUFF_8, 2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   1, GFX_NULL};

s16 gLightningAnimData_800ff468[20] = {0, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3,
                                       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
                                       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_NULL,
                                       1, GFX_NULL};

static s16 sSmokeAnimData_800ff490[8] = {0, GFX_PUFF_2, 1, GFX_PUFF_3, 1, GFX_NULL, 1, GFX_NULL};

static s16 *sAnimPointers_800ff4a0[4] = {gSparkleAnimData_800ff38c, sExplosionAnimData_800ff3a8,
                                         sFlameAnimData_800ff410, sSmokeAnimData_800ff438};

static s16 sImpactAnimData_800ff4b0[18] = {5, GFX_IMPACT_1, 1, GFX_IMPACT_2, 1, GFX_IMPACT_3,
                                           1, GFX_IMPACT_4, 1, GFX_IMPACT_5, 1, GFX_IMPACT_6,
                                           1, GFX_IMPACT_7, 1, GFX_NULL,     0, GFX_NULL};

static s16 sImpactAnimData_800ff4d4[18] = {0, GFX_IMPACT_1, 1, GFX_IMPACT_2, 1, GFX_IMPACT_3,
                                           1, GFX_IMPACT_4, 1, GFX_IMPACT_5, 1, GFX_IMPACT_6,
                                           1, GFX_IMPACT_7, 1, GFX_NULL,     0, GFX_NULL};

static s16 sFlameAnimData_800ff4f8[20] = {
    0, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
    2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    1, GFX_NULL};

static s16 sExplosionAnimData_800ff520[26] = {
    3, GFX_EXPLOSION_1, 2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,  2, GFX_EXPLOSION_4,
    2, GFX_EXPLOSION_5, 2, GFX_EXPLOSION_6,  2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,
    2, GFX_EXPLOSION_9, 2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
    0, GFX_NULL};

static s16 sExplosionAnimData_800ff554[26] = {
    4, GFX_EXPLOSION_1, 1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,  1, GFX_EXPLOSION_4,
    1, GFX_EXPLOSION_5, 1, GFX_EXPLOSION_6,  1, GFX_EXPLOSION_7,  1, GFX_EXPLOSION_8,
    1, GFX_EXPLOSION_9, 1, GFX_EXPLOSION_10, 1, GFX_EXPLOSION_11, 1, GFX_NULL,
    0};

static s16 sLightningAnimData_800ff588[36] = {
    0, GFX_LIGHTNING_1, 1, GFX_LIGHTNING_2, 1, GFX_LIGHTNING_3, 1, GFX_LIGHTNING_4,
    1, GFX_LIGHTNING_5, 1, GFX_LIGHTNING_6, 1, GFX_LIGHTNING_7, 1, GFX_LIGHTNING_8,
    1, GFX_LIGHTNING_1, 1, GFX_LIGHTNING_2, 1, GFX_LIGHTNING_3, 1, GFX_LIGHTNING_4,
    1, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6, 2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8,
    2, GFX_NULL,        0, GFX_NULL};

#undef EVTF
#define EVTF 122
void Evtf122_DarkFire_FX1(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_s1;
   EvtData *evt_s0;
   EvtData *target;
   POLY_FT4 *poly;
   s32 i;
   s16 theta;
   s16 halfSize;

   switch (evt->state) {
   case 0:
      target = Evt_GetUnused();
      target->functionIndex = EVTF_NOOP;
      target->x1.n = gTargetX * 0x100 + 0x80;
      target->z1.n = gTargetZ * 0x100 + 0x80;
      target->y1.n = GetTerrainElevation(gTargetZ, gTargetX) + 0x100;
      EVT.target = target;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.target = target;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.animData = sDarkFireSphereAnimData_800ff344;
      EVT.sprite = evt_s1;

      for (i = 0; i < 0x40; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_DARK_FIRE_RAY;
         evt_s1->d.evtf188.target = target;
      }

      EVT.savedLightRotZ = gLightRotation.vz;
      EVT.savedLightRotY = gLightRotation.vy;
      evt->state++;

   // fallthrough
   case 1:

      switch (evt->state2) {
      case 0:
         if (gLightColor.g > 0x20) {
            gLightColor.g -= 4;
            gLightColor.b -= 4;
            gLightColor.r += 4;
         }
         gQuad_800fe63c[0].vx = -0x18;
         gQuad_800fe63c[0].vy = -0x18;
         gQuad_800fe63c[1].vx = 0x18;
         gQuad_800fe63c[1].vy = -0x18;
         gQuad_800fe63c[2].vx = -0x18;
         gQuad_800fe63c[2].vy = 0x18;
         gQuad_800fe63c[3].vx = 0x18;
         gQuad_800fe63c[3].vy = 0x18;
         break;

      case 1:
         theta = EVT.theta - ANGLE_90_DEGREES;
         if (theta < 0) {
            theta = 0;
         }
         halfSize = 0x18 * (ONE - rsin(theta)) / ONE;
         gQuad_800fe63c[0].vx = -halfSize;
         gQuad_800fe63c[0].vy = -halfSize;
         gQuad_800fe63c[1].vx = halfSize;
         gQuad_800fe63c[1].vy = -halfSize;
         gQuad_800fe63c[2].vx = -halfSize;
         gQuad_800fe63c[2].vy = halfSize;
         gQuad_800fe63c[3].vx = halfSize;
         gQuad_800fe63c[3].vy = halfSize;
         break;
      }

      evt_s2 = EVT.target;
      evt_s1 = EVT.sprite;

      UpdateEvtAnimation(evt_s1);
      if (evt_s1->d.sprite.gfxIdx >= 0x100) {
         evt_s1->d.sprite.gfxIdx -= 0x100;
         evt_s1->d.sprite.facingLeft = 1;
      } else {
         evt_s1->d.sprite.facingLeft = 0;
      }

      evt_s1->x1.n = evt_s2->x1.n;
      evt_s1->z1.n = evt_s2->z1.n;
      evt_s1->y1.n = GetTerrainElevation(evt_s2->z1.s.hi, evt_s2->x1.s.hi) + 0x100;
      evt_s1->d.sprite.semiTrans = 0;
      evt_s1->d.sprite.clut = 4;

      if (evt_s1->d.sprite.gfxIdx != GFX_TILED_GRAY_SPARKLES_DYN_1) {
         AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, 0x30, 0x30, 0x30);
      }

      evt_s0 = Evt_GetUnused();
      CopyEvtData(evt_s1, evt_s0);
      evt_s0->d.sprite.gfxIdx = GFX_LIT_SPHERE_8;
      evt_s0->d.sprite.clut = 4;
      evt_s0->d.sprite.semiTrans = 0;
      AddEvtPrim6(gGraphicsPtr->ot, evt_s0, 0);
      evt_s0->functionIndex = EVTF_NULL;
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x18, 0x18, 0x18);

      switch (evt->state2) {
      case 0:
         if (EVT.timer == 128) {
            evt->state2++;
         }
         EVT.timer += 8;
         gLightRotation.vz = (gLightRotation.vz + 0x80) % ANGLE_360_DEGREES;
         gLightRotation.vy = (gLightRotation.vy + 0x20) % ANGLE_360_DEGREES;
         break;

      case 1:
         if (theta > 0x280) {
            gLightColor.g += 4;
            gLightColor.b += 4;
            gLightColor.r -= 4;
            gLightRotation.vz += (EVT.savedLightRotZ - gLightRotation.vz) >> 3;
            gLightRotation.vy += (EVT.savedLightRotY - gLightRotation.vy) >> 3;
         } else {
            gLightRotation.vz = (gLightRotation.vz + 0x80) % ANGLE_360_DEGREES;
            gLightRotation.vy = (gLightRotation.vy + 0x20) % ANGLE_360_DEGREES;
         }
         if (theta == 0x220) {
            gSignal3 = 1;
         }
         if (theta >= ANGLE_90_DEGREES) {
            evt->functionIndex = EVTF_NULL;
            evt_s1->functionIndex = EVTF_NULL;
            evt_s2->functionIndex = EVTF_NULL;
            gLightColor.r = 0x80;
            gLightColor.g = 0x80;
            gLightColor.b = 0x80;
            gLightRotation.vz = EVT.savedLightRotZ;
            gLightRotation.vy = EVT.savedLightRotY;
         }
         EVT.theta += 0x10;
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 188
void Evtf188_DarkFire_Ray(EvtData *evt) {
   EvtData *target;
   EvtData *sprite;

   switch (evt->state) {
   case 0:
      target = EVT.target;
      evt->y1.n = GetTerrainElevation(target->z1.s.hi, target->x1.s.hi) + 0x100;
      evt->x1.n = target->x1.n;
      evt->z1.n = target->z1.n;

      EVT.todo_x24 = 0x300 + rand() % 0x300;
      EVT.todo_x26 = rand() % 0x1000;
      EVT.todo_x28 = rand() % 0x1000;
      EVT.todo_x2a = rand() % 0x1000;
      EVT.todo_x2c = 0x60 - rand() % 0xc1;
      EVT.todo_x2e = 0x60 - rand() % 0xc1;
      EVT.todo_x30 = 0x60 - rand() % 0xc1;

      evt->state++;

   // fallthrough
   case 1:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_COLOR_14;
      sprite->d.sprite.clut = 4;
      sprite->d.sprite.semiTrans = 1;

      EVT.todo_x34 = EVT.todo_x24 * EVT.todo_x36 / 0x20;

      sprite->d.sprite.coords[0].x = evt->x1.n;
      sprite->d.sprite.coords[0].z = evt->z1.n;
      sprite->d.sprite.coords[0].y = evt->y1.n;
      sprite->d.sprite.coords[1].x = evt->x1.n;
      sprite->d.sprite.coords[1].z = evt->z1.n;
      sprite->d.sprite.coords[1].y = evt->y1.n;
      sprite->d.sprite.coords[2].x = evt->x1.n + EVT.todo_x34 * rcos(EVT.todo_x26) / ONE;
      sprite->d.sprite.coords[2].z = evt->z1.n + EVT.todo_x34 * rsin(EVT.todo_x28) / ONE;
      sprite->d.sprite.coords[2].y = evt->y1.n + EVT.todo_x34 * rsin(EVT.todo_x2a) / ONE;
      sprite->d.sprite.coords[3].x = evt->x1.n + EVT.todo_x34 * rcos(EVT.todo_x26 + 8) / ONE;
      sprite->d.sprite.coords[3].z = evt->z1.n + EVT.todo_x34 * rsin(EVT.todo_x28 + 8) / ONE;
      sprite->d.sprite.coords[3].y = evt->y1.n + EVT.todo_x34 * rsin(EVT.todo_x2a + 8) / ONE;

      AddEvtPrim4(gGraphicsPtr->ot, sprite);
      EVT.todo_x26 += EVT.todo_x2c;
      EVT.todo_x28 += EVT.todo_x2e;
      EVT.todo_x2a += EVT.todo_x30;

      sprite->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.todo_x36 += 2;
         if (EVT.todo_x36 == 0x20) {
            evt->state2++;
         }
         break;
      case 1:
         EVT.todo_x32++;
         if (EVT.todo_x32 == 0x5c) {
            evt->state2++;
         }
         break;
      case 2:
         EVT.todo_x36 -= 2;
         if (EVT.todo_x36 <= 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF Unk80080924
void Evtf_Unk_80080924(EvtData *evt) {
   // Very similar to Evtf215_Cloud
   extern s16 gSmokeAnimData_800ff1b0[24];
   EvtData *sprite;
   s16 halfSize, randomAngle;
   s16 theta;

   switch (evt->state) {
   case 0:
      theta = EVT.theta;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = gSmokeAnimData_800ff1b0;
      sprite->d.sprite.animInitialized = 0;
      EVT.sprite = sprite;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position1.x = rsin(randomAngle) >> 6;
      EVT.position1.z = rcos(randomAngle) >> 6;
      EVT.position1.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position2.x = rsin(randomAngle) >> 6;
      EVT.position2.z = rcos(randomAngle) >> 6;
      EVT.position2.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position3.x = rsin(randomAngle) >> 6;
      EVT.position3.z = rcos(randomAngle) >> 6;
      EVT.position3.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position4.x = rsin(randomAngle) >> 6;
      EVT.position4.z = rcos(randomAngle) >> 6;
      EVT.position4.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position5.x = rsin(randomAngle) >> 6;
      EVT.position5.z = rcos(randomAngle) >> 6;
      EVT.position5.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position6.x = rsin(randomAngle) >> 6;
      EVT.position6.z = rcos(randomAngle) >> 6;
      EVT.position6.y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position7_x = rsin(randomAngle) >> 6;
      EVT.position7_z = rcos(randomAngle) >> 6;
      EVT.position7_y = (rand() % 0x60) + 0x20;

      randomAngle = rand() % ANGLE_360_DEGREES;
      EVT.position8_x = rsin(randomAngle) >> 6;
      EVT.position8_z = rcos(randomAngle) >> 6;
      EVT.position8_y = (rand() % 0x60) + 0x20;

      EVT.halfSize = (rand() % theta) + theta;
      evt->state++;

   // fallthrough
   case 1:
      halfSize = EVT.halfSize;
      gQuad_800fe63c[0].vx = -halfSize;
      gQuad_800fe63c[0].vy = -halfSize;
      gQuad_800fe63c[1].vx = halfSize;
      gQuad_800fe63c[1].vy = -halfSize;
      gQuad_800fe63c[2].vx = -halfSize;
      gQuad_800fe63c[2].vy = halfSize;
      gQuad_800fe63c[3].vx = halfSize;
      gQuad_800fe63c[3].vy = halfSize;

      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);

      sprite->x1.n =
          evt->x1.n + EVT.position1.x + (EVT.position1.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position1.z + (EVT.position1.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position1.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position2.x + (EVT.position2.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position2.z + (EVT.position2.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position2.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position3.x + (EVT.position3.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position3.z + (EVT.position3.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position3.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position4.x + (EVT.position4.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position4.z + (EVT.position4.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position4.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position5.x + (EVT.position5.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position5.z + (EVT.position5.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position5.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position6.x + (EVT.position6.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position6.z + (EVT.position6.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position6.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position7_x + (EVT.position6.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position7_z + (EVT.position6.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position7_y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n =
          evt->x1.n + EVT.position8_x + (EVT.position6.x * (ONE - rcos(EVT.theta)) / ONE);
      sprite->z1.n =
          evt->z1.n + EVT.position8_z + (EVT.position6.z * (ONE - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position8_y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      EVT.theta += (ANGLE_90_DEGREES / 10);
      if (EVT.theta >= ANGLE_90_DEGREES) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void func_800815B0(void) {
   SetDefDispEnv(&gGraphicsPtr->dispEnv, SCREEN_WIDTH, gGraphicsPtr->dispEnv.disp.y, SCREEN_WIDTH,
                 SCREEN_HEIGHT);
   PutDispEnv(&gGraphicsPtr->dispEnv);
   SetDefDispEnv(&gGraphicsPtr->dispEnv, 0, gGraphicsPtr->dispEnv.disp.y, SCREEN_WIDTH,
                 SCREEN_HEIGHT);
   PutDispEnv(&gGraphicsPtr->dispEnv);
}

#undef EVTF
#define EVTF 291
void Evtf291_ChestImpact(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = sImpactAnimData_800ff4d4;
      evt->state++;

   // fallthrough
   case 1:
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
   }
}

#undef EVTF
#define EVTF 080
void Evtf080_RomanFire_FX1(EvtData *evt) {
   EvtData *target;
   EvtData *evt_s1;
   s32 i;

   switch (evt->state) {
   case 0:
      target = Evt_GetUnused();
      target->functionIndex = EVTF_NOOP;
      target->x1.n = gMapCursorX * 0x100 + 0x80;
      target->z1.n = gMapCursorZ * 0x100 + 0x80;
      target->y1.n = GetTerrainElevation(gMapCursorZ, gMapCursorX);
      EVT.target = target;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.target = target;
      evt_s1->d.evtf026.type = 0;
      evt_s1->d.evtf026.zoom = gCameraZoom.vz;
      EVT.cam = evt_s1;

      evt->x1.n = target->x1.n;
      evt->z1.n = target->z1.n;
      evt->y1.n = target->y1.n;
      evt->y2.n = 0x60;
      evt->y3.n = -8;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_TBD_15;
      evt_s1->d.sprite.animData = gSparkleAnimData_800ff38c;
      EVT.sprite = evt_s1;
      evt->state2 = 8;
      evt->state++;
      break;

   case 2:
      if (--evt->state2 == 0) {
         evt->state2 = 1;
         evt->state++;
      }
      break;

   case 3:
      if (evt->state2 == 1) {
         evt_s1 = EVT.cam;
         if (evt_s1->functionIndex == EVTF_CAMERA_TBD_026) {
            evt_s1->functionIndex = EVTF_NULL;
            evt->state2 = 0;
         }
      }

      evt->y1.n += evt->y2.n;
      evt->y2.n += evt->y3.n;
      //?
      gCameraPos.vy += ((evt->y1.n >> 3) - gCameraPos.vy) >> 3;
      gCameraPos.vy = evt->y1.n >> 3;
      evt_s1 = EVT.sprite;
      evt_s1->x1.n = evt->x1.n;
      evt_s1->z1.n = evt->z1.n;
      evt_s1->y1.n = evt->y1.n;

      if (evt->y2.n < 0) {
         evt->state2 = 0x40;
         evt->state++;
      }

      if (evt->y1.n < GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + 0x40) {
         evt->state = 5;
         evt->state2 = 8;
      }

      UpdateEvtAnimation(evt_s1);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);
      gCameraRotation.vy += 16;
      gCameraRotation.vx += 4;
      break;

   case 4:
      for (i = 0; i < 8; i++) {
         evt_s1 = CreatePositionedEvt(evt, EVTF_ROMAN_FIRE_FLAME);
         evt_s1->functionIndex = EVTF_ROMAN_FIRE_FLAME;
         // evt_s1->x2.n = 0x20 * rcos(i * 0x200 + evt->y1.n * 4) >> 12;
         evt_s1->x2.n = rcos(i * 0x200 + evt->y1.n * 4) >> 7;
         evt_s1->z2.n = rsin(i * 0x200 + evt->y1.n * 4) >> 7;
         evt_s1->y2.n = 0x20;
         evt_s1->y3.n = -8;
         evt_s1->d.sprite.animData = sExplosionAnimData_800ff3a8;
         evt_s1->d.sprite.boxIdx = 5;
      }
      evt->state = 3;
      gCameraRotation.vy += 16;
      gCameraRotation.vx += 4;
      gCameraZoom.vz += 4;
      break;

   case 5:
      gCameraRotation.vy += 16;
      gCameraRotation.vx += 4;
      gCameraZoom.vz += 4;
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 6:
      evt_s1 = EVT.sprite;
      target = EVT.target;
      evt_s1->functionIndex = EVTF_NULL;
      target->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 081
void Evtf081_RomanFire_Flame(EvtData *evt) {
   s32 elevation;

   evt->x1.n += evt->x2.n;
   evt->z1.n += evt->z2.n;
   evt->y1.n += evt->y2.n;
   evt->y2.n += evt->y3.n;

   if (evt->d.sprite.animData != NULL) {
      UpdateEvtAnimation(evt);
   }

   AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

   elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
   if (evt->y1.n <= elevation) {
      evt->y1.n = elevation;
      evt->state++;
      evt->y2.n = -(evt->y2.n >> 2);

      switch (evt->state) {
      case 0:
      case 1:
      case 2:
         evt->d.sprite.animInitialized = 0;
         evt->d.sprite.animData = sSmokeAnimData_800ff438;
         break;
      case 3:
      case 4:
         break;
      case 5:
         evt->functionIndex = EVTF_NULL;
         break;
      }
   }
}

#undef EVTF
#define EVTF 094
void Evtf094_MoodRing_FX1(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s1; // ring, cam
   BVectorXZ *p;
   s32 i;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.target = unitSprite;
      evt_s1->d.evtf026.type = 0;
      evt_s1->d.evtf026.zoom = 0x200;

      evt->x2.n = unitSprite->x1.n;
      evt->z2.n = unitSprite->z1.n;
      evt->y2.n = unitSprite->y1.n;

      p = (BVectorXZ *)gTargetCoords;
      for (i = 0; i < 8; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_MOOD_RING_RING;
         evt_s1->x1.n = evt->x2.n + (0x800 * rcos(i * 0x200) >> 12);
         evt_s1->z1.n = evt->z2.n + (0x800 * rsin(i * 0x200) >> 12);
         evt_s1->y1.n = evt->y2.n + 0x100;
         evt_s1->d.evtf095.theta = i * 0x200;
         evt_s1->d.evtf095.radius = 0x800;
         evt_s1->x2.n = evt->x2.n;
         evt_s1->z2.n = evt->z2.n;
         evt_s1->y2.n = evt->y2.n;
         evt_s1->d.evtf095.parent = evt;
         evt_s1->state2 = 56;

         if (p->x != 0xff) {
            unitSprite = GetUnitSpriteAtPosition(p->z, p->x);
            evt_s1->x3.n = unitSprite->x1.n;
            evt_s1->y3.n = unitSprite->y1.n + 0x80;
            evt_s1->z3.n = unitSprite->z1.n;
            p++;
         } else {
            evt_s1->x3.n = evt_s1->x1.n;
            evt_s1->y3.n = evt_s1->y1.n + 0x80;
            evt_s1->z3.n = evt_s1->z1.n;
         }
      }

      evt->state2 = 56;
      evt->state++;
      break;

   case 1:
      if (--evt->state2 < 0) {
         evt->state2 = 3;
         evt->state++;
      }
      break;

   case 2:
      EVT.launchingOutward = 1;
      if (--evt->state2 < 0) {
         evt->state++;
      }
      break;

   case 3:
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

void MoodRing_RenderRing(EvtData *evt, s16 halfSize, s16 theta, s16 clut) {
   SVECTOR vector;
   EvtData *top;
   EvtData *bottom;
   s32 a;

   if ((gCameraRotation.vy & 0xfff) < 0x400 || (gCameraRotation.vy & 0xfff) > 0xa00) {
      halfSize = -halfSize;
   }

   a = abs(halfSize) * 362 >> 8;
   func_800A9E78(&vector, a, theta, 0x200);

   bottom = Evt_GetUnused();
   bottom->functionIndex = EVTF_NOOP;
   bottom->d.sprite.boxIdx = 3;
   bottom->d.sprite.gfxIdx = GFX_RING_BTM;
   bottom->d.sprite.clut = clut;

   top = Evt_GetUnused();
   top->functionIndex = EVTF_NOOP;
   top->d.sprite.boxIdx = 3;
   top->d.sprite.gfxIdx = GFX_RING_TOP;
   top->d.sprite.clut = clut;

   bottom->d.sprite.coords[0].x = top->d.sprite.coords[2].x = evt->x1.n - halfSize;
   bottom->d.sprite.coords[0].z = top->d.sprite.coords[2].z = evt->z1.n + halfSize;
   bottom->d.sprite.coords[0].y = top->d.sprite.coords[2].y = evt->y1.n;
   bottom->d.sprite.coords[1].x = top->d.sprite.coords[3].x = evt->x1.n + halfSize;
   bottom->d.sprite.coords[1].z = top->d.sprite.coords[3].z = evt->z1.n - halfSize;
   bottom->d.sprite.coords[1].y = top->d.sprite.coords[3].y = evt->y1.n;
   bottom->d.sprite.coords[2].x = bottom->d.sprite.coords[0].x + vector.vx;
   bottom->d.sprite.coords[2].z = bottom->d.sprite.coords[0].z + vector.vz;
   bottom->d.sprite.coords[2].y = bottom->d.sprite.coords[0].y + vector.vy;
   bottom->d.sprite.coords[3].x = bottom->d.sprite.coords[1].x + vector.vx;
   bottom->d.sprite.coords[3].z = bottom->d.sprite.coords[1].z + vector.vz;
   bottom->d.sprite.coords[3].y = bottom->d.sprite.coords[1].y + vector.vy;
   bottom->x1.n = bottom->d.sprite.coords[3].x;
   bottom->z1.n = bottom->d.sprite.coords[3].z;
   bottom->y1.n = bottom->d.sprite.coords[3].y;
   AddEvtPrim3(gGraphicsPtr->ot, bottom, 0); //? Extra arg

   top->d.sprite.coords[0].x = top->d.sprite.coords[2].x - vector.vx;
   top->d.sprite.coords[0].z = top->d.sprite.coords[2].z - vector.vz;
   top->d.sprite.coords[0].y = top->d.sprite.coords[2].y - vector.vy;
   top->d.sprite.coords[1].x = top->d.sprite.coords[3].x - vector.vx;
   top->d.sprite.coords[1].z = top->d.sprite.coords[3].z - vector.vz;
   top->d.sprite.coords[1].y = top->d.sprite.coords[3].y - vector.vy;
   top->x1.n = top->d.sprite.coords[0].x;
   top->z1.n = top->d.sprite.coords[0].z;
   top->y1.n = top->d.sprite.coords[0].y;
   AddEvtPrim3(gGraphicsPtr->ot, top, 0); //? Extra arg

   top->functionIndex = EVTF_NULL;
   bottom->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 095
void Evtf095_MoodRing_Ring(EvtData *evt) {
   EvtData *parent;
   s16 a;

   switch (evt->state) {
   case 0:
      EVT.gfxIdx_unused = GFX_RING; // Just guessing
      EVT.boxIdx_unused = 3;        // for these fields
      EVT.unused_0x44 = 0x40;
      evt->state2 = 0x200;
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:
      EVT.theta += 0x40;
      EVT.radius -= (EVT.radius >> 4);
      a = evt->x2.n + (rcos(EVT.theta) * EVT.radius >> 12);
      evt->x1.n = a;
      a = evt->z2.n + (rsin(EVT.theta) * EVT.radius >> 12);
      evt->z1.n = a;
      MoodRing_RenderRing(evt, evt->state2, 0, 3 + evt->state3 % 3);
      evt->state2 -= (evt->state2 - 0x20) >> 4;
      evt->state3++;
      parent = EVT.parent;
      if (parent->d.evtf094.launchingOutward == 1) {
         evt->state3 = 0x10;
         evt->state++;
      }
      break;

   case 2:
      evt->state2 = 0x80;
      evt->x1.n -= (evt->x1.n - evt->x3.n) >> 2;
      evt->z1.n -= (evt->z1.n - evt->z3.n) >> 2;
      evt->y1.n -= (evt->y1.n - evt->y3.n) >> 2;
      MoodRing_RenderRing(evt, evt->state2, 0, 3 + evt->state3 % 3);
      if (--evt->state3 == 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 097
void Evtf097_MoodRing_FX3(EvtData *evt) {
   EVT.fatal = 1;
   evt->functionIndex = EVTF_MOOD_RING_FX2;
}

#undef EVTF
#define EVTF 096
void Evtf096_MoodRing_FX2(EvtData *evt) {
   EvtData *targetSprite;

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.targetSprite = targetSprite;
      SnapToUnit(evt);
      evt->y2.n = evt->y1.n;
      evt->state2 = 0x80;
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.targetSprite;
      evt->y1.n = targetSprite->y1.n + 0xc0;
      MoodRing_RenderRing(evt, evt->state2 + 0x60, 0, 3 + evt->state3 % 3);
      evt->y1.n = targetSprite->y1.n + 0x80;
      MoodRing_RenderRing(evt, evt->state2 + 0x60, 0, 3 + evt->state3 % 3);
      evt->y1.n = targetSprite->y1.n + 0x40;
      MoodRing_RenderRing(evt, evt->state2 + 0x60, 0, 3 + evt->state3 % 3);
      evt->state3++;
      evt->state2 -= 0x10;
      if (evt->state2 <= 0) {
         if (EVT.fatal) {
            CreatePositionedEvt(evt, EVTF_SLAY_FX3);
         } else {
            CreatePositionedEvt(evt, EVTF_DAMAGE_FX2);
         }
         evt->state++;
      }
      break;

   case 2:
      evt->state2++;
      if (evt->state2 >= 0x20) {
         evt->state++;
      }
      break;

   case 3:
      evt->state++;

   // fallthrough
   case 4:
   case 5:
      evt->state++;
      break;

   case 6:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 090
void Evtf090_DaggerStorm_FX2(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *dagger;
   SVECTOR svector;
   VECTOR vector1, vector2;
   s32 i;
   s32 a, b, c;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->state3 = 0x20;
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->state3 < 0) {
         evt_s0 = Evt_GetUnused();
         if (evt_s0 != NULL) {
            PerformAudioCommand(0x5e4);
            evt_s0->functionIndex = EVTF_DISPLAY_DAMAGE_2;
            evt_s0->x1.s.hi = evt->x1.s.hi;
            evt_s0->z1.s.hi = evt->z1.s.hi;
         }
         evt->state3 = 0x20;
         evt->state++;
      }

      if (evt->state3 % 8 == 0) {
         for (i = 0; i < 16; i++) {
            a = EVT.todo_x24 = rand() % 0x400;
            b = EVT.todo_x26 = rand() % 0x1000;
            c = EVT.todo_x28 = 0x200 + rand() % 0x200;

            dagger = Evt_GetUnused();
            if (dagger == NULL) {
               return;
            }
            dagger->functionIndex = EVTF_DAGGER_STORM_DAGGER;
            if (evt->state3 >= 25) {
               dagger->state2 = 0;
            } else {
               dagger->state2 = 2;
            }
            dagger->d.evtf091.todo_x28 = 0xd4;
            dagger->x2.n = evt->x1.n;
            dagger->y2.n = evt->y1.n + 0x80;
            dagger->z2.n = evt->z1.n;
            dagger->d.evtf091.todo_x24 = a;
            dagger->d.evtf091.todo_x26 = b;
            dagger->d.evtf091.todo_x28 = c;
            func_800A9E78(&svector, EVT.todo_x28, EVT.todo_x24, EVT.todo_x26);
            dagger->x1.n = evt->x1.n + svector.vx;
            dagger->y1.n = evt->y1.n + svector.vy;
            dagger->z1.n = evt->z1.n + svector.vz;
            vector1.vx = dagger->x2.n - dagger->x1.n;
            vector1.vy = dagger->y2.n - dagger->y1.n;
            vector1.vz = dagger->z2.n - dagger->z1.n;
            VectorNormalS(&vector1, &svector);
            dagger->x3.n = svector.vx * 0x60 >> 12;
            dagger->y3.n = svector.vy * 0x60 >> 12;
            dagger->z3.n = svector.vz * 0x60 >> 12;
            vector1.vx *= vector1.vx;
            vector1.vy *= vector1.vy;
            vector1.vz *= vector1.vz;
            vector2.vx = dagger->x3.n * dagger->x3.n;
            vector2.vy = dagger->y3.n * dagger->y3.n;
            vector2.vz = dagger->z3.n * dagger->z3.n;
            dagger->state3 = csqrt(vector1.vx + vector1.vy + vector1.vz) /
                             csqrt(vector2.vx + vector2.vy + vector2.vz);
            dagger->x3.n = (dagger->x2.n - dagger->x1.n) / dagger->state3;
            dagger->y3.n = (dagger->y2.n - dagger->y1.n) / dagger->state3;
            dagger->z3.n = (dagger->z2.n - dagger->z1.n) / dagger->state3;
            dagger->d.evtf091.positions[1].y = 0;
            dagger->d.evtf091.positions[1].x = 0;
            dagger->d.evtf091.positions[1].z = 0;
            func_800A9E78(&svector, EVT.todo_x28, EVT.todo_x24, EVT.todo_x26 + 0x40);
            dagger->d.evtf091.positions[3].y = evt->y1.n + svector.vy - dagger->y1.n;
            dagger->d.evtf091.positions[3].x = evt->x1.n + svector.vx - dagger->x1.n;
            dagger->d.evtf091.positions[3].z = evt->z1.n + svector.vz - dagger->z1.n;
            func_800A9E78(&svector, EVT.todo_x28 + 0xa0, EVT.todo_x24, EVT.todo_x26);
            dagger->d.evtf091.positions[0].y = evt->y1.n + svector.vy - dagger->y1.n;
            dagger->d.evtf091.positions[0].x = evt->x1.n + svector.vx - dagger->x1.n;
            dagger->d.evtf091.positions[0].z = evt->z1.n + svector.vz - dagger->z1.n;
            func_800A9E78(&svector, EVT.todo_x28 + 0xa0, EVT.todo_x24, EVT.todo_x26 + 0x40);
            dagger->d.evtf091.positions[2].y = evt->y1.n + svector.vy - dagger->y1.n;
            dagger->d.evtf091.positions[2].x = evt->x1.n + svector.vx - dagger->x1.n;
            dagger->d.evtf091.positions[2].z = evt->z1.n + svector.vz - dagger->z1.n;
         }
      }
      break;

   case 2:
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 3:
      if (EVT.fatal) {
         PerformAudioCommand(0x5e8);
         evt_s0 = Evt_GetUnused();
         if (evt_s0 != NULL) {
            evt_s0->functionIndex = EVTF_STRETCH_WARP_SPRITE;
            evt_s0->x1.n = evt->x1.n;
            evt_s0->z1.n = evt->z1.n;
            evt_s0->y1.n = evt->y1.n;
            evt->state3 = 0x20;
            evt->state++;
         }
      } else {
         evt->state++;
      }
      break;

   case 4:
      if (--evt->state3 < 0) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

void func_80082E48(EvtData *evt) {
   if (--evt->state3 != 0) {
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
   } else {
      evt->functionIndex = EVTF_NULL;
   }
}

#undef EVTF
#define EVTF 337
void Evtf337_DaggerStorm_BloodSplatter(EvtData *evt) {
   s32 elevation;

   switch (evt->state) {
   case 0:
      evt->state3 = 0x20;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->y2.n -= 4;

      elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      if (elevation > evt->y1.n) {
         evt->x2.n = 0;
         evt->z2.n = 0;
         evt->y1.n = elevation;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (--evt->state3 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 091
void Evtf091_DaggerStorm_Dagger(EvtData *evt) {
   EvtData *evt_a1;

   switch (evt->state) {
   case 0:
      evt_a1 = Evt_GetUnused();
      if (evt_a1 == NULL) {
         break;
      }
      evt_a1->functionIndex = EVTF_NOOP;
      EVT.sprite = evt_a1;
      evt_a1->d.sprite.gfxIdx = GFX_DAGGER;
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 2:
      evt->x1.n += evt->x3.n;
      evt->z1.n += evt->z3.n;
      evt->y1.n += evt->y3.n;

      if (abs(evt->x2.n - evt->x1.n) <= 0x80 && abs(evt->y2.n - evt->y1.n) <= 0x80 &&
          abs(evt->z2.n - evt->z1.n) <= 0x80) {

         evt_a1 = Evt_GetUnused();
         if (evt_a1 != NULL) {
            evt_a1->functionIndex = EVTF_BOUNCE_ZOOM;
            evt_a1->d.evtf024.soft = 1;
         }

         if (rand() % 8 == 0) {
            evt_a1 = Evt_GetUnused();
            if (evt_a1 != NULL) {
               evt_a1->functionIndex = EVTF_DAGGER_STORM_BLOOD_SPLATTER;
               evt_a1->x1.n = evt->x1.n;
               evt_a1->z1.n = evt->z1.n;
               evt_a1->y1.n = evt->y1.n;
               evt_a1->x2.n = (evt->x1.n - evt->x2.n) >> 2;
               evt_a1->z2.n = (evt->z1.n - evt->z2.n) >> 2;
               evt_a1->y2.n = (evt->y1.n - evt->y2.n) >> 1;
               evt_a1->d.sprite.gfxIdx = GFX_EXPLOSION_11;
               evt_a1->d.sprite.boxIdx = 5;
               evt_a1->d.sprite.clut = 3;
            }
         }

         evt_a1 = GetUnitSpriteAtPosition(evt->z2.s.hi, evt->x2.s.hi);
         EVT.targetSprite = evt_a1;

         evt->x2.n = evt->x1.n - evt->x2.n;
         evt->z2.n = evt->z1.n - evt->z2.n;
         evt->y2.n = evt->y1.n - evt->y2.n;

         evt->state2 = 2;
         evt->state++;
      }

      break;

   case 3:
      evt_a1 = EVT.targetSprite;
      evt->x1.n = evt_a1->x1.n + evt->x2.n;
      evt->z1.n = evt_a1->z1.n + evt->z2.n;
      evt->y1.n = evt_a1->y1.n + evt->y2.n;

      if (--evt->state2 > 0) {
         evt->x1.n -= evt->x3.n;
         evt->z1.n -= evt->z3.n;
         evt->y1.n -= evt->y3.n;
      }
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 4:
      evt_a1 = EVT.sprite;
      evt_a1->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }

   if (evt->functionIndex != EVTF_NULL) {
      evt_a1 = EVT.sprite;
      evt_a1->d.sprite.coords[0].x = EVT.positions[0].x + evt->x1.n;
      evt_a1->d.sprite.coords[0].y = EVT.positions[0].y + evt->y1.n;
      evt_a1->d.sprite.coords[0].z = EVT.positions[0].z + evt->z1.n;
      evt_a1->d.sprite.coords[1].x = EVT.positions[1].x + evt->x1.n;
      evt_a1->d.sprite.coords[1].y = EVT.positions[1].y + evt->y1.n;
      evt_a1->d.sprite.coords[1].z = EVT.positions[1].z + evt->z1.n;
      evt_a1->d.sprite.coords[2].x = EVT.positions[2].x + evt->x1.n;
      evt_a1->d.sprite.coords[2].y = EVT.positions[2].y + evt->y1.n;
      evt_a1->d.sprite.coords[2].z = EVT.positions[2].z + evt->z1.n;
      evt_a1->d.sprite.coords[3].x = EVT.positions[3].x + evt->x1.n;
      evt_a1->d.sprite.coords[3].y = EVT.positions[3].y + evt->y1.n;
      evt_a1->d.sprite.coords[3].z = EVT.positions[3].z + evt->z1.n;
      evt_a1->x1.n = evt->x1.n + (EVT.positions[2].x << 2);
      evt_a1->z1.n = evt->z1.n + (EVT.positions[2].z << 2);
      evt_a1->y1.n = evt->y1.n + (EVT.positions[2].y << 2);
      evt_a1->d.sprite.otOfs = -16;
      AddEvtPrim5(gGraphicsPtr->ot, evt_a1);
   }
}

void Evtf093_DaggerStorm_FX3(EvtData *evt) {
   evt->functionIndex = EVTF_DAGGER_STORM_FX2;
   evt->d.evtf090.fatal = 1;
}

void Evtf092_DaggerStorm_FX1(EvtData *evt) {
   evt->functionIndex = EVTF_NULL;
   gSignal3 = 1;
}

#undef EVTF
#define EVTF 082
void Evtf082_Fx_TBD(EvtData *evt) {
   EvtData *evt_s3;
   EvtData *sprite;
   SVECTOR vector;

   switch (evt->state) {
   case 0:
      evt_s3 = EVT.todo_x58;
      evt->state3 = evt_s3->functionIndex;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;

      EVT.radius = 0x40;
      EVT.theta1 = 0x400;
      EVT.theta2 = 0x400;
      EVT.theta3 = 0x400;
      EVT.theta4 = 0x400;

      evt->state2 = 0x100;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      evt_s3 = EVT.todo_x58;

      if (evt_s3->functionIndex != evt->state3) {
         evt->state++;
      } else {
         sprite->d.sprite.gfxIdx = GFX_EXPLOSION_1;
         sprite->d.sprite.boxIdx = 5;

         vector.vx = (EVT.radius * rcos(EVT.theta2) >> 12);
         vector.vz = (EVT.radius * rsin(EVT.theta2) >> 12);
         sprite->x1.n = evt_s3->x1.n + vector.vx;
         sprite->y1.n = evt_s3->y1.n + vector.vz;
         sprite->z1.n = evt_s3->z1.n + vector.vz;
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

         vector.vx = (EVT.radius * rcos(EVT.theta4) >> 12);
         vector.vz = (EVT.radius * rsin(EVT.theta4) >> 12);
         sprite->x1.n = evt_s3->x1.n + vector.vx;
         sprite->y1.n = evt_s3->y1.n - vector.vz;
         sprite->z1.n = evt_s3->z1.n + vector.vz;

         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         EVT.theta1 = (EVT.theta1 + 0x80) & 0xfff;
         EVT.theta2 = (EVT.theta2 + 0x80) & 0xfff;
         EVT.theta3 = (EVT.theta3 - 0x80) & 0xfff;
         EVT.theta4 = (EVT.theta4 - 0x80) & 0xfff;
      }
      break;

   case 2:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Evtf083_Fx_TBD(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_COLOR_15;
      evt->d.sprite.boxIdx = 5;
      evt->state++;

   // fallthrough
   case 1:
      evt->x2.n = (evt->x3.n - evt->x1.n) >> 2;
      evt->y2.n = (evt->y3.n - evt->y1.n) >> 2;
      evt->z2.n = (evt->z3.n - evt->z1.n) >> 2;
      evt->x1.n += evt->x2.n;
      evt->y1.n += evt->y2.n;
      evt->z1.n += evt->z2.n;

      evt->d.sprite.gfxIdx = GFX_EXPLOSION_1 + ((0x20 - evt->state2) >> 4);
      evt->d.sprite.boxIdx = 5 - (evt->state2 >> 4);
      evt->d.sprite.clut = 3 + (evt->state2 % 2);

      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }

   if (evt->functionIndex != EVTF_NULL) {
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
   }
}

void Evtf309_Explosion(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_EXPLOSION_1;
      evt->d.sprite.boxIdx = 5;
      if (evt->d.sprite.animData == NULL) {
         evt->d.sprite.animData = sExplosionAnimData_800ff520;
      } else {
         evt->d.sprite.animData = sExplosionAnimData_800ff554;
      }
      evt->state++;

   // fallthrough
   case 1:
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 321
void Evtf321_Fx_TBD(EvtData *evt) {
   EvtData *sprite;
   s32 i;
   s16 *p;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sExplosionAnimData_800ff520;
      sprite->d.sprite.clut = 3;
      sprite->d.sprite.boxIdx = 5;
      EVT.sprite = sprite;
      EVT.thetas[3] = ANGLE_90_DEGREES;
      EVT.thetas[2] = ANGLE_90_DEGREES;
      EVT.thetas[5] = ANGLE_180_DEGREES;
      EVT.thetas[4] = ANGLE_180_DEGREES;
      EVT.thetas[7] = ANGLE_270_DEGREES;
      EVT.thetas[6] = ANGLE_270_DEGREES;
      EVT.radius = 0;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);
      p = &EVT.thetas[0];
      for (i = 0; i < 8; i++) {
         sprite->z1.n = evt->z1.n + (EVT.radius * rsin(*p) >> 12);
         sprite->x1.n = evt->x1.n + (EVT.radius * rcos(*p) >> 12);
         sprite->y1.n = evt->y1.n;
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         p++;
      }
      p = &EVT.thetas[0];
      for (i = 0; i < 4; i++) {
         p[0] += 0x300;
         p[1] -= 0x300;
         p += 2;
      }
      EVT.radius += 24;
      if (sprite->d.sprite.animFinished) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}
