#include "common.h"
#include "evt.h"
#include "state.h"
#include "graphics.h"
#include "field.h"
#include "units.h"
#include "glyphs.h"

void ClearUnits(void) {
   s32 i;
   UnitStatus *pUnit;

   for (i = 0; i < UNIT_CT; i++) {
      pUnit = &gUnits[i];
      pUnit->idx = 0;
   }
}

UnitStatus *CreateUnitInNextSlot(void) { return CreateUnit(0); }

UnitStatus *CreateUnitInLastSlot(void) { return CreateUnit(1); }

UnitStatus *CreateUnit(u8 last) {
   s32 i, j;
   UnitStatus *pUnit = &gUnits[1];

   for (i = 1; i < UNIT_CT; i++, pUnit++) {
      if (pUnit->idx == 0) {
         if (last) {
            pUnit = &gUnits[UNIT_CT - 1];
            i = UNIT_CT - 1;
         }
         pUnit->idx = i;
         pUnit->level = 1;
         pUnit->team = TEAM_NULL;
         pUnit->done = 0;
         for (j = 0; j < 8; j++) {
            pUnit->experience[j] = 0;
         }
         pUnit->exp = 0;
         pUnit->hpFrac = 10000;
         pUnit->mp = 0;
         pUnit->atkVar10000 = 10000;
         pUnit->defVar10000 = 10000;
         pUnit->agiVar10000 = 10000;
         pUnit->hpVar100 = 100;
         pUnit->atkVar100 = 100;
         pUnit->defVar100 = 100;
         pUnit->agiVar100 = 100;
         pUnit->direction = 0;
         pUnit->battler = NULL;
         pUnit->sprite = NULL;
         pUnit->hideMarker = 0;
         pUnit->poisoned = 0;
         pUnit->paralyzed = 0;
         pUnit->atkBoosted = 0;
         pUnit->defBoosted = 0;
         pUnit->aglBoosted = 0;
         break;
      }
   }
   return pUnit;
}

void StartUnitSpritesDecoder(u8 stripIdx) {
   EvtData *evt = &gEvtData050_UnitSpritesDecoder;

   stripIdx -= 2;
   evt->functionIndex = EVTF_UNIT_SPRITES_DECODER;
   evt->state = 0;
   evt->d.evtf050.baseSrcDataPtr = gEncodedUnitSpriteData[gUnitSetEncodedSpriteDataIdx[stripIdx]];
   evt->d.evtf050.baseDstDataPtr = gScratch1_801317c0;
   evt->d.evtf050.vramX = 704;
   evt->d.evtf050.vramY = 0;
}

EvtData *GetUnitSpriteAtPosition(u8 z, u8 x) {
   EvtData *unitSprite;
   u8 unitIdx = gMapUnitsPtr[z][x].s.unitIdx;

   if (unitIdx == 0) {
      unitSprite = NULL;
   } else {
      unitSprite = gUnits[unitIdx].sprite;
   }

   return unitSprite;
}

EvtData *FindUnitSpriteByNameIdx(u8 nameIdx) {
   s32 i;
   UnitStatus *pUnit = &gUnits[1];

   for (i = 1; i < UNIT_CT; i++) {
      if (pUnit->idx != 0 && pUnit->name == nameIdx) {
         return pUnit->sprite;
      }
      pUnit++;
   }

   return NULL;
}

EvtData *FindUnitSpriteByType(u8 unitType) {
   s32 i;
   UnitStatus *pUnit = &gUnits[1];

   for (i = 1; i < UNIT_CT; i++) {
      if (pUnit->idx != 0 && pUnit->unitType == unitType) {
         return pUnit->sprite;
      }
      pUnit++;
   }

   return NULL;
}

