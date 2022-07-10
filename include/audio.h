#ifndef AUDIO_H
#define AUDIO_H

#include "common.h"

/*
 * <PSYQ>
 */

#include "PsyQ/libcd.h"
#include "PsyQ/libsnd.h"
#include "PsyQ/libspu.h"

/*
 * </PSYQ>
 */

#define STARTING_SECTOR 150
#define SECTORS_PER_SECOND 75
#define SECONDS_PER_MINUTE 60

#define CDA_LOOP 0
#define CDA_NO_LOOP 2

typedef enum { AUDIO_MODE_MONO = 0, AUDIO_MODE_STEREO = 1 } AudioMode;

typedef enum {
   AUDIO_JOB_NULL = 0x0,
   AUDIO_JOB_PLAY_SEQ = 0x1,
   AUDIO_JOB_PLAY_CDA = 0x2,
   AUDIO_JOB_PREPARE_CDA = 0x3,
   AUDIO_JOB_STOP_SEQ = 0x4,
   AUDIO_JOB_PAUSE_CDA = 0x5,
   AUDIO_JOB_UNUSED_0x7 = 0x7,
   AUDIO_JOB_UNUSED_0x8 = 0x8,
   AUDIO_JOB_PLAY_PREPARED_CDA = 0xa,
   AUDIO_JOB_RESET_VOLUME = 0xb
} AudioJob;

typedef enum CdaFadeInState {
   CDA_FADE_IN_STATE_NONE = 0,
   CDA_FADE_IN_STATE_PENDING = 1,
   CDA_FADE_IN_STATE_FADING = 2
} CdaFadeInState;

typedef enum CdaVolumeState {
   CDA_VOLUME_STATE_NONE = 0,
   CDA_VOLUME_STATE_DECREASING = 1,
   CDA_VOLUME_STATE_DECREASED = 2,
   CDA_VOLUME_STATE_INCREASING = 3
} CdaVolumeState;

typedef enum VolumeFadeInMode {
   VOLUME_FADE_IN_MODE_CDA = 1,
   VOLUME_FADE_IN_MODE_NONE = 3,
   VOLUME_FADE_IN_MODE_MAIN = 4,
   VOLUME_FADE_IN_MODE_SEQ = 8
} VolumeFadeInMode;

typedef enum AudioJobPauseCdaState {
   AUDIO_JOB_PAUSE_CDA_STATE_FADE = 0,
   AUDIO_JOB_PAUSE_CDA_STATE_MUTE = 1,
   AUDIO_JOB_PAUSE_CDA_STATE_PAUSE = 2,
   AUDIO_JOB_PAUSE_CDA_STATE_DONE = 3
} AudioJobPauseCdaState;

typedef enum SfxRole { SFX_ROLE_NONE = 0, SFX_ROLE_SUSTAIN = 1, SFX_ROLE_RELEASE = 2 } SfxRole;

typedef struct SfxProperties {
   u8 vabid;
   u8 prog;
   u8 note;
   s8 volume;
   u8 role;
   u8 tone;
} SfxProperties;

typedef struct CdaProperties {
   s32 startingSector;
   s32 duration;
   u8 fileId;
   u8 channelId;
   u8 volume;
   u8 padding;
   s32 loop;
} CdaProperties;

typedef struct SeqProperties {
   u8 volume;
   u8 reverb;
   u8 loop;
} SeqProperties;

extern void *gSeqDataPtr;
extern s16 gSeqSetIdx;
extern s16 gSeqAccessNum;
extern s32 gSectorOffset;
extern u8 gSeqCurrentID;
extern s32 D_80125AAC;
extern s16 gCurrentVolume;
extern s16 gMaxVolume;
extern u8 gAudioJobQueue[64];

extern u8 D_8012C0D0;
extern u8 gCdaCurrentID;
extern u8 gSeqCurrentID;
extern u8 gAudioEnabled;

extern u8 gAudioMode;
extern u16 gSfxMasterVolume;
extern u16 gSfxPrevVolume;
extern s16 gSfxSlotSustained;
extern s16 gSfxSlotToggled;

extern s8 D_80124FB4;
extern u8 gAudioJobPrepareCdaState;
extern u8 gAudioJobPlayCdaState;
extern s8 D_801733B8;
extern s32 D_801801B0;
extern u8 gAudioJobPlayPreparedCdaState;
extern u8 gAudioJobPauseCdaState;
extern CdlLOC gCdaCdlLOC;
extern CdlFILTER gCdaControlParam;
extern s16 gCdaCurrentVolume;
extern s16 gCdaFadeInState;
extern u8 gCdaIdToPrepare;
extern u8 gCdaIdToPlay;
extern s8 gCdaLoopEnabled;
extern s16 gCdaMaxVolume;
extern s8 gCdaPauseInProgress;
extern s16 gCdaVolumeState;
extern s16 gSeqCurrentVolume;
extern s16 gSeqMaxVolume;
extern s16 gVolumeCurve[128];
extern s8 gVolumeFadeInMode;
extern s8 gVolumeFadeOutMode;
extern s16 gVolumeFadePauseCounter;
extern s32 gCdaDuration;
extern s32 gCdaStartTime;
extern s8 gClearedSpuIRQ;
extern u8 gSeqIDToPlay;
extern u8 gSeqReverb;
extern s16 gCdaReducedVolumeTarget;

extern volatile u16 gCdaAdjustedVolume;
extern volatile s16 gVolumeFadeInSpeed;
extern volatile s16 gVolumeFadeOutSpeed;
extern s16 gVolumeFadeCounter;

extern CdaProperties gCdaProperties[188];
extern SfxProperties gSfxProperties[269];
extern SeqProperties gSeqProperties[38];

extern s16 gSeqTaggedIDs[38][8];
extern s32 gSeqDataOffsets[39][8];

void SetSeqDataPtr(void *);
void SetCurrentSeqSet(s16);
void SetSeqAccessNum(s16);
void SetSectorOffset(s32);
u32 GetCurrentSeqId(void);
void SetReverbDepth(s16);
u8 ExecuteCdControl(u8, u8 *, u8 *);
void ResetVolume(void);
void ResetAudioState(void);
void SetSerialVolumeFromCurveIdx(s16, s16, s16);
void InitAudio(void);
u8 AudioIsPlaying(void);
void WaitForAudio(void);
void CloseAudio(void);
void SetAudioMode(u8);
void PlayVab(s16, s16, u16);
void PlaySoundEffect(s16);
void QueuePrepareCda(s8);
void AudioJob_PrepareCda(void);
void QueuePlayCda(u8);
void AudioJob_PlayPreparedCda(void);
void AudioJob_PlayCda(void);
void QueuePauseCda(void);
void AudioJob_PauseCda(void);
void SetCdaReverbEnabled(s8);
void AudioJobQueue_Insert(u8);
void AudioJobQueue_Pop(void);
void AudioJob_Unused_0x7(void);
void DisableSpuIRQ(void);
void AudioJob_Unused_0x8(void);
void QueuePlaySeq(s8);
void AudioJob_PlaySeq(void);
void QueueStopSeq(void);
void SetupVolumeFadeOut(s16, s16);
void SetupVolumeFadeIn(s16, u8);
void StopSound(void);
void PerformParamlessAudioCommand(s32);
void PerformAudioCommand(s16);
void AudioJobQueue_ProcessNext(void);

#endif
