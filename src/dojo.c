#include "common.h"
#include "graphics.h"
#include "units.h"
#include "state.h"
#include "evt.h"
#include "cd_files.h"
#include "window.h"
#include "audio.h"

u8 gClassAdvancementNames[18][17] = {
    "",          "Champion", "Paragon", "Swordsman", "Duelist",    "Guardsman",
    "Dragoon",   "Bowman",   "Sniper",  "Hawknight", "Sky Lord",   "Sorcerer",
    "Enchanter", "Monk",     "Ninja",   "Bishop",    "Archbishop", "Vandalier",
};

u8 gPartyClassAdvancements[12][3][2] = {
    {{1, 0}, {2, 0}, {17, 0}},    {{3, 5}, {4, 0}, {6, 0}},     {{7, 9}, {8, 0}, {10, 0}},
    {{11, 13}, {12, 0}, {14, 0}}, {{15, 13}, {16, 0}, {14, 0}}, {{7, 9}, {8, 0}, {10, 0}},
    {{3, 5}, {4, 0}, {6, 0}},     {{3, 5}, {4, 0}, {6, 0}},     {{7, 9}, {8, 0}, {10, 0}},
    {{15, 13}, {16, 0}, {14, 0}}, {{11, 13}, {12, 0}, {14, 0}}, {{7, 9}, {8, 0}, {10, 0}},
};

u8 gPartyClassPathB1StartingWeapons[13] = {
    ITEM_NULL,     ITEM_NULL,     ITEM_IRON_AXE, ITEM_I_SPEAR,  ITEM_IRONCLAW,
    ITEM_IRONCLAW, ITEM_I_SPEAR,  ITEM_IRON_AXE, ITEM_IRON_AXE, ITEM_I_SPEAR,
    ITEM_IRONCLAW, ITEM_IRONCLAW, ITEM_I_SPEAR,
};

s16 gDojoPortraitSet[62] = {
    PORTRAIT_ASH,
    PORTRAIT_ASH_CHAMPION,
    PORTRAIT_ASH_169,
    PORTRAIT_ASH_PARAGON,
    PORTRAIT_ASH_VANDALIER,
    PORTRAIT_CLINT,
    PORTRAIT_CLINT_SWORDSMAN,
    PORTRAIT_CLINT_GUARDSMAN,
    PORTRAIT_CLINT_DUELIST,
    PORTRAIT_CLINT_DRAGOON,
    PORTRAIT_DIEGO,
    PORTRAIT_DIEGO_BOWMAN,
    PORTRAIT_DIEGO_HAWKNIGHT,
    PORTRAIT_DIEGO_SNIPER,
    PORTRAIT_DIEGO_SKY_LORD,
    PORTRAIT_ELENI,
    PORTRAIT_ELENI_SORCERER,
    PORTRAIT_ELENI_MONK,
    PORTRAIT_ELENI_ENCHANTER,
    PORTRAIT_ELENI_NINJA,
    PORTRAIT_HUXLEY,
    PORTRAIT_HUXLEY_BISHOP,
    PORTRAIT_HUXLEY_MONK,
    PORTRAIT_HUXLEY_ARCHBISHOP,
    PORTRAIT_DOJO_MASTER,
    PORTRAIT_HUXLEY_NINJA,
    PORTRAIT_KIRA,
    PORTRAIT_KIRA_BOWMAN,
    PORTRAIT_KIRA_HAWKNIGHT,
    PORTRAIT_KIRA_SNIPER,
    PORTRAIT_KIRA_SKY_LORD,
    PORTRAIT_GROG,
    PORTRAIT_GROG_SWORDSMAN,
    PORTRAIT_GROG_GUARDSMAN,
    PORTRAIT_GROG_DUELIST,
    PORTRAIT_GROG_DRAGOON,
    PORTRAIT_DOLAN,
    PORTRAIT_DOLAN_SWORDSMAN,
    PORTRAIT_DOLAN_GUARDSMAN,
    PORTRAIT_DOLAN_DUELIST,
    PORTRAIT_DOLAN_DRAGOON,
    PORTRAIT_AMON,
    PORTRAIT_AMON_BOWMAN,
    PORTRAIT_AMON_HAWKNIGHT,
    PORTRAIT_AMON_SNIPER,
    PORTRAIT_AMON_SKY_LORD,
    PORTRAIT_SARA,
    PORTRAIT_SARA_BISHOP,
    PORTRAIT_SARA_MONK,
    PORTRAIT_SARA_ARCHBISHOP,
    PORTRAIT_SARA_NINJA,
    PORTRAIT_ZOHAR,
    PORTRAIT_ZOHAR_SORCERER,
    PORTRAIT_ZOHAR_MONK,
    PORTRAIT_ZOHAR_ENCHANTER,
    PORTRAIT_ZOHAR_NINJA,
    PORTRAIT_DARIUS,
    PORTRAIT_DARIUS_BOWMAN,
    PORTRAIT_DARIUS_HAWKNIGHT,
    PORTRAIT_DARIUS_SNIPER,
    PORTRAIT_DARIUS_SKY_LORD,
};

