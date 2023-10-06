#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"
#include "state.h"
#include "units.h"
#include "audio.h"

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
      evt->state2 = rand() % DEG(360);
      evt->mem = CV(0.75) + rand() % CV(0.25);
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
      evt->y1.n += CV(0.5);
      evt->state++;
      evt->state2 = 4;
      evt->mem = 0xc0;
      PerformAudioCommand(AUDIO_CMD_PLAY_SFX(230));

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
         PerformAudioCommand(AUDIO_CMD_PLAY_SFX(231));
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
      evt->y1.n += CV(0.5);
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
      evt->state2 = rand() % DEG(360);
      evt->state++;

   // fallthrough
   case 1:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n + (evt->mem * rcos(evt->state2) >> 12);
      evt->z1.n = parent->z1.n + (evt->mem * rsin(evt->state2) >> 12);
      evt->y1.n = parent->y1.n + CV(0.5) - (evt->mem / 2);
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
      evt->y3.n = parent->y1.n + CV(0.5) - (evt->mem / 2);
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
      evt->y1.n = CV(8.0);
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
            evt->y2.n = CV(-0.5);
            evt->state2++;
            evt->state3 = 0;
         } else {
            evt->y1.n += CV(0.03125);
            if (evt->y1.n >= CV(4.0)) {
               evt->y1.n = CV(4.0);
            }
            evt->x1.n = vortex->x1.n + (CV(0.5) * rcos(evt->state3 * 0x80) >> 12);
            evt->z1.n = vortex->z1.n + (CV(0.5) * rsin(evt->state3 * 0x80) >> 12);
            evt->y2.n = CV(1.75);
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
      evt->d.sprite.clut = CLUT_REDS;
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
      sprite->d.sprite.clut = CLUT_BLUES;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      sprite->d.sprite.semiTrans = 1;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      poly->x0 = poly->x1 = evt->x2.n;
      poly->y0 = poly->y1 = evt->y2.n;

      randomAngle = (rand() >> 2) % DEG(360);
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
      EVT.theta2 = (rand() >> 2) % DEG(360);
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
         if (EVT.height >= CV(0.375)) {
            EVT.height = CV(0.375);
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

      EVT.theta2 += DEG(2.8125);

      sprite = EVT.sprite;
      startingTheta = EVT.theta = parent->d.evtf389.theta;
      parentTheta = startingTheta;
      radius = EVT.radius;
      parentRadius = parent->d.evtf389.radius;

      sprite->d.sprite.gfxIdx = GFX_TILED_LINES;
      sprite->d.sprite.clut = CLUT_GRAYS;
      sprite->d.sprite.semiTrans = 1;

      sprite->d.sprite.coords[0].x = evt->x1.n + (radius * rcos(startingTheta) >> 12);
      sprite->d.sprite.coords[0].z = evt->z1.n + (radius * rsin(startingTheta) >> 12);
      // Connect to previous layer (parent)
      sprite->d.sprite.coords[2].x = parent->x1.n + (parentRadius * rcos(parentTheta) >> 12);
      sprite->d.sprite.coords[2].z = parent->z1.n + (parentRadius * rsin(parentTheta) >> 12);
      sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = evt->y1.n;
      sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = parent->y1.n;

      increment = DEG(360) / 16;

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
      evt->y2.n = evt_a0->y1.n + CV(0.5);
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
      sprite->y1.n = evt->y1.n + CV(2.0);
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

#undef EVTF
#define EVTF 395
void Evtf395_DynamoHum_ElectricOrb(EvtData *evt) {
   extern s16 gExplosionAnimData_800ff3dc[26];
   static u8 cluts[5] = {3, 4, 5, 8, 9};

   EvtData *parent;
   EvtData *ray;
   s32 halfSize;
   s32 i;

   parent = EVT.parent;

   halfSize = ((EVT.size - (0xa0 * rsin(EVT.theta) >> 12)) >> 5) + 4;
   gQuad_800fe63c[0].vx = gQuad_800fe63c[2].vx = -halfSize;
   gQuad_800fe63c[1].vx = gQuad_800fe63c[3].vx = halfSize;
   gQuad_800fe63c[0].vy = gQuad_800fe63c[1].vy = -halfSize;
   gQuad_800fe63c[2].vy = gQuad_800fe63c[3].vy = halfSize;
   gQuad_800fe63c[2].vz = gQuad_800fe63c[3].vz = gQuad_800fe63c[0].vz = gQuad_800fe63c[1].vz = 0;

   evt->mem++;
   evt->mem %= 5;
   EVT.clut = cluts[evt->mem];

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_EXPLOSION_1;
      EVT.boxIdx = 7;
      EVT.size = 0x100;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n = evt->x2.n + (evt->state3 * rcos(evt->state2) >> 12);
      evt->z1.n = evt->z2.n + (evt->state3 * rsin(evt->state2) >> 12);
      evt->y1.n = evt->y2.n;
      evt->state2 += 0x20;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      evt->state3 = EVT.size + (0xa0 * rsin(EVT.theta) >> 12);
      EVT.theta += 0x80;
      EVT.size -= 1;
      if (evt->state3 <= 8) {
         EVT.animData = gExplosionAnimData_800ff3dc;
         evt->state = 2;
         evt->state3 = 0;
      }
      for (i = 0; i < 3; i++) {
         ray = CreatePositionedEvt(evt, EVTF_OUTWARD_RAY);
         ray->x2.n = evt->x1.n;
         ray->z2.n = evt->z1.n;
         ray->y2.n = evt->y1.n;
      }
      if (parent->state == 99) {
         evt->state = 99;
      }
      break;

   case 2:
      if (parent->state == 99) {
         evt->state = 99;
         break;
      }
      if (EVT.animFinished) {
         UpdateEvtAnimation(evt);
         AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      }
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 396
void Evtf396_DynamoHum_OrbElectricity(EvtData *evt) {
   extern s16 gLightningAnimData_800ff468[20];

   EvtData *link1;
   EvtData *link2;
   EvtData *electricity;
   EvtData **p;
   s32 i;
   s32 length;
   s32 dx, dy, dz;
   s16 sVar5;
   s32 rnd;
   VECTOR vector;
   SVECTOR normalized;

   switch (evt->state) {
   case 0:
      link1 = EVT.links[0];
      link2 = EVT.links[1];

      if (evt->state2 != 0) {
         for (i = 0, p = &EVT.links[0]; i < 2; i++) {
            electricity = Evt_GetUnused();
            electricity->functionIndex = EVTF_DYNAMO_HUM_ORB_ELECTRICITY;
            electricity->d.evtf396.links[0] = *p;
            electricity->d.evtf396.links[1] = evt;
            if (evt->state2 == 1) {
               electricity->state3 = 1;
            } else {
               electricity->state2 = evt->state2 - 1;
            }
            electricity->x3.n = evt->x3.n;
            p++;
         }
      }

      EVT.gfxIdx = GFX_LIGHTNING_1;
      EVT.clut = CLUT_REDS;

      if (evt->state3 != 0) {
         EVT.animData = gLightningAnimData_800ff468;
         evt->state3 = 0;
      }

      dx = (link1->x1.n - link2->x1.n);
      dz = (link1->z1.n - link2->z1.n);
      dy = (link1->y1.n - link2->y1.n);
      i = SquareRoot0(dx * dx + dy * dy + dz * dz);
      EVT.length = i;
      EVT.todo_x2a = evt->x3.n;

      if (evt->x3.n != 0) {
         evt->x3.n = 0;
         evt->z3.n = 0;
         evt->y3.n = -1;
      } else {
         evt->x3.n = 0;
         evt->z3.n = 0;
         evt->y3.n = 1;
      }

      evt->state++;

   // fallthrough
   case 1:
      link1 = EVT.links[0];
      link2 = EVT.links[1];

      if (link1->state == 99 || link2->state == 99) {
         evt->state = 99;
         break;
      }

      evt->x1.n = (link1->x1.n + link2->x1.n) / 2;
      evt->z1.n = (link1->z1.n + link2->z1.n) / 2;
      evt->y1.n = (link1->y1.n + link2->y1.n) / 2;
      evt->x1.n += rand() % 0x20 - 0x10;
      evt->z1.n += rand() % 0x20 - 0x10;
      evt->y1.n += rand() % 0x20 - 0x10;

      dx = (link1->x1.n - link2->x1.n);
      dz = (link1->z1.n - link2->z1.n);
      dy = (link1->y1.n - link2->y1.n);
      length = SquareRoot0(dx * dx + dy * dy + dz * dz);

      if (length < EVT.length) {
         sVar5 = (EVT.length - length) / 2;

         switch (EVT.todo_x2a) {
         case 0:
            evt->y1.n += sVar5 * evt->y3.n;
            evt->x1.n += sVar5 * evt->x3.n;
            evt->z1.n += sVar5 * evt->z3.n;
            break;

         case 1:
            evt->y1.n += sVar5 * evt->y3.n;
            evt->x1.n += sVar5 * evt->x3.n;
            evt->z1.n += sVar5 * evt->z3.n;
            break;

         case 2:
         case 3:
            sVar5 = (EVT.length - length) / 4;
            vector.vx = link2->x1.n - link1->x1.n;
            vector.vz = link2->z1.n - link1->z1.n;
            vector.vy = 0;
            VectorNormalS(&vector, &normalized);

            if (EVT.todo_x2a == 2) {
               evt->x3.n = -(normalized.vz >> 11);
               evt->z3.n = normalized.vx >> 11;
               evt->y3.n = 0;
            } else {
               evt->x3.n = normalized.vz >> 11;
               evt->z3.n = -(normalized.vx >> 11);
               evt->y3.n = 0;
            }

            evt->y1.n += sVar5 * evt->y3.n;
            evt->x1.n += sVar5 * evt->x3.n;
            evt->z1.n += sVar5 * evt->z3.n;
            break;
         }
      }

      if (EVT.animData != NULL) {
         UpdateEvtAnimation(evt);
         p = &EVT.links[0];
         for (i = 0; i < ARRAY_COUNT(EVT.links); i++) {
            link1 = *p;
            EVT.coords[0].x = EVT.coords[1].x = link1->x1.n;
            EVT.coords[2].x = EVT.coords[3].x = evt->x1.n;
            EVT.coords[0].z = EVT.coords[1].z = link1->z1.n;
            EVT.coords[2].z = EVT.coords[3].z = evt->z1.n;
            EVT.coords[1].y = link1->y1.n + CV(0.125);
            EVT.coords[0].y = link1->y1.n - CV(0.125);
            EVT.coords[3].y = evt->y1.n + CV(0.125);
            EVT.coords[2].y = evt->y1.n - CV(0.125);
            AddEvtPrim3(gGraphicsPtr->ot, evt);
            p++;
         }
      }

      if (link1->state == 2 || link2->state == 2) {
         evt->state = 2;
         evt->state2 = 0x100;
      }
      break;

   case 2:
      link1 = EVT.links[0];
      link2 = EVT.links[1];

      if (link1->state == 99 || link2->state == 99) {
         evt->state = 99;
         break;
      }

      if (link1->state != 2) {
         link1 = link2;
      }

      rnd = (rand() >> 2) % DEG(360);
      evt->x1.n = link1->x1.n + (evt->state2 * rcos(rnd) >> 12);
      evt->z1.n = link1->z1.n + (evt->state2 * rsin(rnd) >> 12);
      evt->y1.n = link1->y1.n;
      EVT.coords[0].x = EVT.coords[1].x = link1->x1.n;
      EVT.coords[2].x = EVT.coords[3].x = evt->x1.n;
      EVT.coords[0].z = EVT.coords[1].z = link1->z1.n;
      EVT.coords[2].z = EVT.coords[3].z = evt->z1.n;
      EVT.coords[1].y = link1->y1.n + CV(0.125);
      EVT.coords[0].y = link1->y1.n - CV(0.125);
      EVT.coords[3].y = evt->y1.n + CV(0.125);
      EVT.coords[2].y = evt->y1.n - CV(0.125);
      AddEvtPrim3(gGraphicsPtr->ot, evt);

      evt->state2 += 4;
      if (link1->state == 2 || link2->state == 2) {
         evt->state = 2;
      }
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 394
void Evtf394_DynamoHum_FX1(EvtData *evt) {
   EvtData *evt_s1;
   EvtData *orb1;
   EvtData *orb2;
   s32 rnd;
   s32 i;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->y1.n += CV(1.0);

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_DYNAMO_HUM_ELECTRIC_ORB;
      EVT.orb = evt_s1;
      evt_s1->x2.n = evt->x1.n;
      evt_s1->z2.n = evt->z1.n;
      evt_s1->y2.n = evt->y1.n;
      rnd = (rand() >> 2) % DEG(360);
      evt_s1->state2 = rnd;
      evt_s1->state3 = 0x100;
      evt_s1->d.evtf395.parent = evt;
      orb1 = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_DYNAMO_HUM_ELECTRIC_ORB;
      evt_s1->x2.n = evt->x1.n;
      evt_s1->z2.n = evt->z1.n;
      evt_s1->y2.n = evt->y1.n;
      evt_s1->state2 = rnd + DEG(180);
      evt_s1->state3 = 0x100;
      evt_s1->d.evtf395.parent = evt;
      orb2 = evt_s1;

      for (i = 0; i < 4; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_DYNAMO_HUM_ORB_ELECTRICITY;
         evt_s1->d.evtf396.links[0] = orb1;
         evt_s1->d.evtf396.links[1] = orb2;
         evt_s1->state3 = 0;
         evt_s1->x3.n = i;
         evt_s1->state2 = 2;
      }

      evt->state3 = 0xc0;
      evt->state++;
      break;

   case 1:
      evt_s1 = EVT.orb;

      switch (evt->state2) {
      case 0:
         if (evt->state3 == 0x20) {
            FadeOutScreen(1, 8);
            evt->state2++;
            evt->mem = 0;
         }
         break;
      case 1:
         break;
      case 2:
         break;
      }

      if (evt_s1->state == 2 && --evt->mem <= 0) {
         i = rand() % DEG(360);
         evt_s1 = CreatePositionedEvt(evt, EVTF_DYNAMO_HUM_COLORED_BOLT);
         evt_s1->x1.n += CV(1.5) * rcos(i) >> 12;
         evt_s1->z1.n += CV(1.5) * rsin(i) >> 12;
         evt_s1->y1.n = GetTerrainElevation(evt_s1->z1.s.hi, evt_s1->x1.s.hi);
         evt->mem += rand() % 0x10;
      }
      if (--evt->state3 <= 0) {
         evt->state = 99;
         evt->state3 = 8;
         FadeInScreen(1, 0x80);
      }
      break;

   case 99:
      if (--evt->state3 <= 0) {
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 398
void Evtf398_Fx_TBD(EvtData *evt) {
   static s16 explosionAnimData[26] = {3, GFX_EXPLOSION_1,  1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,
                                       1, GFX_EXPLOSION_4,  1, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = explosionAnimData;
      if ((rand() >> 2) % 2 != 0) {
         evt->d.sprite.semiTrans = 2;
      }
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += (evt->x2.n - evt->x1.n) >> 4;
      evt->z1.n += (evt->z2.n - evt->z1.n) >> 4;
      evt->y1.n += (evt->y2.n - evt->y1.n) >> 4;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->d.sprite.animFinished) {
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 397
void Evtf397_Fx_TBD(EvtData *evt) {
   EvtData *evt_s2;
   s32 rnd;

   switch (evt->state) {
   case 0:
      gCameraZoom.vz = 512;
      SnapToUnit(evt);
      evt->d.sprite.clut = 3 + (rand() >> 2) % 3;
      evt->state3 = 0x80;
      evt->state++;

   // fallthrough
   case 1:
      gCameraRotation.vy += 4;
      if (evt->state3 % 3 == 0) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_FX_TBD_398;
         evt_s2->x1.n = evt->x1.n;
         evt_s2->z1.n = evt->z1.n;
         evt_s2->y1.n = evt->y1.n;
         rnd = (rand() >> 2) % DEG(360);
         evt_s2->x2.n = CV(0.5) * rcos(rnd) >> 12;
         evt_s2->z2.n = CV(0.5) * rsin(rnd) >> 12;
         rnd = (rand() >> 2) % 0x20;
         evt_s2->y2.n = (rand() % CV(1.0) + CV(2.5)) * rsin(rnd + 0x3f0) >> 12;
         evt_s2->d.sprite.clut = evt->d.sprite.clut;
         evt_s2->x2.n += evt->x1.n;
         evt_s2->z2.n += evt->z1.n;
         evt_s2->y2.n += evt->y1.n;
      }
      if (--evt->state3 <= 0) {
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
#define EVTF 375
void Evtf375_FlameBreath_Particle(EvtData *evt) {
   static s16 animData[26] = {GFX_EXPLOSION_4,  2, 4, GFX_EXPLOSION_5,  2, 4, GFX_EXPLOSION_6, 2, 4,
                              GFX_EXPLOSION_7,  2, 4, GFX_EXPLOSION_8,  2, 3, GFX_EXPLOSION_9, 2, 3,
                              GFX_EXPLOSION_10, 2, 3, GFX_EXPLOSION_11, 2, 3, GFX_NULL,        0};
   static s16 animData_unused[20] = {3, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                     2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                     2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                     0, GFX_NULL};

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = animData;
      evt->d.sprite.semiTrans = 0;
      evt->state++;

   // fallthrough
   case 1:
      UpdateMultisizeEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->x2.n += (evt->x3.n - evt->x2.n) >> 3;
      evt->z2.n += (evt->z3.n - evt->z2.n) >> 3;
      evt->y2.n += (evt->y3.n - evt->y2.n) >> 3;
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 382
void Evtf382_FlameBreath(EvtData *evt) {
   static s16 thetas[4] = {DEG(90), DEG(0), DEG(270), DEG(180)};
   static s16 radii[4] = {CV(0.75), CV(0.75), CV(0.9375), CV(0.9375)};
   static s16 yOffsets[4] = {CV(0.625), CV(0.625), CV(0.125), CV(0.125)};

   s16 dir;
   EvtData *evt_s2;

   GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

   dir = EVT.direction + (gCameraRotation.vy & DEG(270));
   dir /= DEG(90);

   switch (evt->state) {
   case 0:
      if (++evt->state3 >= 24) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 1:
      SnapToUnit(evt);
      evt->y1.n += CV(0.5);
      EVT.gfxIdx = GFX_SALAMANDER_S;
      EVT.boxIdx = 3;
      if (EVT.clut == CLUT_NULL) {
         EVT.clut = CLUT_REDS;
      }
      evt_s2 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.theta = thetas[((evt_s2->d.sprite.direction & 0xfff) >> 10)];
      EVT.unused_0x40 = (evt_s2->d.sprite.direction & 0xfff) >> 10;
      evt->x3.n = 40 * rcos(EVT.theta) >> 12;
      evt->z3.n = 40 * rsin(EVT.theta) >> 12;
      evt->y3.n = 0x10;
      evt->y2.n = evt->y1.n;
      evt->x2.n = evt->x1.n;
      evt->z2.n = evt->z1.n;
      evt->x1.n += radii[dir] * rcos(EVT.theta) >> 12;
      evt->z1.n += radii[dir] * rsin(EVT.theta) >> 12;
      EVT.yTheta = 0;
      evt->state3 = 0x30;
      evt->state++;

   // fallthrough
   case 2:
      evt_s2 = GetUnitSpriteAtPosition(evt->z2.s.hi, evt->x2.s.hi);
      evt->y1.n = evt_s2->y1.n + yOffsets[dir];
      evt->y3.n = 24 * rsin(0x300 * rsin(EVT.yTheta) >> 12) >> 12;
      EVT.yTheta += 0x20;
      if (evt->state3 % 2 == 0 && evt->state3 >= 16) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_FLAME_BREATH_PARTICLE;
         evt_s2->x1.n = evt->x1.n;
         evt_s2->z1.n = evt->z1.n;
         evt_s2->y1.n = evt->y1.n;
         evt_s2->d.sprite.clut = EVT.clut;
         evt_s2->d.sprite.boxIdx = EVT.boxIdx;
         evt_s2->x2.n = evt->x3.n + ((rand() >> 2) % 13 - 6);
         evt_s2->z2.n = evt->z3.n + ((rand() >> 2) % 13 - 6);
         evt_s2->y2.n = evt->y3.n;
         evt_s2->y3.n = evt_s2->y2.n >> 2;
         evt_s2->x3.n = evt_s2->x2.n >> 2;
         evt_s2->z3.n = evt_s2->z2.n >> 2;
      }
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 3:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Noop_8008bca8() {}

void Evtf274_Noop(EvtData *evt) {}

#undef EVTF
#define EVTF 344
void Evtf344_345_RomanFire_FX2_FX3(EvtData *evt) {
   EVT.clut = CLUT_REDS;
   if (evt->functionIndex == EVTF_ROMAN_FIRE_FX3) {
      evt->functionIndex = EVTF_FX_TBD_142;
   } else if (evt->functionIndex == EVTF_ROMAN_FIRE_FX2) {
      evt->functionIndex = EVTF_FX_TBD_140;
   } else {
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
   }
}

#undef EVTF
#define EVTF 339
void Evtf339_349_Rubble(EvtData *evt) {
   static s16 rockAnimData_Fast[12] = {7, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3,
                                       2, GFX_ROCK_4, 2, GFX_NULL,   1, GFX_NULL};
   static s16 rockAnimData_Slow[12] = {7, GFX_ROCK_1, 3, GFX_ROCK_2, 3, GFX_ROCK_3,
                                       3, GFX_ROCK_4, 3, GFX_NULL,   1, GFX_NULL};

   s32 halfSize;
   s32 rnd1, rnd2, rnd3;
   s16 elevation;
   SVECTOR vector;

   halfSize = evt->d.sprite.coords[0].x;
   gQuad_800fe63c[0].vx = -halfSize;
   gQuad_800fe63c[0].vy = -halfSize;
   gQuad_800fe63c[1].vx = halfSize;
   gQuad_800fe63c[1].vy = -halfSize;
   gQuad_800fe63c[2].vx = -halfSize;
   gQuad_800fe63c[2].vy = halfSize;
   gQuad_800fe63c[3].vx = halfSize;
   gQuad_800fe63c[3].vy = halfSize;

   switch (evt->state) {
   case 0:
      gQuad_800fe63c[0].vx = -2;
      gQuad_800fe63c[0].vy = -2;
      gQuad_800fe63c[1].vx = 2;
      gQuad_800fe63c[1].vy = -2;
      gQuad_800fe63c[2].vx = -2;
      gQuad_800fe63c[2].vy = 2;
      gQuad_800fe63c[3].vx = 2;
      gQuad_800fe63c[3].vy = 2;

      switch (evt->functionIndex) {
      case EVTF_AVALANCHE_RUBBLE:
         evt->d.sprite.gfxIdx = GFX_ROCK_1;
         evt->d.sprite.boxIdx = 7;
         evt->d.sprite.coords[0].x = 1 + (rand() >> 2) % 8;
         rnd1 = rand() % DEG(360);
         if ((rnd1 >> 2) % 2 != 0) {
            evt->d.sprite.animData = rockAnimData_Fast;
         } else {
            evt->d.sprite.animData = rockAnimData_Slow;
         }
         rnd2 = 0x100 + (0x20 * rsin(rand() % DEG(360)) >> 12);
         rnd3 = 0x180 + (0x80 * rsin(rand() % DEG(360)) >> 12);
         func_800A9E78(&vector, rnd2, rnd3, rnd1);
         evt->x2.n = evt->x1.n + vector.vx;
         evt->z2.n = evt->z1.n + vector.vz;
         evt->y2.n = evt->y1.n + vector.vy;
         evt->x3.n = (evt->x2.n - evt->x1.n) / 2;
         evt->z3.n = (evt->z2.n - evt->z1.n) / 2;
         evt->y3.n = (evt->y2.n - evt->y1.n) / 2;
         evt->y2.n = 0;
         evt->state2 = 0x20;
         AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
         evt->state++;
         break;

      case EVTF_RUBBLE:
         evt->d.sprite.gfxIdx = GFX_ROCK_1;
         evt->d.sprite.boxIdx = 7;
         evt->d.sprite.coords[0].x = 1 + (rand() >> 2) % 5;
         rnd1 = rand() % DEG(360);
         if ((rnd1 >> 2) % 2 != 0) {
            evt->d.sprite.animData = rockAnimData_Fast;
         } else {
            evt->d.sprite.animData = rockAnimData_Slow;
         }
         rnd2 = 0x20 + (0x20 * rsin(rand() % DEG(360)) >> 12);
         rnd3 = 0x300 + (0x80 * rsin(rand() % DEG(360)) >> 12);
         func_800A9E78(&vector, rnd2, rnd3, rnd1);
         evt->x2.n = evt->x1.n + vector.vx;
         evt->z2.n = evt->z1.n + vector.vz;
         evt->y2.n = evt->y1.n + vector.vy;
         evt->x3.n = (evt->x2.n - evt->x1.n) / 2;
         evt->z3.n = (evt->z2.n - evt->z1.n) / 2;
         evt->y3.n = (evt->y2.n - evt->y1.n) * 2;
         //?
         evt->y2.n = ~((5 - evt->d.sprite.coords[0].x) / 2) * 8;
         // evt->y2.n = (evt->d.sprite.coords[0].x / 2 - 3) * 8;
         evt->state2 = 0x60;
         AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
         evt->state++;
         break;
      }

      break;

   case 1:
      evt->x1.n = evt->x1.n + evt->x3.n;
      evt->z1.n = evt->z1.n + evt->z3.n;
      evt->y1.n = evt->y1.n + evt->y3.n;
      evt->y3.n = evt->y3.n + evt->y2.n;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      if (evt->y1.n < elevation) {
         evt->y1.n = elevation;
         evt->y3.n = -(evt->y3.n / 2);
         if (OBJ_TERRAIN(evt).s.terrain == TERRAIN_WATER) {
            CreatePositionedEvt(evt, EVTF_RIPPLE);
            evt->functionIndex = EVTF_NULL;
            return;
         }
      }
      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;

   case 2:
      break;
   }
}

#undef EVTF
#define EVTF 317
void Evtf317_338_Avalanche_FX2_FX3(EvtData *evt) {
   EvtData *evt_v1;
   s32 i;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->y1.n += CV(0.5);

      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_UNIT_STRUCK;
      evt_v1->x1.n = evt->x1.n;
      evt_v1->z1.n = evt->z1.n;
      evt_v1->y1.n = evt->y1.n;

      EVT.availableSlots = Evt_CountUnused();
      EVT.rubbleAmount = (EVT.availableSlots - 80) / 8;
      evt->state++;
      evt->state3 = 1;

   // fallthrough
   case 1:
      if (--evt->state3 <= 0) {
         evt->state3 = 4;
         evt->state++;
         evt->state2 = 1;
      }
      break;

   case 2:
      if (--evt->state2 <= 0) {
         for (i = 0; i < EVT.rubbleAmount; i++) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_AVALANCHE_RUBBLE;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->z1.n = evt->z1.n;
            evt_v1->y1.n = evt->y1.n;
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_RUBBLE;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->z1.n = evt->z1.n;
            evt_v1->y1.n = evt->y1.n;
         }
         for (i = 0; i < 8; i++) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_AVALANCHE_DUST_CLOUD;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->z1.n = evt->z1.n;
         }
         if (--evt->state3 <= 0) {
            evt->state++;
            evt->state3 = 0x40;
         } else {
            evt->state2 = 1;
         }
      }
      break;

   case 3:
      if (--evt->state3 <= 0) {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_DISPLAY_DAMAGE_2;
         evt_v1->x1.s.hi = evt->x1.s.hi;
         evt_v1->z1.s.hi = evt->z1.s.hi;
         evt->state++;
         evt->state3 = 0x20;
      }
      break;

   case 4:
      if (--evt->state3 <= 0) {
         if (evt->functionIndex == EVTF_AVALANCHE_FX3) {
            evt->state++;
         } else {
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
         }
      }
      break;

   case 5:
      PerformAudioCommand(AUDIO_CMD_PLAY_SFX(232));
      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_STRETCH_WARP_SPRITE;
      evt_v1->x1.n = evt->x1.n;
      evt_v1->z1.n = evt->z1.n;
      evt_v1->y1.n = evt->y1.n;
      evt->state3 = 0x20;
      evt->state++;
      break;

   case 6:
      if (--evt->state3 <= 0) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 084
void Evtf084_Avalanche_DustCloud(EvtData *evt) {
   s16 randomAngle;

   switch (evt->state) {
   case 0:
      evt->state2 = 0x10;
      evt->d.sprite.gfxIdx = GFX_PUFF_1;
      evt->d.sprite.animData = sSmokeAnimData_800ff688;
      evt->d.sprite.boxIdx = 3 + (rand() >> 2) % 3;
      randomAngle = (rand() >> 2) & 0xfff;
      evt->x2.n = 0x20 * rsin(randomAngle) >> 12;
      evt->z2.n = 0x20 * rcos(randomAngle) >> 12;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 1);
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 2:
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->y2.n = 0x20;
      evt->y3.n = 4;
      evt->state++;
      evt->state2 = 0x30;

   // fallthrough
   case 3:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->y2.n += evt->y3.n;
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 292
void Evtf292_BlueItemSparkle(EvtData *evt) {
   static s16 sparkleAnimData[14] = {7, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};
   EvtData *sprite;
   Quad quad;
   s32 i;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_LIT_SPHERE_8;
      sprite->d.sprite.boxIdx = 7;
      sprite->d.sprite.clut = CLUT_BLUES;
      sprite->d.sprite.semiTrans = 4;
      sprite->d.sprite.animData = sparkleAnimData;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      for (i = 0; i < 4; i++) {
         quad[i] = gQuad_800fe63c[i];
      }
      for (i = 0; i < evt->state2; i++) {
         gQuad_800fe63c[0].vx = -i * 2;
         gQuad_800fe63c[1].vx = i * 2;
         gQuad_800fe63c[2].vx = -i * 2;
         gQuad_800fe63c[3].vx = i * 2;
         gQuad_800fe63c[0].vy = -i * 2;
         gQuad_800fe63c[1].vy = -i * 2;
         gQuad_800fe63c[2].vy = i * 2;
         gQuad_800fe63c[3].vy = i * 2;
         UpdateEvtAnimation(sprite);
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      }
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i] = quad[i];
      }
      if (++evt->state3 > 0x10) {
         evt->state++;
      }
      evt->state2 = abs(8 * rsin(evt->state3 * 0x10) >> 12) + 1;
      break;

   case 2:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 293
void Evtf293_Fx_TBD(EvtData *evt) {
   static s16 sparkleAnimData[14] = {5, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = sparkleAnimData;
      if (evt->state2 == 0) {
         evt->state2 = 0x20;
      }
      evt->state++;

   // fallthrough
   case 1:
      UpdateEvtAnimation(evt);
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 290
void Evtf290_294_761_RevealItem(EvtData *evt) {
   EvtData *evt_s5;
   s16 elevation;
   s32 halfSize;
   s32 i;
   Quad quad;

   switch (evt->state) {
   case 0:
      evt->y1.n += CV(0.5);
      evt->state3 = 0;
      EVT.timer = 1;
      EVT.theta = DEG(90) * 10;
      if (evt->functionIndex == EVTF_REVEAL_CHEST_ITEM) {
         CreatePositionedEvt(evt, EVTF_CHEST_IMPACT);
      }
      evt->state++;

   // fallthrough
   case 1:
      if (++evt->state3 >= 0x10 || evt->functionIndex == EVTF_REVEAL_USED_ITEM) {
         evt->state3 = 0;
         evt->state2 = 0;
         evt->mem = 1;
         evt->state++;
         evt->y2.n = CV(0.25);
         if (evt->functionIndex == EVTF_REVEAL_HIDDEN_ITEM ||
             evt->functionIndex == EVTF_REVEAL_USED_ITEM) {
            evt->y1.n += CV(0.25);
         }
         evt->y3.n = -6;
      }
      break;

   case 2:
      if (--evt->mem <= 0) {
         evt_s5 = CreatePositionedEvt(evt, EVTF_BLUE_ITEM_SPARKLE);
         evt_s5->x1.n += rand() % CV(0.75) - CV(0.375);
         evt_s5->z1.n += rand() % CV(0.75) - CV(0.375);
         evt_s5->y1.n += rand() % CV(0.75) - CV(0.375);
         evt->mem = rand() % 8 + 4;
      }

      switch (evt->state2) {
      case 0:
         evt->y1.n += evt->y2.n;
         evt->y2.n += evt->y3.n;
         elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
         if (evt->functionIndex == EVTF_REVEAL_HIDDEN_ITEM ||
             evt->functionIndex == EVTF_REVEAL_USED_ITEM) {
            elevation += CV(0.75);
         }
         if (evt->y2.n < 0 && evt->y1.n < elevation + CV(1.0)) {
            EVT.theta = 0;
            evt->y1.n = elevation + CV(1.0);
            evt->state2++;
         }
         if (--evt->state3 <= 0 && evt->functionIndex != EVTF_REVEAL_USED_ITEM) {
            for (i = 0; i < 8; i++) {
               evt_s5 = CreatePositionedEvt(evt, EVTF_SPARKLE);
               evt_s5->x2.n = 12 * rcos(i * DEG(45)) >> 12;
               evt_s5->z2.n = 12 * rsin(i * DEG(45)) >> 12;
               evt_s5->y3.n = -2;
               evt_s5->y2.n = 8;
               evt_s5->d.sprite.clut = CLUT_REDS;
               evt_s5->state2 = 24;
            }
            evt->state3 = 6;
         }
         if (++EVT.timer > 12) {
            EVT.timer = 12;
         }
         break;

      case 1:
         if (++EVT.timer > 12) {
            EVT.timer = 12;
         }
         if (gSignal3 == 1) {
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            evt->state++;
         }
         break;
      }

      // Item icon
      evt_s5 = Evt_GetUnused();
      evt_s5->functionIndex = EVTF_NOOP;
      evt_s5->d.sprite.gfxIdx = EVT.gfxIdx;
      evt_s5->d.sprite.boxIdx = 7;
      evt_s5->x1.n = evt->x1.n;
      evt_s5->z1.n = evt->z1.n;
      evt_s5->y1.n = evt->y1.n;

      halfSize = EVT.timer;
      quad[0].vx = -halfSize;
      quad[1].vx = halfSize;
      quad[2].vx = -halfSize;
      quad[3].vx = halfSize;
      quad[0].vy = -halfSize;
      quad[1].vy = -halfSize;
      quad[2].vy = halfSize;
      quad[3].vy = halfSize;

      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i].vx = (quad[i].vx * rcos(EVT.theta) - quad[i].vy * rsin(EVT.theta)) >> 12;
         gQuad_800fe63c[i].vy = (quad[i].vx * rsin(EVT.theta) + quad[i].vy * rcos(EVT.theta)) >> 12;
         gQuad_800fe63c[i].vz = 0;
      }

      EVT.theta += (0 - EVT.theta) >> 2;
      AddEvtPrim6(gGraphicsPtr->ot, evt_s5, 0);
      evt_s5->functionIndex = EVTF_NULL;
      break;

   case 3:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF Unk8008d1f0
void Evtf_Unk_8008d1f0(EvtData *evt) {
   EvtData *unitSprite;
   SVectorXZY *p;
   s32 i;
   s32 camRotY;

   switch (evt->state) {
   case 0:
      unitSprite = SnapToUnit(evt);
      EVT.unitSprite = unitSprite;
      EVT.gfxIdx = GFX_GLOBE_5;
      EVT.boxIdx = 7;
      EVT.clut = CLUT_BLUES;
      EVT.semiTrans = 2;
      evt->state2 = 0;
      EVT.coords[0].y = EVT.coords[1].y = -0x10;
      EVT.coords[2].y = EVT.coords[3].y = 0x10;
      EVT.coords[0].x = EVT.coords[2].x = -0x10;
      EVT.coords[1].x = EVT.coords[3].x = 0x10;
      evt->mem = 0x400;
      evt->state++;

   // fallthrough
   case 1:
      p = &EVT.coords[0];
      i = 0;
      camRotY = gCameraRotation.vy;
      for (; i < 4; i++) {
         gQuad_800fe63c[i].vx = p->x;
         gQuad_800fe63c[i].vy = (p->y * rcos(camRotY) - p->z * rsin(camRotY)) >> 12;
         gQuad_800fe63c[i].vz = (p->y * rsin(camRotY) + p->z * rcos(camRotY)) >> 12;
         p++;
      }
      AddEvtPrim6(gGraphicsPtr->ot, evt, 1);
      if (++evt->state2 >= 0x80) {
         evt->state++;
      }
      break;

   case 2:
      unitSprite = EVT.unitSprite;
      unitSprite->d.sprite.hidden = 0;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 295
void Evtf295_Smoke(EvtData *evt) {
   static s16 animData[32] = {GFX_PUFF_1,  2, 3, GFX_PUFF_2, 2, 3, GFX_PUFF_3, 2, 3,
                              GFX_PUFF_4,  2, 3, GFX_PUFF_5, 2, 3, GFX_PUFF_6, 2, 3,
                              GFX_PUFF_7,  2, 3, GFX_PUFF_8, 2, 3, GFX_PUFF_9, 2, 3,
                              GFX_PUFF_10, 2, 3, GFX_NULL,   0};

   s32 i, ct;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = animData;
      evt->d.sprite.semiTrans = 1;
      ct = rand() % 3;
      for (i = 0; i < ct; i++) {
         UpdateMultisizeEvtAnimation(evt);
      }
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->x2.n += (0 - evt->x2.n) >> 2;
      evt->z2.n += (0 - evt->z2.n) >> 2;
      evt->y2.n += (0 - evt->y2.n) >> 2;
      UpdateMultisizeEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 385
void Evtf385_RevealMimic(EvtData *evt) {
   s32 i;
   EvtData *smoke;
   SVECTOR vector;

   switch (evt->state) {
   case 0:
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + CV(0.25);
      evt->state2 = 0x30;
      evt->state++;

   // fallthrough
   case 1:
      for (i = 0; i < 3; i++) {
         smoke = CreatePositionedEvt(evt, EVTF_SMOKE);
         func_800A9E78(&vector, rand() % evt->state2 + evt->state2, rand() % DEG(45),
                       rand() % DEG(360));
         smoke->x2.n = vector.vx;
         smoke->z2.n = vector.vz;
         smoke->y2.n = vector.vy;
      }
      if (++evt->state2 >= 0x20) {
         evt->state2 = 0x30;
      }
      if (gState.D_8014053E != 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 301
void Evtf301_Map32_SmokestackParticle(EvtData *evt) {
   static s16 animData[24] = {7, GFX_PUFF_1, 2, GFX_PUFF_2,  2, GFX_PUFF_3, 2, GFX_PUFF_4,
                              2, GFX_PUFF_5, 2, GFX_PUFF_6,  2, GFX_PUFF_7, 2, GFX_PUFF_8,
                              2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   0, GFX_NULL};
   static Quad quad = {{-24, -24, 0, 0}, {24, -24, 0, 0}, {-24, 24, 0, 0}, {24, 24, 0, 0}};

   Quad *qswap;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = animData;
      evt->d.sprite.semiTrans = 1;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;

      qswap = gSpriteBoxQuads[7];
      gSpriteBoxQuads[7] = &quad;
      UpdateEvtAnimation(evt);
      if ((evt->x1.s.hi < D_80122E28) || (evt->x1.s.hi > gMapSizeX + D_80122E28 - 1) ||
          (evt->z1.s.hi < D_80122E2C) || (evt->z1.s.hi > gMapSizeZ + D_80122E2C - 1)) {
         evt->d.sprite.hidden = 1;
      } else {
         evt->d.sprite.hidden = 0;
      }
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      gSpriteBoxQuads[7] = qswap;

      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 300
void Evtf300_Map32_Smokestack(EvtData *evt) {
   EvtData *smoke;

   switch (evt->state) {
   case 0:
      evt->state++;
      evt->mem = 3;
      evt->state3 = 0;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         smoke = CreatePositionedEvt(evt, EVTF_MAP32_SMOKESTACK_PARTICLE);
         smoke->z2.n = rand() % 9 - 4;
         smoke->x2.n = -(rand() % 4 + 12);
         smoke->y2.n = rand() % 4 + 12;
         smoke->z3.n = 0;
         smoke->x3.n = -(rand() % 2 + 5);
         evt->mem = rand() % 3 + 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 332
void Evtf332_RollingFire_FX1(EvtData *evt) {
   extern s16 gFlameAnimData_800ff8a4[];

   EvtData *dataStore;
   Cylinder *dsCylinder;

   switch (evt->state) {
   case 0:
      evt->x1.n = gTargetX * CV(1.0) + CV(0.5);
      evt->z1.n = gTargetZ * CV(1.0) + CV(0.5);
      evt->y1.n = GetTerrainElevation(gTargetZ, gTargetX);
      evt->d.sprite.animData = gFlameAnimData_800ff8a4;

      dataStore = Evt_GetUnused();
      dataStore->functionIndex = EVTF_NOOP;
      EVT.dataStore = dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->top.vy = evt->y1.n + CV(4.0);
      dsCylinder->sideCt = 8;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = GFX_FLAME_1;
      dsCylinder->bottomRadius = CV(1.0);
      dsCylinder->topRadius = CV(1.0);
      dsCylinder->useColor = 1;
      dsCylinder->color.r = 0x80;
      dsCylinder->color.g = 0x80;
      dsCylinder->color.b = 0x80;

      evt->state2 = 0x200;
      evt->y2.n = 0x60;
      evt->y3.n = -4;
      evt->x2.n = 0;
      evt->x3.n = 0x10;
      evt->state++;

   // fallthrough
   case 1:
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 2);
      if (++evt->mem >= 4) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 2:
      dataStore = EVT.dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      UpdateEvtAnimation(evt);
      dsCylinder->gfxIdx = EVT.gfxIdx;
      if (evt->mem >= 0x80) {
         dsCylinder->color.r = evt->mem - 1;
         dsCylinder->color.g = evt->mem - 1;
         dsCylinder->color.b = evt->mem - 1;
      }
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vy = evt->y1.n + (CV(6.0) * rsin(evt->mem * 8) >> 12);
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->bottomRadius = CV(2.0) * rsin(evt->mem * 8) >> 12;
      dsCylinder->topRadius =
          dsCylinder->bottomRadius * 2 + (CV(2.0) * rsin(evt->mem * 0x20) >> 12);
      dsCylinder->theta = evt->x2.n;
      evt->x2.n += evt->x3.n;
      evt->x3.n += 1;
      dsCylinder->semiTrans = 4;
      RenderCylinder(dsCylinder);
      dsCylinder->top.vy = evt->y1.n + (CV(4.0) * rsin(evt->mem * 8) >> 12);
      dsCylinder->top.vx = evt->x1.n + (CV(0.5) * rcos(evt->mem * 0x20) >> 12);
      dsCylinder->top.vz = evt->z1.n + (CV(0.5) * rsin(evt->mem * 0x20) >> 12);
      dsCylinder->semiTrans = 2;
      RenderCylinder(dsCylinder);
      dsCylinder->theta = -dsCylinder->theta;
      dsCylinder->bottomRadius -= (dsCylinder->bottomRadius >> 1);
      dsCylinder->topRadius -= (dsCylinder->topRadius >> 1);
      RenderCylinder(dsCylinder);
      evt->state2 += evt->y2.n;
      evt->y2.n += evt->y3.n;
      evt->mem += 2;
      gCameraZoom.vz += (384 - gCameraZoom.vz) >> 4;
      gCameraRotation.vy -= 6;
      if (evt->mem >= 0x100) {
         evt->state++;
      }
      break;

   case 3:
      dataStore = EVT.dataStore;
      dataStore->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 333
void Evtf333_Fx_TBD(EvtData *evt) {
   extern s16 gSparkleAnimData_800ff38c[14];
   EvtData *evt_s3;
   s16 a, b;

   switch (evt->state) {
   case 0:
      evt_s3 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x2.n = evt_s3->x1.n;
      evt->z2.n = evt_s3->z1.n;
      evt->y2.n = evt_s3->y1.n;
      evt->d.sprite.animData = gSparkleAnimData_800ff38c;
      // TODO: EvtData
      evt->d.sprite.coords[0].x = 0x400;
      evt->d.sprite.coords[0].z = -4;
      evt->d.sprite.coords[0].y = 0;
      evt->d.sprite.coords[1].x = 0x40;
      evt->x1.n = evt->x2.n + (CV(4.0) * rcos(0) >> 12);
      evt->z1.n = evt->z2.n + (CV(4.0) * rsin(0) >> 12);
      evt->y1.n = evt->y2.n + CV(1.0);
      evt->state++;

   // fallthrough
   case 1:
      evt_s3 = Evt_GetUnused();
      evt_s3->functionIndex = EVTF_FX_TBD_323;
      evt_s3->d.evtf323.gfxIdx = GFX_RUNE_1 + rand() % 10;
      evt_s3->d.evtf323.semiTrans = 2;
      evt_s3->state2 = 0xff;
      evt_s3->state3 = 8;
      evt_s3->d.evtf323.coords[1].x = evt->x1.n;
      evt_s3->d.evtf323.coords[3].x = evt->x1.n;
      evt_s3->d.evtf323.coords[1].z = evt->z1.n;
      evt_s3->d.evtf323.coords[3].z = evt->z1.n;
      evt_s3->d.evtf323.coords[1].y = evt->y1.n;
      evt_s3->d.evtf323.coords[3].y = evt->y1.n - CV(0.25);
      a = evt->d.sprite.coords[0].x + evt->d.sprite.coords[0].z;
      evt->d.sprite.coords[0].x = a;
      b = evt->d.sprite.coords[0].y + evt->d.sprite.coords[1].x;
      evt->d.sprite.coords[0].y = b;
      evt->x1.n = evt->x2.n + (a * rcos(b) >> 12);
      evt->z1.n = evt->z2.n + (a * rsin(b) >> 12);
      evt->y1.n = evt->y2.n;
      UpdateEvtAnimation(evt);
      evt->d.sprite.boxIdx = 5;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      evt_s3->d.evtf323.coords[0].x = evt->x1.n;
      evt_s3->d.evtf323.coords[2].x = evt->x1.n;
      evt_s3->d.evtf323.coords[0].z = evt->z1.n;
      evt_s3->d.evtf323.coords[2].z = evt->z1.n;
      evt_s3->d.evtf323.coords[0].y = evt->y1.n;
      evt_s3->d.evtf323.coords[2].y = evt->y1.n - CV(0.25);
      evt_s3->x1.n = evt_s3->d.evtf323.coords[0].x;
      evt_s3->z1.n = evt_s3->d.evtf323.coords[0].z;
      evt_s3->y1.n = evt_s3->d.evtf323.coords[0].y;
      if (a < 0x20) {
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
#define EVTF 348
void Evtf348_Fx_TBD(EvtData *evt) {
   EvtData *dataStore;
   EvtData *sprite;
   Cylinder *dsCylinder;
   s32 i;
   s16 iVar6;
   s16 iVar2;
   s16 uVar1;
   s32 half;
   s16 theta_0xc0;

   gGfxSubTextures[GFX_TILED_FLAMES_DYN_1].y =
       gGfxSubTextures[GFX_TILED_FLAMES].y + (evt->mem * 2 % 0x20);

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      dataStore = Evt_GetUnused();
      dataStore->functionIndex = EVTF_NOOP;
      EVT.dataStore = dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->bottom.vx = dsCylinder->top.vx = evt->x1.n;
      dsCylinder->bottom.vz = dsCylinder->top.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vy = evt->y1.n + CV(4.0);
      dsCylinder->sideCt = 16;
      dsCylinder->topRadius = CV(1.0);
      dsCylinder->bottomRadius = CV(1.0);
      dsCylinder->gfxIdx = GFX_TILED_FLAMES_DYN_1;
      dsCylinder->semiTrans = 2;
      dsCylinder->useColor = 1;
      dsCylinder->clut = CLUT_BLUES;
      dsCylinder->color.r = 0x80;
      dsCylinder->color.g = 0x80;
      dsCylinder->color.b = 0x80;
      dsCylinder->theta = 0;

      for (i = 0; i < 8; i++) {
         EVT.todo_x24[i] = (rand() >> 2) % 0x100 + 0x200;
         EVT.todo_x34[i] = 0;
         EVT.todo_x44[i] = (rand() >> 2) % 0x180;
      }

      evt->state++;

   // fallthrough
   case 1:
      dataStore = EVT.dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->theta = evt->mem << 2;
      dsCylinder->color.r = (0x100 - evt->mem) >> 1;
      dsCylinder->color.g = (0x100 - evt->mem) >> 1;
      dsCylinder->color.b = (0x100 - evt->mem) >> 1;

      //@6750
      uVar1 = evt->mem;
      iVar2 = (evt->mem * (0x100 - evt->mem)) >> 4;
      iVar6 = evt->mem;

      half = iVar6 >> 1;
      if (half >= 0x80) {
         half = 0x80;
      }

      for (i = 0; i < 8; i++) {
         dsCylinder->bottom.vy = evt->y1.n + (iVar2 * rsin(i * half) >> 12);
         dsCylinder->top.vy = evt->y1.n + (iVar2 * rsin((i + 1) * half) >> 12);
         dsCylinder->bottomRadius = uVar1 * rcos(i * half) >> 12;
         dsCylinder->topRadius = uVar1 * rcos((i + 1) * half) >> 12;
         RenderCylinder(dsCylinder);
      }

      sprite = Evt_GetUnused();
      sprite->d.sprite.gfxIdx = GFX_TILED_FLAMES;
      sprite->d.sprite.semiTrans = 4;
      theta_0xc0 = 0xc0;

      for (i = 0; i < 8; i++) {
         EVT.todo_x34[i] += (EVT.todo_x24[i] - EVT.todo_x34[i]) >> 3;
         iVar6 = EVT.todo_x34[i];

         sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[0].x =
             evt->x1.n + (iVar6 * rcos(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[0].z =
             evt->z1.n + (iVar6 * rsin(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[1].y = sprite->d.sprite.coords[0].y =
             evt->y1.n + (iVar6 * rsin(theta_0xc0) >> 12);

         iVar6 = uVar1 - 0x80;
         if (iVar6 < 0) {
            iVar6 = 0;
         }

         sprite->d.sprite.coords[3].x = evt->x1.n + (iVar6 * rcos(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[3].z = evt->z1.n + (iVar6 * rsin(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[3].y = evt->y1.n;
         sprite->d.sprite.coords[2].x = evt->x1.n + (uVar1 * rcos(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[2].z = evt->z1.n + (uVar1 * rsin(i * DEG(45)) >> 12);
         sprite->d.sprite.coords[2].y = evt->y1.n;
         AddEvtPrim4(gGraphicsPtr->ot, sprite);
      }

      if (++evt->mem >= 0x100) {
         evt->state++;
      }

      break;

   case 2:
      dataStore = EVT.dataStore;
      dataStore->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 334
void Evtf334_Salamander_FX1(EvtData *evt) {
   EvtData *previous;
   EvtData *current;
   s32 i;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      previous = CreatePositionedEvt(evt, EVTF_SALAMANDER_HEAD);
      previous->d.evtf335.link = evt;
      previous->d.evtf335.parent = evt;

      for (i = 0; i < 32; i++) {
         current = Evt_GetUnused();
         current->functionIndex = EVTF_SALAMANDER_SEGMENT;
         current->x1.n = evt->x1.n;
         current->z1.n = evt->z1.n;
         current->y1.n = evt->y1.n;
         current->d.evtf336.link = previous;
         current->d.evtf336.theta1 = 0;
         current->d.evtf336.theta2 = 0;
         current->d.evtf336.todo_x2c = 0;
         current->d.evtf336.todo_x2e = i * 0x111;
         current->d.evtf336.parent = evt;
         current->mem = 0;
         previous = current;
      }

      EVT.todo_x24 = 0;
      evt->state++;

   // fallthrough
   case 1:
      evt->mem = 0;
      evt->state++;
      break;

   case 2:
      if (gLightColor.r <= 0xd0) {
         gLightColor.r++;
         // gLightColor.g -= 1; // No match; needs to be decrement op.
         gLightColor.g--;
         gLightColor.b--;
      }
      if (evt->mem == 1) {
         evt->state++;
      }
      break;

   case 3:
      gLightColor.r = (0x80 - gLightColor.r) >> 2;
      gLightColor.g = (0x80 - gLightColor.g) >> 2;
      gLightColor.b = (0x80 - gLightColor.b) >> 2;
      if (++evt->mem >= 24) {
         gLightColor.b = 0x80;
         gLightColor.g = 0x80;
         gLightColor.r = 0x80;
         evt->state3 = 1;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 335
void Evtf335_Salamander_Head(EvtData *evt) {
   Quad quad = {{-32, -32, 0, 0}, {32, -32, 0, 0}, {-32, 32, 0, 0}, {32, 32, 0, 0}};
   s16 headGfx[8] = {GFX_SALAMANDER_N, GFX_SALAMANDER_NE, GFX_SALAMANDER_E, GFX_SALAMANDER_SE,
                     GFX_SALAMANDER_S, GFX_SALAMANDER_SE, GFX_SALAMANDER_E, GFX_SALAMANDER_NE};

   EvtData *parent;
   EvtData *sprite;
   s16 unaff_s1;
   s32 i;
   s16 dir;
   s16 theta;

   parent = EVT.parent;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_GLOBE_8;
      sprite->d.sprite.boxIdx = 3;

      evt->x2.n = evt->x1.n;
      evt->y2.n = evt->y1.n;
      evt->z2.n = evt->z1.n;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + CV(2.0);
      evt->state3 = 0x50;
      evt->z3.n = 0;
      EVT.unused_0x4E = 0;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      EVT.position4.x = EVT.position3.x;
      EVT.position3.x = EVT.position2.x;
      EVT.position2.x = EVT.position1.x;
      EVT.position1.x = evt->x1.n;
      EVT.position4.y = EVT.position3.y;
      EVT.position3.y = EVT.position2.y;
      EVT.position2.y = EVT.position1.y;
      EVT.position1.y = evt->y1.n;
      EVT.position4.z = EVT.position3.z;
      EVT.position3.z = EVT.position2.z;
      EVT.position2.z = EVT.position1.z;
      EVT.position1.z = evt->z1.n;

      switch (evt->z3.n) {
      case 0:
         unaff_s1 = 0x40 + (0x30 * rsin(EVT.theta1 * 2) >> 12);
         EVT.theta1 += evt->state3;
         EVT.theta2 = CV(1.0) * rsin(EVT.theta1) >> 12;

         if (++evt->y3.n >= 0x100) {
            evt->y3.n = 0;
            evt->x3.n = 0x100;
            gCameraRotation.vy &= 0xfff;
            evt->z3.n++;
         }

         evt->x1.n -= (unaff_s1 * rcos(EVT.theta1) >> 12);
         evt->z1.n -= (unaff_s1 * rsin(EVT.theta1) >> 12);
         evt->y1.n -= (unaff_s1 * rsin(EVT.theta2) >> 12);

         gCameraRotation.vy += 0x10;
         gCameraZoom.vz = 460;
         break;

      case 1:
         EVT.theta1 = 0;
         EVT.theta2 += evt->x3.n;
         gCameraRotation.vy += (0 - gCameraRotation.vy) >> 2;
         unaff_s1 = 0xa0;

         if (++evt->y3.n >= 0x40) {
            evt->y3.n = 0;
            evt->z3.n++;
         }

         i = unaff_s1 * rcos(EVT.theta2) >> 12;
         evt->x1.n -= (i * rcos(EVT.theta1) >> 12);
         evt->z1.n -= (i * rsin(EVT.theta1) >> 12);
         evt->y1.n -= (unaff_s1 * rsin(EVT.theta2) >> 12);
         break;

      case 2:
         evt->y3.n = 0;
         evt->mem = 99;
         parent->mem++;
         evt->state++;
         break;

      case 3:
         EVT.theta2 = 0;
         unaff_s1 = 0x40;

         if (++evt->y3.n >= 0x80) {
            evt->y3.n = 0;
            evt->z3.n++;
         }

         evt->x1.n -= (unaff_s1 * rcos(EVT.theta1) >> 12);
         evt->z1.n -= (unaff_s1 * rsin(EVT.theta1) >> 12);
         evt->y1.n -= (unaff_s1 * rsin(EVT.theta2) >> 12);
         break;

      case 4:
      default:
         unaff_s1 = 0x40;
         evt->y3.n = 0;
         evt->mem = 99;
         parent->mem++;
         evt->state++;
         break;
      }

      if (EVT.theta1 >= DEG(360)) {
         evt->state3 = -evt->state3;
      } else if (EVT.theta1 <= 0) {
         evt->state3 = -evt->state3;
      }

      EVT.todo_x2a = EVT.theta2;
      EVT.todo_x28 = EVT.theta1;
      EVT.todo_x2c = unaff_s1 + 0xa0;
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 2);

      switch (evt->z3.n) {
      case 0:
         dir = (((gCameraRotation.vy - EVT.theta1) & 0xfff) / DEG(45) - 1) & 7;
         sprite->d.sprite.gfxIdx = headGfx[dir];
         sprite->d.sprite.boxIdx = 7;
         sprite->d.sprite.semiTrans = 2;

         for (i = 0; i < 4; i++) {
            gQuad_800fe63c[i] = quad[i];
         }

         if (dir > 4) {
            sprite->d.sprite.facingLeft = 1;
         } else {
            sprite->d.sprite.facingLeft = 0;
         }

         break;

      case 1:
      case 2:
         sprite->d.sprite.gfxIdx = GFX_SALAMANDER_E;
         dir = (((gCameraRotation.vy - EVT.theta1) & 0xfff) / DEG(45) - 1) & 7;

         if (dir > 4) {
            sprite->d.sprite.facingLeft = 1;
         } else {
            sprite->d.sprite.facingLeft = 0;
         }

         sprite->d.sprite.boxIdx = 7;

         theta = EVT.theta2;
         for (i = 0; i < 4; i++) {
            gQuad_800fe63c[i].vx = (quad[i].vx * rcos(theta) - quad[i].vy * rsin(theta)) >> 12;
            gQuad_800fe63c[i].vy = (quad[i].vx * rsin(theta) + quad[i].vy * rcos(theta)) >> 12;
         }
         break;

      case 3:
         break;
      }

      sprite->d.sprite.semiTrans = 2;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      sprite->d.sprite.semiTrans = 0;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      break;

   case 2:
      gCameraZoom.vz += (256 - gCameraZoom.vz) >> 2;
      if (++evt->y3.n >= 0x10) {
         sprite = EVT.sprite;
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 336
void Evtf336_Salamander_Segment(EvtData *evt) {
   static s16 animData[20] = {7, GFX_FLAME_1, 1, GFX_FLAME_2, 1, GFX_FLAME_3, 1, GFX_FLAME_4,
                              1, GFX_FLAME_5, 1, GFX_FLAME_6, 1, GFX_FLAME_7, 1, GFX_FLAME_8,
                              1, GFX_NULL,    1, GFX_NULL};
   static Quad quad = {{-16, -48, 0, 0}, {16, -48, 0, 0}, {-16, 16, 0, 0}, {16, 16, 0, 0}};

   SVECTOR vector_unused;
   Quad quad_unused = {{-32, -32, 0, 0}, {32, -32, 0, 0}, {-32, 32, 0, 0}, {32, 32, 0, 0}};

   EvtData *fx1;
   EvtData *sprite;
   EvtData *link;
   EvtData *flamingRock;
   s32 ct;
   s32 i;
   s32 randomAngle;
   s16 theta;
   s32 dx, dy, dz;

   fx1 = EVT.parent;

   switch (evt->state) {
   case 0:
      evt->y1.n += CV(2.0);

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_GLOBE_5;
      sprite->d.sprite.boxIdx = 3;
      sprite->d.sprite.animData = animData;

      ct = (rand() >> 2) % 0x10;
      for (i = 0; i < ct; i++) {
         UpdateEvtAnimation(sprite);
      }

      sprite->d.sprite.semiTrans = 2;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      link = EVT.link;

      if (evt->mem < 9) {
         evt->mem = link->mem;
      }

      EVT.position4.x = EVT.position3.x;
      EVT.position3.x = EVT.position2.x;
      EVT.position2.x = EVT.position1.x;
      EVT.position1.x = evt->x1.n;
      EVT.position4.y = EVT.position3.y;
      EVT.position3.y = EVT.position2.y;
      EVT.position2.y = EVT.position1.y;
      EVT.position1.y = evt->y1.n;
      EVT.position4.z = EVT.position3.z;
      EVT.position3.z = EVT.position2.z;
      EVT.position2.z = EVT.position1.z;
      EVT.position1.z = evt->z1.n;

      EVT.todo_x2c = 0x40 + (0x20 * rsin(EVT.todo_x2e) >> 12);
      EVT.todo_x2e = (EVT.todo_x2e + 0x40) & 0xfff;
      evt->x1.n = link->d.evtf336.position1.x;
      evt->z1.n = link->d.evtf336.position1.z;
      evt->y1.n = link->d.evtf336.position1.y;
      EVT.todo_x2c = link->d.evtf336.todo_x2c;
      EVT.theta2 = 0;
      EVT.todo_x2a = link->d.evtf336.todo_x2a;
      EVT.todo_x28 = link->d.evtf336.todo_x28 + EVT.theta1;
      vector_unused.vy = EVT.todo_x2c * rsin(EVT.todo_x2a) >> 12;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      if (evt->mem != 9) {
         dx = link->x1.n - evt->x1.n;
         dz = link->z1.n - evt->z1.n;
         dy = link->y1.n - evt->y1.n;
         theta = ratan2(dy, SquareRoot0(dx * dx + dz * dz));
         for (i = 0; i < 4; i++) {
            gQuad_800fe63c[i].vx = (quad[i].vx * rcos(theta) - quad[i].vy * rsin(theta)) >> 12;
            gQuad_800fe63c[i].vy = (quad[i].vx * rsin(theta) + quad[i].vy * rcos(theta)) >> 12;
         }
         UpdateEvtAnimation(sprite);
      }

      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (evt->mem == 99) {
         if (fx1->d.evtf334.todo_x24 < 2) {
            fx1->d.evtf334.todo_x24++;
         } else {
            if (Evt_CountUnused() > 100) {
               randomAngle = rand() % DEG(360);
               flamingRock = CreatePositionedEvt(evt, EVTF_FLAMING_ROCK);
               flamingRock->y2.n = (rand() & 0x3f) + 0x10;
               flamingRock->x2.n = (0x40 * rsin(randomAngle) >> 12);
               flamingRock->z2.n = (0x40 * rcos(randomAngle) >> 12);
               flamingRock->y3.n = -12;
               fx1->d.evtf334.todo_x24 = 0;
            }
         }
         evt->state++;
      }

      break;

   case 2:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 377
void Evtf377_Fx_TBD(EvtData *evt) {
   SVECTOR vector;
   s16 headGfx[8] = {GFX_SALAMANDER_S, GFX_SALAMANDER_SE, GFX_SALAMANDER_E, GFX_SALAMANDER_NE,
                     GFX_SALAMANDER_N, GFX_SALAMANDER_NE, GFX_SALAMANDER_E, GFX_SALAMANDER_SE};
   Quad quad = {{-32, -32, 0, 0}, {32, -32, 0, 0}, {-32, 32, 0, 0}, {32, 32, 0, 0}};

   EvtData *sprite;
   EvtData *evt_s4; //? evtf unknown - treating as evtf335
   EvtData *evt_s1;
   s16 dir;
   s32 i;
   s32 iVar3, x, z;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_GLOBE_5;
      sprite->d.sprite.boxIdx = 3;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      evt_s4 = EVT.todo_x5c;

      dir = (((gCameraRotation.vy - EVT.theta3) & 0xfff) / DEG(45)) & 7;
      sprite->d.sprite.gfxIdx = headGfx[dir];

      if (dir > 4) {
         sprite->d.sprite.facingLeft = 1;
      } else {
         sprite->d.sprite.facingLeft = 0;
      }

      if (evt->mem == 0) {
         if (evt_s4->mem != 2) {
            sprite->d.sprite.boxIdx = 7;
            for (i = 0; i < 4; i++) {
               gQuad_800fe63c[i] = quad[i];
            }
            EVT.radius = CV(0.5) + (CV(0.125) * rsin(EVT.theta5) >> 12);
            EVT.theta5 = (EVT.theta5 + DEG(5.625)) & 0xfff;
            EVT.theta2 = evt_s4->d.evtf335.theta2;
            gCameraZoom.vz = 512;
            PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 2);
            gCameraRotation.vy += (16 * rsin(EVT.theta4 >> 1) >> 12);
         } else {
            evt_s1 = CreatePositionedEvt(evt, EVTF_FLAME_BREATH_PARTICLE);
            evt_s1->d.sprite.boxIdx = 3;
            evt_s1->x2.n = CV(0.28125) * rcos(EVT.theta3) >> 12;
            evt_s1->z2.n = CV(0.28125) * rsin(EVT.theta3) >> 12;
            evt_s1->x2.n += rand() % 13 - 6;
            evt_s1->z2.n += rand() % 13 - 6;
            evt_s1->y2.n = CV(0.15625) * rsin(DEG(67.5) * rsin(evt->state2) >> 12) >> 12;
            evt_s1->x3.n = evt_s1->x2.n >> 2;
            evt_s1->z3.n = evt_s1->z2.n >> 2;
            evt_s1->y3.n = evt_s1->y2.n >> 2;
            evt->state2 += 0x20;
            gCameraRotation.vy += DEG(1.40625) * rsin(EVT.theta4 >> 1) >> 12;
            PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 2);
            gCameraZoom.vz = 512;
         }
      }

      EVT.theta3 = evt_s4->d.evtf335.todo_x28 + EVT.theta1;
      EVT.theta4 = evt_s4->d.evtf335.todo_x2a + EVT.theta2;

      iVar3 = EVT.radius * rsin(EVT.theta4) >> 12;
      vector.vy = iVar3;
      iVar3 = EVT.radius * rcos(EVT.theta4) >> 12;
      x = iVar3 * rcos(EVT.theta3) >> 12;
      z = iVar3 * rsin(EVT.theta3) >> 12;

      vector.vx = x;
      vector.vz = z;

      evt->x1.n = evt_s4->x1.n + (vector.vx);
      evt->z1.n = evt_s4->z1.n + (vector.vz);
      evt->y1.n = evt_s4->y1.n + (vector.vy);

      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      break;

   case 2:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 331
void Evtf331_Fx_TBD(EvtData *evt) {
   EvtData *parent = EVT.parent;

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_RED_X_BTM_RIGHT;
      EVT.boxIdx = 4;
      evt->state++;

   // fallthrough
   case 1:
      if (evt->state2 == 1) {
         evt->y1.n = parent->y1.n + (CV(1.5) * rsin(DEG(-45)) >> 12);
         evt->x1.n = parent->x1.n + CV(0.5);
         evt->z1.n = parent->z1.n;
      } else {
         evt->y1.n = parent->y1.n + (CV(1.5) * rsin(DEG(-135)) >> 12);
         evt->x1.n = parent->x1.n - CV(0.5);
         evt->z1.n = parent->z1.n;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      break;
   }
}

#undef EVTF
#define EVTF 747
void Evtf747_748_Wyrmfang_Flames(EvtData *evt) {
   EvtData *flame;

   if (evt->functionIndex == EVTF_WYRMFANG_FLAMES_CW) {
      EVT.theta += DEG(8.4375);
      EVT.radius += 8;
   } else {
      EVT.theta -= DEG(8.4375);
      EVT.radius += 8;
   }
   if (EVT.radius > CV(6.0)) {
      EVT.radius = 0;
   }

   EVT.todo_x2c = (EVT.todo_x2c + 280) & 0x7ff;
   EVT.amplitude = 1200 * rcos((EVT.todo_x2c - DEG(90)) & 0xfff) >> 12;

   switch (evt->state) {
   case 0:
      EVT.timer = 1;
      if (evt->functionIndex == EVTF_WYRMFANG_FLAMES_CW) {
         EVT.radius = 0;
      } else {
         EVT.radius = CV(2.0);
      }
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer == 0) {
         EVT.timer = 1;
         flame = CreatePositionedEvt(evt, EVTF_WYRMFANG_FLAME);
         flame->d.evtf749.theta = EVT.theta;
         flame->d.evtf749.radius = EVT.radius;
         flame->d.evtf749.amplitude = EVT.amplitude;
      }
      break;
   }
}

// Used by: Evtf332_RollingFire_FX1, Evtf749_Wyrmfang_Flame
s16 gFlameAnimData_800ff8a4[20] = {0, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4,
                                   2, GFX_FLAME_5, 2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8,
                                   2, GFX_NULL,    1, GFX_NULL};

#undef EVTF
#define EVTF 749
void Evtf749_Wyrmfang_Flame(EvtData *evt) {
   EvtData *sprite;

   sprite = EVT.sprite;
   if (sprite != NULL) {
      UpdateEvtAnimation(sprite);
      AddEvtPrim3(gGraphicsPtr->ot, sprite);
   }

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = gFlameAnimData_800ff8a4;
      sprite->d.sprite.semiTrans = 1;
      EVT.sprite = sprite;
      sprite->d.sprite.coords[0].x =
          evt->x1.n + (rcos((EVT.theta - DEG(11.25)) & 0xfff) * EVT.radius >> 12);
      sprite->d.sprite.coords[0].z =
          evt->z1.n + (rcos((EVT.theta + DEG(78.75)) & 0xfff) * EVT.radius >> 12);
      sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
      sprite->d.sprite.coords[1].x =
          evt->x1.n + (rcos((EVT.theta + DEG(11.25)) & 0xfff) * EVT.radius >> 12);
      sprite->d.sprite.coords[1].z =
          evt->z1.n + (rcos((EVT.theta + DEG(101.25)) & 0xfff) * EVT.radius >> 12);
      sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;
      sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;
      sprite->d.sprite.coords[0].y = evt->y1.n;
      sprite->d.sprite.coords[1].y = evt->y1.n;
      sprite->d.sprite.coords[2].y = evt->y1.n;
      sprite->d.sprite.coords[3].y = evt->y1.n;
      sprite->x1.n = sprite->d.sprite.coords[2].x;
      sprite->z1.n = sprite->d.sprite.coords[2].z;
      sprite->y1.n = sprite->d.sprite.coords[2].y;
      evt->state++;

   // fallthrough
   case 1:
      EVT.yTheta += DEG(4.21875);
      if (EVT.yTheta > DEG(180)) {
         EVT.yTheta = 0;
         evt->state++;
      }
      sprite->d.sprite.coords[0].y =
          evt->y1.n + (rcos((EVT.yTheta - DEG(90)) & 0xfff) * EVT.amplitude >> 12);
      sprite->d.sprite.coords[1].y = sprite->d.sprite.coords[0].y;
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      sprite->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 306
void Evtf306_791_792_793_Healing_FX2(EvtData *evt) {
   // 306: Extra Healing, Hyper Healing
   // 791: Healing Plus
   // 792: Ultra Healing
   // 793: Supreme Healing, Holy H2O
   EvtData *unitSprite;
   EvtData *evt_s0;
   MaskEffectPreset maskEffect;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;
      EVT.unitSprite = unitSprite;
      unitSprite->d.sprite.hidden = 1;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      maskEffect.srcGfxIdx = GFX_TILED_FLAMES;
      maskEffect.dstGfxIdx = GFX_TBD_25;
      maskEffect.width = 0;
      maskEffect.height = EVT.timer % 0x40;
      maskEffect.semiTrans = 2;
      maskEffect.clut = CLUT_BLUES;
      maskEffect.color.r = maskEffect.color.g = maskEffect.color.b =
          0x80 * rsin(EVT.timer * 0x20) >> 12;

      switch (evt->functionIndex) {
      case EVTF_ULTRA_HEALING_FX2:
         maskEffect.srcGfxIdx = GFX_TILED_MAGMA;
         maskEffect.clut = CLUT_REDS;
         break;

      case EVTF_HEALING_FX2:
         // Probably left-over, since this has a dedicated function (Evtf100_Healing_FX2)
         break;

      case EVTF_SUPREME_HEALING_FX2: // or Holy H2O
         maskEffect.srcGfxIdx = GFX_TILED_CLOUDS;
         maskEffect.clut = CLUT_GRAYS;
         break;

      case EVTF_EXTRA_HEALING_FX2: // or Hyper Healing
         maskEffect.srcGfxIdx = GFX_TILED_RED_SPARKLES;
         maskEffect.clut = CLUT_REDS;
         break;

      case EVTF_HEALING_PLUS_FX2:
         maskEffect.clut = CLUT_REDS;
         break;
      }

      ApplyMaskEffectPreset(unitSprite, &maskEffect);

      evt_s0 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s0);
      evt_s0->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s0, 0);
      evt_s0->functionIndex = EVTF_NULL;

      if (--evt->mem <= 0) {
         evt_s0 = CreatePositionedEvt(unitSprite, EVTF_HEALING_SPARKLE_386);

         switch (evt->functionIndex) {
         case EVTF_ULTRA_HEALING_FX2:
            evt_s0->d.evtf386.clut = CLUT_PURPLES;
            evt_s0->y3.n = (rand() % 8 + 6) * 2;
            evt->mem = (rand() + 2) & 3;
            evt->mem = (rand() + 1) & 5;
            break;

         case EVTF_SUPREME_HEALING_FX2: // or Holy H2O
            evt_s0->d.evtf386.clut = CLUT_GREENS;
            evt_s0->y3.n = (rand() % 10 + 7) * 2;
            evt->mem = (rand() + 1) & 3;
            break;

         case EVTF_HEALING_PLUS_FX2:
         case EVTF_HEALING_FX2:
         case EVTF_EXTRA_HEALING_FX2: // or Hyper Healing
         default:
            evt_s0->d.evtf386.clut = CLUT_BLUES;
            evt->mem = (rand() + 3) & 5;
            evt_s0->y3.n = (rand() % 8 + 5) * 2;
            break;
         }
      }

      if (EVT.timer == 30) {
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_DISPLAY_DAMAGE_2;
         evt_s0->x1.s.hi = unitSprite->x1.s.hi;
         evt_s0->z1.s.hi = unitSprite->z1.s.hi;
      }

      if (++EVT.timer == 61) {
         unitSprite->d.sprite.hidden = 0;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef EVTF
#define EVTF 386
void Evtf386_HealingSparkle(EvtData *evt) {
   static s16 sparkleAnimData[20] = {
       5, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5,
       1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_NULL,      1, GFX_NULL};

   s32 skip;
   s32 i;
   s16 theta;

   switch (evt->state) {
   case 0:
      evt->x2.n = evt->x1.n;
      evt->z2.n = evt->z1.n;
      evt->y2.n = evt->y1.n;
      EVT.animData = sparkleAnimData;

      skip = rand() & 7;
      for (i = 0; i < skip; i++) {
         UpdateEvtAnimation(evt);
      }

      evt->state2 = rand() & 0xfff;
      evt->mem = (rand() + 0x20) & 0x3f;

      if (evt->y3.n == 0) {
         evt->y3.n = (rand() % 6 + 4) * 2;
      }

      EVT.timer = 20;
      evt->state++;

   // fallthrough
   case 1:
      theta = evt->state2;
      evt->x1.n = evt->x2.n + (rcos(theta) >> 5);
      evt->z1.n = evt->z2.n + (rsin(theta) >> 5);
      evt->y1.n = evt->y2.n;

      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      evt->y2.n += evt->y3.n;
      evt->state2 += evt->mem;

      if (--EVT.timer <= 0) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 289
void Evtf289_ChimneySmokeRing(EvtData *evt) {
   s16 timer;
   s16 halfSize;
   s16 prevQuadIdx;
   s16 fade;
   POLY_FT4 *poly;
   s32 rgb;

   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_SWIRLY_RING;
      evt->d.sprite.boxIdx = 7;
      evt->d.sprite.clut = CLUT_GRAYS;
      evt->d.sprite.semiTrans = 2;
      evt->d.sprite.coords[0].x = evt->d.sprite.coords[1].x = evt->x1.n - CV(0.125);
      evt->d.sprite.coords[2].x = evt->d.sprite.coords[3].x = evt->x1.n + CV(0.125);
      evt->d.sprite.coords[0].z = evt->d.sprite.coords[2].z = evt->z1.n - CV(0.125);
      evt->d.sprite.coords[1].z = evt->d.sprite.coords[3].z = evt->z1.n + CV(0.125);
      evt->state++;

   // fallthrough
   case 1:
      timer = evt->state2 * 2;

      halfSize = timer + CV(0.125);
      evt->d.sprite.coords[0].x = evt->d.sprite.coords[1].x = evt->x1.n - halfSize;
      evt->d.sprite.coords[2].x = evt->d.sprite.coords[3].x = evt->x1.n + halfSize;
      evt->d.sprite.coords[0].z = evt->d.sprite.coords[2].z = evt->z1.n - halfSize;
      evt->d.sprite.coords[1].z = evt->d.sprite.coords[3].z = evt->z1.n + halfSize;

      evt->y1.n += evt->y2.n;
      evt->d.sprite.coords[0].y = evt->d.sprite.coords[1].y = evt->d.sprite.coords[2].y =
          evt->d.sprite.coords[3].y = evt->y1.n;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim4(gGraphicsPtr->ot, evt);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         fade = (0x80 - timer) - timer;
         rgb = (fade << 16) + (fade << 8) + fade;
         *(u32 *)&poly->r0 = ((GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS) << 24) + rgb;
      }

      if (++evt->state2 >= 32) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 302
void Evtf302_ChimneySmoke(EvtData *evt) {
   EvtData *ring;

   switch (evt->state) {
   case 0:
      evt->y1.n = GetMapModelElevation(evt->z1.s.hi, evt->x1.s.hi) + CV(0.25);
      evt->state++;
      evt->state2 = 0;

   // fallthrough
   case 1:
      if (++evt->state2 > 27) {
         ring = CreatePositionedEvt(evt, EVTF_CHIMNEY_SMOKE_RING);
         ring->y2.n = 6;
         evt->state2 = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 675
void Evtf675_LeenaForcefield(EvtData *evt) {
   // Spawned by: EVDATA74.DAT->271->675, SetupMapExtras()->256->675
   static u8 cluts[4] = {3, 4, 8, 9};

   EvtData *targetSprite;
   s32 i;
   s32 x, z;
   s32 currentSize;
   s32 halfSize;
   s32 theta;

   switch (evt->state) {
   case 0:
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.targetSprite;
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      EVT.targetSprite = NULL;
      EVT.gfxIdx = GFX_RING;
      EVT.boxIdx = 7;
      EVT.clut = CLUT_BLUES;
      EVT.semiTrans = 4;
      evt->y2.n = evt->y1.n;
      evt->state++;

   // fallthrough
   case 2:
      theta = evt->mem;
      x = evt->x1.n;
      z = evt->z1.n;

      evt->state3 += (CV(2.0) - evt->state3) >> 4;
      currentSize = evt->state3;
      EVT.clut = cluts[(evt->mem >> 4) & 3];

      for (i = 0; i < 4; i++) {
         halfSize = currentSize * rcos(theta) >> 12;
         EVT.coords[0].y = EVT.coords[1].y = EVT.coords[2].y = EVT.coords[3].y =
             evt->y2.n + (CV(2.0) * rsin(theta) >> 12);

         EVT.coords[0].x = x + halfSize;
         EVT.coords[0].z = z;
         EVT.coords[1].x = x;
         EVT.coords[1].z = z + halfSize;
         EVT.coords[2].x = x;
         EVT.coords[2].z = z - halfSize;
         EVT.coords[3].x = x - halfSize;
         EVT.coords[3].z = z;

         evt->y1.n = EVT.coords[0].y;
         AddEvtPrim3(gGraphicsPtr->ot, evt, 0); //? Extra arg

         theta += DEG(22.5);
         if (theta >= DEG(90)) {
            theta -= DEG(90);
         }
      }

      evt->mem += DEG(1.58203125);
      if (evt->mem >= DEG(90)) {
         evt->mem = 0;
      }

      break;
   }
}

#undef EVTF
#define EVTF 676
void Evtf676_687_Rainfall(EvtData *evt) {
   // Spawned by: EVDATA86.DAT, EVDATA88.DAT, SetupMapExtras()
   s32 i;
   EvtData *raindrop;

   switch (evt->state) {
   case 0:
      if (evt->functionIndex == EVTF_LIGHT_RAINFALL) {
         evt->state2 = 1;
      }
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         for (i = 0; i < 8; i++) {
            raindrop = Evt_GetUnused();
            raindrop->functionIndex = EVTF_RAINFALL_DROP;
            raindrop->x1.n = rand() % CV(24.0);
            raindrop->z1.n = rand() % CV(12.0);
            raindrop->y1.n = rand() % CV(1.0) + CV(6.0);
         }
         if (evt->state2 != 0) {
            // EVTF_LIGHT_RAINFALL
            evt->mem += (rand() % 4 + 2);
         } else {
            // EVTF_HEAVY_RAINFALL
            evt->mem += (rand() % 2 + 1);
         }
      }

   case 2:
      break;
   }
}

#undef EVTF
#define EVTF 677
void Evtf677_RainfallDrop(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_COLORS;
      evt->d.sprite.clut = CLUT_BLUES;
      evt->d.sprite.coords[0].x = evt->d.sprite.coords[1].x = evt->d.sprite.coords[2].x =
          evt->d.sprite.coords[3].x = evt->x1.n;
      evt->d.sprite.coords[0].z = evt->d.sprite.coords[1].z = evt->d.sprite.coords[2].z =
          evt->d.sprite.coords[3].z = evt->z1.n;
      evt->d.sprite.coords[0].x = evt->x1.n + 4;
      evt->d.sprite.coords[1].x = evt->x1.n + 4;
      evt->d.sprite.coords[2].x = evt->x1.n - 4;
      evt->d.sprite.coords[3].x = evt->x1.n - 4;
      evt->d.sprite.semiTrans = 4;
      evt->d.sprite.coords[0].y = evt->d.sprite.coords[2].y =
          evt->y1.n + (rand() % CV(0.25) + CV(1.0));
      evt->d.sprite.coords[1].y = evt->d.sprite.coords[3].y = evt->y1.n;
      evt->y2.n = CV(-0.5) - rand() % CV(0.25);
      evt->y3.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->state++;
      break;

   case 1:
      evt->d.sprite.coords[0].y += evt->y2.n;
      evt->d.sprite.coords[1].y += evt->y2.n;
      evt->d.sprite.coords[2].y += evt->y2.n;
      evt->d.sprite.coords[3].y += evt->y2.n;
      AddEvtPrim3(gGraphicsPtr->ot, evt);
      evt->y1.n += evt->y2.n;
      if (evt->y1.n <= evt->y3.n) {
         evt->state++;
      }
      break;

   case 2:
      if ((evt->x1.s.hi < D_80122E28) || (evt->x1.s.hi > gMapSizeX + D_80122E28 - 1) ||
          (evt->z1.s.hi < D_80122E2C) || (evt->z1.s.hi > gMapSizeZ + D_80122E2C - 1)) {
         evt->functionIndex = EVTF_NULL;
         break;
      }

      if ((OBJ_TERRAIN(evt).s.terrain == TERRAIN_WATER)) {
         CreatePositionedEvt(evt, EVTF_RIPPLE);
      }

      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 678
void Evtf678_Ripple(EvtData *evt) {
   s16 timer;
   s16 halfSize;
   s16 prevQuadIdx;
   s16 fade;
   POLY_FT4 *poly;
   s32 rgb;

   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_RING;
      evt->d.sprite.clut = CLUT_GRAYS;
      evt->d.sprite.semiTrans = 2;
      evt->d.sprite.coords[0].x = evt->d.sprite.coords[1].x = evt->x1.n - CV(0.125);
      evt->d.sprite.coords[2].x = evt->d.sprite.coords[3].x = evt->x1.n + CV(0.125);
      evt->d.sprite.coords[0].z = evt->d.sprite.coords[2].z = evt->z1.n - CV(0.125);
      evt->d.sprite.coords[1].z = evt->d.sprite.coords[3].z = evt->z1.n + CV(0.125);
      evt->d.sprite.otOfs = 8;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + CV(0.015625);
      evt->state++;

   // fallthrough
   case 1:
      timer = evt->state2 * 2;

      halfSize = timer + CV(0.125);
      evt->d.sprite.coords[0].x = evt->d.sprite.coords[1].x = evt->x1.n - halfSize;
      evt->d.sprite.coords[2].x = evt->d.sprite.coords[3].x = evt->x1.n + halfSize;
      evt->d.sprite.coords[0].z = evt->d.sprite.coords[2].z = evt->z1.n - halfSize;
      evt->d.sprite.coords[1].z = evt->d.sprite.coords[3].z = evt->z1.n + halfSize;

      evt->y1.n += evt->y2.n;
      evt->d.sprite.coords[0].y = evt->d.sprite.coords[1].y = evt->d.sprite.coords[2].y =
          evt->d.sprite.coords[3].y = evt->y1.n;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim4(gGraphicsPtr->ot, evt);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         fade = (0x80 - timer) - timer;
         rgb = (fade << 16) + (fade << 8) + fade;
         *(u32 *)&poly->r0 = ((GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS) << 24) + rgb;
      }

      if (++evt->state2 >= 32) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

static Quad sQuad_800ff8f4 = {{-24, 0, -24, 0}, {24, 0, -24, 0}, {-24, 0, 24, 0}, {24, 0, 24, 0}};

#undef EVTF
#define EVTF 692
void Evtf692_Campfire(EvtData *evt) {
   // Spawned by: EVDATA25.DAT, EVDATA67.DAT
   static s16 animData[12] = {4, GFX_CAMPFIRE_1, 2, GFX_CAMPFIRE_2, 2, GFX_CAMPFIRE_3,
                              2, GFX_CAMPFIRE_4, 2, GFX_NULL,       1, GFX_NULL};

   EvtData *sprite;
   EvtData *entitySprite;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      entitySprite = EVT.entitySprite;
      sprite->x1.n = entitySprite->x1.n;
      sprite->z1.n = entitySprite->z1.n;
      sprite->d.sprite.animData = animData;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 1);
      break;
   }
}

#undef EVTF
#define EVTF 763
void Evtf763_BoulderRubble(EvtData *evt) {
   s32 i;

   for (i = 0; i < 32; i++) {
      CreatePositionedEvt(evt, EVTF_ROCK_SPURT_PARTICLE);
      CreatePositionedEvt(evt, EVTF_RUBBLE);
   }

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 759
void Evtf759_RockSpurtParticle(EvtData *evt) {
   static s16 rockAnimData[12] = {5, GFX_ROCK_1, 3, GFX_ROCK_2, 3, GFX_ROCK_3,
                                  3, GFX_ROCK_4, 3, GFX_NULL,   1, GFX_NULL};

   s16 elevation;
   s16 prevQuadIdx;
   POLY_FT4 *poly;
   s32 theta;
   s32 radius;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = rockAnimData;
      theta = rand() % DEG(360);

      if (evt->state2 != 0) {
         radius = rand() % 15 + CV(0.375);
         evt->y2.n = rand() % CV(0.125) + CV(0.25);
         evt->y3.n = -2;
      } else {
         radius = rand() % 15 + 8;
         evt->y3.n = -8;
         evt->y2.n = rand() % CV(0.125) + CV(0.25);
      }

      evt->x2.n = radius * rcos(theta) >> 12;
      evt->z2.n = radius * rsin(theta) >> 12;
      evt->x3.n = -(evt->x2.n >> 2);
      evt->z3.n = -(evt->z2.n >> 2);
      evt->state++;
      evt->mem = rand() % 8;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         evt->mem = 32;
         UpdateEvtAnimation(evt);
         evt->state++;
      }
      break;

   case 2:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      evt->y2.n += evt->y3.n;

      elevation = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      if (evt->y1.n < elevation) {
         evt->y1.n = elevation;
         evt->y2.n = -(evt->y2.n / 2);
      }

      UpdateEvtAnimation(evt);

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (gQuadIndex != prevQuadIdx && evt->d.sprite.clut != 0) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         poly->clut = evt->d.sprite.clut;
      }

      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

/*void RotateQuadXY(Quad src, Quad dst, s16 theta) {
   //? Or just operate directly on SVECTOR* args (to avoid obscured array args)
   SVECTOR *pSrc = &src[0];
   SVECTOR *pDst = &dst[0];
   s32 i;

   for (i = 0; i < 4; i++) {
      pDst->vx = (pSrc->vx * rcos(theta) - pSrc->vy * rsin(theta)) >> 12;
      pDst->vy = (pSrc->vx * rsin(theta) + pSrc->vy * rcos(theta)) >> 12;
      pDst++;
      pSrc++;
   }
}*/

void RotateQuadXY(SVECTOR *src, SVECTOR *dst, s16 theta) {
   s32 i;

   for (i = 0; i < 4; i++) {
      dst->vx = (src->vx * rcos(theta) - src->vy * rsin(theta)) >> 12;
      dst->vy = (src->vx * rsin(theta) + src->vy * rcos(theta)) >> 12;
      dst++;
      src++;
   }
}

void RotateQuadYZ(SVECTOR *src, SVECTOR *dst, s16 theta) {
   s32 i;

   for (i = 0; i < 4; i++) {
      dst->vy = (src->vy * rcos(theta) - src->vz * rsin(theta)) >> 12;
      dst->vz = (src->vy * rsin(theta) + src->vz * rcos(theta)) >> 12;
      dst->vx = src->vx;
      dst++;
      src++;
   }
}

void RotateQuadXZ(SVECTOR *src, SVECTOR *dst, s16 theta) {
   s32 i;

   for (i = 0; i < 4; i++) {
      dst->vx = (src->vx * rcos(theta) - src->vz * rsin(theta)) >> 12;
      dst->vz = (src->vx * rsin(theta) + src->vz * rcos(theta)) >> 12;
      dst->vy = src->vy;
      dst++;
      src++;
   }
}

#undef EVTF
#define EVTF 272
void Evtf272_StatRaised(EvtData *evt) {
   EvtData *unitSprite;
   s32 theta;
   s32 w, h;
   s32 tmp;

   switch (evt->state) {
   case 0:
      if (evt->mem == 0) {
         evt->mem = 0x40;
      }
      evt->state3 = evt->mem;
      EVT.boxIdx = 7;
      EVT.animRelativeOfs = -8;
      EVT.animYOfs = 8;

      switch (EVT.type) {
      case RAISED_STAT_LV:
         EVT.statGfxIdx = GFX_LV;
         break;
      case RAISED_STAT_AT:
         EVT.statGfxIdx = GFX_AT;
         break;
      case RAISED_STAT_DF:
         EVT.statGfxIdx = GFX_DF;
         break;
      case RAISED_STAT_MV:
         EVT.statGfxIdx = GFX_MV;
         break;
      default:
         EVT.statGfxIdx = GFX_LV;
         break;
      }

      EVT.upGfxIdx = GFX_UP; // "up!"
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n + CV(1.5);

      tmp = evt->state3 - evt->mem;
      if (tmp >= 12) {
         tmp = 12;
      }
      theta = tmp * DEG(11.25);
      w = abs(12 * rsin(theta) >> 12) + 12;
      h = abs(8 * rsin(theta) >> 12) + 8;
      rsin(theta);

      gQuad_800fe63c[0].vz = gQuad_800fe63c[1].vz = gQuad_800fe63c[2].vz = gQuad_800fe63c[3].vz = 0;

      EVT.gfxIdx = EVT.statGfxIdx;
      gQuad_800fe63c[0].vx = -w;
      gQuad_800fe63c[0].vy = -h / 2;
      gQuad_800fe63c[1].vx = 0;
      gQuad_800fe63c[1].vy = -h / 2;
      gQuad_800fe63c[2].vx = -w;
      gQuad_800fe63c[2].vy = h / 2;
      gQuad_800fe63c[3].vx = 0;
      gQuad_800fe63c[3].vy = h / 2;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      EVT.gfxIdx = EVT.upGfxIdx;
      gQuad_800fe63c[0].vx = 0;
      gQuad_800fe63c[0].vy = -h / 2;
      gQuad_800fe63c[1].vx = w;
      gQuad_800fe63c[1].vy = -h / 2;
      gQuad_800fe63c[2].vx = 0;
      gQuad_800fe63c[2].vy = h / 2;
      gQuad_800fe63c[3].vx = w;
      gQuad_800fe63c[3].vy = h / 2;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 380
void Evtf380_LevelUpFx(EvtData *evt) {
   static Quad quad = {{-32, -32, 0, 0}, {32, -32, 0, 0}, {-32, 32, 0, 0}, {32, 32, 0, 0}};

   u8 cluts[4] = {3, 4, 8, 0};

   EvtData *evt_s2;
   EvtData *unitSprite;
   EvtData *statRaised;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;
   s32 gfxX, gfxY, gfxW, gfxH;
   s32 i;
   s16 halfSize;
   s16 theta;
   s16 radius;
   s16 fade;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      EVT.timer = 0;

      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      EVT.sprite = evt_s2;
      evt_s2->d.sprite.gfxIdx = GFX_JP_TBD_245;
      evt_s2->d.sprite.semiTrans = 4;
      evt_s2->d.sprite.boxIdx = 7;
      evt_s2->d.sprite.clut = CLUT_REDS;
      evt_s2->x1.n = evt->x1.n;
      evt_s2->z1.n = evt->z1.n;
      evt_s2->y1.n = evt->y1.n;

      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 1;
      EVT.unitSprite = unitSprite;

      statRaised = CreatePositionedEvt(evt_s2, EVTF_STAT_RAISED);
      statRaised->d.evtf272.unitSprite = unitSprite;

      evt->state2 = 0;
      evt->mem = 3;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 400 << 2, 320, 0,
                      (EVT.timer * 2) % 64, GFX_TBD_25, 0);
      EVT.timer++;

      evt_s2 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s2);
      evt_s2->d.sprite.hidden = 0;
      evt_s2->d.sprite.semiTrans = 2;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];

      gfxX = gGfxSubTextures[GFX_TBD_25].x;
      gfxY = gGfxSubTextures[GFX_TBD_25].y;
      gfxW = gGfxSubTextures[GFX_TBD_25].w;
      gfxH = gGfxSubTextures[GFX_TBD_25].h;
      setUVWH(poly, gfxX, gfxY, gfxW, gfxH);

      poly->clut = gClutIds[cluts[EVT.timer % 3]];
      poly->tpage = GetTPage(0, 1, 576, 256);

      fade = 0x60 + (0x20 * rsin(EVT.timer * 0x80) >> 12);
      setRGB0(poly, fade, fade, fade);

      CopyEvtData(unitSprite, evt_s2);
      evt_s2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s2, 0);
      evt_s2->functionIndex = EVTF_NULL;

      halfSize = 0x80 * rsin(evt->state2 * 2) >> 12;
      quad[0].vx = quad[2].vx = -halfSize;
      quad[1].vx = quad[3].vx = halfSize;
      quad[0].vy = quad[1].vy = -halfSize;
      quad[2].vy = quad[3].vy = halfSize;
      RotateQuadXY(quad, gQuad_800fe63c, evt->state2);

      evt->state2 += 0x20;

      if (--evt->mem <= 0) {
         for (i = 0; i < 2; i++) {
            theta = rand() % DEG(360);
            radius = rand() % 4 + 12;
            evt_s2 = Evt_GetUnused();
            evt_s2->functionIndex = EVTF_FAINT_SPARKLES;
            evt_s2->x2.n = radius * rcos(theta) >> 12;
            evt_s2->z2.n = radius * rsin(theta) >> 12;
            evt_s2->d.evtf278.targetSprite = unitSprite;
         }
         evt->mem = rand() % 5;
         evt->state3++;
      }
      if (evt->state3 >= 30) {
         evt->state3 = 0;
         unitSprite = EVT.unitSprite;
         unitSprite->d.sprite.hidden = 0;
         evt->state++;
      }
      break;

   case 2:
      evt->state++;

   // fallthrough
   case 3:
      if (++evt->state3 >= 40) {
         evt_s2 = EVT.sprite;
         evt_s2->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 278
void Evtf278_FaintSparkles(EvtData *evt) {
   static s16 sparkleAnimData[14] = {7, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};

   EvtData *targetSprite;
   EvtData *sprite;
   Quad quad;
   s32 i;

   targetSprite = EVT.targetSprite;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_LIT_SPHERE_8;
      sprite->d.sprite.boxIdx = 7;
      if (EVT.clut == CLUT_NULL) {
         sprite->d.sprite.clut = CLUT_BLUES;
      } else {
         sprite->d.sprite.clut = EVT.clut;
      }
      sprite->d.sprite.semiTrans = 4;
      sprite->d.sprite.animData = sparkleAnimData;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;
      sprite->x1.n = targetSprite->x1.n + evt->x1.n;
      sprite->z1.n = targetSprite->z1.n + evt->z1.n;
      sprite->y1.n =
          targetSprite->y1.n + evt->y1.n + (targetSprite->d.sprite.animYOfs << 3) + CV(0.5);

      for (i = 0; i < 4; i++) {
         quad[i] = gQuad_800fe63c[i];
      }
      for (i = 0; i < evt->state2; i++) {
         gQuad_800fe63c[0].vx = -i * 2;
         gQuad_800fe63c[1].vx = i * 2;
         gQuad_800fe63c[2].vx = -i * 2;
         gQuad_800fe63c[3].vx = i * 2;
         gQuad_800fe63c[0].vy = -i * 2;
         gQuad_800fe63c[1].vy = -i * 2;
         gQuad_800fe63c[2].vy = i * 2;
         gQuad_800fe63c[3].vy = i * 2;
         UpdateEvtAnimation(sprite);
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      }
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i] = quad[i];
      }

      if (++evt->state3 > 16) {
         if (--evt->mem <= 0) {
            evt->state++;
         } else {
            evt->state3 = 0;
         }
      }

      evt->state2 = abs(8 * rsin(evt->state3 * 16) >> 12) + 1;
      break;

   case 2:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 685
void Evtf685_RockSpurt(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *particle;
   s16 clut;
   s32 i;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

   if (gSpriteStripUnitIds[unitSprite->d.sprite.stripIdx - 2] == UNIT_ID_GOLD_GOLEM ||
       gSpriteStripUnitIds[unitSprite->d.sprite.stripIdx - 2] == UNIT_ID_CLAY_GOLEM) {
      clut = gSpriteStripClutIds[unitSprite->d.sprite.stripIdx];
   } else {
      clut = CLUT_NULL;
   }

   switch (evt->state) {
   case 0:
      PerformAudioCommand(AUDIO_CMD_PLAY_SFX(224));
      for (i = 0; i < 16; i++) {
         particle = CreatePositionedEvt(unitSprite, EVTF_ROCK_SPURT_PARTICLE);
         particle->y1.n += CV(1.0);
         particle->d.sprite.clut = clut;
      }
      evt->mem = 32;
      evt->state++;
      break;

   case 1:
      if (--evt->mem <= 3) {
         PerformAudioCommand(AUDIO_CMD_PLAY_SFX(225));
         for (i = 0; i < 32; i++) {
            particle = CreatePositionedEvt(unitSprite, EVTF_ROCK_SPURT_PARTICLE);
            particle->state2 = 1;
            particle->y1.n += CV(0.5);
            particle->d.sprite.clut = clut;
         }
      }
      if (evt->mem == 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 715
void Evtf715_to_718_Spellbind_FX2_FX3(EvtData *evt) {
   EvtData *infoMarker; // evtf052

   switch (evt->state) {
   case 0:
      infoMarker = CreatePositionedEvt(evt, EVTF_ATTACK_INFO_MARKER);

      switch (evt->functionIndex) {
      case EVTF_SPELLBIND_FX2:
         infoMarker->d.evtf052.type = ATK_MARKER_PARALYZED;
         infoMarker->functionIndex = EVTF_NULL;
         break;
      case EVTF_SPELLBIND_FX3:
         infoMarker->d.evtf052.type = ATK_MARKER_MISS;
         break;
      case EVTF_TBD_717:
         infoMarker->d.evtf052.type = ATK_MARKER_POISONED;
         EVT.unused_0x26 = 8;
         infoMarker->functionIndex = EVTF_NULL;
         break;
      case EVTF_TBD_718:
         infoMarker->d.evtf052.type = ATK_MARKER_MISS;
         EVT.unused_0x26 = 8;
         break;
      }

      evt->mem = 31;
      evt->state++;
      break;

   case 1:
      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 739
void Evtf739_Particle(EvtData *evt) {
   // Spawned by: 790
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

   AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

   if (--evt->mem <= 0) {
      evt->functionIndex = EVTF_NULL;
   }
}

static s16 sSparkleAnimData_800ff980[24] = {7, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3,
                                            1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5, 1, GFX_SPARKLE_1,
                                            1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4,
                                            1, GFX_SPARKLE_5, 1, GFX_NULL,      0, GFX_NULL};

#undef EVTF
#define EVTF 735
void Evtf735_SparkleDust(EvtData *evt) {
   gQuad_800fe63c[0].vx = -2;
   gQuad_800fe63c[1].vx = 2;
   gQuad_800fe63c[2].vx = -2;
   gQuad_800fe63c[3].vx = 2;
   gQuad_800fe63c[0].vy = -4;
   gQuad_800fe63c[1].vy = -4;
   gQuad_800fe63c[2].vy = 4;
   gQuad_800fe63c[3].vy = 4;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = sSparkleAnimData_800ff980;
      evt->d.sprite.clut = CLUT_REDS;
      evt->state++;

   // fallthrough
   case 1:
      UpdateEvtAnimation(evt);
      if (evt->d.sprite.animFinished) {
         evt->state++;
      } else {
         AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      }
      break;

   case 2:
      evt->d.sprite.gfxIdx = GFX_DOTS;
      evt->d.sprite.boxIdx = 7;
      evt->d.sprite.clut = CLUT_REDS;
      evt->y2.n = -8;
      evt->y3.n = -4;
      evt->mem = 32;
      evt->state++;

   // fallthrough
   case 3:
      evt->y1.n += evt->y2.n;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 740
void Evtf740_RemoveParalysis_Sparkles(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = sSparkleAnimData_800ff980;
      evt->d.sprite.clut = CLUT_REDS;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;

      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;

      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (--evt->mem <= 0) {
         evt->state++;
      }

      break;

   case 2:
      CreatePositionedEvt(evt, EVTF_SPARKLE_DUST);
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 736
void Evtf736_RemoveParalysis_Bubble(EvtData *evt) {
   EvtData *sparkles;
   s32 i;
   s32 theta;
   s32 radius;

   gQuad_800fe63c[0].vx = -2;
   gQuad_800fe63c[1].vx = 2;
   gQuad_800fe63c[2].vx = -2;
   gQuad_800fe63c[3].vx = 2;
   gQuad_800fe63c[0].vy = -4;
   gQuad_800fe63c[1].vy = -4;
   gQuad_800fe63c[2].vy = 4;
   gQuad_800fe63c[3].vy = 4;

   switch (evt->state) {
   case 0:
      evt->d.sprite.gfxIdx = GFX_EXPLOSION_1;
      evt->d.sprite.boxIdx = 5;
      evt->d.sprite.clut = CLUT_BLUES;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;

      evt->x2.n += evt->x3.n;
      evt->z2.n += evt->z3.n;
      evt->y2.n += evt->y3.n;

      evt->d.sprite.clut = 3 + (evt->mem % 2);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);

      if (--evt->mem <= 0) {
         evt->state++;
      }
      break;

   case 2:
      theta = rand() % DEG(360);
      radius = 8;

      for (i = 0; i < 4; i++) {
         sparkles = CreatePositionedEvt(evt, EVTF_REMOVE_PARALYSIS_SPARKLES);
         sparkles->x2.n = radius * rcos(theta) >> 12;
         sparkles->z2.n = radius * rsin(theta) >> 12;
         sparkles->y2.n = 0;
         sparkles->x3.n = sparkles->x2.n >> 2;
         sparkles->z3.n = sparkles->z2.n >> 2;
         sparkles->y3.n = 0;
         sparkles->mem = 4;
         theta += DEG(90);
      }

      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 737
void Evtf737_RemoveParalysis(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s1;
   s16 theta;
   s32 i;
   s32 radius;
   MaskEffectPreset unusedMaskEffect;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->y1.n += CV(1.0);
      evt->state3 = rand() % 0x1000;

      if (evt->state2 == 0) {
         evt->state2 = 0x100;
      }

      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt_s1 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s1);
      unitSprite->d.sprite.hidden = 1;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s1, 0);
      evt_s1->functionIndex = EVTF_NULL;

      evt->state++;

   // fallthrough
   case 1:
      SnapToUnit(evt);
      evt->y1.n += CV(1.0);
      if (--evt->mem <= 0) {
         theta = rand() % DEG(360);
         for (i = 0; i < 1; i++) {
            radius = 20;
            evt_s1 = CreatePositionedEvt(evt, EVTF_REMOVE_PARALYSIS_BUBBLE);
            evt_s1->x2.n = radius * rcos(theta) >> 12;
            evt_s1->z2.n = radius * rsin(theta) >> 12;
            evt_s1->y2.n = radius;
            evt_s1->x3.n = evt_s1->x2.n >> 1;
            evt_s1->z3.n = evt_s1->z2.n >> 1;
            evt_s1->y3.n = evt_s1->y2.n >> 1;
            evt_s1->mem = 4;
            theta += DEG(45);
         }
         evt->mem = 4;
      }

      unusedMaskEffect.srcGfxIdx = GFX_TILED_VEINS;
      unusedMaskEffect.dstGfxIdx = GFX_TBD_25;
      unusedMaskEffect.width = 0;
      unusedMaskEffect.height = evt->state2 % 64;
      unusedMaskEffect.semiTrans = 2;
      unusedMaskEffect.clut = CLUT_BLUES;
      unusedMaskEffect.color.r = unusedMaskEffect.color.g = unusedMaskEffect.color.b =
          abs(128 * rsin(evt->state2 * 16) >> 12);

      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt_s1 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s1);
      evt_s1->d.sprite.hidden = 0;
      evt_s1->d.sprite.animRelativeOfs = 0;
      evt_s1->d.sprite.animYOfs = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s1, 0);
      evt_s1->functionIndex = EVTF_NULL;

      if (--evt->state2 <= 0) {
         unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
         unitSprite->d.sprite.hidden = 0;
         evt->state3 = 48;
         evt->state++;
      }
      break;

   case 2:
      if (--evt->state3 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 757
void Evtf757_PushedObjectSplash(EvtData *evt) {
   EvtData *splash;
   s32 i;
   s32 theta;

   switch (evt->state) {
   case 0:
      evt->state3 = 3;
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         for (i = 0; i < 3; i++) {
            theta = rand() % DEG(360);
            splash = CreatePositionedEvt(evt, EVTF_SPLASH_WITH_DROPLETS);
            splash->x1.n += (CV(0.75) * rcos(theta) >> 12);
            splash->z1.n += (CV(0.75) * rsin(theta) >> 12);
            splash->y2.n = evt->y2.n;
         }
         evt->mem += (rand() % 8);
         evt->state3--;
      }
      if (evt->state3 == 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 760
void Evtf760_EliteMeleeSparkles(EvtData *evt) {
   // Used for Dolf and Ash in their ultimate forms
   EvtData *sparkles; // evtf278
   s32 i;
   s32 theta;
   s32 radius;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         for (i = 0; i < 5; i++) {
            radius = rand() % 12 + 4;
            theta = rand() % DEG(360);
            sparkles = Evt_GetUnused();
            sparkles->functionIndex = EVTF_FAINT_SPARKLES;
            sparkles->d.evtf278.targetSprite = EVT.unitSprite;
            sparkles->x2.n = radius * rsin(theta) >> 12;
            sparkles->z2.n = radius * rcos(theta) >> 12;
            sparkles->d.evtf278.clut = CLUT_REDS;
         }
         evt->mem += (rand() % 5);
      }
      if (++evt->state3 >= 32) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 762
void Evtf762_Megaherb(EvtData *evt) {
   // Unused?
   EvtData *reveal;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      reveal = CreatePositionedEvt(evt, EVTF_REVEAL_USED_ITEM);
      reveal->d.evtf761.gfxIdx = GFX_ITEM_MEGAHERB;
      evt->state++;
      evt->mem = 96;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         gSignal3 = 1;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 770
void Evtf770_to_789_ItemSpell(EvtData *evt) {
   // TODO gfx constants where relevant
   static s16 icons[44] = {0x0,   0x0,   0x0,   0x14a, 0x14a, 0x139, 0x0,   0x97,  0x5e,
                           0xaa,  0x16b, 0xb5,  0xd2,  0x0,   0xd1,  0xaf,  0x17d, 0xe0,
                           0x0,   0x184, 0x2d8, 0x2d9, 0x2dc, 0x2da, 0x2db, 0x2de, 0x2dd,
                           0x2df, 0x2e0, 0x2e1, 0x2e2, 0x0,   0x0,   0x0,   0x32f, 0x330,
                           0x331, 0x332, 0x333, 0x334, 0x335, 0x336, 0x337, 0x0};
   EvtData *reveal;

   if ((gCurrentSpell >= 49 && gCurrentSpell <= 59) ||
       (gCurrentSpell >= 63 && gCurrentSpell <= 71)) {
      SnapToUnit(evt);
      reveal = CreatePositionedEvt(evt, EVTF_REVEAL_USED_ITEM);
      reveal->d.evtf761.gfxIdx = icons[gCurrentSpell - 29];
   }
   evt->functionIndex = EVTF_NULL;
}
