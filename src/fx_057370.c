#include "common.h"
#include "object.h"
#include "graphics.h"
#include "field.h"
#include "battle.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

#undef OBJF
#define OBJF 204
void Objf204_SummonCrest(Object *obj) {
   Object *unitSprite;
   Object *fxSprite1;
   Object *fxSprite2;
   s16 tmp; // halfSize, intensity
   POLY_FT4 *poly;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (obj->state) {
   case 0:
      if (OBJ.unitSprite == NULL) {
         OBJ.unitSprite = unitSprite = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      } else {
         unitSprite = OBJ.unitSprite;
      }
      unitSprite->d.sprite.hidden = 1;
      obj->state++;

   // fallthrough
   case 1:
      unitSprite = OBJ.unitSprite;
      fxSprite1 = Obj_GetUnused();
      fxSprite1->d.sprite.gfxIdx = GFX_MASK_EFFECT_4;
      fxSprite1->d.sprite.clut = OBJ.clut;
      fxSprite1->d.sprite.boxIdx = 7;

      tmp = rsin(OBJ.timer * 32) * 0x2a / ONE;
      gQuad_800fe63c[0].vx = -tmp;
      gQuad_800fe63c[0].vy = -tmp;
      gQuad_800fe63c[1].vx = tmp;
      gQuad_800fe63c[1].vy = -tmp;
      gQuad_800fe63c[2].vx = -tmp;
      gQuad_800fe63c[2].vy = tmp;
      gQuad_800fe63c[3].vx = tmp;
      gQuad_800fe63c[3].vy = tmp;

      ApplyMaskEffect(496 << 2, 384, 64, 64, OBJ.vramSrcX, OBJ.vramSrcY, 0, OBJ.timer % 64,
                      GFX_MASK_EFFECT_4, 0);
      fxSprite1->x1.n = unitSprite->x1.n;
      fxSprite1->y1.n = unitSprite->y1.n + CV(1.0);
      fxSprite1->z1.n = unitSprite->z1.n;
      AddObjPrim6(gGraphicsPtr->ot, fxSprite1, 0);
      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 452 << 2, 400, 0, 0,
                      GFX_MASK_EFFECT_1, 0);

      CopyObject(unitSprite, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      fxSprite1->d.sprite.clut = OBJ.maskClut;
      fxSprite1->d.sprite.semiTrans = 2;

      if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      tmp = rsin(OBJ.timer * 64) / 32;
      AddObjPrim6(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, tmp, tmp, tmp);

      fxSprite2 = Obj_GetUnused();
      CopyObject(unitSprite, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      fxSprite2->functionIndex = OBJF_NULL;
      fxSprite1->functionIndex = OBJF_NULL;

      OBJ.timer++;
      if (OBJ.timer == 33) {
         obj->functionIndex = OBJF_NULL;
         unitSprite->d.sprite.hidden = 0;
         gSignal3 = 1;
      }

      break;
   }
}

#undef OBJF
#define OBJF 207
void Objf207_SummonRedCrest(Object *obj) {
   Object *crest;

   crest = Obj_GetUnused();
   crest->functionIndex = OBJF_SUMMON_CREST;
   crest->x1.s.hi = obj->x1.s.hi;
   crest->z1.s.hi = obj->z1.s.hi;
   crest->d.objf204.maskClut = CLUT_MASK;
   crest->d.objf204.clut = CLUT_REDS;
   crest->d.objf204.vramSrcX = 384 << 2;
   crest->d.objf204.vramSrcY = 384;

   obj->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 209
void Objf209_SummonBlueCrest(Object *obj) {
   Object *crest;

   crest = Obj_GetUnused();
   crest->functionIndex = OBJF_SUMMON_CREST;
   crest->x1.s.hi = obj->x1.s.hi;
   crest->z1.s.hi = obj->z1.s.hi;
   crest->d.objf204.maskClut = CLUT_MASK;
   crest->d.objf204.clut = CLUT_BLUES;
   crest->d.objf204.vramSrcX = 384 << 2;
   crest->d.objf204.vramSrcY = 384;

   obj->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 210
void Objf210_SummonGreenCrest(Object *obj) {
   Object *crest;

   crest = Obj_GetUnused();
   crest->functionIndex = OBJF_SUMMON_CREST;
   crest->x1.s.hi = obj->x1.s.hi;
   crest->z1.s.hi = obj->z1.s.hi;
   crest->d.objf204.maskClut = CLUT_MASK;
   crest->d.objf204.clut = CLUT_GREENS;
   crest->d.objf204.vramSrcX = 384 << 2;
   crest->d.objf204.vramSrcY = 384;

   obj->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 208
void Objf208_HolyLightning_FX1(Object *obj) {
   Object *obj_s1; // unitSprite, bolt
   Object *fxSprite1;
   Object *fxSprite2;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (obj->state) {
   case 0:
      OBJ.unitSprite = obj_s1 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = obj_s1->x1.n;
      obj->y1.n = obj_s1->y1.n;
      obj->z1.n = obj_s1->z1.n;
      obj_s1->d.sprite.hidden = 1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 0;
      OBJ.bolts[8] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 4;
      OBJ.bolts[7] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 8;
      OBJ.bolts[6] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 12;
      OBJ.bolts[5] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 16;
      OBJ.bolts[4] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 20;
      OBJ.bolts[3] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 24;
      OBJ.bolts[2] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 28;
      OBJ.bolts[1] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_HOLY_LIGHTNING_CASTING_BOLT;
      obj_s1->d.objf212.parent = obj;
      obj_s1->d.objf212.todo_x4c = 32;
      OBJ.bolts[0] = obj_s1;

      OBJ.timer = 0;
      obj->state++;

   // fallthrough
   case 1:
      obj_s1 = OBJ.unitSprite;
      GetUnitSpriteVramRect(obj_s1, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 432 << 2, 464, 0, OBJ.timer % 64,
                      GFX_MASK_EFFECT_1, 1);

      fxSprite1 = Obj_GetUnused();
      CopyObject(obj_s1, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      fxSprite1->d.sprite.clut = CLUT_PURPLES;
      fxSprite1->d.sprite.semiTrans = 2;

      if (obj_s1->d.sprite.gfxIdx >= 21 && obj_s1->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      AddObjPrim6(gGraphicsPtr->ot, fxSprite1, 0);

      fxSprite2 = Obj_GetUnused();
      CopyObject(obj_s1, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      OBJ.timer++;
      if (OBJ.timer == 40) {
         gSignal3 = 1;
      } else if (OBJ.timer == 45) {
         obj_s1->d.sprite.hidden = 0; // unitSprite
         obj->functionIndex = OBJF_NULL;
         obj_s1 = OBJ.bolts[8];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[7];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[6];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[5];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[4];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[3];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[2];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[1];
         obj_s1->state2 = 99;
         obj_s1 = OBJ.bolts[0];
         obj_s1->state2 = 99;
      }

      fxSprite2->functionIndex = OBJF_NULL;
      fxSprite1->functionIndex = OBJF_NULL;
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

#undef OBJF
#define OBJF 212
void Objf212_HolyLightning_CastingBolt(Object *obj) {
   Object *parent;
   Object *sprite;
   s16 halfHeight;

   switch (obj->state) {
   case 0:
      parent = OBJ.parent;
      obj->x1.n = parent->x1.n;
      obj->y1.n = parent->y1.n;
      obj->z1.n = parent->z1.n;
      OBJ.todo_x24 = 0;
      OBJ.todo_x26 = rand() % DEG(360);
      OBJ.todo_x28 = rand() % 0x30 + 0x64;
      OBJ.todo_x2a = rand() % 0xc0 + 0x40;
      OBJ.todo_x2c = 0;
      OBJ.todo_x2e = rand() % 0x40 + 0x20 + OBJ.todo_x4c;

      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.animData = sLightningAnimData_800febe4;
      sprite->d.sprite.clut = CLUT_BLUES;
      OBJ.sprite = sprite;
      obj->state++;

   // fallthrough
   case 1:
      sprite = OBJ.sprite;
      halfHeight = 4 + rsin(OBJ.todo_x24) * 0x18 / ONE;

      sprite->x1.n = obj->x1.n + CV(0.625) * rcos(OBJ.todo_x26) / ONE;
      sprite->z1.n = obj->z1.n + CV(0.625) * rsin(OBJ.todo_x26) / ONE;
      sprite->y1.n = obj->y1.n + OBJ.todo_x2a + halfHeight + OBJ.todo_x2c;

      gQuad_800fe63c[0].vx = -3;
      gQuad_800fe63c[0].vy = -halfHeight;
      gQuad_800fe63c[1].vx = 3;
      gQuad_800fe63c[1].vy = -halfHeight;
      gQuad_800fe63c[2].vx = -3;
      gQuad_800fe63c[2].vy = halfHeight;
      gQuad_800fe63c[3].vx = 3;
      gQuad_800fe63c[3].vy = halfHeight;

      UpdateObjAnimation(sprite);
      AddObjPrim6(gGraphicsPtr->ot, sprite, 0);

      OBJ.todo_x24 += OBJ.todo_x28;
      OBJ.todo_x2c += OBJ.todo_x2e;

      switch (obj->state2) {
      case 0:
         if (OBJ.todo_x2c >= CV(3.5)) {
            obj->state = 0;
            sprite->functionIndex = OBJF_NULL;
         }
         break;
      case 99:
         if (OBJ.todo_x2c >= CV(3.5)) {
            obj->functionIndex = OBJF_NULL;
            sprite->functionIndex = OBJF_NULL;
         }
         break;
      }

      break;
   }
}

#undef OBJF
#define OBJF 197
void Objf197_RollingThunder_FX1(Object *obj) {
   Object *obj_s1;
   Object *fxSprite1;
   Object *fxSprite2;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (obj->state) {
   case 0:
      if (obj->mem == 0) {
         obj_s1 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      } else {
         obj_s1 = OBJ.variant_0x24.unitSpriteParam;
         OBJ.variant_0x24.unitSpriteParam = NULL;
      }

      OBJ.unitSprite = obj_s1;
      obj->x1.n = obj_s1->x1.n;
      obj->y1.n = obj_s1->y1.n;
      obj->z1.n = obj_s1->z1.n;
      obj_s1->d.sprite.hidden = 1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[8] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[7] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[6] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[5] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[4] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[3] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[2] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[1] = obj_s1;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_ROLLING_THUNDER_CASTING_BOLT;
      obj_s1->d.objf198.parent = obj;
      OBJ.bolts[0] = obj_s1;

      OBJ.variant_0x24.timer = 0;
      obj->state++;

   // fallthrough
   case 1:
      obj_s1 = OBJ.unitSprite;
      GetUnitSpriteVramRect(obj_s1, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 432 << 2, 464, 0,
                      OBJ.variant_0x24.timer % 64, GFX_MASK_EFFECT_1, 1);

      fxSprite1 = Obj_GetUnused();
      CopyObject(obj_s1, fxSprite1);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite1->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      fxSprite1->d.sprite.clut = CLUT_PURPLES;
      fxSprite1->d.sprite.semiTrans = 2;

      if (obj_s1->d.sprite.gfxIdx >= 21 && obj_s1->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      AddObjPrim6(gGraphicsPtr->ot, fxSprite1, 0);

      fxSprite2 = Obj_GetUnused();
      CopyObject(obj_s1, fxSprite2);
      fxSprite2->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      OBJ.variant_0x24.timer++;
      if (OBJ.variant_0x24.timer == 25) {
         gSignal3 = 1;
      } else if (OBJ.variant_0x24.timer == 30) {
         obj_s1->d.sprite.hidden = 0; // unitSprite
         obj->functionIndex = OBJF_NULL;
         obj_s1 = OBJ.bolts[8];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[7];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[6];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[5];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[4];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[3];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[2];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[1];
         obj_s1->state = 99;
         obj_s1 = OBJ.bolts[0];
         obj_s1->state = 99;
      }

      fxSprite2->functionIndex = OBJF_NULL;
      fxSprite1->functionIndex = OBJF_NULL;
      break;
   }
}

#undef OBJF
#define OBJF 198
void Objf198_RollingThunder_CastingBolt(Object *obj) {
   Object *parent;
   Object *sprite;
   s16 halfHeight;
   s16 a, b;

   switch (obj->state) {
   case 0:
      parent = OBJ.parent;
      obj->x1.n = parent->x1.n;
      obj->y1.n = parent->y1.n;
      obj->z1.n = parent->z1.n;
      OBJ.todo_x24 = 0;
      OBJ.todo_x26 = rand() % DEG(360);
      OBJ.todo_x28 = rand() % 8 + 8;
      OBJ.todo_x2a = rand() % 0xc0 + 0x40;
      OBJ.todo_x2c = 0;
      OBJ.todo_x2e = rand() % 0x180 + 0x80;

      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.animData = sLightningAnimData_800febe4;
      sprite->d.sprite.clut = CLUT_BLUES;
      OBJ.sprite = sprite;
      obj->state++;

   // fallthrough
   case 1:
      sprite = OBJ.sprite;

      if (OBJ.todo_x24 <= DEG(90)) {
         a = OBJ.todo_x26;
         halfHeight = 6 + 18 * rsin(OBJ.todo_x24) / ONE;
         b = 16 + CV(3.0) * (ONE - rcos(OBJ.todo_x2c)) / ONE;

         sprite->x1.n = obj->x1.n + (b * rcos(a) / ONE);
         sprite->z1.n = obj->z1.n + (b * rsin(a) / ONE);
         sprite->y1.n = obj->y1.n + OBJ.todo_x2a + halfHeight;

         gQuad_800fe63c[0].vx = -4;
         gQuad_800fe63c[0].vy = -halfHeight;
         gQuad_800fe63c[1].vx = 4;
         gQuad_800fe63c[1].vy = -halfHeight;
         gQuad_800fe63c[2].vx = -4;
         gQuad_800fe63c[2].vy = halfHeight;
         gQuad_800fe63c[3].vx = 4;
         gQuad_800fe63c[3].vy = halfHeight;

         UpdateObjAnimation(sprite);
         AddObjPrim6(gGraphicsPtr->ot, sprite, 0);

         OBJ.todo_x24 += OBJ.todo_x28;
         OBJ.todo_x26 += OBJ.todo_x2e;
         OBJ.todo_x2c += 0x22;
      }

      break;

   case 99:
      sprite = OBJ.sprite;
      sprite->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      break;
   }
}

void Objf060_Healing_FX1(Object *obj) {
   obj->functionIndex = OBJF_NULL;
   gSignal3 = 1;
}

#undef OBJF
#define OBJF 100
void Objf100_Healing_FX2(Object *obj) {
   Object *obj_s2; // unitSprite, dataStore
   Object *obj_s1;
   s32 i;
   s32 spriteX, spriteY, spriteW, spriteH;
   Object **pDataStoreAsObjs;

   switch (obj->state) {
   case 0:
      obj_s2 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = obj_s2->x1.n;
      obj->z1.n = obj_s2->z1.n;
      obj->y1.n = obj_s2->y1.n;
      OBJ.unitSprite = obj_s2;
      obj_s2->d.sprite.hidden = 1;

      obj_s2 = Obj_GetUnused();
      obj_s2->functionIndex = OBJF_NOOP;
      OBJ.dataStore = obj_s2;
      pDataStoreAsObjs = obj_s2->d.dataStore.objs;
      for (i = 0; i < 10; i++) {
         obj_s1 = Obj_GetUnused();
         obj_s1->functionIndex = OBJF_HEALING_SPARKLE;
         obj_s1->d.objf101.parent = obj;
         pDataStoreAsObjs[i] = obj_s1;
      }

      obj->state++;

   // fallthrough
   case 1:
      obj_s2 = OBJ.unitSprite;
      GetUnitSpriteVramRect(obj_s2, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 416 << 2, 384, 0, OBJ.timer % 64,
                      GFX_MASK_EFFECT_1, 0);

      obj_s1 = Obj_GetUnused();
      CopyObject(obj_s2, obj_s1);
      obj_s1->d.sprite.hidden = 0;
      obj_s1->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      obj_s1->d.sprite.clut = CLUT_BLUES;
      obj_s1->d.sprite.semiTrans = 2;

      if (obj_s2->d.sprite.gfxIdx >= 21 && obj_s2->d.sprite.gfxIdx <= 30) {
         obj_s1->d.sprite.boxIdx = 1;
      } else {
         obj_s1->d.sprite.boxIdx = 0;
      }

      AddObjPrim6(gGraphicsPtr->ot, obj_s1, 0);

      CopyObject(obj_s2, obj_s1);
      obj_s1->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, obj_s1, 0);
      obj_s1->functionIndex = OBJF_NULL;

      if (OBJ.timer == 30) {
         obj_s1 = Obj_GetUnused();
         obj_s1->functionIndex = OBJF_DISPLAY_DAMAGE_2;
         obj_s1->x1.s.hi = obj_s2->x1.s.hi;
         obj_s1->z1.s.hi = obj_s2->z1.s.hi;
      }

      OBJ.timer++;

      if (OBJ.timer == 61) {
         obj_s2->d.sprite.hidden = 0;
         obj_s2 = OBJ.dataStore;
         pDataStoreAsObjs = obj_s2->d.dataStore.objs;
         for (i = 0; i < 10; i++) {
            obj_s1 = pDataStoreAsObjs[i];
            obj_s1->state = 99;
         }
         obj_s2->functionIndex = OBJF_NULL;
         obj->functionIndex = OBJF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef OBJF
#define OBJF 101
void Objf101_HealingSparkle(Object *obj) {
   static s16 sparkleAnimData[20] = {
       5, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5,
       1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_NULL,      1, GFX_NULL};

   Object *parent;
   Object *sprite;
   s16 a;

   switch (obj->state) {
   case 0:
      parent = OBJ.parent;
      obj->x1.n = parent->x1.n;
      obj->y1.n = parent->y1.n;
      obj->z1.n = parent->z1.n;

      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.animData = sparkleAnimData;
      OBJ.sprite = sprite;

      obj->state++;

   // fallthrough
   case 1:
      OBJ.todo_x24 = 0;
      OBJ.todo_x26 = rand() % DEG(360);
      OBJ.todo_x28 = (rand() % 6 + 4) * 2;
      OBJ.todo_x2a = rand() % 32 + 32;
      sprite = OBJ.sprite;
      sprite->d.sprite.animInitialized = 0;

      obj->state++;

   // fallthrough
   case 2:
      sprite = OBJ.sprite;
      a = OBJ.todo_x26;
      sprite->x1.n = obj->x1.n + (rcos(a) >> 5);
      sprite->z1.n = obj->z1.n + (rsin(a) >> 5);
      sprite->y1.n = obj->y1.n + OBJ.todo_x24;

      UpdateObjAnimation(sprite);
      AddObjPrim6(gGraphicsPtr->ot, sprite, 0);

      OBJ.todo_x24 += OBJ.todo_x28;
      OBJ.todo_x26 += OBJ.todo_x2a;

      if (OBJ.todo_x24 >= 0x180) {
         obj->state = 1;
      }

      break;

   case 99:
      sprite = OBJ.sprite;
      sprite->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      break;
   }
}

#undef OBJF
#define OBJF 102
void Objf102_227_Poison_FX2(Object *obj) {
   Object *obj_s4; // unitSprite, dataStore
   Object *obj_s1;
   Object **pDataStoreAsObjs;
   s32 i;
   s32 spriteX, spriteY, spriteW, spriteH;

   switch (obj->state) {
   case 0:
      obj_s4 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = obj_s4->x1.n;
      obj->z1.n = obj_s4->z1.n;
      obj->y1.n = obj_s4->y1.n;
      OBJ.unitSprite = obj_s4;
      obj_s4->d.sprite.hidden = 1;

      obj_s4 = Obj_GetUnused();
      obj_s4->functionIndex = OBJF_NOOP;
      OBJ.dataStore = obj_s4;
      pDataStoreAsObjs = obj_s4->d.dataStore.objs;
      for (i = 0; i < 12; i++) {
         obj_s1 = Obj_GetUnused();
         obj_s1->functionIndex = OBJF_POISON_BUBBLES;
         obj_s1->d.objf103.parent = obj;
         pDataStoreAsObjs[i] = obj_s1;
      }

      obj->state++;

   // fallthrough
   case 1:
      obj_s4 = OBJ.unitSprite;
      GetUnitSpriteVramRect(obj_s4, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 400 << 2, 384, 0, OBJ.timer % 64,
                      GFX_MASK_EFFECT_1, 0);

      obj_s1 = Obj_GetUnused();
      CopyObject(obj_s4, obj_s1);
      obj_s1->d.sprite.hidden = 0;
      obj_s1->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      obj_s1->d.sprite.clut = CLUT_PURPLES;
      obj_s1->d.sprite.semiTrans = 2;

      if (obj_s4->d.sprite.gfxIdx >= 21 && obj_s4->d.sprite.gfxIdx <= 30) {
         obj_s1->d.sprite.boxIdx = 1;
      } else {
         obj_s1->d.sprite.boxIdx = 0;
      }

      AddObjPrim6(gGraphicsPtr->ot, obj_s1, 0);

      CopyObject(obj_s4, obj_s1);
      obj_s1->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, obj_s1, 0);
      obj_s1->functionIndex = OBJF_NULL;

      if (OBJ.timer == 60) {
         obj_s1 = Obj_GetUnused();
         obj_s1->functionIndex = OBJF_ATTACK_INFO_MARKER;
         obj_s1->x1.s.hi = obj->x1.s.hi;
         obj_s1->z1.s.hi = obj->z1.s.hi;
         if (obj->functionIndex == OBJF_POISON_FX2) {
            obj_s1->d.objf052.type = ATK_MARKER_POISONED;
            obj_s1->d.objf052.clut = CLUT_REDS;
            obj_s1->functionIndex = OBJF_NULL;
         } else {
            obj_s1->d.objf052.type = ATK_MARKER_MISS;
            obj_s1->d.objf052.clut = CLUT_REDS;
         }
      }

      OBJ.timer++;

      if (OBJ.timer == 91) {
         obj_s4->d.sprite.hidden = 0;
         obj_s4 = OBJ.dataStore;
         pDataStoreAsObjs = obj_s4->d.dataStore.objs;
         for (i = 0; i < 12; i++) {
            obj_s1 = pDataStoreAsObjs[i];
            obj_s1->state = 99;
         }
         obj_s4->functionIndex = OBJF_NULL;
         obj->functionIndex = OBJF_NULL;
         gSignal3 = 1;
      }

      break;
   }
}

#undef OBJF
#define OBJF 103
void Objf103_Poison_Bubbles(Object *obj) {
   static s16 poisonAnimData[20] = {
       4, GFX_POISON_1, 2, GFX_POISON_2, 2, GFX_POISON_3, 2, GFX_POISON_4, 2, GFX_POISON_5,
       2, GFX_POISON_6, 2, GFX_POISON_7, 2, GFX_POISON_8, 2, GFX_NULL,     1, GFX_NULL};

   Object *parent;
   Object *sprite;
   s16 a;
   POLY_FT4 *poly;

   switch (obj->state) {
   case 0:
      parent = OBJ.parent;
      obj->x1.n = parent->x1.n;
      obj->y1.n = parent->y1.n;
      obj->z1.n = parent->z1.n;

      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.animData = poisonAnimData;
      OBJ.sprite = sprite;

      obj->state++;

   // fallthrough
   case 1:
      OBJ.todo_x24 = 0;
      OBJ.todo_x26 = rand() % DEG(360);
      OBJ.todo_x28 = (rand() % 4 + 4) * 2;
      OBJ.todo_x2a = rand() % 12 + 12;
      sprite = OBJ.sprite;
      sprite->d.sprite.animInitialized = 0;

      obj->state++;

   // fallthrough
   case 2:
      sprite = OBJ.sprite;
      a = OBJ.todo_x26;
      sprite->x1.n = obj->x1.n + (rcos(a) * 128 + OBJ.todo_x2a * rsin(OBJ.todo_x24 * 24)) / ONE;
      sprite->z1.n = obj->z1.n + (rsin(a) * 128 + OBJ.todo_x2a * rcos(OBJ.todo_x24 * 24)) / ONE;
      sprite->y1.n = obj->y1.n + OBJ.todo_x24;

      UpdateObjAnimation(sprite);
      AddObjPrim6(gGraphicsPtr->ot, sprite, 0);

      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x61, 0x61, 0x61);

      OBJ.todo_x24 += OBJ.todo_x28;

      if (OBJ.todo_x24 >= 0x120) {
         obj->state = 1;
      }

      break;

   case 99:
      sprite = OBJ.sprite;
      sprite->functionIndex = OBJF_NULL;
      obj->functionIndex = OBJF_NULL;
      break;
   }
}

#undef OBJF
#define OBJF 104
void Objf104_Cure_FX2(Object *obj) {
   Object *obj_s2;
   Object *obj_s1;
   POLY_FT4 *poly;
   s16 unaff_s6;
   s16 a, b;
   s32 i;
   Object **pDataStoreAsObjs;

   switch (obj->state) {
   case 0:
      obj_s2 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->y1.n = GetTerrainElevation(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = obj_s2->x1.n;
      obj->z1.n = obj_s2->z1.n;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.gfxIdx = GFX_SWIRLY_RING;
      obj_s1->d.sprite.clut = CLUT_BLUES;
      obj_s1->d.sprite.semiTrans = 4;
      obj_s1->d.sprite.otOfs = -1;
      OBJ.ringSprite = obj_s1;

      obj->state++;
      obj->state2 = 1;

   // fallthrough
   case 1:

      switch (obj->state2) {
      case 1:
         unaff_s6 = 0xc0 * rsin(OBJ.todo_x24) >> 12;
         break;
      case 2:
      case 3:
      case 4:
         unaff_s6 = 0xc0;
         break;
      case 5:
         unaff_s6 = 0xc0 * rsin(OBJ.todo_x24) >> 12;
         break;
      }

      a = unaff_s6 * rsin(rsin(OBJ.todo_x40)) >> 12;
      b = unaff_s6 * rcos(rsin(OBJ.todo_x40)) >> 12;

      obj_s2 = OBJ.ringSprite;
      obj_s2->d.sprite.coords[0].x = obj->x1.n + a;
      obj_s2->d.sprite.coords[0].z = obj->z1.n + b;
      obj_s2->d.sprite.coords[1].x = obj->x1.n - b;
      obj_s2->d.sprite.coords[1].z = obj->z1.n + a;
      obj_s2->d.sprite.coords[2].x = obj->x1.n + b;
      obj_s2->d.sprite.coords[2].z = obj->z1.n - a;
      obj_s2->d.sprite.coords[3].x = obj->x1.n - a;
      obj_s2->d.sprite.coords[3].z = obj->z1.n - b;
      obj_s2->d.sprite.coords[0].y = obj->y1.n;
      obj_s2->d.sprite.coords[1].y = obj->y1.n;
      obj_s2->d.sprite.coords[2].y = obj->y1.n;
      obj_s2->d.sprite.coords[3].y = obj->y1.n;
      obj_s2->y1.n = obj->y1.n;

      AddObjPrim4(gGraphicsPtr->ot, obj_s2);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      OBJ.todo_x40 += 0x30;

      switch (obj->state2) {
      case 1:
         unaff_s6 = CV(0.75) * rsin(OBJ.todo_x24) >> 12;
         break;
      case 2:
         unaff_s6 = CV(0.75);
         break;
      case 3:
         unaff_s6 = CV(0.75) * rcos(OBJ.todo_x26) >> 12;
         break;
      case 4:
      case 5:
         break;
      }

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.gfxIdx = GFX_COLOR_15;
      obj_s1->d.sprite.semiTrans = 0;
      obj_s1->d.sprite.clut = CLUT_BLUES;

      switch (obj->state2) {
      case 1:
      case 2:
      case 3:
         for (i = 0; i < 32; i++) {
            obj_s1->d.sprite.coords[0].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].y = obj->y1.n + CV(4.0);
            obj_s1->d.sprite.coords[1].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].y = obj_s1->d.sprite.coords[0].y;
            obj_s1->d.sprite.coords[2].x = obj_s1->d.sprite.coords[0].x;
            obj_s1->d.sprite.coords[2].z = obj_s1->d.sprite.coords[0].z;
            obj_s1->d.sprite.coords[2].y = obj->y1.n;
            obj_s1->d.sprite.coords[3].x = obj_s1->d.sprite.coords[1].x;
            obj_s1->d.sprite.coords[3].z = obj_s1->d.sprite.coords[1].z;
            obj_s1->d.sprite.coords[3].y = obj_s1->d.sprite.coords[2].y;

            obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[1].x +
                            obj_s1->d.sprite.coords[2].x + obj_s1->d.sprite.coords[3].x) /
                           4;
            obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[1].z +
                            obj_s1->d.sprite.coords[2].z + obj_s1->d.sprite.coords[3].z) /
                           4;
            obj_s1->y1.n = obj->y1.n;

            AddObjPrim3(gGraphicsPtr->ot, obj_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      case 4:
      case 5:
         break;
      }

      switch (obj->state2) {
      case 1:
      case 2:
         break;
      case 3:
      case 4:
         unaff_s6 = 0xc0;
         break;
      case 5:
         unaff_s6 = 0xc0 * rsin(OBJ.todo_x24) >> 12;
         break;
      }

      obj_s1->d.sprite.semiTrans = 1;

      switch (obj->state2) {
      case 1:
      case 2:
         break;
      case 3:
      case 4:
      case 5:
         for (i = 0; i < 32; i++) {
            obj_s1->d.sprite.coords[0].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].y = obj->y1.n + CV(4.0);
            obj_s1->d.sprite.coords[1].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].y = obj_s1->d.sprite.coords[0].y;
            obj_s1->d.sprite.coords[2].x = obj_s1->d.sprite.coords[0].x;
            obj_s1->d.sprite.coords[2].z = obj_s1->d.sprite.coords[0].z;
            obj_s1->d.sprite.coords[2].y = obj->y1.n;
            obj_s1->d.sprite.coords[3].x = obj_s1->d.sprite.coords[1].x;
            obj_s1->d.sprite.coords[3].z = obj_s1->d.sprite.coords[1].z;
            obj_s1->d.sprite.coords[3].y = obj_s1->d.sprite.coords[2].y;

            obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[1].x +
                            obj_s1->d.sprite.coords[2].x + obj_s1->d.sprite.coords[3].x) /
                           4;
            obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[1].z +
                            obj_s1->d.sprite.coords[2].z + obj_s1->d.sprite.coords[3].z) /
                           4;
            obj_s1->y1.n = obj->y1.n;

            AddObjPrim3(gGraphicsPtr->ot, obj_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      }

      obj_s1->functionIndex = OBJF_NULL;

      switch (obj->state2) {
      case 1:
         if (gLightColor.r > 0x20) {
            gLightColor.r += 0xfc;
            gLightColor.g += 0xfc;
            gLightColor.b += 0xfc;
         }
         OBJ.todo_x24 += 0x2a;
         if (OBJ.todo_x24 >= DEG(90)) {
            obj->state2++;
            OBJ.todo_x24 = 0;
         }
         break;

      case 2:
         obj_s2 = Obj_GetUnused();
         obj_s2->functionIndex = OBJF_NOOP;
         OBJ.dataStore = obj_s2;
         pDataStoreAsObjs = obj_s2->d.dataStore.objs;
         for (i = 0; i < 10; i++) {
            obj_s1 = Obj_GetUnused();
            obj_s1->functionIndex = OBJF_HEALING_SPARKLE;
            obj_s1->d.objf101.parent = obj;
            pDataStoreAsObjs[i] = obj_s1;
         }

         obj->state2++;

      // fallthrough
      case 3:
         OBJ.todo_x26 += 24;
         if (OBJ.todo_x26 >= DEG(90)) {
            obj->state2++;
            OBJ.todo_x24 = 0;
            OBJ.todo_x26 = 0;
         }
         break;

      case 4:
         OBJ.todo_x24 += 1;
         if (OBJ.todo_x24 >= 0x5a) {
            obj->state2++;
            OBJ.todo_x24 = DEG(90);
         }
         break;

      case 5:
         OBJ.todo_x24 -= 42;
         if (gLightColor.r < 0x80) {
            gLightColor.r += 4;
            gLightColor.g += 4;
            gLightColor.b += 4;
         }
         if (unaff_s6 <= 0) {
            obj_s2 = OBJ.dataStore;
            pDataStoreAsObjs = obj_s2->d.dataStore.objs;
            for (i = 0; i < 10; i++) {
               obj_s1 = pDataStoreAsObjs[i];
               obj_s1->state = 99;
            }
            obj_s2->functionIndex = OBJF_NULL;
            obj_s2 = OBJ.ringSprite;
            obj_s2->functionIndex = OBJF_NULL;
            gLightColor.r = 0x80;
            gLightColor.g = 0x80;
            gLightColor.b = 0x80;
            gSignal3 = 1;
            obj->functionIndex = OBJF_NULL;
         }
         break;
      }

      break;
   }
}

#undef OBJF
#define OBJF 106
void Objf106_MagicCharge_FX3(Object *obj) {
   // Beam of light encircled by glyphs; Possibly unused - referenced as FX3 for Magic Charge, but
   // FX3 should be unused for friendly spells (?)
   Object *obj_s0;
   Object *obj_s1;
   s16 unaff_s6, unaff_s7;
   s32 i;
   POLY_FT4 *poly;

   switch (obj->state) {
   case 0:
      obj_s0 = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->y1.n = GetTerrainElevation(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = obj_s0->x1.n;
      obj->z1.n = obj_s0->z1.n;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_MAGIC_CHARGE_GLYPH_RING;
      obj_s1->x1.n = obj->x1.n;
      obj_s1->y1.n = obj->y1.n;
      obj_s1->z1.n = obj->z1.n;
      OBJ.glyphRing = obj_s1;

      obj->state++;
      obj->state2 = 1;

   // fallthrough
   case 1:
      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.gfxIdx = GFX_COLOR_15;

      switch (obj->state2) {
      case 1:
         unaff_s6 = 0xa0 * rsin(OBJ.todo_x24) >> 12;
         unaff_s7 = 0;
         break;
      case 2:
         unaff_s6 = 0xa0;
         unaff_s7 = OBJ.todo_x24;
         break;
      case 3:
      case 4:
         break;
      }

      switch (obj->state2) {
      case 1:
      case 2:
         for (i = 0; i < 32; i++) {
            obj_s1->d.sprite.coords[0].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].y = obj->y1.n + CV(4.0) - unaff_s7;
            obj_s1->d.sprite.coords[1].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].y = obj_s1->d.sprite.coords[0].y;
            obj_s1->d.sprite.coords[2].x = obj_s1->d.sprite.coords[0].x;
            obj_s1->d.sprite.coords[2].z = obj_s1->d.sprite.coords[0].z;
            obj_s1->d.sprite.coords[2].y = obj->y1.n;
            obj_s1->d.sprite.coords[3].x = obj_s1->d.sprite.coords[1].x;
            obj_s1->d.sprite.coords[3].z = obj_s1->d.sprite.coords[1].z;
            obj_s1->d.sprite.coords[3].y = obj_s1->d.sprite.coords[2].y;

            obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[1].x +
                            obj_s1->d.sprite.coords[2].x + obj_s1->d.sprite.coords[3].x) /
                           4;
            obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[1].z +
                            obj_s1->d.sprite.coords[2].z + obj_s1->d.sprite.coords[3].z) /
                           4;
            obj_s1->y1.n = obj->y1.n;

            AddObjPrim3(gGraphicsPtr->ot, obj_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      case 3:
      case 4:
         break;
      }

      switch (obj->state2) {
      case 1:
         break;
      case 2:
      case 3:
         unaff_s6 = 0xa0;
         break;
      case 4:
         unaff_s6 = 0xa0 * rsin(OBJ.todo_x24) >> 12;
         break;
      }

      obj_s1->d.sprite.semiTrans = 1;

      switch (obj->state2) {
      case 1:
         break;
      case 2:
      case 3:
      case 4:
         for (i = 0; i < 32; i++) {
            obj_s1->d.sprite.coords[0].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80) >> 12);
            obj_s1->d.sprite.coords[0].y = obj->y1.n + CV(4.0);
            obj_s1->d.sprite.coords[1].x = obj->x1.n + (unaff_s6 * rsin(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].z = obj->z1.n + (unaff_s6 * rcos(i * 0x80 + 0x80) >> 12);
            obj_s1->d.sprite.coords[1].y = obj_s1->d.sprite.coords[0].y;
            obj_s1->d.sprite.coords[2].x = obj_s1->d.sprite.coords[0].x;
            obj_s1->d.sprite.coords[2].z = obj_s1->d.sprite.coords[0].z;
            obj_s1->d.sprite.coords[2].y = obj->y1.n;
            obj_s1->d.sprite.coords[3].x = obj_s1->d.sprite.coords[1].x;
            obj_s1->d.sprite.coords[3].z = obj_s1->d.sprite.coords[1].z;
            obj_s1->d.sprite.coords[3].y = obj_s1->d.sprite.coords[2].y;

            obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[1].x +
                            obj_s1->d.sprite.coords[2].x + obj_s1->d.sprite.coords[3].x) /
                           4;
            obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[1].z +
                            obj_s1->d.sprite.coords[2].z + obj_s1->d.sprite.coords[3].z) /
                           4;
            obj_s1->y1.n = obj->y1.n;

            AddObjPrim3(gGraphicsPtr->ot, obj_s1);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, unaff_s6 + 0x3f, unaff_s6 + 0x3f, unaff_s6 + 0x3f);
         }
         break;
      }

      obj_s1->functionIndex = OBJF_NULL;

      switch (obj->state2) {
      case 1:
         OBJ.todo_x24 += 32;
         if (OBJ.todo_x24 >= 0x400) {
            obj->state2++;
            OBJ.todo_x24 = 0;
         }
         break;

      case 2:
         OBJ.todo_x26 += 24;
         OBJ.todo_x24 = rsin(OBJ.todo_x26) / 4;
         if (OBJ.todo_x26 >= 0x400) {
            obj->state2++;
            OBJ.todo_x24 = 0;
            OBJ.todo_x26 = 0;
         }
         break;

      case 3:
         OBJ.todo_x24++;
         if (OBJ.todo_x24 >= 60) {
            obj->state2++;
            OBJ.todo_x24 = 0x400;
         }
         break;

      case 4:
         OBJ.todo_x24 -= 24;
         if (OBJ.todo_x24 < 0) {
            obj_s0 = OBJ.glyphRing;
            obj_s0->state2 = 99;
            obj->functionIndex = OBJF_NULL;
            gSignal3 = 1;
         }
         break;
      }

      break;
   }
}

#undef OBJF
#define OBJF 107
void Objf107_MagicCharge_GlyphRing(Object *obj) {
   Object *sprite;
   s32 i;
   s16 a, b, c;
   POLY_FT4 *poly;

   switch (obj->state) {
   case 0:
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;

      a = OBJ.todo_x58;
      b = 0x100;
      c = 0xaa;

      for (i = 0; i < 12; i++) {
         sprite->d.sprite.coords[0].x = obj->x1.n + b * rcos(a + i * 0x154) / ONE;
         sprite->d.sprite.coords[0].y = obj->y1.n;
         sprite->d.sprite.coords[0].z = obj->z1.n + b * rsin(a + i * 0x154) / ONE;

         sprite->d.sprite.coords[2].x = obj->x1.n + b * rcos(a + 0x154 + i * 0x154) / ONE;
         sprite->d.sprite.coords[2].y = obj->y1.n;
         sprite->d.sprite.coords[2].z = obj->z1.n + b * rsin(a + 0x154 + i * 0x154) / ONE;

         sprite->d.sprite.coords[1].x = obj->x1.n + c * rcos(a + i * 0x154) / ONE;
         sprite->d.sprite.coords[1].y = obj->y1.n;
         sprite->d.sprite.coords[1].z = obj->z1.n + c * rsin(a + i * 0x154) / ONE;

         sprite->d.sprite.coords[3].x = obj->x1.n + c * rcos(a + 0x154 + i * 0x154) / ONE;
         sprite->d.sprite.coords[3].y = obj->y1.n;
         sprite->d.sprite.coords[3].z = obj->z1.n + c * rsin(a + 0x154 + i * 0x154) / ONE;

         sprite->x1.n = (sprite->d.sprite.coords[1].x + sprite->d.sprite.coords[3].x) / 2;
         sprite->y1.n = (sprite->d.sprite.coords[1].y + sprite->d.sprite.coords[3].y) / 2;
         sprite->z1.n = (sprite->d.sprite.coords[1].z + sprite->d.sprite.coords[3].z) / 2;

         sprite->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);

         sprite->d.sprite.semiTrans = 2;
         sprite->d.sprite.clut = CLUT_REDS;
         AddObjPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, OBJ.fade, OBJ.fade, OBJ.fade);

         sprite->d.sprite.semiTrans = 3;
         sprite->d.sprite.clut = CLUT_MASK;
         AddObjPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, OBJ.fade, OBJ.fade, OBJ.fade);
      }

      sprite->functionIndex = OBJF_NULL;

      switch (obj->state2) {
      case 0:
         if (OBJ.fadeTicker % 2 == 0) {
            OBJ.fade += 8;
            if (OBJ.fade >= 0x80) {
               OBJ.fade = 0x80;
               OBJ.fadeTicker++;
            }
         } else {
            OBJ.fade -= 8;
            if (OBJ.fade <= 0) {
               OBJ.fade = 0;
               OBJ.fadeTicker++;
            }
         }
         if (OBJ.todo_x4c != 0 && OBJ.fade == 0) {
            obj->functionIndex = OBJF_NULL;
         }
         break;

      case 99:
         OBJ.fade -= 8;
         if (OBJ.fade < 0) {
            obj->functionIndex = OBJF_NULL;
         }
         break;
      }

      break;
   }
}

