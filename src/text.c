#include "common.h"
#include "graphics.h"
#include "object.h"
#include "state.h"
#include "glyphs.h"

s32 CopySjisString(u8 *, u8 *);
s32 DecodeLineOfText(u8 *, u8 *);
void LoadText(s32, u8 *, u8 **);
s32 DrawSjisGlyph(u16, s32, s32, s32);
s32 MsgBox_DrawSjisGlyph(Object *, u16);
s32 MsgBox_DrawFontGlyph(Object *, s16);
s32 ParseDigits(u8 *, s32 *);
void MsgBox_Clear(Object *);
void Objf351_MsgBoxText(Object *);
u8 GetGlyphIdxForAsciiChar(u8);
void DrawFontGlyph(u8, s32, s32, s32);
void DrawText_Internal(s32, s32, s32, s32, s32, u8 *, s32);
void DrawSjisText_Internal(s32, s32, s32, s32, s32, u8 *, s32);
void DrawText(s32, s32, s32, s32, s32, u8 *);
void DrawSjisText(s32, s32, s32, s32, s32, u8 *);
void MsgBox_SetText(s32, s32, s32);
void MsgBox_SetText2(s32, s32, s32);
void Objf798_ResetInputState(Object *);

u8 *gStringTable[100] = {
#include "assets/8010102c.inc"
};

u8 **s_stringTable_80123348;

s32 CopySjisString(u8 *src, u8 *dst) {
   s32 n = 0;

   while (*src != '\0') {
      if ((*src >= 0x81 && *src <= 0x9f) || (*src >= 0xe0 && *src <= 0xfc)) {
         *dst++ = *src++;
         *dst++ = *src++;
         n += 2;
      } else {
         *dst++ = *src++;
         n += 1;
      }
   }

   return n;
}

s32 DecodeLineOfText(u8 *src, u8 *dst) {
   u8 b1, b2;
   s32 n = 0;

   while (1) {
      b1 = ~src[0];
      b2 = ~src[1];
      if (b1 == '\r' && b2 == '\n') {
         break;
      }
      if ((b1 >= 0x81 && b1 <= 0x9f) || (b1 >= 0xe0 && b1 <= 0xfc)) {
         *dst++ = b1;
         *dst++ = b2;
         src += 2;
         n += 2;
      } else {
         *dst++ = b1;
         src++;
         n += 1;
      }
   }

   *dst++ = '\n';
   *dst++ = '\0';
   return n + 2;
}

void LoadText(s32 cdf, u8 *pText, u8 **textPointers) {
   s32 readingEntry;
   s32 entryNum;
   u8 *pInputData;
   u8 buffer[1024];
   s32 n;

   readingEntry = 0;
   LoadCdFile(cdf, 0);
   entryNum = 1;
   pInputData = (u8 *)gScratch1_801317c0;

   while (entryNum <= 100) {
      n = DecodeLineOfText(pInputData, buffer);

      if (buffer[0] == '\n' && readingEntry == 0) {
         readingEntry = 1;
         textPointers[entryNum++] = pText;
         pInputData += n;
      } else if (buffer[0] == '\n' && readingEntry == 1) {
         readingEntry = 0;
         *pText++ = '\0';
      } else if (buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D') {
         *pText++ = '\0';
         break;
      } else if (((buffer[0] >= 0x81 && buffer[0] <= 0x9f) ||
                  (buffer[0] >= 0xe0 && buffer[0] <= 0xfc)) &&
                 (buffer[1] == 0x94)) {
         //? Presumably this is to treat lines starting with SJIS 8194 (#) as comments, but won't
         //  it also include a bunch of false positives?
         pInputData += n;
      } else {
         pText += CopySjisString(buffer, pText);
         pInputData += n;
      }
   }
}

