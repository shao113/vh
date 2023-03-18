#include "common.h"
#include "state.h"
#include "evt.h"
#include "units.h"
#include "window.h"
#include "graphics.h"
#include "glyphs.h"
#include "field.h"
#include "audio.h"
#include "battle.h"

s32 FUN_8001d384(u8 *, u8 *);
s32 FUN_8001d3fc(u8 *, u8 *);
void Evtf574_DisplayIcon(EvtData *);
void Evtf002_MenuChoice(EvtData *);
void Evtf004_005_408_Window(EvtData *);
void DrawGlyphStrip(u8 *, s16, s16, u8);
void ClearGlyphStripBottom(u8 *, s16, s16);
u16 DrawEmbossedSjisChar(u16, s32, s32, s32, s32);
void Evtf422_LowerMsgBoxTail(EvtData *);
void Evtf421_UpperMsgBoxTail(EvtData *);
void Evtf573_BattleItemsList(EvtData *);
void Evtf031_BattleSpellsList(EvtData *);

s32 WindowIsOffScreen(EvtData *evt) {
   if (evt->d.evtf004.x < 0)
      return 1;
   if (evt->d.evtf004.x > SCREEN_WIDTH)
      return 1;
   if (evt->d.evtf004.y < 0)
      return 1;
   if (evt->d.evtf004.y > SCREEN_HEIGHT)
      return 1;
   return 0;
}

void DrawSmallEquipmentWindow(u8 partyIdx) {
   s32 i;
   if ((partyIdx != 0) && (partyIdx < PARTY_CT)) {
      for (i = 2; i < 12; i++) {
         gGlyphStrip_02[i] = GLYPH_BG;
         gGlyphStrip_03[i] = GLYPH_BG;
         gGlyphStrip_04[i] = GLYPH_BG;
         gGlyphStrip_05[i] = GLYPH_BG;
         gGlyphStrip_06[i] = GLYPH_BG;
         gGlyphStrip_07[i] = GLYPH_BG;
         gGlyphStrip_08[i] = GLYPH_BG;
         gGlyphStrip_09[i] = GLYPH_BG;
         gGlyphStrip_0A[i] = GLYPH_BG;
         gGlyphStrip_0B[i] = GLYPH_BG;
      }
      gGlyphStrip_03[2] = GLYPH_WEAPON;
      gGlyphStrip_03[3] = GLYPH_CHAR_COLON;
      gGlyphStrip_05[2] = GLYPH_HELMET;
      gGlyphStrip_05[3] = GLYPH_CHAR_COLON;
      gGlyphStrip_07[2] = GLYPH_ARMOR;
      gGlyphStrip_07[3] = GLYPH_CHAR_COLON;
      StringToGlyphs(&gGlyphStrip_03[4], gItemNames[gPartyMembers[partyIdx].weapon]);
      StringToGlyphs(&gGlyphStrip_05[4], gItemNames[gPartyMembers[partyIdx].helmet]);
      StringToGlyphs(&gGlyphStrip_07[4], gItemNames[gPartyMembers[partyIdx].armor]);
      StringToGlyphs(&gGlyphStrip_09[4], gItemNames[gPartyMembers[partyIdx].items[0]]);
      StringToGlyphs(&gGlyphStrip_0B[4], gItemNames[gPartyMembers[partyIdx].items[1]]);
      DrawGlyphStripGroup(gGlyphStripGroups[1], GFX_WINDOW_TBD_658);
   }
}

