#include "common.h"
#include "state.h"
#include "audio.h"
#include "cd_files.h"
#include "object.h"
#include "window.h"
#include "graphics.h"

void PlayTownBGM(void);
s32 State_Town(void);
void Town_HandleCancel(Object *);
void Town_HandleShopOrDojo(Object *);
s32 Objf580_Town(Object *);
void func_801F5540(Object *);
void func_801F56A4(Object *);

s16 gTownPortraitSets[10][13] = {
    {PORTRAIT_DOJO_MASTER, PORTRAIT_594, PORTRAIT_DIEGO_HAPPY, PORTRAIT_590, PORTRAIT_591,
     PORTRAIT_592, PORTRAIT_593, PORTRAIT_595, PORTRAIT_ASH_ANGRY, PORTRAIT_CLINT_UPSET,
     PORTRAIT_596, PORTRAIT_597, PORTRAIT_598},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_604, PORTRAIT_599, PORTRAIT_600, PORTRAIT_601, PORTRAIT_602,
     PORTRAIT_603, PORTRAIT_687, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_608, PORTRAIT_SARA_ANGRY, PORTRAIT_AMON_62, PORTRAIT_SARA_69,
     PORTRAIT_AMON_ANGRY, PORTRAIT_605, PORTRAIT_607, PORTRAIT_606, PORTRAIT_688, PORTRAIT_682,
     PORTRAIT_NULL, PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_613, PORTRAIT_ZOHAR_ANGRY, PORTRAIT_ZOHAR_HAPPY, PORTRAIT_609,
     PORTRAIT_610, PORTRAIT_611, PORTRAIT_612, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL, PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_617, PORTRAIT_614, PORTRAIT_615, PORTRAIT_616, PORTRAIT_NULL,
     PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_621, PORTRAIT_DARIUS_ANGRY, PORTRAIT_ASH_ANGRY, PORTRAIT_618,
     PORTRAIT_619, PORTRAIT_620, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL, PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_625, PORTRAIT_DIEGO_ANGRY, PORTRAIT_DIEGO_UPSET,
     PORTRAIT_DIEGO_HAPPY, PORTRAIT_ASH_UPSET, PORTRAIT_ASH_ANGRY, PORTRAIT_622, PORTRAIT_623,
     PORTRAIT_624, PORTRAIT_689, PORTRAIT_NULL, PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_629, PORTRAIT_626, PORTRAIT_627, PORTRAIT_628, PORTRAIT_NULL,
     PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_691, PORTRAIT_ELENI_HAPPY, PORTRAIT_ELENI_25, PORTRAIT_ELENI_28,
     PORTRAIT_ASH_ANGRY, PORTRAIT_OROSIUS, PORTRAIT_630, PORTRAIT_631, PORTRAIT_632, PORTRAIT_633,
     PORTRAIT_634, PORTRAIT_690},
    {PORTRAIT_DOJO_MASTER, PORTRAIT_638, PORTRAIT_635, PORTRAIT_636, PORTRAIT_637, PORTRAIT_639,
     PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL, PORTRAIT_NULL,
     PORTRAIT_NULL},
};

void PlayTownBGM(void) {
   PerformAudioCommand(AUDIO_CMD_STOP_SEQ);

   if (gState.townState >= 32) {
      LoadSeqSet(15);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(15));
   } else if (gState.townState >= 27) {
      LoadSeqSet(20);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(20));
   } else if (gState.townState >= 25) {
      LoadSeqSet(18);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(18));
   } else if (gState.townState >= 20) {
      LoadSeqSet(17);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(17));
   } else if (gState.townState >= 18) {
      LoadSeqSet(15);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(15));
   } else if (gState.townState >= 17) {
      LoadSeqSet(14);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(14));
   } else if (gState.townState >= 13) {
      LoadSeqSet(19);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(19));
   } else if (gState.townState >= 11) {
      LoadSeqSet(14);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(14));
   } else if (gState.townState >= 6) {
      LoadSeqSet(21);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(21));
   } else {
      LoadSeqSet(16);
      FinishLoadingSeq();
      PerformAudioCommand(AUDIO_CMD_PLAY_SEQ(16));
   }
}

