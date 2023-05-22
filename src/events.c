#include "common.h"
#include "evt.h"
#include "state.h"
#include "cd_files.h"
#include "graphics.h"

typedef struct EvtEntityProperties {
   u8 **altAnims;
   u8 **baseAnims;
   u8 stripIdxA, stripIdxB;
   u16 padding;
} EvtEntityProperties;

extern s32 gEvtTextFiles[95];
extern s32 gEvtDataFiles[95];
extern s32 gEvtSeqSetFiles[95];
extern EvtEntityProperties *gEvtEntities[95];
extern s16 *gEvtEntityDataPointers[100];
extern u8 *gAnimSet_80101fc0[56];
extern u8 *gAnimSet_80103f8c[8];
extern u8 *gAnimSet_80103fd0[8];
extern u8 *gAnimSet_80104034[16];
extern u8 *gAnimSet_801042a8[92];
extern u8 *gAnimSet_800f2db4[52];
extern u8 *gAnimSet_8010488c[112];
extern u8 *gAnimSet_80104f18[72];
extern u8 *gAnimSet_801053cc[64];
extern u8 *gAnimSet_80105b10[124];
extern u8 *gAnimSet_80105d00[50];
extern u8 *gAnimSet_801063f4[82];

void Evtf417_LoadEvent03(EvtData *);
void Evtf418_LoadEvent06(EvtData *);
void Evtf419_Noop(void);
void Evtf252_LoadEvent05(EvtData *);
void Evtf251_LoadEvent14(EvtData *);
void Evtf250_350_LoadEvent16(EvtData *);
void Evtf416_LoadEvent00(EvtData *);
void LoadEvent(s32);
void Evtf254_LoadEvent23(EvtData *);
void Evtf255_LoadEvent33(EvtData *);
void Evtf257_LoadEvent37(EvtData *);
void Evtf260_LoadEvent42(EvtData *);
void Evtf259_LoadEvent44(EvtData *);
void Evtf262_LoadEvent51(EvtData *);
void Evtf263_LoadEvent54(EvtData *);
void Evtf264_LoadEvent57(EvtData *);
void Noop_800b6da0(EvtData *);
void Noop_800b6da8(EvtData *);
void Evtf267_LoadEvent62(EvtData *);
void Evtf268_LoadEvent67(EvtData *);
void Evtf460_LoadEvent01(EvtData *);
void Evtf461_LoadEvent02(EvtData *);
void Evtf462_LoadEvent04(EvtData *);
void Evtf463_LoadEvent08(EvtData *);
void Evtf464_LoadEvent09(EvtData *);
void Evtf492_LoadEvent10(EvtData *);
void Evtf465_LoadEvent11(EvtData *);
void Evtf466_LoadEvent12(EvtData *);
void Evtf467_LoadEvent13(EvtData *);
void Evtf494_LoadEvent15(EvtData *);
void Evtf495_LoadEvent17(EvtData *);
void Evtf493_LoadEvent18(EvtData *);
void Evtf468_LoadEvent19(EvtData *);
void Evtf523_LoadEvent07(EvtData *);
void Evtf524_LoadEvent20(EvtData *);
void Evtf469_LoadEvent21(EvtData *);
void Evtf470_LoadEvent22(EvtData *);
void Evtf471_LoadEvent24(EvtData *);
void Evtf520_LoadEvent25(EvtData *);
void Evtf522_LoadEvent26(EvtData *);
void Evtf496_LoadEvent27(EvtData *);
void Evtf472_LoadEvent28(EvtData *);
void Evtf473_LoadEvent29(EvtData *);
void Evtf474_LoadEvent30(EvtData *);
void Evtf475_LoadEvent31(EvtData *);
void Evtf497_LoadEvent32(EvtData *);
void Evtf476_LoadEvent34(EvtData *);
void Evtf477_LoadEvent35(EvtData *);
void Evtf478_LoadEvent36(EvtData *);
void Evtf498_LoadEvent38(EvtData *);
void Evtf479_LoadEvent39(EvtData *);
void Evtf480_LoadEvent40(EvtData *);
void Evtf481_LoadEvent41(EvtData *);
void Evtf499_LoadEvent43(EvtData *);
void Evtf500_LoadEvent46(EvtData *);
void Evtf525_LoadEvent47(EvtData *);
void Evtf501_LoadEvent48(EvtData *);
void Evtf502_LoadEvent49(EvtData *);
void Evtf482_LoadEvent50(EvtData *);
void Evtf483_LoadEvent52(EvtData *);
void Evtf503_LoadEvent53(EvtData *);
void Evtf484_LoadEvent55(EvtData *);
void Evtf485_LoadEvent56(EvtData *);
void Evtf491_LoadEvent60(EvtData *);
void Evtf486_LoadEvent61(EvtData *);
void Evtf504_LoadEvent63(EvtData *);
void Evtf487_LoadEvent64(EvtData *);
void Evtf488_LoadEvent65(EvtData *);
void Evtf489_LoadEvent66(EvtData *);
void Evtf450_LoadEvent68(EvtData *);
void Evtf505_LoadEvent69(EvtData *);
void Evtf451_LoadEvent70(EvtData *);
void Evtf490_LoadEvent71(EvtData *);
void Evtf452_LoadEvent72(EvtData *);
void Evtf453_LoadEvent73(EvtData *);
void Evtf506_LoadEvent74(EvtData *);
void Evtf454_LoadEvent75(EvtData *);
void Evtf455_LoadEvent76(EvtData *);
void Evtf507_LoadEvent77(EvtData *);
void Evtf508_LoadEvent78(EvtData *);
void Evtf456_LoadEvent79(EvtData *);
void Evtf457_LoadEvent80(EvtData *);
void Evtf458_LoadEvent81(EvtData *);
void Evtf509_LoadEvent82(EvtData *);
void Evtf459_LoadEvent83(EvtData *);
void Evtf510_LoadEvent84(EvtData *);
void Evtf511_LoadEvent85(EvtData *);
void Evtf512_LoadEvent86(EvtData *);
void Evtf513_LoadEvent87(EvtData *);
void Evtf514_LoadEvent88(EvtData *);
void Evtf515_LoadEvent89(EvtData *);
void Evtf516_LoadEvent90(EvtData *);
void Evtf517_LoadEvent91(EvtData *);
void Evtf518_LoadEvent92(EvtData *);
void Evtf519_LoadEvent93(EvtData *);
void Evtf521_LoadEvent94(EvtData *);

