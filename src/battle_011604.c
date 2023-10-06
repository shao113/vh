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
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(101));
      gPlayerControlSuppressed = 1;
      gSignal5 = 0;

      gCameraRotation.vy &= 0xfff;
      EVT.dstRotY = func_800C4150(sprite->z1.s.hi, sprite->x1.s.hi, 1);
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
         if (diff > DEG(180)) {
            EVT.dstRotY -= DEG(360);
         }
      } else if (diff < DEG(-180)) {
         EVT.dstRotY += DEG(360);
      }

      EVT.timer = 45;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      EVT.dstGeomOfsY = 190;
      evt->state++;
      // fallthrough
   case 1:
      if (EVT.timer == 12) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(101));
      }
      if (--EVT.timer != 0) {
         gCameraPos.vx += (-(sprite->x1.n >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(sprite->z1.n >> 3) - gCameraPos.vz) >> 2;
         gCameraPos.vy += ((sprite->y1.n >> 3) - gCameraPos.vy) >> 2;

         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += (0 - gCameraRotation.vz) >> 2;

         gCameraZoom.vz += (EVT.dstZoom - gCameraZoom.vz) >> 2;

         EVT.geomOfsY += (EVT.dstGeomOfsY - EVT.geomOfsY) >> 3;
         SetGeomOffset(160, EVT.geomOfsY);
         gGeomOffsetY = EVT.geomOfsY;
      } else {
         OBJ_TILE_STATE(sprite).action = TA_LEVEL_UP;
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
            if (diff > DEG(180)) {
               EVT.camSavedRotY -= DEG(360);
            }
         } else if (diff < DEG(-180)) {
            EVT.camSavedRotY += DEG(360);
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
      EVT.todo_x26 += DEG(8.4375);
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
      if (EVT.todo_x26 >= DEG(90)) {
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
         EVT.dstCamRotY = func_800C4150(sprite->z1.s.hi, sprite->x1.s.hi, 0);
      } else {
         // ?: ranged
         EVT.dstCamRotY = func_800C3F50(sprite->z1.s.hi, sprite->x1.s.hi, 1);
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
         if (diff > DEG(180)) {
            EVT.dstCamRotY -= DEG(360);
         }
      } else if (diff < DEG(-180)) {
         EVT.dstCamRotY += DEG(360);
      }

      EVT.timer = 35;
      EVT.savedGeomOfsY = gGeomOffsetY;
      EVT.geomOfsY = gGeomOffsetY;
      EVT.dstGeomOfsY = 150;
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer != 0) {
         gCameraPos.vx += (-(sprite->x1.n >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(sprite->z1.n >> 3) - gCameraPos.vz) >> 2;
         gCameraPos.vy += ((sprite->y1.n >> 3) - gCameraPos.vy) >> 2;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += (0 - gCameraRotation.vz) >> 2;
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
            if (diff > DEG(180)) {
               EVT.camSavedRotY -= DEG(360);
            }
         } else if (diff < DEG(-180)) {
            EVT.camSavedRotY += DEG(360);
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
   EvtData *target;
   s16 diff;

   target = EVT.target;

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
         EVT.dstCamRotY = GetBestViewOfTarget(target->z1.s.hi, target->x1.s.hi, 1);
         break;
      case 1:
         EVT.dstCamRotY = func_800C4150(target->z1.s.hi, target->x1.s.hi, EVT.todo_x44);
         break;
      case 2:
         EVT.dstCamRotY = func_800C3F50(target->z1.s.hi, target->x1.s.hi, EVT.todo_x44);
         break;
      case 3:
         EVT.dstCamRotY = func_800C3D50(target->z1.s.hi, target->x1.s.hi, EVT.todo_x44);
         break;
      }

      diff = EVT.dstCamRotY - gCameraRotation.vy;
      if (diff > 0) {
         if (diff > DEG(180)) {
            EVT.dstCamRotY -= DEG(360);
         }
      } else if (diff < DEG(-180)) {
         EVT.dstCamRotY += DEG(360);
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
         gCameraPos.vx += (-(target->x1.n >> 3) - gCameraPos.vx) >> 2;
         gCameraPos.vz += (-(target->z1.n >> 3) - gCameraPos.vz) >> 2;
         if (evt->functionIndex == EVTF_CAMERA_TBD_588) {
            gCameraPos.vy += (((target->y1.n + CV(0.5)) >> 3) - gCameraPos.vy) >> 2;
         } else {
            gCameraPos.vy += ((target->y1.n >> 3) - gCameraPos.vy) >> 2;
         }
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
         gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 4;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         gCameraRotation.vz += (0 - gCameraRotation.vz) >> 2;
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
      otherEvt = EVT.unit->battler;
      otherEvt = otherEvt->d.evtf014.sprite;
      gSignal4 = otherEvt->d.sprite.direction;
      evt->x1.n = otherEvt->x1.n;
      evt->z1.n = otherEvt->z1.n;
      evt->y1.n = otherEvt->y1.n + CV(0.5);
      evt->state++;
   // fallthrough
   case 1:
      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
         dir = DEG(0);
         break;
      case CAM_DIR_EAST:
         dir = DEG(270);
         break;
      case CAM_DIR_NORTH:
         dir = DEG(180);
         break;
      case CAM_DIR_WEST:
         dir = DEG(90);
         break;
      }

      if (gPadStateNewPresses & PAD_UP) {
         gSignal4 = dir + DEG(0);
      }
      if (gPadStateNewPresses & PAD_RIGHT) {
         gSignal4 = dir + DEG(90);
      }
      if (gPadStateNewPresses & PAD_DOWN) {
         gSignal4 = dir + DEG(180);
      }
      if (gPadStateNewPresses & PAD_LEFT) {
         gSignal4 = dir + DEG(270);
      }
      gSignal4 &= 0xfff;

      if (gPadStateNewPresses & PAD_X) {
         gSignal2 = 2;
         evt->functionIndex = EVTF_NULL;
         return;
      }

      EVT.angle += DEG(45);

      fInner = (rcos(EVT.angle & 0xfff) * 50 >> 12) + CV(0.75);
      fOuter = (rcos(EVT.angle & 0xfff) * 130 >> 12) + CV(2.0);
      nfInner = (rcos(EVT.angle & 0xfff) * 25 >> 12) + CV(0.75);
      nfOuter = (rcos(EVT.angle & 0xfff) * 50 >> 12) + CV(1.5);
      otherEvt = Evt_GetUnused();

      RenderDirectionArrow(evt, otherEvt, DEG(0), gSignal4 == DEG(0), fInner, fOuter, nfInner,
                           nfOuter);
      RenderDirectionArrow(evt, otherEvt, DEG(90), gSignal4 == DEG(90), fInner, fOuter, nfInner,
                           nfOuter);
      RenderDirectionArrow(evt, otherEvt, DEG(180), gSignal4 == DEG(180), fInner, fOuter, nfInner,
                           nfOuter);
      RenderDirectionArrow(evt, otherEvt, DEG(270), gSignal4 == DEG(270), fInner, fOuter, nfInner,
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
      angle2 = angle - DEG(33.75);
      angle3 = angle + DEG(33.75);
      angle0 = angle - DEG(11.25);
      angle1 = angle + DEG(11.25);
      inner = fInner;
      outer = fOuter;
      sprite->d.sprite.gfxIdx = GFX_DIRECTION_ARROW_FILLED;
   } else {
      angle2 = angle - DEG(16.875);
      angle3 = angle + DEG(16.875);
      angle0 = angle - DEG(8.4375);
      angle1 = angle + DEG(8.4375);
      inner = nfInner;
      outer = nfOuter;
      sprite->d.sprite.gfxIdx = GFX_DIRECTION_ARROW_EMPTY;
   }

   sprite->d.sprite.coords[0].x = owner->x1.n + (rcos((angle0 - DEG(90)) & 0xfff) * outer >> 12);
   sprite->d.sprite.coords[0].z = owner->z1.n + (rcos(angle0 & 0xfff) * outer >> 12);

   sprite->d.sprite.coords[2].x = owner->x1.n + (rcos((angle2 - DEG(90)) & 0xfff) * inner >> 12);
   sprite->d.sprite.coords[2].z = owner->z1.n + (rcos(angle2 & 0xfff) * inner >> 12);

   sprite->d.sprite.coords[1].x = owner->x1.n + (rcos((angle1 - DEG(90)) & 0xfff) * outer >> 12);
   sprite->d.sprite.coords[1].z = owner->z1.n + (rcos(angle1 & 0xfff) * outer >> 12);

   sprite->d.sprite.coords[3].x = owner->x1.n + (rcos((angle3 - DEG(90)) & 0xfff) * inner >> 12);
   sprite->d.sprite.coords[3].z = owner->z1.n + (rcos(angle3 & 0xfff) * inner >> 12);

   sprite->d.sprite.coords[0].y = owner->y1.n;
   sprite->d.sprite.coords[1].y = owner->y1.n;
   sprite->d.sprite.coords[2].y = owner->y1.n;
   sprite->d.sprite.coords[3].y = owner->y1.n;

   sprite->x1.n = sprite->d.sprite.coords[2].x;
   sprite->z1.n = sprite->d.sprite.coords[2].z;
   sprite->y1.n = sprite->d.sprite.coords[2].y;
   sprite->d.sprite.otOfs = 10;
   AddEvtPrim5(gGraphicsPtr->ot, sprite);
}
