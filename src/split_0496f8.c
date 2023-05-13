#include "common.h"
#include "state.h"
#include "units.h"
#include "graphics.h"
#include "evt.h"
#include "field.h"
#include "battle.h"

#include "PsyQ/libsn.h"

void ClearPortraitSet(void);
void ResetStateForNewGame(void);
void SetupParty(u8);
void SetupPartySprites(void);
void SetDefaultStatsForParty(void);
void PopulateUnitSpellList(UnitStatus *);
void SetupPartyBattleUnit(u8, u8, u8, u8);
void SyncPartyUnits(u8);
void Evtf424_BattleEnder(EvtData *);
void func_8005AA7C(void);
void SetupBattleUnit(s16, u8, u8, s8, u8, u8, u8, u8, u8);
void CreateBattleUnitForUnit(UnitStatus *);
void SetupBattleUnits(void);
s32 PCDrv_WriteFile(s8 *, void *, s32);
void ResetGeomOffset(void);
void Initialize(void);
//...

void ClearPortraitSet(void) {
   s32 i;

   for (i = 0; i < ARRAY_COUNT(gState.currentPortraits); i++) {
      gState.currentPortraits[i] = 0;
      gState.portraitsToLoad[i] = 0;
   }
}

void ResetStateForNewGame(void) {
   s32 i, j;

   for (i = 0; i < PARTY_CT; i++) {
      gPartyMembers[i].inParty = 0;
      gPartyMembers[i].advChosePathB = 0;
      gPartyMembers[i].advLevelFirst = 0;
      gPartyMembers[i].advLevelSecond = 0;

      for (j = 0; j < 8; j++) {
         gPartyMembers[i].experience[j] = 0;
      }
   }

   gState.gold = 0;
   gState.frameCounter = 0;
}

void SetupParty(u8 idx) {
   s32 i, j;
   s32 level;

   for (i = 1; i < PARTY_CT; i++) {
      level = gInitialLevels[idx][i - 1];

      if (level != 0) {
         gPartyMembers[i].inParty = 1;

         if (level != 1) {
            gPartyMembers[i].advChosePathB = 0;
            gPartyMembers[i].advLevelFirst = 0;
            gPartyMembers[i].advLevelSecond = 0;
            gPartyMembers[i].items[0] = gUnitInfo[i].heldItem;
            gPartyMembers[i].items[1] = ITEM_NULL;
            gPartyMembers[i].field9_0x9 = 0;
            gPartyMembers[i].weapon = gUnitInfo[i].weapon;
            gPartyMembers[i].helmet = gUnitInfo[i].helmet;
            gPartyMembers[i].armor = gUnitInfo[i].armor;

            for (j = 0; j < 8; j++) {
               gPartyMembers[i].experience[j] = gExperienceLevels[level - 1][j];
            }

            gPartyMembers[i].hpVar100 = 100;
            gPartyMembers[i].atkVar100 = 100;
            gPartyMembers[i].defVar100 = 100;
            gPartyMembers[i].agiVar100 = 100;
         }
      } else {
         gPartyMembers[i].inParty = 0;
      }
   }
}

