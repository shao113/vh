#include "common.h"
#include "state.h"
#include "graphics.h"
#include "units.h"
#include "cd_files.h"

extern s16 gSceneLoaders[96];
extern s8 gSceneMapNum[96];
extern s16 gSceneSpriteStripUnitIds[105][20];
extern s16 gSceneUnitSets[100][20];

void State_SetupScene(void) {
   s32 i;

   SwapOutCodeToVram();
   Evt_ResetFromIdx10();

   if (gState.scene == 1) {
      SetupParty(0);
   }
   if (gState.scene == 10) {
      SetupParty(1);
   }
   if (gState.scene == 0xb) {
      SetupParty(2);
   }
   if (gState.scene == 0x10) {
      SetupParty(3);
   }
   if (gState.scene == 0x15) {
      SetupParty(4);
   }
   if (gState.scene == 0x25) {
      SetupParty(5);
   }
   if (gState.scene == 0x30) {
      SetupParty(6);
   }
   if (gState.scene == 0x1f) {
      SetupParty(7);
   }

   gState.vsyncMode = 0;
   gClearSavedPadState = 1;
   gState.fieldRenderingDisabled = 1;
   gState.state7 = gSceneLoaders[gState.scene];
   gState.mapNum = gSceneMapNum[gState.scene];

   if (gState.primary == 12) {
      gState.primary = 4;
      for (i = 0; i < ARRAY_COUNT(gSpriteStripUnitIds); i++) {
         gSpriteStripUnitIds[i] = 0;
      }
      for (i = 0; i < ARRAY_COUNT(gCurrentUnitSet); i++) {
         gSpriteStripUnitIds[i] = gSceneSpriteStripUnitIds[gState.scene][i];
         gCurrentUnitSet[i] = gSceneUnitSets[gState.scene][i];
      }
   }

   if (gState.primary == 0x1a) {
      gState.primary = 0x19;
      for (i = 0; i < ARRAY_COUNT(gSpriteStripUnitIds); i++) {
         gSpriteStripUnitIds[i] = 0;
      }
      for (i = 0; i < ARRAY_COUNT(gCurrentUnitSet); i++) {
         gSpriteStripUnitIds[i] = gSceneSpriteStripUnitIds[gState.scene][i];
         gCurrentUnitSet[i] = gSceneUnitSets[gState.scene][i];
      }
   }

   for (i = 0; i < ARRAY_COUNT(gCurrentUnitSet); i++) {
      if (gCurrentUnitSet[i] == 0) {
         gCurrentUnitSet[i] = gSpriteStripUnitIds[i];
      }
   }

   if (gState.primary == 0x12) {
      gState.primary = 0x10;
   }

   if (gState.primary == 0x13) {
      gState.primary = 0x11;
   }

   gState.secondary = 0;
   gState.state3 = 0;
   gState.state4 = 0;
}