void GetUnitSpriteVramRect(EvtData *unitSprite, s32 *px, s32 *py, s32 *pw, s32 *ph) {
   s32 tpage;
   s32 gfx = unitSprite->d.sprite.gfxIdx;
   s32 stripIdx = unitSprite->d.sprite.stripIdx;

   if (gfx < 35) {
      if (gfx < 26) {
         *px = 0xb00;
         *py = 0;
      } else {
         *px = 0xb00;
         *py = 0x100;
      }
      *px += gTexwSpriteSetFrames[gfx].x;
      *py += gTexwSpriteSetFrames[gfx].y;
      *pw = gTexwSpriteSetFrames[gfx].w;
      *ph = gTexwSpriteSetFrames[gfx].h;
   } else {
      if (gfx < 40) {
         tpage = gSpriteStripTPageIds[stripIdx * 2];
         gfx += (stripIdx % 5) * 5;
         gfx -= 35;
      } else {
         tpage = gSpriteStripTPageIds[stripIdx * 2 + 1];
         gfx += (stripIdx % 5) * 5;
         gfx -= 40;
      }
      if (tpage >= 16) {
         tpage -= 16;
         *px = tpage * 0x100;
         *py = 0x100;
      } else {
         *px = tpage * 0x100;
         *py = 0;
      }
      *px += gTexwSpriteStripFrames[gfx].x;
      *py += gTexwSpriteStripFrames[gfx].y;
      *pw = gTexwSpriteStripFrames[gfx].w;
      *ph = gTexwSpriteStripFrames[gfx].h;
   }
}

s16 s_savedDamage_80123230;

void SaveRestoreDamage(s16 *damage, s32 restore) {
   if (!restore) {
      s_savedDamage_80123230 = *damage;
   } else {
      *damage = s_savedDamage_80123230;
   }
}

s16 s_savedBeforeHpPercent_80123234;
s16 s_savedAfterHpPercent_80123238;

void SaveRestoreHpPercents(s16 *before, s16 *after, s32 restore) {
   if (!restore) {
      s_savedBeforeHpPercent_80123234 = *before;
      s_savedAfterHpPercent_80123238 = *after;
   } else {
      *before = s_savedBeforeHpPercent_80123234;
      *after = s_savedAfterHpPercent_80123238;
   }
}

s16 s_savedMaxHp_8012323c;
s16 s_savedPrevHp_80123240;
s16 s_savedCurHp_80123244;

void SaveRestoreHp(s16 *maxHp, s16 *prevHp, s16 *curHp, s32 restore) {
   if (!restore) {
      s_savedMaxHp_8012323c = *maxHp;
      s_savedPrevHp_80123240 = *prevHp;
      s_savedCurHp_80123244 = *curHp;
   } else {
      *maxHp = s_savedMaxHp_8012323c;
      *prevHp = s_savedPrevHp_80123240;
      *curHp = s_savedCurHp_80123244;
   }
}

s16 s_savedPosX_80123248;
s16 s_savedPosY_8012324c;

void SaveRestorePos(s16 *x, s16 *y, s32 restore) {
   if (!restore) {
      s_savedPosX_80123248 = *x;
      s_savedPosY_8012324c = *y;
   } else {
      *x = s_savedPosX_80123248;
      *y = s_savedPosY_8012324c;
   }
}

void IntToLeftPaddedGlyphs2(s16 n, u8 *dst) {
   // Uses alternative digit font (e.g. for damage hp gauge)
   s16 units;

   units = n / 100;
   n -= units * 100;
   *dst++ = GLYPH_CHAR_ALT_0 + units;

   units = n / 10;
   n -= units * 10;
   *dst++ = GLYPH_CHAR_ALT_0 + units;

   *dst = GLYPH_CHAR_ALT_0 + n;

   // Replace leading zeroes with blanks
   if (dst[-2] == GLYPH_CHAR_ALT_0) {
      dst[-2] = GLYPH_BG;
      if (dst[-1] == GLYPH_CHAR_ALT_0) {
         dst[-1] = GLYPH_BG;
      }
   }
}