void SetupPartySprites(void) {
   s32 i;
   s32 partyIdx;
   s32 id;
   s32 adv;
   s8 unused[8];

   for (i = 0; i < 20; i++) {
      id = gSpriteStripUnitIds[i];
      if (id >= 151 && id <= 274) {
         // IDs 151..300 are reserved for main party event sprites (sitting, etc.), but they start
         // off as a "virtual" ID, since they need to be adjusted to account for advancements.
         // Larger ranges (e.g. 157..[181..183]) are (presumably) to leave room for the preceding
         // ranges' adjustment. (Also, I wonder why Clint is out of order here.)
         if (id >= 273) { ///////// 273..274
            partyIdx = UNIT_DARIUS;
         } else if (id >= 247) { // 247..272
            partyIdx = UNIT_ZOHAR;
         } else if (id >= 245) { // 245..246
            partyIdx = UNIT_SARA;
         } else if (id >= 243) { // 243..244
            partyIdx = UNIT_AMON;
         } else if (id >= 217) { // 217..242
            partyIdx = UNIT_DOLAN;
         } else if (id >= 215) { // 215..216
            partyIdx = UNIT_GROG;
         } else if (id >= 213) { // 213..214
            partyIdx = UNIT_KIRA;
         } else if (id >= 187) { // 187..212
            partyIdx = UNIT_CLINT;
         } else if (id >= 184) { // 184..186
            partyIdx = UNIT_HUXLEY;
         } else if (id >= 157) { // 157..183
            partyIdx = UNIT_ELENI;
         } else if (id >= 154) { // 154..156
            partyIdx = UNIT_DIEGO;
         } else { ///////////////// 151..153
            partyIdx = UNIT_ASH;
         }

         adv = 0;
         if (gPartyMembers[partyIdx].advLevelFirst != 0) {
            if (gPartyMembers[partyIdx].advChosePathB) {
               adv = 2;
            } else {
               adv = 1;
            }
            if (gPartyMembers[partyIdx].advLevelSecond != 0) {
               adv += 2;
            }
         }
         gSpriteStripUnitIds[i] += (adv * 6);
      } // 151..274

      if (id >= 1 && id <= 60) {
         adv = 0;
         partyIdx = (id - 1) % 12 + 1;
         if (gPartyMembers[partyIdx].advLevelFirst != 0) {
            if (gPartyMembers[partyIdx].advChosePathB) {
               adv = 2;
            } else {
               adv = 1;
            }
            if (gPartyMembers[partyIdx].advLevelSecond != 0) {
               adv += 2;
            }
         }
         gSpriteStripUnitIds[i] += (adv * 12);
      } // 1..60
   }

   for (i = 0; i < ARRAY_COUNT(gCurrentUnitSet); i++) {
      s16 tmp;
      id = gSpriteStripUnitIds[i];
      tmp = id;
      if (id >= 151 && id <= 300) {
         gCurrentUnitSet[i] = tmp;
      }
      if (id <= 60) {
         gCurrentUnitSet[i] = gSpriteStripUnitIds[i];
      }
   }
}

void SetDefaultStatsForParty(void) {
   s32 i, j;
   u8 level;
   s32 adv1, adv2;

   if (rand() % 2 != 0) {
      adv1 = 1;
      adv2 = 0;
   } else {
      adv1 = 0;
      adv2 = 1;
   }

   for (i = 1; i < PARTY_CT; i++) {
      level = gPartyDefaultLevels[i][gState.mapNum];

      for (j = 0; j < 8; j++) {
         gPartyMembers[i].experience[j] = gExperienceLevels[level - 1][j];
      }

      gPartyMembers[i].advChosePathB = 0;
      gPartyMembers[i].advLevelFirst = 0;
      gPartyMembers[i].advLevelSecond = 0;
      gPartyMembers[i].items[0] = ITEM_NULL;
      gPartyMembers[i].items[1] = ITEM_NULL;
      gPartyMembers[i].hpVar100 = 100;
      gPartyMembers[i].atkVar100 = 100;
      gPartyMembers[i].defVar100 = 100;
      gPartyMembers[i].agiVar100 = 100;

      do {
         // Vary party advancements
         if (i % 2 != 0) {
            gPartyMembers[i].advChosePathB = adv1;
         } else {
            gPartyMembers[i].advChosePathB = adv2;
         }
      } while (0);

      if (i == UNIT_ASH) {
         gPartyMembers[1].advChosePathB = 0;
      }
      if (level >= 10) {
         gPartyMembers[i].advLevelFirst = 10;
      }
      if (level >= 20) {
         gPartyMembers[i].advLevelSecond = 20;
      }
   }
}

void PopulateUnitSpellList(UnitStatus *unit) {
   s32 i;
   s32 partyIdx;
   s16 requiredLevel;

   if (unit->name < PARTY_CT && unit->name != UNIT_NULL) {
      partyIdx = unit->name;
      for (i = 0; i < ARRAY_COUNT(unit->spells); i++) {
         unit->spells[i] = gSpellLists[partyIdx][gPartyMembers[partyIdx].advChosePathB][i];
      }
      for (i = 0; i < ARRAY_COUNT(unit->spells); i++) {
         requiredLevel = gSpellLevelRequirement[unit->spells[i]];
         if (unit->level < requiredLevel) {
            unit->spells[i] = SPELL_NULL;
         } else {
            // Weird. This doesn't match as an else-if, unless nested in a sub-block.
            // Even weirder: while experimenting with variations, at some point removing the braces
            // still matched, but only with a line-break! Hopefully this kind of issue is rare.
            if ((requiredLevel > 20 && gPartyMembers[partyIdx].advLevelSecond == 0) ||
                (requiredLevel > 10 && gPartyMembers[partyIdx].advLevelFirst == 0)) {
               unit->spells[i] = SPELL_NULL;
            }
         }
      }
   }
}

