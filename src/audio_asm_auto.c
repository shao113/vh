#include "include_asm.h"

INCLUDE_ASM("asm/nonmatchings/audio_asm_auto", HandleVolumeFadeIn);

INCLUDE_ASM("asm/nonmatchings/audio_asm_auto", HandleVolumeFadeOut);

INCLUDE_ASM("asm/nonmatchings/audio_asm_auto", HandleCdaCompletion);

INCLUDE_ASM("asm/nonmatchings/audio_asm_auto", AdjustCdaVolume);

INCLUDE_ASM("asm/nonmatchings/audio_asm_auto", UpdateAudio);
