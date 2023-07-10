#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

#undef EVTF
#define EVTF 172
void Evtf172_SpellFx_HolyPressure_Cube(EvtData *evt) {
   s16 local_88[6][4] = {{0, 256, 0, 0},    {-135, 0, -135, 0}, {135, 0, -135, 0},
                         {-135, 0, 135, 0}, {135, 0, 135, 0},   {0, -256, 0, 0}};
   s16 local_58[8][3] = {{0, 1, 2}, {0, 2, 3}, {0, 3, 4}, {0, 4, 1},
                         {5, 1, 2}, {5, 2, 3}, {5, 3, 4}, {5, 4, 1}};

   s32 i, j;
   s16 a;
   EvtData *evt_s4;

   FntPrint("8side:%d\n", evt->state);

   switch (evt->state) {
   case 0:
      evt_s4 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = evt_s4->y1.n + 0x80;
      evt->x1.n = evt_s4->x1.n;
      evt->z1.n = evt_s4->z1.n;
      evt->state++;

   // fallthrough
   case 1:
      evt_s4 = Evt_GetUnused();
      evt_s4->functionIndex = EVTF_NOOP;
      evt_s4->d.sprite.semiTrans = 4;
      evt_s4->d.sprite.clut = 8;
      evt_s4->d.sprite.gfxIdx = GFX_TILED_TBD_162;

      a = EVT.todo_x28;
      for (i = 0; i < 2; i++) {
         switch (evt->state3) {
         case 0:
            for (j = 1; j < 5; j++) {
               local_88[j][0] = 0x100 * rsin(i * 0x20 + a + (j - 1) * 0x400 + 0x200) >> 12;
               local_88[j][2] = 0x100 * rcos(i * 0x20 + a + (j - 1) * 0x400 + 0x200) >> 12;
               local_88[j][0] = local_88[j][0] * rsin(evt->state2) >> 12;
               local_88[j][2] = local_88[j][2] * rsin(evt->state2) >> 12;
            }
            local_88[0][1] = 0x100 - (0x100 * rcos(evt->state2) >> 12);
            local_88[5][1] = -local_88[0][1];
            break;

         case 1:
            for (j = 1; j < 5; j++) {
               local_88[j][0] = 0x100 * rsin(i * 0x20 + a + (j - 1) * 0x400 + 0x200) / ONE;
               local_88[j][2] = 0x100 * rcos(i * 0x20 + a + (j - 1) * 0x400 + 0x200) / ONE;
            }
            break;

         case 2:
            for (j = 1; j < 5; j++) {
               local_88[j][0] = 0x100 * rsin(i * 0x20 + a + (j - 1) * 0x400 + 0x200) >> 12;
               local_88[j][2] = 0x100 * rcos(i * 0x20 + a + (j - 1) * 0x400 + 0x200) >> 12;
               local_88[j][0] = local_88[j][0] * rcos(evt->state2) >> 12;
               local_88[j][2] = local_88[j][2] * rcos(evt->state2) >> 12;
            }
            local_88[0][1] = 0x100;
            local_88[5][1] = -0x100;
            break;
         }

         for (j = 0; j < 8; j++) {
            evt_s4->d.sprite.coords[0].x = local_88[local_58[j][0]][0] + evt->x1.n;
            evt_s4->d.sprite.coords[0].z = local_88[local_58[j][0]][2] + evt->z1.n;
            evt_s4->d.sprite.coords[0].y = local_88[local_58[j][0]][1] + evt->y1.n;
            evt_s4->d.sprite.coords[1].x = evt_s4->d.sprite.coords[0].x;
            evt_s4->d.sprite.coords[1].z = evt_s4->d.sprite.coords[0].z;
            evt_s4->d.sprite.coords[1].y = evt_s4->d.sprite.coords[0].y;
            evt_s4->d.sprite.coords[2].x = local_88[local_58[j][1]][0] + evt->x1.n;
            evt_s4->d.sprite.coords[2].z = local_88[local_58[j][1]][2] + evt->z1.n;
            evt_s4->d.sprite.coords[2].y = local_88[local_58[j][1]][1] + evt->y1.n;
            evt_s4->d.sprite.coords[3].x = local_88[local_58[j][2]][0] + evt->x1.n;
            evt_s4->d.sprite.coords[3].z = local_88[local_58[j][2]][2] + evt->z1.n;
            evt_s4->d.sprite.coords[3].y = local_88[local_58[j][2]][1] + evt->y1.n;
            AddEvtPrim4(gGraphicsPtr->ot, evt_s4);
         }
      } // for (i)

      evt_s4->functionIndex = EVTF_NULL;

      switch (evt->state3) {
      case 0:
         EVT.todo_x28 += 0x40;
         evt->state2 += 0x20;
         if (evt->state2 >= 0x400) {
            evt->state3++;
            evt->state2 = 0;
         }
         break;

      case 1:
         EVT.todo_x28 += 0x40;
         if (++EVT.timer == 90) {
            evt->state3++;
         }
         break;

      case 2:
         EVT.todo_x28 += 0x40;
         evt->state2 += 0x20;
         if (evt->state2 >= 0x400) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

void Evtf128_SpellFx2_ThunderBall(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_FX_TBD_148;
   fx->x1.n = unitSprite->x1.n;
   fx->y1.n = unitSprite->y1.n;
   fx->z1.n = unitSprite->z1.n;
   fx->d.evtf148.endingFxType = 0;

   evt->functionIndex = EVTF_NULL;
}

void Evtf129_SpellFx3_ThunderBall(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_FX_TBD_148;
   fx->x1.n = unitSprite->x1.n;
   fx->y1.n = unitSprite->y1.n;
   fx->z1.n = unitSprite->z1.n;
   fx->d.evtf148.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 148
void Evtf148_Fx_TBD(EvtData *evt) {
   EvtData *evt_v1;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         if (EVT.timer == 0) {
            EVT.clut = 3;
         }
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_FX_TBD_147;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->y1.n = evt->y1.n;
         evt_v1->z1.n = evt->z1.n;
         evt_v1->d.evtf147.clut = EVT.clut;
         EVT.timer = 25;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == -1) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_FX_TBD_136 + EVT.endingFxType;
            evt_v1->d.evtf136.clut = EVT.clut;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->y1.n = evt->y1.n;
            evt_v1->z1.n = evt->z1.n;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

static s16 sColorsAnimData_800feea8[16] = {5, GFX_COLOR_10, 1, GFX_COLOR_11, 1, GFX_COLOR_12,
                                           1, GFX_COLOR_13, 1, GFX_COLOR_12, 1, GFX_COLOR_11,
                                           1, GFX_NULL,     1, GFX_NULL};

static s16 sExplosionAnimData_800feec8[12] = {4, GFX_EXPLOSION_1, 1, GFX_EXPLOSION_2,
                                              1, GFX_EXPLOSION_3, 1, GFX_EXPLOSION_2,
                                              1, GFX_NULL,        1, GFX_NULL};

static s16 sColorsAnimData_800feee0[64] = {
    4, GFX_COLOR_1,  1, GFX_COLOR_2,  1, GFX_COLOR_3,  1, GFX_COLOR_4,  1, GFX_COLOR_5,
    1, GFX_COLOR_6,  1, GFX_COLOR_7,  1, GFX_COLOR_8,  1, GFX_COLOR_9,  1, GFX_COLOR_10,
    1, GFX_COLOR_11, 1, GFX_COLOR_12, 1, GFX_COLOR_13, 1, GFX_COLOR_12, 1, GFX_COLOR_13,
    1, GFX_COLOR_14, 1, GFX_COLOR_15, 1, GFX_COLOR_14, 1, GFX_COLOR_13, 1, GFX_COLOR_12,
    1, GFX_COLOR_11, 1, GFX_COLOR_10, 1, GFX_COLOR_9,  1, GFX_COLOR_8,  1, GFX_COLOR_7,
    1, GFX_COLOR_6,  1, GFX_COLOR_5,  1, GFX_COLOR_4,  1, GFX_COLOR_3,  1, GFX_COLOR_2,
    1, GFX_NULL,     1, GFX_NULL};

#undef EVTF
#define EVTF 167
void Evtf167_RainbowSwirl(EvtData *evt) {
   // Swirling strips of translucent rainbow colors;
   // Used during FX1 of Spread Force and Rainbow Storm / Unicorn
   s16 rainbowCluts[4] = {3, 4, 8, 9};
   EvtData *sprite;
   s32 i;
   s16 a, b, c;
   s16 angle, angleOfs;
   s16 sVar2;
   s32 sum;

   switch (evt->state) {
   case 0:
      a = EVT.todo_x58;
      EVT.todo_x24 = rand() % 0x1000;
      EVT.todo_x26 = rand() % 0x300 + 0x300;
      EVT.todo_x28 = rand() % a + a;
      EVT.todo_x2a = -0x600;
      EVT.todo_x2c = rand() % 0x1000;
      EVT.todo_x2e = rand() % 0x300 + 0x300;
      EVT.todo_x30 = rand() % a + a;
      EVT.todo_x32 = -0x600;
      EVT.todo_x34 = rand() % 0x1000;
      EVT.todo_x36 = rand() % 0x300 + 0x300;
      EVT.todo_x38 = rand() % a + a;
      EVT.todo_x3a = -0x600;
      EVT.todo_x3c = rand() % 0x1000;
      EVT.todo_x3e = rand() % 0x300 + 0x300;
      EVT.todo_x40 = rand() % a + a;
      EVT.todo_x42 = -0x600;
      EVT.todo_x44 = rand() % 0x1000;
      EVT.todo_x46 = rand() % 0x300 + 0x300;
      EVT.todo_x48 = rand() % a + a;
      EVT.todo_x4a = -0x600;
      EVT.todo_x4c = rand() % 0x1000;
      EVT.todo_x4e = rand() % 0x300 + 0x300;
      EVT.todo_x50 = rand() % a + a;
      EVT.todo_x52 = -0x600;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sColorsAnimData_800feee0;
      sprite->d.sprite.animInitialized = 0;
      EVT.sprite = sprite;

      evt->state++;
      break;

   case 1:
      sprite = EVT.sprite;
      sprite->d.sprite.semiTrans = EVT.todo_x5a;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      sum = 0;
      angle = EVT.todo_x58;
      EVT.todo_x58 += 0x60;
      sVar2 = EVT.todo_x54;
      b = EVT.todo_x56;

      for (i = 0; i < 6; i++) {
         switch (i) {
         case 0:
            a = EVT.todo_x2a;
            c = EVT.todo_x26 + a;
            angleOfs = EVT.todo_x24;
            break;
         case 1:
            a = EVT.todo_x32;
            c = EVT.todo_x2e + a;
            angleOfs = EVT.todo_x2c;
            break;
         case 2:
            a = EVT.todo_x3a;
            c = EVT.todo_x36 + a;
            angleOfs = EVT.todo_x34;
            break;
         case 3:
            a = EVT.todo_x42;
            c = EVT.todo_x3e + a;
            angleOfs = EVT.todo_x3c;
            break;
         case 4:
            a = EVT.todo_x4a;
            c = EVT.todo_x46 + a;
            angleOfs = EVT.todo_x44;
            break;
         case 5:
            a = EVT.todo_x52;
            c = EVT.todo_x4e + a;
            angleOfs = EVT.todo_x4c;
            break;
         }

         if (a >= 0x800) {
            a = 0x800;
         } else if (a <= 0) {
            a = 0;
         }
         if (c >= 0x800) {
            c = 0x800;
         } else if (c <= 0) {
            c = 0;
         }

         sum += c;

         sprite->d.sprite.coords[0].x = evt->x1.n + (b * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[0].y = evt->y1.n + a;
         sprite->d.sprite.coords[0].z = evt->z1.n + (b * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[1].x = evt->x1.n + (b * rcos(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[1].y = evt->y1.n + a;
         sprite->d.sprite.coords[1].z = evt->z1.n + (b * rsin(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[2].x = evt->x1.n + (b * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[2].y = evt->y1.n + c;
         sprite->d.sprite.coords[2].z = evt->z1.n + (b * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[3].x = evt->x1.n + (b * rcos(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[3].y = evt->y1.n + c;
         sprite->d.sprite.coords[3].z = evt->z1.n + (b * rsin(angle + angleOfs + sVar2) >> 12);

         sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[3].x) / 2;
         sprite->y1.n = sprite->d.sprite.coords[3].y;
         sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[3].z) / 2;

         UpdateEvtAnimation(sprite);
         sprite->d.sprite.clut = rainbowCluts[rand() % 4];
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
      }

      EVT.todo_x2a += EVT.todo_x28;
      EVT.todo_x32 += EVT.todo_x30;
      EVT.todo_x3a += EVT.todo_x38;
      EVT.todo_x42 += EVT.todo_x40;
      EVT.todo_x4a += EVT.todo_x48;
      EVT.todo_x52 += EVT.todo_x50;

      if (sum == 0x3000) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 168
void Evtf168_SpellFx_RainbowStroke_RainbowSwirl(EvtData *evt) {
   s16 rainbowCluts[4] = {3, 4, 8, 9};
   EvtData *sprite;
   s32 i;
   s16 a, b, c;
   s16 angle, angleOfs;
   s16 sVar2;
   s32 sum;

   switch (evt->state) {
   case 0:
      a = EVT.todo_x58;
      EVT.todo_x24 = rand() % 0x1000;
      EVT.todo_x26 = rand() % 0x300 + 0x300;
      EVT.todo_x28 = rand() % a + a;
      EVT.todo_x2a = 0x600;
      EVT.todo_x2c = rand() % 0x1000;
      EVT.todo_x2e = rand() % 0x300 + 0x300;
      EVT.todo_x30 = rand() % a + a;
      EVT.todo_x32 = 0x600;
      EVT.todo_x34 = rand() % 0x1000;
      EVT.todo_x36 = rand() % 0x300 + 0x300;
      EVT.todo_x38 = rand() % a + a;
      EVT.todo_x3a = 0x600;
      EVT.todo_x3c = rand() % 0x1000;
      EVT.todo_x3e = rand() % 0x300 + 0x300;
      EVT.todo_x40 = rand() % a + a;
      EVT.todo_x42 = 0x600;
      EVT.todo_x44 = rand() % 0x1000;
      EVT.todo_x46 = rand() % 0x300 + 0x300;
      EVT.todo_x48 = rand() % a + a;
      EVT.todo_x4a = 0x600;
      EVT.todo_x4c = rand() % 0x1000;
      EVT.todo_x4e = rand() % 0x300 + 0x300;
      EVT.todo_x50 = rand() % a + a;
      EVT.todo_x52 = 0x600;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sColorsAnimData_800feee0;
      sprite->d.sprite.animInitialized = 0;
      EVT.sprite = sprite;

      evt->state++;
      break;

   case 1:
      sprite = EVT.sprite;
      sprite->d.sprite.semiTrans = EVT.todo_x5a;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n;

      sum = 0;
      angle = EVT.todo_x58;
      EVT.todo_x58 += 0x60;
      sVar2 = EVT.todo_x54;
      b = EVT.todo_x56;

      for (i = 0; i < 6; i++) {
         switch (i) {
         case 0:
            a = EVT.todo_x2a;
            c = EVT.todo_x26 + a;
            angleOfs = EVT.todo_x24;
            break;
         case 1:
            a = EVT.todo_x32;
            c = EVT.todo_x2e + a;
            angleOfs = EVT.todo_x2c;
            break;
         case 2:
            a = EVT.todo_x3a;
            c = EVT.todo_x36 + a;
            angleOfs = EVT.todo_x34;
            break;
         case 3:
            a = EVT.todo_x42;
            c = EVT.todo_x3e + a;
            angleOfs = EVT.todo_x3c;
            break;
         case 4:
            a = EVT.todo_x4a;
            c = EVT.todo_x46 + a;
            angleOfs = EVT.todo_x44;
            break;
         case 5:
            a = EVT.todo_x52;
            c = EVT.todo_x4e + a;
            angleOfs = EVT.todo_x4c;
            break;
         }

         if (a >= 0x800) {
            a = 0x800;
         } else if (a <= 0) {
            a = 0;
         }
         if (c >= 0x800) {
            c = 0x800;
         } else if (c <= 0) {
            c = 0;
         }

         sum += c;

         sprite->d.sprite.coords[0].x = evt->x1.n + (b * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[0].y = evt->y1.n + a;
         sprite->d.sprite.coords[0].z = evt->z1.n + (b * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[1].x = evt->x1.n + (b * rcos(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[1].y = evt->y1.n + a;
         sprite->d.sprite.coords[1].z = evt->z1.n + (b * rsin(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[2].x = evt->x1.n + (b * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[2].y = evt->y1.n + c;
         sprite->d.sprite.coords[2].z = evt->z1.n + (b * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[3].x = evt->x1.n + (b * rcos(angle + angleOfs + sVar2) >> 12);
         sprite->d.sprite.coords[3].y = evt->y1.n + c;
         sprite->d.sprite.coords[3].z = evt->z1.n + (b * rsin(angle + angleOfs + sVar2) >> 12);

         sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[3].x) / 2;
         sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[3].z) / 2;
         sprite->y1.n = sprite->d.sprite.coords[3].y;

         UpdateEvtAnimation(sprite);
         sprite->d.sprite.clut = rainbowCluts[rand() % 4];
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
      }

      EVT.todo_x2a -= EVT.todo_x28;
      EVT.todo_x32 -= EVT.todo_x30;
      EVT.todo_x3a -= EVT.todo_x38;
      EVT.todo_x42 -= EVT.todo_x40;
      EVT.todo_x4a -= EVT.todo_x48;
      EVT.todo_x52 -= EVT.todo_x50;

      if (sum == 0) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 169
void Evtf169_SpellFx1_EvilStream(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *sprite;
   POLY_FT4 *poly;
   s16 a;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + 0x100;
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      if (EVT.clut == 0) {
         EVT.clut = 3;
      }
      EVT.fade = 0x80;

      evt->state++;

   // fallthrough
   case 1:
      ApplyMaskEffect(448 << 2, 384, 64, 64, (384 << 2) + evt->state3 % 8 * 32, 0, 0,
                      (evt->state3 * 3) % 64, GFX_TBD_26, 0);

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_TBD_26;
      sprite->d.sprite.boxIdx = 7;
      sprite->x1.n = evt->x1.n;
      sprite->y1.n = evt->y1.n;
      sprite->z1.n = evt->z1.n;

      a = 0x80 * rsin(EVT.todo_x26) >> 12;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -a;
      gQuad_800fe63c[1].vx = a;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -a;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = a;

      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.clut = EVT.clut;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      sprite->d.sprite.semiTrans = 3;
      sprite->d.sprite.clut = 10;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      sprite->functionIndex = EVTF_NULL;

      a = 0x48 * rsin(EVT.todo_x26) >> 12;
      gQuad_800fe63c[0].vx = -a;
      gQuad_800fe63c[0].vy = -a;
      gQuad_800fe63c[1].vx = a;
      gQuad_800fe63c[1].vy = -a;
      gQuad_800fe63c[2].vx = -a;
      gQuad_800fe63c[2].vy = a;
      gQuad_800fe63c[3].vx = a;
      gQuad_800fe63c[3].vy = a;

      ApplyMaskEffect(496 << 2, 384, 64, 64, (384 << 2) + evt->state3 % 8 * 32, 0, 0, 0, GFX_TBD_25,
                      0);

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_TBD_25;
      sprite->d.sprite.boxIdx = 7;
      sprite->x1.n = evt->x1.n;
      sprite->y1.n = evt->y1.n;
      sprite->z1.n = evt->z1.n;

      sprite->d.sprite.semiTrans = 2;
      sprite->d.sprite.clut = EVT.clut;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x80 - EVT.fade, 0x80 - EVT.fade, 0x80 - EVT.fade);

      sprite->d.sprite.semiTrans = 3;
      sprite->d.sprite.clut = 10;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0x80 - EVT.fade, 0x80 - EVT.fade, 0x80 - EVT.fade);

      evt->state3++;
      EVT.fade -= 4;

      EVT.todo_x26 += 0x20;
      if (EVT.todo_x26 > 0x400) {
         evt->state++;
         EVT.fade = 0x80;
      }

      sprite->functionIndex = EVTF_NULL;
      break;

   case 2:
      gQuad_800fe63c[0].vx = -0x48;
      gQuad_800fe63c[0].vy = -0x48;
      gQuad_800fe63c[1].vx = 0x48;
      gQuad_800fe63c[1].vy = -0x48;
      gQuad_800fe63c[2].vx = -0x48;
      gQuad_800fe63c[2].vy = 0x48;
      gQuad_800fe63c[3].vx = 0x48;
      gQuad_800fe63c[3].vy = 0x48;

      ApplyMaskEffect(496 << 2, 384, 64, 64, (384 << 2) + evt->state3 % 8 * 32, 0, 0, 0, GFX_TBD_25,
                      0);

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_TBD_25;
      sprite->d.sprite.boxIdx = 7;
      sprite->x1.n = evt->x1.n;
      sprite->y1.n = evt->y1.n;
      sprite->z1.n = evt->z1.n;

      sprite->d.sprite.clut = EVT.clut;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      sprite->d.sprite.semiTrans = 3;
      sprite->d.sprite.clut = 10;
      AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      evt->state3++;

      EVT.fade -= 0x10;
      if (EVT.fade == 0) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }

      sprite->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 161
void Evtf161_SpellFx1_PiercingLight(EvtData *evt) {
   s16 cluts[4] = {3, 4, 9, 8};
   EvtData *casterSprite;
   EvtData *targetSprite;
   EvtData *homingRay;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.clut = cluts[rand() % 4];
      evt->state++;
      break;

   case 1:
      casterSprite = EVT.unitSprite;
      homingRay = Evt_GetUnused();
      homingRay->functionIndex = EVTF_HOMING_RAY;
      homingRay->x1.n = casterSprite->x1.n;
      homingRay->y1.n = casterSprite->y1.n + 0x80;
      homingRay->z1.n = casterSprite->z1.n;
      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      homingRay->d.evtf171.todo_x2c = targetSprite->x1.n - casterSprite->x1.n;
      homingRay->d.evtf171.todo_x2e = targetSprite->z1.n - casterSprite->z1.n;
      homingRay->d.evtf171.todo_x30 = (targetSprite->y1.n - casterSprite->y1.n) + 0x70;
      homingRay->d.evtf171.clut = EVT.clut;

      if (++evt->state2 == 17) {
         evt->state++;
      }

      gCameraZoom.vz += 11;
      gCameraRotation.vx += 8;
      break;

   case 2:
      gCameraZoom.vz += 11;
      gCameraRotation.vx += 8;
      if (evt->state2 == 36) {
         gSignal3 = 1;
      } else if (evt->state2 == 43) {
         evt->functionIndex = EVTF_NULL;
      }
      evt->state2++;
      break;
   }
}

#undef EVTF
#define EVTF 170
void Evtf170_SpellFx1_PiercingRay_Etc(EvtData *evt) {
   // FX1 for Plasma Wave and Piercing Ray / Aura Gem
   s16 cluts[5] = {3, 4, 9, 8, 5};
   EvtData *casterSprite;
   EvtData *targetSprite;
   EvtData *homingRay;
   u8 *p;
   s16 i;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->state++;
      break;

   case 1:
      casterSprite = EVT.unitSprite;
      p = gTargetCoords;
      for (i = 0; *p != 0xff && i < 15; i++) {
         homingRay = Evt_GetUnused();
         homingRay->functionIndex = EVTF_HOMING_RAY;
         homingRay->x1.n = casterSprite->x1.n;
         homingRay->y1.n = casterSprite->y1.n + 0x80;
         homingRay->z1.n = casterSprite->z1.n;
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         homingRay->d.evtf171.todo_x2c = targetSprite->x1.n - casterSprite->x1.n;
         homingRay->d.evtf171.todo_x2e = targetSprite->z1.n - casterSprite->z1.n;
         homingRay->d.evtf171.todo_x30 = (targetSprite->y1.n - casterSprite->y1.n) + 0x70;
         homingRay->d.evtf171.clut = cluts[i % 5];
         p += 2;
      }

      gCameraZoom.vz += 12;
      gCameraRotation.vx += 8;

      if (Evt_CountUnused() >= 50) {
         if (++evt->state2 == 17) {
            evt->state++;
         }
      } else {
         evt->state++;
      }
      break;

   case 2:
      gCameraZoom.vz += 12;
      gCameraRotation.vx += 8;
      if (++evt->state2 == 39) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 171
void Evtf171_HomingRay(EvtData *evt) {
   s16 colorGfx[16] = {GFX_COLOR_6,  GFX_COLOR_7,  GFX_COLOR_8,  GFX_COLOR_9,
                       GFX_COLOR_10, GFX_COLOR_11, GFX_COLOR_12, GFX_COLOR_13,
                       GFX_COLOR_13, GFX_COLOR_12, GFX_COLOR_11, GFX_COLOR_10,
                       GFX_COLOR_9,  GFX_COLOR_8,  GFX_COLOR_7,  GFX_COLOR_6};
   EvtData *sprite;
   s32 i;
   s16 colorIdx;

   switch (evt->state) {
   case 0:
      if (EVT.clut == 0) {
         EVT.clut = 3;
      }
      EVT.todo_x26 = 0x180 - rand() % 0x300;
      EVT.todo_x28 = 0x180 + rand() % 0x180;
      EVT.todo_x2a = 0x180 - rand() % 0x300;
      evt->state++;
      break;

   case 1:
      if (gQuadIndex < 1200) {
         sprite = Evt_GetUnused();
         sprite->functionIndex = EVTF_NOOP;
         sprite->d.sprite.animInitialized = 0;
         sprite->d.sprite.clut = EVT.clut;

         colorIdx = 0;
         for (i = EVT.todo_x36; i < EVT.todo_x34; i++) {
            sprite->d.sprite.coords[0].x = evt->x1.n + (EVT.todo_x26 * rsin((i + 1) * 0x80) >> 11) +
                                           (i + 1) * EVT.todo_x2c * 0x100 / ONE;
            sprite->d.sprite.coords[0].z = evt->z1.n + (EVT.todo_x2a * rsin((i + 1) * 0x80) >> 11) +
                                           (i + 1) * EVT.todo_x2e * 0x100 / ONE;
            sprite->d.sprite.coords[0].y = evt->y1.n + (EVT.todo_x28 * rsin((i + 1) * 0x80) >> 11) +
                                           (i + 1) * EVT.todo_x30 * 0x100 / ONE;

            sprite->d.sprite.coords[1].x = evt->x1.n + (EVT.todo_x26 * rsin(i * 0x80) >> 11) +
                                           (i * EVT.todo_x2c * 0x100 / ONE);
            sprite->d.sprite.coords[1].z = evt->z1.n + (EVT.todo_x2a * rsin(i * 0x80) >> 11) +
                                           (i * EVT.todo_x2e * 0x100 / ONE);
            sprite->d.sprite.coords[1].y = evt->y1.n + (EVT.todo_x28 * rsin(i * 0x80) >> 11) +
                                           (i * EVT.todo_x30 * 0x100 / ONE);

            sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
            sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
            sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[0].y + 0x40;

            sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;
            sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;
            sprite->d.sprite.coords[3].y = sprite->d.sprite.coords[1].y + 0x40;

            sprite->x1.n = sprite->d.sprite.coords[0].x;
            sprite->y1.n = sprite->d.sprite.coords[0].y;
            sprite->z1.n = sprite->d.sprite.coords[0].z;

            sprite->d.sprite.gfxIdx = colorGfx[colorIdx++];
            AddEvtPrim3(gGraphicsPtr->ot, sprite);
         } // for (i)

         sprite->functionIndex = EVTF_NULL;
      }

      switch (evt->state2) {
      case 0:
         if (++EVT.todo_x34 == 16) {
            EVT.todo_x36 = 0;
            evt->state2++;
         }
         break;
      case 1:
         if (++EVT.todo_x36 == 16) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 151
void Evtf151_SpellFx1_FireGem(EvtData *evt) {
   EvtData *casterSprite;
   EvtData *targetSprite;
   EvtData *evt_s0;
   s16 i;
   u8 *p;

   switch (evt->state) {
   case 0:
      casterSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      p = gTargetCoords;
      for (i = 0; *p != 0xff && i < 15; i++) {
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_SPELL_FX_FIRE_GEM_BEAM;
         evt_s0->y1.n = GetTerrainElevation(p[1], p[0]);
         evt_s0->x1.n = targetSprite->x1.n;
         evt_s0->z1.n = targetSprite->z1.n;
         p += 2;
      }

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s0->d.evtf026.sprite = targetSprite;

      evt->state++;
      break;

   case 1:
      gCameraZoom.vz += 2;
      gCameraRotation.vx += 2;
      gCameraRotation.vy += 16;
      if (++evt->state2 == 121) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 175
void Evtf175_SpellFx1_RainbowStorm(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *evt_s1;
   // register EvtData *evt_s1 asm("s1");
   EvtData *targetSprite; // fixes regswap
   EvtData **pDataStoreAsEvts;
   u8 *p;
   s32 i;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s0;

      pDataStoreAsEvts = evt_s0->d.dataStore.evts;
      p = gTargetCoords;
      for (EVT.iterator = 0; *p != 0xff && EVT.iterator < 15; EVT.iterator++) {
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_FX_TBD_149;
         evt_s1->d.evtf149.clut = 4;
         evt_s1->x1.s.hi = targetSprite->x1.s.hi;
         evt_s1->z1.s.hi = targetSprite->z1.s.hi;
         pDataStoreAsEvts[EVT.iterator] = evt_s1;
         p += 2;
      }

      evt->state2 = 0;
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:
      if (EVT.todo_x24 < 0x400) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_RAINBOW_SWIRL;
         evt_s1->d.evtf167.todo_x56 = 0x300 * (ONE - rsin(EVT.todo_x24)) / ONE;
         evt_s1->d.evtf167.todo_x54 = 0x60;
         evt_s1->d.evtf167.todo_x58 = 0x20 + 0x28 * rcos(EVT.todo_x24) / ONE;
         evt_s1->d.evtf167.todo_x5a = 1;
         evt_s1->x1.n = evt->x1.n;
         evt_s1->y1.n = evt->y1.n;
         evt_s1->z1.n = evt->z1.n;
      }

      gCameraZoom.vz += 8;
      gCameraRotation.vx -= 2;
      EVT.todo_x24 += 0x40;

      switch (evt->state2) {
      case 0:
         gLightColor.r -= 8;
         gLightColor.g -= 8;
         gLightColor.b -= 8;
         if (gLightColor.r <= 0x20) {
            evt->state2++;
         }
         break;
      case 1:
         if (EVT.todo_x24 > 0xb00) {
            gLightColor.r += 8;
            gLightColor.g += 8;
            gLightColor.b += 8;
         }
         break;
      }

      if (EVT.todo_x24 > 0xe00) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;

         evt_s0 = EVT.dataStore;
         pDataStoreAsEvts = evt_s0->d.dataStore.evts;
         for (i = 0; i < EVT.iterator; i++) {
            evt_s1 = pDataStoreAsEvts[i];
            evt_s1->state = 99;
         }
         evt_s0->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 176
void Evtf176_SpellFx1_RainbowStroke(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *evt_s1;
   EvtData *targetSprite; // fixes regswap (again)

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt->y1.n = GetTerrainElevation(evt_s0->z1.s.hi, evt_s0->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.sprite = evt_s0;
      evt_s1->d.evtf026.type = 2;
      EVT.cam = evt_s1;

      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_FX_TBD_149;
      evt_s1->d.evtf149.clut = 4;
      evt_s1->x1.s.hi = targetSprite->x1.s.hi;
      evt_s1->z1.s.hi = targetSprite->z1.s.hi;
      EVT.fx = evt_s1;

      evt->state++;

   // fallthrough
   case 1:
      if (EVT.todo_x24 < 0x400) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_SPELL_FX_RAINBOW_STROKE_RAINBOW_SWIRL;
         evt_s1->d.evtf168.todo_x56 = 0xe0 * (ONE - rsin(EVT.todo_x24)) / ONE;
         evt_s1->d.evtf168.todo_x54 = 0x60;
         evt_s1->d.evtf168.todo_x58 = 0x20 + 0x10 * rcos(EVT.todo_x24) / ONE;
         evt_s1->d.evtf168.todo_x5a = 0;
         evt_s1->x1.n = evt->x1.n;
         evt_s1->y1.n = evt->y1.n;
         evt_s1->z1.n = evt->z1.n;
      }

      gCameraZoom.vz += 6;
      gCameraRotation.vx -= 6;
      gCameraRotation.vy -= 12;

      EVT.todo_x24 += 0x40;

      switch (evt->state2) {
      case 0:
         gLightColor.r -= 8;
         gLightColor.g -= 8;
         gLightColor.b -= 8;
         if (gLightColor.r <= 0x20) {
            evt->state2++;
         }
         break;
      case 1:
         if (EVT.todo_x24 > 0xb00) {
            gLightColor.r += 8;
            gLightColor.g += 8;
            gLightColor.b += 8;
         }
         break;
      }

      if (EVT.todo_x24 == 0x1c0) {
         evt_s0 = EVT.cam;
         evt_s0->functionIndex = EVTF_NULL;
      }

      if (EVT.todo_x24 > 0xe00) {
         evt->functionIndex = EVTF_NULL;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
         gSignal3 = 1;
         evt_s0 = EVT.fx;
         evt_s0->state = 99;
      }

      break;
   }
}

#undef EVTF
#define EVTF 177
void Evtf177_SpellFx1_HolyPressure(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_s1;
   EvtData *targetSprite;
   EvtData **pDataStoreAsEvts;
   u8 *p;
   s32 i;

   switch (evt->state) {
   case 0:
      evt_s2 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s2->x1.n;
      evt->y1.n = evt_s2->y1.n;
      evt->z1.n = evt_s2->z1.n;

      p = gTargetCoords;
      evt_s2 = GetUnitSpriteAtPosition(p[1], p[0]);
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.sprite = evt_s2;
      evt_s1->d.evtf026.type = 2;

      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s2;
      pDataStoreAsEvts = evt_s2->d.dataStore.evts;

      for (EVT.iterator = 0; *p != 0xff && EVT.iterator < 15; /*EVT.iterator++*/) {
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_FX_TBD_149;
         evt_s1->d.evtf149.clut = 9;
         evt_s1->x1.n = targetSprite->x1.n;
         evt_s1->z1.n = targetSprite->z1.n;
         evt_s1->y1.n = targetSprite->y1.n;
         pDataStoreAsEvts[EVT.iterator] = evt_s1;
         EVT.iterator++;

         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_SPELL_FX_HOLY_PRESSURE_CUBE;
         evt_s1->x1.s.hi = targetSprite->x1.s.hi;
         evt_s1->z1.s.hi = targetSprite->z1.s.hi;

         p += 2;
      }

      evt->state++;

   // fallthrough
   case 1:
      gCameraZoom.vz += 2;
      gCameraRotation.vx -= 1;

      if (++EVT.timer > 150) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;

         evt_s2 = EVT.dataStore;
         pDataStoreAsEvts = evt_s2->d.dataStore.evts;
         for (i = 0; i < EVT.iterator; i++) {
            evt_s1 = pDataStoreAsEvts[i];
            FntPrint("%d\n", evt_s1->functionIndex);
            evt_s1->state = 99;
         }
         evt_s2->functionIndex = EVTF_NULL;
      }

      break;
   }
}
