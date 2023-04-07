#include "common.h"
#include "state.h"
#include "evt.h"
#include "audio.h"
#include "cd_files.h"
#include "graphics.h"

typedef struct MovieInfo {
   s32 startingSector;
   s32 frameCt;
   s32 skip; //? 1:can skip w/ start, 2:wait for x/o at end, 0:proceed automatically
   s32 transition;
   s16 volume;
   s16 padding;
} MovieInfo;

s32 s_movieFinished_80123274;
s32 s_delay_80123278;
s32 s_movieIdx_8012327c;

void State_Movie(void) {
   s32 i;

   static MovieInfo movies[17] = {
       {0x1fded, 0x8f, 0x1, 0x63, 0x56, 0x0},   {0x20385, 0x1db, 0x1, 0x63, 0x7f, 0x0},
       {0x21618, 0x9e8, 0x0, 0x63, 0x7f, 0x0},  {0x34172, 0xb6, 0x2, 0x63, 0x7f, 0x0},
       {0x27a08, 0x25a, 0x0, 0x14, 0x60, 0x0},  {0x2919f, 0x7f0, 0x0, 0x14, 0x50, 0x0},
       {0x2e115, 0x38b, 0x0, 0x14, 0x50, 0x0},  {0x30604, 0x342, 0x0, 0x14, 0x48, 0x0},
       {0x32773, 0x298, 0x0, 0x14, 0x50, 0x0},  {0x36c20, 0x5d6, 0x0, 0x1e, 0x70, 0x0},
       {0x3a68c, 0x10e3, 0x0, 0x63, 0x7f, 0x0}, {0x3488f, 0xb6, 0x2, 0x63, 0x7f, 0x0},
       {0x34fac, 0xb6, 0x2, 0x63, 0x7f, 0x0},   {0x356c9, 0xb6, 0x2, 0x63, 0x7f, 0x0},
       {0x35de6, 0xb6, 0x2, 0x63, 0x7f, 0x0},   {0x36503, 0xb6, 0x2, 0x63, 0x7f, 0x0},
       {0x0, 0x0, 0x0, 0x63, 0x0, 0x0}};

   switch (gState.secondary) {
   case 0:
      FadeInScreen(2, 0xff);
      Evt_ResetFromIdx10();
      gState.vsyncMode = 0;
      gState.fieldRenderingDisabled = 1;
      gState.inEvent = 0;
      ClearScreen(1);
      s_movieIdx_8012327c = gState.movieIdxToPlay;
      SsSetMVol(0x7f, 0x7f);
      SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
      s_delay_80123278 = 10;
      gState.secondary++;

   // fallthrough
   case 1:
      if (--s_delay_80123278 != 0) {
         break;
      }
      gState.secondary = 10;

   // fallthrough
   case 10:
      SsSetSerialVol(SS_SERIAL_A, movies[s_movieIdx_8012327c].volume,
                     movies[s_movieIdx_8012327c].volume);
      PlayMovie(movies[s_movieIdx_8012327c].startingSector, movies[s_movieIdx_8012327c].frameCt, 1);
      gState.secondary++;

   // fallthrough
   case 11:
      s_movieFinished_80123274 = Movie_PlayNextFrame();
      if (s_movieFinished_80123274 ||
          (movies[s_movieIdx_8012327c].skip == 1 && (gPadStateNewPresses & PAD_START))) {
         SsSetSerialVol(SS_SERIAL_A, 0, 0);
         Movie_Finish();
         gState.secondary =
             (movies[s_movieIdx_8012327c].skip == 2) ? 12 : movies[s_movieIdx_8012327c].transition;
      }
      if (gState.debug && (gPad2StateNewPresses & PAD_START)) {
         SsSetSerialVol(SS_SERIAL_A, 0, 0);
         Movie_Finish();
         gState.secondary = movies[s_movieIdx_8012327c].transition;
      }
      break;

   case 12:
      if (gPadStateNewPresses & (PAD_CIRCLE | PAD_X)) {
         gState.secondary = movies[s_movieIdx_8012327c].transition;
      }
      break;

   case 20:
      s_movieIdx_8012327c = gState.movieIdxToPlay + 7;
      gState.secondary = 10;
      break;

   case 30:
      s_delay_80123278 = 180;
      gState.secondary++;

   // fallthrough
   case 31:
      if (--s_delay_80123278 == 0) {
         s_movieIdx_8012327c = gState.movieIdxToPlay + 1;
         gState.secondary = 10;
      }
      break;

   case 99:
      ClearScreen(0);
      FadeOutScreen(2, 0xff);
      gState.secondary++;
      break;

   case 100:
      gState.secondary = 0;
      gState.state3 = 0;
      gState.state4 = 0;

      switch (gState.movieIdxToPlay) {
      case MOV_LOGO_USA_STR:
         if (s_movieFinished_80123274) {
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_TITLE_WS_STR;
            break;
         }
      // fallthrough
      case MOV_TITLE_WS_STR:
         gState.primary = STATE_TITLE_SCREEN;
         gState.state7 = 0;
         break;
      case MOV_1BU_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 0;
         break;
      case MOV_EPI1_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 1;
         break;
      case MOV_2BU_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 0x13;
         break;
      case MOV_3BU_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 0x23;
         break;
      case MOV_4BU_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 0x33;
         break;
      case MOV_5BU_WS_STR:
         gState.primary = STATE_26;
         gState.scene = 0x44;
         break;
      case MOV_6BU_WS_STR:
         gState.primary = STATE_7;
         gState.townState = 0x20;
         break;
      case MOV_END1_WS_STR:
         gState.primary = STATE_12;
         gState.scene = 0x5e;
      }

      if (gState.primary != STATE_MOVIE) {
         for (i = 0; i < ARRAY_COUNT(gState.portraitsToLoad); i++) {
            gState.portraitsToLoad[i] = 0;
            gState.currentPortraits[i] = 0;
         }
      }
      if (gState.primary == STATE_7) {
         LoadFWD();
      }
   }
}

