#include "common.h"
#include "evt.h"
#include "cd_files.h"
#include "state.h"
#include "graphics.h"
#include "audio.h"

#include "PsyQ/libcd.h"
#include "PsyQ/libpress.h"

static StHEADER *sMovieSectorHeader = NULL;

extern struct {
   s32 state;
   s32 loadingVabBody;
   s32 cdf;
   s32 stalledCounter;
   s32 sectorsToRead;
   s32 sector;
   s32 sectorsRead;
   CdlLOC location;
} gCdLoader;

extern struct {
   s32 state;
   s32 vabId;
   s32 headerCdf;
   s32 bodyCdf;
   s32 bodyTransferResult;
} gVabLoader;

extern struct {
   s32 state;
   s32 setIdx;
   s32 unused;
   s32 cdf;
} gSeqLoader;

extern struct {
   u32 *vlcBufferPtrs[2];
   s32 vlcBufferIdx;
   u32 *imgBufferPtr;
   RECT bufferRects[2];
   s32 bufferRectIdx;
   RECT slice;
   s16 frameFinished;
   s16 mode;
} gMovieDecoder;

typedef struct SoundSet {
   s32 vabId;
   s32 cdfVabHeader;
   s32 cdfVabBody;
   void *bufferPtr;
} SoundSet;

typedef struct SeqSet {
   s32 cdf;
   void *bufferPtr;
} SeqSet;

// TODO: Replace addresses w/ relocs
SoundSet gSoundSets[13] = {{0, CDF_SD_JOU_VH, CDF_SD_JOU_VB, (void *)0x80140878},
                           {1, CDF_SD_SEQ_VH, CDF_SD_SEQ_VB, (void *)0x80141878},
                           {2, CDF_SD_BAT_VH, CDF_SD_BAT_VB, (void *)0x80144878},
                           {2, CDF_SD_TORI_VH, CDF_SD_TORI_VB, (void *)0x80144878},
                           {2, CDF_SD_TAKI_VH, CDF_SD_TAKI_VB, (void *)0x80144878},
                           {2, CDF_SD_KAZE_VH, CDF_SD_KAZE_VB, (void *)0x80144878},
                           {2, CDF_SD_BIBI_VH, CDF_SD_BIBI_VB, (void *)0x80144878},
                           {2, CDF_SD_ZIHI_VH, CDF_SD_ZIHI_VB, (void *)0x80144878},
                           {2, CDF_SD_RIN_VH, CDF_SD_RIN_VB, (void *)0x80144878},
                           {2, CDF_SD_AMA_VH, CDF_SD_AMA_VB, (void *)0x80144878},
                           {2, CDF_SD_BERA_VH, CDF_SD_BERA_VB, (void *)0x80144878},
                           {2, CDF_SD_DOR_VH, CDF_SD_DOR_VB, (void *)0x80144878},
                           {2, CDF_SD_HI_VH, CDF_SD_HI_VB, (void *)0x80144878}};

SeqSet gSeqSets[38] = {
    {CDF_SD_S00_BIN, gSeqData}, {CDF_SD_S01_BIN, gSeqData}, {CDF_SD_S02_BIN, gSeqData},
    {CDF_SD_S03_BIN, gSeqData}, {CDF_SD_S04_BIN, gSeqData}, {CDF_SD_S05_BIN, gSeqData},
    {CDF_SD_S06_BIN, gSeqData}, {CDF_SD_S07_BIN, gSeqData}, {CDF_SD_S08_BIN, gSeqData},
    {CDF_SD_S09_BIN, gSeqData}, {CDF_SD_S10_BIN, gSeqData}, {CDF_SD_S11_BIN, gSeqData},
    {CDF_SD_S12_BIN, gSeqData}, {CDF_SD_S13_BIN, gSeqData}, {CDF_SD_S14_BIN, gSeqData},
    {CDF_SD_S15_BIN, gSeqData}, {CDF_SD_S16_BIN, gSeqData}, {CDF_SD_S17_BIN, gSeqData},
    {CDF_SD_S18_BIN, gSeqData}, {CDF_SD_S19_BIN, gSeqData}, {CDF_SD_S20_BIN, gSeqData},
    {CDF_SD_S21_BIN, gSeqData}, {CDF_SD_S22_BIN, gSeqData}, {CDF_SD_S23_BIN, gSeqData},
    {CDF_SD_S24_BIN, gSeqData}, {CDF_SD_S25_BIN, gSeqData}, {CDF_SD_S26_BIN, gSeqData},
    {CDF_SD_S27_BIN, gSeqData}, {CDF_SD_S28_BIN, gSeqData}, {CDF_SD_S29_BIN, gSeqData},
    {CDF_SD_S30_BIN, gSeqData}, {CDF_SD_S31_BIN, gSeqData}, {CDF_SD_S32_BIN, gSeqData},
    {CDF_SD_S33_BIN, gSeqData}, {CDF_SD_S34_BIN, gSeqData}, {CDF_SD_S35_BIN, gSeqData},
    {CDF_SD_S36_BIN, gSeqData}, {CDF_SD_S37_BIN, gSeqData}};

