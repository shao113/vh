#include "common.h"
#include "audio.h"

static inline u32 EncodeDigits(u32 val) { return (val / 10) * 16 + (val % 10); }

void SetSeqDataPtr(void *arg0) { gSeqDataPtr = arg0; }

void SetCurrentSeqSet(s16 arg0) { gSeqSetIdx = arg0; }

void SetSeqAccessNum(s16 arg0) { gSeqAccessNum = arg0; }

void SetSectorOffset(s32 arg0) { gSectorOffset = arg0; }

u32 GetCurrentSeqId(void) {
   u16 res = 0;
   if (gSeqCurrentID != 0) {
      res = gSeqCurrentID | 0x200;
   } else {
      res = 0;
   }
   return res;
}

void SetReverbDepth(s16 depth) { SsUtSetReverbDepth(depth, depth); }

u8 ExecuteCdControl(u8 arg0, u8 *arg1, u8 *arg2) {
   extern s32 s_cdSyncStatus;
   u8 buf[8];

   s_cdSyncStatus = CdSync(1, buf);

   if (((s_cdSyncStatus & 0x10) != 0) || ((s_cdSyncStatus & 4) != 0)) {
      CdControl(1, 0, 0);
      return 2;
   }
   if (s_cdSyncStatus == CdlComplete) {
      if (CdControl(arg0, arg1, arg2) != 0) {
         D_80125AAC = 0;
         return 0;
      }

      return 1;
   }
   return 1;
}

void ResetVolume(void) {
   gCurrentVolume = 0x7F;
   gMaxVolume = 0x7F;
   SsSetMVol(0x7F, 0x7F);
}

void ResetAudioState(void) {
   extern s32 s_audioJobIdx;
   for (s_audioJobIdx = 0; s_audioJobIdx < 63; s_audioJobIdx++) {
      gAudioJobQueue[s_audioJobIdx] = AUDIO_JOB_NULL;
   }
   gSfxSlotSustained = 0;
   gCdaAdjustedVolume = gCdaMaxVolume;
   gSfxSlotToggled = 0;
   D_801801B0 = 0;
   gSeqAccessNum = 1;
   gSeqDataPtr = NULL;
   gSeqSetIdx = 0;
   gCdaCurrentID = 0;
   gCdaIdToPrepare = 0;
   gCdaLoopEnabled = 0;
   gCdaPauseInProgress = 0;
   D_801733B8 = 0;
   D_8012C0D0 = 0;
   gAudioJobPlayCdaState = 0;
   gAudioJobPlayPreparedCdaState = 0;
   gAudioJobPrepareCdaState = 0;
   gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_FADE;
   D_80125AAC = 0;
   gCdaVolumeState = CDA_VOLUME_STATE_NONE;
   gVolumeFadeOutMode = 3;
   gVolumeFadeInMode = VOLUME_FADE_IN_MODE_NONE;
   gVolumeFadePauseCounter = 0;
   gCdaFadeInState = CDA_FADE_IN_STATE_NONE;
   gSfxMasterVolume = 0x7F;
   gSeqCurrentVolume = 0x70;
   gSeqMaxVolume = 0x70;
   gVolumeFadeOutSpeed = 0;
   gVolumeFadeInSpeed = 0;
}

void SetSerialVolumeFromCurveIdx(s16 arg0, s16 arg1, s16 arg2) {
   SsSetSerialVol(arg0, gVolumeCurve[arg1], gVolumeCurve[arg2]);
}

void InitAudio(void) {
   gAudioEnabled = 1;
   SsInit();
   SsSetTickMode(SS_TICK60);
   SetAudioMode(AUDIO_MODE_STEREO);
   SsSetReservedVoice(0x14);
   SsStart();
   SsSetTableSize(&D_80124FB4, 0x10, 1);
   SsUtSetReverbType(SPU_REV_MODE_STUDIO_C);
   SpuClearReverbWorkArea(SPU_REV_MODE_STUDIO_C);
   SsUtReverbOn();
   ResetVolume();
   gCdaCurrentVolume = 0x6E;
   SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
   SetSerialVolumeFromCurveIdx(0, gCdaCurrentVolume, gCdaCurrentVolume);
   gCdaControlParam.file = (CdlModeSF | CdlModeRT | CdlModeSpeed);
   ExecuteCdControl(CdlSetmode, (void *)&gCdaControlParam, NULL);
   ResetAudioState();
   SetReverbDepth(0x3C);
   SpuSetTransferMode(0);
   SpuSetIRQCallback(0);
   CdReadyCallback(0);
   SpuMallocWithStartAddr(0x1010, 0x10000);
}

