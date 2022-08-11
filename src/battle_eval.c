#include "common.h"
#include "units.h"
#include "evt.h"
#include "audio.h"
#include "field.h"
#include "battle.h"
#include "state.h"

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
   // TBD gLightRotation.vy ?
   gLightRotation_vy += 0x10;

   if (gState.needEval) {
      gState.needEval = 0;
   }

   if (gState.signal) {
      gState.signal = 0;
   }

   switch (evt->state) {
   case 0:
      if ((gState.D_80140859 != 0) || ((gPadStateNewPresses & PADstart) != 0)) {
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
         gState.state3 = 0;
         gState.state4 = 0;
         gState.state7 = 1;
      }
      break;
   }
}

void Evtf426_EvaluateBattle10(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_ZOOT)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf427_EvaluateBattle11(EvtData *evt) {
   UnitStatus *p;
   s32 i, arrived;
   u8 x, z;
   EvtData_Sprite *sprite;

   if (gState.needEval != 0) {
      gState.needEval = 0;
      arrived = 1;
      for (i = 1; i < UNIT_CT; i++) {
         p = &gUnits[i];
         if ((p->idx != 0) && (p->team == TEAM_PLAYER)) {
            sprite = &p->evtSprite->d.sprite;
            z = TO_TILE(sprite->z1);
            x = TO_TILE(sprite->x1);
            if (x == 3) {
               if (z == 7)
                  continue;
               if (z == 8)
                  continue;
               if (z == 9)
                  continue;
               if (z == 10)
                  continue;
            }
            if (x == 4) {
               if (z == 8)
                  continue;
               if (z == 9)
                  continue;
            }
            arrived = 0;
            break;
         }
      }
      if (arrived) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 11 && gState.searchZ == 1) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf428_EvaluateBattle12(EvtData *evt) {
   gLightRotation_vy += 0x10;
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf429_EvaluateBattle13(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
   }
}

void Evtf430_EvaluateBattle14(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfType(UNIT_TYPE_DEATH_ANT) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf431_EvaluateBattle15(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_HASSAN)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf432_EvaluateBattle16(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfType(UNIT_TYPE_EVILSTATUE) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (CountUnitsOfType(UNIT_TYPE_VILLAGER) == 0) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf433_EvaluateBattle17(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (!FindUnitByNameIdx(UNIT_DOLAN)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 10 && gState.searchZ == 16) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf434_EvaluateStandardBattle(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf435_EvaluateBattle19(EvtData *evt) {
   UnitStatus *p;
   s32 i, arrived;
   u8 x, z;
   EvtData_Sprite *sprite;

   if (gState.needEval != 0) {
      gState.needEval = 0;
      arrived = 1;
      for (i = 1; i < UNIT_CT; i++) {
         p = &gUnits[i];
         if ((p->idx != 0) && (p->team == TEAM_PLAYER)) {
            sprite = &p->evtSprite->d.sprite;
            z = TO_TILE(sprite->z1);
            x = TO_TILE(sprite->x1);
            if (x == 2) {
               if (z == 7)
                  continue;
               if (z == 8)
                  continue;
            }
            if (x == 3) {
               if (z == 6)
                  continue;
               if (z == 7)
                  continue;
               if (z == 8)
                  continue;
               if (z == 9)
                  continue;
            }
            if (x == 4) {
               if (z == 6)
                  continue;
               if (z == 7)
                  continue;
               if (z == 8)
                  continue;
               if (z == 9)
                  continue;
            }
            arrived = 0;
            break;
         }
      }
      if (arrived) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.searchX == 19 && gState.searchZ == 3) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
      if (gState.searchX == 19 && gState.searchZ == 12) {
         gState.mapState.n.field_0x0 = 2;
         gState.signal = 2;
      }
   }
}

void Evtf436_EvaluateBattle20(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_MAGNUS)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf437_EvaluateBattle21(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (gState.turn > 6) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf439_EvaluateBattle23(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (gState.turn > 5) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (CountUnitsOfType(UNIT_TYPE_MAGE_TOWER) == 0) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf442_EvaluateBattle26(EvtData *evt) {
   UnitStatus *p;
   s32 i;
   s8 x;
   EvtData_Sprite *sprite;

   if (gState.needEval != 0) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }

      for (i = 1; i < UNIT_CT; i++) {
         p = &gUnits[i];
         if ((p->idx != 0) && (p->team == TEAM_ENEMY)) {
            sprite = &p->evtSprite->d.sprite;
            x = TO_TILE(sprite->x1);
            if (x == 0 || x == 47) {
               gState.battleEval = BATTLE_EVAL_DEFEAT;
            }
         }
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 35 && gState.searchZ == 8) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf443_EvaluateBattle27(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfTeam(TEAM_ENEMY) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_CLINT)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 22 && gState.searchZ == 14) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.n.field_0x1 == 0 && gState.searchX == 9 && gState.searchZ == 9) {
         gState.mapState.n.field_0x1 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.n.field_0x2 == 0 && gState.searchX == 15 && gState.searchZ == 12) {
         gState.mapState.n.field_0x2 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.n.field_0x3 == 0 && gState.searchX == 1 && gState.searchZ == 1) {
         gState.mapState.n.field_0x3 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf444_EvaluateBattle28(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_DUMAS)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_CLINT)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 2 && gState.searchZ == 19) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf445_EvaluateBattle29(EvtData *evt) {
   UnitStatus *p;
   s32 i, arrived;
   u8 x, z;
   EvtData_Sprite *sprite;

   if (gState.needEval != 0) {
      gState.needEval = 0;
      arrived = 1;
      for (i = 1; i < UNIT_CT; i++) {
         p = &gUnits[i];
         if ((p->idx != 0) && (p->team == TEAM_PLAYER)) {
            sprite = &p->evtSprite->d.sprite;
            z = TO_TILE(sprite->z1);
            x = TO_TILE(sprite->x1);
            if (z == 27 && x > 5 && x < 14)
               continue;
            if (z == 26 && x > 5 && x < 14)
               continue;
            if (x == 9 && z == 25)
               continue;
            if (x == 10 && z == 25)
               continue;
            arrived = 0;
            break;
         }
      }
      if (arrived) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 18 && gState.searchZ == 12) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf552_EvaluateBattle32(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_DALLAS)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf553_EvaluateBattle33(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfType(UNIT_TYPE_DEATH_DEV_) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (!FindUnitByNameIdx(UNIT_KIRA_2)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (gState.turn > 8) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf555_EvaluateBattle35(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_KURTZ)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 18 && gState.searchZ == 8) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf557_EvaluateBattle37(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (CountUnitsOfType(UNIT_TYPE_SALAMANDER) == 0) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf558_EvaluateBattle38(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_SABINA)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal == 1) {
      gState.signal = 0;
      if (gState.mapState.n.field_0x0 == 0 && gState.searchX == 21 && gState.searchZ == 4) {
         gState.mapState.n.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf559_EvaluateBattle39(EvtData *evt) {
   s32 unitIdx;
   if (gState.needEval) {
      gState.needEval = 0;
      unitIdx = gMapUnitsPtr[6][3].unitIdx;
      if (unitIdx != 0 && gUnits[unitIdx].name == UNIT_LEENA) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
      if (!FindUnitByNameIdx(UNIT_LEENA)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf560_EvaluateBattle40(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_KANE)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf562_EvaluateBattle42(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_XENO)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}

void Evtf563_EvaluateBattle43(EvtData *evt) {
   if (gState.needEval) {
      gState.needEval = 0;
      if (!FindUnitByNameIdx(UNIT_DOLF)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
      if (!FindUnitByNameIdx(UNIT_ASH)) {
         gState.battleEval = BATTLE_EVAL_DEFEAT;
      }
   }
   if (gState.signal != 0) {
      gState.signal = 0;
   }
}
