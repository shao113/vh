#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

static s16 sExplosionAnimData_800fefb0[26] = {
    7, GFX_EXPLOSION_1, 2, GFX_EXPLOSION_2,  2, GFX_EXPLOSION_3,  2, GFX_EXPLOSION_4,
    2, GFX_EXPLOSION_5, 2, GFX_EXPLOSION_6,  2, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,
    2, GFX_EXPLOSION_9, 2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
    0, GFX_NULL};

static s16 sGlobeAnimData_800fefe4[32] = {
    5, GFX_GLOBE_1, 1, GFX_GLOBE_2, 1, GFX_GLOBE_3, 1, GFX_GLOBE_4, 1, GFX_GLOBE_5, 1, GFX_GLOBE_6,
    1, GFX_GLOBE_7, 1, GFX_GLOBE_8, 1, GFX_GLOBE_7, 1, GFX_GLOBE_6, 1, GFX_GLOBE_5, 1, GFX_GLOBE_4,
    1, GFX_GLOBE_3, 1, GFX_GLOBE_2, 1, GFX_NULL,    1, GFX_NULL};

static s16 sGlobeAnimData_800ff024[32] = {
    4, GFX_GLOBE_1, 1, GFX_GLOBE_2, 1, GFX_GLOBE_3, 1, GFX_GLOBE_4, 1, GFX_GLOBE_5, 1, GFX_GLOBE_6,
    1, GFX_GLOBE_7, 1, GFX_GLOBE_8, 1, GFX_GLOBE_7, 1, GFX_GLOBE_6, 1, GFX_GLOBE_5, 1, GFX_GLOBE_4,
    1, GFX_GLOBE_3, 1, GFX_GLOBE_2, 1, GFX_NULL,    1, GFX_NULL};

static s16 sLightningAnimData_800ff064[32] = {
    4, GFX_LIGHTNING_1, 1, GFX_LIGHTNING_2, 1, GFX_LIGHTNING_3, 1, GFX_LIGHTNING_4,
    1, GFX_LIGHTNING_5, 1, GFX_LIGHTNING_6, 1, GFX_LIGHTNING_7, 1, GFX_LIGHTNING_8,
    1, GFX_LIGHTNING_7, 1, GFX_LIGHTNING_6, 1, GFX_LIGHTNING_5, 1, GFX_LIGHTNING_4,
    1, GFX_LIGHTNING_3, 1, GFX_LIGHTNING_2, 1, GFX_NULL,        1, GFX_NULL};

static s16 sExplosionAnimData_800ff0a4[26] = {
    0, GFX_EXPLOSION_1, 1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,  1, GFX_EXPLOSION_4,
    1, GFX_EXPLOSION_5, 1, GFX_EXPLOSION_6,  1, GFX_EXPLOSION_7,  1, GFX_EXPLOSION_8,
    1, GFX_EXPLOSION_9, 1, GFX_EXPLOSION_10, 1, GFX_EXPLOSION_11, 1, GFX_NULL,
    0, GFX_NULL};