u8 AudioIsPlaying(void) {
   if (gCdaCurrentID != 0) {
      return 1;
   }
   if (gSeqCurrentID != 0) {
      return 3;
   }
   if (D_8012C0D0 != 0) {
      return 2;
   }
   return 0;
}

void WaitForAudio(void) {
   while (AudioIsPlaying()) {
      VSync(0);
      UpdateAudio();
   }
}

void CloseAudio(void) {
   SsVabClose(0);
   SsVabClose(1);
   SsEnd();
   SsQuit();
   gAudioEnabled = 0;
}

void SetAudioMode(u8 param_1) {
   CdlATV atv;

   switch (param_1) {
   case AUDIO_MODE_MONO:
      if (gAudioMode != AUDIO_MODE_MONO) {
         SsSetMono();
         atv.val2 = 79;
         atv.val0 = 79;
         atv.val3 = 79;
         atv.val1 = 79;
         CdMix(&atv);
         gSfxMasterVolume = 108;
         gAudioMode = 0;
      }
      break;
   case AUDIO_MODE_STEREO:
      if (gAudioMode != AUDIO_MODE_STEREO) {
         SsSetStereo();
         atv.val2 = 127;
         atv.val0 = 127;
         atv.val3 = 0;
         atv.val1 = 0;
         CdMix(&atv);
         gSfxMasterVolume = 127;
         gAudioMode = 1;
      }
      break;
   }
}

void PlayVab(s16 vab_pro, s16 pitch, u16 vol) {
   if (pitch != 0) {
      SsVoKeyOff(vab_pro, pitch);
      SsVoKeyOn(vab_pro, pitch, vol, vol);
   }
}

void PlaySoundEffect(s16 param_1) {
   extern s16 s_sfxCurrentId;
   s_sfxCurrentId = param_1 - 0x500;
   AudioJobQueue_Insert(AUDIO_JOB_RESET_VOLUME);

   gSfxPrevVolume = (gSfxMasterVolume * gSfxProperties[s_sfxCurrentId].volume) >> 7;

   if (gSfxProperties[s_sfxCurrentId].role == SFX_ROLE_SUSTAIN) {
      // Once sustained, toggle state is locked, and subsequent SFX will be played in the
      // toggled/alternate slot until it is released;
      gSfxSlotSustained = 1;
      SsUtKeyOnV(gSfxSlotToggled * 2 + 20, gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);

      SsUtKeyOnV(gSfxSlotToggled * 2 + 21, gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone + 1,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);
      return;
   } else if (gSfxProperties[s_sfxCurrentId].role == SFX_ROLE_RELEASE) {
      gSfxSlotSustained = 0;
      SsUtKeyOffV(gSfxSlotToggled * 2 + 20);
      SsUtKeyOffV(gSfxSlotToggled * 2 + 21);
      return;
   }

   // SFX_ROLE_NONE:
   if (gSfxSlotSustained) {
      SsUtKeyOnV((gSfxSlotToggled ^ 1) * 2 + 20, gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);

      SsUtKeyOnV((gSfxSlotToggled ^ 1) * 2 + 21, gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone + 1,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);
   } else {
      SsUtKeyOnV((gSfxSlotToggled * 2 + 20), gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);

      SsUtKeyOnV((gSfxSlotToggled * 2 + 21), gSfxProperties[s_sfxCurrentId].vabid,
                 gSfxProperties[s_sfxCurrentId].prog, gSfxProperties[s_sfxCurrentId].tone + 1,
                 gSfxProperties[s_sfxCurrentId].note, 0, gSfxPrevVolume, gSfxPrevVolume);
      gSfxSlotToggled ^= 1;
   }
}

void QueuePrepareCda(s8 arg0) {
   gCdaIdToPrepare = arg0;
   AudioJobQueue_Insert(AUDIO_JOB_PREPARE_CDA);
}

