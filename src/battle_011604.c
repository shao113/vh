#include "common.h"
#include "evt.h"
#include "audio.h"
#include "battle.h"
#include "units.h"

//...
void RenderDirectionArrow(EvtData *, EvtData *, s16, u8, s16, s16, s16, s16);
//...

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

      EVT.timer = 45;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      EVT.dstGeomOfsY = 190;
      evt->state++;
      // fallthrough
   case 1:
      if (EVT.timer == 12) {
         PerformAudioCommand(0x365);
      }
      if (--EVT.timer != 0) {
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
         EVT.timer = 1;
         evt->state++;
      }
      break;
   case 3:
      if (--EVT.timer == 0) {
         EVT.timer = 25;
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
      if (--EVT.timer != 0) {
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

#undef EVTF
#define EVTF 024
void Evtf024_BounceZoom(EvtData *evt) {
   s16 tmp;

   switch (evt->state) {
   case 0:
      EVT.savedZoom = gCameraZoom.vz;
      // ?: no state++
   // fallthrough
   case 1:
      EVT.todo_x26 += 96;
      if (EVT.soft) {
         EVT.todo_x28 = (rcos(EVT.todo_x26 & 0xfff) * 5) * 2 >> 12;
      } else {
         EVT.todo_x28 = (rcos(EVT.todo_x26 & 0xfff) * 35) * 2 >> 12;
      }
      EVT.todo_x2a += 1000;
      if (EVT.soft) {
         tmp = (rcos(EVT.todo_x2a & 0xfff) * EVT.todo_x28) >> 12;
         gCameraZoom.vz = tmp + 320;
      } else {
         tmp = (rcos(EVT.todo_x2a & 0xfff) * EVT.todo_x28) >> 12;
         gCameraZoom.vz = tmp + 350;
      }
      if (EVT.todo_x26 >= 0x400) {
         gCameraZoom.vz = EVT.savedZoom;
         evt->functionIndex = EVTF_NULL;
      }
   }
}

#undef EVTF
#define EVTF 017
void Evtf017_Camera_TBD(EvtData *evt) {
   EvtData *sprite;
   s16 diff;

   sprite = EVT.sprite;

   switch (evt->state) {
   case 0:
      gCameraRotation.vy &= 0xfff;
      gPlayerControlSuppressed = 1;
      gSignal5 = 0;
      // ?: LO byte of camSavedX doubles as a caller-set arg for specifying melee/ranged
      if (LO(EVT.camSavedX) != 0) {
         // ?: melee
         EVT.dstCamRotY = func_800C4150(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 0);
      } else {
         // ?: ranged
         EVT.dstCamRotY = func_800C3F50(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 1);
      }
      EVT.camSavedX = gCameraPos.vx;
      EVT.camSavedZ = gCameraPos.vz;
      EVT.camSavedY = gCameraPos.vy;
      EVT.camSavedRotX = gCameraRotation.vx;
      EVT.camSavedRotZ = gCameraRotation.vz;
      EVT.camSavedRotY = gCameraRotation.vy;
      EVT.camSavedZoom = gCameraZoom.vz;
      EVT.dstZoom = 250;

      diff = EVT.dstCamRotY - gCameraRotation.vy;
      if (diff > 0) {
         if (diff > ANGLE_180_DEGREES) {
            EVT.dstCamRotY -= ANGLE_360_DEGREES;
         }
      } else if (diff < -ANGLE_180_DEGREES) {
         EVT.dstCamRotY += ANGLE_360_DEGREES;
      }

      EVT.timer = 35;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      EVT.dstGeomOfsY = 150;
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         gCameraPos.vx += (-(sprite->d.sprite.x1 >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(sprite->d.sprite.z1 >> 3) - gCameraPos.vz) >> 2;
         gCameraPos.vy += ((sprite->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (0x180 - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += -gCameraRotation.vz >> 2;
         gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> 2;
         EVT.geomOfsY += (EVT.dstGeomOfsY - EVT.geomOfsY) >> 2;
         SetGeomOffset(160, EVT.geomOfsY);
         gGeomOffsetY = EVT.geomOfsY;
      } else {
         gSignal5 = 1;
         evt->state++;
      }
      break;
   case 2:
      if (gSignal5 == 99) {
         evt->state++;
         EVT.timer = 20;
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
   case 3:
      if (--EVT.timer != 0) {
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
      }
      break;
   }
}

#undef EVTF
#define EVTF 026
void Evtf026_588_Camera_TBD(EvtData *evt) {
   EvtData *sprite;
   s16 diff;

   sprite = EVT.sprite;

   switch (evt->state) {
   case 0:
      EVT.todo_x44 = !EVT.todo_x44;
      gCameraRotation.vy &= 0xfff;

      if (EVT.zoom == 0) {
         EVT.dstZoom = 250;
         if (evt->functionIndex == EVTF_CAMERA_TBD_588) {
            EVT.dstZoom = 350;
         }
      } else {
         EVT.dstZoom = EVT.zoom;
      }

      switch (EVT.type) {
      case 0:
         EVT.dstCamRotY = GetBestViewOfTarget(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 1);
         break;
      case 1:
         EVT.dstCamRotY =
             func_800C4150(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), EVT.todo_x44);
         break;
      case 2:
         EVT.dstCamRotY =
             func_800C3F50(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), EVT.todo_x44);
         break;
      case 3:
         EVT.dstCamRotY =
             func_800C3D50(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), EVT.todo_x44);
         break;
      }

      diff = EVT.dstCamRotY - gCameraRotation.vy;
      if (diff > 0) {
         if (diff > ANGLE_180_DEGREES) {
            EVT.dstCamRotY -= ANGLE_360_DEGREES;
         }
      } else if (diff < -ANGLE_180_DEGREES) {
         EVT.dstCamRotY += ANGLE_360_DEGREES;
      }

      EVT.timer = 35;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      if (evt->functionIndex == EVTF_CAMERA_TBD_588) {
         EVT.dstGeomOfsY = gGeomOffsetY;
      } else {
         EVT.dstGeomOfsY = 150;
      }
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         gCameraPos.vx += (-(sprite->d.sprite.x1 >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(sprite->d.sprite.z1 >> 3) - gCameraPos.vz) >> 2;
         if (evt->functionIndex == EVTF_CAMERA_TBD_588) {
            gCameraPos.vy += (((sprite->d.sprite.y1 + 0x80) >> 3) - gCameraPos.vy) >> 2;
         } else {
            gCameraPos.vy += ((sprite->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;
         }
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (0x180 - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += -gCameraRotation.vz >> 2;
         gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> 2;
         EVT.geomOfsY += (EVT.dstGeomOfsY - EVT.geomOfsY) >> 2;
         SetGeomOffset(160, EVT.geomOfsY);
         gGeomOffsetY = EVT.geomOfsY;
      } else {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 410
void Evtf410_EventZoom(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.timer = 35;
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         gCameraZoom.vz += (EVT.zoom - gCameraZoom.vz) >> 2;
      } else {
         evt->functionIndex = EVTF_NULL;
      }
   }
}

#undef EVTF
#define EVTF 016
void Evtf016_ChooseDoneDirection(EvtData *evt) {
   s16 dir;
   s16 fInner, fOuter, nfInner, nfOuter;
   EvtData *otherEvt;

   dir = 0;

   switch (evt->state) {
   case 0:
      otherEvt = EVT.unit->evtBattler;
      otherEvt = otherEvt->d.evtf014.sprite;
      gSignal4 = otherEvt->d.sprite.direction;
      EVT.x = otherEvt->d.sprite.x1;
      EVT.z = otherEvt->d.sprite.z1;
      EVT.y = otherEvt->d.sprite.y1 + 0x80;
      evt->state++;
   // fallthrough
   case 1:
      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         dir = ANGLE_0_DEGREES;
         break;
      case CAM_DIR_EAST:
         dir = ANGLE_270_DEGREES;
         break;
      case CAM_DIR_NORTH:
         dir = ANGLE_180_DEGREES;
         break;
      case CAM_DIR_WEST:
         dir = ANGLE_90_DEGREES;
         break;
      }

      if (gPadStateNewPresses & PAD_UP) {
         gSignal4 = dir + ANGLE_0_DEGREES;
      }
      if (gPadStateNewPresses & PAD_RIGHT) {
         gSignal4 = dir + ANGLE_90_DEGREES;
      }
      if (gPadStateNewPresses & PAD_DOWN) {
         gSignal4 = dir + ANGLE_180_DEGREES;
      }
      if (gPadStateNewPresses & PAD_LEFT) {
         gSignal4 = dir + ANGLE_270_DEGREES;
      }
      gSignal4 &= 0xfff;

      if (gPadStateNewPresses & PAD_X) {
         gSignal2 = 2;
         evt->functionIndex = EVTF_NULL;
         return;
      }

      EVT.angle += ANGLE_45_DEGREES;

      fInner = (rcos(EVT.angle & 0xfff) * 50 >> 12) + 192;
      fOuter = (rcos(EVT.angle & 0xfff) * 130 >> 12) + 512;
      nfInner = (rcos(EVT.angle & 0xfff) * 25 >> 12) + 192;
      nfOuter = (rcos(EVT.angle & 0xfff) * 50 >> 12) + 384;
      otherEvt = Evt_GetUnused();

      RenderDirectionArrow(evt, otherEvt, 0, gSignal4 == 0, fInner, fOuter, nfInner, nfOuter);
      RenderDirectionArrow(evt, otherEvt, 0x400, gSignal4 == 0x400, fInner, fOuter, nfInner,
                           nfOuter);
      RenderDirectionArrow(evt, otherEvt, 0x800, gSignal4 == 0x800, fInner, fOuter, nfInner,
                           nfOuter);
      RenderDirectionArrow(evt, otherEvt, 0xc00, gSignal4 == 0xc00, fInner, fOuter, nfInner,
                           nfOuter);
   }

   if (gPadStateNewPresses & PAD_CIRCLE) {
      gSignal2 = 1;
      evt->functionIndex = EVTF_NULL;
      return;
   }
}

void RenderDirectionArrow(EvtData *owner, EvtData *sprite, s16 angle, u8 filled, s16 fInner,
                          s16 fOuter, s16 nfInner, s16 nfOuter) {
   s16 angle2, angle3, angle0, angle1;
   s16 inner, outer;

   if (filled) {
      angle2 = angle - 384;
      angle3 = angle + 384;
      angle0 = angle - 128;
      angle1 = angle + 128;
      inner = fInner;
      outer = fOuter;
      sprite->d.sprite.gfxIdx = GFX_DIRECTION_ARROW_FILLED;
   } else {
      angle2 = angle - 192;
      angle3 = angle + 192;
      angle0 = angle - 96;
      angle1 = angle + 96;
      inner = nfInner;
      outer = nfOuter;
      sprite->d.sprite.gfxIdx = GFX_DIRECTION_ARROW_EMPTY;
   }

   sprite->d.sprite.coords[0].x =
       owner->d.evtf016.x + (rcos((angle0 - ANGLE_90_DEGREES) & 0xfff) * outer >> 12);
   sprite->d.sprite.coords[0].z = owner->d.evtf016.z + (rcos(angle0 & 0xfff) * outer >> 12);

   sprite->d.sprite.coords[2].x =
       owner->d.evtf016.x + (rcos((angle2 - ANGLE_90_DEGREES) & 0xfff) * inner >> 12);
   sprite->d.sprite.coords[2].z = owner->d.evtf016.z + (rcos(angle2 & 0xfff) * inner >> 12);

   sprite->d.sprite.coords[1].x =
       owner->d.evtf016.x + (rcos((angle1 - ANGLE_90_DEGREES) & 0xfff) * outer >> 12);
   sprite->d.sprite.coords[1].z = owner->d.evtf016.z + (rcos(angle1 & 0xfff) * outer >> 12);

   sprite->d.sprite.coords[3].x =
       owner->d.evtf016.x + (rcos((angle3 - ANGLE_90_DEGREES) & 0xfff) * inner >> 12);
   sprite->d.sprite.coords[3].z = owner->d.evtf016.z + (rcos(angle3 & 0xfff) * inner >> 12);

   sprite->d.sprite.coords[0].y = owner->d.evtf016.y;
   sprite->d.sprite.coords[1].y = owner->d.evtf016.y;
   sprite->d.sprite.coords[2].y = owner->d.evtf016.y;
   sprite->d.sprite.coords[3].y = owner->d.evtf016.y;

   sprite->d.sprite.x1 = sprite->d.sprite.coords[2].x;
   sprite->d.sprite.z1 = sprite->d.sprite.coords[2].z;
   sprite->d.sprite.y1 = sprite->d.sprite.coords[2].y;
   sprite->d.sprite.otOfs = 10;
   AddEvtPrim5(gGraphicsPtr->ot, sprite);
}
