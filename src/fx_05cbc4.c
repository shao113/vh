#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

#undef EVTF
#define EVTF 173
void Evtf173_FireGem_Beam(EvtData *evt) {
   s16 x, y, z;
   s16 a;
   s32 i;
   EvtData *evt_s2;
   EvtData *evt_v1;
   SVECTOR positions[6];
   s16 local_38[6][2] = {{0, 1}, {3, 4}, {0, 2}, {3, 5}, {2, 1}, {5, 4}};

   x = evt->x1.n;
   y = evt->y1.n;
   z = evt->z1.n;

   switch (evt->state) {
   case 0:
      EVT.todo_x2a = 0x800;
      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.gfxIdx = GFX_COLOR_1 + EVT.todo_x26;
      evt_s2->d.sprite.semiTrans = 1;

      a = rsin(EVT.todo_x24);
      positions[0].vx = 0x140 * rcos(a) >> 12;
      positions[0].vz = 0x140 * rsin(a) >> 12;
      positions[1].vx = 0x140 * rcos(a + 0x555) >> 12;
      positions[1].vz = 0x140 * rsin(a + 0x555) >> 12;
      positions[2].vx = 0x140 * rcos(a + 0xaaa) >> 12;
      positions[2].vz = 0x140 * rsin(a + 0xaaa) >> 12;
      positions[3].vx = 0x140 * rcos(a + 0x2ab) >> 12;
      positions[3].vz = 0x140 * rsin(a + 0x2ab) >> 12;
      positions[4].vx = 0x140 * rcos(a + 0x7ff) >> 12;
      positions[4].vz = 0x140 * rsin(a + 0x7ff) >> 12;
      positions[5].vx = 0x140 * rcos(a + 0xd55) >> 12;
      positions[5].vz = 0x140 * rsin(a + 0xd55) >> 12;
      EVT.todo_x24 += 0x20;

      evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + positions[0].vx;
      evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + positions[0].vz;
      // y += EVT.todo_x2a;
      evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y = evt_s2->d.sprite.coords[2].y =
          evt_s2->d.sprite.coords[3].y = (y + EVT.todo_x2a);
      evt_s2->d.sprite.coords[2].x = x + positions[1].vx;
      evt_s2->d.sprite.coords[2].z = z + positions[1].vz;
      evt_s2->d.sprite.coords[3].x = x + positions[2].vx;
      evt_s2->d.sprite.coords[3].z = z + positions[2].vz;
      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

      evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + positions[3].vx;
      evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + positions[3].vz;
      evt_s2->d.sprite.coords[2].x = x + positions[4].vx;
      evt_s2->d.sprite.coords[2].z = z + positions[4].vz;
      evt_s2->d.sprite.coords[3].x = x + positions[5].vx;
      evt_s2->d.sprite.coords[3].z = z + positions[5].vz;
      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

      if ((EVT.todo_x2c == 0) && (++EVT.todo_x2e % 3 == 0)) {
         EVT.todo_x26++;
         if (EVT.todo_x26 == 7) {
            EVT.todo_x2c++;
            EVT.todo_x26 = 6;
         }
      } else if ((EVT.todo_x2e % 3 == 0) && (--EVT.todo_x26 < 0)) {
         EVT.todo_x2c = 0;
         EVT.todo_x26 = 0;
      }

      EVT.todo_x2a -= 0x40;
      if (EVT.todo_x2a < 0) {
         EVT.todo_x2a = 0;
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->z1.n = evt->z1.n;
         EVT.fx = evt_v1;
         evt->state++;
      }

      evt_s2->functionIndex = EVTF_NULL;
      break;

   case 2:
      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.gfxIdx = GFX_COLOR_1 + EVT.todo_x26;
      evt_s2->d.sprite.semiTrans = 1;

      a = rsin(EVT.todo_x24);
      positions[0].vx = 0x140 * rcos(a) >> 12;
      positions[0].vz = 0x140 * rsin(a) >> 12;
      positions[1].vx = 0x140 * rcos(a + 0x555) >> 12;
      positions[1].vz = 0x140 * rsin(a + 0x555) >> 12;
      positions[2].vx = 0x140 * rcos(a + 0xaaa) >> 12;
      positions[2].vz = 0x140 * rsin(a + 0xaaa) >> 12;
      positions[3].vx = 0x140 * rcos(a + 0x2ab) >> 12;
      positions[3].vz = 0x140 * rsin(a + 0x2ab) >> 12;
      positions[4].vx = 0x140 * rcos(a + 0x7ff) >> 12;
      positions[4].vz = 0x140 * rsin(a + 0x7ff) >> 12;
      positions[5].vx = 0x140 * rcos(a + 0xd55) >> 12;
      positions[5].vz = 0x140 * rsin(a + 0xd55) >> 12;
      EVT.todo_x24 += 0x20;

      for (i = 0; i < 6; i++) {
         evt_s2->d.sprite.coords[0].x = x;
         evt_s2->d.sprite.coords[0].z = z;
         evt_s2->d.sprite.coords[0].y = evt->y1.n + EVT.todo_x28;
         evt_s2->d.sprite.coords[1].x = x;
         evt_s2->d.sprite.coords[1].z = z;
         evt_s2->d.sprite.coords[1].y = evt->y1.n + EVT.todo_x28;
         evt_s2->d.sprite.coords[2].x = x + positions[local_38[i][0]].vx;
         evt_s2->d.sprite.coords[2].z = z + positions[local_38[i][0]].vz;
         evt_s2->d.sprite.coords[2].y = evt->y1.n + EVT.todo_x2a;
         evt_s2->d.sprite.coords[3].x = x + positions[local_38[i][1]].vx;
         evt_s2->d.sprite.coords[3].z = z + positions[local_38[i][1]].vz;
         evt_s2->d.sprite.coords[3].y = evt->y1.n + EVT.todo_x2a;

         evt_s2->x1.n = (evt_s2->d.sprite.coords[0].x + evt_s2->d.sprite.coords[1].x +
                         evt_s2->d.sprite.coords[2].x + evt_s2->d.sprite.coords[3].x) >>
                        2;
         evt_s2->z1.n = (evt_s2->d.sprite.coords[0].z + evt_s2->d.sprite.coords[1].z +
                         evt_s2->d.sprite.coords[2].z + evt_s2->d.sprite.coords[3].z) >>
                        2;
         evt_s2->y1.n = evt_s2->d.sprite.coords[3].y;

         AddEvtPrim3(gGraphicsPtr->ot, evt_s2);
         if ((i + 1) % 2 != 0) {
            evt_s2->d.sprite.gfxIdx++;
         }
      }

      evt_s2->d.sprite.coords[0].x = x + positions[0].vx;
      evt_s2->d.sprite.coords[0].z = z + positions[0].vz;
      evt_s2->d.sprite.coords[0].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[1].x = x + positions[0].vx;
      evt_s2->d.sprite.coords[1].z = z + positions[0].vz;
      evt_s2->d.sprite.coords[1].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[2].x = x + positions[1].vx;
      evt_s2->d.sprite.coords[2].z = z + positions[1].vz;
      evt_s2->d.sprite.coords[2].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[3].x = x + positions[2].vx;
      evt_s2->d.sprite.coords[3].z = z + positions[2].vz;
      evt_s2->d.sprite.coords[3].y = evt->y1.n + EVT.todo_x2a;
      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

      evt_s2->d.sprite.coords[0].x = x + positions[3].vx;
      evt_s2->d.sprite.coords[0].z = z + positions[3].vz;
      evt_s2->d.sprite.coords[0].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[1].x = x + positions[3].vx;
      evt_s2->d.sprite.coords[1].z = z + positions[3].vz;
      evt_s2->d.sprite.coords[1].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[2].x = x + positions[4].vx;
      evt_s2->d.sprite.coords[2].z = z + positions[4].vz;
      evt_s2->d.sprite.coords[2].y = evt->y1.n + EVT.todo_x2a;
      evt_s2->d.sprite.coords[3].x = x + positions[5].vx;
      evt_s2->d.sprite.coords[3].z = z + positions[5].vz;
      evt_s2->d.sprite.coords[3].y = evt->y1.n + EVT.todo_x2a;
      AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

      evt_s2->functionIndex = EVTF_NULL;

      switch (evt->state3) {
      case 0:
         if ((EVT.todo_x2c == 0) && (++EVT.todo_x2e % 3 == 0)) {
            EVT.todo_x26++;
            if (EVT.todo_x26 == 7) {
               EVT.todo_x2c++;
               EVT.todo_x26 = 6;
            }
         } else if ((EVT.todo_x2e % 3 == 0) && (--EVT.todo_x26 < 0)) {
            EVT.todo_x2c = 0;
            EVT.todo_x26 = 0;
         }
         EVT.todo_x28 += 0xc0;
         if (EVT.todo_x28 > 0x800) {
            EVT.todo_x28 = 0x800;
            evt->state3++;
         }
         break;

      case 1:
         if ((EVT.todo_x2c == 0) && (++EVT.todo_x2e % 3 == 0)) {
            EVT.todo_x26++;
            if (EVT.todo_x26 == 7) {
               EVT.todo_x2c++;
               EVT.todo_x26 = 6;
            }
         } else if ((EVT.todo_x2e % 3 == 0) && (--EVT.todo_x26 < 0)) {
            EVT.todo_x2c = 0;
            EVT.todo_x26 = 0;
            EVT.todo_x30++;
         }
         if (EVT.todo_x30 == 2) {
            evt->state3++;
            EVT.todo_x28 = 0;
         }
         break;

      case 2:
         EVT.todo_x28 = (0x1000U - rsin(EVT.todo_x30)) >> 1;
         if ((EVT.todo_x2c == 0) && (++EVT.todo_x2e % 3 == 0)) {
            EVT.todo_x26++;
            if (EVT.todo_x26 == 7) {
               EVT.todo_x2c++;
               EVT.todo_x26 = 6;
            }
         } else if ((EVT.todo_x2e % 3 == 0) && (--EVT.todo_x26 < 0)) {
            EVT.todo_x2c = 0;
            EVT.todo_x26 = 0;
         }
         EVT.todo_x30 += 0x20;
         if (EVT.todo_x30 > 0x400) {
            evt->functionIndex = EVTF_NULL;
            evt_v1 = EVT.fx;
            evt_v1->state = 99;
         }
         break;
      }

      break;
   }
}

