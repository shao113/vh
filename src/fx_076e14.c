#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

#undef EVTF
#define EVTF 330
void Evtf330_MagicRestoration_FX1(EvtData *evt) {
   static const SVectorXZ deltas[4] = {{-1, 0}, {0, 1}, {0, -1}, {1, 0}};
   static const u8 indices[4][4] = {[CAM_DIR_SOUTH] = {0, 1, 2, 3},
                                    [CAM_DIR_EAST] = {2, 0, 3, 1},
                                    [CAM_DIR_NORTH] = {3, 2, 1, 0},
                                    [CAM_DIR_WEST] = {1, 3, 0, 2}};
   static s16 increment = 0x200;

   EvtData *evt_s6; // targetSprite, ringBottom
   EvtData *evt_s7; // ringTop, ray
   EvtData *casterSprite;
   EvtData *cam;
   EvtData *dataStore;
   Cylinder *dsCylinder;
   BVectorXZ *p;
   s32 i;
   s32 ringSize;
   s32 direction;
   s16 savedY;
   s32 dx_0, dz_0;
   s32 dx_1, dz_1;
   s32 dx_2, dz_2;
   s32 dx_3, dz_3;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         SnapToUnit(evt);
         evt->y2.n = evt->y1.n;
         EVT.startingTheta1 = 0;
         EVT.startingTheta2 = DEG(90);
         EVT.ringSize = CV(1.0);
         evt->state2++;

      // fallthrough
      case 1:
         p = (BVectorXZ *)gTargetCoords;
         evt_s6 = GetUnitSpriteAtPosition(p->z, p->x);
         casterSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
         if (evt_s6 == casterSprite) {
            evt->state2 = 0;
            evt->state++;
         } else {
            cam = CreatePositionedEvt(evt, EVTF_CAMERA_TBD_026);
            cam->d.evtf026.target = evt_s6;
            evt->x1.n = evt_s6->x1.n;
            evt->y2.n = evt->y1.n = evt_s6->y1.n;
            evt->z1.n = evt_s6->z1.n;
            evt->state3 = 8;
            evt->state2++;
         }
         break;

      case 2:
         if (--evt->state3 <= 0) {
            evt->state2 = 0;
            evt->state3 = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 1:
      evt_s7 = Evt_GetUnused();
      evt_s7->functionIndex = EVTF_NOOP;
      EVT.ringTop = evt_s7;
      direction = (gCameraRotation.vy & 0xfff) / DEG(90);
      evt_s7->d.sprite.gfxIdx = GFX_RING_TOP;
      evt_s7->d.sprite.clut = CLUT_BLUES;

      ringSize = EVT.ringSize;
      dx_0 = deltas[indices[direction][0]].x * ringSize;
      dz_0 = deltas[indices[direction][0]].z * ringSize;
      dx_1 = deltas[indices[direction][1]].x * ringSize;
      dz_1 = deltas[indices[direction][1]].z * ringSize;
      dx_2 = deltas[indices[direction][2]].x * ringSize;
      dz_2 = deltas[indices[direction][2]].z * ringSize;
      dx_3 = deltas[indices[direction][3]].x * ringSize;
      dz_3 = deltas[indices[direction][3]].z * ringSize;

      evt_s7->d.sprite.coords[0].x = evt->x1.n + dx_0;
      evt_s7->d.sprite.coords[0].z = evt->z1.n + dz_0;
      evt_s7->d.sprite.coords[1].x = evt->x1.n + dx_1;
      evt_s7->d.sprite.coords[1].z = evt->z1.n + dz_1;
      evt_s7->d.sprite.coords[2].x = evt->x1.n + (dx_0 + dx_2) / 2;
      evt_s7->d.sprite.coords[2].z = evt->z1.n + (dz_0 + dz_2) / 2;
      evt_s7->d.sprite.coords[3].x = evt->x1.n + (dx_1 + dx_3) / 2;
      evt_s7->d.sprite.coords[3].z = evt->z1.n + (dz_1 + dz_3) / 2;

      evt_s6 = Evt_GetUnused();
      evt_s6->functionIndex = EVTF_NOOP;
      EVT.ringBottom = evt_s6;
      evt_s6->d.sprite.gfxIdx = GFX_RING_BTM;
      evt_s6->d.sprite.clut = CLUT_BLUES;
      evt_s6->d.sprite.coords[2].x = evt->x1.n + dx_2;
      evt_s6->d.sprite.coords[2].z = evt->z1.n + dz_2;
      evt_s6->d.sprite.coords[3].x = evt->x1.n + dx_3;
      evt_s6->d.sprite.coords[3].z = evt->z1.n + dz_3;
      evt_s6->d.sprite.coords[0].x = evt->x1.n + (dx_0 + dx_2) / 2;
      evt_s6->d.sprite.coords[0].z = evt->z1.n + (dz_0 + dz_2) / 2;
      evt_s6->d.sprite.coords[1].x = evt->x1.n + (dx_1 + dx_3) / 2;
      evt_s6->d.sprite.coords[1].z = evt->z1.n + (dz_1 + dz_3) / 2;
      evt->state2 = 0;
      evt->mem = 0;
      evt->state++;

   // fallthrough
   case 2:

      switch (evt->state2) {
      case 0:
         dataStore = Evt_GetUnused();
         dataStore->functionIndex = EVTF_NOOP;
         EVT.dataStore = dataStore;
         dsCylinder = &dataStore->d.dataStore.cylinder;
         dsCylinder->top.vx = dsCylinder->bottom.vx = evt->x1.n;
         dsCylinder->top.vz = dsCylinder->bottom.vz = evt->z1.n;
         dsCylinder->bottom.vy = evt->y1.n;
         dsCylinder->top.vy = evt->y1.n + CV(4.0);
         dsCylinder->sideCt = 16;
         dsCylinder->semiTrans = 4;
         dsCylinder->gfxIdx = GFX_TILED_FLAMES;
         evt->mem = 0;
         dsCylinder->bottomRadius = 0;
         dsCylinder->topRadius = 0;
         dsCylinder->useColor = 1;
         dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = 0;
         dsCylinder->theta = 0;
         dsCylinder->clut = CLUT_PURPLES;
         evt->state2++;

      // fallthrough
      case 1:
         dataStore = EVT.dataStore;
         dsCylinder = &dataStore->d.dataStore.cylinder;
         evt->mem += 0x10;
         dsCylinder->topRadius = dsCylinder->bottomRadius = evt->mem;
         dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = evt->mem >> 1;
         dsCylinder->theta += DEG(2.8125);
         RenderCylinder(dsCylinder);
         if (evt->mem >= 0x100) {
            evt->state++;
         }
         break;

      case 2:
         break;
      }

      break;

   case 3:
      dataStore = EVT.dataStore;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      evt->mem -= 2;
      if (evt->mem <= 0) {
         evt->mem = 0;
      }
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = evt->mem >> 1;
      dsCylinder->theta += DEG(2.8125);
      RenderCylinder(dsCylinder);

      evt->y1.n += CV(0.09375);
      evt_s7 = EVT.ringTop;
      evt_s6 = EVT.ringBottom;
      savedY = evt->y1.n;
      EVT.theta1 = EVT.startingTheta1;
      EVT.theta2 = EVT.startingTheta2;

      for (i = 0; i < 16; i++) {
         evt_s7->d.sprite.coords[0].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta1) >> 12);
         evt_s7->d.sprite.coords[1].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta2) >> 12);
         evt_s7->d.sprite.coords[2].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta2 + DEG(180)) >> 12);
         evt_s7->d.sprite.coords[3].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta1 + DEG(180)) >> 12);
         evt_s7->d.sprite.coords[2].y =
             (evt_s7->d.sprite.coords[2].y + evt_s7->d.sprite.coords[0].y) / 2;
         evt_s7->d.sprite.coords[3].y =
             (evt_s7->d.sprite.coords[3].y + evt_s7->d.sprite.coords[1].y) / 2;
         evt_s7->d.sprite.otOfs = -6;
         evt_s7->x1.n = evt_s7->d.sprite.coords[0].x;
         evt_s7->z1.n = evt_s7->d.sprite.coords[0].z;
         evt_s7->y1.n = evt_s7->d.sprite.coords[0].y;
         AddEvtPrim5(gGraphicsPtr->ot, evt_s7);

         evt_s6->d.sprite.coords[0].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta1) >> 12);
         evt_s6->d.sprite.coords[1].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta2) >> 12);
         evt_s6->d.sprite.coords[2].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta2 + DEG(180)) >> 12);
         evt_s6->d.sprite.coords[3].y = evt->y1.n + (CV(0.25) * rsin(EVT.theta1 + DEG(180)) >> 12);
         evt_s6->d.sprite.coords[0].y =
             (evt_s6->d.sprite.coords[2].y + evt_s6->d.sprite.coords[0].y) / 2;
         evt_s6->d.sprite.coords[1].y =
             (evt_s6->d.sprite.coords[3].y + evt_s6->d.sprite.coords[1].y) / 2;
         evt_s6->x1.n = evt_s6->d.sprite.coords[3].x;
         evt_s6->z1.n = evt_s6->d.sprite.coords[3].z;
         evt_s6->y1.n = evt_s6->d.sprite.coords[3].y;
         evt_s6->d.sprite.otOfs = 8;
         AddEvtPrim5(gGraphicsPtr->ot, evt_s6);

         EVT.theta1 += increment;
         EVT.theta2 += increment;

         evt->y1.n -= CV(0.5);
         if (evt->y1.n < evt->y2.n) {
            break;
         }
      }

      for (i = 0; i < 2; i++) {
         evt_s7 = Evt_GetUnused();
         evt_s7->functionIndex = EVTF_VERTICAL_RAY;
         evt_s7->x2.n = evt->x1.n;
         evt_s7->z2.n = evt->z1.n;
         evt_s7->y2.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
         evt_s7->d.evtf315.theta = rand() % DEG(360);
         evt_s7->d.evtf315.radius = CV(0.75);
         evt_s7->d.evtf315.top = evt->y2.n;
         evt_s7->d.evtf315.height = rand() % CV(1.0);
         evt_s7->d.evtf315.rotationSpeed = 0x40;
         evt_s7->d.evtf315.speed = 0x18 + rand() % 0x30;
         evt_s7->d.evtf315.clut = 3 + (rand() & 3);
         evt_s7->d.evtf315.gfxIdx = GFX_COLOR_15;
         evt_s7->state = 1;
      }

      evt->y1.n = savedY;
      EVT.startingTheta1 += DEG(33.75);
      EVT.startingTheta2 += DEG(33.75);
      if (evt->y1.n > evt->y2.n + CV(6.0)) {
         evt->state++;
      }
      break;

   case 4:
      evt_s7 = EVT.ringTop;
      evt_s7->functionIndex = EVTF_NULL;
      evt_s6 = EVT.ringBottom;
      evt_s6->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      dataStore = EVT.dataStore;
      dataStore->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