void IntToLeftPaddedGlyphs(s16 n, u8 *dst) {
   s16 units;

   units = n / 100;
   n -= units * 100;
   *dst++ = GLYPH_CHAR_0 + units;

   units = n / 10;
   n -= units * 10;
   *dst++ = GLYPH_CHAR_0 + units;

   *dst = GLYPH_CHAR_0 + n;

   // Replace leading zeroes with blanks
   if (dst[-2] == GLYPH_CHAR_0) {
      dst[-2] = GLYPH_BG;
      if (dst[-1] == GLYPH_CHAR_0) {
         dst[-1] = GLYPH_BG;
      }
   }
}

s32 IntToGlyphs(s16 n, u8 *dst) {
   s16 units;
   s32 len = 1;
   s32 started = 0;

   units = n / 1000;
   n -= units * 1000;

   if (units != 0 || started) {
      *dst++ = GLYPH_CHAR_0 + units;
      started = 1;
      len++;
   }

   units = n / 100;
   n -= units * 100;

   if (units != 0 || started) {
      *dst++ = GLYPH_CHAR_0 + units;
      started = 1;
      len++;
   }

   units = n / 10;
   n -= units * 10;

   if (units != 0 || started) {
      *dst++ = GLYPH_CHAR_0 + units;
      started = 1;
      len++;
   }

   *dst = GLYPH_CHAR_0 + n;
   return len;
}

#undef EVTF
#define EVTF 032
void Evtf032_033_DisplayDamage(EvtData *evt) {
   // FIXME: maxHp, prevHp, curHp?
   EvtData *evt_s2, *evt_s3, *unitSprite;
   SVECTOR screenXY;
   s32 p;
   s32 flag;
   s16 savedX;
   s16 savedY;
   s16 savedDamage;
   s16 savedHpBefore;
   s16 savedHpAfter;

   evt_s2 = EVT.barFg;
   evt_s3 = EVT.barBg;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      RotTransPers(&unitSprite->vec, &screenXY, &p, &flag);
      screenXY.vy += 28;
      if (screenXY.vy > 0xc0) {
         screenXY.vy = 0xc0;
      }

      SaveRestorePos(&screenXY.vx, &screenXY.vy, 0);
      SaveRestorePos(&savedX, &savedY, 1);
      SaveRestoreDamage(&savedDamage, 1);

      if (savedDamage != 0) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_FLOATING_DAMAGE_TEXT;
         evt_s2->x1.s.hi = evt->x1.s.hi;
         evt_s2->z1.s.hi = evt->z1.s.hi;
         evt_s2->d.evtf051.damage = savedDamage;
         evt_s2->d.evtf051.clut = 4;
      }

      if (OBJ_MAP_UNIT(evt).s.team == TEAM_PLAYER) {
         DisplayBasicWindow(0x1b);
         DisplayBasicWindow(0x1c);
      } else {
         DisplayCustomWindow(0x1b, 0, 1, 0, 0, 23);
         DisplayCustomWindow(0x1c, 0, 1, 0, 0, 23);
      }

      SaveRestoreHpPercents(&savedHpBefore, &savedHpAfter, 1);

      evt_s2 = Evt_GetUnused();
      EVT.barFg = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->x1.n = savedX - 56;
      evt_s2->x3.n = savedX - 56 + (savedHpBefore * 80 / 100);
      evt_s2->y1.n = savedY - 4;
      evt_s2->y3.n = savedY + 4;
      evt_s2->d.sprite.gfxIdx = GFX_COLOR_5;
      evt_s2->d.sprite.clut = 3;
      evt_s2->d.sprite.otOfs = 3;

      evt_s3 = Evt_GetUnused();
      EVT.barBg = evt_s3;
      evt_s3->functionIndex = EVTF_NOOP;
      evt_s3->x1.n = savedX - 56;
      evt_s3->x3.n = savedX + 23;
      evt_s3->y1.n = savedY - 4;
      evt_s3->y3.n = savedY + 4;
      evt_s3->d.sprite.gfxIdx = GFX_COLOR_1;
      evt_s3->d.sprite.otOfs = 3;

      if (savedHpAfter < savedHpBefore) {
         //?
         s32 tmp = savedHpAfter;
         if ((tmp < 3) && (tmp % 2)) {
            savedHpAfter = 3;
         }
         EVT.barDstX3 = (savedX - 56) + (savedHpAfter * 80 / 100);
      } else {
         EVT.barDstX3 = (savedX - 54) + (savedHpAfter * 80 / 100);
      }

      //?
      SaveRestoreHp((s16 *)&EVT.maxHp, (s16 *)&EVT.prevHp, (s16 *)&EVT.curHp, 1);
      EVT.maxHp = (s16)EVT.maxHp;
      EVT.prevHp = (s16)EVT.prevHp << 0x10;
      EVT.curHp = (s16)EVT.curHp << 0x10;

      EVT.timer = 40;
      evt->state++;

   // fallthrough
   case 1:
      EVT.prevHp += (EVT.curHp - EVT.prevHp) >> 2;
      if (EVT.timer == 30) {
         EVT.prevHp = EVT.curHp;
      }

      IntToLeftPaddedGlyphs2(EVT.prevHp >> 0x10, &gGlyphStrip_50[1]);
      IntToLeftPaddedGlyphs2(EVT.maxHp, &gGlyphStrip_50[5]);
      DrawGlyphStripGroup(gGlyphStripGroups[28], GFX_TBD_685);

      evt_s2->x3.n += (EVT.barDstX3 - evt_s2->x3.n) >> 2;

      if (--EVT.timer == 0) {
         CloseWindow(0x1b);
         CloseWindow(0x1c);
         evt->functionIndex = EVTF_NULL;
         evt_s2->functionIndex = EVTF_NULL;
         evt_s3->functionIndex = EVTF_NULL;
         return;
      }
   }

   if (evt_s2->x3.n > evt_s3->x1.n) {
      // Bar fill (if visible)
      AddEvtPrim_Gui(gGraphicsPtr->ot, evt_s2);
   }
   // Bar background
   AddEvtPrim_Gui(gGraphicsPtr->ot, evt_s3);
}