void SetupPartyBattleUnit(u8 partyIdx, u8 z, u8 x, u8 direction) {
   UnitStatus *unit;
   EvtData *battler;
   s32 tmp;
   s32 i;
   u8 level;
   s32 class;

   if (gState.mapNum > 5 || gPartyMembers[partyIdx].inParty) {
      unit = CreateUnitInNextSlot();

      battler = Evt_GetUnused();
      unit->evtBattler = battler;
      battler->functionIndex = EVTF_BATTLE_UNIT;
      battler->d.evtf014.unit = unit;
      battler->d.evtf014.unitIdx = unit->idx;
      battler->d.evtf014.team = TEAM_PLAYER;

      unit->stripIdx = partyIdx + 1;
      unit->name = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].name;
      unit->unitType = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].unitType;
      unit->class = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].class;
      unit->step = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].step;
      unit->travelRange = gTravelRange[unit->step];
      unit->attackRange = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].attackRange;
      unit->ailmentSusceptibility =
          gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].ailmentSusceptibility;
      unit->magicSusceptibility = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].magicSusceptibility;
      unit->advantage = gUnitInfo[gSpriteStripUnitIds[partyIdx - 1]].advantage;
      unit->unitId = gSpriteStripUnitIds[partyIdx - 1];
      tmp = direction << 10; //?
      unit->direction = tmp;

      unit->weapon = gPartyMembers[partyIdx].weapon;
      unit->helmet = gPartyMembers[partyIdx].helmet;
      unit->armor = gPartyMembers[partyIdx].armor;
      unit->items[0] = gPartyMembers[partyIdx].items[0];
      unit->items[1] = gPartyMembers[partyIdx].items[1];
      unit->droppedItem = ITEM_NULL;

      unit->hpVar100 = gPartyMembers[partyIdx].hpVar100;
      unit->atkVar100 = gPartyMembers[partyIdx].atkVar100;
      unit->defVar100 = gPartyMembers[partyIdx].defVar100;
      unit->agiVar100 = gPartyMembers[partyIdx].agiVar100;

      level = 0;
      while (BigIntCompare(gPartyMembers[partyIdx].experience, gExperienceLevels[level]) < 2) {
         level++;
      }
      unit->level = level;

      for (i = 0; i < 8; i++) {
         unit->experience[i] = gPartyMembers[partyIdx].experience[i];
      }

      class = unit->class;
      if (unit->name == UNIT_ASH) {
         class = 7;
      }
      unit->mp = gClassMpMultiplier[class] * unit->level;
      if (unit->class == CLASS_MONK) {
         //? I guess monks can get a small mp boost by delaying advancement?
         unit->mp += gPartyMembers[partyIdx].advLevelFirst;
      }
      if (unit->mp > 99) {
         unit->mp = 99;
      }
      if (gState.debug) {
         unit->mp = 99;
      }
      if (gState.useDefaultUnits) {
         unit->weapon = gClassDefaultLoadouts[unit->class][0][gState.mapNum];
         unit->helmet = gClassDefaultLoadouts[unit->class][1][gState.mapNum];
         unit->armor = gClassDefaultLoadouts[unit->class][2][gState.mapNum];
      }
      unit->maxMp = unit->mp;
      PopulateUnitSpellList(unit);
      battler->d.evtf014.x = (x << 8) + 0x80;
      battler->d.evtf014.z = (z << 8) + 0x80;
   }
}

