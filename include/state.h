#ifndef STATE_H
#define STATE_H

#include "common.h"
#include "evt.h"

typedef struct State {
   u8 worldMapState;
   u8 townState;
   u8 scene;
   u8 field_0x3;
   u8 D_80140530;
   u8 chapterOutro;
   u8 movieIdxToPlay;
   u8 worldMapDestination;
   u8 prevState;
   u8 saveLocation;
   u8 field_0xa;
   u8 field_0xb;
   s32 frameCounter;
   u8 chapter;
   u8 section;
   u8 D_8014053E;
   u8 field_0x13;
   DynamicIcon dynamicIcons[7];
   BigInt experience;
   u8 expScalingLevel;
   u8 field_0x4f;
   s32 debug;
   s32 useDefaultUnits;
   s32 primary;
   s32 secondary;
   s32 state3;
   s32 state4;
   s32 state5;
   s32 state6;
   s32 state7;
   s32 state8;
   s32 D_801405A4;

   u32 battleEval;
   u8 needEval;

   // MapState still TBD, but these 20 bytes are saved to card file, so starting w/ this:
   union {
      struct {
         u8 field_0x0;
         u8 field_0x1;
         u8 field_0x2;
         u8 field_0x3;
         u8 field_0x4;
         u8 field_0x5;
         u8 field_0x6;
         u8 field_0x7;
         u8 field_0x8;
         u8 field_0x9;
         u8 field_0xa;
         u8 foundHiddenItem1;
         u8 foundHiddenItem2;
         u8 field_0xd;
         u8 field_0xe;
         u8 field_0xf;
         u8 field_0x10;
         u8 field_0x11;
         u8 field_0x12;
         u8 field_0x13;
      } n;
      u8 bytes[20];
   } mapState;

   u8 signal;
   u8 field_0x96;
   u8 preciseSprites;

   u8 field_0x98;
   u8 msgFinished;
   u8 searchX;
   u8 searchZ;

   u8 droppedItem;
   u8 field_0x9d;
   s16 turn;
   u8 field_0xa0;
   u8 field_0xa1;
   s16 field_0xa2;
   u8 battleNum;
   u8 fieldRenderingDisabled;
   u8 vsyncMode;
   u8 field_0xa7;
   s16 depot[150];
   s32 gold;
   u8 field_0x1d8;
   u8 msgTextWaitTimer[5];
   s16 portraitsToLoad[75];
   s16 currentPortraits[75];
   u8 compassDisabled;
   u8 field_0x30b;
   s16 eventResumeLocation;
   SVectorXYZ eventCameraPan;
   s16 field_0x314;
   s16 field_0x316;
   EvtData *field_0x318;
   u8 msgBoxFinished;
   u8 field_0x31d;
   u8 field_0x31e;
   u8 field_0x31f;
   s8 **currentTextPointers;
   u8 field_0x324;
   u8 zoom;
   s16 unitMarkerSpin;
   u8 lastSelectedUnit;
   u8 mono;
   u8 cameraMode;
   u8 mapCursorOutOfRange;
   u8 field_0x32c;
   u8 D_80140859;
   u8 vsyncNoWait;
   u8 field_0x32f;
   u8 field_0x330;
   u8 choices[6];
   u8 enableMapScaling;
   u8 textSpeed;
   u8 field_0x339;
   u8 field_0x33a;
   u8 field_0x33b;
   EvtData *evtScreenEffect;
   u8 field_0x340;
   u8 field_0x341;
   u8 field_0x342;
   u8 field_0x343;
} State;

extern State gState;

#endif
