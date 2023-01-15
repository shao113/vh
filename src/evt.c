#include "common.h"
#include "evt.h"
#include "state.h"
#include "graphics.h"
#include "audio.h"

void Evtf581_AudioCommand(EvtData *evt) {
   switch (evt->state) {
   case 0:
      if ((evt->d.evtf581.delay != 0) && (--evt->d.evtf581.delay != 0)) {
         return;
      }
      evt->state++;
      /* fallthrough */
   case 1:
      PerformAudioCommand(evt->d.evtf581.cmd);
      evt->functionIndex = EVTF_NULL;
   }
}

void Evt_ResetByFunction(s16 functionIndex) {
   s32 i;
   EvtData *evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, evt++) {
      if (evt->functionIndex == functionIndex) {
         evt->functionIndex = EVTF_NULL;
      }
   }
}

EvtData *Evt_FindByFunction(s16 functionIndex) {
   s32 i;
   gTempEvt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, gTempEvt++) {
      if (gTempEvt->functionIndex == functionIndex) {
         return gTempEvt;
      }
   }
   // return NULL;
}

void Evtf407_NoopIncState(EvtData *evt) {
   // ?
   s32 tmp;
   if (evt->state == 0) {
      tmp = (u16)evt->state;
      evt->state = tmp + 1;
   }
}

void Evtf405_Panorama(EvtData *evt) {
   EvtData *spr;
   s32 i, iy;
   // s32 ix, gfx; //<- to match, these were combined into "i"
   s16 xOfs = 0;
   s16 yOfs = 0;

   switch (evt->state) {
   case 0:
      evt->state++;
   case 1:
      evt->d.evtf405.pan += 0x100;
      xOfs = (evt->d.evtf405.yRot - GetCamRotY()) >> 4;
      xOfs += HI(evt->d.evtf405.pan);
      if (HI(evt->d.evtf405.pan) != 0) {
         HI(evt->d.evtf405.pan) = 0;
      }
      yOfs = (evt->d.evtf405.xRot - GetCamRotX()) >> 5;
      evt->d.evtf405.yRot = GetCamRotY();
      evt->d.evtf405.xRot = GetCamRotX();
   }

   evt->d.evtf405.xOffset += xOfs;
   evt->d.evtf405.yOffset += yOfs;
   evt->d.evtf405.xOffset &= 0x7f;
   evt->d.evtf405.yOffset &= 0x7f;
   spr = Evt_GetUnused();

   switch (gState.mapNum) {
   case 10:
   case 16:
   case 24:
   case 29:
      i = GFX_PANORAMA_DEEP_BLUE;
      break;
   case 8:
   case 13:
   case 15:
   case 30:
   case 34:
   case 39:
      i = GFX_PANORAMA_2;
      break;
   case 12:
   case 19:
   case 26:
   case 35:
   case 42:
      i = GFX_PANORAMA_3;
      break;
   case 14:
   case 18:
   case 28:
   case 33:
   case 36:
      i = GFX_PANORAMA_4;
      break;
   case 22:
   case 23:
   case 37:
      i = GFX_PANORAMA_RED;
      break;
   case 11:
   case 17:
   case 25:
   case 32:
   case 38:
   case 40:
   case 41:
   case 63:
      i = GFX_PANORAMA_6;
      break;
   case 0:
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 21:
   case 58:
   case 64:
   case 65:
   case 66:
   case 67:
   case 68:
      i = GFX_PANORAMA_8;
      break;
   default:
      i = GFX_PANORAMA_7;
      break;
   }

   spr->d.sprite.gfxIdx = i;

   for (i = 0; i < 4; i++) {
      for (iy = 0; iy < 3; iy++) {
         spr->d.sprite.x1 = i * 128 - evt->d.evtf405.xOffset;
         spr->d.sprite.y1 = iy * 128 + evt->d.evtf405.yOffset - 128;
         spr->d.sprite.x3 = spr->d.sprite.x1 + 128;
         spr->d.sprite.y3 = spr->d.sprite.y1 + 128;
         AddEvtPrim_Panorama(gGraphicsPtr->ot, spr);
      }
   }
}

void Evt_Execute(void) {
   EvtData *evt;
   s32 i;

   evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, evt++) {
      if (evt->functionIndex != EVTF_NULL) {
         (*gEvtFunctionPointers[evt->functionIndex])(evt);
      }
   }
}

void DecodeUnitSprites(void) {
   EvtData *evt = &gEvtData050_UnitSpritesDecoder;
   if (evt->functionIndex != EVTF_NULL) {
      (*gEvtFunctionPointers[EVTF_UNIT_SPRITES_DECODER])(evt);
   }
}

void Evtf001_Noop(EvtData *evt) { return; }

void Evt_ResetFromIdx10(void) {
   s32 i;
   for (i = 10; i < EVT_DATA_CT; i++) {
      gTempEvt = &gEvtDataArray[i];
      gTempEvt->functionIndex = EVTF_NULL;
   }
}

void Evt_ResetAll(void) {
   s32 i;
   for (i = 0; i < EVT_DATA_CT; i++) {
      gTempEvt = &gEvtDataArray[i];
      gTempEvt->functionIndex = EVTF_NULL;
   }
}

EvtData *Evt_GetUnused(void) {
   s32 i;
   EvtData *p;
   // TBD What is the significance of first 20 entries? reserved?
   p = &gEvtDataArray[20];
   for (i = 20; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

EvtData *Evt_GetFirstUnused(void) {
   s32 i;
   EvtData *p;
   p = &gEvtDataArray[0];
   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

EvtData *Evt_GetLastUnused(void) { return Evt_GetLastUnusedSkippingTail(0); }

EvtData *Evt_GetLastUnusedSkippingTail(s32 tailEntriesToSkip) {
   s32 i;
   EvtData *p;
   p = gEvtDataArray + EVT_DATA_LAST_IDX - tailEntriesToSkip;
   // Ensures scan doesn't enter first 20 entries (reserved?)
   for (i = 0; i < (EVT_DATA_CT - 20) - tailEntriesToSkip; i++, p--) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

s32 Evt_CountUnused(void) {
   s32 i, ct;
   EvtData *p;
   ct = 0;
   p = &gEvtDataArray[20];
   for (i = 20; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ct++;
      }
   }
   return ct;
}
