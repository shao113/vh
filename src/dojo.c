#include "common.h"
#include "graphics.h"
#include "units.h"
#include "state.h"
#include "evt.h"
#include "cd_files.h"

/*typedef struct ClassAdvancements {
   u8 pathA1; // e.g. Bowman
   u8 pathB1; // e.g. Hawknight
   u8 pathA2; // e.g. Sniper
   u8 field3_0x3;
   u8 pathB2; // e.g. Sky Lord
   u8 field5_0x5;
} ClassAdvancements;*/

u8 gClassAdvancementNames[18][17] = {
    "",          "Champion", "Paragon", "Swordsman", "Duelist",    "Guardsman",
    "Dragoon",   "Bowman",   "Sniper",  "Hawknight", "Sky Lord",   "Sorcerer",
    "Enchanter", "Monk",     "Ninja",   "Bishop",    "Archbishop", "Vandalier",
};

u8 gPartyClassAdvancements[12][6] = {
    {1, 0, 2, 0, 17, 0},    {3, 5, 4, 0, 6, 0},     {7, 9, 8, 0, 10, 0},    {11, 13, 12, 0, 14, 0},
    {15, 13, 16, 0, 14, 0}, {7, 9, 8, 0, 10, 0},    {3, 5, 4, 0, 6, 0},     {3, 5, 4, 0, 6, 0},
    {7, 9, 8, 0, 10, 0},    {15, 13, 16, 0, 14, 0}, {11, 13, 12, 0, 14, 0}, {7, 9, 8, 0, 10, 0},
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

      if (gState.primary == STATE_29) {
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
