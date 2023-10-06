#include "common.h"
#include "evt.h"
#include "battle.h"
#include "units.h"
#include "state.h"
#include "field.h"
#include "window.h"
#include "audio.h"
#include "graphics.h"

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
      unit2 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
      if (unit2->class == CLASS_ARCHER) {
         PopulateRangedAttackGrid(evt->z1.s.hi, evt->x1.s.hi, unit2->attackRange, 0);
      } else {
         PopulateMeleeAttackGrid(evt->z1.s.hi, evt->x1.s.hi, 0, unit2->attackRange);
      }
      evt->state++;
      break;

   case 1:
      if (gSavedPadStateNewPresses & PAD_CIRCLE) {
         if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.team == TEAM_CHEST &&
             gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_UNSET) {

            ClearGrid(0);
            gClearSavedPadState = 1;
            gTargetX = gMapCursorX;
            gTargetZ = gMapCursorZ;
            gYellowTargetGridPtr[gMapCursorZ][gMapCursorX] = 1;
            CloseWindow(0x1e);

            unitIdx = OBJ_MAP_UNIT(evt).s.unitIdx;
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
                gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx != 0) {

               gTargetX = gMapCursorX;
               gTargetZ = gMapCursorZ;
               unit2 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
               unit1 = &gUnits[gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx];

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

                  unit1 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
                  unit2 = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx];
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

         unitIdx = gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx;
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
         newEvt->x1.n = evt->x1.n;
         newEvt->z1.n = evt->z1.n;

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
         newEvt->x1.n = evt->x1.n;
         newEvt->z1.n = evt->z1.n;

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

