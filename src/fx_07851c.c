#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

static s16 sFlameAnimData_800ff5f8[20] = {
    0, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
    2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    1, GFX_NULL};

static s16 sFlameAnimData_800ff620[20] = {
    0, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
    2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    0, GFX_NULL};

static s16 sFlameAnimData_800ff648[20] = {
    7, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
    2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    1, GFX_NULL};

// Used by Evtf379_EvilStream_Rock
s16 gRockAnimData_800ff670[12] = {5, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3,
                                  2, GFX_ROCK_4, 2, GFX_NULL,   1, GFX_NULL};

static s16 sSmokeAnimData_800ff688[24] = {
    4, GFX_PUFF_1, 2, GFX_PUFF_2, 2, GFX_PUFF_3, 2, GFX_PUFF_4,  2, GFX_PUFF_5, 2, GFX_PUFF_6,
    2, GFX_PUFF_7, 2, GFX_PUFF_8, 2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   1, GFX_NULL};

///////////////////RDATA TODO/////////////////////
///////////////////RDATA TODO/////////////////////
static const Quad sQuad_80014b84 = {
    {-32, -32, 0, 0}, {32, -32, 0, 0}, {-32, 32, 0, 0}, {32, 32, 0, 0}};

static const s16 RDATA_TODO_80014ba4[8] = {GFX_SALAMANDER_N,  GFX_SALAMANDER_NE, GFX_SALAMANDER_E,
                                           GFX_SALAMANDER_SE, GFX_SALAMANDER_S,  GFX_SALAMANDER_SE,
                                           GFX_SALAMANDER_E,  GFX_SALAMANDER_NE};

static const s16 RDATA_TODO_80014bb4[8] = {GFX_SALAMANDER_S,  GFX_SALAMANDER_SE, GFX_SALAMANDER_E,
                                           GFX_SALAMANDER_NE, GFX_SALAMANDER_N,  GFX_SALAMANDER_NE,
                                           GFX_SALAMANDER_E,  GFX_SALAMANDER_SE};
///////////////////RDATA TODO/////////////////////
///////////////////RDATA TODO/////////////////////

#undef EVTF
#define EVTF 284
void Evtf284_Fx_TBD(EvtData *evt) {
   static s16 sparkleAnimData[14] = {7, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};

   s32 i;
   SVECTOR *p;
   Quad unused;

   switch (evt->state) {
   case 0:
      EVT.animData = sparkleAnimData;
      EVT.boxIdx = 7;
      evt->mem = 4 + rand() % 16;
      evt->x3.n = (evt->x2.n - evt->x1.n) / evt->mem;
      evt->z3.n = (evt->z2.n - evt->z1.n) / evt->mem;
      evt->y3.n = (evt->y2.n - evt->y1.n) / evt->mem;

      p = &EVT.quad[0];
      for (i = 0; i < 4; i++) {
         p->vx = gQuad_800fe53c[i].vx;
         p->vy = gQuad_800fe53c[i].vy;
         p->vz = gQuad_800fe53c[i].vz;
         p++;
      }

      evt->state2 = 3;
      evt->state++;

   // fallthrough
   case 1:
      EVT.clut = 3 + evt->mem % 3;
      EVT.quad[0].vx = -evt->state2;
      EVT.quad[0].vy = -evt->state2;
      EVT.quad[1].vx = evt->state2;
      EVT.quad[1].vy = -evt->state2;
      EVT.quad[2].vx = -evt->state2;
      EVT.quad[2].vy = evt->state2;
      EVT.quad[3].vx = evt->state2;
      EVT.quad[3].vy = evt->state2;

      p = &EVT.quad[0];
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i].vx = p->vx;
         gQuad_800fe63c[i].vy = p->vy;
         gQuad_800fe63c[i].vz = 0;
         p++;
      }

      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      evt->x1.n += (evt->x2.n - evt->x1.n) >> 3;
      evt->z1.n += (evt->z2.n - evt->z1.n) >> 3;
      evt->y1.n += (evt->y2.n - evt->y1.n) >> 3;

      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 286