void AudioJob_PrepareCda(void) {
   extern u8 s_cdSyncResultBuffer_80124f0c[8];
   extern u8 s_cdaIdToPrepare;
   extern u32 s_minutes_80124f1c;
   u8 res;
   u32 seconds;
   u32 sector;
   u32 minutes;
   u32 resultToCheck;

   switch (gAudioJobPrepareCdaState) {
   case 0:
      gCdaCurrentVolume -= 0x20;
      if (gCdaCurrentVolume < 0) {
         gCdaCurrentVolume = 0;
      }
      SetSerialVolumeFromCurveIdx(SS_SERIAL_A, gCdaCurrentVolume, gCdaCurrentVolume);
      if (gCdaCurrentVolume == 0) {
         gAudioJobPrepareCdaState = 1;
      }
      break;
   case 1:
      gCdaCurrentID = 0;
      gCdaControlParam.file = (CdlModeSF | CdlModeRT | CdlModeSpeed);
      s_cdaIdToPrepare = gCdaIdToPrepare;

      gAudioJobPrepareCdaState = 2;
      /* fallthrough */
   case 2:
      res = ExecuteCdControl(CdlSetmode, (void *)&gCdaControlParam, NULL);

      if (res == 0) {
         gAudioJobPrepareCdaState = 3;
      }
      resultToCheck = res;
   CheckResult:
      if (resultToCheck == 2) {
         gAudioJobPrepareCdaState = 8;
      }
      break;
   case 3:
      gCdaControlParam.file = gCdaProperties[s_cdaIdToPrepare].fileId;
      gCdaControlParam.chan = gCdaProperties[s_cdaIdToPrepare].channelId & 0xf;

      gAudioJobPrepareCdaState = 4;
      /* fallthrough */
   case 4:
      res = ExecuteCdControl(CdlSetfilter, (void *)&gCdaControlParam, NULL);

      if (res == 0) {
         gAudioJobPrepareCdaState = 5;
      }
      resultToCheck = res;
      goto CheckResult;
   case 5:
      s_minutes_80124f1c = gCdaProperties[s_cdaIdToPrepare].channelId +
                           gCdaProperties[s_cdaIdToPrepare].startingSector + STARTING_SECTOR;
      s_minutes_80124f1c += gSectorOffset;

      seconds = s_minutes_80124f1c / SECTORS_PER_SECOND;
      sector = s_minutes_80124f1c % SECTORS_PER_SECOND;
      gCdaCdlLOC.sector = EncodeDigits(sector);

      s_minutes_80124f1c = seconds;
      minutes = seconds / SECONDS_PER_MINUTE;
      seconds = seconds % SECONDS_PER_MINUTE;
      gCdaCdlLOC.second = EncodeDigits(seconds);

      s_minutes_80124f1c = minutes;
      gCdaCdlLOC.minute = EncodeDigits(minutes);

      gAudioJobPrepareCdaState = 6;
      /* fallthrough */
   case 6:
      res = ExecuteCdControl(CdlSeekL, (void *)&gCdaCdlLOC, NULL);

      if (res == 0) {
         gAudioJobPrepareCdaState = 7;
      }
      resultToCheck = res;
      goto CheckResult;
   case 7: {
      /* FIXME */
      register int v1 asm("v1");
      register int v0 asm("v0");

      CdSync(1, s_cdSyncResultBuffer_80124f0c);
      v1 = s_cdSyncResultBuffer_80124f0c[0];
      v0 = v1 & CdlStatSeek;
      if (v0 == 0) {
         gAudioJobPrepareCdaState = 8;
      }
      break;
   }
   case 8:
      gAudioJobPrepareCdaState = 0;
      AudioJobQueue_Pop();
   }
}

void QueuePlayCda(u8 id) {
   gCdaIdToPlay = id;

   gCdaVolumeState = CDA_VOLUME_STATE_NONE;
   gCdaAdjustedVolume = gCdaMaxVolume;
   gCdaMaxVolume = gCdaProperties[gCdaIdToPlay].volume;

   if (gCdaIdToPrepare == gCdaIdToPlay) {
      AudioJobQueue_Insert(AUDIO_JOB_RESET_VOLUME);
      AudioJobQueue_Insert(AUDIO_JOB_PLAY_PREPARED_CDA);
   } else {
      if (gCdaCurrentID != 0) {
         AudioJobQueue_Insert(AUDIO_JOB_PAUSE_CDA);
      }
      AudioJobQueue_Insert(AUDIO_JOB_RESET_VOLUME);
      AudioJobQueue_Insert(AUDIO_JOB_PLAY_CDA);
   }
   gCdaIdToPrepare = 0;
}