void DrawWindow(s16 windowId, s16 x, s16 y, s16 width, s16 height, s16 dispX, s16 dispY,
                u8 borderStyle, u8 choices) {
   s32 usingMultipleTPages;
   s32 type; // TBD
   s16 yPadding;
   s32 maxColumnsPerTPage;
   s32 i;

   usingMultipleTPages = 0;
   gWindowChoicesCount = choices;
   type = 0;

   if (x >= 1024) {
      x -= 1024;
      if (x >= 256) {
         x -= 256;
         type = 3;
      } else {
         type = 4;
      }
   }
   if (x >= 256) {
      x -= 256;
      type = 1;
   }

   width = width >> 3;
   yPadding = height % 9;
   height = height / 9;

   if (borderStyle == WBS_DRAGON) {
      maxColumnsPerTPage = 32;
      if (width >= 32) {
         usingMultipleTPages++;
      }
   } else {
      maxColumnsPerTPage = 31;
      if (width >= 31) {
         usingMultipleTPages++;
      }
   }

   if (!usingMultipleTPages) {
      if (borderStyle != WBS_DRAGON) {
         if (borderStyle == WBS_CROSSED) {
            // WBS_CROSSED:

            gGlyphStrip_C8[1] = GLYPH_CROSSED_BORDER_TOP_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_C8[i] = GLYPH_CROSSED_BORDER_TOP;
            }
            gGlyphStrip_C8[i] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
            gGlyphStrip_C8[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_C9[1] = GLYPH_CROSSED_BORDER_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_C9[i] = GLYPH_BG;
            }
            gGlyphStrip_C9[i] = GLYPH_CROSSED_BORDER_RIGHT;
            gGlyphStrip_C9[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CA[1] = GLYPH_CROSSED_BORDER_BOTTOM_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_CA[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }
            gGlyphStrip_CA[i] = GLYPH_CROSSED_BORDER_BOTTOM_RIGHT;
            gGlyphStrip_CA[i + 1] = GLYPH_TERMINATOR;
         } else {
            // WBS_ROUNDED:

            gGlyphStrip_C8[1] = GLYPH_ROUNDED_BORDER_TOP_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_C8[i] = GLYPH_ROUNDED_BORDER_TOP;
            }
            gGlyphStrip_C8[i] = GLYPH_ROUNDED_BORDER_TOP_RIGHT;
            gGlyphStrip_C8[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_C9[1] = GLYPH_ROUNDED_BORDER_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_C9[i] = GLYPH_BG;
            }
            gGlyphStrip_C9[i] = GLYPH_ROUNDED_BORDER_RIGHT;
            gGlyphStrip_C9[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CA[1] = GLYPH_ROUNDED_BORDER_BOTTOM_LEFT;
            for (i = 2; i < width; i++) {
               gGlyphStrip_CA[i] = GLYPH_ROUNDED_BORDER_BOTTOM;
            }
            gGlyphStrip_CA[i] = GLYPH_ROUNDED_BORDER_BOTTOM_RIGHT;
            gGlyphStrip_CA[i + 1] = GLYPH_TERMINATOR;
         }

         gGlyphStripGroup_800f0bd4[0] = 0;
         gGlyphStripGroup_800f0bd4[1] = 0xc8;
         for (i = 2; i < height; i++) {
            gGlyphStripGroup_800f0bd4[i] = 0xc9;
         }
         gGlyphStripGroup_800f0bd4[i++] = 0xca;
         gGlyphStripGroup_800f0bd4[i] = 0;
      } else {
         // WBS_DRAGON:

         gGlyphStrip_C8[1] = GLYPH_LDRAGON_TOP_1;
         gGlyphStrip_C8[2] = GLYPH_LDRAGON_TOP_2;
         gGlyphStrip_C8[3] = GLYPH_LDRAGON_TOP_3;
         for (i = 4; i < width + 1; i++) {
            gGlyphStrip_C8[i] = GLYPH_BLANK;
         }
         gGlyphStrip_C8[i] = GLYPH_TERMINATOR;

         gGlyphStrip_C9[1] = GLYPH_LDRAGON_MID_1;
         gGlyphStrip_C9[2] = GLYPH_LDRAGON_MID_2;
         gGlyphStrip_C9[3] = GLYPH_LDRAGON_MID_3;
         for (i = 4; i < width - 1; i++) {
            gGlyphStrip_C9[i] = GLYPH_CROSSED_BORDER_TOP;
         }
         gGlyphStrip_C9[i] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
         gGlyphStrip_C9[i + 1] = GLYPH_BLANK;
         gGlyphStrip_C9[i + 2] = GLYPH_TERMINATOR;

         gGlyphStrip_CA[1] = GLYPH_BLANK;
         gGlyphStrip_CA[2] = GLYPH_LDRAGON_TAIL;
         for (i = 3; i < width - 1; i++) {
            gGlyphStrip_CA[i] = GLYPH_BG;
         }
         gGlyphStrip_CA[i] = GLYPH_CROSSED_BORDER_RIGHT;
         gGlyphStrip_CA[i + 1] = GLYPH_BLANK;
         gGlyphStrip_CA[i + 2] = GLYPH_TERMINATOR;
         if (height == 6) {
            gGlyphStrip_CA[i] = GLYPH_RDRAGON_TAIL_TIP;
         }

         gGlyphStrip_CB[1] = GLYPH_BLANK;
         gGlyphStrip_CB[2] = GLYPH_LDRAGON_TAIL_TIP;
         for (i = 3; i < width - 1; i++) {
            gGlyphStrip_CB[i] = GLYPH_BG;
         }
         gGlyphStrip_CB[i] = GLYPH_CROSSED_BORDER_RIGHT;
         gGlyphStrip_CB[i + 1] = GLYPH_BLANK;
         gGlyphStrip_CB[i + 2] = GLYPH_TERMINATOR;
         if (height == 6) {
            gGlyphStrip_CB[i] = GLYPH_RDRAGON_TAIL;
         }
         if (height == 7) {
            gGlyphStrip_CB[i] = GLYPH_RDRAGON_TAIL_TIP;
         }

         gGlyphStrip_CC[1] = GLYPH_BLANK;
         gGlyphStrip_CC[2] = GLYPH_CROSSED_BORDER_LEFT;
         for (i = 3; i < width - 1; i++) {
            gGlyphStrip_CC[i] = GLYPH_BG;
         }
         gGlyphStrip_CC[i] = GLYPH_CROSSED_BORDER_RIGHT;
         gGlyphStrip_CC[i + 1] = GLYPH_BLANK;
         gGlyphStrip_CC[i + 2] = GLYPH_TERMINATOR;

         gGlyphStrip_CD[1] = GLYPH_BLANK;
         gGlyphStrip_CD[2] = GLYPH_CROSSED_BORDER_LEFT;
         for (i = 3; i < width - 1; i++) {
            gGlyphStrip_CD[i] = GLYPH_BG;
         }
         gGlyphStrip_CD[i] = GLYPH_RDRAGON_TAIL_TIP;
         gGlyphStrip_CD[i + 1] = GLYPH_BLANK;
         gGlyphStrip_CD[i + 2] = GLYPH_TERMINATOR;

         gGlyphStrip_CE[1] = GLYPH_BLANK;
         gGlyphStrip_CE[2] = GLYPH_CROSSED_BORDER_LEFT;
         for (i = 3; i < width - 1; i++) {
            gGlyphStrip_CE[i] = GLYPH_BG;
         }
         gGlyphStrip_CE[i] = GLYPH_RDRAGON_TAIL;
         gGlyphStrip_CE[i + 1] = GLYPH_BLANK;
         gGlyphStrip_CE[i + 2] = GLYPH_TERMINATOR;

         gGlyphStrip_CF[1] = GLYPH_BLANK;
         gGlyphStrip_CF[2] = GLYPH_CROSSED_BORDER_BOTTOM_LEFT;
         for (i = 3; i < width - 2; i++) {
            gGlyphStrip_CF[i] = GLYPH_CROSSED_BORDER_BOTTOM;
         }
         gGlyphStrip_CF[i] = GLYPH_RDRAGON_MID_1;
         gGlyphStrip_CF[i + 1] = GLYPH_RDRAGON_MID_2;
         gGlyphStrip_CF[i + 2] = GLYPH_RDRAGON_MID_3;
         gGlyphStrip_CF[i + 3] = GLYPH_TERMINATOR;

         for (i = 0; i < width - 2; i++) {
            gGlyphStrip_D0[i] = GLYPH_BLANK;
         }
         gGlyphStrip_D0[i] = GLYPH_RDRAGON_BOTTOM_1;
         gGlyphStrip_D0[i + 1] = GLYPH_RDRAGON_BOTTOM_2;
         gGlyphStrip_D0[i + 2] = GLYPH_RDRAGON_BOTTOM_3;
         gGlyphStrip_D0[i + 3] = GLYPH_TERMINATOR;

         i = 0;
         gGlyphStripGroup_800f0bd4[i++] = 0;
         gGlyphStripGroup_800f0bd4[i++] = 0xc8;
         gGlyphStripGroup_800f0bd4[i++] = 0xc9;
         gGlyphStripGroup_800f0bd4[i++] = 0xca;
         gGlyphStripGroup_800f0bd4[i++] = 0xcb;
         if (height > 8) {
            for (i = 5; i < height - 3; i++) {
               gGlyphStripGroup_800f0bd4[i] = 0xcc;
            }
         }
         if (height > 7) {
            gGlyphStripGroup_800f0bd4[i++] = 0xcd;
         }
         if (height > 6) {
            gGlyphStripGroup_800f0bd4[i++] = 0xce;
         }
         gGlyphStripGroup_800f0bd4[i++] = 0xcf;
         gGlyphStripGroup_800f0bd4[i++] = 0xd0;
         gGlyphStripGroup_800f0bd4[i] = 0;
      }

      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].x = x;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].y = y;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].w = width * 8;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].h = yPadding + height * 9;
      gGfxSubTextures[GFX_WINDOW_TBD_707].x = x;
      gGfxSubTextures[GFX_WINDOW_TBD_707].y = y;
   } else {
      // usingMultipleTPages:
      width++;
      width = width - maxColumnsPerTPage;

      if (borderStyle != WBS_DRAGON) {
         if (borderStyle == WBS_CROSSED) {
            // WBS_CROSSED:

            for (i = 2; i < maxColumnsPerTPage; i++) {
               gGlyphStrip_C8[i] = GLYPH_CROSSED_BORDER_TOP;
               gGlyphStrip_C9[i] = GLYPH_BG;
               gGlyphStrip_CA[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }

            gGlyphStrip_C8[1] = GLYPH_CROSSED_BORDER_TOP_LEFT;
            gGlyphStrip_C8[i] = GLYPH_TERMINATOR;

            gGlyphStrip_C9[1] = GLYPH_CROSSED_BORDER_LEFT;
            gGlyphStrip_C9[i] = GLYPH_TERMINATOR;

            gGlyphStrip_CA[1] = GLYPH_CROSSED_BORDER_BOTTOM_LEFT;
            gGlyphStrip_CA[i] = GLYPH_TERMINATOR;

            if (width > 1) {
               for (i = 1; i < width; i++) {
                  gGlyphStrip_CB[i] = GLYPH_CROSSED_BORDER_TOP;
                  gGlyphStrip_CC[i] = GLYPH_BG;
                  gGlyphStrip_CD[i] = GLYPH_CROSSED_BORDER_BOTTOM;
               }
            }

            gGlyphStrip_CB[i] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
            gGlyphStrip_CB[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CC[i] = GLYPH_CROSSED_BORDER_RIGHT;
            gGlyphStrip_CC[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CD[i] = GLYPH_CROSSED_BORDER_BOTTOM_RIGHT;
            gGlyphStrip_CD[i + 1] = GLYPH_TERMINATOR;
         } else {
            // WBS_ROUNDED:

            for (i = 2; i < maxColumnsPerTPage; i++) {
               gGlyphStrip_C8[i] = GLYPH_ROUNDED_BORDER_TOP;
               gGlyphStrip_C9[i] = GLYPH_BG;
               gGlyphStrip_CA[i] = GLYPH_ROUNDED_BORDER_BOTTOM;
            }

            gGlyphStrip_C8[1] = GLYPH_ROUNDED_BORDER_TOP_LEFT;
            gGlyphStrip_C8[i] = GLYPH_TERMINATOR;

            gGlyphStrip_C9[1] = GLYPH_ROUNDED_BORDER_LEFT;
            gGlyphStrip_C9[i] = GLYPH_TERMINATOR;

            gGlyphStrip_CA[1] = GLYPH_ROUNDED_BORDER_BOTTOM_LEFT;
            gGlyphStrip_CA[i] = GLYPH_TERMINATOR;

            if (width > 1) {
               for (i = 1; i < width; i++) {
                  gGlyphStrip_CB[i] = GLYPH_ROUNDED_BORDER_TOP;
                  gGlyphStrip_CC[i] = GLYPH_BG;
                  gGlyphStrip_CD[i] = GLYPH_ROUNDED_BORDER_BOTTOM;
               }
            }

            gGlyphStrip_CB[i] = GLYPH_ROUNDED_BORDER_TOP_RIGHT;
            gGlyphStrip_CB[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CC[i] = GLYPH_ROUNDED_BORDER_RIGHT;
            gGlyphStrip_CC[i + 1] = GLYPH_TERMINATOR;

            gGlyphStrip_CD[i] = GLYPH_ROUNDED_BORDER_BOTTOM_RIGHT;
            gGlyphStrip_CD[i + 1] = GLYPH_TERMINATOR;
         }

         gGlyphStripGroup_800f0bd4[0] = 0;
         gGlyphStripGroup_800f0bd4[1] = 0xc8;
         for (i = 2; i < height; i++) {
            gGlyphStripGroup_800f0bd4[i] = 0xc9;
         }
         gGlyphStripGroup_800f0bd4[i++] = 0xca;
         gGlyphStripGroup_800f0bd4[i] = 0;

         gGlyphStripGroup_800f0bf0[0] = 0;
         gGlyphStripGroup_800f0bf0[1] = 0xcb;
         for (i = 2; i < height; i++) {
            gGlyphStripGroup_800f0bf0[i] = 0xcc;
         }
         gGlyphStripGroup_800f0bf0[i++] = 0xcd;
         gGlyphStripGroup_800f0bf0[i++] = 0;

      } else {
         // WBS_DRAGON:

         gGlyphStrip_C8[1] = GLYPH_LDRAGON_TOP_1;
         gGlyphStrip_C8[2] = GLYPH_LDRAGON_TOP_2;
         gGlyphStrip_C8[3] = GLYPH_LDRAGON_TOP_3;
         for (i = 4; i < 32; i++) {
            gGlyphStrip_C8[i] = GLYPH_BLANK;
         }
         gGlyphStrip_C8[i] = GLYPH_TERMINATOR;

         gGlyphStrip_C9[1] = GLYPH_LDRAGON_MID_1;
         gGlyphStrip_C9[2] = GLYPH_LDRAGON_MID_2;
         gGlyphStrip_C9[3] = GLYPH_LDRAGON_MID_3;
         if (width == 1) {
            for (i = 4; i < 31; i++) {
               gGlyphStrip_C9[i] = GLYPH_CROSSED_BORDER_TOP;
            }
            gGlyphStrip_C9[i++] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
         } else {
            for (i = 4; i < 32; i++) {
               gGlyphStrip_C9[i] = GLYPH_CROSSED_BORDER_TOP;
            }
         }
         gGlyphStrip_C9[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CA[1] = GLYPH_BLANK;
         gGlyphStrip_CA[2] = GLYPH_LDRAGON_TAIL;
         if (width == 1) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CA[i] = GLYPH_BG;
            }
            gGlyphStrip_CA[i++] = GLYPH_CROSSED_BORDER_RIGHT;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CA[i] = GLYPH_BG;
            }
         }
         gGlyphStrip_CA[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CB[1] = GLYPH_BLANK;
         gGlyphStrip_CB[2] = GLYPH_LDRAGON_TAIL_TIP;
         if (width == 1) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CB[i] = GLYPH_BG;
            }
            gGlyphStrip_CB[i++] = GLYPH_CROSSED_BORDER_RIGHT;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CB[i] = GLYPH_BG;
            }
         }
         gGlyphStrip_CB[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CC[1] = GLYPH_BLANK;
         gGlyphStrip_CC[2] = GLYPH_CROSSED_BORDER_LEFT;
         if (width == 1) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CC[i] = GLYPH_BG;
            }
            gGlyphStrip_CC[i++] = GLYPH_CROSSED_BORDER_RIGHT;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CC[i] = GLYPH_BG;
            }
         }
         gGlyphStrip_CC[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CD[1] = GLYPH_BLANK;
         gGlyphStrip_CD[2] = GLYPH_CROSSED_BORDER_LEFT;
         if (width == 1) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CD[i] = GLYPH_BG;
            }
            gGlyphStrip_CD[i++] = GLYPH_RDRAGON_TAIL_TIP;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CD[i] = GLYPH_BG;
            }
         }
         gGlyphStrip_CD[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CE[1] = GLYPH_BLANK;
         gGlyphStrip_CE[2] = GLYPH_CROSSED_BORDER_LEFT;
         if (width == 1) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CE[i] = GLYPH_BG;
            }
            gGlyphStrip_CE[i++] = GLYPH_RDRAGON_TAIL;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CE[i] = GLYPH_BG;
            }
         }
         gGlyphStrip_CE[i] = GLYPH_TERMINATOR;

         gGlyphStrip_CF[1] = GLYPH_BLANK;
         gGlyphStrip_CF[2] = GLYPH_CROSSED_BORDER_BOTTOM_LEFT;
         if (width == 1) {
            for (i = 3; i < 30; i++) {
               gGlyphStrip_CF[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }
            gGlyphStrip_CF[i++] = GLYPH_RDRAGON_MID_1;
            gGlyphStrip_CF[i++] = GLYPH_RDRAGON_MID_2;
         } else if (width == 2) {
            for (i = 3; i < 31; i++) {
               gGlyphStrip_CF[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }
            gGlyphStrip_CF[i++] = GLYPH_RDRAGON_MID_1;
         } else {
            for (i = 3; i < 32; i++) {
               gGlyphStrip_CF[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }
         }
         gGlyphStrip_CF[i] = GLYPH_TERMINATOR;

         if (width == 1) {
            for (i = 1; i < 30; i++) {
               gGlyphStrip_D0[i] = GLYPH_BLANK;
            }
            gGlyphStrip_D0[i++] = GLYPH_RDRAGON_BOTTOM_1;
            gGlyphStrip_D0[i++] = GLYPH_RDRAGON_BOTTOM_2;
         } else if (width == 2) {
            for (i = 1; i < 31; i++) {
               gGlyphStrip_D0[i] = GLYPH_BLANK;
            }
            gGlyphStrip_D0[i++] = GLYPH_RDRAGON_BOTTOM_1;
         } else {
            for (i = 1; i < 32; i++) {
               gGlyphStrip_D0[i] = GLYPH_BLANK;
            }
         }
         gGlyphStrip_D0[i] = GLYPH_TERMINATOR;

         for (i = 1; i < width + 1; i++) {
            gGlyphStrip_D1[i] = GLYPH_BLANK;
         }
         gGlyphStrip_D1[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D2[i++] = GLYPH_BLANK;
         } else if (width == 2) {
            gGlyphStrip_D2[i++] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
            gGlyphStrip_D2[i++] = GLYPH_BLANK;
         } else {
            for (i = 1; i < width - 1; i++) {
               gGlyphStrip_D2[i] = GLYPH_CROSSED_BORDER_TOP;
            }
            gGlyphStrip_D2[i++] = GLYPH_CROSSED_BORDER_TOP_RIGHT;
            gGlyphStrip_D2[i++] = GLYPH_BLANK;
         }
         gGlyphStrip_D2[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D3[i++] = GLYPH_BLANK;
         } else if (width == 2) {
            gGlyphStrip_D3[i++] = GLYPH_CROSSED_BORDER_RIGHT;
            gGlyphStrip_D3[i++] = GLYPH_BLANK;
         } else {
            for (i = 1; i < width - 1; i++) {
               gGlyphStrip_D3[i] = GLYPH_BG;
            }
            gGlyphStrip_D3[i++] = GLYPH_CROSSED_BORDER_RIGHT;
            gGlyphStrip_D3[i++] = GLYPH_BLANK;
         }
         gGlyphStrip_D3[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D4[i++] = GLYPH_BLANK;
         } else if (width == 2) {
            gGlyphStrip_D4[i++] = GLYPH_RDRAGON_TAIL_TIP;
            gGlyphStrip_D4[i++] = GLYPH_BLANK;
         } else {
            for (i = 1; i < width - 1; i++) {
               gGlyphStrip_D4[i] = GLYPH_BG;
            }
            gGlyphStrip_D4[i++] = GLYPH_RDRAGON_TAIL_TIP;
            gGlyphStrip_D4[i++] = GLYPH_BLANK;
         }
         gGlyphStrip_D4[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D5[i++] = GLYPH_BLANK;
         } else if (width == 2) {
            gGlyphStrip_D5[i++] = GLYPH_RDRAGON_TAIL;
            gGlyphStrip_D5[i++] = GLYPH_BLANK;
         } else {
            for (i = 1; i < width - 1; i++) {
               gGlyphStrip_D5[i] = GLYPH_BG;
            }
            gGlyphStrip_D5[i++] = GLYPH_RDRAGON_TAIL;
            gGlyphStrip_D5[i++] = GLYPH_BLANK;
         }
         gGlyphStrip_D5[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_3;
         } else if (width == 2) {
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_2;
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_3;
         } else if (width == 3) {
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_1;
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_2;
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_3;
         } else {
            for (i = 1; i < width - 2; i++) {
               gGlyphStrip_D6[i] = GLYPH_CROSSED_BORDER_BOTTOM;
            }
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_1;
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_2;
            gGlyphStrip_D6[i++] = GLYPH_RDRAGON_MID_3;
         }
         gGlyphStrip_D6[i] = GLYPH_TERMINATOR;

         i = 1;
         if (width == 1) {
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_3;
         } else if (width == 2) {
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_2;
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_3;
         } else if (width == 3) {
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_1;
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_2;
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_3;
         } else {
            for (i = 1; i < width - 2; i++) {
               gGlyphStrip_D7[i] = GLYPH_BLANK;
            }
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_1;
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_2;
            gGlyphStrip_D7[i++] = GLYPH_RDRAGON_BOTTOM_3;
         }
         gGlyphStrip_D7[i] = GLYPH_TERMINATOR;

         gGlyphStripGroup_800f0bd4[0] = 0;
         gGlyphStripGroup_800f0bd4[1] = 0xc8;
         gGlyphStripGroup_800f0bd4[2] = 0xc9;
         gGlyphStripGroup_800f0bd4[3] = 0xca;
         gGlyphStripGroup_800f0bd4[4] = 0xcb;

         for (i = 5; i < height - 3; i++) {
            gGlyphStripGroup_800f0bd4[i] = 0xcc;
         }

         gGlyphStripGroup_800f0bd4[i] = 0xcd;
         gGlyphStripGroup_800f0bd4[i + 1] = 0xce;
         gGlyphStripGroup_800f0bd4[i + 2] = 0xcf;
         gGlyphStripGroup_800f0bd4[i + 3] = 0xd0;
         gGlyphStripGroup_800f0bd4[i + 4] = 0;

         gGlyphStripGroup_800f0bf0[0] = 0;
         gGlyphStripGroup_800f0bf0[1] = 0xd1;
         gGlyphStripGroup_800f0bf0[2] = 0xd2;

         for (i = 3; i < height - 3; i++) {
            gGlyphStripGroup_800f0bf0[i] = 0xd3;
         }

         gGlyphStripGroup_800f0bf0[i] = 0xd4;
         gGlyphStripGroup_800f0bf0[i + 1] = 0xd5;
         gGlyphStripGroup_800f0bf0[i + 2] = 0xd6;
         gGlyphStripGroup_800f0bf0[i + 3] = 0xd7;
         gGlyphStripGroup_800f0bf0[i + 4] = 0;
      }

      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].x = 0;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].y = y;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].w = (maxColumnsPerTPage - 1) * 8;
      gGfxSubTextures[GFX_WINDOW_TBD_657 + windowId].h = yPadding + height * 9;

      gGfxSubTextures[GFX_WINDOW_TBD_658 + windowId].x = 0;
      gGfxSubTextures[GFX_WINDOW_TBD_658 + windowId].y = y;
      gGfxSubTextures[GFX_WINDOW_TBD_658 + windowId].w = width * 8;
      gGfxSubTextures[GFX_WINDOW_TBD_658 + windowId].h = yPadding + height * 9;

      gGfxSubTextures[GFX_WINDOW_TBD_707].x = 0;
      gGfxSubTextures[GFX_WINDOW_TBD_707].y = y;
      gGfxSubTextures[GFX_WINDOW_TBD_708].x = 0;
      gGfxSubTextures[GFX_WINDOW_TBD_708].y = y;
   }

   if (usingMultipleTPages) {
      if (type == 0) {
         DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
         DrawGlyphStripGroup(gGlyphStripGroups[51], GFX_WINDOW_TBD_708);

         gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[8];
         gGfxTPageCells[GFX_WINDOW_TBD_658 + windowId] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_658 + windowId] = gTPageIds[9];
      } else {
         gGfxTPageCells[GFX_WINDOW_TBD_707] = 14;
         gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[14];
         gGfxTPageCells[GFX_WINDOW_TBD_708] = 15;
         gGfxTPageIds[GFX_WINDOW_TBD_708] = gTPageIds[15];

         DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
         DrawGlyphStripGroup(gGlyphStripGroups[51], GFX_WINDOW_TBD_708);

         gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[8];
         gGfxTPageCells[GFX_WINDOW_TBD_708] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_708] = gTPageIds[9];

         gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[8];
         gGfxTPageCells[GFX_WINDOW_TBD_658 + windowId] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_658 + windowId] = gTPageIds[9];
      }
   } else {
      // !usingMultipleTPages:
      if (type == 0) {
         DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
         gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[8];
      } else {
         if (type == 1) {
            gGfxTPageCells[GFX_WINDOW_TBD_707] = 9;
            gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[9];
            DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
            gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
            gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[8];
            gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 9;
            gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[9];
         } else {
            if (type == 2) {
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 14;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[14];
               DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[8];
               gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 14;
               gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[14];
            } else {
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 15;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[15];
               DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[8];
               gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 15;
               gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[15];
            }
         }
      }
   }
   if (dispX != 0) {
      gWindowDisplayX[windowId] = dispX;
   }
   if (dispY != 0) {
      gWindowDisplayY[windowId] = dispY;
   }
   if (usingMultipleTPages) {
      gWindowDisplayX[windowId + 1] = dispX + maxColumnsPerTPage * 8 - 8;
      gWindowDisplayY[windowId + 1] = dispY;
   }
}

