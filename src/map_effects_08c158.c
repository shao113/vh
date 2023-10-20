#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

void DepressButton(s32 x, s32 z) {
   AdjustFaceElevation(&gMapRowPointers[z][x], 0, 16);
   gTerrainPtr[z][x].s.elevation = -gMapRowPointers[z][x].vertices[0].vy >> 4;
}

#undef EVTF
#define EVTF 346
void Evtf346_ButtonDepress(EvtData *evt) {
   switch (evt->state) {
   case 0:
      EVT.dstCamRotY = GetBestViewOfTarget(evt->z1.s.hi, evt->x1.s.hi, 1);
      gCameraRotation.vy &= 0xfff;
      EVT.dstCamRotY = func_800A96A8(gCameraRotation.vy, DEG(135));
      evt->state2 = 48;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(33));
      evt->state++;
      break;

   case 1:
      if (--evt->state2 <= 0) {
         gCameraRotation.vy = EVT.dstCamRotY;
         gCameraRotation.vx = DEG(33.75);
         evt->state3 = 4;
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(33));
         evt->state++;
      }
      PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
      gCameraRotation.vy += (EVT.dstCamRotY - gCameraRotation.vy) >> 3;
      gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 3;
      gCameraZoom.vz += (384 - gCameraZoom.vz) >> 3;
      break;

   case 2:
      AdjustFaceElevation(&OBJ_TILE_MODEL(evt), 0, 4);
      OBJ_TERRAIN(evt).s.elevation = -OBJ_TILE_MODEL(evt).vertices[0].vy >> 4;
      if (--evt->state3 <= 0) {
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 3:
      if (--evt->state3 <= 0) {
         evt->state = 99;
      }
      break;

   case 99:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 347
void Evtf347_Map26(EvtData *evt) {
   EvtData *buttonDepress;

   switch (evt->state) {
   case 0:
      evt->state++;
      if (gState.mapState.s.field_0x0 != 0) {
         evt->functionIndex = EVTF_NULL;
      }

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 1) {
         evt->state2 = 48;
         AssignFromMainCamera(&EVT.camera);

         buttonDepress = Evt_GetUnused();
         buttonDepress->functionIndex = EVTF_BUTTON_DEPRESS;
         buttonDepress->x1.n = CV(35.0);
         buttonDepress->z1.n = CV(8.0);
         EVT.buttonDepress = buttonDepress;

         gPlayerControlSuppressed = 1;
         evt->state++;
      }
      break;

   case 2:
      buttonDepress = EVT.buttonDepress;
      if (buttonDepress->state == 99) {
         evt->state++;
      }
      break;

   case 3:
      gState.mapState.s.field_0x0 = 2;
      evt->state++;
      break;

   case 4:
      if (gState.mapState.s.field_0x0 == 3) {
         gCameraRotation.vy &= 0xfff;
         EVT.camera.posX = func_800A96A8(gCameraRotation.vy, EVT.camera.posX);
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 5:
      EaseOutCamera(&EVT.camera, 2);
      if (--evt->state3 <= 0) {
         AssignToMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 0;
         gState.signal = 0;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 352
void Evtf352_Map29(EvtData *evt) {
   EvtData *buttonDepress;

   switch (evt->state) {
   case 0:
      evt->state++;
      if (gState.mapState.s.field_0x0 != 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 1) {
         evt->state2 = 48;
         AssignFromMainCamera(&EVT.camera);

         buttonDepress = Evt_GetUnused();
         buttonDepress->functionIndex = EVTF_BUTTON_DEPRESS;
         buttonDepress->x1.n = CV(18.0);
         buttonDepress->z1.n = CV(12.0);
         EVT.buttonDepress = buttonDepress;

         gPlayerControlSuppressed = 1;
         evt->state++;
      }
      break;

   case 2:
      buttonDepress = EVT.buttonDepress;
      if (buttonDepress->state == 99) {
         evt->state++;
      }
      break;

   case 3:
      gState.mapState.s.field_0x0 = 2;
      evt->state++;
      break;

   case 4:
      if (gState.mapState.s.field_0x0 == 3) {
         gCameraRotation.vy &= 0xfff;
         EVT.camera.posX = func_800A96A8(gCameraRotation.vy, EVT.camera.posX);
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 5:
      EaseOutCamera(&EVT.camera, 2);
      if (--evt->state3 <= 0) {
         AssignToMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 0;
         gState.signal = 0;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 667
void Evtf667_Map14_LowerSandTile(EvtData *evt) {
   extern void UpdateTileElevation(u8, u8);

   s32 tileZ, tileX;
   s32 i;
   MapTileModel *tileModel;

   tileModel = &OBJ_TILE_MODEL(evt);
   tileX = evt->x1.s.hi;
   tileZ = evt->z1.s.hi;

   switch (evt->state) {
   case 0:
      evt->state3 = 0;
      evt->state++;

   // fallthrough
   case 1:
      if (evt->mem > evt->state2) {
         AdjustFaceElevation(&gMapRowPointers[tileZ][tileX], 0, evt->state2);
         evt->mem -= evt->state2;
      } else {
         AdjustFaceElevation(&gMapRowPointers[tileZ][tileX], 0, evt->mem);
         evt->mem = 0;
         evt->state++;
      }
      UpdateMapTileLighting(tileModel);
      gTerrainPtr[tileZ][tileX].s.elevation = -gMapRowPointers[tileZ][tileX].vertices[0].vy >> 4;
      break;

   case 2:
      for (i = 1; i < 5; i++) {
         tileModel->gfx[i] = GFX_NULL;
      }
      UpdateTileElevation(tileZ, tileX);
      UpdateMapTileLighting(tileModel);
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 669
void Evtf669_Map14_RaiseSandTile(EvtData *evt) {
   extern void UpdateTileElevation(u8, u8);

   s32 tileZ, tileX;
   s32 i;
   MapTileModel *tileModel;

   tileModel = &OBJ_TILE_MODEL(evt);
   tileX = evt->x1.s.hi;
   tileZ = evt->z1.s.hi;

   switch (evt->state) {
   case 0:
      evt->state3 = 0;
      evt->state++;
      for (i = 1; i < 5; i++) {
         tileModel->gfx[i] = GFX_MAP_TEXTURE_1; // Flowing sand
      }

   // fallthrough
   case 1:
      if (evt->mem < evt->state2) {
         AdjustFaceElevation(&gMapRowPointers[tileZ][tileX], 0, evt->state2);
         evt->mem -= evt->state2;
      } else {
         AdjustFaceElevation(&gMapRowPointers[tileZ][tileX], 0, evt->mem);
         evt->mem = 0;
         evt->state++;
      }
      UpdateMapTileLighting(tileModel);
      gTerrainPtr[tileZ][tileX].s.elevation = -gMapRowPointers[tileZ][tileX].vertices[0].vy >> 4;
      break;

   case 2:
      UpdateTileElevation(tileZ, tileX);
      UpdateMapTileLighting(tileModel);
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Map14_AdjustSandTile(s32 x, s32 z, s32 amount) {
   extern void UpdateTileElevation(u8, u8);

   s32 i;
   MapTileModel *tileModel;

   if ((x >= 0 && x < 18) && (z >= 0 && z < 18)) {
      tileModel = &gMapRowPointers[z][x];
      AdjustFaceElevation(tileModel, 0, amount);
      UpdateMapTileLighting(tileModel);
      gTerrainPtr[z][x].s.elevation = -gMapRowPointers[z][x].vertices[0].vy >> 4;

      for (i = 1; i < 5; i++) {
         tileModel->gfx[i] = GFX_MAP_TEXTURE_1; // Flowing sand
      }
      UpdateTileElevation(z, x);
   }
}

void Map14_RaiseSandTile(s32 x, s32 z, s32 amount) {
   EvtData *obj;

   if ((x >= 0 && x < 18) && (z >= 0 && z < 18)) {
      obj = Evt_GetUnused();
      obj->functionIndex = EVTF_MAP14_RAISE_SAND_TILE;
      obj->x1.s.hi = x;
      obj->z1.s.hi = z;
      obj->mem = amount;
      obj->state2 = -2;
   }
}

void Map14_LowerSandTile(s32 x, s32 z, s32 amount) {
   EvtData *obj;

   if ((x >= 0 && x < 18) && (z >= 0 && z < 18)) {
      obj = Evt_GetUnused();
      obj->functionIndex = EVTF_MAP14_LOWER_SAND_TILE;
      obj->x1.s.hi = x;
      obj->z1.s.hi = z;
      obj->mem = amount;
      obj->state2 = 2;
   }
}

#undef EVTF
#define EVTF 668
void Evtf668_Map14_RaiseSandMound(EvtData *evt) {
   s32 tileZ, tileX;
   s32 i;
   s32 count, offset;
   s32 amount;
   s32 ix, iz;

   tileZ = evt->z1.s.hi;
   tileX = evt->x1.s.hi;

   switch (evt->state) {
   case 0:
      Map14_RaiseSandTile(tileX, tileZ, evt->mem * -16);
      evt->mem--;
      EVT.count = 1;
      EVT.offset = 1;
      evt->state++;
      evt->state3 = 0;
      break;

   case 1:
      if (++evt->state3 >= 8) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 2:
      tileZ = evt->z1.s.hi;
      tileX = evt->x1.s.hi;
      amount = evt->mem;
      if (amount <= 0) {
         evt->state += 2;
         return;
      }

      count = EVT.count;
      offset = EVT.offset;

      // 1
      iz = tileZ;
      ix = tileX - offset;

      for (i = 0; i < count; i++) {
         Map14_RaiseSandTile(ix, iz, amount * -16);
         iz++;
         ix++;
      }

      // 2
      iz = tileZ + offset;
      ix = tileX;

      for (i = 0; i < count; i++) {
         Map14_RaiseSandTile(ix, iz, amount * -16);
         iz--;
         ix++;
      }

      // 3
      iz = tileZ;
      ix = tileX + offset;

      for (i = 0; i < count; i++) {
         Map14_RaiseSandTile(ix, iz, amount * -16);
         iz--;
         ix--;
      }

      // 4
      iz = tileZ - offset;
      ix = tileX;

      for (i = 0; i < count; i++) {
         Map14_RaiseSandTile(ix, iz, amount * -16);
         iz++;
         ix--;
      }

      evt->state++;
      break;

   case 3:
      if (++evt->state3 >= 8) {
         EVT.count++;
         EVT.offset++;
         evt->mem--;
         evt->state = 2;
         evt->state3 = 0;
      }
      break;

   case 4:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void Map14_SetupSandMound(s32 x, s32 z, s32 size) {
   s32 i;
   s32 count, offset;
   s32 ix, iz;

   count = 1;
   offset = 1;

   Map14_AdjustSandTile(x, z, size * -16);

   while (--size > 0) {
      // 1
      iz = z;
      ix = x - offset;

      for (i = 0; i < count; i++) {
         Map14_AdjustSandTile(ix, iz, size * -16);
         iz++;
         ix++;
      }

      // 2
      iz = z + offset;
      ix = x;

      for (i = 0; i < count; i++) {
         Map14_AdjustSandTile(ix, iz, size * -16);
         iz--;
         ix++;
      }

      // 3
      iz = z;
      ix = x + offset;

      for (i = 0; i < count; i++) {
         Map14_AdjustSandTile(ix, iz, size * -16);
         iz--;
         ix--;
      }

      // 4
      iz = z - offset;
      ix = x;

      for (i = 0; i < count; i++) {
         Map14_AdjustSandTile(ix, iz, size * -16);
         iz++;
         ix--;
      }

      count++;
      offset++;
   }
}

#undef EVTF
#define EVTF 666
void Evtf666_Map14_LowerSandMound(EvtData *evt) {
   s32 tileZ, tileX;
   s32 i;
   s32 count, offset;
   s32 amount;
   s32 ix, iz;

   tileZ = evt->z1.s.hi;
   tileX = evt->x1.s.hi;
   amount = evt->mem;

   switch (evt->state) {
   case 0:
      Map14_LowerSandTile(tileX, tileZ, amount * 16);
      evt->mem--;
      EVT.count = 1;
      EVT.offset = 1;
      evt->state++;
      evt->state3 = 0;
      break;

   case 1:
      if (++evt->state3 >= 8) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 2:
      tileZ = evt->z1.s.hi;
      tileX = evt->x1.s.hi;
      amount = evt->mem;
      if (amount <= 0) {
         evt->state += 2;
         return;
      }

      count = EVT.count;
      offset = EVT.offset;

      // 1
      iz = tileZ;
      ix = tileX - offset;

      for (i = 0; i < count; i++) {
         Map14_LowerSandTile(ix, iz, amount * 16);
         iz++;
         ix++;
      }

      // 2
      iz = tileZ + offset;
      ix = tileX;

      for (i = 0; i < count; i++) {
         Map14_LowerSandTile(ix, iz, amount * 16);
         iz--;
         ix++;
      }

      // 3
      iz = tileZ;
      ix = tileX + offset;

      for (i = 0; i < count; i++) {
         Map14_LowerSandTile(ix, iz, amount * 16);
         iz--;
         ix--;
      }

      // 4
      iz = tileZ - offset;
      ix = tileX;

      for (i = 0; i < count; i++) {
         Map14_LowerSandTile(ix, iz, amount * 16);
         iz++;
         ix--;
      }

      evt->state++;
      break;

   case 3:
      if (++evt->state3 >= 8) {
         EVT.count++;
         EVT.offset++;
         evt->mem--;
         evt->state = 2;
         evt->state3 = 0;
      }
      break;

   case 4:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void AssignFromMainCamera(Camera *camera) {
   camera->rotX = gCameraRotation.vx;
   camera->rotY = gCameraRotation.vy;
   camera->rotZ = gCameraRotation.vz;
   camera->zoom = gCameraZoom.vz;
   camera->posX = gCameraPos.vx;
   camera->posY = gCameraPos.vy;
   camera->posZ = gCameraPos.vz;
}

void AssignToMainCamera(Camera *camera) {
   gCameraRotation.vx = camera->rotX;
   gCameraRotation.vy = camera->rotY;
   gCameraRotation.vz = camera->rotZ;
   gCameraZoom.vz = camera->zoom;
   gCameraPos.vx = camera->posX;
   gCameraPos.vy = camera->posY;
   gCameraPos.vz = camera->posZ;
}

void EaseOutShort(s16 *p, s16 target, s16 smoothness, s16 minStep) {
   s16 delta;
   s16 step;

   delta = target - *p;
   step = delta >> smoothness;

   if (abs(step) > minStep) {
      *p += step;
   } else if (abs(delta) > minStep) {
      *p += (step < 0) ? -minStep : +minStep;
   } else {
      *p += delta;
   }
}

void EaseOutInt(s32 *p, s32 target, s16 smoothness, s32 minStep) {
   s32 delta;
   s32 step;

   delta = target - *p;
   step = delta >> smoothness;

   if (abs(step) > minStep) {
      *p += step;
   } else if (abs(delta) > minStep) {
      *p += (step < 0) ? -minStep : +minStep;
   } else {
      *p += delta;
   }
}

void EaseOutCamera(Camera *camera, s16 smoothness) {
   EaseOutShort(&gCameraRotation.vx, camera->rotX, smoothness, 2);
   EaseOutShort(&gCameraRotation.vy, camera->rotY, smoothness, 2);
   EaseOutShort(&gCameraRotation.vz, camera->rotZ, smoothness, 2);
   EaseOutInt(&gCameraZoom.vz, camera->zoom, smoothness, 2);
   EaseOutShort(&gCameraPos.vx, camera->posX, smoothness, 2);
   EaseOutShort(&gCameraPos.vy, camera->posY, smoothness, 2);
   EaseOutShort(&gCameraPos.vz, camera->posZ, smoothness, 2);
}

#undef EVTF
#define EVTF 670
void Evtf670_Map14_Sand(EvtData *evt) {
   EvtData *mound;

   switch (evt->state) {
   case 0:
      evt->state2 = 0;
      if (gState.mapState.s.field_0x0 == 0) {
         Map14_SetupSandMound(6, 8, 6);
         evt->z1.s.hi = 8;
         evt->x1.s.hi = 6;
         evt->state = 3;
      } else {
         evt->z1.s.hi = gState.mapState.s.field_0x1;
         evt->x1.s.hi = gState.mapState.s.field_0x2;
         Map14_SetupSandMound(evt->x1.s.hi, evt->z1.s.hi, 6);
         evt->state = 3;
      }
      break;

   case 1:

      switch (evt->state2) {
      case 0:
         if (gState.mapState.s.field_0x0 == 1) {
            evt->z1.s.hi = gState.mapState.s.field_0x1;
            evt->x1.s.hi = gState.mapState.s.field_0x2;
            evt->state2++;
         }
         break;

      case 1:
         evt->state2++;

      // fallthrough
      case 2:
         evt->state2++;

      // fallthrough
      case 3:
         mound = Evt_GetUnused();
         mound->functionIndex = EVTF_MAP14_RAISE_SAND_MOUND;
         mound->z1.s.hi = evt->z1.s.hi;
         mound->x1.s.hi = evt->x1.s.hi;
         mound->mem = 6; // size
         EVT.mound = mound;
         evt->state2++;

      // fallthrough
      case 4:
         mound = EVT.mound;
         if (mound->functionIndex != EVTF_MAP14_RAISE_SAND_MOUND) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 5:
         evt->state2++;

      // fallthrough
      case 6:
         gState.field_0x96 = 0;
         evt->state2 = 0;
         evt->state++;
         break;
      } // switch (evt->state2) (via state:1)

      break;

   case 2:
      evt->state++;
      break;

   case 3:

      switch (evt->state2) {
      case 0:
         if (gState.mapState.s.field_0x0 == 2) {
            evt->state2++;
         }
         break;

      case 1:
         evt->state3 = 32;
         evt->state2++;
         evt->state3 = 0;

      // fallthrough
      case 2:
         evt->state2++;

      // fallthrough
      case 3:
         mound = Evt_GetUnused();
         mound->functionIndex = EVTF_MAP14_LOWER_SAND_MOUND;
         mound->z1.s.hi = evt->z1.s.hi;
         mound->x1.s.hi = evt->x1.s.hi;
         mound->mem = 6;
         EVT.mound = mound;
         evt->state2++;

      // fallthrough
      case 4:
         mound = EVT.mound;
         if (mound->functionIndex != EVTF_MAP14_LOWER_SAND_MOUND) {
            evt->state3 = 0;
            evt->state2++;
         }
         break;

      case 5:
         evt->state2++;

      // fallthrough
      case 6:
         if (--evt->state3 <= 0) {
            gState.field_0x96 = 0;
            evt->state2 = 0;
            evt->state++;
         }
         break;
      } // switch (evt->state2) (via state:3)

      break;

   case 4:
      evt->state = 1;
      break;
   }
}

#undef EVTF
#define EVTF 708
void Evtf708_709_Map14_Unused(EvtData *evt) {
   EvtData *sand;

   switch (evt->functionIndex) {
   case EVTF_MAP14_UNUSED_708:
      gState.mapState.s.field_0x0 = 2;
      break;
   case EVTF_MAP14_UNUSED_709:
      sand = Evt_GetUnused();
      sand->functionIndex = EVTF_MAP14_SAND;
      gState.mapState.s.field_0x0 = 1;
      gState.mapState.s.field_0x1 = 6;
      gState.mapState.s.field_0x2 = 8;
      break;
   }

   evt->functionIndex = EVTF_NULL;
}

#undef EVTF
#define EVTF 752
void Evtf752_Map14_Scn15_SandMoundSpawner(EvtData *evt) {
   EvtData *entitySprite;
   EvtData *mound;

   entitySprite = evt->d.entitySpawn.entitySpriteParam;
   mound = Evt_GetUnused();
   mound->functionIndex = EVTF_MAP14_RAISE_SAND_MOUND;
   mound->z1.s.hi = entitySprite->z1.s.hi;
   mound->x1.s.hi = entitySprite->x1.s.hi;
   mound->mem = 6;
   evt->functionIndex = EVTF_NULL;
}