void Evtf417_LoadEvent03(EvtData *evt) {
   LoadEvent(3);
   evt->functionIndex = EVTF_NULL;
}

void Evtf418_LoadEvent06(EvtData *evt) {
   LoadEvent(6);
   evt->functionIndex = EVTF_NULL;
}

void Evtf419_Noop(void) {}

void Evtf252_LoadEvent05(EvtData *evt) {
   LoadEvent(5);
   evt->functionIndex = EVTF_NULL;
}

void Evtf251_LoadEvent14(EvtData *evt) {
   LoadEvent(14);
   evt->functionIndex = EVTF_NULL;
}

void Evtf250_350_LoadEvent16(EvtData *evt) {
   LoadEvent(16);
   evt->functionIndex = EVTF_NULL;
}

void Evtf416_LoadEvent00(EvtData *evt) {
   LoadEvent(0);
   evt->functionIndex = EVTF_NULL;
}

void LoadEvent(s32 num) {
   s32 i, entityCt;
   EvtEntityProperties *p;

   gPlayerControlSuppressed = 1;
   gState.inEvent = 1;
   gState.eventResumeLocation = 0;

   if (gEvtTextFiles[num]) {
      LoadText(gEvtTextFiles[num], gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
   }

   p = gEvtEntities[num];
   LoadCdFile(gEvtDataFiles[num], 0);
   entityCt = LoadEventData();
   for (i = 0; i < entityCt; i++, p++) {
      SetupEventEntity(p->altAnims, p->baseAnims, gEvtEntityDataPointers[i], p->stripIdxA,
                       p->stripIdxB);
   }

   if (gEvtSeqSetFiles[num]) {
      LoadSeqSet(gEvtSeqSetFiles[num]);
      FinishLoadingSeq();
   }
}

void Evtf254_LoadEvent23(EvtData *evt) {
   LoadEvent(23);
   evt->functionIndex = EVTF_NULL;
}

void Evtf255_LoadEvent33(EvtData *evt) {
   LoadEvent(33);
   evt->functionIndex = EVTF_NULL;
}

void Evtf257_LoadEvent37(EvtData *evt) {
   LoadEvent(37);
   evt->functionIndex = EVTF_NULL;
}

void Evtf260_LoadEvent42(EvtData *evt) {
   LoadEvent(42);
   evt->functionIndex = EVTF_NULL;
}

void Evtf259_LoadEvent44(EvtData *evt) {
   LoadEvent(44);
   evt->functionIndex = EVTF_NULL;
}

void Evtf262_LoadEvent51(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      LoadText(CDF_SIBAIB_DAT, gText, gTextPointers);
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      gState.currentTextPointers = gTextPointers;
      LoadCdFile(CDF_EVDATA51_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[0], 4, 5);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[1], 6, 6);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[2], 0, 0);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[4], 2, 2);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[5], 1, 1);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[6], 7, 8);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[7], 9, 6);
      LoadSeqSet(34);
      FinishLoadingSeq();
      evt->state++;
   }
}

void Evtf263_LoadEvent54(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      LoadText(CDF_SIBAIC_DAT, gText, gTextPointers);
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      gState.currentTextPointers = gTextPointers;
      LoadCdFile(CDF_EVDATA54_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[0], 0, 1);
      SetupEventEntity(gAnimSet_80103f8c, gAnimSet_80101fc0, gEvtEntityDataPointers[1], 2, 3);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[2], 4, 5);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[3], 6, 6);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[4], 6, 6);
      LoadSeqSet(28);
      FinishLoadingSeq();
      evt->state++;
   }
}

