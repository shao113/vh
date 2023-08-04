#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

#undef EVTF
#define EVTF 204
void Evtf204_SummonCrest(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fxSprite1;
   EvtData *fxSprite2;
   s16 tmp; // halfSize, intensity
   POLY_FT4 *poly;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (evt->state) {
   case 0:
      if (EVT.unitSprite == NULL) {
         EVT.unitSprite = unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      } else {
         unitSprite = EVT.unitSprite;
      }
      unitSprite->d.sprite.hidden = 1;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      fxSprite1 = Evt_GetUnused();
      fxSprite1->d.sprite.gfxIdx = GFX_TBD_28;
      fxSprite1->d.sprite.clut = EVT.clut;
      fxSprite1->d.sprite.boxIdx = 7;

      tmp = rsin(EVT.timer * 32) * 0x2a / ONE;
      gQuad_800fe63c[0].vx = -tmp;
      gQuad_800fe63c[0].vy = -tmp;
      gQuad_800fe63c[1].vx = tmp;
      gQuad_800fe63c[1].vy = -tmp;
      gQuad_800fe63c[2].vx = -tmp;
      gQuad_800fe63c[2].vy = tmp;
      gQuad_800fe63c[3].vx = tmp;
      gQuad_800fe63c[3].vy = tmp;

      ApplyMaskEffect(496 << 2, 384, 64, 64, EVT.vramSrcX, EVT.vramSrcY, 0, EVT.timer % 64,
                      GFX_TBD_28, 0);
      fxSprite1->x1.n = unitSprite->x1.n;
      fxSprite1->y1.n = unitSprite->y1.n + 0x100;
      fxSprite1->z1.n = unitSprite->z1.n;
      AddEvtPrim6(gGraphicsPtr->ot, fxSprite1, 0);
      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 452 << 2, 400, 0, 0, GFX_TBD_25,
                      0);

      CopyEvtData(unitSprite, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_TBD_25;
      fxSprite1->d.sprite.clut = EVT.maskClut;
      fxSprite1->d.sprite.semiTrans = 2;

      if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      tmp = rsin(EVT.timer * 64) / 32;
      AddEvtPrim6(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, tmp, tmp, tmp);

      fxSprite2 = Evt_GetUnused();
      CopyEvtData(unitSprite, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      fxSprite2->functionIndex = EVTF_NULL;
      fxSprite1->functionIndex = EVTF_NULL;

      EVT.timer++;
      if (EVT.timer == 33) {
         evt->functionIndex = EVTF_NULL;
         unitSprite->d.sprite.hidden = 0;
         gSignal3 = 1;
      }

      break;
   }
}

#undef EVTF
#define EVTF 207
void Evtf207_SummonRedCrest(EvtData *evt) {
   EvtData *crest;

   crest = Evt_GetUnused();
   crest->functionIndex = EVTF_SUMMON_CREST;
   crest->x1.s.hi = evt->x1.s.hi;
   crest->z1.s.hi = evt->z1.s.hi;
   crest->d.evtf204.maskClut = 10;
   crest->d.evtf204.clut = 3;
   crest->d.evtf204.vramSrcX = 384 << 2;
   crest->d.evtf204.vramSrcY = 384;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 209
void Evtf209_SummonBlueCrest(EvtData *evt) {
   EvtData *crest;

   crest = Evt_GetUnused();
   crest->functionIndex = EVTF_SUMMON_CREST;
   crest->x1.s.hi = evt->x1.s.hi;
   crest->z1.s.hi = evt->z1.s.hi;
   crest->d.evtf204.maskClut = 10;
   crest->d.evtf204.clut = 4;
   crest->d.evtf204.vramSrcX = 384 << 2;
   crest->d.evtf204.vramSrcY = 384;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 210
void Evtf210_SummonGreenCrest(EvtData *evt) {
   EvtData *crest;

   crest = Evt_GetUnused();
   crest->functionIndex = EVTF_SUMMON_CREST;
   crest->x1.s.hi = evt->x1.s.hi;
   crest->z1.s.hi = evt->z1.s.hi;
   crest->d.evtf204.maskClut = 10;
   crest->d.evtf204.clut = 9;
   crest->d.evtf204.vramSrcX = 384 << 2;
   crest->d.evtf204.vramSrcY = 384;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 208
void Evtf208_SpellFx1_HolyLightning(EvtData *evt) {
   EvtData *evt_s1; // unitSprite, bolt
   EvtData *fxSprite1;
   EvtData *fxSprite2;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = evt_s1 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s1->x1.n;
      evt->y1.n = evt_s1->y1.n;
      evt->z1.n = evt_s1->z1.n;
      evt_s1->d.sprite.hidden = 1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 0;
      EVT.bolts[8] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 4;
      EVT.bolts[7] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 8;
      EVT.bolts[6] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 12;
      EVT.bolts[5] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 16;
      EVT.bolts[4] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 20;
      EVT.bolts[3] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 24;
      EVT.bolts[2] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 28;
      EVT.bolts[1] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_CASTING_BOLT;
      evt_s1->d.evtf212.parent = evt;
      evt_s1->d.evtf212.todo_x4c = 32;
      EVT.bolts[0] = evt_s1;

      EVT.timer = 0;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = EVT.unitSprite;
      GetUnitSpriteVramRect(evt_s1, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 432 << 2, 464, 0, EVT.timer % 64,
                      GFX_TBD_25, 1);

      fxSprite1 = Evt_GetUnused();
      CopyEvtData(evt_s1, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_TBD_25;
      fxSprite1->d.sprite.clut = 8;
      fxSprite1->d.sprite.semiTrans = 2;

      if (evt_s1->d.sprite.gfxIdx >= 21 && evt_s1->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, fxSprite1, 0);

      fxSprite2 = Evt_GetUnused();
      CopyEvtData(evt_s1, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      EVT.timer++;
      if (EVT.timer == 40) {
         gSignal3 = 1;
      } else if (EVT.timer == 45) {
         evt_s1->d.sprite.hidden = 0; // unitSprite
         evt->functionIndex = EVTF_NULL;
         evt_s1 = EVT.bolts[8];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[7];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[6];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[5];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[4];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[3];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[2];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[1];
         evt_s1->state2 = 99;
         evt_s1 = EVT.bolts[0];
         evt_s1->state2 = 99;
      }

      fxSprite2->functionIndex = EVTF_NULL;
      fxSprite1->functionIndex = EVTF_NULL;
      break;
   }
}

static s16 sLightningAnimData_800febe4[20] = {
    7, GFX_LIGHTNING_1, 1, GFX_LIGHTNING_2, 1, GFX_LIGHTNING_3, 1, GFX_LIGHTNING_4,
    1, GFX_LIGHTNING_5, 1, GFX_LIGHTNING_6, 1, GFX_LIGHTNING_7, 1, GFX_LIGHTNING_8,
    1, GFX_NULL,        1, GFX_NULL};

static s16 sSmokeAnimData_800fec0c[20] = {
    4, GFX_PUFF_1, 2, GFX_PUFF_2, 2, GFX_PUFF_3, 2, GFX_PUFF_4, 2, GFX_PUFF_6,
    2, GFX_PUFF_7, 2, GFX_PUFF_8, 2, GFX_PUFF_9, 2, GFX_NULL,   1, GFX_NULL};

static s16 sRunesAnimData_800fec34[48] = {
    4, GFX_RUNE_1,  1, GFX_RUNE_2,  1, GFX_RUNE_3, 1, GFX_RUNE_4,  1, GFX_RUNE_5,  1, GFX_RUNE_6,
    1, GFX_RUNE_7,  1, GFX_RUNE_8,  1, GFX_RUNE_9, 1, GFX_RUNE_10, 1, GFX_RUNE_11, 1, GFX_RUNE_12,
    1, GFX_RUNE_11, 1, GFX_RUNE_10, 1, GFX_RUNE_9, 1, GFX_RUNE_8,  1, GFX_RUNE_7,  1, GFX_RUNE_6,
    1, GFX_RUNE_5,  1, GFX_RUNE_4,  1, GFX_RUNE_3, 1, GFX_RUNE_2,  1, GFX_NULL,    1, GFX_NULL};

static s16 sRockAnimData_800fec94[12] = {4, GFX_ROCK_1, 8, GFX_ROCK_2, 8, GFX_ROCK_3,
                                         8, GFX_ROCK_4, 8, GFX_NULL,   1, GFX_NULL};

#undef EVTF
#define EVTF 212
void Evtf212_SpellFx_HolyLightning_CastingBolt(EvtData *evt) {
   EvtData *parent;
   EvtData *sprite;
   s16 halfHeight;

   switch (evt->state) {
   case 0:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n;
      evt->y1.n = parent->y1.n;
      evt->z1.n = parent->z1.n;
      EVT.todo_x24 = 0;
      EVT.todo_x26 = rand() % ANGLE_360_DEGREES;
      EVT.todo_x28 = rand() % 0x30 + 0x64;
      EVT.todo_x2a = rand() % 0xc0 + 0x40;
      EVT.todo_x2c = 0;
      EVT.todo_x2e = rand() % 0x40 + 0x20 + EVT.todo_x4c;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sLightningAnimData_800febe4;
      sprite->d.sprite.clut = 4;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      halfHeight = 4 + rsin(EVT.todo_x24) * 0x18 / ONE;

      sprite->x1.n = evt->x1.n + rcos(EVT.todo_x26) * 0xa0 / ONE;
      sprite->z1.n = evt->z1.n + rsin(EVT.todo_x26) * 0xa0 / ONE;
      sprite->y1.n = evt->y1.n + EVT.todo_x2a + halfHeight + EVT.todo_x2c;

      gQuad_800fe63c[0].vx = -3;
      gQuad_800fe63c[0].vy = -halfHeight;
      gQuad_800fe63c[1].vx = 3;
      gQuad_800fe63c[1].vy = -halfHeight;
      gQuad_800fe63c[2].vx = -3;
      gQuad_800fe63c[2].vy = halfHeight;
      gQuad_800fe63c[3].vx = 3;
      gQuad_800fe63c[3].vy = halfHeight;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      EVT.todo_x24 += EVT.todo_x28;
      EVT.todo_x2c += EVT.todo_x2e;

      switch (evt->state2) {
      case 0:
         if (EVT.todo_x2c >= 0x380) {
            evt->state = 0;
            sprite->functionIndex = EVTF_NULL;
         }
         break;
      case 99:
         if (EVT.todo_x2c >= 0x380) {
            evt->functionIndex = EVTF_NULL;
            sprite->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 197
void Evtf197_SpellFx1_RollingThunder(EvtData *evt) {
   EvtData *evt_s1;
   EvtData *fxSprite1;
   EvtData *fxSprite2;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (evt->state) {
   case 0:
      if (evt->mem == 0) {
         evt_s1 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      } else {
         evt_s1 = EVT.variant_0x24.unitSpriteParam;
         EVT.variant_0x24.unitSpriteParam = NULL;
      }

      EVT.unitSprite = evt_s1;
      evt->x1.n = evt_s1->x1.n;
      evt->y1.n = evt_s1->y1.n;
      evt->z1.n = evt_s1->z1.n;
      evt_s1->d.sprite.hidden = 1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[8] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[7] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[6] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[5] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[4] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[3] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[2] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[1] = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_CASTING_BOLT;
      evt_s1->d.evtf198.parent = evt;
      EVT.bolts[0] = evt_s1;

      EVT.variant_0x24.timer = 0;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = EVT.unitSprite;
      GetUnitSpriteVramRect(evt_s1, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 432 << 2, 464, 0,
                      EVT.variant_0x24.timer % 64, GFX_TBD_25, 1);

      fxSprite1 = Evt_GetUnused();
      CopyEvtData(evt_s1, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_TBD_25;
      fxSprite1->d.sprite.clut = 8;
      fxSprite1->d.sprite.semiTrans = 2;

      if (evt_s1->d.sprite.gfxIdx >= 21 && evt_s1->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, fxSprite1, 0);

      fxSprite2 = Evt_GetUnused();
      CopyEvtData(evt_s1, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      EVT.variant_0x24.timer++;
      if (EVT.variant_0x24.timer == 25) {
         gSignal3 = 1;
      } else if (EVT.variant_0x24.timer == 30) {
         evt_s1->d.sprite.hidden = 0; // unitSprite
         evt->functionIndex = EVTF_NULL;
         evt_s1 = EVT.bolts[8];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[7];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[6];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[5];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[4];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[3];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[2];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[1];
         evt_s1->state = 99;
         evt_s1 = EVT.bolts[0];
         evt_s1->state = 99;
      }

      fxSprite2->functionIndex = EVTF_NULL;
      fxSprite1->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 198
void Evtf198_SpellFx_RollingThunder_CastingBolt(EvtData *evt) {
   EvtData *parent;
   EvtData *sprite;
   s16 halfHeight;
   s16 a, b;

   switch (evt->state) {
   case 0:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n;
      evt->y1.n = parent->y1.n;
      evt->z1.n = parent->z1.n;
      EVT.todo_x24 = 0;
      EVT.todo_x26 = rand() % ANGLE_360_DEGREES;
      EVT.todo_x28 = rand() % 8 + 8;
      EVT.todo_x2a = rand() % 0xc0 + 0x40;
      EVT.todo_x2c = 0;
      EVT.todo_x2e = rand() % 0x180 + 0x80;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sLightningAnimData_800febe4;
      sprite->d.sprite.clut = 4;
      EVT.sprite = sprite;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;

      if (EVT.todo_x24 <= ANGLE_90_DEGREES) {
         a = EVT.todo_x26;
         halfHeight = 6 + 18 * rsin(EVT.todo_x24) / ONE;
         b = 16 + 0x300 * (ONE - rcos(EVT.todo_x2c)) / ONE;

         sprite->x1.n = evt->x1.n + (b * rcos(a) / ONE);
         sprite->z1.n = evt->z1.n + (b * rsin(a) / ONE);
         sprite->y1.n = evt->y1.n + EVT.todo_x2a + halfHeight;

         gQuad_800fe63c[0].vx = -4;
         gQuad_800fe63c[0].vy = -halfHeight;
         gQuad_800fe63c[1].vx = 4;
         gQuad_800fe63c[1].vy = -halfHeight;
         gQuad_800fe63c[2].vx = -4;
         gQuad_800fe63c[2].vy = halfHeight;
         gQuad_800fe63c[3].vx = 4;
         gQuad_800fe63c[3].vy = halfHeight;

         UpdateEvtAnimation(sprite);
         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

         EVT.todo_x24 += EVT.todo_x28;
         EVT.todo_x26 += EVT.todo_x2e;
         EVT.todo_x2c += 0x22;
      }

      break;

   case 99:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Evtf060_SpellFx1_Healing(EvtData *evt) {
   evt->functionIndex = EVTF_NULL;
   gSignal3 = 1;
}

#undef EVTF
#define EVTF 100
void Evtf100_SpellFx2_Healing(EvtData *evt) {
   EvtData *evt_s2; // unitSprite, dataStore
   EvtData *evt_s1;
   s32 i;
   s32 spriteX, spriteY, spriteW, spriteH;
   EvtData **pDataStoreAsEvts;

   switch (evt->state) {
   case 0:
      evt_s2 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s2->x1.n;
      evt->z1.n = evt_s2->z1.n;
      evt->y1.n = evt_s2->y1.n;
      EVT.unitSprite = evt_s2;
      evt_s2->d.sprite.hidden = 1;

      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s2;
      pDataStoreAsEvts = evt_s2->d.dataStore.evts;
      for (i = 0; i < 10; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_HEALING_SPARKLE;
         evt_s1->d.evtf101.parent = evt;
         pDataStoreAsEvts[i] = evt_s1;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = EVT.unitSprite;
      GetUnitSpriteVramRect(evt_s2, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 416 << 2, 384, 0, EVT.timer % 64,
                      GFX_TBD_25, 0);

      evt_s1 = Evt_GetUnused();
      CopyEvtData(evt_s2, evt_s1);
      evt_s1->d.sprite.hidden = 0;
      evt_s1->d.sprite.gfxIdx = GFX_TBD_25;
      evt_s1->d.sprite.clut = 4;
      evt_s1->d.sprite.semiTrans = 2;

      if (evt_s2->d.sprite.gfxIdx >= 21 && evt_s2->d.sprite.gfxIdx <= 30) {
         evt_s1->d.sprite.boxIdx = 1;
      } else {
         evt_s1->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);

      CopyEvtData(evt_s2, evt_s1);
      evt_s1->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s1, 0);
      evt_s1->functionIndex = EVTF_NULL;

      if (EVT.timer == 30) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_DISPLAY_DAMAGE_2;
         evt_s1->x1.s.hi = evt_s2->x1.s.hi;
         evt_s1->z1.s.hi = evt_s2->z1.s.hi;
      }

      EVT.timer++;

      if (EVT.timer == 61) {
         evt_s2->d.sprite.hidden = 0;
         evt_s2 = EVT.dataStore;
         pDataStoreAsEvts = evt_s2->d.dataStore.evts;
         for (i = 0; i < 10; i++) {
            evt_s1 = pDataStoreAsEvts[i];
            evt_s1->state = 99;
         }
         evt_s2->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef EVTF
#define EVTF 101
void Evtf101_HealingSparkle(EvtData *evt) {
   static s16 sparkleAnimData[20] = {
       5, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5,
       1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_NULL,      1, GFX_NULL};

   EvtData *parent;
   EvtData *sprite;
   s16 a;

   switch (evt->state) {
   case 0:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n;
      evt->y1.n = parent->y1.n;
      evt->z1.n = parent->z1.n;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sparkleAnimData;
      EVT.sprite = sprite;

      evt->state++;

   // fallthrough
   case 1:
      EVT.todo_x24 = 0;
      EVT.todo_x26 = rand() % ANGLE_360_DEGREES;
      EVT.todo_x28 = (rand() % 6 + 4) * 2;
      EVT.todo_x2a = rand() % 32 + 32;
      sprite = EVT.sprite;
      sprite->d.sprite.animInitialized = 0;

      evt->state++;

   // fallthrough
   case 2:
      sprite = EVT.sprite;
      a = EVT.todo_x26;
      sprite->x1.n = evt->x1.n + (rcos(a) >> 5);
      sprite->z1.n = evt->z1.n + (rsin(a) >> 5);
      sprite->y1.n = evt->y1.n + EVT.todo_x24;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      EVT.todo_x24 += EVT.todo_x28;
      EVT.todo_x26 += EVT.todo_x2a;

      if (EVT.todo_x24 >= 0x180) {
         evt->state = 1;
      }

      break;

   case 99:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 102
void Evtf102_227_SpellFx2_Poison(EvtData *evt) {
   EvtData *evt_s4; // unitSprite, dataStore
   EvtData *evt_s1;
   EvtData **pDataStoreAsEvts;
   s32 i;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (evt->state) {
   case 0:
      evt_s4 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s4->x1.n;
      evt->z1.n = evt_s4->z1.n;
      evt->y1.n = evt_s4->y1.n;
      EVT.unitSprite = evt_s4;
      evt_s4->d.sprite.hidden = 1;

      evt_s4 = Evt_GetUnused();
      evt_s4->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s4;
      pDataStoreAsEvts = evt_s4->d.dataStore.evts;
      for (i = 0; i < 12; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_POISON_BUBBLES;
         evt_s1->d.evtf103.parent = evt;
         pDataStoreAsEvts[i] = evt_s1;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s4 = EVT.unitSprite;
      GetUnitSpriteVramRect(evt_s4, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 400 << 2, 384, 0, EVT.timer % 64,
                      GFX_TBD_25, 0);

      evt_s1 = Evt_GetUnused();
      CopyEvtData(evt_s4, evt_s1);
      evt_s1->d.sprite.hidden = 0;
      evt_s1->d.sprite.gfxIdx = GFX_TBD_25;
      evt_s1->d.sprite.clut = 8;
      evt_s1->d.sprite.semiTrans = 2;

      if (evt_s4->d.sprite.gfxIdx >= 21 && evt_s4->d.sprite.gfxIdx <= 30) {
         evt_s1->d.sprite.boxIdx = 1;
      } else {
         evt_s1->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);

      CopyEvtData(evt_s4, evt_s1);
      evt_s1->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s1, 0);
      evt_s1->functionIndex = EVTF_NULL;

      if (EVT.timer == 60) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_ATTACK_INFO_MARKER;
         evt_s1->x1.s.hi = evt->x1.s.hi;
         evt_s1->z1.s.hi = evt->z1.s.hi;
         if (evt->functionIndex == EVTF_SPELL_FX2_POISON) {
            evt_s1->d.evtf052.type = 3;
            evt_s1->d.evtf052.clut = 3;
            evt_s1->functionIndex = EVTF_NULL;
         } else {
            evt_s1->d.evtf052.type = 0;
            evt_s1->d.evtf052.clut = 3;
         }
      }

      EVT.timer++;

      if (EVT.timer == 91) {
         evt_s4->d.sprite.hidden = 0;
         evt_s4 = EVT.dataStore;
         pDataStoreAsEvts = evt_s4->d.dataStore.evts;
         for (i = 0; i < 12; i++) {
            evt_s1 = pDataStoreAsEvts[i];
            evt_s1->state = 99;
         }
         evt_s4->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef EVTF
#define EVTF 103
void Evtf103_PoisonBubbles(EvtData *evt) {
   static s16 poisonAnimData[20] = {
       4, GFX_POISON_1, 2, GFX_POISON_2, 2, GFX_POISON_3, 2, GFX_POISON_4, 2, GFX_POISON_5,
       2, GFX_POISON_6, 2, GFX_POISON_7, 2, GFX_POISON_8, 2, GFX_NULL,     1, GFX_NULL};

   EvtData *parent;
   EvtData *sprite;
   s16 a;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      parent = EVT.parent;
      evt->x1.n = parent->x1.n;
      evt->y1.n = parent->y1.n;
      evt->z1.n = parent->z1.n;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.animData = poisonAnimData;
      EVT.sprite = sprite;

      evt->state++;

   // fallthrough
   case 1:
      EVT.todo_x24 = 0;
      EVT.todo_x26 = rand() % ANGLE_360_DEGREES;
      EVT.todo_x28 = (rand() % 4 + 4) * 2;
      EVT.todo_x2a = rand() % 12 + 12;
      sprite = EVT.sprite;
      sprite->d.sprite.animInitialized = 0;

      evt->state++;

   // fallthrough
   case 2:
      sprite = EVT.sprite;
      a = EVT.todo_x26;
      sprite->x1.n = evt->x1.n + (rcos(a) * 128 + EVT.todo_x2a * rsin(EVT.todo_x24 * 24)) / ONE;
      sprite->z1.n = evt->z1.n + (rsin(a) * 128 + EVT.todo_x2a * rcos(EVT.todo_x24 * 24)) / ONE;
      sprite->y1.n = evt->y1.n + EVT.todo_x24;

      UpdateEvtAnimation(sprite);
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x61, 0x61, 0x61);

      EVT.todo_x24 += EVT.todo_x28;

      if (EVT.todo_x24 >= 0x120) {
         evt->state = 1;
      }

      break;

   case 99:
      sprite = EVT.sprite;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 104
void Evtf104_SpellFx2_Cure(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_s1;
   POLY_FT4 *poly;
   s16 unaff_s6;
   s16 a, b;
   s32 i;
   EvtData **pDataStoreAsEvts;

   switch (evt->state) {
   case 0:
      evt_s2 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s2->x1.n;
      evt->z1.n = evt_s2->z1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_SWIRLY_RING;
      evt_s1->d.sprite.clut = 4;
      evt_s1->d.sprite.semiTrans = 4;
      evt_s1->d.sprite.otOfs = -1;
      EVT.ringSprite = evt_s1;

      evt->state++;
      evt->state2 = 1;

   // fallthrough
   case 1:

      switch (evt->state2) {
      case 1:
         unaff_s6 = 0xc0 * rsin(EVT.todo_x24) >> 12;
         break;
      case 2:
      case 3:
      case 4:
         unaff_s6 = 0xc0;
         break;
      case 5:
         unaff_s6 = 0xc0 * rsin(EVT.todo_x24) >> 12;
         break;
      }

      a = unaff_s6 * rsin(rsin(EVT.todo_x40)) >> 12;
      b = unaff_s6 * rcos(rsin(EVT.todo_x40)) >> 12;

      evt_s2 = EVT.ringSprite;
      evt_s2->d.sprite.coords[0].x = evt->x1.n + a;
      evt_s2->d.sprite.coords[0].z = evt->z1.n + b;
      evt_s2->d.sprite.coords[1].x = evt->x1.n - b;
      evt_s2->d.sprite.coords[1].z = evt->z1.n + a;
      evt_s2->d.sprite.coords[2].x = evt->x1.n + b;
      evt_s2->d.sprite.coords[2].z = evt->z1.n - a;
      evt_s2->d.sprite.coords[3].x = evt->x1.n - a;
      evt_s2->d.sprite.coords[3].z = evt->z1.n - b;
      evt_s2->d.sprite.coords[0].y = evt->y1.n;
      evt_s2->d.sprite.coords[1].y = evt->y1.n;
      evt_s2->d.sprite.coords[2].y = evt->y1.n;
      evt_s2->d.sprite.coords[3].y = evt->y1.n;
      evt_s2->y1.n = evt->y1.n;

      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      EVT.todo_x40 += 0x30;

      switch (evt->state2) {
      case 1:
         unaff_s6 = 0xc0 * rsin(EVT.todo_x24) >> 12;
         break;
      case 2:
         unaff_s6 = 0xc0;
         break;
      case 3:
         unaff_s6 = 0xc0 * rcos(EVT.todo_x26) >> 12;
         break;
      case 4:
      case 5:
         break;
      }

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_COLOR_15;
      evt_s1->d.sprite.semiTrans = 0;
      evt_s1->d.sprite.clut = 4;

      switch (evt->state2) {
      case 1:
      case 2:
      case 3:
         for (i = 0; i < 32; i++) {
            evt_s1->d.sprite.coords[0].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].y = evt_s1->d.sprite.coords[0].y;
            evt_s1->d.sprite.coords[2].x = evt_s1->d.sprite.coords[0].x;
            evt_s1->d.sprite.coords[2].z = evt_s1->d.sprite.coords[0].z;
            evt_s1->d.sprite.coords[2].y = evt->y1.n;
            evt_s1->d.sprite.coords[3].x = evt_s1->d.sprite.coords[1].x;
            evt_s1->d.sprite.coords[3].z = evt_s1->d.sprite.coords[1].z;
            evt_s1->d.sprite.coords[3].y = evt_s1->d.sprite.coords[2].y;

            evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[1].x +
                            evt_s1->d.sprite.coords[2].x + evt_s1->d.sprite.coords[3].x) /
                           4;
            evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[1].z +
                            evt_s1->d.sprite.coords[2].z + evt_s1->d.sprite.coords[3].z) /
                           4;
            evt_s1->y1.n = evt->y1.n;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      case 4:
      case 5:
         break;
      }

      switch (evt->state2) {
      case 1:
      case 2:
         break;
      case 3:
      case 4:
         unaff_s6 = 0xc0;
         break;
      case 5:
         unaff_s6 = 0xc0 * rsin(EVT.todo_x24) >> 12;
         break;
      }

      evt_s1->d.sprite.semiTrans = 1;

      switch (evt->state2) {
      case 1:
      case 2:
         break;
      case 3:
      case 4:
      case 5:
         for (i = 0; i < 32; i++) {
            evt_s1->d.sprite.coords[0].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].y = evt_s1->d.sprite.coords[0].y;
            evt_s1->d.sprite.coords[2].x = evt_s1->d.sprite.coords[0].x;
            evt_s1->d.sprite.coords[2].z = evt_s1->d.sprite.coords[0].z;
            evt_s1->d.sprite.coords[2].y = evt->y1.n;
            evt_s1->d.sprite.coords[3].x = evt_s1->d.sprite.coords[1].x;
            evt_s1->d.sprite.coords[3].z = evt_s1->d.sprite.coords[1].z;
            evt_s1->d.sprite.coords[3].y = evt_s1->d.sprite.coords[2].y;

            evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[1].x +
                            evt_s1->d.sprite.coords[2].x + evt_s1->d.sprite.coords[3].x) /
                           4;
            evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[1].z +
                            evt_s1->d.sprite.coords[2].z + evt_s1->d.sprite.coords[3].z) /
                           4;
            evt_s1->y1.n = evt->y1.n;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      }

      evt_s1->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 1:
         if (gLightColor.r > 0x20) {
            gLightColor.r += 0xfc;
            gLightColor.g += 0xfc;
            gLightColor.b += 0xfc;
         }
         EVT.todo_x24 += 0x2a;
         if (EVT.todo_x24 >= ANGLE_90_DEGREES) {
            evt->state2++;
            EVT.todo_x24 = 0;
         }
         break;

      case 2:
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_NOOP;
         EVT.dataStore = evt_s2;
         pDataStoreAsEvts = evt_s2->d.dataStore.evts;
         for (i = 0; i < 10; i++) {
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_HEALING_SPARKLE;
            evt_s1->d.evtf101.parent = evt;
            pDataStoreAsEvts[i] = evt_s1;
         }

         evt->state2++;

      // fallthrough
      case 3:
         EVT.todo_x26 += 24;
         if (EVT.todo_x26 >= ANGLE_90_DEGREES) {
            evt->state2++;
            EVT.todo_x24 = 0;
            EVT.todo_x26 = 0;
         }
         break;

      case 4:
         EVT.todo_x24 += 1;
         if (EVT.todo_x24 >= 0x5a) {
            evt->state2++;
            EVT.todo_x24 = ANGLE_90_DEGREES;
         }
         break;

      case 5:
         EVT.todo_x24 -= 42;
         if (gLightColor.r < 0x80) {
            gLightColor.r += 4;
            gLightColor.g += 4;
            gLightColor.b += 4;
         }
         if (unaff_s6 <= 0) {
            evt_s2 = EVT.dataStore;
            pDataStoreAsEvts = evt_s2->d.dataStore.evts;
            for (i = 0; i < 10; i++) {
               evt_s1 = pDataStoreAsEvts[i];
               evt_s1->state = 99;
            }
            evt_s2->functionIndex = EVTF_NULL;
            evt_s2 = EVT.ringSprite;
            evt_s2->functionIndex = EVTF_NULL;
            gLightColor.r = 0x80;
            gLightColor.g = 0x80;
            gLightColor.b = 0x80;
            gSignal3 = 1;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 106
void Evtf106_SpellFx3_MagicCharge(EvtData *evt) {
   // Beam of light encircled by glyphs; Possibly unused - referenced as FX3 for Magic Charge, but
   // FX3 should be unused for friendly spells (?)
   EvtData *evt_s0;
   EvtData *evt_s1;
   s16 unaff_s6, unaff_s7;
   s32 i;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_MAGIC_CHARGE_GLYPH_RING;
      evt_s1->x1.n = evt->x1.n;
      evt_s1->y1.n = evt->y1.n;
      evt_s1->z1.n = evt->z1.n;
      EVT.glyphRing = evt_s1;

      evt->state++;
      evt->state2 = 1;

   // fallthrough
   case 1:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_COLOR_15;

      switch (evt->state2) {
      case 1:
         unaff_s6 = 0xa0 * rsin(EVT.todo_x24) >> 12;
         unaff_s7 = 0;
         break;
      case 2:
         unaff_s6 = 0xa0;
         unaff_s7 = EVT.todo_x24;
         break;
      case 3:
      case 4:
         break;
      }

      switch (evt->state2) {
      case 1:
      case 2:
         for (i = 0; i < 32; i++) {
            evt_s1->d.sprite.coords[0].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400 - unaff_s7;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].y = evt_s1->d.sprite.coords[0].y;
            evt_s1->d.sprite.coords[2].x = evt_s1->d.sprite.coords[0].x;
            evt_s1->d.sprite.coords[2].z = evt_s1->d.sprite.coords[0].z;
            evt_s1->d.sprite.coords[2].y = evt->y1.n;
            evt_s1->d.sprite.coords[3].x = evt_s1->d.sprite.coords[1].x;
            evt_s1->d.sprite.coords[3].z = evt_s1->d.sprite.coords[1].z;
            evt_s1->d.sprite.coords[3].y = evt_s1->d.sprite.coords[2].y;

            evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[1].x +
                            evt_s1->d.sprite.coords[2].x + evt_s1->d.sprite.coords[3].x) /
                           4;
            evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[1].z +
                            evt_s1->d.sprite.coords[2].z + evt_s1->d.sprite.coords[3].z) /
                           4;
            evt_s1->y1.n = evt->y1.n;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      case 3:
      case 4:
         break;
      }

      switch (evt->state2) {
      case 1:
         break;
      case 2:
      case 3:
         unaff_s6 = 0xa0;
         break;
      case 4:
         unaff_s6 = 0xa0 * rsin(EVT.todo_x24) >> 12;
         break;
      }

      evt_s1->d.sprite.semiTrans = 1;

      switch (evt->state2) {
      case 1:
         break;
      case 2:
      case 3:
      case 4:
         for (i = 0; i < 32; i++) {
            evt_s1->d.sprite.coords[0].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            evt_s1->d.sprite.coords[1].y = evt_s1->d.sprite.coords[0].y;
            evt_s1->d.sprite.coords[2].x = evt_s1->d.sprite.coords[0].x;
            evt_s1->d.sprite.coords[2].z = evt_s1->d.sprite.coords[0].z;
            evt_s1->d.sprite.coords[2].y = evt->y1.n;
            evt_s1->d.sprite.coords[3].x = evt_s1->d.sprite.coords[1].x;
            evt_s1->d.sprite.coords[3].z = evt_s1->d.sprite.coords[1].z;
            evt_s1->d.sprite.coords[3].y = evt_s1->d.sprite.coords[2].y;

            evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[1].x +
                            evt_s1->d.sprite.coords[2].x + evt_s1->d.sprite.coords[3].x) /
                           4;
            evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[1].z +
                            evt_s1->d.sprite.coords[2].z + evt_s1->d.sprite.coords[3].z) /
                           4;
            evt_s1->y1.n = evt->y1.n;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      }

      evt_s1->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 1:
         EVT.todo_x24 += 32;
         if (EVT.todo_x24 >= 0x400) {
            evt->state2++;
            EVT.todo_x24 = 0;
         }
         break;

      case 2:
         EVT.todo_x26 += 24;
         EVT.todo_x24 = rsin(EVT.todo_x26) / 4;
         if (EVT.todo_x26 >= 0x400) {
            evt->state2++;
            EVT.todo_x24 = 0;
            EVT.todo_x26 = 0;
         }
         break;

      case 3:
         EVT.todo_x24++;
         if (EVT.todo_x24 >= 60) {
            evt->state2++;
            EVT.todo_x24 = 0x400;
         }
         break;

      case 4:
         EVT.todo_x24 -= 24;
         if (EVT.todo_x24 < 0) {
            evt_s0 = EVT.glyphRing;
            evt_s0->state2 = 99;
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 107
void Evtf107_SpellFx_MagicCharge_GlyphRing(EvtData *evt) {
   EvtData *sprite;
   s32 i;
   s16 a, b, c;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;

      a = EVT.todo_x58;
      b = 0x100;
      c = 0xaa;

      for (i = 0; i < 12; i++) {
         sprite->d.sprite.coords[0].x = evt->x1.n + b * rcos(a + i * 0x154) / ONE;
         sprite->d.sprite.coords[0].y = evt->y1.n;
         sprite->d.sprite.coords[0].z = evt->z1.n + b * rsin(a + i * 0x154) / ONE;

         sprite->d.sprite.coords[2].x = evt->x1.n + b * rcos(a + 0x154 + i * 0x154) / ONE;
         sprite->d.sprite.coords[2].y = evt->y1.n;
         sprite->d.sprite.coords[2].z = evt->z1.n + b * rsin(a + 0x154 + i * 0x154) / ONE;

         sprite->d.sprite.coords[1].x = evt->x1.n + c * rcos(a + i * 0x154) / ONE;
         sprite->d.sprite.coords[1].y = evt->y1.n;
         sprite->d.sprite.coords[1].z = evt->z1.n + c * rsin(a + i * 0x154) / ONE;

         sprite->d.sprite.coords[3].x = evt->x1.n + c * rcos(a + 0x154 + i * 0x154) / ONE;
         sprite->d.sprite.coords[3].y = evt->y1.n;
         sprite->d.sprite.coords[3].z = evt->z1.n + c * rsin(a + 0x154 + i * 0x154) / ONE;

         sprite->x1.n = (sprite->d.sprite.coords[1].x + sprite->d.sprite.coords[3].x) / 2;
         sprite->y1.n = (sprite->d.sprite.coords[1].y + sprite->d.sprite.coords[3].y) / 2;
         sprite->z1.n = (sprite->d.sprite.coords[1].z + sprite->d.sprite.coords[3].z) / 2;

         sprite->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);

         sprite->d.sprite.semiTrans = 2;
         sprite->d.sprite.clut = 3;
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

         sprite->d.sprite.semiTrans = 3;
         sprite->d.sprite.clut = 10;
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      }

      sprite->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         if (EVT.fadeTicker % 2 == 0) {
            EVT.fade += 8;
            if (EVT.fade >= 0x80) {
               EVT.fade = 0x80;
               EVT.fadeTicker++;
            }
         } else {
            EVT.fade -= 8;
            if (EVT.fade <= 0) {
               EVT.fade = 0;
               EVT.fadeTicker++;
            }
         }
         if (EVT.todo_x4c != 0 && EVT.fade == 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;

      case 99:
         EVT.fade -= 8;
         if (EVT.fade < 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 108
void Evtf108_SpellFx2_HarmfulWave(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s4;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;
   s32 i;
   s16 a, b, c, d;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;
      EVT.unitSprite = unitSprite;
      unitSprite->d.sprite.hidden = 1;

      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;

      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 416 << 2, 320, 0, EVT.timer % 64,
                      GFX_TBD_25, 0);

      evt_s4 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s4);
      evt_s4->d.sprite.hidden = 0;
      evt_s4->d.sprite.gfxIdx = GFX_TBD_25;
      evt_s4->d.sprite.clut = 8;
      evt_s4->d.sprite.semiTrans = 2;

      if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
         evt_s4->d.sprite.boxIdx = 1;
      } else {
         evt_s4->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt_s4, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x3f, 0x3f, 0x3f);

      CopyEvtData(unitSprite, evt_s4);
      evt_s4->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, evt_s4, 0);
      evt_s4->functionIndex = EVTF_NULL;

      if (EVT.timer <= 128) {
         if (EVT.timer % 4 == 0) {
            evt_s4 = Evt_GetUnused();
            evt_s4->functionIndex = EVTF_SPELL_FX_HARMFUL_WAVE_RING;
            evt_s4->d.evtf109.height = abs(rsin(EVT.timer * 16)) / 16 + 16;
            evt_s4->d.evtf109.unitSprite = unitSprite;
         }

         ApplyMaskEffect(452 << 2, 400, 32, 32, 432 << 2, 256, (EVT.timer * 2) % 64, 0, GFX_TBD_26,
                         0);

         evt_s4 = Evt_GetUnused();
         evt_s4->functionIndex = EVTF_NOOP;
         evt_s4->d.sprite.clut = 8;
         evt_s4->d.sprite.gfxIdx = GFX_TBD_26;
         evt_s4->d.sprite.semiTrans = 4;

         a = evt->y1.n + 16 + abs(rsin(EVT.timer * 16)) / 16;
         evt_s4->d.sprite.coords[0].y = a;
         evt_s4->d.sprite.coords[1].y = a;
         b = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
         evt_s4->d.sprite.coords[2].y = b;
         evt_s4->d.sprite.coords[3].y = b;
         evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[2].y) / 2;

         for (i = 0; i < 32; i++) {
            a = 0x100 * rcos(i * 0x80) >> 12;
            b = 0x100 * rsin(i * 0x80) >> 12;
            c = 0x100 * rcos((i + 1) * 0x80) >> 12;
            d = 0x100 * rsin((i + 1) * 0x80) >> 12;

            evt_s4->d.sprite.coords[0].x = evt->x1.n + a;
            evt_s4->d.sprite.coords[1].x = evt->x1.n + c;
            evt_s4->d.sprite.coords[2].x = evt->x1.n + a;
            evt_s4->d.sprite.coords[3].x = evt->x1.n + c;

            evt_s4->d.sprite.coords[0].z = evt->z1.n + b;
            evt_s4->d.sprite.coords[1].z = evt->z1.n + d;
            evt_s4->d.sprite.coords[2].z = evt->z1.n + b;
            evt_s4->d.sprite.coords[3].z = evt->z1.n + d;

            evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[1].x +
                            evt_s4->d.sprite.coords[2].x + evt_s4->d.sprite.coords[3].x) /
                           4;
            evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[1].z +
                            evt_s4->d.sprite.coords[2].z + evt_s4->d.sprite.coords[3].z) /
                           4;
            AddEvtPrim3(gGraphicsPtr->ot, evt_s4);

            a = 0xf0 * rcos(i * 0x80) >> 12;
            b = 0xf0 * rsin(i * 0x80) >> 12;
            c = 0xf0 * rcos((i + 1) * 0x80) >> 12;
            d = 0xf0 * rsin((i + 1) * 0x80) >> 12;

            evt_s4->d.sprite.coords[0].x = evt->x1.n + a;
            evt_s4->d.sprite.coords[1].x = evt->x1.n + c;
            evt_s4->d.sprite.coords[2].x = evt->x1.n + a;
            evt_s4->d.sprite.coords[3].x = evt->x1.n + c;

            evt_s4->d.sprite.coords[0].z = evt->z1.n + b;
            evt_s4->d.sprite.coords[1].z = evt->z1.n + d;
            evt_s4->d.sprite.coords[2].z = evt->z1.n + b;
            evt_s4->d.sprite.coords[3].z = evt->z1.n + d;

            evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[1].x +
                            evt_s4->d.sprite.coords[2].x + evt_s4->d.sprite.coords[3].x) /
                           4;
            evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[1].z +
                            evt_s4->d.sprite.coords[2].z + evt_s4->d.sprite.coords[3].z) /
                           4;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
         }

         evt_s4->functionIndex = EVTF_NULL;

         if (EVT.timer == 128) {
            evt_s4 = Evt_GetUnused();
            evt_s4->functionIndex = EVTF_DISPLAY_DAMAGE_2;
            evt_s4->x1.s.hi = evt->x1.s.hi;
            evt_s4->z1.s.hi = evt->z1.s.hi;
         }
      }

      EVT.timer++;
      if (EVT.timer == 159) {
         evt->state++;
         unitSprite = EVT.unitSprite;
         unitSprite->d.sprite.hidden = 0;
      }

      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef EVTF
#define EVTF 109
void Evtf109_SpellFx_HarmfulWave_Ring(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *sprite;
   s16 y, halfSize;

   switch (evt->state) {
   case 0:
      unitSprite = EVT.unitSprite;
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.clut = 8;
      sprite->d.sprite.gfxIdx = GFX_RING_TOP;
      sprite->d.sprite.semiTrans = 1;

      halfSize = rcos(EVT.timer) >> 4;
      y = unitSprite->y1.n + EVT.height;

      sprite->d.sprite.coords[0].y = y;
      sprite->d.sprite.coords[1].y = y;
      sprite->d.sprite.coords[2].y = y;
      sprite->d.sprite.coords[3].y = y;
      sprite->y1.n = y;

      sprite->d.sprite.coords[0].x = unitSprite->x1.n - halfSize;
      sprite->d.sprite.coords[1].x = unitSprite->x1.n + halfSize;
      sprite->d.sprite.coords[2].x = unitSprite->x1.n - halfSize;
      sprite->d.sprite.coords[3].x = unitSprite->x1.n + halfSize;

      sprite->d.sprite.coords[0].z = unitSprite->z1.n - halfSize;
      sprite->d.sprite.coords[1].z = unitSprite->z1.n - halfSize;
      sprite->d.sprite.coords[2].z = unitSprite->z1.n;
      sprite->d.sprite.coords[3].z = unitSprite->z1.n;

      sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x +
                      sprite->d.sprite.coords[2].x + sprite->d.sprite.coords[3].x) /
                     4;
      sprite->z1.n = sprite->d.sprite.coords[0].z;

      AddEvtPrim3(gGraphicsPtr->ot, sprite);

      sprite->d.sprite.coords[0].z = unitSprite->z1.n + halfSize;
      sprite->d.sprite.coords[1].z = unitSprite->z1.n + halfSize;
      sprite->d.sprite.coords[2].z = unitSprite->z1.n;
      sprite->d.sprite.coords[3].z = unitSprite->z1.n;

      sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x +
                      sprite->d.sprite.coords[2].x + sprite->d.sprite.coords[3].x) /
                     4;
      sprite->z1.n = sprite->d.sprite.coords[0].z;

      AddEvtPrim3(gGraphicsPtr->ot, sprite);

      sprite->functionIndex = EVTF_NULL;

      EVT.timer += 32;
      if (EVT.timer >= 928) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

void Evtf111_SpellFx2_BlessWeapon(EvtData *evt) {
   EvtData *summonBuff;

   summonBuff = Evt_GetUnused();
   summonBuff->functionIndex = EVTF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = evt->x1.s.hi;
   summonBuff->z1.s.hi = evt->z1.s.hi;
   summonBuff->d.evtf110.clut = 3;

   evt->functionIndex = EVTF_NULL;
}

void Evtf112_SpellFx2_MysticShield(EvtData *evt) {
   EvtData *summonBuff;

   summonBuff = Evt_GetUnused();
   summonBuff->functionIndex = EVTF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = evt->x1.s.hi;
   summonBuff->z1.s.hi = evt->z1.s.hi;
   summonBuff->d.evtf110.clut = 4;

   evt->functionIndex = EVTF_NULL;
}

void Evtf113_SpellFx2_MysticEnergy(EvtData *evt) {
   EvtData *summonBuff;

   summonBuff = Evt_GetUnused();
   summonBuff->functionIndex = EVTF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = evt->x1.s.hi;
   summonBuff->z1.s.hi = evt->z1.s.hi;
   summonBuff->d.evtf110.clut = 9;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 110
void Evtf110_CastingStatBuff(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s1;
   POLY_FT4 *poly;
   s32 i;
   s16 a, b, c, d, e;
   s16 x, y, z;
   s16 height;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_STAT_BUFF_FX;
      evt_s1->d.evtf681.unitSprite = unitSprite;
      evt_s1->d.evtf681.clut = EVT.clut;
      EVT.buffFx = evt_s1;

      EVT.todo_x24 = rand() % ANGLE_360_DEGREES;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.semiTrans = 1;
      evt_s1->d.sprite.clut = EVT.clut;

      a = 0x100;
      b = 0x180;
      c = EVT.todo_x24;
      x = evt->x1.n;
      y = evt->y1.n;
      z = evt->z1.n;

      for (i = 0; i < evt->state3; i++) {
         d = c + i * 0x154;
         e = c + 0x154 + i * 0x154;

         evt_s1->d.sprite.coords[3].y = y;
         evt_s1->d.sprite.coords[2].y = y;
         evt_s1->d.sprite.coords[1].y = y;
         evt_s1->d.sprite.coords[0].y = y;

         evt_s1->d.sprite.coords[0].z = z + b * rsin(d) / ONE;
         evt_s1->d.sprite.coords[1].z = z + a * rsin(d) / ONE;
         evt_s1->d.sprite.coords[2].z = z + b * rsin(e) / ONE;
         evt_s1->d.sprite.coords[3].z = z + a * rsin(e) / ONE;

         evt_s1->d.sprite.coords[0].x = x + b * rcos(d) / ONE;
         evt_s1->d.sprite.coords[1].x = x + a * rcos(d) / ONE;
         evt_s1->d.sprite.coords[2].x = x + b * rcos(e) / ONE;
         evt_s1->d.sprite.coords[3].x = x + a * rcos(e) / ONE;

         evt_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);
         AddEvtPrim4(gGraphicsPtr->ot, evt_s1);
      }

      i = evt->state3;

      evt_s1->d.sprite.coords[0].x = evt->x1.n + b * rcos(c + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[0].z = evt->z1.n + b * rsin(c + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[0].y = evt->y1.n;

      evt_s1->d.sprite.coords[2].x = evt->x1.n + b * rcos(c + 0x154 + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[2].z = evt->z1.n + b * rsin(c + 0x154 + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[2].y = evt->y1.n;

      evt_s1->d.sprite.coords[1].x = evt->x1.n + a * rcos(c + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[1].z = evt->z1.n + a * rsin(c + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[1].y = evt->y1.n;

      evt_s1->d.sprite.coords[3].x = evt->x1.n + a * rcos(c + 0x154 + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[3].z = evt->z1.n + a * rsin(c + 0x154 + i * 0x154) / ONE;
      evt_s1->d.sprite.coords[3].y = evt->y1.n;

      evt_s1->x1.n = (evt_s1->d.sprite.coords[1].x + evt_s1->d.sprite.coords[3].x) / 2;
      evt_s1->y1.n = (evt_s1->d.sprite.coords[1].y + evt_s1->d.sprite.coords[3].y) / 2;
      evt_s1->z1.n = (evt_s1->d.sprite.coords[1].z + evt_s1->d.sprite.coords[3].z) / 2;
      evt_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);

      AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.glyphsFade, EVT.glyphsFade, EVT.glyphsFade);

      evt_s1->functionIndex = EVTF_NULL;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_CREST;
      evt_s1->d.sprite.clut = EVT.clut;
      evt_s1->d.sprite.semiTrans = 2;

      b = 0x160 * rcos(c) / ONE;
      a = 0x160 * rsin(c) / ONE;
      evt_s1->d.sprite.coords[0].x = evt->x1.n - b;
      evt_s1->d.sprite.coords[0].z = evt->z1.n - a;
      evt_s1->d.sprite.coords[1].x = evt->x1.n + a;
      evt_s1->d.sprite.coords[1].z = evt->z1.n - b;
      evt_s1->d.sprite.coords[2].x = evt->x1.n - a;
      evt_s1->d.sprite.coords[2].z = evt->z1.n + b;
      evt_s1->d.sprite.coords[3].x = evt->x1.n + b;
      evt_s1->d.sprite.coords[3].z = evt->z1.n + a;
      evt_s1->d.sprite.coords[0].y = evt->y1.n;
      evt_s1->d.sprite.coords[1].y = evt->y1.n;
      evt_s1->d.sprite.coords[2].y = evt->y1.n;
      evt_s1->d.sprite.coords[3].y = evt->y1.n;

      evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[3].x) / 2;
      evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[3].z) / 2;
      evt_s1->y1.n = evt->y1.n;

      AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.crestFade, EVT.crestFade, EVT.crestFade);

      evt_s1->d.sprite.clut = 10;
      evt_s1->d.sprite.semiTrans = 3;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.crestFade, EVT.crestFade, EVT.crestFade);

      evt_s1->functionIndex = EVTF_NULL;

      EVT.crestFade += 3;
      EVT.glyphsFade += 0x20;
      if (EVT.glyphsFade == 0x80) {
         evt->state3++;
         EVT.glyphsFade = 0;
         if (evt->state3 == 12) {
            evt->state3 = 0;
            evt->state++;
            evt_s1 = EVT.buffFx;
            evt_s1->state++;
         }
      }

      break;

   case 2:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.semiTrans = 1;
      evt_s1->d.sprite.clut = EVT.clut;
      //@4a18
      c = EVT.todo_x24;

      switch (evt->state3) {
      case 0:
         a = 0x100;
         b = 0x100 + rcos(evt->state2) / 32;
         height = 0x280 * rsin(evt->state2) / ONE;
         break;
      case 1:
         a = 0x100;
         b = 0x100;
         height = 0x280 * rsin(evt->state2 * 8) / ONE;
         break;
      }

      for (i = 0; i < 12; i++) {
         evt_s1->d.sprite.coords[0].x = evt->x1.n + b * rcos(c + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[0].z = evt->z1.n + b * rsin(c + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[0].y = evt->y1.n + height;

         evt_s1->d.sprite.coords[2].x = evt->x1.n + b * rcos(c + 0x154 + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[2].z = evt->z1.n + b * rsin(c + 0x154 + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[2].y = evt->y1.n + height;

         evt_s1->d.sprite.coords[1].x = evt->x1.n + a * rcos(c + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[1].z = evt->z1.n + a * rsin(c + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[1].y = evt->y1.n;

         evt_s1->d.sprite.coords[3].x = evt->x1.n + a * rcos(c + 0x154 + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[3].z = evt->z1.n + a * rsin(c + 0x154 + i * 0x154) / ONE;
         evt_s1->d.sprite.coords[3].y = evt->y1.n;

         evt_s1->x1.n = (evt_s1->d.sprite.coords[1].x + evt_s1->d.sprite.coords[3].x) / 2;
         evt_s1->y1.n = evt_s1->d.sprite.coords[0].y;
         evt_s1->z1.n = (evt_s1->d.sprite.coords[1].z + evt_s1->d.sprite.coords[3].z) / 2;

         evt_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);
         AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
      }

      evt_s1->functionIndex = EVTF_NULL;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_CREST;
      evt_s1->d.sprite.clut = EVT.clut;

      b = 0x160 * rcos(c) / ONE;
      a = 0x160 * rsin(c) / ONE;
      evt_s1->d.sprite.coords[0].x = evt->x1.n - b;
      evt_s1->d.sprite.coords[0].z = evt->z1.n - a;
      evt_s1->d.sprite.coords[1].x = evt->x1.n + a;
      evt_s1->d.sprite.coords[1].z = evt->z1.n - b;
      evt_s1->d.sprite.coords[2].x = evt->x1.n - a;
      evt_s1->d.sprite.coords[2].z = evt->z1.n + b;
      evt_s1->d.sprite.coords[3].x = evt->x1.n + b;
      evt_s1->d.sprite.coords[3].z = evt->z1.n + a;
      evt_s1->d.sprite.coords[0].y = evt->y1.n;
      evt_s1->d.sprite.coords[1].y = evt->y1.n;
      evt_s1->d.sprite.coords[2].y = evt->y1.n;
      evt_s1->d.sprite.coords[3].y = evt->y1.n;

      evt_s1->x1.n = (evt_s1->d.sprite.coords[0].x + evt_s1->d.sprite.coords[3].x) / 2;
      evt_s1->z1.n = (evt_s1->d.sprite.coords[0].z + evt_s1->d.sprite.coords[3].z) / 2;
      evt_s1->y1.n = evt->y1.n;

      AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
      evt_s1->functionIndex = EVTF_NULL;

      switch (evt->state3) {
      case 0:
         EVT.todo_x24 += 0x20;
         evt->state2 += 0x10;
         if (evt->state2 == 0x400) {
            evt->state3++;
            evt->state2 = 0x80;
            EVT.crestFade = 0x80;
         }
         break;
      case 1:
         EVT.todo_x24 += 32;
         EVT.crestFade -= 8;
         evt->state2 -= 8;
         if (evt->state2 <= 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 681
void Evtf681_StatBuffFx(EvtData *evt) {
   MaskEffectPreset maskEffect;
   EvtData *unitSprite;
   EvtData *evt_v1;

   maskEffect.srcGfxIdx = GFX_TILED_DIAMONDS;
   maskEffect.dstGfxIdx = GFX_TBD_25;
   maskEffect.width = 0;
   maskEffect.height = evt->state3 % 0x40;
   maskEffect.semiTrans = 2;
   maskEffect.clut = EVT.clut;
   maskEffect.color.r = maskEffect.color.g = maskEffect.color.b = EVT.fade;

   switch (evt->state) {
   case 0:
      unitSprite = EVT.unitSprite;
      unitSprite->d.sprite.hidden = 1;
      evt->state2 = 0x80;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = EVT.unitSprite;
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;
      evt->state3++;

      ApplyMaskEffectPreset(unitSprite, &maskEffect);
      unitSprite->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, unitSprite, 0);
      unitSprite->d.sprite.hidden = 1;

      EVT.fade += 8;
      if (EVT.fade > 0x80) {
         EVT.fade = 0x80;
      }
      break;

   case 2:
      if (EVT.clut == 3 || EVT.clut == 4) {
         evt_v1 = CreatePositionedEvt(evt, EVTF_STAT_BUFF_ICON);
         evt_v1->d.evtf733.type = EVT.clut;
         evt_v1->d.evtf733.clut = EVT.clut;
      } else {
         evt_v1 = CreatePositionedEvt(evt, EVTF_STAT_BUFF_ICON);
         evt_v1->d.evtf733.type = 3;
         evt_v1->d.evtf733.clut = EVT.clut;
         evt_v1->mem = -20;
         evt_v1 = CreatePositionedEvt(evt, EVTF_STAT_BUFF_ICON);
         evt_v1->d.evtf733.type = 4;
         evt_v1->d.evtf733.clut = EVT.clut;
         evt_v1->mem = 20;
      }

      evt->state++;

   // fallthrough
   case 3:
      unitSprite = EVT.unitSprite;
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;

      maskEffect.color.r = maskEffect.color.g = maskEffect.color.b = evt->state2;
      ApplyMaskEffectPreset(unitSprite, &maskEffect);
      unitSprite->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, unitSprite, 0);
      unitSprite->d.sprite.hidden = 1;

      evt->state3++;

      switch (EVT.todo_x50) {
      case 0:
         EVT.todo_x28 += 0x10;
         if (EVT.todo_x28 > 0x400) {
            EVT.todo_x50++;
            EVT.todo_x28 = 0x400;
         }
         break;
      case 1:
         evt->state2 -= 8;
         if (evt->state2 < 0) {
            unitSprite = EVT.unitSprite;
            unitSprite->d.sprite.hidden = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 4: {
      // FIXME: Fake match (forced register)
      register EvtData *evt_tmp asm("v1");
      // EvtData *evt_tmp;

      unitSprite = EVT.unitSprite;
      evt_tmp = CreatePositionedEvt(unitSprite, EVTF_STAT_RAISED); //@5388
      if (EVT.clut == 4) {
         evt_tmp->d.evtf272.type = 2;
      } else {
         evt_tmp->d.evtf272.type = 1;
      }
      evt_tmp->mem = 0x30;
      evt_tmp->d.evtf272.unitSprite = unitSprite;
      evt->state2 = 0x30;
      evt->state++;
   }

   // fallthrough
   case 5:
      evt->state2--;
      if (evt->state2 <= 0) {
         if (EVT.clut == 9) {
            evt->state++;
         } else {
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
         }
      }
      break;

   case 6:
      unitSprite = EVT.unitSprite;
      evt_v1 = CreatePositionedEvt(unitSprite, EVTF_STAT_RAISED);
      evt_v1->d.evtf272.type = 2;
      evt_v1->mem = 0x30;
      evt_v1->d.evtf272.unitSprite = unitSprite;
      evt->state2 = 0x30;
      evt->state++;
      break;

   case 7:
      evt->state2--;
      if (evt->state2 <= 0) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 733
void Evtf733_StatBuffIcon(EvtData *evt) {
   EvtData *sprite;
   s32 i;
   s16 a, b;
   s16 iconVramX, iconVramY, iconWidth, iconHeight;
   POLY_FT4 *poly;

   FntPrint("rollw:%d\n", evt->state);

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      EVT.sprite = sprite;
      sprite->d.sprite.boxIdx = 7;
      evt->state2 = 0x80;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;

      a = rcos(rsin(EVT.todo_x28) * 4 + 0x200) >> 8;
      b = rsin(rsin(EVT.todo_x28) * 4 + 0x200) >> 8;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -b;
      gQuad_800fe63c[1].vx = b;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -b;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = b;
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i].vx += evt->mem;
      }

      sprite->d.sprite.semiTrans = 2;

      if (EVT.type == 3) {
         iconVramX = 496 << 2;
         iconVramY = 448;
         iconWidth = 32;
         iconHeight = 32;
      } else if (EVT.type == 4) {
         iconVramX = 504 << 2;
         iconVramY = 448;
         iconWidth = 32;
         iconHeight = 32;
      } else {
         iconVramX = 368 << 2;
         iconVramY = 256;
         iconWidth = 64;
         iconHeight = 64;
      }

      if (EVT.type == 3) {
         sprite->d.sprite.gfxIdx = GFX_TBD_26;
      } else if (EVT.type == 4) {
         sprite->d.sprite.gfxIdx = GFX_TBD_27;
      } else {
         sprite->d.sprite.gfxIdx = GFX_TBD_26;
      }

      ApplyMaskEffect(iconVramX, iconVramY, iconWidth, iconHeight, 384 << 2, 384, 0,
                      evt->state3 % 64, sprite->d.sprite.gfxIdx, 0);

      sprite->d.sprite.clut = EVT.clut;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n + 0x1a0;

      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, evt->state2, evt->state2, evt->state2);

      if (EVT.type == 3) {
         sprite->d.sprite.clut = 12;
      } else if (EVT.type == 4) {
         sprite->d.sprite.clut = 12;
      } else {
         sprite->d.sprite.clut = 6;
      }

      if (EVT.type == 3) {
         sprite->d.sprite.gfxIdx = GFX_SWORD;
      } else if (EVT.type == 4) {
         sprite->d.sprite.gfxIdx = GFX_SHIELD;
      } else {
         sprite->d.sprite.gfxIdx = GFX_COMPASS;
      }

      sprite->d.sprite.semiTrans = 0;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);

      evt->state3++;

      switch (EVT.todo_x50) {
      case 0:
         EVT.todo_x28 += 0x10;
         if (EVT.todo_x28 > 0x400) {
            EVT.todo_x50++;
            EVT.todo_x28 = 0x400;
         }
         break;
      case 1:
         evt->state2 -= 8;
         if (evt->state2 < 0) {
            sprite = EVT.sprite;
            sprite->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}