CdFileInfo gCdFiles[712] = {{0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x27e8, 0x01, gScratch1_801317c0},
                            {0x27e9, 0x01, gScratch1_801317c0},
                            {0x27ea, 0x02, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x27ed, 0x01, gScratch1_801317c0},
                            {0x27ee, 0x01, gScratch1_801317c0},
                            {0x27ef, 0x01, gScratch1_801317c0},
                            {0x27f0, 0x01, gScratch1_801317c0},
                            {0x27f1, 0x01, gScratch1_801317c0},
                            {0x27f2, 0x02, gScratch1_801317c0},
                            {0x27f4, 0x01, gScratch1_801317c0},
                            {0x27f5, 0x02, gScratch1_801317c0},
                            {0x27f7, 0x01, gScratch1_801317c0},
                            {0x27f8, 0x01, gScratch1_801317c0},
                            {0x27f9, 0x01, gScratch1_801317c0},
                            {0x27fa, 0x01, gScratch1_801317c0},
                            {0x27fb, 0x01, gScratch1_801317c0},
                            {0x27fc, 0x01, gScratch1_801317c0},
                            {0x27fd, 0x01, gScratch1_801317c0},
                            {0x23b3, 0x02, gScratch3_80180210},
                            {0x23af, 0x04, gScratch3_80180210},
                            {0x26bc, 0x06, gScratch3_80180210},
                            {0x263a, 0x82, gScratch3_80180210},
                            {0x2283, 0x05, gScratch3_80180210},
                            {0x221a, 0x69, gScratch3_80180210},
                            {0x276b, 0x02, gScratch3_80180210},
                            {0x2700, 0x6b, gScratch3_80180210},
                            {0x26fe, 0x02, gScratch3_80180210},
                            {0x26c2, 0x3c, gScratch3_80180210},
                            {0x2404, 0x02, gScratch3_80180210},
                            {0x23b5, 0x4f, gScratch3_80180210},
                            {0x22e7, 0x02, gScratch3_80180210},
                            {0x229c, 0x4b, gScratch3_80180210},
                            {0x27e4, 0x02, gScratch3_80180210},
                            {0x277b, 0x69, gScratch3_80180210},
                            {0x2452, 0x02, gScratch3_80180210},
                            {0x2420, 0x32, gScratch3_80180210},
                            {0x2457, 0x05, gScratch3_80180210},
                            {0x245c, 0x06, gScratch3_80180210},
                            {0x2462, 0x10, gScratch3_80180210},
                            {0x2472, 0x0a, gScratch3_80180210},
                            {0x247c, 0x0d, gScratch3_80180210},
                            {0x2489, 0x0f, gScratch3_80180210},
                            {0x2498, 0x0f, gScratch3_80180210},
                            {0x24a7, 0x14, gScratch3_80180210},
                            {0x24bb, 0x17, gScratch3_80180210},
                            {0x24d2, 0x0c, gScratch3_80180210},
                            {0x24de, 0x11, gScratch3_80180210},
                            {0x24ef, 0x13, gScratch3_80180210},
                            {0x2502, 0x11, gScratch3_80180210},
                            {0x2513, 0x0e, gScratch3_80180210},
                            {0x2521, 0x03, gScratch3_80180210},
                            {0x2524, 0x06, gScratch3_80180210},
                            {0x252a, 0x0e, gScratch3_80180210},
                            {0x2538, 0x09, gScratch3_80180210},
                            {0x2541, 0x05, gScratch3_80180210},
                            {0x2546, 0x04, gScratch3_80180210},
                            {0x254a, 0x0a, gScratch3_80180210},
                            {0x2554, 0x03, gScratch3_80180210},
                            {0x2557, 0x16, gScratch3_80180210},
                            {0x256d, 0x15, gScratch3_80180210},
                            {0x2582, 0x16, gScratch3_80180210},
                            {0x2598, 0x17, gScratch3_80180210},
                            {0x25af, 0x15, gScratch3_80180210},
                            {0x25c4, 0x0d, gScratch3_80180210},
                            {0x25d1, 0x0c, gScratch3_80180210},
                            {0x25dd, 0x12, gScratch3_80180210},
                            {0x25ef, 0x0b, gScratch3_80180210},
                            {0x25fa, 0x07, gScratch3_80180210},
                            {0x2601, 0x0a, gScratch3_80180210},
                            {0x260b, 0x09, gScratch3_80180210},
                            {0x2614, 0x0a, gScratch3_80180210},
                            {0x261e, 0x05, gScratch3_80180210},
                            {0x2623, 0x01, gScratch3_80180210},
                            {0x2624, 0x0e, gScratch3_80180210},
                            {0x220c, 0x02, gScratch3_80180210},
                            {0x21c6, 0x46, gScratch3_80180210},
                            {0x229a, 0x02, gScratch3_80180210},
                            {0x228c, 0x0e, gScratch3_80180210},
                            {0x2357, 0x02, gScratch3_80180210},
                            {0x234f, 0x08, gScratch3_80180210},
                            {0x23ad, 0x02, gScratch3_80180210},
                            {0x2389, 0x24, gScratch3_80180210},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x2aa0, 0x4a, gScratch3_80180210},
                            {0x2aea, 0x4d, gScratch3_80180210},
                            {0x2b37, 0x4c, gScratch3_80180210},
                            {0x2b83, 0x4a, gScratch3_80180210},
                            {0x2bcd, 0x45, gScratch3_80180210},
                            {0x2c12, 0x48, gScratch3_80180210},
                            {0x2c5a, 0x4c, gScratch3_80180210},
                            {0x2ca6, 0x50, gScratch3_80180210},
                            {0x2cf6, 0x47, gScratch3_80180210},
                            {0x2d3d, 0x4b, gScratch3_80180210},
                            {0x2d88, 0x4a, gScratch3_80180210},
                            {0x2dd2, 0x4e, gScratch3_80180210},
                            {0x2e20, 0x43, gScratch3_80180210},
                            {0x2e63, 0x4a, gScratch3_80180210},
                            {0x2ead, 0x3c, gScratch3_80180210},
                            {0x2ee9, 0x42, gScratch3_80180210},
                            {0x2f2b, 0x4c, gScratch3_80180210},
                            {0x2f77, 0x4c, gScratch3_80180210},
                            {0x2fc3, 0x45, gScratch3_80180210},
                            {0x3008, 0x42, gScratch3_80180210},
                            {0x304a, 0x4a, gScratch3_80180210},
                            {0x3094, 0x3b, gScratch3_80180210},
                            {0x30cf, 0x34, gScratch3_80180210},
                            {0x3103, 0x42, gScratch3_80180210},
                            {0x3145, 0x37, gScratch3_80180210},
                            {0x317c, 0x38, gScratch3_80180210},
                            {0x31b4, 0x37, gScratch3_80180210},
                            {0x31eb, 0x37, gScratch3_80180210},
                            {0x3222, 0x36, gScratch3_80180210},
                            {0x3258, 0x34, gScratch3_80180210},
                            {0x328c, 0x31, gScratch3_80180210},
                            {0x32bd, 0x32, gScratch3_80180210},
                            {0x32ef, 0x31, gScratch3_80180210},
                            {0x3320, 0x31, gScratch3_80180210},
                            {0x3351, 0x30, gScratch3_80180210},
                            {0x3381, 0x31, gScratch3_80180210},
                            {0x33b2, 0x31, gScratch3_80180210},
                            {0x33e3, 0x31, gScratch3_80180210},
                            {0x3414, 0x31, gScratch3_80180210},
                            {0x3445, 0x30, gScratch3_80180210},
                            {0x3475, 0x2d, gScratch3_80180210},
                            {0x34a2, 0x2d, gScratch3_80180210},
                            {0x34cf, 0x2d, gScratch3_80180210},
                            {0x34fc, 0x2d, gScratch3_80180210},
                            {0x3529, 0x2d, gScratch3_80180210},
                            {0x3556, 0x2d, gScratch3_80180210},
                            {0x3583, 0x2c, gScratch3_80180210},
                            {0x35af, 0x2c, gScratch3_80180210},
                            {0x35db, 0x2c, gScratch3_80180210},
                            {0x3607, 0x2e, gScratch3_80180210},
                            {0x1b4c, 0x0b, gScratch1_801317c0},
                            {0x1bcc, 0x0c, gScratch1_801317c0},
                            {0x1c4d, 0x0d, gScratch1_801317c0},
                            {0x1ccf, 0x0b, gScratch1_801317c0},
                            {0x1d4f, 0x0b, gScratch1_801317c0},
                            {0x1dcf, 0x0e, gScratch1_801317c0},
                            {0x1e52, 0x0c, gScratch1_801317c0},
                            {0x1ed3, 0x0e, gScratch1_801317c0},
                            {0x1f56, 0x0b, gScratch1_801317c0},
                            {0x1fd6, 0x10, gScratch1_801317c0},
                            {0x205b, 0x0c, gScratch1_801317c0},
                            {0x2067, 0x0c, gScratch1_801317c0},
                            {0x2073, 0x0a, gScratch1_801317c0},
                            {0x207d, 0x09, gScratch1_801317c0},
                            {0x2086, 0x0e, gScratch1_801317c0},
                            {0x2094, 0x0c, gScratch1_801317c0},
                            {0x20a0, 0x0f, gScratch1_801317c0},
                            {0x20af, 0x0c, gScratch1_801317c0},
                            {0x20bb, 0x0d, gScratch1_801317c0},
                            {0x20c8, 0x0f, gScratch1_801317c0},
                            {0x20d7, 0x0d, gScratch1_801317c0},
                            {0x20e4, 0x10, gScratch1_801317c0},
                            {0x20f4, 0x12, gScratch1_801317c0},
                            {0x2106, 0x0e, gScratch1_801317c0},
                            {0x2114, 0x0f, gScratch1_801317c0},
                            {0x2123, 0x0d, gScratch1_801317c0},
                            {0x2130, 0x0c, gScratch1_801317c0},
                            {0x213c, 0x11, gScratch1_801317c0},
                            {0x214d, 0x0f, gScratch1_801317c0},
                            {0x215c, 0x14, gScratch1_801317c0},
                            {0x2170, 0x10, gScratch1_801317c0},
                            {0x2180, 0x0b, gScratch1_801317c0},
                            {0x218b, 0x0e, gScratch1_801317c0},
                            {0x2199, 0x0d, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x0000, 0x00, gScratch1_801317c0},
                            {0x1a72, 0x07, gScratch1_801317c0},
                            {0x1a79, 0x08, gScratch1_801317c0},
                            {0x1a81, 0x07, gScratch1_801317c0},
                            {0x1a88, 0x09, gScratch1_801317c0},
                            {0x1a91, 0x08, gScratch1_801317c0},
                            {0x1347, 0x07, gScratch1_801317c0},
                            {0x1a99, 0x0b, gScratch1_801317c0},
                            {0x1aa4, 0x0d, gScratch1_801317c0},
                            {0x135a, 0x3d, gScratch3_80180210},
                            {0x1282, 0x50, gScratch3_80180210},
                            {0x0e41, 0x6c, gScratch3_80180210},
                            {0x0f1a, 0x6c, gScratch3_80180210},
                            {0x0a30, 0x6c, gScratch3_80180210},
                            {0x0b09, 0x6c, gScratch3_80180210},
                            {0x0be2, 0x6c, gScratch3_80180210},
                            {0x0ddb, 0x10, gScratch3_80180210},
                            {0x0deb, 0x10, gScratch3_80180210},
                            {0x0dfb, 0x10, gScratch3_80180210},
                            {0x0e0b, 0x10, gScratch3_80180210},
                            {0x0000, 0x00, gScratch3_80180210},
                            {0x108f, 0x10, gScratch3_80180210},
                            {0x109f, 0x10, gScratch3_80180210},
                            {0x10af, 0x10, gScratch3_80180210},
                            {0x10bf, 0x10, gScratch3_80180210},
                            {0x1116, 0x10, gScratch3_80180210},
                            {0x1126, 0x10, gScratch3_80180210},
                            {0x1136, 0x10, gScratch3_80180210},
                            {0x1146, 0x10, gScratch3_80180210},
                            {0x1156, 0x10, gScratch3_80180210},
                            {0x1166, 0x10, gScratch3_80180210},
                            {0x1176, 0x10, gScratch3_80180210},
                            {0x1186, 0x10, gScratch3_80180210},
                            {0x1196, 0x10, gScratch3_80180210},
                            {0x11a6, 0x10, gScratch3_80180210},
                            {0x11b6, 0x10, gScratch3_80180210},
                            {0x11c6, 0x10, gScratch3_80180210},
                            {0x11d6, 0x10, gScratch3_80180210},
                            {0x11e6, 0x10, gScratch3_80180210},
                            {0x11f6, 0x10, gScratch3_80180210},
                            {0x1206, 0x10, gScratch3_80180210},
                            {0x1216, 0x10, gScratch3_80180210},
                            {0x1226, 0x10, gScratch3_80180210},
                            {0x106f, 0x10, gScratch3_80180210},
                            {0x107f, 0x10, gScratch3_80180210},
                            {0x3635, 0x2b, gScratch3_80180210},
                            {0x3660, 0x3c, gScratch3_80180210},
                            {0x369c, 0x2e, gScratch3_80180210},
                            {0x36ca, 0x2b, gScratch3_80180210},
                            {0x36f5, 0x42, gScratch3_80180210},
                            {0x3737, 0x30, gScratch3_80180210},
                            {0x3767, 0x31, gScratch3_80180210},
                            {0x3798, 0x2f, gScratch3_80180210},
                            {0x37c7, 0x30, gScratch3_80180210},
                            {0x37f7, 0x34, gScratch3_80180210},
                            {0x382b, 0x3d, gScratch3_80180210},
                            {0x3868, 0x38, gScratch3_80180210},
                            {0x38a0, 0x34, gScratch3_80180210},
                            {0x38d4, 0x3e, gScratch3_80180210},
                            {0x3912, 0x36, gScratch3_80180210},
                            {0x3948, 0x2f, gScratch3_80180210},
                            {0x3977, 0x2e, gScratch3_80180210},
                            {0x39a5, 0x32, gScratch3_80180210},
                            {0x39d7, 0x3e, gScratch3_80180210},
                            {0x3a15, 0x40, gScratch3_80180210},
                            {0x3a55, 0x45, gScratch3_80180210},
                            {0x3a9a, 0x38, gScratch3_80180210},
                            {0x3ad2, 0x2f, gScratch3_80180210},
                            {0x3b01, 0x37, gScratch3_80180210},
                            {0x3b38, 0x32, gScratch3_80180210},
                            {0x3b6a, 0x31, gScratch3_80180210},
                            {0x3b9b, 0x2f, gScratch3_80180210},
                            {0x3bca, 0x40, gScratch3_80180210},
                            {0x3c0a, 0x2f, gScratch3_80180210},
                            {0x3c39, 0x2b, gScratch3_80180210},
                            {0x3c64, 0x3b, gScratch3_80180210},
                            {0x27e6, 0x02, gScratch1_801317c0},
                            {0x21c1, 0x05, gScratch1_801317c0},
                            {0x10f4, 0x22, gScratch3_80180210},
                            {0x10cf, 0x25, gScratch3_80180210},
                            {0x1ab1, 0x0a, gScratch1_801317c0},
                            {0x1abb, 0x0c, gScratch1_801317c0},
                            {0x1ac7, 0x0c, gScratch1_801317c0},
                            {0x1ad3, 0x04, gScratch1_801317c0},
                            {0x105f, 0x10, gScratch3_80180210},
                            {0x0ccb, 0x10, gScratch3_80180210},
                            {0x0cdb, 0x10, gScratch3_80180210},
                            {0x0ceb, 0x10, gScratch3_80180210},
                            {0x0cfb, 0x10, gScratch3_80180210},
                            {0x0d0b, 0x10, gScratch3_80180210},
                            {0x0d1b, 0x10, gScratch3_80180210},
                            {0x0d2b, 0x10, gScratch3_80180210},
                            {0x0d3b, 0x10, gScratch3_80180210},
                            {0x19f9, 0x0b, gScratch1_801317c0},
                            {0x1a04, 0x0a, gScratch1_801317c0},
                            {0x1a0e, 0x07, gScratch1_801317c0},
                            {0x1a15, 0x07, gScratch1_801317c0},
                            {0x1a1c, 0x07, gScratch1_801317c0},
                            {0x1a23, 0x07, gScratch1_801317c0},
                            {0x1a2a, 0x07, gScratch1_801317c0},
                            {0x1a31, 0x07, gScratch1_801317c0},
                            {0x1a38, 0x08, gScratch1_801317c0},
                            {0x1a40, 0x07, gScratch1_801317c0},
                            {0x1a47, 0x07, gScratch1_801317c0},
                            {0x1a4e, 0x07, gScratch1_801317c0},
                            {0x1a55, 0x08, gScratch1_801317c0},
                            {0x1a5d, 0x07, gScratch1_801317c0},
                            {0x1a64, 0x07, gScratch1_801317c0},
                            {0x1a6b, 0x07, gScratch1_801317c0},
                            {0x0629, 0x26, gScratch3_80180210},
                            {0x0759, 0x26, gScratch3_80180210},
                            {0x0863, 0x26, gScratch3_80180210},
                            {0x0889, 0x26, gScratch3_80180210},
                            {0x4b4e, 0x26, gScratch3_80180210},
                            {0x1236, 0x26, gScratch3_80180210},
                            {0x07cb, 0x26, gScratch3_80180210},
                            {0x0000, 0x00, NULL},
                            {0x4b74, 0x26, gScratch3_80180210},
                            {0x0000, 0x00, NULL},
                            {0x1397, 0x26, gScratch3_80180210},
                            {0x13bd, 0x26, gScratch3_80180210},
                            {0x13e3, 0x26, gScratch3_80180210},
                            {0x1409, 0x26, gScratch3_80180210},
                            {0x142f, 0x26, gScratch3_80180210},
                            {0x1455, 0x26, gScratch3_80180210},
                            {0x147b, 0x26, gScratch3_80180210},
                            {0x14a1, 0x26, gScratch3_80180210},
                            {0x14c7, 0x26, gScratch3_80180210},
                            {0x14ed, 0x26, gScratch3_80180210},
                            {0x1513, 0x26, gScratch3_80180210},
                            {0x1539, 0x26, gScratch3_80180210},
                            {0x155f, 0x26, gScratch3_80180210},
                            {0x1585, 0x26, gScratch3_80180210},
                            {0x15ab, 0x26, gScratch3_80180210},
                            {0x15d1, 0x26, gScratch3_80180210},
                            {0x15f7, 0x26, gScratch3_80180210},
                            {0x161d, 0x26, gScratch3_80180210},
                            {0x1643, 0x26, gScratch3_80180210},
                            {0x1669, 0x26, gScratch3_80180210},
                            {0x168f, 0x26, gScratch3_80180210},
                            {0x16b5, 0x26, gScratch3_80180210},
                            {0x16db, 0x26, gScratch3_80180210},
                            {0x1701, 0x26, gScratch3_80180210},
                            {0x1727, 0x26, gScratch3_80180210},
                            {0x4a90, 0x26, gScratch3_80180210},
                            {0x4ab6, 0x26, gScratch3_80180210},
                            {0x4adc, 0x26, gScratch3_80180210},
                            {0x4b02, 0x26, gScratch3_80180210},
                            {0x4b28, 0x26, gScratch3_80180210},
                            {0x0817, 0x26, gScratch3_80180210},
                            {0x07a5, 0x26, gScratch3_80180210},
                            {0x08af, 0x26, gScratch3_80180210},
                            {0x06e7, 0x26, gScratch3_80180210},
                            {0x083d, 0x26, gScratch3_80180210},
                            {0x070d, 0x26, gScratch3_80180210},
                            {0x0733, 0x26, gScratch3_80180210},
                            {0x07f1, 0x26, gScratch3_80180210},
                            {0x06c1, 0x26, gScratch3_80180210},
                            {0x069b, 0x26, gScratch3_80180210},
                            {0x077f, 0x26, gScratch3_80180210},
                            {0x09b2, 0x01, gScratch1_801317c0},
                            {0x09b3, 0x01, gScratch1_801317c0},
                            {0x09b4, 0x02, gScratch1_801317c0},
                            {0x09b6, 0x02, gScratch1_801317c0},
                            {0x4be6, 0x26, gScratch3_80180210},
                            {0x2908, 0x26, gScratch3_80180210},
                            {0x28bc, 0x26, gScratch3_80180210},
                            {0x2954, 0x26, gScratch3_80180210},
                            {0x284a, 0x26, gScratch3_80180210},
                            {0x292e, 0x26, gScratch3_80180210},
                            {0x2870, 0x26, gScratch3_80180210},
                            {0x2896, 0x26, gScratch3_80180210},
                            {0x28e2, 0x26, gScratch3_80180210},
                            {0x2824, 0x26, gScratch3_80180210},
                            {0x27fe, 0x26, gScratch3_80180210},
                            {0x0507, 0x26, gScratch3_80180210},
                            {0x04bb, 0x26, gScratch3_80180210},
                            {0x0553, 0x26, gScratch3_80180210},
                            {0x0449, 0x26, gScratch3_80180210},
                            {0x052d, 0x26, gScratch3_80180210},
                            {0x046f, 0x26, gScratch3_80180210},
                            {0x0495, 0x26, gScratch3_80180210},
                            {0x04e1, 0x26, gScratch3_80180210},
                            {0x0423, 0x26, gScratch3_80180210},
                            {0x03fd, 0x26, gScratch3_80180210},
                            {0x0949, 0x01, gScratch2_8013e054},
                            {0x094c, 0x01, gScratch2_8013e054},
                            {0x0952, 0x02, gScratch2_8013e054},
                            {0x0954, 0x01, gScratch2_8013e054},
                            {0x094f, 0x03, gScratch2_8013e054},
                            {0x095b, 0x01, gScratch2_8013e054},
                            {0x095d, 0x01, gScratch2_8013e054},
                            {0x0961, 0x02, gScratch2_8013e054},
                            {0x0965, 0x01, gScratch2_8013e054},
                            {0x0968, 0x01, gScratch2_8013e054},
                            {0x0970, 0x01, gScratch2_8013e054},
                            {0x0975, 0x01, gScratch2_8013e054},
                            {0x097a, 0x01, gScratch2_8013e054},
                            {0x097c, 0x02, gScratch2_8013e054},
                            {0x097e, 0x01, gScratch2_8013e054},
                            {0x0980, 0x02, gScratch2_8013e054},
                            {0x0985, 0x01, gScratch2_8013e054},
                            {0x0988, 0x01, gScratch2_8013e054},
                            {0x098c, 0x02, gScratch2_8013e054},
                            {0x0990, 0x01, gScratch2_8013e054},
                            {0x0995, 0x01, gScratch2_8013e054},
                            {0x4bc0, 0x26, gScratch3_80180210},
                            {0x4b9a, 0x26, gScratch3_80180210},
                            {0x0675, 0x26, gScratch3_80180210},
                            {0x09b8, 0x01, gScratch1_801317c0},
                            {0x09b9, 0x01, gScratch1_801317c0},
                            {0x09bb, 0x01, gScratch1_801317c0},
                            {0x09be, 0x01, gScratch1_801317c0},
                            {0x09bf, 0x01, gScratch1_801317c0},
                            {0x09c0, 0x01, gScratch1_801317c0},
                            {0x09c1, 0x01, gScratch1_801317c0},
                            {0x09c2, 0x01, gScratch1_801317c0},
                            {0x09c3, 0x01, gScratch1_801317c0},
                            {0x09c4, 0x01, gScratch1_801317c0},
                            {0x09c6, 0x01, gScratch1_801317c0},
                            {0x09c8, 0x01, gScratch1_801317c0},
                            {0x09c9, 0x01, gScratch1_801317c0},
                            {0x09ca, 0x01, gScratch1_801317c0},
                            {0x09cc, 0x01, gScratch1_801317c0},
                            {0x09cd, 0x01, gScratch1_801317c0},
                            {0x09cf, 0x01, gScratch1_801317c0},
                            {0x09d0, 0x01, gScratch1_801317c0},
                            {0x09d1, 0x01, gScratch1_801317c0},
                            {0x09d2, 0x01, gScratch1_801317c0},
                            {0x09d3, 0x01, gScratch1_801317c0},
                            {0x09d4, 0x01, gScratch1_801317c0},
                            {0x09d5, 0x02, gScratch1_801317c0},
                            {0x09d7, 0x01, gScratch1_801317c0},
                            {0x09d8, 0x01, gScratch1_801317c0},
                            {0x09da, 0x01, gScratch1_801317c0},
                            {0x09db, 0x01, gScratch1_801317c0},
                            {0x09dc, 0x01, gScratch1_801317c0},
                            {0x09de, 0x01, gScratch1_801317c0},
                            {0x09df, 0x01, gScratch1_801317c0},
                            {0x09e0, 0x01, gScratch1_801317c0},
                            {0x09e1, 0x01, gScratch1_801317c0},
                            {0x09e3, 0x01, gScratch1_801317c0},
                            {0x09e6, 0x01, gScratch1_801317c0},
                            {0x09e8, 0x01, gScratch1_801317c0},
                            {0x09e9, 0x01, gScratch1_801317c0},
                            {0x09ea, 0x01, gScratch1_801317c0},
                            {0x09eb, 0x01, gScratch1_801317c0},
                            {0x09ec, 0x01, gScratch1_801317c0},
                            {0x09ed, 0x01, gScratch1_801317c0},
                            {0x09ee, 0x01, gScratch1_801317c0},
                            {0x09ef, 0x01, gScratch1_801317c0},
                            {0x09f0, 0x01, gScratch1_801317c0},
                            {0x09f1, 0x01, gScratch1_801317c0},
                            {0x09f2, 0x01, gScratch1_801317c0},
                            {0x09f3, 0x01, gScratch1_801317c0},
                            {0x09f4, 0x01, gScratch1_801317c0},
                            {0x09f5, 0x01, gScratch1_801317c0},
                            {0x09f6, 0x01, gScratch1_801317c0},
                            {0x09f7, 0x01, gScratch1_801317c0},
                            {0x09f8, 0x01, gScratch1_801317c0},
                            {0x09f9, 0x01, gScratch1_801317c0},
                            {0x09fa, 0x01, gScratch1_801317c0},
                            {0x09fb, 0x01, gScratch1_801317c0},
                            {0x09fc, 0x01, gScratch1_801317c0},
                            {0x09fd, 0x01, gScratch1_801317c0},
                            {0x09fe, 0x01, gScratch1_801317c0},
                            {0x09ff, 0x01, gScratch1_801317c0},
                            {0x0a00, 0x01, gScratch1_801317c0},
                            {0x0a01, 0x01, gScratch1_801317c0},
                            {0x0a02, 0x01, gScratch1_801317c0},
                            {0x0a03, 0x01, gScratch1_801317c0},
                            {0x0a04, 0x01, gScratch1_801317c0},
                            {0x0a05, 0x01, gScratch1_801317c0},
                            {0x0a06, 0x01, gScratch1_801317c0},
                            {0x0a07, 0x01, gScratch1_801317c0},
                            {0x0a08, 0x01, gScratch1_801317c0},
                            {0x0a09, 0x01, gScratch1_801317c0},
                            {0x0a0a, 0x01, gScratch1_801317c0},
                            {0x0a0b, 0x01, gScratch1_801317c0},
                            {0x0a0c, 0x01, gScratch1_801317c0},
                            {0x0a0d, 0x01, gScratch1_801317c0},
                            {0x0a0e, 0x01, gScratch1_801317c0},
                            {0x0a0f, 0x01, gScratch1_801317c0},
                            {0x09e7, 0x01, gScratch1_801317c0},
                            {0x09cb, 0x01, gScratch1_801317c0},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x094a, 0x01, gScratch2_8013e054},
                            {0x094b, 0x01, gScratch2_8013e054},
                            {0x094d, 0x02, gScratch2_8013e054},
                            {0x0954, 0x01, gScratch2_8013e054},
                            {0x0955, 0x01, gScratch2_8013e054},
                            {0x0956, 0x01, gScratch2_8013e054},
                            {0x0957, 0x01, gScratch2_8013e054},
                            {0x0958, 0x01, gScratch2_8013e054},
                            {0x0959, 0x01, gScratch2_8013e054},
                            {0x095a, 0x01, gScratch2_8013e054},
                            {0x095c, 0x01, gScratch2_8013e054},
                            {0x095e, 0x01, gScratch2_8013e054},
                            {0x095f, 0x01, gScratch2_8013e054},
                            {0x0960, 0x01, gScratch2_8013e054},
                            {0x0963, 0x01, gScratch2_8013e054},
                            {0x0964, 0x01, gScratch2_8013e054},
                            {0x0966, 0x01, gScratch2_8013e054},
                            {0x0967, 0x01, gScratch2_8013e054},
                            {0x0968, 0x01, gScratch2_8013e054},
                            {0x0969, 0x01, gScratch2_8013e054},
                            {0x096a, 0x01, gScratch2_8013e054},
                            {0x096b, 0x01, gScratch2_8013e054},
                            {0x096c, 0x02, gScratch2_8013e054},
                            {0x096e, 0x01, gScratch2_8013e054},
                            {0x096f, 0x01, gScratch2_8013e054},
                            {0x0971, 0x02, gScratch2_8013e054},
                            {0x0973, 0x01, gScratch2_8013e054},
                            {0x0974, 0x01, gScratch2_8013e054},
                            {0x0976, 0x01, gScratch2_8013e054},
                            {0x0977, 0x01, gScratch2_8013e054},
                            {0x0978, 0x01, gScratch2_8013e054},
                            {0x0979, 0x01, gScratch2_8013e054},
                            {0x097b, 0x01, gScratch2_8013e054},
                            {0x097f, 0x01, gScratch2_8013e054},
                            {0x0982, 0x01, gScratch2_8013e054},
                            {0x0983, 0x01, gScratch2_8013e054},
                            {0x0984, 0x01, gScratch2_8013e054},
                            {0x0986, 0x01, gScratch2_8013e054},
                            {0x0987, 0x01, gScratch2_8013e054},
                            {0x0989, 0x02, gScratch2_8013e054},
                            {0x098b, 0x01, gScratch2_8013e054},
                            {0x098e, 0x01, gScratch2_8013e054},
                            {0x098f, 0x01, gScratch2_8013e054},
                            {0x0991, 0x01, gScratch2_8013e054},
                            {0x0992, 0x01, gScratch2_8013e054},
                            {0x0993, 0x01, gScratch2_8013e054},
                            {0x0994, 0x01, gScratch2_8013e054},
                            {0x0996, 0x01, gScratch2_8013e054},
                            {0x0997, 0x01, gScratch2_8013e054},
                            {0x0998, 0x01, gScratch2_8013e054},
                            {0x0999, 0x01, gScratch2_8013e054},
                            {0x099a, 0x01, gScratch2_8013e054},
                            {0x099b, 0x01, gScratch2_8013e054},
                            {0x099c, 0x01, gScratch2_8013e054},
                            {0x099d, 0x01, gScratch2_8013e054},
                            {0x099e, 0x01, gScratch2_8013e054},
                            {0x099f, 0x01, gScratch2_8013e054},
                            {0x09a0, 0x01, gScratch2_8013e054},
                            {0x09a1, 0x01, gScratch2_8013e054},
                            {0x09a2, 0x01, gScratch2_8013e054},
                            {0x09a3, 0x01, gScratch2_8013e054},
                            {0x09a4, 0x01, gScratch2_8013e054},
                            {0x09a5, 0x02, gScratch2_8013e054},
                            {0x09a7, 0x01, gScratch2_8013e054},
                            {0x09a8, 0x01, gScratch2_8013e054},
                            {0x09a9, 0x01, gScratch2_8013e054},
                            {0x09aa, 0x01, gScratch2_8013e054},
                            {0x09ab, 0x01, gScratch2_8013e054},
                            {0x09ac, 0x01, gScratch2_8013e054},
                            {0x09ad, 0x01, gScratch2_8013e054},
                            {0x09ae, 0x01, gScratch2_8013e054},
                            {0x09af, 0x01, gScratch2_8013e054},
                            {0x09b0, 0x01, gScratch2_8013e054},
                            {0x09b1, 0x01, gScratch2_8013e054},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0000, 0x00, NULL},
                            {0x0579, 0x01, gScratch1_801317c0},
                            {0x057a, 0x01, gScratch1_801317c0},
                            {0x057b, 0x01, gScratch1_801317c0},
                            {0x057c, 0x01, gScratch1_801317c0},
                            {0x057d, 0x01, gScratch1_801317c0},
                            {0x057e, 0x01, gScratch1_801317c0},
                            {0x057f, 0x01, gScratch1_801317c0},
                            {0x0580, 0x01, gScratch1_801317c0},
                            {0x0581, 0x01, gScratch1_801317c0},
                            {0x0582, 0x01, gScratch1_801317c0},
                            {0x0583, 0x01, gScratch1_801317c0},
                            {0x0584, 0x01, gScratch1_801317c0},
                            {0x0585, 0x01, gScratch1_801317c0},
                            {0x0586, 0x01, gScratch1_801317c0},
                            {0x0587, 0x01, gScratch1_801317c0},
                            {0x0588, 0x01, gScratch1_801317c0},
                            {0x0589, 0x01, gScratch1_801317c0},
                            {0x058a, 0x01, gScratch1_801317c0},
                            {0x058b, 0x01, gScratch1_801317c0},
                            {0x058c, 0x01, gScratch1_801317c0},
                            {0x058d, 0x01, gScratch1_801317c0},
                            {0x058e, 0x01, gScratch1_801317c0},
                            {0x058f, 0x01, gScratch1_801317c0},
                            {0x0590, 0x01, gScratch1_801317c0},
                            {0x0591, 0x01, gScratch1_801317c0},
                            {0x0592, 0x01, gScratch1_801317c0},
                            {0x0593, 0x01, gScratch1_801317c0},
                            {0x0594, 0x01, gScratch1_801317c0},
                            {0x0595, 0x01, gScratch1_801317c0},
                            {0x0596, 0x01, gScratch1_801317c0},
                            {0x0597, 0x01, gScratch1_801317c0},
                            {0x0598, 0x01, gScratch1_801317c0},
                            {0x0599, 0x01, gScratch1_801317c0},
                            {0x059a, 0x01, gScratch1_801317c0},
                            {0x059b, 0x01, gScratch1_801317c0},
                            {0x059c, 0x01, gScratch1_801317c0},
                            {0x059d, 0x01, gScratch1_801317c0},
                            {0x059e, 0x01, gScratch1_801317c0},
                            {0x059f, 0x01, gScratch1_801317c0},
                            {0x05a0, 0x01, gScratch1_801317c0},
                            {0x05a1, 0x01, gScratch1_801317c0},
                            {0x05a2, 0x01, gScratch1_801317c0},
                            {0x05a3, 0x01, gScratch1_801317c0},
                            {0x05a4, 0x01, gScratch1_801317c0},
                            {0x29a2, 0x02, gScratch1_801317c0},
                            {0x29a0, 0x02, gScratch1_801317c0},
                            {0x05a5, 0x04, gScratch1_801317c0},
                            {0x3c9f, 0x34, gScratch3_80180210},
                            {0x3cd3, 0x2f, gScratch3_80180210},
                            {0x3d02, 0x29, gScratch3_80180210},
                            {0x3d2b, 0x3e, gScratch3_80180210},
                            {0x3d69, 0x2d, gScratch3_80180210},
                            {0x3d96, 0x2c, gScratch3_80180210},
                            {0x3dc2, 0x34, gScratch3_80180210},
                            {0x3df6, 0x45, gScratch3_80180210},
                            {0x3e3b, 0x2c, gScratch3_80180210},
                            {0x3e67, 0x2b, gScratch3_80180210},
                            {0x3e92, 0x4f, gScratch3_80180210},
                            {0x3ee1, 0x33, gScratch3_80180210},
                            {0x3f14, 0x34, gScratch3_80180210},
                            {0x3f48, 0x29, gScratch3_80180210},
                            {0x3f71, 0x32, gScratch3_80180210},
                            {0x3fa3, 0x2b, gScratch3_80180210},
                            {0x3fce, 0x3c, gScratch3_80180210},
                            {0x400a, 0x31, gScratch3_80180210},
                            {0x403b, 0x41, gScratch3_80180210},
                            {0x407c, 0x2f, gScratch3_80180210},
                            {0x40ab, 0x45, gScratch3_80180210},
                            {0x40f0, 0x2c, gScratch3_80180210},
                            {0x411c, 0x2c, gScratch3_80180210},
                            {0x4148, 0x36, gScratch3_80180210},
                            {0x417e, 0x4b, gScratch3_80180210},
                            {0x41c9, 0x2b, gScratch3_80180210},
                            {0x41f4, 0x40, gScratch3_80180210},
                            {0x4234, 0x2f, gScratch3_80180210},
                            {0x4263, 0x4b, gScratch3_80180210},
                            {0x42ae, 0x2a, gScratch3_80180210},
                            {0x42d8, 0x30, gScratch3_80180210},
                            {0x4308, 0x44, gScratch3_80180210},
                            {0x434c, 0x2a, gScratch3_80180210},
                            {0x4376, 0x3d, gScratch3_80180210},
                            {0x43b3, 0x2a, gScratch3_80180210},
                            {0x43dd, 0x38, gScratch3_80180210},
                            {0x4415, 0x2e, gScratch3_80180210},
                            {0x4443, 0x47, gScratch3_80180210},
                            {0x448a, 0x36, gScratch3_80180210},
                            {0x44c0, 0x33, gScratch3_80180210},
                            {0x44f3, 0x30, gScratch3_80180210},
                            {0x4523, 0x2f, gScratch3_80180210},
                            {0x4552, 0x29, gScratch3_80180210},
                            {0x457b, 0x47, gScratch3_80180210},
                            {0x45c2, 0x2f, gScratch3_80180210},
                            {0x45f1, 0x49, gScratch3_80180210},
                            {0x463a, 0x2f, gScratch3_80180210},
                            {0x4669, 0x2d, gScratch3_80180210},
                            {0x4696, 0x2a, gScratch3_80180210},
                            {0x46c0, 0x29, gScratch3_80180210},
                            {0x46e9, 0x44, gScratch3_80180210},
                            {0x472d, 0x4e, gScratch3_80180210},
                            {0x477b, 0x34, gScratch3_80180210},
                            {0x47af, 0x3a, gScratch3_80180210},
                            {0x47e9, 0x40, gScratch3_80180210},
                            {0x4829, 0x4b, gScratch3_80180210},
                            {0x4874, 0x35, gScratch3_80180210},
                            {0x48a9, 0x45, gScratch3_80180210},
                            {0x48ee, 0x4b, gScratch3_80180210},
                            {0x4939, 0x3f, gScratch3_80180210},
                            {0x4978, 0x4d, gScratch3_80180210},
                            {0x49c5, 0x50, gScratch3_80180210},
                            {0x4a15, 0x33, gScratch3_80180210},
                            {0x0000, 0x00, gScratch3_80180210},
                            {0x21b2, 0x0f, gScratch1_801317c0}};