void Evtf264_LoadEvent57(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      LoadText(CDF_SIBAID_DAT, gText, gTextPointers);
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      gState.currentTextPointers = gTextPointers;
      LoadCdFile(CDF_EVDATA57_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[0], 0, 1);
      SetupEventEntity(gAnimSet_80103fd0, gAnimSet_80101fc0, gEvtEntityDataPointers[1], 2, 3);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[2], 4, 5);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[3], 6, 7);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[4], 19, 19);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[5], 18, 18);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[6], 17, 17);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[7], 16, 16);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[8], 15, 15);
      LoadSeqSet(17);
      FinishLoadingSeq();
      evt->state++;
   }
}

void Noop_800b6da0(EvtData *evt) { evt->functionIndex = EVTF_NULL; }

void Noop_800b6da8(EvtData *evt) { evt->functionIndex = EVTF_NULL; }

void Evtf267_LoadEvent62(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      LoadText(CDF_SIBAIG_DAT, gText, gTextPointers);
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      gState.currentTextPointers = gTextPointers;
      LoadCdFile(CDF_EVDATA62_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80104034, gAnimSet_80101fc0, gEvtEntityDataPointers[0], 13, 13);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[1], 2, 3);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[2], 18, 0);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[3], 16, 17);
      evt->state++;
      LoadSeqSet(27);
      FinishLoadingSeq();
   }
}

void Evtf268_LoadEvent67(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      LoadText(CDF_SIBAIH_DAT, gText, gTextPointers);
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      gState.currentTextPointers = gTextPointers;
      LoadCdFile(CDF_EVDATA67_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[0], 12, 12);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[1], 6, 6);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[2], 7, 7);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[3], 0, 13);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[4], 9, 9);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[5], 1, 1);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[6], 8, 8);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[7], 3, 3);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[8], 4, 4);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[9], 10, 10);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[10], 5, 5);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[11], 2, 2);
      SetupEventEntity(gAnimSet_80101fc0, gAnimSet_80101fc0, gEvtEntityDataPointers[12], 0, 0);
      LoadSeqSet(27);
      FinishLoadingSeq();
      evt->state++;
   }
}

void Evtf460_LoadEvent01(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT01_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(7);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA01_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 18);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 17);
      SetupEventEntity(gUnitAnimSets[82], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[101], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 5, 5);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 4, 4);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 4, 4);
      SetupEventEntity(gUnitAnimSets[101], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 5, 5);
      SetupEventEntity(gUnitAnimSets[101], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 5, 5);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 4, 4);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 13, 13);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 12, 12);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 11, 11);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[13], 0, 0);
      evt->state++;
   }
}

void Evtf461_LoadEvent02(EvtData *evt) {
   LoadEvent(2);
   evt->functionIndex = EVTF_NULL;
}

void Evtf462_LoadEvent04(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT04_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(23);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA04_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 18, 18);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 17, 17);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 15, 16);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 14, 14);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 12, 12);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 12, 12);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[74], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 3, 3);
      SetupEventEntity(gUnitAnimSets[74], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 3, 3);
      SetupEventEntity(gUnitAnimSets[112], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 4, 4);
      SetupEventEntity(gUnitAnimSets[112], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 4, 4);
      SetupEventEntity(gUnitAnimSets[112], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 4, 4);
      SetupEventEntity(gUnitAnimSets[112], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 4, 4);
      evt->state++;
   }
}

void Evtf463_LoadEvent08(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT08_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(3);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA08_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 17, 17);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 15, 16);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 14, 14);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 3, 3);
      SetupEventEntity(gUnitAnimSets[113], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 3, 3);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[13], 0, 0);
      evt->state++;
   }
}

void Evtf464_LoadEvent09(EvtData *evt) {
   LoadEvent(9);
   evt->functionIndex = EVTF_NULL;
}

void Evtf492_LoadEvent10(EvtData *evt) {
   LoadEvent(10);
   evt->functionIndex = EVTF_NULL;
}

void Evtf465_LoadEvent11(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT11_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(22);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA11_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 18, 18);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 17, 17);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 14, 14);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 16, 16);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 6, 6);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 6, 6);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 6, 6);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 6, 6);
      SetupEventEntity(gUnitAnimSets[100], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 6, 6);
      SetupEventEntity(gUnitAnimSets[101], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 7, 7);
      SetupEventEntity(gUnitAnimSets[101], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 7, 7);
      SetupEventEntity(gUnitAnimSets[114], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 8, 8);
      SetupEventEntity(gUnitAnimSets[114], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 8, 8);
      SetupEventEntity(gUnitAnimSets[114], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 8, 8);
      SetupEventEntity(gUnitAnimSets[114], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 8, 8);
      SetupEventEntity(gUnitAnimSets[114], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 0, 0);
      evt->state++;
   }
}

void Evtf466_LoadEvent12(EvtData *evt) {
   LoadEvent(12);
   evt->functionIndex = EVTF_NULL;
}