s32 DrawSjisGlyph(u16 sjis, s32 x, s32 y, s32 color) {
   // For anti-aliasing effect:
   static s32 whiteShades[5] = {0, 1, 2, 3, 4};
   static s32 redShades[5] = {0, 5, 6, 7, 8};

   u16 buffer[45];
   s32 *colors;
   RECT rect;
   u8 *pInputData;
   u16 *pOutputData;
   s32 i;
   u8 byte;
   u16 output1, output2, output3;
   u16 bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7;
   u16 tmp1, tmp2; //?

   if (color != 0) {
      colors = redShades;
   } else {
      colors = whiteShades;
   }

   if (sjis >= 0x83fd && sjis <= 0x843f) {
      return -1;
   }

   pInputData = (u8 *)Krom2RawAdd(sjis);
   if (pInputData == -1) {
      return -1;
   }

   if (y > 240) {
      return -2;
   }

   pOutputData = &buffer[0];

   for (i = 0; i < 15; i++) {
      byte = *pInputData; // byte1

      bit0 = byte & 1;
      byte >>= 1;
      bit1 = byte & 1;
      byte >>= 1;
      bit2 = byte & 1;
      byte >>= 1;
      bit3 = byte & 1;
      byte >>= 1;

      output2 = colors[bit0 * 3 + bit1] << 4;
      output2 |= colors[bit1 * 2 + bit2 * 2];
      output1 = tmp1 = colors[bit3 * 3 + bit2] << 12;

      bit4 = byte & 1;
      byte >>= 1;
      bit5 = byte & 1;
      byte >>= 1;
      bit6 = byte & 1;
      byte >>= 1;
      bit7 = byte & 1;
      byte >>= 1;

      output1 |= colors[bit4 * 3 + bit5] << 8;
      output1 |= colors[bit5 * 2 + bit6 * 2] << 4;
      output1 |= colors[bit7 * 3 + bit6];

      pInputData++;
      byte = *pInputData; // byte2

      bit0 = byte & 1;
      byte >>= 1;
      bit1 = byte & 1;
      byte >>= 1;
      bit2 = byte & 1;
      byte >>= 1;
      bit3 = byte & 1;
      byte >>= 1;

      output3 = tmp2 = colors[bit0 * 3 + bit1] << 12;
      output3 |= colors[bit1 * 2 + bit2 * 2] << 8;
      output3 |= colors[bit3 * 3 + bit2] << 4;

      bit4 = byte & 1;
      byte >>= 1;
      bit5 = byte & 1;
      byte >>= 1;
      bit6 = byte & 1;
      byte >>= 1;
      bit7 = byte & 1;
      byte >>= 1;

      output3 |= colors[bit4 * 3 + bit5];
      output2 |= colors[bit5 * 2 + bit6 * 2] << 12;
      output2 |= colors[bit7 * 3 + bit6] << 8;

      *pOutputData++ = output1;
      *pOutputData++ = output2;
      *pOutputData++ = output3;
      pInputData++;
   }

   rect.x = x;
   rect.y = y;
   rect.w = 12 >> 2;
   rect.h = 15;
   LoadImage(&rect, buffer);
   DrawSync(0);
   return 0;
}

static u8 sMsgBoxVramOffsets[6][4] = {{0, 0, -8, 108}, {0, 0, 72, 108},   {0, 0, -8, 108},
                                      {0, 0, 72, 108}, {0, 100, -8, 108}, {0, 100, 72, 108}};

s32 MsgBox_DrawSjisGlyph(Object *msg, u16 sjis) {
   // x3: column, y3: row
   if (msg->x3.n < msg->d.objf351.pregapChars) {
      DrawSjisGlyph(sjis, 512 + msg->x3.n * (12 >> 2) + msg->x1.n,
                    msg->y3.n * (msg->d.objf351.lineSpacing + 15) + msg->y1.n +
                        sMsgBoxVramOffsets[msg->d.objf351.type * 2][1],
                    0);
      return 0;
   } else if (msg->x3.n < msg->d.objf351.maxCharsPerLine) {
      DrawSjisGlyph(sjis, 576 + (msg->x3.n - msg->d.objf351.pregapChars) * (12 >> 2),
                    msg->y3.n * (msg->d.objf351.lineSpacing + 15) + msg->y1.n +
                        sMsgBoxVramOffsets[msg->d.objf351.type * 2 + 1][1],
                    0);
      return 0;
   } else {
      return -1;
   }
}