#undef EVTF
#define EVTF 008
void Evtf008_BattlePortrait(EvtData *evt) {
   EvtData *window, *sprite;
   u8 gfx;
   s32 i;
   s16 portraitNum;

   sprite = EVT.sprite;
   window = EVT.window;

   gfx = EVT.flipped ? GFX_PORTRAIT_B_FACE : GFX_PORTRAIT_A_FACE;

   switch (evt->state) {
   case 0:
      window = &gEvtDataArray[0];
      for (i = 0; i < EVT_DATA_CT; i++) {
         if ((window->functionIndex == EVTF_WINDOW_TBD_004 ||
              window->functionIndex == EVTF_WINDOW_TBD_005) &&
             window->d.evtf004.windowId == EVT.windowId) {
            EVT.window = window;
            break;
         }
         window++;
      }
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = gfx;
      sprite->d.sprite.otOfs = 2;
      EVT.sprite = sprite;
      evt->state++;
      break;

   case 1:
      if (EVT.flipped) {
         sprite->x1.n = window->x1.n + 24;
         sprite->x3.n = window->x1.n - 23;
      } else {
         sprite->x1.n = window->x1.n - 23;
         sprite->x3.n = window->x1.n + 24;
      }
      sprite->y1.n = window->y1.n - 23;
      sprite->y3.n = window->y1.n + 24;

      for (portraitNum = 0; portraitNum <= 60; portraitNum++) {
         if (gState.portraitsToLoad[portraitNum] == EVT.portraitId) {
            break;
         }
      }
      if (portraitNum == 61) {
         portraitNum = 0;
      }

      sprite->d.sprite.clut = gPortraitClutIds[portraitNum];

      if (portraitNum < 25) {
         gGfxTPageIds[gfx] = gTPageIds[10];
      } else {
         if (portraitNum < 50) {
            gGfxTPageIds[gfx] = gTPageIds[7];
         } else {
            gGfxTPageIds[gfx] = gTPageIds[5];
         }
      }

      if (portraitNum >= 50) {
         portraitNum -= 50;
      }
      if (portraitNum >= 25) {
         portraitNum -= 25;
      }

      gGfxSubTextures[gfx].x = (portraitNum % 5) * 48;
      gGfxSubTextures[gfx].y = (portraitNum / 5) * 48;
      break;
   }

   AddEvtPrim_Gui(gGraphicsPtr->ot, sprite);
}

