#include "common.h"
#include "card.h"
#include "evt.h"
#include "graphics.h"
#include "state.h"
#include "cd_files.h"
#include "audio.h"
#include "units.h"
#include "battle.h"

void ShowFileSaveScreen(void) {
   EvtData *evt;

   switch (gState.secondary) {
   case 0:
      Evt_ResetFromIdx10();
      LoadFullscreenImage(CDF_US_LOAD_TIM);
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_FULLSCREEN_IMAGE;
      gState.secondary++;
      break;
   case 1:
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FILE_SAVE_DIALOG;
      gState.D_8014053E = 0;
      gState.secondary++;
      break;
   case 2:
      if (gState.D_8014053E != 0) {
         gState.primary = gState.state6;
         gState.secondary = 0;
         gState.state3 = 0;
         gState.state4 = 0;
      }
      break;
   }
}

void ShowFileLoadScreen(void) {
   EvtData *evt;

   switch (gState.secondary) {
   case 0:
      Evt_ResetFromIdx10();
      LoadFullscreenImage(CDF_US_LOAD_TIM);
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_FULLSCREEN_IMAGE;
      gState.secondary++;
      break;
   case 1:
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FILE_LOAD_DIALOG;
      gState.D_8014053E = 0;
      gState.secondary++;
      break;
   case 2:
      if (gState.D_8014053E != 0) {
         gState.primary = gState.state6;
         gState.secondary = 0;
         gState.state3 = 0;
         gState.state4 = 0;
      }
      break;
   }
}

void Card_PopulateRegularSave(void) {
   s32 i;

   for (i = 0; i < PARTY_CT; i++) {
      gRegularSave.party[i] = gPartyMembers[i];
   }
   for (i = 0; i < 150; i++) {
      gRegularSave.depot[i] = gState.depot[i];
   }

   gRegularSave.frameCounter = gState.frameCounter;
   gRegularSave.gold = gState.gold;
   gRegularSave.textSpeed = gState.textSpeed;
   gRegularSave.cameraMode = gState.cameraMode;
   gRegularSave.mono = gState.mono;
   gRegularSave.worldMapState = gState.worldMapState;
   gRegularSave.townState = gState.townState;
   gRegularSave.worldMapDestination = gState.worldMapDestination;
   gRegularSave.saveLocation = gState.saveLocation;
   gRegularSave.chapter = gState.chapter;
   gRegularSave.section = gState.section;
   gRegularSave.debug = gState.debug;
}

void Card_LoadRegularSaveFromBuf(void) {
   s32 i;

   for (i = 0; i < PARTY_CT; i++) {
      gPartyMembers[i] = gRegularSave.party[i];
   }
   for (i = 0; i < 150; i++) {
      gState.depot[i] = gRegularSave.depot[i];
   }

   gState.frameCounter = gRegularSave.frameCounter;
   gState.gold = gRegularSave.gold;
   gState.textSpeed = gRegularSave.textSpeed;
   gState.cameraMode = gRegularSave.cameraMode;
   gState.mono = gRegularSave.mono;
   gState.worldMapState = gRegularSave.worldMapState;
   gState.townState = gRegularSave.townState;
   gState.worldMapDestination = gRegularSave.worldMapDestination;
   gState.saveLocation = gRegularSave.saveLocation;
   gState.chapter = gRegularSave.chapter;
   gState.section = gRegularSave.section;
   gState.debug = gRegularSave.debug;
}

void Card_LoadRegularSave_Internal(void) {
   Card_LoadRegularSaveFromBuf();
   PerformAudioCommand(!gState.mono ? 2 : 1);

   switch (gState.saveLocation) {
   case SAVE_LOC_UNK:
      gState.primary = 10;
      break;
   case SAVE_LOC_BATTLE:
      gState.primary = 0x1f;
      break;
   case SAVE_LOC_WORLD_MAP:
      gState.primary = 6;
      ClearPortraitSet();
      LoadPortraits();
      break;
   case SAVE_LOC_TOWN:
      gState.primary = 7;
      break;
   }

   gState.secondary = 0;
   gState.state3 = 0;
   gState.state4 = 0;
}

void CommitPartyMemberStatus(UnitStatus *status) {
   s32 idx, i;
   idx = status->name;

   if (idx < PARTY_CT) {
      gPartyMembers[idx].helmet = status->helmet;
      gPartyMembers[idx].armor = status->armor;
      gPartyMembers[idx].weapon = status->weapon;
      gPartyMembers[idx].item1 = status->item1;
      gPartyMembers[idx].item2 = status->item2;

      for (i = 0; i < 8; i++) {
         gPartyMembers[idx].experience[i] = status->experience[i];
      }

      gPartyMembers[idx].hpVar100 = status->hpVar100;
      gPartyMembers[idx].atkVar100 = status->atkVar100;
      gPartyMembers[idx].defVar100 = status->defVar100;
      gPartyMembers[idx].agiVar100 = status->agiVar100;
   }
}