#undef OBJF
#define OBJF 108
void Objf108_HarmfulWave_FX2(Object *obj) {
   Object *unitSprite;
   Object *obj_s4;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;
   s32 i;
   s16 a, b, c, d;

   switch (obj->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = unitSprite->x1.n;
      obj->z1.n = unitSprite->z1.n;
      obj->y1.n = unitSprite->y1.n;
      OBJ.unitSprite = unitSprite;
      unitSprite->d.sprite.hidden = 1;

      obj->state++;

   // fallthrough
   case 1:
      unitSprite = OBJ.unitSprite;

      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 416 << 2, 320, 0, OBJ.timer % 64,
                      GFX_MASK_EFFECT_1, 0);

      obj_s4 = Obj_GetUnused();
      CopyObject(unitSprite, obj_s4);
      obj_s4->d.sprite.hidden = 0;
      obj_s4->d.sprite.gfxIdx = GFX_MASK_EFFECT_1;
      obj_s4->d.sprite.clut = CLUT_PURPLES;
      obj_s4->d.sprite.semiTrans = 2;

      if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
         obj_s4->d.sprite.boxIdx = 1;
      } else {
         obj_s4->d.sprite.boxIdx = 0;
      }

      AddObjPrim6(gGraphicsPtr->ot, obj_s4, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x3f, 0x3f, 0x3f);

      CopyObject(unitSprite, obj_s4);
      obj_s4->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, obj_s4, 0);
      obj_s4->functionIndex = OBJF_NULL;

      if (OBJ.timer <= 128) {
         if (OBJ.timer % 4 == 0) {
            obj_s4 = Obj_GetUnused();
            obj_s4->functionIndex = OBJF_HARMFUL_WAVE_RING;
            obj_s4->d.objf109.height = abs(rsin(OBJ.timer * 16)) / 16 + 16;
            obj_s4->d.objf109.unitSprite = unitSprite;
         }

         ApplyMaskEffect(452 << 2, 400, 32, 32, 432 << 2, 256, (OBJ.timer * 2) % 64, 0,
                         GFX_MASK_EFFECT_2, 0);

         obj_s4 = Obj_GetUnused();
         obj_s4->functionIndex = OBJF_NOOP;
         obj_s4->d.sprite.clut = CLUT_PURPLES;
         obj_s4->d.sprite.gfxIdx = GFX_MASK_EFFECT_2;
         obj_s4->d.sprite.semiTrans = 4;

         a = obj->y1.n + CV(0.0625) + abs(rsin(OBJ.timer * 16)) / 16;
         obj_s4->d.sprite.coords[0].y = a;
         obj_s4->d.sprite.coords[1].y = a;
         b = GetTerrainElevation(obj->z1.s.hi, obj->x1.s.hi);
         obj_s4->d.sprite.coords[2].y = b;
         obj_s4->d.sprite.coords[3].y = b;
         obj_s4->y1.n = (obj_s4->d.sprite.coords[0].y + obj_s4->d.sprite.coords[2].y) / 2;

         for (i = 0; i < 32; i++) {
            a = CV(1.0) * rcos(i * 0x80) >> 12;
            b = CV(1.0) * rsin(i * 0x80) >> 12;
            c = CV(1.0) * rcos((i + 1) * 0x80) >> 12;
            d = CV(1.0) * rsin((i + 1) * 0x80) >> 12;

            obj_s4->d.sprite.coords[0].x = obj->x1.n + a;
            obj_s4->d.sprite.coords[1].x = obj->x1.n + c;
            obj_s4->d.sprite.coords[2].x = obj->x1.n + a;
            obj_s4->d.sprite.coords[3].x = obj->x1.n + c;

            obj_s4->d.sprite.coords[0].z = obj->z1.n + b;
            obj_s4->d.sprite.coords[1].z = obj->z1.n + d;
            obj_s4->d.sprite.coords[2].z = obj->z1.n + b;
            obj_s4->d.sprite.coords[3].z = obj->z1.n + d;

            obj_s4->x1.n = (obj_s4->d.sprite.coords[0].x + obj_s4->d.sprite.coords[1].x +
                            obj_s4->d.sprite.coords[2].x + obj_s4->d.sprite.coords[3].x) /
                           4;
            obj_s4->z1.n = (obj_s4->d.sprite.coords[0].z + obj_s4->d.sprite.coords[1].z +
                            obj_s4->d.sprite.coords[2].z + obj_s4->d.sprite.coords[3].z) /
                           4;
            AddObjPrim3(gGraphicsPtr->ot, obj_s4);

            a = CV(0.9375) * rcos(i * 0x80) >> 12;
            b = CV(0.9375) * rsin(i * 0x80) >> 12;
            c = CV(0.9375) * rcos((i + 1) * 0x80) >> 12;
            d = CV(0.9375) * rsin((i + 1) * 0x80) >> 12;

            obj_s4->d.sprite.coords[0].x = obj->x1.n + a;
            obj_s4->d.sprite.coords[1].x = obj->x1.n + c;
            obj_s4->d.sprite.coords[2].x = obj->x1.n + a;
            obj_s4->d.sprite.coords[3].x = obj->x1.n + c;

            obj_s4->d.sprite.coords[0].z = obj->z1.n + b;
            obj_s4->d.sprite.coords[1].z = obj->z1.n + d;
            obj_s4->d.sprite.coords[2].z = obj->z1.n + b;
            obj_s4->d.sprite.coords[3].z = obj->z1.n + d;

            obj_s4->x1.n = (obj_s4->d.sprite.coords[0].x + obj_s4->d.sprite.coords[1].x +
                            obj_s4->d.sprite.coords[2].x + obj_s4->d.sprite.coords[3].x) /
                           4;
            obj_s4->z1.n = (obj_s4->d.sprite.coords[0].z + obj_s4->d.sprite.coords[1].z +
                            obj_s4->d.sprite.coords[2].z + obj_s4->d.sprite.coords[3].z) /
                           4;

            AddObjPrim3(gGraphicsPtr->ot, obj_s4);
         }

         obj_s4->functionIndex = OBJF_NULL;

         if (OBJ.timer == 128) {
            obj_s4 = Obj_GetUnused();
            obj_s4->functionIndex = OBJF_DISPLAY_DAMAGE_2;
            obj_s4->x1.s.hi = obj->x1.s.hi;
            obj_s4->z1.s.hi = obj->z1.s.hi;
         }
      }

      OBJ.timer++;
      if (OBJ.timer == 159) {
         obj->state++;
         unitSprite = OBJ.unitSprite;
         unitSprite->d.sprite.hidden = 0;
      }

      break;

   case 2:
      obj->functionIndex = OBJF_NULL;
      gSignal3 = 1;
      break;
   }
}

