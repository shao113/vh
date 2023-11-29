#include "common.h"
#include "cd_files.h"
#include "audio.h"
#include "card.h"
#include "state.h"
#include "evt.h"
#include "units.h"
#include "window.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

void main(void);
void UpdateState(void);
void Evtf582_MainMenu_Jpn(EvtData *evt);
void Evtf583_LoadingIndicator(EvtData *evt);
void Evtf006_Logo(EvtData *evt);
void State_Init(void);
void State_EventScene(void);
void Evtf584_Noop(void);

void main(void) {
   s32 i;
   u8 *p;

   //__main(); // <- inserted automatically by gcc
   p = (u8 *)&gState;

   for (i = 0; i < sizeof(State); i++) {
      *(p++) = 0;
   }

   while (1) {
      UpdateState();
      UpdateEngine();
   }
}

void UpdateState(void) {
   EvtData *evt;

   switch (gState.primary) {
   case STATE_0:
      gState.suppressLoadingScreen = 1;
      gUnitDataPtr = gScratch3_80180210 + 0x2db80;
      gRegularSaveDataPtr = gScratch1_801317c0 + 0x5000;
      gInBattleSaveDataPtr = gScratch1_801317c0 + 0x6000;
      gCardFileListingPtr = gScratch1_801317c0 + 0xa000;
      gState.textSpeed = 1;
      Initialize();
      LoadItemIcons();
      InitAudio();
      SetXaCdSectorOffset(0x4c0d);
      LoadSoundSet(0);
      FinishLoadingVab();
      LoadSoundSet(1);
      FinishLoadingVab();
      LoadSoundSet(2);
      FinishLoadingVab();
      SwapInCodeFromVram();
      gState.primary++;
   case STATE_1:
      gState.field_0x96 = 0;
      gUnitDataPtr = gScratch3_80180210 + 0x2db80;
      ClearPortraitSet();
      State_Init();
      break;
   case STATE_3:
   case STATE_LOAD_IN_BATTLE_SAVE:
   case STATE_27:
   case STATE_30:
   case STATE_31:
      State_Battle();
      break;
   case STATE_4:
   case STATE_16:
   case STATE_17:
   case STATE_25:
      State_EventScene();
      break;
   case STATE_SHOP:
   case STATE_DEPOT:
      State_ShopOrDepot();
      break;
   case STATE_6:
   case STATE_28:
      State_WorldMap();
      break;
   case STATE_7:
   case STATE_20:
      State_Town();
      break;
   case STATE_TAVERN:
      State_Tavern();
      break;
   case STATE_9:
   case STATE_29:
      State_Dojo();
      break;
   case STATE_MOVIE:
      State_Movie();
      break;
   case STATE_12:
   case STATE_18:
   case STATE_19:
   case STATE_26:
      State_SetupScene();
      break;
   case STATE_FINISH_CHAPTER:
      State_FinishChapter();
      break;
   case STATE_ENDING_SCREEN:
      State_EndingScreen();
      break;
   case STATE_SET_SCENE_STATE:
      State_SetSceneState();
      break;
   case STATE_FILE_SAVE_SCREEN:
      State_FileSaveScreen();
      break;
   case STATE_FILE_LOAD_SCREEN:
      State_FileLoadScreen();
      break;
   case STATE_TITLE_SCREEN:
      State_TitleScreen();
      break;
   case STATE_TITLE_LOAD_SCREEN:
      State_Title_FileLoadScreen();
      break;
   case STATE_LOAD_DEBUG_MENU:
      Evt_ResetFromIdx10();
      SwapOutCodeToVram();
      gState.vsyncMode = 0;
      gClearSavedPadState = 1;
      gState.fieldRenderingDisabled = 1;
      LoadFullscreenImage(CDF_US_TITLE_TIM);
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FULLSCREEN_IMAGE;
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DEBUG_MENU;
      gState.primary = STATE_DEBUG_MENU;
   case STATE_DEBUG_MENU:
      break;
   }
}