#undef EVTF
#define EVTF 413
void Evtf413_MsgBoxPortrait(EvtData *evt) {
   // evt->state3: blinkState
   EvtData *speakSprite, *blinkSprite, *faceSprite, *anchor;
   // This could use some fiddling
   u8 portrait_u8;
   s16 portrait_s16;
   s32 gfx;

   faceSprite = EVT.faceSprite;
   speakSprite = EVT.speakSprite;
   blinkSprite = EVT.blinkSprite;

   portrait_u8 = EVT.portrait.idx;

   switch (evt->state) {
   case 0:
      for (portrait_s16 = 0; portrait_s16 < 75; portrait_s16++) {
         if (gState.portraitsToLoad[portrait_s16] == EVT.portrait.id) {
            break;
         }
      }
      if (portrait_s16 == 75) {
         portrait_s16 = 0;
      }

      // Starts as s16 portrait ID to search for, and is replaced by the resulting one-byte index
      EVT.portrait.idx = portrait_s16;

      gfx = (!EVT.flipped) ? GFX_PORTRAIT_A_FACE : GFX_PORTRAIT_B_FACE;

      portrait_s16 = EVT.portrait.idx;
      portrait_u8 = EVT.portrait.idx;

      if (portrait_s16 < 25) {
         gGfxTPageIds[gfx++] = gTPageIds[10];
      } else {
         gGfxTPageIds[gfx++] = gTPageIds[7];
      }

      switch (portrait_s16 / 5) {
      case 0:
         gGfxTPageIds[gfx++] = gTPageIds[0];
         gGfxTPageIds[gfx++] = gTPageIds[0];
         gGfxTPageIds[gfx] = gTPageIds[0];
         break;
      case 1:
         gGfxTPageIds[gfx++] = gTPageIds[1];
         gGfxTPageIds[gfx++] = gTPageIds[1];
         gGfxTPageIds[gfx] = gTPageIds[1];
         break;
      case 2:
         gGfxTPageIds[gfx++] = gTPageIds[2];
         gGfxTPageIds[gfx++] = gTPageIds[2];
         gGfxTPageIds[gfx] = gTPageIds[2];
         break;
      case 3:
         gGfxTPageIds[gfx++] = gTPageIds[3];
         gGfxTPageIds[gfx++] = gTPageIds[3];
         gGfxTPageIds[gfx] = gTPageIds[3];
         break;
      case 4:
         gGfxTPageIds[gfx++] = gTPageIds[4];
         gGfxTPageIds[gfx++] = gTPageIds[4];
         gGfxTPageIds[gfx] = gTPageIds[4];
         break;
      case 5:
         gGfxTPageIds[gfx++] = gTPageIds[16];
         gGfxTPageIds[gfx++] = gTPageIds[16];
         gGfxTPageIds[gfx] = gTPageIds[16];
         break;
      case 6:
         gGfxTPageIds[gfx++] = gTPageIds[17];
         gGfxTPageIds[gfx++] = gTPageIds[17];
         gGfxTPageIds[gfx] = gTPageIds[17];
         break;
      case 7:
         gGfxTPageIds[gfx++] = gTPageIds[18];
         gGfxTPageIds[gfx++] = gTPageIds[18];
         gGfxTPageIds[gfx] = gTPageIds[18];
         break;
      case 8:
         gGfxTPageIds[gfx++] = gTPageIds[19];
         gGfxTPageIds[gfx++] = gTPageIds[19];
         gGfxTPageIds[gfx] = gTPageIds[19];
         break;
      case 9:
         gGfxTPageIds[gfx++] = gTPageIds[20];
         gGfxTPageIds[gfx++] = gTPageIds[20];
         gGfxTPageIds[gfx] = gTPageIds[20];
         break;
      }
      gfx -= 3;

      // Mouth overlays
      gGfxSubTextures[gfx + 1].x = (portrait_s16 % 5) * 48;
      gGfxSubTextures[gfx + 2].x = (portrait_s16 % 5) * 48 + 16;

      // Eye overlay
      gGfxSubTextures[gfx + 3].x = (portrait_s16 % 5) * 48 + 32;

      if (portrait_s16 >= 25) {
         portrait_s16 -= 25;
      }

      gGfxSubTextures[gfx].x = (portrait_s16 % 5) * 48;
      gGfxSubTextures[gfx].y = (portrait_s16 / 5) * 48;

      faceSprite = Evt_GetUnused();
      faceSprite->functionIndex = EVTF_NOOP;
      faceSprite->d.sprite.gfxIdx = gfx;
      faceSprite->d.sprite.otOfs = 3;
      faceSprite->d.sprite.clut = gPortraitClutIds[portrait_u8];
      EVT.faceSprite = faceSprite;

      speakSprite = Evt_GetUnused();
      speakSprite->functionIndex = EVTF_NOOP;
      speakSprite->d.sprite.gfxIdx = gfx + 1;
      speakSprite->d.sprite.clut = gPortraitClutIds[portrait_u8];
      speakSprite->d.sprite.otOfs = 2;
      EVT.speakSprite = speakSprite;

      blinkSprite = Evt_GetUnused();
      blinkSprite->functionIndex = EVTF_NOOP;
      blinkSprite->d.sprite.gfxIdx = gfx + 3;
      blinkSprite->d.sprite.clut = gPortraitClutIds[portrait_u8];
      blinkSprite->d.sprite.otOfs = 2;
      EVT.blinkSprite = blinkSprite;

      evt->state2 = 0; // (speaking state)
      evt->state3 = 0; // (blinking state)
      evt->state++;
      break;

   case 1:
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      faceSprite->functionIndex = EVTF_NULL;
      speakSprite->functionIndex = EVTF_NULL;
      blinkSprite->functionIndex = EVTF_NULL;
      return;
   }

   anchor = EVT.anchor;
   if (anchor != NULL) {
      evt->x1.n = anchor->x1.n + EVT.anchorOfsX;
      evt->y1.n = anchor->y1.n + EVT.anchorOfsY;
   }

   if (!EVT.flipped) {
      faceSprite->x1.n = evt->x1.n;
      faceSprite->x3.n = evt->x1.n + 47;
   } else {
      faceSprite->x1.n = evt->x1.n + 47;
      faceSprite->x3.n = evt->x1.n;
   }
   faceSprite->y1.n = evt->y1.n;
   faceSprite->y3.n = evt->y1.n + 47;

   if (!EVT.flipped) {
      speakSprite->x1.n = evt->x1.n + gPortraitOverlayOffsets[portrait_u8].s.speakX;
      speakSprite->x3.n = speakSprite->x1.n + 15;
   } else {
      speakSprite->x1.n = evt->x1.n + 46 - gPortraitOverlayOffsets[portrait_u8].s.speakX + 1;
      speakSprite->x3.n = speakSprite->x1.n - 15;
   }
   speakSprite->y1.n = evt->y1.n + gPortraitOverlayOffsets[portrait_u8].s.speakY;
   speakSprite->y3.n = speakSprite->y1.n + 16;

   if (!EVT.flipped) {
      blinkSprite->x1.n = evt->x1.n + gPortraitOverlayOffsets[portrait_u8].s.blinkX;
      blinkSprite->x3.n = blinkSprite->x1.n + 15;
   } else {
      blinkSprite->x1.n = evt->x1.n + 46 - gPortraitOverlayOffsets[portrait_u8].s.blinkX + 1;
      blinkSprite->x3.n = blinkSprite->x1.n - 15;
   }
   blinkSprite->y1.n = evt->y1.n + gPortraitOverlayOffsets[portrait_u8].s.blinkY;
   blinkSprite->y3.n = blinkSprite->y1.n + 16;

   if (gState.msgTextWaitTimer[EVT.flipped + 1] != 0) {
      gState.msgTextWaitTimer[EVT.flipped + 1]--;

      // Speaking animation
      switch (evt->state2) {
      case 0:
         gfx = (gState.vsyncMode != 2) ? 2 : 1;
         EVT.speakTimer = gfx;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.speakTimer == 0) {
            gfx = (gState.vsyncMode != 2) ? 6 : 3;
            EVT.speakTimer = gfx;
            evt->state2++;
         }
         break;

      case 2:
         speakSprite->d.sprite.hidden = 0;

         if (--EVT.speakTimer == 0) {
            speakSprite->d.sprite.gfxIdx++;
            gfx = (gState.vsyncMode != 2) ? 4 : 2;
            EVT.speakTimer = gfx;
            evt->state2++;
         }
         break;

      case 3:
         speakSprite->d.sprite.hidden = 0;

         if (--EVT.speakTimer == 0) {
            speakSprite->d.sprite.gfxIdx--;
            gfx = (gState.vsyncMode != 2) ? 4 : 2;
            EVT.speakTimer = gfx;
            evt->state2++;
         }
         break;

      case 4:
         speakSprite->d.sprite.hidden = 0;

         if (--EVT.speakTimer == 0) {
            gfx = (gState.vsyncMode != 2) ? 2 : 1;
            EVT.speakTimer = gfx;
            speakSprite->d.sprite.hidden = 1;
            evt->state2 -= 3;
         }
         break;
      }

   } else {
      speakSprite->d.sprite.hidden = 1;
   }

   // Blinking animation
   switch (evt->state3) {
   case 0:
      blinkSprite->d.sprite.hidden = 1;

      if (gState.vsyncMode == 2) {
         EVT.blinkTimer = rand() % 50 + 20;
         if (rand() == 0) {
            EVT.blinkTimer = 2;
         }
      } else {
         EVT.blinkTimer = rand() % 100 + 40;
         if (rand() == 0) {
            EVT.blinkTimer = 4;
         }
      }
      evt->state3++;
      break;

   case 1:
      if (--EVT.blinkTimer == 0) {
         blinkSprite->d.sprite.hidden = 0;

         if (gState.vsyncMode == 2) {
            EVT.blinkTimer = 5;
            if (rand() == 0) {
               //?: TINY chance to blink 4x as long?
               EVT.blinkTimer = 20;
            }
         } else {
            EVT.blinkTimer = 10;
            if (rand() == 0) {
               EVT.blinkTimer = 40;
            }
         }
         evt->state3++;
      }
      break;

   case 2:
      if (--EVT.blinkTimer == 0) {
         evt->state3 -= 2;
      }
      break;
   }

   AddEvtPrim_Gui(gGraphicsPtr->ot, faceSprite);
   AddEvtPrim_Gui(gGraphicsPtr->ot, speakSprite);
   AddEvtPrim_Gui(gGraphicsPtr->ot, blinkSprite);
}

