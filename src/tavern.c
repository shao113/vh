#include "common.h"
#include "state.h"
#include "units.h"
#include "evt.h"
#include "cd_files.h"
#include "window.h"
#include "audio.h"

s32 PlayerHasItem(s32 item) {
   s32 i;

   if (gState.depot[item] != 0) {
      return 1;
   }
   for (i = 1; i < PARTY_CT; i++) {
      if (gPartyMembers[i].inParty) {
         if (gPartyMembers[i].items[0] == item) {
            return 1;
         }
         if (gPartyMembers[i].items[1] == item) {
            return 1;
         }
      }
   }
   return 0;
}

void State_Tavern(void) {
   s32 cdFile;

   switch (gState.secondary) {
   case 0:
      Evt_ResetFromIdx10();
      gState.vsyncMode = 0;
      gClearSavedPadState = 1;
      gState.fieldRenderingDisabled = 1;

      //? This do-while I could see being a macro
      do {
         if (gState.townState >= 0x20) {
            cdFile = CDF_BR_GRSGO_TIM;
         } else if (gState.townState >= 0x1b) {
            cdFile = CDF_BR_HNKYO_TIM;
         } else if (gState.townState >= 0x19) {
            cdFile = CDF_BR_SORBO_TIM;
         } else if (gState.townState >= 0x14) {
            cdFile = CDF_BR_KRCH_TIM;
         } else if (gState.townState >= 0x12) {
            cdFile = CDF_BR_KNRTH_TIM;
         } else if (gState.townState >= 0x11) {
            cdFile = CDF_BR_TESTA_TIM;
         } else if (gState.townState >= 0xd) {
            cdFile = CDF_BR_JIGEN_TIM;
         } else if (gState.townState >= 0xb) {
            cdFile = CDF_BR_USU_TIM;
         } else if (gState.townState >= 0x6) {
            cdFile = CDF_BR_PORTS_TIM;
         } else {
            cdFile = CDF_BR_SYM_TIM;
         }
      } while (0);

      LoadFullscreenImage(cdFile);
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_FULLSCREEN_IMAGE;

      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_TAVERN;

      LoadText(CDF_SAKABA_T_DAT, gText, gTextPointers);
      gState.currentTextPointers = &gTextPointers[1];
      gState.secondary++;
      break;

   case 1:
      break;
   }
}

void func_8004404C(u8 lower) {
   if (lower == 0) {
      if (gState.primary != STATE_7) {
         SlideWindowTo(0x41, 4, 10);
         SlideWindowTo(0x42, 252, 10);
      } else {
         SlideWindowTo(0x41, 4, 44);
         SlideWindowTo(0x42, 252, 44);
      }
   } else {
      SlideWindowTo(0x43, 4, 140);
      SlideWindowTo(0x44, 252, 140);
   }
}

void func_800440DC(u8 lower) {
   if (lower == 0) {
      SlideWindowTo(0x41, 4, 75);
      SlideWindowTo(0x42, 252, 75);
   } else {
      SlideWindowTo(0x43, 4, 75);
      SlideWindowTo(0x44, 252, 75);
   }
}

void func_80044134(u8 lower) {
   if (lower == 0) {
      SlideWindowTo(0x41, 4, -90);
      SlideWindowTo(0x42, 252, -90);
   } else {
      SlideWindowTo(0x43, 4, 250);
      SlideWindowTo(0x44, 252, 250);
   }
}