void SyncPartyUnits(u8 partyIdx) {
   UnitStatus *unit;
   s32 i;
   u8 level;
   s32 class;
   s32 unitId;

   unit = &gUnits[partyIdx];
   unit->idx = partyIdx;
   unit->team = TEAM_PLAYER;

   level = 0;
   while (BigIntCompare(gPartyMembers[partyIdx].experience, gExperienceLevels[level]) < 2) {
      level++;
   }
   unit->level = level;

   for (i = 0; i < 8; i++) {
      unit->experience[i] = gPartyMembers[partyIdx].experience[i];
   }

   if (gPartyMembers[partyIdx].advLevelSecond != 0 && level >= 20) {
      unitId = gPartyUnitIds[partyIdx][gPartyMembers[partyIdx].advChosePathB][2];
   } else if (gPartyMembers[partyIdx].advLevelFirst != 0 && level >= 10) {
      unitId = gPartyUnitIds[partyIdx][gPartyMembers[partyIdx].advChosePathB][1];
   } else {
      unitId = gPartyUnitIds[partyIdx][gPartyMembers[partyIdx].advChosePathB][0];
   }

   unit->name = gUnitInfo[unitId].name;
   unit->unitType = gUnitInfo[unitId].unitType;
   unit->class = gUnitInfo[unitId].class;
   unit->unitId = unitId;
   unit->weapon = gPartyMembers[partyIdx].weapon;
   unit->helmet = gPartyMembers[partyIdx].helmet;
   unit->armor = gPartyMembers[partyIdx].armor;
   unit->items[0] = gPartyMembers[partyIdx].items[0];
   unit->items[1] = gPartyMembers[partyIdx].items[1];
   unit->droppedItem = ITEM_NULL;
   unit->hpVar100 = gPartyMembers[partyIdx].hpVar100;
   unit->atkVar100 = gPartyMembers[partyIdx].atkVar100;
   unit->defVar100 = gPartyMembers[partyIdx].defVar100;
   unit->agiVar100 = gPartyMembers[partyIdx].agiVar100;

   class = unit->class;
   if (unit->name == UNIT_ASH) {
      class = 7;
   }
   unit->mp = gClassMpMultiplier[class] * unit->level;
   if (unit->class == CLASS_MONK) {
      unit->mp += gPartyMembers[partyIdx].advLevelFirst;
   }
   if (unit->mp > 99) {
      unit->mp = 99;
   }
   unit->maxMp = unit->mp;
   PopulateUnitSpellList(unit);
   unit->hpFrac = 10000;
   CalculateUnitStats(unit);
}