void ClearScreen(s32 is24bit) {
   RECT bufferRect1 = {0, 16, 480, 240};
   RECT bufferRect2 = {0, 272, 480, 240};

   ClearImage(&bufferRect1, 0, 0, 0);
   ClearImage(&bufferRect2, 0, 0, 0);

   gGraphicBuffers[0].dispEnv.isrgb24 = gGraphicBuffers[1].dispEnv.isrgb24 = is24bit;

   SetDefDrawEnv(&gGraphicBuffers[0].drawEnv, 0, 16,
                 (is24bit ? SCREEN_WIDTH * 3 / 2 : SCREEN_WIDTH), SCREEN_HEIGHT);
   SetDefDrawEnv(&gGraphicBuffers[1].drawEnv, 0, 272,
                 (is24bit ? SCREEN_WIDTH * 3 / 2 : SCREEN_WIDTH), SCREEN_HEIGHT);
}

void State_FinishChapter(void) {
   switch (gState.secondary) {
   case 0:
      FadeInScreen(2, 10);
      Evt_ResetFromIdx10();
      gState.vsyncMode = 0;
      gClearSavedPadState = 1;
      gState.fieldRenderingDisabled = 1;

      switch (gState.chapterOutro) {
      case 0:
         LoadFullscreenImage(CDF_USEND_1_TIM);
         break;
      case 1:
         LoadFullscreenImage(CDF_USEND_2_TIM);
         break;
      case 2:
         LoadFullscreenImage(CDF_USEND_3_TIM);
         break;
      case 3:
         LoadFullscreenImage(CDF_USEND_4_TIM);
         break;
      case 4:
         LoadFullscreenImage(CDF_USEND_5_TIM);
         break;
      }

      gTempEvt = Evt_GetUnused();
      gTempEvt->functionIndex = EVTF_FULLSCREEN_IMAGE;
      gState.secondary++;
      break;

   case 1:
      if (gPadStateNewPresses & PAD_CIRCLE) {
         gState.secondary = 0;
         gState.state3 = 0;
         gState.state4 = 0;

         switch (gState.chapterOutro) {
         case 0:
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_2BU_WS_STR;
            break;
         case 1:
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_3BU_WS_STR;
            break;
         case 2:
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_4BU_WS_STR;
            break;
         case 3:
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_5BU_WS_STR;
            break;
         case 4:
            gState.primary = STATE_MOVIE;
            gState.movieIdxToPlay = MOV_6BU_WS_STR;
            break;
         }

         break;
      }
      break;
   }
}
