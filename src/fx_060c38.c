#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

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
      homingRay->d.evtf171.dx = targetSprite->x1.n - casterSprite->x1.n;
      homingRay->d.evtf171.dz = targetSprite->z1.n - casterSprite->z1.n;
      homingRay->d.evtf171.dy = (targetSprite->y1.n - casterSprite->y1.n) + 0x70;
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
         homingRay->d.evtf171.dx = targetSprite->x1.n - casterSprite->x1.n;
         homingRay->d.evtf171.dz = targetSprite->z1.n - casterSprite->z1.n;
         homingRay->d.evtf171.dy = (targetSprite->y1.n - casterSprite->y1.n) + 0x70;
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
                                           (i + 1) * EVT.dx * 0x100 / ONE;
            sprite->d.sprite.coords[0].z = evt->z1.n + (EVT.todo_x2a * rsin((i + 1) * 0x80) >> 11) +
                                           (i + 1) * EVT.dz * 0x100 / ONE;
            sprite->d.sprite.coords[0].y = evt->y1.n + (EVT.todo_x28 * rsin((i + 1) * 0x80) >> 11) +
                                           (i + 1) * EVT.dy * 0x100 / ONE;

            sprite->d.sprite.coords[1].x =
                evt->x1.n + (EVT.todo_x26 * rsin(i * 0x80) >> 11) + (i * EVT.dx * 0x100 / ONE);
            sprite->d.sprite.coords[1].z =
                evt->z1.n + (EVT.todo_x2a * rsin(i * 0x80) >> 11) + (i * EVT.dz * 0x100 / ONE);
            sprite->d.sprite.coords[1].y =
                evt->y1.n + (EVT.todo_x28 * rsin(i * 0x80) >> 11) + (i * EVT.dy * 0x100 / ONE);

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
      evt_s0->d.evtf026.target = targetSprite;

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
         evt_s1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
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
      evt_s1->d.evtf026.target = evt_s0;
      evt_s1->d.evtf026.type = 2;
      EVT.cam = evt_s1;

      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
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
      evt_s1->d.evtf026.target = evt_s2;
      evt_s1->d.evtf026.type = 2;

      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s2;
      pDataStoreAsEvts = evt_s2->d.dataStore.evts;

      for (EVT.iterator = 0; *p != 0xff && EVT.iterator < 15; /*EVT.iterator++*/) {
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
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

#undef EVTF
#define EVTF 160
void Evtf160_SpellFx_IceStorm_Target(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *fxSprite1;
   EvtData *fxSprite2;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      targetSprite = EVT.targetSprite;
      targetSprite->d.sprite.hidden = 1;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.targetSprite;
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = targetSprite->y1.n;

      fxSprite1 = Evt_GetUnused();
      CopyEvtData(targetSprite, fxSprite1);
      fxSprite2 = Evt_GetUnused();
      CopyEvtData(targetSprite, fxSprite2);
      fxSprite1->d.sprite.hidden = 0;
      fxSprite2->d.sprite.hidden = 0;

      GetUnitSpriteVramRect(targetSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 384 << 2, 384, 0,
                      -(evt->state3++ % 64), GFX_TBD_25, 0);

      fxSprite1->d.sprite.gfxIdx = GFX_TBD_25;
      fxSprite1->d.sprite.clut = EVT.clut;
      fxSprite1->d.sprite.semiTrans = 2;

      if (targetSprite->d.sprite.gfxIdx >= 21 && targetSprite->d.sprite.gfxIdx <= 30) {
         fxSprite1->d.sprite.boxIdx = 1;
      } else {
         fxSprite1->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, fxSprite1, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, fxSprite2, 0);

      fxSprite1->functionIndex = EVTF_NULL;
      fxSprite2->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.fade += 4;
         if (EVT.fade > 0x40) {
            evt->state2++;
            EVT.fade = 0x40;
         }
         break;
      case 1:
         EVT.timer++;
         if (EVT.timer == 88) {
            evt->state2++;
            EVT.todo_x2c = 0x40;
         }
         break;
      case 2:
         EVT.fade -= 8;
         if (EVT.fade < 0) {
            targetSprite = EVT.targetSprite;
            targetSprite->d.sprite.hidden = 0;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 162
void Evtf162_SpellFx_IceStorm_Splash(EvtData *evt) {
   static s16 splashAnimData[] = {
       7, GFX_SPLASH_1, 1, GFX_SPLASH_2, 1, GFX_SPLASH_3, 1, GFX_SPLASH_4, 1, GFX_SPLASH_5,
       1, GFX_SPLASH_6, 1, GFX_SPLASH_7, 1, GFX_SPLASH_8, 1, GFX_NULL,     0, GFX_NULL};

   POLY_FT4 *poly;
   EvtData *sprite;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = splashAnimData;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;

      EVT.xOfs = 0x80 - rand() % 0x101;
      EVT.zOfs = 0x80 - rand() % 0x101;
      EVT.fade = 0x80;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);
      if (sprite->d.sprite.animFinished) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      } else {
         sprite->x1.n = evt->x1.n + EVT.xOfs;
         sprite->z1.n = evt->z1.n + EVT.zOfs;
         sprite->y1.n = evt->y1.n;

         gQuad_800fe63c[0].vx = -12;
         gQuad_800fe63c[0].vy = -64;
         gQuad_800fe63c[1].vx = 12;
         gQuad_800fe63c[1].vy = -64;
         gQuad_800fe63c[2].vx = -12;
         gQuad_800fe63c[2].vy = 0;
         gQuad_800fe63c[3].vx = 12;
         gQuad_800fe63c[3].vy = 0;

         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
         EVT.fade -= 10;
      }
      break;
   }
}