void AudioJob_PlayPreparedCda(void) {
   extern u8 s_cdaId_80124f24;
   extern s32 s_delay_80124f2c;
   u8 res;

   switch (gAudioJobPlayPreparedCdaState) {
   case 0:
      s_delay_80124f2c = 0xA;
      s_cdaId_80124f24 = gCdaIdToPlay;
      if (gCdaFadeInState == CDA_FADE_IN_STATE_PENDING) {
         gCdaCurrentVolume = 0;
         SetSerialVolumeFromCurveIdx(SS_SERIAL_A, 0, 0);
         gCdaFadeInState = CDA_FADE_IN_STATE_FADING;
      } else {
         gCdaCurrentVolume = gCdaMaxVolume;
         SetSerialVolumeFromCurveIdx(SS_SERIAL_A, gCdaMaxVolume, gCdaMaxVolume);
      }

      gCdaDuration = gCdaProperties[s_cdaId_80124f24].duration + 0x10;
      if (gCdaProperties[s_cdaId_80124f24].loop == CDA_NO_LOOP) {
         gCdaLoopEnabled = 0;
      } else {
         gCdaLoopEnabled = 1;
      }
      gAudioJobPlayPreparedCdaState = 1;
      /* fallthrough */
   case 1:
      res = ExecuteCdControl(CdlReadN, NULL, NULL);
      if (res == 0) {
         gAudioJobPlayPreparedCdaState = 2;
      }
      if (res == 2) {
         gAudioJobPlayPreparedCdaState = 3;
      }
      break;
   case 2:
      gCdaCurrentID = s_cdaId_80124f24;
      SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
      gCdaStartTime = VSync(-1);
      gAudioJobPlayPreparedCdaState = 3;
      break;
   case 3:
      if (s_delay_80124f2c != 0) {
         s_delay_80124f2c--;
         return;
      }
      gAudioJobPlayPreparedCdaState = 0;
      AudioJobQueue_Pop();
      break;
   }
}

void AudioJob_PlayCda(void) {
   extern u8 s_cdSyncResultBuffer_80124f34[8];
   extern u32 s_minutes_80124f44;
   extern u8 s_cdaIdToPlay_80124f3c;
   extern s32 s_delay_80124f4c;
   u32 seconds;
   u32 sector;
   u32 minutes;
   u8 res;

   switch (gAudioJobPlayCdaState) {
   case 0:
      s_delay_80124f4c = 10;
      gCdaControlParam.file = (CdlModeSF | CdlModeRT | CdlModeSpeed);
      gAudioJobPlayCdaState = 1;
      s_cdaIdToPlay_80124f3c = gCdaIdToPlay;
      /* fallthrough */
   case 1:
      res = ExecuteCdControl(CdlSetmode, (void *)&gCdaControlParam, NULL);
      if (res == 0) {
         gAudioJobPlayCdaState = 2;
      }
      if (res == 2) {
         gAudioJobPlayCdaState = 9;
      }
      break;
   case 2:
      gCdaControlParam.file = gCdaProperties[s_cdaIdToPlay_80124f3c].fileId;
      gCdaControlParam.chan = gCdaProperties[s_cdaIdToPlay_80124f3c].channelId & 0xf;
      gAudioJobPlayCdaState = 3;
      /* fallthrough */
   case 3:
      res = ExecuteCdControl(CdlSetfilter, (void *)&gCdaControlParam, NULL);
      if (res == 0) {
         gAudioJobPlayCdaState = 4;
      }
      if (res == 2) {
         gAudioJobPlayCdaState = 9;
      }
      break;
   case 4:
      s_minutes_80124f44 = gCdaProperties[s_cdaIdToPlay_80124f3c].channelId +
                           gCdaProperties[s_cdaIdToPlay_80124f3c].startingSector + STARTING_SECTOR;

      gCdaDuration = gCdaProperties[s_cdaIdToPlay_80124f3c].duration + 0x10;

      if (gCdaProperties[s_cdaIdToPlay_80124f3c].loop == CDA_NO_LOOP) {
         gCdaLoopEnabled = 0;
      } else {
         gCdaLoopEnabled = 1;
      }
      s_minutes_80124f44 += gSectorOffset;

      seconds = s_minutes_80124f44 / SECTORS_PER_SECOND;
      sector = s_minutes_80124f44 % SECTORS_PER_SECOND;
      gCdaCdlLOC.sector = EncodeDigits(sector);

      s_minutes_80124f44 = seconds;
      minutes = seconds / SECONDS_PER_MINUTE;
      seconds = seconds % SECONDS_PER_MINUTE;
      gCdaCdlLOC.second = EncodeDigits(seconds);
      s_minutes_80124f44 = minutes;
      gCdaCdlLOC.minute = EncodeDigits(minutes);

      gAudioJobPlayCdaState = 5;
      /* fallthrough */
   case 5:
      res = ExecuteCdControl(CdlSeekL, (void *)&gCdaCdlLOC, NULL);
      if (res == 0) {
         gAudioJobPlayCdaState = 6;
      }
      if (res == 2) {
         gAudioJobPlayCdaState = 9;
      }
      break;
   case 6:
      CdSync(1, s_cdSyncResultBuffer_80124f34);

      if ((s_cdSyncResultBuffer_80124f34[0] & CdlStatSeek) != 0) {
         return;
      }

      if ((s_cdSyncResultBuffer_80124f34[0] & CdlStatShellOpen) == 0) {
         gAudioJobPlayCdaState = 7;
         return;
      }
      gAudioJobPlayCdaState = 9;
      break;
   case 7:
      res = ExecuteCdControl(CdlReadN, NULL, NULL);
      if (res == 0) {
         gAudioJobPlayCdaState = 8;
      }
      if (res == 2) {
         gAudioJobPlayCdaState = 9;
      }
      break;
   case 8:
      if (gCdaFadeInState == CDA_FADE_IN_STATE_PENDING) {
         gCdaCurrentVolume = 0;
         SetSerialVolumeFromCurveIdx(SS_SERIAL_A, 0, 0);
         gCdaFadeInState = CDA_FADE_IN_STATE_FADING;
      } else {
         gCdaCurrentVolume = gCdaMaxVolume;
         SetSerialVolumeFromCurveIdx(0, gCdaCurrentVolume, gCdaCurrentVolume);
      }
      gCdaCurrentID = s_cdaIdToPlay_80124f3c;
      SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SON);
      gCdaStartTime = VSync(-1);
      gAudioJobPlayCdaState = 9;
      break;
   case 9:
      if (s_delay_80124f4c != 0) {
         s_delay_80124f4c--;
         return;
      }
      gAudioJobPlayCdaState = 0;
      AudioJobQueue_Pop();
   default:
      break;
   }
}

