#include "common.h"
#include "evt.h"
#include "window.h"
#include "state.h"
#include "card.h"

u8 s_menuMem_config_801232ac;

void ShowConfigMenu(EvtData *evt) {
   switch (evt->state2) {
   case 0:
      DrawWindow(0x34, 256, 0, 104, 74, 100, 80, WBS_CROSSED, 3);
      DrawText(256, 10, 20, 2, 0, "Text Speed\nSound\nCamera\n");
      DisplayBasicWindowWithSetChoice(0x34, s_menuMem_config_801232ac);
      gWindowActiveIdx = 0x34;
      evt->state2++;
      break;

   case 1:
      if (gWindowChoice.raw == 0x34ff) {
         s_menuMem_config_801232ac = GetWindowChoice(0x34) - 1;
         CloseWindow(0x34);
         evt->state = 1;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3401) {
         s_menuMem_config_801232ac = 0;
         CloseWindow(0x34);
         evt->state = 8;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3402) {
         s_menuMem_config_801232ac = 1;
         CloseWindow(0x34);
         evt->state = 9;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3403) {
         s_menuMem_config_801232ac = 2;
         CloseWindow(0x34);
         evt->state = 10;
         evt->state2 = 0;
      }
   }
}

void ShowTextSpeedSettings(EvtData *evt) {
   switch (evt->state2) {
   case 0:
      DrawWindow(0x34, 256, 0, 80, 74, 112, 80, WBS_CROSSED, 3);
      DrawText(260, 11, 10, 2, 0, "Fast\nNormal\nSlow");
      DisplayBasicWindowWithSetChoice(0x34, 2 - gState.textSpeed);
      gWindowActiveIdx = 0x34;
      evt->state2++;
      break;

   case 1:
      if (gWindowChoice.raw == 0x34ff) {
         CloseWindow(0x34);
         evt->state = 7;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3401) {
         CloseWindow(0x34);
         gState.textSpeed = 2;
         evt->state = 7;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3402) {
         CloseWindow(0x34);
         gState.textSpeed = 1;
         evt->state = 7;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3403) {
         CloseWindow(0x34);
         gState.textSpeed = 0;
         evt->state = 7;
         evt->state2 = 0;
      }
   }
}

void ShowAudioSettings(EvtData *evt) {
   switch (evt->state2) {
   case 0:
      DrawWindow(0x34, 256, 0, 80, 54, 115, 80, WBS_CROSSED, 2);
      DrawText(260, 11, 10, 2, 0, "Stereo\nMono");
      DisplayBasicWindowWithSetChoice(0x34, gState.mono);
      gWindowActiveIdx = 0x34;
      evt->state2++;
      break;

   case 1:
      if (gWindowChoice.raw == 0x34ff) {
         CloseWindow(0x34);
         evt->state = 7;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3401) {
         CloseWindow(0x34);
         gState.mono = 0;
         PerformAudioCommand(2);
         evt->state = 7;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3402) {
         PerformAudioCommand(1);
         CloseWindow(0x34);
         gState.mono = 1;
         evt->state = 7;
         evt->state2 = 0;
      }
   }
}

void ShowCameraSettings(EvtData *evt) {
   switch (evt->state2) {
   case 0:
      DrawWindow(0x34, 256, 0, 104, 54, 100, 80, WBS_CROSSED, 2);
      DrawText(260, 11, 10, 2, 0, "Dynamic\nFixed");
      DisplayBasicWindowWithSetChoice(0x34, gState.cameraMode);
      gWindowActiveIdx = 0x34;
      evt->state2++;
      break;

   case 1:
      if (gWindowChoice.raw == 0x34ff) {
         CloseWindow(0x34);
         evt->state = 7;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3401) {
         CloseWindow(0x34);
         gState.cameraMode = CAMERA_MODE_DYNAMIC;
         evt->state = 7;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3402) {
         CloseWindow(0x34);
         gState.cameraMode = CAMERA_MODE_FIXED;
         evt->state = 7;
         evt->state2 = 0;
      }
   }
}

u8 s_newState_801232a0;
u8 s_delay_801232a4;
u8 s_menuMem_worldActions_801232a8;

s32 Evtf598_WorldActions(EvtData *evt) {
   EvtData *statusWindowMgr;

   switch (evt->state) {
   case 0:
      s_menuMem_worldActions_801232a8 = 0;
      evt->state++;

   // fallthrough
   case 1:
      gWindowChoiceHeight = 17;
      gWindowChoicesTopMargin = 10;
      DrawWindow(0x34, 256, 0, 104, 126, 10, 16, WBS_CROSSED, 6);
      DrawText(260, 11, 10, 2, 0, "Move\nItems\nStatus\nOptions\nSave\nLoad");
      DisplayBasicWindowWithSetChoice(0x34, s_menuMem_worldActions_801232a8);
      gWindowActiveIdx = 0x34;
      evt->state++;
      break;

   case 2:
      if (gWindowChoice.raw == 0x3401) {
         s_menuMem_worldActions_801232a8 = 0;
         CloseWindow(0x34);
         evt->functionIndex = EVTF_NULL;
         gSignal1 = 0;
         break;
      }
      if (gWindowChoice.raw == 0x3404) {
         s_menuMem_worldActions_801232a8 = 3;
         evt->state = 7;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3402) {
         s_menuMem_worldActions_801232a8 = 1;
         gState.prevState = gState.primary;
         s_newState_801232a0 = STATE_21;
         evt->state = 100;
         evt->state2 = 0;
         break;
      }
      if (gWindowChoice.raw == 0x3403) {
         s_menuMem_worldActions_801232a8 = 3;
         evt->state = 6;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3405) {
         s_menuMem_worldActions_801232a8 = 4;
         if (gState.primary == STATE_6) {
            gState.saveLocation = SAVE_LOC_WORLD_MAP;
            gState.state6 = STATE_28;
         } else {
            gState.saveLocation = SAVE_LOC_TOWN;
            gState.state6 = STATE_20;
         }
         s_newState_801232a0 = STATE_FILE_SAVE_SCREEN;
         evt->state = 100;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3406) {
         s_menuMem_worldActions_801232a8 = 5;
         if (gState.primary == STATE_6) {
            gState.state6 = STATE_28;
         } else {
            gState.state6 = STATE_20;
         }
         s_newState_801232a0 = STATE_FILE_LOAD_SCREEN;
         evt->state = 100;
         evt->state2 = 0;
      }
      break;

   case 6:

      switch (evt->state2) {
      case 0:
         s_menuMem_worldActions_801232a8 = 2;
         CloseWindow(0x34);
         gSignal2 = 0;
         statusWindowMgr = Evt_GetUnused();
         statusWindowMgr->functionIndex = EVTF_STATUS_WINDOW_MGR;
         evt->state2++;
      // fallthrough
      case 1:
         if (gSignal2 == 100) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 7:
      ShowConfigMenu(evt);
      break;

   case 8:
      ShowTextSpeedSettings(evt);
      break;

   case 9:
      ShowAudioSettings(evt);
      break;

   case 10:
      ShowCameraSettings(evt);
      break;

   case 99:
      evt->state = 1;
      evt->state2 = 0;
      break;

   case 100:

      switch (evt->state2) {
      case 0:
         FadeOutScreen(2, 6);
         s_delay_801232a4 = 50;
         evt->state2++;
      // fallthrough
      case 1:
         if (--s_delay_801232a4 == 0) {
            gState.primary = s_newState_801232a0;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;
         }
         break;
      }

      break;
   }
}