s32 StringToGlyphs(u8 *dest, u8 *src) {
   s32 ct = 0;

   if (*src == '\0') {
      return -1;
   }
   while (*src != '\0') {
      *dest++ = GetGlyphIdxForAsciiChar(*src++);
      ct++;
   }
   return ct;
}

s32 FUN_8001d384(u8 *dest, u8 *src) {
   // TBD: Unused? Maybe left-over kana glyph stuff?
   s32 ct = 0;
   s32 inByte, outByte;

   if (*src == 0xff) {
      return -1;
   }
   while (*src != 0xff) {
      inByte = *src++;
      outByte = inByte;
      if (inByte >= 0x9b) {
         outByte = inByte - 0x89;
         if (inByte >= 0xaa && inByte <= 0xae) {
            outByte = inByte - 0x84;
         }
      }
      if (inByte == 0x9a) {
         outByte = 0xf;
      }
      *dest++ = outByte;
      ct++;
   }
   return ct;
}

s32 FUN_8001d3fc(u8 *dest, u8 *src) {
   // TBD: Unused? Maybe left-over kana glyph stuff?
   s32 ct = 0;
   s32 inByte, outByte;

   if (*src == 0xff) {
      return -1;
   }
   while (*src != 0xff) {
      inByte = *src++;
      outByte = 1;
      if (inByte >= 0x9a) {
         outByte = 0x9a;
         if (inByte >= 0xaa && inByte <= 0xae) {
            outByte = 0x9b;
         }
      }
      *dest++ = outByte;
      ct++;
   }
   return ct;
}