s32 MsgBox_DrawFontGlyph(Object *msg, s16 idx) {
   // x3: column, y3: row
   if (msg->x3.n < msg->d.objf351.pregapChars) {
      DrawFontGlyph(idx, 512 + msg->x3.n * (8 >> 2) + msg->x1.n,
                    msg->y3.n * (msg->d.objf351.lineSpacing + 15) + msg->y1.n +
                        sMsgBoxVramOffsets[msg->d.objf351.type * 2][1],
                    0);
      return 0;
   } else if (msg->x3.n < msg->d.objf351.maxCharsPerLine) {
      DrawFontGlyph(idx, 576 + (msg->x3.n - msg->d.objf351.pregapChars) * (8 >> 2),
                    msg->y3.n * (msg->d.objf351.lineSpacing + 15) + msg->y1.n +
                        sMsgBoxVramOffsets[msg->d.objf351.type * 2 + 1][1],
                    0);
      return 0;
   } else {
      return -1;
   }
}

s32 ParseDigits(u8 *str, s32 *output) {
   s32 value;
   s32 n;

   value = 0;
   n = 1;

   while (*str >= '0' && *str <= '9') {
      value *= 10;
      value += (*str - '0');
      str++;
      n++;
   }

   *output = value;
   return n;
}

void MsgBox_Clear(Object *msg) {
   Object *buttonIcon;

   buttonIcon = msg->d.objf351.buttonIcon;
   buttonIcon->functionIndex = OBJF_NULL;
   msg->functionIndex = OBJF_NULL;
   gState.msgBoxFinished = 1;
}