s32 State_Town(void) {
   s32 i;
   s32 town;

   switch (gState.secondary) {
   case 0:
      gState.suppressLoadingScreen = 1;
      gState.choices[0] = 0;
      Obj_ResetFromIdx10();

      for (i = 1; i < PARTY_CT; i++) {
         SyncPartyUnit(i);
      }

      gState.vsyncMode = 0;
      gClearSavedPadState = 1;
      gState.fieldRenderingDisabled = 1;
      LoadText(CDF_TOWN_T_DAT, gText, gTextPointers);
      gState.currentTextPointers = &gTextPointers[1];

      if (gState.primary == STATE_7) {
         PlayTownBGM();
      }

      if (gState.townState >= 32) {
         town = 9;
         LoadFullscreenImage(CDF_D_GRSGO_TIM);
      } else if (gState.townState >= 27) {
         town = 8;
         LoadFullscreenImage(CDF_D_HEKITI_TIM);
      } else if (gState.townState >= 25) {
         town = 7;
         LoadFullscreenImage(CDF_D_SORUBO_TIM);
      } else if (gState.townState >= 20) {
         town = 6;
         LoadFullscreenImage(CDF_D_KRCH_TIM);
      } else if (gState.townState >= 18) {
         town = 5;
         LoadFullscreenImage(CDF_D_KNRTH_TIM);
      } else if (gState.townState >= 17) {
         town = 4;
         LoadFullscreenImage(CDF_D_TESTA_TIM);
      } else if (gState.townState >= 13) {
         town = 3;
         LoadFullscreenImage(CDF_D_JIGEN_TIM);
      } else if (gState.townState >= 11) {
         town = 2;
         LoadFullscreenImage(CDF_D_USU_TIM);
      } else if (gState.townState >= 6) {
         town = 1;
         LoadFullscreenImage(CDF_D_PORTS_TIM);
      } else {
         town = 0;
         LoadFullscreenImage(CDF_D_SYMRIA_TIM);
      }

      gState.primary = STATE_7;

      gTempObj = Obj_GetUnused();
      gTempObj->functionIndex = OBJF_FULLSCREEN_IMAGE;

      gTempObj = Obj_GetUnused();
      gTempObj->functionIndex = OBJF_TOWN;

      for (i = 24; i < 50; i++) {
         gState.portraitsToLoad[i] = PORTRAIT_NULL;
      }
      for (i = 24; i < 37; i++) {
         gState.portraitsToLoad[i] = gTownPortraitSets[town][i - 24];
      }

      LoadPortraits();
      gState.suppressLoadingScreen = 0;
      gState.secondary++;
      break;

   case 1:
      break;
   }
}

void Town_HandleCancel(Object *town) {
   if (gWindowChoice.raw == 0x3dff) {
      gState.choices[0] = GetWindowChoice(0x3d) - 1;
      CloseWindow(0x3d);
      town->state = 4;
      town->state2 = 0;
   }
}

void Town_HandleShopOrDojo(Object *town) {
   if (gWindowChoice.raw == 0x3d01) {
      gWindowActiveIdx = 0;
      gState.state7 = STATE_SHOP;
      town->state = 99;
      town->state2 = 0;
   }
   if (gWindowChoice.raw == 0x3d02) {
      gWindowActiveIdx = 0;
      gState.state7 = STATE_DOJO;
      town->state = 99;
      town->state2 = 0;
   }
}