void UpdateSkillStatusWindow(UnitStatus *unit) {
   s32 i;
   s32 a, def;
   EvtData *icon;

   for (a = 2; a < 26; a++) {
      gGlyphStrip_65[a] = GLYPH_BG;
      gGlyphStrip_66[a] = GLYPH_BG;
      gGlyphStrip_67[a] = GLYPH_BG;
      gGlyphStrip_68[a] = GLYPH_BG;
   }
   for (a = 16; a < 28; a++) {
      gGlyphStrip_5F[a] = GLYPH_BG;
      gGlyphStrip_61[a] = GLYPH_BG;
      gGlyphStrip_63[a] = GLYPH_BG;
   }

   // Plot the 2x2 glyphs that make up the class icon
   gGlyphStrip_65[9] = gClassIconStartingGlyph[unit->class];
   gGlyphStrip_65[10] = gClassIconStartingGlyph[unit->class] + 1;
   gGlyphStrip_66[9] = gClassIconStartingGlyph[unit->class] + 2;
   gGlyphStrip_66[10] = gClassIconStartingGlyph[unit->class] + 3;

   i = StringToGlyphs(&gGlyphStrip_68[9], gCharacterNames[unit->name]);
   StringToGlyphs(&gGlyphStrip_68[10 + i], gUnitTypeNames[unit->unitType]);
   StringToGlyphs(&gGlyphStrip_5F[16], gItemNames[unit->weapon]);
   StringToGlyphs(&gGlyphStrip_61[16], gItemNames[unit->helmet]);
   StringToGlyphs(&gGlyphStrip_63[16], gItemNames[unit->armor]);

   gGlyphStrip_5D[6] = GLYPH_BG;
   gGlyphStrip_5D[12] = GLYPH_BG;
   IntToLeftPaddedGlyphs(unit->hp, &gGlyphStrip_5D[6]);
   IntToGlyphs(unit->maxHp, &gGlyphStrip_5D[10]);

   gGlyphStrip_5E[6] = GLYPH_BG;
   gGlyphStrip_5E[11] = GLYPH_BG;
   IntToLeftPaddedGlyphs(unit->mp, &gGlyphStrip_5E[6]);
   IntToGlyphs(unit->maxMp, &gGlyphStrip_5E[10]);

   gGlyphStrip_61[7] = GLYPH_BG;
   gGlyphStrip_61[8] = GLYPH_BG;
   gGlyphStrip_61[9] = GLYPH_BG;

   a = unit->attack;
   if (unit->team == TEAM_PLAYER) {
      a += gItemEquipmentDisplayPower[unit->weapon];
   }
   i = IntToGlyphs(a, &gGlyphStrip_61[6]);
   if (unit->atkBoosted) {
      gGlyphStrip_61[6 + i] = GLYPH_CHAR_PLUS;
   }

   gGlyphStrip_62[7] = GLYPH_BG;
   gGlyphStrip_62[8] = GLYPH_BG;
   gGlyphStrip_62[9] = GLYPH_BG;

   def = unit->defense;
   if (unit->team == TEAM_PLAYER) {
      def += gItemEquipmentDisplayPower[unit->helmet];
      def += gItemEquipmentDisplayPower[unit->armor];
   }
   i = IntToGlyphs(def, &gGlyphStrip_62[6]);
   if (unit->defBoosted) {
      gGlyphStrip_62[6 + i] = GLYPH_CHAR_PLUS;
   }

   gGlyphStrip_63[7] = GLYPH_BG;
   gGlyphStrip_63[8] = GLYPH_BG;
   gGlyphStrip_63[9] = GLYPH_BG;

   i = IntToGlyphs(unit->agility, &gGlyphStrip_63[6]);
   if (unit->aglBoosted) {
      gGlyphStrip_63[6 + i] = GLYPH_CHAR_PLUS;
   }

   gGlyphStrip_5F[7] = GLYPH_BG;
   gGlyphStrip_5F[8] = GLYPH_BG;
   gGlyphStrip_5F[9] = GLYPH_BG;
   IntToGlyphs(unit->exp, &gGlyphStrip_5F[6]);

   gGlyphStrip_60[7] = GLYPH_BG;
   gGlyphStrip_60[8] = GLYPH_BG;

   // exp needed for next level
   IntToGlyphs(100 - unit->exp % 100, &gGlyphStrip_60[6]);

   gGlyphStrip_5C[16] = GLYPH_BG;
   IntToGlyphs(unit->level, &gGlyphStrip_5C[15]);

   icon = Evt_GetLastUnused();
   icon->functionIndex = EVTF_DISPLAY_ICON;
   icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->weapon;
   icon->d.sprite.x1 = 117;
   icon->d.sprite.y1 = 128;

   icon = Evt_GetLastUnused();
   icon->functionIndex = EVTF_DISPLAY_ICON;
   icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->helmet;
   icon->d.sprite.x1 = 117;
   icon->d.sprite.y1 = 146;

   icon = Evt_GetLastUnused();
   icon->functionIndex = EVTF_DISPLAY_ICON;
   icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->armor;
   icon->d.sprite.x1 = 117;
   icon->d.sprite.y1 = 164;
}

void Evtf574_DisplayIcon(EvtData *evt) {
   evt->d.sprite.x3 = evt->d.sprite.x1 + 15;
   evt->d.sprite.y3 = evt->d.sprite.y1 + 15;
   AddEvtPrim_Gui(gGraphicsPtr->ot, evt);
}

void ClearIcons(void) {
   EvtData *p;
   s32 i;

   p = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++) {
      if (p->functionIndex == EVTF_DISPLAY_ICON) {
         p->functionIndex = EVTF_NULL;
      }
      p++;
   }
}

void UpdateCompactUnitInfoWindow(UnitStatus *unit, UnitStatus *unused, u8 param_3) {
   s32 i, px, full, rem;
   s32 terrainBonus;
   EvtData_Sprite *sprite;
   u8 *pTop;
   u8 *pMid;
   u8 *pBtm;

   // ?: 0: bottom;  1: top;  2: off-field (dojo?)
   if (param_3 != 0) {
      pTop = gGlyphStrip_57;
      pMid = gGlyphStrip_58;
      pBtm = gGlyphStrip_59;
   } else {
      pTop = gGlyphStrip_52;
      pMid = gGlyphStrip_53;
      pBtm = gGlyphStrip_54;
   }

   for (i = 2; i < 26; i++) {
      pTop[i] = GLYPH_BG;
      pMid[i] = GLYPH_BG;
   }

   pTop[2] = gClassIconStartingGlyph[unit->class];
   pTop[3] = gClassIconStartingGlyph[unit->class] + 1;
   pMid[2] = gClassIconStartingGlyph[unit->class] + 2;
   pMid[3] = gClassIconStartingGlyph[unit->class] + 3;

   if (param_3 != 2) {
      sprite = &unit->evtSprite->d.sprite;
      terrainBonus =
          gTerrainBonus[gTerrainPtr[TO_TILE(sprite->z1)][TO_TILE(sprite->x1)].terrain] / 100;
      IntToLeftPaddedGlyphs(terrainBonus, &pBtm[2]);
   } else {
      terrainBonus = 0;
      IntToLeftPaddedGlyphs(terrainBonus, &pBtm[2]);
   }

   pBtm[2] = GLYPH_BG;
   IntToLeftPaddedGlyphs(unit->hp, &pBtm[19]);
   pBtm[25] = GLYPH_BG;
   IntToGlyphs(unit->maxHp, &pBtm[23]);
   pBtm[5] = GLYPH_CHAR_PERCENT;

   i = StringToGlyphs(&pMid[5], gCharacterNames[unit->name]);
   i += 5;

   pTop[i] = GLYPH_BG;
   pMid[i] = GLYPH_BG;
   i++;

   i += StringToGlyphs(&pMid[i], gUnitTypeNames[unit->unitType]);
   pTop[i] = GLYPH_BG;
   pMid[i] = GLYPH_BG;

   if (unit->level < 10) {
      IntToLeftPaddedGlyphs(unit->level, &pMid[i]);
      pMid[i + 1] = GLYPH_CHAR_UPPERCASE_L;
   } else {
      IntToLeftPaddedGlyphs(unit->level, &pMid[i + 1]);
      pMid[i + 1] = GLYPH_CHAR_UPPERCASE_L;
   }

   // HP Bar (10 blocks/glyphs):

   // Start out with empty blocks:
   for (i = 0; i < 10; i++) {
      pBtm[i + 9] = GLYPH_HP_BAR_0_PX;
   }

   // Calculate required blocks:
   // e.g full hp: 10000 / 125 = 80px; 80px / 8 = 10 blocks, rem 0
   //     half hp:  5000 / 125 = 40px; 40px / 8 =  5 blocks, rem 0
   px = unit->hpFrac;
   px /= 125;

   if (px == 0) {
      px = 1;
   }

   full = px / 8;
   rem = px % 8;

   // Plot full blocks:
   if (full != 0) {
      for (i = 0; i < full; i++) {
         pBtm[i + 9] = GLYPH_HP_BAR_8_PX;
      }
   }

   // Plot partial block for remainder:
   if (rem != 0) {
      pBtm[full + 9] = GLYPH_HP_BAR_1_PX + rem;
   }
}