void Dojo_DisplayUnitPreview(u8 partyIdx) {
   s32 i;
   s32 n;

   i = 0;
   if (gPartyMembers[partyIdx].advLevelFirst != 0) {
      if (gPartyMembers[partyIdx].advChosePathB) {
         i = 3;
      } else {
         i = 1;
      }
      if (gPartyMembers[partyIdx].advLevelSecond != 0) {
         i += 1;
      }
   }

   gGfxSubTextures[GFX_UNIT_PREVIEW][0] = i * 48;                      // x
   gGfxSubTextures[GFX_UNIT_PREVIEW][1] = (partyIdx - 1) / 2 % 5 * 48; // y

   i = 12;
   n = partyIdx;

   if (n > 10) {
      // Last two (Zohar and Darius) are in the bottom half of VRAM
      i = 28;
      n -= 10;
   }

   n--;
   if (n % 2 != 0) {
      // Alternate between the two neighboring cells (12/13 or 28/29)
      i++;
   }

   gGfxTPageIds[GFX_UNIT_PREVIEW] = gTPageIds[i];
   gGfxClutIds[GFX_UNIT_PREVIEW] = gClutIds[gUnitClutIds[gUnits[partyIdx].unitId]];
   DisplayCustomWindow(3, 5, 0, 5, 0, 0);
}

void State_Dojo(void) {
   RECT rect;
   TIM_IMAGE tim;
   s32 i;

   switch (gState.secondary) {
   case 0:
      Evt_ResetFromIdx10();
      ClearUnits();
      gState.vsyncMode = 0;
      gClearSavedPadState = 1;
      gState.fieldRenderingDisabled = 1;

      LoadFullscreenImage(CDF_D_TNSK_TIM);
      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_FULLSCREEN_IMAGE;
      DrawSync(0);

      if (gState.primary == STATE_TRIAL_COMPLETE) {
         PlayTownBGM();
      }

      // Sprites
      LoadCdFile(CDF_F_TENS_TIM, 0);
      OpenTIM(gScratch3_80180210);

      if (ReadTIM(&tim)) {
         rect.x = 768;
         rect.y = 0;
         rect.w = 240 >> 2;
         rect.h = 240;
         LoadImage(&rect, tim.paddr);
         DrawSync(0);
         rect.x = 832;
         LoadImage(&rect, tim.paddr + 0x1c20);
         DrawSync(0);
         rect.x = 768;
         rect.y = 256;
         rect.h = 48;
         LoadImage(&rect, tim.paddr + 0x3840);
         DrawSync(0);
         rect.x = 832;
         LoadImage(&rect, tim.paddr + 0x3de0);
         DrawSync(0);
      }

      // Portraits
      LoadCdFile(CDF_F_TEN2_TIM, 0);
      OpenTIM(gScratch3_80180210);

      if (ReadTIM(&tim)) {
         rect.x = 640;
         rect.y = 0;
         rect.w = 240 >> 2;
         rect.h = 240;
         LoadImage(&rect, tim.paddr);
         DrawSync(0);
         rect.x = 448;
         LoadImage(&rect, tim.paddr + 0x1c20);
         DrawSync(0);
         rect.x = 320;
         LoadImage(&rect, tim.paddr + 0x3840);
         DrawSync(0);
      }

      for (i = 0; i < ARRAY_COUNT(gDojoPortraitSet); i++) {
         gState.currentPortraits[i] = PORTRAIT_NULL;
         gState.portraitsToLoad[i] = gDojoPortraitSet[i];
         gPortraitClutIds[i] = gPortraitsDb.cluts[gState.portraitsToLoad[i]];
      }

      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_DOJO;
      gState.secondary++;
      break;

   case 1:
      break;
   }
}