void HealingCircle_RenderBlueRing(EvtData *healingCircle, s32 outerRadius, s32 thickness, s32 t) {
   EvtData *sprite;
   s32 cx, cz;
   POLY_FT4 *poly;
   s32 i;
   u32 rgbCode;

   sprite = Evt_GetUnused();
   sprite->functionIndex = EVTF_NOOP;
   cx = healingCircle->x1.n;
   cz = healingCircle->z1.n;
   sprite->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES;
   sprite->d.sprite.semiTrans = 2;
   sprite->d.sprite.clut = CLUT_BLUES;

   sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = sprite->d.sprite.coords[2].y =
       sprite->d.sprite.coords[3].y = healingCircle->y1.n;

   sprite->d.sprite.coords[0].x = cx + ((outerRadius - thickness) * rcos(0) >> 12);
   sprite->d.sprite.coords[1].x = cx + (outerRadius * rcos(0) >> 12);
   sprite->d.sprite.coords[0].z = cz + ((outerRadius - thickness) * rsin(0) >> 12);
   sprite->d.sprite.coords[1].z = cz + (outerRadius * rsin(0) >> 12);

   rgbCode = POLY_RGB_CODE(GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS, t, t, t);

   for (i = 0; i < 32; i++) {
      sprite->d.sprite.coords[2].x = cx + ((outerRadius - thickness) * rcos((i + 1) * 0x80) >> 12);
      sprite->d.sprite.coords[3].x = cx + (outerRadius * rcos((i + 1) * 0x80) >> 12);
      sprite->d.sprite.coords[2].z = cz + ((outerRadius - thickness) * rsin((i + 1) * 0x80) >> 12);
      sprite->d.sprite.coords[3].z = cz + (outerRadius * rsin((i + 1) * 0x80) >> 12);
      sprite->x1.n = cx;
      sprite->z1.n = cz;
      sprite->y1.n = healingCircle->y1.n;
      AddEvtPrim3(gGraphicsPtr->ot, sprite);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      *(u32 *)&poly->r0 = rgbCode;
      sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[2].x;
      sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[3].x;
      sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[2].z;
      sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[3].z;
   }

   sprite->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 381
void Evtf381_HealingCircle_FX1(EvtData *evt) {
   EvtData *sparkle;
   BVectorXZ *p;
   s32 i;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      p = (BVectorXZ *)gTargetCoords;
      GetUnitSpriteAtPosition(p->z, p->x);
      evt->state2 = 0x10;
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:
      gLightColor.r -= 2;
      gLightColor.g -= 2;
      gLightColor.b -= 2;
      gCameraZoom.vz += 4;

      if (++evt->state3 >= 0x20) {
         evt->state++;
      }

      break;

   case 2:
      for (i = 0; i < 32; i++) {
         sparkle = Evt_GetUnused();
         sparkle->functionIndex = EVTF_SPARKLE;
         sparkle->x1.n = evt->x1.n;
         sparkle->z1.n = evt->z1.n;
         sparkle->y1.n = evt->y1.n;
         sparkle->x2.n = 0x20 * rcos(i * 0x80) >> 12;
         sparkle->z2.n = 0x20 * rsin(i * 0x80) >> 12;
         sparkle->state2 = 0x40;
      }
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 3:
      gCameraZoom.vz += 4;
      gCameraRotation.vx += 2;
      gCameraRotation.vy += 3;

      if (evt->state2 <= 0x200 - 4) {
         evt->state2 += (((0x200 - 4) - evt->state2) >> 4) + 4;
      } else {
         for (i = 0; i < 32; i++) {
            sparkle = Evt_GetUnused();
            sparkle->functionIndex = EVTF_SPARKLE;
            sparkle->x1.n = evt->x1.n;
            sparkle->z1.n = evt->z1.n;
            sparkle->y1.n = evt->y1.n;
            sparkle->x2.n = 0x20 * rcos(i * 0x80) >> 12;
            sparkle->z2.n = 0x20 * rsin(i * 0x80) >> 12;
            sparkle->state2 = 0x40;
         }
         evt->state2 = 0;
         return;
      }

      //? Last arg u8?
      HealingCircle_RenderBlueRing(evt, evt->state2, 0x10, ((0x200 - evt->state2) >> 1) & 0xff);

      if (++evt->state3 >= 0x80) {
         gSignal3 = 1;
         evt->state3 = 0;
         evt->state++;
      }

      break;

   case 4:
      gLightColor.r += 2;
      gLightColor.g += 2;
      gLightColor.b += 2;

      if (++evt->state3 >= 0x20) {
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 383
void Evtf383_Sparkle(EvtData *evt) {
   static s16 sparkleAnimData[20] = {
       5, GFX_SPARKLE_1, 2, GFX_SPARKLE_2, 2, GFX_SPARKLE_3, 2, GFX_SPARKLE_4, 2, GFX_SPARKLE_5,
       2, GFX_SPARKLE_4, 2, GFX_SPARKLE_3, 2, GFX_SPARKLE_2, 2, GFX_NULL,      1, GFX_NULL};

   switch (evt->state) {
   case 0:
      evt->d.sprite.boxIdx = 3;
      evt->d.sprite.animData = sparkleAnimData;
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

      if (--evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 384
void Evtf384_Fx_TBD(EvtData *evt) {
   // Unused FX1? Stretched skull restored row by row, with the current bottom row drawn stretched.
   EvtData *unitSprite;
   EvtData *sprite;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      unitSprite = SnapToUnit(evt);
      unitSprite->d.sprite.hidden = 1;
      evt->state2 = 0;
      evt->state3 = 0x20;
      evt->state++;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].x = gGfxSubTextures[GFX_SKULL].x;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = gGfxSubTextures[GFX_SKULL].y;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].w = gGfxSubTextures[GFX_SKULL].w;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].h = gGfxSubTextures[GFX_SKULL].h - 0x10;
      gGfxTPageIds[GFX_TILED_RED_SPARKLES_DYN_1] = gGfxTPageIds[GFX_SKULL];
      gQuad_800fe63c[0].vx = -0x10;
      gQuad_800fe63c[0].vy = -0x10;
      gQuad_800fe63c[1].vx = 0x10;
      gQuad_800fe63c[1].vy = -0x10;
      gQuad_800fe63c[2].vx = -0x10;
      gQuad_800fe63c[2].vy = 0;
      gQuad_800fe63c[3].vx = 0x10;
      gQuad_800fe63c[3].vy = 0;

   // fallthrough
   case 1:
      sprite = Evt_GetUnused();
      gQuad_800fe63c[0].vx = -0x10;
      gQuad_800fe63c[0].vy = -0x10;
      gQuad_800fe63c[1].vx = 0x10;
      gQuad_800fe63c[1].vy = -0x10;
      gQuad_800fe63c[2].vx = -0x10;
      gQuad_800fe63c[2].vy = 0x10 - evt->state3;
      gQuad_800fe63c[3].vx = 0x10;
      gQuad_800fe63c[3].vy = 0x10 - evt->state3;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].x = gGfxSubTextures[GFX_SKULL].x;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y = gGfxSubTextures[GFX_SKULL].y;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].w = gGfxSubTextures[GFX_SKULL].w;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].h = gGfxSubTextures[GFX_SKULL].h - evt->state3;
      sprite->d.sprite.boxIdx = 7;
      sprite->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES_DYN_1;
      sprite->d.sprite.clut = CLUT_2;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n + (evt->state3 << 2);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      gQuad_800fe63c[0].vx = -0x10;
      gQuad_800fe63c[0].vy = 0x10 - evt->state3;
      gQuad_800fe63c[1].vx = 0x10;
      gQuad_800fe63c[1].vy = 0x10 - evt->state3;
      gQuad_800fe63c[2].vx = -0x10;
      gQuad_800fe63c[2].vy = 0x10;
      gQuad_800fe63c[3].vx = 0x10;
      gQuad_800fe63c[3].vy = 0x10;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].x = gGfxSubTextures[GFX_SKULL].x;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].y =
          gGfxSubTextures[GFX_SKULL].y + (0x20 - evt->state3);
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].w = gGfxSubTextures[GFX_SKULL].w;
      gGfxSubTextures[GFX_TILED_RED_SPARKLES_DYN_1].h = 0;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      poly->y3 = 0xf0;
      poly->y2 = 0xf0;
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      break;

   case 2:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      unitSprite->d.sprite.hidden = 0;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