void SetupTownMsgBox(s16 portraitId, u8 lower) {
   s32 i;
   s32 windowId;
   EvtData *evt1;
   EvtData *evt2;

   if (lower == 0) {
      DrawWindow(0x41, 0, 0, 312, 90, 4, -390, WBS_DRAGON, 0);
   } else {
      DrawWindow(0x43, 0, 100, 312, 90, 4, 540, WBS_DRAGON, 0);
   }

   evt1 = gEvtDataArray;
   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt1->functionIndex == EVTF_MSGBOX_PORTRAIT && evt1->d.evtf413.flipped == lower) {
         evt2 = evt1->d.evtf413.faceSprite;
         evt2->functionIndex = EVTF_NULL;
         evt2 = evt1->d.evtf413.speakSprite;
         evt2->functionIndex = EVTF_NULL;
         evt2 = evt1->d.evtf413.blinkSprite;
         evt2->functionIndex = EVTF_NULL;
         break;
      }
      evt1++;
   }
   if (i == EVT_DATA_CT) {
      evt1 = Evt_GetLastUnused();
   }
   evt1->functionIndex = EVTF_MSGBOX_PORTRAIT;
   evt1->state = 0;
   evt1->state2 = 0;
   evt1->state3 = 0;
   evt1->d.evtf413.flipped = lower;
   evt1->d.evtf413.portrait.id = portraitId;

   if (!evt1->d.evtf413.flipped) {
      evt1->d.evtf413.anchorOfsX = -100;
      evt1->d.evtf413.anchorOfsY = -24;
   } else {
      evt1->d.evtf413.anchorOfsX = 116;
      evt1->d.evtf413.anchorOfsY = -24;
   }

   if (!evt1->d.evtf413.flipped) {
      windowId = 0x41;
   } else {
      windowId = 0x43;
   }

   if (gState.primary == STATE_5) {
      windowId = 0x34;
   }

   evt2 = gEvtDataArray;
   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt2->functionIndex == EVTF_WINDOW_TBD_005 && evt2->d.evtf004.windowId == windowId) {
         break;
      }
      evt2++;
   }
   evt1->d.evtf413.anchor = evt2;
}

void func_80044364(s16 textPtrIdx, s8 param_2) {
   gState.msgBoxFinished = 0;
   gState.field_0x31d = 0;
   MsgBox_SetText(param_2 + 3, textPtrIdx, 0x80);
}

void func_800443A8(void) {
   DrawWindow(0x38, 0, 200, 104, 54, 366, 93, WBS_CROSSED, 2);
   DrawText(12, 211, 20, 3, 0, "#79\n#80");
   DisplayBasicWindow(0x38);
   SlideWindowTo(0x38, 116, 93);
   gWindowActiveIdx = 0x38;
}

void func_80044440(void) {
   SlideWindowTo(0x38, 116, 93);
   gWindowActiveIdx = 0x38;
}

void func_80044474(void) {
   SlideWindowTo(0x38, -180, 20);
   SlideWindowTo(0x39, 124, 266);
}

void func_800444AC(EvtData *tavernEvt) {
   if (gState.msgBoxFinished) {
      func_80044134(0);
      SlideWindowTo(0x38, 116, 93);
      gWindowActiveIdx = 0x38;
      tavernEvt->state2 = 1;
   }
}

void func_8004450C(EvtData *tavernEvt) {
   if (--tavernEvt->d.evtf576.timer == 0) {
      func_80044364(tavernEvt->d.evtf576.textPtrIdx, 2);
      tavernEvt->state3++;
   }
}

