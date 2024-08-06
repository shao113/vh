#include "common.h"
#include "card.h"
#include "object.h"
#include "graphics.h"
#include "state.h"
#include "cd_files.h"
#include "audio.h"
#include "units.h"
#include "battle.h"

#include "PsyQ/sys/file.h"
#include "PsyQ/stdio.h"
#include "PsyQ/kernel.h"
#include "PsyQ/memory.h"
#include "PsyQ/strings.h"

extern void InitCard(s32);
extern s32 StartCard(void);
extern void _bu_init(void);
extern s32 _card_info(s32);
extern s32 _card_async_load_directory(s32);
extern s32 _card_clear(s32);
extern s32 FormatDevice(u8 *);
extern s32 FileOpen(u8 *, s32);
extern s32 FileClose(s32);
extern s32 FileRead(s32, void *, s32);
extern s32 FileWrite(s32, void *, s32);
extern s32 FileSeek(s32, s32, s32);
extern struct DIRENTRY *firstfile(u8 *, struct DIRENTRY *);
extern struct DIRENTRY *nextfile(struct DIRENTRY *);
extern s32 TestEvent(s32);

/* .data */
u8 gCardFilePath[] = "bu00:BASLUS-00447VH";
// Could be defined here, but leaving external since it contains image data:
extern CardFileData_Header gCardFileHeader;

/* .sdata (definitions moved further down for proper ordering)
u8 *gCardFilenameBufferPtr;
u8 *gCardFileBufferPtr;
u8 *gCardFileVerifyBufferPtr;
*/

/* .sbss */
s32 gEventSwCardIOE;
s32 gEventSwCardError;
s32 gEventSwCardTimeout;
s32 gEventSwCardNew;
s32 gEventHwCardIOE;
s32 gEventHwCardError;
s32 gEventHwCardTimeout;
s32 gEventHwCardNew;
s16 gCardEventState;
s16 gCardState;

/* .bss? */
// extern struct DIRENTRY s_dirEntry;

typedef struct {
   u8 bytes[0x300];
} RawSaveData;

typedef struct {
   u8 bytes[0x80];
} RawListingData;

typedef struct {
   u8 bytes[0x16cc];
} RawInBattleSaveData;

