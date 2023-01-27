#include "common.h"
#include "evt.h"
#include "battle.h"
#include "units.h"
#include "state.h"
#include "field.h"
#include "window.h"

void ClearBattlePortraits(void) {
   EvtData *p;
   s32 i;

   p = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++) {
      if (p->functionIndex == EVTF_BATTLE_PORTRAIT) {
         p->functionIndex = EVTF_NULL;
         p->d.evtf008.sprite->functionIndex = EVTF_NULL;
      }
      p++;
   }
}

#undef EVTF
#define EVTF 015
void Evtf015_TargetingAttack(EvtData *evt) {
   // UnitStatus *attacker, *target; //regs inconsistent
   UnitStatus *unit1, *unit2;
   s32 unitIdx;
   EvtData *newEvt;

   if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != 0) {
      gState.mapCursorOutOfRange = 0;
   } else {
      gState.mapCursorOutOfRange = 1;
   }

   switch (evt->state) {
   case 0:
      ClearGrid(1);
      unit2 = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx];
      if (unit2->class == CLASS_ARCHER) {
         PopulateRangedAttackGrid(HI(EVT.z), HI(EVT.x), unit2->attackRange, 0);
      } else {
         PopulateMeleeAttackGrid(HI(EVT.z), HI(EVT.x), 0, unit2->attackRange);
      }
      evt->state++;
      break;

   case 1:
      if (gSavedPadStateNewPresses & PAD_CIRCLE) {
         if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].team == TEAM_CHEST &&
             gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_UNSET) {

            ClearGrid(0);
            gClearSavedPadState = 1;
            gTargetX = gMapCursorX;
            gTargetZ = gMapCursorZ;
            gYellowTargetGridPtr[gMapCursorZ][gMapCursorX] = 1;
            CloseWindow(0x1e);

            unitIdx = gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx;
            unit2 = &gUnits[unitIdx];
            UpdateCompactUnitInfoWindow(unit2, unit2, 0);
            DisplayBasicWindow(0x1d);
            SlideWindowTo(0x1d, 104, 135);
            DisplayBasicWindow(0x1a);
            SlideWindowTo(0x1a, 10, 66);

            newEvt = Evt_GetLastUnused();
            newEvt->functionIndex = EVTF_BATTLE_PORTRAIT;
            newEvt->d.evtf008.windowId = 0x41;
            newEvt->d.evtf008.flipped = 0;
            newEvt->d.evtf008.portraitId = gUnitPortraitIds[unit2->unitId];

            DrawWindow(0x41, 400, 190, 64, 63, -114, 113, WBS_CROSSED, 0);
            DisplayBasicWindow(0x41);
            SlideWindowTo(0x41, 36, 113);

            evt->state = 4;
            return;
         }

         if (gSavedPadStateNewPresses & PAD_CIRCLE) {
            if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_INVALID &&
                gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_UNSET &&
                gMapUnitsPtr[gMapCursorZ][gMapCursorX].unitIdx != 0) {

               gTargetX = gMapCursorX;
               gTargetZ = gMapCursorZ;
               unit2 = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx];
               unit1 = &gUnits[gMapUnitsPtr[gMapCursorZ][gMapCursorX].unitIdx];

               if (gState.debug || unit2->team != unit1->team) {
                  ClearGrid(0);
                  gYellowTargetGridPtr[gTargetZ][gTargetX] = 1;
                  gClearSavedPadState = 1;
                  UpdateCompactUnitInfoWindow(unit2, unit1, 0);
                  if (unit2->team == TEAM_PLAYER) {
                     DisplayBasicWindow(0x1d);
                  } else {
                     DisplayCustomWindow(0x1d, 0, 1, 0, 0, 23);
                  }
                  CloseWindow(0x1e);

                  unit1 = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx];
                  unit2 = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].unitIdx];
                  UpdateCompactUnitInfoWindow(unit2, unit1, 1);

                  if (unit2->team == TEAM_PLAYER) {
                     DisplayBasicWindow(0x1e);
                  } else {
                     DisplayCustomWindow(0x1e, 0, 1, 0, 0, 23);
                  }
                  SlideWindowTo(0x1d, 104, 135);
                  SlideWindowTo(0x1e, 10, 66);

                  newEvt = Evt_GetLastUnused();
                  newEvt->functionIndex = 8;
                  newEvt->d.evtf008.windowId = 0x41;
                  newEvt->d.evtf008.flipped = 0;
                  newEvt->d.evtf008.portraitId = gUnitPortraitIds[unit1->unitId];

                  newEvt = Evt_GetLastUnused();
                  newEvt->functionIndex = 8;
                  newEvt->d.evtf008.windowId = 0x42;
                  newEvt->d.evtf008.flipped = 1;
                  newEvt->d.evtf008.portraitId = gUnitPortraitIds[unit2->unitId];

                  DrawWindow(0x41, 400, 190, 64, 63, -114, 113, WBS_CROSSED, 0);
                  DrawWindow(0x42, 400, 290, 64, 63, 371, 63, WBS_CROSSED, 0);
                  DisplayBasicWindow(0x41);
                  DisplayCustomWindowWithSetChoice(0x42, 0, 1, 0, 0, 23, 0);
                  SlideWindowTo(0x41, 36, 113);
                  SlideWindowTo(0x42, 221, 63);
                  evt->state += 2;
               }
               return;
            }
         }
      } // PAD_CIRCLE (outer)

      if (gSavedPadStateNewPresses & PAD_X) {
         evt->state++;
         gSignal2 = 1;
      }
      break;

   case 2:
      ClearGrid(0);
      ClearGrid(1);
      evt->functionIndex = EVTF_NULL;
      break;

   case 3:
      if (gPadStateNewPresses & PAD_X) {
         CloseWindow(0x1d);
         CloseWindow(0x1e);
         CloseWindow(0x41);
         CloseWindow(0x42);
         ClearBattlePortraits();

         unitIdx = gMapUnitsPtr[gTargetZ][gTargetX].unitIdx;
         unit1 = &gUnits[unitIdx];
         UpdateCompactUnitInfoWindow(unit1, unit1, 1);
         if (unit1->team == TEAM_PLAYER) {
            DisplayBasicWindow(0x1e);
         } else {
            DisplayCustomWindow(0x1e, 0, 1, 0, 0, 23);
         }
         gClearSavedPadState = 0;
         evt->state -= 3;
      } else if (gPadStateNewPresses & PAD_CIRCLE) {
         CloseWindow(0x1d);
         CloseWindow(0x1e);
         CloseWindow(0x41);
         CloseWindow(0x42);
         ClearBattlePortraits();
         gSignal2 = 2;
         ClearGrid(1);
         ClearGrid(0);

         newEvt = Evt_GetLastUnused();
         newEvt->functionIndex = EVTF_UNIT_ATTACKING;
         newEvt->d.evtf021.x = EVT.x;
         newEvt->d.evtf021.z = EVT.z;

         evt->state = 99;
      }
      break;

   case 4:
      DrawText(40, 62, 25, 2, 0, "Treasure Chest");
      evt->state++;

   // fallthrough
   case 5:

      if (gPadStateNewPresses & PAD_CIRCLE) {
         CloseWindow(0x1d);
         CloseWindow(0x1a);
         CloseWindow(0x41);
         ClearBattlePortraits();
         gSignal2 = 2;
         ClearGrid(1);

         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_OPENING_CHEST;
         /// TODO: Replace with d.evtf567 once defined:
         newEvt->d.sprite.x1 = EVT.x;
         newEvt->d.sprite.z1 = EVT.z;

         evt->state = 99;
      }
      if (gPadStateNewPresses & PAD_X) {
         CloseWindow(0x1d);
         CloseWindow(0x1a);
         CloseWindow(0x41);
         ClearBattlePortraits();
         gClearSavedPadState = 0;
         evt->state = 0;
      }
      break;

   case 99:
      if (gSignal2 == 99) {
         evt->functionIndex = EVTF_NULL;
         return;
      }
      break;
   }
}

