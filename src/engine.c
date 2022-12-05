#include "common.h"
#include "audio.h"
#include "state.h"
#include "evt.h"
#include "units.h"
#include "window.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

#include "PsyQ/kernel.h"

void UpdateEngine(void) {
   u32 osc;
   s32 flag;

   UpdateAudio();
   rand();
   if (gState.vsyncMode == 2) {
      gState.frameCounter += 2;
   } else {
      gState.frameCounter += 1;
   }
   if (gState.debug) {
      /* while holding 2P Circle to freeze, press 2P R1 to frame-step */
      while ((PadRead(1) >> 0x10) & PAD_CIRCLE) {
         UpdateInput();
         if (gPad2StateNewPresses & PAD_R1)
            break;
      }
      /* 1P Select: setup transition to state 98 (debug menu) */
      if (gState.debug && gPadStateNewPresses & PAD_SELECT) {
         PerformAudioCommand(6);
         gState.primary = 1;
      }
      /* 2P Start: win battle */
      if (gState.debug && (gPad2StateNewPresses & PAD_START)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
   }
   gState.unitMarkerSpin += 0x20;
   osc = (gOscillation + 0x100) & 0xfff;
   gOscillation = osc;
   gGridColorOscillation = rcos(osc) * 100 >> 12;
   gGridColorOscillation += 155U;
   gQuadIndex = 0;
   UpdateInput();
   UpdateCamera();
   UpdateLight(0x38);
   gGraphicsPtr = (gGraphicsPtr == &gGraphicBuffers[0]) ? &gGraphicBuffers[1] : &gGraphicBuffers[0];
   ClearOTag(gGraphicsPtr->ot, OT_SIZE);
   CenterCamera(0);
   RotTrans(&gCameraPos, (VECTOR *)gCameraMatrix.t, &flag);
   SetTransMatrix(&gCameraMatrix);
   if (!gState.fieldRenderingDisabled) {
      RenderField();
   }
   Evt_Execute();
   DrawSync(1);
   FntPrint("VSYNC : %d\n", GetRCnt(RCntCNT1));
   DrawSync(0);
   DecodeUnitSprites();
   if (gState.vsyncNoWait && (gPad2State & PAD_L1)) {
      VSync(1);
   } else {
      VSync(gState.vsyncMode);
   }
   ResetRCnt(RCntCNT1);
   PutDrawEnv(&gGraphicsPtr->drawEnv);
   PutDispEnv(&gGraphicsPtr->dispEnv);
   DrawOTag(gGraphicsPtr->ot);
   FntFlush(-1);
}