void State_FileSaveScreen(void) {
   Object *obj;

   switch (gState.secondary) {
   case 0:
      Obj_ResetFromIdx10();
      LoadFullscreenImage(CDF_US_LOAD_TIM);
      gTempObj = Obj_GetUnused();
      gTempObj->functionIndex = OBJF_FULLSCREEN_IMAGE;
      gState.secondary++;
      break;
   case 1:
      obj = Obj_GetUnused();
      obj->functionIndex = OBJF_FILE_SAVE_DIALOG;
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

void State_FileLoadScreen(void) {
   Object *obj;

   switch (gState.secondary) {
   case 0:
      Obj_ResetFromIdx10();
      LoadFullscreenImage(CDF_US_LOAD_TIM);
      gTempObj = Obj_GetUnused();
      gTempObj->functionIndex = OBJF_FULLSCREEN_IMAGE;
      gState.secondary++;
      break;
   case 1:
      obj = Obj_GetUnused();
      obj->functionIndex = OBJF_FILE_LOAD_DIALOG;
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
   for (i = 0; i < DEPOT_CT; i++) {
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
   for (i = 0; i < DEPOT_CT; i++) {
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
   PerformAudioCommand(!gState.mono ? AUDIO_CMD_STEREO : AUDIO_CMD_MONO);

   switch (gState.saveLocation) {
   case SAVE_LOC_UNK:
      gState.primary = STATE_MOVIE;
      break;
   case SAVE_LOC_BATTLE:
      gState.primary = STATE_31;
      break;
   case SAVE_LOC_WORLD_MAP:
      gState.primary = STATE_6;
      ClearPortraitSet();
      LoadPortraits();
      break;
   case SAVE_LOC_TOWN:
      gState.primary = STATE_7;
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
      gPartyMembers[idx].items[0] = status->items[0];
      gPartyMembers[idx].items[1] = status->items[1];

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

   for (i = 0; i < DEPOT_CT; i++) {
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
   Object *obj;
   u8 numChests, numCrates, numBoulders;

   for (i = 0; i < PARTY_CT; i++) {
      gInBattleSaveDataPtr->party[i] = gPartyMembers[i];
   }

   for (i = 0; i < UNIT_CT; i++) {
      if (gUnits[i].idx) {
         obj = gUnits[i].sprite;
         gUnits[i].tileX = obj->x1.s.hi;
         gUnits[i].tileZ = obj->z1.s.hi;
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
   gInBattleSaveDataPtr->battle = gState.mapNum;

   for (i = 0; i < DEPOT_CT; i++) {
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
   gInBattleSaveDataPtr->cameraRotationY = gCameraRotation.vy;
   gInBattleSaveDataPtr->debug = gState.debug;

   for (i = 0; i < 20; i++) {
      gInBattleSaveDataPtr->chests[i].z = 99;
   }
   for (i = 0; i < 10; i++) {
      gInBattleSaveDataPtr->crates[i].z = 99;
      gInBattleSaveDataPtr->boulders[i].z = 99;
   }

   numChests = 0;
   numCrates = 0;
   numBoulders = 0;
   obj = gObjectArray;

   for (i = 0; i < OBJ_DATA_CT; i++) {
      if (obj->functionIndex == OBJF_MAP_OBJECT_CHEST) {
         gInBattleSaveDataPtr->chests[numChests].z = obj->z1.s.hi;
         gInBattleSaveDataPtr->chests[numChests].x = obj->x1.s.hi;
         gInBattleSaveDataPtr->chests[numChests++].item = obj->d.mapObj.param;
      }
      if (obj->functionIndex == OBJF_MAP_OBJECT_CRATE) {
         gInBattleSaveDataPtr->crates[numCrates].z = obj->z1.s.hi;
         gInBattleSaveDataPtr->crates[numCrates++].x = obj->x1.s.hi;
      }
      if (obj->functionIndex == OBJF_MAP_OBJECT_BOULDER) {
         gInBattleSaveDataPtr->boulders[numBoulders].z = obj->z1.s.hi;
         gInBattleSaveDataPtr->boulders[numBoulders++].x = obj->x1.s.hi;
      }
      obj++;
   }
}

s32 Card_ReadInBattleSaveIntoBuf(void) { return Card_ReadInBattleSaveInto(gInBattleSaveDataPtr); }

s32 Card_WriteInBattleSaveFromBuf(void) { return Card_WriteInBattleSaveFrom(gInBattleSaveDataPtr); }

s32 Card_ReadRegularSaveIntoBuf(u8 fileIdx) {
   s32 res;

   res = Card_ReadRegularSaveInto(fileIdx, gRegularSaveDataPtr);
   if (res != 0) {
      return res;
   }
   gRegularSave = *gRegularSaveDataPtr;
   return res;
}

s32 Card_WriteRegularSaveFromBuf(u8 fileIdx) {
   return Card_WriteRegularSaveFrom(fileIdx, &gRegularSave);
}

s32 Card_WriteFileListingFromBuf(void) { return Card_WriteFileListingFrom(gCardFileListingPtr); }

s32 Card_ReadFileListingIntoBuf(void) { return Card_ReadFileListingInto(gCardFileListingPtr); }

s32 Card_WriteRegularSave(u8 fileIdx) {
   Card_PopulateRegularSave();
   return Card_WriteRegularSaveFromBuf(fileIdx);
}

s32 Card_LoadRegularSave(u8 fileIdx) {
   s32 res = Card_ReadRegularSaveIntoBuf(fileIdx);
   if (res == 0) {
      SwapOutCodeToVram();
      Card_LoadRegularSave_Internal();
   }
   return res;
}

void ReloadBattle(void) {
   /* Loads from gRegularSave as prepared by Card_PopulateRegularSave (@800406c0) */
   Card_LoadRegularSave_Internal();
}

s32 Card_LoadInBattleSave(void) {
   s32 res;

   /* TBD: is this redundant? need to check if it's ever actually used;
      Maybe for preparing gRegularSave in lieu of Card_PopulateRegularSave (for reloading from
      battle start) */
   res = Card_ReadRegularSaveIntoBuf(3);
   if (res != 0) {
      return res;
   }
   res = Card_ReadInBattleSaveIntoBuf();
   if (res != 0) {
      return res;
   }
   Card_LoadInBattleSaveFromBuf();
   gState.primary = STATE_LOAD_IN_BATTLE_SAVE;
   gState.secondary = 0;
   gState.state3 = 0;
   gState.state4 = 0;
   return 0;
}

s32 Card_WriteInBattleSave(void) {
   s32 res;

   gState.saveLocation = SAVE_LOC_BATTLE;

   res = Card_WriteRegularSaveFromBuf(3);
   if (res != 0) {
      return res;
   }
   Card_PopulateInBattleSave();
   return Card_WriteInBattleSaveFromBuf();
}

s32 Card_WriteFileListing(void) { return Card_WriteFileListingFromBuf(); }

s32 Card_ReadFileListing(void) { return Card_ReadFileListingIntoBuf(); }

s32 Card_EmbedIntIntoCaption(s32 n, u8 *dest) {
   s32 i, ct;
   u8 *p;
   u8 buffer[64] = "";

   sprintf(buffer, "%d", n);
   p = &buffer[0];
   ct = 0;
   for (i = 0; i < 8; i++) {
      if (*p == ' ') {
         p++;
         continue;
      }
      if (*p == '\0') {
         break;
      }
      *(dest++) = *(p++);
      ct++;
   }
   return ct;
}

s32 Card_EmbedIntIntoSjisCaption(s32 n, u8 *dest) {
   s32 i, ct;
   u8 *p;
   u8 buffer[64] = "";

   sprintf(buffer, "%d", n);
   p = &buffer[0];
   ct = 0;
   if (*p == '-') {
      *(dest++) = 0x81;
      *(dest++) = 0x7c;
      p++;
      ct++;
   }
   for (i = 0; i < 8; i++) {
      if (*p == ' ') {
         p++;
         continue;
      }
      if (*p == '\0') {
         break;
      }
      *(dest++) = 0x82;
      *(dest++) = *(p++) + 0x1f;
      ct++;
   }
   return ct;
}

void Card_UpdateCaption(u8 fileIdx) {
   s32 level, hours, minutes, totalSeconds, totalMinutes;
   u8 *p;
   u8 chapter, section;
   u8 unused[48];
   u8 buffer[40] = "Chap.   Sct.    L      :  ";

   // e.g. Chap. 1 Sct. 1  L5    0:06
   //      01234567890123456789012345

   level = 0;
   while (BigIntCompare(gPartyMembers[UNIT_ASH].experience, gExperienceLevels[level]) < 2) {
      level++;
   }
   if (level > 50) {
      level = 50;
   }

   chapter = gState.chapter;
   section = gState.section;
   buffer[6] = (chapter & 0xf) + ASCII_DIGIT;
   buffer[13] = (section & 0xf) + ASCII_DIGIT;
   Card_EmbedIntIntoCaption(level, &buffer[17]);

   totalSeconds = gState.frameCounter / 60;
   totalMinutes = totalSeconds / 60;
   hours = totalMinutes / 60;
   minutes = totalMinutes % 60;

   p = &buffer[20];
   if (hours > 999) {
      hours = 999;
   }
   if (hours < 10) {
      p += 2;
   } else if (hours < 100) {
      p++;
   }
   Card_EmbedIntIntoCaption(hours, p);

   p = &buffer[24];
   if (minutes < 10) {
      Card_EmbedIntIntoCaption(0, p);
      p++;
   }
   Card_EmbedIntIntoCaption(minutes, p);

   strcpy(gCardFileListingPtr->captions[fileIdx], buffer);
}

u8 *gCardFilenameBufferPtr = gScratch1_801317c0;
// TODO Generates a relocation currently unsupported by psyq-obj-parser; works with maspsx!
u8 *gCardFileBufferPtr = &gScratch1_801317c0[64];
u8 *gCardFileVerifyBufferPtr = &gScratch1_801317c0[8256];
// u8 *gCardFileBufferPtr = (u8 *)0x80131800;       // &gScratch1_801317c0[64]
// u8 *gCardFileVerifyBufferPtr = (u8 *)0x80133800; // &gScratch1_801317c0[8256]

s32 Card_WriteRegularSaveFrom(s32 fileIdx, CardFileData_RegularSave *buf) {
   CardFileData_RegularSave *save;
   s32 res, size = 0x380;
   res = Card_FileExists(gCardFilePath);
   if (res == 0) {
      res = Card_CreateFile(gCardFilePath, &gCardFileHeader);
      if (res != 0) {
         return res;
      }
   }
   memset(gCardFileBufferPtr, '\0', size);
   *(RawSaveData *)gCardFileBufferPtr = *(RawSaveData *)buf;
   save = (CardFileData_RegularSave *)gCardFileBufferPtr;
   // Size/location are adjusted by 4 bytes to exclude previous checksum from calculation
   save->checksum = CalculateChecksum(sizeof(CardFileData_RegularSave) - 4, gCardFileBufferPtr + 4);
   // 1 is added to fileIdx since first 0x400 bytes are reserved for listing data
   return Card_WriteFile(gCardFilePath, gCardFileBufferPtr, size, (fileIdx + 1) * 0x400);
}

s32 Card_ReadRegularSaveInto(s32 fileIdx, CardFileData_RegularSave *buf) {
   CardFileData_RegularSave *save;
   u32 checksum;
   s32 res, size = 0x380;
   if (!Card_FileExists(gCardFilePath)) {
      return 1;
   }
   memset(gCardFileBufferPtr, '\0', size);
   save = (CardFileData_RegularSave *)gCardFileBufferPtr;
   res = Card_ReadFile(gCardFilePath, gCardFileBufferPtr, size, (fileIdx + 1) * 0x400);
   if (res != 0) {
      return res;
   }
   checksum = CalculateChecksum(sizeof(CardFileData_RegularSave) - 4, gCardFileBufferPtr + 4);
   if (save->checksum != checksum) {
      return 2;
   } else {
      *(RawSaveData *)buf = *(RawSaveData *)gCardFileBufferPtr;
      return 0;
   }
}

s32 Card_WriteFileListingFrom(CardFileData_Listing *buf) {
   CardFileData_Listing *listing;
   s32 res, size = 0x100;
   res = Card_FileExists(gCardFilePath);
   if (res == 0) {
      res = Card_CreateFile(gCardFilePath, &gCardFileHeader);
      if (res != 0) {
         return res;
      }
   }
   memset(gCardFileBufferPtr, '\0', size);
   *(RawListingData *)gCardFileBufferPtr = *(RawListingData *)buf;
   listing = (CardFileData_Listing *)gCardFileBufferPtr;
   listing->checksum = CalculateChecksum(sizeof(CardFileData_Listing) - 4, gCardFileBufferPtr + 4);
   return Card_WriteFile(gCardFilePath, gCardFileBufferPtr, size, 0);
}

s32 Card_ReadFileListingInto(CardFileData_Listing *buf) {
   CardFileData_Listing *listing;
   u32 checksum;
   s32 res, size = 0x100;
   if (!Card_FileExists(gCardFilePath)) {
      return 1;
   }
   memset(gCardFileBufferPtr, '\0', size);
   listing = (CardFileData_Listing *)gCardFileBufferPtr;
   res = Card_ReadFile(gCardFilePath, gCardFileBufferPtr, size, 0);
   if (res != 0) {
      return res;
   }
   checksum = CalculateChecksum(sizeof(CardFileData_Listing) - 4, gCardFileBufferPtr + 4);
   if (listing->checksum != checksum) {
      return 2;
   } else {
      *(RawListingData *)buf = *(RawListingData *)gCardFileBufferPtr;
      return 0;
   }
}

s32 Card_WriteInBattleSaveFrom(CardFileData_InBattleSave *buf) {
   CardFileData_InBattleSave *save;
   s32 res, size = 0x1700;
   res = Card_FileExists(gCardFilePath);
   if (res == 0) {
      res = Card_CreateFile(gCardFilePath, &gCardFileHeader);
      if (res != 0) {
         return res;
      }
   }
   memset(gCardFileBufferPtr, '\0', size);
   *(RawInBattleSaveData *)gCardFileBufferPtr = *(RawInBattleSaveData *)buf;
   save = (CardFileData_InBattleSave *)gCardFileBufferPtr;
   save->checksum =
       CalculateChecksum(sizeof(CardFileData_InBattleSave) - 4, gCardFileBufferPtr + 4);
   return Card_WriteFile(gCardFilePath, gCardFileBufferPtr, size, 0x2000);
}

s32 Card_ReadInBattleSaveInto(CardFileData_InBattleSave *buf) {
   CardFileData_InBattleSave *save;
   u32 checksum;
   s32 res, size = 0x1700;
   if (!Card_FileExists(gCardFilePath)) {
      return 1;
   }
   memset(gCardFileBufferPtr, '\0', size);
   save = (CardFileData_InBattleSave *)gCardFileBufferPtr;
   res = Card_ReadFile(gCardFilePath, gCardFileBufferPtr, size, 0x2000);
   if (res != 0) {
      return res;
   }
   checksum = CalculateChecksum(sizeof(CardFileData_InBattleSave) - 4, gCardFileBufferPtr + 4);
   if (save->checksum != checksum) {
      return 2;
   } else {
      *(RawInBattleSaveData *)buf = *(RawInBattleSaveData *)gCardFileBufferPtr;
      return 0;
   }
}

void Card_Init(void) {
   gEventSwCardIOE = OpenEvent(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
   gEventSwCardError = OpenEvent(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
   gEventSwCardTimeout = OpenEvent(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
   gEventSwCardNew = OpenEvent(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
   gEventHwCardIOE = OpenEvent(HwCARD, EvSpIOE, EvMdNOINTR, NULL);
   gEventHwCardError = OpenEvent(HwCARD, EvSpERROR, EvMdNOINTR, NULL);
   gEventHwCardTimeout = OpenEvent(HwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
   gEventHwCardNew = OpenEvent(HwCARD, EvSpNEW, EvMdNOINTR, NULL);
   InitCard(1);
   StartCard();
   _bu_init();
   gCardEventState = 0;
   gCardState = 0;
   EnableEvent(gEventSwCardIOE);
   EnableEvent(gEventSwCardError);
   EnableEvent(gEventSwCardTimeout);
   EnableEvent(gEventSwCardNew);
   EnableEvent(gEventHwCardIOE);
   EnableEvent(gEventHwCardError);
   EnableEvent(gEventHwCardTimeout);
   EnableEvent(gEventHwCardNew);
}

s32 Card_CheckState(void) {
   s32 eventType;
   Card_ClearSwCardEvents();
   _card_info(0);
   eventType = Card_WaitForSwCardEvent();
   if ((gCardEventState == 2 && eventType == CARD_EVENT_TYPE_IOE) ||
       eventType == CARD_EVENT_TYPE_NEW) {
      /* successful retry or new card; confirm via _card_clear */
      Card_ClearHwCardEvents();
      _card_clear(0);
      Card_WaitForHwCardEvent();
   }
   if (gCardState == 0 || gCardEventState != eventType) {
      if (eventType == CARD_EVENT_TYPE_IOE || eventType == CARD_EVENT_TYPE_NEW) {
         Card_ClearSwCardEvents();
         _card_async_load_directory(0);
         eventType = Card_WaitForSwCardEvent();
         switch (eventType) {
         case CARD_EVENT_TYPE_IOE:
            gCardEventState = 0;
            gCardState = -1;
            break;
         case CARD_EVENT_TYPE_ERROR:
            gCardEventState = 1;
            gCardState = -2;
            break;
         case CARD_EVENT_TYPE_TIMEOUT:
            gCardEventState = 2;
            gCardState = -3;
            break;
         case CARD_EVENT_TYPE_NEW:
            gCardEventState = 0;
            gCardState = -4;
            break;
         }
      } else {
         gCardEventState = eventType;
         if (eventType == CARD_EVENT_TYPE_ERROR) {
            gCardState = -2;
         } else {
            gCardState = -3;
         }
      }
   }
   return gCardState;
}

s32 Card_Format(void) {
   gCardState = 0;
   return FormatDevice("bu00:") == 0;
}

s32 Card_CountFreeBlocks(void) {
   extern struct DIRENTRY s_dirEntry;
   struct DIRENTRY *dir;
   s32 usedBlocks;
   s32 res;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   usedBlocks = 0;
   strcpy(gCardFilenameBufferPtr, "bu00:");
   strcat(gCardFilenameBufferPtr, "*");
   dir = firstfile(gCardFilenameBufferPtr, &s_dirEntry);
   while (dir == &s_dirEntry) {
      usedBlocks += s_dirEntry.size / BYTES_PER_BLOCK;
      dir = nextfile(&s_dirEntry);
   }
   return TOTAL_BLOCKS - usedBlocks;
}

s32 Card_CreateFile(u8 *filename, CardFileData_Header *hdr) {
   s32 res, blocks, fd, n;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   blocks = Card_CountFreeBlocks();
   if (blocks < REQUIRED_BLOCKS) {
      return -5;
   }
   fd = FileOpen(filename, O_CREAT | (REQUIRED_BLOCKS << 16));
   if (fd == -1) {
      return -2;
   }
   FileClose(fd);
   fd = FileOpen(filename, O_WRONLY);
   if (fd == -1) {
      return -2;
   }
   n = FileWrite(fd, hdr, sizeof(CardFileData_Header));
   if (n != sizeof(CardFileData_Header)) {
      FileClose(fd);
      return -2;
   }
   FileClose(fd);
   fd = FileOpen(filename, O_RDONLY);
   if (fd == -1) {
      return -2;
   }
   n = FileRead(fd, gCardFileVerifyBufferPtr, sizeof(CardFileData_Header));
   if (n != sizeof(CardFileData_Header)) {
      FileClose(fd);
      return -2;
   }
   FileClose(fd);
   res = memcmp(hdr, gCardFileVerifyBufferPtr, sizeof(CardFileData_Header));
   if (res != 0) {
      return -2;
   }
   return 0;
}

s32 Card_FileExists(u8 *filename) {
   s32 res, fd;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   fd = FileOpen(filename, O_RDONLY);
   if (fd == -1) {
      return 0;
   }
   FileClose(fd);
   return 1;
}

s32 Card_WriteFile(u8 *filename, void *buf, s32 len, s32 offset) {
   s32 fd, res, n;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   fd = FileOpen(filename, O_WRONLY);
   if (fd == -1) {
      return -2;
   }
   n = FileSeek(fd, offset + sizeof(CardFileData_Header), SEEK_SET);
   if (n == -1) {
      FileClose(fd);
      return -2;
   }
   n = FileWrite(fd, buf, len);
   if (n == -1) {
      FileClose(fd);
      return -2;
   }
   FileClose(fd);
   res = Card_ReadFile(filename, gCardFileVerifyBufferPtr, len, offset);
   if (res != 0) {
      return res;
   }
   res = memcmp(buf, gCardFileVerifyBufferPtr, len);
   if (res != 0) {
      return -2;
   }
   return 0;
}

s32 Card_ReadFile(u8 *filename, void *buf, s32 len, s32 offset) {
   s32 res, fd, n;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   fd = FileOpen(filename, O_RDONLY);
   if (fd == -1) {
      return -2;
   }
   n = FileSeek(fd, offset + sizeof(CardFileData_Header), SEEK_SET);
   if (n == -1) {
      FileClose(fd);
      return -2;
   }
   n = FileRead(fd, buf, len);
   if (n == -1) {
      FileClose(fd);
      return -2;
   }
   FileClose(fd);
   return 0;
}

s32 Card_ReadHeadOfArbitraryFile(u8 *filename, void *buf) {
   /* Reads first 128 bytes of a file; unused? */
   s32 res, fd, n;

   res = Card_CheckState();
   if (res != -1) {
      return res;
   }
   strcpy(gCardFilenameBufferPtr, "bu00:");
   strcat(gCardFilenameBufferPtr, filename);
   fd = FileOpen(gCardFilenameBufferPtr, O_RDONLY);
   if (fd == -1) {
      return -2;
   }
   n = FileRead(fd, buf, 128);
   if (n == -1) {
      FileClose(fd);
      return -2;
   }
   FileClose(fd);
   return 0;
}

u32 CalculateChecksum(u32 n, u8 *s) {
   s32 i;
   u32 j, bit, crc = 0xffffffff;

   for (i = 0; i < n; i++) {
      crc ^= s[i];
      for (j = 0; j < 8; j++) {
         bit = crc & 1;
         if (bit != 0) {
            crc = crc >> 1 ^ 0xedb88320;
         } else {
            crc >>= 1;
         }
      }
   }
   return ~crc;
}

s32 Card_WaitForSwCardEvent(void) {
   while (1) {
      if (TestEvent(gEventSwCardIOE) == 1) {
         return CARD_EVENT_TYPE_IOE;
      }
      if (TestEvent(gEventSwCardError) == 1) {
         return CARD_EVENT_TYPE_ERROR;
      }
      if (TestEvent(gEventSwCardTimeout) == 1) {
         return CARD_EVENT_TYPE_TIMEOUT;
      }
      if (TestEvent(gEventSwCardNew) == 1) {
         return CARD_EVENT_TYPE_NEW;
      }
   }
}

void Card_ClearSwCardEvents(void) {
   TestEvent(gEventSwCardIOE);
   TestEvent(gEventSwCardError);
   TestEvent(gEventSwCardTimeout);
   TestEvent(gEventSwCardNew);
}

s32 Card_WaitForHwCardEvent(void) {
   while (1) {
      if (TestEvent(gEventHwCardIOE) == 1) {
         return CARD_EVENT_TYPE_IOE;
      }
      if (TestEvent(gEventHwCardError) == 1) {
         return CARD_EVENT_TYPE_ERROR;
      }
      if (TestEvent(gEventHwCardTimeout) == 1) {
         return CARD_EVENT_TYPE_TIMEOUT;
      }
      if (TestEvent(gEventHwCardNew) == 1) {
         return CARD_EVENT_TYPE_NEW;
      }
   }
}

void Card_ClearHwCardEvents(void) {
   TestEvent(gEventHwCardIOE);
   TestEvent(gEventHwCardError);
   TestEvent(gEventHwCardTimeout);
   TestEvent(gEventHwCardNew);
}
