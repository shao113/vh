#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "glyphs.h"
#include "units.h"
#include "field.h"

void Evtf574_DisplayIcon(EvtData *evt) {
   evt->d.sprite.x3 = evt->d.sprite.x1 + 15;
   evt->d.sprite.y3 = evt->d.sprite.y1 + 15;
   AddEvtPrim_TBD(gGraphicsPtr->ot, evt);
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

/*void IntToLeftPaddedGlyphs(s32 n, GlyphIndex *dest);
s32 StringToGlyphs(GlyphIndex *dest, u8 *src);
s32 IntToGlyphs(s32 n, GlyphIndex *dest);*/

void UpdateCompactUnitInfoWindow(UnitStatus *unit, UnitStatus *unused, u8 param_3) {
   s32 i, px, full, rem;
   s32 terrainBonus;
   EvtData_Sprite *sprite;
   GlyphIndex *pTop;
   GlyphIndex *pMid;
   GlyphIndex *pBtm;

   /* ?: 0: bottom;  1: top;  2: off-field (dojo?) */
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

   /* HP Bar (10 blocks/glyphs): */

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
