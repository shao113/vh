#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

s32 GetIdxOfHighestVtx(MapTileModel *tileModel) {
   s32 i, j;
   s32 highest;
   s32 faceCt;

   faceCt = tileModel->faceCt;
   highest = 0;

   for (i = 0; i < faceCt; i++) {
      for (j = 0; j < 4; j++) {
         if (tileModel->vertices[tileModel->faces[i][j]].vy < tileModel->vertices[highest].vy) {
            highest = tileModel->faces[i][j];
         }
      }
   }

   return highest;
}

#undef EVTF
#define EVTF 672
void Evtf672_Map39_SplashingTile(EvtData *evt) {
   MapTileModel *tileModel;
   EvtData *splash;
   s16 y;

   tileModel = EVT.tileModel;

   switch (evt->state) {
   case 0:
      evt->mem = GetIdxOfHighestVtx(EVT.tileModel);
      evt->state++;
      break;

   case 1:
      y = -tileModel->vertices[evt->mem].vy << 4; //?
      if (y > evt->y1.n - CV(0.5)) {
         evt->state3 = rand() % 16;
         evt->state++;
      }
      break;

   case 2:
      if (--evt->state3 <= 0) {
         splash = CreatePositionedEvt(evt, EVTF_SPLASH);
         // TODO replace
         splash->d.sprite.coords[0].x = CV(0.25) * rsin(evt->state2 * DEG(22.5)) >> 12;
         evt->state3 += (rand() % 6 + 6);
         evt->state2++;
      }
      if (evt->state2 >= 8) {
         evt->state++;
      }
      break;

   case 3:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void SwapTiles(MapTileModel *one, MapTileModel *two) {
   MapTileModel swap;

   swap = *one;
   *one = *two;
   *two = swap;
}

void RenderArea(s32 x1, s32 z1, s32 x2, s32 z2) {
   s32 ix, iz;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         RenderMapTile(gGraphicsPtr->ot, &gMapRowPointers[iz][ix], GRID_COLOR_NONE);
      }
   }
}

void AdjustAreaElevation(s32 x1, s32 z1, s32 x2, s32 z2, s16 amount) {
   s32 ix, iz;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         AdjustTileElevation(&gMapRowPointers[iz][ix], amount);
      }
   }
}

void ShiftTile(MapTileModel *tileModel, s16 dx, s16 dz) {
   s32 i;

   for (i = 0; i < ARRAY_COUNT(tileModel->vertices); i++) {
      tileModel->vertices[i].vx += dx;
      tileModel->vertices[i].vz += dz;
   }
}

void ShiftArea(s32 x1, s32 z1, s32 x2, s32 z2, s16 dx, s16 dz) {
   s32 ix, iz;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         ShiftTile(&gMapRowPointers[iz][ix], dx, dz);
      }
   }
}

s32 Map39_GetTileTerrainType(MapTileModel *tileModel) {
   s32 gfxIdx;
   s32 terrain;

   gfxIdx = tileModel->gfx[0];
   terrain = TERRAIN_NO_ENTRY;

   if (gfxIdx >= GFX_MAP_TEXTURE_5) {
      terrain = TERRAIN_BARREN;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_21) {
      terrain = TERRAIN_VILE_BOG;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_25) {
      terrain = TERRAIN_PRAIRIE;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_51) {
      terrain = TERRAIN_WATER;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_54) {
      terrain = TERRAIN_BOUNDARY;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_55) {
      terrain = TERRAIN_THICKET;
   }
   if (gfxIdx >= GFX_MAP_TEXTURE_56) {
      terrain = TERRAIN_PLAINS;
   }
   return terrain;
}

void Map39_UpdateTerrainType(s16 x, s16 z) {
   gTerrainPtr[z][x].s.terrain = Map39_GetTileTerrainType(&gMapRowPointers[z][x]);
}

void Map39_SwapSection(s32 x1, s32 z1, s32 x2, s32 z2) {
   // The temple and platform tiles are stored beyond the visible bounds, and this is used to swap
   // in sections as they are revealed.

   s32 ix, iz;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         SwapTiles(&gMapRowPointers[iz][ix + 28], &gMapRowPointers[iz][ix]);
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         Map39_UpdateTerrainType(ix, iz);
         UpdateTileElevation(iz, ix);
      }
   }
}