void UpdateUnitInfoWindow(UnitStatus *unit) {
   s32 i, px, full, rem;
   s32 terrainBonus;
   EvtData_Sprite *sprite;
   u8 *firstRow, *secondRow, *thirdRow, *fourthRow;

   firstRow = gGlyphStrip_52;
   secondRow = gGlyphStrip_53;
   thirdRow = gGlyphStrip_54;
   fourthRow = gGlyphStrip_55;

   for (i = 2; i < 26; i++) {
      firstRow[i] = GLYPH_BG;
      secondRow[i] = GLYPH_BG;
   }

   firstRow[2] = gClassIconStartingGlyph[unit->class];
   firstRow[3] = gClassIconStartingGlyph[unit->class] + 1;
   secondRow[2] = gClassIconStartingGlyph[unit->class] + 2;
   secondRow[3] = gClassIconStartingGlyph[unit->class] + 3;

   sprite = &unit->evtSprite->d.sprite;
   thirdRow[3] = GLYPH_BG;
   thirdRow[4] = GLYPH_BG;
   thirdRow[5] = GLYPH_BG;
   terrainBonus =
       gTerrainBonus[gTerrainPtr[TO_TILE(sprite->z1)][TO_TILE(sprite->x1)].terrain] / 100;
   IntToLeftPaddedGlyphs(terrainBonus, &fourthRow[2]);
   fourthRow[2] = GLYPH_BG;

   IntToLeftPaddedGlyphs(unit->hp, &thirdRow[19]);
   thirdRow[25] = GLYPH_BG;
   IntToGlyphs(unit->maxHp, &thirdRow[23]);
   IntToLeftPaddedGlyphs(unit->exp % 100, &fourthRow[19]);

   i = StringToGlyphs(&secondRow[5], gCharacterNames[unit->name]);
   i += 5;
   firstRow[i] = GLYPH_BG;
   secondRow[i] = GLYPH_BG;
   i++;
   i += StringToGlyphs(&secondRow[i], gUnitTypeNames[unit->unitType]);
   firstRow[i] = GLYPH_BG;
   secondRow[i] = GLYPH_BG;

   if (unit->level < 10) {
      IntToLeftPaddedGlyphs(unit->level, &secondRow[i]);
      secondRow[i + 1] = GLYPH_CHAR_UPPERCASE_L;
   } else {
      IntToLeftPaddedGlyphs(unit->level, &secondRow[i + 1]);
      secondRow[i + 1] = GLYPH_CHAR_UPPERCASE_L;
   }

   // HP Bar
   for (i = 0; i < 10; i++) {
      thirdRow[i + 9] = GLYPH_HP_BAR_0_PX;
   }

   px = unit->hpFrac;
   px /= 125;
   if (px == 0) {
      px = 1;
   }

   full = px / 8;
   rem = px % 8;
   if (full != 0) {
      for (i = 0; i < full; i++) {
         thirdRow[i + 9] = GLYPH_HP_BAR_8_PX;
      }
   }
   if (rem != 0) {
      thirdRow[full + 9] = GLYPH_HP_BAR_1_PX + rem;
   }

   // EXP Bar
   for (i = 0; i < 10; i++) {
      fourthRow[i + 9] = GLYPH_HP_BAR_0_PX;
   }

   px = (unit->exp % 100) * 80 / 100;
   if (px == 0 && (unit->exp % 100) != 0) {
      px = 1;
   }
   if ((unit->exp % 100) == 0) {
      px = 0;
   }

   full = px / 8;
   rem = px % 8;
   if (full != 0) {
      for (i = 0; i < full; i++) {
         fourthRow[i + 9] = GLYPH_EXP_BAR_8_PX;
      }
   }
   if (rem != 0) {
      fourthRow[full + 9] = GLYPH_EXP_BAR_0_PX + rem;
   }
}

void Evtf002_MenuChoice(EvtData *evt) {
   if (gWindowChoice.s.windowId != (s8)gWindowActiveIdx) {
      gWindowChoice.s.choice = 0;
      gWindowChoice.s.windowId = gWindowActiveIdx;
      gWindowActivatedChoice.raw = gWindowChoice.raw;
   } else {
      gWindowChoice.s.windowId = gWindowActiveIdx;
   }
   if (gWindowActivatedChoice.raw == gWindowChoice.raw) {
      gWindowChoice.s.choice = 0;
      gWindowActivatedChoice.s.choice = 0;
   } else {
      gWindowChoice = gWindowActivatedChoice;
   }
}

void DisplayBasicWindow(s32 windowId) {
   DisplayCustomWindowWithSetChoice(windowId, 0, 1, 0, 0, 0, 0);
}

void DisplayBasicWindowWithSetChoice(s32 windowId, u8 choiceIdx) {
   DisplayCustomWindowWithSetChoice(windowId, 0, 1, 0, 0, 0, choiceIdx);
}

void DisplayCustomWindow(s32 windowId, u8 effect, u8 translucentHighlight, u8 otOfs,
                         u8 disableWraparound, u8 clut) {
   DisplayCustomWindowWithSetChoice(windowId, effect, translucentHighlight, otOfs,
                                    disableWraparound, clut, 0);
}

void DisplayCustomWindowWithSetChoice(s32 windowId, u8 effect, u8 translucentHighlight, u8 otOfs,
                                      u8 disableWraparound, u8 clut, u8 choiceIdx) {
   EvtData *window;
   s16 restored1, restored2, restored3;

   gWindowChoice.raw = 0;
   CloseWindow(windowId);
   window = Evt_GetLastUnusedSkippingTail(20);

   window->functionIndex = EVTF_WINDOW_TBD_004;
   window->d.evtf004.windowId = windowId;
   window->d.evtf004.effect = effect;
   window->d.evtf004.translucentHighlight = translucentHighlight;
   window->d.evtf004.otOfs = otOfs;
   window->d.evtf004.disableWraparound = disableWraparound;
   window->d.evtf004.clut = clut;
   window->state2 = choiceIdx;

   switch (windowId) {
   case 0x00:
   case 0x01:
      window->d.evtf004.x = 350;
      window->d.evtf004.y = 90;
      break;
   case 0x03:
      window->d.evtf004.x = 140;
      window->d.evtf004.y = 120;
      window->functionIndex = EVTF_WINDOW_TBD_005;
      break;
   case 0x1b:
      SaveRestorePos(&restored1, &restored2, 1);
      window->d.evtf004.x = restored1 - 64;
      window->d.evtf004.y = restored2 - 13;
      HI_H(window->d.evtf004.todo_x30) = 1;
      break;
   case 0x1c:
      SaveRestoreHp(&restored1, &restored2, &restored3, 1);
      IntToLeftPaddedGlyphs2(restored2, &gGlyphStrip_50[1]);
      IntToLeftPaddedGlyphs2(restored1, &gGlyphStrip_50[5]);
      SaveRestorePos(&restored1, &restored2, 1);
      window->d.evtf004.x = restored1 + 27;
      window->d.evtf004.y = restored2 - 4;
      break;
   case 0x02:
   case 0x1d:
      window->d.evtf004.x = 102;
      window->d.evtf004.y = 184;
      break;
   case 0x04:
   case 0x1a:
   case 0x1e:
      window->d.evtf004.x = 10;
      window->d.evtf004.y = 16;
      break;
   case 0x1f:
      window->d.evtf004.x = 102;
      window->d.evtf004.y = 175;
      break;
   case 0x20:
      window->d.evtf004.x = 14;
      window->d.evtf004.y = 56;
      break;
   case 0x32:
   case 0x33:
   case 0x34:
   case 0x35:
   case 0x36:
   case 0x37:
   case 0x38:
   case 0x39:
   case 0x3a:
   case 0x3b:
   case 0x3c:
   case 0x3d:
   case 0x3e:
   case 0x3f:
   case 0x40:
   case 0x41:
   case 0x42:
   case 0x43:
   case 0x44:
   case 0x45:
   case 0x46:
      window->functionIndex = EVTF_WINDOW_TBD_005;
      window->d.evtf004.x = gWindowDisplayX[windowId];
      window->d.evtf004.y = gWindowDisplayY[windowId];
      window->d.evtf004.choicesTopMargin = gWindowChoicesTopMargin;
      window->d.evtf004.highlightHeight = gWindowChoiceHeight;
      window->d.evtf004.choiceCt = gWindowChoicesCount;
      window->d.evtf004.choiceHeight = gWindowChoiceHeight;
   }

   window->d.evtf004.halfWidth =
       gGfxSubTextures[GFX_WINDOW_TBD_657 + window->d.evtf004.windowId].w / 2;
   window->d.evtf004.halfHeight =
       gGfxSubTextures[GFX_WINDOW_TBD_657 + window->d.evtf004.windowId].h / 2;
}

void CloseWindow(s32 windowId) {
   s32 i;
   EvtData *p = &gEvtDataArray[0];

   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if ((p->functionIndex == EVTF_WINDOW_TBD_004 || p->functionIndex == EVTF_WINDOW_TBD_005) &&
          p->d.evtf004.windowId == windowId) {
         p->d.evtf004.windowId = 99;
         p->functionIndex = EVTF_CLOSED_WINDOW;
         p->state = 99;
         p->d.evtf004.effectState = 0;

         if (gWindowActivatedChoice.s.windowId == windowId) {
            gWindowActivatedChoice.raw = 0;
            gWindowChoice.raw = 0;
         }
         return;
      }
   }
}

s32 GetWindowChoice(s32 windowId) {
   s32 i;
   EvtData *p = &gEvtDataArray[0];

   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if ((p->functionIndex == EVTF_WINDOW_TBD_004 || p->functionIndex == EVTF_WINDOW_TBD_005) &&
          p->d.evtf004.windowId == windowId) {
         return p->state2 + 1;
      }
   }

   return 1;
}

s32 GetWindowChoice2(s32 windowId) {
   s32 i;
   EvtData *p = &gEvtDataArray[0];

   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if ((p->functionIndex == EVTF_WINDOW_TBD_004 || p->functionIndex == EVTF_WINDOW_TBD_005) &&
          p->d.evtf004.windowId == windowId && p->state == 2) {
         return p->state2 + 1;
      }
   }

   return 0;
}

void SlideWindowTo(s32 windowId, s16 x, s16 y) {
   // Initiates a slide to specified location; actual movement is done by Evtf004_005_408_Window
   s32 i;
   EvtData *p = &gEvtDataArray[0];

   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if ((p->functionIndex == EVTF_WINDOW_TBD_004 || p->functionIndex == EVTF_WINDOW_TBD_005) &&
          p->d.evtf004.windowId == windowId) {
         // Convert specified top-left point to window center point
         p->d.evtf004.destX = x + p->d.evtf004.halfWidth;
         p->d.evtf004.destY = y + p->d.evtf004.halfHeight;
         return;
      }
   }
}