s32 s_frameNum_80123264;
s32 s_totalFrames_80123268;
s32 s_movieFinished_8012326c;
u8 s_cdMode_80123270;

void CDInit(void) {
   CdInit();
   gCdLoader.state = 99;
   gVabLoader.state = 99;
   gSeqLoader.state = 99;
}

void LoadCdFile(s32 cdfIdx, s32 showLoadingScreen) {
   extern TILE s_blackScreen_80123918;
   EvtData *nowLoading;
   EvtData *logo;
   EvtData *faerie;
   s32 faerieFrameTimer;
   s32 faerieFrameToggle;

   gCdLoader.cdf = cdfIdx;
   gCdLoader.loadingVabBody = 0;
   gCdLoader.state = 0;
   gCdLoader.stalledCounter = 0;
   gCdLoader.sectorsToRead = gCdFiles[cdfIdx].sectorCt;
   gCdLoader.sector = gCdFiles[cdfIdx].startingSector;

   if (gState.suppressLoadingScreen || !showLoadingScreen) {
      while (GetCdFileLoadStatus() != 0) {
         ContinueLoadingCdFile();
         VSync(0);
      }
   } else {
      SetTile(&s_blackScreen_80123918);
      s_blackScreen_80123918.r0 = s_blackScreen_80123918.g0 = s_blackScreen_80123918.b0 = 0;
      s_blackScreen_80123918.x0 = s_blackScreen_80123918.y0 = 0;
      s_blackScreen_80123918.w = SCREEN_WIDTH;
      s_blackScreen_80123918.h = SCREEN_HEIGHT;

      nowLoading = Evt_GetUnused();
      nowLoading->functionIndex = EVTF_NOOP;
      nowLoading->d.sprite.gfxIdx = GFX_NOW_LOADING;
      nowLoading->x1.n = 120;
      nowLoading->y1.n = 95;
      nowLoading->x3.n = nowLoading->x1.n + 80;
      nowLoading->y3.n = nowLoading->y1.n + 50;

      logo = Evt_GetUnused();
      logo->functionIndex = EVTF_NOOP;
      logo->d.sprite.gfxIdx = GFX_VANDAL_HEARTS;
      logo->x1.n = 156;
      logo->y1.n = 184;
      logo->x3.n = logo->x1.n + 128;
      logo->y3.n = logo->y1.n + 32;

      faerie = Evt_GetUnused();
      faerie->functionIndex = EVTF_NOOP;
      faerie->x1.n = 24;
      faerie->y1.n = 192;
      faerie->x3.n = faerie->x1.n + 24;
      faerie->y3.n = faerie->y1.n + 24;
      faerieFrameTimer = 0;
      faerieFrameToggle = 0;
      faerie->d.sprite.gfxIdx = GFX_FAERIE_1;

      do {
         UpdateAudio();
         rand();
         if (gState.vsyncMode == 2) {
            gState.frameCounter += 2;
         } else {
            gState.frameCounter += 1;
         }
         gState.unitMarkerSpin += 0x20;
         gOscillation = (gOscillation + 0x100) & 0xfff;
         gGridColorOscillation = rcos(gOscillation) * 100 >> 12;
         gGridColorOscillation += 155U;
         gQuadIndex = 0;
         UpdateInput();
         gGraphicsPtr =
             (gGraphicsPtr == &gGraphicBuffers[0]) ? &gGraphicBuffers[1] : &gGraphicBuffers[0];
         ClearOTag(gGraphicsPtr->ot, OT_SIZE);
         if (++faerieFrameTimer > 4) {
            faerieFrameToggle = !faerieFrameToggle;
            faerie->d.sprite.gfxIdx = GFX_FAERIE_1 + faerieFrameToggle;
            faerieFrameTimer = 0;
         }
         AddEvtPrim_Gui(gGraphicsPtr->ot, nowLoading);
         AddEvtPrim_Gui(gGraphicsPtr->ot, logo);
         AddEvtPrim_Gui(gGraphicsPtr->ot, faerie);
         AddPrim(&gGraphicsPtr->ot[OT_SIZE - 1], &s_blackScreen_80123918);
         DrawSync(0);
         VSync(0);
         PutDrawEnv(&gGraphicsPtr->drawEnv);
         PutDispEnv(&gGraphicsPtr->dispEnv);
         DrawOTag(gGraphicsPtr->ot);
         ContinueLoadingCdFile();
      } while (GetCdFileLoadStatus() != 0);

      faerie->functionIndex = EVTF_NULL;
      logo->functionIndex = EVTF_NULL;
      nowLoading->functionIndex = EVTF_NULL;
   }
}

