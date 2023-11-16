#include "common.h"
#include "evt.h"
#include "state.h"
#include "graphics.h"

void SetScreenEffectOrdering(s16 otOfs) { gState.screenEffect->d.evtf369.otOfs = otOfs; }

void Noop_800a8b0c(void) {}

#undef EVTF
#define EVTF 369
void Evtf369_ScreenEffect(EvtData *evt) {
   extern DR_MODE s_drawModes1_80124804[2];
   extern DR_MODE s_drawModes2_8012481c[2];
   extern POLY_F4 s_polys_801247d4[2];

   s16 otOfs;
   s32 i;
   POLY_F4 *pPoly;
   s16 r, g, b;
   s16 r2, g2, b2;
   u8 intensity;
   DR_MODE *pDrawMode;
   // state2: abr

   evt->mem = (evt->mem + 1) & 1;
   otOfs = EVT.otOfs + (OT_SIZE - 3);
   if (otOfs > (OT_SIZE - 2)) {
      otOfs = (OT_SIZE - 2);
   }

   switch (evt->state) {
   case 0:
      EVT.semiTrans = 1;
      pPoly = &s_polys_801247d4[0];

      for (i = 0; i < 2; i++) {
         SetPolyF4(pPoly);
         if (EVT.semiTrans != 0) {
            setSemiTrans(pPoly, 1);
         } else {
            setSemiTrans(pPoly, 0);
         }
         setXYWH(pPoly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
         pPoly++;
         SetDrawMode(&s_drawModes1_80124804[i], 0, 0, GetTPage(0, 1, 640, 0), NULL);
         SetDrawMode(&s_drawModes2_8012481c[i], 0, 0, GetTPage(0, 0, 640, 0), NULL);
      }

      EVT.intensity = 0;
      LO(EVT.speed) = 2; //?
      break;

   case 1:
      if (evt->state2 == 0) {
         break;
      }
      for (i = 0; i < 2; i++) {
         SetDrawMode(&s_drawModes1_80124804[i], 0, 0, GetTPage(0, evt->state2, 640, 0), NULL);
      }

      EVT.speed = evt->state3;
      EVT.intensity = 0;
      evt->state++;

   // fallthrough
   case 2:
      EVT.intensity += EVT.speed;
      if (EVT.intensity >= 255) {
         EVT.intensity = 255;
         evt->state2 = 0;
         evt->state++;
      }
      break;

   case 3:
      if (evt->state2 == 0) {
         break;
      }
      EVT.speed = evt->state3;
      EVT.intensity = 255;
      evt->state++;

   // fallthrough
   case 4:
      EVT.intensity -= EVT.speed;
      if (EVT.intensity <= 0) {
         EVT.intensity = 0;
         evt->state2 = 0;
         evt->state = 1;
         EVT.unused_0x30 = 0;
      }
      break;

   case 5:
   case 6:
   case 7:
      r = EVT.color.r;
      g = EVT.color.g;
      b = EVT.color.b;
      r += EVT.rd;
      g += EVT.gd;
      b += EVT.bd;
      if (EVT.rd > 0 && r > EVT.rmax) {
         r = EVT.rmax;
      }
      if (EVT.gd > 0 && g > EVT.gmax) {
         g = EVT.gmax;
      }
      if (EVT.bd > 0 && b > EVT.bmax) {
         b = EVT.bmax;
      }
      if (EVT.rd < 0 && r < EVT.rmax) {
         r = EVT.rmax;
      }
      if (EVT.gd < 0 && g < EVT.gmax) {
         g = EVT.gmax;
      }
      if (EVT.bd < 0 && b < EVT.bmax) {
         b = EVT.bmax;
      }
      EVT.color.r = r;
      EVT.color.g = g;
      EVT.color.b = b;

      if (evt->state == 7) {
         if (r == EVT.rmax && g == EVT.gmax && b == EVT.bmax) {
            evt->state++;
         }
      }
      break;

   case 8:
      EVT.rd = 0;
      EVT.gd = 0;
      EVT.bd = 0;
      evt->state = 0;
      break;
   }

   switch (evt->state) {
   case 0:
   case 1:
      break;

   case 2:
   case 3:
   case 4:
      pDrawMode = &s_drawModes2_8012481c[evt->mem];
      addPrim(&gGraphicsPtr->ot[OT_SIZE - 2], pDrawMode);

      pPoly = &s_polys_801247d4[evt->mem];
      intensity = EVT.intensity;
      setRGB0(pPoly, intensity, intensity, intensity);
      addPrim(&gGraphicsPtr->ot[otOfs], pPoly);

      pDrawMode = &s_drawModes1_80124804[evt->mem];
      addPrim(&gGraphicsPtr->ot[otOfs], pDrawMode);

      break;

   case 5:
   case 6:
   case 7:
      pPoly = s_polys_801247d4;
      if (EVT.semiTrans != 0) {
         setSemiTrans(&s_polys_801247d4[evt->mem], 1);
      } else {
         setSemiTrans(&s_polys_801247d4[evt->mem], 0);
      }

      SetDrawMode(&s_drawModes1_80124804[evt->mem], 0, 0, GetTPage(0, evt->state2, 640, 0), NULL);

      pDrawMode = &s_drawModes2_8012481c[evt->mem];
      addPrim(&gGraphicsPtr->ot[OT_SIZE - 2], pDrawMode);

      r2 = EVT.color.r;
      g2 = EVT.color.g;
      b2 = EVT.color.b;

      pPoly = &s_polys_801247d4[evt->mem];
      setRGB0(pPoly, r2, g2, b2);
      addPrim(&gGraphicsPtr->ot[otOfs], pPoly);

      pDrawMode = &s_drawModes1_80124804[evt->mem];
      addPrim(&gGraphicsPtr->ot[otOfs], pDrawMode);

      break;

   case 8:
      break;
   }
}

void SetScreenEffectIntensity(s32 intensity) {
   gState.screenEffect->d.evtf369.color.r = intensity;
   gState.screenEffect->d.evtf369.color.g = intensity;
   gState.screenEffect->d.evtf369.color.b = intensity;
}

void Evtf316_Noop(EvtData *evt) {}

void FadeOutScreen(s32 abr, s32 speed) {
   EvtData *screenEffect;

   screenEffect = gState.screenEffect;
   screenEffect->state2 = abr;
   screenEffect->d.evtf369.rd = screenEffect->d.evtf369.gd = screenEffect->d.evtf369.bd = speed;
   screenEffect->d.evtf369.rmax = screenEffect->d.evtf369.gmax = screenEffect->d.evtf369.bmax = 255;
   screenEffect->state = 6;
}

void FadeInScreen(s32 abr, s32 speed) {
   EvtData *screenEffect;

   screenEffect = gState.screenEffect;
   screenEffect->state2 = abr;
   screenEffect->d.evtf369.rd = screenEffect->d.evtf369.gd = screenEffect->d.evtf369.bd = -speed;
   screenEffect->d.evtf369.rmax = screenEffect->d.evtf369.gmax = screenEffect->d.evtf369.bmax = 0;
   screenEffect->state = 7;
}

EvtData *gScreenFade;

void Event_FadeOutScreen(s32 abr, s32 speed) {
   gScreenFade->state = 1;
   gScreenFade->state3 = abr;
   gScreenFade->d.evtf795.delta = speed;
   gScreenFade->d.evtf795.max = 255;
}

void Event_FadeInScreen(s32 abr, s32 speed) {
   gScreenFade->state = 2;
   gScreenFade->state3 = abr;
   gScreenFade->d.evtf795.delta = -speed;
   gScreenFade->d.evtf795.max = 0;
}

#undef EVTF
#define EVTF 795
void Evtf795_EventFade(EvtData *evt) {
   //? Why so many elements?
   extern POLY_F4 s_polys_80124834[2][16];
   extern DR_MODE s_drawModes_80124b34[2][32];

   POLY_F4 *pPoly;
   DR_MODE *pDrawMode;
   s16 fade;
   s16 finished;
   s16 otOfs;
   u8 b;
   // state3: abr

   evt->mem = (evt->mem + 1) & 1;
   otOfs = OT_SIZE - 2;

   switch (evt->state) {
   case 0:
      break;

   case 1:
   case 2:
      fade = EVT.fade;
      fade += EVT.delta;
      finished = 0;
      if (EVT.delta > 0 && fade > EVT.max) {
         fade = EVT.max;
         finished = 1;
      }
      if (EVT.delta < 0 && fade < EVT.max) {
         fade = EVT.max;
         finished = 1;
      }
      EVT.fade = fade;
      if (evt->state == 2 && finished) {
         evt->state++;
      }
      break;

   // fallthrough
   case 3:
      evt->state = 0;
      break;
   }

   switch (evt->state) {
   case 1:
   case 2:
      pDrawMode = &s_drawModes_80124b34[evt->mem][0];
      SetDrawMode(pDrawMode, 0, 0, GetTPage(0, 1, 640, 0), NULL);
      AddPrim(&gGraphicsPtr->ot[otOfs], pDrawMode);

      pPoly = &s_polys_80124834[evt->mem][0];
      setPolyF4(pPoly);
      setXYWH(pPoly, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
      setSemiTrans(pPoly, 1);
      b = EVT.fade;
      setRGB0(pPoly, b, b, b);
      AddPrim(&gGraphicsPtr->ot[otOfs], pPoly);

      pDrawMode = &s_drawModes_80124b34[evt->mem][1];
      SetDrawMode(pDrawMode, 0, 0, GetTPage(0, evt->state3, 0, 0), NULL);
      AddPrim(&gGraphicsPtr->ot[otOfs], pDrawMode);
      break;
   }
}

s32 func_800A96A8(s32 theta1, s16 theta2) {
   s16 diff;
   s16 ret;

   diff = theta2 - (theta1 & 0xfff);
   ret = theta2;
   if (diff > DEG(0)) {
      if (diff > DEG(180)) {
         ret = theta2 - DEG(360);
      } else if (diff < DEG(-180)) {
         ret = theta2 + DEG(360);
      }
   }
   return ret;
}

#undef EVTF
#define EVTF 387
void Evtf387_FullscreenImage(EvtData *evt) {
   extern DR_MODE s_drawModes1_80124e34[2];
   extern DR_MODE s_drawModes2_80124e4c[2];
   extern SPRT s_sprites_80124e64[2][2];

   s32 i;
   EvtData *p;
   SPRT *pSprt;
   DR_MODE *pDrawMode;

   switch (evt->state) {
   case 0:
      for (i = 0; i < EVT_DATA_CT; i++) {
         p = &gEvtDataArray[i];
         if (p->functionIndex == EVTF_FULLSCREEN_IMAGE && p != evt) {
            evt->functionIndex = EVTF_NULL;
            return;
         }
      }
      for (i = 0; i < 2; i++) {
         pDrawMode = &s_drawModes1_80124e34[i];
         SetDrawMode(pDrawMode, 0, 0, GetTPage(1, 0, 320, 256), NULL);
         pDrawMode = &s_drawModes2_80124e4c[i];
         SetDrawMode(pDrawMode, 0, 0, GetTPage(1, 0, 448, 256), NULL);

         pSprt = &s_sprites_80124e64[i][0];
         SetSprt(&pSprt[0]);
         setRGB0(&pSprt[0], 128, 128, 128);
         setXY0(&pSprt[0], 0, 0);
         setUV0(&pSprt[0], 0, 0);
         setClut(&pSprt[0], 768, 248);
         setWH(&pSprt[0], 256, 240);

         SetSprt(&pSprt[1]);
         setRGB0(&pSprt[1], 128, 128, 128);
         setXY0(&pSprt[1], 256, 0);
         setUV0(&pSprt[1], 0, 0);
         setClut(&pSprt[1], 768, 248);
         setWH(&pSprt[1], 64, 240);
      }
      evt->state++;

   // fallthrough
   case 1:
      evt->mem++;
      evt->mem %= 2;
      pSprt = &s_sprites_80124e64[evt->mem][0];
      AddPrim(&gGraphicsPtr->ot[2], &pSprt[1]);
      AddPrim(&gGraphicsPtr->ot[2], &s_drawModes2_80124e4c[evt->mem]);
      AddPrim(&gGraphicsPtr->ot[2], &pSprt[0]);
      AddPrim(&gGraphicsPtr->ot[2], &s_drawModes1_80124e34[evt->mem]);
      break;
   }
}

void LoadFullscreenImage(s32 cdf) {
   RECT timRect;
   RECT clutRect;
   TIM_IMAGE tim;

   LoadCdFile(cdf, 0);
   OpenTIM(&gScratch3_80180210[0]);

   if (ReadTIM(&tim) != 0) {
      setRECT(&timRect, 320, 256, 160, 240);
      setRECT(&clutRect, 768, 248, 256, 1);
      LoadImage(&timRect, tim.paddr);
      LoadImage(&clutRect, tim.caddr);
   }
}
