#include "common.h"
#include "evt.h"
#include "audio.h"
#include "battle.h"

#undef EVTF
#define EVTF 571
void Evtf571_LevelUp(EvtData *evt) {
   EvtData *sprite;
   s16 diff;

   sprite = EVT.sprite;

   switch (evt->state) {
   case 0:
      PerformAudioCommand(0x50);
      PerformAudioCommand(0x1365);
      gPlayerControlSuppressed = 1;
      gSignal5 = 0;

      gCameraRotation.vy &= 0xfff;
      EVT.dstRotY = func_800C4150(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 1);
      EVT.camSavedX = gCameraPos.vx;
      EVT.camSavedZ = gCameraPos.vz;
      EVT.camSavedY = gCameraPos.vy;

      EVT.camSavedRotX = gCameraRotation.vx;
      EVT.camSavedRotZ = gCameraRotation.vz;
      EVT.camSavedRotY = gCameraRotation.vy;

      EVT.camSavedZoom = gCameraZoom.vz;
      EVT.dstZoom = 250;

      diff = EVT.dstRotY - gCameraRotation.vy;
      if (diff > 0) {
         if (diff > ANGLE_180_DEGREES) {
            EVT.dstRotY -= ANGLE_360_DEGREES;
         }
      } else if (diff < -ANGLE_180_DEGREES) {
         EVT.dstRotY += ANGLE_360_DEGREES;
      }

      EVT.delay = 45;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      EVT.dstGeomOfsY = 190;
      evt->state++;
      // fallthrough
   case 1:
      if (EVT.delay == 12) {
         PerformAudioCommand(0x365);
      }
      if (--EVT.delay != 0) {
         gCameraPos.vx += (-(sprite->d.sprite.x1 >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(sprite->d.sprite.z1 >> 3) - gCameraPos.vz) >> 2;
         gCameraPos.vy += ((sprite->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;

         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (0x180 - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += -gCameraRotation.vz >> 2;

         gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> 2;

         EVT.geomOfsY += (EVT.dstGeomOfsY - EVT.geomOfsY) >> 3;
         SetGeomOffset(160, EVT.geomOfsY);
         gGeomOffsetY = EVT.geomOfsY;
      } else {
         gTileStateGridPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].action = TA_LEVEL_UP;
         gSignal3 = 0;
         evt->state++;
      }
      break;
   case 2:
      if (gSignal3 != 0) {
         EVT.delay = 1;
         evt->state++;
      }
      break;
   case 3:
      if (--EVT.delay == 0) {
         EVT.delay = 25;
         evt->state++;
         gCameraRotation.vy &= 0xfff;

         diff = EVT.camSavedRotY - gCameraRotation.vy;
         if (diff > 0) {
            if (diff > ANGLE_180_DEGREES) {
               EVT.camSavedRotY -= ANGLE_360_DEGREES;
            }
         } else if (diff < -ANGLE_180_DEGREES) {
            EVT.camSavedRotY += ANGLE_360_DEGREES;
         }
      }
      break;
   case 4:
      if (--EVT.delay != 0) {
         gCameraPos.vx += (EVT.camSavedX - gCameraPos.vx) >> 2;
         gCameraPos.vz += (EVT.camSavedZ - gCameraPos.vz) >> 2;
         gCameraPos.vy += (EVT.camSavedY - gCameraPos.vy) >> 2;

         gCameraRotation.vx += (EVT.camSavedRotX - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += (EVT.camSavedRotZ - gCameraRotation.vz) >> 2;
         gCameraRotation.vy += (EVT.camSavedRotY - gCameraRotation.vy) >> 2;

         gCameraZoom.vz += (EVT.camSavedZoom - gCameraZoom.vz) >> 2;

         EVT.geomOfsY += (EVT.savedGeomOfsY - EVT.geomOfsY) >> 2;
         SetGeomOffset(160, EVT.geomOfsY);
         gGeomOffsetY = EVT.geomOfsY;
      } else {
         gCameraRotation.vx = EVT.camSavedRotX;
         gCameraRotation.vz = EVT.camSavedRotZ;
         gCameraRotation.vy = EVT.camSavedRotY;
         gCameraZoom.vz = EVT.camSavedZoom;
         SetGeomOffset(160, EVT.savedGeomOfsY);
         gGeomOffsetY = EVT.savedGeomOfsY;
         gSignal5 = 100;
         gPlayerControlSuppressed = 0;
         evt->functionIndex = EVTF_NULL;
         PerformAudioCommand(0x51);
      }
   }
}