void Evtf286_Fx_TBD(EvtData *evt) {
   s32 radius;

   switch (evt->state) {
   case 0:
      evt->state2 = rand() % ANGLE_360_DEGREES;
      evt->mem = 0xc0 + rand() % 0x40;
      evt->d.sprite.gfxIdx = GFX_COLOR_15;
      evt->state3 = 8;
      evt->d.sprite.semiTrans = 2;
      evt->state++;

   // fallthrough
   case 1:
      radius = evt->mem;
      evt->d.sprite.coords[0].x = evt->x2.n + (radius * rcos(evt->state2) >> 12);
      evt->d.sprite.coords[0].z = evt->z2.n + (radius * rsin(evt->state2) >> 12);
      evt->d.sprite.coords[1].x = evt->x2.n + (radius * rcos(evt->state2 + 0x10) >> 12);
      evt->d.sprite.coords[1].z = evt->z2.n + (radius * rsin(evt->state2 + 0x10) >> 12);

      radius = evt->mem + ((0 - evt->mem) >> 2);
      evt->d.sprite.coords[2].x = evt->x2.n + (radius * rcos(evt->state2) >> 12);
      evt->d.sprite.coords[2].z = evt->z2.n + (radius * rsin(evt->state2) >> 12);
      evt->d.sprite.coords[3].x = evt->x2.n + (radius * rcos(evt->state2 + 0x10) >> 12);
      evt->d.sprite.coords[3].z = evt->z2.n + (radius * rsin(evt->state2 + 0x10) >> 12);

      evt->d.sprite.coords[0].y = evt->d.sprite.coords[1].y = evt->d.sprite.coords[2].y =
          evt->d.sprite.coords[3].y = evt->y2.n;

      evt->x1.n = evt->d.sprite.coords[0].x;
      evt->z1.n = evt->d.sprite.coords[0].z;
      evt->y1.n = evt->d.sprite.coords[0].y;
      evt->d.sprite.otOfs = -8;
      AddEvtPrim5(gGraphicsPtr->ot, evt);

      evt->mem += (0 - evt->mem) >> 3;
      if (evt->mem <= 4) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 285
void Evtf285_CastingFx(EvtData *evt) {
   EvtData *ray;
   s32 i;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->y1.n += 0x80;
      evt->state++;
      evt->state2 = 4;
      evt->mem = 0xc0;
      PerformAudioCommand(0x5e6);

   // fallthrough
   case 1:
      if (--evt->state2 <= 0) {
         for (i = 0; i < 3; i++) {
            ray = Evt_GetUnused();
            ray->functionIndex = EVTF_INWARD_RAY;
            ray->x2.n = evt->x1.n;
            ray->z2.n = evt->z1.n;
            ray->y2.n = evt->y1.n;
         }
         evt->state2 = (rand() >> 2) % 3;
      }
      if (gSignal3 == 1) {
         evt->mem = 0x20;
         evt->state++;
      }
      break;

   case 2:
      if (--evt->mem <= 0) {
         PerformAudioCommand(0x5e7);
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 287
void Evtf287_Fx_TBD(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *castingFx;

   switch (evt->state) {
   case 0:
      unitSprite = SnapToUnit(evt);
      evt->y1.n += 0x80;
      EVT.unitSprite = unitSprite;
      evt->state3 = 0;
      evt->state++;

      castingFx = Evt_GetUnused();
      castingFx->functionIndex = EVTF_CASTING_FX;
      castingFx->mem = 0x30;
      castingFx->x1.n = evt->x1.n;
      castingFx->z1.n = evt->z1.n;
      castingFx->y1.n = evt->y1.n;

   // fallthrough
   case 1:
      if (gSignal3 == 1) {
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 283
void Evtf283_DarkHurricane_Cloud(EvtData *evt) {
   EvtData *parent;

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_PUFF_1;
      EVT.boxIdx = 4;
      EVT.animData = sSmokeAnimData_800ff688;
      evt->mem = 0x100;
      evt->state2 = rand() % ANGLE_360_DEGREES;
      evt->state++;

   // fallthrough
   case 1:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n + (evt->mem * rcos(evt->state2) >> 12);
      evt->z1.n = parent->z1.n + (evt->mem * rsin(evt->state2) >> 12);
      evt->y1.n = parent->y1.n + 0x80 - (evt->mem / 2);
      evt->mem -= 0x10;
      evt->state2 += 0x40;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->mem <= 0x20) {
         evt->functionIndex = EVTF_NULL;
      }
      if (parent->state == 99) {
         evt->state++;
      }
      break;

   case 2:
      parent = EVT.parent;
      evt->x3.n = parent->x1.n + (evt->mem * rcos(evt->state2) >> 12);
      evt->z3.n = parent->z1.n + (evt->mem * rsin(evt->state2) >> 12);
      evt->y3.n = parent->y1.n + 0x80 - (evt->mem / 2);
      evt->x2.n = evt->x3.n - evt->x1.n;
      evt->z2.n = evt->z3.n - evt->z1.n;
      evt->y2.n = evt->y3.n - evt->y1.n;
      evt->mem = 0;
      evt->state++;

   // fallthrough
   case 3:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (++evt->mem >= 0x40) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 281
void Evtf281_282_DarkHurricane_FX2_FX3(EvtData *evt) {
   EvtData *fxSprite;
   EvtData *targetSprite;
   EvtData *evt_v1;
   s16 theta;
   s32 i;
   Quad quad;

   switch (evt->state) {
   case 0:
      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_NOOP;
      EVT.sprite = evt_v1;

      targetSprite = SnapToUnit(evt);
      EVT.targetSprite = targetSprite;

      evt->state3 = 0;
      evt->y1.n = 0x800;
      evt->y3.n = -8;
      EVT.theta = 0;
      evt->state++;

   // fallthrough
   case 1:
      GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      targetSprite = EVT.targetSprite;
      targetSprite->d.sprite.hidden = 1;
      fxSprite = EVT.sprite;
      CopyEvtData(targetSprite, fxSprite);
      fxSprite->functionIndex = EVTF_NOOP;
      fxSprite->d.sprite.hidden = 0;
      evt->state3++;

      switch (evt->state2) {
      case 0:
         evt->state2++;

      // fallthrough
      case 1:
         evt->y1.n += evt->y2.n;
         evt->y2.n += evt->y3.n;
         EVT.theta += (evt->y2.n * 8);
         theta = EVT.theta;

         if (GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) > evt->y1.n) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_BOUNCE_ZOOM;
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_DISPLAY_DAMAGE_2;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->z1.n = evt->z1.n;
            evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
            evt->y2.n = -(evt->y2.n / 2);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 2:
         evt->y1.n += evt->y2.n;
         evt->y2.n += evt->y3.n;
         EVT.theta += (0 - EVT.theta) >> 2;
         theta = EVT.theta;

         if (GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) > evt->y1.n) {
            evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
            evt->y2.n = 0;
            evt->y3.n = 0;
            fxSprite->functionIndex = EVTF_NULL;
            targetSprite->d.sprite.hidden = 0;
            evt->state3 = 0;

            if (evt->functionIndex == EVTF_DARK_HURRICANE_FX2) {
               evt->state += 1;
            } else if (evt->functionIndex == EVTF_DARK_HURRICANE_FX3) {
               evt->state += 2;
            }
         }

         break;
      }

      fxSprite->y1.n = evt->y1.n;
      fxSprite->x1.n = evt->x1.n;
      fxSprite->z1.n = evt->z1.n;

      for (i = 0; i < 4; i++) {
         quad[i] = gQuad_800fe53c[i];
      }

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx = (quad[i].vx * rcos(theta) - (quad[i].vy + 8) * rsin(theta)) >> 12;
         gQuad_800fe53c[i].vy = (quad[i].vx * rsin(theta) + (quad[i].vy + 8) * rcos(theta)) >> 12;
         gQuad_800fe53c[i].vz = 0;
      }

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i] = quad[i];
      }

      break;

   case 2:
      if (++evt->state3 >= 15) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;

   case 3:
      if (++evt->state3 >= 10) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 4:
      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_STRETCH_WARP_SPRITE;
      evt_v1->x1.n = evt->x1.n;
      evt_v1->z1.n = evt->z1.n;
      evt_v1->y1.n = evt->y1.n;
      evt->state++;

   // fallthrough
   case 5:
      if (++evt->state3 >= 15) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 280
void Evtf280_DarkHurricane_Target(EvtData *evt) {
   EvtData *fxSprite; // copy of unit sprite to toss around
   EvtData *targetSprite;
   EvtData *evt_s1;
   EvtData *vortex;
   s32 i;
   s16 theta;
   Quad quad;

   switch (evt->state) {
   case 0:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      EVT.sprite = evt_s1;
      evt_s1 = EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->state3 = rand() % 0x20;
      evt->x1.n = evt_s1->x1.n;
      evt->z1.n = evt_s1->z1.n;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->y3.n = -8;
      EVT.theta = 0;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.unitSprite;
      targetSprite->d.sprite.hidden = 1;
      fxSprite = EVT.sprite;
      CopyEvtData(targetSprite, fxSprite);
      fxSprite->functionIndex = EVTF_NOOP;
      fxSprite->d.sprite.hidden = 0;
      evt->state3++;
      EVT.theta += evt->y2.n * 8;
      theta = EVT.theta;

      switch (evt->state2) {
      case 0:
         vortex = EVT.parent;
         if (vortex->state == 99) {
            evt->y2.n = -0x80;
            evt->state2++;
            evt->state3 = 0;
         } else {
            evt->y1.n += 8;
            if (evt->y1.n >= 0x400) {
               evt->y1.n = 0x400;
            }
            evt->x1.n = vortex->x1.n + (0x80 * rcos(evt->state3 * 0x80) >> 12);
            evt->z1.n = vortex->z1.n + (0x80 * rsin(evt->state3 * 0x80) >> 12);
            evt->y2.n = 0x1c0;
         }
         break;

      case 1:
         evt->y1.n += evt->y2.n;
         evt->y2.n += evt->y3.n;
         fxSprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
         break;
      }

      fxSprite->y1.n = evt->y1.n;
      fxSprite->x1.n = evt->x1.n;
      fxSprite->z1.n = evt->z1.n;

      for (i = 0; i < 4; i++) {
         quad[i] = gQuad_800fe53c[i];
      }

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx =
             (quad[i].vx * rcos(theta) - (quad[i].vy + 8) * rsin(theta)) >> 1 >> 12;
         gQuad_800fe53c[i].vy =
             (quad[i].vx * rsin(theta) + (quad[i].vy + 8) * rcos(theta)) >> 1 >> 12;
         gQuad_800fe53c[i].vz = 0;
      }

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i] = quad[i];
      }

      break;
   }
}

void Evtf_Unk_80089298(EvtData *evt) {
   EvtData *sprite;
   POLY_FT4 *poly;
   s32 randomAngle;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->d.sprite.gfxIdx = GFX_SALAMANDER_S;
      evt->d.sprite.boxIdx = 4;
      evt->d.sprite.clut = 3;
      evt->state2 = 0x80;
      evt->state++;

   // fallthrough
   case 1:
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      evt->x2.n = (poly->x0 + poly->x1) / 2;
      evt->y2.n = (poly->y0 + poly->y2) / 2;

      sprite = Evt_GetUnused();
      sprite->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES;
      sprite->d.sprite.boxIdx = 4;
      sprite->d.sprite.clut = 4;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      sprite->d.sprite.semiTrans = 1;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      poly->x0 = poly->x1 = evt->x2.n;
      poly->y0 = poly->y1 = evt->y2.n;

      randomAngle = (rand() >> 2) % ANGLE_360_DEGREES;
      poly->x2 = poly->x0 + (0x100 * rcos(randomAngle) >> 12);
      poly->y2 = poly->y0 + (0x100 * rsin(randomAngle) >> 12);
      poly->x3 = poly->x0 + (0x100 * rcos(randomAngle + 0x20) >> 12);
      poly->y3 = poly->y0 + (0x100 * rsin(randomAngle + 0x20) >> 12);

      if (--evt->state2 <= 0) {
         evt->state++;
      }

      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 389
void Evtf389_DarkHurricane_Vortex(EvtData *evt) {
   EvtData *parent;
   EvtData *sprite;

   parent = EVT.parent;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_EXPLOSION_1;
      sprite->d.sprite.boxIdx = 5;
      EVT.sprite = sprite;

      evt->x2.n = evt->x1.n;
      evt->z2.n = evt->z1.n;
      evt->y2.n = evt->y1.n;
      evt->state2 = 0;
      evt->state++;

   // fallthrough
   case 1:
      evt->state2 = parent->state2;
      evt->x1.n = evt->x2.n + (0x80 * rcos(evt->state3 * 0x10) >> 12);
      evt->z1.n = evt->z2.n + (0x80 * rcos(evt->state3 * 0x10) >> 12);
      evt->y1.n = evt->y2.n;
      evt->state3++;

      sprite = EVT.sprite;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      EVT.theta += 0x40;

      if (parent->state == 99) {
         evt->state3 = 0;
         evt->state = 99;
      }
      break;

   case 99:
      if (++evt->state3 >= 2) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 390
void Evtf390_DarkHurricane_VortexLayer(EvtData *evt) {
   EvtData *parent;
   EvtData *sprite;
   s32 i;
   s32 startingTheta;
   s32 parentTheta;
   s32 radius;
   s32 parentRadius;
   POLY_FT4 *poly;
   s32 increment;

   parent = EVT.parent;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_EXPLOSION_1;
      sprite->d.sprite.boxIdx = 5;
      EVT.sprite = sprite;

      EVT.height = 0;
      EVT.radius2 = 0;
      EVT.theta2 = (rand() >> 2) % ANGLE_360_DEGREES;
      EVT.maxRadius = EVT.radius;
      EVT.radius = 0;
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:

      switch (evt->state2) {
      case 0:
         EVT.height += 2;
         EVT.radius2 = 0x10 + (0x30 * rcos(evt->state3 * 0x40) >> 12);
         EVT.radius += 8;
         if (EVT.radius > EVT.maxRadius) {
            EVT.radius = EVT.maxRadius;
         }
         if (EVT.height >= 0x60) {
            EVT.height = 0x60;
         }
         if (parent->state2 == 1) {
            evt->state2++;
            EVT.radius = EVT.maxRadius;
         }
         break;

      case 1:
         evt->state3++;
         EVT.radius2 = 0x10 + (0x30 * rcos(evt->state3 * 0x40) >> 12);
         EVT.height = 0x60 + (0x20 * rsin(evt->state3 * 0x20) >> 12);
         if (parent->state2 == 2) {
            evt->state2++;
         }
         break;

      case 2:
         EVT.height += 2;
         EVT.radius += (0 - EVT.radius) >> 3;
         EVT.radius2 += (0 - EVT.radius2) >> 2;
         if (parent->state2 == 3) {
            evt->state2++;
         }
         break;
      }

      evt->x1.n = parent->x1.n + (EVT.radius2 * rcos(EVT.theta2) >> 12);
      evt->z1.n = parent->z1.n + (EVT.radius2 * rsin(EVT.theta2) >> 12);
      evt->y1.n = parent->y1.n + EVT.height;

      EVT.theta2 += 0x20;

      sprite = EVT.sprite;
      startingTheta = EVT.theta = parent->d.evtf389.theta;
      parentTheta = startingTheta;
      radius = EVT.radius;
      parentRadius = parent->d.evtf389.radius;

      sprite->d.sprite.gfxIdx = GFX_TILED_LINES;
      sprite->d.sprite.clut = 5;
      sprite->d.sprite.semiTrans = 1;

      sprite->d.sprite.coords[0].x = evt->x1.n + (radius * rcos(startingTheta) >> 12);
      sprite->d.sprite.coords[0].z = evt->z1.n + (radius * rsin(startingTheta) >> 12);
      // Connect to previous layer (parent)
      sprite->d.sprite.coords[2].x = parent->x1.n + (parentRadius * rcos(parentTheta) >> 12);
      sprite->d.sprite.coords[2].z = parent->z1.n + (parentRadius * rsin(parentTheta) >> 12);
      sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = evt->y1.n;
      sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = parent->y1.n;

      increment = ANGLE_360_DEGREES / 16;

      for (i = 0; i < 16; i++) {
         sprite->d.sprite.coords[1].x =
             evt->x1.n + (radius * rcos(startingTheta + increment * (i + 1)) >> 12);
         sprite->d.sprite.coords[1].z =
             evt->z1.n + (radius * rsin(startingTheta + increment * (i + 1)) >> 12);
         sprite->d.sprite.coords[3].x =
             parent->x1.n + (parentRadius * rcos(parentTheta + increment * (i + 1)) >> 12);
         sprite->d.sprite.coords[3].z =
             parent->z1.n + (parentRadius * rsin(parentTheta + increment * (i + 1)) >> 12);

         sprite->x1.n = sprite->d.sprite.coords[3].x;
         sprite->z1.n = sprite->d.sprite.coords[3].z;
         sprite->y1.n = sprite->d.sprite.coords[3].y;

         AddEvtPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, 0x64, 0x64, 0x64);

         sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[1].x;
         sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z;
         sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[3].x;
         sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[3].z;
      }

      if (parent->state == 99) {
         evt->state = 99;
      }
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 388
void Evtf388_DarkHurricane_FX1(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_a0;
   BVectorXZ *p;
   s32 i;

   switch (evt->state) {
   case 0:
      evt->z1.s.hi = gTargetZ;
      evt->x1.s.hi = gTargetX;
      evt->y1.n = GetTerrainElevation(gTargetZ, gTargetX);

      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_DARK_HURRICANE_VORTEX;
      evt_s2->x1.n = evt->x1.n;
      evt_s2->z1.n = evt->z1.n;
      evt_s2->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt_s2->d.evtf389.parent = evt;
      EVT.vortex = evt_s2;

      p = (BVectorXZ *)gTargetCoords;
      evt_a0 = GetUnitSpriteAtPosition(p->z, p->x);
      evt->x2.n = evt_a0->x1.n;
      evt->z2.n = evt_a0->z1.n;
      evt->y2.n = evt_a0->y1.n + 0x80;
      while (p->x != 0xff) {
         evt_a0 = Evt_GetUnused();
         evt_a0->functionIndex = EVTF_DARK_HURRICANE_TARGET;
         evt_a0->z1.s.hi = p->z;
         evt_a0->x1.s.hi = p->x;
         evt_a0->d.evtf280.parent = evt_s2;
         p++;
      }

      for (i = 0; i < 8; i++) {
         evt_a0 = Evt_GetUnused();
         evt_a0->functionIndex = EVTF_DARK_HURRICANE_VORTEX_LAYER;
         evt_a0->d.evtf390.parent = evt_s2;
         evt_a0->d.evtf390.radius = (i + 1) * 0x40;
         evt_a0->d.evtf390.unused_0x38 = i * 0x20;
         evt_s2 = evt_a0;
      }

      evt->state++;
      evt->state3 = 0x100;
      evt->mem = 0;
      evt->state2 = 0;
      break;

   case 1:
      gCameraZoom.vz += (400 - gCameraZoom.vz) >> 3;
      gCameraRotation.vy += 0x10;
      PanCamera(evt->x2.n, evt->y2.n, evt->z2.n, 2);

      if (--evt->mem <= 0) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_DARK_HURRICANE_CLOUD;
         evt_s2->d.evtf283.parent = EVT.vortex;
         evt->mem = rand() % 3;
      }

      evt->state3--;

      switch (evt->state2) {
      case 0:
         if (evt->state3 <= 0xc0) {
            evt->state2++;
         }
         break;
      case 1:
         if (evt->state3 <= 0x20) {
            evt->state2++;
         }
         break;
      case 2:
         if (evt->state3 <= 8) {
            evt->state2++;
         }
         break;
      }

      if (evt->state3 <= 0) {
         evt->state3 = 0;
         evt->state = 99;
      }
      break;

   case 99:
      if (++evt->state3 >= 2) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 392
void Evtf392_Fx_TBD(EvtData *evt) {
   EvtData *evt_s1;
   EvtData *evt1;
   EvtData *evt2;
   EvtData *evt3;
   EvtData *evt4;
   s32 radius;
   s8 unused[24];

   // Not enough context to identify which evtf to use for this.
   evt_s1 = EVT.todo_x5c;

   if (evt_s1->d.evtfUnkUsedBy392.todo_x28 == 99) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   evt1 = EVT.todo_x24;
   evt2 = EVT.todo_x28;
   evt3 = EVT.todo_x2c;
   evt4 = EVT.todo_x30;

   switch (evt_s1->d.evtfUnkUsedBy392.todo_x24) {
   case 0:
      evt->x1.n = evt->x3.n * 12 - 0x30;
      evt->y1.n = evt->y3.n * 12 - 0x30;
      if (evt->y1.n == 0) {
         evt->state3 = 0x200 + (8 - evt->x1.n) * 0x71;
      } else if (evt->y1.n == 8) {
         evt->state3 = -0x200 - (8 - evt->x1.n) * 0x71;
      }
      if (evt->x1.n == 0) {
         evt->state3 = evt->y1.n * 0x71 + 0x600;
      } else if (evt->x1.n == 8) {
         evt->state3 = -0x188;
      }
      break;

   case 1:
      if (evt1 != NULL && evt2 != NULL) {
         evt->y1.n += ((evt1->y1.n + evt2->y1.n) / 2 - evt->y1.n) >> 2;
      }
      if (evt3 != NULL && evt4 != NULL) {
         evt->x1.n += ((evt3->x1.n + evt4->x1.n) / 2 - evt->x1.n) >> 2;
      }
      if (evt3 == NULL || evt4 == NULL || evt1 == NULL || evt2 == NULL) {
         evt->x1.n += 4 * rcos(0x800 * rsin(evt_s1->d.evtfUnkUsedBy392.theta) >> 12) >> 12;
         evt->y1.n += 4 * rsin(0x800 * rsin(evt_s1->d.evtfUnkUsedBy392.theta) >> 12) >> 12;
      }
      break;

   case 2:
      if (evt->y3.n == 0 || evt->y3.n == 8) {
         radius = 0;
      } else {
         radius = 0x100 * rsin(evt->y3.n * 0x100) >> 12;
      }
      evt->x1.n = -(radius * rcos(evt->x3.n * 0x200) >> 12);
      evt->z1.n = -(radius * rsin(evt->x3.n * 0x200) >> 12);
      evt->y1.n = (8 - evt->y3.n) * 0x40;
      break;
   }
}

#undef EVTF
#define EVTF Unk8008a364
void Evtf_Unk_8008a364(EvtData *evt) {
   EvtData *sprite;
   POLY_FT4 *poly;
   s32 clipX, clipY;
   s32 swap;
   u8 unused[32];

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n + 0x200;
      EVT.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_LARGE_RED_CIRCLE;
      sprite->d.sprite.boxIdx = 3;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      clipX = gGraphicsPtr->drawEnv.clip.x;
      clipY = gGraphicsPtr->drawEnv.clip.y;
      gCameraRotation.vy += 1;

      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      poly->u0 = poly->x2;
      poly->v0 = poly->y2;
      poly->u1 = poly->x3;
      poly->v1 = poly->y3;
      poly->u2 = poly->x2 + 0xe0;
      poly->v2 = 0xff;
      poly->u3 = poly->x3 + 0x20;
      poly->v3 = 0xff;

      swap = poly->x0;
      poly->x0 = poly->x2;
      poly->x2 = swap;
      swap = poly->x1;
      poly->x1 = poly->x3;
      poly->x3 = swap;
      swap = poly->y0;
      poly->y0 = poly->y2;
      poly->y2 = swap;
      swap = poly->y1;
      poly->y1 = poly->y3;
      poly->y3 = swap;

      poly->tpage = GetTPage(2, 0, clipX, clipY);
      break;
   }
}