void UpdateAreaElevation(s32 x1, s32 z1, s32 x2, s32 z2) {
   s32 ix, iz;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
         UpdateTileElevation(iz, ix);
      }
   }
}

void Map39_SetupSectionSplashing(s32 x1, s32 z1, s32 x2, s32 z2) {
   s32 ix, iz;
   EvtData *splash;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         if (Map39_GetTileTerrainType(&gMapRowPointers[iz][ix]) != TERRAIN_WATER) {
            splash = Evt_GetUnused();
            splash->functionIndex = EVTF_MAP39_SPLASHING_TILE;
            splash->x1.n = ix * CV(1.0) + CV(0.5);
            splash->z1.n = iz * CV(1.0) + CV(0.5);
            splash->y1.n = CV(0.5);
            splash->d.evtf672.tileModel = &gMapRowPointers[iz][ix];
         }
      }
   }
}

void Map39_SetupSectionSplashing_2(s32 x1, s32 z1, s32 x2, s32 z2) {
   // Duplicate of previous function

   s32 ix, iz;
   EvtData *splash;

   for (ix = x1; ix < x2; ix++) {
      for (iz = z1; iz < z2; iz++) {
         if (Map39_GetTileTerrainType(&gMapRowPointers[iz][ix]) != TERRAIN_WATER) {
            splash = Evt_GetUnused();
            splash->functionIndex = EVTF_MAP39_SPLASHING_TILE;
            splash->x1.n = ix * CV(1.0) + CV(0.5);
            splash->z1.n = iz * CV(1.0) + CV(0.5);
            splash->y1.n = CV(0.5);
            splash->d.evtf672.tileModel = &gMapRowPointers[iz][ix];
         }
      }
   }
}

#undef EVTF
#define EVTF 754
void Evtf754_Map39_Scn82(EvtData *evt) {
   EvtData *focus;

   focus = EVT.focus;

   switch (evt->state) {
   case 0:
      AdjustAreaElevation(28, 0, 56, 14, 256);
      ShiftArea(28, 0, 56, 14, CV(-28.0) >> 3, 0);
      focus = Evt_GetUnused();
      focus->functionIndex = EVTF_NOOP;
      EVT.focus = focus;
      PerformAudioCommand(AUDIO_CMD_PREPARE_XA(32));
      evt->state++;

   // fallthrough
   case 1:
      evt->state2 = 0;
      evt->state++;
      gCameraRotation.vy |= 0xf000;
      gCameraRotation.vx &= 0xfff;
      AssignFromMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 1;
      focus->x1.n = CV(16.0);
      focus->y1.n = CV(1.0);
      focus->z1.n = CV(11.0);
      gState.focus = focus;
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         gCameraRotation.vy += (DEG(45) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (512 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 24) {
            evt->x1.n = CV(16.0);
            evt->z1.n = CV(11.0);
            evt->y1.n = CV(1.0);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         Map39_SwapSection(19, 5, 24, 14);
         Map39_SwapSection(15, 7, 19, 14);
         Map39_SetupSectionSplashing(19, 5, 24, 14);
         Map39_SetupSectionSplashing(15, 7, 19, 14);
         evt->state3 = 0;
         evt->state2 = 0;
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(32));
         evt->state++;
         break;
      }

      break;

   case 3:
      RenderArea(47, 5, 52, 14);
      RenderArea(43, 7, 47, 14);

      switch (evt->state2) {
      case 0:
         AdjustAreaElevation(19, 5, 24, 14, -1);
         AdjustAreaElevation(15, 7, 19, 14, -1);
         if (++evt->state3 >= 192) {
            evt->x1.n += CV(3.0);
            evt->z1.n -= CV(2.0);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         AdjustAreaElevation(19, 5, 24, 14, -1);
         AdjustAreaElevation(15, 7, 19, 14, -1);
         gCameraRotation.vy += (DEG(-45) - gCameraRotation.vy) >> 4;
         if (++evt->state3 >= 64) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 2:
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 4);
            evt->mem = rand() % 8;
         }
         if (++evt->state3 >= 32) {
            UpdateAreaElevation(19, 5, 24, 14);
            UpdateAreaElevation(15, 7, 19, 14);
            evt->state++;
            evt->state2 = 0;
            evt->state3 = 0;
         }
         break;
      }

      break;

   case 4:
      evt->state++;

   // fallthrough
   case 5:
      RenderArea(28, 0, 35, 14);

      switch (evt->state2) {
      case 0:
         focus->x1.n = CV(2.0);
         focus->y1.n = CV(2.0);
         focus->z1.n = CV(7.0);
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 8);
            evt->mem = rand() % 8;
         }
         gCameraRotation.vy += (DEG(157.5) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (512 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(45) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 48) {
            Map39_SwapSection(0, 0, 4, 4);
            Map39_SwapSection(0, 4, 7, 14);
            Map39_SetupSectionSplashing(0, 0, 7, 14);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 8);
            evt->mem = rand() % 8;
         }
         AdjustAreaElevation(0, 0, 4, 4, -1);
         AdjustAreaElevation(0, 4, 7, 14, -1);
         gCameraRotation.vy -= DEG(0.52734375);
         gCameraRotation.vx = DEG(45) + (DEG(22.5) * rsin(evt->state3 * 16) >> 12);
         if (++evt->state3 >= 256) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 2:
         if (++evt->state3 >= 128) {
            UpdateAreaElevation(0, 0, 4, 4);
            UpdateAreaElevation(0, 4, 7, 14);
            evt->state3 = 0;
            evt->state2 = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 6:
      evt->state++;

   // fallthrough
   case 7:
      evt->state++;
      gState.focus = NULL;
      gCameraRotation.vy &= 0xfff;
      gCameraRotation.vx &= 0xfff;

   // fallthrough
   case 8:
      EaseOutCamera(&EVT.camera, 3);
      focus->x1.n = EVT.camera.posX;
      focus->y1.n = EVT.camera.posY;
      focus->z1.n = EVT.camera.posZ;
      if (++evt->state3 >= 32) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 9:
      AssignToMainCamera(&EVT.camera);
      evt->functionIndex = EVTF_NULL;
      break;
   }

   gState.eventCameraRot = gCameraRotation.vy;
}

