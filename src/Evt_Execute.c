#include "common.h"
#include "evt.h"
#include "graphics.h"

void Evt_Execute(void) {
   EvtData *evt;
   s32 i;

   evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt->functionIndex != EVTF_NULL) {
         (*gEvtFunctionPointers[evt->functionIndex])(evt);
      }
      evt++;
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
   /*  TBD What is the significance of first 20 entries? reserved? */
   p = &gEvtDataArray[20];
   for (i = 20; i < EVT_DATA_CT; i++) {
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
      p++;
   }
   return NULL;
}

EvtData *Evt_GetFirstUnused(void) {
   s32 i;
   EvtData *p;
   p = &gEvtDataArray[0];
   for (i = 0; i < EVT_DATA_CT; i++) {
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
      p++;
   }
   return NULL;
}

EvtData *Evt_GetLastUnused(void) { return Evt_GetLastUnusedSkippingTail(0); }

/* TBD: Bad output (maybe because offset > 0x8000; psyq-obj-parser bug?) */
/* Update: Well, the pile of hacks grows ever larger; added addend-fixup.py to convert-psyq-lnk.sh,
 * which scans the psyq-obj-parser dump for large addends and patches affected lui instructions;
 * will need to investigate the psyq-obj-parser src for a true fix. */
EvtData *Evt_GetLastUnusedSkippingTail(s32 tailEntriesToSkip) {
   s32 i;
   EvtData *p;
   p = gEvtDataArray + EVT_DATA_LAST_IDX - tailEntriesToSkip;
   // Ensures scan doesn't enter first 20 entries (reserved?)
   for (i = 0; i < (EVT_DATA_CT - 20) - tailEntriesToSkip; i++) {
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
      p--;
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