#undef EVTF
#define EVTF 424
void Evtf424_BattleEnder(EvtData *evt) {
   EvtData *newEvt;

   switch (evt->state) {
   case 0:
      if (gState.debug && (gPadStateNewPresses & PAD_START)) {
         CommitPartyStatus();
         EVT.timer = 1;
         evt->state = 3;
         evt->state2 = 3;
      }
      if (gState.battleEval == BATTLE_EVAL_NONE) {
         break;
      }
      if (gState.battleEval == BATTLE_EVAL_DEFEAT) {
         evt->state = 2;
      }
      if (gState.battleEval == BATTLE_EVAL_VICTORY) {
         evt->state = 1;
      }
      break;

   case 1:
      gClearSavedPadState = 1;

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x20);
         PerformAudioCommand(0x30);
         EVT.timer = 20;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            EVT.timer = 4;
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            LoadSeqSet(1);
            FinishLoadingSeq();
            PerformAudioCommand(0x217);
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_BATTLE_VICTORY;
            evt->state += 2;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 2:
      gClearSavedPadState = 1;

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x21);
         EVT.timer = 30;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            EVT.timer = 4;
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            LoadSeqSet(1);
            FinishLoadingSeq();
            PerformAudioCommand(0x218);
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_BATTLE_DEFEAT;
            evt->state += 2;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 3:

      switch (evt->state2) {
      case 0:
         gSignal2 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (gSignal2 != 0) {
            evt->state2++;
         }
         break;

      case 2:
         if (gPadStateNewPresses & PAD_CIRCLE) {
            PerformAudioCommand(0x21);
            FadeOutScreen(2, 4);
            EVT.timer = 75;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;

            switch (gState.mapNum) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
               SwapOutCodeToVram();
               gState.primary = STATE_29;
               break;

            case 10:
               gState.primary = STATE_18;
               gState.scene = 2;
               break;

            case 11:
               gState.primary = STATE_12;
               gState.scene = 5;
               break;

            case 12:
               gState.primary = STATE_18;
               gState.scene = 9;
               break;

            case 13:
               gState.primary = STATE_18;
               gState.scene = 0xc;
               break;

            case 14:
               gState.primary = STATE_19;
               gState.scene = 0x10;
               break;

            case 15:
               gState.primary = STATE_18;
               gState.scene = 0x12;
               break;

            case 16:
               gState.primary = STATE_12;
               gState.scene = 7;
               break;

            case 17:
               gState.primary = STATE_12;
               gState.scene = 0x16;
               break;

            case 18:
               gState.primary = STATE_12;
               gState.scene = 0x19;
               break;

            case 19:
               gState.primary = STATE_12;
               gState.scene = 0x1c;
               break;

            case 20:
               gState.primary = STATE_12;
               gState.scene = 0x1e;
               break;

            case 21:
               gState.primary = STATE_19;
               gState.scene = 0x21;
               break;

            case 22:
               gState.primary = STATE_18;
               gState.scene = 0x24;
               break;

            case 23:
               gState.primary = STATE_18;
               gState.scene = 0x27;
               break;

            case 24:
               gState.primary = STATE_12;
               gState.scene = 0x29;
               break;

            case 25:
               gState.primary = STATE_12;
               gState.scene = 0x2c;
               break;

            case 26:
               gState.worldMapDestination = 0xf;
               SwapOutCodeToVram();
               gState.primary = STATE_6;
               gState.worldMapState = 0x2b;
               break;

            case 27:
               gState.primary = STATE_12;
               gState.scene = 0x31;
               break;

            case 28:
               gState.primary = STATE_12;
               gState.scene = 0x32;
               break;

            case 29:
               gState.primary = STATE_12;
               gState.scene = 0x36;
               break;

            case 30:
               gState.primary = STATE_12;
               gState.scene = 0x38;
               break;

            case 31:
               gState.primary = STATE_12;
               gState.scene = 0x3d;
               break;

            case 32:
               gState.primary = STATE_12;
               gState.scene = 0x40;
               break;

            case 33:
               gState.primary = STATE_12;
               gState.scene = 0x42;
               break;

            case 34:
               gState.worldMapDestination = 0x16;
               SwapOutCodeToVram();
               gState.primary = STATE_6;
               gState.worldMapState = 0x3d;
               break;

            case 35:
               gState.primary = STATE_12;
               gState.scene = 0x48;
               break;
            case 36:
               gState.primary = STATE_12;
               gState.scene = 0x4b;
               break;

            case 37:
               gState.worldMapDestination = 0x1b;
               SwapOutCodeToVram();
               gState.primary = STATE_6;
               gState.worldMapState = 0x42;
               break;

            case 38:
               gState.primary = STATE_12;
               gState.scene = 0x4f;
               break;

            case 39:
               gState.primary = STATE_12;
               gState.scene = 0x53;
               break;

            case 40:
               gState.worldMapDestination = 0x1e;
               SwapOutCodeToVram();
               gState.primary = STATE_6;
               gState.worldMapState = 0x48;
               break;

            case 41:
               gState.primary = STATE_12;
               gState.scene = 0x56;
               break;

            case 42:
               gState.primary = STATE_12;
               gState.scene = 0x5b;
               break;

            case 43:
               gState.primary = STATE_12;
               gState.scene = 0x5d;
               break;
            } // switch (gState.mapNum)
         }    // timer

         break;
      } // switch (evt->state2) (via state:3)

      break;

   case 4:

      switch (evt->state2) {
      case 0:
         gSignal2 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (gSignal2 != 0) {
            evt->state2++;
         }
         break;

      case 2:
         if (PressedCircleOrX()) {
            PerformAudioCommand(0x21);
            EVT.timer = 90;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            gPlayerControlSuppressed = 1;
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_FILE_LOAD_DIALOG_376;
            evt->state2++;
         }
         break;

      case 4:
         break;
      }

      break;
   }
}