#undef EVTF
#define EVTF 578
void Evtf578_Dojo(EvtData *evt) {
   extern u8 s_dojoPartyRows;
   extern u8 s_dojoVandalierUnlocked;

   s32 i, j;
   u8 partyIdx;
   EvtData *portrait;

   gClearSavedPadState = 1;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         gState.choices[0] = 0;
         gState.choices[1] = 0;
         FadeInScreen(2, 10);
         LoadText(CDF_TENS_T_DAT, gText, gTextPointers);
         gState.currentTextPointers = &gTextPointers[1];
         j = 0;
         for (i = 1; i < PARTY_CT; i++) {
            if (gPartyMembers[i].inParty) {
               gCurrentParty[j++] = i;
            }
         }
         gCurrentParty[j] = 0; // Null terminator

         i = 0;
         while (gCurrentParty[i] != 0) {
            i++;
         }
         if (i > 7) {
            i = 7;
         }
         s_dojoPartyRows = i;

         gState.vsyncMode = 0;
         DrawWindow(0x41, 0, 0, 312, 90, 4, -390, WBS_DRAGON, 0);
         DisplayCustomWindow(0x41, 0, 1, 50, 0, 0);
         DisplayCustomWindow(0x42, 0, 1, 50, 0, 0);
         SetupTownMsgBox(PORTRAIT_DOJO_MASTER, 0);
         portrait = Evt_GetLastUnused();
         portrait->functionIndex = EVTF_UNIT_PORTRAIT;
         portrait->d.evtf447.windowId = 0x3e;
         DrawWindow(0x3e, 400, 0, 64, 63, 210, 280, WBS_CROSSED, 0);
         DisplayCustomWindow(0x3e, 2, 1, 20, 0, 0);
         EVT.timer = 40;
         evt->state2++;
         break;

      case 1:
         if (--EVT.timer == 0) {
            func_8004404C(0);
            EVT.timer = 20;
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            evt->state++;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 1:

      switch (evt->state2) {
      case 0:
         if (gState.primary == STATE_TRIAL_COMPLETE) {
            evt->state = 99;
            evt->state2 = 0;
         } else {
            func_80044364(0, 0);
            evt->state2++;
         }
         break;

      case 1:
         if (func_80037C28()) {
            evt->state++;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 2:

      switch (evt->state2) {
      case 0:
         gWindowChoiceHeight = 17;
         gWindowChoicesTopMargin = 10;
         DrawWindow(0x36, 0, 90, 112, 72, 410, 110, WBS_CROSSED, 3);
         DrawText(12, 101, 10, 2, 0, "#76");
         DisplayCustomWindow(0x36, 5, 1, 40, 0, 0);
         SlideWindowTo(0x36, 110, 110);
         gWindowActiveIdx = 0x36;
         evt->state2 += 2;
         break;

      case 1:
         SlideWindowTo(0x36, 110, 110);
         gWindowActiveIdx = 0x36;
         evt->state2++;
         break;

      case 2:
         if (gWindowChoice.raw == 0x3603 || gWindowChoice.raw == 0x36ff) {
            gWindowActiveIdx = 0;
            evt->state = 99;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3601) {
            SlideWindowTo(0x36, 12, 90);
            gWindowActiveIdx = 0;
            evt->state = 3;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3602) {
            SlideWindowTo(0x36, -150, 110);
            gWindowActiveIdx = 0;
            evt->state = 10;
            evt->state2 = 0;
         }
         break;

      case 3:
         if (gWindowChoice.raw == 0x3602) {
            SlideWindowTo(0x36, 12, 90);
            gWindowActiveIdx = 0;
            evt->state = 10;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 3:

      switch (evt->state2) {
      case 0:
         func_80044364(1, 0);
         evt->state2++;

      // fallthrough
      case 1:
         if (func_80037C28()) {
            DrawWindow(0x37, 128, 90, 88, 108, 400, 101, WBS_CROSSED, 5);
            DrawText(136, 101, 10, 2, 0, "#77");
            DisplayCustomWindowWithSetChoice(0x37, 2, 1, 30, 0, 0, gState.choices[0]);
            SlideWindowTo(0x37, 130, 110);
            evt->state2++;
         }
         break;

      case 2:
         gWindowActiveIdx = 0x37;
         evt->state2++;
         break;

      case 3:
         if (gWindowChoice.raw == 0x3705 || gWindowChoice.raw == 0x37ff) {
            gState.choices[0] = GetWindowChoice(0x37) - 1;
            if (GetWindowChoice(0x37) == 5) {
               gState.choices[0] = 0;
            }
            SlideWindowTo(0x37, 350, 110);
            CloseWindow(0x37);
            evt->state = 9;
            evt->state2 = 0;
            break;
         }
         if (gWindowChoice.raw == 0x3701) {
            gWindowActiveIdx = 0;
            evt->state = 5;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3702) {
            gWindowActiveIdx = 0;
            evt->state = 6;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3703) {
            gWindowActiveIdx = 0;
            evt->state = 7;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3704) {
            gWindowActiveIdx = 0;
            evt->state = 4;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 4:
   case 5:
   case 6:
   case 7:

      switch (evt->state2) {
      case 0:
         func_80044364(evt->state - 2, 0);
         evt->state2++;
         break;

      case 1:
         if (gState.msgBoxFinished) {
            evt->state = 8;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 8:

      switch (evt->state2) {
      case 0:
         func_80044364(6, 0);
         evt->state2++;
         break;

      case 1:
         if (func_80037C28()) {
            evt->state = 3;
            evt->state2 = 2;
         }
         break;
      }

      break;

   case 9:

      switch (evt->state2) {
      case 0:
         SlideWindowTo(0x36, 110, 110);
         func_80044364(17, 0);
         evt->state2++;
         break;

      case 1:
         if (func_80037C28()) {
            evt->state = 2;
            evt->state2 = 1;
         }
         break;
      }

      break;

   case 10:

      switch (evt->state2) {
      case 0:
         func_80044364(7, 0);
         evt->state2++;

      // fallthrough
      case 1:
         if (func_80037C28()) {
            evt->state2++;
         }
         break;

      case 2:
         DrawWindow(0x38, 412, 100, 96, 136, 420, 290, WBS_CROSSED, s_dojoPartyRows);
         ListParty(EVT.partyTop, s_dojoPartyRows, 0);
         DrawSjisText(420, 110, 19, 2, 0, gPartyListBuffer);
         DisplayCustomWindowWithSetChoice(0x38, 5, 1, 50, 1, 0, gState.choices[1]);
         SlideWindowTo(0x38, 8, 90);
         DrawWindow(0x3f, 320, 0, 72, 72, 130, 108, WBS_CROSSED, 0);
         DisplayCustomWindow(0x3f, 5, 1, 6, 0, 0);
         evt->state++;
         evt->state2 = 0;
         break;
      }

      break;

   case 11:

      switch (evt->state2) {
      case 0:
         gWindowActiveIdx = 0x38;
         EVT.partyIdx = 100;
         evt->state2++;
         break;

      case 1:
         i = EVT.partyIdx;
         EVT.partyIdx = gCurrentParty[EVT.partyTop + gHighlightedChoice - 1];
         partyIdx = EVT.partyIdx;
         if (i != partyIdx) {
            UpdateCompactUnitInfoWindow(&gUnits[partyIdx], &gUnits[partyIdx], 2);
            DisplayBasicWindow(2);
            Dojo_DisplayUnitPreview(partyIdx);
         }

         i = 0;
         if (gPartyMembers[partyIdx].advLevelFirst != 0) {
            if (gPartyMembers[partyIdx].advChosePathB) {
               i = 2;
            } else {
               i = 1;
            }
         }
         if (gPartyMembers[partyIdx].advLevelSecond != 0) {
            i += 2;
         }
         gState.unitListPortraitId = gUnitPortraitIds[gUnits[EVT.partyIdx].unitId] + i * 84;
         SlideWindowTo(0x3e, 220, 96);

         if (gWindowChoice.raw == 0x38ff) {
            gState.choices[1] = GetWindowChoice(0x38) - 1;
            SlideWindowTo(0x38, 350, 250);
            CloseWindow(0x38);
            SlideWindowTo(0x3e, 350, 108);
            CloseWindow(2);
            SlideWindowTo(3, 350, 120);
            CloseWindow(3);
            SlideWindowTo(0x3f, 350, 120);
            CloseWindow(0x3f);
            gWindowActiveIdx = 0;
            evt->state = 9;
            evt->state2 = 0;
         }
         if (gWindowChoice.s.windowId == 0x38 && gWindowChoice.s.choice != 0) {
            s_dojoVandalierUnlocked = 0;
            if (partyIdx == UNIT_ASH && gPartyMembers[partyIdx].advLevelSecond != 0 &&
                !gPartyMembers[partyIdx].advChosePathB &&
                gPartyMembers[partyIdx].weapon == ITEM_V_HEART && PlayerHasItem(ITEM_HEAVEN_P)) {
               s_dojoVandalierUnlocked = 1;
            }
            if (gPartyMembers[partyIdx].advLevelSecond != 0 && !s_dojoVandalierUnlocked) {
               gWindowActiveIdx = 0;
               evt->state = 12;
               evt->state2 = 0;
            } else if (gUnits[partyIdx].level < 10) {
               gWindowActiveIdx = 0;
               evt->state = 14;
               evt->state2 = 0;
            } else if (gUnits[partyIdx].level < 20 && gPartyMembers[partyIdx].advLevelFirst != 0) {
               gWindowActiveIdx = 0;
               evt->state = 14;
               evt->state2 = 0;
            } else {
               SlideWindowTo(0x38, -150, 250);
               gWindowActiveIdx = 0;
               evt->state = 15;
               evt->state2 = 0;
            }
         }
         break;
      }

      break;

   case 12:

      switch (evt->state2) {
      case 0:
         func_80044364(15, 0);
         evt->state2++;
         break;

      case 1:
         if (gState.msgBoxFinished) {
            evt->state = 13;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 13:

      switch (evt->state2) {
      case 0:
         func_80044364(13, 0);
         evt->state2++;
         break;

      case 1:
         if (func_80037C28()) {
            gWindowActiveIdx = 0x38;
            evt->state = 11;
            evt->state2 = 1;
         }
         break;
      }

      break;

   case 14:

      switch (evt->state2) {
      case 0:
         func_80044364(14, 0);
         evt->state2++;
         break;

      case 1:
         if (gState.msgBoxFinished) {
            evt->state = 13;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 15:

      switch (evt->state2) {
      case 0:
         i = 0;
         if (gPartyMembers[EVT.partyIdx].advLevelFirst != 0) {
            if (gPartyMembers[EVT.partyIdx].advChosePathB) {
               i = 2;
            } else {
               i = 1;
            }
            if (s_dojoVandalierUnlocked) {
               i += 1;
            }
         }
         DrawWindow(0x40, 256, 150, 120, 54, 358, 118, WBS_CROSSED, 2);
         DrawText(256, 160, 20, 2, 0,
                  gClassAdvancementNames[gPartyClassAdvancements[EVT.partyIdx - 1][i][0]]);
         DrawText(256, 178, 20, 2, 0,
                  gClassAdvancementNames[gPartyClassAdvancements[EVT.partyIdx - 1][i][1]]);
         DisplayBasicWindow(0x40);
         SlideWindowTo(0x40, 8, 118);
         evt->state2++;

      // fallthrough
      case 1:
         func_80044364(8, 0);
         evt->state2++;
         break;

      case 2:
         if (func_80037C28()) {
            gWindowActiveIdx = 0x40;
            evt->state2++;
         }
         break;

      case 3:
         if (gWindowChoice.raw == 0x40ff) {
            gWindowActiveIdx = 0;
            CloseWindow(0x40);
            SlideWindowTo(0x38, 8, 90);
            func_80044364(7, 0);
            evt->state2++;
         }
         if (gWindowChoice.raw == 0x4002) {
            if (gPartyMembers[EVT.partyIdx].advLevelFirst != 0 || EVT.partyIdx == UNIT_ASH) {
               break;
            }
            EVT.chosePathB = 1;
            gWindowActiveIdx = 0;
            evt->state2 += 2;
         }
         if (gWindowChoice.raw == 0x4001) {
            EVT.chosePathB = 0;
            gWindowActiveIdx = 0;
            evt->state2 += 2;
         }
         break;

      case 4:
         if (func_80037C28()) {
            evt->state = 11;
            evt->state2 = 1;
            gWindowActiveIdx = 0x38;
         }
         break;

      case 5:
         func_80044364(9, 0);
         evt->state2++;

      // fallthrough
      case 6:
         if (func_80037C28()) {
            evt->state2++;
         }
         break;

      case 7:
         // YES/NO
         DrawWindow(0x39, 320, 80, 64, 54, 28, 175, WBS_CROSSED, 2);
         DrawSjisText(324, 91, 10, 2, 0, "\x82\x78\x82\x64\x82\x72\n\x82\x6d\x82\x6e");
         DisplayCustomWindow(0x39, 7, 1, 5, 0, 0);
         gWindowActiveIdx = 0x39;
         evt->state2++;
         break;

      case 8:
         if (gWindowChoice.raw == 0x3902 || gWindowChoice.raw == 0x39ff) {
            CloseWindow(0x39);
            evt->state2 = 1;
         }
         if (gWindowChoice.raw == 0x3901) {
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(100));

            if (!s_dojoVandalierUnlocked) {
               if (EVT.chosePathB) {
                  gPartyMembers[EVT.partyIdx].advChosePathB = 1;
                  gPartyMembers[EVT.partyIdx].advLevelFirst = gUnits[EVT.partyIdx].level;
               } else if (gPartyMembers[EVT.partyIdx].advLevelFirst != 0) {
                  gPartyMembers[EVT.partyIdx].advLevelSecond = gUnits[EVT.partyIdx].level;
               } else {
                  gPartyMembers[EVT.partyIdx].advChosePathB = 0;
                  gPartyMembers[EVT.partyIdx].advLevelFirst = gUnits[EVT.partyIdx].level;
               }
            } else {
               gPartyMembers[UNIT_ASH].advChosePathB = 1;
            }

            i = 0;
            if (gPartyMembers[EVT.partyIdx].advChosePathB != 0) {
               j = 1;
            } else {
               j = 0;
            }
            if (gPartyMembers[EVT.partyIdx].advLevelSecond != 0) {
               i = 1;
               j = 0;
               if (gPartyMembers[EVT.partyIdx].advChosePathB) {
                  i = 2;
               }
            }

            gStringTable[0] = gStringTable[14 + gPartyClassAdvancements[EVT.partyIdx - 1][i][j]];
            CloseWindow(0x39);
            func_80044364(10, 0);
            evt->state2++;
            break;
         }

         break;

      case 9:
         if (gState.msgBoxFinished) {
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_8_8);
            EVT.timer = 70;
            FadeOutScreen(1, 7);
            evt->state2++;
         }
         break;

      case 10:
         if (--EVT.timer == 0) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(100));
            EVT.advancementTimer = 500;
            evt->state2++;
         }
         break;

      case 11:
         if (--EVT.advancementTimer == 0) {
            FadeInScreen(1, 7);
            PerformAudioCommand(AUDIO_CMD_FADE_IN_8_8);
            EVT.timer = 100;
            SyncPartyUnit(EVT.partyIdx);
            UpdateCompactUnitInfoWindow(&gUnits[EVT.partyIdx], &gUnits[EVT.partyIdx], 2);

            i = 0;
            if (gPartyMembers[EVT.partyIdx].advLevelFirst != 0) {
               if (gPartyMembers[EVT.partyIdx].advChosePathB) {
                  i = 2;
               } else {
                  i = 1;
               }
            }
            if (gPartyMembers[EVT.partyIdx].advLevelSecond != 0) {
               i += 2;
            }
            gState.unitListPortraitId = gUnitPortraitIds[gUnits[EVT.partyIdx].unitId] + i * 84;
            DisplayBasicWindow(2);
            Dojo_DisplayUnitPreview(EVT.partyIdx);
            evt->state2++;
         }
         break;

      case 12:
         if (--EVT.timer == 0) {
            evt->state++;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 16:

      switch (evt->state2) {
      case 0:
         func_80044364(11, 0);
         evt->state2++;
         break;

      case 1:
         if (func_80037C28()) {
            evt->state2++;
         }
         break;

      case 2:
         if (PressedCircleOrX()) {
            if (gPartyMembers[EVT.partyIdx].advLevelSecond == 0 &&
                gPartyMembers[EVT.partyIdx].advChosePathB && EVT.partyIdx != UNIT_ASH) {
               gState.depot[gPartyMembers[EVT.partyIdx].weapon]++;
               gPartyMembers[EVT.partyIdx].weapon = gPartyClassPathB1StartingWeapons[EVT.partyIdx];
               func_80044364(12, 0);
               SyncPartyUnit(EVT.partyIdx);
               evt->state2++;
            } else if (s_dojoVandalierUnlocked) {
               gState.depot[gPartyMembers[UNIT_ASH].helmet]++;
               gState.depot[gPartyMembers[UNIT_ASH].armor]++;
               gPartyMembers[UNIT_ASH].weapon = ITEM_V_HEART_2;
               gPartyMembers[UNIT_ASH].helmet = ITEM_V_HELM;
               gPartyMembers[UNIT_ASH].armor = ITEM_V_ARMOR;
               func_80044364(12, 0);
               SyncPartyUnit(UNIT_ASH);
               evt->state2++;
            } else {
               evt->state2 += 2;
            }
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            evt->state2++;
         }
         break;

      case 4:
         CloseWindow(0x40);
         SlideWindowTo(0x38, 8, 90);
         evt->state = 13;
         evt->state2 = 0;
         break;
      }

      break;

   case 50:

      switch (evt->state2) {
      case 0:
         func_80044364(18, 0);
         evt->state2++;
         break;

      case 1:
         if (MsgBox_IsFinished()) {
            gWindowChoiceHeight = 17;
            gWindowChoicesTopMargin = 10;
            DrawWindow(0x3b, 0, 90, 104, 54, 406, 110, WBS_CROSSED, 2);
            DrawText(12, 101, 10, 2, 0, "#78");
            DisplayCustomWindow(0x3b, 5, 1, 40, 0, 0);
            SlideWindowTo(0x3b, 110, 110);
            gWindowActiveIdx = 0x3b;
            evt->state2++;
         }
         break;

      case 2:
         if (gWindowChoice.raw == 0x3b02 || gWindowChoice.raw == 0x3bff) {
            CloseWindow(0x3b);
            evt->state = 100;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3b01) {
            func_80044364(19, 0);
            evt->state2++;
         }
         break;

      case 3:
         if (MsgBox_IsFinished()) {
            PerformAudioCommand(AUDIO_CMD_FADE_OUT_32_4);
            FadeOutScreen(2, 6);
            EVT.timer = 50;
            evt->state2++;
         }
         break;

      case 4:
         if (--EVT.timer == 0) {
            for (i = 0; i < ARRAY_COUNT(gState.currentPortraits); i++) {
               gState.portraitsToLoad[i] = PORTRAIT_NULL;
               gState.currentPortraits[i] = PORTRAIT_NULL;
            }
            gState.mapNum = Dojo_GetTrialNum() - 1;
            Evt_ResetFromIdx10();
            ClearUnits();
            gState.primary = STATE_27;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;
         }
         break;
      }

      return;

   case 99:
   case 100:

      switch (evt->state2) {
      case 0:
         CloseWindow(0x36);
         if (gState.primary == STATE_TRIAL_COMPLETE) {
            if (!PlayerHasItem(ITEM_NOVA_P_ + Dojo_GetTrialNum() - 1)) {
               func_80044364(21, 0);
            } else {
               if (gPartyMembers[UNIT_ASH].items[0] == (ITEM_NOVA_KEY + Dojo_GetTrialNum() - 1)) {
                  gPartyMembers[UNIT_ASH].items[0] = ITEM_NULL;
               } else if (gPartyMembers[UNIT_ASH].items[1] ==
                          (ITEM_NOVA_KEY + Dojo_GetTrialNum() - 1)) {
                  gPartyMembers[UNIT_ASH].items[1] = ITEM_NULL;
               }
               func_80044364(22, 0);
            }
            evt->state2++;
         } else {
            if (evt->state == 100) {
               func_80044364(20, 0);
               evt->state2++;
            } else {
               if (Dojo_GetTrialNum() != 0) {
                  evt->state = 50;
                  evt->state2 = 0;
               } else {
                  func_80044364(16, 0);
                  evt->state2++;
               }
            }
         }
         break;

      case 1:
         if (MsgBox_IsFinished()) {
            EVT.timer = 10;
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            func_80044134(0);
            FadeOutScreen(2, 6);
            EVT.timer = 50;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            for (i = 0; i < ARRAY_COUNT(gState.currentPortraits); i++) {
               gState.currentPortraits[i] = PORTRAIT_NULL;
               gState.portraitsToLoad[i] = PORTRAIT_NULL;
            }
            gState.primary = STATE_20;
            gState.secondary = 0;
            gState.state3 = 0;
            gState.state4 = 0;
         }
         break;
      }

      break;
   }

   gStringTable[32] = gStringTable[EVT.partyIdx];

   if (gWindowActiveIdx == 0x38) {
      i = 0;
      while (gCurrentParty[i] != 0) {
         i++;
      }
      j = i;
      if (i > s_dojoPartyRows) {
         i -= s_dojoPartyRows;
      } else {
         i = 0;
      }
      if ((gPadStateNewPresses & PAD_DOWN) && (i > EVT.partyTop) &&
          (gHighlightedChoice == s_dojoPartyRows)) {
         EVT.partyTop++;
         EVT.partyNeedsRedraw++;
      }
      if ((EVT.partyTop != 0) && (gPadStateNewPresses & PAD_UP) && (gHighlightedChoice == 1)) {
         EVT.partyTop--;
         EVT.partyNeedsRedraw++;
      }
   }

   switch (evt->mem) {
   case 0:
      if (EVT.partyNeedsRedraw) {
         EVT.partyNeedsRedraw = 0;
         DrawWindow(0x38, 412, 100, 96, 136, 84, 250, WBS_CROSSED, s_dojoPartyRows);
         ListParty(EVT.partyTop, s_dojoPartyRows, 1);
         DrawSjisText(420, 110, 19, 2, 0, gPartyListBuffer);
         evt->mem++;
      }
      break;

   case 1:
      DrawWindow(0x38, 412, 100, 96, 136, 84, 250, WBS_CROSSED, s_dojoPartyRows);
      ListParty(EVT.partyTop, s_dojoPartyRows, 0);
      DrawSjisText(420, 110, 19, 2, 0, gPartyListBuffer);
      evt->mem = 0;
      break;
   }
}

s32 Dojo_GetTrialNum(void) {
   s32 item;

   item = gPartyMembers[UNIT_ASH].items[0];
   if (item >= ITEM_NOVA_KEY && item <= ITEM_HEAVEN_K) {
      return item - ITEM_NOVA_KEY + 1;
   }

   item = gPartyMembers[UNIT_ASH].items[1];
   if (item >= ITEM_NOVA_KEY && item <= ITEM_HEAVEN_K) {
      return item - ITEM_NOVA_KEY + 1;
   }

   return 0;
}
