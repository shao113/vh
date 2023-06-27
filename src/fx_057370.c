#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "state.h"

//////////////////TODO//////////////////////////////////////////////////////////////////////////////
static const char TEMP_RDATA[] = "rollw:%d\n";
//////////////////TODO//////////////////////////////////////////////////////////////////////////////

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
         halfHeight = 6 + rsin(EVT.todo_x24) * 18 / ONE;
         b = 16 + (ANGLE_360_DEGREES - rcos(EVT.todo_x2c)) * 0x300 / ONE;

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
         unaff_s6 = (rsin(EVT.todo_x24) * 3) >> 6;
         break;
      case 2:
      case 3:
      case 4:
         unaff_s6 = 0xc0;
         break;
      case 5:
         unaff_s6 = (rsin(EVT.todo_x24) * 3) >> 6;
         break;
      }

      a = (unaff_s6 * rsin(rsin(EVT.todo_x40))) >> 12;
      b = (unaff_s6 * rcos(rsin(EVT.todo_x40))) >> 12;

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
         unaff_s6 = (rsin(EVT.todo_x24) * 3) >> 6;
         break;
      case 2:
         unaff_s6 = 0xc0;
         break;
      case 3:
         unaff_s6 = (rcos(EVT.todo_x26) * 3) >> 6;
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
            evt_s1->d.sprite.coords[0].x = evt->x1.n + ((unaff_s6 * rsin(i * 0x80)) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + ((unaff_s6 * rcos(i * 0x80)) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + ((unaff_s6 * rsin(i * 0x80 + 0x80)) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + ((unaff_s6 * rcos(i * 0x80 + 0x80)) >> 12);
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
         unaff_s6 = (rsin(EVT.todo_x24) * 3) >> 6;
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
            evt_s1->d.sprite.coords[0].x = evt->x1.n + ((unaff_s6 * rsin(i * 0x80)) >> 12);
            evt_s1->d.sprite.coords[0].z = evt->z1.n + ((unaff_s6 * rcos(i * 0x80)) >> 12);
            evt_s1->d.sprite.coords[0].y = evt->y1.n + 0x400;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + ((unaff_s6 * rsin(i * 0x80 + 0x80)) >> 12);
            evt_s1->d.sprite.coords[1].z = evt->z1.n + ((unaff_s6 * rcos(i * 0x80 + 0x80)) >> 12);
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