void func_8006D1AC(EvtData *fx, EvtData *sprite, s16 param_3, s16 param_4, s16 param_5, s16 param_6,
                   s16 param_7, s16 param_8, s16 param_9) {

   sprite->d.sprite.coords[0].x = fx->x1.n + param_3 + 0x100;
   sprite->d.sprite.coords[1].x = fx->x1.n + param_3 - 0x100;
   if (param_9) { // 1
      sprite->d.sprite.coords[2].x = fx->x1.n + param_6 + 0x110 - (rand() % 0x20);
      sprite->d.sprite.coords[3].x = fx->x1.n + param_6 - 0xf0 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].x = fx->x1.n + param_6 + 0x100;
      sprite->d.sprite.coords[3].x = fx->x1.n + param_6 - 0x100;
   }
   sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = fx->y1.n + param_4;
   sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = fx->y1.n + param_7;
   sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z = fx->z1.n + param_5;
   if (param_9) { // 2
      sprite->d.sprite.coords[2].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
      sprite->d.sprite.coords[3].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].z = fx->z1.n + param_8;
      sprite->d.sprite.coords[3].z = fx->z1.n + param_8;
   }
   sprite->d.sprite.semiTrans = 0;
   AddEvtPrim4(gGraphicsPtr->ot, sprite);

   sprite->d.sprite.coords[0].x = fx->x1.n - (param_3 / 2) + 0x100;
   sprite->d.sprite.coords[1].x = fx->x1.n - (param_3 / 2) - 0x100;
   if (param_9) { // 3
      sprite->d.sprite.coords[2].x = fx->x1.n + param_6 + 0x110 - (rand() % 0x20);
      sprite->d.sprite.coords[3].x = fx->x1.n + param_6 - 0xf0 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].x = fx->x1.n - (param_6 / 2) + 0x100;
      sprite->d.sprite.coords[3].x = fx->x1.n - (param_6 / 2) - 0x100;
   }
   sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = fx->y1.n + param_4;
   sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = fx->y1.n + param_7;
   sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z = fx->z1.n - param_5 / 2;
   if (param_9) { // 4
      sprite->d.sprite.coords[2].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
      sprite->d.sprite.coords[3].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[3].z = fx->z1.n - param_8 / 2;
   }
   sprite->d.sprite.semiTrans = 4;
   AddEvtPrim4(gGraphicsPtr->ot, sprite);

   sprite->d.sprite.coords[0].x = fx->x1.n - param_3 + 0x100;
   sprite->d.sprite.coords[1].x = fx->x1.n - param_3 - 0x100;
   if (param_9) { // 5
      sprite->d.sprite.coords[2].x = fx->x1.n + param_6 + 0x110 - (rand() % 0x20);
      sprite->d.sprite.coords[3].x = fx->x1.n + param_6 - 0xf0 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].x = fx->x1.n - param_6 + 0x100;
      sprite->d.sprite.coords[3].x = fx->x1.n - param_6 - 0x100;
   }
   sprite->d.sprite.coords[0].y = sprite->d.sprite.coords[1].y = fx->y1.n + param_4;
   do {
      // FIXME: This do-while fixes the reg-swaps, but should try to identify any actual macros.
      sprite->d.sprite.coords[2].y = sprite->d.sprite.coords[3].y = fx->y1.n + param_7;
   } while (0);
   sprite->d.sprite.coords[0].z = sprite->d.sprite.coords[1].z = fx->z1.n - param_5;
   if (param_9) { // 6
      sprite->d.sprite.coords[2].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
      sprite->d.sprite.coords[3].z = fx->z1.n + param_8 + 0x10 - (rand() % 0x20);
   } else {
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[3].z = fx->z1.n - param_8;
   }
   sprite->d.sprite.semiTrans = 1;
   AddEvtPrim4(gGraphicsPtr->ot, sprite);
}