#undef EVTF
#define EVTF 021
void Evtf021_UnitAttacking(EvtData *evt) {
   EvtData *evt1, *evt2;
   EvtData *newEvt;
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

      EVT.attacker = attacker = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
      EVT.attackerSprite = attacker->sprite;

      EVT.defender = defender = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx];
      EVT.defenderSprite = defender->sprite;
      //@be8
      evt1 = Evt_GetUnused();
      evt1->functionIndex = EVTF_CAMERA_TBD_017;
      evt1->d.evtf017.sprite = evt2 = attacker->sprite;

      if (attacker->class != CLASS_ARCHER) {
         // ?: LO byte of camSavedX doubles as a caller-set arg for specifying melee/ranged
         // LO(evt1->d.evtf017.camSavedX) = 1;
         evt1->d.bytes[4] = 1;
      }
      if (gTargetX > evt2->x1.s.hi) {
         evt2->d.sprite.direction = ANGLE_WEST;
      }
      if (gTargetX < evt2->x1.s.hi) {
         evt2->d.sprite.direction = ANGLE_EAST;
      }
      if (gTargetZ > evt2->z1.s.hi) {
         evt2->d.sprite.direction = ANGLE_SOUTH;
      }
      if (gTargetZ < evt2->z1.s.hi) {
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
         z = evt1->z1.s.hi;
         x = evt1->x1.s.hi;
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
            OBJ_TILE_STATE(evt1).action = TA_RANGED_ATK;
         } else {
            OBJ_TILE_STATE(evt1).action = TA_MELEE_ATK;
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

      opposite = evt1->d.sprite.direction + DEG(180);
      if (opposite >= 0x1000) {
         opposite = evt1->d.sprite.direction - DEG(180);
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
      newEvt->x1.s.hi = gTargetX;
      newEvt->z1.s.hi = gTargetZ;

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
            PopulateRangedAttackGrid(evt2->z1.s.hi, evt2->x1.s.hi, defender->attackRange, 0);
         } else {
            PopulateMeleeAttackGrid(evt2->z1.s.hi, evt2->x1.s.hi, 0, defender->attackRange);
         }
         if (gRedAttackGridPtr[evt1->z1.s.hi][evt1->x1.s.hi] != PATH_STEP_UNSET &&
             !defender->paralyzed) {

            s_countering_80123198 = 1;
            gTargetX = evt1->x1.s.hi;
            gTargetZ = evt1->z1.s.hi;

            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_CAMERA_TBD_026;
            newEvt->d.evtf026.target = evt2;
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
            z = evt2->z1.s.hi;
            x = evt2->x1.s.hi;
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
               OBJ_TILE_STATE(evt2).action = TA_RANGED_ATK;
            } else {
               OBJ_TILE_STATE(evt2).action = TA_MELEE_ATK;
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
         newEvt->x1.s.hi = gTargetX;
         newEvt->z1.s.hi = gTargetZ;

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
         if (OBJ_MAP_UNIT(evt1).s.unitIdx != 0) {
            OBJ_TILE_STATE(evt1).action = TA_XB;
         }
         if (OBJ_MAP_UNIT(evt2).s.unitIdx != 0) {
            OBJ_TILE_STATE(evt2).action = TA_XB;
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
               s_unitSprite_80123190 = attacker->sprite;
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

#undef EVTF
#define EVTF 567
void Evtf567_OpeningChest(EvtData *evt) {
   EvtData *evt1, *evt2;
   UnitStatus *opener;
   s32 item;
   s32 unitIdx;

   evt1 = EVT.openerSprite;
   opener = EVT.opener;

   switch (evt->state) {
   case 0:
      unitIdx = OBJ_MAP_UNIT(evt).s.unitIdx;
      EVT.opener = opener = &gUnits[unitIdx];
      EVT.openerSprite = opener->sprite;
      if (opener->items[0] != ITEM_NULL && opener->items[1] != ITEM_NULL) {
         EVT.sendToDepot = 1;
      }
      gTileStateGridPtr[gTargetZ][gTargetX].action = TA_CHEST_2;
      evt->state++;
      break;

   case 1:
      item = gTileStateGridPtr[gTargetZ][gTargetX].cachedByte;
      if (item != ITEM_NULL) {
         if (opener->items[0] == ITEM_NULL) {
            opener->items[0] = item;
         } else if (opener->items[1] == ITEM_NULL) {
            opener->items[1] = item;
         } else {
            gState.depot[item]++;
         }
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(36));
      } else {
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(132));
      }

      evt1 = Evt_GetUnused();
      evt1->functionIndex = EVTF_CAMERA_TBD_017;
      evt1->d.evtf017.sprite = evt2 = opener->sprite;
      if (opener->class != CLASS_ARCHER) {
         evt1->d.bytes[4] = 1;
      }

      if (gTargetX > evt2->x1.s.hi) {
         evt2->d.sprite.direction = ANGLE_WEST;
      }
      if (gTargetX < evt2->x1.s.hi) {
         evt2->d.sprite.direction = ANGLE_EAST;
      }
      if (gTargetZ > evt2->z1.s.hi) {
         evt2->d.sprite.direction = ANGLE_SOUTH;
      }
      if (gTargetZ < evt2->z1.s.hi) {
         evt2->d.sprite.direction = ANGLE_NORTH;
      }
      opener->direction = evt2->d.sprite.direction;
      gTileStateGridPtr[gTargetZ][gTargetX].action = TA_CHEST_3;

      if (opener->direction == ANGLE_SOUTH) {
         gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 1;
      } else if (opener->direction == ANGLE_NORTH) {
         gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 0;
      } else {
         gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 2;
      }

      gTileStateGridPtr[gTargetZ][gTargetX].cachedShort =
          ((opener->direction + DEG(180)) & 0xfff) >> 10;

      evt->state++;
      break;

   case 2:
      if (gSignal5 == 1) {
         evt->state++;
      }
      break;

   case 3:
      if (opener->class == CLASS_ARCHER) {
         OBJ_TILE_STATE(evt1).action = TA_RANGED_ATK;
      } else {
         OBJ_TILE_STATE(evt1).action = TA_MELEE_ATK;
      }
      gSignal3 = 0;
      gSignal4 = 0;
      evt->state++;
      break;

   case 4:
      if (gSignal3 == 1) {
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_BOUNCE_ZOOM;
         gTileStateGridPtr[gTargetZ][gTargetX].action = TA_CHEST_1;
         gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = EVT.sendToDepot;
         gSignal3 = 0;
         evt->state++;
         evt->state2 = 0;
      }
      break;

   case 5:
      if (gTileStateGridPtr[gTargetZ][gTargetX].action == TA_NONE) {
         evt->state = 99;
         evt->state2 = 0;
      }
      break;

   case 99:

      switch (evt->state2) {
      case 0:
         gSignal5 = 99;
         evt->state2++;
         break;
      case 1:
         if (gSignal5 == 100) {
            gSignal2 = 99;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 007
void Evtf007_ApplyPoison(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.timer = 70;
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer == 0) {
         EVT.unit->poisoned = 1;
         evt->functionIndex = EVTF_NULL;
      }
   }
}

u8 *s_pTargetCoordsToGet_8012319c;
u8 *s_pTargetCoordsToSet_801231a0;
UnitStatus *s_casterUnit_801231a4;
UnitStatus *s_targetUnit_801231a8;
u8 s_gainedLevel_801231ac;
u8 s_targetCount_801231b0;

#undef EVTF
#define EVTF 028
void Evtf028_UnitCasting(EvtData *evt) {
   // evt->x3: targetX
   // evt->z3: targetZ
   s32 iz, ix;
   EvtData *evt_s1, *evt_a2;
   s16 xdist, zdist;
   s16 value1, value2;
   u8 unitIdx;
   u8 result;

   // TODO PerformAudioCommand definition?
   // TODO Any way around re-use shenanigans?

   switch (evt->state) {
   case 0:
      iz = 0;
      gWindowActiveIdx = 0;
      s_targetCount_801231b0 = 0;

      if (gSpellSounds[gCurrentSpell] != SPELL_NULL) {
         // Prepare XA for main sound (played by Evtf014_BattleUnit)
         PerformAudioCommand(gSpellSounds[gCurrentSpell] + 0x1000);
      }

      s_casterUnit_801231a4 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
      s_casterUnit_801231a4->mp -= gSpells[gCurrentSpell].mpCost;
      //@23ec
      do {
         gState.experience[iz] = 0;
         iz++;
      } while (iz < 8);
      // for (iz = 0; iz < 8; iz++)
      //   gState.experience[iz] = 0;

      EVT.mapSizeX = gMapSizeX;
      EVT.mapSizeZ = gMapSizeZ;
      s_pTargetCoordsToSet_801231a0 = gTargetCoords;
      evt->state2 = gSpells[gCurrentSpell].targeting;
      evt->state++;
   // fallthrough
   case 1:

      switch (evt->state2) {

      case 0:
      case 1:
      case 2:
      case 6:
         *s_pTargetCoordsToSet_801231a0++ = gTargetX;
         *s_pTargetCoordsToSet_801231a0++ = gTargetZ;
         evt->state++;
         break;

      case 3:
         if (gSpells[gCurrentSpell].area == SPELL_AREA_AOE) {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].fieldSize, 0);
         } else {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);
         }
         //@258c

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0 &&
                   gMapUnitsPtr[iz][ix].s.team != OBJ_MAP_UNIT(evt).s.team) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;

      case 4:
         if (gSpells[gCurrentSpell].area == SPELL_AREA_AOE) {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].fieldSize, 0);
         } else {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);
         }

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0 &&
                   gMapUnitsPtr[iz][ix].s.team == OBJ_MAP_UNIT(evt).s.team) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;

      case 5:
         if (gSpells[gCurrentSpell].area == SPELL_AREA_AOE) {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].fieldSize, 0);
         } else {
            PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);
         }

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;

      case 7:
         PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0 &&
                   gMapUnitsPtr[iz][ix].s.team != OBJ_MAP_UNIT(evt).s.team) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;

      case 8:
         PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0 &&
                   gMapUnitsPtr[iz][ix].s.team == OBJ_MAP_UNIT(evt).s.team) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;

      case 9:
         PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);

         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != PATH_STEP_UNSET &&
                   gMapUnitsPtr[iz][ix].s.unitIdx != 0) {

                  *s_pTargetCoordsToSet_801231a0++ = ix;
                  *s_pTargetCoordsToSet_801231a0++ = iz;
               }
            }
         }
         evt->state++;
         break;
      } // switch (evt->state2) (via state:1)

   // fallthrough
   case 2:
      ClearGrid(0);
      *s_pTargetCoordsToSet_801231a0++ = 0xff;
      evt->state++;

   // fallthrough
   case 3:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_017;
      evt_s1->d.bytes[4] = 0;

      evt_a2 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt_s1->d.evtf017.sprite = evt_a2;

      xdist = gTargetX - evt_a2->x1.s.hi;
      if (xdist < 0) {
         xdist = -xdist;
      }
      zdist = gTargetZ - evt_a2->z1.s.hi;
      if (zdist < 0) {
         zdist = -zdist;
      }
      if (xdist != 0 || zdist != 0) {
         if (xdist >= zdist) {
            if (gTargetX > evt_a2->x1.s.hi) {
               evt_a2->d.sprite.direction = ANGLE_WEST;
            }
            if (gTargetX < evt_a2->x1.s.hi) {
               evt_a2->d.sprite.direction = ANGLE_EAST;
            }
         } else {
            if (gTargetZ > evt_a2->z1.s.hi) {
               evt_a2->d.sprite.direction = ANGLE_SOUTH;
            }
            if (gTargetZ < evt_a2->z1.s.hi) {
               evt_a2->d.sprite.direction = ANGLE_NORTH;
            }
         }
         s_casterUnit_801231a4->direction = evt_a2->d.sprite.direction;
      }
      EVT.timer = 4;
      evt->state++;
      break;

   case 4:
      if (--EVT.timer == 0) {
         if (gMapSizeX > 16) {
            gMapSizeX = 16;
         }
         if (gMapSizeZ > 16) {
            gMapSizeZ = 16;
         }
      }
      if (gSignal5 != 1) {
         return;
      }
      evt_a2 = Evt_GetUnused();
      evt_a2->functionIndex = EVTF_ITEM_SPELL;
      evt_a2->x1.s.hi = evt->x1.s.hi;
      evt_a2->z1.s.hi = evt->z1.s.hi;

      DrawWindow(0x38, 0, 100, 144, 36, 172, 200, WBS_CROSSED, 0);
      DrawText(12, 111, 20, 0, 0, gSpellNames[gCurrentSpell]);

      unitIdx = OBJ_MAP_UNIT(evt).s.unitIdx;
      s_casterUnit_801231a4 = &gUnits[unitIdx];
      if (s_casterUnit_801231a4->team == TEAM_PLAYER) {
         DisplayBasicWindow(0x38);
      } else {
         DisplayCustomWindow(0x38, 0, 1, 0, 0, 23);
      }
      //@310c

      iz = gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT];
      if (iz == SPELL_EFFECT_DAMAGE || iz == SPELL_EFFECT_POISON || iz == SPELL_EFFECT_PARALYZE) {
         // Harmful spell - allies should "take cover"
         gState.shielding = 1;
         for (iz = 0; iz < gMapMaxZ; iz++) {
            for (ix = 0; ix < gMapMaxX; ix++) {
               u8 unitIdxAtLocation = gMapUnitsPtr[iz][ix].s.unitIdx;
               if (unitIdxAtLocation != 0 &&
                   gMapUnitsPtr[iz][ix].s.team == s_casterUnit_801231a4->team &&
                   unitIdx != unitIdxAtLocation) {
                  gTileStateGridPtr[iz][ix].action = TA_X1F;
               }
            }
         }
      }

      OBJ_TILE_STATE(evt).action = TA_CAST;
      gSignal3 = 0;
      evt->state++;
      break;

   case 5:
      if (gSignal3 != 2) {
         return;
      }
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_MAIN];
      evt_s1->x1.s.hi = evt->x1.s.hi;
      evt_s1->z1.s.hi = evt->z1.s.hi;
      gSignal3 = 0;
      evt->state++;
      break;

   case 6:
      if (gSignal3 != 1) {
         return;
      }
      evt->state++;
      break;

   case 7:
      s_pTargetCoordsToGet_8012319c = gTargetCoords;
      evt->state++;

   // fallthrough
   case 8:
      //@331c
      if (*s_pTargetCoordsToGet_8012319c != 0xff) {
         s32 bx, bz;
         bx = evt->x3.s.hi = *s_pTargetCoordsToGet_8012319c++;
         bz = evt->z3.s.hi = *s_pTargetCoordsToGet_8012319c++;

         evt_s1 = GetUnitSpriteAtPosition(bz, bx);
         evt_a2 = Evt_GetUnused();
         evt_a2->functionIndex = EVTF_CAMERA_TBD_026;
         evt_a2->d.evtf026.target = evt_s1;
         evt_a2->d.evtf026.type = 1;
         EVT.timer = 10;
         evt->state2 = 0;
         evt->state++;
         break;
      } else {
         evt->state = 99;
         return;
      }
      break;

   case 9:

      switch (evt->state2) {
      case 0:
         if (EVT.timer == 4 && s_targetCount_801231b0 != 0) {
            // Play XA for hit sound
            PerformAudioCommand(gSpellSounds2[gCurrentSpell]);
         }
         if (--EVT.timer != 0) {
            return;
         }
         s_casterUnit_801231a4 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
         s_targetUnit_801231a8 = &gUnits[OBJ_TARGET_MAP_UNIT(evt).s.unitIdx];

         switch (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT]) {
         case SPELL_EFFECT_DAMAGE:
            value1 = s_targetUnit_801231a8->hpFrac / 100;
            if (value1 == 0) {
               value1 = 1;
            }
            value2 = CalculateSpellPowerAndExp(s_casterUnit_801231a4, s_targetUnit_801231a8);
            s_targetUnit_801231a8->hpFrac -= value2;

            value2 = s_targetUnit_801231a8->hpFrac / 100;
            if (value2 == 0 && s_targetUnit_801231a8->hpFrac != 0) {
               value2 = 1;
            }
            //@35e8
            SaveRestoreHpPercents(&value1, &value2, 0);

            value1 = s_targetUnit_801231a8->hp;
            CalculateUnitStats(s_targetUnit_801231a8);
            value2 = value1 - s_targetUnit_801231a8->hp;
            SaveRestoreDamage(&value2, 0);
            SaveRestoreHp(&s_targetUnit_801231a8->maxHp, &value1, &s_targetUnit_801231a8->hp, 0);
            //@364c
            if (s_targetUnit_801231a8->hp == 0) {
               result = ATK_RES_DEFEATED;
            } else {
               result = ATK_RES_HIT;
            }
            evt_s1 = Evt_GetUnused();
            evt_s1->x1.s.hi = evt->x3.s.hi;
            evt_s1->z1.s.hi = evt->z3.s.hi;
            gSignal4 = 1;

            switch (result) {
            case ATK_RES_HIT:
               OBJ_TARGET_TILE_STATE(evt).action = TA_MAG_HIT;
               evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_TARGET];
               break;
            case ATK_RES_DEFEATED:
               if (!HasDefeatSpeech(s_targetUnit_801231a8)) {
                  evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_DEFEAT];
                  OBJ_TARGET_TILE_STATE(evt).action = TA_MAG_DEFEAT;
               } else {
                  OBJ_TARGET_TILE_STATE(evt).action = TA_MAG_DEFEAT_MSG;
                  evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_TARGET];
                  gSignal4 = 0;
               }
               break;
            }

            break;

         case SPELL_EFFECT_RESTORE_HP:
         case SPELL_EFFECT_RESTORE_HP_UNK:
            if (gCurrentSpell == SPELL_LIFE_ORB) {
               s_targetUnit_801231a8->mp = s_targetUnit_801231a8->maxMp;
            }
            evt_s1 = Evt_GetUnused();
            evt_s1->x1.s.hi = evt->x3.s.hi;
            evt_s1->z1.s.hi = evt->z3.s.hi;
            evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_TARGET];

            value1 = s_targetUnit_801231a8->hpFrac / 100;
            value2 = CalculateSpellPowerAndExp(s_casterUnit_801231a4, s_targetUnit_801231a8);
            s_targetUnit_801231a8->hpFrac += value2;

            value2 = s_targetUnit_801231a8->hpFrac / 100;
            SaveRestoreHpPercents(&value1, &value2, 0);

            value1 = s_targetUnit_801231a8->hp;
            CalculateUnitStats(s_targetUnit_801231a8);
            value2 = s_targetUnit_801231a8->hp - value1;
            SaveRestoreDamage(&value2, 0);
            SaveRestoreHp(&s_targetUnit_801231a8->maxHp, &value1, &s_targetUnit_801231a8->hp, 0);
            gSignal4 = 1;
            break;

         case SPELL_EFFECT_POISON:
         case SPELL_EFFECT_PARALYZE:
            if (s_targetUnit_801231a8->poisoned != 0) {
               s_targetUnit_801231a8->poisoned = 1;
            }
            TryInflictingAilment(s_casterUnit_801231a4, s_targetUnit_801231a8,
                                 gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] ==
                                     SPELL_EFFECT_PARALYZE);

            if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] == SPELL_EFFECT_POISON) {
               ix = 1;
               switch (s_targetUnit_801231a8->poisoned) {
               case 1:
                  s_targetUnit_801231a8->poisoned = 0;
                  evt_a2 = Evt_GetUnused();
                  evt_a2->functionIndex = EVTF_APPLY_POISON;
                  evt_a2->d.evtf007.unit = s_targetUnit_801231a8;
                  break;
               case 0:
                  ix = 2;
                  break;
               }
            } else {
               // SPELL_EFFECT_PARALYZE:
               if (s_targetUnit_801231a8->paralyzed != 0) {
                  s_targetUnit_801231a8->paralyzed = 1;
                  ix = 1;
               } else {
                  ix = 2;
               }
            }
            //@3a58
            evt_s1 = Evt_GetUnused();
            evt_s1->x1.s.hi = evt->x3.s.hi;
            evt_s1->z1.s.hi = evt->z3.s.hi;
            evt_s1->functionIndex = gSpellsEx[gCurrentSpell][ix];

            gSignal4 = 1;
            break;

         case SPELL_EFFECT_BOOST_ATK:
            if (!s_targetUnit_801231a8->atkBoosted) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->atkBoosted = 1;
            break;

         case SPELL_EFFECT_BOOST_DEF:
            if (!s_targetUnit_801231a8->defBoosted) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->defBoosted = 1;
            break;

         case SPELL_EFFECT_BOOST_ATK_AND_DEF:
            if (!s_targetUnit_801231a8->atkBoosted || !s_targetUnit_801231a8->defBoosted) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->atkBoosted = 1;
            s_targetUnit_801231a8->defBoosted = 1;
            break;

         case SPELL_EFFECT_CURE_AILMENTS:
            if (s_targetUnit_801231a8->poisoned || s_targetUnit_801231a8->paralyzed) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->poisoned = 0;
            s_targetUnit_801231a8->paralyzed = 0;
            break;

         case SPELL_EFFECT_BOOST_AGL:
            if (!s_targetUnit_801231a8->aglBoosted) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->aglBoosted = 1;
            break;

         case SPELL_EFFECT_RESTORE_MP:
            if (s_targetUnit_801231a8->maxMp > s_targetUnit_801231a8->mp) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->mp += gSpellsEx[gCurrentSpell][SPELL_EX_MP_BONUS];
            if (s_targetUnit_801231a8->maxMp < s_targetUnit_801231a8->mp) {
               s_targetUnit_801231a8->mp = s_targetUnit_801231a8->maxMp;
            }
            break;

         case SPELL_EFFECT_TRANSFER_MP:
            if (s_casterUnit_801231a4 != s_targetUnit_801231a8 &&
                s_targetUnit_801231a8->maxMp > s_targetUnit_801231a8->mp) {
               CalculateSupportSpellExp(s_targetUnit_801231a8);
            }
            s_targetUnit_801231a8->mp += gSpellsEx[gCurrentSpell][SPELL_EX_MP_BONUS];
            if (s_targetUnit_801231a8->maxMp < s_targetUnit_801231a8->mp) {
               s_targetUnit_801231a8->mp = s_targetUnit_801231a8->maxMp;
            }
            break;

         } // switch (effect) (via state:9->state2:0)

         if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] >= SPELL_EFFECT_SUPPORT_OFS) {
            evt_s1 = Evt_GetUnused();
            evt_s1->x1.s.hi = evt->x3.s.hi;
            evt_s1->z1.s.hi = evt->z3.s.hi;
            evt_s1->functionIndex = gSpellsEx[gCurrentSpell][SPELL_EX_EVTF_TARGET];
            gSignal4 = 1;
         }
         gSignal3 = 0;
         evt->state2++;
         return;

      case 1:
         if (gSignal3 == 0 || gSignal4 == 0) {
            return;
         }
         EVT.timer = 7;
         evt->state2++;
         return;
      case 2:
         if (--EVT.timer == 0) {
            if (gSpellSounds2[gCurrentSpell] != 0) {
               // Prepare XA for hit sound
               PerformAudioCommand(gSpellSounds2[gCurrentSpell] + 0x1000);
            }
            s_targetCount_801231b0++;
            evt->state--;
         }
         return;
      } // switch (evt->state2) (via state:9)
      return;

   case 99:
      gState.shielding = 0;
      CloseWindow(0x38);
      gMapSizeX = EVT.mapSizeX;
      gMapSizeZ = EVT.mapSizeZ;
      gSignal5 = 99;
      evt->state++;
      evt->state2 = 0;
      return;

   case 100:

      switch (evt->state2) {
      case 0:
         if (gSignal5 != 100) {
            return;
         }
         EVT.timer = 1;
         s_gainedLevel_801231ac = 0;
         s_casterUnit_801231a4 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];

         if (s_casterUnit_801231a4->team == TEAM_PLAYER) {
            u32 prevExp;
            s32 prevLevel;
            prevExp = s_casterUnit_801231a4->exp;
            SyncGainedHp(s_casterUnit_801231a4, 0);
            prevLevel = s_casterUnit_801231a4->level;
            if (s_casterUnit_801231a4->level < 50) {
               BigIntAdd(s_casterUnit_801231a4->experience, gState.experience);
            }
            CalculateUnitStats(s_casterUnit_801231a4);
            if (prevLevel != s_casterUnit_801231a4->level) {
               DetermineMaxMpAndStatVariance(s_casterUnit_801231a4);
               CalculateUnitStats(s_casterUnit_801231a4);
               s_gainedLevel_801231ac++;
               PopulateUnitSpellList(s_casterUnit_801231a4);
               StartUnitSpritesDecoder(s_casterUnit_801231a4->sprite->d.sprite.stripIdx);
            }
            SyncGainedHp(s_casterUnit_801231a4, 1);
            prevExp = s_casterUnit_801231a4->exp - prevExp;
            if (prevExp != 0) {
               ShowExpDialog(prevExp, 0x3c);
               EVT.timer = 50;
            }
         }
         evt->state2++;
         return;

      case 1:
         if ((--EVT.timer == 0) || (gPadStateNewPresses & PAD_CIRCLE)) {
            CloseWindow(0x3c);
            if (!s_gainedLevel_801231ac) {
               evt->state2 = 99;
               return;
            }
            evt_a2 = Evt_GetUnused();
            evt_a2->functionIndex = EVTF_LEVEL_UP;
            s_casterUnit_801231a4 = &gUnits[OBJ_MAP_UNIT(evt).s.unitIdx];
            evt_a2->d.evtf571.sprite = s_casterUnit_801231a4->sprite;
            evt->state2++;
         }
         return;

      case 2:
         if (gSignal5 == 100) {
            evt->state2 = 99;
         }
         return;

      case 99:
         gSignal2 = 99;
         evt->functionIndex = EVTF_NULL;
         return;

      } // switch (evt->state2) (via state:100)
   }
}