void PrepareToBeginLoadingVabBody(s32 cdfIdx, s32 maxSectors) {
   gCdLoader.cdf = cdfIdx;
   gCdLoader.loadingVabBody = 1;
   gCdLoader.state = 0;
   gCdLoader.stalledCounter = 0;
   gCdLoader.sector = gCdFiles[cdfIdx].startingSector;
   gCdLoader.sectorsRead = 0;
   gCdLoader.sectorsToRead = MIN(gCdFiles[cdfIdx].sectorCt, maxSectors);
}

void PrepareToResumeLoadingVabBody(void) {
   s32 remaining;

   gCdLoader.state = 0;
   gCdLoader.sector += gCdLoader.sectorsToRead;
   remaining = gCdFiles[gCdLoader.cdf].sectorCt - gCdLoader.sectorsRead;
   gCdLoader.sectorsToRead = MIN(remaining, gCdLoader.sectorsToRead);
}

s32 s_waitTimer_80123258;

void ContinueLoadingCdFile(void) {
   s32 result;
   s32 originalState = gCdLoader.state;

   switch (gCdLoader.state) {
   case 0:
      CdIntToPos(gCdLoader.sector, &gCdLoader.location);
      gCdLoader.state++;

   // fallthrough
   case 1:
      if (CdControl(CdlSetloc, &gCdLoader.location, NULL) == 0) {
         break;
      }

      gCdLoader.state++;

   // fallthrough
   case 2:
      s_cdMode_80123270 = CdlModeSpeed;
      if (CdControl(CdlSetmode, &s_cdMode_80123270, NULL) == 0) {
         break;
      }

      gCdLoader.state++;
      break;

   case 3:
      if (CdSync(1, NULL) != CdlComplete) {
         break;
      }

      s_waitTimer_80123258 = 0;
      gCdLoader.state++;
      break;

   case 4:
      if (++s_waitTimer_80123258 < 3) {
         break;
      }

      gCdLoader.state++;

   // fallthrough
   case 5:
      CdRead(gCdLoader.sectorsToRead, gCdFiles[gCdLoader.cdf].bufferPtr, 0);
      gCdLoader.state++;

   // fallthrough
   case 6:
      result = CdReadSync(1, NULL);
      if (result > 0) {
         // Still some sectors remaining
         break;
      }
      if (result < 0) {
         // -1: Error occurred
         CdControl(CdlReset, NULL, NULL);
         gCdLoader.state = 1;
         break;
      }

      // 0: Completed
      gCdLoader.state++;

   // fallthrough
   case 7:
      if (gCdLoader.loadingVabBody) {
         gCdLoader.sectorsRead += gCdLoader.sectorsToRead;

         if (gCdLoader.sectorsRead >= gCdFiles[gCdLoader.cdf].sectorCt) {
            gCdLoader.state = 99;
         } else {
            gCdLoader.state = 98;
         }
      } else {
         gCdLoader.state = 99;
      }
      break;

   case 8:
      break;

   case 99:
      break;
   }

   if (gCdLoader.state < 6 && gCdLoader.state == originalState &&
       (++gCdLoader.stalledCounter > 60)) {
      CdControlB(CdlNop, NULL, NULL);
      CdControlB(CdlReset, NULL, NULL);
      gCdLoader.state = 1;
      gCdLoader.stalledCounter = 0;
   }
   ContinueLoadingVab();
   ContinueLoadingSeq();
}