void QueuePauseCda(void) {
   gCdaVolumeState = CDA_VOLUME_STATE_NONE;
   gCdaAdjustedVolume = gCdaMaxVolume;
   AudioJobQueue_Insert(AUDIO_JOB_PAUSE_CDA);
}

void AudioJob_PauseCda(void) {
   s32 res;

   if (gCdaCurrentID != 0) {
      gCdaPauseInProgress = 1;
      switch (gAudioJobPauseCdaState) {
      case AUDIO_JOB_PAUSE_CDA_STATE_FADE:
         gCdaCurrentVolume -= 0x20;
         if (gCdaCurrentVolume < 0) {
            gCdaCurrentVolume = 0;
         }
         SetSerialVolumeFromCurveIdx(SS_SERIAL_A, gCdaCurrentVolume, gCdaCurrentVolume);
         if (gCdaCurrentVolume == 0) {
            gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_MUTE;
         }
         break;
      case AUDIO_JOB_PAUSE_CDA_STATE_MUTE:
         SsSetSerialAttr(SS_SERIAL_A, SS_MIX, SS_SOFF);
         gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_PAUSE;
         /* fallthrough */
      case AUDIO_JOB_PAUSE_CDA_STATE_PAUSE:
         res = ExecuteCdControl(CdlPause, NULL, NULL);
         if (res == 0) {
            gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_DONE;
         }
         if (res == 2) {
            gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_DONE;
         }
         break;
      case AUDIO_JOB_PAUSE_CDA_STATE_DONE:
         gCdaPauseInProgress = 0;
         gCdaCurrentID = 0;
         gCdaFadeInState = CDA_FADE_IN_STATE_NONE;
         gAudioJobPauseCdaState = AUDIO_JOB_PAUSE_CDA_STATE_FADE;
         AudioJobQueue_Pop();
      }
   } else {
      AudioJobQueue_Pop();
   }
}

void SetCdaReverbEnabled(s8 enabled) { SsSetSerialAttr(SS_SERIAL_A, SS_REV, enabled == 1); }