void func_8005AA7C(void) {
   s32 i;

   for (i = 0; i < ARRAY_COUNT(gSpriteStripUnitIds); i++) {
      gSpriteStripUnitIds[i] = 0;
   }

   if (gState.mapNum < BATTLE_CT) {
      for (i = 0; i < 20; i++) {
         gSpriteStripUnitIds[i] = gSceneSpriteStripUnitIds[gBattleSceneId2[gState.mapNum]][i];
      }
      for (i = 0; i < 20; i++) {
         gCurrentUnitSet[i] = gSceneUnitSets[gBattleSceneId2[gState.mapNum]][i];
      }
   }

   for (i = 0; i < PARTY_CT - 1; i++) {
      if (gSpriteStripUnitIds[i] == i + 1) {
         if (gState.mapNum > 5) {
            gPartyMembers[gSpriteStripUnitIds[i]].inParty = 1;
         }
      } else {
         gPartyMembers[i + 1].inParty = 0;
      }
   }

   if (gState.mapNum != 8) {
      SetupPartySprites();
   }
}

void SetupBattleUnit(s16 stripIdx, u8 z, u8 x, s8 level, u8 team, u8 direction, u8 to_x1c,
                     u8 expMulti, u8 to_x1d) {
   extern UnitStatus *gTempUnitPtr;

   EvtData *battler;
   s32 i;

   if (gSpriteStripUnitIds[stripIdx] == UNIT_ID_LEENA) {
      gTempUnitPtr = CreateUnitInLastSlot();
   } else {
      gTempUnitPtr = CreateUnitInNextSlot();
   }

   battler = Evt_GetUnused();
   gTempUnitPtr->evtBattler = battler;
   battler->functionIndex = EVTF_BATTLE_UNIT;
   battler->d.evtf014.unit = gTempUnitPtr;
   battler->d.evtf014.unitIdx = gTempUnitPtr->idx;
   battler->d.evtf014.team = team;

   if (gState.mapNum == 8) {
      // Demo battle
      battler->d.evtf014.team = (rand() % 70) + 2;
   }
   if (gState.mapNum == 9 && x > 8) {
      // Test battle
      battler->d.evtf014.team = TEAM_PLAYER;
   }

   gTempUnitPtr->stripIdx = stripIdx + 2;
   gTempUnitPtr->name = gUnitInfo[gSpriteStripUnitIds[stripIdx]].name;
   gTempUnitPtr->unitType = gUnitInfo[gSpriteStripUnitIds[stripIdx]].unitType;
   if (gTempUnitPtr->unitType == UNIT_TYPE_MAGE_TOWER ||
       (gTempUnitPtr->name == 30 || gTempUnitPtr->name == 31)) {
      battler->d.evtf014.team = TEAM_PLAYER;
   }

   gTempUnitPtr->class = gUnitInfo[gSpriteStripUnitIds[stripIdx]].class;
   gTempUnitPtr->weapon = gUnitInfo[gSpriteStripUnitIds[stripIdx]].weapon;
   gTempUnitPtr->helmet = gUnitInfo[gSpriteStripUnitIds[stripIdx]].helmet;
   gTempUnitPtr->armor = gUnitInfo[gSpriteStripUnitIds[stripIdx]].armor;
   gTempUnitPtr->step = gUnitInfo[gSpriteStripUnitIds[stripIdx]].step;
   gTempUnitPtr->travelRange = gTravelRange[gTempUnitPtr->step];
   gTempUnitPtr->attackRange = gUnitInfo[gSpriteStripUnitIds[stripIdx]].attackRange;
   gTempUnitPtr->ailmentSusceptibility =
       gUnitInfo[gSpriteStripUnitIds[stripIdx]].ailmentSusceptibility;
   gTempUnitPtr->magicSusceptibility = gUnitInfo[gSpriteStripUnitIds[stripIdx]].magicSusceptibility;
   gTempUnitPtr->advantage = gUnitInfo[gSpriteStripUnitIds[stripIdx]].advantage;
   gTempUnitPtr->unitId = gSpriteStripUnitIds[stripIdx];
   // tmp = direction << 10;
   gTempUnitPtr->direction = direction << 10;
   gTempUnitPtr->field7_0x1c = to_x1c; // TBD: involved in ai behavior; related to travel range
   gTempUnitPtr->mp = gUnitInfo[gSpriteStripUnitIds[stripIdx]].maxMp;
   gTempUnitPtr->maxMp = gUnitInfo[gSpriteStripUnitIds[stripIdx]].maxMp;

   for (i = 0; i < ARRAY_COUNT(gTempUnitPtr->spells); i++) {
      gTempUnitPtr->spells[i] = SPELL_NULL;
   }
   gTempUnitPtr->spells[0] = gUnitInfo[gSpriteStripUnitIds[stripIdx]].spell1;
   gTempUnitPtr->spells[1] = gUnitInfo[gSpriteStripUnitIds[stripIdx]].spell2;
   gTempUnitPtr->items[0] = ITEM_NULL;
   gTempUnitPtr->items[1] = ITEM_NULL;
   gTempUnitPtr->droppedItem = gUnitInfo[gSpriteStripUnitIds[stripIdx]].heldItem;

   if (gState.mapNum < 9) {
      gTempUnitPtr->droppedItem = ITEM_NULL;
   }

   if (gState.mapNum == 8) {
      // Demo battle
      gTempUnitPtr->spells[0] = SPELL_NULL;
      gTempUnitPtr->spells[1] = SPELL_NULL;
      gTempUnitPtr->mp = 50;
      gTempUnitPtr->maxMp = 50;

      if (gTempUnitPtr->unitId == UNIT_ID_DARK_MAGE) {
         gTempUnitPtr->spells[0] = SPELL_DARK_HURRICANE;
      }
      if (gTempUnitPtr->unitId == UNIT_ID_ELENI_MAGE) {
         gTempUnitPtr->spells[0] = SPELL_SALAMANDER;
      }
      if (gTempUnitPtr->unitId == UNIT_ID_ZOHAR_SORCEROR) {
         gTempUnitPtr->spells[0] = SPELL_AVALANCHE;
      }
      if (gTempUnitPtr->unitId == UNIT_ID_DUMAS) {
         gTempUnitPtr->spells[0] = SPELL_PLASMA_WAVE;
      }
   }

   gTempUnitPtr->hpVar100 = 100;
   gTempUnitPtr->atkVar100 = 100;
   gTempUnitPtr->defVar100 = 100;
   gTempUnitPtr->agiVar100 = 100;
   gTempUnitPtr->field8_0x1d = to_x1d; // TBD: involved in ai behavior; related to ally count
   gTempUnitPtr->expMulti = expMulti;

   if (gState.mapNum <= 5) {
      // Trials of Toroah
      for (i = 0; i < 8; i++) {
         gTempUnitPtr->experience[i] = gPartyMembers[UNIT_ASH].experience[i];
      }
      i = 0;
      while (BigIntCompare(gTempUnitPtr->experience, gExperienceLevels[i]) < 2) {
         i++;
      }
      gTempUnitPtr->level = i;
   } else {
      for (i = 0; i < 8; i++) {
         gTempUnitPtr->experience[i] = gExperienceLevels[level - 1][i];
      }
      gTempUnitPtr->level = level;
   }

   if (gState.debug && gTempUnitPtr->name <= UNIT_END_OF_PARTY) {
      gTempUnitPtr->mp = 99;
      gTempUnitPtr->maxMp = 99;
   }

   battler->d.evtf014.x = (x << 8) + 0x80;
   battler->d.evtf014.z = (z << 8) + 0x80;
}