s32 GetCdFileLoadStatus(void) {
   if (gCdLoader.state == 99) {
      return 0;
   }
   if (gCdLoader.state == 98) {
      return 2;
   }
   return 1;
}

s32 LoadSoundSet(u32 setIdx) {
   static s16 prevSetIdx = -1;

   if (setIdx > 12) {
      return 0;
   }

   if (gSoundSets[setIdx].vabId == 2 && prevSetIdx == setIdx) {
      return 0;
   }

   prevSetIdx = setIdx;
   gVabLoader.state = 0;
   gVabLoader.vabId = gSoundSets[setIdx].vabId;
   gVabLoader.headerCdf = gSoundSets[setIdx].cdfVabHeader;
   gVabLoader.bodyCdf = gSoundSets[setIdx].cdfVabBody;
   SsVabClose(gVabLoader.vabId);
   gCdFiles[gVabLoader.headerCdf].bufferPtr = gSoundSets[setIdx].bufferPtr;
   LoadCdFile(gVabLoader.headerCdf, 1);

   return 1;
}

void ContinueLoadingVab(void) {
   switch (gVabLoader.state) {
   case 0:
      if (GetCdFileLoadStatus() != 0) {
         return;
      }
      gVabLoader.state++;

   // fallthrough
   case 1:
      SsVabOpenHeadSticky(gCdFiles[gVabLoader.headerCdf].bufferPtr, gVabLoader.vabId,
                          gVabSoundBufferAddr[gVabLoader.vabId]);
      gVabLoader.state++;

   // fallthrough
   case 2:
      PrepareToBeginLoadingVabBody(gVabLoader.bodyCdf, 90);
      gVabLoader.state++;

   // fallthrough
   case 3:
      if (GetCdFileLoadStatus() == 1) {
         return;
      }

      gVabLoader.bodyTransferResult =
          SsVabTransBodyPartly(gCdFiles[gVabLoader.bodyCdf].bufferPtr, 90 * 2048, gVabLoader.vabId);
      gVabLoader.state++;

   // fallthrough
   case 4:
      if (SsVabTransCompleted(0) != 1) {
         return;
      }
      gVabLoader.state++;

   // fallthrough
   case 5:
      if (gVabLoader.bodyTransferResult == -2) {
         // -2: Incomplete
         PrepareToResumeLoadingVabBody();
         gVabLoader.state = 3;
      } else if (gVabLoader.bodyTransferResult == -1) {
         //-1: Transfer failed
         gVabLoader.state = 3;
      } else {
         gVabLoader.state = 99;
      }
      break;

   case 99:
      break;
   }
}

