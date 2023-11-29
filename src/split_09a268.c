#include "common.h"
#include "evt.h"
#include "units.h"
#include "state.h"

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