// TBD: Which anims use those wiggly things?
static s16 sAnimData_800fecfc[20] = {5, GFX_TBD_278, 1, GFX_TBD_279, 1, GFX_TBD_280, 1, GFX_TBD_281,
                                     1, GFX_TBD_282, 1, GFX_TBD_283, 1, GFX_TBD_284, 1, GFX_TBD_285,
                                     1, GFX_NULL,    1, GFX_NULL};

#undef EVTF
#define EVTF 147
void Evtf147_Fx_TBD(EvtData *evt) {
   static s16 lightningAnimData[64] = {
       4, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4,
       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2,
       2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5,
       2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6, 2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8,
       2, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4,
       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_NULL,        0, GFX_NULL};

   EvtData *sprite;

   switch (evt->state) {
   case 0:
      evt->y1.n += 0x400;

      EVT.todo_x4c = 0x80 - (rand() % 0x100);
      EVT.todo_x50 = 0x80 - (rand() % 0x100);
      EVT.todo_x4e = -0x400;
      EVT.todo_x34 = EVT.todo_x4c / 4;
      EVT.todo_x36 = EVT.todo_x4e / 4;
      EVT.todo_x38 = EVT.todo_x50 / 4;
      EVT.todo_x3a = EVT.todo_x34 * 2;
      EVT.todo_x3c = EVT.todo_x36 * 2;
      EVT.todo_x3e = EVT.todo_x38 * 2;
      EVT.todo_x40 = EVT.todo_x34 * 3;
      EVT.todo_x42 = EVT.todo_x36 * 3;
      EVT.todo_x44 = EVT.todo_x38 * 3;

      if (EVT.clut == 0) {
         EVT.clut = 3;
      }

      sprite = Evt_GetUnused();
      EVT.sprite = sprite;
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = lightningAnimData;
      sprite->d.sprite.animInitialized = 0;
      sprite->d.sprite.clut = EVT.clut;

      evt->state++;
      break;

   case 1:
      sprite = EVT.sprite;

      EVT.todo_x34 += 0x40 - (rand() % 0x80);
      EVT.todo_x36 += 8 - (rand() % 0x10);
      EVT.todo_x38 += 0x40 - (rand() % 0x80);
      EVT.todo_x3a += 0x40 - (rand() % 0x80);
      EVT.todo_x3c += 8 - (rand() % 0x10);
      EVT.todo_x3e += 0x40 - (rand() % 0x80);
      EVT.todo_x40 += 0x40 - (rand() % 0x80);
      EVT.todo_x42 += 8 - (rand() % 0x10);
      EVT.todo_x44 += 0x40 - (rand() % 0x80);

      UpdateEvtAnimation(sprite);
      func_8006D1AC(evt, sprite, 0, 0, 0, EVT.todo_x34, EVT.todo_x36, EVT.todo_x38, 0);
      func_8006D1AC(evt, sprite, EVT.todo_x34, EVT.todo_x36, EVT.todo_x38, EVT.todo_x3a,
                    EVT.todo_x3c, EVT.todo_x3e, 0);
      func_8006D1AC(evt, sprite, EVT.todo_x3a, EVT.todo_x3c, EVT.todo_x3e, EVT.todo_x40,
                    EVT.todo_x42, EVT.todo_x44, 0);
      func_8006D1AC(evt, sprite, EVT.todo_x40, EVT.todo_x42, EVT.todo_x44, EVT.todo_x4c,
                    EVT.todo_x4e, EVT.todo_x50, 1);

      if (sprite->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void func_8006DCD8(EvtData *deltaMirage) {
   s32 i;
   s16 *p;
   s16 current;
   s16 a, b, c, d, e;
   s16 x_1, z_1, x_2, z_2, x_3, z_3;
   POLY_FT4 *poly;
   EvtData *sprite;

   if (deltaMirage->state == 3) {
      a = 0x100 + (0x400 * rsin(ANGLE_90_DEGREES - deltaMirage->d.evtf156.todo_x36 * 2) >> 12);
      b = 0;
      c = 0x200 * rcos(ANGLE_90_DEGREES - deltaMirage->d.evtf156.todo_x36 * 2) >> 12;
   } else {
      a = 0x100;
      b = 0;
      c = 0x200;
   }

   ApplyMaskEffect(452 << 2, 400, 32, 32, 432 << 2, 256, deltaMirage->d.evtf156.timer % 64, 0,
                   GFX_TBD_25, 0);

   sprite = Evt_GetUnused();
   sprite->functionIndex = EVTF_NOOP;
   sprite->d.sprite.gfxIdx = GFX_TBD_25;
   sprite->d.sprite.clut = 4;
   sprite->d.sprite.semiTrans = 1;

   p = &deltaMirage->d.evtf156.todo_x26;
   for (i = 0; i < 3 - deltaMirage->state2; i++, p++) {
      current = *p;

      switch (i) {
      case 2:
         d = deltaMirage->d.evtf156.todo_x24 + 0xaaa;
         e = deltaMirage->d.evtf156.todo_x24;
         break;
      case 1:
         d = deltaMirage->d.evtf156.todo_x24 + 0x555;
         e = deltaMirage->d.evtf156.todo_x24 + 0xaaa;
         break;
      case 0:
      default:
         d = deltaMirage->d.evtf156.todo_x24;
         e = deltaMirage->d.evtf156.todo_x24 + 0x555;
         break;
      }

      x_1 = c * rcos(d) >> 12;
      z_1 = c * rsin(d) >> 12;
      x_2 = (c * rcos(e) >> 12) - x_1;
      z_2 = (c * rsin(e) >> 12) - z_1;
      x_3 = x_2 * rsin(current) >> 12;
      z_3 = z_2 * rsin(current) >> 12;

      sprite->d.sprite.coords[0].x = deltaMirage->x1.n + x_1;
      sprite->d.sprite.coords[1].x = sprite->d.sprite.coords[0].x + x_3;
      sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
      sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;

      sprite->d.sprite.coords[0].z = deltaMirage->z1.n + z_1;
      sprite->d.sprite.coords[1].z = sprite->d.sprite.coords[0].z + z_3;
      sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
      sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;

      sprite->d.sprite.coords[0].y = deltaMirage->y1.n + a;
      sprite->d.sprite.coords[2].y = deltaMirage->y1.n + b;
      sprite->d.sprite.coords[1].y = sprite->d.sprite.coords[0].y;
      sprite->d.sprite.coords[3].y = sprite->d.sprite.coords[2].y;

      sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x) >> 1;
      sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[1].z) >> 1;
      sprite->y1.n = (sprite->d.sprite.coords[0].y + sprite->d.sprite.coords[2].y) >> 1;

      AddEvtPrim3(gGraphicsPtr->ot, sprite);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);
   }

   sprite->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 156
void Evtf156_DeltaMirage_FX1(EvtData *evt) {
   s32 i;
   EvtData *evt_v1;
   EvtData *unitSprite;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt->y1.n = GetTerrainElevation(gTargetZ, gTargetX);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      EVT.todo_x24 = gCameraRotation.vy - 0x656;

      evt_v1 = Evt_GetUnused();
      evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_v1->d.evtf026.target = unitSprite;

      evt->state++;
      evt->state2 = 2;

   // fallthrough
   case 1:
      func_8006DCD8(evt);

      switch (evt->state2) {
      case 0:
         EVT.todo_x2a += 0x20;
         if (EVT.todo_x2a > ANGLE_90_DEGREES) {
            EVT.todo_x2a = ANGLE_90_DEGREES;
            evt->state++;
         }
         break;
      case 1:
         EVT.todo_x28 += 0x20;
         if (EVT.todo_x28 > ANGLE_90_DEGREES) {
            EVT.todo_x28 = ANGLE_90_DEGREES;
            evt->state2--;
         }
         break;
      case 2:
         EVT.todo_x26 += 0x20;
         if (EVT.todo_x26 > ANGLE_90_DEGREES) {
            EVT.todo_x26 = ANGLE_90_DEGREES;
            evt->state2--;
         }
         break;
      }

      EVT.timer++;
      if (gLightColor.r > 0x20) {
         gLightColor.r -= 4;
         gLightColor.g -= 4;
         gLightColor.b -= 4;
      }

      break;

   case 2:
      for (i = 0; i < 0x40; i++) {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_DELTA_MIRAGE_RAY;
         evt_v1->d.evtf157.parent = evt;
      }
      EVT.todo_x36 = 0x200;
      evt->state++;

   // fallthrough
   case 3:
      func_8006DCD8(evt);
      EVT.todo_x24 += 0x10;
      EVT.timer++;
      EVT.todo_x36 -= 4;
      if (EVT.todo_x36 == 0x28) {
         gSignal3 = 1;
      }
      if (EVT.todo_x36 < 0x20) {
         gLightColor.r += 12;
         gLightColor.g += 12;
         gLightColor.b += 12;
      }
      if (EVT.todo_x36 < 0) {
         evt->functionIndex = EVTF_NULL;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
      }
      break;
   }
}