#undef OBJF
#define OBJF 580
s32 Objf580_Town(Object *obj) {
   extern s32 D_801F6DA8;

   Object *actions;

   switch (obj->state) {
   case 0:
      D_801F6DA8 = 0;
      FadeInScreen(2, 10);
      DrawWindow(0x41, 0, 0, 312, 90, 4, -390, WBS_DRAGON, 0);
      DisplayCustomWindow(0x41, 0, 1, 50, 0, 0);
      DisplayCustomWindow(0x42, 0, 1, 50, 0, 0);
      DrawWindow(0x43, 0, 100, 312, 90, 4, 540, WBS_DRAGON, 0);
      DisplayCustomWindow(0x43, 0, 1, 50, 0, 0);
      DisplayCustomWindow(0x44, 0, 1, 50, 0, 0);
      obj->state = 3;
      obj->state2 = 0;

   // fallthrough
   case 3:
      func_801F56A4(obj);
      break;

   case 4:

      switch (obj->state2) {
      case 0:
         gSignal1 = 1;
         actions = Obj_GetUnused();
         actions->functionIndex = OBJF_WORLD_ACTIONS;
         obj->state2++;

      // fallthrough
      case 1:
         if (gSignal1 == 0) {
            obj->state = 5;
            obj->state2 = 0;
         }
         break;
      }

      break;

   case 5:

      switch (gState.townState) {
      case 1:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#70\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_12;
               gState.scene = 3;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 1;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 2;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 3:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#70\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 0;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 4;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 2;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 4:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 126, 10, 10, WBS_CROSSED, 6);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#70\n#71\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 4;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d03 || gWindowChoice.raw == 0x3d06 ||
                gWindowChoice.raw == 0x3d01 || gWindowChoice.raw == 0x3d02) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 5;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d04) {
               OBJ.textPtrIdx = 6;
               gWindowActiveIdx = 0;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 5:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#70\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 1;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d05) {
               gState.worldMapDestination = 1;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               if (gState.worldMapState < 12) {
                  gState.worldMapState = 12;
               }
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 7;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 7:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 2;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 4;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 17;
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 8:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#72\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 14;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 10;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 11;
               OBJ.portrait = PORTRAIT_DIEGO;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 9:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#72\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d05) {
               gState.worldMapDestination = 4;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 15;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 12;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 13;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 10:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 3;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 4;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 16;
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 11:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 4;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 7;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               if (gState.worldMapState < 21) {
                  gState.worldMapState = 21;
               }
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 12:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 4;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               SlideWindowTo(0x3d, -200, 10);
               OBJ.timer = 30;
               gWindowActiveIdx = 0;
               obj->state2 = 3;
            }
            break;

         case 2:
            break;

         case 3:
            if (--OBJ.timer == 0) {
               SetupTownMsgBox(PORTRAIT_DOLAN, 0);
               func_800440DC(0);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 4:
            if (--OBJ.timer == 0) {
               func_80044364(14, 2);
               obj->state2++;
            }
            break;

         case 5:
            if (gState.msgBoxFinished) {
               SetupTownMsgBox(PORTRAIT_SARA, 1);
               func_8004404C(0);
               func_8004404C(1);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 6:
            if (--OBJ.timer == 0) {
               func_80044364(15, 1);
               obj->state2++;
            }
            break;

         case 7:
            if (gState.field_0x31d != 0) {
               gState.field_0x31d = 0;
               obj->state2++;
            }
            break;

         case 8:
            if (gState.field_0x31d != 0) {
               SetupTownMsgBox(PORTRAIT_SARA_ANGRY, 1);
               obj->state2++;
            }
            break;

         case 9:
            if (gState.msgBoxFinished) {
               SetupTownMsgBox(PORTRAIT_AMON_62, 0);
               func_80044364(16, 3);
               obj->state2++;
            }
            break;

         case 10:
            if (gState.msgBoxFinished) {
               SetupTownMsgBox(PORTRAIT_SARA_69, 1);
               func_80044364(17, 1);
               obj->state2++;
            }
            break;

         case 11:
            if (gState.msgBoxFinished) {
               SetupTownMsgBox(PORTRAIT_AMON_ANGRY, 0);
               func_80044364(18, 3);
               obj->state2++;
            }
            break;

         case 12:
            if (gState.msgBoxFinished) {
               func_80044134(0);
               func_80044134(1);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 13:
            if (--OBJ.timer == 0) {
               gState.worldMapDestination = 7;
               gState.state7 = STATE_6;
               gState.worldMapState = 22;
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 14:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 5;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 22;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 16:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 12;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 32;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 29;
               OBJ.portrait = PORTRAIT_ZOHAR;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 17:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 6;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 13;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               if (gState.worldMapState < 41) {
                  gState.worldMapState = 41;
               }
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 18:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 7;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               SlideWindowTo(0x3d, -200, 10);
               OBJ.timer = 30;
               obj->state2 = 3;
            }
            break;

         case 3:
            if (--OBJ.timer == 0) {
               SetupTownMsgBox(PORTRAIT_DARIUS_ANGRY, 0);
               func_800440DC(0);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 4:
            if (--OBJ.timer == 0) {
               func_80044364(30, 0);
               obj->state2++;
            }
            break;

         case 5:
            if (gState.msgBoxFinished) {
               SetupTownMsgBox(PORTRAIT_ASH_ANGRY, 1);
               func_8004404C(0);
               func_8004404C(1);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 6:
            if (--OBJ.timer == 0) {
               func_80044364(31, 1);
               obj->state2++;
            }
            break;

         case 7:
            if (gState.msgBoxFinished) {
               func_80044134(0);
               func_80044134(1);
               OBJ.timer = 30;
               obj->state2++;
            }
            break;

         case 8:
            if (--OBJ.timer == 0) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 53;
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 21:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#73\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 8;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 57;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 40;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 22:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 126, 10, 10, WBS_CROSSED, 6);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#73\n#74\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 8;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 60;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 41;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d06) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 42;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 23:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 126, 10, 10, WBS_CROSSED, 6);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#73\n#75\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d05) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_26;
               gState.scene = 63;
               obj->state = 99;
               obj->state2 = 0;
               break;
            }
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 43;
               OBJ.portrait = PORTRAIT_ASH_ANGRY;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 44;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            if (gWindowChoice.raw == 0x3d06) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 43;
               OBJ.portrait = PORTRAIT_ASH_ANGRY;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 24:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 108, 10, 10, WBS_CROSSED, 5);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#73\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 8;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d05) {
               gState.worldMapDestination = 19;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 53;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 41;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 26:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 9;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 23;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               if (gState.worldMapState < 62) {
                  gState.worldMapState = 62;
               }
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 28:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 10;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 0x34;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 30:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 25;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 64;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               OBJ.textPtrIdx = 5;
               OBJ.portrait = PORTRAIT_ASH;
               obj->state2++;
               obj->state3 = 0;
            }
            break;

         case 2:
            func_801F5540(obj);
            break;
         }

         break;

      case 31:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#67");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 11;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 25;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               gState.worldMapState = 65;
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;

      case 32:

         switch (obj->state2) {
         case 0:
            DrawWindow(0x3d, 328, 0, 176, 90, 10, 10, WBS_CROSSED, 4);
            DrawText(328, 11, 20, 2, 0, "#63\n#64\n#65\n#66");
            DisplayBasicWindowWithSetChoice(0x3d, gState.choices[0]);
            gWindowActiveIdx = 0x3d;
            obj->state2++;
            break;

         case 1:
            Town_HandleCancel(obj);
            Town_HandleShopOrDojo(obj);
            if (gWindowChoice.raw == 0x3d03) {
               gWindowActiveIdx = 0;
               gState.state7 = STATE_TAVERN;
               gState.D_80140530 = 12;
               obj->state = 99;
               obj->state2 = 0;
            } else if (gWindowChoice.raw == 0x3d04) {
               gState.worldMapDestination = 29;
               gWindowActiveIdx = 0;
               gState.state7 = STATE_6;
               if (gState.worldMapState < 71) {
                  gState.worldMapState = 71;
               }
               obj->state = 99;
               obj->state2 = 0;
            }
            break;
         }

         break;
      }

      break;

   case 99:

      switch (obj->state2) {
      case 0:
         gWindowActiveIdx = 0;
         if (!(gState.state7 == STATE_26 && (gState.scene == 57 || gState.scene == 14)) &&
             gState.state7 != STATE_SHOP && gState.state7 != STATE_DEPOT &&
             gState.state7 != STATE_TAVERN && gState.state7 != STATE_DOJO) {
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_32_4);
         }
         FadeOutScreen(2, 6);
         OBJ.fadeTimer = 50;
         obj->state2++;

      // fallthrough
      case 1:
         if (--OBJ.fadeTimer == 0) {
            gState.primary = gState.state7;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;
         }
         break;
      }

      break;
   }
}

