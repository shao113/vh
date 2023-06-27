#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "state.h"

// TBD: When to use ANGLE_360_DEGREES vs ONE; tough to make good guesses without knowing the math

#undef EVTF
#define EVTF 119
void Evtf119_Fx_TBD(EvtData *evt) {
   // Spawned by: ?->182->183->119

   static s16 impactAnimData[18] = {4, GFX_IMPACT_1, 2, GFX_IMPACT_2, 2, GFX_IMPACT_3,
                                    2, GFX_IMPACT_4, 2, GFX_IMPACT_5, 2, GFX_IMPACT_6,
                                    2, GFX_IMPACT_7, 2, GFX_NULL,     0, GFX_NULL};

   static s16 explosionAnimData[26] = {4, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                                       2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

   static s16 smokeAnimData[24] = {4, GFX_PUFF_1, 2, GFX_PUFF_2,  2, GFX_PUFF_3, 2, GFX_PUFF_4,
                                   2, GFX_PUFF_5, 2, GFX_PUFF_6,  2, GFX_PUFF_7, 2, GFX_PUFF_8,
                                   2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   0, GFX_NULL};

   static s16 sparkleAnimData[18] = {4, GFX_SPARKLE_1, 2, GFX_SPARKLE_2, 2, GFX_SPARKLE_4,
                                     2, GFX_SPARKLE_5, 2, GFX_SPARKLE_4, 2, GFX_SPARKLE_3,
                                     2, GFX_SPARKLE_2, 2, GFX_NULL,      0, GFX_NULL};

   static s16 flameAnimData[20] = {4, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4,
                                   2, GFX_FLAME_5, 2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8,
                                   2, GFX_NULL,    0, GFX_NULL};

   static s16 lightningAnimData[20] = {4, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3,
                                       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
                                       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_NULL,
                                       0, GFX_NULL};

   static s16 lightOrbAnimData[14] = {4, GFX_EXPLOSION_1, 2, GFX_EXPLOSION_2, 2, GFX_EXPLOSION_3,
                                      2, GFX_EXPLOSION_2, 2, GFX_EXPLOSION_1, 2, GFX_NULL,
                                      0, GFX_NULL};

   s32 rnd1, rnd2;
   EvtData *sprite;
   s16 *animData;

   switch (evt->state) {
   case 0:
      if (EVT.boxIdx == 0) {
         EVT.boxIdx = 4;
      }
      if (EVT.clut == 0) {
         EVT.clut = 4;
      }

      rnd1 = rand();
      EVT.theta = rnd1 % ANGLE_360_DEGREES;
      rnd2 = rand();
      EVT.h = 0x80 - (rnd2 % 0x100);

      EVT.sprite = sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;

      switch (EVT.type) {
      case 0:
         animData = impactAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 1:
         animData = explosionAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 2:
         animData = smokeAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 3:
         animData = sparkleAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 4:
         animData = flameAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 5:
         animData = lightningAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      case 6:
         animData = lightOrbAnimData;
         animData[0] = EVT.boxIdx;
         sprite->d.sprite.animData = animData;
         break;
      } // switch (type) (via state:0)

      sprite->d.sprite.animInitialized = 0;
      sprite->d.sprite.clut = EVT.clut;
      sprite->d.sprite.semiTrans = EVT.semiTrans;
      evt->state++;
      break;

   case 1:
      if (EVT.timer == 0) {
         evt->state++;
      } else {
         --EVT.timer;
      }
      break;

   case 2:
      sprite = EVT.sprite;
      sprite->x1.n = evt->x1.n + (EVT.radius * rcos(EVT.theta) >> 12);
      sprite->z1.n = evt->z1.n + (EVT.radius * rsin(EVT.theta) >> 12);
      sprite->y1.n = evt->y1.n + EVT.h;
      UpdateEvtAnimation(sprite);
      if (sprite->d.sprite.animFinished) {
         if (!EVT.once) {
            evt->state = 0;
            sprite->functionIndex = EVTF_NULL;
         } else {
            evt->functionIndex = EVTF_NULL;
            sprite->functionIndex = EVTF_NULL;
         }
      } else {
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      }
      break;
   }
}

#undef EVTF
#define EVTF 062
void Evtf062_StretchWarpSprite(EvtData *evt) {
   EvtData *unitSprite, *fxSprite;
   Quad quad;
   s32 i;

   switch (evt->state) {
   case 0:
      if (EVT.sprite == NULL) {
         // If original sprite was not provided, retrieve by location
         EVT.sprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      } else {
         unitSprite = EVT.sprite;
      }
      unitSprite->d.sprite.hidden = 1;
      if (EVT.speed == 0) {
         EVT.speed = ANGLE_270_DEGREES / 0x10;
      }
      if (evt->mem != 0) {
         // Reversed (warp in)
         EVT.phase = ANGLE_270_DEGREES;
      }
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.sprite;
      fxSprite = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite);
      fxSprite->d.sprite.hidden = 0;
      for (i = 0; i < 4; i++) {
         quad[i].vx = gQuad_800fe53c[i].vx;
         quad[i].vz = gQuad_800fe53c[i].vz;
         quad[i].vy = gQuad_800fe53c[i].vy;
      }
      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx =
             (quad[i].vx * (rsin(EVT.phase) + ANGLE_360_DEGREES)) / ANGLE_360_DEGREES;
         gQuad_800fe53c[i].vz =
             (quad[i].vz * (rsin(EVT.phase) + ANGLE_360_DEGREES)) / ANGLE_360_DEGREES;

         if (EVT.phase <= ANGLE_180_DEGREES) {
            gQuad_800fe53c[i].vy = quad[i].vy *
                                   (rsin(EVT.phase + ANGLE_180_DEGREES) / 2 + ANGLE_180_DEGREES) /
                                   ANGLE_180_DEGREES;
         } else {
            gQuad_800fe53c[i].vy = quad[i].vy *
                                   (rsin(EVT.phase + ANGLE_180_DEGREES) * 3 + ANGLE_360_DEGREES) /
                                   ANGLE_360_DEGREES;
         }
      }
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);
      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx = quad[i].vx;
         gQuad_800fe53c[i].vz = quad[i].vz;
         gQuad_800fe53c[i].vy = quad[i].vy;
      }
      if (evt->mem == 0) {
         EVT.phase += EVT.speed;
         if (EVT.phase >= ANGLE_270_DEGREES) {
            evt->functionIndex = EVTF_NULL;
         }
      } else {
         EVT.phase -= EVT.speed;
         if (EVT.phase <= 0) {
            unitSprite->d.sprite.hidden = 0;
            evt->functionIndex = EVTF_NULL;
         }
      }
      fxSprite->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 074