#undef EVTF
#define EVTF 189
void Evtf189_SpellFx1_IceStorm(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_s1;
   s32 i, j;
   s32 a;
   s16 b, c, d;

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt->y1.n = GetTerrainElevation(gTargetZ, gTargetX);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;

      evt_s1 = CreatePositionedEvt(evt, EVTF_SPELL_FX_ICE_STORM_TARGET);
      evt_s1->d.evtf160.targetSprite = targetSprite;
      evt_s1->d.evtf160.clut = 4;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_SPELL_FX_ICE_STORM_CAMERA;
      evt_s1->d.evtf279.targetSprite = targetSprite;

      EVT.todo_x24 = 0x280;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_TILED_ICE_DYN_1;
      evt_s1->d.sprite.clut = 4;
      evt_s1->d.sprite.semiTrans = 1;

      EVT.todo_x5c += 2;
      a = EVT.todo_x2c++;

      switch (evt->state2) {
      case 0:
         b = 0x80 + 0x200 * rcos(EVT.todo_x26) / ONE;
         c = 0x600 * rsin(EVT.todo_x26) / ONE;
         d = 0x40 + 0x3c0 * (ONE - rcos(EVT.todo_x26)) / ONE;
         d /= 4;
         break;
      case 1:
         b = 0x80;
         c = 0x400 * (ONE - rsin(EVT.todo_x28)) / ONE;
         d = 0x100;
         break;
      }

      for (i = 0; i < 4; i++) {
         for (j = 0; j < 4; j++) {
            gGfxSubTextures[GFX_TILED_ICE_DYN_1].x = 0x80 + ((i * 4 + j) * 2 + EVT.todo_x5c) % 0x20;
            gGfxSubTextures[GFX_TILED_ICE_DYN_1].y = 0x40 + ((i * 4 + j) * 2 + EVT.todo_x5c) % 0x20;
            gGfxSubTextures[GFX_TILED_ICE_DYN_1].w = 2;
            gGfxSubTextures[GFX_TILED_ICE_DYN_1].h = 31;

            evt_s1->d.sprite.coords[0].x = evt->x1.n + b * rcos(a * 0x40 + j * d) / ONE;
            evt_s1->d.sprite.coords[0].z = evt->z1.n + b * rsin(a * 0x40 + j * d) / ONE;
            evt_s1->d.sprite.coords[0].y = evt->y1.n + c / 2;

            evt_s1->d.sprite.coords[1].x = evt->x1.n + b * rcos(a * 0x40 + (j + 1) * d) / ONE;
            evt_s1->d.sprite.coords[1].z = evt->z1.n + b * rsin(a * 0x40 + (j + 1) * d) / ONE;
            evt_s1->d.sprite.coords[1].y = evt->y1.n + c / 2;

            evt_s1->d.sprite.coords[2].x = evt->x1.n + b * rcos(a * 0x40 + j * d) / ONE;
            evt_s1->d.sprite.coords[2].z = evt->z1.n + b * rsin(a * 0x40 + j * d) / ONE;
            evt_s1->d.sprite.coords[2].y = evt->y1.n;

            evt_s1->d.sprite.coords[3].x = evt->x1.n + b * rcos(a * 0x40 + (j + 1) * d) / ONE;
            evt_s1->d.sprite.coords[3].z = evt->z1.n + b * rsin(a * 0x40 + (j + 1) * d) / ONE;
            evt_s1->d.sprite.coords[3].y = evt->y1.n;

            AddEvtPrim4(gGraphicsPtr->ot, evt_s1);

            evt_s1->d.sprite.coords[0].y = evt->y1.n + c;
            evt_s1->d.sprite.coords[1].y = evt->y1.n + c;
            evt_s1->d.sprite.coords[2].y = evt->y1.n + c / 2;
            evt_s1->d.sprite.coords[3].y = evt->y1.n + c / 2;
            evt_s1->y1.n = (evt_s1->d.sprite.coords[0].y + evt_s1->d.sprite.coords[1].y +
                            evt_s1->d.sprite.coords[2].y + evt_s1->d.sprite.coords[3].y) /
                           4;

            AddEvtPrim3(gGraphicsPtr->ot, evt_s1);
         }
         a += 0x10;
      }

      evt_s1->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.todo_x26 += 0x10;
         if (EVT.todo_x26 >= 0x400) {
            evt->state2++;
         }
         break;
      case 1:
         CreatePositionedEvt(evt, EVTF_SPELL_FX_ICE_STORM_SPLASH);
         CreatePositionedEvt(evt, EVTF_SPELL_FX_ICE_STORM_SPLASH);
         EVT.todo_x28 += 0x10;
         if (EVT.todo_x28 >= 0x400) {
            gSignal3 = 1;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 155
void Evtf155_SpellFx_Spellbind_Glyph(EvtData *evt) {
   POLY_FT4 *poly;
   s16 red, green, blue;
   s16 radius, theta, x_1, z_1, x_2, z_2, y;

   switch (evt->state) {
   case 0:
      EVT.idx = EVT.initialIdx;
      EVT.theta = EVT.idx * 0x100;
      EVT.todo_x58 = EVT.idx / 16 * 0x40;
      EVT.todo_x5a = 0x400;
      EVT.todo_x5c = 0x100;
      EVT.todo_x28 = rand() % 12 + 0x89;
      EVT.todo_x31 = 1;
      evt->x2.n = evt->x1.n;
      evt->z2.n = evt->z1.n;
      evt->y2.n = evt->y1.n;
      EVT.initialIdx = 0;
      evt->state++;

   // fallthrough
   case 1:

      switch (evt->state3) {
      case 0:
         radius = 0xa0;
         break;
      case 1:
         radius = 0xa0 + rsin(EVT.todo_x5e) / 0x40;
         EVT.todo_x5e += 0xc0;
         break;
      case 2:
         radius = 0xa0 + rsin(EVT.todo_x5e) / 0x40;
         if (EVT.todo_x5e & 0x800U) {
            EVT.todo_x5e += 0x40;
         }
         break;
      }

      theta = EVT.theta;
      x_1 = radius * rcos(theta) >> 12;
      z_1 = radius * rsin(theta) >> 12;
      x_2 = radius * rcos(theta + 0x100) >> 12;
      z_2 = radius * rsin(theta + 0x100) >> 12;
      EVT.coords[0].x = EVT.coords[2].x = evt->x2.n + x_1;
      EVT.coords[1].x = EVT.coords[3].x = evt->x2.n + x_2;
      EVT.coords[0].z = EVT.coords[2].z = evt->z2.n + z_1;
      EVT.coords[1].z = EVT.coords[3].z = evt->z2.n + z_2;

      y = evt->y2.n + EVT.todo_x58 + EVT.todo_x5a;
      EVT.coords[0].y = EVT.coords[1].y = y + 0x40;
      EVT.coords[2].y = EVT.coords[3].y = y - 8;

      AddEvtPrim4(gGraphicsPtr->ot, evt);

      switch (evt->state3) {
      case 0:
         EVT.todo_x5a -= EVT.todo_x5c;
         EVT.todo_x5c += (-EVT.todo_x5c >> 2);
         if (EVT.todo_x5c <= 1) {
            evt->state3++;
         }
         break;

      case 1:
         red = abs(0x80 * rcos(evt->state2) / ONE);
         green = abs(0x80 * rcos(evt->state2 * 2) / ONE);
         blue = abs(0x80 * rcos(evt->state2 / 2) / ONE);
         evt->state2 += 0x80;
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, red, green, blue);

         if (evt->z3.n++ == (0x60 - EVT.idx) * 2 + 0x1e) {
            evt->functionIndex = EVTF_NULL;
            if (EVT.idx == 10) {
               gSignal3 = 1;
            }
         }
         break;

      case 2:
         EVT.todo_x5a += EVT.todo_x5c;
         EVT.todo_x5c += (0x100 - EVT.todo_x5c) >> 2;
         if (EVT.todo_x5a >= 0x400) {
            evt->functionIndex = EVTF_NULL;
            if (EVT.idx == 10) {
               gSignal3 = 1;
            }
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 181
void Evtf181_SpellFx1_Spellbind(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_v1;

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt->y1.n = GetTerrainElevation(gTargetZ, gTargetX);
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;

      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_v1->d.evtf026.target = targetSprite;

      evt->state++;

   // fallthrough
   case 1:
      if (evt->state3 == 0x60) {
         evt->functionIndex = EVTF_NULL;
      } else {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_SPELL_FX_SPELLBIND_GLYPH;
         evt_v1->y1.n = evt->y1.n;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->z1.n = evt->z1.n;
         evt_v1->d.evtf155.initialIdx = evt->state3;
         evt->state3++;
      }
      break;
   }
}

#undef EVTF
#define EVTF 179
void Evtf179_SpellFx_ThunderFlash_Ray(EvtData *evt) {
   EvtData *evt_s2;
   s16 radius;

   switch (evt->state) {
   case 0:
      evt_s2 = EVT.parent;
      evt->x1.n = evt_s2->x1.n;
      evt->z1.n = evt_s2->z1.n;
      evt->y1.n = evt_s2->y1.n;

      EVT.todo_x24 = rand() % 0x400 + 0x400;
      EVT.todo_x26 = rand() % 0x1000;
      EVT.todo_x28 = rand() % 0x1000;
      EVT.todo_x2a = rand() % 0x1000;
      EVT.todo_x2c = 0x40 - rand() % 0x80;
      EVT.todo_x2e = 0x40 - rand() % 0x80;
      EVT.todo_x30 = 0x40 - rand() % 0x80;

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.gfxIdx = GFX_COLOR_11;
      evt_s2->d.sprite.clut = 9;
      evt_s2->d.sprite.semiTrans = 1;

      radius = EVT.todo_x24 * EVT.todo_x36 / 30;
      evt_s2->d.sprite.coords[0].x = evt->x1.n;
      evt_s2->d.sprite.coords[0].z = evt->z1.n;
      evt_s2->d.sprite.coords[0].y = evt->y1.n;
      evt_s2->d.sprite.coords[1].x = evt->x1.n;
      evt_s2->d.sprite.coords[1].z = evt->z1.n;
      evt_s2->d.sprite.coords[1].y = evt->y1.n;
      evt_s2->d.sprite.coords[2].x = evt->x1.n + radius * rcos(EVT.todo_x26) / ONE;
      evt_s2->d.sprite.coords[2].z = evt->z1.n + radius * rsin(EVT.todo_x28) / ONE;
      evt_s2->d.sprite.coords[2].y = evt->y1.n + radius * rsin(EVT.todo_x2a) / ONE;
      evt_s2->d.sprite.coords[3].x = evt->x1.n + radius * rcos(EVT.todo_x26 + 0x30) / ONE;
      evt_s2->d.sprite.coords[3].z = evt->z1.n + radius * rsin(EVT.todo_x28 + 0x30) / ONE;
      evt_s2->d.sprite.coords[3].y = evt->y1.n + radius * rsin(EVT.todo_x2a + 0x30) / ONE;

      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);
      evt_s2->functionIndex = EVTF_NULL;

      EVT.todo_x26 += EVT.todo_x2c;
      EVT.todo_x28 += EVT.todo_x2e;
      EVT.todo_x2a += EVT.todo_x30;

      switch (evt->state2) {
      case 0:
         EVT.todo_x32++;
         EVT.todo_x36++;
         if (EVT.todo_x36 == 32) {
            evt->state2++;
         }
         break;
      case 1:
         EVT.todo_x32++;
         if (EVT.todo_x32 == 98) {
            evt->state2++;
            EVT.todo_x32 = 0;
         }
         break;
      case 2:
         EVT.todo_x36 -= 2;
         if (EVT.todo_x36 == 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 178
void Evtf178_SpellFx1_ThunderFlash(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s1;
   s32 i;
   s16 a, b;
   s16 h_1, h_2;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      EVT.todo_x24 = 0x380;
      evt->state++;

   // fallthrough
   case 1:
      for (i = 0; i < 16; i++) {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_SPELL_FX_THUNDER_FLASH_RAY;
         evt_s1->d.evtf179.parent = evt;
      }
      EVT.todo_x5e = 0;
      evt->state++;

   // fallthrough
   case 2:
      if (gLightColor.r > 32) {
         gLightColor.r -= 4;
         gLightColor.g -= 4;
         gLightColor.b -= 4;
      }
      EVT.timer++;
      if (EVT.timer == 106) {
         evt->state++;
      }
      break;

   case 3:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_COLOR_13;
      evt_s1->d.sprite.clut = 4;
      evt_s1->d.sprite.semiTrans = 1;

      a = b = EVT.todo_x24 * (ONE - rcos(EVT.todo_x28)) / ONE;
      h_2 = 0x800;
      h_1 = 0;

      for (i = 0; i < 16; i++) {
         evt_s1->d.sprite.coords[0].x = evt->x1.n + a * rcos(i * 0x100) / ONE;
         evt_s1->d.sprite.coords[0].z = evt->z1.n + a * rsin(i * 0x100) / ONE;
         evt_s1->d.sprite.coords[0].y = evt->y1.n + h_1;
         evt_s1->d.sprite.coords[1].x = evt->x1.n + a * rcos((i + 1) * 0x100) / ONE;
         evt_s1->d.sprite.coords[1].z = evt->z1.n + a * rsin((i + 1) * 0x100) / ONE;
         evt_s1->d.sprite.coords[1].y = evt->y1.n + h_1;
         evt_s1->d.sprite.coords[2].x = evt->x1.n + b * rcos(i * 0x100) / ONE;
         evt_s1->d.sprite.coords[2].z = evt->z1.n + b * rsin(i * 0x100) / ONE;
         evt_s1->d.sprite.coords[2].y = evt->y1.n + h_2;
         evt_s1->d.sprite.coords[3].x = evt->x1.n + b * rcos((i + 1) * 0x100) / ONE;
         evt_s1->d.sprite.coords[3].z = evt->z1.n + b * rsin((i + 1) * 0x100) / ONE;
         evt_s1->d.sprite.coords[3].y = evt->y1.n + h_2;
         AddEvtPrim4(gGraphicsPtr->ot, evt_s1);
      }

      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
      gQuadIndex++;

      EVT.fade += 8;
      EVT.todo_x28 += 0x40;
      if (EVT.todo_x28 == 0x400) {
         gSignal3 = 1;
         evt->state++;
      }

      evt_s1->functionIndex = EVTF_NULL;
      break;

   case 4:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
      gQuadIndex++;

      EVT.fade -= 16;
      gLightColor.r += 12;
      gLightColor.g += 12;
      gLightColor.b += 12;
      if (EVT.fade < 0) {
         evt->functionIndex = EVTF_NULL;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
      }
      break;
   }
}

#undef EVTF
#define EVTF 180
void Evtf180_SpellFx1_SpreadForce(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *targetSprite;
   EvtData *evt_s1;
   s32 i, j;
   EvtData **pDataStoreAsEvts;
   u8 *p;
   POLY_FT4 *poly;
   s16 a, b, c;
   s16 h_1, h_2;
   s16 unaff_s0;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;
      EVT.todo_x24 = 0x280;
      evt->state++;

   // fallthrough
   case 1:
      EVT.timer++;
      if (EVT.timer % 2 == 0) {
         i = EVT.timer / 2;
         evt_s1 = CreatePositionedEvt(evt, EVTF_RAINBOW_SWIRL);
         evt_s1->d.evtf167.todo_x56 = i * 0x14 + 0x30;
         evt_s1->d.evtf167.todo_x54 = 0x40;
         evt_s1->d.evtf167.todo_x58 = i + 0x20;
         evt_s1->d.evtf167.todo_x5a = 1;
      }
      if (EVT.timer < 17) {
         gLightColor.r -= 6;
         gLightColor.g -= 6;
         gLightColor.b -= 6;
      }
      if (EVT.timer == 60) {
         evt->state++;
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_NOOP;
         EVT.dataStore = evt_s0;
         pDataStoreAsEvts = evt_s0->d.dataStore.evts;
         p = gTargetCoords;
         for (EVT.iterator = 0; *p != 0xff && EVT.iterator < 15; EVT.iterator++) {
            targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
            evt_s1->d.evtf149.clut = 4;
            evt_s1->x1.s.hi = targetSprite->x1.s.hi;
            evt_s1->z1.s.hi = targetSprite->z1.s.hi;
            pDataStoreAsEvts[EVT.iterator] = evt_s1;
            p += 2;
         }
      }
      break;

   case 2:
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.gfxIdx = GFX_COLOR_13;
      evt_s1->d.sprite.clut = 4;
      evt_s1->d.sprite.semiTrans = 1;
      a = EVT.todo_x24 * EVT.todo_x28 / 0x80;

      switch (evt->state2) {
      case 0:
         unaff_s0 = 0xcd;
         break;
      case 1:
         unaff_s0 = EVT.todo_x2a;
         break;
      }

      for (i = 0; i < 5; i++) {
         b = a * rcos(i * unaff_s0) / ONE;
         c = a * rcos((i + 1) * unaff_s0) / ONE;
         h_2 = a * rsin(i * unaff_s0) / ONE;
         h_1 = a * rsin((i + 1) * unaff_s0) / ONE;
         for (j = 0; j < 16; j++) {
            evt_s1->d.sprite.coords[0].x = evt->x1.n + c * rcos(j * 0x100) / ONE;
            evt_s1->d.sprite.coords[0].z = evt->z1.n + c * rsin(j * 0x100) / ONE;
            evt_s1->d.sprite.coords[0].y = evt->y1.n + h_1;
            evt_s1->d.sprite.coords[1].x = evt->x1.n + c * rcos((j + 1) * 0x100) / ONE;
            evt_s1->d.sprite.coords[1].z = evt->z1.n + c * rsin((j + 1) * 0x100) / ONE;
            evt_s1->d.sprite.coords[1].y = evt->y1.n + h_1;
            evt_s1->d.sprite.coords[2].x = evt->x1.n + b * rcos(j * 0x100) / ONE;
            evt_s1->d.sprite.coords[2].z = evt->z1.n + b * rsin(j * 0x100) / ONE;
            evt_s1->d.sprite.coords[2].y = evt->y1.n + h_2;
            evt_s1->d.sprite.coords[3].x = evt->x1.n + b * rcos((j + 1) * 0x100) / ONE;
            evt_s1->d.sprite.coords[3].z = evt->z1.n + b * rsin((j + 1) * 0x100) / ONE;
            evt_s1->d.sprite.coords[3].y = evt->y1.n + h_2;
            AddEvtPrim4(gGraphicsPtr->ot, evt_s1);
         }
      }

      evt_s1->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.todo_x28 += 4;
         if (EVT.todo_x28 == 0x80) {
            EVT.todo_x2a = 0xcd;
            evt->state2++;
         }
         break;

      case 1:
         if (EVT.todo_x2a <= 0x60) {
            gLightColor.r += 6;
            gLightColor.g += 6;
            gLightColor.b += 6;

            a = EVT.fade;
            if (a > 0x80) {
               a = 0x80;
            }

            poly = &gGraphicsPtr->quads[gQuadIndex];
            SetSemiTrans(poly, 1);
            poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
            poly->clut = gGfxClutIds[GFX_COLOR_15];
            setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
                    gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
            setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            setRGB0(poly, a, a, a);
            AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
            gQuadIndex++;

            EVT.fade += 8;
         }

         EVT.todo_x2a -= 6;
         if (EVT.todo_x2a <= 0) {
            EVT.todo_x2a = 0x80;
            gSignal3 = 1;

            evt_s0 = EVT.dataStore;
            pDataStoreAsEvts = evt_s0->d.dataStore.evts;
            for (i = 0; i < EVT.iterator; i++) {
               evt_s1 = pDataStoreAsEvts[i];
               evt_s1->state = 99;
            }
            evt_s0->functionIndex = EVTF_NULL;

            EVT.fade = 0x80;
            evt->state++;
         }
         break;
      }

      break;

   case 3:
      poly = &gGraphicsPtr->quads[gQuadIndex];
      SetSemiTrans(poly, 1);
      poly->tpage = gTPageIds[32 + gGfxTPageCells[GFX_COLOR_15]];
      poly->clut = gGfxClutIds[GFX_COLOR_15];
      setUVWH(poly, gGfxSubTextures[GFX_COLOR_15].x, gGfxSubTextures[GFX_COLOR_15].y,
              gGfxSubTextures[GFX_COLOR_15].w, gGfxSubTextures[GFX_COLOR_15].h);
      setXYWH(poly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
      AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], poly);
      gQuadIndex++;

      EVT.fade -= 16;
      if (EVT.fade <= 0) {
         evt->functionIndex = EVTF_NULL;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
      }
      break;
   }
}

#undef EVTF
#define EVTF 182
void Evtf182_Fx_TBD(EvtData *evt) {
   // Toss spiked metal balls onto the target; Unused spell?
   EvtData *evt_s0;
   EvtData *unitSprite;
   EvtData *targetSprite;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s0->d.evtf026.target = unitSprite;
      evt_s0->d.evtf026.type = 2;
      EVT.cam = evt_s0;
      EVT.savedZoom = gCameraZoom.vz;
      evt->state++;

   // fallthrough
   case 1:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      if (evt->state3 % 8 == 0) {
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_FX_TBD_183;
         evt_s0->x1.n = unitSprite->x1.n;
         evt_s0->y1.n = unitSprite->y1.n + 0x80;
         evt_s0->z1.n = unitSprite->z1.n;
         targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
         evt_s0->d.evtf183.dx = targetSprite->x1.n - unitSprite->x1.n;
         evt_s0->d.evtf183.dz = targetSprite->z1.n - unitSprite->z1.n;
         evt_s0->d.evtf183.dy = targetSprite->y1.n - unitSprite->y1.n;
      }
      gCameraZoom.vz = EVT.savedZoom + 0x100 * rsin(EVT.theta) / ONE;
      EVT.theta += 0x20;

      evt_s0 = EVT.cam;
      if (evt_s0->functionIndex == EVTF_CAMERA_TBD_026) {
         evt_s0->d.evtf026.zoom = gCameraZoom.vz;
      }

      evt->state3++;
      if (evt->state3 == 41) {
         evt->state++;
      }
      break;

   case 2:
      if (EVT.theta <= 0x800) {
         gCameraZoom.vz = EVT.savedZoom + 0x100 * rsin(EVT.theta) / ONE;
         EVT.theta += 0x20;
      }

      evt_s0 = EVT.cam;
      if (evt_s0->functionIndex == EVTF_CAMERA_TBD_026) {
         evt_s0->d.evtf026.zoom = gCameraZoom.vz;
      }

      evt->state2++;
      if (evt->state2 == 61) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 183
void Evtf183_Fx_TBD(EvtData *evt) {
   static s16 spikedBallAnimData[] = {4, GFX_SPIKED_BALL_1, 2, GFX_SPIKED_BALL_2,
                                      2, GFX_NULL,          1, GFX_NULL};

   EvtData *evt_s0;
   s32 timer;

   switch (evt->state) {
   case 0:
      EVT.todo_x26 = 0x100 - rand() % 0x200;
      EVT.todo_x28 = 0x180 + rand() % 0x180;
      EVT.todo_x2a = 0x100 - rand() % 0x200;

      evt_s0 = Evt_GetUnused();
      EVT.sprite = evt_s0;
      evt_s0->functionIndex = EVTF_NOOP;
      evt_s0->d.sprite.animData = spikedBallAnimData;
      evt_s0->d.sprite.animInitialized = 0;
      evt_s0->d.sprite.boxIdx = 4;

      evt->state++;

   // fallthrough
   case 1:
      evt_s0 = EVT.sprite;
      timer = EVT.timer;
      evt_s0->x1.n =
          evt->x1.n + (EVT.todo_x26 * rsin(timer * 0x20) >> 11) + (timer * EVT.dx / 0x40);
      evt_s0->z1.n =
          evt->z1.n + (EVT.todo_x2a * rsin(timer * 0x20) >> 11) + (timer * EVT.dz / 0x40);
      evt_s0->y1.n =
          evt->y1.n + (EVT.todo_x28 * rsin(timer * 0x20) >> 11) + (timer * EVT.dy / 0x40);
      UpdateEvtAnimation(evt_s0);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s0, 0);

      EVT.timer++;
      if (EVT.timer == 60) {
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_FX_TBD_119;
         evt_s0->d.evtf119.type = 0;
         evt_s0->d.evtf119.boxIdx = 3;
         evt_s0->d.evtf119.clut = 4;
         evt_s0->d.evtf119.semiTrans = 0;
         evt_s0->d.evtf119.once = 1;
         evt_s0->d.evtf119.radius = 0;
         evt_s0->x1.n = evt->x1.n + EVT.dx;
         evt_s0->z1.n = evt->z1.n + EVT.dz;
         evt_s0->y1.n = evt->y1.n + EVT.dy + 0xd0;
      } else if (EVT.timer == 65) {
         evt_s0->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_BOUNCE_ZOOM;
         evt_s0->d.evtf024.soft = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 184
void Evtf184_SpellFx1_Avalanche(EvtData *evt) {
   EvtData *evt_a0;
   EvtData *boulder;
   s16 dy;

   switch (evt->state) {
   case 0:
      evt_a0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_a0->x1.n;
      evt->y1.n = evt_a0->y1.n;
      evt->z1.n = evt_a0->z1.n;

      boulder = Evt_GetUnused();
      boulder->functionIndex = EVTF_SPELL_FX_AVALANCHE_BOULDER;
      boulder->x1.n = evt->x1.n;
      boulder->z1.n = evt->z1.n;
      boulder->y1.n = evt->y1.n + 0x300;
      boulder->d.evtf211.todo_x24 = 0x180;
      boulder->d.evtf211.todo_x4c = 0;
      boulder->d.evtf211.rotY = 0;
      boulder->d.evtf211.rotZ = 0;
      boulder->d.evtf211.rotX = 0;
      EVT.boulder = boulder;

      evt->state++;

   // fallthrough
   case 1:
      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[5] = evt_a0;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[4] = evt_a0;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[3] = evt_a0;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[2] = evt_a0;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[1] = evt_a0;

      evt_a0 = Evt_GetUnused();
      evt_a0->functionIndex = EVTF_SPELL_FX_AVALANCHE_ROCK;
      evt_a0->d.evtf185.parent = evt;
      EVT.rocks[0] = evt_a0;

      evt->state++;

   // fallthrough
   case 2:
      gCameraZoom.vz += 3;
      gCameraRotation.vy += 0x10;

      boulder = EVT.boulder;
      boulder->d.evtf211.todo_x4c = EVT.timer * 3;
      boulder->d.evtf211.rotY += 12;
      boulder->d.evtf211.rotZ -= 7;
      boulder->d.evtf211.rotX += 3;
      boulder->x1.n = evt->x1.n;
      boulder->z1.n = evt->z1.n;
      boulder->y1.n = evt->y1.n + 0x300;

      EVT.timer++;
      if (EVT.timer == 80) {
         evt_a0 = Evt_GetUnused();
         evt_a0->functionIndex = EVTF_CAMERA_TBD_026;
         evt_a0->d.evtf026.target = boulder;
         evt_a0->d.evtf026.type = 3;
         EVT.cam = evt_a0;
      }
      if (EVT.timer >= 90) {
         evt_a0 = EVT.rocks[5];
         evt_a0->state = 99;
         evt_a0 = EVT.rocks[4];
         evt_a0->state = 99;
         evt_a0 = EVT.rocks[3];
         evt_a0->state = 99;
         evt_a0 = EVT.rocks[2];
         evt_a0->state = 99;
         evt_a0 = EVT.rocks[1];
         evt_a0->state = 99;
         evt_a0 = EVT.rocks[0];
         evt_a0->state = 99;

         evt->state++;
         evt_a0 = EVT.cam;
         if (evt_a0->functionIndex == EVTF_CAMERA_TBD_026) {
            evt_a0->functionIndex = EVTF_NULL;
         }
      }

      break;

   case 3:
      boulder = EVT.boulder;
      boulder->d.evtf211.todo_x4c = 0x10e - EVT.todo_x26 / 3;
      boulder->d.evtf211.rotY += 12;
      boulder->d.evtf211.rotZ -= 7;
      boulder->d.evtf211.rotX += 3;
      boulder->x1.n = evt->x1.n;
      boulder->z1.n = evt->z1.n;
      boulder->y1.n = evt->y1.n + 0x300 + EVT.todo_x26 * 0x20;

      gCameraRotation.vx += (0x400 - gCameraRotation.vx) >> 5;
      gCameraRotation.vy += 0x10;
      gCameraPos.vx = -(boulder->x1.n >> 3);
      gCameraPos.vz = -(boulder->z1.n >> 3);
      gCameraPos.vy = boulder->y1.n >> 3;

      EVT.todo_x26 += 2;
      if (EVT.todo_x26 == 180) {
         EVT.todo_x2c = 45;
         evt_a0 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
         EVT.dx = evt_a0->x1.n - evt->x1.n;
         EVT.dz = evt_a0->z1.n - evt->z1.n;
         dy = boulder->y1.n - evt_a0->y1.n;
         EVT.todo_x2a = dy;
         EVT.todo_x38 = dy;
         evt->x1.n = evt_a0->x1.n;
         evt->z1.n = evt_a0->z1.n;
         evt->state++;
      }
      break;

   case 4:
      boulder = EVT.boulder;
      boulder->d.evtf211.todo_x4c = 0x10e - EVT.todo_x26 / 3;
      boulder->d.evtf211.rotY += 12;
      boulder->d.evtf211.rotZ -= 7;
      boulder->d.evtf211.rotX += 3;
      EVT.todo_x34 = (EVT.dx * (EVT.todo_x38 - EVT.todo_x3a)) / EVT.todo_x38;
      EVT.todo_x36 = (EVT.dz * (EVT.todo_x38 - EVT.todo_x3a)) / EVT.todo_x38;
      EVT.todo_x3a = EVT.todo_x2a * rcos((0x2d - EVT.todo_x2c) * 0x1a) / ONE;
      boulder->y1.n = evt->y1.n + EVT.todo_x3a;
      EVT.todo_x26 -= 4;
      AddEvtPrim6(gGraphicsPtr->ot, boulder, 0);

      gCameraPos.vx = -(boulder->x1.n >> 3);
      gCameraPos.vz = -(boulder->z1.n >> 3);
      gCameraPos.vy = boulder->y1.n >> 3;
      gCameraRotation.vy += 0x10;

      EVT.todo_x2c--;
      if (EVT.todo_x2c == -1) {
         evt->state++;
      } else if (EVT.todo_x2c == 6) {
         gSignal3 = 1;
      }
      break;

   case 5:
      boulder = EVT.boulder;
      boulder->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 185
void Evtf185_SpellFx_Avalanche_Rock(EvtData *evt) {
   static s16 rockAnimData[] = {5, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3,
                                2, GFX_ROCK_4, 2, GFX_NULL,   1, GFX_NULL};

   EvtData *evt_s0;

   switch (evt->state) {
   case 0:
      evt_s0 = EVT.parent;
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;
      evt->y1.n = evt_s0->y1.n;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      evt_s0->d.sprite.animData = rockAnimData;
      EVT.sprite = evt_s0;

   // fallthrough
   case 1:
      evt_s0 = EVT.sprite;
      evt_s0->d.sprite.animInitialized = 0;
      EVT.height = 0;
      EVT.theta = rand() % ANGLE_360_DEGREES;
      EVT.velocity = rand() % 0x3c + 0x32;
      evt->state++;
      break;

   case 2:
      evt_s0 = EVT.sprite;
      evt_s0->x1.n = evt->x1.n + 0x60 * rcos(EVT.theta) / ONE;
      evt_s0->z1.n = evt->z1.n + 0x60 * rsin(EVT.theta) / ONE;
      evt_s0->y1.n = evt->y1.n + EVT.height;
      UpdateEvtAnimation(evt_s0);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s0, 0);
      EVT.height += EVT.velocity;
      if (EVT.height >= 0x300) {
         evt->state = 1;
      }
      break;

   case 99:
      evt_s0 = EVT.sprite;
      evt_s0->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 187
void Evtf187_Fx_TBD(EvtData *evt) {
   EvtData *fx;

   fx = CreatePositionedEvt(evt, EVTF_FX_TBD_186);
   fx->d.evtf186.fxType = 2;
   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 186
void Evtf186_Fx_TBD(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;

      fx = CreatePositionedEvt(evt, EVTF_FX_TBD_140 + EVT.fxType);
      fx->d.evtf140.clut = 3;

      fx = Evt_GetUnused();
      fx->functionIndex = EVTF_BOUNCE_ZOOM;

      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 190
void Evtf190_Fx_TBD(EvtData *evt) {
   // Unused? Maybe a single-target version of Magic Arrow
   EvtData *unitSprite;
   EvtData *targetSprite;
   EvtData *arrow;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->y1.n = unitSprite->y1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->state++;

   // fallthrough
   case 1:
      arrow = Evt_GetUnused();
      arrow->functionIndex = EVTF_SPELL_FX_MAGIC_ARROW_ARROW;
      arrow->x1.n = evt->x1.n;
      arrow->y1.n = evt->y1.n + 0x40;
      arrow->z1.n = evt->z1.n;

      targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      arrow->d.evtf191.dx = targetSprite->x1.n - evt->x1.n;
      arrow->d.evtf191.dz = targetSprite->z1.n - evt->z1.n;
      arrow->d.evtf191.dy = (targetSprite->y1.n - evt->y1.n) + 0x40;

      evt->state3++;
      if (evt->state3 == 17) {
         evt->state++;
      }
      break;

   case 2:
      gCameraZoom.vz += 10;
      gCameraRotation.vx += 1;
      if (evt->state2 == 22) {
         gSignal3 = 1;
      } else if (evt->state2 == 34) {
         evt->functionIndex = EVTF_NULL;
      }
      evt->state2++;
      break;
   }
}

#undef EVTF
#define EVTF 199
void Evtf199_SpellFx1_MagicArrow(EvtData *evt) {
   EvtData *casterSprite;
   EvtData *targetSprite;
   EvtData *arrow;
   u8 *p;
   s16 i;

   switch (evt->state) {
   case 0:
      casterSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.unitSprite = casterSprite;
      evt->state++;

   // fallthrough
   case 1:
      casterSprite = EVT.unitSprite;

      p = gTargetCoords;
      for (i = 0; *p != 0xff && i < 15; i++) {
         arrow = Evt_GetUnused();
         arrow->functionIndex = EVTF_SPELL_FX_MAGIC_ARROW_ARROW;
         arrow->x1.n = casterSprite->x1.n;
         arrow->y1.n = casterSprite->y1.n + 0x40;
         arrow->z1.n = casterSprite->z1.n;
         targetSprite = GetUnitSpriteAtPosition(p[1], p[0]);
         arrow->d.evtf191.dx = targetSprite->x1.n - casterSprite->x1.n;
         arrow->d.evtf191.dz = targetSprite->z1.n - casterSprite->z1.n;
         arrow->d.evtf191.dy = (targetSprite->y1.n - casterSprite->y1.n) + 0x40;
         p += 2;
      }

      gCameraZoom.vz += 9;
      gCameraRotation.vx += 8;
      gCameraRotation.vy += 0x10;

      if (Evt_CountUnused() < 50) {
         evt->state++;
      }

      evt->state2++;
      if (evt->state2 == 17) {
         evt->state++;
      }
      break;

   case 2:
      gCameraZoom.vz += 9;
      gCameraRotation.vx += 8;
      gCameraRotation.vy += 0x10;

      evt->state2++;
      if (evt->state2 == 35) {
         evt->functionIndex = EVTF_NULL;
         gSignal3 = 1;
      }
      break;
   }
}

#undef EVTF
#define EVTF 191
void Evtf191_SpellFx_MagicArrow_Arrow(EvtData *evt) {
   EvtData *sprite;
   POLY_FT4 *poly;
   s32 timer, i, intensity;
   s16 a;

   switch (evt->state) {
   case 0:
      if (EVT.clut_unused == 0) {
         EVT.clut_unused = 3;
      }
      EVT.todo_x26 = 0x280 - rand() % 0x500;
      EVT.todo_x28 = 0x180 + rand() % 0x180;
      EVT.todo_x2a = 0x280 - rand() % 0x500;
      evt->state++;
      break;

   case 1:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.gfxIdx = GFX_ARROW;
      sprite->d.sprite.semiTrans = 0;

      timer = EVT.timer;
      sprite->d.sprite.coords[0].x =
          evt->x1.n + (EVT.todo_x26 * rsin(timer * 0x40) >> 12) + timer * EVT.dx / 0x20;
      sprite->d.sprite.coords[0].z =
          evt->z1.n + (EVT.todo_x2a * rsin(timer * 0x40) >> 12) + timer * EVT.dz / 0x20;
      sprite->d.sprite.coords[0].y =
          evt->y1.n + (EVT.todo_x28 * rsin(timer * 0x40) >> 12) + timer * EVT.dy / 0x20;

      sprite->d.sprite.coords[1].x =
          evt->x1.n + (EVT.todo_x26 * rsin((timer + 2) * 0x40) >> 12) + (timer + 2) * EVT.dx / 0x20;
      sprite->d.sprite.coords[1].z =
          evt->z1.n + (EVT.todo_x2a * rsin((timer + 2) * 0x40) >> 12) + (timer + 2) * EVT.dz / 0x20;
      sprite->d.sprite.coords[1].y =
          evt->y1.n + (EVT.todo_x28 * rsin((timer + 2) * 0x40) >> 12) + (timer + 2) * EVT.dy / 0x20;

      sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
      sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[0].y + 0x80;
      sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;
      sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;
      sprite->d.sprite.coords[3].y = sprite->d.sprite.coords[1].y + 0x80;

      sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x) / 2;
      sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[1].z) / 2;
      sprite->y1.n = (sprite->d.sprite.coords[0].y + sprite->d.sprite.coords[1].y +
                      sprite->d.sprite.coords[2].y + sprite->d.sprite.coords[3].y) /
                     4;

      AddEvtPrim3(gGraphicsPtr->ot, sprite);

      if (timer >= 4) {
         i = 4;
         sprite->d.sprite.semiTrans = 2;
         for (; i > 0; i--) {
            a = timer - (i + 1);

            sprite->d.sprite.coords[0].x =
                evt->x1.n + (EVT.todo_x26 * rsin(a * 0x40) >> 12) + a * EVT.dx / 0x20;
            sprite->d.sprite.coords[0].z =
                evt->z1.n + (EVT.todo_x2a * rsin(a * 0x40) >> 12) + a * EVT.dz / 0x20;
            sprite->d.sprite.coords[0].y =
                evt->y1.n + (EVT.todo_x28 * rsin(a * 0x40) >> 12) + a * EVT.dy / 0x20;

            sprite->d.sprite.coords[1].x =
                evt->x1.n + (EVT.todo_x26 * rsin((a + 2) * 0x40) >> 12) + (a + 2) * EVT.dx / 0x20;
            sprite->d.sprite.coords[1].z =
                evt->z1.n + (EVT.todo_x2a * rsin((a + 2) * 0x40) >> 12) + (a + 2) * EVT.dz / 0x20;
            sprite->d.sprite.coords[1].y =
                evt->y1.n + (EVT.todo_x28 * rsin((a + 2) * 0x40) >> 12) + (a + 2) * EVT.dy / 0x20;

            sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
            sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
            sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[0].y + 0x80;
            sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;
            sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;
            sprite->d.sprite.coords[3].y = sprite->d.sprite.coords[1].y + 0x80;

            sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x) / 2;
            sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[1].z) / 2;
            sprite->y1.n = (sprite->d.sprite.coords[0].y + sprite->d.sprite.coords[1].y +
                            sprite->d.sprite.coords[2].y + sprite->d.sprite.coords[3].y) /
                           4;

            AddEvtPrim3(gGraphicsPtr->ot, sprite);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            intensity = 0x40 / (i + 1);
            setRGB0(poly, intensity, intensity, intensity);
         }
      }

      EVT.timer++;
      if (EVT.timer == 31) {
         evt->functionIndex = EVTF_NULL;
      }
      sprite->functionIndex = EVTF_NULL;
      break;
   }
}