void Evtf448_UnitPortraitWrapper(EvtData *evt) { Evtf447_UnitPortrait(evt); }

#undef EVTF
#define EVTF 447
void Evtf447_UnitPortrait(EvtData *evt) {
   EvtData *sprite, *window;
   s32 i;
   s16 portraitNum;

   if (gState.unitListPortraitId == 0) {
      return;
   }

   sprite = EVT.sprite;
   window = EVT.window;

   switch (evt->state) {
   case 0:
      window = &gEvtDataArray[0];
      for (i = 0; i < EVT_DATA_CT; i++) {
         if ((window->functionIndex == EVTF_WINDOW_TBD_004 ||
              window->functionIndex == EVTF_WINDOW_TBD_005) &&
             window->d.evtf004.windowId == EVT.windowId) {
            EVT.window = window;
            break;
         }
         window++;
      }
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_PORTRAIT_B_FACE;
      sprite->d.sprite.otOfs = 10;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      sprite->x1.n = window->x1.n + 24;
      sprite->x3.n = window->x1.n - 23;
      sprite->y1.n = window->y1.n - 23;
      sprite->y3.n = window->y1.n + 24;

      for (portraitNum = 0; portraitNum <= 60; portraitNum++) {
         if (gState.portraitsToLoad[portraitNum] == gState.unitListPortraitId) {
            break;
         }
      }
      if (portraitNum == 61) {
         portraitNum = 0;
      }

      sprite->d.sprite.clut = gPortraitClutIds[portraitNum];

      if (portraitNum < 25) {
         gGfxTPageIds[GFX_PORTRAIT_B_FACE] = gTPageIds[10];
      } else {
         if (portraitNum < 50) {
            gGfxTPageIds[GFX_PORTRAIT_B_FACE] = gTPageIds[7];
         } else {
            gGfxTPageIds[GFX_PORTRAIT_B_FACE] = gTPageIds[5];
         }
      }

      if (portraitNum >= 50) {
         portraitNum -= 50;
      }
      if (portraitNum >= 25) {
         portraitNum -= 25;
      }

      gGfxSubTextures[GFX_PORTRAIT_B_FACE].x = (portraitNum % 5) * 48;
      gGfxSubTextures[GFX_PORTRAIT_B_FACE].y = (portraitNum / 5) * 48;
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      sprite->functionIndex = EVTF_NULL;
      return;
   }

   AddEvtPrim_Gui(gGraphicsPtr->ot, sprite);
}