void FinishLoadingVab(void) {
   while (gVabLoader.state != 99) {
      ContinueLoadingCdFile();
   }
}

void LoadSeqSet(u32 setIdx) {
   static s16 prevSetIdx = -1;

   if (setIdx < 38 && prevSetIdx != setIdx) {
      PerformAudioCommand(AUDIO_CMD_STOP_SEQ);
      SetSeqDataPtr(&gSeqData);
      prevSetIdx = setIdx;
      gSeqLoader.state = 0;
      gSeqLoader.setIdx = setIdx;
      gSeqLoader.cdf = gSeqSets[setIdx].cdf;
      gCdFiles[gSeqLoader.cdf].bufferPtr = gSeqSets[setIdx].bufferPtr;
      LoadCdFile(gSeqLoader.cdf, 0);
   }
}

void ContinueLoadingSeq(void) {
   switch (gSeqLoader.state) {
   case 0:
      if (GetCdFileLoadStatus() != 0) {
         return;
      }
      gSeqLoader.state++;
   // fallthrough
   case 1:
      SetCurrentSeqSet(gSeqLoader.setIdx);
      gSeqLoader.state = 99;
   // fallthrough
   case 99:
      break;
   }
}

void FinishLoadingSeq(void) {
   while (gSeqLoader.state != 99) {
      ContinueLoadingCdFile();
   }
}

