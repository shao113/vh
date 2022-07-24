#include "common.h"
#include "units.h"
#include "evt.h"
#include "audio.h"
#include "field.h"
#include "battle.h"

UnitStatus *FindUnitByNameIdx(s16 nameIdx) {
   UnitStatus *p;
   s32 i;

   for (i = 1; i < UNIT_CT; i++) {
      p = &gUnits[i];

      if ((p->idx != 0) && (p->name == nameIdx)) {
         return p;
      }
   }

   return NULL;
}

s32 CountUnitsOfType(s16 unitType) {
   s32 i, ct;
   UnitStatus *p;

   ct = 0;

   for (i = 1; i < UNIT_CT; i++) {
      p = &gUnits[i];
      if ((p->idx != 0) && (p->unitType == unitType)) {
         ct++;
      }
   }

   return ct;
}

s32 CountUnitsOfTeam(s16 team) {
   s32 i, ct;
   UnitStatus *p;

   ct = 0;

   for (i = 1; i < UNIT_CT; i++) {
      p = &gUnits[i];
      if ((p->idx != 0) && (p->team == team)) {
         ct++;
      }
   }

   return ct;
}

void Evtf438_EvaluateBattle08(EvtData *evt) {
   extern u8 D_80140859;

   // TBD gLightRotation.vy ?
   gLightRotation_vy = gLightRotation_vy + 0x10;

   if (gMapState.needEval) {
      gMapState.needEval = 0;
   }

   if (gMapState.signal) {
      gMapState.signal = 0;
   }

   switch (evt->state) {
   case 0:
      if ((D_80140859 != 0) || ((gPadStateNewPresses & PADstart) != 0)) {
         FadeOutScreen(2, 6);
         PerformAudioCommand(0x21);
         evt->d.evtf438.delay = 75;
         evt->state++;
      }
      break;
   case 1:
      if (--evt->d.evtf438.delay == 0) {
         PerformAudioCommand(6);
         gIsEnemyTurn = 0;
         gState.primary = 0x33;
         gState.secondary = 0;
         gState.field2_0x8 = 0;
         gState.field3_0xc = 0;
         gState.field6_0x18 = 1;
      }
      break;
   }
}

void Evtf426_EvaluateBattle10(EvtData *evt) {
   if (gMapState.needEval) {
      gMapState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_ZOOT)) {
         gMapState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gMapState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gMapState.signal != 0) {
      gMapState.signal = 0;
   }
}

/*
#define CHECK_SEARCH_POS(z, x) ((((u32 *)&gMapState)[7] & 0xffff0000) == ((z) << 24 | (x) << 16))
#define CHECK_POS(sx, sz, dx, dz) (((sx) == (dx)) && ((sz) == (dz)))

void Evtf427_EvaluateBattle11(EvtData *evt) {
   UnitStatus *p;
   s32 i, arrived;
   u8 x, z;
   u8 eight, nine;
   EvtData_Sprite *sprite;

   eight = 8;
   nine = 9;
   arrived = 1;

   if (gMapState.needEval != 0) {
      gMapState.needEval = 0;

      for (i = 1; i < UNIT_CT; i++) {
         p = &gUnits[i];

         if ((p->idx != 0) && (p->team == TEAM_PLAYER)) {
            sprite = &p->evtSprite->d.sprite;
            z = (s8)sprite->z1;
            x = (s8)sprite->x1;

            if (!((x == 3) && (z == 7 || z == eight || z == nine || z == 10)) &&
                !((x == 4) && (z == eight || z == nine))) {
               arrived = 0;
               break;
            }
         }
      }

      if (arrived) {
         gMapState.battleEval = BATTLE_EVAL_VICTORY;
      }

      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gMapState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }

   if (((gMapState.signal == 1) && (gMapState.signal = 0, gMapState.field2_0x5 == 0)) &&
       CHECK_SEARCH_POS(1, 11)) {
      gMapState.field2_0x5 = 1;
      gMapState.signal = 2;
   }
}

#undef CHECK_POS
#undef CHECK_SEARCH_POS
*/