#undef EVTF
#define EVTF 576
s32 Evtf576_Tavern(EvtData *evt) {
   //? Unused return value
   s32 i;

   gWindowChoiceHeight = 18;
   gWindowChoicesTopMargin = 10;
   if (gWindowActiveIdx == 0x39) {
      gState.choices[0] = GetWindowChoice(0x39) - 1;
   }

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         gState.choices[0] = 0;
         FadeInScreen(2, 10);
         DrawWindow(0x41, 0, 0, 312, 90, 4, -390, WBS_DRAGON, 0);
         DisplayCustomWindow(0x41, 0, 1, 50, 0, 0);
         DisplayCustomWindow(0x42, 0, 1, 50, 0, 0);
         DrawWindow(0x43, 0, 100, 312, 90, 4, 540, WBS_DRAGON, 0);
         DisplayCustomWindow(0x43, 0, 1, 50, 0, 0);
         DisplayCustomWindow(0x44, 0, 1, 50, 0, 0);
         EVT.timer = 30;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            evt->state++;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 1:

      switch (gState.D_80140530) {
      case 0:

         switch (evt->state2) {
         case 0:
            func_800443A8();

            for (i = 0; i < 4; i++) {
               EVT.needSpeak[i] = 1;
            }

            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#82\n#84\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state3 = 0;
               EVT.timer = 30;

               if (!EVT.needSpeak[0] && !EVT.needSpeak[1] && !EVT.needSpeak[2] &&
                   !EVT.needSpeak[3]) {
                  evt->state2 = 4;
                  evt->state3 = 0;
               } else {
                  func_80044474();
                  func_800440DC(0);
                  SetupTownMsgBox(PORTRAIT_DIEGO, 0);
                  EVT.textPtrIdx = 4;
                  gWindowActiveIdx = 0;
                  evt->state2 += 2;
               }
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_590, 0);
               EVT.needSpeak[0] = 0;
               EVT.textPtrIdx = 0;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_591, 0);
               EVT.needSpeak[1] = 0;
               EVT.textPtrIdx = 1;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_592, 0);
               EVT.needSpeak[2] = 0;
               EVT.textPtrIdx = 2;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_593, 0);
               EVT.needSpeak[3] = 0;
               EVT.textPtrIdx = 3;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_595, 0);
               evt->state3++;
               break;

            case 1:
               if (--EVT.timer == 0) {
                  PerformAudioCommand(AUDIO_CMD_STOP_SEQ);
                  LoadSeqSet(0x10);
                  FinishLoadingSeq();
                  PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(29));
                  func_80044364(5, 2);
                  evt->state3++;
               }
               break;

            case 2:
               if (gState.msgBoxFinished) {
                  func_8004404C(0);
                  func_8004404C(1);
                  SetupTownMsgBox(PORTRAIT_ASH_ANGRY, 1);
                  func_80044364(6, 1);
                  evt->state3++;
               }
               break;

            case 3:
               if (gState.msgBoxFinished) {
                  SetupTownMsgBox(PORTRAIT_CLINT, 1);
                  func_80044364(7, 1);
                  evt->state3++;
               }
               break;

            case 4:
               if (gState.msgBoxFinished) {
                  func_80044364(8, 3);
                  evt->state3++;
               }
               break;

            case 5:
               if (gState.msgBoxFinished) {
                  SetupTownMsgBox(PORTRAIT_DIEGO, 1);
                  func_80044364(9, 1);
                  evt->state3++;
               }
               break;

            case 6:
               if (gState.msgBoxFinished) {
                  func_80044364(10, 0);
                  evt->state3++;
               }
               break;

            case 7:
               if (gState.msgBoxFinished) {
                  SetupTownMsgBox(PORTRAIT_CLINT_UPSET, 1);
                  func_80044364(0xb, 1);
                  evt->state3++;
               }
               break;

            case 8:
               if (gState.msgBoxFinished) {
                  SetupTownMsgBox(PORTRAIT_ASH, 1);
                  func_80044364(0xc, 1);
                  evt->state3++;
               }
               break;

            case 9:
               if (gState.msgBoxFinished) {
                  evt->state = 99;
                  evt->state2 = 0;
               }
               break;
            } // switch (state3) (via state:1 -> D_80140530:0 -> state2:4)

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:0)

         break;

      case 1:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#82\n#84\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_590, 0);
               EVT.textPtrIdx = 0xd;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_596, 0);
               EVT.textPtrIdx = 0xe;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_597, 0);
               EVT.textPtrIdx = 0xf;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_598, 0);
               EVT.textPtrIdx = 0x10;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:1)

         break;

      case 2:

         switch (evt->state2) {
         case 0:
            func_800443A8();

            for (i = 0; i < 3; i++) {
               EVT.needSpeak[i] = 1;
            }

            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 90, 424, 75, WBS_CROSSED, 4);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 75);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state3 = 0;
               EVT.timer = 30;

               if (!EVT.needSpeak[0] && !EVT.needSpeak[1] && !EVT.needSpeak[2] &&
                   !EVT.needSpeak[3]) {
                  evt->state2 = 4;
                  evt->state3 = 0;
               } else {
                  func_80044474();
                  func_800440DC(0);
                  SetupTownMsgBox(PORTRAIT_CLINT, 0);
                  EVT.textPtrIdx = 0x14;
                  gWindowActiveIdx = 0;
                  evt->state2 += 2;
               }
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_599, 0);
               EVT.needSpeak[0] = 0;
               EVT.textPtrIdx = 0x11;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_600, 0);
               EVT.needSpeak[1] = 0;
               EVT.textPtrIdx = 0x12;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_601, 0);
               EVT.needSpeak[2] = 0;
               EVT.textPtrIdx = 0x13;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_599, 0);
               evt->state3++;
               break;

            case 1:
               if (--EVT.timer == 0) {
                  func_80044364(0x15, 2);
                  evt->state3++;
               }
               break;

            case 2:
               if (gState.msgBoxFinished) {
                  func_8004404C(0);
                  func_8004404C(1);
                  SetupTownMsgBox(PORTRAIT_ASH, 1);
                  func_80044364(0x16, 1);
                  evt->state3++;
               }
               break;

            case 3:
               if (gState.msgBoxFinished) {
                  func_80044364(0x17, 0);
                  evt->state3++;
               }
               break;

            case 4:
               if (gState.msgBoxFinished) {
                  func_80044364(0x18, 1);
                  evt->state3++;
               }
               break;

            case 5:
               if (gState.msgBoxFinished) {
                  evt->state = 99;
                  evt->state2 = 0;
               }
               break;
            } // switch (state3) (via state:1 -> D_80140530:2 -> state2:4)

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:2)

         break;

      case 3:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#84\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_599, 0);
               EVT.textPtrIdx = 0x19;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_602, 0);
               EVT.textPtrIdx = 0x1a;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_601, 0);
               EVT.textPtrIdx = 0x1b;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_687, 0);
               EVT.textPtrIdx = 0x3d;
               evt->state2++;

               if (!PlayerHasItem(ITEM_NOVA_KEY) && !PlayerHasItem(ITEM_NOVA_P_)) {
                  evt->state2++;
               }
            }

            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               if (--EVT.timer == 0) {
                  func_80044364(0x3b, 2);
                  evt->state3++;
               }
               break;

            case 1:
               if (gState.msgBoxFinished) {
                  gState.depot[ITEM_NOVA_KEY] += 1;
                  DrawWindow(0x3c, 0, 150, 240, 36, 40, 160, WBS_CROSSED, 0);
                  DrawText(12, 160, 25, 2, 0, gState.currentTextPointers[0x3c]);
                  DisplayBasicWindow(0x3c);
                  evt->state3++;
               }
               break;

            case 2:
               if (PressedCircleOrX()) {
                  CloseWindow(0x3c);
                  func_80044134(0);
                  SlideWindowTo(0x38, 116, 93);
                  gWindowActiveIdx = 0x38;
                  evt->state2 = 1;
               }
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:3)

         break;

      case 4:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#82\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_605, 0);
               EVT.textPtrIdx = 0x1c;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_607, 0);
               EVT.textPtrIdx = 0x1d;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_606, 0);
               EVT.textPtrIdx = 0x1e;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               EVT.textPtrIdx = 0x41;
               evt->state2++;

               i = PORTRAIT_688;
               if (!PlayerHasItem(ITEM_EARTH_K_) && !PlayerHasItem(ITEM_ICE_P_)) {
                  if (!PlayerHasItem(ITEM_BANANA)) {
                     EVT.textPtrIdx = 0x3e;
                  } else {
                     i = PORTRAIT_682;
                     evt->state2++;
                  }
               }
               SetupTownMsgBox(i, 0);
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               if (--EVT.timer == 0) {
                  func_80044364(0x3f, 2);
                  evt->state3++;
               }
               break;

            case 1:
               if (gState.msgBoxFinished) {
                  gState.depot[ITEM_EARTH_K_] += 1;
                  DrawWindow(0x3c, 0, 150, 240, 36, 40, 160, WBS_CROSSED, 0);
                  DrawText(12, 160, 25, 2, 0, gState.currentTextPointers[0x40]);
                  DisplayBasicWindow(0x3c);
                  evt->state3++;
               }
               break;

            case 2:
               if (PressedCircleOrX()) {
                  CloseWindow(0x3c);
                  func_80044134(0);
                  SlideWindowTo(0x38, 116, 93);
                  gWindowActiveIdx = 0x38;
                  evt->state2 = 1;
               }
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:4)

         break;

      case 5:

         switch (evt->state2) {
         case 0:
            func_800443A8();

            for (i = 0; i < 4; i++) {
               EVT.needSpeak[i] = 1;
            }

            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#85\n#83\n#84\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state3 = 0;
               EVT.timer = 30;

               if (!EVT.needSpeak[0] && !EVT.needSpeak[1] && !EVT.needSpeak[2] &&
                   !EVT.needSpeak[3]) {
                  evt->state = 99;
                  evt->state2 = 0;
               } else {
                  func_80044474();
                  func_800440DC(0);
                  SetupTownMsgBox(PORTRAIT_ASH, 0);
                  EVT.textPtrIdx = 0x25;
                  gWindowActiveIdx = 0;
                  evt->state2 += 2;
               }
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               EVT.needSpeak[0] = 0;
               evt->state2 = 4;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_610, 0);
               EVT.needSpeak[1] = 0;
               EVT.textPtrIdx = 0x22;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_611, 0);
               EVT.needSpeak[2] = 0;
               EVT.textPtrIdx = 0x23;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_612, 0);
               EVT.needSpeak[3] = 0;
               EVT.textPtrIdx = 0x24;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_609, 0);
               evt->state3++;
               break;

            case 1:
               if (--EVT.timer == 0) {
                  func_80044364(0x1f, 2);
                  evt->state3++;
               }
               break;

            case 2:
               if (gState.msgBoxFinished) {
                  func_8004404C(0);
                  func_8004404C(1);
                  SetupTownMsgBox(PORTRAIT_ASH, 1);
                  func_80044364(0x20, 1);
                  evt->state3++;
               }
               break;

            case 3:
               if (gState.msgBoxFinished) {
                  func_80044364(0x21, 0);
                  evt->state3++;
               }
               break;

            case 4:
               if (gState.msgBoxFinished) {
                  func_80044134(0);
                  func_80044134(1);
                  SlideWindowTo(0x38, 116, 93);
                  gWindowActiveIdx = 0x38;
                  evt->state2 = 1;
               }
               break;
            } // switch (state3) (via state:1 -> D_80140530:5 -> state2:4)

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:5)

         break;

      case 6:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 90, 424, 74, WBS_CROSSED, 4);
               DrawText(372, 10, 20, 3, 0, "#81\n#82\n#84\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_614, 0);
               EVT.textPtrIdx = 0x26;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_615, 0);
               EVT.textPtrIdx = 0x27;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_616, 0);
               EVT.textPtrIdx = 0x28;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:6)

         break;

      case 7:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 90, 424, 74, WBS_CROSSED, 4);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_618, 0);
               EVT.textPtrIdx = 0x29;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_619, 0);
               EVT.textPtrIdx = 0x2a;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_620, 0);
               EVT.textPtrIdx = 0x2b;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:7)

         break;

      case 8:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#85\n#84\n#83\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_622, 0);
               EVT.textPtrIdx = 0x2c;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_623, 0);
               EVT.textPtrIdx = 0x2d;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_624, 0);
               EVT.textPtrIdx = 0x2e;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_689, 0);
               EVT.textPtrIdx = 0x45;
               evt->state2++;

               if (!PlayerHasItem(ITEM_CHAOS_K_) && !PlayerHasItem(ITEM_CHAOS_P_)) {
                  if (PlayerHasItem(ITEM_MACROMAN) && PlayerHasItem(ITEM_TAROT) &&
                      PlayerHasItem(ITEM_RAMEN)) {
                     evt->state2++;
                  } else {
                     EVT.textPtrIdx = 0x42;
                  }
               }
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               if (--EVT.timer == 0) {
                  func_80044364(0x43, 2);
                  evt->state3++;
               }
               break;

            case 1:
               if (gState.msgBoxFinished) {
                  gState.depot[ITEM_CHAOS_K_] += 1;
                  DrawWindow(0x3c, 0, 150, 240, 36, 40, 160, WBS_CROSSED, 0);
                  DrawText(12, 160, 25, 2, 0, gState.currentTextPointers[0x44]);
                  DisplayBasicWindow(0x3c);
                  evt->state3++;
               }
               break;

            case 2:
               if (PressedCircleOrX()) {
                  CloseWindow(0x3c);
                  func_80044134(0);
                  SlideWindowTo(0x38, 116, 93);
                  gWindowActiveIdx = 0x38;
                  evt->state2 = 1;
               }
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:8)

         break;

      case 9:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 90, 424, 74, WBS_CROSSED, 4);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_626, 0);
               EVT.textPtrIdx = 0x2f;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_627, 0);
               EVT.textPtrIdx = 0x30;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_628, 0);
               EVT.textPtrIdx = 0x31;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:9)

         break;

      case 10:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 90, 424, 66, WBS_CROSSED, 4);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#84\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_630, 0);
               EVT.textPtrIdx = 0x32;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_631, 0);
               EVT.textPtrIdx = 0x33;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_632, 0);
               EVT.textPtrIdx = 0x34;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:10)

         break;

      case 11:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#84\n#85\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_630, 0);
               EVT.textPtrIdx = 0x35;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_633, 0);
               EVT.textPtrIdx = 0x36;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_634, 0);
               EVT.textPtrIdx = 0x37;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_690, 0);
               EVT.textPtrIdx = 0x46;
               evt->state2++;
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:11)

         break;

      case 12:

         switch (evt->state2) {
         case 0:
            func_800443A8();
            evt->state2++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3801) {
               SlideWindowTo(0x38, 20, 20);
               DrawWindow(0x39, 368, 0, 104, 108, 424, 66, WBS_CROSSED, 5);
               DrawText(372, 10, 20, 3, 0, "#81\n#83\n#85\n#86\n#87");
               DisplayBasicWindowWithSetChoice(0x39, gState.choices[0]);
               SlideWindowTo(0x39, 124, 66);
               gWindowActiveIdx = 0x39;
               evt->state2++;
               evt->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3802 || gWindowChoice.raw == 0x38ff) {
               evt->state = 99;
               evt->state2 = 0;
            }
            break;

         case 2:
            EVT.timer = 30;
            if (gWindowChoice.raw == 0x39ff || gWindowChoice.raw == 0x3905) {
               func_80044474();
               func_80044440();
               evt->state2--;
            }
            if (gWindowChoice.raw == 0x3901) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_635, 0);
               EVT.textPtrIdx = 0x38;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3902) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_636, 0);
               EVT.textPtrIdx = 0x39;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3903) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_637, 0);
               EVT.textPtrIdx = 0x3a;
               evt->state2++;
            }
            if (gWindowChoice.raw == 0x3904) {
               func_80044474();
               func_800440DC(0);
               SetupTownMsgBox(PORTRAIT_639, 0);
               EVT.textPtrIdx = 0x4a;
               evt->state2++;

               if (!PlayerHasItem(ITEM_HEAVEN_K) && !PlayerHasItem(ITEM_HEAVEN_P)) {
                  if (PlayerHasItem(ITEM_NOVA_P_) && PlayerHasItem(ITEM_ICE_P_) &&
                      PlayerHasItem(ITEM_MANA_P_) && PlayerHasItem(ITEM_CHAOS_P_) &&
                      PlayerHasItem(ITEM_LOGO_P_)) {
                     evt->state2++;
                  } else {
                     EVT.textPtrIdx = 0x47;
                  }
               }
            }
            break;

         case 3:

            switch (evt->state3) {
            case 0:
               func_8004450C(evt);
               break;
            case 1:
               func_800444AC(evt);
               break;
            }

            break;

         case 4:

            switch (evt->state3) {
            case 0:
               if (--EVT.timer == 0) {
                  func_80044364(0x48, 2);
                  evt->state3++;
               }
               break;

            case 1:
               if (gState.msgBoxFinished) {
                  gState.depot[ITEM_HEAVEN_K] += 1;
                  DrawWindow(0x3c, 0, 150, 240, 36, 40, 160, WBS_CROSSED, 0);
                  DrawText(12, 160, 25, 2, 0, gState.currentTextPointers[0x49]);
                  DisplayBasicWindow(0x3c);
                  evt->state3++;
               }
               break;

            case 2:
               if (PressedCircleOrX()) {
                  CloseWindow(0x3c);
                  func_80044134(0);
                  SlideWindowTo(0x38, 116, 93);
                  gWindowActiveIdx = 0x38;
                  evt->state2 = 1;
               }
               break;
            }

            break;
         } // switch (evt->state2) (via state:1 -> D_80140530:12)

         break;
      } // switch (gState.D_80140530) (via state:1)

      break;

   case 99:

      switch (evt->state2) {
      case 0:
         gWindowActiveIdx = 0;
         EVT.timer = 20;
         SlideWindowTo(0x38, -120, 93);
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            FadeOutScreen(2, 6);
            FadeOutScreen(2, 6);
            EVT.needSpeak[0] = 50; //<- Reused as a timer here
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.needSpeak[0] == 0) {
            gState.primary = STATE_20;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;

            switch (gState.D_80140530) {
            case 0:
               gState.townState = 4;
               break;
            case 1:
               gState.townState = 5;
               break;
            case 2:
               gState.townState = 8;
               break;
            case 3:
               gState.townState = 0xa;
               break;
            case 4:
               if (gState.townState < 0xb) {
                  gState.townState = 0xb;
               }
               break;
            case 5:
               gState.townState = 0xf;
               break;
            case 6:
               gState.townState = 0x11;
               break;
            case 7:
               gState.townState = 0x12;
               break;
            case 8:
               if (gState.townState < 0x14) {
                  gState.townState = 0x14;
               }
               break;
            case 9:
               if (gState.townState < 25) {
                  gState.townState = 25;
               }
               break;
            case 10:
               gState.townState = 0x1d;
               break;
            case 11:
               gState.townState = 0x1f;
               break;
            case 12:
               gState.townState = 0x20;
               break;
            } // switch (gState.D_80140530) (via state:99 -> state2:2)

            break;
         }
         break;
      } // switch (evt->state2) (via state:99)

      break;
   }
}