CdlLOC s_movieLocation_8012325c;

s32 PlayMovie(s32 sector, s32 frameCt, s32 is24bit) {
   extern void Movie_StrCallback(void);

   CdIntToPos(sector, &s_movieLocation_8012325c);
   s_frameNum_80123264 = 0;
   s_totalFrames_80123268 = frameCt;
   gMovieDecoder.mode = is24bit ? 3 : 2;

   Movie_InitDecoder();
   // Note: Requires tweaking fixupTextRelocs to also scan for HI16/LO16 (for function ptr)
   Movie_Init(&s_movieLocation_8012325c, 0xffffffff, Movie_StrCallback, 0);
   Movie_DecodeNextFrame();
   return 0;
}

void Movie_Reset(s32 frameCt) {
   //?
   s_frameNum_80123264 = 0;
   s_totalFrames_80123268 = frameCt;
   s_movieFinished_8012326c = 0;
}

s32 Movie_PlayNextFrame(void) {
   DecDCTin(gMovieDecoder.vlcBufferPtrs[gMovieDecoder.vlcBufferIdx], gMovieDecoder.mode);
   DecDCTout(gMovieDecoder.imgBufferPtr, gMovieDecoder.slice.w * gMovieDecoder.slice.h / 2);

   if (Movie_DecodeNextFrame() == 0) {
      Movie_SyncFrame();
      return s_movieFinished_8012326c;
   }

   return 1;
}