void Card_LoadInBattleSaveFromBuf(void) {
   // Loads some data immediately (gPartyMembers, gUnits) and saves rest into
   // gDeferredInBattleSaveData for deferred loading (80040bf8, 800183e0, 80051a84, ...)
   s32 i;

   for (i = 0; i < PARTY_CT; i++) {
      gPartyMembers[i] = gInBattleSaveDataPtr->party[i];
   }

   for (i = 0; i < UNIT_CT; i++) {
      gUnits[i] = gInBattleSaveDataPtr->units[i];
   }

   for (i = 0; i < 20; i++) {
      gDeferredInBattleSaveData.chests[i].z = gInBattleSaveDataPtr->chests[i].z;
      gDeferredInBattleSaveData.chests[i].x = gInBattleSaveDataPtr->chests[i].x;
      gDeferredInBattleSaveData.chests[i].item = gInBattleSaveDataPtr->chests[i].item;
   }
   for (i = 0; i < 10; i++) {
      gDeferredInBattleSaveData.crates[i].z = gInBattleSaveDataPtr->crates[i].z;
      gDeferredInBattleSaveData.crates[i].x = gInBattleSaveDataPtr->crates[i].x;
      gDeferredInBattleSaveData.boulders[i].z = gInBattleSaveDataPtr->boulders[i].z;
      gDeferredInBattleSaveData.boulders[i].x = gInBattleSaveDataPtr->boulders[i].x;
   }

   gDeferredInBattleSaveData.worldMapState = gInBattleSaveDataPtr->worldMapState;
   gDeferredInBattleSaveData.townState = gInBattleSaveDataPtr->townState;
   gDeferredInBattleSaveData.worldMapDestination = gInBattleSaveDataPtr->worldMapDestination;
   gDeferredInBattleSaveData.frameCounter = gInBattleSaveDataPtr->frameCounter;
   gDeferredInBattleSaveData.chapter = gInBattleSaveDataPtr->chapter;
   gDeferredInBattleSaveData.section = gInBattleSaveDataPtr->section;

   for (i = 0; i < 20; i++) {
      gDeferredInBattleSaveData.mapState[i] = gInBattleSaveDataPtr->mapState[i];
   }

   gDeferredInBattleSaveData.turn = gInBattleSaveDataPtr->turn;
   gDeferredInBattleSaveData.battle = gInBattleSaveDataPtr->battle;

   for (i = 0; i < 150; i++) {
      gDeferredInBattleSaveData.depot[i] = gInBattleSaveDataPtr->depot[i];
   }

   gDeferredInBattleSaveData.gold = gInBattleSaveDataPtr->gold;
   gDeferredInBattleSaveData.zoom = gInBattleSaveDataPtr->zoom;
   gDeferredInBattleSaveData.lastSelectedUnit = gInBattleSaveDataPtr->lastSelectedUnit;
   gDeferredInBattleSaveData.mono = gInBattleSaveDataPtr->mono;
   gDeferredInBattleSaveData.cameraMode = gInBattleSaveDataPtr->cameraMode;
   gDeferredInBattleSaveData.textSpeed = gInBattleSaveDataPtr->textSpeed;

   for (i = 0; i < 20; i++) {
      gDeferredInBattleSaveData.slainUnits[i].unitId = gInBattleSaveDataPtr->slainUnits[i].unitId;
      gDeferredInBattleSaveData.slainUnits[i].count = gInBattleSaveDataPtr->slainUnits[i].count;
      gDeferredInBattleSaveData.slainUnits[i].reward = gInBattleSaveDataPtr->slainUnits[i].reward;
   }

   for (i = 0; i < 12; i++) {
      gDeferredInBattleSaveData.partyMemberSlain[i] = gInBattleSaveDataPtr->partyMemberSlain[i];
   }

   gDeferredInBattleSaveData.mapCursorX = gInBattleSaveDataPtr->mapCursorX;
   gDeferredInBattleSaveData.mapCursorZ = gInBattleSaveDataPtr->mapCursorZ;
   gDeferredInBattleSaveData.cameraRotationX = gInBattleSaveDataPtr->cameraRotationX;
   gDeferredInBattleSaveData.cameraRotationY = gInBattleSaveDataPtr->cameraRotationY;
   gDeferredInBattleSaveData.debug = gInBattleSaveDataPtr->debug;
}