void Evtf711_712_Noop(void) {}

#undef EVTF
#define EVTF Unk80087b58
void Evtf_Unk_80087b58(EvtData *evt) {
   EvtData *dataStore;
   Cylinder *dsCylinder;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      dataStore = Evt_GetUnused();
      dataStore->functionIndex = EVTF_NOOP;
      EVT.dataStore = dataStore;

      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->sideCt = 32;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = GFX_COLOR_13;
      dsCylinder->topRadius = CV(1.0);
      dsCylinder->bottomRadius = CV(1.0);
      dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->top.vy = evt->y1.n + CV(4.0);
      dsCylinder->useColor = 1;
      dsCylinder->color.r = 0x40;
      dsCylinder->color.g = 0x40;
      dsCylinder->color.b = 0x40;

      evt->state++;
      evt->state2 = 0;

   // fallthrough
   case 1:
      dataStore = EVT.dataStore;
      evt->state3 += 0x10;
      dsCylinder = &dataStore->d.dataStore.cylinder;
      dsCylinder->topRadius = 1;
      dsCylinder->topRadius = dsCylinder->bottomRadius =
          CV(1.0) + (CV(1.0) * rsin(evt->state3) >> 12);
      RenderCylinder(dsCylinder);
      RenderCylinder(dsCylinder);
      if (++evt->state2 >= 0x200) {
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