s32 s_unused_80123344;

#undef EVTF
#define EVTF 656
void Evtf656_Map39(EvtData *evt) {
   s32 amount;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 <= 1) {
         gState.mapState.s.field_0x0 = 1;
         evt->state = 12;
      } else {
         amount = -256;
         if (gState.mapState.s.field_0x0 >= 2) {
            AdjustAreaElevation(37, 0, 40, 4, amount);
            AdjustAreaElevation(40, 0, 43, 10, amount);
            AdjustAreaElevation(43, 0, 47, 7, amount);
            AdjustAreaElevation(47, 0, 52, 5, amount);
            Map39_SwapSection(9, 0, 12, 4);
            Map39_SwapSection(12, 0, 15, 10);
            Map39_SwapSection(15, 0, 19, 7);
            Map39_SwapSection(19, 0, 24, 5);
            evt->state = 18;
         }
         if (gState.mapState.s.field_0x0 >= 3) {
            AdjustAreaElevation(35, 4, 40, 11, -256);
            AdjustAreaElevation(39, 11, 43, 12, -256);
            Map39_SwapSection(7, 4, 12, 11);
            Map39_SwapSection(11, 11, 15, 12);
            evt->functionIndex = EVTF_NULL;
            return;
         }
         if (gState.mapState.s.field_0x0 == 0) {
            evt->state++;
         }
      }

      break;

   case 1:
      AdjustAreaElevation(28, 0, 56, 14, 256);
      evt->state++;

   // fallthrough
   case 2:
      ShiftArea(28, 0, 56, 14, CV(-28.0) >> 3, 0);
      evt->state++;

   // fallthrough
   case 3:
      if (gState.mapState.s.field_0x0 != 0) {
         evt->state2 = 0;
         evt->state++;
         gCameraRotation.vy &= 0xfff;
         gCameraRotation.vx &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 1;
      }
      break;

   case 4:

      switch (evt->state2) {
      case 0:
         PanCamera(CV(16.0), CV(1.0), CV(11.0), 2);
         gCameraRotation.vy += (DEG(45) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (512 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 24) {
            evt->x1.n = CV(16.0);
            evt->z1.n = CV(11.0);
            evt->y1.n = CV(1.0);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
         Map39_SwapSection(19, 5, 24, 14);
         Map39_SwapSection(15, 7, 19, 14);
         Map39_SetupSectionSplashing(19, 5, 24, 14);
         Map39_SetupSectionSplashing(15, 7, 19, 14);
         evt->state3 = 0;
         evt->state2 = 0;
         evt->state++;
         break;
      }

      break;

   case 5:
      RenderArea(47, 5, 52, 14);
      RenderArea(43, 7, 47, 14);

      switch (evt->state2) {
      case 0:
         AdjustAreaElevation(19, 5, 24, 14, -1);
         AdjustAreaElevation(15, 7, 19, 14, -1);
         if (++evt->state3 >= 192) {
            evt->x1.n += CV(3.0);
            evt->z1.n -= CV(2.0);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         AdjustAreaElevation(19, 5, 24, 14, -1);
         AdjustAreaElevation(15, 7, 19, 14, -1);
         gCameraRotation.vy += (DEG(-45) - gCameraRotation.vy) >> 4;
         PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
         if (++evt->state3 >= 64) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 2:
         PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 4);
            evt->mem = rand() % 8;
         }
         if (++evt->state3 >= 32) {
            UpdateAreaElevation(19, 5, 24, 14);
            UpdateAreaElevation(15, 7, 19, 14);
            evt->state++;
            evt->state2 = 0;
            evt->state3 = 0;
         }
         break;
      }

      break;

   case 6:
      evt->state++;

   // fallthrough
   case 7:
      RenderArea(28, 0, 35, 14);

      switch (evt->state2) {
      case 0:
         PanCamera(CV(2.0), CV(2.0), CV(7.0), 3);
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 8);
            evt->mem = rand() % 8;
         }
         gCameraRotation.vy += (DEG(157.5) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (512 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(45) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 48) {
            Map39_SwapSection(0, 0, 4, 4);
            Map39_SwapSection(0, 4, 7, 14);
            Map39_SetupSectionSplashing(0, 0, 7, 14);
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         PanCamera(CV(2.0), CV(2.0), CV(7.0), 3);
         if (--evt->mem <= 0) {
            gCameraPos.vy -= (rand() % 8);
            evt->mem = rand() % 8;
         }
         AdjustAreaElevation(0, 0, 4, 4, -1);
         AdjustAreaElevation(0, 4, 7, 14, -1);
         gCameraRotation.vy -= DEG(0.52734375);
         gCameraRotation.vx = DEG(45) + (DEG(22.5) * rsin(evt->state3 * 16) >> 12);
         if (++evt->state3 >= 256) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 2:
         PanCamera(CV(2.0), CV(2.0), CV(7.0), 3);
         if (++evt->state3 >= 128) {
            UpdateAreaElevation(0, 0, 4, 4);
            UpdateAreaElevation(0, 4, 7, 14);
            evt->state3 = 0;
            evt->state2 = 0;
            evt->state++;
         }
         break;
      }

      break;

   case 8:
      evt->state++;

   // fallthrough
   case 9:
      evt->state++;

   // fallthrough
   case 10:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 11:
      AssignToMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 0;
      evt->state++;
      break;

   case 12:
      if (gState.mapState.s.field_0x0 >= 2) {
         evt->state2 = 0;
         evt->state++;
         gCameraRotation.vy &= 0xfff;
         gCameraRotation.vx &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(32));
         gPlayerControlSuppressed = 1;
      }
      break;

   case 13:

      switch (evt->state2) {
      case 0:
         PanCamera(CV(16.0), CV(1.0), CV(3.0), 3);
         gCameraRotation.vy += (DEG(270) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (640 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(45) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 48) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         Map39_SwapSection(9, 0, 12, 4);
         Map39_SwapSection(12, 0, 15, 10);
         Map39_SwapSection(15, 0, 19, 7);
         Map39_SwapSection(19, 0, 24, 5);
         Map39_SetupSectionSplashing(9, 0, 12, 4);
         Map39_SetupSectionSplashing(12, 0, 15, 10);
         Map39_SetupSectionSplashing(15, 0, 19, 7);
         Map39_SetupSectionSplashing(19, 0, 24, 5);
         evt->state3 = 0;
         evt->state++;
         break;
      }

      break;

   case 14:
      RenderArea(37, 0, 40, 4);
      RenderArea(40, 0, 43, 10);
      RenderArea(43, 0, 47, 7);
      RenderArea(47, 0, 52, 5);
      gCameraRotation.vy -= DEG(0.52734375);
      gCameraRotation.vx = DEG(45) + (DEG(22.5) * rsin(evt->state3 * 16) >> 12);
      AdjustAreaElevation(9, 0, 12, 4, -1);
      AdjustAreaElevation(12, 0, 15, 10, -1);
      AdjustAreaElevation(15, 0, 19, 7, -1);
      AdjustAreaElevation(19, 0, 24, 5, -1);
      evt->state3++;
      if (evt->state3 == 30) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(32));
      }
      if (evt->state3 >= 256) {
         evt->state++;
         evt->state3 = 0;
         PerformAudioCommand(AUDIO_CMD_FADE_OUT_8_1);
      }
      break;

   case 15:
      RenderArea(37, 0, 40, 4);
      RenderArea(40, 0, 43, 10);
      RenderArea(43, 0, 47, 7);
      RenderArea(47, 0, 52, 5);
      if (++evt->state3 >= 128) {
         UpdateAreaElevation(9, 0, 12, 4);
         UpdateAreaElevation(12, 0, 15, 10);
         UpdateAreaElevation(15, 0, 19, 7);
         UpdateAreaElevation(19, 0, 24, 5);
         evt->state3 = 0;
         evt->state++;
         EVT.camera.rotY = func_800A96A8(gCameraRotation.vy, EVT.camera.rotY);
      }
      gCameraRotation.vy -= DEG(0.52734375);
      break;

   case 16:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 17:
      AssignToMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 0;
      gState.field_0x96 = 0;
      evt->state++;
      break;

   case 18:
      if (gState.mapState.s.field_0x0 >= 3) {
         s_unused_80123344 = 0;
         evt->state2 = 0;
         evt->state++;
         gCameraRotation.vy &= 0xfff;
         gCameraRotation.vx &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         AdjustAreaElevation(35, 4, 40, 11, -64);
         AdjustAreaElevation(39, 11, 43, 12, -64);
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(32));
         gPlayerControlSuppressed = 1;
      }
      break;

   case 19:

      switch (evt->state2) {
      case 0:
         PanCamera(CV(9.0), CV(2.0), CV(7.0), 3);
         gCameraRotation.vy += (DEG(157.5) - gCameraRotation.vy) >> 2;
         gCameraZoom.vz += (640 - gCameraZoom.vz) >> 2;
         gCameraRotation.vx += (DEG(45) - gCameraRotation.vx) >> 2;
         if (++evt->state3 >= 48) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 1:
         Map39_SwapSection(7, 4, 12, 11);
         Map39_SwapSection(11, 11, 15, 12);
         Map39_SetupSectionSplashing_2(7, 4, 12, 11);
         Map39_SetupSectionSplashing_2(11, 11, 15, 12);
         evt->state3 = 0;
         evt->state++;
         break;
      }

      break;

   case 20:
      RenderArea(35, 4, 40, 11);
      RenderArea(39, 11, 43, 12);
      if (evt->state3 < 192) {
         AdjustAreaElevation(7, 4, 12, 11, -1);
         AdjustAreaElevation(11, 11, 15, 12, -1);
         evt->state3++;
      }
      gCameraRotation.vy -= DEG(0.52734375);
      gCameraRotation.vx = DEG(45);
      if (evt->state3 == 20) {
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(32));
      }
      if (evt->state3 == 192) {
         evt->state++;
         evt->state3 = 0;
         PerformAudioCommand(AUDIO_CMD_FADE_OUT_8_1);
      }
      break;

   case 21:
      RenderArea(35, 4, 40, 11);
      RenderArea(39, 11, 43, 12);
      if (++evt->state3 >= 128) {
         UpdateAreaElevation(7, 4, 12, 11);
         UpdateAreaElevation(11, 11, 15, 12);
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 22:
      EaseOutCamera(&EVT.camera, 3);
      if (++evt->state3 >= 32) {
         evt->state3 = 0;
         evt->state++;
      }
      break;

   case 23:
      AssignToMainCamera(&EVT.camera);
      gPlayerControlSuppressed = 0;
      gState.field_0x96 = 0;
      evt->state++;
      break;

   case 24:
      break;
   }
}