void Evtf582_MainMenu_Jpn(EvtData *evt) {
   // Left-over debugging stuff?
   EvtData *dialog;
   s32 i;

   switch (evt->state) {
   case 0:
      FadeInScreen(2, 0xff);
      gWindowChoiceHeight = 17;
      gWindowChoicesTopMargin = 10;
      gWindowActiveIdx = 0x34;
      DrawWindow(0x34, 0, 0, 128, 54, 180, 8, WBS_CROSSED, 2);
      DrawSjisText(12, 11, 20, 2, 0,
                   "\x8e\x6e\x82\xdf\x82\xa9\x82\xe7\x0a\x83\x8d\x81\x5b\x83\x68");
      DisplayBasicWindow(0x34);
      evt->state++;
      evt->state2 = 0;
      break;
   case 1:
      gState.gold = 0;
      gState.frameCounter = 0;

      for (i = 0; i < DEPOT_CT; i++) {
         gState.depot[i] = ITEM_NULL;
      }

      if (gWindowChoice.s.windowId == 0x34 && gWindowChoice.s.choice != 0) {
         if (gWindowChoice.raw == 0x3401) {
            ResetStateForNewGame();
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_LOGO_USA_STR;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;
         }
         if (gWindowChoice.raw == 0x3402) {
            CloseWindow(0x34);
            dialog = Evt_GetUnused();
            dialog->functionIndex = EVTF_FILE_LOAD_DIALOG_360;
            evt->state++;
            gState.D_8014053E = 0;
         }
      }
      break;
   case 2:
      if (gState.D_8014053E != 0) {
         gState.primary = STATE_LOAD_DEBUG_MENU;
      }
      break;
   }
}

void Evtf583_LoadingIndicator(EvtData *evt) {
   EvtData *spr = Evt_GetUnused();

   spr->d.sprite.gfxIdx = GFX_NOW_LOADING;
   spr->x1.n = 120;
   spr->y1.n = 95;
   spr->x3.n = spr->x1.n + 80;
   spr->y3.n = spr->y1.n + 50;
   AddEvtPrim_Gui(gGraphicsPtr->ot, spr);

   spr->d.sprite.gfxIdx = GFX_VANDAL_HEARTS;
   spr->x1.n = 156;
   spr->y1.n = 184;
   spr->x3.n = spr->x1.n + 128;
   spr->y3.n = spr->y1.n + 32;
   AddEvtPrim_Gui(gGraphicsPtr->ot, spr);
}

void Evtf006_Logo(EvtData *evt) {
   evt->d.sprite.gfxIdx = GFX_VANDAL_HEARTS;
   evt->d.sprite.otOfs = 2;
   evt->d.sprite.clut = CLUT_BLUES;
   evt->x1.n = 12;
   evt->y1.n = 50;
   evt->x3.n = evt->x1.n + 128;
   evt->y3.n = evt->y1.n + 32;
   AddEvtPrim_Gui(gGraphicsPtr->ot, evt);
}

void State_Init(void) {
   SwapOutCodeToVram();
   LoadPortraits();
   gClearSavedPadState = 0;
   gIsEnemyTurn = 0;
   gPlayerControlSuppressed = 0;
   gMapCursorSuppressed = 0;
   gSignal1 = 0;
   gSignal2 = 0;
   gMapCursorX = 0;
   gMapCursorZ = 0;
   gState.inEvent = 0;
   gState.previewingRange = 0;
   gSavedPadState = 0;
   gSavedPad2State = 0;
   gSavedPadStateNewPresses = 0;
   gSavedPad2StateNewPresses = 0;
   gPadState = 0;
   gPad2State = 0;
   gPadStateNewPresses = 0;
   gPad2StateNewPresses = 0;
   gWindowChoice.raw = 0;
   gWindowActivatedChoice.raw = 0;
   gMapDataPtr = gScratch3_80180210;
   gGraphicsPtr = &gGraphicBuffers[0];
   ClearUnits();
   Evt_ResetAll();
   LoadFWD();
   SetupGfx();

   gTempGfxEvt = Evt_GetFirstUnused();
   gTempGfxEvt->functionIndex = EVTF_NOOP;

   gTempEvt = Evt_GetFirstUnused();
   gTempEvt->functionIndex = EVTF_NOOP_407;

   gTempEvt = Evt_GetFirstUnused();
   gTempEvt->functionIndex = EVTF_EVENT_CAMERA;

   gTempEvt = Evt_GetFirstUnused();
   gTempEvt->functionIndex = EVTF_MENU_CHOICE;

   gTempEvt = Evt_GetFirstUnused();
   gTempEvt->functionIndex = EVTF_SCREEN_EFFECT;
   gState.screenEffect = gTempEvt;

   gDecodingSprites = 0;
   gState.fieldRenderingDisabled = 1;
   gState.vsyncMode = 2;
   gOverheadMapState = 0;

   if (gState.debug) {
      gState.primary = STATE_LOAD_DEBUG_MENU;
      gState.secondary = 0;
      gState.state3 = 0;
      gState.state4 = 0;
   } else {
      gState.primary = STATE_MOVIE;
      gState.secondary = 0;
      gState.state3 = 0;
      gState.state4 = 0;
      gState.movieIdxToPlay = MOV_LOGO_USA_STR;
   }

   gState.suppressLoadingScreen = 0;
   SetDispMask(1);
}