#undef EVTF
#define EVTF 157
void Evtf157_DeltaMirage_Ray(EvtData *evt) {
   EvtData *evt_s0;

   switch (evt->state) {
   case 0:
      evt_s0 = EVT.parent;
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;
      evt->y1.n = evt_s0->y1.n;

      EVT.todo_x24 = 0x400 + (rand() % 0x400);
      EVT.todo_x26 = rand() % 0x1000;
      EVT.todo_x28 = rand() % 0x1000;
      EVT.todo_x2a = rand() % 0x1000;
      EVT.todo_x2c = 0x60 - (rand() % 0xc1);
      EVT.todo_x2e = 0x60 - (rand() % 0xc1);
      EVT.todo_x30 = 0x60 - (rand() % 0xc1);

      evt->state++;

   // fallthrough
   case 1:
      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      evt_s0->d.sprite.gfxIdx = GFX_COLOR_14;
      evt_s0->d.sprite.clut = 4;
      evt_s0->d.sprite.semiTrans = 1;

      EVT.todo_x34 = EVT.todo_x24 * EVT.todo_x36 / 0x30;
      evt_s0->d.sprite.coords[0].x = evt->x1.n;
      evt_s0->d.sprite.coords[0].z = evt->z1.n;
      evt_s0->d.sprite.coords[0].y = evt->y1.n;
      evt_s0->d.sprite.coords[1].x = evt->x1.n;
      evt_s0->d.sprite.coords[1].z = evt->z1.n;
      evt_s0->d.sprite.coords[1].y = evt->y1.n;
      evt_s0->d.sprite.coords[2].x = evt->x1.n + EVT.todo_x34 * rcos(EVT.todo_x26) / ONE;
      evt_s0->d.sprite.coords[2].z = evt->z1.n + EVT.todo_x34 * rsin(EVT.todo_x28) / ONE;
      evt_s0->d.sprite.coords[2].y = evt->y1.n + EVT.todo_x34 * rsin(EVT.todo_x2a) / ONE;
      evt_s0->d.sprite.coords[3].x = evt->x1.n + EVT.todo_x34 * rcos(EVT.todo_x26 + 0x10) / ONE;
      evt_s0->d.sprite.coords[3].z = evt->z1.n + EVT.todo_x34 * rsin(EVT.todo_x28 + 0x10) / ONE;
      evt_s0->d.sprite.coords[3].y = evt->y1.n + EVT.todo_x34 * rsin(EVT.todo_x2a + 0x10) / ONE;

      AddEvtPrim4(gGraphicsPtr->ot, evt_s0);

      EVT.todo_x26 += EVT.todo_x2c;
      EVT.todo_x28 += EVT.todo_x2e;
      EVT.todo_x2a += EVT.todo_x30;

      evt_s0->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.todo_x32++;
         EVT.todo_x36++;
         if (EVT.todo_x36 == 0x31) {
            evt->state2++;
            EVT.todo_x36 = 0x30;
         }
         break;
      case 1:
         EVT.todo_x32++;
         if (EVT.todo_x32 == 0x81) {
            evt->state2++;
         }
         break;
      case 2:
         EVT.todo_x32++;
         EVT.todo_x36 -= 3;
         if (EVT.todo_x36 == 0) {
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 158
void Evtf158_Explosion_FX1(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *evt_s0;
   s32 spriteX, spriteY, spriteW, spriteH;
   s16 width, height;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt->x1.n = unitSprite->x1.n;
      evt->y1.n = unitSprite->y1.n;
      evt->z1.n = unitSprite->z1.n;
      EVT.unitSprite = unitSprite;
      unitSprite->d.sprite.hidden = 1;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s0->d.evtf026.target = unitSprite;
      evt_s0->d.evtf026.type = 3;

      evt->state++;

   // fallthrough
   case 1:
      width = 0x40 * rsin(EVT.timer * 0x30) / ONE;
      if (width == 0x40) {
         width = 0x3f;
      } else if (width == -0x40) {
         width = -0x3f;
      }
      height = 0x10 * rcos(EVT.timer * 0x30) / ONE;

      unitSprite = EVT.unitSprite;
      GetUnitSpriteVramRect(unitSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 432 << 2, 256, width, height,
                      GFX_TBD_25, 0);

      evt_s0 = Evt_GetUnused();
      CopyEvtData(unitSprite, evt_s0);
      evt_s0->d.sprite.hidden = 0;
      evt_s0->d.sprite.gfxIdx = GFX_TBD_25;
      evt_s0->d.sprite.clut = 3;
      evt_s0->d.sprite.semiTrans = 0;

      if (unitSprite->d.sprite.gfxIdx >= 21 && unitSprite->d.sprite.gfxIdx <= 30) {
         evt_s0->d.sprite.boxIdx = 1;
      } else {
         evt_s0->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt_s0, 0);
      evt_s0->functionIndex = EVTF_NULL;

      EVT.timer += 2;
      if (EVT.timer % 4 == 0) {
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_EXPLOSION_RAYS;
         evt_s0->x1.n = evt->x1.n;
         evt_s0->y1.n = evt->y1.n + 0x40;
         evt_s0->z1.n = evt->z1.n;
         evt_s0->d.evtf159.todo_x5a = EVT.timer;
      }
      if (EVT.timer == 150) {
         gSignal3 = 1;
      } else if (EVT.timer == 180) {
         evt->functionIndex = EVTF_NULL;
         unitSprite->d.sprite.hidden = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 159
void Evtf159_Explosion_Rays(EvtData *evt) {
   static s16 colorsAnimData[64] = {
       4, GFX_COLOR_1,  3, GFX_COLOR_2,  3, GFX_COLOR_3,  3, GFX_COLOR_4,  3, GFX_COLOR_5,
       3, GFX_COLOR_6,  3, GFX_COLOR_7,  3, GFX_COLOR_8,  3, GFX_COLOR_9,  3, GFX_COLOR_10,
       3, GFX_COLOR_11, 3, GFX_COLOR_12, 3, GFX_COLOR_13, 3, GFX_COLOR_12, 3, GFX_COLOR_13,
       3, GFX_COLOR_14, 3, GFX_COLOR_15, 3, GFX_COLOR_14, 3, GFX_COLOR_13, 3, GFX_COLOR_12,
       3, GFX_COLOR_11, 3, GFX_COLOR_10, 3, GFX_COLOR_9,  3, GFX_COLOR_8,  3, GFX_COLOR_7,
       3, GFX_COLOR_6,  3, GFX_COLOR_5,  3, GFX_COLOR_4,  3, GFX_COLOR_3,  3, GFX_COLOR_2,
       3, GFX_NULL,     1, GFX_NULL};

   EvtData *sprite;
   s32 i;
   s16 a, b;
   s16 angle, angleOfs;
   s16 h_1, h_2;
   s32 sum;

   switch (evt->state) {
   case 0:
      EVT.todo_x24 = rand() % 0x1000;
      EVT.todo_x26 = rand() % 0x300 + 0x300;
      EVT.todo_x28 = rand() % 0x40 + 0x40;
      EVT.todo_x2c = rand() % 0x1000;
      EVT.todo_x2e = rand() % 0x300 + 0x300;
      EVT.todo_x30 = rand() % 0x40 + 0x40;
      EVT.todo_x34 = rand() % 0x1000;
      EVT.todo_x36 = rand() % 0x300 + 0x300;
      EVT.todo_x38 = rand() % 0x40 + 0x40;
      EVT.todo_x3c = rand() % 0x1000;
      EVT.todo_x3e = rand() % 0x300 + 0x300;
      EVT.todo_x40 = rand() % 0x40 + 0x40;
      EVT.todo_x44 = rand() % 0x1000;
      EVT.todo_x46 = rand() % 0x300 + 0x300;
      EVT.todo_x48 = rand() % 0x40 + 0x40;
      EVT.todo_x4c = rand() % 0x1000;
      EVT.todo_x4e = rand() % 0x300 + 0x300;
      EVT.todo_x50 = rand() % 0x40 + 0x40;

      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = colorsAnimData;
      sprite->d.sprite.animInitialized = 0;
      sprite->d.sprite.semiTrans = 1;
      sprite->x1.n = evt->x1.n;
      sprite->z1.n = evt->z1.n;
      sprite->y1.n = evt->y1.n + 0x80;
      EVT.sprite = sprite;

      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      sum = 0;
      angle = EVT.todo_x58;
      EVT.todo_x58 += 8;

      for (i = 0; i < 6; i++) {
         switch (i) {
         case 0:
            a = EVT.todo_x2a;
            b = a - EVT.todo_x26;
            angleOfs = EVT.todo_x24;
            break;
         case 1:
            a = EVT.todo_x32;
            b = a - EVT.todo_x2e;
            angleOfs = EVT.todo_x2c;
            break;
         case 2:
            a = EVT.todo_x3a;
            b = a - EVT.todo_x36;
            angleOfs = EVT.todo_x34;
            break;
         case 3:
            a = EVT.todo_x42;
            b = a - EVT.todo_x3e;
            angleOfs = EVT.todo_x3c;
            break;
         case 4:
            a = EVT.todo_x4a;
            b = a - EVT.todo_x46;
            angleOfs = EVT.todo_x44;
            break;
         case 5:
            a = EVT.todo_x52;
            b = a - EVT.todo_x4e;
            angleOfs = EVT.todo_x4c;
            break;
         }

         if (a >= 0x600) {
            a = 0x600;
         }
         if (b >= 0x600) {
            b = 0x600;
         } else if (b <= 0) {
            b = 0;
         }

         sum += b;
         h_1 = a * EVT.todo_x5a / 180;
         h_2 = b * EVT.todo_x5a / 180;
         a = a * (180 - EVT.todo_x5a) / 180;
         b = b * (180 - EVT.todo_x5a) / 180;

         sprite->d.sprite.coords[0].x = evt->x1.n + (a * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[0].z = evt->z1.n + (a * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[0].y = evt->y1.n + h_1;
         sprite->d.sprite.coords[1].x = evt->x1.n + (a * rcos(angle + angleOfs + 0x40) >> 12);
         sprite->d.sprite.coords[1].z = evt->z1.n + (a * rsin(angle + angleOfs + 0x40) >> 12);
         sprite->d.sprite.coords[1].y = evt->y1.n + h_1;
         sprite->d.sprite.coords[2].x = evt->x1.n + (b * rcos(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[2].z = evt->z1.n + (b * rsin(angle + angleOfs) >> 12);
         sprite->d.sprite.coords[2].y = evt->y1.n + h_2;
         sprite->d.sprite.coords[3].x = evt->x1.n + (b * rcos(angle + angleOfs + 0x40) >> 12);
         sprite->d.sprite.coords[3].z = evt->z1.n + (b * rsin(angle + angleOfs + 0x40) >> 12);
         sprite->d.sprite.coords[3].y = evt->y1.n + h_2;

         sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[3].x) >> 1;
         sprite->y1.n = (sprite->d.sprite.coords[0].y + sprite->d.sprite.coords[3].y) >> 1;
         sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[3].z) >> 1;

         UpdateEvtAnimation(sprite);
         AddEvtPrim3(gGraphicsPtr->ot, sprite);
      }

      EVT.todo_x2a += EVT.todo_x28;
      EVT.todo_x32 += EVT.todo_x30;
      EVT.todo_x3a += EVT.todo_x38;
      EVT.todo_x42 += EVT.todo_x40;
      EVT.todo_x4a += EVT.todo_x48;
      EVT.todo_x52 += EVT.todo_x50;

      if (sum == 0x2400) {
         evt->functionIndex = EVTF_NULL;
         sprite->functionIndex = EVTF_NULL;
      }
      break;
   }
}

static s16 sAnimData_800fee24[28] = {
    4,  GFX_COLOR_1, 16, GFX_COLOR_2, 16, GFX_COLOR_3, 16, GFX_COLOR_4, 16, GFX_COLOR_5,
    16, GFX_COLOR_6, 16, GFX_COLOR_7, 16, GFX_COLOR_6, 16, GFX_COLOR_5, 16, GFX_COLOR_4,
    16, GFX_COLOR_3, 16, GFX_COLOR_2, 16, GFX_NULL,    1,  GFX_NULL};

#undef EVTF
#define EVTF 220
void Evtf220_Explosion_FX2(EvtData *evt) {
   static s16 explosionAnimData[26] = {0, GFX_EXPLOSION_1,  4, GFX_EXPLOSION_2,  4, GFX_EXPLOSION_3,
                                       4, GFX_EXPLOSION_4,  4, GFX_EXPLOSION_5,  4, GFX_EXPLOSION_6,
                                       4, GFX_EXPLOSION_7,  4, GFX_EXPLOSION_8,  4, GFX_EXPLOSION_9,
                                       4, GFX_EXPLOSION_10, 4, GFX_EXPLOSION_11, 4, GFX_NULL,
                                       0, GFX_NULL};

   EvtData *evt_s6;
   EvtData *unitSprite;
   s32 i, j;
   s16 a, b, c;

   switch (evt->state) {
   case 0:
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n;

      evt_s6 = Evt_GetUnused();
      evt_s6->functionIndex = EVTF_NOOP;
      evt_s6->d.sprite.animData = explosionAnimData;
      EVT.sprite = evt_s6;

      evt->state++;

   // fallthrough
   case 1:
      evt_s6 = EVT.sprite;
      UpdateEvtAnimation(evt_s6);

      for (i = 0; i < 6; i++) {
         for (j = 0; j < 16; j++) {
            if (EVT.todo_x24 <= 0x400) {
               a = (i * 0x80 + 0x80) * (ONE - rcos(EVT.todo_x24)) / ONE;
            } else {
               a = (i * 0x80 + 0x80) + (i * 0x80 + 0x80) * (ONE - rcos(EVT.todo_x24 - 0x400)) / ONE;
            }

            if (i % 2 == 0) {
               b = a * rcos(j * 0x100 + EVT.todo_x26 * i) >> 12;
               c = a * rsin(j * 0x100 + EVT.todo_x26 * i) >> 12;
            } else {
               b = a * rcos(j * 0x100 - EVT.todo_x26 * i) >> 12;
               c = a * rsin(j * 0x100 - EVT.todo_x26 * i) >> 12;
            }

            evt_s6->x1.n = evt->x1.n + b;
            evt_s6->z1.n = evt->z1.n + c;

            if (EVT.todo_x24 <= 0x400) {
               evt_s6->y1.n = evt->y1.n + (rsin(a * 4) >> 4);
            } else {
               evt_s6->y1.n =
                   evt->y1.n + (rsin(a * 4) >> 4) - 0x200 * rsin(EVT.todo_x24 - 0x400) / ONE;
            }

            AddEvtPrim6(gGraphicsPtr->ot, evt_s6, 0);
         }
      }

      EVT.todo_x26 += 0x60;
      EVT.todo_x24 += 0x24;
      if (EVT.todo_x24 == 0x5e8) {
         evt_s6 = Evt_GetUnused();
         evt_s6->functionIndex = EVTF_FX_TBD_132 + EVT.endingFxType;
         evt_s6->x1.n = evt->x1.n;
         evt_s6->z1.n = evt->z1.n;
      } else if (EVT.todo_x24 >= 0x800) {
         evt->functionIndex = EVTF_NULL;
         evt_s6->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void Evtf221_Explosion_FX3(EvtData *evt) {
   EvtData *evt_s0;

   evt_s0 = Evt_GetUnused();
   evt_s0->functionIndex = EVTF_EXPLOSION_FX2;
   evt_s0->x1.s.hi = evt->x1.s.hi;
   evt_s0->z1.s.hi = evt->z1.s.hi;
   evt_s0->d.evtf220.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 163
void Evtf163_StoneShower_FX1(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *sprite;
   POLY_FT4 *poly;
   EvtData **pDataStoreAsEvts;
   s32 i;
   s16 radius;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s0;

      evt->state++;

   // fallthrough
   case 1:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.semiTrans = 1;
      sprite->d.sprite.gfxIdx = GFX_COLOR_15;

      radius = EVT.todo_x24;
      for (i = 0; i < 32; i++) {
         sprite->d.sprite.coords[0].x = evt->x1.n + (radius * rsin(i * 0x80) >> 12);
         sprite->d.sprite.coords[0].z = evt->z1.n + (radius * rcos(i * 0x80) >> 12);
         sprite->d.sprite.coords[0].y = evt->y1.n + 0xa00;
         sprite->d.sprite.coords[1].x = evt->x1.n + (radius * rsin(i * 0x80 + 0x80) >> 12);
         sprite->d.sprite.coords[1].z = evt->z1.n + (radius * rcos(i * 0x80 + 0x80) >> 12);
         sprite->d.sprite.coords[1].y = sprite->d.sprite.coords[0].y;
         sprite->d.sprite.coords[2].x = sprite->d.sprite.coords[0].x;
         sprite->d.sprite.coords[2].z = sprite->d.sprite.coords[0].z;
         sprite->d.sprite.coords[2].y = evt->y1.n;
         sprite->d.sprite.coords[3].x = sprite->d.sprite.coords[1].x;
         sprite->d.sprite.coords[3].z = sprite->d.sprite.coords[1].z;
         sprite->d.sprite.coords[3].y = sprite->d.sprite.coords[2].y;

         sprite->x1.n = (sprite->d.sprite.coords[0].x + sprite->d.sprite.coords[1].x +
                         sprite->d.sprite.coords[2].x + sprite->d.sprite.coords[3].x) >>
                        2;
         sprite->z1.n = (sprite->d.sprite.coords[0].z + sprite->d.sprite.coords[1].z +
                         sprite->d.sprite.coords[2].z + sprite->d.sprite.coords[3].z) >>
                        2;
         sprite->y1.n = evt->y1.n + 0xe0;

         AddEvtPrim3(gGraphicsPtr->ot, sprite);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, radius + 0x3f, radius + 0x3f, radius + 0x3f);
      }

      sprite->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         gCameraZoom.vz += 2;
         gCameraRotation.vx += 4;
         gCameraRotation.vy += 16;

         EVT.todo_x24 += 8;
         if (EVT.todo_x24 >= 0xc0) {
            sprite->functionIndex = EVTF_NULL;
            evt->state2++;
            EVT.todo_x24 = 0xc0;
         }
         break;

      case 1:
         gCameraZoom.vz += 2;
         gCameraRotation.vx += 4;
         gCameraRotation.vy += 16;

         evt_s0 = EVT.dataStore;
         pDataStoreAsEvts = evt_s0->d.dataStore.evts;
         for (i = 0; i < 15; i++) {
            evt_s0 = Evt_GetUnused();
            evt_s0->functionIndex = EVTF_STONE_SHOWER_ROCK;
            evt_s0->d.evtf164.parent = evt;
            evt_s0->d.evtf164.downward = 0;
            evt_s0->d.evtf164.todo_x32 = rand() % 5;
            pDataStoreAsEvts[i] = evt_s0;
         }
         evt->state2++;
         break;

      case 2:
         gCameraZoom.vz += 2;
         gCameraRotation.vx += 2;
         gCameraRotation.vy += 16;

         EVT.timer++;
         if (EVT.timer >= 90) {
            sprite->functionIndex = EVTF_NULL;
            evt->state2++;
         }
         break;

      case 3:
         gCameraZoom.vz += 2;
         gCameraRotation.vx += 4;
         gCameraRotation.vy += 16;

         EVT.todo_x24 -= 8;
         if (EVT.todo_x24 < 0) {
            evt_s0 = EVT.dataStore;
            evt_s0->functionIndex = EVTF_NULL;
            pDataStoreAsEvts = evt_s0->d.dataStore.evts;
            for (i = 0; i < 15; i++) {
               evt_s0 = pDataStoreAsEvts[i];
               evt_s0->state = 99;
            }
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 165
void Evtf165_StoneShower_FX2(EvtData *evt) {
   s32 i;
   EvtData *evt_s0;
   EvtData *endingFx;
   EvtData **pDataStoreAsEvts;
   s16 waiting;

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      evt_s0 = Evt_GetUnused();
      evt_s0->functionIndex = EVTF_NOOP;
      EVT.dataStore = evt_s0;

      evt->state++;

   // fallthrough
   case 1:
      evt_s0 = EVT.dataStore;
      pDataStoreAsEvts = evt_s0->d.dataStore.evts;
      for (i = 0; i < 15; i++) {
         evt_s0 = Evt_GetUnused();
         evt_s0->functionIndex = EVTF_STONE_SHOWER_ROCK;
         evt_s0->d.evtf164.parent = evt;
         evt_s0->d.evtf164.downward = 1;
         evt_s0->d.evtf164.todo_x32 = 0;
         pDataStoreAsEvts[i] = evt_s0;
      }
      evt->state++;

   // fallthrough
   case 2:
      if (EVT.timer == 5) {
         endingFx = Evt_GetUnused();
         endingFx->functionIndex = EVTF_FX_TBD_140 + EVT.endingFxType;
         endingFx->x1.s.hi = evt->x1.s.hi;
         endingFx->z1.s.hi = evt->z1.s.hi;
      }
      if (EVT.endingFxType == 0) {
         waiting = (EVT.timer < 60);
      } else {
         waiting = (EVT.timer < 98);
      }
      if (!waiting) {
         evt->state++;
      } else {
         EVT.timer++;
      }
      break;

   case 3:
      evt_s0 = EVT.dataStore;
      evt_s0->functionIndex = EVTF_NULL;
      pDataStoreAsEvts = evt_s0->d.dataStore.evts;
      for (i = 0; i < 15; i++) {
         evt_s0 = pDataStoreAsEvts[i];
         evt_s0->state = 99;
      }
      gSignal3 = 1;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Evtf166_StoneShower_FX3(EvtData *evt) {
   EvtData *evt_s0;

   evt_s0 = Evt_GetUnused();
   evt_s0->functionIndex = EVTF_STONE_SHOWER_FX2;
   evt_s0->x1.s.hi = evt->x1.s.hi;
   evt_s0->z1.s.hi = evt->z1.s.hi;
   evt_s0->d.evtf165.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 164
void Evtf164_StoneShower_Rock(EvtData *evt) {
   static s16 rockAnimData[12] = {5, GFX_ROCK_1, 2, GFX_ROCK_2, 2, GFX_ROCK_3,
                                  2, GFX_ROCK_4, 2, GFX_NULL,   1, GFX_NULL};

   EvtData *evt_s1;
   s16 a;

   switch (evt->state) {
   case 0:
      evt_s1 = EVT.parent;
      evt->x1.n = evt_s1->x1.n;
      evt->z1.n = evt_s1->z1.n;
      evt->y1.n = evt_s1->y1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.animData = rockAnimData;
      EVT.sprite = evt_s1;

      evt->state2 = 0;
      evt->state++;

   // fallthrough
   case 1:
      if (!EVT.downward) {
         EVT.todo_x4c = 0;
         EVT.todo_x4e = 0x800;
         EVT.todo_x28 = rand() % 0x50 + 0x32;
      } else {
         EVT.todo_x4c = 0x400;
         EVT.todo_x4e = 0;
         EVT.todo_x28 = -0x50 - (rand() % 0x78);
      }

      EVT.todo_x26 = rand() % 0x1000;
      EVT.todo_x30 = rand() % 0xa0 + 0x20;
      EVT.todo_x24 = EVT.todo_x4c;
      evt_s1 = EVT.sprite;
      evt_s1->d.sprite.animInitialized = 0;
      evt->state++;

   // fallthrough
   case 2:
      if (evt->state2++ == EVT.todo_x32) {
         evt->state++;
         evt->state2 = 0;
      }
      break;

   case 3:
      evt_s1 = EVT.sprite;
      a = EVT.todo_x26;
      evt_s1->x1.n = evt->x1.n + EVT.todo_x30 * rcos(a) / ONE;
      evt_s1->z1.n = evt->z1.n + EVT.todo_x30 * rsin(a) / ONE;
      evt_s1->y1.n = evt->y1.n + EVT.todo_x24;
      UpdateEvtAnimation(evt_s1);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);

      EVT.todo_x24 += EVT.todo_x28;
      if (!EVT.downward) {
         if (EVT.todo_x24 >= EVT.todo_x4e) {
            evt->state = 1;
         }
      } else {
         if (EVT.todo_x24 <= EVT.todo_x4e) {
            evt->state = 1;
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_BOUNCE_ZOOM;
            evt_s1->d.evtf024.soft = 1;
         }
      }
      break;

   case 99:
      evt_s1 = EVT.sprite;
      evt_s1->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}