void CreateBattleUnitForUnit(UnitStatus *unit) {
   EvtData *battler;

   battler = Evt_GetUnused();
   unit->evtBattler = battler;
   battler->functionIndex = EVTF_BATTLE_UNIT;
   battler->d.evtf014.unit = unit;
   battler->d.evtf014.unitIdx = unit->idx;
   battler->d.evtf014.team = unit->team;
   HI(battler->d.evtf014.x) = unit->tileX;
   HI(battler->d.evtf014.z) = unit->tileZ;
   LO(battler->d.evtf014.x) = 0x80;
   LO(battler->d.evtf014.z) = 0x80;
}

void SetupBattleUnits(void) {
   BattleEnemyUnitInitialState *pEnemyInit;
   BattlePartyUnitInitialState *pPartyInit;

   pEnemyInit = &gBattleEnemyUnitInitialStates[gState.mapNum][0];
   while (pEnemyInit->stripIdx != 0) {
      SetupBattleUnit(pEnemyInit->stripIdx, pEnemyInit->z, pEnemyInit->x, pEnemyInit->level,
                      TEAM_ENEMY, pEnemyInit->direction, pEnemyInit->to_x1c, pEnemyInit->expMulti,
                      pEnemyInit->to_x1d);
      pEnemyInit++;
   }

   pPartyInit = &gBattlePartyUnitInitialStates[gState.mapNum][0];
   while (pPartyInit->partyIdx != UNIT_INVALID) {
      SetupPartyBattleUnit(pPartyInit->partyIdx, pPartyInit->z, pPartyInit->x,
                           pPartyInit->direction);
      pPartyInit++;
   }
}