#undef OBJF
#define OBJF 109
void Objf109_HarmfulWave_Ring(Object *obj) {
   Object *unitSprite;
   Object *sprite;
   s16 y, halfSize;

   switch (obj->state) {
   case 0:
      unitSprite = OBJ.unitSprite;
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      sprite->d.sprite.clut = CLUT_PURPLES;
      sprite->d.sprite.gfxIdx = GFX_RING_TOP;
      sprite->d.sprite.semiTrans = 1;

      halfSize = rcos(OBJ.timer) >> 4;
      y = unitSprite->y1.n + OBJ.height;

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

      AddObjPrim3(gGraphicsPtr->ot, sprite);

      sprite->d.sprite.coords[0].z = unitSprite->z1.n + halfSize;
      sprite->d.sprite.coords[1].z = unitSprite->z1.n + halfSize;
      sprite->d.sprite.coords[2].z = unitSprite->z1.n;
      sprite->d.sprite.coords[3].z = unitSprite->z1.n;

      sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x +
                      sprite->d.sprite.coords[2].x + sprite->d.sprite.coords[3].x) /
                     4;
      sprite->z1.n = sprite->d.sprite.coords[0].z;

      AddObjPrim3(gGraphicsPtr->ot, sprite);

      sprite->functionIndex = OBJF_NULL;

      OBJ.timer += 32;
      if (OBJ.timer >= 928) {
         obj->functionIndex = OBJF_NULL;
      }

      break;
   }
}