#undef EVTF
#define EVTF 004
void Evtf004_005_408_Window(EvtData *evt) {
   // TODO: todo_x2c, todo_x30 (highlight location)
   EvtData *window;
   EvtData *highlight;
   s8 unused[8];

   window = EVT.window;
   highlight = EVT.highlight;

   switch (evt->state) {
   case 0:
      EVT.x += EVT.halfWidth;

      if (EVT.windowId == 0x1c) {
         EVT.halfWidth >>= 1;
         EVT.x -= EVT.halfWidth;
      }

      if (evt->functionIndex == EVTF_WINDOW_TBD_004) {
         DrawGlyphStripGroup(gGlyphStripGroups[EVT.windowId], GFX_WINDOW_TBD_657 + EVT.windowId);
      }

      EVT.todo_x3a = EVT.halfHeight + (EVT.halfHeight >> 1);
      EVT.y += EVT.halfHeight;

      window = Evt_GetUnused();
      window->functionIndex = EVTF_NOOP;
      window->d.sprite.clut = EVT.clut;
      window->d.sprite.semiTrans = EVT.translucentHighlight;
      if (EVT.otOfs == 0) {
         EVT.otOfs = 3;
      }
      window->d.sprite.otOfs = EVT.otOfs;
      window->d.sprite.gfxIdx = GFX_WINDOW_TBD_657 + EVT.windowId;
      EVT.window = window;

      highlight = Evt_GetUnused();
      highlight->functionIndex = EVTF_NOOP;
      highlight->d.sprite.semiTrans = EVT.translucentHighlight;
      highlight->d.sprite.otOfs = EVT.otOfs;
      highlight->d.sprite.gfxIdx = GFX_WINDOW_TBD_657;
      highlight->d.sprite.hidden = 1;
      EVT.highlight = highlight;

      evt->mem = EVT.effect;
      if (EVT.destX == 0) {
         EVT.destX = EVT.x;
      }
      if (EVT.destY == 0) {
         EVT.destY = EVT.y;
      }

      evt->state++;
   // fallthrough
   case 1:
      //@3a5c
      switch (evt->mem) {
      case 0:
         EVT.relQuadX0 = -EVT.halfWidth;
         EVT.relQuadX1 = EVT.halfWidth;
         EVT.relQuadX2 = -EVT.halfWidth;
         EVT.relQuadX3 = EVT.halfWidth;
         EVT.relQuadY0 = -EVT.halfHeight;
         EVT.relQuadY1 = -EVT.halfHeight;
         EVT.relQuadY2 = EVT.halfHeight;
         EVT.relQuadY3 = EVT.halfHeight;

         if (highlight) {
            highlight->d.sprite.hidden = 0;
         }

         EVT.todo_x2c = EVT.choicesTopMargin << 16;
         evt->state++;
         EVT.effectState = 0;
         EVT.todo_x2c = (evt->state2 * EVT.choiceHeight + EVT.choicesTopMargin) << 16;
         break;
      case 1:
      case 2:

         switch (EVT.effectState) {
         case 0:
            EVT.relQuadX1 = EVT.halfWidth << 1;
            EVT.clut = 8;
            EVT.effectState++;
         // fallthrough
         case 1:
            if (--EVT.clut != 0) {
               EVT.relQuadX1 += (EVT.halfWidth - EVT.relQuadX1) >> 1;
               EVT.relQuadX0 = -EVT.relQuadX1;
               EVT.relQuadX2 = -EVT.relQuadX1;
               EVT.relQuadX3 = EVT.relQuadX1;
               EVT.relQuadY2 += (EVT.halfHeight - EVT.relQuadY2) >> 1;
               EVT.relQuadY0 = -EVT.relQuadY2;
               EVT.relQuadY1 = -EVT.relQuadY2;
               EVT.relQuadY3 = EVT.relQuadY2;
            } else {
               EVT.relQuadX0 = -EVT.halfWidth;
               EVT.relQuadX1 = EVT.halfWidth;
               EVT.relQuadX2 = -EVT.halfWidth;
               EVT.relQuadX3 = EVT.halfWidth;
               EVT.relQuadY0 = -EVT.halfHeight;
               EVT.relQuadY1 = -EVT.halfHeight;
               EVT.relQuadY2 = EVT.halfHeight;
               EVT.relQuadY3 = EVT.halfHeight;
               if (highlight) {
                  highlight->d.sprite.hidden = 0;
               }
               EVT.todo_x2c = EVT.choicesTopMargin << 16;
               evt->state++;
               EVT.effectState = 0;
               EVT.todo_x2c = (evt->state2 * EVT.choiceHeight + EVT.choicesTopMargin) << 16;
            }
         }
         break;
      case 3:
      case 4:
      case 5:

         switch (EVT.effectState) {
         case 0:
            EVT.relQuadX0 = EVT.halfWidth;
            EVT.relQuadX1 = EVT.halfWidth;
            EVT.relQuadX2 = -EVT.halfWidth;
            EVT.relQuadX3 = -EVT.halfWidth;
            EVT.relQuadY0 = EVT.halfHeight;
            EVT.relQuadY1 = EVT.halfHeight;
            EVT.relQuadY2 = -EVT.halfHeight;
            EVT.relQuadY3 = -EVT.halfHeight;
            EVT.effectX = EVT.halfWidth >> 3;
            EVT.effectY = EVT.halfHeight >> 3;
            EVT.clut = 16;
            EVT.effectState++;
         // fallthrough
         case 1:
            if (--EVT.clut != 0) {
               EVT.relQuadX0 = EVT.relQuadX0 - EVT.effectX;
               EVT.relQuadX3 = EVT.relQuadX3 + EVT.effectX;
               EVT.relQuadY0 = EVT.relQuadY0 - EVT.effectY;
               EVT.relQuadY1 = EVT.relQuadY1 - EVT.effectY;
               EVT.relQuadY2 = EVT.relQuadY2 + EVT.effectY;
               EVT.relQuadY3 = EVT.relQuadY3 + EVT.effectY;
            } else {

               EVT.relQuadX0 = -EVT.halfWidth;
               EVT.relQuadX1 = EVT.halfWidth;
               EVT.relQuadX2 = -EVT.halfWidth;
               EVT.relQuadX3 = EVT.halfWidth;
               EVT.relQuadY0 = -EVT.halfHeight;
               EVT.relQuadY1 = -EVT.halfHeight;
               EVT.relQuadY2 = EVT.halfHeight;
               EVT.relQuadY3 = EVT.halfHeight;

               if (highlight) {
                  highlight->d.sprite.hidden = 0;
               }
               EVT.todo_x2c = EVT.choicesTopMargin << 16;
               evt->state++;
               EVT.effectState = 0;
               EVT.todo_x2c = (evt->state2 * EVT.choiceHeight + EVT.choicesTopMargin) << 16;
            }
         }
         break;
      case 6:
      case 7:
         EVT.relQuadX0 = -EVT.halfWidth;
         EVT.relQuadX1 = EVT.halfWidth;
         EVT.relQuadX2 = -EVT.halfWidth;
         EVT.relQuadX3 = EVT.halfWidth;

         EVT.effectPhase += 0xc0;
         EVT.relQuadY0 = -(rcos((EVT.effectPhase - 0x400) & 0xfff) * EVT.todo_x3a >> 12);
         EVT.relQuadY1 = EVT.relQuadY0;
         EVT.relQuadY2 = -EVT.relQuadY0;
         EVT.relQuadY3 = -EVT.relQuadY0;

         //@3f60
         if (EVT.effectPhase > 0x400 && -EVT.relQuadY0 < EVT.halfHeight) {
            EVT.relQuadY0 = -EVT.halfHeight;
            EVT.relQuadY1 = -EVT.halfHeight;
            EVT.relQuadY2 = EVT.halfHeight;
            EVT.relQuadY3 = EVT.halfHeight;
            if (highlight) {
               highlight->d.sprite.hidden = 0;
            }
            EVT.todo_x2c = EVT.choicesTopMargin << 16;
            evt->state++;
            //@3fdc
            EVT.todo_x2c = (evt->state2 * EVT.choiceHeight + EVT.choicesTopMargin) << 16;
         }
         break;

      case 8:
      } // END of switch (evt->mem)  (via state:1)

      break;
   case 2:
      if (gWindowActiveIdx == EVT.windowId) {
         if (gPadStateNewPresses & PAD_DOWN) {
            if (EVT.choiceCt > 1 && !WindowIsOffScreen(evt)) {
               PerformAudioCommand(0x5ed);
            }
            if (!EVT.disableWraparound && evt->state2 + 1 == EVT.choiceCt) {
               // Wrap-around to first choice
               evt->state2 = 0;
            } else {
               // Go to next choice
               evt->state2++;
            }
         }
         if (gPadStateNewPresses & PAD_UP) {
            if (EVT.choiceCt > 1 && !WindowIsOffScreen(evt)) {
               PerformAudioCommand(0x5ed);
            }
            if (!EVT.disableWraparound && evt->state2 == 0) {
               // Wrap-around to last choice
               evt->state2 = EVT.choiceCt - 1;
            } else {
               // Go to previous choice
               evt->state2--;
            }
         }
         evt->state2 = CLAMP(evt->state2, 0, EVT.choiceCt - 1); //@4110
      }
      EVT.todo_x30 = (evt->state2 * EVT.choiceHeight + EVT.choicesTopMargin) << 16;

      if (gState.vsyncMode == 0) {
         EVT.todo_x2c += (EVT.todo_x30 - EVT.todo_x2c) >> 1;
      } else {
         EVT.todo_x2c = EVT.todo_x30;
      }

      if (gWindowActiveIdx == EVT.windowId) {
         gHighlightedChoice = evt->state2 + 1;
         if (gPadStateNewPresses & PAD_CIRCLE) {
            gWindowActivatedChoice.s.windowId = EVT.windowId;
            gWindowActivatedChoice.s.choice = evt->state2 + 1;
         }
         if (gPadStateNewPresses & PAD_X) {
            gWindowActivatedChoice.s.windowId = EVT.windowId;
            gWindowActivatedChoice.s.choice = -1;
         }
      }
      break;

   case 99:

      switch (evt->mem) {
      case 0:
         if (highlight) {
            highlight->d.sprite.hidden = 1;
         }
         //@428c
         evt->state++;
         EVT.effectState = 0;
         evt->functionIndex = EVTF_NULL;
         window->functionIndex = EVTF_NULL;
         highlight->functionIndex = EVTF_NULL;
         return;
      case 1:
      case 2:

         switch (EVT.effectState) {
         case 0:
            EVT.relQuadX0 = -EVT.halfWidth;
            EVT.relQuadX1 = EVT.halfWidth;
            EVT.relQuadX2 = -EVT.halfWidth;
            EVT.relQuadX3 = EVT.halfWidth;
            EVT.relQuadY0 = -EVT.halfHeight;
            EVT.relQuadY1 = -EVT.halfHeight;
            EVT.relQuadY2 = EVT.halfHeight;
            EVT.relQuadY3 = EVT.halfHeight;
            EVT.clut = 8;
            EVT.effectState++;
         // fallthrough
         case 1:
            if (--EVT.clut == 0) {
               if (highlight) {
                  highlight->d.sprite.hidden = 1;
               }
               evt->state++;
               EVT.effectState = 0;
            } else {
               EVT.relQuadY2 += (-EVT.relQuadY2 >> 1);
               EVT.relQuadY0 = -EVT.relQuadY2;
               EVT.relQuadY1 = -EVT.relQuadY2;
               EVT.relQuadY3 = EVT.relQuadY2;

               EVT.relQuadX1 = EVT.relQuadX1 + EVT.relQuadY2;
               EVT.relQuadX0 = -EVT.relQuadX1;
               EVT.relQuadX2 = -EVT.relQuadX1;
               EVT.relQuadX3 = EVT.relQuadX1;
            }
         }

         break;
      case 3:
      case 4:
      case 5:

         switch (EVT.effectState) {
         case 0:
            EVT.relQuadX0 = -EVT.halfWidth;
            EVT.relQuadX1 = EVT.halfWidth;
            EVT.relQuadX2 = -EVT.halfWidth;
            EVT.relQuadX3 = EVT.halfWidth;
            EVT.relQuadY0 = -EVT.halfHeight;
            EVT.relQuadY1 = -EVT.halfHeight;
            EVT.relQuadY2 = EVT.halfHeight;
            EVT.relQuadY3 = EVT.halfHeight;
            EVT.effectX = EVT.halfWidth >> 3;
            EVT.effectY = EVT.halfHeight >> 3;
            EVT.clut = 16;
            EVT.effectState++;
         // fallthrough
         case 1:
            EVT.relQuadX0 = EVT.relQuadX0 + EVT.effectX;
            EVT.relQuadX3 = EVT.relQuadX3 - EVT.effectX;
            EVT.relQuadY0 = EVT.relQuadY0 + EVT.effectY;
            EVT.relQuadY1 = EVT.relQuadY1 + EVT.effectY;
            EVT.relQuadY2 = EVT.relQuadY2 - EVT.effectY;
            EVT.relQuadY3 = EVT.relQuadY3 - EVT.effectY;

            if (--EVT.clut == 0) {
               if (highlight) {
                  highlight->d.sprite.hidden = 1;
               }
               evt->state++;
               EVT.effectState = 0;
            }
         }

         break;
      case 7:
         //@457c
         switch (EVT.effectState) {
         case 0:
            EVT.relQuadX0 = -EVT.halfWidth;
            EVT.relQuadX1 = EVT.halfWidth;
            EVT.relQuadX2 = -EVT.halfWidth;
            EVT.relQuadX3 = EVT.halfWidth;
            EVT.relQuadY0 = -EVT.halfHeight;
            EVT.relQuadY1 = -EVT.halfHeight;
            EVT.relQuadY2 = EVT.halfHeight;
            EVT.relQuadY3 = EVT.halfHeight;
            EVT.effectState++;
         // fallthrough
         case 1:
            EVT.effectPhase -= 0xc0;
            EVT.relQuadY0 = -(rcos((EVT.effectPhase - 0x400U) & 0xfff) * EVT.todo_x3a >> 12);
            EVT.relQuadY1 = EVT.relQuadY0;
            EVT.relQuadY2 = -EVT.relQuadY0;
            EVT.relQuadY3 = -EVT.relQuadY0;

            if (EVT.effectPhase <= 0) {
               EVT.clut = 0;
               EVT.effectState = 0;
               evt->state++;
            }

            if (highlight) {
               highlight->d.sprite.hidden = 1;
            }
         }

         break;
      case 8:
         break;
      } // END of SECOND switch (evt->mem) (via state:99)
      break;
   case 100:
      evt->functionIndex = EVTF_NULL;
      window->functionIndex = EVTF_NULL;
      highlight->functionIndex = EVTF_NULL;
      return;
   } // END of switch (evt->state)

   // switchD_8001f2ac_caseD_6: //@46a0
   EVT.x += (EVT.destX - EVT.x) >> 2;
   EVT.y += (EVT.destY - EVT.y) >> 2;

   window->d.sprite.coords[0].x = EVT.x + EVT.relQuadX0;
   window->d.sprite.coords[0].z = EVT.y + EVT.relQuadY0;
   window->d.sprite.coords[0].y = EVT.x + EVT.relQuadX1;
   window->d.sprite.coords[1].x = EVT.y + EVT.relQuadY1;
   window->d.sprite.coords[1].z = EVT.x + EVT.relQuadX2;
   window->d.sprite.coords[1].y = EVT.y + EVT.relQuadY2;
   window->d.sprite.coords[2].x = EVT.x + EVT.relQuadX3;
   window->d.sprite.coords[2].z = EVT.y + EVT.relQuadY3;

   if (highlight && EVT.choiceCt != 0 && evt->state == 2) {
      //@479c
      highlight->d.sprite.y1 = window->d.sprite.coords[1].x + (EVT.todo_x2c >> 16);
      highlight->d.sprite.y3 = highlight->d.sprite.y1 + EVT.highlightHeight;
      highlight->d.sprite.x1 = window->d.sprite.coords[0].x;
      highlight->d.sprite.x3 = window->d.sprite.coords[0].y;

      gGfxSubTextures[GFX_WINDOW_TBD_657].x = gGfxSubTextures[GFX_WINDOW_TBD_657 + EVT.windowId].x;
      gGfxSubTextures[GFX_WINDOW_TBD_657].w = gGfxSubTextures[GFX_WINDOW_TBD_657 + EVT.windowId].w;
      gGfxTPageCells[GFX_WINDOW_TBD_657] = gGfxTPageCells[GFX_WINDOW_TBD_657 + EVT.windowId];
      gGfxTPageIds[GFX_WINDOW_TBD_657] = gGfxTPageIds[GFX_WINDOW_TBD_657 + EVT.windowId];
      gGfxSubTextures[GFX_WINDOW_TBD_657].y =
          (EVT.todo_x2c >> 16) + gGfxSubTextures[GFX_WINDOW_TBD_657 + EVT.windowId].y;
      gGfxSubTextures[GFX_WINDOW_TBD_657].h = EVT.highlightHeight;
      if (EVT.windowId == gWindowActiveIdx) {
         highlight->d.sprite.clut = 0;
      } else {
         highlight->d.sprite.clut = 19;
      }
      AddEvtPrim_Gui(gGraphicsPtr->ot, highlight);
   }

   AddEvtPrim2(gGraphicsPtr->ot, window);

   if (gWindowActiveIdx == EVT.windowId) {
      gHighlightedChoice = evt->state2 + 1;
      if (gPadStateNewPresses & PAD_CIRCLE) {
         gWindowActivatedChoice.s.windowId = EVT.windowId;
         gWindowActivatedChoice.s.choice = evt->state2 + 1;
      }
      if (gPadStateNewPresses & PAD_X) {
         gWindowActivatedChoice.s.windowId = EVT.windowId;
         gWindowActivatedChoice.s.choice = -1;
      }
   }
}