void Evtf467_LoadEvent13(EvtData *evt) {
   LoadEvent(13);
   evt->functionIndex = EVTF_NULL;
}

void Evtf494_LoadEvent15(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT15_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(9);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA15_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 18, 18);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 17, 17);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 16, 16);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 15, 15);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 14, 14);
      SetupEventEntity(gUnitAnimSets[115], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 7, 7);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 7, 7);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 7, 7);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 7, 7);
      SetupEventEntity(gUnitAnimSets[116], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 7, 7);
      evt->state++;
   }
}

void Evtf495_LoadEvent17(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT17_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(5);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA17_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 17, 17);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 16, 16);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 15, 15);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 14, 14);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 13, 13);
      SetupEventEntity(gAnimSet_801042a8, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 12, 12);
      SetupEventEntity(gUnitAnimSets[105], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 8, 8);
      SetupEventEntity(gUnitAnimSets[103], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 8, 8);
      SetupEventEntity(gUnitAnimSets[102], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 9, 9);
      SetupEventEntity(gUnitAnimSets[102], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 9, 9);
      SetupEventEntity(gUnitAnimSets[102], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 9, 9);
      SetupEventEntity(gUnitAnimSets[102], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 9, 9);
      evt->state++;
   }
}

void Evtf493_LoadEvent18(EvtData *evt) {
   LoadEvent(18);
   evt->functionIndex = EVTF_NULL;
}

void Evtf468_LoadEvent19(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT19_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(36);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA19_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 17, 18);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 16, 16);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 15, 15);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 14, 14);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 13, 13);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 12, 12);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 7, 7);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 7, 7);
      SetupEventEntity(gUnitAnimSets[117], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 8, 8);
      SetupEventEntity(gUnitAnimSets[117], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 8, 8);
      SetupEventEntity(gUnitAnimSets[117], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 8, 8);
      SetupEventEntity(gUnitAnimSets[118], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 9, 9);
      SetupEventEntity(gUnitAnimSets[118], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 9, 9);
      SetupEventEntity(gUnitAnimSets[118], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 9, 9);
      SetupEventEntity(gUnitAnimSets[119], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 10, 10);
      SetupEventEntity(gUnitAnimSets[119], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 10, 10);
      SetupEventEntity(gUnitAnimSets[119], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 10, 10);
      evt->state++;
   }
}

void Evtf523_LoadEvent07(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT07_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(36);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA07_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 8);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 9, 10);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 11, 12);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 7, 8);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 9, 10);
      evt->state++;
   }
}

void Evtf524_LoadEvent20(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT20_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(32);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA20_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[107], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 13, 13);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 13, 13);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 13, 13);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[16], 0, 0);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[17], 7, 7);
      evt->state++;
   }
}

void Evtf469_LoadEvent21(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT21_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(22);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA21_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 18, 19);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 16, 17);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 14, 15);
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 6, 6);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 10, 10);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 11, 11);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 11, 11);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 11, 11);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 11, 11);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 11, 11);
      evt->state++;
   }
}

void Evtf470_LoadEvent22(EvtData *evt) {
   LoadEvent(22);
   evt->functionIndex = EVTF_NULL;
}

void Evtf471_LoadEvent24(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT24_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(36);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA24_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 18);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 16);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 10, 10);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 11, 11);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 12, 12);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 12, 12);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 12, 12);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 13, 13);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 13, 13);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 13, 13);
      evt->state++;
   }
}

void Evtf520_LoadEvent25(EvtData *evt) {
   LoadEvent(25);
   evt->functionIndex = EVTF_NULL;
}

void Evtf522_LoadEvent26(EvtData *evt) {
   LoadEvent(26);
   evt->functionIndex = EVTF_NULL;
}

void Evtf496_LoadEvent27(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT27_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(3);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA27_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 10, 10);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 10, 10);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 11, 11);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 12, 12);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 12, 12);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 13, 13);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 13, 13);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 13, 13);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 14, 14);
      SetupEventEntity(gUnitAnimSets[77], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 14, 14);
      evt->state++;
   }
}

void Evtf472_LoadEvent28(EvtData *evt) {
   LoadEvent(28);
   evt->functionIndex = EVTF_NULL;
}

void Evtf473_LoadEvent29(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT29_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(25);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA29_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 3);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 18, 19);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gAnimSet_8010488c, gUnitAnimSets[107], gEvtEntityDataPointers[10], 10, 17);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 11, 11);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 11, 11);
      SetupEventEntity(gUnitAnimSets[65], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 11, 11);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 12, 12);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 12, 12);
      SetupEventEntity(gUnitAnimSets[121], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 12, 12);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 13, 13);
      SetupEventEntity(gUnitAnimSets[122], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 13, 13);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 14, 14);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 14, 14);
      SetupEventEntity(gUnitAnimSets[123], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 14, 14);
      evt->state++;
   }
}

void Evtf474_LoadEvent30(EvtData *evt) {
   LoadEvent(30);
   evt->functionIndex = EVTF_NULL;
}