#undef OBJF
#define OBJF 351
void Objf351_MsgBoxText(Object *obj) {
   static s16 buttonIconAnimData1[12] = {2, GFX_MSGBOX_BUTTON_1,
                                         3, GFX_MSGBOX_BUTTON_2,
                                         3, GFX_MSGBOX_BUTTON_3,
                                         3, GFX_MSGBOX_BUTTON_4,
                                         3, GFX_NULL,
                                         1, GFX_NULL};

   static s16 buttonIconAnimData2[12] = {2, GFX_MSGBOX_BUTTON_1,
                                         6, GFX_MSGBOX_BUTTON_2,
                                         6, GFX_MSGBOX_BUTTON_3,
                                         6, GFX_MSGBOX_BUTTON_4,
                                         6, GFX_NULL,
                                         1, GFX_NULL};

   static SVectorXY buttonIconPositions[6] = {{289, 58},  {232, 203}, {281, 64},
                                              {236, 197}, {285, 129}, {281, 99}};

   static s32 textSpeeds[8] = {0x80, 0x100, 0x400, 0x400, 0x400, 0x400, 0x400, 0x400};

   s16 buttonIconX, buttonIconY;
   Object *buttonIcon;
   RECT rect;
   u8 *p;
   s32 maxCharsPerLine;
   s32 parsedInt;
   s32 n;
   u32 sjis;

   maxCharsPerLine = OBJ.maxCharsPerLine;

   switch (obj->state) {
   case 0:
      gState.msgBoxFinished = 0;
      gState.field_0x31d = 0;
      obj->state3 = 1;
      OBJ.todo_x48 = 0;

      OBJ.textSpeed = textSpeeds[gState.textSpeed & 7];
      if (gState.vsyncMode != 2) {
         OBJ.textSpeed >>= 1;
      }

      OBJ.maxCharsPerLine = 26;
      OBJ.lineSpacing = 1;
      OBJ.maxRows = 3;

      buttonIconX = buttonIconPositions[OBJ.type - 1].x;
      buttonIconY = buttonIconPositions[OBJ.type - 1].y;

      switch (OBJ.type) {
      case 0:
      case 1:
         obj->x1.n = 64 >> 2;
         obj->y1.n = 8;
         OBJ.pregapChars = (240 - obj->x1.n * 4) / 8;
         break;
      case 2:
         obj->x1.n = 12 >> 2;
         obj->y1.n = 8;
         OBJ.pregapChars = (240 - obj->x1.n * 4) / 8;
         break;
      case 3:
         obj->x1.n = 80 >> 2;
         obj->y1.n = 22;
         OBJ.pregapChars = (248 - obj->x1.n * 4) / 8;
         OBJ.type = 1;
         break;
      case 4:
         obj->x1.n = 24 >> 2;
         obj->y1.n = 22;
         OBJ.pregapChars = (248 - obj->x1.n * 4) / 8;
         OBJ.type = 2;
         break;
      case 5:
         obj->x1.n = 80 >> 2;
         obj->y1.n = 22;
         OBJ.pregapChars = (248 - obj->x1.n * 4) / 8;
         OBJ.type = 1;
         break;
      case 6:
         obj->x1.n = 80 >> 2;
         obj->y1.n = 22;
         OBJ.pregapChars = (248 - obj->x1.n * 4) / 8;
         OBJ.type = 1;
         break;
      }

      obj->x3.n = 0; // current column
      obj->y3.n = 0; // current row
      OBJ.textSpeedAccum = 0;
      if (OBJ.textSpeed == 0) {
         OBJ.textSpeed = 0x100;
      }
      if (OBJ.pregapChars > OBJ.maxCharsPerLine) {
         OBJ.pregapChars = OBJ.maxCharsPerLine;
      }

      buttonIcon = Obj_GetUnused();
      buttonIcon->functionIndex = OBJF_NOOP;
      OBJ.buttonIcon = buttonIcon;
      if (gState.vsyncMode == 2) {
         buttonIcon->d.sprite.animData = buttonIconAnimData1;
      } else {
         buttonIcon->d.sprite.animData = buttonIconAnimData2;
      }
      buttonIcon->d.sprite.gfxIdx = GFX_TBD_42;
      buttonIcon->d.sprite.semiTrans = 1;
      buttonIcon->d.sprite.clut = CLUT_25;
      buttonIcon->x1.n = buttonIconX;
      buttonIcon->y1.n = buttonIconY;
      buttonIcon->x3.n = buttonIconX + 16;
      buttonIcon->y3.n = buttonIconY + 16;

      OBJ.textPtr = gState.currentTextPointers[OBJ.textPtrIdx];
      OBJ.todo_x44 = 0;

      OBJ.rect.x = obj->x1.n + 512;
      if (OBJ.type == 1) {
         OBJ.rect.y = obj->y1.n;
      } else if (OBJ.type == 2) {
         OBJ.rect.y = obj->y1.n + 100;
      }
      OBJ.rect.w = 64 + (OBJ.maxCharsPerLine - OBJ.pregapChars) * (12 >> 2) - obj->x1.n; //
      OBJ.rect.w = 64 + (OBJ.maxCharsPerLine - OBJ.pregapChars) * (8 >> 2) - obj->x1.n;
      OBJ.rect.h = (OBJ.lineSpacing + 15) * OBJ.maxRows;
      rect.x = OBJ.rect.x;
      rect.y = OBJ.rect.y;
      rect.w = OBJ.rect.w;
      rect.h = OBJ.rect.h;
      ClearImage(&rect, 0, 0, 0);
      obj->state++;
      break;

   case 1:
      if (--obj->state3 > 0) {
         break;
      }
      obj->state++;

   // fallthrough
   case 2:
      p = OBJ.textPtr;
      if (gPadStateNewPresses & PAD_X) {
         OBJ.textSpeedAccum += 0x4000;
      }
      if (OBJ.todo_x44 == 0 && (gPadStateNewPresses & PAD_CIRCLE)) {
         OBJ.todo_x44 = 1;
      }
      if (OBJ.todo_x45 != 0 && !(gPadState & PAD_CIRCLE)) {
         OBJ.todo_x44 = 0;
      }
      if ((gPadState & PAD_CIRCLE) && (OBJ.todo_x44 != 0)) {
         OBJ.textSpeedAccum += 0x200;
         if (OBJ.textSpeedAccum > 0x4000) {
            OBJ.textSpeedAccum = 0x4000;
         }
      } else {
         OBJ.textSpeedAccum += OBJ.textSpeed;
      }

      while ((OBJ.textSpeedAccum >> 8) > 0) {
         if (*p == '\0') {
            // NUL
            if (OBJ.readingFromStringTable) {
               // End of string table string; resume from after insertion point
               OBJ.textPtr = OBJ.textResumePtr;
               p = OBJ.textPtr;
               OBJ.readingFromStringTable = 0;
            } else {
               gState.field_0x31d = 1;
               obj->state = 5;
               return;
            }
         } else if ((*p >= 0x81 && *p <= 0x9f) || (*p >= 0xe0 && *p <= 0xfc)) {
            // SJIS
            if (obj->x3.n > maxCharsPerLine) {
               obj->x3.n = OBJ.indentChars;
               obj->y3.n++;
               if (obj->y3.n >= OBJ.maxRows) {
                  gState.field_0x31d = 1;
                  obj->state = 4;
                  return;
               }
            }
            sjis = (p[0] << 8) | p[1];
            MsgBox_DrawSjisGlyph(obj, sjis);
            OBJ.textPtr += 2;
            p += 2;
            obj->x3.n++;
            OBJ.textSpeedAccum -= 0x100;
            if (sjis > 0x823f && OBJ.todo_x48 == 0) {
               if (gState.vsyncMode != 2) {
                  n = 6;
               } else {
                  n = 3;
               }
               gState.msgTextWaitTimer[OBJ.type] = n;
            }
         } else {
            // ASCII
            switch (*p) {
            case '\n':
               obj->x3.n = OBJ.indentChars;
               obj->y3.n++;
               OBJ.textPtr++;
               p++;
               if (obj->y3.n >= OBJ.maxRows) {
                  if (*p == '\0') {
                     gState.field_0x31d = 1;
                     obj->state = 5;
                  } else {
                     gState.field_0x31d = 1;
                     obj->state = 4;
                  }
                  return;
               }
               break;

            case '$':
               // Command code, e.g. $T for a timed delay
               p++;
               OBJ.textPtr++;

               switch (*p) {
               case 'W':
               case 'w':
                  // Wait for button press
                  p++;
                  OBJ.textPtr++;
                  if (*p == '\n') {
                     OBJ.textPtr++;
                  }
                  gState.field_0x31d = 1;
                  obj->state = 4;
                  return;

               case 'F':
               case 'f':
                  //? Flag for e.g. event entity animation?
                  gState.field_0x31d = 1;
                  OBJ.textPtr++;
                  return;

               case 'P':
               case 'p':
                  gState.field_0x31d = 1;
                  obj->state = 6;
                  OBJ.textPtr++;
                  return;

               case 'S':
               case 's':
                  // Set text speed
                  p++;
                  n = ParseDigits(p, &parsedInt);
                  // ParseDigits returns numDigits+1 (which is used to skip past the 'S' here)
                  OBJ.textPtr += n;
                  p = OBJ.textPtr;
                  OBJ.textSpeed = parsedInt;
                  continue;

               case 'T':
               case 't':
                  // Delay
                  p++;
                  n = ParseDigits(p, &parsedInt);
                  OBJ.textPtr += n;
                  p = OBJ.textPtr;
                  obj->state3 = parsedInt;
                  obj->state = 3;
                  return;

               case 'O':
               case 'o':
                  OBJ.todo_x48++;
                  OBJ.todo_x48 %= 2;
                  p++;
                  OBJ.textPtr++;
                  continue;
               }

               break;

            case '#':
               p++;
               if (*p != '#') {
                  // Single # specifies an index into the string table
                  OBJ.readingFromStringTable = 1;
                  n = ParseDigits(p, &parsedInt);
                  p += n - 1;
                  OBJ.textResumePtr = p;
                  s_stringTable_80123348 = gStringTable;
                  OBJ.textPtr = gStringTable[parsedInt];
                  p = OBJ.textPtr;
                  continue;
               }
               // Double # indicates escaped #; fall-through to handle
               OBJ.textPtr++;

            // fallthrough
            default:
               MsgBox_DrawFontGlyph(obj, GetGlyphIdxForAsciiChar(*p));
               if (*p >= 'A' && *p <= 'z') {
                  if (gState.vsyncMode != 2) {
                     n = 6;
                  } else {
                     n = 3;
                  }
                  gState.msgTextWaitTimer[OBJ.type] = n;
               }
               p++;
               OBJ.textPtr++;
               obj->x3.n++;
               OBJ.textSpeedAccum -= 0x100;
               break;
            }
         }
      }
      break;

   case 3:
      if (--obj->state3 <= 0 || (gPadStateNewPresses & PAD_X) ||
          (gPadStateNewPresses & PAD_CIRCLE)) {
         obj->state = 2;
      }
      break;

   case 4:
      buttonIcon = OBJ.buttonIcon;
      UpdateObjAnimation(buttonIcon);
      AddObjPrim_Gui(gGraphicsPtr->ot, buttonIcon);
      if ((gPadStateNewPresses & PAD_CIRCLE) || (gPadStateNewPresses & PAD_X)) {
         rect.x = OBJ.rect.x;
         rect.y = OBJ.rect.y;
         rect.w = OBJ.rect.w;
         rect.h = OBJ.rect.h;
         ClearImage(&rect, 0, 0, 0);
         obj->state3 = 1;
         obj->state = 1;
         obj->x3.n = 0;
         obj->y3.n = 0;
         gState.field_0x31d = 1;
         OBJ.textSpeedAccum = 0;
         OBJ.todo_x44 = 0;
      }
      break;

   case 5:
      if ((gPadStateNewPresses & PAD_CIRCLE) || (gPadStateNewPresses & PAD_X)) {
         obj->state++;
      }
      break;

   case 6:
      MsgBox_Clear(obj);
      break;
   }
}