u8 *s_pTargetCoordsToGet_801231b4;
u8 s_unitTypeToSpawn_801231b8;
u8 *s_pTargetCoordsToSet_801231bc;
UnitStatus *s_unit_801231c0;

#undef EVTF
#define EVTF 592
void Evtf592_BattleTurnStart(EvtData *evt) {
   s32 i, ct;
   EvtData *evt_s1, *evt_v1;
   s16 value1, value2;

   const u8 antArmSpawnCoords[8] = {0, 0, 0, 17, 17, 0, 17, 17};
   const u8 sparkieSpawnCoords[8] = {0, 0, 0, 16, 16, 0, 16, 16};

   switch (evt->state) {
   case 0:
      // Clear buffs; gather targets atop healing circles
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(141));
      gSignal2 = 0;
      s_unit_801231c0 = &gUnits[1];

      for (i = 1; i < UNIT_CT; i++, s_unit_801231c0++) {
         if (s_unit_801231c0->idx != 0 && s_unit_801231c0->team == EVT.team) {
            s_unit_801231c0->atkBoosted = 0;
            s_unit_801231c0->defBoosted = 0;
         }
      }
      s_pTargetCoordsToSet_801231bc = gTargetCoords;
      s_unit_801231c0 = &gUnits[1];
      ct = 0;

      for (i = 1; i < UNIT_CT; i++, s_unit_801231c0++) {
         evt_s1 = s_unit_801231c0->sprite;
         if (s_unit_801231c0->idx != 0 && s_unit_801231c0->team == EVT.team &&
             gTerrainPtr[evt_s1->z1.s.hi][evt_s1->x1.s.hi].s.terrain == TERRAIN_BOUNDARY) {
            *s_pTargetCoordsToSet_801231bc++ = evt_s1->z1.s.hi;
            *s_pTargetCoordsToSet_801231bc++ = evt_s1->x1.s.hi;
            ct++;
         }
      }

      if (ct == 0) {
         // No suitable targets: advance state to paralysis check
         evt->state += 3;
         return;
      }

      // Finish gathering healing circle targets
      *s_pTargetCoordsToSet_801231bc++ = 0xff;

      s_pTargetCoordsToGet_801231b4 = gTargetCoords;
      evt->state++;

   // fallthrough
   case 1:
      if (*s_pTargetCoordsToGet_801231b4 != 0xff) {
         // Initiate next healing circle target
         gTargetZ = *s_pTargetCoordsToGet_801231b4++;
         gTargetX = *s_pTargetCoordsToGet_801231b4++;

         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);

         if (EVT.targets == 0) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_017;
            evt_v1->d.bytes[4] = 0;
            evt_v1->d.evtf017.sprite = evt_s1;
         } else {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
            evt_v1->d.evtf026.target = evt_s1;
            evt_v1->d.evtf026.type = 1;
         }
         EVT.targets++;
         EVT.timer = 20;
         evt->state2 = 0;
         evt->state++;
      } else {
         // Finished processing healing circle units
         evt->state = 3;
      }
      return;

   case 2:
      // Healing circle
      switch (evt->state2) {
      case 0:
         if (--EVT.timer != 0) {
            return;
         }
         //@443c
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(141));
         s_unit_801231c0 = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx];
         s_unit_801231c0->mp += s_unit_801231c0->maxMp / 5;
         if (s_unit_801231c0->mp > s_unit_801231c0->maxMp) {
            s_unit_801231c0->mp = s_unit_801231c0->maxMp;
         }
         value1 = s_unit_801231c0->hpFrac / 100;
         if (value1 == 0) {
            value1 = 1;
         }
         value2 = 3000;
         s_unit_801231c0->hpFrac += value2;
         if (s_unit_801231c0->hpFrac > 10000) {
            s_unit_801231c0->hpFrac = 10000;
         }
         value2 = s_unit_801231c0->hpFrac / 100;
         if (value2 == 0 && s_unit_801231c0->hpFrac != 0) {
            value2 = 1;
         }
         SaveRestoreHpPercents(&value1, &value2, 0);
         value1 = s_unit_801231c0->hp;
         CalculateUnitStats(s_unit_801231c0);
         value2 = s_unit_801231c0->hp - value1;
         SaveRestoreDamage(&value2, 0);
         SaveRestoreHp(&s_unit_801231c0->maxHp, &value1, &s_unit_801231c0->hp, 0);

         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_HEALING_FX2;
         evt_v1->x1.s.hi = gTargetX;
         evt_v1->z1.s.hi = gTargetZ;

         gSignal3 = 0;
         evt->state2++;
         return;
      case 1:
         if (gSignal3 == 0) {
            return;
         }
         EVT.timer = 10;
         evt->state2++;
         return;
      case 2:
         if (--EVT.timer != 0) {
            return;
         }
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(141));
         evt->state--;
         return;
      } // switch (evt->state2) (via state:2)

      return;

   case 3:
      // Gather targets for paralysis removal
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(135));
      s_pTargetCoordsToSet_801231bc = gTargetCoords;
      s_unit_801231c0 = &gUnits[1];

      ct = 0;
      for (i = 1; i < UNIT_CT; i++, s_unit_801231c0++) {
         evt_s1 = s_unit_801231c0->sprite;
         if (s_unit_801231c0->idx != 0 && s_unit_801231c0->team == EVT.team &&
             s_unit_801231c0->paralyzed) {
            if ((rand() & 0x7fff) < (s_unit_801231c0->paralyzed - 1) * 0x1999) {
               s_unit_801231c0->paralyzed = 0;
               *s_pTargetCoordsToSet_801231bc++ = evt_s1->z1.s.hi;
               *s_pTargetCoordsToSet_801231bc++ = evt_s1->x1.s.hi;
               ct++;
            } else {
               // Increase odds for next time
               s_unit_801231c0->paralyzed++;
            }
         }
      }
      if (ct == 0) {
         // No suitable targets: advance state to poison check
         evt->state += 3;
         return;
      }

      *s_pTargetCoordsToSet_801231bc++ = 0xff;
      s_pTargetCoordsToGet_801231b4 = gTargetCoords;
      evt->state++;

   // fallthrough
   case 4:
      if (*s_pTargetCoordsToGet_801231b4 != 0xff) {
         // Initiate next paralysis removal
         gTargetZ = *s_pTargetCoordsToGet_801231b4++;
         gTargetX = *s_pTargetCoordsToGet_801231b4++;

         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);

         if (EVT.targets == 0) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_017;
            evt_v1->d.bytes[4] = 0;
            evt_v1->d.evtf017.sprite = evt_s1;
         } else {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
            evt_v1->d.evtf026.target = evt_s1;
            evt_v1->d.evtf026.type = 1;
         }
         EVT.targets++;
         StartUnitSpritesDecoder(evt_s1->d.sprite.stripIdx);
         EVT.timer = 20;
         evt->state2 = 0;
         evt->state++;
      } else {
         // Finished processing paralysis removals
         evt->state = 6;
      }
      return;

   case 5:
      // Paralysis removal
      switch (evt->state2) {
      case 0:
         if (--EVT.timer != 0) {
            return;
         }
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(135));
         s_unit_801231c0 = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx];

         gTileStateGridPtr[gTargetZ][gTargetX].action = TA_X1E;
         if (s_unit_801231c0->team == TEAM_PLAYER) {
            gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 22;
         } else {
            gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 12;
         }

         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_REMOVE_PARALYSIS;
         evt_v1->x1.n = evt_s1->x1.n;
         evt_v1->z1.n = evt_s1->z1.n;
         evt_v1->state2 = 50;
         EVT.timer = 60;
         evt->state2++;
         return;
      case 1:
         if (--EVT.timer != 0) {
            return;
         }
         EVT.timer = 10;
         evt->state2++;
         return;
      case 2:
         if (--EVT.timer != 0) {
            return;
         }
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(135));
         evt->state--;
         return;
      } // switch (evt->state2) (via state:5)

      return;

   case 6:
      // Gather targets for poison damage
      s_pTargetCoordsToSet_801231bc = gTargetCoords;
      s_unit_801231c0 = &gUnits[1];

      ct = 0;
      for (i = 1; i < UNIT_CT; i++, s_unit_801231c0++) {
         if (s_unit_801231c0->idx != 0 && s_unit_801231c0->team == EVT.team) {
            evt_s1 = s_unit_801231c0->sprite;
            if (s_unit_801231c0->poisoned) {
               *s_pTargetCoordsToSet_801231bc++ = evt_s1->z1.s.hi;
               *s_pTargetCoordsToSet_801231bc++ = evt_s1->x1.s.hi;
               ct++;
            }
         }
      }
      if (ct == 0 && EVT.targets == 0) {
         evt->state += 3;
         return;
      }
      *s_pTargetCoordsToSet_801231bc++ = 0xff;
      s_pTargetCoordsToGet_801231b4 = gTargetCoords;
      evt->state++;

   // fallthrough
   case 7:
      if (*s_pTargetCoordsToGet_801231b4 != 0xff) {
         // Initiate next poison damage
         gTargetZ = *s_pTargetCoordsToGet_801231b4++;
         gTargetX = *s_pTargetCoordsToGet_801231b4++;

         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);

         if (EVT.targets == 0) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_017;
            evt_v1->d.bytes[4] = 0;
            evt_v1->d.evtf017.sprite = evt_s1;
         } else {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
            evt_v1->d.evtf026.target = evt_s1;
            evt_v1->d.evtf026.type = 1;
         }
         EVT.targets++;
         EVT.timer = 20;
         evt->state2 = 0;
         evt->state++;
      } else {
         // Finished processing poison damages
         evt->state = 9;
      }
      return;

   case 8:
      // Poison damage
      switch (evt->state2) {
      case 0:
         if (--EVT.timer != 0) {
            return;
         }
         s_unit_801231c0 = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx];

         value1 = s_unit_801231c0->hpFrac / 100;
         if (value1 == 0) {
            value1 = 1;
         }
         value2 = 2500;
         s_unit_801231c0->hpFrac -= value2;
         if (s_unit_801231c0->hpFrac < 0) {
            s_unit_801231c0->hpFrac = 0;
         }
         value2 = s_unit_801231c0->hpFrac / 100;
         if (value2 == 0 && s_unit_801231c0->hpFrac != 0) {
            value2 = 1;
         }
         SaveRestoreHpPercents(&value1, &value2, 0);
         value1 = s_unit_801231c0->hp;
         CalculateUnitStats(s_unit_801231c0);
         value2 = value1 - s_unit_801231c0->hp;
         SaveRestoreDamage(&value2, 0);
         SaveRestoreHp(&s_unit_801231c0->maxHp, &value1, &s_unit_801231c0->hp, 0);

         if (s_unit_801231c0->hp != 0) {
            gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_HIT;
         } else {
            if (!HasDefeatSpeech(s_unit_801231c0)) {
               gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT;
            } else {
               gTileStateGridPtr[gTargetZ][gTargetX].action = TA_PHYS_DEFEAT_MSG;
            }
         }
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_DISPLAY_DAMAGE;
         evt_v1->x1.s.hi = gTargetX;
         evt_v1->z1.s.hi = gTargetZ;

         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_BOUNCE_ZOOM;

         gSignal3 = 0;
         evt->state2++;
         return;
      case 1:
         if (gSignal3 == 0) {
            return;
         }
         EVT.timer = 10;
         evt->state2++;
         return;
      case 2:
         if (--EVT.timer != 0) {
            return;
         }
         evt->state--;
         return;

      } // switch (evt->state2) (via state:8)
      return;

   case 9:
      // Enemy respawns
      if (EVT.team == TEAM_PLAYER) {
         evt->state = 99;
         return;
      }
      if (gState.mapNum == 14) {
         s_unitTypeToSpawn_801231b8 = UNIT_TYPE_ANT_ARM;
         s_pTargetCoordsToGet_801231b4 = antArmSpawnCoords;
      } else if (gState.mapNum == 23) {
         s_unitTypeToSpawn_801231b8 = UNIT_TYPE_SPARKIE;
         s_pTargetCoordsToGet_801231b4 = sparkieSpawnCoords;
      } else {
         evt->state = 99;
         return;
      }
      if (CountUnitsOfType(s_unitTypeToSpawn_801231b8) > 5) {
         evt->state = 99;
         return;
      }
      evt->state++;

   // fallthrough
   case 10:
      if (s_unitTypeToSpawn_801231b8 == UNIT_TYPE_ANT_ARM) {
         s_pTargetCoordsToGet_801231b4 = antArmSpawnCoords;
      } else {
         s_pTargetCoordsToGet_801231b4 = sparkieSpawnCoords;
      }
      ct = 0;
      for (i = 0; i < 4; i++) {
         if (gMapUnitsPtr[s_pTargetCoordsToGet_801231b4[i * 2]]
                         [s_pTargetCoordsToGet_801231b4[i * 2 + 1]]
                             .s.unitIdx == 0) {
            ct++;
         }
      }
      if (ct == 0) {
         evt->state = 99;
         return;
      }

      i = rand() % 4;
      while (1) {
         gTargetZ = s_pTargetCoordsToGet_801231b4[i * 2];
         gTargetX = s_pTargetCoordsToGet_801231b4[i * 2 + 1];
         i++;
         if (gMapUnitsPtr[gTargetZ][gTargetX].s.unitIdx == 0) {
            break;
         }
         if (i == 4) {
            i = 0;
         }
      }
      gTileStateGridPtr[gTargetZ][gTargetX].action = TA_X19;
      if (s_unitTypeToSpawn_801231b8 == UNIT_TYPE_ANT_ARM) {
         SetupBattleUnit(7, gTargetZ, gTargetX, 1, TEAM_ENEMY, DIR_SOUTH, 99, 0, 99);
      } else {
         SetupBattleUnit(1, gTargetZ, gTargetX, 10, TEAM_ENEMY, DIR_SOUTH, 99, 10, 99);
      }
      evt->state++;
      return;

   case 11:
      evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      StartUnitSpritesDecoder(evt_s1->d.sprite.stripIdx);

      if (EVT.targets == 0) {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_CAMERA_TBD_017;
         evt_v1->d.bytes[4] = 0;
         evt_v1->d.evtf017.sprite = evt_s1;
      } else {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
         evt_v1->d.evtf026.target = evt_s1;
         evt_v1->d.evtf026.type = 1;
      }
      EVT.targets++;
      EVT.timer = 20;
      evt->state++;
      return;

   case 12:
      if (--EVT.timer != 0) {
         return;
      }
      if (s_unitTypeToSpawn_801231b8 == UNIT_TYPE_ANT_ARM) {
         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
         gTileStateGridPtr[gTargetZ][gTargetX].action = TA_X1E;
         gTileStateGridPtr[gTargetZ][gTargetX].cachedByte = 16;
         EVT.timer = 70;
      } else {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_STRETCH_WARP_SPRITE;
         evt_v1->mem = 1;
         evt_v1->z1.s.hi = gTargetZ;
         evt_v1->x1.s.hi = gTargetX;
         EVT.timer = 20;
      }
      evt->state++;
      return;

   case 13:
      if (s_unitTypeToSpawn_801231b8 == UNIT_TYPE_ANT_ARM) {
         evt_s1 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
         if (EVT.timer == 68) {
            evt_s1->d.sprite.hidden = 0;
         }
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_CLOUD;
         evt_v1->x1.n = evt_s1->x1.n;
         evt_v1->z1.n = evt_s1->z1.n;
         evt_v1->y1.n = evt_s1->y1.n;
      }
      if (--EVT.timer != 0) {
         return;
      }
      evt->state = 9;
      return;

   case 99:
      if (EVT.targets == 0) {
         evt->state += 2;
         return;
      }
      gSignal5 = 99;
      evt->state++;

   // fallthrough
   case 100:
      if (gSignal5 == 100) {
         evt->state++;
      }
      return;

   case 101:
      gSignal2 = 99;
      evt->functionIndex = EVTF_NULL;
      return;

   } // switch (evt->state)
}
