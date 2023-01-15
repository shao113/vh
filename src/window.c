#include "common.h"
#include "state.h"
#include "evt.h"
#include "units.h"
#include "window.h"
#include "graphics.h"
#include "glyphs.h"
#include "field.h"

s32 WindowIsOffScreen(EvtData *evt) {
   if (evt->d.evtf004.x < 0)
      return 1;
   if (evt->d.evtf004.x > 320)
      return 1;
   if (evt->d.evtf004.y < 0)
      return 1;
   if (evt->d.evtf004.y > 240)
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
      StringToGlyphs(&gGlyphStrip_09[4], gItemNames[gPartyMembers[partyIdx].item1]);
      StringToGlyphs(&gGlyphStrip_0B[4], gItemNames[gPartyMembers[partyIdx].item2]);
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
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][8];
         gGfxTPageCells[GFX_WINDOW_TBD_658 + windowId] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_658 + windowId] = gTPageIds[0][9];
      } else {
         gGfxTPageCells[GFX_WINDOW_TBD_707] = 14;
         gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][14];
         gGfxTPageCells[GFX_WINDOW_TBD_708] = 15;
         gGfxTPageIds[GFX_WINDOW_TBD_708] = gTPageIds[0][15];

         DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
         DrawGlyphStripGroup(gGlyphStripGroups[51], GFX_WINDOW_TBD_708);

         gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][8];
         gGfxTPageCells[GFX_WINDOW_TBD_708] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_708] = gTPageIds[0][9];

         gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][8];
         gGfxTPageCells[GFX_WINDOW_TBD_658 + windowId] = 9;
         gGfxTPageIds[GFX_WINDOW_TBD_658 + windowId] = gTPageIds[0][9];
      }
   } else {
      // !usingMultipleTPages:
      if (type == 0) {
         DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
         gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 8;
         gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][8];
      } else {
         if (type == 1) {
            gGfxTPageCells[GFX_WINDOW_TBD_707] = 9;
            gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][9];
            DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
            gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
            gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][8];
            gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 9;
            gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][9];
         } else {
            if (type == 2) {
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 14;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][14];
               DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][8];
               gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 14;
               gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][14];
            } else {
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 15;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][15];
               DrawGlyphStripGroup(gGlyphStripGroups[50], GFX_WINDOW_TBD_707);
               gGfxTPageCells[GFX_WINDOW_TBD_707] = 8;
               gGfxTPageIds[GFX_WINDOW_TBD_707] = gTPageIds[0][8];
               gGfxTPageCells[GFX_WINDOW_TBD_657 + windowId] = 15;
               gGfxTPageIds[GFX_WINDOW_TBD_657 + windowId] = gTPageIds[0][15];
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

s32 StringToGlyphs(GlyphIndex *dest, u8 *src) {
   s32 ct = 0;

   if (*src == '\0') {
      return -1;
   }
   while (*src != '\0') {
      *(dest++) = GetGlyphIdxForAsciiChar(*(src++));
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
      inByte = *(src++);
      outByte = inByte;
      if (outByte >= 0x9b) {
         outByte = inByte - 0x89;
         if (inByte >= 0xaa && inByte <= 0xae) {
            outByte = inByte - 0x84;
         }
      }
      if (inByte == 0x9a) {
         outByte = 0xf;
      }
      *(dest++) = outByte;
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
      inByte = *(src++);
      outByte = 1;
      if (inByte >= 0x9a) {
         outByte = 0x9a;
         if (inByte >= 0xaa && inByte <= 0xae) {
            outByte = 0x9b;
         }
      }
      *(dest++) = outByte;
      ct++;
   }
   return ct;
}

void UpdateSkillStatusWindow(UnitStatus *unit) {
   s32 i;
   s32 atk, def;
   EvtData *icon;

   {
      // TODO: Why are a0/v1 swapped without this hack? What am I missing...
      register s32 i asm("a0");
      for (i = 2; i < 26; i++) {
         gGlyphStrip_65[i] = GLYPH_BG;
         gGlyphStrip_66[i] = GLYPH_BG;
         gGlyphStrip_67[i] = GLYPH_BG;
         gGlyphStrip_68[i] = GLYPH_BG;
      }
      for (i = 16; i < 28; i++) {
         gGlyphStrip_5F[i] = GLYPH_BG;
         gGlyphStrip_61[i] = GLYPH_BG;
         gGlyphStrip_63[i] = GLYPH_BG;
      }
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

   atk = unit->attack;
   if (unit->team == TEAM_PLAYER) {
      atk += gItemEquipmentDisplayPower[unit->weapon];
   }
   i = IntToGlyphs(atk, &gGlyphStrip_61[6]);
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
   GlyphIndex *pTop;
   GlyphIndex *pMid;
   GlyphIndex *pBtm;

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