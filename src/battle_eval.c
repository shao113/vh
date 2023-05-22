#include "common.h"
#include "units.h"
#include "evt.h"
#include "audio.h"
#include "field.h"
#include "battle.h"
#include "state.h"
#include "graphics.h"
#include "cd_files.h"
#include "card.h"
#include "window.h"

u8 gBattleSceneId[50] = {96, 97, 98, 99, 100, 101, 0,  0,  102, 95, 1,  4,  8,  11, 15, 17, 19,
                         21, 24, 27, 29, 32,  35,  38, 40, 43,  46, 48, 49, 53, 55, 60, 63, 65,
                         69, 71, 74, 77, 78,  82,  84, 85, 90,  92, 0,  0,  0,  0,  0,  0};

s16 gBattleEvaluator[BATTLE_CT] = {
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_08,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_10,  EVTF_EVALUATE_BATTLE_11,
    EVTF_EVALUATE_BATTLE_12,  EVTF_EVALUATE_BATTLE_13,  EVTF_EVALUATE_BATTLE_14,
    EVTF_EVALUATE_BATTLE_15,  EVTF_EVALUATE_BATTLE_16,  EVTF_EVALUATE_BATTLE_17,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_19,  EVTF_EVALUATE_BATTLE_20,
    EVTF_EVALUATE_BATTLE_21,  EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_23,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_26,
    EVTF_EVALUATE_BATTLE_27,  EVTF_EVALUATE_BATTLE_28,  EVTF_EVALUATE_BATTLE_29,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_32,
    EVTF_EVALUATE_BATTLE_33,  EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_35,
    EVTF_EVALUATE_BATTLE_STD, EVTF_EVALUATE_BATTLE_37,  EVTF_EVALUATE_BATTLE_38,
    EVTF_EVALUATE_BATTLE_39,  EVTF_EVALUATE_BATTLE_40,  EVTF_EVALUATE_BATTLE_STD,
    EVTF_EVALUATE_BATTLE_42,  EVTF_EVALUATE_BATTLE_43};

BVectorZXY gMapCursorStartingPos[BATTLE_CT] = {
    {6, 18, 0},  {7, 21, 2},  {10, 13, 0}, {5, 19, 0},  {19, 7, 1}, {0, 57, 0},  {0, 0, 0},
    {0, 0, 0},   {15, 15, 2}, {0, 0, 0},   {8, 8, 3},   {6, 21, 2}, {11, 15, 1}, {3, 16, 3},
    {8, 14, 3},  {7, 10, 2},  {6, 21, 0},  {14, 20, 2}, {14, 9, 0}, {7, 28, 3},  {4, 7, 2},
    {9, 16, 3},  {7, 8, 0},   {7, 8, 0},   {8, 21, 3},  {9, 13, 3}, {3, 16, 2},  {1, 21, 0},
    {11, 6, 0},  {9, 10, 2},  {5, 16, 2},  {11, 9, 3},  {3, 8, 0},  {3, 18, 3},  {20, 9, 0},
    {7, 15, 1},  {18, 18, 3}, {12, 16, 3}, {12, 26, 3}, {7, 24, 3}, {6, 40, 3},  {6, 20, 2},
    {11, 22, 3}, {8, 20, 3}};

void PlayBattleBGM(u8 battleNum) {
   PerformAudioCommand(6);
   if (gState.mapNum == 28 && gState.turn > 1) {
      LoadSeqSet(23);
      FinishLoadingSeq();
      PerformAudioCommand(0x202);
   } else if (gState.mapNum == 40 && gState.mapState.s.field_0x0 != 0) {
      LoadSeqSet(25);
      FinishLoadingSeq();
      PerformAudioCommand(0x20b);
   } else {
      LoadSeqSet(gBattleBGM[battleNum].seqSetIdx);
      FinishLoadingSeq();
      PerformAudioCommand(gBattleBGM[battleNum].seqId);
   }
}

void PlayCurrentBattleBGM(void) {
   static u8 battleOverrideBGM[50] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1,
                                      1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0,
                                      0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1};

   if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE || gState.primary == STATE_31) {
      PlayBattleBGM(gState.mapNum);
   } else if (battleOverrideBGM[gState.mapNum]) {
      PlayBattleBGM(gState.mapNum);
   }
}