void Objf111_BlessWeapon_FX2(Object *obj) {
   Object *summonBuff;

   summonBuff = Obj_GetUnused();
   summonBuff->functionIndex = OBJF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = obj->x1.s.hi;
   summonBuff->z1.s.hi = obj->z1.s.hi;
   summonBuff->d.objf110.clut = CLUT_REDS;

   obj->functionIndex = OBJF_NULL;
}

void Objf112_MysticShield_FX2(Object *obj) {
   Object *summonBuff;

   summonBuff = Obj_GetUnused();
   summonBuff->functionIndex = OBJF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = obj->x1.s.hi;
   summonBuff->z1.s.hi = obj->z1.s.hi;
   summonBuff->d.objf110.clut = CLUT_BLUES;

   obj->functionIndex = OBJF_NULL;
}

void Objf113_MysticEnergy_FX2(Object *obj) {
   Object *summonBuff;

   summonBuff = Obj_GetUnused();
   summonBuff->functionIndex = OBJF_CASTING_STAT_BUFF;
   summonBuff->x1.s.hi = obj->x1.s.hi;
   summonBuff->z1.s.hi = obj->z1.s.hi;
   summonBuff->d.objf110.clut = CLUT_GREENS;

   obj->functionIndex = OBJF_NULL;
}

