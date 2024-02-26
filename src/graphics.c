#include "common.h"
#include "graphics.h"
#include "evt.h"
#include "battle.h"
#include "state.h"
#include "field.h"
#include "inline_gte.h"

#include "PsyQ/kernel.h"
#include "PsyQ/gtemac.h"

void LoadDecodedUnitSprites(EvtData *evt) {
   RECT rect;

   rect.x = evt->d.evtf050.vramX;
   rect.y = evt->d.evtf050.vramY;
   rect.w = 256 >> 2;
   rect.h = 352;
   LoadImage(&rect, &gScratch1_801317c0[0]);
}

// TBD: Simpler way around lui anomaly?

static inline void SetFunctionIndex(s16 value) { gUnitSpritesDecoder.functionIndex = value; }

static inline u16 GetState(void) { return gUnitSpritesDecoder.state; }
static inline void SetState(u16 value) { gUnitSpritesDecoder.state = value; }

static inline s32 GetRemainingBytes(void) { return gUnitSpritesDecoder.d.evtf050.remainingBytes; }
static inline void SetRemainingBytes(s32 value) {
   gUnitSpritesDecoder.d.evtf050.remainingBytes = value;
}

static inline u16 GetCacheOfs(void) { return gUnitSpritesDecoder.d.evtf050.cacheOfs; }
static inline void SetCacheOfs(u16 value) { gUnitSpritesDecoder.d.evtf050.cacheOfs = value; }

static inline u16 GetModeBits(void) { return gUnitSpritesDecoder.d.evtf050.encodingBits; }
static inline void SetModeBits(u16 value) { gUnitSpritesDecoder.d.evtf050.encodingBits = value; }

static inline u8 *GetSrc(void) { return gUnitSpritesDecoder.d.evtf050.src; }
static inline void SetSrc(u8 *value) { gUnitSpritesDecoder.d.evtf050.src = value; }

static inline u8 *GetDst(void) { return gUnitSpritesDecoder.d.evtf050.dst; }
static inline void SetDst(u8 *value) { gUnitSpritesDecoder.d.evtf050.dst = value; }

static inline u8 *GetBaseSrc(void) { return (u8 *)gUnitSpritesDecoder.d.evtf050.baseSrcDataPtr; }
static inline u8 *GetBaseDst(void) { return (u8 *)gUnitSpritesDecoder.d.evtf050.baseDstDataPtr; }

#undef EVTF
#define EVTF 050
void Evtf050_UnitSpritesDecoder(EvtData *evt) {
   s32 i, j, k;
   u8 *pCache;
   s32 maxTicks;
   u16 cacheOfs;
   u16 modeBits;
   s32 remainingBytes;
   u8 *pSrc;
   u8 *pDst;
   s32 b;
   // s32 cachedBytesToRead;
   // s32 cacheIdx;
   s16 state;

   pCache = (u8 *)0x1f800000;
   state = GetState();

   switch (state) {
   case 0:
      remainingBytes = 0xb000;
      cacheOfs = 958;
      pSrc = GetBaseSrc();
      pDst = GetBaseDst();
      SetRemainingBytes(remainingBytes);
      SetModeBits(0);
      SetCacheOfs(cacheOfs);
      SetSrc(pSrc + 4);
      SetDst(pDst);

      for (j = 0; j < 958; j++) {
         pCache[j] = 0;
      }

      gDecodingSprites = 1;
      // gUnitSpritesDecoder.state++;
      SetState(GetState() + 1);

   // fallthrough
   case 1:
      if (!gIsEnemyTurn || GetRCnt(RCntCNT1) <= 470) {
         j = GetRCnt(RCntCNT1);
         if (gIsEnemyTurn) {
            maxTicks = j + ((256 - j) & 0xff) - 30;
         } else {
            maxTicks = j + ((256 - j) & 0xff);
         }
         if ((j >> 8) == 0) {
            maxTicks += 256;
         }

         remainingBytes = GetRemainingBytes();
         modeBits = GetModeBits();
         cacheOfs = GetCacheOfs();
         pSrc = GetSrc();
         pDst = GetDst();

         do {
            modeBits >>= 1;
            if ((modeBits & 0x100) == 0) {
               // Ran out of sentinel bits; read a byte from input and OR a sentinel hi-byte onto it
               modeBits = *pSrc++ | 0xff00;
            }
            if ((modeBits & 1) != 0) {
               // Mode lo-bit is set; read and cache one byte from input
               b = *pSrc++;
               *pDst++ = b;
               remainingBytes--;
               if (remainingBytes == 0) {
                  LoadDecodedUnitSprites(&gUnitSpritesDecoder);
                  SetFunctionIndex(EVTF_NULL);
                  gDecodingSprites = 0;
                  return;
               }
               pCache[cacheOfs++ & 0x3ff] = b;
            } else {
               // Mode lo-bit is clear; next two bytes decode to cache idx and length.
               // b1: cccc_cccc; b2: CCnn_nnnn;
               // CCcccccccc: 10-bit cache index
               // nnnnnn + 3: num bytes to read from cache (3..66)
               // (0x3f: 0b0011_1111; 0xc0: 0b1100_0000)
               j = *pSrc++;          // b1
               k = *pSrc++;          // b2
               j |= (k & 0xc0) << 2; // cacheIdx
               k = (k & 0x3f) + 3;   // cachedBytesToRead
               for (i = 0; i < k; i++) {
                  b = pCache[(j + i) & 0x3ff];
                  *pDst++ = b;
                  pCache[cacheOfs++ & 0x3ff] = b;
               }
               remainingBytes -= k;
               if (remainingBytes <= 0) {
                  LoadDecodedUnitSprites(&gUnitSpritesDecoder);
                  SetFunctionIndex(EVTF_NULL);
                  gDecodingSprites = 0;
                  return;
               }
            }
         } while (GetRCnt(RCntCNT1) <= maxTicks);

         SetRemainingBytes(remainingBytes);
         SetModeBits(modeBits);
         SetCacheOfs(cacheOfs);
         SetSrc(pSrc);
         SetDst(pDst);
      }
      break;

   case 2:
      gDecodingSprites = 0;
      SetFunctionIndex(EVTF_NULL);
      DrawSync(0);
      break;
   }
}