void AudioJobQueue_Insert(u8 job) {
   extern s32 s_iJob_80124f54, s_iJob2_80124f5c;

   /* Remove any existing jobs of this type by shifting elements to the left: */
   for (s_iJob_80124f54 = 1; s_iJob_80124f54 < 62; s_iJob_80124f54++) {
      if (gAudioJobQueue[s_iJob_80124f54] == job) {
         for (s_iJob2_80124f5c = s_iJob_80124f54; s_iJob2_80124f5c < 62; s_iJob2_80124f5c++) {
            gAudioJobQueue[s_iJob2_80124f5c] = gAudioJobQueue[s_iJob2_80124f5c + 1];
         }
         gAudioJobQueue[63] = AUDIO_JOB_NULL;
      }
   }
   /* Insert job into first available slot: */
   for (s_iJob_80124f54 = 0; s_iJob_80124f54 < 63; s_iJob_80124f54++) {
      if (gAudioJobQueue[s_iJob_80124f54] == AUDIO_JOB_NULL) {
         gAudioJobQueue[s_iJob_80124f54] = job;
         return;
      }
   }
}

void AudioJobQueue_Pop(void) {
   extern s32 s_iJob_80124f64;

   if (gAudioJobQueue[0] != AUDIO_JOB_NULL) {
      for (s_iJob_80124f64 = 0; s_iJob_80124f64 < 62; s_iJob_80124f64++) {
         gAudioJobQueue[s_iJob_80124f64] = gAudioJobQueue[s_iJob_80124f64 + 1];
      }
      gAudioJobQueue[63] = AUDIO_JOB_NULL;
   }
}

void AudioJob_Unused_0x7(void) { return; }

void DisableSpuIRQ(void) {
   SpuSetIRQ(SPU_OFF);
   SpuSetIRQCallback((SpuIRQCallbackProc)NULL);
   gClearedSpuIRQ = 1;
}

void AudioJob_Unused_0x8(void) { return; }

void QueuePlaySeq(s8 id) {
   gSeqIDToPlay = id;
   if (gSeqCurrentID != 0) {
      AudioJobQueue_Insert(AUDIO_JOB_STOP_SEQ);
   }
   AudioJobQueue_Insert(AUDIO_JOB_RESET_VOLUME);
   AudioJobQueue_Insert(AUDIO_JOB_PLAY_SEQ);
}

void AudioJob_PlaySeq(void) {
   extern s16 s_seqId;
   s32 *pSeqDataOffset;
   s16 *p;
   s32 idx, j;

   s_seqId = gSeqIDToPlay;
   p = &gSeqTaggedIDs[gSeqSetIdx][0];

   /* Search for seq id in current set: */
   for (idx = 0; idx < 8; idx++) {
      /* before comparing, re-or the upper 0x02 byte onto gSeqIDToPlay that
       * was dropped in PerformAudioCommand / QueuePlaySeq */
      if ((s_seqId | 0x200) == *p) {
         break;
      }
      p++;
   }

   /* Increment a matching amount to locate the associated data offset: */
   pSeqDataOffset = &gSeqDataOffsets[gSeqSetIdx][0];
   for (j = 0; j < idx; j++)
      pSeqDataOffset++;

   gSeqAccessNum = SsSeqOpen(gSeqDataPtr + *pSeqDataOffset, 1);
   gSeqReverb = gSeqProperties[s_seqId].reverb;
   SetReverbDepth(gSeqReverb);
   gSeqCurrentVolume = gSeqProperties[s_seqId].volume;
   gSeqMaxVolume = gSeqProperties[s_seqId].volume;
   SsSeqSetVol(gSeqAccessNum, gSeqCurrentVolume, gSeqCurrentVolume);
   if ((gSeqProperties[s_seqId].loop & 0xf) == 0) {
      SsSeqPlay(gSeqAccessNum, SSPLAY_PLAY, 1);
   } else {
      SsSeqPlay(gSeqAccessNum, SSPLAY_PLAY, SSPLAY_INFINITY);
   }

   gSeqCurrentID = gSeqIDToPlay;
   AudioJobQueue_Pop();
}

void QueueStopSeq(void) { AudioJobQueue_Insert(AUDIO_JOB_STOP_SEQ); }