u8 GetGlyphIdxForAsciiChar(u8 asc) {
   static u8 mappings[128] = {
       128, 0,  0,  0,   0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,  0,  0,  0,   0,  0,  0,  0,
       0,   0,  0,  0,   0,  0,  0,  0,  0,  0,  128, 97, 40, 42, 0,  99, 0,  39,  0,  0,  0,  102,
       95,  12, 94, 101, 2,  3,  4,  5,  6,  7,  8,   9,  10, 11, 96, 0,  41, 100, 0,  98, 0,  68,
       69,  70, 71, 72,  73, 74, 75, 76, 77, 78, 79,  80, 81, 82, 83, 84, 85, 86,  87, 88, 89, 90,
       91,  92, 93, 0,   0,  0,  0,  0,  0,  68, 69,  70, 71, 72, 73, 74, 75, 76,  77, 78, 79, 80,
       81,  82, 83, 84,  85, 86, 87, 88, 89, 90, 91,  92, 93, 0,  0,  0,  0,  0};

   return mappings[asc & 0x7f];
}

static u8 sFontGlyphBitmaps[128][9] = {
#include "assets/801012e4.inc"
};

void DrawFontGlyph(u8 glyphIdx, s32 x, s32 y, s32 color) {
   s32 i, j, k;
   RECT rect;
   u16 buffer[32];
   u16 *pOutputData;
   u8 *pInputData;
   u8 byte;
   s16 output;
   s32 colorPlusOne;

   pInputData = &sFontGlyphBitmaps[glyphIdx][0];
   pOutputData = &buffer[0];

   if (y < 247 && glyphIdx <= 128) {
      colorPlusOne = color + 1;
      for (i = 0; i < 9; i++) {
         byte = *pInputData++;
         for (j = 0; j < 2; j++) {
            output = 0;
            for (k = 0; k < 4; k++) {
               output >>= 4;
               if (byte & 0x80) {
                  output += colorPlusOne << 14;
               }
               byte <<= 1;
            }
            *pOutputData++ = output;
         }
      }
      rect.x = x;
      rect.y = y;
      rect.w = 8 >> 2;
      rect.h = 9;
      LoadImage(&rect, buffer);
      DrawSync(0);
   }
}