void Evtf475_LoadEvent31(EvtData *evt) {
   LoadEvent(31);
   evt->functionIndex = EVTF_NULL;
}

void Evtf497_LoadEvent32(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT32_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(10);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA32_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 9, 9);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 3, 3);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 3, 3);
      evt->state++;
   }
}

void Evtf476_LoadEvent34(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT34_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(33);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA34_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 10);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 12);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 13, 14);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 15, 16);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 17, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 17, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 17, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 17, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 17, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 17, 18);
      SetupEventEntity(gAnimSet_8010488c, gAnimSet_800f2db4, gEvtEntityDataPointers[19], 0, 0);
      evt->state++;
   }
}

void Evtf477_LoadEvent35(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT35_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(9);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA35_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 18, 19);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 16, 17);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 14, 15);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 13, 13);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 1, 1);
      evt->state++;
   }
}

void Evtf478_LoadEvent36(EvtData *evt) {
   LoadEvent(36);
   evt->functionIndex = EVTF_NULL;
}

void Evtf498_LoadEvent38(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT38_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(12);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA38_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 17, 17);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 15, 15);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 13, 13);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 1, 1);
      SetupEventEntity(gUnitAnimSets[73], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 1, 1);
      SetupEventEntity(gUnitAnimSets[93], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 2, 2);
      SetupEventEntity(gUnitAnimSets[93], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 2, 2);
      SetupEventEntity(gUnitAnimSets[93], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 2, 2);
      SetupEventEntity(gUnitAnimSets[93], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 2, 2);
      evt->state++;
   }
}

void Evtf479_LoadEvent39(EvtData *evt) {
   LoadEvent(39);
   evt->functionIndex = EVTF_NULL;
}
void Evtf480_LoadEvent40(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT40_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(22);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA40_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 2, 2);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 19, 19);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 14, 14);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 4, 4);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 18, 18);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 17, 17);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 16, 16);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 15, 15);
      SetupEventEntity(gUnitAnimSets[104], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 1, 1);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 3, 3);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 3, 3);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 3, 3);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 3, 3);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 3, 3);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 5, 5);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 5, 5);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 5, 5);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 8, 8);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 8, 8);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 8, 8);
      SetupEventEntity(gUnitAnimSets[79], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 11, 11);
      SetupEventEntity(gUnitAnimSets[78], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 12, 12);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[22], 0, 0);
      evt->state++;
   }
}

void Evtf481_LoadEvent41(EvtData *evt) {
   LoadEvent(41);
   evt->functionIndex = EVTF_NULL;
}

void Evtf499_LoadEvent43(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT43_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(8);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA43_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 2, 2);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 4, 4);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 7, 7);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 10, 10);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 1, 1);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 1, 1);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 1, 1);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 1, 1);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 3, 3);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 3, 3);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 5, 5);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 5, 5);
      SetupEventEntity(gUnitAnimSets[79], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 8, 8);
      SetupEventEntity(gUnitAnimSets[78], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 11, 11);
      evt->state++;
   }
}

void Evtf500_LoadEvent46(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT46_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(10);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA46_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 18, 18);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 17, 17);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 16, 16);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 15, 15);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 14, 14);
      SetupEventEntity(gAnimSet_80104f18, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 13, 13);
      SetupEventEntity(gUnitAnimSets[90], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 1, 1);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 3, 3);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 3, 3);
      SetupEventEntity(gUnitAnimSets[70], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 5, 5);
      SetupEventEntity(gUnitAnimSets[70], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 5, 5);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 8, 8);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 8, 8);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 8, 8);
      SetupEventEntity(gUnitAnimSets[120], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 8, 8);
      evt->state++;
   }
}

void Evtf525_LoadEvent47(EvtData *evt) {
   LoadEvent(47);
   evt->functionIndex = EVTF_NULL;
}

void Evtf501_LoadEvent48(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT48_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(13);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA48_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 1, 1);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 3, 3);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 8, 8);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 11, 11);
      evt->state++;
   }
}

void Evtf502_LoadEvent49(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT49_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(8);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA49_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 1, 1);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 3, 3);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 8, 8);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 11, 11);
      SetupEventEntity(gUnitAnimSets[106], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 5, 5);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 12, 12);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 13, 13);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 13, 13);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 13, 13);
      SetupEventEntity(gUnitAnimSets[94], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 14, 14);
      SetupEventEntity(gUnitAnimSets[94], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 14, 14);
      SetupEventEntity(gUnitAnimSets[79], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 15, 15);
      SetupEventEntity(gUnitAnimSets[79], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 15, 15);
      SetupEventEntity(gUnitAnimSets[78], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 16, 16);
      SetupEventEntity(gUnitAnimSets[78], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 16, 16);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 17, 17);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 17, 17);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 17, 17);
      evt->state++;
   }
}

void Evtf482_LoadEvent50(EvtData *evt) {
   LoadEvent(50);
   evt->functionIndex = EVTF_NULL;
}

