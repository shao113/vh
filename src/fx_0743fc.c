#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

#undef EVTF
#define EVTF 327
void Evtf327_SpellFx2_HealingCircle(EvtData *evt) {
   // FX2 for Healing Circle, Healing Wave, Kingfoil, Life Orb

   // Any way to resolve this discrepancy?
   extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s32, s16, s16);
   extern s16 gSparkleAnimData_800ffab4[36];

   EvtData *targetSprite;
   EvtData *fxSprite;
   EvtData *evt_s1;
   s32 fade;
   POLY_FT4 *poly;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = targetSprite->y1.n;
      EVT.targetSprite = targetSprite;
      evt->state2 = 0x40;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = targetSprite->y1.n;
      targetSprite = EVT.targetSprite;

      GetUnitSpriteVramRect(targetSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      //? These increments and the need for an alternate ApplyMaskEffect declaration seem odd.
      spriteW++;
      spriteH++;
      ApplyMaskEffect(spriteX, spriteY, spriteW, spriteH, 416 << 2, 320, 0,
                      0x40 - (evt->state2 & 0x3f), GFX_TBD_25, 0);

      fxSprite = CreatePositionedEvt(evt, EVTF_NOOP);

      if (targetSprite->d.sprite.gfxIdx >= 21 && targetSprite->d.sprite.gfxIdx <= 30) {
         fxSprite->d.sprite.boxIdx = 1;
      } else {
         fxSprite->d.sprite.boxIdx = 0;
      }

      fxSprite->d.sprite.gfxIdx = GFX_TBD_25;
      fxSprite->d.sprite.clut = 4;
      fxSprite->d.sprite.semiTrans = 2;
      fxSprite->d.sprite.facingLeft = targetSprite->d.sprite.facingLeft;
      AddEvtPrim6(gGraphicsPtr->ot, fxSprite, 0);
      fxSprite->functionIndex = EVTF_NULL;

      fade = rsin(evt->state2 * 0x20) >> 4;
      if (fade > 0xe0 || fade < 0) { //? Could also be u32 fade or 0xe0u instead.
         fade = 0xe0;
      }
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, fade, fade, fade);
      evt->state2--;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_PARTICLE_710;
      evt_s1->x1.n = evt->x1.n + rand() % 0x120 - 0x8f;
      evt_s1->z1.n = evt->z1.n + rand() % 0x120 - 0x8f;
      evt_s1->y1.n = evt->y1.n + rand() % 0x80 + 0x41;
      evt_s1->state2 = (rand() >> 4) % 5;
      evt_s1->mem = 0x10;
      evt_s1->d.sprite.clut = 3;
      evt_s1->d.sprite.animData = gSparkleAnimData_800ffab4;

      if (evt->state2 == 0x20) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_DISPLAY_DAMAGE_2;
         evt_s1->x1.n = evt->x1.n;
         evt_s1->z1.n = evt->z1.n;
      }
      if (evt->state2 <= 0) {
         evt->state++;
      }

      break;

   case 2:
      targetSprite = EVT.targetSprite;
      targetSprite->d.sprite.semiTrans = 0;
      targetSprite->d.sprite.hidden = 0;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 322