void Map39_Setup(void) {
   // Submerged tiles start at x+28 (out-of-view)
   ShiftArea(28, 0, 56, 14, CV(-28.0) >> 3, 0);
   AdjustAreaElevation(28, 0, 56, 14, 256);
   AdjustAreaElevation(47, 5, 52, 14, -256);
   AdjustAreaElevation(43, 7, 47, 14, -256);
   AdjustAreaElevation(28, 0, 35, 14, -256);
   Map39_SwapSection(19, 5, 24, 14);
   Map39_SwapSection(15, 7, 19, 14);
   Map39_SwapSection(0, 0, 4, 4);
   Map39_SwapSection(0, 4, 7, 14);
}

#undef EVTF
#define EVTF 703
void Evtf703_Map40_Barricade(EvtData *evt) {
   EvtData *lowerBarricade;

   switch (evt->state) {
   case 0:
      if (gState.mapState.s.field_0x0 == 0) {
         evt->state++;
      } else {
         Map40_FlattenBarricade();
         evt->functionIndex = EVTF_NULL;
      }
      break;

   case 1:
      if (gState.mapState.s.field_0x0 != 0) {
         lowerBarricade = Evt_GetUnused();
         lowerBarricade->functionIndex = EVTF_MAP40_LOWER_BARRICADE;
         evt->state++;
      }
      break;

   case 2:
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

#undef EVTF
#define EVTF 738
void Evtf738_Map40_LowerBarricade(EvtData *evt) {
   s32 i, j;
   EvtData *cloud;
   s32 tmp;

   switch (evt->state) {
   case 0:
      evt->state3 = 80;
      evt->y2.n = gCameraPos.vy;
      evt->state++;

   // fallthrough
   case 1:
      for (i = 16; i <= 17; i++) {
         for (j = 5; j <= 8; j++) {
            AdjustFaceElevation(&gMapRowPointers[j][i], 0, 1);
         }
         gGfxSubTextures[GFX_MAP_TEXTURE_232][3] = evt->state3 >> 1;
         gGfxSubTextures[GFX_MAP_TEXTURE_233][3] = evt->state3 >> 1;
      }
      if (--evt->z3.n <= 0) {
         for (i = 5; i <= 8; i++) {
            cloud = Evt_GetUnused();
            cloud->functionIndex = EVTF_SMOKE;
            cloud->x1.n = CV(16.0);
            cloud->z1.n = i * CV(1.0) + CV(0.5);
            cloud->y1.n = CV(1.5);
            cloud->x2.n = -rand() % CV(0.25);
            cloud->z2.n = rand() % CV(0.25) - CV(0.125);
            cloud->y2.n = rand() % CV(0.25);

            cloud = Evt_GetUnused();
            cloud->functionIndex = EVTF_SMOKE;
            cloud->x1.n = CV(18.0);
            cloud->z1.n = i * CV(1.0) + CV(0.5);
            cloud->y1.n = CV(1.5);
            cloud->x2.n = rand() % CV(0.25);
            cloud->z2.n = rand() % CV(0.25) - CV(0.125);
            cloud->y2.n = rand() % CV(0.25);
         }
         evt->z3.n += (rand() % 5 + 3);
      }
      gCameraPos.vy = evt->y2.n + rand() % 4;
      if (--evt->state3 <= 0) {
         for (i = 16; i <= 17; i++) {
            for (j = 5; j <= 8; j++) {
               UpdateTileElevation(j, i);
            }
         }
         gGfxSubTextures[GFX_MAP_TEXTURE_232][3] = 0;
         gGfxSubTextures[GFX_MAP_TEXTURE_233][3] = 0;
         gState.field_0x96 = 0;
         evt->functionIndex = EVTF_NULL;
      }
      break;

   case 3:
      gCameraPos.vy = evt->y2.n + rand() % 2;
      if (++evt->mem >= 32) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 4:
      rand(); //?
      tmp = evt->y2.n;
      gCameraPos.vy = tmp;
      if (++evt->mem >= 32) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 5:
      for (j = 13; j <= 15; j++) {
         gTerrainPtr[j][13].s.elevation = -gMapRowPointers[j][13].vertices[0].vy >> 4;
      }
      evt->functionIndex = EVTF_NULL;
      gCameraPos.vy = evt->y2.n;
      break;

   case 6:
      break;
   }
}

void Map40_FlattenBarricade(void) {
   s32 i, j;

   for (i = 16; i <= 17; i++) {
      for (j = 5; j <= 8; j++) {
         AdjustFaceElevation(&gMapRowPointers[j][i], 0, 80);
         UpdateTileElevation(j, i);
      }
   }
}