void State_SetSceneState(void) {
   gState.secondary = 0;
   gState.state3 = 0;
   gState.state4 = 0;

   switch (gState.scene) {
   case 0:
      gState.primary = STATE_MOVIE;
      gState.movieIdxToPlay = MOV_EPI1_WS_STR;
      break;

   case 1:
      gState.primary = 0x1e;
      gState.mapNum = 10;
      gState.gold = 0;
      gState.chapter = 1;
      gState.section = 1;
      break;

   case 2:
      gState.worldMapDestination = 0;
      gState.primary = 6;
      gState.worldMapState = 0xb;
      break;

   case 3:
      gState.primary = 7;
      gState.townState = 2;
      break;

   case 4:
      gState.primary = 0x1e;
      gState.mapNum = 11;
      gState.chapter = 1;
      gState.section = 2;
      break;

   case 5:
      gState.primary = 0xc;
      gState.scene = 6;
      break;

   case 6:
      gState.primary = 7;
      gState.townState = 5;
      break;

   case 7:
      gState.primary = 0xc;
      gState.scene = 0x14;
      break;

   case 8:
      gState.primary = 0x1e;
      gState.mapNum = 12;
      gState.chapter = 1;
      gState.section = 3;
      break;

   case 9:
      gState.primary = 0x13;
      gState.scene = 10;
      break;

   case 10:
      gState.worldMapDestination = 2;
      gState.primary = 6;
      gState.worldMapState = 0xd;
      break;

   case 0xb:
      gState.primary = 0x1e;
      gState.mapNum = 13;
      gState.chapter = 1;
      gState.section = 4;
      break;

   case 0xc:
      gState.primary = 0x13;
      gState.scene = 0xd;
      break;

   case 0xd:
      gState.worldMapDestination = 3;
      gState.primary = 6;
      gState.worldMapState = 0xe;
      break;

   case 0xe:
      gState.primary = 0x14;
      gState.townState = 9;
      break;

   case 0xf:
      gState.primary = 0x1e;
      gState.mapNum = 14;
      gState.chapter = 1;
      gState.section = 5;
      break;

   case 0x10:
      gState.primary = 0x14;
      gState.townState = 10;
      break;

   case 0x11:
      gState.primary = 0x1e;
      gState.mapNum = 15;
      gState.chapter = 1;
      gState.section = 6;
      break;

   case 0x12:
      gState.primary = STATE_FINISH_CHAPTER;
      gState.chapterOutro = 0;
      gState.chapter = 2;
      gState.section = 1;
      break;

   case 0x13:
      gState.primary = 0x1e;
      gState.mapNum = 16;
      gState.chapter = 2;
      gState.section = 1;
      break;

   case 0x14:
      gState.primary = 7;
      gState.townState = 0xb;
      break;

   case 0x15:
      gState.primary = 0x1e;
      gState.mapNum = 17;
      gState.chapter = 2;
      gState.section = 2;
      break;

   case 0x16:
      gState.primary = 0x13;
      gState.scene = 0x17;
      break;

   case 0x17:
      gState.primary = 7;
      gState.townState = 0xc;
      break;

   case 0x18:
      gState.primary = 0x1e;
      gState.mapNum = 18;
      gState.chapter = 2;
      gState.section = 3;
      break;

   case 0x19:
      gState.primary = 0x12;
      gState.scene = 0x1a;
      break;

   case 0x1a:
      gState.worldMapDestination = 9;
      gState.primary = 6;
      gState.worldMapState = 0x17;
      break;

   case 0x1b:
      gState.primary = 0x1e;
      gState.mapNum = 19;
      gState.chapter = 2;
      gState.section = 4;
      break;

   case 0x1c:
      if (gState.D_8014085C == 0) {
         gState.worldMapDestination = 9;
         gState.primary = 6;
         gState.worldMapState = 0x18;
      } else {
         gState.primary = 0xc;
         gState.scene = 0x1d;
      }
      break;

   case 0x1d:
      gState.primary = 0x1e;
      gState.mapNum = 20;
      gState.chapter = 2;
      gState.section = 5;
      break;

   case 0x1e:
      gState.primary = 0xc;
      gState.scene = 0x1f;
      break;

   case 0x1f:
      gState.primary = 6;
      gState.worldMapState = 0x19;
      gState.worldMapDestination = 10;
      break;

   case 0x20:
      gState.primary = 0x1e;
      gState.mapNum = 21;
      gState.chapter = 2;
      gState.section = 6;
      break;

   case 0x21:
      gState.primary = 0xc;
      gState.scene = 0x22;
      break;

   case 0x22:
      gState.primary = STATE_FINISH_CHAPTER;
      gState.chapterOutro = 1;
      gState.chapter = 3;
      gState.section = 1;
      break;

   case 0x23:
      gState.primary = 0x1e;
      gState.mapNum = 22;
      gState.chapter = 3;
      gState.section = 1;
      break;

   case 0x24:
      gState.worldMapDestination = 0xb;
      gState.primary = 6;
      gState.worldMapState = 0x1f;
      break;

   case 0x25:
      gState.primary = 7;
      gState.townState = 0x10;
      break;

   case 0x26:
      gState.primary = 0x1e;
      gState.mapNum = 23;
      gState.chapter = 3;
      gState.section = 2;
      break;

   case 0x27:
      gState.primary = 0xc;
      gState.scene = 0x28;
      break;

   case 0x28:
      gState.primary = 0x1e;
      gState.mapNum = 24;
      gState.chapter = 3;
      gState.section = 3;
      break;

   case 0x29:
      gState.primary = 0x13;
      gState.scene = 0x2a;
      break;

   case 0x2a:
      gState.primary = 7;
      gState.townState = 0x11;
      break;

   case 0x2b:
      gState.primary = 0x1e;
      gState.mapNum = 25;
      gState.chapter = 3;
      gState.section = 4;
      break;

   case 0x2c:
      gState.worldMapDestination = 0xe;
      gState.primary = 6;
      gState.worldMapState = 0x2a;
      break;

   case 0x2e:
      gState.primary = 0x1e;
      gState.mapNum = 26;
      gState.chapter = 3;
      gState.section = 5;
      break;

   case 0x2f:
      gState.primary = 0xc;
      gState.scene = 0x30;
      break;

   case 0x30:
      gState.primary = 0x1e;
      gState.mapNum = 27;
      gState.chapter = 3;
      gState.section = 6;
      break;

   case 0x31:
      gState.primary = 0x1e;
      gState.mapNum = 28;
      gState.chapter = 3;
      gState.section = 7;
      break;

   case 0x32:
      gState.primary = STATE_FINISH_CHAPTER;
      gState.chapterOutro = 2;
      gState.chapter = 4;
      gState.section = 1;
      break;

   case 0x33:
      gState.primary = 0x13;
      gState.scene = 0x34;
      break;

   case 0x34:
      gState.primary = 7;
      gState.townState = 0x12;
      break;

   case 0x35:
      gState.primary = 0x1e;
      gState.mapNum = 29;
      gState.chapter = 4;
      gState.section = 2;
      break;

   case 0x36:
      gState.worldMapDestination = 0x11;
      gState.primary = 6;
      gState.worldMapState = 0x33;
      break;

   case 0x37:
      gState.primary = 0x1e;
      gState.mapNum = 30;
      gState.chapter = 4;
      gState.section = 3;
      break;

   case 0x38:
      gState.worldMapDestination = 0x12;
      gState.primary = 6;
      gState.worldMapState = 0x34;
      break;

   case 0x39:
      gState.primary = 7;
      gState.townState = 0x16;
      break;

   case 0x3c:
      gState.primary = 0x1e;
      gState.mapNum = 31;
      gState.chapter = 4;
      gState.section = 4;
      break;

   case 0x3d:
      gState.primary = 0x13;
      gState.scene = 0x3e;
      break;

   case 0x3e:
      gState.primary = 7;
      gState.townState = 0x17;
      break;

   case 0x3f:
      gState.primary = 0x1e;
      gState.mapNum = 32;
      gState.chapter = 4;
      gState.section = 5;
      break;

   case 0x40:
      gState.worldMapDestination = 0x14;
      gState.primary = 6;
      gState.worldMapState = 0x35;
      break;

   case 0x41:
      gState.primary = 0x1e;
      gState.mapNum = 33;
      gState.chapter = 4;
      gState.section = 6;
      break;

   case 0x42:
      gState.primary = 0x13;
      gState.scene = 0x43;
      BigIntAdd(gPartyMembers[UNIT_KIRA].experience, gExperienceLevels[20]);
      break;

   case 0x43:
      gState.primary = STATE_FINISH_CHAPTER;
      gState.chapterOutro = 3;
      gState.chapter = 5;
      gState.section = 1;
      break;

   case 0x44:
      gState.primary = 0xc;
      gState.scene = 0x45;
      break;

   case 0x45:
      gState.primary = 0x1e;
      gState.mapNum = 34;
      gState.chapter = 5;
      gState.section = 1;
      break;

   case 0x46:
      gState.primary = 7;
      gState.townState = 0x1a;
      break;

   case 0x47:
      gState.primary = 3;
      gState.mapNum = 35;
      gState.chapter = 5;
      gState.section = 2;
      break;

   case 0x48:
      gState.worldMapDestination = 0x18;
      gState.primary = 6;
      gState.worldMapState = 0x3f;
      break;

   case 0x49:
      gState.primary = 7;
      gState.townState = 0x1e;
      break;

   case 0x4a:
      gState.primary = 0x1e;
      gState.mapNum = 36;
      gState.chapter = 5;
      gState.section = 3;
      break;

   case 0x4b:
      gState.primary = 0x13;
      gState.scene = 0x4c;
      break;

   case 0x4c:
      gState.primary = 7;
      gState.townState = 0x1f;
      break;

   case 0x4d:
      gState.primary = 0x1e;
      gState.mapNum = 37;
      gState.chapter = 5;
      gState.section = 4;
      break;

   case 0x4e:
      gState.primary = 3;
      gState.mapNum = 38;
      gState.chapter = 5;
      gState.section = 5;
      break;

   case 0x4f:
      gState.primary = 0xc;
      gState.scene = 0x50;
      break;

   case 0x50:
      gState.primary = 0x12;
      gState.scene = 0x51;
      break;

   case 0x51:
      gState.worldMapDestination = 0x19;
      gState.primary = 6;
      gState.worldMapState = 0x43;
      break;

   case 0x52:
      gState.primary = 0x1e;
      gState.mapNum = 39;
      gState.chapter = 5;
      gState.section = 6;
      break;

   case 0x53:
      gState.primary = STATE_FINISH_CHAPTER;
      gState.chapterOutro = 4;
      gState.chapter = 6;
      gState.section = 1;
      gState.depot[gPartyMembers[UNIT_ASH].weapon] += 1;
      gPartyMembers[UNIT_ASH].weapon = ITEM_V_HEART;
      break;

   case 0x54:
      gState.primary = 0x1e;
      gState.mapNum = 40;
      gState.chapter = 6;
      gState.section = 2;
      break;

   case 0x55:
      gState.primary = 0x1e;
      gState.mapNum = 41;
      gState.chapter = 6;
      gState.section = 3;
      break;

   case 0x56:
      gState.primary = 0xc;
      gState.scene = 0x57;
      break;

   case 0x57:
      gState.primary = 0x12;
      gState.scene = 0x58;
      break;

   case 0x58:
      gState.primary = 0xc;
      gState.scene = 0x59;
      break;

   case 0x59:
      gState.worldMapDestination = 0x1d;
      gState.primary = 6;
      gState.worldMapState = 0x49;
      break;

   case 0x5a:
      gState.primary = 0x1e;
      gState.mapNum = 42;
      gState.chapter = 6;
      gState.section = 4;
      break;

   case 0x5b:
      if (gState.D_8014085C == 0) {
         gState.worldMapDestination = 0x20;
         gState.primary = 6;
         gState.worldMapState = 0x4a;
      } else {
         gState.primary = 0xc;
         gState.scene = 0x5c;
      }
      break;

   case 0x5c:
      gState.primary = 0x1e;
      gState.mapNum = 43;
      gState.chapter = 6;
      gState.section = 5;
      break;

   case 0x5d:
      gState.primary = STATE_MOVIE;
      gState.movieIdxToPlay = MOV_END1_WS_STR;
      break;

   case 0x5e:
      gState.primary = STATE_ENDING_SCREEN;
      break;
   }
}