void Evtf074_FadeInSprite(EvtData *evt) {
   EvtData *unitSprite, *fxSprite1, *fxSprite2;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.sprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 1;
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.sprite;
      fxSprite1 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->x1.n = unitSprite->x1.n;
      fxSprite1->z1.n = unitSprite->z1.n;
      fxSprite1->y1.n = unitSprite->y1.n;
      fxSprite1->d.sprite.semiTrans = 2;

      fxSprite2 = Evt_GetUnused();
      CopyEvtData(fxSprite1, fxSprite2);
      fxSprite2->d.sprite.semiTrans = 3;
      fxSprite2->d.sprite.clut = 10;

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      EVT.fade += EVT.speed;
      if (EVT.fade > 0x80) {
         unitSprite->d.sprite.hidden = 0;
         evt->functionIndex = EVTF_NULL;
      }
      fxSprite1->functionIndex = EVTF_NULL;
      fxSprite2->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 075
void Evtf075_FadeOutSprite(EvtData *evt) {
   EvtData *unitSprite, *fxSprite1, *fxSprite2;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.sprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 1;
      EVT.fade = 0x80;
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.sprite;
      fxSprite1 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->x1.n = unitSprite->x1.n;
      fxSprite1->z1.n = unitSprite->z1.n;
      fxSprite1->y1.n = unitSprite->y1.n;
      fxSprite1->d.sprite.semiTrans = 2;

      fxSprite2 = Evt_GetUnused();
      CopyEvtData(fxSprite1, fxSprite2);
      fxSprite2->d.sprite.semiTrans = 0;

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      EVT.fade -= EVT.speed;
      if (EVT.fade < 0) {
         evt->functionIndex = EVTF_NULL;
      }
      fxSprite1->functionIndex = EVTF_NULL;
      fxSprite2->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 072
void Evtf072_FadeFromWhite(EvtData *evt) {
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.fade = 0x80;
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
      gQuadIndex++;

      EVT.fade -= EVT.speed;
      if (EVT.fade < 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 073
void Evtf073_FadeToWhite(EvtData *evt) {
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
      gQuadIndex++;

      EVT.fade += EVT.speed;
      if (EVT.fade > 0x80) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 070
void Evtf070_FadeFromBlack(EvtData *evt) {
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.fade = 0x80;
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[64 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 2], poly);
      gQuadIndex++;

      EVT.fade -= EVT.speed;
      if (EVT.fade < 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 071
void Evtf071_FadeToBlack(EvtData *evt) {
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      if (EVT.speed == 0) {
         EVT.speed = 4;
      }
      evt->state++;

   // fallthrough
   case 1:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[64 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 2], poly);
      gQuadIndex++;

      EVT.fade += EVT.speed;
      if (EVT.fade > 0x80) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 076
void Evtf076_Circles_TBD(EvtData *evt) {
   EvtData *unitSprite, *circle;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n + 0xc0;

      switch (unitSprite->d.sprite.direction) {
      case ANGLE_WEST:
         EVT.todo_x50 = -1;
         EVT.todo_x4e = 1;
         evt->x1.n += 0x90;
         break;
      case ANGLE_EAST:
         EVT.todo_x50 = 1;
         EVT.todo_x4e = 1;
         evt->x1.n -= 0x90;
         break;
      case ANGLE_SOUTH:
         EVT.todo_x50 = -1;
         EVT.todo_x4e = 0;
         evt->z1.n += 0x90;
         break;
      case ANGLE_NORTH:
         EVT.todo_x50 = 1;
         EVT.todo_x4e = 0;
         evt->z1.n -= 0x90;
         break;
      }

      evt->state++;

   // fallthrough
   case 1:
      circle = Evt_GetUnused();
      circle->functionIndex = EVTF_CIRCLE;
      circle->x1.n = evt->x1.n;
      circle->z1.n = evt->z1.n;
      circle->y1.n = evt->y1.n;
      circle->d.evtf077.todo_x4e = EVT.todo_x4e;
      circle->d.evtf077.todo_x50 = EVT.todo_x50;
      if (++EVT.timer == 3) {
         evt->state++;
         EVT.timer = 0;
      }
      break;

   case 2:
      if (++EVT.timer >= 16) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 077
void Evtf077_Circle_TBD(EvtData *evt) {
   EvtData *ringSprite;
   s16 a, b;
   s16 fade;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.fade = 0xff;
      evt->state++;

   // fallthrough
   case 1:
      ringSprite = Evt_GetUnused();
      ringSprite->functionIndex = EVTF_NOOP;
      ringSprite->d.sprite.gfxIdx = GFX_RING;

      a = rsin(EVT.angle) * 0xe0 / ONE;
      b = rsin(EVT.angle * 2) * 0xe0 / ONE;
      EVT.angle += 0x80;

      fade = EVT.fade;
      if (fade < 0) {
         fade = 0;
      }

      ringSprite->d.sprite.coords[0].x = evt->x1.n;
      ringSprite->d.sprite.coords[1].x = evt->x1.n;
      ringSprite->d.sprite.coords[2].x = evt->x1.n;
      ringSprite->d.sprite.coords[3].x = evt->x1.n;
      ringSprite->d.sprite.coords[0].z = evt->z1.n;
      ringSprite->d.sprite.coords[1].z = evt->z1.n;
      ringSprite->d.sprite.coords[2].z = evt->z1.n;
      ringSprite->d.sprite.coords[3].z = evt->z1.n;

      if (EVT.todo_x4e == 0) {
         ringSprite->d.sprite.coords[0].x -= b;
         ringSprite->d.sprite.coords[1].x += b;
         ringSprite->d.sprite.coords[2].x -= b;
         ringSprite->d.sprite.coords[3].x += b;

         ringSprite->d.sprite.coords[0].z += (a * EVT.todo_x50);
         ringSprite->d.sprite.coords[1].z = ringSprite->d.sprite.coords[0].z;
         ringSprite->d.sprite.coords[2].z = ringSprite->d.sprite.coords[0].z;
         ringSprite->d.sprite.coords[3].z = ringSprite->d.sprite.coords[0].z;
      } else {
         ringSprite->d.sprite.coords[0].z -= b;
         ringSprite->d.sprite.coords[1].z += b;
         ringSprite->d.sprite.coords[2].z -= b;
         ringSprite->d.sprite.coords[3].z += b;

         ringSprite->d.sprite.coords[0].x += (a * EVT.todo_x50);
         ringSprite->d.sprite.coords[1].x = ringSprite->d.sprite.coords[0].x;
         ringSprite->d.sprite.coords[2].x = ringSprite->d.sprite.coords[0].x;
         ringSprite->d.sprite.coords[3].x = ringSprite->d.sprite.coords[0].x;
      }

      ringSprite->d.sprite.coords[0].y = evt->y1.n - b;
      ringSprite->d.sprite.coords[1].y = ringSprite->d.sprite.coords[0].y;
      ringSprite->d.sprite.coords[2].y = evt->y1.n + b;
      ringSprite->d.sprite.coords[3].y = ringSprite->d.sprite.coords[2].y;

      ringSprite->d.sprite.clut = 4;
      ringSprite->d.sprite.semiTrans = 2;
      AddEvtPrim4(gGraphicsPtr->ot, ringSprite);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, fade, fade, fade);

      ringSprite->d.sprite.clut = 10;
      ringSprite->d.sprite.semiTrans = 3;
      AddEvtPrim3(gGraphicsPtr->ot, ringSprite);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, fade, fade, fade);

      EVT.fade -= 24;
      if (EVT.fade <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      ringSprite->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 078
void Evtf078_SpellFx2_Damage(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_BOUNCE_ZOOM;
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_DISPLAY_DAMAGE_2;
      newEvt->x1.s.hi = evt->x1.s.hi;
      newEvt->z1.s.hi = evt->z1.s.hi;
      EVT.timer = 30;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer == -1) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 079
void Evtf079_SpellFx3_Slay(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_BOUNCE_ZOOM;
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_STRETCH_WARP_SPRITE;
      newEvt->x1.s.hi = evt->x1.s.hi;
      newEvt->z1.s.hi = evt->z1.s.hi;
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_DISPLAY_DAMAGE_2;
      newEvt->x1.s.hi = evt->x1.s.hi;
      newEvt->z1.s.hi = evt->z1.s.hi;
      EVT.timer = 30;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer == -1) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF Unk8006183c
void Evtf_Unk_8006183c(EvtData *evt) {
   EvtData *unitSprite, *fxSprite;
   Quad quad;
   s32 i;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 1;
      if (EVT.clut == 0) {
         EVT.clut = 4;
      }
      if (EVT.scale == 0) {
         EVT.scale = 110;
      }
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      fxSprite = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite);
      fxSprite->d.sprite.hidden = 0;
      fxSprite->x1.n = unitSprite->x1.n;
      fxSprite->z1.n = unitSprite->z1.n;
      fxSprite->y1.n = unitSprite->y1.n;
      fxSprite->d.sprite.semiTrans = 0;
      fxSprite->d.sprite.clut = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);

      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         fxSprite->z1.n += 16;
         fxSprite->x1.n -= 16;
         break;
      case CAM_DIR_EAST:
         fxSprite->x1.n -= 16;
         fxSprite->z1.n -= 16;
         break;
      case CAM_DIR_NORTH:
         fxSprite->z1.n -= 16;
         fxSprite->x1.n += 16;
         break;
      case CAM_DIR_WEST:
         fxSprite->z1.n += 16;
         fxSprite->x1.n += 16;
         break;
      }

      for (i = 0; i < 4; i++) {
         quad[i].vx = gQuad_800fe53c[i].vx;
         quad[i].vz = gQuad_800fe53c[i].vz;
         quad[i].vy = gQuad_800fe53c[i].vy;
      }
      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx *= EVT.scale;
         gQuad_800fe53c[i].vz *= EVT.scale;
         gQuad_800fe53c[i].vy *= EVT.scale;
      }
      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx /= 100;
         gQuad_800fe53c[i].vz /= 100;
         gQuad_800fe53c[i].vy /= 100;
      }

      fxSprite->y1.n -= (abs(gQuad_800fe53c[0].vy) - abs(quad[0].vy));
      fxSprite->d.sprite.semiTrans = 1;
      fxSprite->d.sprite.clut = EVT.clut;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite, 0);

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx = quad[i].vx;
         gQuad_800fe53c[i].vz = quad[i].vz;
         gQuad_800fe53c[i].vy = quad[i].vy;
      }

      fxSprite->functionIndex = EVTF_NULL;
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      unitSprite = EVT.unitSprite;
      unitSprite->d.sprite.hidden = 0;
      break;
   }
}