s32 State_Battle(void) {
   // Note: Unused return value needed for this match
   s32 i, ct;
   EvtData *newEvt;

   switch (gState.secondary) {
   case 0:

      switch (gState.state3) {
      case 0:
         gState.suppressLoadingScreen = 1;
         FadeOutScreen(2, 0xff);
         ResetGeomOffset();
         if (gState.primary != STATE_LOAD_IN_BATTLE_SAVE) {
            gState.saveLocation = SAVE_LOC_BATTLE;
            Card_PopulateRegularSave();
         } else {
            gState.mapNum = gDeferredInBattleSaveData.battle;
            ClearPortraitSet();
            ClearPortraitSet();
         }
         LoadText(CDF_B_TXT00_DAT + gState.mapNum, gText, gTextPointers);
         gState.currentTextPointers = &gTextPointers[1];
         gState.state4 = 1;
         gState.state3++;

      // fallthrough
      case 1:
         gState.state4--;
         if (gState.state4 == 0) {
            if (gState.primary < STATE_30 && gState.mapNum != 35 && gState.mapNum != 38) {
               gState.suppressLoadingScreen = 0;
            }
            gState.state4 = 2;
            gState.state3++;
         }

         break;

      case 2:
         gState.state4--;
         if (gState.state4 == 0) {
            gState.state3++;
         }

         break;

      case 3:
         SetGeomScreen(0x200);
         Evt_ResetFromIdx10();
         if (gState.primary != STATE_LOAD_IN_BATTLE_SAVE) {
            ClearUnits();
         }
         gState.vsyncMode = 2;
         PlayCurrentBattleBGM();
         if (gState.primary == STATE_27 || gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
            LoadFWD();
         }
         if (gState.primary < STATE_30) {
            LoadFCOM4XX();
            LoadMapTextures();
         }
         gState.inEvent = 0;
         gPlayerControlSuppressed = 1;
         gClearSavedPadState = 1;
         gSignal1 = 0;
         gSignal2 = 0;
         gIsEnemyTurn = 0;
         gState.lastSelectedUnit = 1;
         gState.mapCursorOutOfRange = 0;
         gState.expScalingLevel = gBattleExpScalingLevels[gState.mapNum];

         if (gState.useDefaultUnits) {
            SetDefaultStatsForParty();
         }
         func_8005AA7C();
         if (gState.primary < STATE_30) {
            SetupSprites();
         }
         for (i = 0; i < PARTY_CT - 1; i++) {
            gPartyMemberSlain[i] = 0;
         }
         for (i = 0; i < ARRAY_COUNT(gSlainUnits); i++) {
            gSlainUnits[i].unitId = 0;
            gSlainUnits[i].count = 0;
            gSlainUnits[i].reward = 0;
         }
         ct = 0;
         for (i = 0; i < ARRAY_COUNT(gSlainUnits); i++) {
            if (gSpriteStripUnitIds[i] > UNIT_ID_END_OF_PARTY) {
               gSlainUnits[ct].reward = gBattleUnitRewards[gState.mapNum][i];
               gSlainUnits[ct].unitId = gSpriteStripUnitIds[i];
               ct++;
            }
         }
         if (gState.primary < STATE_30) {
            for (i = 24; i < 50; i++) {
               gState.portraitsToLoad[i] =
                   gPortraitsDb.sceneSets[gBattleSceneId[gState.mapNum]][i - 24];
            }
            if (gState.mapNum != 8) {
               LoadPortraits();
            }
            if (gState.primary < STATE_30) {
               LoadUnits();
            }
         }
         if (gState.primary != STATE_LOAD_IN_BATTLE_SAVE) {
            for (i = 0; i < ARRAY_COUNT(gState.mapState.bytes); i++) {
               gState.mapState.bytes[i] = 0;
            }
            SetupBattleUnits();
         }

         gTempEvt = Evt_GetUnused();
         gTempEvt->functionIndex = EVTF_BATTLE_ENDER;

         if (LoadSoundSet(2) != 0 || gState.primary != STATE_30 || gState.mapNum == 13 ||
             gState.mapNum == 14 || gState.mapNum == 15 || gState.mapNum == 33 ||
             gState.mapNum == 39) {
            FinishLoadingVab();
            LoadMap();
            SetupLight();
         }
         SetupField();
         SetupMap();
         gState.fieldRenderingDisabled = 0;
         gState.battleEval = BATTLE_EVAL_NONE;
         gState.needEval = 0;
         gState.signal = 0;
         gState.turn = 1;
         gState.zoom = 1;

         if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
            gState.worldMapState = gDeferredInBattleSaveData.worldMapState;
            gState.worldMapDestination = gDeferredInBattleSaveData.worldMapDestination;
            gState.townState = gDeferredInBattleSaveData.townState;
            gState.frameCounter = gDeferredInBattleSaveData.frameCounter;
            gState.chapter = gDeferredInBattleSaveData.chapter;
            gState.section = gDeferredInBattleSaveData.section;
            for (i = 0; i < ARRAY_COUNT(gState.mapState.bytes); i++) {
               gState.mapState.bytes[i] = gDeferredInBattleSaveData.mapState[i];
            }
            gState.turn = gDeferredInBattleSaveData.turn;
            for (i = 0; i < DEPOT_CT; i++) {
               gState.depot[i] = gDeferredInBattleSaveData.depot[i];
            }
            gState.gold = gDeferredInBattleSaveData.gold;
            gState.zoom = gDeferredInBattleSaveData.zoom;
            gState.lastSelectedUnit = gDeferredInBattleSaveData.lastSelectedUnit;
            gState.mono = gDeferredInBattleSaveData.mono;
            PerformAudioCommand(!gState.mono ? 2 : 1);
            gState.debug = gDeferredInBattleSaveData.debug;
            gState.cameraMode = gDeferredInBattleSaveData.cameraMode;
            gState.textSpeed = gDeferredInBattleSaveData.textSpeed;
            for (i = 0; i < ARRAY_COUNT(gSlainUnits); i++) {
               gSlainUnits[i].unitId = gDeferredInBattleSaveData.slainUnits[i].unitId;
               gSlainUnits[i].count = gDeferredInBattleSaveData.slainUnits[i].count;
               gSlainUnits[i].reward = gDeferredInBattleSaveData.slainUnits[i].reward;
            }
            for (i = 0; i < PARTY_CT - 1; i++) {
               gPartyMemberSlain[i] = gDeferredInBattleSaveData.partyMemberSlain[i];
            }
            gCameraRotation.vy = gDeferredInBattleSaveData.cameraRotationY;
            gCameraRotation.vx = gDeferredInBattleSaveData.cameraRotationX;

            switch (gDeferredInBattleSaveData.zoom) {
            case 0:
               gCameraZoom.vz = 450;
               break;
            case 1:
               gCameraZoom.vz = 640;
               break;
            case 2:
               gCameraZoom.vz = 896;
               break;
            }

            gClearSavedPadState = 0;
            gPlayerControlSuppressed = 0;

            for (i = 0; i < UNIT_CT; i++) {
               if (gUnits[i].idx != 0) {
                  CreateBattleUnitForUnit(&gUnits[i]);
               }
            }
         }
         gState.useDefaultUnits = 0;
         gState.secondary++;
         break;
      } // switch (gState.state3)

      if (gState.mapNum == 13 && gState.mapState.s.field_0x0 < 3) {
         gState.mapState.s.field_0x0 = 3;
      }
      if (gState.mapNum == 33 && gState.mapState.s.field_0x0 == 0) {
         gState.mapState.s.field_0x0 = 1;
      }
      if (gState.mapNum == 39 && gState.mapState.s.field_0x0 == 0) {
         gState.mapState.s.field_0x0 = 1;
      }

      break;

   case 1:
      gState.suppressLoadingScreen = 0;
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = gBattleEvaluator[gState.mapNum];
      gState.secondary++;

   // fallthrough
   case 2:
      FadeInScreen(2, 20);
      if (gState.mapNum == 8) {
         // Demo
         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_LOGO;
         gClearSavedPadState = 1;
         gPlayerControlSuppressed = 1;
         gIsEnemyTurn = 1;
         gState.D_80140859 = 0;
      } else if (gState.primary != STATE_LOAD_IN_BATTLE_SAVE) {
         gSignal1 = 1;
         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_BATTLE_INTRO;
      }
      gState.secondary++;
      break;

   case 3:
      if (gState.battleEval == 99) {
         gState.secondary++;
      }
      break;
   } // switch (gState.secondary)
}

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
   gLightRotation.vy = GetLightRotY() + 0x10;

   if (gState.needEval) {
      gState.needEval = 0;
   }

   if (gState.signal) {
      gState.signal = 0;
   }

   switch (evt->state) {
   case 0:
      if ((gState.D_80140859 != 0) || ((gPadStateNewPresses & PAD_START) != 0)) {
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
         gState.primary = STATE_TITLE_SCREEN;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 11 && gState.searchZ == 1) {
         gState.mapState.s.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf428_EvaluateBattle12(EvtData *evt) {
   gLightRotation.vy = GetLightRotY() + 0x10;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 10 && gState.searchZ == 16) {
         gState.mapState.s.field_0x0 = 1;
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
         gState.mapState.s.field_0x0 = 1;
         gState.signal = 2;
      }
      if (gState.searchX == 19 && gState.searchZ == 12) {
         gState.mapState.s.field_0x0 = 2;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 35 && gState.searchZ == 8) {
         gState.mapState.s.field_0x0 = 1;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 22 && gState.searchZ == 14) {
         gState.mapState.s.field_0x0 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.s.field_0x1 == 0 && gState.searchX == 9 && gState.searchZ == 9) {
         gState.mapState.s.field_0x1 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.s.field_0x2 == 0 && gState.searchX == 15 && gState.searchZ == 12) {
         gState.mapState.s.field_0x2 = 1;
         gState.signal = 2;
      }
      if (gState.mapState.s.field_0x3 == 0 && gState.searchX == 1 && gState.searchZ == 1) {
         gState.mapState.s.field_0x3 = 1;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 2 && gState.searchZ == 19) {
         gState.mapState.s.field_0x0 = 1;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 18 && gState.searchZ == 12) {
         gState.mapState.s.field_0x0 = 1;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 18 && gState.searchZ == 8) {
         gState.mapState.s.field_0x0 = 1;
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
      if (gState.mapState.s.field_0x0 == 0 && gState.searchX == 21 && gState.searchZ == 4) {
         gState.mapState.s.field_0x0 = 1;
         gState.signal = 2;
      }
   }
}

void Evtf559_EvaluateBattle39(EvtData *evt) {
   s32 unitIdx;
   if (gState.needEval) {
      gState.needEval = 0;
      unitIdx = gMapUnitsPtr[6][3].s.unitIdx;
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

void Evtf423_BattleDefeat(EvtData *evt) { Evtf420_BattleVictory(evt); }

#undef EVTF
#define EVTF 420
void Evtf420_BattleVictory(EvtData *evt) {
   static s16 youWin[] = {96,  110, GFX_YOU_1, 140, 110, GFX_YOU_2,
                          184, 110, GFX_WIN_1, 228, 110, GFX_WIN_2};
   static s16 youLose[] = {96,  110, GFX_YOU_1,  140, 110, GFX_YOU_2,
                           184, 110, GFX_LOSE_1, 228, 110, GFX_LOSE_2};

   s32 i;
   s16 *p;
   EvtData *newEvt;
   s16 delay;

   switch (evt->state) {
   case 0:
      for (i = 0; i < 100; i++) {
         CloseWindow(i);
      }
      Evt_ResetByFunction(EVTF_COMPASS);

   // fallthrough
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   case 7:
      if (evt->functionIndex == EVTF_BATTLE_VICTORY) {
         p = youWin;
      } else {
         p = youLose;
      }

      delay = 1;
      for (i = 0; i < 4; i++) {
         newEvt = Evt_GetUnused();
         newEvt->functionIndex = EVTF_BATTLE_VICTORY_PARTICLE;
         newEvt->d.evtf446.x = *p++;
         newEvt->d.evtf446.y = *p++;
         newEvt->d.evtf446.gfxIdx = *p++;
         newEvt->d.evtf446.delay = delay;
         delay += 2;
      }

      EVT.timer = 100;
      evt->state++;
      break;

   case 8:
      if (--EVT.timer == 0) {
         if (evt->functionIndex == EVTF_BATTLE_VICTORY) {
            Evt_ResetByFunction(EVTF_BATTLE_VICTORY_PARTICLE);
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_BATTLE_RESULTS;
         } else {
            gSignal2 = 1;
         }
         evt->state++;
      }
      break;

   case 9:
      evt->functionIndex = EVTF_NULL;
      return;
   }
}

#undef EVTF
#define EVTF 446
void Evtf446_BattleVictoryParticle(EvtData *evt) {
   s16 x;
   s16 y;
   s16 angle;
   s16 d;

   switch (evt->state) {
   case 0:
      if (--EVT.delay != 0) {
         return;
      }
      evt->state++;

   // fallthrough
   case 1:
      EVT.todo_x5c = 50;
      EVT.todo_x50 = rand() % 100 + 100;
      EVT.todo_x58 = rand() % 5 + 2;
      EVT.todo_x52 = rand() % 100 + 100;
      EVT.todo_x5a = rand() % 5 + 2;

      if (rand() % 2 != 0) {
         EVT.todo_x50 *= -1;
      }
      if (rand() % 2 != 0) {
         EVT.todo_x52 *= -1;
      }

      EVT.todo_x4c = EVT.todo_x50 * EVT.todo_x5c;
      EVT.todo_x4e = EVT.todo_x52 * EVT.todo_x5c;
      EVT.todo_x54 = EVT.todo_x58 * EVT.todo_x5c;
      EVT.todo_x56 = EVT.todo_x5a * EVT.todo_x5c;

      evt->state++;
      break;

   case 2:
      if (--EVT.todo_x5c != -1) {
         EVT.todo_x4c -= EVT.todo_x50;
         EVT.todo_x4e -= EVT.todo_x52;
         EVT.todo_x54 -= EVT.todo_x58;
         EVT.todo_x56 -= EVT.todo_x5a;
      } else {
         evt->state++;
      }
      break;
   }

   x = (rcos(EVT.todo_x4c & 0xfff) * EVT.todo_x54) >> 12;
   y = (rcos((EVT.todo_x4c + ANGLE_90_DEGREES) & 0xfff) * EVT.todo_x54) >> 12;

   x += (rcos(EVT.todo_x4e & 0xfff) * EVT.todo_x56) >> 12;
   y += (rcos((EVT.todo_x4e + ANGLE_90_DEGREES) & 0xfff) * EVT.todo_x56) >> 12;
   angle = EVT.todo_x4c + ANGLE_45_DEGREES;

   if (x > 0) {
      d = x * x;
   } else {
      d = -x * -x;
   }
   if (y > 0) {
      d += y * y;
   } else {
      d += -y * -y;
   }

   d = (SquareRoot0(d) / 4) + 32;
   x += EVT.x;
   y += EVT.y;
   EVT.coords[1].x = x + ((rcos(angle & 0xfff) * d) >> 12);
   EVT.coords[0].x = x + ((rcos((angle + ANGLE_90_DEGREES) & 0xfff) * d) >> 12);
   EVT.coords[3].x = x + ((rcos((angle + ANGLE_270_DEGREES) & 0xfff) * d) >> 12);
   EVT.coords[2].x = x + ((rcos((angle + ANGLE_180_DEGREES) & 0xfff) * d) >> 12);
   EVT.coords[1].y = y + ((rcos((angle + ANGLE_90_DEGREES) & 0xfff) * d) >> 12);
   EVT.coords[0].y = y + ((rcos((angle + ANGLE_180_DEGREES) & 0xfff) * d) >> 12);
   EVT.coords[3].y = y + ((rcos(angle & 0xfff) * d) >> 12);
   EVT.coords[2].y = y + ((rcos((angle + ANGLE_270_DEGREES) & 0xfff) * d) >> 12);

   EVT.otOfs = 55 - EVT.todo_x5c;
   AddEvtPrim2(gGraphicsPtr->ot, evt);
}