void DrawText_Internal(s32 x, s32 y, s32 maxCharsPerLine, s32 lineSpacing, s32 color_, u8 *text,
                       s32 gapType) {
   s32 readingFromStringTable;
   u8 *insertionPoint;
   u8 *p;
   s32 n;
   s32 pad;
   s32 column;
   s32 rowY;
   s32 unk_s4;
   s32 parsedInt;
   s32 color;
   s32 rowHeight;

   readingFromStringTable = 0;
   unk_s4 = 0;
   x = (x + (512 * 4)) / 4;
   y += 3;
   color = color_;
   rowHeight = lineSpacing + 15; // s8=s7+15
   p = text;
   rowY = 0;   // s2
   column = 0; // s3

   while (1) {
      switch (*p) {
      case '\0':
         if (readingFromStringTable) {
            // End of string table string; resume from after insertion point
            p = insertionPoint;
            readingFromStringTable = 0;
            continue;
         }
         // End of text
         return;

      case '\n':
         p++;
         column = 0;
         if (unk_s4 == 0) {
            rowY += rowHeight;
         } else if (unk_s4 >= 0) {
            if (unk_s4 < 3) {
               rowY += (lineSpacing + 8);
               unk_s4 = 0;
            }
         }
         continue;

      case '#':
         p++;
         if (*p != '#') {
            // Single # specifies an index into the string table
            readingFromStringTable = 1;
            n = ParseDigits(p, &parsedInt);
            p += n - 1;
            insertionPoint = p;
            s_stringTable_80123348 = gStringTable;
            p = gStringTable[parsedInt];
            continue;
         }
         // Double # indicates escaped #; fall-through to handle

      default:
         pad = 1;
         if (gapType != 0) {
            if (column * (8 >> 2) + x > 571) {
               pad = 3;
            }
         } else {
            // gapType is 0 when drawing battle spell/item description
            if (column * (8 >> 2) + x > 569) {
               pad = 5;
            }
         }
         DrawFontGlyph(GetGlyphIdxForAsciiChar(*p++), column * (8 >> 2) + x + pad, rowY + y, color);
         column++;
         if (column >= maxCharsPerLine) {
            column = 0;
            if (unk_s4 == 0) {
               rowY += rowHeight;
            } else if (unk_s4 >= 0) {
               if (unk_s4 < 3) {
                  rowY += (lineSpacing + 8);
                  unk_s4 = 0;
               }
            }
         }
         continue;
      }
   }
}

