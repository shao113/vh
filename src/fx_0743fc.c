#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

#undef EVTF
#define EVTF 327
void Evtf327_HealingCircle_FX2(EvtData *evt) {
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
void Evtf322_370_371_372_MagicRestoration_FX2(EvtData *evt) {
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
      case EVTF_MAGE_OIL_FX2:
         EVT.amount = 4;
         break;
      case EVTF_RESTORE_8_MP_UNK_FX2:
         EVT.amount = 8;
         break;
      case EVTF_MAGIC_CHARGE_FX2:
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
void Evtf310_LifeOrb_Beam(EvtData *evt) {
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
void Evtf313_LifeOrb_FX1(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_v1;
   BVectorXZ *p;
   s32 i;

   gCameraRotation.vy += 0x10;

   switch (evt->state) {
   case 0:
      p = (BVectorXZ *)gTargetCoords;
      targetSprite = GetUnitSpriteAtPosition(p->z, p->x);
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
         evt_v1->functionIndex = EVTF_LIFE_ORB_BEAM;
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

#undef EVTF
#define EVTF 314
void Evtf314_InwardRay(EvtData *evt) {
   s32 radius;
   s32 rnd;

   switch (evt->state) {
   case 0:
      EVT.theta1 = -(rand() % 0x180);
      EVT.theta2 = rand() % ANGLE_360_DEGREES;
      if (EVT.todo_x38 == 0) {
         EVT.todo_x38 = 0x500;
      }
      EVT.unused_0x3A = rand() % 0x40 + 0x20;
      rnd = (rand() >> 3) % 0x1000;
      if (rnd >= 0x200) {
         evt->mem = 3;
      } else {
         evt->mem = 2;
      }
      EVT.todo_x54 = -0x20 - rand() % 0x40;
      EVT.gfxIdx = GFX_COLOR_15;
      evt->state2 = 0;

      radius = EVT.todo_x38 * rcos(EVT.theta1) >> 12;
      EVT.coords[1].x = evt->x2.n + (radius * rcos(EVT.theta2) >> 12);
      EVT.coords[1].z = evt->z2.n + (radius * rsin(EVT.theta2) >> 12);
      EVT.coords[3].x = evt->x2.n + (radius * rcos(EVT.theta2 + 16) >> 12);
      EVT.coords[3].z = evt->z2.n + (radius * rsin(EVT.theta2 + 16) >> 12);
      EVT.coords[1].y = EVT.coords[3].y = evt->y2.n + (EVT.todo_x38 * rsin(EVT.theta1) >> 12);

      EVT.otOfs = 16;
      evt->state++;

   // fallthrough
   case 1:
      evt->state2++;
      EVT.clut = 3 + evt->state2 % 3;

      EVT.todo_x38 += (0 - EVT.todo_x38) >> evt->mem;
      radius = (EVT.todo_x38 * rcos(EVT.theta1)) >> 12;
      EVT.coords[0].x = evt->x2.n + (radius * rcos(EVT.theta2) >> 12);
      EVT.coords[0].z = evt->z2.n + (radius * rsin(EVT.theta2) >> 12);
      EVT.coords[2].x = evt->x2.n + (radius * rcos(EVT.theta2 + 16) >> 12);
      EVT.coords[2].z = evt->z2.n + (radius * rsin(EVT.theta2 + 16) >> 12);
      EVT.coords[0].y = EVT.coords[2].y = evt->y2.n + (EVT.todo_x38 * rsin(EVT.theta1) >> 12);

      evt->x1.n = EVT.coords[0].x;
      evt->z1.n = EVT.coords[0].z;
      evt->y1.n = EVT.coords[0].y;

      AddEvtPrim5(gGraphicsPtr->ot, evt);

      EVT.coords[1].x = EVT.coords[0].x;
      EVT.coords[1].z = EVT.coords[0].z;
      EVT.coords[3].x = EVT.coords[2].x;
      EVT.coords[3].z = EVT.coords[2].z;
      EVT.coords[1].y = EVT.coords[3].y = EVT.coords[0].y;

      if (EVT.todo_x38 <= 0x80) {
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 315
void Evtf315_VerticalRay(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.theta = rand() % ANGLE_360_DEGREES;
      EVT.radius = 0x20;
      EVT.top = evt->y2.n;
      EVT.height = rand() % 0x100;
      EVT.rotationSpeed = 0x20;
      EVT.speed = 0x40;
      EVT.clut = 3 + rand() % 3;
      EVT.gfxIdx = GFX_COLOR_15;
      evt->state++;

   // fallthrough
   case 1:
      EVT.coords[0].x = EVT.coords[2].x = evt->x2.n + (EVT.radius * rcos(EVT.theta) >> 12);
      EVT.coords[0].z = EVT.coords[2].z = evt->z2.n + (EVT.radius * rsin(EVT.theta) >> 12);
      EVT.coords[1].x = EVT.coords[3].x = evt->x2.n + (EVT.radius * rcos(EVT.theta + 16) >> 12);
      EVT.coords[1].z = EVT.coords[3].z = evt->z2.n + (EVT.radius * rsin(EVT.theta + 16) >> 12);
      EVT.coords[0].y = EVT.coords[1].y = EVT.top;
      EVT.coords[2].y = EVT.top - EVT.height;
      if (EVT.coords[2].y < evt->y2.n) {
         EVT.coords[2].y = evt->y2.n;
      }
      EVT.coords[3].y = EVT.coords[2].y;
      evt->x1.n = EVT.coords[0].x;
      evt->z1.n = EVT.coords[0].z;
      evt->y1.n = EVT.coords[0].y;
      EVT.theta += EVT.rotationSpeed;
      AddEvtPrim5(gGraphicsPtr->ot, evt, 0x400); //? Extra arg
      EVT.top += EVT.speed;
      if (EVT.top >= evt->y2.n + 0x500) {
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void RenderCylinder(Cylinder *cylinder) {
   EvtData *face;
   s32 sideCt;
   s32 i;
   s16 increment;
   s16 startingTheta;
   s16 theta;
   POLY_FT4 *poly;

   face = Evt_GetUnused();
   face->d.sprite.semiTrans = cylinder->semiTrans;
   face->d.sprite.gfxIdx = cylinder->gfxIdx;
   if (cylinder->clut != 0) {
      face->d.sprite.clut = cylinder->clut;
   }

   sideCt = cylinder->sideCt;
   increment = ANGLE_360_DEGREES / sideCt;

   if (cylinder->theta != 0) {
      startingTheta = cylinder->theta;
   } else {
      startingTheta = 0;
   }

   theta = startingTheta + increment;
   face->d.sprite.coords[0].x =
       cylinder->top.vx + (cylinder->topRadius * rcos(startingTheta) >> 12);
   face->d.sprite.coords[2].x =
       cylinder->bottom.vx + (cylinder->bottomRadius * rcos(startingTheta) >> 12);
   face->d.sprite.coords[0].z =
       cylinder->top.vz + (cylinder->topRadius * rsin(startingTheta) >> 12);
   face->d.sprite.coords[2].z =
       cylinder->bottom.vz + (cylinder->bottomRadius * rsin(startingTheta) >> 12);
   face->d.sprite.coords[0].y = face->d.sprite.coords[1].y = cylinder->top.vy;
   face->d.sprite.coords[2].y = face->d.sprite.coords[3].y = cylinder->bottom.vy;

   for (i = 0; i < sideCt; theta += increment, i++) {
      face->d.sprite.coords[1].x = cylinder->top.vx + (cylinder->topRadius * rcos(theta) >> 12);
      face->d.sprite.coords[3].x =
          cylinder->bottom.vx + (cylinder->bottomRadius * rcos(theta) >> 12);
      face->d.sprite.coords[1].z = cylinder->top.vz + (cylinder->topRadius * rsin(theta) >> 12);
      face->d.sprite.coords[3].z =
          cylinder->bottom.vz + (cylinder->bottomRadius * rsin(theta) >> 12);

      face->x1.n = face->d.sprite.coords[0].x;
      face->z1.n = face->d.sprite.coords[0].z;
      face->y1.n = face->d.sprite.coords[0].y;

      AddEvtPrim3(gGraphicsPtr->ot, face);

      face->d.sprite.coords[0].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[2].x = face->d.sprite.coords[3].x;
      face->d.sprite.coords[0].z = face->d.sprite.coords[1].z;
      face->d.sprite.coords[2].z = face->d.sprite.coords[3].z;

      if (cylinder->useColor) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, cylinder->color.r, cylinder->color.g, cylinder->color.b);
      }
   }
}

void RenderCylinderWithRotatedTexture(Cylinder *cylinder) {
   EvtData *face;
   s32 sideCt;
   s32 i;
   s16 increment;
   s16 startingTheta;
   s16 theta;
   POLY_FT4 *poly;

   face = Evt_GetUnused();
   face->d.sprite.semiTrans = cylinder->semiTrans;
   face->d.sprite.gfxIdx = cylinder->gfxIdx;
   if (cylinder->clut != 0) {
      face->d.sprite.clut = cylinder->clut;
   }

   sideCt = cylinder->sideCt;
   increment = ANGLE_360_DEGREES / sideCt;

   if (cylinder->theta != 0) {
      startingTheta = cylinder->theta;
   } else {
      startingTheta = 0;
   }

   theta = startingTheta + increment;
   face->d.sprite.coords[2].x =
       cylinder->top.vx + (cylinder->topRadius * rcos(startingTheta) >> 12);
   face->d.sprite.coords[3].x =
       cylinder->bottom.vx + (cylinder->bottomRadius * rcos(startingTheta) >> 12);
   face->d.sprite.coords[2].z =
       cylinder->top.vz + (cylinder->topRadius * rsin(startingTheta) >> 12);
   face->d.sprite.coords[3].z =
       cylinder->bottom.vz + (cylinder->bottomRadius * rsin(startingTheta) >> 12);
   face->d.sprite.coords[2].y = face->d.sprite.coords[0].y = cylinder->top.vy;
   face->d.sprite.coords[3].y = face->d.sprite.coords[1].y = cylinder->bottom.vy;

   for (i = 0; i < sideCt; theta += increment, i++) {
      face->d.sprite.coords[0].x = cylinder->top.vx + (cylinder->topRadius * rcos(theta) >> 12);
      face->d.sprite.coords[1].x =
          cylinder->bottom.vx + (cylinder->bottomRadius * rcos(theta) >> 12);
      face->d.sprite.coords[0].z = cylinder->top.vz + (cylinder->topRadius * rsin(theta) >> 12);
      face->d.sprite.coords[1].z =
          cylinder->bottom.vz + (cylinder->bottomRadius * rsin(theta) >> 12);

      face->x1.n = face->d.sprite.coords[1].x;
      face->z1.n = face->d.sprite.coords[1].z;
      face->y1.n = face->d.sprite.coords[1].y;

      AddEvtPrim3(gGraphicsPtr->ot, face);

      face->d.sprite.coords[2].x = face->d.sprite.coords[0].x;
      face->d.sprite.coords[3].x = face->d.sprite.coords[1].x;
      face->d.sprite.coords[2].z = face->d.sprite.coords[0].z;
      face->d.sprite.coords[3].z = face->d.sprite.coords[1].z;

      if (cylinder->useColor) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, cylinder->color.r, cylinder->color.g, cylinder->color.b);
      }
   }
}

void EvilStream_RenderSkull(EvtData *evilStream, s16 height) {
   EvtData *skullSprite;
   s16 halfSize;

   skullSprite = CreatePositionedEvt(evilStream, EVTF_NOOP);
   skullSprite->y1.n += 0x80;
   skullSprite->d.sprite.gfxIdx = GFX_SKULL;
   skullSprite->d.sprite.boxIdx = 7;
   height >>= 3;
   halfSize = 0x10;
   gQuad_800fe63c[0].vx = -halfSize;
   gQuad_800fe63c[1].vx = halfSize;
   gQuad_800fe63c[2].vx = -halfSize;
   gQuad_800fe63c[3].vx = halfSize;
   gQuad_800fe63c[0].vy = -height / 2;
   gQuad_800fe63c[1].vy = gQuad_800fe63c[0].vy;
   gQuad_800fe63c[2].vy = height / 2;
   gQuad_800fe63c[3].vy = gQuad_800fe63c[2].vy;
   gQuad_800fe63c[0].vz = 0;
   gQuad_800fe63c[1].vz = 0;
   gQuad_800fe63c[2].vz = 0;
   gQuad_800fe63c[3].vz = 0;
   AddEvtPrim6(gGraphicsPtr->ot, skullSprite, 0);
   skullSprite->functionIndex = EVTF_NULL;
}

void Evtf325_Fx_TBD(EvtData *evt) {
   POLY_FT4 *poly;

   evt->d.sprite.clut = 3 + evt->state2 % 3;
   AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
   if (evt->d.sprite.semiTrans != 0) {
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, evt->state2, evt->state2, evt->state2);
   }
   if (--evt->state2 <= 0) {
      evt->functionIndex = EVTF_NULL;
   }
}

#undef EVTF
#define EVTF 307
void Evtf307_324_EvilStream_FX2_FX3(EvtData *evt) {
   EvtData *evt_s1;
   EvtData *fxSprite;
   s32 rnd;
   Cylinder *cylinder;

   switch (evt->state) {
   case 0:
      evt_s1 = SnapToUnit(evt);
      fxSprite = Evt_GetUnused();
      CopyEvtData(evt_s1, fxSprite);
      fxSprite->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.hidden = 1;
      EVT.sprite = fxSprite;
      EVT.targetSprite = evt_s1;
      evt->mem = 1;

      // TODO: union?
      *(s32 *)&gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1] =
          *(s32 *)&gGfxSubTextures[GFX_TILED_RED_SPARKLES];

      cylinder = &EVT.cylinder;
      cylinder->top.vx = cylinder->bottom.vx = evt->x1.n;
      cylinder->top.vz = cylinder->bottom.vz = evt->z1.n;
      cylinder->bottom.vy = evt->y1.n;
      cylinder->top.vy = cylinder->bottom.vy + 0x400;
      cylinder->sideCt = 16;
      cylinder->semiTrans = 2;
      cylinder->gfxIdx = GFX_TILED_RED_SPARKLES_DYN_1;
      cylinder->topRadius = 0;
      cylinder->bottomRadius = 0;
      cylinder->useColor = 1;
      cylinder->color.r = cylinder->color.g = cylinder->color.b = 0;
      cylinder->clut = 3;

      evt->state2 = 0x80;
      evt->mem = 0;
      evt->state++;

   // fallthrough
   case 1:
      cylinder = &EVT.cylinder;

      switch (evt->mem) {
      case 0:
         cylinder->topRadius += (0xc0 - cylinder->topRadius) >> 4;
         cylinder->bottomRadius = cylinder->topRadius;
         cylinder->color.b += (0x40 - cylinder->color.b) >> 3;
         cylinder->color.r += (0x40 - cylinder->color.r) >> 3;
         cylinder->color.g += (0x40 - cylinder->color.g) >> 3;
         gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = -0x60 - evt->state2 * 8 % 0x20;
         RenderCylinder(cylinder);

         gLightColor.r -= (gLightColor.r - 0x10) >> 4;
         gLightColor.g -= (gLightColor.g - 0x10) >> 4;
         gLightColor.b -= (gLightColor.b - 0x10) >> 4;

         if (evt->state2 % 2 == 0) {
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_EVIL_STREAM_ROCK;
            rnd = (rand() >> 2) % ANGLE_360_DEGREES;
            evt_s1->x1.n = evt->x1.n + (0x80 * rcos(rnd) >> 12);
            evt_s1->z1.n = evt->z1.n + (0x80 * rsin(rnd) >> 12);
            evt_s1->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
            evt_s1->y2.n = 0x20 + (rand() >> 2) % 0x20; // y velocity
            evt_s1->d.evtf379.maxHeight = evt_s1->y1.n + 0x800;
         }
         if (--evt->state2 <= 32) {
            evt->mem++;
         }
         break;

      case 1:
         cylinder->topRadius += (0 - cylinder->topRadius) >> 4;
         cylinder->bottomRadius = cylinder->topRadius;
         cylinder->color.b += (0xff - cylinder->color.b) >> 3;
         cylinder->color.r += (0xff - cylinder->color.r) >> 3;
         cylinder->color.g += (0xff - cylinder->color.g) >> 3;
         gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = -0x60 - evt->state2 * 8 % 0x20;
         cylinder->clut = 3 + evt->state2 % 3;
         RenderCylinder(cylinder);
         if (--evt->state2 <= 0) {
            evt->mem++;
         }
         break;

      case 2:
         evt->state++;
         break;
      }

      fxSprite = EVT.sprite;
      evt_s1 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      CopyEvtData(evt_s1, fxSprite);
      fxSprite->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.hidden = 1;
      fxSprite->d.sprite.hidden = 0;
      fxSprite->x1.n = evt_s1->x1.n + rand() % 0x41 - 0x20;
      fxSprite->y1.n = evt_s1->y1.n;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);
      if (evt->state2 >= 0x20 && evt->state2 <= 0x80) {
         EvilStream_RenderSkull(evt, evt->state2 << 3);
      }
      break;

   case 2:
      fxSprite = EVT.sprite;
      fxSprite->functionIndex = EVTF_NULL;
      evt_s1 = EVT.targetSprite;
      evt_s1->d.sprite.hidden = 0;
      if (evt->functionIndex == EVTF_EVIL_STREAM_FX3) {
         evt_s1 = CreatePositionedEvt(evt, EVTF_FX_TBD_134);
         evt_s1->d.evtf134.clut = 3;
      } else { // EVTF_EVIL_STREAM_FX2:
         evt_s1 = CreatePositionedEvt(evt, EVTF_FX_TBD_132);
         evt_s1->d.evtf132.clut = 3;
      }
      evt->functionIndex = EVTF_NULL;
      gLightColor.r = gLightColor.g = gLightColor.b = 0x80;
      break;
   }
}

#undef EVTF
#define EVTF 326
void Evtf326_Fx_TBD(EvtData *evt) {
   EvtData *sprite;
   EvtData *dataStore;
   Cylinder *dsCylinder;
   s32 i;
   s32 radius;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES_DYN_1;
      sprite->d.sprite.clut = 3;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;

      EVT.height = 0;
      EVT.speed = 0x20;
      EVT.radius = 2;

      // TODO: union?
      *(s32 *)&gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1] =
          *(s32 *)&gGfxSubTextures[GFX_TILED_RED_SPARKLES];

      evt->x2.n = evt->x1.n;
      evt->z2.n = evt->z1.n;
      evt->y2.n = evt->y1.n;

      dataStore = Evt_GetUnused();
      dataStore->functionIndex = EVTF_NOOP;
      EVT.dataStore = dataStore;

      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->sideCt = 16;
      dsCylinder->semiTrans = 2;
      dsCylinder->gfxIdx = GFX_TILED_RED_SPARKLES_DYN_1;
      dsCylinder->topRadius = 2;
      dsCylinder->bottomRadius = 2;
      dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->top.vy = evt->y1.n;
      dsCylinder->useColor = 1;

      evt->state++;

   // fallthrough
   case 1:
      dataStore = EVT.dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      EVT.radius += (0xc0 - EVT.radius) >> 4;
      if (EVT.height < 0x400) {
         EVT.height += EVT.speed;
      }
      dsCylinder->top.vy = evt->y1.n + EVT.height;
      dsCylinder->bottomRadius = dsCylinder->topRadius = EVT.radius;
      dsCylinder->color.r = 0x40;
      dsCylinder->color.g = 0x40;
      dsCylinder->color.b = 0x40;
      RenderCylinder(dsCylinder);

      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = -0x60 - evt->state2 * 16 % 0x20;
      sprite = EVT.sprite;

      for (i = 0; i < 16; i++) {
         radius = EVT.radius;
         gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = -0x60 - evt->state2 * i % 0x20;

         sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[2].x =
             evt->x1.n + (radius * rcos(i * 0x100) >> 12);
         sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[3].x =
             evt->x1.n + (radius * rcos((i + 1) * 0x100) >> 12);
         sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[2].z =
             evt->z1.n + (radius * rsin(i * 0x100) >> 12);
         sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[3].z =
             evt->z1.n + (radius * rsin((i + 1) * 0x100) >> 12);
         sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = evt->y1.n + EVT.height;
         sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = evt->y1.n;

         sprite->x1.n = sprite->d.sprite.coords[0].x;
         sprite->z1.n = sprite->d.sprite.coords[0].z;
         sprite->y1.n = sprite->d.sprite.coords[0].y;
      }

      if (--evt->state2 <= 0) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 379
void Evtf379_EvilStream_Rock(EvtData *evt) {
   extern s16 gRockAnimData_800ff670[12];

   switch (evt->state) {
   case 0:
      EVT.boxIdx = 4;
      EVT.animData = gRockAnimData_800ff670;
      evt->state++;

   // fallthrough
   case 1:
      evt->y1.n += evt->y2.n;
      if (evt->y1.n > EVT.maxHeight) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }

   UpdateEvtAnimation(evt);
   AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
}

void Evtf329_Noop(EvtData *evt) {}