void DrawGlyphStrip(u8 *strip, s16 x, s16 y, u8 unused) {
   RECT rect;
   u8 current, first;

   rect.w = VRAM_GLYPH_W;
   first = *strip++;
   current = *strip++;

   while (current != GLYPH_TERMINATOR) {
      if (first != GLYPH_BLANK) {
         rect.h = 4;
      } else {
         rect.h = 9;
      }
      rect.x = VRAM_GLYPHS_X + (current % VRAM_GLYPHS_PER_ROW) * VRAM_GLYPH_W;
      rect.y = VRAM_GLYPHS_Y + (current / VRAM_GLYPHS_PER_ROW) * VRAM_GLYPH_H;
      if (first != GLYPH_BLANK) {
         rect.y += 5;
      }
      MoveImage(&rect, x, y);
      x += VRAM_GLYPH_W;
      current = *strip++;
   }
}

void ClearGlyphStripBottom(u8 *strip, s16 x, s16 y) {
   u8 current;
   u8 dbl;
   u8 first;
   u8 unused[8];
   RECT rect;

   dbl = 0;
   rect.w = (16 >> 2);
   rect.h = 1;
   rect.x = VRAM_GLYPHS_X;
   rect.y = VRAM_GLYPHS_Y;

   first = *strip++;
   current = *strip++;

   while (current != GLYPH_TERMINATOR) {
      if (current == GLYPH_TBD_254) {
         dbl = 1;
         current = *strip++;
      }
      if (current == GLYPH_TBD_253) {
         dbl = 0;
         current = *strip++;
      }

      if (dbl) {
         MoveImage(&rect, x, y + 9);
         x += VRAM_GLYPH_W;
         MoveImage(&rect, x, y + 9);
         x += VRAM_GLYPH_W;
      } else {
         MoveImage(&rect, x, y + 9);
         x += VRAM_GLYPH_W;
      }

      current = *strip++;
   }
}

void DrawGlyphStripGroup(u8 *group, s16 gfxIdx) {
   s16 x, y;
   u8 current, first;

   y = 0;
   x = 512;
   if (gGfxTPageCells[gfxIdx] == 9) {
      x += 64;
   }
   if (gGfxTPageCells[gfxIdx] == 14) {
      x += 384;
   }
   if (gGfxTPageCells[gfxIdx] == 15) {
      x += 448;
   }

   x += gGfxSubTextures[gfxIdx].x >> 2;
   y += gGfxSubTextures[gfxIdx].y;

   first = *group++;
   current = *group++;

   while (current != 0) {
      DrawGlyphStrip(gGlyphStrips[current], x, y, first);
      first = !first; // hm

      if (gGlyphStrips[current][0] != GLYPH_BLANK) {
         y += 4;
      } else {
         y += 9;
      }
      current = *group++;
   }
   group -= 2;
   current = *group;
   ClearGlyphStripBottom(gGlyphStrips[current], x, y - 9);
   ClearGlyphStripBottom(gGlyphStrips[current], x, y - 8);
   DrawSync(0);
}

u16 DrawEmbossedSjisChar(u16 sjis, s32 x, s32 y, s32 fgc, s32 bgc) {
   extern u32 s_embossedCharData[34];

   s32 i, j;
   u8 bits;
   u8 *p;
   u32 fg, bg, mask;
   RECT rect;
   u8 unused[128];

   p = Krom2RawAdd(sjis);
   if (p == -1) {
      return 0xff;
   }

   for (i = 0; i < 34; i++) {
      s_embossedCharData[i] = 0;
   }

   for (i = 0; i < 30; i++, p++) {
      bg = 0;
      mask = 0;
      fg = 0;
      bits = *p;

      for (j = 0; j < 7; j++) {
         if (bits & 1) {
            fg |= fgc;
            bg |= bgc;
            mask |= 0xf;
         }
         bits >>= 1;
         fg <<= 4;
         bg <<= 4;
         mask <<= 4;
      }
      if (bits & 1) {
         fg |= fgc;
         bg |= bgc;
         mask |= 0xf;
      }

      s_embossedCharData[i] |= bg << 4;
      s_embossedCharData[i + 2] |= bg;
      s_embossedCharData[i + 2] |= (bg << 4);

      if ((bg >> 28) && !(i & 1)) {
         s_embossedCharData[i + 1] |= bgc;
         s_embossedCharData[i + 3] |= bgc;
      }

      s_embossedCharData[i] &= ~mask;
      s_embossedCharData[i] |= fg;
   }

   s_embossedCharData[32] = s_embossedCharData[33] = 0;

   rect.x = x;
   rect.y = y;
   rect.w = 16 >> 2;
   rect.h = 16;
   LoadImage(&rect, s_embossedCharData);
   DrawSync(0);
   return GetTPage(0, 0, x, y);
}

void Evtf422_LowerMsgBoxTail(EvtData *evt) { Evtf421_UpperMsgBoxTail(evt); }