#undef EVTF
#define EVTF 051
void Evtf051_FloatingDamageText(EvtData *evt) {
   EvtData *unitSprite;
   EvtData *digitSprite;
   s32 i;
   s16 hundreds, tens;
   s16 digits[4];
   s16 a, b;
   s16 x_1, x_2, z_1, z_2;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      EVT.hundreds = hundreds = EVT.damage / 100;
      EVT.damage -= EVT.hundreds * 100;
      EVT.tens = tens = EVT.damage / 10;
      EVT.damage -= EVT.tens * 10;
      EVT.ones = EVT.damage;

      if (hundreds != 0) {
         EVT.numDigits = 3;
      } else if (tens != 0) {
         EVT.numDigits = 2;
      } else {
         EVT.numDigits = 1;
      }

      digitSprite = Evt_GetUnused();
      EVT.digitSprite = digitSprite;
      digitSprite->functionIndex = EVTF_NOOP;

      if (EVT.clut == CLUT_NULL) {
         EVT.clut = CLUT_REDS;
      }
      digitSprite->d.sprite.clut = EVT.clut;

      evt->state++;
      break;

   case 1:
      digits[0] = EVT.ones;
      digits[1] = EVT.tens;
      digits[2] = EVT.hundreds;

      unitSprite = EVT.unitSprite;
      digitSprite = EVT.digitSprite;

      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n + CV(0.875);

      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         digitSprite->z1.n -= CV(0.125);
         digitSprite->x1.n += CV(0.125);
         digitSprite->d.sprite.coords[0].x = CV(-0.15625);
         digitSprite->d.sprite.coords[0].z = CV(-0.15625);
         digitSprite->d.sprite.coords[1].x = CV(0.15625);
         digitSprite->d.sprite.coords[1].z = CV(0.15625);
         break;
      case CAM_DIR_EAST:
         digitSprite->x1.n += CV(0.125);
         digitSprite->z1.n += CV(0.125);
         digitSprite->d.sprite.coords[0].x = CV(0.15625);
         digitSprite->d.sprite.coords[0].z = CV(-0.15625);
         digitSprite->d.sprite.coords[1].x = CV(-0.15625);
         digitSprite->d.sprite.coords[1].z = CV(0.15625);
         break;
      case CAM_DIR_NORTH:
         digitSprite->z1.n += CV(0.125);
         digitSprite->x1.n -= CV(0.125);
         digitSprite->d.sprite.coords[0].x = CV(0.15625);
         digitSprite->d.sprite.coords[0].z = CV(0.15625);
         digitSprite->d.sprite.coords[1].x = CV(-0.15625);
         digitSprite->d.sprite.coords[1].z = CV(-0.15625);
         break;
      case CAM_DIR_WEST:
         digitSprite->z1.n -= CV(0.125);
         digitSprite->x1.n -= CV(0.125);
         digitSprite->d.sprite.coords[0].x = CV(-0.15625);
         digitSprite->d.sprite.coords[0].z = CV(0.15625);
         digitSprite->d.sprite.coords[1].x = CV(0.15625);
         digitSprite->d.sprite.coords[1].z = CV(-0.15625);
         break;
      }

      digitSprite->d.sprite.coords[0].y = CV(0.1875);
      digitSprite->d.sprite.coords[1].y = CV(0.1875);

      a = rsin(EVT.phase) + 0x800;
      digitSprite->d.sprite.coords[0].x = (digitSprite->d.sprite.coords[0].x * a) >> 12;
      digitSprite->d.sprite.coords[1].x = (digitSprite->d.sprite.coords[1].x * a) >> 12;
      digitSprite->d.sprite.coords[0].z = (digitSprite->d.sprite.coords[0].z * a) >> 12;
      digitSprite->d.sprite.coords[1].z = (digitSprite->d.sprite.coords[1].z * a) >> 12;
      digitSprite->d.sprite.coords[0].y = (digitSprite->d.sprite.coords[0].y * a) >> 12;
      digitSprite->d.sprite.coords[1].y = (digitSprite->d.sprite.coords[1].y * a) >> 12;
      digitSprite->d.sprite.coords[2].x = digitSprite->d.sprite.coords[0].x;
      digitSprite->d.sprite.coords[3].x = digitSprite->d.sprite.coords[1].x;
      digitSprite->d.sprite.coords[2].z = digitSprite->d.sprite.coords[0].z;
      digitSprite->d.sprite.coords[3].z = digitSprite->d.sprite.coords[1].z;
      digitSprite->d.sprite.coords[2].y = -1 * digitSprite->d.sprite.coords[0].y;
      digitSprite->d.sprite.coords[3].y = -1 * digitSprite->d.sprite.coords[1].y;

      b = abs(digitSprite->d.sprite.coords[0].x) * 2;
      digitSprite->d.sprite.coords[0].x += evt->x1.n;
      digitSprite->d.sprite.coords[1].x += evt->x1.n;
      digitSprite->d.sprite.coords[2].x += evt->x1.n;
      digitSprite->d.sprite.coords[3].x += evt->x1.n;

      digitSprite->d.sprite.coords[0].z += evt->z1.n;
      digitSprite->d.sprite.coords[1].z += evt->z1.n;
      digitSprite->d.sprite.coords[2].z += evt->z1.n;
      digitSprite->d.sprite.coords[3].z += evt->z1.n;

      digitSprite->d.sprite.coords[0].y += evt->y1.n + (a - 0x800) / 16;
      digitSprite->d.sprite.coords[1].y += evt->y1.n + (a - 0x800) / 16;
      digitSprite->d.sprite.coords[2].y += evt->y1.n + (a - 0x800) / 16;
      digitSprite->d.sprite.coords[3].y += evt->y1.n + (a - 0x800) / 16;

      digitSprite->d.sprite.otOfs = 32;

      switch (EVT.numDigits) {
      case 1:
         digitSprite->d.sprite.gfxIdx = GFX_DIGIT_0 + digits[0];
         AddEvtPrim4(gGraphicsPtr->ot, digitSprite);
         break;

      case 2:

         switch ((gCameraRotation.vy & 0xfff) >> 10) {
         case CAM_DIR_SOUTH:
            x_1 = (b * 3) / 2;
            z_1 = (b * 3) / 2;
            x_2 = -1 * b;
            z_2 = -1 * b;
            break;
         case CAM_DIR_EAST:
            x_1 = -(b * 3) / 2;
            z_1 = (b * 3) / 2;
            x_2 = b;
            z_2 = -1 * b;
            break;
         case CAM_DIR_NORTH:
            x_1 = -(b * 3) / 2;
            z_1 = -(b * 3) / 2;
            x_2 = b;
            z_2 = b;
            break;
         default:
            x_1 = (b * 3) / 2;
            z_1 = -(b * 3) / 2;
            x_2 = -1 * b;
            z_2 = b;
            break;
         }

         digitSprite->d.sprite.coords[0].x += x_1;
         digitSprite->d.sprite.coords[1].x += x_1;
         digitSprite->d.sprite.coords[2].x += x_1;
         digitSprite->d.sprite.coords[3].x += x_1;
         digitSprite->d.sprite.coords[0].z += z_1;
         digitSprite->d.sprite.coords[1].z += z_1;
         digitSprite->d.sprite.coords[2].z += z_1;
         digitSprite->d.sprite.coords[3].z += z_1;

         for (i = 0; i < 2; i++) {
            digitSprite->d.sprite.gfxIdx = GFX_DIGIT_0 + digits[i];
            digitSprite->d.sprite.coords[0].x += x_2;
            digitSprite->d.sprite.coords[1].x += x_2;
            digitSprite->d.sprite.coords[2].x += x_2;
            digitSprite->d.sprite.coords[3].x += x_2;
            digitSprite->d.sprite.coords[0].z += z_2;
            digitSprite->d.sprite.coords[1].z += z_2;
            digitSprite->d.sprite.coords[2].z += z_2;
            digitSprite->d.sprite.coords[3].z += z_2;
            AddEvtPrim4(gGraphicsPtr->ot, digitSprite);
         }

         break;

      case 3:

         switch ((gCameraRotation.vy & 0xfff) >> 10) {
         case CAM_DIR_SOUTH:
            x_1 = b * 2;
            z_1 = b * 2;
            x_2 = -1 * b;
            z_2 = -1 * b;
            break;
         case CAM_DIR_EAST:
            x_1 = -(b * 2);
            z_1 = b * 2;
            x_2 = b;
            z_2 = -1 * b;
            break;
         case CAM_DIR_NORTH:
            x_1 = -(b * 2);
            z_1 = -(b * 2);
            x_2 = b;
            z_2 = b;
            break;
         case CAM_DIR_WEST:
            x_1 = b * 2;
            z_1 = -(b * 2);
            x_2 = -1 * b;
            z_2 = b;
            break;
         }

         digitSprite->d.sprite.coords[0].x += x_1;
         digitSprite->d.sprite.coords[1].x += x_1;
         digitSprite->d.sprite.coords[2].x += x_1;
         digitSprite->d.sprite.coords[3].x += x_1;
         digitSprite->d.sprite.coords[0].z += z_1;
         digitSprite->d.sprite.coords[1].z += z_1;
         digitSprite->d.sprite.coords[2].z += z_1;
         digitSprite->d.sprite.coords[3].z += z_1;

         for (i = 0; i < 3; i++) {
            digitSprite->d.sprite.gfxIdx = GFX_DIGIT_0 + digits[i];
            digitSprite->d.sprite.coords[0].x += x_2;
            digitSprite->d.sprite.coords[1].x += x_2;
            digitSprite->d.sprite.coords[2].x += x_2;
            digitSprite->d.sprite.coords[3].x += x_2;
            digitSprite->d.sprite.coords[0].z += z_2;
            digitSprite->d.sprite.coords[1].z += z_2;
            digitSprite->d.sprite.coords[2].z += z_2;
            digitSprite->d.sprite.coords[3].z += z_2;
            AddEvtPrim4(gGraphicsPtr->ot, digitSprite);
         }

         break;
      }

      switch (evt->state2) {
      case 0:
         EVT.phase += DEG(16.34765625);
         if (EVT.phase >= DEG(135)) {
            evt->state2++;
         }
         break;

      case 1:
         if (EVT.timer == 30) {
            evt->functionIndex = EVTF_NULL;
            digitSprite->functionIndex = EVTF_NULL;
         }
         EVT.timer++;
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 052
void Evtf052_AttackInfoMarker(EvtData *evt) {
   s16 markers[4] = {GFX_MISS, GFX_PARALYZED, GFX_SUPPORT, GFX_POISONED};
   EvtData *unitSprite;
   EvtData *markerSprite;
   s16 a;

   switch (evt->state) {
   case 0:
      EVT.unitSprite = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);

      markerSprite = Evt_GetUnused();
      EVT.sprite = markerSprite;
      markerSprite->functionIndex = EVTF_NOOP;
      markerSprite->d.sprite.gfxIdx = markers[EVT.type];

      if (EVT.clut == CLUT_NULL) {
         EVT.clut = CLUT_REDS;
      }
      markerSprite->d.sprite.clut = EVT.clut;

      evt->state++;
      break;

   case 1:
      unitSprite = EVT.unitSprite;
      markerSprite = EVT.sprite;
      evt->x1.n = unitSprite->x1.n;
      evt->z1.n = unitSprite->z1.n;
      evt->y1.n = unitSprite->y1.n + CV(0.875);

      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         markerSprite->z1.n -= CV(0.125);
         markerSprite->x1.n += CV(0.125);
         markerSprite->d.sprite.coords[0].x = CV(-0.375);
         markerSprite->d.sprite.coords[0].z = CV(-0.375);
         markerSprite->d.sprite.coords[1].x = CV(0.375);
         markerSprite->d.sprite.coords[1].z = CV(0.375);
         break;
      case CAM_DIR_EAST:
         markerSprite->x1.n += CV(0.125);
         markerSprite->z1.n += CV(0.125);
         markerSprite->d.sprite.coords[0].x = CV(0.375);
         markerSprite->d.sprite.coords[0].z = CV(-0.375);
         markerSprite->d.sprite.coords[1].x = CV(-0.375);
         markerSprite->d.sprite.coords[1].z = CV(0.375);
         break;
      case CAM_DIR_NORTH:
         markerSprite->z1.n += CV(0.125);
         markerSprite->x1.n -= CV(0.125);
         markerSprite->d.sprite.coords[0].x = CV(0.375);
         markerSprite->d.sprite.coords[0].z = CV(0.375);
         markerSprite->d.sprite.coords[1].x = CV(-0.375);
         markerSprite->d.sprite.coords[1].z = CV(-0.375);
         break;
      case CAM_DIR_WEST:
         markerSprite->z1.n -= CV(0.125);
         markerSprite->x1.n -= CV(0.125);
         markerSprite->d.sprite.coords[0].x = CV(-0.375);
         markerSprite->d.sprite.coords[0].z = CV(0.375);
         markerSprite->d.sprite.coords[1].x = CV(0.375);
         markerSprite->d.sprite.coords[1].z = CV(-0.375);
         break;
      }

      markerSprite->d.sprite.coords[0].y = CV(0.1875);
      markerSprite->d.sprite.coords[1].y = CV(0.1875);

      if (EVT.type == ATK_MARKER_SUPPORT) {
         markerSprite->d.sprite.coords[0].y = CV(0.125);
         markerSprite->d.sprite.coords[1].y = CV(0.125);
         markerSprite->d.sprite.coords[0].x /= 2;
         markerSprite->d.sprite.coords[0].z /= 2;
         markerSprite->d.sprite.coords[1].x /= 2;
         markerSprite->d.sprite.coords[1].z /= 2;
      }

      a = rsin(EVT.angle) + 0x800;
      markerSprite->d.sprite.coords[0].x = markerSprite->d.sprite.coords[0].x * a / 0xc00;
      markerSprite->d.sprite.coords[1].x = markerSprite->d.sprite.coords[1].x * a / 0xc00;
      markerSprite->d.sprite.coords[0].z = markerSprite->d.sprite.coords[0].z * a / 0xc00;
      markerSprite->d.sprite.coords[1].z = markerSprite->d.sprite.coords[1].z * a / 0xc00;
      markerSprite->d.sprite.coords[0].y = markerSprite->d.sprite.coords[0].y * a / 0xc00;
      markerSprite->d.sprite.coords[1].y = markerSprite->d.sprite.coords[1].y * a / 0xc00;
      markerSprite->d.sprite.coords[0].x += evt->x1.n;
      markerSprite->d.sprite.coords[1].x += evt->x1.n;
      markerSprite->d.sprite.coords[0].z += evt->z1.n;
      markerSprite->d.sprite.coords[1].z += evt->z1.n;
      markerSprite->d.sprite.coords[2].x = markerSprite->d.sprite.coords[0].x;
      markerSprite->d.sprite.coords[3].x = markerSprite->d.sprite.coords[1].x;
      markerSprite->d.sprite.coords[2].z = markerSprite->d.sprite.coords[0].z;
      markerSprite->d.sprite.coords[3].z = markerSprite->d.sprite.coords[1].z;
      markerSprite->d.sprite.coords[2].y = -markerSprite->d.sprite.coords[0].y;
      markerSprite->d.sprite.coords[3].y = -markerSprite->d.sprite.coords[1].y;
      markerSprite->d.sprite.coords[0].y += evt->y1.n + ((a - 0x800) >> 4);
      markerSprite->d.sprite.coords[1].y += evt->y1.n + ((a - 0x800) >> 4);
      markerSprite->d.sprite.coords[2].y += evt->y1.n + ((a - 0x800) >> 4);
      markerSprite->d.sprite.coords[3].y += evt->y1.n + ((a - 0x800) >> 4);
      AddEvtPrim4(gGraphicsPtr->ot, markerSprite);

      switch (evt->state2) {
      case 0:
         EVT.angle += DEG(16.171875);
         if (EVT.angle >= DEG(135)) {
            evt->state2++;
         }
         if (EVT.type == ATK_MARKER_SUPPORT) {
            EVT.timer = 20;
         } else {
            EVT.timer = 30;
         }
         break;

      case 1:
         if (--EVT.timer == 0) {
            evt->functionIndex = EVTF_NULL;
            markerSprite->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 059
void Evtf059_DebugVram(EvtData *evt) {
   extern s32 D_8012340C;

   s16 widths[5] = {256, 320, 384, 512, 640};
   s16 heights[2] = {240, 480};
   Graphics *g;

   switch (evt->state) {
   case 0:
      EVT.widthIdx = 1;
      EVT.heightIdx = 0;
      EVT.width = widths[EVT.widthIdx];
      EVT.height = heights[EVT.heightIdx];
      evt->state++;
      break;

   case 1:
      if (gSavedPad2StateNewPresses & PAD_SELECT) {
         D_8012340C = 0;
         evt->functionIndex = EVTF_NULL;
         SetDefDispEnv(&gGraphicBuffers[0].dispEnv, 0, 272, SCREEN_WIDTH, SCREEN_HEIGHT);
         SetDefDispEnv(&gGraphicBuffers[1].dispEnv, 0, 16, SCREEN_WIDTH, SCREEN_HEIGHT);
      } else {
         if (gSavedPad2State & PAD_UP) {
            gGraphicsPtr->dispEnv.disp.y -= 8;
         }
         if (gSavedPad2State & PAD_DOWN) {
            gGraphicsPtr->dispEnv.disp.y += 8;
         }
         if (gSavedPad2State & PAD_LEFT) {
            gGraphicsPtr->dispEnv.disp.x -= 8;
         }
         if (gSavedPad2State & PAD_RIGHT) {
            gGraphicsPtr->dispEnv.disp.x += 8;
         }
         if (gSavedPad2StateNewPresses & PAD_L1) {
            EVT.widthIdx++;
            EVT.widthIdx %= 5;
            EVT.width = widths[EVT.widthIdx];
         }
         if (gSavedPad2StateNewPresses & PAD_L2) {
            EVT.heightIdx++;
            EVT.heightIdx %= 2;
            EVT.height = heights[EVT.heightIdx];
         }
         gGraphicsPtr->dispEnv.disp.w = widths[EVT.widthIdx];
         gGraphicsPtr->dispEnv.disp.h = heights[EVT.heightIdx];

         g = (gGraphicsPtr == &gGraphicBuffers[0]) ? &gGraphicBuffers[1] : &gGraphicBuffers[0];
         g->dispEnv.disp.x = gGraphicsPtr->dispEnv.disp.x;
         g->dispEnv.disp.y = gGraphicsPtr->dispEnv.disp.y;
         g->dispEnv.disp.w = gGraphicsPtr->dispEnv.disp.w;
         g->dispEnv.disp.h = gGraphicsPtr->dispEnv.disp.h;
      }
      break;
   }
}

void CopyEvtData(EvtData *src, EvtData *dst) {
   s32 i;

   for (i = 0; i < 24; i++) {
      ((u32 *)dst)[i] = ((u32 *)src)[i];
   }
}

void SetupLightNormals(void) {
   VECTOR v[26];
   s32 i;

   v[0].vx = 0;
   v[0].vy = 1;
   v[0].vz = 0;
   v[1].vx = 0;
   v[1].vy = -1;
   v[1].vz = 0;
   v[2].vx = 1;
   v[2].vy = 0;
   v[2].vz = 0;
   v[3].vx = 0;
   v[3].vy = 0;
   v[3].vz = 1;
   v[4].vx = -1;
   v[4].vy = 0;
   v[4].vz = 0;
   v[5].vx = 0;
   v[5].vy = 0;
   v[5].vz = -1;
   v[6].vx = 1;
   v[6].vy = 0;
   v[6].vz = -1;
   v[7].vx = 1;
   v[7].vy = 0;
   v[7].vz = 1;
   v[8].vx = -1;
   v[8].vy = 0;
   v[8].vz = 1;
   v[9].vx = -1;
   v[9].vy = 0;
   v[9].vz = -1;
   v[10].vx = 1;
   v[10].vy = 1;
   v[10].vz = 0;
   v[11].vx = 0;
   v[11].vy = 1;
   v[11].vz = 1;
   v[12].vx = -1;
   v[12].vy = 1;
   v[12].vz = 0;
   v[13].vx = 0;
   v[13].vy = 1;
   v[13].vz = -1;
   v[14].vx = 1;
   v[14].vy = 1;
   v[14].vz = -1;
   v[15].vx = 1;
   v[15].vy = 1;
   v[15].vz = 1;
   v[16].vx = -1;
   v[16].vy = 1;
   v[16].vz = 1;
   v[17].vx = -1;
   v[17].vy = 1;
   v[17].vz = -1;
   v[18].vx = 1;
   v[18].vy = -1;
   v[18].vz = 0;
   v[19].vx = 0;
   v[19].vy = -1;
   v[19].vz = 1;
   v[20].vx = -1;
   v[20].vy = -1;
   v[20].vz = 0;
   v[21].vx = 0;
   v[21].vy = -1;
   v[21].vz = -1;
   v[22].vx = 1;
   v[22].vy = -1;
   v[22].vz = -1;
   v[23].vx = 1;
   v[23].vy = -1;
   v[23].vz = 1;
   v[24].vx = -1;
   v[24].vy = -1;
   v[24].vz = 1;
   v[25].vx = -1;
   v[25].vy = -1;
   v[25].vz = -1;

   for (i = 0; i < ARRAY_COUNT(v); i++) {
      VectorNormalS(&v[i], &gSideLightNormals[i]);
   }
}

void UpdateLight(s16 intensity) {
   static s32 indices[10][2] = {{3, 5},   {7, 6},   {8, 9},   {11, 13}, {15, 14},
                                {16, 17}, {19, 21}, {23, 22}, {24, 25}, {-1, -1}};

   MATRIX mtx;
   VECTOR vec;
   SVECTOR svec = {0x1000, 0, 0, 0};
   CVECTOR unused;
   CVECTOR color1 = {128, 128, 128, 0};
   CVECTOR color2;
   s32 flag;
   s16 i;
   s32 idx1, idx2;
   s32 j;

   PushMatrix();
   RotMatrix(&gLightRotation, &mtx);
   mtx.t[0] = 0;
   mtx.t[1] = 0;
   mtx.t[2] = 0;
   SetRotMatrix(&mtx);
   SetTransMatrix(&mtx);
   RotTrans(&svec, &vec, &flag);
   mtx.m[0][0] = -vec.vx;
   mtx.m[0][1] = -vec.vy;
   mtx.m[0][2] = -vec.vz;
   mtx.m[1][0] = 0;
   mtx.m[1][1] = 0;
   mtx.m[1][2] = 0;
   mtx.m[2][0] = 0;
   mtx.m[2][1] = 0;
   mtx.m[2][2] = 0;
   mtx.t[0] = 0;
   mtx.t[1] = 0;
   mtx.t[2] = 0;
   SetLightMatrix(&mtx);

   for (i = 0; i < 26; i++) {
      gte_NormalColorCol(&gSideLightNormals[i], &color1, &color2);
      gSideLightColor[i].r = gLightColor.r * (intensity + (color2.r * (128 - intensity) >> 7)) >> 7;
      gSideLightColor[i].g = gLightColor.g * (intensity + (color2.g * (128 - intensity) >> 7)) >> 7;
      gSideLightColor[i].b = gLightColor.b * (intensity + (color2.b * (128 - intensity) >> 7)) >> 7;
   }
   PopMatrix();

   if (gState.mapNum == 32) {
      for (j = 0; indices[j][0] != -1; j++) {
         idx1 = indices[j][0];
         idx2 = indices[j][1];
         gSideLightColor[idx2].r = gSideLightColor[idx1].r;
         gSideLightColor[idx2].g = gSideLightColor[idx1].g;
         gSideLightColor[idx2].b = gSideLightColor[idx1].b;
      }
   }
}

void UpdateMapLighting(void) {
   s16 ix, iz;
   s8 unused[56];

   for (iz = 0; iz < gMapMaxZ + 1; iz++) {
      for (ix = 0; ix < gMapMaxX + 1; ix++) {
         UpdateMapTileLighting(&gMapRowPointers[iz][ix]);
      }
   }
}

void UpdateMapTileLighting(MapTileModel *tileModel) {
   VECTOR vec_sp10;
   VECTOR vec_sp20;
   VECTOR vec_sp30;
   SVECTOR svec_sp40;

   s16 var_a1, var_a0, var_t0, var_a2;
   s16 i, j;
   s32 greatest;
   s32 iVar6;
   s16 iVar5;
   s32 ct1, ct2;

   PushMatrix();

   for (i = 0; i < tileModel->faceCt; i++) {
      var_a1 = tileModel->faces[i][0];
      var_a0 = tileModel->faces[i][1];
      var_t0 = tileModel->faces[i][2];
      var_a2 = tileModel->faces[i][3];

      if (var_a1 == var_a0) {
         var_a1 = tileModel->faces[i][2];
         var_a0 = tileModel->faces[i][0];
         var_t0 = tileModel->faces[i][3];
      } else if (var_a1 == var_t0) {
         var_a1 = tileModel->faces[i][1];
         var_a0 = tileModel->faces[i][0];
         var_t0 = tileModel->faces[i][3];
      } else if (var_a1 == var_a2) {
         var_a1 = tileModel->faces[i][1];
         var_a0 = tileModel->faces[i][0];
         var_t0 = tileModel->faces[i][2];
      } else if (var_a0 == var_t0) {
         var_a1 = tileModel->faces[i][1];
         var_a0 = tileModel->faces[i][0];
         var_t0 = tileModel->faces[i][3];
      }

      vec_sp20.vx = tileModel->vertices[var_a0].vx - tileModel->vertices[var_a1].vx;
      vec_sp20.vy = tileModel->vertices[var_a0].vy - tileModel->vertices[var_a1].vy;
      vec_sp20.vz = tileModel->vertices[var_a0].vz - tileModel->vertices[var_a1].vz;

      vec_sp30.vx = tileModel->vertices[var_t0].vx - tileModel->vertices[var_a1].vx;
      vec_sp30.vy = tileModel->vertices[var_t0].vy - tileModel->vertices[var_a1].vy;
      vec_sp30.vz = tileModel->vertices[var_t0].vz - tileModel->vertices[var_a1].vz;

      gte_OuterProduct0(&vec_sp20, &vec_sp30, &vec_sp10);
      VectorNormalS(&vec_sp10, &svec_sp40);

      greatest = -0x1000000;
      iVar5 = 0;
      for (j = 0; j < 26; j++) {
         iVar6 = gSideLightNormals[j].vx * svec_sp40.vx + gSideLightNormals[j].vy * svec_sp40.vy +
                 gSideLightNormals[j].vz * svec_sp40.vz;
         if (iVar6 > greatest) {
            iVar5 = j;
            greatest = iVar6;
         } else if (iVar6 == greatest) {
            ct1 = 0;
            ct2 = 0;

            var_a1 = gSideLightNormals[j].vx;
            var_a0 = gSideLightNormals[j].vy;
            var_t0 = gSideLightNormals[j].vz;

            if (svec_sp40.vx > 0 && var_a1 > 0) {
               ct1++;
            } else if (svec_sp40.vx < 0 && var_a1 < 0) {
               ct1++;
            }
            if (svec_sp40.vy > 0 && var_a0 > 0) {
               ct1++;
            } else if (svec_sp40.vy < 0 && var_a0 < 0) {
               ct1++;
            }
            if (svec_sp40.vz > 0 && var_t0 > 0) {
               ct1++;
            } else if (svec_sp40.vz < 0 && var_t0 < 0) {
               ct1++;
            }

            if (svec_sp40.vx == var_a1) {
               ct1++;
            }
            if (svec_sp40.vy == var_a0) {
               ct1++;
            }
            if (svec_sp40.vz == var_t0) {
               ct1++;
            }

            var_a1 = gSideLightNormals[iVar5].vx;
            var_a0 = gSideLightNormals[iVar5].vy;
            var_t0 = gSideLightNormals[iVar5].vz;

            if (svec_sp40.vx > 0 && var_a1 > 0) {
               ct2++;
            } else if (svec_sp40.vx < 0 && var_a1 < 0) {
               ct2++;
            }
            if (svec_sp40.vy > 0 && var_a0 > 0) {
               ct2++;
            } else if (svec_sp40.vy < 0 && var_a0 < 0) {
               ct2++;
            }
            if (svec_sp40.vz > 0 && var_t0 > 0) {
               ct2++;
            } else if (svec_sp40.vz < 0 && var_t0 < 0) {
               ct2++;
            }

            if (svec_sp40.vx == var_a1) {
               ct2++;
            }
            if (svec_sp40.vy == var_a0) {
               ct2++;
            }
            if (svec_sp40.vz == var_t0) {
               ct2++;
            }

            if (ct1 > ct2) {
               iVar5 = j;
            }
         }

         tileModel->shades[i] = iVar5;
      }
   }

   PopMatrix();
}
