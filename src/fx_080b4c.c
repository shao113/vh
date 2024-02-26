#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"
#include "state.h"
#include "units.h"
#include "audio.h"

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

      RenderMaskEffect(unitSprite, &maskEffect);

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
   static u8 cluts[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};

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

   u8 cluts[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES};

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

      gfxX = gGfxSubTextures[GFX_TBD_25][0];
      gfxY = gGfxSubTextures[GFX_TBD_25][1];
      gfxW = gGfxSubTextures[GFX_TBD_25][2];
      gfxH = gGfxSubTextures[GFX_TBD_25][3];
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

s16 gExplosionAnimData_800ffa80[26] = {7, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                                       2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
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
      particle->d.sprite.animData = gExplosionAnimData_800ffa80;
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