void func_801F5540(Object *town) {
   switch (town->state3) {
   case 0:
      SlideWindowTo(0x3d, -200, 10);
      town->d.objf580.timer = 30;
      town->state3++;

   // fallthrough
   case 1:
      if (--town->d.objf580.timer == 0) {
         SetupTownMsgBox(town->d.objf580.portrait, 0);
         func_800440DC(0);
         town->d.objf580.timer = 30;
         town->state3++;
      }
      break;

   case 2:
      if (--town->d.objf580.timer == 0) {
         func_80044364(town->d.objf580.textPtrIdx, 0);
         town->state3++;
      }
      break;

   case 3:
      if (gState.msgBoxFinished) {
         func_80044134(0);
         town->d.objf580.timer = 30;
         town->state3++;
      }
      break;

   case 4:
      if (--town->d.objf580.timer == 0) {
         SlideWindowTo(0x3d, 10, 10);
         gWindowActiveIdx = 0x3d;
         town->state2--;
         town->state3 = 0;
      }
      break;
   }
}

void func_801F56A4(Object *town) {
   switch (gState.townState) {
   case 0:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(0, 0);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 1;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 2:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_DIEGO, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(3, 0);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 3;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 6:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(8, 2);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_CLINT, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            func_80044364(9, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 6:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 7;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 13:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_GROG, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(19, 0);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_SARA, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            func_80044364(20, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_80044364(21, 0);
            town->state2++;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 7:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 14;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 15:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_GROG, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(23, 2);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_SARA, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            func_80044364(24, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_80044364(25, 3);
            town->state2++;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 7:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ZOHAR_ANGRY, 1);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 8:
         if (--town->d.objf580.timer == 0) {
            func_80044364(26, 1);
            town->state2++;
         }
         break;

      case 9:
         if (gState.msgBoxFinished) {
            func_80044364(27, 3);
            town->state2++;
         }
         break;

      case 10:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ZOHAR, 1);
            func_80044364(28, 1);
            town->state2++;
         }
         break;

      case 11:
      case 12:
      case 13:
      case 15:
         if (gState.field_0x31d != 0) {
            gState.field_0x31d = 0;
            town->state2++;
         }
         break;

      case 14:
         if (gState.field_0x31d != 0) {
            SetupTownMsgBox(PORTRAIT_ZOHAR_HAPPY, 1);
            town->state2++;
         }
         break;

      case 16:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 17:
         if (--town->d.objf580.timer == 0) {
            gState.state7 = STATE_26;
            gState.scene = 37;
            town->state = 99;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 19:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(32, 0);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            gState.state7 = STATE_6;
            if (gState.worldMapState < 51) {
               gState.worldMapState = 51;
            }
            town->state = 99;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 20:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_CLINT, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(33, 2);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            func_80044364(34, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_DIEGO_ANGRY, 0);
            func_80044364(35, 3);
            town->state2++;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            func_80044364(36, 1);
            town->state2++;
         }
         break;

      case 7:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_DIEGO_UPSET, 0);
            func_80044364(37, 3);
            town->state2++;
         }
         break;

      case 8:
         if (gState.field_0x31d != 0) {
            gState.field_0x31d = 0;
            town->state2++;
         }
         break;

      case 9:
         if (gState.field_0x31d != 0) {
            SetupTownMsgBox(PORTRAIT_DIEGO_HAPPY, 0);
            town->state2++;
         }
         break;

      case 10:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH_UPSET, 1);
            func_80044364(38, 1);
            town->state2++;
         }
         break;

      case 11:
         if (gState.msgBoxFinished) {
            func_80044364(39, 3);
            town->state2++;
         }
         break;

      case 12:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 13:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 21;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 25:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(45, 2);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            gState.state7 = STATE_26;
            gState.scene = 70;
            town->state = 99;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 27:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(46, 2);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_AMON, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            func_80044364(47, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ZOHAR, 1);
            func_80044364(48, 1);
            town->state2++;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            func_80044364(49, 3);
            town->state2++;
         }
         break;

      case 7:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ELENI, 1);
            func_80044364(50, 1);
            town->state2++;
         }
         break;

      case 8:
         if (gState.msgBoxFinished) {
            func_80044364(51, 3);
            town->state2++;
         }
         break;

      case 9:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 10:
         if (--town->d.objf580.timer == 0) {
            gState.townState = 28;
            town->state = 4;
            town->state2 = 0;
         }
         break;
      }

      break;

   case 29:

      switch (town->state2) {
      case 0:
         town->d.objf580.timer = 30;
         town->state2++;

      // fallthrough
      case 1:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_800440DC(0);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 2:
         if (--town->d.objf580.timer == 0) {
            func_80044364(53, 0);
            town->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ELENI_HAPPY, 1);
            func_8004404C(0);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 4:
         if (--town->d.objf580.timer == 0) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(96));
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_128_2);
            func_80044364(54, 1);
            town->state2++;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            func_80044364(55, 3);
            town->state2++;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ELENI, 1);
            func_80044364(56, 1);
            town->state2++;
         }
         break;

      case 7:
      case 9:
         if (gState.field_0x31d != 0) {
            gState.field_0x31d = 0;
            town->state2++;
         }
         break;

      case 8:
         if (gState.field_0x31d != 0) {
            gState.field_0x31d = 0;
            SetupTownMsgBox(PORTRAIT_ELENI_28, 1);
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_1_4);
            town->state2++;
         }
         break;

      case 10:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH_ANGRY, 0);
            func_80044364(57, 3);
            town->state2++;
         }
         break;

      case 11:
         if (gState.msgBoxFinished) {
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 12:
         if (--town->d.objf580.timer == 0) {
            SetupTownMsgBox(PORTRAIT_OROSIUS, 1);
            func_8004404C(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 13:
         if (--town->d.objf580.timer == 0) {
            func_80044364(58, 1);
            town->state2++;
         }
         break;

      case 14:
         if (gState.msgBoxFinished) {
            SetupTownMsgBox(PORTRAIT_ASH, 0);
            func_80044364(59, 3);
            town->state2++;
         }
         break;

      case 15:
         if (gState.msgBoxFinished) {
            func_80044364(60, 1);
            town->state2++;
         }
         break;

      case 16:
         if (gState.msgBoxFinished) {
            func_80044134(0);
            func_80044134(1);
            town->d.objf580.timer = 30;
            town->state2++;
         }
         break;

      case 17:
         if (--town->d.objf580.timer == 0) {
            gState.state7 = STATE_26;
            gState.scene = 73;
            town->state = 99;
            town->state2 = 0;
         }
         break;
      }

      break;

   default:
      town->state = 4;
      town->state2 = 0;
      break;
   }
}
