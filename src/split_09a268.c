#include "common.h"
#include "evt.h"
#include "units.h"
#include "state.h"
#include "graphics.h"

s32 D_801233A8;

#undef EVTF
#define EVTF 391
void Evtf391(EvtData *evt) {
   switch (evt->state) {
   case 0:
      D_801233A8 = 0;
      evt->state++;

   // fallthrough
   case 1:
      if (gSavedPad2StateNewPresses & PAD_L2) {
         evt->state = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 674
void Evtf674_DebugSounds(EvtData *evt) {
   static s32 unitId = 1;

   switch (evt->state) {
   case 0:
      evt->d.dataStore.shorts[0] = 1; //
      evt->state2 = 0;
      evt->state++;

   // fallthrough
   case 1:
      if (gSavedPad2State & PAD_R1) {
         unitId++;
      }
      if (gSavedPad2State & PAD_R2) {
         unitId--;
      }
      if (gSavedPad2StateNewPresses & PAD_L2) {
         evt->state2++;
         evt->state2 %= 2;
      }

      switch (evt->state2) {
      case 0:
         if (gSavedPad2StateNewPresses & PAD_UP) {
            gUnitSoundDelays_Attacking1[unitId][0]++;
         }
         if (gSavedPad2StateNewPresses & PAD_DOWN) {
            // Note: Using "-= 1" instead of the decrement causes a mismatch.
            gUnitSoundDelays_Attacking1[unitId][0]--;
         }
         if (gSavedPad2StateNewPresses & PAD_RIGHT) {
            gUnitSoundDelays_Attacking1[unitId][1]++;
         }
         if (gSavedPad2StateNewPresses & PAD_LEFT) {
            gUnitSoundDelays_Attacking1[unitId][1]--;
         }
         break;

      case 1:
         if (gSavedPad2StateNewPresses & PAD_UP) {
            gUnitSoundDelays_Attacking2[unitId][0]++;
         }
         if (gSavedPad2StateNewPresses & PAD_DOWN) {
            gUnitSoundDelays_Attacking2[unitId][0]--;
         }
         if (gSavedPad2StateNewPresses & PAD_RIGHT) {
            gUnitSoundDelays_Attacking2[unitId][1]++;
         }
         if (gSavedPad2StateNewPresses & PAD_LEFT) {
            gUnitSoundDelays_Attacking2[unitId][1]--;
         }
         break;
      }

      break;
   }
}

EvtData *SnapToUnit(EvtData *evt) {
   EvtData *unitSprite;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   evt->x1.n = unitSprite->x1.n;
   evt->z1.n = unitSprite->z1.n;
   evt->y1.n = unitSprite->y1.n;

   return unitSprite;
}

SVECTOR *func_800A9E78(SVECTOR *vec, s32 param_2, s32 theta1, s32 theta2) {
   s32 r;
   SVECTOR local_20;

   local_20.vy = param_2 * rsin(theta1) >> 12;
   r = param_2 * rcos(theta1) >> 12;
   local_20.vx = r * rcos(theta2) >> 12;
   local_20.vz = r * rsin(theta2) >> 12;

   *vec = local_20;
   return vec;
}

EvtData *CreatePositionedEvt(EvtData *target, s16 evtf) {
   EvtData *evt;

   evt = Evt_GetUnused();
   evt->functionIndex = evtf;
   evt->x1.n = target->x1.n;
   evt->z1.n = target->z1.n;
   evt->y1.n = target->y1.n;
   return evt;
}

void PrintDigit(s32 x, s32 y, s32 digit) {
   // Unused?

   POLY_FT4 *poly;

   x *= 8;
   y *= 8;

   poly = &gGraphicsPtr->quads[gQuadIndex++];
   setcode(poly, GPU_CODE_POLY_FT4);
   setXYWH(poly, x, y, 8, 8);
   setUVWH(poly, digit * 8 + 16, 0, 8, 8);

   setTPage(poly, 0, 1, 640, 256);
   poly->clut = gClutIds[17];
   setRGB0(poly, 128, 128, 128);
   AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
}

void Noop_800aa0ac(void) {}

void Evtf688_Noop(EvtData *evt) {}

EvtData *D_801233CC;

#undef EVTF
#define EVTF 277
void Evtf277_Zoom(EvtData *evt) {
   //? Maybe for debugging? Spawned by SetupMapExtras() (assigned to D_801233CC); can be manipulated
   // via func_800AA10C() / func_800AA154(), but those appear to be unused.

   s32 smoothness;

   switch (evt->state) {
   case 0:
      break;
   case 1:
      smoothness = EVT.smoothness & 0x7;
      gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> smoothness;
      break;
   }
}

void func_800AA10C(s16 param_1, s16 param_2, s16 zoom, s16 smoothness) {
   switch (D_801233CC->state) {
   case 0:
      D_801233CC->state++;

   // fallthrough
   default:
      D_801233CC->d.evtf277.todo_x24 = param_1;
      D_801233CC->d.evtf277.todo_x26 = param_2;
      D_801233CC->d.evtf277.dstZoom = zoom;
      D_801233CC->d.evtf277.smoothness = smoothness;
      break;
   }
}

void func_800AA154(void) { D_801233CC->state = 0; }

#undef EVTF
#define EVTF 279
void Evtf279_IceStorm_Camera(EvtData *evt) {
   static s16 zoomLevels[6] = {256, 384, 400, 480, 512, 768};
   static s16 angles[6] = {DEG(11.25), DEG(22.5), DEG(33.75), DEG(45), DEG(56.25), DEG(67.5)};

   EvtData *targetSprite;
   s32 rotY;
   s16 diff;

   targetSprite = EVT.targetSprite;

   switch (evt->state) {
   case 0:
      gCameraRotation.vy &= 0xfff;
      rotY = GetBestViewOfTarget(targetSprite->z1.s.hi, targetSprite->x1.s.hi, 1);
      diff = rotY - gCameraRotation.vy;
      if (diff > DEG(0)) {
         if (diff > DEG(180)) {
            rotY -= DEG(360);
         }
      } else if (diff < DEG(-180)) {
         rotY += DEG(360);
      }
      EVT.dstCamRotY = rotY;
      EVT.dstZoom = zoomLevels[rand() % 5 + 1];
      EVT.dstCamRotX = angles[rand() % 5 + 1];
      EVT.delayType = rand() % 3 + 2;

      switch (EVT.delayType) {
      case 2:
         EVT.timer = 35;
         break;
      case 3:
         EVT.timer = 50;
         break;
      case 4:
         EVT.timer = 65;
         break;
      }

      evt->state++;

   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         PanCamera(targetSprite->x1.n, targetSprite->y1.n + CV(0.5), targetSprite->z1.n, 3);
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vx += (EVT.dstCamRotX - gCameraRotation.vx) >> 3;
         gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> 3;
      } else {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void DimScreen(void) {
   gState.screenEffect->state = 6;
   gState.screenEffect->d.evtf369.color.r = gState.screenEffect->d.evtf369.color.g =
       gState.screenEffect->d.evtf369.color.b = 64;
   SetScreenEffectOrdering(-10);
}

void func_800AA42C(void) {
   // for un-dim?
   gState.screenEffect->state = 7;
   gState.screenEffect->d.evtf369.color.r = gState.screenEffect->d.evtf369.color.g =
       gState.screenEffect->d.evtf369.color.b = 0;
   SetScreenEffectOrdering(-10);
   gState.screenEffect->state2 = 2;
   SetScreenEffectOrdering(0);
}

void RenderMaskEffect(EvtData *unitSprite, MaskEffectPreset *preset) {
   EvtData *clonedSprite;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;
   s32 x, y;
   s32 cell;

   clonedSprite = Evt_GetUnused();
   CopyEvtData(unitSprite, clonedSprite);
   clonedSprite->functionIndex = EVTF_NOOP;
   clonedSprite->d.sprite.hidden = 0;
   GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
   cell = gGfxTPageCells[preset->srcGfxIdx];
   x = (cell & 0xf) * 256 + gGfxSubTextures[preset->srcGfxIdx][0];
   y = (cell >> 4) * 256 + gGfxSubTextures[preset->srcGfxIdx][1];
   ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, x, y, preset->width, preset->height,
                   preset->dstGfxIdx, 0);
   clonedSprite->d.sprite.gfxIdx = GFX_TBD_25;
   clonedSprite->d.sprite.clut = preset->clut;
   clonedSprite->d.sprite.semiTrans = preset->semiTrans;

   if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
      clonedSprite->d.sprite.boxIdx = 1;
   } else {
      clonedSprite->d.sprite.boxIdx = 0;
   }

   AddEvtPrim6(gGraphicsPtr->ot, clonedSprite, 0);
   poly = &gGraphicsPtr->quads[gQuadIndex - 1];
   setRGB0(poly, preset->color.r, preset->color.g, preset->color.b);
   clonedSprite->functionIndex = EVTF_NULL;
}

static s16 sSparkleAnimData_80101838[14] = {5, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                            3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                            1, GFX_NULL};

#undef EVTF
#define EVTF 393
void Evtf393_Map44_Scn00_ExplosionRays(EvtData *evt) {
   SVECTOR local_58;
   SVECTOR local_50;
   SVECTOR local_48;
   SVECTOR local_40;
   EvtData *target;
   EvtData *sprite;
   s32 i;
   s32 ct;
   s16 theta1;
   s16 theta2;
   s16 tmp_0x80;
   s16 tmp_0x800;

   switch (evt->state) {
   case 0:
      target = EVT.target;
      evt->x1.n = target->x1.n;
      evt->z1.n = target->z1.n;
      evt->y1.n = target->y1.n + CV(0.5);
      evt->state++;
      break;

   case 1:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_COLOR_15;
      sprite->d.sprite.semiTrans = 4;
      sprite->d.sprite.clut = CLUT_BLUES;
      evt->x2.n = 16;
      evt->state++;

   // fallthrough
   case 2:
      gCameraZoom.vz += 3;
      sprite = EVT.sprite;
      evt->x2.n = evt->state3 + 16;
      evt->mem += evt->x2.n;
      theta2 = evt->mem;
      theta1 = 300 - (theta2 >> 5);
      tmp_0x80 = 0x80;
      tmp_0x800 = 0x800;
      ct = evt->state3 / 16 + 8;

      for (i = 0; i < ct; i++) {
         func_800A9E78(&local_58, tmp_0x80, theta1 + DEG(2.8125), theta2);
         func_800A9E78(&local_50, tmp_0x80, theta1, theta2);
         func_800A9E78(&local_48, tmp_0x800, theta1 + DEG(2.8125), theta2);
         func_800A9E78(&local_40, tmp_0x800, theta1, theta2);

         sprite->d.sprite.coords[0].x = evt->x1.n + local_58.vx;
         sprite->d.sprite.coords[0].z = evt->z1.n + local_58.vz;
         sprite->d.sprite.coords[0].y = evt->y1.n + local_58.vy;
         sprite->d.sprite.coords[1].x = evt->x1.n + local_50.vx;
         sprite->d.sprite.coords[1].z = evt->z1.n + local_50.vz;
         sprite->d.sprite.coords[1].y = evt->y1.n + local_58.vy;
         sprite->d.sprite.coords[2].x = evt->x1.n + local_48.vx;
         sprite->d.sprite.coords[2].z = evt->z1.n + local_48.vz;
         sprite->d.sprite.coords[2].y = evt->y1.n + local_48.vy;
         sprite->d.sprite.coords[3].x = evt->x1.n + local_40.vx;
         sprite->d.sprite.coords[3].z = evt->z1.n + local_40.vz;
         sprite->d.sprite.coords[3].y = evt->y1.n + local_40.vy;

         AddEvtPrim4(gGraphicsPtr->ot, sprite);
         theta2 += DEG(360) / ct;
      }
      if (evt->state3 == 128) {
         evt->state++;
      }
      break;

   case 3:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 276
void Evtf276_Fx_TBD(EvtData *evt) {
   static s16 animData[20] = {
       0, GFX_FLAME_WALL_1, 2, GFX_FLAME_WALL_2, 2, GFX_FLAME_WALL_3, 2, GFX_FLAME_WALL_4,
       2, GFX_FLAME_WALL_5, 2, GFX_FLAME_WALL_6, 2, GFX_FLAME_WALL_7, 2, GFX_FLAME_WALL_8,
       2, GFX_NULL,         1, GFX_NULL,
   };

   s16 cluts_unused[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};
   EvtData *evt_a0;
   Cylinder *dsCylinder;
   s32 i;
   s16 a, b;
   s32 r;

   switch (evt->state) {
   case 0:
      evt_a0 = EVT.variant_0x24.targetParam;
      evt->x1.n = evt_a0->x1.n;
      evt->z1.n = evt_a0->z1.n;
      evt->y1.n = evt_a0->y1.n;
      EVT.animData = animData;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_a0;
      dsCylinder = &evt_a0->d.dataStore.cylinder;
      dsCylinder->top.vx = dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->top.vz = dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vy = evt->y1.n + CV(4.0);
      dsCylinder->sideCt = 16;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = GFX_TILED_FLAMES;
      evt->mem = CV(0.25);
      dsCylinder->bottomRadius = CV(0.25);
      dsCylinder->topRadius = CV(0.25);
      dsCylinder->useColor = 1;
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = 128;
      dsCylinder->theta = 0;
      dsCylinder->clut = CLUT_REDS;
      EVT.variant_0x24.unk = 5;
      evt->state++;

   // fallthrough
   case 1:
      evt->state2++;
      evt_a0 = EVT.dataStore;
      dsCylinder = &evt_a0->d.dataStore.cylinder;
      dsCylinder->theta += evt->state2 * 2;
      UpdateEvtAnimation(evt);

      for (i = 0; i < 8; i++) {
         a = (i + 1) * DEG(11.25);
         b = i * DEG(11.25);
         r = evt->mem;
         dsCylinder->semiTrans = 4;
         dsCylinder->bottom.vy = evt->y1.n + (r * rsin(b) >> 12);
         dsCylinder->top.vy = evt->y1.n + (r * rsin(a) >> 12);
         dsCylinder->bottomRadius = r * rcos(b) >> 12;
         dsCylinder->topRadius = r * rcos(a) >> 12;
         RenderCylinder(dsCylinder);
      }
      evt->mem += CV(0.015625);
      break;

   case 2:
      evt_a0 = EVT.dataStore;
      evt_a0->functionIndex = EVTF_NULL;
      evt_a0 = EVT.unused_0x58;
      evt_a0->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 275
void Evtf275_Fx_TBD(EvtData *evt) {
   static s16 animData[] = {4, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                            2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                            2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                            2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                            0, GFX_NULL};

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_EXPLOSION_1;
      EVT.animData = animData;
      evt->state++;

   // fallthrough
   case 1:
      UpdateEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      evt->x1.n += (evt->x2.n - evt->x1.n) >> 2;
      evt->z1.n += (evt->z2.n - evt->z1.n) >> 2;
      evt->y1.n += (evt->y2.n - evt->y1.n) >> 2;
      if (EVT.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 273
void Evtf273_OutwardRay(EvtData *evt) {
   s32 rnd;
   s32 r;

   switch (evt->state) {
   case 0:
      EVT.theta1 = rand() % DEG(33.75);
      EVT.theta2 = rand() % DEG(360);
      EVT.unused_0x3A = rand() % 64 + 32;
      EVT.todo_x38 = CV(0.5);
      evt->x3.n = 32;
      evt->z3.n = 8;

      rnd = (rand() >> 3) % 4096;
      if (rnd >= 512) {
         evt->mem = 3;
      } else {
         evt->mem = 2;
      }

      EVT.todo_x54 = -32 - rand() % 64;

      switch (evt->state3) {
      case 0:
         EVT.gfxIdx = GFX_COLOR_15;
         EVT.semiTrans = 2;
         EVT.clut = CLUT_BLUES;
         break;
      case 1:
         EVT.gfxIdx = GFX_COLOR_12;
         EVT.semiTrans = 0;
         EVT.clut = CLUT_PURPLES;
         break;
      }

      evt->state2 = 0;

      r = (EVT.todo_x38 * rcos(EVT.theta1)) >> 12;
      EVT.coords[1].x = evt->x2.n + (r * rcos(EVT.theta2) >> 12);
      EVT.coords[1].z = evt->z2.n + (r * rsin(EVT.theta2) >> 12);
      EVT.coords[3].x = evt->x2.n + (r * rcos(EVT.theta2 + DEG(1.40625)) >> 12);
      EVT.coords[3].z = evt->z2.n + (r * rsin(EVT.theta2 + DEG(1.40625)) >> 12);
      EVT.coords[1].y = EVT.coords[3].y = evt->y2.n + (EVT.todo_x38 * rsin(EVT.theta1) >> 12);

      evt->state++;

   // fallthrough
   case 1:
      EVT.todo_x38 += evt->x3.n;
      evt->x3.n += evt->z3.n;

      r = (EVT.todo_x38 * rcos(EVT.theta1)) >> 12;
      EVT.coords[0].x = evt->x2.n + (r * rcos(EVT.theta2) >> 12);
      EVT.coords[0].z = evt->z2.n + (r * rsin(EVT.theta2) >> 12);
      EVT.coords[2].x = evt->x2.n + (r * rcos(EVT.theta2 + DEG(1.40625)) >> 12);
      EVT.coords[2].z = evt->z2.n + (r * rsin(EVT.theta2 + DEG(1.40625)) >> 12);
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

      if (EVT.todo_x38 >= CV(4.5)) {
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 271
void Evtf271_Map36_Scn74_LeenaCastingShield(EvtData *evt) {
   EvtData *evt_s5;
   EvtData *forcefield;
   Cylinder *dsCylinder;
   s32 i;
   s32 theta1;
   s32 theta2;
   s32 iVar6;

   switch (evt->state) {
   case 0:
      evt_s5 = EVT.entitySprite;
      evt->x1.n = evt_s5->x1.n;
      evt->z1.n = evt_s5->z1.n;
      evt->y1.n = evt_s5->y1.n;

      evt_s5 = Evt_GetUnused();
      evt_s5->functionIndex = EVTF_NOOP;
      dsCylinder = &evt_s5->d.dataStore.cylinder;
      dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->top.vy = evt->y1.n;
      dsCylinder->sideCt = 32;
      dsCylinder->semiTrans = 2;
      dsCylinder->gfxIdx = GFX_COLOR_11;
      dsCylinder->clut = CLUT_BLUES;
      dsCylinder->useColor = 1;
      dsCylinder->color.r = 255;
      dsCylinder->color.g = 255;
      dsCylinder->color.b = 255;
      EVT.dataStore = evt_s5;
      evt->mem = 0;
      evt->state++;

   // fallthrough
   case 1:
      iVar6 = (CV(1.0) - evt->state3) * 4;
      theta1 = evt->mem;
      iVar6 += CV(0.8125);
      theta2 = theta1 + DEG(2.8125);
      if (theta2 >= DEG(90)) {
         theta2 = DEG(90);
      }

      evt_s5 = EVT.dataStore;
      dsCylinder = &evt_s5->d.dataStore.cylinder;
      dsCylinder->theta = evt->state3 << 7; //* 128
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = (1024 - theta1) >> 2;

      for (i = 0; i < 2; i++) {
         dsCylinder->bottomRadius = iVar6 * rcos(theta1) >> 12;
         dsCylinder->topRadius = iVar6 * rcos(theta2) >> 12;
         dsCylinder->bottom.vy = evt->y1.n + (iVar6 * rsin(theta1) >> 12);
         dsCylinder->top.vy = evt->y1.n + (iVar6 * rsin(theta2) >> 12);
         RenderCylinder(dsCylinder);
         theta1 += DEG(2.8125);
         theta2 += DEG(2.8125);
         if (theta1 >= DEG(90)) {
            theta1 = DEG(90);
         }
         if (theta2 >= DEG(90)) {
            theta1 = DEG(90);
         }
      }

      theta1 = 0;
      theta2 = DEG(11.25);
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = (1024 - evt->mem) >> 2;

      for (i = 0; i < 8; i++) {
         dsCylinder->bottomRadius = iVar6 * rcos(theta1) >> 12;
         dsCylinder->topRadius = iVar6 * rcos(theta2) >> 12;
         dsCylinder->bottom.vy = evt->y1.n + (iVar6 * rsin(theta1) >> 12);
         dsCylinder->top.vy = evt->y1.n + (iVar6 * rsin(theta2) >> 12);
         RenderCylinder(dsCylinder);
         theta1 += DEG(11.25);
         theta2 += DEG(11.25);
      }

      evt->mem += 64;
      if (evt->mem >= 1024) {
         evt->mem = 0;
      }

      evt->state3 += 8;
      if (evt->state3 == 192) {
         forcefield = Evt_GetUnused();
         forcefield->functionIndex = EVTF_LEENA_FORCEFIELD;
         forcefield->d.evtf675.targetSprite = EVT.entitySprite;
      }
      if (evt->state3 >= 208) {
         evt_s5->functionIndex = EVTF_NULL;
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

static s16 sCluts_801230a4[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};

#undef EVTF
#define EVTF 270
void Evtf270_Fx_TBD(EvtData *evt) {
   static s16 splashAnimData[20] = {
       0, GFX_SPLASH_1, 1, GFX_SPLASH_2, 1, GFX_SPLASH_3, 1, GFX_SPLASH_4, 1, GFX_SPLASH_5,
       1, GFX_SPLASH_6, 1, GFX_SPLASH_7, 1, GFX_SPLASH_8, 1, GFX_NULL,     1, GFX_NULL};

   s32 i;
   EvtData *link;   // s2
   EvtData *sprite; // s1
   EvtData *evt_s0;
   Cylinder *dsCylinder;

   switch (evt->state) {
   case 0:
      SnapToUnit(evt);
      evt->y1.n += CV(0.5);

      link = evt;
      for (i = 0; i < 8; i++) {
         CreatePositionedEvt(evt, EVTF_FX_TBD_269);
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_FX_TBD_269;
         evt_s0->d.evtf269.link = link;
         evt_s0->d.evtf269.parent = evt;
         link = evt_s0;
      }

      gCameraZoom.vz = 256;
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.animData = splashAnimData;
      gCameraRotation.vy += 16;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;

      if (--evt->mem <= 0) {
         evt_s0 = CreatePositionedEvt(evt, EVTF_FX_TBD_099);
         evt_s0->x1.n += rand() % CV(0.5);
         evt_s0->z1.n += rand() % CV(0.5);
         evt_s0->y1.n += rand() % CV(0.5);
         evt_s0->d.evtf099.theta1 = 0;
         evt_s0->d.evtf099.theta2 = 0;
         evt_s0->d.evtf099.todo_x28 = rand() % 64 + 128;
         evt_s0->d.evtf099.clut = sCluts_801230a4[rand() % 4];
         evt->mem += rand() % 4;
      }

      if (--evt->mem <= 0) {
         evt_s0 = CreatePositionedEvt(evt, EVTF_FX_TBD_088);
         evt_s0->x1.n += rand() % CV(0.5);
         evt_s0->z1.n += rand() % CV(0.5);
         evt_s0->y1.n += rand() % CV(0.5);
         evt_s0->y2.n = rand() % 8 + 8;
         evt_s0->mem = 64;
         evt->mem += rand() % 2;
      }

      UpdateEvtAnimation(sprite);

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      dsCylinder = &evt_s0->d.dataStore.cylinder;
      dsCylinder->bottom.vx = evt->x1.n;
      dsCylinder->bottom.vz = evt->z1.n;
      dsCylinder->bottom.vy = evt->y1.n;
      dsCylinder->top.vx = evt->x1.n;
      dsCylinder->top.vz = evt->z1.n;
      dsCylinder->top.vy = evt->y1.n + CV(1.0);
      dsCylinder->sideCt = 16;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = sprite->d.sprite.gfxIdx;
      dsCylinder->bottomRadius = CV(1.0);
      dsCylinder->topRadius = CV(1.0);
      dsCylinder->theta = sprite->state2;
      sprite->state2 += 32;
      dsCylinder->clut = CLUT_PURPLES;
      RenderCylinder(dsCylinder);
      evt_s0->functionIndex = EVTF_NULL;

      EVT.todo_x24 = 0;
      EVT.todo_x26 = 0;
      break;
   }
}

#undef EVTF
#define EVTF 269
void Evtf269_Fx_TBD(EvtData *evt) {
   static s16 lightningAnimData[20] = {0, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3,
                                       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
                                       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_NULL,
                                       1, GFX_NULL};

   EvtData *link;
   EvtData *sprite;
   s16 radius;
   s16 theta;

   link = EVT.link;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_COLOR_15;
      sprite->d.sprite.clut = CLUT_REDS;
      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.animData = lightningAnimData;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      evt->mem++;
      radius = abs(192 * rsin(evt->mem * 16) >> 12);
      sprite = EVT.sprite;
      EVT.theta2 = EVT.theta1 = rand() % DEG(180) - DEG(90);
      UpdateEvtAnimation(sprite);
      theta = EVT.theta2;
      evt->x1.n = link->x1.n;
      evt->z1.n = link->z1.n + (radius * rcos(theta) >> 12);
      evt->y1.n = link->y1.n + (radius * rsin(theta) >> 12);
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[1].x = link->x1.n;
      sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[3].x = evt->x1.n;
      sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z = link->z1.n;
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[3].z = evt->z1.n;
      sprite->d.sprite.coords[0].y = link->y1.n - CV(0.5);
      sprite->d.sprite.coords[1].y = link->y1.n;
      sprite->d.sprite.coords[2].y = evt->y1.n - CV(0.5);
      sprite->d.sprite.coords[3].y = evt->y1.n;
      AddEvtPrim3(gGraphicsPtr->ot, sprite);
      break;
   }
}

#undef EVTF
#define EVTF 099
void Evtf099_Fx_TBD(EvtData *evt) {
   EvtData *sprite;
   SVECTOR svec;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;
      sprite->d.sprite.gfxIdx = GFX_POISONED;
      sprite->d.sprite.clut = EVT.clut;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;
      evt->mem = EVT.todo_x30;
      evt->state++;

   // fallthrough
   case 1:
      func_800A9E78(&svec, EVT.todo_x28, EVT.theta1, EVT.theta2);
      evt->x1.n += svec.vx;
      evt->z1.n += svec.vz;
      evt->y1.n += svec.vy;
      sprite = EVT.sprite;
      sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[3].x = evt->x1.n;
      sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[2].x = sprite->x1.n;
      sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[3].z = evt->z1.n;
      sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[2].z = sprite->z1.n;
      sprite->d.sprite.coords[1].y = evt->y1.n;
      sprite->d.sprite.coords[3].y = evt->y1.n + CV(0.0625);
      sprite->d.sprite.coords[0].y = sprite->y1.n;
      sprite->d.sprite.coords[2].y = sprite->y1.n + CV(0.0625);
      AddEvtPrim3(gGraphicsPtr->ot, sprite);

      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      if (--evt->mem <= 0) {
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
#define EVTF 088
void Evtf088_Fx_TBD(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_COLOR_12;
      EVT.clut = CLUT_BLUES;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += evt->x2.n;
      evt->z1.n += evt->z2.n;
      evt->y1.n += evt->y2.n;

      EVT.coords[0].x = EVT.coords[1].x = evt->x1.n;
      EVT.coords[2].x = EVT.coords[3].x = evt->x1.n;
      EVT.coords[0].z = EVT.coords[2].z = evt->z1.n;
      EVT.coords[1].z = EVT.coords[3].z = evt->z1.n + CV(0.03125);
      EVT.coords[0].y = EVT.coords[1].y = evt->y1.n;
      EVT.coords[2].y = EVT.coords[3].y = evt->y1.n + CV(0.03125);

      AddEvtPrim3(gGraphicsPtr->ot, evt);

      if (--evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void func_800ABFB8(EvtData *sprite) {
   s32 dx, dz, dy;
   s32 i;
   s16 rnd;
   EvtData *sprite_s0;
   s16 tmp;

   dx = (sprite->x2.n - sprite->x1.n) / 8;
   dy = (sprite->y2.n - sprite->y1.n) / 8;
   dz = (sprite->z2.n - sprite->z1.n) / 8;

   sprite_s0 = Evt_GetUnused();
   sprite_s0->functionIndex = EVTF_NOOP;
   sprite_s0->d.sprite.coords[0].y = sprite_s0->d.sprite.coords[1].y = sprite->y1.n;
   sprite_s0->d.sprite.coords[0].z = sprite->z1.n + CV(0.25);
   sprite_s0->d.sprite.coords[1].z = sprite->z1.n - CV(0.25);
   sprite_s0->d.sprite.coords[0].x = sprite_s0->d.sprite.coords[1].x = sprite->x1.n;
   sprite_s0->d.sprite.gfxIdx = sprite->d.sprite.gfxIdx;
   sprite_s0->d.sprite.clut = sprite->d.sprite.clut;
   sprite_s0->d.sprite.semiTrans = sprite->d.sprite.semiTrans;
   if (sprite_s0->d.sprite.gfxIdx == GFX_NULL) {
      sprite_s0->d.sprite.gfxIdx = GFX_LIGHTNING_5;
   }
   if (sprite_s0->d.sprite.clut == CLUT_NULL) {
      sprite_s0->d.sprite.clut = CLUT_BLUES;
   }
   for (i = 1; i < 8; i++) {
      sprite_s0->d.sprite.coords[2].y = sprite_s0->d.sprite.coords[3].y = sprite->y1.n + dy * i;
      rnd = rand() % CV(1.0);
      tmp = 63;
      sprite_s0->d.sprite.coords[2].z = sprite->z1.n + dz * i + (rnd - tmp);
      tmp = 191;
      sprite_s0->d.sprite.coords[3].z = sprite->z1.n + dz * i + (rnd - tmp);
      sprite_s0->d.sprite.coords[2].x = sprite_s0->d.sprite.coords[3].x = sprite->x1.n + dx * i;
      AddEvtPrim4(gGraphicsPtr->ot, sprite_s0);
      sprite_s0->d.sprite.coords[0].y = sprite_s0->d.sprite.coords[1].y =
          sprite_s0->d.sprite.coords[2].y;
      sprite_s0->d.sprite.coords[0].z = sprite_s0->d.sprite.coords[2].z;
      sprite_s0->d.sprite.coords[1].z = sprite_s0->d.sprite.coords[3].z;
      sprite_s0->d.sprite.coords[0].x = sprite_s0->d.sprite.coords[2].x;
      sprite_s0->d.sprite.coords[1].x = sprite_s0->d.sprite.coords[3].x;
   }
   sprite_s0->d.sprite.coords[2].y = sprite_s0->d.sprite.coords[3].y = sprite->y2.n;
   sprite_s0->d.sprite.coords[2].z = sprite->z2.n + CV(0.25);
   sprite_s0->d.sprite.coords[3].z = sprite->z2.n - CV(0.25);
   sprite_s0->d.sprite.coords[2].x = sprite_s0->d.sprite.coords[3].x = sprite->x2.n;
   AddEvtPrim4(gGraphicsPtr->ot, sprite_s0);
   sprite_s0->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 340
void Evtf340_Map48_Scn20_TBD(EvtData *evt) {
   EvtData *evt_s0;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         evt->x1.n = CV(5.5);
         evt->z1.n = CV(2.5);
         evt->y1.n = CV(0.5);
         evt->state2++;

      // fallthrough
      case 1:
         evt->mem += 2;
         if (evt->mem >= 96) {
            evt->state2 = 0;
            evt->mem = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 1:
      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_FX_TBD_099;
      evt_s0->x1.n = evt->x1.n + CV(2.0);
      evt_s0->y1.n = evt->y1.n;
      evt_s0->z1.n = evt->z1.n + (rand() % 1024 - CV(2.0));
      evt_s0->d.evtf099.theta1 = DEG(180);
      evt_s0->d.evtf099.theta2 = DEG(0);
      evt_s0->d.evtf099.todo_x28 = rand() % 64 + 256;
      evt_s0->d.evtf099.clut = sCluts_801230a4[rand() % 4];
      evt_s0->d.evtf099.todo_x30 = 16;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      evt_s0->x1.n = evt->x1.n;
      evt_s0->z1.n = evt->z1.n;
      evt_s0->y1.n = evt->y1.n + CV(0.5);
      evt_s0->x2.n = evt->x1.n - evt->state3;
      evt_s0->z2.n = evt->z1.n;
      evt_s0->y2.n = evt->y1.n + CV(0.5);
      evt_s0->d.sprite.clut = CLUT_BLUES;
      func_800ABFB8(evt_s0);
      evt_s0->z2.n = evt->z1.n + (evt->state3 >> 2);
      evt_s0->d.sprite.clut = CLUT_GREENS;
      func_800ABFB8(evt_s0);
      evt_s0->z2.n = evt->z1.n - (evt->state3 >> 2);
      evt_s0->d.sprite.clut = CLUT_PURPLES;
      func_800ABFB8(evt_s0);
      evt_s0->functionIndex = EVTF_NULL;
      evt->state3 += (1024 - evt->state3) >> 4;
      break;

   case 2:
      gLightColor.r = 128;
      gLightColor.g = 128;
      gLightColor.b = 128;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 305
void Evtf305_328_MagicStoneFx(EvtData *evt) {
   EvtData *evt_a0;
   EvtData *sprite;
   s32 i;
   s32 iVar7;
   u32 tmp;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      evt_a0 = EVT.variant_0x24.entitySpriteParam;
      evt->x1.n = evt_a0->x1.n;
      evt->z1.n = evt_a0->z1.n;
      evt->y1.n = evt_a0->y1.n;
      EVT.variant_0x24.timers.ray = 5;

      sprite = CreatePositionedEvt(evt, EVTF_NOOP);
      sprite->y1.n += CV(0.5625);
      sprite->d.sprite.gfxIdx = GFX_LARGE_RED_CIRCLE;
      sprite->d.sprite.boxIdx = 7;
      sprite->d.sprite.clut = CLUT_PURPLES;
      sprite->d.sprite.semiTrans = 4;
      EVT.sprite = sprite;

      evt->state++;

   // fallthrough
   case 1:
      evt->state2++;
      UpdateEvtAnimation(evt);
      evt->mem += 4;
      if (--EVT.variant_0x24.timers.ray <= 0) {
         for (i = 0; i < 3; i++) {
            tmp = rand() % 2; //
            evt_a0 = CreatePositionedEvt(evt, EVTF_OUTWARD_RAY);
            evt_a0->x2.n = evt->x1.n;
            evt_a0->z2.n = evt->z1.n;
            evt_a0->y2.n = evt->y1.n + CV(0.5);
            if (evt->functionIndex == EVTF_MAP43_SCN93_MAGIC_STONE_FX) {
               evt_a0->state3 = 1;
            }
         }
         EVT.variant_0x24.timers.ray = rand() % 3;
      }
      if (--EVT.variant_0x24.timers.explosion <= 0) {
         if (evt->functionIndex == EVTF_MAP43_SCN93_MAGIC_STONE_FX) {
            CreatePositionedEvt(evt, EVTF_MAGIC_STONE_EXPLOSION);
         }
         EVT.variant_0x24.timers.explosion = rand() % 16 + 8;
      }
      sprite = EVT.sprite;
      i = rsin(evt->state2 * 32); //
      iVar7 = abs(24 * i >> 12);
      sprite->d.sprite.gfxIdx = GFX_GLOBE_1 + iVar7 / 3;
      if (sprite->d.sprite.gfxIdx >= GFX_GLOBE_8) {
         sprite->d.sprite.gfxIdx = GFX_GLOBE_8;
      }
      if (evt->functionIndex == EVTF_MAP43_SCN93_MAGIC_STONE_FX) {
         tmp = 16;
      } else {
         tmp = 0;
      }
      for (i = 0; i < iVar7; i += 2) {
         gQuad_800fe63c[0].vx = -i - tmp;
         gQuad_800fe63c[0].vy = -i - tmp;
         gQuad_800fe63c[0].vz = 0;
         gQuad_800fe63c[1].vx = i + tmp;
         gQuad_800fe63c[1].vy = -i - tmp;
         gQuad_800fe63c[1].vz = 0;
         gQuad_800fe63c[2].vx = -i - tmp;
         gQuad_800fe63c[2].vy = i + tmp;
         gQuad_800fe63c[2].vz = 0;
         gQuad_800fe63c[3].vx = i + tmp;
         gQuad_800fe63c[3].vy = i + tmp;
         gQuad_800fe63c[3].vz = 0;
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         poly->r0 = (iVar7 - i) * 4;
         poly->g0 = (iVar7 - i) * 4;
         poly->b0 = (iVar7 - i) * 4;
      }
      break;

   case 2:
      evt_a0 = EVT.unused_0x5C; //?
      evt_a0->functionIndex = EVTF_NULL;
      evt_a0 = EVT.sprite;
      evt_a0->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

static s16 sCluts_80101900[] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};

#undef EVTF
#define EVTF 690
void Evtf690_MagicStoneExplosion(EvtData *evt) {
   static s16 animData[35] = {GFX_EXPLOSION_1,  2, 4, GFX_EXPLOSION_2,  2, 4, GFX_EXPLOSION_3, 2, 4,
                              GFX_EXPLOSION_4,  2, 3, GFX_EXPLOSION_5,  2, 3, GFX_EXPLOSION_6, 2, 3,
                              GFX_EXPLOSION_7,  2, 3, GFX_EXPLOSION_8,  2, 3, GFX_EXPLOSION_9, 2, 3,
                              GFX_EXPLOSION_10, 2, 3, GFX_EXPLOSION_11, 2, 3, GFX_NULL,        0};

   s32 r;
   s32 theta;
   s32 dx, dy, dz;

   switch (evt->state) {
   case 0:
      evt->d.sprite.animData = animData;
      evt->d.sprite.clut = CLUT_REDS;
      r = rand() % CV(0.5) + CV(1.0);
      theta = rand() % DEG(360);
      dx = r * rcos(theta) >> 12;
      dz = r * rsin(theta) >> 12;
      dy = r * rsin(rand() % 512) >> 12;
      evt->x2.n = evt->x1.n + dx;
      evt->z2.n = evt->z1.n + dz;
      evt->y2.n = evt->y1.n + dy;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n += (evt->x2.n - evt->x1.n) >> 3;
      evt->z1.n += (evt->z2.n - evt->z1.n) >> 3;
      evt->y1.n += (evt->y2.n - evt->y1.n) >> 3;
      UpdateMultisizeEvtAnimation(evt);
      AddEvtPrim6(gGraphicsPtr->ot, evt, 0);
      if (evt->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 691
void Evtf691_Map43_Scn93_CameraShake(EvtData *evt) {
   switch (evt->state) {
   case 0:
      evt->y1.n = gCameraPos.vy;
      evt->state++;

   // fallthrough
   case 1:
      gCameraPos.vy = evt->y1.n + rand() % 8;
      break;
   }
}

void func_800ACB4C(EvtData *sprite) {
   s32 iVar5;
   s32 iVar4;
   s32 i;
   EvtData *dataStore;
   Cylinder *dsCylinder;
   s32 theta1;
   s32 theta2;

   iVar5 = sprite->mem;
   iVar4 = sprite->state3;

   dataStore = Evt_GetUnused();
   dataStore->functionIndex = EVTF_NOOP;
   dsCylinder = &dataStore->d.dataStore.cylinder;
   dsCylinder->bottom.vx = sprite->x1.n;
   dsCylinder->bottom.vz = sprite->z1.n;
   dsCylinder->bottom.vy = sprite->y1.n;
   dsCylinder->top.vx = sprite->x1.n;
   dsCylinder->top.vz = sprite->z1.n;
   dsCylinder->top.vy = sprite->y1.n;
   dsCylinder->sideCt = 16;
   dsCylinder->semiTrans = sprite->d.sprite.semiTrans;
   dsCylinder->gfxIdx = sprite->d.sprite.gfxIdx;
   dsCylinder->clut = sprite->d.sprite.clut;
   dsCylinder->theta = sprite->state2;

   for (i = 0; i < 4; i++) {
      theta1 = i * DEG(22.5);
      theta2 = (i + 1) * DEG(22.5);
      dsCylinder->bottomRadius = iVar5 * rcos(theta1) >> 12;
      dsCylinder->topRadius = iVar5 * rcos(theta2) >> 12;
      dsCylinder->bottom.vy = sprite->y1.n + (iVar4 * rsin(theta1) >> 12);
      dsCylinder->top.vy = sprite->y1.n + (iVar4 * rsin(theta2) >> 12);
      RenderCylinder(dsCylinder);
   }

   for (i = 0; i < 4; i++) {
      theta1 = i * DEG(-22.5);
      theta2 = (i + 1) * DEG(-22.5);
      dsCylinder->bottomRadius = iVar5 * rcos(theta1) >> 12;
      dsCylinder->topRadius = iVar5 * rcos(theta2) >> 12;
      dsCylinder->bottom.vy = sprite->y1.n + (iVar4 * rsin(theta1) >> 12);
      dsCylinder->top.vy = sprite->y1.n + (iVar4 * rsin(theta2) >> 12);
      RenderCylinder(dsCylinder);
   }

   dataStore->functionIndex = EVTF_NULL;
}