void State_EventScene(void) {
   s32 i;

   switch (gState.secondary) {
   case 0:
      gState.suppressLoadingScreen = 1;
      FadeOutScreen(2, 0xff);
      ResetGeomOffset();
      gState.vsyncMode = 2;
      gState.preciseSprites = 0;
      Evt_ResetFromIdx10();
      ClearUnits();
      gIsEnemyTurn = 0;
      gCameraRotation.vx = 0x180;
      gClearSavedPadState = 1;
      gPlayerControlSuppressed = 1;
      gState.eventCameraPan.x = 0;
      gState.eventCameraPan.y = 0;
      gState.eventCameraPan.z = 0;
      gState.eventCameraHeight = 0;
      gState.focus = NULL;

      for (i = 0; i < 20; i++) {
         gState.mapState.bytes[i] = 0;
      }

      if (gState.scene == 94) {
         DrawWindow(0x34, 0, 0, 200, 81, 60, 79, WBS_DRAGON, 0);
         DisplayCustomWindow(0x34, 0, 1, 1, 0, 0);
         DrawText(20, 24, 25, 2, 0, "The days go by one\nafter the other...");
      }
      if (gState.scene == 0) {
         DrawWindow(0x34, 0, 0, 184, 100, 68, 70, WBS_DRAGON, 0);
         DisplayCustomWindow(0x34, 0, 1, 1, 0, 0);
         DrawText(24, 24, 25, 3, 0, "1254 AT\nGillbaris Island\n \"Castle ruins\"");
      }
      if ((gState.scene != 0) && (gState.scene != 1) && (gState.scene != 94) &&
          (gState.scene != 7) && (gState.scene != 20) && (gState.scene != 3) &&
          (gState.scene != 6) && (gState.scene != 19) && (gState.scene != 28) &&
          (gState.scene != 31) && (gState.scene != 34) && (gState.scene != 35) &&
          (gState.scene != 51) && (gState.scene != 68) && (gState.scene != 70) &&
          (gState.scene != 80) && (gState.scene != 87) && (gState.scene != 89) &&
          (gState.primary == STATE_4 || gState.primary == STATE_25)) {
         gState.suppressLoadingScreen = 0;
      }
   case 1:
   case 2:
   case 3:
      gState.secondary++;
      break;
   case 4:
      if (gState.primary == STATE_25) {
         LoadFWD();
         gState.primary = STATE_4;
      }
      if (gState.primary == STATE_4) {
         SetupPartySprites();
         for (i = 24; i < 50; i++) {
            gState.portraitsToLoad[i] = gPortraitsDb.sceneSets[gState.scene][i - 24];
         }
         LoadPortraits();
         LoadUnits();
         SetupSprites();
      }
      if (gState.scene == 29) {
         LoadSoundSet(10);
         FinishLoadingVab();
      }
      if (gState.scene == 39) {
         LoadSoundSet(5);
         FinishLoadingVab();
         LoadMap();
      }
      if (gState.scene == 55) {
         LoadSoundSet(3);
         FinishLoadingVab();
      }
      if (gState.scene == 88) {
         LoadSoundSet(9);
         FinishLoadingVab();
         LoadMap();
      }
      if (gState.scene == 93) {
         LoadSoundSet(7);
         FinishLoadingVab();
      }
      if (gState.scene == 51) {
         LoadSoundSet(11);
         FinishLoadingVab();
      }
      if (gState.scene == 52) {
         LoadSoundSet(2);
         FinishLoadingVab();
      }
      if (gState.scene == 61) {
         LoadSoundSet(12);
         FinishLoadingVab();
         LoadMap();
      }
      if (gState.scene == 94) {
         LoadSoundSet(11);
         FinishLoadingVab();
      }
      if (gState.scene == 90) {
         LoadSoundSet(2);
         FinishLoadingVab();
      }
      if (gState.scene == 63) {
         LoadSoundSet(2);
         FinishLoadingVab();
      }
      if (gState.primary != STATE_16) {
         LoadFCOM4XX();
         LoadMapTextures();
         LoadMap();
      }
      SetupField();
      SetupLight();
      gState.fieldRenderingDisabled = 0;
      gState.primary = STATE_4;
      gState.secondary++;
      gState.suppressLoadingScreen = 0;
      break;
   case 5:
      gState.secondary++;
      break;
   case 6:
      Evt_ResetFromIdx10();
      SetupMap();
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = gState.state7;
      FadeInScreen(2, 6);
      gState.secondary++;
   case 7:
      break;
   }
}

void Evtf584_Noop(void) {}
