#ifndef WINDOW_H
#define WINDOW_H

#include "common.h"

typedef enum WindowBorderStyle {
   WBS_CROSSED = 0,
   WBS_DRAGON = 1,
   WBS_ROUNDED = 2
} WindowBorderStyle;

typedef struct WindowChoice {
   union {
      s8 bytes[2];
      s16 raw;
      struct {
         s8 choice, windowId;
      } s;
   } d;
} WindowChoice;

extern u16 gWindowChoiceHeight, gWindowChoicesCount, gWindowChoicesTopMargin;
extern s16 gWindowActiveIdx;
extern u8 gHighlightedChoice;
extern s16 gWindowDisplayX[16];
extern s16 gWindowDisplayY[16];
extern WindowChoice gWindowChoice;
extern WindowChoice gWindowActivatedChoice;

#endif