#undef EVTF
#define EVTF 802
void Evtf802_Fx_TBD(EvtData *evt) {
   static s16 explosionAnimData[26] = {7, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                                       2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                                       2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                                       2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                                       0, GFX_NULL};

   EvtData *sprite;
   s32 rnd1, rnd2;
   s16 a;

   gQuad_800fe63c[0].vx = -12;
   gQuad_800fe63c[0].vy = -24;
   gQuad_800fe63c[0].vz = 0;
   gQuad_800fe63c[1].vx = 12;
   gQuad_800fe63c[1].vy = -24;
   gQuad_800fe63c[1].vz = 0;
   gQuad_800fe63c[2].vx = -12;
   gQuad_800fe63c[2].vy = 0;
   gQuad_800fe63c[2].vz = 0;
   gQuad_800fe63c[3].vx = 12;
   gQuad_800fe63c[3].vy = 0;
   gQuad_800fe63c[3].vz = 0;

   switch (evt->state) {
   case 0:
      rnd1 = rand();
      EVT.theta = rnd1 % ANGLE_360_DEGREES;
      rnd2 = rand();
      EVT.h = (rnd2 % 0x60) - 0xd0;

      EVT.sprite = sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = explosionAnimData;
      sprite->d.sprite.animInitialized = 0;
      sprite->d.sprite.clut = EVT.clut;
      sprite->d.sprite.semiTrans = EVT.semiTrans;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer <= 0) {
         evt->state++;
      }
      break;

   case 2:
      sprite = EVT.sprite;
      a = EVT.theta;

      gQuad_800fe63c[0].vy = -6 - ((EVT.todo_x28 - 6) * abs(rsin(EVT.todo_x34)) / ONE);
      gQuad_800fe63c[1].vy = gQuad_800fe63c[0].vy;

      gQuad_800fe63c[0].vx = -3 - ((EVT.todo_x28 / 2 - 3) * abs(rsin(EVT.todo_x34)) / ONE);
      gQuad_800fe63c[2].vx = gQuad_800fe63c[0].vx;
      gQuad_800fe63c[1].vx = -gQuad_800fe63c[0].vx;
      gQuad_800fe63c[3].vx = -gQuad_800fe63c[0].vx;

      EVT.todo_x34 += EVT.todo_x2a;

      sprite->x1.n = evt->x1.n + (EVT.radius * rcos(a) / ONE);
      sprite->z1.n = evt->z1.n + (EVT.radius * rsin(a) / ONE);
      sprite->y1.n = evt->y1.n + EVT.h;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      if (sprite->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

// Definition order is swapped, so these must not be function-scoped. I guess the others shouldn't
// be either, but I'm fine with whatever works, for now.
static s16 sFlameAnimData_800fead4[20] = {
    7, GFX_FLAME_1, 1, GFX_FLAME_2, 1, GFX_FLAME_3, 1, GFX_FLAME_4, 1, GFX_FLAME_5,
    1, GFX_FLAME_6, 1, GFX_FLAME_7, 1, GFX_FLAME_8, 1, GFX_NULL,    0, GFX_NULL};

static s16 sLightningAnimData_800feafc[20] = {
    7, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4,
    2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6, 2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8,
    2, GFX_NULL,        0, GFX_NULL};

#undef EVTF
#define EVTF 803
void Evtf803_Fx_TBD(EvtData *evt) {
   EvtData *sprite;
   s32 rnd1, rnd2;

   gQuad_800fe63c[0].vx = -12;
   gQuad_800fe63c[0].vy = -24;
   gQuad_800fe63c[0].vz = 0;
   gQuad_800fe63c[1].vx = 12;
   gQuad_800fe63c[1].vy = -24;
   gQuad_800fe63c[1].vz = 0;
   gQuad_800fe63c[2].vx = -12;
   gQuad_800fe63c[2].vy = 0;
   gQuad_800fe63c[2].vz = 0;
   gQuad_800fe63c[3].vx = 12;
   gQuad_800fe63c[3].vy = 0;
   gQuad_800fe63c[3].vz = 0;

   switch (evt->state) {
   case 0:
      rnd1 = rand();
      EVT.theta = rnd1 % ANGLE_360_DEGREES;
      rnd2 = rand();
      EVT.h = (rnd2 % 0x20) - 0xd0;

      EVT.sprite = sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sLightningAnimData_800feafc;
      sprite->d.sprite.animInitialized = 0;
      sprite->d.sprite.clut = EVT.clut;
      sprite->d.sprite.semiTrans = EVT.semiTrans;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer <= 0) {
         evt->state++;
      }
      break;

   case 2:
      sprite = EVT.sprite;

      gQuad_800fe63c[0].vy =
          -12 - ((EVT.todo_x28 - 12) * abs(rsin(EVT.todo_x34)) / ANGLE_360_DEGREES);
      gQuad_800fe63c[1].vy = gQuad_800fe63c[0].vy;

      EVT.todo_x34 += EVT.todo_x2a;

      sprite->x1.n = evt->x1.n + (EVT.radius * rcos(EVT.theta) / ONE);
      sprite->z1.n = evt->z1.n + (EVT.radius * rsin(EVT.theta) / ONE);
      sprite->y1.n = evt->y1.n + EVT.h;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      if (sprite->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 801
void Evtf801_Fx_TBD(EvtData *evt) {
   EvtData *sprite;
   s32 rnd1, rnd2;

   gQuad_800fe63c[0].vx = -12;
   gQuad_800fe63c[0].vy = -32;
   gQuad_800fe63c[0].vz = 0;
   gQuad_800fe63c[1].vx = 12;
   gQuad_800fe63c[1].vy = -32;
   gQuad_800fe63c[1].vz = 0;
   gQuad_800fe63c[2].vx = -12;
   gQuad_800fe63c[2].vy = 0;
   gQuad_800fe63c[2].vz = 0;
   gQuad_800fe63c[3].vx = 12;
   gQuad_800fe63c[3].vy = 0;
   gQuad_800fe63c[3].vz = 0;

   Noop_800aa0ac(10, 10, evt->state);

   switch (evt->state) {
   case 0:
      rnd1 = rand();
      EVT.theta = rnd1 % ANGLE_360_DEGREES;
      rnd2 = rand();
      EVT.h = (rnd2 % 0x46) - 0xd0;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.animData = sFlameAnimData_800fead4;
      sprite->d.sprite.clut = 0;
      sprite->d.sprite.semiTrans = 2;
      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer <= 0) {
         evt->state++;
      }
      break;

   case 2:
      sprite = EVT.sprite;

      gQuad_800fe63c[0].vy =
          -16 - ((EVT.todo_x28 - 16) * abs(rsin(EVT.todo_x34)) / ANGLE_360_DEGREES);
      gQuad_800fe63c[1].vy = gQuad_800fe63c[0].vy;

      EVT.todo_x34 += EVT.todo_x2a;

      sprite->x1.n = evt->x1.n + (EVT.radius * rcos(EVT.theta) >> 12);
      sprite->z1.n = evt->z1.n + (EVT.radius * rsin(EVT.theta) >> 12);
      sprite->y1.n = evt->y1.n + EVT.h;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      if (sprite->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 133
void Evtf133_Fx_TBD(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      newEvt = CreatePositionedEvt(evt, EVTF_FX_TBD_801);
      newEvt->d.evtf801.radius = EVT.radius;
      newEvt->d.evtf801.todo_x2a = EVT.todo_x2a;
      newEvt->d.evtf801.todo_x28 = EVT.todo_x28;
      newEvt->d.evtf801.semiTrans = EVT.semiTrans;
      newEvt->d.evtf801.clut = EVT.clut;
      newEvt->d.evtf801.timer = EVT.timer;
      evt->mem--;
      if (evt->mem <= 0) {
         evt->state++;
      }
      break;
   case 1:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 141
void Evtf141_Fx_TBD(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      newEvt = CreatePositionedEvt(evt, EVTF_FX_TBD_802);
      newEvt->d.evtf802.radius = EVT.radius;
      newEvt->d.evtf802.todo_x2a = EVT.todo_x2a;
      newEvt->d.evtf802.todo_x28 = EVT.todo_x28;
      newEvt->d.evtf802.semiTrans = EVT.semiTrans;
      newEvt->d.evtf802.clut = EVT.clut;
      newEvt->d.evtf802.timer = EVT.timer;
      evt->mem--;
      if (evt->mem <= 0) {
         evt->state++;
      }
      break;
   case 1:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 137
void Evtf137_Fx_TBD(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      newEvt = CreatePositionedEvt(evt, EVTF_FX_TBD_803);
      newEvt->d.evtf803.radius = EVT.radius;
      newEvt->d.evtf803.todo_x2a = EVT.todo_x2a;
      newEvt->d.evtf803.todo_x28 = EVT.todo_x28;
      newEvt->d.evtf803.semiTrans = EVT.semiTrans;
      newEvt->d.evtf803.clut = EVT.clut;
      newEvt->d.evtf803.timer = EVT.timer;
      evt->mem--;
      if (evt->mem <= 0) {
         evt->state++;
      }
      break;
   case 1:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 132
/* 132, 134, 136, 138, 140, 142, 799, 800 */
void Evtf132_Etc_Fx_TBD(EvtData *evt) {
   static struct {
      s16 clut, evtf, radius, to_x28, to_x2a;
   } fxInfo[3] = {{3, EVTF_FX_TBD_133, 0x70, 0x60, 0x100},
                  {8, EVTF_FX_TBD_137, 0x80, 0x50, 0x100},
                  {3, EVTF_FX_TBD_141, 0x70, 0x20, 0xaa}};

   EvtData *unitSprite, *fxEvt1, *fxEvt2, *dmgEvt;
   POLY_FT4 *poly;
   s32 i, fadeSpeed, stretchSpeed;
   Quad quad;

   switch (evt->state) {
   case 0:

      switch (evt->functionIndex) {
      case EVTF_FX_TBD_132:
      case EVTF_FX_TBD_134:
         i = 0;
         break;

      case EVTF_FX_TBD_138:
      case EVTF_FX_TBD_136:
      case EVTF_FX_TBD_799:
      case EVTF_FX_TBD_800:
         i = 1;
         break;

      case EVTF_FX_TBD_142:
         i = 2;
         break;

      case EVTF_FX_TBD_141:
      default:
         i = 2;
         break;
      }

      EVT.unitSprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 1;
      EVT.fade = 0x80;
      EVT.increasing = 0;
      if (EVT.clut == 0) {
         EVT.clut = fxInfo[i].clut;
      }

      fxEvt1 = Evt_GetUnused();
      fxEvt1->functionIndex = fxInfo[i].evtf;
      fxEvt1->x1.n = unitSprite->x1.n;
      fxEvt1->z1.n = unitSprite->z1.n;
      fxEvt1->y1.n = unitSprite->y1.n + 0xd0;
      fxEvt1->mem = 30;
      fxEvt1->d.evtf133.radius = fxInfo[i].radius;
      fxEvt1->d.evtf133.todo_x2a = fxInfo[i].to_x2a;
      fxEvt1->d.evtf133.todo_x28 = fxInfo[i].to_x28;
      fxEvt1->d.evtf133.semiTrans = 2;
      fxEvt1->d.evtf133.clut = EVT.clut;
      fxEvt1->d.evtf133.timer = 0;

      evt->state++;
      break;

   case 1:
      unitSprite = EVT.unitSprite;
      fxEvt1 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxEvt1);
      fxEvt2 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxEvt2);

      fxEvt1->d.sprite.hidden = 0;
      fxEvt2->d.sprite.hidden = 0;
      fxEvt2->d.sprite.clut = EVT.clut;
      fxEvt1->d.sprite.clut = EVT.clut;
      fxEvt1->d.sprite.semiTrans = 2;

      RenderUnitSprite(gGraphicsPtr->ot, fxEvt1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, fxEvt2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      if (!EVT.increasing) {
         EVT.fade -= 12;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.increasing = 1;
         }
      } else {
         EVT.fade += 12;
         if (EVT.fade >= 0x80) {
            EVT.fade = 0x80;
            EVT.increasing = 0;
         }
      }

      EVT.phase++;
      if (EVT.phase == 30) {
         dmgEvt = Evt_GetUnused();
         dmgEvt->functionIndex = EVTF_DISPLAY_DAMAGE_2;
         dmgEvt->x1.s.hi = evt->x1.s.hi;
         dmgEvt->z1.s.hi = evt->z1.s.hi;

         if (evt->functionIndex == EVTF_FX_TBD_132 || evt->functionIndex == EVTF_FX_TBD_136 ||
             evt->functionIndex == EVTF_FX_TBD_140 || evt->functionIndex == EVTF_FX_TBD_799) {
            unitSprite->d.sprite.hidden = 0;
         }
      } else if (EVT.phase == 60) {
         fxEvt1->functionIndex = EVTF_NULL;
         fxEvt2->functionIndex = EVTF_NULL;
         EVT.phase = 0;

         if (evt->functionIndex == EVTF_FX_TBD_132 || evt->functionIndex == EVTF_FX_TBD_136 ||
             evt->functionIndex == EVTF_FX_TBD_140 || evt->functionIndex == EVTF_FX_TBD_799) {
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
            return;
         }

         evt->state++;
         break;
      }

      fxEvt1->functionIndex = EVTF_NULL;
      fxEvt2->functionIndex = EVTF_NULL;
      break;

   case 2:

      switch (evt->functionIndex) {
      case EVTF_FX_TBD_138:
         fadeSpeed = 8;
         stretchSpeed = 0xc0;
         break;
      case EVTF_FX_TBD_134:
         fadeSpeed = 12;
         stretchSpeed = 0xe0;
         break;
      case EVTF_FX_TBD_139:
         fadeSpeed = 8;
         stretchSpeed = 0xc0;
         break;
      default:
         fadeSpeed = 8;
         stretchSpeed = 0xc0;
      }

      unitSprite = EVT.unitSprite;
      fxEvt1 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxEvt1);
      fxEvt2 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxEvt2);
      fxEvt1->d.sprite.hidden = 0;
      fxEvt2->d.sprite.hidden = 0;
      fxEvt2->d.sprite.clut = EVT.clut;
      fxEvt1->d.sprite.clut = EVT.clut;
      fxEvt1->d.sprite.semiTrans = 2;

      for (i = 0; i < 4; i++) {
         quad[i].vx = gQuad_800fe53c[i].vx;
         quad[i].vz = gQuad_800fe53c[i].vz;
         quad[i].vy = gQuad_800fe53c[i].vy;
      }

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx =
             (quad[i].vx * (rsin(EVT.phase) + ANGLE_360_DEGREES)) / ANGLE_360_DEGREES;
         gQuad_800fe53c[i].vz =
             (quad[i].vz * (rsin(EVT.phase) + ANGLE_360_DEGREES)) / ANGLE_360_DEGREES;

         if (EVT.phase <= ANGLE_180_DEGREES) {
            gQuad_800fe53c[i].vy = quad[i].vy *
                                   (rsin(EVT.phase + ANGLE_180_DEGREES) / 2 + ANGLE_180_DEGREES) /
                                   ANGLE_180_DEGREES;
         } else {
            gQuad_800fe53c[i].vy = quad[i].vy *
                                   (rsin(EVT.phase + ANGLE_180_DEGREES) * 3 + ANGLE_360_DEGREES) /
                                   ANGLE_360_DEGREES;
         }
      }

      RenderUnitSprite(gGraphicsPtr->ot, fxEvt1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, fxEvt2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      for (i = 0; i < 4; i++) {
         gQuad_800fe53c[i].vx = quad[i].vx;
         gQuad_800fe53c[i].vz = quad[i].vz;
         gQuad_800fe53c[i].vy = quad[i].vy;
      }

      if (!EVT.increasing) {
         EVT.fade -= fadeSpeed;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.increasing = 1;
         }
      } else {
         EVT.fade += fadeSpeed;
         if (EVT.fade >= 0x80) {
            EVT.fade = 0x80;
            EVT.increasing = 0;
         }
      }

      EVT.phase += stretchSpeed;
      if (EVT.phase > ANGLE_270_DEGREES) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      fxEvt1->functionIndex = EVTF_NULL;
      fxEvt2->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 149
void Evtf149_Fx_TBD(EvtData *evt) {
   EvtData *unitSprite, *fxSprite1, *fxSprite2;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.fade = 0x80;
      EVT.increasing = 0;
      unitSprite->d.sprite.hidden = 1;
      if (EVT.clut == 0) {
         EVT.clut = 3;
      }
      if (EVT.fadeSpeed == 0) {
         EVT.fadeSpeed = 16;
      }
      evt->state++;
      break;

   case 1:
      unitSprite = EVT.unitSprite;
      fxSprite1 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite1);
      fxSprite2 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite2);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite2->d.sprite.hidden = 0;
      fxSprite1->d.sprite.semiTrans = 2;
      fxSprite1->d.sprite.clut = EVT.clut;

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x80 - EVT.fade, 0x80 - EVT.fade, 0x80 - EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      if (!EVT.increasing) {
         EVT.fade -= EVT.fadeSpeed;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.increasing = 1;
         }
      } else {
         EVT.fade += EVT.fadeSpeed;
         if (EVT.fade >= 0x80) {
            EVT.fade = 0x80;
            EVT.increasing = 0;
         }
      }

      fxSprite2->functionIndex = EVTF_NULL;
      fxSprite1->functionIndex = EVTF_NULL;
      break;

   case 99:
      unitSprite = EVT.unitSprite;
      unitSprite->d.sprite.hidden = 0;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

// Also used for blood (by setting poly color to red)
static s16 sImpactAnimData_800feb44[18] = {5, GFX_IMPACT_1, 2, GFX_IMPACT_2, 2, GFX_IMPACT_3,
                                           2, GFX_IMPACT_4, 2, GFX_IMPACT_5, 2, GFX_IMPACT_6,
                                           2, GFX_IMPACT_7, 2, GFX_NULL,     0, GFX_NULL};

// Used instead of blood for "rocky" enemy types like golems
static s16 sRockAnimData_800feb68[20] = {5, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3, 2, GFX_ROCK_4,
                                         2, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3, 2, GFX_ROCK_4,
                                         2, GFX_NULL,   1, GFX_NULL};

// Used for clouds of sand or dust, etc.
static s16 sCloudAnimData_800feb90[24] = {
    7, GFX_PUFF_1, 1, GFX_PUFF_2, 1, GFX_PUFF_3, 1, GFX_PUFF_4,  1, GFX_PUFF_5, 1, GFX_PUFF_6,
    1, GFX_PUFF_7, 1, GFX_PUFF_8, 1, GFX_PUFF_9, 1, GFX_PUFF_10, 1, GFX_NULL,   0, GFX_NULL};

#undef EVTF
#define EVTF 215
void Evtf215_Cloud(EvtData *evt) {
   EvtData *sprite;
   s16 a;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sCloudAnimData_800feb90;
      sprite->d.sprite.animInitialized = 0;
      EVT.sprite = sprite;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position1.x = rsin(a) >> 5;
      EVT.position1.z = rcos(a) >> 5;
      EVT.position1.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position2.x = rsin(a) >> 5;
      EVT.position2.z = rcos(a) >> 5;
      EVT.position2.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position3.x = rsin(a) >> 5;
      EVT.position3.z = rcos(a) >> 5;
      EVT.position3.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position4.x = rsin(a) >> 5;
      EVT.position4.z = rcos(a) >> 5;
      EVT.position4.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position5.x = rsin(a) >> 5;
      EVT.position5.z = rcos(a) >> 5;
      EVT.position5.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position6.x = rsin(a) >> 5;
      EVT.position6.z = rcos(a) >> 5;
      EVT.position6.y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position7_x = rsin(a) >> 5;
      EVT.position7_z = rcos(a) >> 5;
      EVT.position7_y = (rand() % 0x60) + 0x20;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position8_x = rsin(a) >> 5;
      EVT.position8_z = rcos(a) >> 5;
      EVT.position8_y = (rand() % 0x60) + 0x20;

      EVT.halfSize = (rand() % 6) + 4;
      evt->state++;

   // fallthrough
   case 1:
      a = EVT.halfSize;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -a;
      gQuad_800fe63c[1].vx = a;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -a;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = a;

      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);

      sprite->x1.n = evt->x1.n + EVT.position1.x +
                     (EVT.position1.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position1.z +
                     (EVT.position1.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position1.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position2.x +
                     (EVT.position2.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position2.z +
                     (EVT.position2.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position2.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position3.x +
                     (EVT.position3.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position3.z +
                     (EVT.position3.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position3.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position4.x +
                     (EVT.position4.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position4.z +
                     (EVT.position4.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position4.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position5.x +
                     (EVT.position5.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position5.z +
                     (EVT.position5.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position5.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position6.x +
                     (EVT.position6.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position6.z +
                     (EVT.position6.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position6.y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position7_x +
                     (EVT.position6.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position7_z +
                     (EVT.position6.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.position7_y * rsin(EVT.theta)) / ONE;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      sprite->x1.n = evt->x1.n + EVT.position8_x +
                     (EVT.position6.x * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
      sprite->z1.n = evt->z1.n + EVT.position8_z +
                     (EVT.position6.z * (ANGLE_360_DEGREES - rcos(EVT.theta)) / ONE);
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

#undef EVTF
#define EVTF 213
void Evtf213_DustCloudSpawner(EvtData *evt) {
   // Spawned by: EVTF_MAP_OBJECT_CRATE->213; SetupMapExtras()->362->213 (lowering bridge, etc.)
   EvtData *particle;

   switch (evt->state) {
   case 0:
      EVT.timer = 10;
      evt->state++;
   // fallthrough
   case 1:
      if (EVT.timer > 2) {
         particle = Evt_GetUnused();
         particle->functionIndex = EVTF_DUST_CLOUD;
         particle->x1.n = evt->x1.n;
         particle->z1.n = evt->z1.n;
         particle->y1.n = evt->y1.n;
      }
      if (--EVT.timer == -1) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 214
void Evtf214_DustCloud(EvtData *evt) {
   SVectorXZY *pPosition;
   EvtData *sprite;
   s32 i;
   s16 a, theta;
   s16 qx, qz;

   pPosition = &EVT.positions[0];

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sCloudAnimData_800feb90;
      sprite->d.sprite.animInitialized = 0;

      EVT.halfSize = rand() % 4 + 4;
      EVT.sprite = sprite;

      for (i = 0; i < ARRAY_COUNT(EVT.positions); i++, pPosition++) {
         a = rand() % ANGLE_360_DEGREES;
         pPosition->x = rsin(a) >> 5;
         pPosition->z = rcos(a) >> 5;
         pPosition->y = (rand() % 0x40) + 0x20;
      }

      evt->state++;

   // fallthrough
   case 1:
      a = EVT.halfSize;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -a;
      gQuad_800fe63c[1].vx = a;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -a;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = a;

      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);

      theta = EVT.theta;

      for (i = 0; i < ARRAY_COUNT(EVT.positions); i++, pPosition++) {
         qx = pPosition->x >> 2;
         qz = pPosition->z >> 2;

         sprite->x1.n = evt->x1.n + pPosition->x + qx +
                        (pPosition->x * (ANGLE_360_DEGREES - rcos(theta / 2)) / ONE);

         sprite->z1.n = evt->z1.n + pPosition->z + qz +
                        (pPosition->z * (ANGLE_360_DEGREES - rcos(theta / 2)) / ONE);

         sprite->y1.n = evt->y1.n + (pPosition->y * rsin(theta) / ONE);

         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      }

      EVT.theta += (ANGLE_180_DEGREES / 10);
      if (EVT.theta >= (ANGLE_180_DEGREES / 10 * 10)) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 130
void Evtf130_Fx_TBD(EvtData *evt) {
   EvtData *sprite, *unitSprite;
   s32 i, j;
   s16 a, b, c, d, e; // TBD

   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;

      switch (unitSprite->d.sprite.direction) {
      case ANGLE_WEST:
         EVT.todo_x50 = -1;
         EVT.todo_x4e = 0;
         break;
      case ANGLE_EAST:
         EVT.todo_x50 = 1;
         EVT.todo_x4e = 0;
         break;
      case ANGLE_SOUTH:
         EVT.todo_x50 = 0;
         EVT.todo_x4e = -1;
         break;
      case ANGLE_NORTH:
         EVT.todo_x50 = 0;
         EVT.todo_x4e = 1;
         break;
      }

      evt->state++;

   // fallthrough
   case 1:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_COLOR_13;
      sprite->d.sprite.clut = 4;
      sprite->d.sprite.semiTrans = 1;

      switch (EVT.todo_x50) {
      case 0:
         a = (EVT.todo_x4e == 1) ? ANGLE_180_DEGREES : 0;
         break;
      case 1:
         a = (EVT.todo_x50 == 1) ? ANGLE_180_DEGREES : 0;
         break;
      default:
         a = 0;
         break;
      }

      for (i = EVT.todo_x24; i < EVT.todo_x26; i++) {
         b = (rsin(i * 401) * 13) >> 8;
         c = (rsin((i + 1) * 401) * 13) >> 8;
         d = (rcos(a + i * 401) * 13) >> 8;
         e = (rcos(a + (i + 1) * 401) * 13) >> 8;

         for (j = 0; j < 16; j++) {

            switch (EVT.todo_x50) {
            case 0:
               sprite->d.sprite.coords[0].x = evt->x1.n + (b * rcos(j * 128) >> 12);
               sprite->d.sprite.coords[0].z = evt->z1.n + d;

               sprite->d.sprite.coords[1].x = evt->x1.n + (b * rcos((j + 1) * 128) >> 12);
               sprite->d.sprite.coords[1].z = evt->z1.n + d;

               sprite->d.sprite.coords[2].x = evt->x1.n + (c * rcos(j * 128) >> 12);
               sprite->d.sprite.coords[2].z = evt->z1.n + e;

               sprite->d.sprite.coords[3].x = evt->x1.n + (c * rcos((j + 1) * 128) >> 12);
               sprite->d.sprite.coords[3].z = evt->z1.n + e;
               break;
            case 1:
               sprite->d.sprite.coords[0].z = evt->z1.n + (b * rcos(j * 128) >> 12);
               sprite->d.sprite.coords[0].x = evt->x1.n + d;

               sprite->d.sprite.coords[1].z = evt->z1.n + (b * rcos((j + 1) * 128) >> 12);
               sprite->d.sprite.coords[1].x = evt->x1.n + d;

               sprite->d.sprite.coords[2].z = evt->z1.n + (c * rcos(j * 128) >> 12);
               sprite->d.sprite.coords[2].x = evt->x1.n + e;

               sprite->d.sprite.coords[3].z = evt->z1.n + (c * rcos((j + 1) * 128) >> 12);
               sprite->d.sprite.coords[3].x = evt->x1.n + e;
               break;
            }

            sprite->d.sprite.coords[0].y = evt->y1.n + (b * rsin(j * 128) >> 12);
            sprite->d.sprite.coords[1].y = evt->y1.n + (b * rsin((j + 1) * 128) >> 12);

            sprite->d.sprite.coords[2].y = evt->y1.n + (c * rsin(j * 128) >> 12);
            sprite->d.sprite.coords[3].y = evt->y1.n + (c * rsin((j + 1) * 128) >> 12);

            AddEvtPrim4(gGraphicsPtr->ot, sprite);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, 0xff, 0xff, 0xff);
         }
      }

      sprite->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.todo_x26++;
         if (EVT.todo_x26 == 6) {
            evt->state2++;
            EVT.todo_x26 = 5;
         }
         break;
      case 1:
         EVT.todo_x24++;
         if (EVT.todo_x24 == 5) {
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 131
void Evtf131_SlayUnit(EvtData *evt) {
   EvtData *warpFx;
   s16 initSpeed;

   switch (evt->state) {
   case 0:
      warpFx = Evt_GetUnused();
      warpFx->functionIndex = EVTF_STRETCH_WARP_SPRITE;
      warpFx->x1.s.hi = evt->x1.s.hi;
      warpFx->z1.s.hi = evt->z1.s.hi;
      warpFx->d.evtf062.speed = EVT.speed;
      if (EVT.unitSprite != NULL) {
         warpFx->d.evtf062.sprite = EVT.unitSprite;
      }

      if (EVT.speed == 0) {
         initSpeed = 0xc0;
      } else {
         initSpeed = EVT.speed;
      }
      EVT.speed = (0xc00 / initSpeed);

      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.speed == -1) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef EVTF
#define EVTF 201
void Evtf201_UnitStruck(EvtData *evt) {
   static s16 impactAnimData[18] = {3, GFX_IMPACT_1, 2, GFX_IMPACT_2, 2, GFX_IMPACT_3,
                                    2, GFX_IMPACT_4, 2, GFX_IMPACT_5, 2, GFX_IMPACT_6,
                                    2, GFX_IMPACT_7, 2, GFX_NULL,     0, GFX_NULL};

   EvtData *evt_s0;
   EvtData *evt_s2;
   EvtData *evt_s3;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;
      evt->y1.n = evt_s0->y1.n;
      EVT.unitSprite = evt_s0;
      evt_s0->d.sprite.hidden = 1;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      evt_s0->d.sprite.animData = impactAnimData;
      evt_s0->x1.n = evt->x1.n;
      evt_s0->z1.n = evt->z1.n;
      evt_s0->y1.n = evt->y1.n + 0xe0;

      switch (evt_s0->d.sprite.direction) {
      case ANGLE_WEST:
         evt_s0->x1.n += 0x20;
         break;
      case ANGLE_EAST:
         evt_s0->x1.n -= 0x20;
         break;
      case ANGLE_SOUTH:
         evt_s0->z1.n += 0x20;
         break;
      case ANGLE_NORTH:
         evt_s0->z1.n -= 0x20;
         break;
      }

      EVT.impactSprite = evt_s0;
      evt->state++;

   // fallthrough
   case 1:
      evt_s0 = EVT.unitSprite;
      evt_s2 = Evt_GetUnused();
      CopyEvtData(evt_s0, evt_s2);
      evt_s3 = Evt_GetUnused();
      CopyEvtData(evt_s0, evt_s3);
      evt_s2->d.sprite.hidden = 0;
      evt_s3->d.sprite.hidden = 0;
      evt_s2->d.sprite.semiTrans = 2;
      evt_s2->d.sprite.clut = 10;

      RenderUnitSprite(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x80 - EVT.fade, 0x80 - EVT.fade, 0x80 - EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, evt_s3, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      if (EVT.flashState == 0) {
         EVT.fade -= 0x20;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.flashState = 1;
         }
      } else if (EVT.flashState == 1) {
         EVT.fade += 0x20;
         if (EVT.fade >= 0x80) {
            EVT.fade = 0x80;
            EVT.flashState = 2;
         }
      }

      evt_s0 = EVT.impactSprite;
      UpdateEvtAnimation(evt_s0);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s0, 0);

      if (evt_s0->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         evt_s0 = EVT.impactSprite;
         evt_s0->functionIndex = EVTF_NULL;
         evt_s0 = EVT.unitSprite;
         evt_s0->d.sprite.hidden = 0;
      }

      evt_s3->functionIndex = EVTF_NULL;
      evt_s2->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 202
void Evtf202_746_UnitBlocking(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *evt_s2;
   EvtData *evt_s3;
   POLY_FT4 *poly;
   s32 i;

   switch (evt->state) {
   case 0:
      if (evt->functionIndex == EVTF_ENTITY_BLOCKING) {
         evt_s0 = EVT.variant_0x24.entitySpriteParam;
         evt_s0->d.sprite.direction = DIR_WEST;
         EVT.variant_0x24.entitySpriteParam = NULL;
      } else {
         evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      }

      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;
      evt->y1.n = evt_s0->y1.n;
      EVT.unitSprite = evt_s0;
      evt_s0->d.sprite.hidden = 1;
      EVT.variant_0x24.timer = 14;

      for (i = 0; i < 8; i++) {
         evt_s3 = Evt_GetUnused();
         evt_s3->functionIndex = EVTF_BLOCKING_IMPACT_PARTICLE;
         evt_s3->x1.n = evt->x1.n;
         evt_s3->z1.n = evt->z1.n;
         evt_s3->y1.n = evt->y1.n + 0xe0;
         evt_s3->d.evtf203.direction = evt_s0->d.sprite.direction;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s0 = EVT.unitSprite;
      evt_s2 = Evt_GetUnused();
      CopyEvtData(evt_s0, evt_s2);
      evt_s3 = Evt_GetUnused();
      CopyEvtData(evt_s0, evt_s3);
      evt_s2->d.sprite.hidden = 0;
      evt_s3->d.sprite.hidden = 0;
      evt_s2->d.sprite.semiTrans = 2;
      evt_s2->d.sprite.clut = 10;

      RenderUnitSprite(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x80 - EVT.fade, 0x80 - EVT.fade, 0x80 - EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, evt_s3, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      if (EVT.flashState == 0) {
         EVT.fade -= 0x20;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.flashState = 1;
         }
      } else if (EVT.flashState == 1) {
         EVT.fade += 0x20;
         if (EVT.fade >= 0x80) {
            EVT.fade = 0x80;
            EVT.flashState = 2;
         }
      }

      if (--EVT.variant_0x24.timer == -1) {
         evt->functionIndex = EVTF_NULL;
         evt_s0 = EVT.unusedSprite;
         evt_s0->functionIndex = EVTF_NULL;
         evt_s0 = EVT.unitSprite;
         evt_s0->d.sprite.hidden = 0;
      }

      evt_s3->functionIndex = EVTF_NULL;
      evt_s2->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 203
void Evtf203_BlockingImpactParticle(EvtData *evt) {
   EvtData *sprite;
   s16 a;
   s8 unused[32];

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.clut = 4;
      sprite->d.sprite.animData = sImpactAnimData_800feb44;
      sprite->d.sprite.animInitialized = 0;
      EVT.sprite = sprite;

      a = rand() % ANGLE_360_DEGREES;

      switch (EVT.direction) {
      case ANGLE_WEST:
         evt->x1.n += 0x20;
         EVT.todo_x4e = rsin(a) / 24;
         EVT.todo_x50 = rcos(a) / 24;
         EVT.todo_x4c = 0xc0 - rand() % 0x180;
         break;
      case ANGLE_EAST:
         evt->x1.n -= 0x20;
         EVT.todo_x4e = rsin(a) / 24;
         EVT.todo_x50 = rcos(a) / 24;
         EVT.todo_x4c = 0xc0 - rand() % 0x180;
         break;
      case ANGLE_SOUTH:
         evt->z1.n += 0x20;
         EVT.todo_x4c = rsin(a) / 24;
         EVT.todo_x50 = rcos(a) / 24;
         EVT.todo_x4e = 0xc0 - rand() % 0x180;
         break;
      case ANGLE_NORTH:
         evt->z1.n -= 0x20;
         EVT.todo_x4c = rsin(a) / 24;
         EVT.todo_x50 = rcos(a) / 24;
         EVT.todo_x4e = 0xc0 - rand() % 0x180;
         break;
      }

      EVT.todo_x50 = abs(EVT.todo_x50) * 3;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      sprite->x1.n = evt->x1.n + (EVT.todo_x4c * rsin(EVT.todo_x28) / ONE);
      sprite->z1.n = evt->z1.n + (EVT.todo_x4e * rsin(EVT.todo_x28) / ONE);
      sprite->y1.n = evt->y1.n + (EVT.todo_x50 * rsin(EVT.todo_x28) / ONE) - (EVT.todo_x28 / 4);

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      EVT.todo_x28 += 0x4e;
      if (EVT.todo_x28 >= ANGLE_90_DEGREES) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 205
void Evtf205_BloodSpurt(EvtData *evt) {
   s32 particleEvtf;
   EvtData *unitSprite;
   EvtData *evt_s2;
   EvtData *evt_s4;
   POLY_FT4 *poly;

   //? Rock spurt stuff may be left-over code since separate evtf exists (Evtf685_RockSpurt)
   particleEvtf = (evt->functionIndex == EVTF_ROCK_SPURT) ? EVTF_ROCK_SPURT_PARTICLE_2
                                                          : EVTF_BLOOD_SPURT_PARTICLE;

   switch (evt->state) {
   case 0:
      PerformAudioCommand(0x5df);
      if (EVT.unitSprite == NULL) {
         EVT.unitSprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      } else {
         unitSprite = EVT.unitSprite;
      }
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;
      unitSprite->d.sprite.hidden = 1;
      EVT.timer = 22;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      evt_s4 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s4);
      evt_s2 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s2);
      evt_s4->d.sprite.hidden = 0;
      evt_s2->d.sprite.hidden = 0;

      if (evt->functionIndex == EVTF_BLOOD_SPURT) {
         evt_s4->d.sprite.semiTrans = 2;

         RenderUnitSprite(gGraphicsPtr->ot, evt_s4, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade, 0, 0);

         RenderUnitSprite(gGraphicsPtr->ot, evt_s2, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade + 0x7f, 0x80 - EVT.fade, 0x80 - EVT.fade);
      } else {
         RenderUnitSprite(gGraphicsPtr->ot, evt_s4, 0);
      }

      if (EVT.flashState == 0) {
         EVT.fade += 12;
         if (EVT.fade > 0x80) {
            EVT.fade = 0x80;
            EVT.flashState = 1;
         }
      } else if (EVT.flashState == 1) {
         EVT.fade -= 10;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
            EVT.flashState = 2;
         }
      }

      evt_s2->functionIndex = EVTF_NULL;
      evt_s4->functionIndex = EVTF_NULL;

      evt_s2 = Evt_GetUnused();
      if (gState.mapNum == 32) {
         evt_s2->functionIndex = EVTF_BLOOD_SPURT_PARTICLE_OFFSET;
      } else {
         evt_s2->functionIndex = particleEvtf;
      }
      evt_s2->x1.n = unitSprite->x1.n;
      evt_s2->z1.n = unitSprite->z1.n;
      evt_s2->y1.n = unitSprite->y1.n + 0x40 + rand() % 0x20;

      if (EVT.timer % 2 == 0) {
         evt_s2 = Evt_GetUnused();
         if (gState.mapNum == 32) {
            evt_s2->functionIndex = EVTF_BLOOD_SPURT_PARTICLE_OFFSET;
         } else {
            evt_s2->functionIndex = particleEvtf;
         }
         evt_s2->x1.n = unitSprite->x1.n;
         evt_s2->z1.n = unitSprite->z1.n;
         evt_s2->y1.n = unitSprite->y1.n + 0x40 + rand() % 0x20;
      }

      if (--EVT.timer == -1) {
         evt->functionIndex = EVTF_NULL;
         unitSprite = EVT.unitSprite;
         unitSprite->d.sprite.hidden = 0;
      }

      break;
   }
}

#undef EVTF
#define EVTF 206
void Evtf206_686_BloodSpurtParticle(EvtData *evt) {
   EvtData *sprite;
   s32 i;
   s32 rnd;
   s16 a, b;
   POLY_FT4 *poly;
   s16 red, green, blue;
   s16 prevQuadIdx;
   s16 shade;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;

      if (evt->functionIndex == EVTF_BLOOD_SPURT_PARTICLE ||
          evt->functionIndex == EVTF_BLOOD_SPURT_PARTICLE_OFFSET) {
         sprite->d.sprite.animData = sImpactAnimData_800feb44;
         sprite->d.sprite.clut = 3;
         sprite->d.sprite.animInitialized = 0;
      } else {
         sprite->d.sprite.animData = sRockAnimData_800feb68;
         sprite->d.sprite.clut = 2;
         sprite->d.sprite.animInitialized = 0;
         rnd = rand() % 3;
         for (i = 0; i < rnd; i++) {
            UpdateEvtAnimation(sprite);
         }
      }

      EVT.sprite = sprite;
      EVT.shade = rand() % 0x60;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position1.x = (rand() % 0x40 + 0x192) * rsin(a) / ONE;
      EVT.position1.z = (rand() % 0x40 + 0x192) * rcos(a) / ONE;
      EVT.position1.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position2.x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position2.z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position2.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position3.x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position3.z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position3.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position4.x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position4.z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position4.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position5.x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position5.z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position5.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position6.x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position6.z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position6.y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position7_x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position7_z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position7_y = rand() % 0x200 + 0x100;

      a = rand() % ANGLE_360_DEGREES;
      EVT.position8_x = (rand() % 0xc0 + 0x140) * rsin(a) / ONE;
      EVT.position8_z = (rand() % 0xc0 + 0x140) * rcos(a) / ONE;
      EVT.position8_y = rand() % 0x200 + 0x100;

      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);

      a = ANGLE_360_DEGREES - rcos(EVT.theta / 2);
      b = rsin(EVT.theta);
      shade = EVT.shade + 0x40;

      if (evt->functionIndex == EVTF_BLOOD_SPURT_PARTICLE ||
          evt->functionIndex == EVTF_BLOOD_SPURT_PARTICLE_OFFSET) {
         red = shade;
         green = 0;
         blue = 0;
      } else {
         red = 0x80;
         green = 0x80;
         blue = 0x80;
      }

      sprite->x1.n = evt->x1.n + EVT.position1.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position1.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position1.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position2.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position2.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position2.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position3.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position3.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position3.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position4.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position4.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position4.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position5.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position5.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position5.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position6.x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position6.z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position6.y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position7_x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position7_z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position7_y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      sprite->x1.n = evt->x1.n + EVT.position8_x * a / ONE;
      sprite->z1.n = evt->z1.n + EVT.position8_z * a / ONE;
      sprite->y1.n = evt->y1.n + EVT.position8_y * b / ONE;

      prevQuadIdx = gQuadIndex;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);
      }

      EVT.theta += (ANGLE_180_DEGREES / 32);
      if (EVT.theta >= ANGLE_180_DEGREES) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }

      break;
   }
}