void Evtf322_370_371_372_SpellFx2_MagicRestoration(EvtData *evt) {
   // FX2 for Mage Oil, Mage Gem, Magic Charge
   EvtData *targetSprite;
   EvtData *fxSprite;
   EvtData *floatingText;
   u8 fade;
   POLY_FT4 *poly;
   MaskEffectPreset maskEffect;

   switch (evt->state) {
   case 0:

      switch (evt->functionIndex) {
      case EVTF_SPELL_FX2_MAGE_OIL:
         EVT.amount = 4;
         break;
      case EVTF_SPELL_FX2_RESTORE_8_MP_UNK:
         EVT.amount = 8;
         break;
      case EVTF_SPELL_FX2_MAGIC_CHARGE:
         // Also used for Mage Gem
         EVT.amount = 10;
         break;
      default:
         EVT.amount = 10;
         break;
      }

      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      targetSprite->d.sprite.hidden = 1;
      evt->state2 = 0x60;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = targetSprite->y1.n;

      fxSprite = Evt_GetUnused();
      CopyEvtData(targetSprite, fxSprite);
      fxSprite->functionIndex = EVTF_NOOP;

      maskEffect.srcGfxIdx = GFX_TILED_VEINS;
      maskEffect.dstGfxIdx = GFX_TBD_25;
      maskEffect.width = 0;
      maskEffect.height = 0x60 - evt->state2;
      maskEffect.semiTrans = 2;
      fade = evt->state2 << 1;
      maskEffect.color.r = maskEffect.color.g = maskEffect.color.b = fade;
      maskEffect.clut = 8;
      ApplyMaskEffectPreset(fxSprite, &maskEffect);
      fxSprite->functionIndex = EVTF_NULL;

      targetSprite->d.sprite.hidden = 0;
      fade = (0x60 - evt->state2) << 1;
      RenderUnitSprite(gGraphicsPtr->ot, targetSprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, fade, fade, fade);
      targetSprite->d.sprite.hidden = 1;

      if (--evt->state2 == 0x20) {
         floatingText = CreatePositionedEvt(targetSprite, EVTF_FLOATING_DAMAGE_TEXT);
         floatingText->d.evtf051.damage = EVT.amount;
         floatingText->d.evtf051.clut = 8;
      }
      if (evt->state2 <= 0) {
         targetSprite->d.sprite.hidden = 0;
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
#define EVTF 310
void Evtf310_SpellFx_LifeOrb_Beam(EvtData *evt) {
   EvtData *sprite;
   POLY_FT4 *poly;
   s32 i;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES_DYN_1;
      sprite->d.sprite.clut = 4;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;
      EVT.height = 0;
      EVT.unused_0x26 = 0x10;
      EVT.radius = 0x40;
      evt->state++;

   // fallthrough
   case 1:
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = 160 - evt->state2 % 0x20;
      sprite = EVT.sprite;

      for (i = 0; i < 8; i++) {
         sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[2].x =
             evt->x1.n + (EVT.radius * rcos(i * 0x200) >> 12);
         sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[3].x =
             evt->x1.n + (EVT.radius * rcos((i + 1) * 0x200) >> 12);
         sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[2].z =
             evt->z1.n + (EVT.radius * rsin(i * 0x200) >> 12);
         sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[3].z =
             evt->z1.n + (EVT.radius * rsin((i + 1) * 0x200) >> 12);
         sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = evt->y1.n + EVT.height;
         sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = evt->y1.n;
         sprite->x1.n = sprite->d.sprite.coords[0].x;
         sprite->z1.n = sprite->d.sprite.coords[0].z;
         sprite->y1.n = sprite->d.sprite.coords[0].y;
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, 0xff, 0xff, 0xff);
      }

      if (EVT.height < 0x500) {
         EVT.height += 0x20;
      }

      if (--evt->state2 <= 0) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

void Evtf311_BubbleSwirl(EvtData *evt) {
   EvtData *bubble;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->state2 = 128; // Number of bubbles
      evt->state++;

   // fallthrough
   case 1:
      bubble = Evt_GetUnused();
      bubble->functionIndex = EVTF_BUBBLE_SWIRL_BUBBLE;
      bubble->x2.n = evt->x1.n;
      bubble->z2.n = evt->z1.n;
      bubble->y2.n = evt->y1.n;
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
#define EVTF 312
void Evtf312_BubbleSwirl_Bubble(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_BUBBLE;
      EVT.boxIdx = 5;
      EVT.clut = 4;
      EVT.theta = rand() % ANGLE_360_DEGREES;
      EVT.yVelocity = 0x10;
      EVT.rVelocity = 1;
      EVT.radius = 0x100;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n = evt->x2.n + (EVT.radius * rcos(EVT.theta) >> 12);
      evt->z1.n = evt->z2.n + (EVT.radius * rsin(EVT.theta) >> 12);
      evt->y1.n = evt->y2.n;
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      EVT.theta = (EVT.theta + 0x80) & 0xfff;
      evt->y2.n += EVT.yVelocity;
      EVT.radius += EVT.rVelocity;
      if (evt->y2.n >= 0x500) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 313
void Evtf313_SpellFx1_LifeOrb(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_v1;
   u8 *p;
   s32 i;

   gCameraRotation.vy += 0x10;

   switch (evt->state) {
   case 0:
      p = gTargetCoords;
      targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->state2 = 0x40;
      evt->state3 = 0x80;
      evt->mem = 3;
      evt->state++;

   // fallthrough
   case 1:
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
      if (--evt->mem <= 0) {
         for (i = 0; i < 12; i++) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_INWARD_RAY;
            evt_v1->x2.n = evt->x1.n;
            evt_v1->z2.n = evt->z1.n;
            evt_v1->y2.n = evt->y1.n;
            evt->mem = 2 + rand() % 3;
         }
      }
      gLightColor.r = evt->state3;
      gLightColor.g = evt->state3;
      gLightColor.b = evt->state3;
      if (evt->state3 > 32) {
         evt->state3 -= 4;
      }
      if (--evt->state2 <= 0) {
         evt->state2 = 0x40;
         evt->state++;
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_SPELL_FX_LIFE_ORB_BEAM;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->z1.n = evt->z1.n;
         evt_v1->y1.n = evt->y1.n;
         evt_v1->state2 = 0x40;
         EVT.beam = evt_v1;
      }
      break;

   case 2:
      evt_v1 = EVT.beam;
      if (evt_v1->state2 < 0x40) {
         evt_v1->d.evtf310.radius = (0x40 - evt_v1->state2) << 1;
      }
      if (--evt->mem <= 0) {
         for (i = 0; i < 4; i++) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_INWARD_RAY;
            evt_v1->x2.n = evt->x1.n;
            evt_v1->z2.n = evt->z1.n;
            evt_v1->y2.n = evt->y1.n;
            evt->mem = 2 + rand() % 3;
         }
      }
      for (i = 0; i < 4; i++) {
         evt_v1 = Evt_GetUnused();
         if (evt_v1 != NULL) {
            evt_v1->functionIndex = EVTF_VERTICAL_RAY;
            evt_v1->x2.n = evt->x1.n;
            evt_v1->z2.n = evt->z1.n;
            evt_v1->y2.n = evt->y1.n;
         }
      }
      gLightColor.r = evt->state3;
      gLightColor.g = evt->state3;
      gLightColor.b = evt->state3;
      if (--evt->state2 <= 0) {
         evt->state++;
      }
      break;

   case 3:
      evt_v1 = EVT.beam;
      if (evt_v1->state2 < 0x40) {
         evt_v1->d.evtf310.radius = evt_v1->state2;
      }
      if (evt->state3 < 0x80) {
         evt->state3 += 4;
         gLightColor.r = evt->state3;
         gLightColor.g = evt->state3;
         gLightColor.b = evt->state3;
      } else {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
      }
      break;

   case 4:
      break;
   }
}