void Card_PopulateInBattleSave(void) {
   s32 i;
   EvtData *evt;
   u8 numChests, numCrates, numBoulders;

   for (i = 0; i < PARTY_CT; i++) {
      gInBattleSaveDataPtr->party[i] = gPartyMembers[i];
   }

   for (i = 0; i < UNIT_CT; i++) {
      if (gUnits[i].idx) {
         evt = gUnits[i].evtSprite;
         gUnits[i].tileX = TO_TILE(evt->d.sprite.x1);
         gUnits[i].tileZ = TO_TILE(evt->d.sprite.z1);
      }
      gInBattleSaveDataPtr->units[i] = gUnits[i];
   }

   gInBattleSaveDataPtr->worldMapState = gState.worldMapState;
   gInBattleSaveDataPtr->worldMapDestination = gState.worldMapDestination;
   gInBattleSaveDataPtr->townState = gState.townState;
   gInBattleSaveDataPtr->frameCounter = gState.frameCounter;
   gInBattleSaveDataPtr->chapter = gState.chapter;
   gInBattleSaveDataPtr->section = gState.section;

   for (i = 0; i < 20; i++) {
      gInBattleSaveDataPtr->mapState[i] = gState.mapState.bytes[i];
   }

   gInBattleSaveDataPtr->turn = gState.turn;
   gInBattleSaveDataPtr->battle = gState.battleNum;

   for (i = 0; i < 150; i++) {
      gInBattleSaveDataPtr->depot[i] = gState.depot[i];
   }

   gInBattleSaveDataPtr->gold = gState.gold;
   gInBattleSaveDataPtr->zoom = gState.zoom;
   gInBattleSaveDataPtr->lastSelectedUnit = gState.lastSelectedUnit;
   gInBattleSaveDataPtr->mono = gState.mono;
   gInBattleSaveDataPtr->cameraMode = gState.cameraMode;
   gInBattleSaveDataPtr->textSpeed = gState.textSpeed;

   for (i = 0; i < 20; i++) {
      gInBattleSaveDataPtr->slainUnits[i].unitId = gSlainUnits[i].unitId;
      gInBattleSaveDataPtr->slainUnits[i].count = gSlainUnits[i].count;
      gInBattleSaveDataPtr->slainUnits[i].reward = gSlainUnits[i].reward;
   }

   for (i = 0; i < 12; i++) {
      gInBattleSaveDataPtr->partyMemberSlain[i] = gPartyMemberSlain[i];
   }

   gInBattleSaveDataPtr->mapCursorX = gMapCursorX;
   gInBattleSaveDataPtr->mapCursorZ = gMapCursorZ;
   gInBattleSaveDataPtr->cameraRotationX = gCameraRotation.vx;
   gInBattleSaveDataPtr->cameraRotationY = gCameraRotation_vy;
   gInBattleSaveDataPtr->debug = gState.debug;

   for (i = 0; i < 20; i++) {
      gInBattleSaveDataPtr->chests[i].z = 99;
   }
   for (i = 0; i < 10; i++) {
      gInBattleSaveDataPtr->crates[i].z = 99;
      gInBattleSaveDataPtr->boulders[i].z = 99;
   }

   /* TODO Replace raw byte accesses with proper evtdata (once defined) */

   numChests = 0;
   numCrates = 0;
   numBoulders = 0;
   evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt->functionIndex == EVTF_MAP_OBJECT_CHEST) {
         gInBattleSaveDataPtr->chests[numChests].z = evt->d.bytes[0x7];
         gInBattleSaveDataPtr->chests[numChests].x = evt->d.bytes[0x3];
         gInBattleSaveDataPtr->chests[numChests++].item = evt->d.bytes[0x14];
      }
      if (evt->functionIndex == EVTF_MAP_OBJECT_CRATE) {
         gInBattleSaveDataPtr->crates[numCrates].z = evt->d.bytes[0x7];
         gInBattleSaveDataPtr->crates[numCrates++].x = evt->d.bytes[0x3];
      }
      if (evt->functionIndex == EVTF_MAP_OBJECT_BOULDER) {
         gInBattleSaveDataPtr->boulders[numBoulders].z = evt->d.bytes[0x7];
         gInBattleSaveDataPtr->boulders[numBoulders++].x = evt->d.bytes[0x3];
      }
      evt++;
   }
}

s32 Card_ReadInBattleSaveIntoBuf(void) { return Card_ReadInBattleSaveInto(gInBattleSaveDataPtr); }

s32 Card_WriteInBattleSaveFromBuf(void) { return Card_WriteInBattleSaveFrom(gInBattleSaveDataPtr); }