void Movie_FinishNB(void) {
   DecDCToutCallback(NULL);
   StUnSetRing();
   sMovieSectorHeader = NULL;
}

void Movie_Finish(void) {
   u8 unused[8];

   DecDCToutCallback(NULL);
   StUnSetRing();
   while (CdControlB(CdlPause, NULL, NULL) == 0) {
      // Nothing
   }
   sMovieSectorHeader = NULL;
}

u32 Movie_GetFrameNum(void) {
   if (sMovieSectorHeader == NULL) {
      return 0;
   } else {
      return sMovieSectorHeader->frameCount;
   }
}

void Movie_InitDecoder(void) {
   static void *vlcBufAddr1 = &gScratch3_80180210;
   static void *vlcBufAddr2 = (void *)0x801a24f0; //&gScratch3_80180210[140000]; //FIXME (reloc)
   static void *imgBufAddr = (void *)0x801c47d0;  //&gScratch3_80180210[280000]; //FIXME (reloc)

   gMovieDecoder.vlcBufferIdx = 0;
   gMovieDecoder.bufferRectIdx = (gGraphicsPtr != &gGraphicBuffers[0]);
   gMovieDecoder.frameFinished = 0;
   gMovieDecoder.vlcBufferPtrs[0] = vlcBufAddr1;
   gMovieDecoder.vlcBufferPtrs[1] = vlcBufAddr2;
   gMovieDecoder.imgBufferPtr = imgBufAddr;
   gMovieDecoder.bufferRects[0].x = 0;
   gMovieDecoder.bufferRects[0].y = 16;
   gMovieDecoder.bufferRects[0].w = SCREEN_WIDTH * gMovieDecoder.mode / 2;
   gMovieDecoder.bufferRects[0].h = SCREEN_HEIGHT;
   gMovieDecoder.bufferRects[1].x = 0;
   gMovieDecoder.bufferRects[1].y = 272;
   gMovieDecoder.bufferRects[1].w = SCREEN_WIDTH * gMovieDecoder.mode / 2;
   gMovieDecoder.bufferRects[1].h = SCREEN_HEIGHT;
   gMovieDecoder.slice.x = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].x;
   gMovieDecoder.slice.y = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].y;
   gMovieDecoder.slice.w = 16 * gMovieDecoder.mode / 2;
   gMovieDecoder.slice.h = SCREEN_HEIGHT;
}

void Movie_Init(CdlLOC *location, u32 endFrame, void (*strCallback)(), void (*endCallback)()) {
   static void *ringBufAddr = (void *)0x801c74d0; //&gScratch3_80180210[291520]; //FIXME (reloc)

   DecDCTReset(0);
   s_movieFinished_8012326c = 0;
   DecDCToutCallback(strCallback);
   StSetRing(ringBufAddr, 0x20);
   StSetStream(gMovieDecoder.mode != 2, 1, endFrame, NULL, endCallback);
   Movie_Start(location);
}

void Movie_StrCallback(void) {
   extern u32 StCdIntrFlag;
   if (gMovieDecoder.mode == 3 && StCdIntrFlag != 0) {
      StCdInterrupt();
      StCdIntrFlag = 0;
   }
   LoadImage(&gMovieDecoder.slice, gMovieDecoder.imgBufferPtr);
   gMovieDecoder.slice.x += gMovieDecoder.slice.w;
   if (gMovieDecoder.slice.x < (gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].x +
                                gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].w)) {
      DecDCTout(gMovieDecoder.imgBufferPtr, gMovieDecoder.slice.w * gMovieDecoder.slice.h / 2);
   } else {
      gMovieDecoder.frameFinished = 1;
      gMovieDecoder.bufferRectIdx = !gMovieDecoder.bufferRectIdx;
      gMovieDecoder.slice.x = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].x;
      gMovieDecoder.slice.y = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].y;
   }
}

s32 Movie_DecodeNextFrame(void) {
   u32 *nextFrame;
   s32 tries = 1;

   while ((nextFrame = Movie_GetNextFrame()) == 0) {
      if (--tries == 0) {
         return -1;
      }
   }

   gMovieDecoder.vlcBufferIdx = !gMovieDecoder.vlcBufferIdx;
   DecDCTvlc(nextFrame, gMovieDecoder.vlcBufferPtrs[gMovieDecoder.vlcBufferIdx]);
   StFreeRing(nextFrame);
   return 0;
}

u32 *Movie_GetNextFrame(void) {
   u32 *addr;
   s32 tries = 0x100000;

   if (CdSync(1, NULL) & 0x10) {
      //? What flag is 0x10? Does CdSync report shell open?
      return NULL;
   }

   while (StGetNext(&addr, &sMovieSectorHeader) != 0) {
      if (--tries == 0) {
         return NULL;
      }
   }

   //? Can't make sense of this.
   if (addr[0] != sMovieSectorHeader->dummy1 || addr[1] != sMovieSectorHeader->dummy2) {
      StFreeRing(addr);
      return NULL;
   }

   if (sMovieSectorHeader->frameCount >= s_totalFrames_80123268) {
      s_movieFinished_8012326c = 1;
   }

   if (sMovieSectorHeader->frameCount < s_frameNum_80123264) {
      s_movieFinished_8012326c = 1;
   } else {
      s_frameNum_80123264 = sMovieSectorHeader->frameCount;
   }

   return addr;
}

void Movie_SyncFrame(void) {
   s32 tries = WAIT_TIME;

   while (!gMovieDecoder.frameFinished) {
      if (--tries == 0) {
         gMovieDecoder.frameFinished = 1;
         gMovieDecoder.bufferRectIdx = !gMovieDecoder.bufferRectIdx;
         gMovieDecoder.slice.x = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].x;
         gMovieDecoder.slice.y = gMovieDecoder.bufferRects[gMovieDecoder.bufferRectIdx].y;
      }
   }

   gMovieDecoder.frameFinished = 0;
}

s32 s_waitTimer_80123260;

void Movie_Start(CdlLOC *location) {
   s32 prevState;
   s32 result;
   s32 tries = 0;
   s32 state = 0;

   while (1) {
      prevState = state;

      switch (state) {
      case 0:
         if (CdControl(CdlNop, NULL, NULL) == 0) {
            break;
         }
         state++;

      // fallthrough
      case 1:
         result = CdSync(1, NULL);
         if (result & 0x10) { //?
            return;
         }
         if (result != CdlComplete) {
            break;
         }
         state++;

      // fallthrough
      case 2:
         if (CdControl(CdlSeekL, location, NULL) == 0) {
            break;
         }
         state++;

      // fallthrough
      case 3:
         s_cdMode_80123270 = CdlModeSpeed;
         if (CdControl(CdlSetmode, &s_cdMode_80123270, NULL) != 0) {
            state++;
         }
         break;

      case 4:
         if (CdSync(1, NULL) == CdlComplete) {
            s_waitTimer_80123260 = 0;
            state++;
         }
         break;

      case 5:
         if (++s_waitTimer_80123260 < 3) {
            break;
         }
         state++;

      // fallthrough
      case 6:
         if (CdRead2(CdlModeStream | CdlModeSpeed | CdlModeRT) != 0) {
            // 1: Success
            return;
         }
      } // switch (state)

      if (state == prevState) {
         if (state == 4 || state == 5) {
            VSync(0);
         }
         if (++tries > 16) {
            return;
         }
      }
   }
}