#undef EVTF
#define EVTF 421
void Evtf421_UpperMsgBoxTail(EvtData *evt) {
   s32 p, flag;
   SVECTOR position;

   RotTransPers(&EVT.sprite->vec, (s32 *)&position, &p, &flag);
   EVT.left = position.vx - 4;
   gTempGfxEvt->d.sprite.gfxIdx = GFX_MSGBOX_TAIL_DOWN;

   if (evt->functionIndex == EVTF_UPPER_MSGBOX_TAIL) {
      position.vy -= 45;
      if (position.vy < 86) {
         s16 width = 18;
         if (EVT.left < SCREEN_HALF_WIDTH) {
            gTempGfxEvt->d.sprite.gfxIdx = GFX_MSGBOX_TAIL_LEFT;
            EVT.left += width;
         } else {
            gTempGfxEvt->d.sprite.gfxIdx = GFX_MSGBOX_TAIL_RIGHT;
            EVT.left -= width;
         }
         position.vy = 99;
      }
      EVT.top = 78;
      EVT.bottom = position.vy;
   } else {
      // EVTF_LOWER_MSGBOX_TAIL:
      position.vy += -10;
      if (position.vy > 150) {
         position.vy = 150;
      }
      EVT.bottom = position.vy;
      EVT.top = 162;
   }

   gTempGfxEvt->d.sprite.hidden = 0;
   gTempGfxEvt->d.sprite.clut = 25;
   gTempGfxEvt->d.sprite.otOfs = 2;

   if (gTempGfxEvt->d.sprite.gfxIdx == GFX_MSGBOX_TAIL_DOWN) {
      gTempGfxEvt->d.sprite.x1 = EVT.left;
      gTempGfxEvt->d.sprite.x3 = EVT.left + 10;
   } else {
      gTempGfxEvt->d.sprite.x1 = EVT.left - 2;
      gTempGfxEvt->d.sprite.x3 = EVT.left + 14;
   }

   gTempGfxEvt->d.sprite.y1 = EVT.top;
   gTempGfxEvt->d.sprite.y3 = EVT.bottom;
   AddEvtPrim_Gui(gGraphicsPtr->ot, gTempGfxEvt);
}

void Evtf573_BattleItemsList(EvtData *evt) {
   s32 i;
   UnitStatus *unit;
   EvtData *icon;
   s32 tmp;

   unit = evt->d.evtf573.unit;

   switch (evt->state) {
   case 0:
      CloseWindow(0x1e);
      gWindowChoiceHeight = 18;
      gWindowChoicesTopMargin = 9;
      DrawWindow(0x38, 0, 50, 136, 54, 70, 93, WBS_CROSSED, 2);

      for (i = 0; i < 2; i++) {
         if (gItemSpells[unit->items[i]] == SPELL_NULL)
            tmp = 1; // Color to indicate item is unusable
         else
            tmp = 0;
         DrawSjisText(28, i * 18 + 60, 20, 0, tmp, gItemNamesSjis[unit->items[i]]);
      }

      icon = Evt_GetUnused();
      icon->functionIndex = EVTF_DISPLAY_ICON;
      icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->items[0];
      icon->d.sprite.x1 = 79;
      icon->d.sprite.y1 = 103;

      icon = Evt_GetUnused();
      icon->functionIndex = EVTF_DISPLAY_ICON;
      icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->items[1];
      icon->d.sprite.x1 = 79;
      icon->d.sprite.y1 = 121;

      DisplayBasicWindow(0x38);
      gWindowActiveIdx = 0x38;
      gClearSavedPadState = 1;
      evt->state++;
      break;
   case 1:
      if (gWindowChoice.raw == 0x38ff) {
         // Canceled:
         CloseWindow(0x38);
         gSignal2 = 1;
         evt->functionIndex = EVTF_NULL;
         CloseWindow(0x3c);
         CloseWindow(0x3d);
         ClearIcons();
         return;
      }
      if (gWindowChoice.s.windowId == 0x38 && gWindowChoice.s.choice != 0) {
         gCurrentSpell = gItemSpells[unit->items[gWindowChoice.s.choice - 1]];
         if (gCurrentSpell != SPELL_NULL) {
            gState.activeItem = unit->items[gWindowChoice.s.choice - 1];
            gState.activeItemSlot = gWindowChoice.s.choice - 1;
            unit->items[gWindowChoice.s.choice - 1] = ITEM_NULL;
            CloseWindow(0x38);
            gSignal2 = 2;
            gClearSavedPadState = 0;
            evt->functionIndex = EVTF_NULL;
            CloseWindow(0x3c);
            CloseWindow(0x3d);
            ClearIcons();
            return;
         }
      }
      break;
   }

   switch (evt->d.evtf573.drawState) {
   case 0:
      evt->d.evtf573.item = -1;
      DrawWindow(0x3c, 0, 0, 288, 36, 14, 192, WBS_CROSSED, 0);
      DisplayBasicWindow(0x3c);
      DisplayBasicWindow(0x3d);
      evt->d.evtf573.drawState++;
      break;
   case 1:
      tmp = unit->items[GetWindowChoice(0x38) - 1];
      if (evt->d.evtf573.item != tmp) {
         evt->d.evtf573.item = tmp;
         DrawWindow(60, 0, 0, 288, 36, 4, 192, WBS_CROSSED, 0);
         DrawText_Internal(12, 10, 35, 2, 0, gItemDescriptions[tmp], 0);
      }
      break;
   }
}

void Evtf031_BattleSpellsList(EvtData *evt) {
   static u8 mpBuffer[9] = "\x82\x6c\x82\x6f\x81\x40\x81\x40\x00";
   UnitStatus *unit;
   s32 spellIdx;
   s32 i;
   s32 extraTopMargin, extraHeight, shorten;
   s32 numSpells;
   s32 spell;
   // evt->state2: page number

   unit = evt->d.evtf031.unit;

   switch (evt->state) {
   case 0:
      DrawWindow(0x3e, 400, 200, 72, 36, 214, 140, WBS_CROSSED, 0);
      EmbedIntAsSjis(unit->mp, &mpBuffer[4], 2);
      DrawSjisText(404, 210, 20, 0, 0, mpBuffer);
      DisplayBasicWindow(0x3e);
      CloseWindow(0x1e);
      gWindowChoiceHeight = 17;

      if (!gState.debug && unit->weapon != ITEM_V_HEART_2) {
         numSpells = 0;
         while (unit->spells[numSpells] != SPELL_NULL) {
            numSpells++;
         }

         extraTopMargin = numSpells;
         shorten = 0;
         if (numSpells >= 7) {
            shorten = 9;
            extraTopMargin = numSpells - 7;
            extraHeight = 1;
         } else {
            extraHeight = 0;
         }
         extraTopMargin /= 2;
         gWindowChoicesTopMargin = extraTopMargin + 9;

         DrawWindow(0x38, 0, 50, 144, extraHeight + (numSpells * 18 + 18 - shorten), 70,
                    10 + (10 - numSpells) * 18 / 2, WBS_CROSSED, numSpells);

         for (i = 0; i < numSpells; i++) {
            s32 color;
            if (unit->mp < gSpells[unit->spells[i]].mpCost)
               color = 1;
            else
               color = 0;
            DrawText(12, 60 + i * 17 + extraTopMargin, 20, 0, color, gSpellNames[unit->spells[i]]);
         }
         DisplayBasicWindow(0x38);
      }

      gWindowActiveIdx = 0x38;
      gClearSavedPadState = 1;

      if (gState.debug || unit->weapon == ITEM_V_HEART_2) {
         gWindowChoicesTopMargin = 9;
         DrawWindow(0x38, 0, 50, 144, 190, 70, 8, WBS_CROSSED, 10);

         i = 0;
         spellIdx = evt->state2 * 10 + 1;

         while (spellIdx < evt->state2 * 10 + 1 + 10) {
            DrawText(12, i * 17 + 60, 20, 0, 0, gSpellNames[spellIdx]);
            i++;
            spellIdx++;
         }

         DisplayBasicWindow(0x38);
      }

      evt->state++;
      break;

   case 1:

      if (gState.debug || unit->weapon == ITEM_V_HEART_2) {
         // Paged menu to select from all spells; for debug mode / vandalier
         if (gPadStateNewPresses & PAD_RIGHT) {
            if (evt->state2 != 6) {
               evt->state2++; // Next page
            }
            DrawWindow(0x38, 0, 50, 144, 190, 70, 8, WBS_CROSSED, 10);

            i = 0;
            spellIdx = evt->state2 * 10 + 1;

            while (spellIdx < evt->state2 * 10 + 1 + 10) {
               DrawText(12, i * 17 + 60, 20, 0, 0, gSpellNames[spellIdx]);
               i++;
               spellIdx++;
            }
         }
         if (gPadStateNewPresses & PAD_LEFT) {
            if (evt->state2 != 0) {
               evt->state2--; // Previous page
            }
            DrawWindow(0x38, 0, 50, 144, 190, 70, 8, WBS_CROSSED, 8);

            i = 0;
            spellIdx = evt->state2 * 10 + 1;

            while (spellIdx < evt->state2 * 10 + 1 + 10) {
               DrawText(12, i * 17 + 60, 20, 0, 0, gSpellNames[spellIdx]);
               i++;
               spellIdx++;
            }
         }
      }

      if (gWindowChoice.raw == 0x38ff) {
         // Canceled
         CloseWindow(0x38);
         CloseWindow(0x3e);
         gSignal2 = 1;
         evt->functionIndex = EVTF_NULL;
         CloseWindow(0x3c);
         CloseWindow(0x3d);
         return;
      }
      if (gWindowChoice.s.windowId == 0x38 && gWindowChoice.s.choice != 0) {
         gCurrentSpell = unit->spells[gWindowChoice.s.choice - 1];
         if (gState.debug || unit->weapon == ITEM_V_HEART_2) {
            gCurrentSpell = gWindowChoice.s.choice + evt->state2 * 10;
         }
         if (unit->mp < gSpells[gCurrentSpell].mpCost) {
            gCurrentSpell = SPELL_NULL;
         }
         if (gCurrentSpell != SPELL_NULL) {
            CloseWindow(0x38);
            CloseWindow(0x3e);
            gSignal2 = 2;
            gClearSavedPadState = 0;
            evt->functionIndex = EVTF_NULL;
            CloseWindow(0x3c);
            CloseWindow(0x3d);
            return;
         }
      }
      break;
   }

   switch (evt->d.evtf031.drawState) {
   case 0:
      DrawWindow(0x3c, 0, 0, 288, 36, 14, 192, WBS_CROSSED, 0);
      DisplayBasicWindow(0x3c);
      DisplayBasicWindow(0x3d);
      evt->d.evtf031.drawState++;
      break;
   case 1:
      spellIdx = GetWindowChoice(0x38);
      spell = unit->spells[spellIdx - 1];
      if (gState.debug || unit->weapon == ITEM_V_HEART_2) {
         spell = spellIdx + evt->state2 * 10;
      }
      if (evt->d.evtf031.spell != spell) {
         evt->d.evtf031.spell = spell;
         DrawWindow(60, 0, 0, 288, 36, 4, 192, WBS_CROSSED, 0);
         DrawText_Internal(12, 10, 35, 2, 0, gSpellDescriptions[spell], 0);
      }
      break;
   }
}