void Evtf483_LoadEvent52(EvtData *evt) {
   LoadEvent(52);
   evt->functionIndex = EVTF_NULL;
}

void Evtf503_LoadEvent53(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT53_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(4);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA53_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 11);
      SetupEventEntity(gUnitAnimSets[89], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 5, 5);
      SetupEventEntity(gUnitAnimSets[91], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[108], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 13, 13);
      SetupEventEntity(gUnitAnimSets[61], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 15, 15);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 17, 17);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 17, 17);
      SetupEventEntity(gUnitAnimSets[68], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 18, 18);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 15, 15);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 15, 15);
      SetupEventEntity(gUnitAnimSets[68], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 18, 18);
      evt->state++;
   }
}

void Evtf484_LoadEvent55(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT55_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(29);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA55_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 6, 6);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 9, 9);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 19, 19);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 7, 7);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 11);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 18, 18);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 5, 5);
      SetupEventEntity(gUnitAnimSets[75], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 5, 5);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[29], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[30], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[31], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[32], 12, 12);
      SetupEventEntity(gUnitAnimSets[76], gAnimSet_800f2db4, gEvtEntityDataPointers[33], 12, 12);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[34], 0, 0);
      evt->state++;
   }
}

void Evtf485_LoadEvent56(EvtData *evt) {
   LoadEvent(56);
   evt->functionIndex = EVTF_NULL;
}

void Evtf491_LoadEvent60(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT60_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(12);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA60_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 7, 7);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 11);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 5, 5);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 12, 12);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 13, 13);
      evt->state++;
   }
}

void Evtf486_LoadEvent61(EvtData *evt) {
   LoadEvent(61);
   evt->functionIndex = EVTF_NULL;
}

void Evtf504_LoadEvent63(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT63_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(6);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA63_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 7, 7);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 11);
      SetupEventEntity(gUnitAnimSets[108], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 5, 5);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 12, 12);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 13, 13);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 13, 13);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 13, 13);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 13, 13);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 14, 14);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 14, 14);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 14, 14);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 14, 14);
      evt->state++;
   }
}

void Evtf487_LoadEvent64(EvtData *evt) {
   LoadEvent(64);
   evt->functionIndex = EVTF_NULL;
}

void Evtf488_LoadEvent65(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT65_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(26);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA65_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 19, 19);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 6, 6);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 10, 10);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 7, 7);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 11, 11);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 18, 18);
      SetupEventEntity(gAnimSet_801053cc, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 17, 17);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 5, 5);
      SetupEventEntity(gUnitAnimSets[95], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 5, 5);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 12, 12);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 12, 12);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 12, 12);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[29], 14, 14);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[30], 15, 15);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[31], 15, 15);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[32], 15, 15);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[33], 15, 15);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[34], 16, 16);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[35], 16, 16);
      evt->state++;
   }
}

void Evtf489_LoadEvent66(EvtData *evt) {
   LoadEvent(66);
   evt->functionIndex = EVTF_NULL;
}

void Evtf450_LoadEvent68(EvtData *evt) {
   LoadEvent(68);
   evt->functionIndex = EVTF_NULL;
}

void Evtf505_LoadEvent69(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT69_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(2);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA69_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 14, 14);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 15, 15);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 15, 15);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 16, 16);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 16, 16);
      evt->state++;
   }
}

void Evtf451_LoadEvent70(EvtData *evt) {
   LoadEvent(70);
   evt->functionIndex = EVTF_NULL;
}

void Evtf490_LoadEvent71(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT71_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(24);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA71_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 7, 7);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 6, 6);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 13);
      SetupEventEntity(gUnitAnimSets[61], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 14, 14);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[14], 15, 15);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[15], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[16], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[17], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[18], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[19], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[20], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[21], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[22], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[23], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[24], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[25], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[26], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[27], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[28], 19, 19);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[29], 19, 19);
      evt->state++;
   }
}

void Evtf452_LoadEvent72(EvtData *evt) {
   LoadEvent(72);
   evt->functionIndex = EVTF_NULL;
}

void Evtf453_LoadEvent73(EvtData *evt) {
   LoadEvent(73);
   evt->functionIndex = EVTF_NULL;
}

void Evtf506_LoadEvent74(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT74_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(10);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA74_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 18, 18);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[13], 19, 19);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 12, 12);
      SetupEventEntity(gUnitAnimSets[99], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 12, 12);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 13, 13);
      SetupEventEntity(gUnitAnimSets[63], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 13, 13);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 14, 14);
      SetupEventEntity(gUnitAnimSets[64], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 14, 14);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 15, 15);
      SetupEventEntity(gUnitAnimSets[80], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 15, 15);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 16, 16);
      SetupEventEntity(gUnitAnimSets[124], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[29], 18, 18);
      evt->state++;
   }
}

void Evtf454_LoadEvent75(EvtData *evt) {
   LoadEvent(75);
   evt->functionIndex = EVTF_NULL;
}

void Evtf455_LoadEvent76(EvtData *evt) {
   LoadEvent(76);
   evt->functionIndex = EVTF_NULL;
}