#undef EVTF
#define EVTF 575
void Evtf575_StatusPortrait(EvtData *evt) {
   s16 portraitNum;

   switch (evt->state) {
   case 0:
      EVT.gfxIdx = GFX_PORTRAIT_B_FACE;
      EVT.otOfs = 2;
      evt->state++;

   // fallthrough
   case 1:
      evt->x1.n = 29;
      evt->x3.n = 76;
      evt->y1.n = 65;
      evt->y3.n = 112;

      for (portraitNum = 0; portraitNum < 75; portraitNum++) {
         if (gState.portraitsToLoad[portraitNum] == EVT.portraitId) {
            break;
         }
      }
      if (portraitNum == 75) {
         portraitNum = 0;
      }

      EVT.clut = gPortraitClutIds[portraitNum];

      if (portraitNum < 25) {
         gGfxTPageIds[GFX_PORTRAIT_B_FACE] = gTPageIds[10];
      } else {
         gGfxTPageIds[GFX_PORTRAIT_B_FACE] = gTPageIds[7];
      }

      if (portraitNum >= 25) {
         portraitNum -= 25;
      }

      gGfxSubTextures[GFX_PORTRAIT_B_FACE].x = (portraitNum % 5) * 48;
      gGfxSubTextures[GFX_PORTRAIT_B_FACE].y = (portraitNum / 5) * 48;

      evt->state++;
      break;

   case 2:
      break;
   }

   AddEvtPrim_Gui(gGraphicsPtr->ot, evt);
}