s32 PCDrv_WriteFile(s8 *filename, void *buf, s32 size) {
   const s32 CHUNK_SIZE = 0xfffe;
   s32 fd;
   s32 offset;
   s32 chunks;
   s32 rem;
   u8 *p;

   fd = PCcreat(filename, 0);
   PClseek(fd, 0, 0);

   chunks = size / CHUNK_SIZE;
   rem = size % CHUNK_SIZE;
   offset = 0;
   p = (u8 *)buf;

   while (chunks > 0) {
      PClseek(fd, offset, 0);
      PCwrite(fd, p, CHUNK_SIZE);
      offset += CHUNK_SIZE;
      p += CHUNK_SIZE;
      chunks--;
   }

   if (rem != 0) {
      PCwrite(fd, p, rem);
   }

   PCclose(fd);
   return 0;
}

void ResetGeomOffset(void) {
   gGeomOffsetX = SCREEN_HALF_WIDTH;
   gGeomOffsetY = SCREEN_HALF_HEIGHT;
   SetGeomOffset(gGeomOffsetX, gGeomOffsetY);
}

void Initialize(void) {
   MATRIX localColorMatrix = {{{ONE, 0, 0}, {ONE, 0, 0}, {ONE, 0, 0}}};
   s32 i;

   ResetCallback();
   PadInit(0);
   Card_Init();
   CDInit();
   ResetGraph(0);
   SetGraphDebug(0);
   InitGeom();
   ResetGeomOffset();
   SetGeomScreen(0x200);
   SetBackColor(0, 0, 0);
   SetColorMatrix(&localColorMatrix);
   SetDefDrawEnv(&gGraphicBuffers[0].drawEnv, 0, 16, SCREEN_WIDTH, SCREEN_HEIGHT);
   SetDefDrawEnv(&gGraphicBuffers[1].drawEnv, 0, 272, SCREEN_WIDTH, SCREEN_HEIGHT);
   SetDefDispEnv(&gGraphicBuffers[0].dispEnv, 0, 272, SCREEN_WIDTH, SCREEN_HEIGHT);
   SetDefDispEnv(&gGraphicBuffers[1].dispEnv, 0, 16, SCREEN_WIDTH, SCREEN_HEIGHT);
   gGraphicBuffers[0].drawEnv.dtd = gGraphicBuffers[1].drawEnv.dtd = 0;
   gGraphicBuffers[0].drawEnv.isbg = gGraphicBuffers[1].drawEnv.isbg = 0;
   gGraphicBuffers[0].dispEnv.isinter = gGraphicBuffers[1].dispEnv.isinter = 0;
   gGraphicBuffers[0].dispEnv.isrgb24 = gGraphicBuffers[1].dispEnv.isrgb24 = 0;
   gTempRect.x = 0;
   gTempRect.y = 0;
   gTempRect.w = VRAM_WIDTH;
   gTempRect.h = VRAM_HEIGHT;
   ClearImage(&gTempRect, 0, 0, 0);
   DrawSync(0);
   SetupQuads(&gGraphicBuffers[0]);
   SetupQuads(&gGraphicBuffers[1]);

   for (i = 9; i != -1; i--) {
      gGraphicsPtr =
          (gGraphicsPtr == &gGraphicBuffers[0]) ? &gGraphicBuffers[1] : &gGraphicBuffers[0];
      ClearOTag(gGraphicsPtr->ot, OT_SIZE);
      gTempRect.x = 0;
      gTempRect.y = 0;
      gTempRect.w = VRAM_WIDTH;
      gTempRect.h = VRAM_HEIGHT;
      ClearImage(&gTempRect, 0, 0, 0);
      DrawSync(0);
      VSync(0);
      PutDrawEnv(&gGraphicsPtr->drawEnv);
      PutDispEnv(&gGraphicsPtr->dispEnv);
      DrawOTag(gGraphicsPtr->ot);
   }
}