void Evtf507_LoadEvent77(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT77_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(13);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA77_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 18, 19);
      evt->state++;
   }
}

void Evtf508_LoadEvent78(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT78_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(8);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA78_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 13);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[13], 14, 14);
      SetupEventEntity(gUnitAnimSets[91], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 15, 15);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[15], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[16], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[17], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[18], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[19], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[20], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[21], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[22], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[23], 16, 16);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[24], 17, 17);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[25], 17, 17);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[26], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[27], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[28], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[29], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[30], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[31], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[32], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[33], 18, 18);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[34], 18, 18);
      evt->state++;
   }
}

void Evtf456_LoadEvent79(EvtData *evt) {
   LoadEvent(79);
   evt->functionIndex = EVTF_NULL;
}

void Evtf457_LoadEvent80(EvtData *evt) {
   LoadEvent(80);
   evt->functionIndex = EVTF_NULL;
}

void Evtf458_LoadEvent81(EvtData *evt) {
   LoadEvent(81);
   evt->functionIndex = EVTF_NULL;
}

void Evtf509_LoadEvent82(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT82_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(3);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA82_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_80105d00, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 19, 19);
      SetupEventEntity(gUnitAnimSets[126], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 12, 12);
      SetupEventEntity(gUnitAnimSets[126], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 12, 12);
      SetupEventEntity(gUnitAnimSets[126], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 12, 12);
      SetupEventEntity(gUnitAnimSets[126], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 12, 12);
      SetupEventEntity(gUnitAnimSets[127], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 13, 13);
      SetupEventEntity(gUnitAnimSets[127], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 13, 13);
      SetupEventEntity(gUnitAnimSets[127], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 13, 13);
      SetupEventEntity(gUnitAnimSets[127], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 13, 13);
      SetupEventEntity(gAnimSet_80105b10, gAnimSet_800f2db4, gEvtEntityDataPointers[21], 0, 0);
      evt->state++;
   }
}

void Evtf459_LoadEvent83(EvtData *evt) {
   LoadEvent(83);
   evt->functionIndex = EVTF_NULL;
}

void Evtf510_LoadEvent84(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT84_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(35);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA84_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[89], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 14, 14);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 14, 14);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 15, 15);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 15, 15);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 15, 15);
      SetupEventEntity(gUnitAnimSets[71], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 15, 15);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 16, 16);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 16, 16);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 16, 16);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 16, 16);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 16, 16);
      SetupEventEntity(gUnitAnimSets[69], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 16, 16);
      SetupEventEntity(gUnitAnimSets[67], gAnimSet_800f2db4, gEvtEntityDataPointers[29], 18, 18);
      SetupEventEntity(gUnitAnimSets[67], gAnimSet_800f2db4, gEvtEntityDataPointers[30], 18, 18);
      SetupEventEntity(gUnitAnimSets[67], gAnimSet_800f2db4, gEvtEntityDataPointers[31], 18, 18);
      SetupEventEntity(gUnitAnimSets[72], gAnimSet_800f2db4, gEvtEntityDataPointers[32], 19, 19);
      SetupEventEntity(gUnitAnimSets[72], gAnimSet_800f2db4, gEvtEntityDataPointers[33], 19, 19);
      SetupEventEntity(gUnitAnimSets[72], gAnimSet_800f2db4, gEvtEntityDataPointers[34], 19, 19);
      evt->state++;
   }
}

void Evtf511_LoadEvent85(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT85_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(9);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA85_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 19, 19);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 0, 0);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 12, 12);
      SetupEventEntity(gUnitAnimSets[110], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 12, 12);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 13, 13);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 13, 13);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 13, 13);
      SetupEventEntity(gUnitAnimSets[111], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 13, 13);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 14, 14);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 14, 14);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 14, 14);
      SetupEventEntity(gUnitAnimSets[62], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 14, 14);
      evt->state++;
   }
}

void Evtf512_LoadEvent86(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT86_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(32);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA86_DAT, 0);
      LoadEventData();
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 1);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[1], 2, 2);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[2], 3, 3);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[3], 4, 5);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[4], 6, 6);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[5], 7, 7);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[6], 8, 8);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[7], 9, 9);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[8], 10, 10);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[9], 11, 11);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[10], 12, 12);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[11], 13, 13);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[12], 14, 15);
      SetupEventEntity(gAnimSet_801063f4, gAnimSet_800f2db4, gEvtEntityDataPointers[13], 16, 16);
      evt->state++;
   }
}

void Evtf513_LoadEvent87(EvtData *evt) {
   LoadEvent(87);
   evt->functionIndex = EVTF_NULL;
}

void Evtf514_LoadEvent88(EvtData *evt) {
   LoadEvent(88);
   evt->functionIndex = EVTF_NULL;
}

void Evtf515_LoadEvent89(EvtData *evt) {
   LoadEvent(89);
   evt->functionIndex = EVTF_NULL;
}

