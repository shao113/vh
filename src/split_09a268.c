#include "common.h"
#include "object.h"
#include "units.h"
#include "state.h"
#include "graphics.h"

s32 D_801233A8;

#undef OBJF
#define OBJF 391
void Objf391(Object *obj) {
   switch (obj->state) {
   case 0:
      D_801233A8 = 0;
      obj->state++;

   // fallthrough
   case 1:
      if (gSavedPad2StateNewPresses & PAD_L2) {
         obj->state = 0;
      }
      break;
   }
}

#undef OBJF
#define OBJF 674
void Objf674_DebugSounds(Object *obj) {
   static s32 unitId = 1;

   switch (obj->state) {
   case 0:
      obj->d.dataStore.shorts[0] = 1; //
      obj->state2 = 0;
      obj->state++;

   // fallthrough
   case 1:
      if (gSavedPad2State & PAD_R1) {
         unitId++;
      }
      if (gSavedPad2State & PAD_R2) {
         unitId--;
      }
      if (gSavedPad2StateNewPresses & PAD_L2) {
         obj->state2++;
         obj->state2 %= 2;
      }

      switch (obj->state2) {
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

Object *SnapToUnit(Object *obj) {
   Object *unitSprite;

   unitSprite = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
   obj->x1.n = unitSprite->x1.n;
   obj->z1.n = unitSprite->z1.n;
   obj->y1.n = unitSprite->y1.n;

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

Object *CreatePositionedObj(Object *target, s16 objf) {
   Object *obj;

   obj = Obj_GetUnused();
   obj->functionIndex = objf;
   obj->x1.n = target->x1.n;
   obj->z1.n = target->z1.n;
   obj->y1.n = target->y1.n;
   return obj;
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

void Objf688_Noop(Object *obj) {}

Object *D_801233CC;

#undef OBJF
#define OBJF 277
void Objf277_Zoom(Object *obj) {
   //? Maybe for debugging? Spawned by SetupMapExtras() (assigned to D_801233CC); can be manipulated
   // via func_800AA10C() / func_800AA154(), but those appear to be unused.

   s32 smoothness;

   switch (obj->state) {
   case 0:
      break;
   case 1:
      smoothness = OBJ.smoothness & 0x7;
      gCameraZoom.vz += (OBJ.dstZoom - gCameraZoom.vz) >> smoothness;
      break;
   }
}

void func_800AA10C(s16 param_1, s16 param_2, s16 zoom, s16 smoothness) {
   switch (D_801233CC->state) {
   case 0:
      D_801233CC->state++;

   // fallthrough
   default:
      D_801233CC->d.objf277.todo_x24 = param_1;
      D_801233CC->d.objf277.todo_x26 = param_2;
      D_801233CC->d.objf277.dstZoom = zoom;
      D_801233CC->d.objf277.smoothness = smoothness;
      break;
   }
}

void func_800AA154(void) { D_801233CC->state = 0; }

#undef OBJF
#define OBJF 279
void Objf279_IceStorm_Camera(Object *obj) {
   static s16 zoomLevels[6] = {256, 384, 400, 480, 512, 768};
   static s16 angles[6] = {DEG(11.25), DEG(22.5), DEG(33.75), DEG(45), DEG(56.25), DEG(67.5)};

   Object *targetSprite;
   s32 rotY;
   s16 diff;

   targetSprite = OBJ.targetSprite;

   switch (obj->state) {
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
      OBJ.dstCamRotY = rotY;
      OBJ.dstZoom = zoomLevels[rand() % 5 + 1];
      OBJ.dstCamRotX = angles[rand() % 5 + 1];
      OBJ.delayType = rand() % 3 + 2;

      switch (OBJ.delayType) {
      case 2:
         OBJ.timer = 35;
         break;
      case 3:
         OBJ.timer = 50;
         break;
      case 4:
         OBJ.timer = 65;
         break;
      }

      obj->state++;

   // fallthrough
   case 1:
      if (--OBJ.timer != 0) {
         PanCamera(targetSprite->x1.n, targetSprite->y1.n + CV(0.5), targetSprite->z1.n, 3);
         gCameraRotation.vy += (OBJ.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vx += (OBJ.dstCamRotX - gCameraRotation.vx) >> 3;
         gCameraZoom.vz += (OBJ.dstZoom - gCameraZoom.vz) >> 3;
      } else {
         obj->functionIndex = OBJF_NULL;
      }
      break;
   }
}

void DimScreen(void) {
   gState.screenEffect->state = 6;
   gState.screenEffect->d.objf369.color.r = gState.screenEffect->d.objf369.color.g =
       gState.screenEffect->d.objf369.color.b = 64;
   SetScreenEffectOrdering(-10);
}

void func_800AA42C(void) {
   // for un-dim?
   gState.screenEffect->state = 7;
   gState.screenEffect->d.objf369.color.r = gState.screenEffect->d.objf369.color.g =
       gState.screenEffect->d.objf369.color.b = 0;
   SetScreenEffectOrdering(-10);
   gState.screenEffect->state2 = 2;
   SetScreenEffectOrdering(0);
}

void RenderMaskEffect(Object *unitSprite, MaskEffectPreset *preset) {
   Object *clonedSprite;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;
   s32 x, y;
   s32 cell;

   clonedSprite = Obj_GetUnused();
   CopyObject(unitSprite, clonedSprite);
   clonedSprite->functionIndex = OBJF_NOOP;
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

   AddObjPrim6(gGraphicsPtr->ot, clonedSprite, 0);
   poly = &gGraphicsPtr->quads[gQuadIndex - 1];
   setRGB0(poly, preset->color.r, preset->color.g, preset->color.b);
   clonedSprite->functionIndex = OBJF_NULL;
}

s16 gSparkleAnimData_80101838[14] = {5, GFX_SPARKLE_1, 3, GFX_SPARKLE_2, 3, GFX_SPARKLE_3,
                                     3, GFX_SPARKLE_4, 3, GFX_SPARKLE_5, 3, GFX_NULL,
                                     1, GFX_NULL};

#undef OBJF
#define OBJF 393
void Objf393_Map44_Scn00_ExplosionRays(Object *obj) {
   SVECTOR local_58;
   SVECTOR local_50;
   SVECTOR local_48;
   SVECTOR local_40;
   Object *target;
   Object *sprite;
   s32 i;
   s32 ct;
   s16 theta1;
   s16 theta2;
   s16 tmp_0x80;
   s16 tmp_0x800;

   switch (obj->state) {
   case 0:
      target = OBJ.entitySprite;
      obj->x1.n = target->x1.n;
      obj->z1.n = target->z1.n;
      obj->y1.n = target->y1.n + CV(0.5);
      obj->state++;
      break;

   case 1:
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      OBJ.sprite = sprite;
      sprite->d.sprite.gfxIdx = GFX_COLOR_15;
      sprite->d.sprite.semiTrans = 4;
      sprite->d.sprite.clut = CLUT_BLUES;
      obj->x2.n = 16;
      obj->state++;

   // fallthrough
   case 2:
      gCameraZoom.vz += 3;
      sprite = OBJ.sprite;
      obj->x2.n = obj->state3 + 16;
      obj->mem += obj->x2.n;
      theta2 = obj->mem;
      theta1 = 300 - (theta2 >> 5);
      tmp_0x80 = 0x80;
      tmp_0x800 = 0x800;
      ct = obj->state3 / 16 + 8;

      for (i = 0; i < ct; i++) {
         func_800A9E78(&local_58, tmp_0x80, theta1 + DEG(2.8125), theta2);
         func_800A9E78(&local_50, tmp_0x80, theta1, theta2);
         func_800A9E78(&local_48, tmp_0x800, theta1 + DEG(2.8125), theta2);
         func_800A9E78(&local_40, tmp_0x800, theta1, theta2);

         sprite->d.sprite.coords[0].x = obj->x1.n + local_58.vx;
         sprite->d.sprite.coords[0].z = obj->z1.n + local_58.vz;
         sprite->d.sprite.coords[0].y = obj->y1.n + local_58.vy;
         sprite->d.sprite.coords[1].x = obj->x1.n + local_50.vx;
         sprite->d.sprite.coords[1].z = obj->z1.n + local_50.vz;
         sprite->d.sprite.coords[1].y = obj->y1.n + local_58.vy;
         sprite->d.sprite.coords[2].x = obj->x1.n + local_48.vx;
         sprite->d.sprite.coords[2].z = obj->z1.n + local_48.vz;
         sprite->d.sprite.coords[2].y = obj->y1.n + local_48.vy;
         sprite->d.sprite.coords[3].x = obj->x1.n + local_40.vx;
         sprite->d.sprite.coords[3].z = obj->z1.n + local_40.vz;
         sprite->d.sprite.coords[3].y = obj->y1.n + local_40.vy;

         AddObjPrim4(gGraphicsPtr->ot, sprite);
         theta2 += DEG(360) / ct;
      }
      if (obj->state3 == 128) {
         obj->state++;
      }
      break;

   case 3:
      sprite = OBJ.sprite;
      sprite->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 276
void Objf276_Fx_TBD(Object *obj) {
   static s16 animData[20] = {
       0, GFX_FLAME_WALL_1, 2, GFX_FLAME_WALL_2, 2, GFX_FLAME_WALL_3, 2, GFX_FLAME_WALL_4,
       2, GFX_FLAME_WALL_5, 2, GFX_FLAME_WALL_6, 2, GFX_FLAME_WALL_7, 2, GFX_FLAME_WALL_8,
       2, GFX_NULL,         1, GFX_NULL,
   };

   s16 cluts_unused[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};
   Object *obj_a0;
   Cylinder *dsCylinder;
   s32 i;
   s16 a, b;
   s32 r;

   switch (obj->state) {
   case 0:
      obj_a0 = OBJ.variant_0x24.targetParam;
      obj->x1.n = obj_a0->x1.n;
      obj->z1.n = obj_a0->z1.n;
      obj->y1.n = obj_a0->y1.n;
      OBJ.animData = animData;

      obj_a0 = Obj_GetUnused();
      obj_a0->functionIndex = OBJF_NOOP;
      OBJ.dataStore = obj_a0;
      dsCylinder = &obj_a0->d.dataStore.cylinder;
      dsCylinder->top.vx = dsCylinder->bottom.vx = obj->x1.n;
      dsCylinder->top.vz = dsCylinder->bottom.vz = obj->z1.n;
      dsCylinder->bottom.vy = obj->y1.n;
      dsCylinder->top.vy = obj->y1.n + CV(4.0);
      dsCylinder->sideCt = 16;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = GFX_TILED_FLAMES;
      obj->mem = CV(0.25);
      dsCylinder->bottomRadius = CV(0.25);
      dsCylinder->topRadius = CV(0.25);
      dsCylinder->useColor = 1;
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = 128;
      dsCylinder->theta = 0;
      dsCylinder->clut = CLUT_REDS;
      OBJ.variant_0x24.unk = 5;
      obj->state++;

   // fallthrough
   case 1:
      obj->state2++;
      obj_a0 = OBJ.dataStore;
      dsCylinder = &obj_a0->d.dataStore.cylinder;
      dsCylinder->theta += obj->state2 * 2;
      UpdateObjAnimation(obj);

      for (i = 0; i < 8; i++) {
         a = (i + 1) * DEG(11.25);
         b = i * DEG(11.25);
         r = obj->mem;
         dsCylinder->semiTrans = 4;
         dsCylinder->bottom.vy = obj->y1.n + (r * rsin(b) >> 12);
         dsCylinder->top.vy = obj->y1.n + (r * rsin(a) >> 12);
         dsCylinder->bottomRadius = r * rcos(b) >> 12;
         dsCylinder->topRadius = r * rcos(a) >> 12;
         RenderCylinder(dsCylinder);
      }
      obj->mem += CV(0.015625);
      break;

   case 2:
      obj_a0 = OBJ.dataStore;
      obj_a0->functionIndex = OBJF_NULL;
      obj_a0 = OBJ.unused_0x58; //?
      obj_a0->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 275
void Objf275_Fx_TBD(Object *obj) {
   static s16 animData[] = {4, GFX_EXPLOSION_1,  2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,
                            2, GFX_EXPLOSION_4,  2, GFX_EXPLOSION_5,  2, GFX_EXPLOSION_6,
                            2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,  2, GFX_EXPLOSION_9,
                            2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
                            0, GFX_NULL};

   switch (obj->state) {
   case 0:
      OBJ.gfxIdx = GFX_EXPLOSION_1;
      OBJ.animData = animData;
      obj->state++;

   // fallthrough
   case 1:
      UpdateObjAnimation(obj);
      AddObjPrim6(gGraphicsPtr->ot, obj, 0);
      obj->x1.n += (obj->x2.n - obj->x1.n) >> 2;
      obj->z1.n += (obj->z2.n - obj->z1.n) >> 2;
      obj->y1.n += (obj->y2.n - obj->y1.n) >> 2;
      if (OBJ.animFinished) {
         obj->functionIndex = OBJF_NULL;
      }
      break;
   }
}

#undef OBJF
#define OBJF 273
void Objf273_OutwardRay(Object *obj) {
   s32 rnd;
   s32 r;

   switch (obj->state) {
   case 0:
      OBJ.theta1 = rand() % DEG(33.75);
      OBJ.theta2 = rand() % DEG(360);
      OBJ.unused_0x3A = rand() % 64 + 32;
      OBJ.todo_x38 = CV(0.5);
      obj->x3.n = 32;
      obj->z3.n = 8;

      rnd = (rand() >> 3) % 4096;
      if (rnd >= 512) {
         obj->mem = 3;
      } else {
         obj->mem = 2;
      }

      OBJ.todo_x54 = -32 - rand() % 64;

      switch (obj->state3) {
      case 0:
         OBJ.gfxIdx = GFX_COLOR_15;
         OBJ.semiTrans = 2;
         OBJ.clut = CLUT_BLUES;
         break;
      case 1:
         OBJ.gfxIdx = GFX_COLOR_12;
         OBJ.semiTrans = 0;
         OBJ.clut = CLUT_PURPLES;
         break;
      }

      obj->state2 = 0;

      r = (OBJ.todo_x38 * rcos(OBJ.theta1)) >> 12;
      OBJ.coords[1].x = obj->x2.n + (r * rcos(OBJ.theta2) >> 12);
      OBJ.coords[1].z = obj->z2.n + (r * rsin(OBJ.theta2) >> 12);
      OBJ.coords[3].x = obj->x2.n + (r * rcos(OBJ.theta2 + DEG(1.40625)) >> 12);
      OBJ.coords[3].z = obj->z2.n + (r * rsin(OBJ.theta2 + DEG(1.40625)) >> 12);
      OBJ.coords[1].y = OBJ.coords[3].y = obj->y2.n + (OBJ.todo_x38 * rsin(OBJ.theta1) >> 12);

      obj->state++;

   // fallthrough
   case 1:
      OBJ.todo_x38 += obj->x3.n;
      obj->x3.n += obj->z3.n;

      r = (OBJ.todo_x38 * rcos(OBJ.theta1)) >> 12;
      OBJ.coords[0].x = obj->x2.n + (r * rcos(OBJ.theta2) >> 12);
      OBJ.coords[0].z = obj->z2.n + (r * rsin(OBJ.theta2) >> 12);
      OBJ.coords[2].x = obj->x2.n + (r * rcos(OBJ.theta2 + DEG(1.40625)) >> 12);
      OBJ.coords[2].z = obj->z2.n + (r * rsin(OBJ.theta2 + DEG(1.40625)) >> 12);
      OBJ.coords[0].y = OBJ.coords[2].y = obj->y2.n + (OBJ.todo_x38 * rsin(OBJ.theta1) >> 12);

      obj->x1.n = OBJ.coords[0].x;
      obj->z1.n = OBJ.coords[0].z;
      obj->y1.n = OBJ.coords[0].y;

      AddObjPrim5(gGraphicsPtr->ot, obj);

      OBJ.coords[1].x = OBJ.coords[0].x;
      OBJ.coords[1].z = OBJ.coords[0].z;
      OBJ.coords[3].x = OBJ.coords[2].x;
      OBJ.coords[3].z = OBJ.coords[2].z;
      OBJ.coords[1].y = OBJ.coords[3].y = OBJ.coords[0].y;

      if (OBJ.todo_x38 >= CV(4.5)) {
         obj->state++;
      }
      break;

   case 2:
      obj->functionIndex = OBJF_NULL;
      break;
   }
}

#undef OBJF
#define OBJF 271
void Objf271_Map36_Scn74_LeenaCastingShield(Object *obj) {
   Object *obj_s5;
   Object *forcefield;
   Cylinder *dsCylinder;
   s32 i;
   s32 theta1;
   s32 theta2;
   s32 iVar6;

   switch (obj->state) {
   case 0:
      obj_s5 = OBJ.entitySprite;
      obj->x1.n = obj_s5->x1.n;
      obj->z1.n = obj_s5->z1.n;
      obj->y1.n = obj_s5->y1.n;

      obj_s5 = Obj_GetUnused();
      obj_s5->functionIndex = OBJF_NOOP;
      dsCylinder = &obj_s5->d.dataStore.cylinder;
      dsCylinder->bottom.vx = obj->x1.n;
      dsCylinder->bottom.vz = obj->z1.n;
      dsCylinder->bottom.vy = obj->y1.n;
      dsCylinder->top.vx = obj->x1.n;
      dsCylinder->top.vz = obj->z1.n;
      dsCylinder->top.vy = obj->y1.n;
      dsCylinder->sideCt = 32;
      dsCylinder->semiTrans = 2;
      dsCylinder->gfxIdx = GFX_COLOR_11;
      dsCylinder->clut = CLUT_BLUES;
      dsCylinder->useColor = 1;
      dsCylinder->color.r = 255;
      dsCylinder->color.g = 255;
      dsCylinder->color.b = 255;
      OBJ.dataStore = obj_s5;
      obj->mem = 0;
      obj->state++;

   // fallthrough
   case 1:
      iVar6 = (CV(1.0) - obj->state3) * 4;
      theta1 = obj->mem;
      iVar6 += CV(0.8125);
      theta2 = theta1 + DEG(2.8125);
      if (theta2 >= DEG(90)) {
         theta2 = DEG(90);
      }

      obj_s5 = OBJ.dataStore;
      dsCylinder = &obj_s5->d.dataStore.cylinder;
      dsCylinder->theta = obj->state3 << 7; //* 128
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = (1024 - theta1) >> 2;

      for (i = 0; i < 2; i++) {
         dsCylinder->bottomRadius = iVar6 * rcos(theta1) >> 12;
         dsCylinder->topRadius = iVar6 * rcos(theta2) >> 12;
         dsCylinder->bottom.vy = obj->y1.n + (iVar6 * rsin(theta1) >> 12);
         dsCylinder->top.vy = obj->y1.n + (iVar6 * rsin(theta2) >> 12);
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
      dsCylinder->color.r = dsCylinder->color.g = dsCylinder->color.b = (1024 - obj->mem) >> 2;

      for (i = 0; i < 8; i++) {
         dsCylinder->bottomRadius = iVar6 * rcos(theta1) >> 12;
         dsCylinder->topRadius = iVar6 * rcos(theta2) >> 12;
         dsCylinder->bottom.vy = obj->y1.n + (iVar6 * rsin(theta1) >> 12);
         dsCylinder->top.vy = obj->y1.n + (iVar6 * rsin(theta2) >> 12);
         RenderCylinder(dsCylinder);
         theta1 += DEG(11.25);
         theta2 += DEG(11.25);
      }

      obj->mem += 64;
      if (obj->mem >= 1024) {
         obj->mem = 0;
      }

      obj->state3 += 8;
      if (obj->state3 == 192) {
         forcefield = Obj_GetUnused();
         forcefield->functionIndex = OBJF_LEENA_FORCEFIELD;
         forcefield->d.objf675.targetSprite = OBJ.entitySprite;
      }
      if (obj->state3 >= 208) {
         obj_s5->functionIndex = OBJF_NULL;
         obj->state++;
      }
      break;

   case 2:
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

s16 gCluts_801230a4[4] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};

#undef OBJF
#define OBJF 270
void Objf270_Fx_TBD(Object *obj) {
   static s16 splashAnimData[20] = {
       0, GFX_SPLASH_1, 1, GFX_SPLASH_2, 1, GFX_SPLASH_3, 1, GFX_SPLASH_4, 1, GFX_SPLASH_5,
       1, GFX_SPLASH_6, 1, GFX_SPLASH_7, 1, GFX_SPLASH_8, 1, GFX_NULL,     1, GFX_NULL};

   s32 i;
   Object *link;   // s2
   Object *sprite; // s1
   Object *obj_s0;
   Cylinder *dsCylinder;

   switch (obj->state) {
   case 0:
      SnapToUnit(obj);
      obj->y1.n += CV(0.5);

      link = obj;
      for (i = 0; i < 8; i++) {
         CreatePositionedObj(obj, OBJF_FX_TBD_269);
         obj_s0 = Obj_GetUnused();
         obj_s0->functionIndex = OBJF_FX_TBD_269;
         obj_s0->d.objf269.link = link;
         obj_s0->d.objf269.parent = obj;
         link = obj_s0;
      }

      gCameraZoom.vz = 256;
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      OBJ.sprite = sprite;
      sprite->d.sprite.animData = splashAnimData;
      gCameraRotation.vy += 16;
      obj->state++;

   // fallthrough
   case 1:
      sprite = OBJ.sprite;

      if (--obj->mem <= 0) {
         obj_s0 = CreatePositionedObj(obj, OBJF_FX_TBD_099);
         obj_s0->x1.n += rand() % CV(0.5);
         obj_s0->z1.n += rand() % CV(0.5);
         obj_s0->y1.n += rand() % CV(0.5);
         obj_s0->d.objf099.theta1 = 0;
         obj_s0->d.objf099.theta2 = 0;
         obj_s0->d.objf099.todo_x28 = rand() % 64 + 128;
         obj_s0->d.objf099.clut = gCluts_801230a4[rand() % 4];
         obj->mem += rand() % 4;
      }

      if (--obj->mem <= 0) {
         obj_s0 = CreatePositionedObj(obj, OBJF_FX_TBD_088);
         obj_s0->x1.n += rand() % CV(0.5);
         obj_s0->z1.n += rand() % CV(0.5);
         obj_s0->y1.n += rand() % CV(0.5);
         obj_s0->y2.n = rand() % 8 + 8;
         obj_s0->mem = 64;
         obj->mem += rand() % 2;
      }

      UpdateObjAnimation(sprite);

      obj_s0 = Obj_GetUnused();
      obj_s0->functionIndex = OBJF_NOOP;
      dsCylinder = &obj_s0->d.dataStore.cylinder;
      dsCylinder->bottom.vx = obj->x1.n;
      dsCylinder->bottom.vz = obj->z1.n;
      dsCylinder->bottom.vy = obj->y1.n;
      dsCylinder->top.vx = obj->x1.n;
      dsCylinder->top.vz = obj->z1.n;
      dsCylinder->top.vy = obj->y1.n + CV(1.0);
      dsCylinder->sideCt = 16;
      dsCylinder->semiTrans = 4;
      dsCylinder->gfxIdx = sprite->d.sprite.gfxIdx;
      dsCylinder->bottomRadius = CV(1.0);
      dsCylinder->topRadius = CV(1.0);
      dsCylinder->theta = sprite->state2;
      sprite->state2 += 32;
      dsCylinder->clut = CLUT_PURPLES;
      RenderCylinder(dsCylinder);
      obj_s0->functionIndex = OBJF_NULL;

      OBJ.todo_x24 = 0;
      OBJ.todo_x26 = 0;
      break;
   }
}

#undef OBJF
#define OBJF 269
void Objf269_Fx_TBD(Object *obj) {
   static s16 lightningAnimData[20] = {0, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3,
                                       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
                                       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_NULL,
                                       1, GFX_NULL};

   Object *link;
   Object *sprite;
   s16 radius;
   s16 theta;

   link = OBJ.link;

   switch (obj->state) {
   case 0:
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_COLOR_15;
      sprite->d.sprite.clut = CLUT_REDS;
      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.animData = lightningAnimData;
      OBJ.sprite = sprite;
      obj->state++;

   // fallthrough
   case 1:
      obj->mem++;
      radius = abs(192 * rsin(obj->mem * 16) >> 12);
      sprite = OBJ.sprite;
      OBJ.theta2 = OBJ.theta1 = rand() % DEG(180) - DEG(90);
      UpdateObjAnimation(sprite);
      theta = OBJ.theta2;
      obj->x1.n = link->x1.n;
      obj->z1.n = link->z1.n + (radius * rcos(theta) >> 12);
      obj->y1.n = link->y1.n + (radius * rsin(theta) >> 12);
      sprite->x1.n = obj->x1.n;
      sprite->z1.n = obj->z1.n;
      sprite->y1.n = obj->y1.n;
      sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[1].x = link->x1.n;
      sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[3].x = obj->x1.n;
      sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z = link->z1.n;
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[3].z = obj->z1.n;
      sprite->d.sprite.coords[0].y = link->y1.n - CV(0.5);
      sprite->d.sprite.coords[1].y = link->y1.n;
      sprite->d.sprite.coords[2].y = obj->y1.n - CV(0.5);
      sprite->d.sprite.coords[3].y = obj->y1.n;
      AddObjPrim3(gGraphicsPtr->ot, sprite);
      break;
   }
}

#undef OBJF
#define OBJF 099
void Objf099_Fx_TBD(Object *obj) {
   Object *sprite;
   SVECTOR svec;

   switch (obj->state) {
   case 0:
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->x1.n = obj->x1.n;
      sprite->z1.n = obj->z1.n;
      sprite->y1.n = obj->y1.n;
      sprite->d.sprite.gfxIdx = GFX_POISONED;
      sprite->d.sprite.clut = OBJ.clut;
      sprite->d.sprite.semiTrans = 2;
      OBJ.sprite = sprite;
      obj->mem = OBJ.todo_x30;
      obj->state++;

   // fallthrough
   case 1:
      func_800A9E78(&svec, OBJ.todo_x28, OBJ.theta1, OBJ.theta2);
      obj->x1.n += svec.vx;
      obj->z1.n += svec.vz;
      obj->y1.n += svec.vy;
      sprite = OBJ.sprite;
      sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[3].x = obj->x1.n;
      sprite->d.sprite.coords[0].x = sprite->d.sprite.coords[2].x = sprite->x1.n;
      sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[3].z = obj->z1.n;
      sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[2].z = sprite->z1.n;
      sprite->d.sprite.coords[1].y = obj->y1.n;
      sprite->d.sprite.coords[3].y = obj->y1.n + CV(0.0625);
      sprite->d.sprite.coords[0].y = sprite->y1.n;
      sprite->d.sprite.coords[2].y = sprite->y1.n + CV(0.0625);
      AddObjPrim3(gGraphicsPtr->ot, sprite);

      sprite->x1.n = obj->x1.n;
      sprite->z1.n = obj->z1.n;
      sprite->y1.n = obj->y1.n;

      if (--obj->mem <= 0) {
         obj->state++;
      }
      break;

   case 2:
      sprite = OBJ.sprite;
      sprite->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      break;
   }
}

#undef OBJF
#define OBJF 088
void Objf088_Fx_TBD(Object *obj) {
   switch (obj->state) {
   case 0:
      OBJ.gfxIdx = GFX_COLOR_12;
      OBJ.clut = CLUT_BLUES;
      obj->state++;

   // fallthrough
   case 1:
      obj->x1.n += obj->x2.n;
      obj->z1.n += obj->z2.n;
      obj->y1.n += obj->y2.n;

      OBJ.coords[0].x = OBJ.coords[1].x = obj->x1.n;
      OBJ.coords[2].x = OBJ.coords[3].x = obj->x1.n;
      OBJ.coords[0].z = OBJ.coords[2].z = obj->z1.n;
      OBJ.coords[1].z = OBJ.coords[3].z = obj->z1.n + CV(0.03125);
      OBJ.coords[0].y = OBJ.coords[1].y = obj->y1.n;
      OBJ.coords[2].y = OBJ.coords[3].y = obj->y1.n + CV(0.03125);

      AddObjPrim3(gGraphicsPtr->ot, obj);

      if (--obj->mem <= 0) {
         obj->functionIndex = OBJF_NULL;
      }
      break;
   }
}

void func_800ABFB8(Object *sprite) {
   s32 dx, dz, dy;
   s32 i;
   s16 rnd;
   Object *sprite_s0;
   s16 tmp;

   dx = (sprite->x2.n - sprite->x1.n) / 8;
   dy = (sprite->y2.n - sprite->y1.n) / 8;
   dz = (sprite->z2.n - sprite->z1.n) / 8;

   sprite_s0 = Obj_GetUnused();
   sprite_s0->functionIndex = OBJF_NOOP;
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
      AddObjPrim4(gGraphicsPtr->ot, sprite_s0);
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
   AddObjPrim4(gGraphicsPtr->ot, sprite_s0);
   sprite_s0->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 340
void Objf340_Map48_Scn20_TBD(Object *obj) {
   Object *obj_s0;

   switch (obj->state) {
   case 0:

      switch (obj->state2) {
      case 0:
         obj->x1.n = CV(5.5);
         obj->z1.n = CV(2.5);
         obj->y1.n = CV(0.5);
         obj->state2++;

      // fallthrough
      case 1:
         obj->mem += 2;
         if (obj->mem >= 96) {
            obj->state2 = 0;
            obj->mem = 0;
            obj->state++;
         }
         break;
      }

      break;

   case 1:
      obj_s0 = Obj_GetUnused();
      obj_s0->functionIndex = OBJF_FX_TBD_099;
      obj_s0->x1.n = obj->x1.n + CV(2.0);
      obj_s0->y1.n = obj->y1.n;
      obj_s0->z1.n = obj->z1.n + (rand() % 1024 - CV(2.0));
      obj_s0->d.objf099.theta1 = DEG(180);
      obj_s0->d.objf099.theta2 = DEG(0);
      obj_s0->d.objf099.todo_x28 = rand() % 64 + 256;
      obj_s0->d.objf099.clut = gCluts_801230a4[rand() % 4];
      obj_s0->d.objf099.todo_x30 = 16;

      obj_s0 = Obj_GetUnused();
      obj_s0->functionIndex = OBJF_NOOP;
      obj_s0->x1.n = obj->x1.n;
      obj_s0->z1.n = obj->z1.n;
      obj_s0->y1.n = obj->y1.n + CV(0.5);
      obj_s0->x2.n = obj->x1.n - obj->state3;
      obj_s0->z2.n = obj->z1.n;
      obj_s0->y2.n = obj->y1.n + CV(0.5);
      obj_s0->d.sprite.clut = CLUT_BLUES;
      func_800ABFB8(obj_s0);
      obj_s0->z2.n = obj->z1.n + (obj->state3 >> 2);
      obj_s0->d.sprite.clut = CLUT_GREENS;
      func_800ABFB8(obj_s0);
      obj_s0->z2.n = obj->z1.n - (obj->state3 >> 2);
      obj_s0->d.sprite.clut = CLUT_PURPLES;
      func_800ABFB8(obj_s0);
      obj_s0->functionIndex = OBJF_NULL;
      obj->state3 += (1024 - obj->state3) >> 4;
      break;

   case 2:
      gLightColor.r = 128;
      gLightColor.g = 128;
      gLightColor.b = 128;
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 305
void Objf305_328_MagicStoneFx(Object *obj) {
   Object *obj_a0;
   Object *sprite;
   s32 i;
   s32 iVar7;
   u32 tmp;
   POLY_FT4 *poly;

   switch (obj->state) {
   case 0:
      obj_a0 = OBJ.variant_0x24.entitySpriteParam;
      obj->x1.n = obj_a0->x1.n;
      obj->z1.n = obj_a0->z1.n;
      obj->y1.n = obj_a0->y1.n;
      OBJ.variant_0x24.timers.ray = 5;

      sprite = CreatePositionedObj(obj, OBJF_NOOP);
      sprite->y1.n += CV(0.5625);
      sprite->d.sprite.gfxIdx = GFX_LARGE_RED_CIRCLE;
      sprite->d.sprite.boxIdx = 7;
      sprite->d.sprite.clut = CLUT_PURPLES;
      sprite->d.sprite.semiTrans = 4;
      OBJ.sprite = sprite;

      obj->state++;

   // fallthrough
   case 1:
      obj->state2++;
      UpdateObjAnimation(obj);
      obj->mem += 4;
      if (--OBJ.variant_0x24.timers.ray <= 0) {
         for (i = 0; i < 3; i++) {
            tmp = rand() % 2; //
            obj_a0 = CreatePositionedObj(obj, OBJF_OUTWARD_RAY);
            obj_a0->x2.n = obj->x1.n;
            obj_a0->z2.n = obj->z1.n;
            obj_a0->y2.n = obj->y1.n + CV(0.5);
            if (obj->functionIndex == OBJF_MAP43_SCN93_MAGIC_STONE_FX) {
               obj_a0->state3 = 1;
            }
         }
         OBJ.variant_0x24.timers.ray = rand() % 3;
      }
      if (--OBJ.variant_0x24.timers.explosion <= 0) {
         if (obj->functionIndex == OBJF_MAP43_SCN93_MAGIC_STONE_FX) {
            CreatePositionedObj(obj, OBJF_MAGIC_STONE_EXPLOSION);
         }
         OBJ.variant_0x24.timers.explosion = rand() % 16 + 8;
      }
      sprite = OBJ.sprite;
      i = rsin(obj->state2 * 32); //
      iVar7 = abs(24 * i >> 12);
      sprite->d.sprite.gfxIdx = GFX_GLOBE_1 + iVar7 / 3;
      if (sprite->d.sprite.gfxIdx >= GFX_GLOBE_8) {
         sprite->d.sprite.gfxIdx = GFX_GLOBE_8;
      }
      if (obj->functionIndex == OBJF_MAP43_SCN93_MAGIC_STONE_FX) {
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
         AddObjPrim6(gGraphicsPtr->ot, sprite, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         poly->r0 = (iVar7 - i) * 4;
         poly->g0 = (iVar7 - i) * 4;
         poly->b0 = (iVar7 - i) * 4;
      }
      break;

   case 2:
      obj_a0 = OBJ.unused_0x5C; //?
      obj_a0->functionIndex = OBJF_NULL;
      obj_a0 = OBJ.sprite;
      obj_a0->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

s16 gCluts_80101900[] = {CLUT_REDS, CLUT_BLUES, CLUT_PURPLES, CLUT_GREENS};

#undef OBJF
#define OBJF 690
void Objf690_MagicStoneExplosion(Object *obj) {
   static s16 animData[35] = {GFX_EXPLOSION_1,  2, 4, GFX_EXPLOSION_2,  2, 4, GFX_EXPLOSION_3, 2, 4,
                              GFX_EXPLOSION_4,  2, 3, GFX_EXPLOSION_5,  2, 3, GFX_EXPLOSION_6, 2, 3,
                              GFX_EXPLOSION_7,  2, 3, GFX_EXPLOSION_8,  2, 3, GFX_EXPLOSION_9, 2, 3,
                              GFX_EXPLOSION_10, 2, 3, GFX_EXPLOSION_11, 2, 3, GFX_NULL,        0};

   s32 r;
   s32 theta;
   s32 dx, dy, dz;

   switch (obj->state) {
   case 0:
      obj->d.sprite.animData = animData;
      obj->d.sprite.clut = CLUT_REDS;
      r = rand() % CV(0.5) + CV(1.0);
      theta = rand() % DEG(360);
      dx = r * rcos(theta) >> 12;
      dz = r * rsin(theta) >> 12;
      dy = r * rsin(rand() % 512) >> 12;
      obj->x2.n = obj->x1.n + dx;
      obj->z2.n = obj->z1.n + dz;
      obj->y2.n = obj->y1.n + dy;
      obj->state++;

   // fallthrough
   case 1:
      obj->x1.n += (obj->x2.n - obj->x1.n) >> 3;
      obj->z1.n += (obj->z2.n - obj->z1.n) >> 3;
      obj->y1.n += (obj->y2.n - obj->y1.n) >> 3;
      UpdateMultisizeObjAnimation(obj);
      AddObjPrim6(gGraphicsPtr->ot, obj, 0);
      if (obj->d.sprite.animFinished) {
         obj->functionIndex = OBJF_NULL;
      }
      break;
   }
}

#undef OBJF
#define OBJF 691
void Objf691_Map43_Scn93_CameraShake(Object *obj) {
   switch (obj->state) {
   case 0:
      obj->y1.n = gCameraPos.vy;
      obj->state++;

   // fallthrough
   case 1:
      gCameraPos.vy = obj->y1.n + rand() % 8;
      break;
   }
}

void func_800ACB4C(Object *sprite) {
   s32 iVar5;
   s32 iVar4;
   s32 i;
   Object *dataStore;
   Cylinder *dsCylinder;
   s32 theta1;
   s32 theta2;

   iVar5 = sprite->mem;
   iVar4 = sprite->state3;

   dataStore = Obj_GetUnused();
   dataStore->functionIndex = OBJF_NOOP;
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

   dataStore->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 318
void Objf318_Fx_TBD(Object *obj) {
   switch (obj->state) {
   case 0:
      SnapToUnit(obj);
      obj->y1.n += CV(1.0);
      obj->d.sprite.gfxIdx = GFX_TILED_RED_SPARKLES;
      obj->d.sprite.semiTrans = 4;
      obj->d.sprite.clut = CLUT_BLUES;
      obj->state++;

   // fallthrough
   case 1:
      obj->state2 += 32;
      if (++obj->mem >= 512) {
         obj->state++;
      }
      func_800ACB4C(obj);
      break;

   case 2:
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 742
void Objf742_Map67_Scn34_TBD(Object *obj) {
   s32 i;
   Object *p;

   p = &gObjectArray[0];

   for (i = 0; i < ARRAY_COUNT(gObjectArray); i++) {
      if (p->functionIndex == OBJF_MAP67_SCN34_TBD_319) {
         if (p->state == 1 && p->state2 == 2) {
            p->state2++;
         }
         break;
      }
      p++;
   }

   obj->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 319
void Objf319_Map67_Scn34_TBD(Object *obj) {
   Object *obj_s2;
   Object *obj_s0;
   Object *obj_s3;

   switch (obj->state) {
   case 0:
      obj_s2 = OBJ.entitySprite;
      obj->x1.n = obj_s2->x1.n;
      obj->z1.n = obj_s2->x1.n;
      obj->y1.n = obj_s2->y1.n + CV(1.0);

      obj_s2 = CreatePositionedObj(obj, OBJF_FX_TBD_320);
      obj_s2->x3.n = obj->x1.n - CV(1.5);
      OBJ.todo_x5c = obj_s2;

      obj_s0 = CreatePositionedObj(obj, OBJF_NOOP);
      obj_s0->x1.n -= CV(3.0);
      OBJ.todo_x58 = obj_s0;
      obj_s0->d.sprite.clut = CLUT_PURPLES;
      obj_s0->d.sprite.semiTrans = 2;
      OBJ.semiTrans = 2;

      obj_s2 = CreatePositionedObj(obj_s0, OBJF_FX_TBD_320);
      obj_s2->x3.n = obj_s0->x1.n + CV(1.5);
      OBJ.todo_x50 = obj_s2;
      obj->state++;

   // fallthrough
   case 1:
      obj_s2 = OBJ.todo_x5c;
      obj->x2.n = obj_s2->x1.n;
      obj->z2.n = obj_s2->z1.n;
      obj->y2.n = obj_s2->y1.n;
      func_800ABFB8(obj);
      func_800ABFB8(obj);
      func_800ABFB8(obj);
      obj_s0 = OBJ.todo_x58;
      obj_s3 = OBJ.todo_x50;
      obj_s0->x2.n = obj_s3->x1.n;
      obj_s0->z2.n = obj_s3->z1.n;
      obj_s0->y2.n = obj_s3->y1.n;
      func_800ABFB8(obj_s0);
      func_800ABFB8(obj_s0);

      switch (obj->state2) {
      case 0:
         obj->state2++;

      // fallthrough
      case 1:
         obj->state2++;

      // fallthrough
      case 2:
         if (--obj->state3 <= 0) {
            obj_s2 = CreatePositionedObj(obj, OBJF_FX_TBD_728);
            obj_s2->x1.n -= CV(1.5);
            obj_s2->mem = 16;
            obj->state3 = rand() % 3 + 8;
         }
         break;

      case 3:
         obj_s2 = OBJ.todo_x5c;
         obj_s3 = OBJ.todo_x50;
         obj_s2->x2.n = obj_s3->x2.n = obj->x1.n - CV(1.5) - obj->mem;
         if (--obj->state3 <= 0) {
            obj_s0 = CreatePositionedObj(obj_s3, OBJF_FX_TBD_728);
            obj_s0->mem = 16;
            obj->state3 = rand() % 3 + 8;
         }
         obj->mem += 2;
         if (obj->mem >= 384) {
            obj_s2->functionIndex = OBJF_NULL;
            obj_s3->functionIndex = OBJF_NULL;
            obj->state++;
            obj->mem = 384;
         }
         break;

      case 4:
         obj->state++;
         break;
      }

      break;

   case 2:
      FadeOutScreen(0, 0xff);
      obj->state++;
      break;

   case 3:
      obj->state++;
      break;

   case 4:
      FadeInScreen(0, 8);
      obj->state++;
      break;

   case 5:
      obj_s0 = OBJ.todo_x58;
      obj_s0->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 320
void Objf320_Fx_TBD(Object *obj) {
   s32 i;
   Object *obj_s4;
   s32 r;
   s32 a, b;

   switch (obj->state) {
   case 0:
      obj->state++;

   // fallthrough
   case 1:
      obj->x1.n += (obj->x3.n - obj->x1.n) >> 2;
      if (++obj->mem >= 32) {
         obj->state++;
      }
      break;

   case 2:
      obj->x2.n = obj->x1.n;
      obj->z2.n = obj->z1.n;
      obj->y2.n = obj->y1.n;
      obj->mem = 0;
      obj->state++;

   // fallthrough
   case 3:
      rsin(obj->mem << 4);
      obj->x1.n = obj->x2.n;
      obj->z1.n = obj->z2.n;
      obj->y1.n = obj->y2.n;
      obj->mem++;

      for (i = 0; i < 5; i++) {
         obj_s4 = CreatePositionedObj(obj, OBJF_FX_TBD_088);
         a = rand() % DEG(360);
         b = rand() % DEG(360);
         r = rand() % CV(0.125) + CV(0.0625);
         obj_s4->x2.n = r * rcos(a) >> 12;
         obj_s4->z2.n = r * rsin(a) >> 12;
         obj_s4->y2.n = r * rsin(b) >> 12;
         obj_s4->mem = 12;
      }
      break;
   }
}

#undef OBJF
#define OBJF 728
void Objf728_Fx_TBD(Object *obj) {
   s16 halfSize;

   switch (obj->state) {
   case 0:
      obj->d.sprite.gfxIdx = GFX_RING;
      obj->d.sprite.clut = CLUT_BLUES;
      obj->state++;

   // fallthrough
   case 1:
      halfSize = obj->state2;
      obj->state2 += 8;
      obj->d.sprite.coords[0].x = obj->d.sprite.coords[1].x = obj->d.sprite.coords[2].x =
          obj->d.sprite.coords[3].x = obj->x1.n;
      obj->d.sprite.coords[0].z = obj->d.sprite.coords[2].z = obj->z1.n - halfSize;
      obj->d.sprite.coords[1].z = obj->d.sprite.coords[3].z = obj->z1.n + halfSize;
      obj->d.sprite.coords[0].y = obj->d.sprite.coords[1].y = obj->y1.n - halfSize;
      obj->d.sprite.coords[2].y = obj->d.sprite.coords[3].y = obj->y1.n + halfSize;
      obj->mem++;
      obj->d.sprite.hidden = obj->mem % 2;
      AddObjPrim4(gGraphicsPtr->ot, obj);
      if (obj->state2 >= 256) {
         obj->functionIndex = OBJF_NULL;
      }
      break;
   }
}

// gSparkleAnimData_80101838, gCluts_801230a4, gCluts_80101900 static?