EvtData *s_unitSprite_80123190;
u8 s_gainedLevel_80123194;
u8 s_countering_80123198;
void StartUnitSpritesDecoder(u8);

#undef EVTF
#define EVTF 021
void Evtf021_UnitAttacking(EvtData *evt) {
   EvtData *evt1, *evt2;
   EvtData *newEvt;
   // register EvtData *evt asm("s3") = evt_;
   UnitStatus *attacker;
   UnitStatus *defender;
   s16 opposite;
   s16 value1, value2;
   u8 attackResult;
   u8 x, z, team;

   // TODO: This resolves the s3/s4 swap, but should look for a real fix
   do {
      evt1 = EVT.attackerSprite;
      evt2 = EVT.defenderSprite;
      attacker = EVT.attacker;
      defender = EVT.defender;
   } while (0);

   switch (evt->state) {
   case 0:
      ClearGrid(0);
      EVT.mapSizeX = gMapSizeX;
      EVT.mapSizeZ = gMapSizeZ;

      EVT.attacker = attacker = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx];
      EVT.attackerSprite = attacker->evtSprite;

      EVT.defender = defender = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].unitIdx];
      EVT.defenderSprite = defender->evtSprite;
      //@be8
      evt1 = Evt_GetUnused();
      evt1->functionIndex = EVTF_CAMERA_TBD_017;
      evt1->d.evtf017.sprite = evt2 = attacker->evtSprite;

      if (attacker->class != CLASS_ARCHER) {
         // ?: LO byte of camSavedX doubles as a caller-set arg for specifying melee/ranged
         // LO(evt1->d.evtf017.camSavedX) = 1;
         evt1->d.bytes[0x18] = 1;
      }
      if (gTargetX > HI(evt2->d.sprite.x1)) {
         evt2->d.sprite.direction = ANGLE_WEST;
      }
      if (gTargetX < HI(evt2->d.sprite.x1)) {
         evt2->d.sprite.direction = ANGLE_EAST;
      }
      if (gTargetZ > HI(evt2->d.sprite.z1)) {
         evt2->d.sprite.direction = ANGLE_SOUTH;
      }
      if (gTargetZ < HI(evt2->d.sprite.z1)) {
         evt2->d.sprite.direction = ANGLE_NORTH;
      }
      attacker->direction = evt2->d.sprite.direction;

      if (gIsEnemyTurn) {
         UpdateCompactUnitInfoWindow(attacker, defender, 0);
         if (attacker->team == TEAM_PLAYER) {
            DisplayBasicWindow(0x1d);
         } else {
            DisplayCustomWindow(0x1d, 0, 1, 0, 0, 23);
         }
         UpdateCompactUnitInfoWindow(defender, attacker, 1);
         if (defender->team == TEAM_PLAYER) {
            DisplayBasicWindow(0x1e);
         } else {
            DisplayCustomWindow(0x1e, 0, 1, 0, 0, 23);
         }
      }

      EVT.timer = 6;
      evt->state++;
      return;

   case 1:
      if (--EVT.timer == 0) {
         z = HI(evt1->d.sprite.z1);
         x = HI(evt1->d.sprite.x1);
         team = attacker->team;
         DisplaySupporterBonus(gTargetZ - 1, gTargetX, z, x, team);
         DisplaySupporterBonus(gTargetZ + 1, gTargetX, z, x, team);
         DisplaySupporterBonus(gTargetZ, gTargetX - 1, z, x, team);
         DisplaySupporterBonus(gTargetZ, gTargetX + 1, z, x, team);

         if (gMapSizeX > 16) {
            gMapSizeX = 16;
         }
         if (gMapSizeZ > 16) {
            gMapSizeZ = 16;
         }
      }
      if (gSignal5 == 1) {
         if (gIsEnemyTurn) {
            EVT.timer = 10;
            evt->state++;
         } else {
            EVT.timer = 1;
            evt->state += 2;
         }
      }
      return;

   case 2:
      if (--EVT.timer == 0) {
         CloseWindow(0x1d);
         CloseWindow(0x1e);
         EVT.timer = 10;
         evt->state++;
      }
      return;

   case 3:
      if (--EVT.timer == 0) {
         if (attacker->class == CLASS_ARCHER) {
            gTileStateGridPtr[HI(evt1->d.sprite.z1)][HI(evt1->d.sprite.x1)].action = TA_RANGED_ATK;
         } else {
            gTileStateGridPtr[HI(evt1->d.sprite.z1)][HI(evt1->d.sprite.x1)].action = TA_MELEE_ATK;
         }
         gSignal3 = 0;
         gSignal4 = 0;
         evt->state++;
      }
      return;

   case 4:
      if (gSignal3 != 1) {
         return;
      }

      opposite = evt1->d.sprite.direction + ANGLE_180_DEGREES;
      if (opposite >= 0x1000) {
         opposite = evt1->d.sprite.direction - ANGLE_180_DEGREES;
      }
      evt2->d.sprite.direction = opposite;

      value1 = defender->hpFrac / 100;
      if (value1 == 0) {
         value1 = 1;
      }
      value2 = CalculateAttackDamage(attacker, defender);
      defender->hpFrac -= value2;

      value2 = defender->hpFrac / 100;
      if (value2 == 0 && defender->hpFrac != 0) {
         value2 = 1;
      }
      SaveRestoreHpPercents(&value1, &value2, 0);

      value1 = defender->hp;
      CalculateUnitStats(defender);
      value2 = value1 - defender->hp;
      SaveRestoreDamage(&value2, 0);
      SaveRestoreHp(&defender->maxHp, &value1, &defender->hp, 0);

      if (value2 == 0) {
         attackResult = ATK_RES_BLOCKED;
      } else {
         attackResult = ATK_RES_HIT;
      }
      if (defender->hp == 0) {
         attackResult = ATK_RES_DEFEATED;
      }

      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_DISPLAY_DAMAGE;
      /// TODO: Replace with d.evtf032 once defined:
      HI(newEvt->d.sprite.x1) = gTargetX;
      HI(newEvt->d.sprite.z1) = gTargetZ;

      newEvt = Evt_GetUnused();
      newEvt->functionIndex = EVTF_BOUNCE_ZOOM;
      gSignal3 = 0;

      if (attackResult == ATK_RES_HIT) {
         gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_HIT;
         evt->state += 1;
      } else if (attackResult == ATK_RES_BLOCKED) {
         gTileStateGridPtr[gTargetZ][gTargetX].action = TA_BLOCK;
         newEvt->d.evtf024.soft = 1;
         evt->state += 2;
      } else {
         // ATK_RES_DEFEATED:
         if (!HasDefeatSpeech(defender)) {
            gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT;
         } else {
            gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT_MSG;
         }
         evt->state += 3;
      }

      evt->state2 = 0;
      return;

   case 5:
      if (gSignal3 == 1 && gSignal4 == 1) {
         evt->state = 8;
         evt->state2 = 0;
      }
      return;

   case 6:
      if (gSignal3 == 1 && gSignal4 == 1) {
         evt->state = 8;
         evt->state2 = 0;
      }
      return;

   case 8:

      switch (evt->state2) {

      case 0:
         s_countering_80123198 = 0;
         if (defender->class == CLASS_ARCHER) {
            PopulateRangedAttackGrid(HI(evt2->d.sprite.z1), HI(evt2->d.sprite.x1),
                                     defender->attackRange, 0);
         } else {
            PopulateMeleeAttackGrid(HI(evt2->d.sprite.z1), HI(evt2->d.sprite.x1), 0,
                                    defender->attackRange);
         }
         if (gRedAttackGridPtr[HI(evt1->d.sprite.z1)][HI(evt1->d.sprite.x1)] != PATH_STEP_UNSET &&
             !defender->paralyzed) {

            s_countering_80123198 = 1;
            gTargetX = HI(evt1->d.sprite.x1);
            gTargetZ = HI(evt1->d.sprite.z1);

            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_CAMERA_TBD_026;
            newEvt->d.evtf026.sprite = evt2;
            newEvt->d.evtf026.type = 3;
            if (attacker->class != CLASS_ARCHER) {
               newEvt->d.evtf026.todo_x44 = 1;
            }

            EVT.countering = 1;
            EVT.timer = 20;
            evt->state2++;
         } else {
            evt->state = 99;
            evt->state2 = 0;
         }
         ClearGrid(0);
         return;

      case 1:
         if (EVT.timer == 15) {
            z = HI(evt2->d.sprite.z1);
            x = HI(evt2->d.sprite.x1);
            team = defender->team;
            DisplaySupporterBonus(gTargetZ - 1, gTargetX, z, x, team);
            DisplaySupporterBonus(gTargetZ + 1, gTargetX, z, x, team);
            DisplaySupporterBonus(gTargetZ, gTargetX - 1, z, x, team);
            DisplaySupporterBonus(gTargetZ, gTargetX + 1, z, x, team);
         }
         if (--EVT.timer == 0) {
            StartUnitSpritesDecoder(evt2->d.sprite.stripIdx);
            EVT.timer = 5;
            evt->state2++;
         }
         return;

      case 2:
         if (--EVT.timer != 0) {
            return;
         }
         if (!gDecodingSprites) {
            if (defender->class == CLASS_ARCHER) {
               gTileStateGridPtr[HI(evt2->d.sprite.z1)][HI(evt2->d.sprite.x1)].action =
                   TA_RANGED_ATK;
            } else {
               gTileStateGridPtr[HI(evt2->d.sprite.z1)][HI(evt2->d.sprite.x1)].action =
                   TA_MELEE_ATK;
            }
            gSignal3 = 0;
            evt->state2++;
         } else {
            EVT.timer = 1;
         }
         return;

      case 3:
         if (gSignal3 != 1) {
            return;
         }

         value1 = attacker->hpFrac / 100;
         if (value1 == 0) {
            value1 = 1;
         }
         value2 = CalculateAttackDamage(defender, attacker);
         attacker->hpFrac -= value2;

         value2 = attacker->hpFrac / 100;
         if (value2 == 0 && attacker->hpFrac != 0) {
            value2 = 1;
         }
         SaveRestoreHpPercents(&value1, &value2, 0);

         value1 = attacker->hp;
         CalculateUnitStats(attacker);
         value2 = value1 - attacker->hp;
         SaveRestoreDamage(&value2, 0);
         SaveRestoreHp(&attacker->maxHp, &value1, &attacker->hp, 0);

         if (value2 == 0) {
            attackResult = ATK_RES_BLOCKED;
         } else {
            attackResult = ATK_RES_HIT;
         }
         if (attacker->hp == 0) {
            attackResult = ATK_RES_DEFEATED;
         }

         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_DISPLAY_DAMAGE;
         /// TODO: Replace with d.evtf032 once defined:
         HI(newEvt->d.sprite.x1) = gTargetX;
         HI(newEvt->d.sprite.z1) = gTargetZ;

         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_BOUNCE_ZOOM;
         gSignal3 = 0;

         if (attackResult == ATK_RES_HIT) {
            gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_HIT;
            evt->state += 1;
         } else if (attackResult == ATK_RES_BLOCKED) {
            gTileStateGridPtr[gTargetZ][gTargetX].action = TA_BLOCK;
            newEvt->d.evtf024.soft = 1;
            evt->state += 2;
         } else {
            // ATK_RES_DEFEATED:
            if (!HasDefeatSpeech(attacker)) {
               gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT;
            } else {
               gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT_MSG;
            }
            evt->state += 3;
         }
         return;
      } // switch(evt->state2) (via state:8)

      return;

   case 7:
   case 9:
   case 10:
   case 11:
      if (gSignal3 == 1 && gSignal4 == 1) {
         evt->state2 = 0;
         evt->state = 12;
      }
      return;
   case 12:

      switch (evt->state2) {
      case 0:
         EVT.timer = 15;
         evt->state2++;
      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            evt->state = 99;
            evt->state2 = 0;
         }
      }
      return;

   case 99:

      switch (evt->state2) {
      case 0:
         if (gSignal3 != 1) {
            return;
         }
         if (gMapUnitsPtr[HI(evt1->d.sprite.z1)][HI(evt1->d.sprite.x1)].unitIdx != 0) {
            gTileStateGridPtr[HI(evt1->d.sprite.z1)][HI(evt1->d.sprite.x1)].action = TA_XB;
         }
         if (gMapUnitsPtr[HI(evt2->d.sprite.z1)][HI(evt2->d.sprite.x1)].unitIdx != 0) {
            gTileStateGridPtr[HI(evt2->d.sprite.z1)][HI(evt2->d.sprite.x1)].action = TA_XB;
         }
         gSignal5 = 99;
         gMapSizeX = EVT.mapSizeX;
         gMapSizeZ = EVT.mapSizeZ;
         evt->state2++;
         return;
      case 1:
         if (gSignal5 != 100) {
            return;
         }
         EVT.timer = 1;
         s_gainedLevel_80123194 = 0;

         if (s_countering_80123198 && (attacker->team != TEAM_PLAYER) &&
             (defender->team == TEAM_PLAYER)) {
            // Allow player unit to gain EXP when countering
            attacker = defender;
         }
         if (attacker->hpFrac && attacker->team == TEAM_PLAYER) {
            u32 prevExp;
            s32 prevLevel;
            prevExp = attacker->exp;
            SyncGainedHp(attacker, 0);
            prevLevel = attacker->level;
            if (attacker->level < 50) {
               BigIntAdd(attacker->experience, gState.experience);
            }
            CalculateUnitStats(attacker);
            if (prevLevel != attacker->level) {
               DetermineMaxMpAndStatVariance(attacker);
               CalculateUnitStats(attacker);
               s_gainedLevel_80123194++;
               s_unitSprite_80123190 = attacker->evtSprite;
               PopulateUnitSpellList(attacker);
               StartUnitSpritesDecoder(s_unitSprite_80123190->d.sprite.stripIdx);
            }
            SyncGainedHp(attacker, 1);
            prevExp = attacker->exp - prevExp;
            if (prevExp != 0) {
               ShowExpDialog(prevExp, 0x3c);
               EVT.timer = 50;
            }
         }
         evt->state2++;
         return;
      case 2:
         if ((--EVT.timer == 0) || (gPadStateNewPresses & PAD_CIRCLE)) {
            CloseWindow(0x3c);
            if (!s_gainedLevel_80123194) {
               evt->state2 = 99;
               return;
            }
            evt->state2++;
         }
         return;
      case 3:
         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_LEVEL_UP;
         newEvt->d.evtf571.sprite = s_unitSprite_80123190;
         evt->state2++;
         return;
      case 4:
         if (gSignal5 == 100) {
            evt->state2 = 99;
         }
         return;
      case 99:
         ClearGrid(0);
         gSignal2 = 99;
         evt->functionIndex = EVTF_NULL;
         return;
      } // switch (evt->state2) (via state:99)
   }
}