void AudioJob_StopSeq(void) {
   if (gSeqCurrentID != 0) {
      SsSeqStop(gSeqAccessNum);
      SsSeqClose(gSeqAccessNum);
      gSeqCurrentID = 0;
   }
   AudioJobQueue_Pop();
}

void SetupVolumeFadeOut(s16 speed, s16 mode) {
   extern s8 s_unused_80125a94;
   gVolumeFadeCounter = 0;
   gVolumeFadeOutSpeed = speed;
   gVolumeFadeOutMode = mode;
   s_unused_80125a94 = 0;
}

void SetupVolumeFadeIn(s16 speed, u8 mode) {
   switch (mode) {
   case VOLUME_FADE_IN_MODE_CDA:
      if (gCdaCurrentID == 0) {
         gVolumeFadeCounter = 0;
         gVolumeFadeInSpeed = speed;
         gVolumeFadeInMode = mode;
         gCdaFadeInState = CDA_FADE_IN_STATE_PENDING;
         gCdaCurrentVolume = 0;
         gCdaMaxVolume = 0;
         return;
      }
      return;
   case VOLUME_FADE_IN_MODE_SEQ:
      if (gSeqCurrentID != 0) {
         gVolumeFadeCounter = 0;
         gVolumeFadeInSpeed = speed;
         gVolumeFadeInMode = mode;
         return;
      }
      break;
   case VOLUME_FADE_IN_MODE_MAIN:
      if ((gCdaCurrentID == 0) && (gSeqCurrentID == 0)) {
         gVolumeFadeCounter = 0;
         gVolumeFadeInSpeed = speed;
         gVolumeFadeInMode = mode;
         gCurrentVolume = 0;
         SsSetMVol(0, 0);
         gMaxVolume = 0x7F;
      }
      break;
   }
}

void StopSound(void) {
   SpuSetKey(SPU_OFF, SPU_ALLCH);
   gSfxSlotSustained = 0;
}