void DrawSjisText_Internal(s32 x, s32 y, s32 maxCharsPerLine, s32 lineSpacing, s32 color_, u8 *text,
                           s32 gapType) {
   s32 readingFromStringTable;
   u8 *insertionPoint;
   u8 *p;
   s32 n;
   s32 pad;
   s32 column;
   s32 rowY;
   s32 unk_s3;
   s32 parsedInt;
   s32 color;
   s32 rowHeight;
   u32 sjis;

   readingFromStringTable = 0;
   unk_s3 = 0;
   x = (x + (512 * 4)) / 4;
   color = color_;
   rowHeight = lineSpacing + 15;
   p = text;
   rowY = 0;
   column = 0;

   while (1) {
      switch (*p) {
      case '\0':
         if (readingFromStringTable) {
            // End of string table string; resume from after insertion point
            p = insertionPoint;
            readingFromStringTable = 0;
            continue;
         }
         // End of text
         return;

      case '\n':
         p++;
         column = 0;
         if (unk_s3 == 0) {
            rowY += rowHeight;
         } else if (unk_s3 >= 0) {
            if (unk_s3 < 3) {
               rowY += (lineSpacing + 8);
               unk_s3 = 0;
            }
         }
         continue;

      case '#':
         p++;
         if (*p != '#') {
            // Single # specifies an index into the string table
            readingFromStringTable = 1;
            n = ParseDigits(p, &parsedInt);
            p += n - 1;
            insertionPoint = p;
            s_stringTable_80123348 = gStringTable;
            p = gStringTable[parsedInt];
            continue;
         }
         // Double # indicates escaped #; break to handle outside switch
         break;

      case 'U':
         p++;
         unk_s3 = 1;
         continue;

      case 'D':
         p++;
         unk_s3 = 2;
         continue;
      }

      pad = 0;
      if (gapType != 0) {
         if (column * (12 >> 2) + x > 573) {
            pad = 2;
         } else {
            pad = 0;
         }
      } else {
         if (column * (12 >> 2) + x > 571) {
            pad = 4;
         }
      }
      sjis = (p[0] << 8) | p[1];
      DrawSjisGlyph(sjis, column * (12 >> 2) + x + pad, rowY + y, color);
      p += 2;
      column++;
      if (column >= maxCharsPerLine) {
         column = 0;
         if (unk_s3 == 0) {
            rowY += rowHeight;
         } else if (unk_s3 >= 0) {
            if (unk_s3 < 3) {
               rowY += (lineSpacing + 8);
               unk_s3 = 0;
            }
         }
      }
   }
}