void Evtf516_LoadEvent90(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT90_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(29);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA90_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[98], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 19, 19);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 19, 19);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 19, 19);
      SetupEventEntity(gUnitAnimSets[81], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 19, 19);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 13, 13);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 13, 13);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 13, 13);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 13, 13);
      SetupEventEntity(gUnitAnimSets[129], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 14, 14);
      SetupEventEntity(gUnitAnimSets[129], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 14, 14);
      SetupEventEntity(gUnitAnimSets[128], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 15, 15);
      SetupEventEntity(gUnitAnimSets[128], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 15, 15);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 16, 16);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 16, 16);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[27], 16, 16);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[28], 16, 16);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[29], 17, 17);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[30], 17, 17);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[31], 17, 17);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[32], 17, 17);
      evt->state++;
   }
}

void Evtf517_LoadEvent91(EvtData *evt) {
   LoadEvent(91);
   evt->functionIndex = EVTF_NULL;
}

void Evtf518_LoadEvent92(EvtData *evt) {
   if (evt->state == 0) {
      gPlayerControlSuppressed = 1;
      gState.inEvent = 1;
      gState.eventResumeLocation = 0;
      LoadText(CDF_EVENT92_DAT, gText, gTextPointers);
      gState.currentTextPointers = gTextPointers;
      LoadSeqSet(27);
      FinishLoadingSeq();
      LoadCdFile(CDF_EVDATA92_DAT, 0);
      LoadEventData();
      SetupEventEntity(gUnitAnimSets[1], gAnimSet_800f2db4, gEvtEntityDataPointers[0], 0, 0);
      SetupEventEntity(gUnitAnimSets[2], gAnimSet_800f2db4, gEvtEntityDataPointers[1], 1, 1);
      SetupEventEntity(gUnitAnimSets[3], gAnimSet_800f2db4, gEvtEntityDataPointers[2], 2, 2);
      SetupEventEntity(gUnitAnimSets[4], gAnimSet_800f2db4, gEvtEntityDataPointers[3], 3, 3);
      SetupEventEntity(gUnitAnimSets[5], gAnimSet_800f2db4, gEvtEntityDataPointers[4], 4, 4);
      SetupEventEntity(gUnitAnimSets[6], gAnimSet_800f2db4, gEvtEntityDataPointers[5], 5, 5);
      SetupEventEntity(gUnitAnimSets[7], gAnimSet_800f2db4, gEvtEntityDataPointers[6], 6, 6);
      SetupEventEntity(gUnitAnimSets[8], gAnimSet_800f2db4, gEvtEntityDataPointers[7], 7, 7);
      SetupEventEntity(gUnitAnimSets[9], gAnimSet_800f2db4, gEvtEntityDataPointers[8], 8, 8);
      SetupEventEntity(gUnitAnimSets[10], gAnimSet_800f2db4, gEvtEntityDataPointers[9], 9, 9);
      SetupEventEntity(gUnitAnimSets[11], gAnimSet_800f2db4, gEvtEntityDataPointers[10], 10, 10);
      SetupEventEntity(gUnitAnimSets[12], gAnimSet_800f2db4, gEvtEntityDataPointers[11], 11, 11);
      SetupEventEntity(gUnitAnimSets[97], gAnimSet_800f2db4, gEvtEntityDataPointers[12], 12, 12);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[13], 14, 14);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[14], 14, 14);
      SetupEventEntity(gUnitAnimSets[92], gAnimSet_800f2db4, gEvtEntityDataPointers[15], 14, 14);
      SetupEventEntity(gUnitAnimSets[129], gAnimSet_800f2db4, gEvtEntityDataPointers[16], 15, 15);
      SetupEventEntity(gUnitAnimSets[129], gAnimSet_800f2db4, gEvtEntityDataPointers[17], 15, 15);
      SetupEventEntity(gUnitAnimSets[129], gAnimSet_800f2db4, gEvtEntityDataPointers[18], 15, 15);
      SetupEventEntity(gUnitAnimSets[128], gAnimSet_800f2db4, gEvtEntityDataPointers[19], 16, 16);
      SetupEventEntity(gUnitAnimSets[128], gAnimSet_800f2db4, gEvtEntityDataPointers[20], 16, 16);
      SetupEventEntity(gUnitAnimSets[128], gAnimSet_800f2db4, gEvtEntityDataPointers[21], 16, 16);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[22], 17, 17);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[23], 17, 17);
      SetupEventEntity(gUnitAnimSets[130], gAnimSet_800f2db4, gEvtEntityDataPointers[24], 17, 17);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[25], 18, 18);
      SetupEventEntity(gUnitAnimSets[96], gAnimSet_800f2db4, gEvtEntityDataPointers[26], 18, 18);
      evt->state++;
   }
}

void Evtf519_LoadEvent93(EvtData *evt) {
   LoadEvent(93);
   evt->functionIndex = EVTF_NULL;
}

void Evtf521_LoadEvent94(EvtData *evt) {
   gState.textSpeed = 1;
   LoadEvent(94);
   evt->functionIndex = EVTF_NULL;
}