void PerformParamlessAudioCommand(s32 cmd) {
   s16 cmdMinusOne = cmd - 1;

   switch (cmdMinusOne) {
   case 0x0:
      SetAudioMode(AUDIO_MODE_MONO);
      break;
   case 0x1:
      SetAudioMode(AUDIO_MODE_STEREO);
      break;
   case 0x2:
      QueueStopSeq();
      break;
   case 0x3:
      StopSound();
      break;
   case 0x4:
      QueuePauseCda();
      break;
   case 0x5:
   case 0x6:
      QueuePauseCda();
      QueueStopSeq();
      WaitForAudio();
      StopSound();
      gVolumeFadeOutSpeed = 0;
      break;
   case 0x7:
      WaitForAudio();
      break;
   case 0x4F:
      StopSound();
      SsSeqSetVol(gSeqAccessNum, 64, 64);
      break;
   case 0x50:
      SsSeqSetVol(gSeqAccessNum, gSeqCurrentVolume, gSeqCurrentVolume);
      break;
   case 0x51:
      SetupVolumeFadeOut(8, 8);
      break;
   case 0x52:
      SetupVolumeFadeIn(8, 8);
      break;
   case 0x53:
      gCdaVolumeState = CDA_VOLUME_STATE_DECREASING;
      gCdaAdjustedVolume = gCdaMaxVolume;
      gCdaReducedVolumeTarget = gCdaMaxVolume / 3;
      break;
   case 0x54:
      gCdaVolumeState = CDA_VOLUME_STATE_INCREASING;
      break;
   case 0x1F:
      SetupVolumeFadeOut(32, 4);
      break;
   case 0x20:
      SetupVolumeFadeOut(8, 4);
      break;
   case 0x21:
      SetupVolumeFadeOut(4, 4);
      break;
   case 0x22:
      SetupVolumeFadeOut(2, 4);
      break;
   case 0x23:
      SetupVolumeFadeOut(1, 4);
      break;
   case 0x27:
      SetupVolumeFadeOut(128, 3);
      break;
   case 0x28:
      SetupVolumeFadeOut(8, 3);
      break;
   case 0x29:
      SetupVolumeFadeOut(4, 3);
      break;
   case 0x2A:
      SetupVolumeFadeOut(2, 3);
      break;
   case 0x2B:
      SetupVolumeFadeOut(1, 3);
      break;
   case 0x2F:
      SetupVolumeFadeOut(128, 1);
      break;
   case 0x30:
      SetupVolumeFadeOut(8, 1);
      break;
   case 0x31:
      SetupVolumeFadeOut(4, 1);
      break;
   case 0x32:
      SetupVolumeFadeOut(2, 1);
      break;
   case 0x33:
      SetupVolumeFadeOut(1, 1);
      break;
   case 0x37:
      SetupVolumeFadeOut(128, 2);
      break;
   case 0x38:
      SetupVolumeFadeOut(8, 2);
      break;
   case 0x39:
      SetupVolumeFadeOut(4, 2);
      break;
   case 0x3A:
      SetupVolumeFadeOut(2, 2);
      break;
   case 0x3B:
      SetupVolumeFadeOut(1, 2);
      break;
   case 0x3F:
      SetupVolumeFadeIn(32, VOLUME_FADE_IN_MODE_CDA);
      break;
   case 0x40:
      SetupVolumeFadeIn(8, VOLUME_FADE_IN_MODE_CDA);
      break;
   case 0x41:
      SetupVolumeFadeIn(4, VOLUME_FADE_IN_MODE_CDA);
      break;
   case 0x42:
      SetupVolumeFadeIn(2, VOLUME_FADE_IN_MODE_CDA);
      break;
   case 0x43:
      SetupVolumeFadeIn(1, VOLUME_FADE_IN_MODE_CDA);
      break;
   case 0x47:
      SetupVolumeFadeIn(4, VOLUME_FADE_IN_MODE_MAIN);
      break;
   case 0xAF:
      SetCdaReverbEnabled(1);
      break;
   case 0xB0:
      SetCdaReverbEnabled(0);
      break;
   case 0xBF:
      SetReverbDepth(8);
      break;
   case 0xC0:
      SetReverbDepth(0x10);
      break;
   case 0xC1:
      SetReverbDepth(0x18);
      break;
   case 0xC2:
      SetReverbDepth(0x20);
      break;
   case 0xC3:
      SetReverbDepth(0x28);
      break;
   case 0xC4:
      SetReverbDepth(0x30);
      break;
   case 0xC5:
      SetReverbDepth(0x38);
      break;
   case 0xC6:
      SetReverbDepth(0x40);
      break;
   case 0xC7:
      SetReverbDepth(0x48);
      break;
   case 0xC8:
      SetReverbDepth(0x50);
      break;
   case 0xC9:
      SetReverbDepth(0x58);
      break;
   case 0xCA:
      SetReverbDepth(0x60);
      break;
   case 0xCB:
      SetReverbDepth(0x68);
      break;
   case 0xCC:
      SetReverbDepth(0x70);
      break;
   case 0xCD:
      SetReverbDepth(0x78);
      break;
   case 0xCE:
      SetReverbDepth(0x7F);
      break;
   default:
      return;
   }
}

void PerformAudioCommand(s16 cmd) {
   if (gAudioEnabled) {
      switch ((u8)(cmd >> 8)) {
      case 0:
         PerformParamlessAudioCommand(cmd);
         return;
      case 2:
         QueuePlaySeq(cmd);
         return;
      case 3:
         QueuePlayCda(cmd);
         return;
      case 5:
      case 6:
      case 7:
         PlaySoundEffect(cmd);
         return;
      case 0x13:
         QueuePrepareCda(cmd);
         break;
      }
   }
}

void AudioJobQueue_ProcessNext(void) {
   switch (gAudioJobQueue[0]) {
   case AUDIO_JOB_NULL:
      break;
   case AUDIO_JOB_PLAY_SEQ:
      AudioJob_PlaySeq();
      break;
   case AUDIO_JOB_PLAY_CDA:
      AudioJob_PlayCda();
      break;
   case AUDIO_JOB_PLAY_PREPARED_CDA:
      AudioJob_PlayPreparedCda();
      break;
   case AUDIO_JOB_PREPARE_CDA:
      AudioJob_PrepareCda();
      break;
   case AUDIO_JOB_STOP_SEQ:
      AudioJob_StopSeq();
      break;
   case AUDIO_JOB_PAUSE_CDA:
      AudioJob_PauseCda();
      break;
   case AUDIO_JOB_UNUSED_0x8:
      AudioJob_Unused_0x8();
      break;
   case AUDIO_JOB_UNUSED_0x7:
      AudioJob_Unused_0x7();
      break;
   case AUDIO_JOB_RESET_VOLUME:
      ResetVolume();
      AudioJobQueue_Pop();
      break;
   default:
      return;
   }
}