#undef OBJF
#define OBJF 110
void Objf110_CastingStatBuff(Object *obj) {
   Object *unitSprite;
   Object *obj_s1;
   POLY_FT4 *poly;
   s32 i;
   s16 a, b, c, d, e;
   s16 x, y, z;
   s16 height;

   switch (obj->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(obj->z1.s.hi, obj->x1.s.hi);
      obj->y1.n = GetTerrainElevation(obj->z1.s.hi, obj->x1.s.hi);
      obj->x1.n = unitSprite->x1.n;
      obj->z1.n = unitSprite->z1.n;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_STAT_BUFF_FX;
      obj_s1->d.objf681.unitSprite = unitSprite;
      obj_s1->d.objf681.clut = OBJ.clut;
      OBJ.buffFx = obj_s1;

      OBJ.todo_x24 = rand() % DEG(360);
      obj->state++;

   // fallthrough
   case 1:
      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.semiTrans = 1;
      obj_s1->d.sprite.clut = OBJ.clut;

      a = 0x100;
      b = 0x180;
      c = OBJ.todo_x24;
      x = obj->x1.n;
      y = obj->y1.n;
      z = obj->z1.n;

      for (i = 0; i < obj->state3; i++) {
         d = c + i * 0x154;
         e = c + 0x154 + i * 0x154;

         obj_s1->d.sprite.coords[3].y = y;
         obj_s1->d.sprite.coords[2].y = y;
         obj_s1->d.sprite.coords[1].y = y;
         obj_s1->d.sprite.coords[0].y = y;

         obj_s1->d.sprite.coords[0].z = z + b * rsin(d) / ONE;
         obj_s1->d.sprite.coords[1].z = z + a * rsin(d) / ONE;
         obj_s1->d.sprite.coords[2].z = z + b * rsin(e) / ONE;
         obj_s1->d.sprite.coords[3].z = z + a * rsin(e) / ONE;

         obj_s1->d.sprite.coords[0].x = x + b * rcos(d) / ONE;
         obj_s1->d.sprite.coords[1].x = x + a * rcos(d) / ONE;
         obj_s1->d.sprite.coords[2].x = x + b * rcos(e) / ONE;
         obj_s1->d.sprite.coords[3].x = x + a * rcos(e) / ONE;

         obj_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);
         AddObjPrim4(gGraphicsPtr->ot, obj_s1);
      }

      i = obj->state3;

      obj_s1->d.sprite.coords[0].x = obj->x1.n + b * rcos(c + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[0].z = obj->z1.n + b * rsin(c + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[0].y = obj->y1.n;

      obj_s1->d.sprite.coords[2].x = obj->x1.n + b * rcos(c + 0x154 + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[2].z = obj->z1.n + b * rsin(c + 0x154 + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[2].y = obj->y1.n;

      obj_s1->d.sprite.coords[1].x = obj->x1.n + a * rcos(c + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[1].z = obj->z1.n + a * rsin(c + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[1].y = obj->y1.n;

      obj_s1->d.sprite.coords[3].x = obj->x1.n + a * rcos(c + 0x154 + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[3].z = obj->z1.n + a * rsin(c + 0x154 + i * 0x154) / ONE;
      obj_s1->d.sprite.coords[3].y = obj->y1.n;

      obj_s1->x1.n = (obj_s1->d.sprite.coords[1].x + obj_s1->d.sprite.coords[3].x) / 2;
      obj_s1->y1.n = (obj_s1->d.sprite.coords[1].y + obj_s1->d.sprite.coords[3].y) / 2;
      obj_s1->z1.n = (obj_s1->d.sprite.coords[1].z + obj_s1->d.sprite.coords[3].z) / 2;
      obj_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);

      AddObjPrim3(gGraphicsPtr->ot, obj_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, OBJ.glyphsFade, OBJ.glyphsFade, OBJ.glyphsFade);

      obj_s1->functionIndex = OBJF_NULL;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.gfxIdx = GFX_CREST;
      obj_s1->d.sprite.clut = OBJ.clut;
      obj_s1->d.sprite.semiTrans = 2;

      b = 0x160 * rcos(c) / ONE;
      a = 0x160 * rsin(c) / ONE;
      obj_s1->d.sprite.coords[0].x = obj->x1.n - b;
      obj_s1->d.sprite.coords[0].z = obj->z1.n - a;
      obj_s1->d.sprite.coords[1].x = obj->x1.n + a;
      obj_s1->d.sprite.coords[1].z = obj->z1.n - b;
      obj_s1->d.sprite.coords[2].x = obj->x1.n - a;
      obj_s1->d.sprite.coords[2].z = obj->z1.n + b;
      obj_s1->d.sprite.coords[3].x = obj->x1.n + b;
      obj_s1->d.sprite.coords[3].z = obj->z1.n + a;
      obj_s1->d.sprite.coords[0].y = obj->y1.n;
      obj_s1->d.sprite.coords[1].y = obj->y1.n;
      obj_s1->d.sprite.coords[2].y = obj->y1.n;
      obj_s1->d.sprite.coords[3].y = obj->y1.n;

      obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[3].x) / 2;
      obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[3].z) / 2;
      obj_s1->y1.n = obj->y1.n;

      AddObjPrim3(gGraphicsPtr->ot, obj_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, OBJ.crestFade, OBJ.crestFade, OBJ.crestFade);

      obj_s1->d.sprite.clut = CLUT_MASK;
      obj_s1->d.sprite.semiTrans = 3;
      AddObjPrim3(gGraphicsPtr->ot, obj_s1);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, OBJ.crestFade, OBJ.crestFade, OBJ.crestFade);

      obj_s1->functionIndex = OBJF_NULL;

      OBJ.crestFade += 3;
      OBJ.glyphsFade += 0x20;
      if (OBJ.glyphsFade == 0x80) {
         obj->state3++;
         OBJ.glyphsFade = 0;
         if (obj->state3 == 12) {
            obj->state3 = 0;
            obj->state++;
            obj_s1 = OBJ.buffFx;
            obj_s1->state++;
         }
      }

      break;

   case 2:
      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.semiTrans = 1;
      obj_s1->d.sprite.clut = OBJ.clut;
      //@4a18
      c = OBJ.todo_x24;

      switch (obj->state3) {
      case 0:
         a = 0x100;
         b = 0x100 + rcos(obj->state2) / 32;
         height = 0x280 * rsin(obj->state2) / ONE;
         break;
      case 1:
         a = 0x100;
         b = 0x100;
         height = 0x280 * rsin(obj->state2 * 8) / ONE;
         break;
      }

      for (i = 0; i < 12; i++) {
         obj_s1->d.sprite.coords[0].x = obj->x1.n + b * rcos(c + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[0].z = obj->z1.n + b * rsin(c + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[0].y = obj->y1.n + height;

         obj_s1->d.sprite.coords[2].x = obj->x1.n + b * rcos(c + 0x154 + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[2].z = obj->z1.n + b * rsin(c + 0x154 + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[2].y = obj->y1.n + height;

         obj_s1->d.sprite.coords[1].x = obj->x1.n + a * rcos(c + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[1].z = obj->z1.n + a * rsin(c + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[1].y = obj->y1.n;

         obj_s1->d.sprite.coords[3].x = obj->x1.n + a * rcos(c + 0x154 + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[3].z = obj->z1.n + a * rsin(c + 0x154 + i * 0x154) / ONE;
         obj_s1->d.sprite.coords[3].y = obj->y1.n;

         obj_s1->x1.n = (obj_s1->d.sprite.coords[1].x + obj_s1->d.sprite.coords[3].x) / 2;
         obj_s1->y1.n = obj_s1->d.sprite.coords[0].y;
         obj_s1->z1.n = (obj_s1->d.sprite.coords[1].z + obj_s1->d.sprite.coords[3].z) / 2;

         obj_s1->d.sprite.gfxIdx = GFX_RUNE_1 + (i % 12);
         AddObjPrim3(gGraphicsPtr->ot, obj_s1);
      }

      obj_s1->functionIndex = OBJF_NULL;

      obj_s1 = Obj_GetUnused();
      obj_s1->functionIndex = OBJF_NOOP;
      obj_s1->d.sprite.gfxIdx = GFX_CREST;
      obj_s1->d.sprite.clut = OBJ.clut;

      b = 0x160 * rcos(c) / ONE;
      a = 0x160 * rsin(c) / ONE;
      obj_s1->d.sprite.coords[0].x = obj->x1.n - b;
      obj_s1->d.sprite.coords[0].z = obj->z1.n - a;
      obj_s1->d.sprite.coords[1].x = obj->x1.n + a;
      obj_s1->d.sprite.coords[1].z = obj->z1.n - b;
      obj_s1->d.sprite.coords[2].x = obj->x1.n - a;
      obj_s1->d.sprite.coords[2].z = obj->z1.n + b;
      obj_s1->d.sprite.coords[3].x = obj->x1.n + b;
      obj_s1->d.sprite.coords[3].z = obj->z1.n + a;
      obj_s1->d.sprite.coords[0].y = obj->y1.n;
      obj_s1->d.sprite.coords[1].y = obj->y1.n;
      obj_s1->d.sprite.coords[2].y = obj->y1.n;
      obj_s1->d.sprite.coords[3].y = obj->y1.n;

      obj_s1->x1.n = (obj_s1->d.sprite.coords[0].x + obj_s1->d.sprite.coords[3].x) / 2;
      obj_s1->z1.n = (obj_s1->d.sprite.coords[0].z + obj_s1->d.sprite.coords[3].z) / 2;
      obj_s1->y1.n = obj->y1.n;

      AddObjPrim3(gGraphicsPtr->ot, obj_s1);
      obj_s1->functionIndex = OBJF_NULL;

      switch (obj->state3) {
      case 0:
         OBJ.todo_x24 += 0x20;
         obj->state2 += 0x10;
         if (obj->state2 == 0x400) {
            obj->state3++;
            obj->state2 = 0x80;
            OBJ.crestFade = 0x80;
         }
         break;
      case 1:
         OBJ.todo_x24 += 32;
         OBJ.crestFade -= 8;
         obj->state2 -= 8;
         if (obj->state2 <= 0) {
            obj->functionIndex = OBJF_NULL;
         }
         break;
      }

      break;
   }
}

#undef OBJF
#define OBJF 681
void Objf681_StatBuffFx(Object *obj) {
   MaskEffectPreset maskEffect;
   Object *unitSprite;
   Object *obj_v1;

   maskEffect.srcGfxIdx = GFX_TILED_DIAMONDS;
   maskEffect.dstGfxIdx = GFX_MASK_EFFECT_1;
   maskEffect.width = 0;
   maskEffect.height = obj->state3 % 0x40;
   maskEffect.semiTrans = 2;
   maskEffect.clut = OBJ.clut;
   maskEffect.color.r = maskEffect.color.g = maskEffect.color.b = OBJ.fade;

   switch (obj->state) {
   case 0:
      unitSprite = OBJ.unitSprite;
      unitSprite->d.sprite.hidden = 1;
      obj->state2 = 0x80;
      obj->state++;

   // fallthrough
   case 1:
      unitSprite = OBJ.unitSprite;
      obj->x1.n = unitSprite->x1.n;
      obj->z1.n = unitSprite->z1.n;
      obj->y1.n = unitSprite->y1.n;
      obj->state3++;

      RenderMaskEffect(unitSprite, &maskEffect);
      unitSprite->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, unitSprite, 0);
      unitSprite->d.sprite.hidden = 1;

      OBJ.fade += 8;
      if (OBJ.fade > 0x80) {
         OBJ.fade = 0x80;
      }
      break;

   case 2:
      if (OBJ.clut == CLUT_REDS || OBJ.clut == CLUT_BLUES) {
         obj_v1 = CreatePositionedObj(obj, OBJF_STAT_BUFF_ICON);
         obj_v1->d.objf733.type = OBJ.clut;
         obj_v1->d.objf733.clut = OBJ.clut;
      } else {
         obj_v1 = CreatePositionedObj(obj, OBJF_STAT_BUFF_ICON);
         obj_v1->d.objf733.type = 3;
         obj_v1->d.objf733.clut = OBJ.clut;
         obj_v1->mem = -20;
         obj_v1 = CreatePositionedObj(obj, OBJF_STAT_BUFF_ICON);
         obj_v1->d.objf733.type = 4;
         obj_v1->d.objf733.clut = OBJ.clut;
         obj_v1->mem = 20;
      }

      obj->state++;

   // fallthrough
   case 3:
      unitSprite = OBJ.unitSprite;
      obj->x1.n = unitSprite->x1.n;
      obj->z1.n = unitSprite->z1.n;
      obj->y1.n = unitSprite->y1.n;

      maskEffect.color.r = maskEffect.color.g = maskEffect.color.b = obj->state2;
      RenderMaskEffect(unitSprite, &maskEffect);
      unitSprite->d.sprite.hidden = 0;
      RenderUnitSprite(gGraphicsPtr->ot, unitSprite, 0);
      unitSprite->d.sprite.hidden = 1;

      obj->state3++;

      switch (OBJ.todo_x50) {
      case 0:
         OBJ.todo_x28 += 0x10;
         if (OBJ.todo_x28 > 0x400) {
            OBJ.todo_x50++;
            OBJ.todo_x28 = 0x400;
         }
         break;
      case 1:
         obj->state2 -= 8;
         if (obj->state2 < 0) {
            unitSprite = OBJ.unitSprite;
            unitSprite->d.sprite.hidden = 0;
            obj->state++;
         }
         break;
      }

      break;

   case 4: {
      // FIXME: Fake match (forced register)
      register Object *obj_tmp asm("v1");
      // Object *obj_tmp;

      unitSprite = OBJ.unitSprite;
      obj_tmp = CreatePositionedObj(unitSprite, OBJF_STAT_RAISED); //@5388
      if (OBJ.clut == CLUT_BLUES) {
         obj_tmp->d.objf272.type = RAISED_STAT_DF;
      } else {
         obj_tmp->d.objf272.type = RAISED_STAT_AT;
      }
      obj_tmp->mem = 0x30;
      obj_tmp->d.objf272.unitSprite = unitSprite;
      obj->state2 = 0x30;
      obj->state++;
   }

   // fallthrough
   case 5:
      obj->state2--;
      if (obj->state2 <= 0) {
         if (OBJ.clut == CLUT_GREENS) {
            obj->state++;
         } else {
            obj->functionIndex = OBJF_NULL;
            gSignal3 = 1;
         }
      }
      break;

   case 6:
      unitSprite = OBJ.unitSprite;
      obj_v1 = CreatePositionedObj(unitSprite, OBJF_STAT_RAISED);
      obj_v1->d.objf272.type = RAISED_STAT_DF;
      obj_v1->mem = 0x30;
      obj_v1->d.objf272.unitSprite = unitSprite;
      obj->state2 = 0x30;
      obj->state++;
      break;

   case 7:
      obj->state2--;
      if (obj->state2 <= 0) {
         obj->functionIndex = OBJF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef OBJF
#define OBJF 733
void Objf733_StatBuffIcon(Object *obj) {
   Object *sprite;
   s32 i;
   s16 a, b;
   s16 iconVramX, iconVramY, iconWidth, iconHeight;
   POLY_FT4 *poly;

   FntPrint("rollw:%d\n", obj->state);

   switch (obj->state) {
   case 0:
      sprite = Obj_GetUnused();
      sprite->functionIndex = OBJF_NOOP;
      OBJ.sprite = sprite;
      sprite->d.sprite.boxIdx = 7;
      obj->state2 = 0x80;
      obj->state++;

   // fallthrough
   case 1:
      sprite = OBJ.sprite;

      a = rcos(rsin(OBJ.todo_x28) * 4 + 0x200) >> 8;
      b = rsin(rsin(OBJ.todo_x28) * 4 + 0x200) >> 8;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -b;
      gQuad_800fe63c[1].vx = b;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -b;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = b;
      for (i = 0; i < 4; i++) {
         gQuad_800fe63c[i].vx += obj->mem;
      }

      sprite->d.sprite.semiTrans = 2;

      if (OBJ.type == 3) {
         iconVramX = 496 << 2;
         iconVramY = 448;
         iconWidth = 32;
         iconHeight = 32;
      } else if (OBJ.type == 4) {
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

      if (OBJ.type == 3) {
         sprite->d.sprite.gfxIdx = GFX_MASK_EFFECT_2;
      } else if (OBJ.type == 4) {
         sprite->d.sprite.gfxIdx = GFX_MASK_EFFECT_3;
      } else {
         sprite->d.sprite.gfxIdx = GFX_MASK_EFFECT_2;
      }

      ApplyMaskEffect(iconVramX, iconVramY, iconWidth, iconHeight, 384 << 2, 384, 0,
                      obj->state3 % 64, sprite->d.sprite.gfxIdx, 0);

      sprite->d.sprite.clut = OBJ.clut;
      sprite->x1.n = obj->x1.n;
      sprite->z1.n = obj->z1.n;
      sprite->y1.n = obj->y1.n + CV(1.625);

      AddObjPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, obj->state2, obj->state2, obj->state2);

      if (OBJ.type == 3) {
         sprite->d.sprite.clut = CLUT_12;
      } else if (OBJ.type == 4) {
         sprite->d.sprite.clut = CLUT_12;
      } else {
         sprite->d.sprite.clut = CLUT_6;
      }

      if (OBJ.type == 3) {
         sprite->d.sprite.gfxIdx = GFX_SWORD;
      } else if (OBJ.type == 4) {
         sprite->d.sprite.gfxIdx = GFX_SHIELD;
      } else {
         sprite->d.sprite.gfxIdx = GFX_COMPASS;
      }

      sprite->d.sprite.semiTrans = 0;
      AddObjPrim6(gGraphicsPtr->ot, sprite, 0);

      obj->state3++;

      switch (OBJ.todo_x50) {
      case 0:
         OBJ.todo_x28 += 0x10;
         if (OBJ.todo_x28 > 0x400) {
            OBJ.todo_x50++;
            OBJ.todo_x28 = 0x400;
         }
         break;
      case 1:
         obj->state2 -= 8;
         if (obj->state2 < 0) {
            sprite = OBJ.sprite;
            sprite->functionIndex = OBJF_NULL;
            obj->functionIndex = OBJF_NULL;
         }
         break;
      }

      break;
   }
}