void DrawText(s32 x, s32 y, s32 maxCharsPerLine, s32 lineSpacing, s32 color, u8 *text) {
   DrawText_Internal(x, y, maxCharsPerLine, lineSpacing, color, text, 1);
}

void DrawSjisText(s32 x, s32 y, s32 maxCharsPerLine, s32 lineSpacing, s32 color, u8 *text) {
   DrawSjisText_Internal(x, y, maxCharsPerLine, lineSpacing, color, text, 1);
}

void MsgBox_SetText(s32 type, s32 textPtrIdx, s32 textSpeed) {
   s32 i;
   Object *p;
   Object *msg;

   p = gObjectArray;
   for (i = 0; i < OBJ_DATA_CT; i++) {
      if (p->functionIndex == OBJF_MSGBOX_TEXT) {
         MsgBox_Clear(p);
      }
      p++;
   }

   msg = Obj_GetUnused();
   msg->functionIndex = OBJF_MSGBOX_TEXT;
   msg->d.objf351.type = type;
   msg->d.objf351.textPtrIdx = textPtrIdx;
   msg->d.objf351.textSpeed = textSpeed;
}

void MsgBox_SetText2(s32 type, s32 textPtrIdx, s32 textSpeed) {
   s32 i;
   Object *p;
   Object *msg;

   p = gObjectArray;
   for (i = 0; i < OBJ_DATA_CT; i++) {
      if (p->functionIndex == OBJF_MSGBOX_TEXT) {
         MsgBox_Clear(p);
      }
      p++;
   }

   msg = Obj_GetLastUnused();
   msg->functionIndex = OBJF_MSGBOX_TEXT;
   msg->d.objf351.type = type;
   msg->d.objf351.textPtrIdx = textPtrIdx;
   msg->d.objf351.textSpeed = textSpeed;
}

#undef OBJF
#define OBJF 798
void Objf798_ResetInputState(Object *obj) {
   gPadStateNewPresses = 0;
   gPadState = 0;
}