#undef EVTF
#define EVTF 146
void Evtf146_SpellFx3_HolyLightning(EvtData *evt) {
   EvtData *fx2;

   fx2 = Evt_GetUnused();
   fx2->functionIndex = EVTF_SPELL_FX2_HOLY_LIGHTNING;
   fx2->x1.s.hi = evt->x1.s.hi;
   fx2->z1.s.hi = evt->z1.s.hi;
   fx2->d.evtf144.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 144
void Evtf144_SpellFx2_HolyLightning(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_v1;
   s32 i;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
         evt->x1.n = targetSprite->x1.n;
         evt->y1.n = targetSprite->y1.n;
         evt->z1.n = targetSprite->z1.n;

         for (i = 0; i < EVT_DATA_CT; i++) {
            evt_v1 = &gEvtDataArray[i];
            if (evt_v1->functionIndex == EVTF_CAMERA_TBD_026) {
               evt_v1->functionIndex = EVTF_NULL;
            }
         }

         EVT.unitSprite = targetSprite;

         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_FLASHING_UNIT_SPRITE;
         evt_v1->d.evtf149.clut = 4;
         evt_v1->x1.s.hi = targetSprite->x1.s.hi;
         evt_v1->z1.s.hi = targetSprite->z1.s.hi;
         EVT.targetFlashingFx = evt_v1;

         EVT.timer = 150;
         evt->state2++;

      // fallthrough
      case 1:
         if (EVT.timer > 135) {
            gLightColor.r -= 6;
            gLightColor.g -= 6;
            gLightColor.b -= 6;
         } else if (EVT.timer < 15) {
            gLightColor.r += 6;
            gLightColor.g += 6;
            gLightColor.b += 6;
         }
         gCameraZoom.vz += 1;
         gCameraRotation.vx += 2;
         gCameraRotation.vy += 0x10;

         // EVT.timer--;
         if (EVT.timer-- % 8 == 0) {
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_SPELL_FX_HOLY_LIGHTNING_ELECTRIC_ORB;
            evt_v1->x1.n = evt->x1.n;
            evt_v1->y1.n = evt->y1.n;
            evt_v1->z1.n = evt->z1.n;
         }
         if (EVT.timer == 10) {
            targetSprite = EVT.unitSprite;
            evt_v1 = Evt_GetUnused();
            evt_v1->functionIndex = EVTF_CAMERA_TBD_026;
            evt_v1->d.evtf026.target = targetSprite;
         } else if (EVT.timer == 0) {
            evt_v1 = EVT.targetFlashingFx;
            evt_v1->state = 99;
            evt->state2++;
         }
         break;

      case 2:
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_FX_TBD_136 + EVT.endingFxType;
         evt_v1->d.evtf136.clut = 4;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->y1.n = evt->y1.n;
         evt_v1->z1.n = evt->z1.n;

         evt->functionIndex = EVTF_NULL;
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 145
void Evtf145_SpellFx_HolyLightning_ElectricOrb(EvtData *evt) {
   static s16 lightningAnimData[] = {
       4, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2, 2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4,
       2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6,
       2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8, 2, GFX_LIGHTNING_1, 2, GFX_LIGHTNING_2,
       2, GFX_LIGHTNING_3, 2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_4, 2, GFX_LIGHTNING_5,
       2, GFX_LIGHTNING_5, 2, GFX_LIGHTNING_6, 2, GFX_LIGHTNING_7, 2, GFX_LIGHTNING_8,
       2, GFX_NULL,        0, GFX_NULL};

   static s16 globeAnimData[] = {4, GFX_GLOBE_2, 1, GFX_GLOBE_3, 1, GFX_GLOBE_4, 1, GFX_GLOBE_5,
                                 1, GFX_GLOBE_6, 1, GFX_GLOBE_7, 1, GFX_GLOBE_8, 1, GFX_GLOBE_8,
                                 1, GFX_GLOBE_7, 1, GFX_GLOBE_6, 1, GFX_GLOBE_5, 1, GFX_GLOBE_4,
                                 1, GFX_GLOBE_3, 1, GFX_GLOBE_2, 1, GFX_NULL,    0, GFX_NULL};

   s16 cluts[4] = {3, 4, 8, 9};
   EvtData *evt_s4;
   EvtData *orbSprite;
   POLY_FT4 *poly;
   s16 a, clut;

   switch (evt->state) {
   case 0:
      EVT.x_3 = 0x40 - rand() % 0x80;
      EVT.z_3 = 0x40 - rand() % 0x80;
      EVT.x_4 = 0x200 - rand() % 0x400;
      EVT.z_4 = 0x200 - rand() % 0x400;

      a = 0x300 - rsin(((abs(EVT.x_4) + abs(EVT.z_4)) / 2 + 0x200) * 2) / 0x40;
      evt->y1.n += a;
      EVT.y_3 = -a;

      if (rand() % 2 != 0) {
         EVT.z_1 = -((EVT.z_4 - EVT.z_3) * rsin(0x156)) / ONE;
         EVT.x_2 = -((EVT.x_4 - EVT.x_3) * rsin(0x2aa)) / ONE;
      } else {
         EVT.z_1 = -((EVT.z_4 - EVT.z_3) * rcos(0x2aa)) / ONE;
         EVT.x_2 = -((EVT.x_4 - EVT.x_3) * rcos(0x156)) / ONE;
      }
      if (rand() % 2 != 0) {
         EVT.y_1 = EVT.y_3 * rsin(0x156) / ONE;
         EVT.y_2 = EVT.y_3 * rsin(0x2aa) / ONE;
      } else {
         EVT.y_1 = EVT.y_3 * rcos(0x2aa) / ONE;
         EVT.y_2 = EVT.y_3 * rcos(0x156) / ONE;
      }
      if (rand() % 2 != 0) {
         EVT.z_1 = -((EVT.z_4 - EVT.z_3) * rsin(0x156)) / ONE;
         EVT.z_2 = -((EVT.z_4 - EVT.z_3) * rsin(0x2aa)) / ONE;
      } else {
         EVT.z_1 = -((EVT.z_4 - EVT.z_3) * rcos(0x2aa)) / ONE;
         EVT.z_2 = -((EVT.z_4 - EVT.z_3) * rcos(0x156)) / ONE;
      }

      clut = cluts[rand() % ARRAY_COUNT(cluts)];

      evt_s4 = Evt_GetUnused();
      EVT.lightningSprite = evt_s4;
      evt_s4->functionIndex = EVTF_NOOP;
      evt_s4->d.sprite.animData = lightningAnimData;
      evt_s4->d.sprite.animInitialized = 0;
      evt_s4->d.sprite.clut = clut;

      evt_s4 = Evt_GetUnused();
      EVT.orbSprite = evt_s4;
      evt_s4->functionIndex = EVTF_NOOP;
      evt_s4->d.sprite.animData = globeAnimData;
      evt_s4->d.sprite.animInitialized = 0;
      evt_s4->d.sprite.boxIdx = 4;
      evt_s4->d.sprite.clut = clut;
      evt_s4->x1.n = evt->x1.n + EVT.x_4;
      evt_s4->z1.n = evt->z1.n + EVT.z_4;
      evt_s4->y1.n = evt->y1.n;

      evt->state++;
      break;

   case 1:
      orbSprite = EVT.orbSprite;
      UpdateEvtAnimation(orbSprite);
      clut = orbSprite->d.sprite.clut;

      orbSprite->d.sprite.semiTrans = 2;
      AddEvtPrim6(gGraphicsPtr->ot, orbSprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      orbSprite->d.sprite.semiTrans = 3;
      orbSprite->d.sprite.clut = 10;
      AddEvtPrim6(gGraphicsPtr->ot, orbSprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      orbSprite->d.sprite.clut = clut;
      evt_s4 = EVT.lightningSprite;

      EVT.x_1 += 0x18 - rand() % 0x30;
      EVT.y_1 += 4 - rand() % 8;
      EVT.z_1 += 0x18 - rand() % 0x30;
      EVT.x_2 += 0x18 - rand() % 0x30;
      EVT.y_2 += 4 - rand() % 8;
      EVT.z_2 += 0x18 - rand() % 0x30;
      EVT.todo_x40 += 0x18 - rand() % 0x30;
      EVT.todo_x42 += 4 - rand() % 8;
      EVT.todo_x44 += 0x18 - rand() % 0x30;

      evt_s4->d.sprite.coords[0].x = evt->x1.n + 0xc0 + EVT.x_4;
      evt_s4->d.sprite.coords[0].y = evt->y1.n;
      evt_s4->d.sprite.coords[0].z = evt->z1.n + EVT.z_4;
      evt_s4->d.sprite.coords[1].x = evt->x1.n - 0xc0 + EVT.x_4;
      evt_s4->d.sprite.coords[1].y = evt->y1.n;
      evt_s4->d.sprite.coords[1].z = evt->z1.n + EVT.z_4;
      evt_s4->d.sprite.coords[2].x = evt->x1.n + EVT.x_1 + 0xc0;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[2].z = evt->z1.n + EVT.z_1;
      evt_s4->d.sprite.coords[3].x = evt->x1.n + EVT.x_1 - 0xc0;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[3].z = evt->z1.n + EVT.z_1;

      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      UpdateEvtAnimation(evt_s4);

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      evt_s4->d.sprite.coords[0].x = evt->x1.n + 0xc0;
      evt_s4->d.sprite.coords[0].y = evt->y1.n;
      evt_s4->d.sprite.coords[0].z = evt->z1.n;
      evt_s4->d.sprite.coords[1].x = evt->x1.n - 0xc0;
      evt_s4->d.sprite.coords[1].y = evt->y1.n;
      evt_s4->d.sprite.coords[1].z = evt->z1.n;
      evt_s4->d.sprite.coords[2].x = evt->x1.n - EVT.x_1 / 2 + 0xc0;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[2].z = evt->z1.n - EVT.z_1 / 2;
      evt_s4->d.sprite.coords[3].x = evt->x1.n - EVT.x_1 / 2 - 0xc0;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[3].z = evt->z1.n - EVT.z_1 / 2;

      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade / 3, EVT.fade / 3, EVT.fade / 3);

      evt_s4->d.sprite.coords[0].x = evt->x1.n + EVT.x_1 + 0xc0;
      evt_s4->d.sprite.coords[0].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[0].z = evt->z1.n + EVT.z_1;
      evt_s4->d.sprite.coords[1].x = evt->x1.n + EVT.x_1 - 0xc0;
      evt_s4->d.sprite.coords[1].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[1].z = evt->z1.n + EVT.z_1;
      evt_s4->d.sprite.coords[2].x = evt->x1.n + EVT.x_2 + 0xc0;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[2].z = evt->z1.n + EVT.z_2;
      evt_s4->d.sprite.coords[3].x = evt->x1.n + EVT.x_2 - 0xc0;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[3].z = evt->z1.n + EVT.z_2;

      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      evt_s4->d.sprite.coords[0].x = evt->x1.n - EVT.x_1 / 2 + 0xc0;
      evt_s4->d.sprite.coords[0].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[0].z = evt->z1.n - EVT.z_1 / 2;
      evt_s4->d.sprite.coords[1].x = evt->x1.n - EVT.x_1 / 2 - 0xc0;
      evt_s4->d.sprite.coords[1].y = evt->y1.n + EVT.y_1;
      evt_s4->d.sprite.coords[1].z = evt->z1.n - EVT.z_1 / 2;
      evt_s4->d.sprite.coords[2].x = evt->x1.n - EVT.x_2 / 2 + 0xc0;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[2].z = evt->z1.n - EVT.z_2 / 2;
      evt_s4->d.sprite.coords[3].x = evt->x1.n - EVT.x_2 / 2 - 0xc0;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[3].z = evt->z1.n - EVT.z_2 / 2;

      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade / 3, EVT.fade / 3, EVT.fade / 3);

      evt_s4->d.sprite.coords[0].x = evt->x1.n + EVT.x_2 + 0xc0;
      evt_s4->d.sprite.coords[0].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[0].z = evt->z1.n + EVT.z_2;
      evt_s4->d.sprite.coords[1].x = evt->x1.n + EVT.x_2 - 0xc0;
      evt_s4->d.sprite.coords[1].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[1].z = evt->z1.n + EVT.z_2;
      evt_s4->d.sprite.coords[2].x = evt->x1.n + EVT.x_3 + 0xd0 - rand() % 0x20;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_3;
      evt_s4->d.sprite.coords[2].z = evt->z1.n + EVT.z_3 + 0x10 - rand() % 0x20;
      evt_s4->d.sprite.coords[3].x = evt->x1.n + EVT.x_3 - 0xb0 - rand() % 0x20;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_3;
      evt_s4->d.sprite.coords[3].z = evt->z1.n + EVT.z_3 + 0x10 - rand() % 0x20;
      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      evt_s4->d.sprite.coords[0].x = evt->x1.n - EVT.x_2 / 2 + 0xc0;
      evt_s4->d.sprite.coords[0].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[0].z = evt->z1.n - EVT.z_2 / 2;
      evt_s4->d.sprite.coords[1].x = evt->x1.n - EVT.x_2 / 2 - 0xc0;
      evt_s4->d.sprite.coords[1].y = evt->y1.n + EVT.y_2;
      evt_s4->d.sprite.coords[1].z = evt->z1.n - EVT.z_2 / 2;
      evt_s4->d.sprite.coords[2].x = evt->x1.n - EVT.x_3 / 2 + 0xd0 - rand() % 0x20;
      evt_s4->d.sprite.coords[2].y = evt->y1.n + EVT.y_3;
      evt_s4->d.sprite.coords[2].z = evt->z1.n - EVT.z_3 / 2 + 0x10 - rand() % 0x20;
      evt_s4->d.sprite.coords[3].x = evt->x1.n - EVT.x_3 / 2 - 0xb0 - rand() % 0x20;
      evt_s4->d.sprite.coords[3].y = evt->y1.n + EVT.y_3;
      evt_s4->d.sprite.coords[3].z = evt->z1.n - EVT.z_3 / 2 + 0x10 - rand() % 0x20;

      evt_s4->x1.n = (evt_s4->d.sprite.coords[0].x + evt_s4->d.sprite.coords[3].x) / 2;
      evt_s4->y1.n = (evt_s4->d.sprite.coords[0].y + evt_s4->d.sprite.coords[3].y) / 2;
      evt_s4->z1.n = (evt_s4->d.sprite.coords[0].z + evt_s4->d.sprite.coords[3].z) / 2;

      evt_s4->d.sprite.semiTrans = 2;
      AddEvtPrim3(gGraphicsPtr->ot, evt_s4);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade / 3, EVT.fade / 3, EVT.fade / 3);

      switch (evt->state2) {
      case 0:
         EVT.fade += 15;
         if (EVT.fade > 191) {
            evt->state2++;
         }
         break;
      case 1:
         EVT.fade -= 15;
         if (EVT.fade < 0) {
            EVT.fade = 0;
         }
         break;
      }

      if (evt_s4->d.sprite.animFinished) {
         evt->functionIndex = EVTF_NULL;
         evt_s4->functionIndex = EVTF_NULL;
         orbSprite->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 224
void Evtf224_SpellFx1_ThunderBall(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *orb;

   switch (evt->state) {
   case 0:
      EVT.savedZoom = gCameraZoom.vz;
      unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      orb = Evt_GetUnused();
      orb->functionIndex = EVTF_SPELL_FX_THUNDER_BALL_INITIAL_ORB;
      orb->x1.n = unitSprite->x1.n;
      orb->y1.n = unitSprite->y1.n;
      orb->z1.n = unitSprite->z1.n;
      orb->d.evtf225.dx = 0; // unused
      orb->d.evtf225.dz = 0; // unused
      orb->d.evtf225.dy = 0x300;

      evt->state++;
      break;

   case 1:
      evt->state3++;
      if (gLightColor.r < 36) {
         evt->state++;
      } else {
         gLightColor.r -= 4;
         gLightColor.g -= 4;
         gLightColor.b -= 4;
      }
      break;

   case 2:
      evt->state3++;
      if (evt->state3 > 56) {
         gLightColor.r += 8;
         gLightColor.g += 8;
         gLightColor.b += 8;
      }
      if (evt->state3 == 68) {
         gLightColor.r = 0x80;
         gLightColor.g = 0x80;
         gLightColor.b = 0x80;
         gSignal3 = 1;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 225
void Evtf225_SpellFx_ThunderBall_InitialOrb(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_s1;
   POLY_FT4 *poly;
   u8 *p;

   switch (evt->state) {
   case 0:
      evt_s2 = Evt_GetUnused();
      EVT.orbSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sGlobeAnimData_800ff024;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 4;

      evt_s2 = Evt_GetUnused();
      EVT.lightningSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sLightningAnimData_800ff064;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 8;

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = EVT.orbSprite;
      evt_s1 = EVT.lightningSprite;
      evt_s2->x1.n = evt->x1.n;
      evt_s2->z1.n = evt->z1.n;
      evt_s2->y1.n = evt->y1.n + EVT.timer * EVT.dy / 0x30 + 0xe0;
      evt_s1->x1.n = evt_s2->x1.n;
      evt_s1->z1.n = evt_s2->z1.n;
      evt_s1->y1.n = evt_s2->y1.n;

      if (evt->state2 == 0) {
         UpdateEvtAnimation(evt_s1);
         AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);
      }
      UpdateEvtAnimation(evt_s2);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);
      gCameraZoom.vz += 10;

      switch (evt->state2) {
      case 0:
         EVT.timer++;
         if (EVT.timer == 37) {
            evt_s1->functionIndex = EVTF_NULL;
            evt->state2++;
            evt_s2->d.sprite.animData = sExplosionAnimData_800ff0a4;
            evt_s2->d.sprite.animInitialized = 0;
            evt_s2->d.sprite.boxIdx = 4;
            evt_s2->d.sprite.clut = 3;
         }
         break;

      case 1:
         EVT.timer++;
         if (EVT.timer == 46) {
            for (p = gTargetCoords; *p != 0xff; p += 2) {
               evt_s1 = GetUnitSpriteAtPosition(p[1], p[0]);
               evt_s2 = Evt_GetUnused();
               evt_s2->functionIndex = EVTF_SPELL_FX_THUNDER_BALL_CHILD_ORB;
               evt_s2->x1.n = evt->x1.n;
               evt_s2->z1.n = evt->z1.n;
               evt_s2->y1.n = evt->y1.n + EVT.dy + 0xe0;
               evt_s2->d.evtf226.dx = evt_s1->x1.n - evt_s2->x1.n;
               evt_s2->d.evtf226.dz = evt_s1->z1.n - evt_s2->z1.n;
               evt_s2->d.evtf226.dy = (evt_s1->y1.n - evt_s2->y1.n) + 0x80;
            }
         } else if (EVT.timer == 48) {
            evt_s2->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 226
void Evtf226_SpellFx_ThunderBall_ChildOrb(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *lightningSprite;
   POLY_FT4 *poly;
   s32 timer;

   switch (evt->state) {
   case 0:
      EVT.todo_x26 = 0x100 - rand() % 0x200;
      EVT.todo_x2a = 0x100 - rand() % 0x200;

      evt_s2 = Evt_GetUnused();
      EVT.orbSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sGlobeAnimData_800ff024;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 4;

      evt_s2 = Evt_GetUnused();
      EVT.lightningSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sLightningAnimData_800ff064;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 8;

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = EVT.orbSprite;
      lightningSprite = EVT.lightningSprite;
      timer = EVT.timer;

      evt_s2->x1.n =
          evt->x1.n + (EVT.todo_x26 * rsin(timer * 0x40) >> 11) + (timer * EVT.dx / 0x20);
      evt_s2->z1.n =
          evt->z1.n + (EVT.todo_x2a * rsin(timer * 0x40) >> 11) + (timer * EVT.dz / 0x20);
      evt_s2->y1.n = evt->y1.n + (0x180 * rsin(timer * 0x40) >> 11) + (timer * EVT.dy / 0x20);

      lightningSprite->x1.n = evt_s2->x1.n;
      lightningSprite->z1.n = evt_s2->z1.n;
      lightningSprite->y1.n = evt_s2->y1.n;

      if (EVT.timer < 22) {
         UpdateEvtAnimation(lightningSprite);
         AddEvtPrim6(gGraphicsPtr->ot, lightningSprite, 0);
      }
      UpdateEvtAnimation(evt_s2);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      EVT.timer++;
      if (EVT.timer == 22) {
         lightningSprite->functionIndex = EVTF_NULL;
         evt_s2->d.sprite.animData = sExplosionAnimData_800ff0a4;
         evt_s2->d.sprite.animInitialized = 0;
         evt_s2->d.sprite.boxIdx = 4;
         evt_s2->d.sprite.clut = 3;
      } else if (EVT.timer == 32) {
         evt_s2->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 222
void Evtf222_Fx_TBD(EvtData *evt) {
   EvtData *evt_s0;
   EvtData *evt_s1;
   EvtData *evt_s2;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      EVT.savedZoom = gCameraZoom.vz;
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_FX_TBD_223;
      evt_s1->x1.n = evt_s0->x1.n;
      evt_s1->y1.n = evt_s0->y1.n;
      evt_s1->z1.n = evt_s0->z1.n;
      evt_s2 = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
      evt_s1->d.evtf223.dx = evt_s2->x1.n - evt_s0->x1.n;
      evt_s1->d.evtf223.dz = evt_s2->z1.n - evt_s0->z1.n;
      evt_s1->d.evtf223.dy = (evt_s2->y1.n - evt_s0->y1.n) + 0x200;
      EVT.fx = evt_s1;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
      evt_s1->d.evtf026.target = evt_s2;
      evt_s1->d.evtf026.type = 2;
      EVT.cam = evt_s1;

      evt->state++;
      break;

   case 1:
      evt_s0 = EVT.fx;
      evt_s2 = evt_s0->d.evtf223.orbSprite;
      gCameraZoom.vz = EVT.savedZoom + (evt_s2->y1.n >> 4) + (evt_s2->y1.n >> 5);
      evt_s1 = EVT.cam;
      if (evt_s1->functionIndex == EVTF_CAMERA_TBD_026) {
         evt_s1->d.evtf026.target = evt_s2;
         evt_s1->d.evtf026.zoom = gCameraZoom.vz;
      } else {
         evt_s1 = Evt_GetUnused();
         evt_s1->functionIndex = EVTF_CAMERA_TBD_026;
         evt_s1->d.evtf026.target = evt_s2;
         EVT.cam = evt_s1;
         evt_s1->d.evtf026.zoom = gCameraZoom.vz;
      }

      if (evt->state3++ > 51) {
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

         EVT.fade += 16;
      }
      if (evt->state3 == 61) {
         evt_s1->functionIndex = EVTF_NULL;
         evt->state++;
         gSignal3 = 1;
         evt_s1->d.evtf026.todo_x2c = 0x80;
      }
      break;

   case 2:
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
      if (EVT.fade < 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 223
void Evtf223_Fx_TBD(EvtData *evt) {
   EvtData *evt_s2;
   EvtData *evt_s1;
   s32 timer;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      evt_s2 = Evt_GetUnused();
      EVT.orbSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sGlobeAnimData_800ff024;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 4;

      evt_s2 = Evt_GetUnused();
      EVT.lightningSprite = evt_s2;
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.animData = sLightningAnimData_800ff064;
      evt_s2->d.sprite.animInitialized = 0;
      evt_s2->d.sprite.boxIdx = 4;
      evt_s2->d.sprite.clut = 8;

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = EVT.orbSprite;
      evt_s1 = EVT.lightningSprite;

      timer = EVT.timer;
      evt_s2->x1.n = evt->x1.n + timer * EVT.dx / 0x40;
      evt_s2->z1.n = evt->z1.n + timer * EVT.dz / 0x40;
      evt_s2->y1.n = evt->y1.n + (0x200 * rsin(timer * 0x20) >> 11) + (timer * EVT.dy / 0x40);

      evt_s1->x1.n = evt_s2->x1.n;
      evt_s1->z1.n = evt_s2->z1.n;
      evt_s1->y1.n = evt_s2->y1.n;

      if (EVT.timer < 52) {
         UpdateEvtAnimation(evt_s1);
         AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);
      }
      UpdateEvtAnimation(evt_s2);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      EVT.timer++;
      if (EVT.timer == 53) {
         evt_s1->functionIndex = EVTF_NULL;
         evt_s2->d.sprite.animData = sExplosionAnimData_800ff0a4;
         evt_s2->d.sprite.animInitialized = 0;
         evt_s2->d.sprite.boxIdx = 4;
         evt_s2->d.sprite.clut = 3;
      } else if (EVT.timer == 64) {
         evt_s2->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 120
void Evtf120_Fx_TBD(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_FX_TBD_148;
   fx->x1.n = unitSprite->x1.n;
   fx->y1.n = unitSprite->y1.n;
   fx->z1.n = unitSprite->z1.n;
   fx->d.evtf148.timer = 4;
   fx->d.evtf148.endingFxType = 0;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 121
void Evtf121_Fx_TBD(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_FX_TBD_148;
   fx->x1.n = unitSprite->x1.n;
   fx->y1.n = unitSprite->y1.n;
   fx->z1.n = unitSprite->z1.n;
   fx->d.evtf148.timer = 4;
   fx->d.evtf148.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 200
void Evtf200_SpellFx3_RollingThunder(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *fx;

   unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_SPELL_FX2_ROLLING_THUNDER;
   fx->x1.n = unitSprite->x1.n;
   fx->y1.n = unitSprite->y1.n;
   fx->z1.n = unitSprite->z1.n;
   fx->d.evtf148.endingFxType = 2;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 195
void Evtf195_SpellFx2_RollingThunder(EvtData *evt) {
   // static u8 cluts[4] = {3, 4, 8, 9}; //? Explicit size gets this placed into sdata
   static u8 cluts[] = {3, 4, 8, 9}; //? But implicit size gets this placed into data

   s32 i;
   EvtData *evt_v1;

   switch (evt->state) {
   case 0:
      for (i = 0; i < 4; i++) {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_SPELL_FX_ROLLING_THUNDER_ORB_PAIR;
         evt_v1->x1.s.hi = evt->x1.s.hi;
         evt_v1->z1.s.hi = evt->z1.s.hi;
         evt_v1->d.evtf196.todo_x28 = i * 0x400;
         evt_v1->d.evtf196.todo_x2a = 0x200;
         evt_v1->d.evtf196.clut = cluts[i];
      }
      evt->state++;

   // fallthrough
   case 1:
      EVT.timer++;
      if (EVT.timer == 191) {
         evt_v1 = Evt_GetUnused();
         evt_v1->functionIndex = EVTF_FX_TBD_136 + EVT.endingFxType;
         evt_v1->d.evtf136.clut = 4;
         evt_v1->x1.n = evt->x1.n;
         evt_v1->y1.n = evt->y1.n;
         evt_v1->z1.n = evt->z1.n;
      } else if (EVT.timer == 196) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 196
void Evtf196_SpellFx_RollingThunder_OrbPair(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *lightningSprite;
   EvtData *orbSprite;
   POLY_FT4 *poly;
   s8 unused[96]; //? EvtData is also 96 bytes
   s16 x_3, y_3, z_3;
   s16 x_1, y_1, z_1;
   s16 x_2, y_2, z_2;

   switch (evt->state) {
   case 0:
      EVT.targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      orbSprite = Evt_GetUnused();
      orbSprite->functionIndex = EVTF_NOOP;
      orbSprite->d.sprite.clut = EVT.clut;
      orbSprite->d.sprite.animData = sGlobeAnimData_800fefe4;
      EVT.orbSprite = orbSprite;

      orbSprite = Evt_GetUnused(); // Re-used for lightning here
      orbSprite->functionIndex = EVTF_NOOP;
      orbSprite->d.sprite.clut = EVT.clut;
      orbSprite->d.sprite.animData = sLightningAnimData_800ff064;
      EVT.lightningSprite = orbSprite;

      evt->state++;

   // fallthrough
   case 1:

      switch (evt->state2) {
      case 0:
         EVT.todo_x34 += 0x10;
         if (EVT.todo_x34 == 0x100) {
            evt->state2++;
         }
         break;
      case 1:
         if (EVT.timer == 180) {
            evt->state2++;
         }
         break;
      case 2:
         EVT.todo_x34 -= 0x10;
         if (EVT.todo_x34 == 0) {
            evt->state2++;
         }
         break;
      }

      targetSprite = EVT.targetSprite;
      orbSprite = EVT.orbSprite;
      lightningSprite = EVT.lightningSprite;

      EVT.todo_x2c = rsin(rcos(EVT.timer * 8));
      x_3 = EVT.todo_x34 * rcos(EVT.todo_x28 + EVT.todo_x2c) >> 12;
      z_3 = EVT.todo_x34 * rsin(EVT.todo_x28 + EVT.todo_x2c) >> 12;
      y_3 = EVT.todo_x34 * rsin(EVT.todo_x2a) >> 12;
      x_3 = rcos(EVT.todo_x2a) * x_3 / ONE;
      z_3 = rcos(EVT.todo_x2a) * z_3 / ONE;
      EVT.todo_x2a += 0x40;

      UpdateEvtAnimation(orbSprite);

      orbSprite->x1.n = targetSprite->x1.n + x_3;
      orbSprite->z1.n = targetSprite->z1.n + z_3;
      orbSprite->y1.n = targetSprite->y1.n + 0x80 + y_3;
      AddEvtPrim6(gGraphicsPtr->ot, orbSprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      x_1 = orbSprite->x1.n;
      y_1 = orbSprite->y1.n;
      z_1 = orbSprite->z1.n;

      orbSprite->x1.n = targetSprite->x1.n - x_3;
      orbSprite->z1.n = targetSprite->z1.n - z_3;
      orbSprite->y1.n = targetSprite->y1.n + 0x80 - y_3;
      AddEvtPrim6(gGraphicsPtr->ot, orbSprite, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, 0xff, 0xff, 0xff);

      x_2 = orbSprite->x1.n;
      y_2 = orbSprite->y1.n;
      z_2 = orbSprite->z1.n;

      x_3 = 0x80 - rand() % 0x100;
      z_3 = 0x80 - rand() % 0x100;
      y_3 = 0x80 - rand() % 0x100;

      UpdateEvtAnimation(lightningSprite);

      lightningSprite->d.sprite.otOfs = 2;
      lightningSprite->d.sprite.semiTrans = 2;
      lightningSprite->d.sprite.coords[0].x = x_1;
      lightningSprite->d.sprite.coords[1].x = x_1;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 + x_3;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 + x_3;
      lightningSprite->d.sprite.coords[0].y = y_1 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_1 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 + y_3 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 + y_3 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_1;
      lightningSprite->d.sprite.coords[1].z = z_1;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 + z_3;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 + z_3;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 1

      lightningSprite->d.sprite.coords[0].x = x_2;
      lightningSprite->d.sprite.coords[1].x = x_2;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 + x_3;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 + x_3;
      lightningSprite->d.sprite.coords[0].y = y_2 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_2 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 + y_3 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 + y_3 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_2;
      lightningSprite->d.sprite.coords[1].z = z_2;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 + z_3;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 + z_3;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 2

      lightningSprite->d.sprite.semiTrans = 1;
      lightningSprite->d.sprite.coords[0].x = x_1;
      lightningSprite->d.sprite.coords[1].x = x_1;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 - x_3;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 - x_3;
      lightningSprite->d.sprite.coords[0].y = y_1 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_1 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 - y_3 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 - y_3 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_1;
      lightningSprite->d.sprite.coords[1].z = z_1;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 - z_3;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 - z_3;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 3

      lightningSprite->d.sprite.coords[0].x = x_2;
      lightningSprite->d.sprite.coords[1].x = x_2;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 - x_3;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 - x_3;
      lightningSprite->d.sprite.coords[0].y = y_2 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_2 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 - y_3 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 - y_3 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_2;
      lightningSprite->d.sprite.coords[1].z = z_2;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 - z_3;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 - z_3;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 4

      lightningSprite->d.sprite.semiTrans = 4;
      lightningSprite->d.sprite.coords[0].x = x_1;
      lightningSprite->d.sprite.coords[1].x = x_1;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 - x_3 / 2;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 - x_3 / 2;
      lightningSprite->d.sprite.coords[0].y = y_1 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_1 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 - y_3 / 2 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 - y_3 / 2 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_1;
      lightningSprite->d.sprite.coords[1].z = z_1;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 - z_3 / 2;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 - z_3 / 2;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 5

      lightningSprite->d.sprite.coords[0].x = x_2;
      lightningSprite->d.sprite.coords[1].x = x_2;
      lightningSprite->d.sprite.coords[2].x = (x_1 + x_2) / 2 - x_3 / 2;
      lightningSprite->d.sprite.coords[3].x = (x_1 + x_2) / 2 - x_3 / 2;
      lightningSprite->d.sprite.coords[0].y = y_2 + 0x40;
      lightningSprite->d.sprite.coords[1].y = y_2 - 0x40;
      lightningSprite->d.sprite.coords[2].y = (y_1 + y_2) / 2 - y_3 / 2 + 0x40;
      lightningSprite->d.sprite.coords[3].y = (y_1 + y_2) / 2 - y_3 / 2 - 0x40;
      lightningSprite->d.sprite.coords[0].z = z_2;
      lightningSprite->d.sprite.coords[1].z = z_2;
      lightningSprite->d.sprite.coords[2].z = (z_1 + z_2) / 2 - z_3 / 2;
      lightningSprite->d.sprite.coords[3].z = (z_1 + z_2) / 2 - z_3 / 2;
      AddEvtPrim4(gGraphicsPtr->ot, lightningSprite); // 6

      EVT.timer++;
      if (EVT.timer == 197) {
         orbSprite->functionIndex = EVTF_NULL;
         lightningSprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 194
void Evtf194_SpellFx3_DarkStar(EvtData *evt) {
   EvtData *fx;

   fx = Evt_GetUnused();
   fx->functionIndex = EVTF_SPELL_FX2_DARK_STAR;
   fx->x1.s.hi = evt->x1.s.hi;
   fx->z1.s.hi = evt->z1.s.hi;
   fx->d.evtf193.endingFxType = 1;

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 193
void Evtf193_SpellFx2_DarkStar(EvtData *evt) {
   static u8 faces[6][4] = {
       {0, 1, 3, 5}, {0, 2, 3, 4}, {1, 2, 4, 5}, {6, 7, 9, 11}, {6, 8, 9, 10}, {7, 8, 10, 11},
   };

   EvtData *targetSprite;
   EvtData *evt_s2;
   s16 x, z, y;
   s16 a, b, c;
   s16 tmp;
   s32 i, j;
   SVectorXZY *pCoord;
   SVECTOR vertices[12];

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.targetSprite = targetSprite;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.targetSprite;

      switch (evt->state2) {
      case 0:
         if (EVT.timer < 32) {
            evt->state3 = 0;
         } else if (EVT.timer > 163) {
            evt->state3 = 2;
         } else {
            evt->state3 = 1;
         }

         switch (evt->state3) {
         case 0:
            EVT.todo_x4e += 6;
            EVT.todo_x50 += 8;
            gLightColor.r -= 3;
            gLightColor.g -= 3;
            gLightColor.b -= 3;

         // fallthrough
         case 1:
            break;

         case 2:
            EVT.todo_x4e -= 6;
            EVT.todo_x50 -= 8;
            gLightColor.r += 3;
            gLightColor.g += 3;
            gLightColor.b += 3;
            break;
         }

         a = EVT.todo_x50;
         ApplyMaskEffect(452 << 2, 400, 32, 32, 416 << 2, 384, EVT.timer * 2 % 64, 0, GFX_TBD_25,
                         0);

         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_NOOP;
         evt_s2->d.sprite.gfxIdx = GFX_TBD_25;
         evt_s2->d.sprite.clut = 4;
         evt_s2->d.sprite.semiTrans = 0;

         x = targetSprite->x1.n;
         z = targetSprite->z1.n;
         y = targetSprite->y1.n;

         // 1 //

         b = a * rcos(EVT.timer * 0x20) >> 12;
         c = a * rsin(EVT.timer * 0x20) >> 12;

         tmp = EVT.todo_x4e * rcos(EVT.timer * 0x10) / ONE + b;
         evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + tmp;
         evt_s2->d.sprite.coords[2].x = x + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0x556) / ONE + b;
         evt_s2->d.sprite.coords[3].x = x + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0xaac) / ONE + b;

         tmp = EVT.todo_x4e * rsin(EVT.timer * 8) / ONE + 0x80;
         evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y = y + tmp;
         evt_s2->d.sprite.coords[2].y = y + EVT.todo_x4e * rsin(EVT.timer * 8 + 0x556) / ONE + 0x80;
         evt_s2->d.sprite.coords[3].y = y + EVT.todo_x4e * rsin(EVT.timer * 8 + 0xaac) / ONE + 0x80;

         tmp = EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10)) / ONE + c;
         evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + tmp;
         evt_s2->d.sprite.coords[2].z =
             z + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0x556) / ONE + c;
         evt_s2->d.sprite.coords[3].z =
             z + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0xaac) / ONE + c;

         AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

         vertices[0].vx = evt_s2->d.sprite.coords[0].x;
         vertices[0].vz = evt_s2->d.sprite.coords[0].z;
         vertices[0].vy = evt_s2->d.sprite.coords[0].y;
         vertices[1].vx = evt_s2->d.sprite.coords[2].x;
         vertices[1].vz = evt_s2->d.sprite.coords[2].z;
         vertices[1].vy = evt_s2->d.sprite.coords[2].y;
         vertices[2].vx = evt_s2->d.sprite.coords[3].x;
         vertices[2].vz = evt_s2->d.sprite.coords[3].z;
         vertices[2].vy = evt_s2->d.sprite.coords[3].y;

         // 2 //

         b = a * rcos(EVT.timer * 0x20 + 0x800) >> 12;
         c = a * rsin(EVT.timer * 0x20 + 0x800) >> 12;

         tmp = EVT.todo_x4e * rcos(EVT.timer * 0x10) / ONE + c;
         evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + tmp;
         evt_s2->d.sprite.coords[2].z = z + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0x556) / ONE + c;
         evt_s2->d.sprite.coords[3].z = z + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0xaac) / ONE + c;

         tmp = EVT.todo_x4e * rsin(EVT.timer * 8) / ONE + b;
         evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + tmp;
         evt_s2->d.sprite.coords[2].x = x + EVT.todo_x4e * rsin(EVT.timer * 8 + 0x556) / ONE + b;
         evt_s2->d.sprite.coords[3].x = x + EVT.todo_x4e * rsin(EVT.timer * 8 + 0xaac) / ONE + b;

         tmp = EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10)) / ONE + 0x80;
         evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y = y + tmp;
         evt_s2->d.sprite.coords[2].y =
             y + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0x556) / ONE + 0x80;
         evt_s2->d.sprite.coords[3].y =
             y + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0xaac) / ONE + 0x80;

         AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

         vertices[3].vx = evt_s2->d.sprite.coords[0].x;
         vertices[3].vz = evt_s2->d.sprite.coords[0].z;
         vertices[3].vy = evt_s2->d.sprite.coords[0].y;
         vertices[4].vx = evt_s2->d.sprite.coords[2].x;
         vertices[4].vz = evt_s2->d.sprite.coords[2].z;
         vertices[4].vy = evt_s2->d.sprite.coords[2].y;
         vertices[5].vx = evt_s2->d.sprite.coords[3].x;
         vertices[5].vz = evt_s2->d.sprite.coords[3].z;
         vertices[5].vy = evt_s2->d.sprite.coords[3].y;

         // 3 //

         b = a * rcos(EVT.timer * 0x20 + 0x400) >> 12;
         c = a * rsin(EVT.timer * 0x20 + 0x400) >> 12;

         tmp = EVT.todo_x4e * rcos(EVT.timer * 0x10) / ONE + 0x80;
         evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y = y + tmp;
         evt_s2->d.sprite.coords[2].y =
             y + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0x556) / ONE + 0x80;
         evt_s2->d.sprite.coords[3].y =
             y + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0xaac) / ONE + 0x80;

         tmp = EVT.todo_x4e * rsin(EVT.timer * 8) / ONE + c;
         evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + tmp;
         evt_s2->d.sprite.coords[2].z = z + EVT.todo_x4e * rsin(EVT.timer * 8 + 0x556) / ONE + c;
         evt_s2->d.sprite.coords[3].z = z + EVT.todo_x4e * rsin(EVT.timer * 8 + 0xaac) / ONE + c;

         tmp = EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10)) / ONE + b;
         evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + tmp;
         evt_s2->d.sprite.coords[2].x =
             x + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0x556) / ONE + b;
         evt_s2->d.sprite.coords[3].x =
             x + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0xaac) / ONE + b;

         AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

         vertices[6].vx = evt_s2->d.sprite.coords[0].x;
         vertices[6].vz = evt_s2->d.sprite.coords[0].z;
         vertices[6].vy = evt_s2->d.sprite.coords[0].y;
         vertices[7].vx = evt_s2->d.sprite.coords[2].x;
         vertices[7].vz = evt_s2->d.sprite.coords[2].z;
         vertices[7].vy = evt_s2->d.sprite.coords[2].y;
         vertices[8].vx = evt_s2->d.sprite.coords[3].x;
         vertices[8].vz = evt_s2->d.sprite.coords[3].z;
         vertices[8].vy = evt_s2->d.sprite.coords[3].y;

         // 4 //

         b = a * rcos(EVT.timer * 0x20 + 0xc18) >> 12;
         c = a * rsin(EVT.timer * 0x20 + 0xc18) >> 12;

         tmp = EVT.todo_x4e * rcos(EVT.timer * 0x10) / ONE + c;
         evt_s2->d.sprite.coords[0].z = evt_s2->d.sprite.coords[1].z = z + tmp;
         evt_s2->d.sprite.coords[2].z = z + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0x556) / ONE + c;
         evt_s2->d.sprite.coords[3].z = z + EVT.todo_x4e * rcos(EVT.timer * 0x10 + 0xaac) / ONE + c;

         tmp = EVT.todo_x4e * rsin(EVT.timer * 8) / ONE + 0x80;
         evt_s2->d.sprite.coords[0].y = evt_s2->d.sprite.coords[1].y = y + tmp;
         evt_s2->d.sprite.coords[2].y = y + EVT.todo_x4e * rsin(EVT.timer * 8 + 0x556) / ONE + 0x80;
         evt_s2->d.sprite.coords[3].y = y + EVT.todo_x4e * rsin(EVT.timer * 8 + 0xaac) / ONE + 0x80;

         tmp = EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10)) / ONE + b;
         evt_s2->d.sprite.coords[0].x = evt_s2->d.sprite.coords[1].x = x + tmp;
         evt_s2->d.sprite.coords[2].x =
             x + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0x556) / ONE + b;
         evt_s2->d.sprite.coords[3].x =
             x + EVT.todo_x4e * rsin(rcos(EVT.timer * 0x10) + 0xaac) / ONE + b;

         AddEvtPrim4(gGraphicsPtr->ot, evt_s2);

         vertices[9].vx = evt_s2->d.sprite.coords[0].x;
         vertices[9].vz = evt_s2->d.sprite.coords[0].z;
         vertices[9].vy = evt_s2->d.sprite.coords[0].y;
         vertices[10].vx = evt_s2->d.sprite.coords[2].x;
         vertices[10].vz = evt_s2->d.sprite.coords[2].z;
         vertices[10].vy = evt_s2->d.sprite.coords[2].y;
         vertices[11].vx = evt_s2->d.sprite.coords[3].x;
         vertices[11].vz = evt_s2->d.sprite.coords[3].z;
         vertices[11].vy = evt_s2->d.sprite.coords[3].y;

         evt_s2->d.sprite.gfxIdx = GFX_COLOR_13;
         evt_s2->d.sprite.clut = 4;
         evt_s2->d.sprite.semiTrans = 4;

         for (i = 0; i < 6; i++) {
            pCoord = &evt_s2->d.sprite.coords[0];
            for (j = 0; j < 4; j++) {
               pCoord->x = vertices[faces[i][j]].vx;
               pCoord->z = vertices[faces[i][j]].vz;
               pCoord->y = vertices[faces[i][j]].vy;
               pCoord++;
            }
            AddEvtPrim4(gGraphicsPtr->ot, evt_s2);
         }

         evt_s2->functionIndex = EVTF_NULL;
         if (EVT.timer == 196) {
            evt->state2++;
         }

         break;
      } // switch (state2)

      EVT.timer++;
      if (EVT.timer == 191) {
         evt_s2 = Evt_GetUnused();
         evt_s2->functionIndex = EVTF_SPELL_FX2_DAMAGE + EVT.endingFxType;
         evt_s2->x1.s.hi = evt->x1.s.hi;
         evt_s2->z1.s.hi = evt->z1.s.hi;
      } else if (EVT.timer == 196) {
         evt->functionIndex = EVTF_NULL;
      }

      break;
   }
}

#undef EVTF
#define EVTF 192
void Evtf192_SpellFx2_PerfectGuard(EvtData *evt) {
   extern SVECTOR s_faces_80123a4c[6][4];

   EvtData *targetSprite;
   EvtData *evt_s2;
   POLY_FT4 *poly;
   s32 i, j;
   s32 flag;
   s32 fade;
   s16 xOfs, zOfs;
   s16 a;
   MATRIX matrix;
   VECTOR local_1c0[6][4];
   SVECTOR local_40;

   switch (evt->state) {
   case 0:
      targetSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = targetSprite->x1.n;
      evt->y1.n = targetSprite->y1.n + 0x80;
      evt->z1.n = targetSprite->z1.n;

      for (i = 0; i < 6; i++) {
         s_faces_80123a4c[i][0].vx = -0xa0;
         s_faces_80123a4c[i][0].vy = -0xa0;
         s_faces_80123a4c[i][0].vz = -0xa0;
         s_faces_80123a4c[i][1].vx = 0xa0;
         s_faces_80123a4c[i][1].vy = -0xa0;
         s_faces_80123a4c[i][1].vz = -0xa0;
         s_faces_80123a4c[i][2].vx = -0xa0;
         s_faces_80123a4c[i][2].vy = -0xa0;
         s_faces_80123a4c[i][2].vz = 0xa0;
         s_faces_80123a4c[i][3].vx = 0xa0;
         s_faces_80123a4c[i][3].vy = -0xa0;
         s_faces_80123a4c[i][3].vz = 0xa0;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.semiTrans = 2;

      gGfxSubTextures[GFX_TILED_DIAMONDS_DYN_1].x = EVT.todo_x5c % 0x20;
      gGfxSubTextures[GFX_TILED_DIAMONDS_DYN_1].y = 0x80 + EVT.todo_x5c % 0x20;
      gGfxSubTextures[GFX_TILED_DIAMONDS_DYN_1].w = 0x20;
      gGfxSubTextures[GFX_TILED_DIAMONDS_DYN_1].h = 0x20;
      EVT.todo_x5c++;

      // 1 //
      switch (evt->state3) {
      case 1:

         switch (evt->state2) {
         case 0:
            s_faces_80123a4c[1][0].vx = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][2].vx = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 += 0x80;
            if (EVT.todo_x24 > 0x400) {
               EVT.todo_x24 = 0;
               evt->state2++;
            }
            break;

         case 1:
            s_faces_80123a4c[2][0].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][1].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 += 0x80;
            if (EVT.todo_x24 > 0x400) {
               EVT.todo_x24 = 0;
               evt->state2++;
            }
            break;

         case 2:
            s_faces_80123a4c[3][1].vx = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[3][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[3][3].vx = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[3][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 += 0x80;
            if (EVT.todo_x24 > 0x400) {
               EVT.todo_x24 = 0;
               evt->state2++;
            }
            break;

         case 3:
            s_faces_80123a4c[4][2].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[4][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[4][3].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[4][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            s_faces_80123a4c[5][2].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[5][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][3].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[5][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 += 0x80;
            if (EVT.todo_x24 > 0x400) {
               EVT.todo_x24 = 0;
               evt->state2++;
            }
            break;

         case 4:
            s_faces_80123a4c[5][0].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][1].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 += 0x80;
            if (EVT.todo_x24 > 0x400) {
               EVT.todo_x24 = 0;
               evt->state2 = 0;
               EVT.todo_x58++;
            }
            break;

         case 5:
            break;
         }

         break;

      case 2:
         PushMatrix();
         local_40.vx = 0;
         local_40.vz = 0;
         local_40.vy = rcos(EVT.todo_x5e * 0x40);
         RotMatrix(&local_40, &matrix);
         matrix.t[0] = 0;
         matrix.t[1] = 0;
         matrix.t[2] = 0;
         SetRotMatrix(&matrix);
         SetTransMatrix(&matrix);
         for (i = 0; i < 6; i++) {
            for (j = 0; j < 4; j++) {
               RotTrans(&s_faces_80123a4c[i][j], &local_1c0[i][j], &flag);
            }
         }
         PopMatrix();
         break;

      case 3:

         switch (evt->state2) {
         case 0:
            s_faces_80123a4c[5][0].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][1].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 -= 0x80;
            if (EVT.todo_x24 < 0) {
               EVT.todo_x24 = 0x400;
               evt->state2++;
            }
            break;

         case 1:
            s_faces_80123a4c[4][2].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[4][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[4][3].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[4][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            s_faces_80123a4c[5][2].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[5][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[5][3].vz = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[5][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 -= 0x80;
            if (EVT.todo_x24 < 0) {
               EVT.todo_x24 = 0x400;
               evt->state2++;
            }
            break;

         case 2:
            s_faces_80123a4c[3][1].vx = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[3][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[3][3].vx = 0xa0 - 0x140 * rsin(EVT.todo_x24) / ONE;
            s_faces_80123a4c[3][3].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 -= 0x80;
            if (EVT.todo_x24 < 0) {
               EVT.todo_x24 = 0x400;
               evt->state2++;
            }
            break;

         case 3:
            s_faces_80123a4c[2][0].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][1].vz = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[2][1].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 -= 0x80;
            if (EVT.todo_x24 < 0) {
               EVT.todo_x24 = 0x400;
               evt->state2++;
            }
            break;

         case 4:
            s_faces_80123a4c[1][0].vx = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][0].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][2].vx = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;
            s_faces_80123a4c[1][2].vy = 0x140 * rsin(EVT.todo_x24) / ONE - 0xa0;

            EVT.todo_x24 -= 0x80;
            if (EVT.todo_x24 < 0) {
               EVT.todo_x24 = 0x400;
               EVT.todo_x5a++;
            }
            break;

         case 5:
            break;
         }

         break;

      case 0:
      case 4:
         a = 0x400 * (ONE - rsin(EVT.todo_x24)) / ONE;
         xOfs = a * rsin(EVT.todo_x24 * 8) / ONE;
         zOfs = a * rcos(EVT.todo_x24 * 8) / ONE;
         break;
      }

      // 2 //
      switch (evt->state3) {
      case 0:
         for (i = 0; i < 6; i++) {
            evt_s2->d.sprite.coords[0].x = evt->x1.n + s_faces_80123a4c[i][0].vx + xOfs;
            evt_s2->d.sprite.coords[0].y = evt->y1.n + s_faces_80123a4c[i][0].vy;
            evt_s2->d.sprite.coords[0].z = evt->z1.n + s_faces_80123a4c[i][0].vz + zOfs;
            evt_s2->d.sprite.coords[1].x = evt->x1.n + s_faces_80123a4c[i][1].vx + xOfs;
            evt_s2->d.sprite.coords[1].y = evt->y1.n + s_faces_80123a4c[i][1].vy;
            evt_s2->d.sprite.coords[1].z = evt->z1.n + s_faces_80123a4c[i][1].vz + zOfs;
            evt_s2->d.sprite.coords[2].x = evt->x1.n + s_faces_80123a4c[i][2].vx + xOfs;
            evt_s2->d.sprite.coords[2].y = evt->y1.n + s_faces_80123a4c[i][2].vy;
            evt_s2->d.sprite.coords[2].z = evt->z1.n + s_faces_80123a4c[i][2].vz + zOfs;
            evt_s2->d.sprite.coords[3].x = evt->x1.n + s_faces_80123a4c[i][3].vx + xOfs;
            evt_s2->d.sprite.coords[3].y = evt->y1.n + s_faces_80123a4c[i][3].vy;
            evt_s2->d.sprite.coords[3].z = evt->z1.n + s_faces_80123a4c[i][3].vz + zOfs;

            evt_s2->x1.n = (evt_s2->d.sprite.coords[1].x + evt_s2->d.sprite.coords[2].x +
                            evt_s2->d.sprite.coords[3].x + evt_s2->d.sprite.coords[0].x) /
                           4;
            evt_s2->y1.n = (evt_s2->d.sprite.coords[1].y + evt_s2->d.sprite.coords[2].y +
                            evt_s2->d.sprite.coords[3].y + evt_s2->d.sprite.coords[0].y) /
                           4;
            evt_s2->z1.n = (evt_s2->d.sprite.coords[1].z + evt_s2->d.sprite.coords[2].z +
                            evt_s2->d.sprite.coords[3].z + evt_s2->d.sprite.coords[0].z) /
                           4;

            evt_s2->d.sprite.gfxIdx = GFX_TILED_DIAMONDS_DYN_1;
            AddEvtPrim3(gGraphicsPtr->ot, evt_s2);
         }

         evt_s2->functionIndex = EVTF_NULL;
         break;

      case 2:
         for (i = 0; i < 6; i++) {
            evt_s2->d.sprite.coords[0].x = evt->x1.n + local_1c0[i][0].vx;
            evt_s2->d.sprite.coords[0].y = evt->y1.n + local_1c0[i][0].vy;
            evt_s2->d.sprite.coords[0].z = evt->z1.n + local_1c0[i][0].vz;
            evt_s2->d.sprite.coords[1].x = evt->x1.n + local_1c0[i][1].vx;
            evt_s2->d.sprite.coords[1].y = evt->y1.n + local_1c0[i][1].vy;
            evt_s2->d.sprite.coords[1].z = evt->z1.n + local_1c0[i][1].vz;
            evt_s2->d.sprite.coords[2].x = evt->x1.n + local_1c0[i][2].vx;
            evt_s2->d.sprite.coords[2].y = evt->y1.n + local_1c0[i][2].vy;
            evt_s2->d.sprite.coords[2].z = evt->z1.n + local_1c0[i][2].vz;
            evt_s2->d.sprite.coords[3].x = evt->x1.n + local_1c0[i][3].vx;
            evt_s2->d.sprite.coords[3].y = evt->y1.n + local_1c0[i][3].vy;
            evt_s2->d.sprite.coords[3].z = evt->z1.n + local_1c0[i][3].vz;

            evt_s2->x1.n = (evt_s2->d.sprite.coords[1].x + evt_s2->d.sprite.coords[2].x +
                            evt_s2->d.sprite.coords[3].x + evt_s2->d.sprite.coords[0].x) /
                           4;
            evt_s2->y1.n = (evt_s2->d.sprite.coords[1].y + evt_s2->d.sprite.coords[2].y +
                            evt_s2->d.sprite.coords[3].y + evt_s2->d.sprite.coords[0].y) /
                           4;
            evt_s2->z1.n = (evt_s2->d.sprite.coords[1].z + evt_s2->d.sprite.coords[2].z +
                            evt_s2->d.sprite.coords[3].z + evt_s2->d.sprite.coords[0].z) /
                           4;

            evt_s2->d.sprite.gfxIdx = GFX_TILED_DIAMONDS_DYN_1;
            AddEvtPrim3(gGraphicsPtr->ot, evt_s2);
         }

         evt_s2->functionIndex = EVTF_NULL;
         break;

      case 1:
      case 3:

         for (i = 0; i < 6; i++) {
            evt_s2->d.sprite.coords[0].x = evt->x1.n + s_faces_80123a4c[i][0].vx;
            evt_s2->d.sprite.coords[0].y = evt->y1.n + s_faces_80123a4c[i][0].vy;
            evt_s2->d.sprite.coords[0].z = evt->z1.n + s_faces_80123a4c[i][0].vz;
            evt_s2->d.sprite.coords[1].x = evt->x1.n + s_faces_80123a4c[i][1].vx;
            evt_s2->d.sprite.coords[1].y = evt->y1.n + s_faces_80123a4c[i][1].vy;
            evt_s2->d.sprite.coords[1].z = evt->z1.n + s_faces_80123a4c[i][1].vz;
            evt_s2->d.sprite.coords[2].x = evt->x1.n + s_faces_80123a4c[i][2].vx;
            evt_s2->d.sprite.coords[2].y = evt->y1.n + s_faces_80123a4c[i][2].vy;
            evt_s2->d.sprite.coords[2].z = evt->z1.n + s_faces_80123a4c[i][2].vz;
            evt_s2->d.sprite.coords[3].x = evt->x1.n + s_faces_80123a4c[i][3].vx;
            evt_s2->d.sprite.coords[3].y = evt->y1.n + s_faces_80123a4c[i][3].vy;
            evt_s2->d.sprite.coords[3].z = evt->z1.n + s_faces_80123a4c[i][3].vz;

            evt_s2->x1.n = (evt_s2->d.sprite.coords[1].x + evt_s2->d.sprite.coords[2].x +
                            evt_s2->d.sprite.coords[3].x + evt_s2->d.sprite.coords[0].x) /
                           4;
            evt_s2->y1.n = (evt_s2->d.sprite.coords[1].y + evt_s2->d.sprite.coords[2].y +
                            evt_s2->d.sprite.coords[3].y + evt_s2->d.sprite.coords[0].y) /
                           4;
            evt_s2->z1.n = (evt_s2->d.sprite.coords[1].z + evt_s2->d.sprite.coords[2].z +
                            evt_s2->d.sprite.coords[3].z + evt_s2->d.sprite.coords[0].z) /
                           4;

            evt_s2->d.sprite.gfxIdx = GFX_TILED_DIAMONDS_DYN_1;
            AddEvtPrim3(gGraphicsPtr->ot, evt_s2);

            if (evt->state3 == 3) {
               fade = ~evt->mem;
               poly = &gGraphicsPtr->quads[gQuadIndex - 1];
               setRGB0(poly, fade, fade, fade);
               evt->mem++;
               if (evt->mem >= 0xff) {
                  evt->mem = 0xff;
               }
            }
         }

         evt_s2->functionIndex = EVTF_NULL;
         break;

      default:
         evt_s2->functionIndex = EVTF_NULL;
         break;
      }

      // 3 //
      switch (evt->state3) {
      case 0:
         EVT.todo_x24 += 0x20;
         if (EVT.todo_x24 >= 0x400) {
            evt->state3++;
            EVT.todo_x24 = 0;
         }
         break;

      case 1:
         if (EVT.todo_x58 != 0) {
            evt->state3++;
         }
         break;

      case 2:
         EVT.todo_x5e++;
         if (EVT.todo_x5e == 0x40) {
            evt->state3++;
            EVT.todo_x24 = 0x400;
         }
         break;

      case 3:
         if (EVT.todo_x5a != 0) {
            evt->state3++;
            EVT.todo_x24 = 0x20;
            evt_s2 = Evt_GetUnused();
            evt_s2->functionIndex = EVTF_ATTACK_INFO_MARKER;
            evt_s2->x1.s.hi = evt->x1.s.hi;
            evt_s2->z1.s.hi = evt->z1.s.hi;
            evt_s2->d.evtf052.type = 2;
            evt_s2->d.evtf052.clut = 9;
         }
         break;

      case 4:
         EVT.todo_x24--;
         if (EVT.todo_x24 <= 0) {
            gSignal3 = 1;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}
