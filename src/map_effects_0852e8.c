#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "units.h"
#include "audio.h"

// TODO: Rename elevation stuff to disambiguate kind

s32 InEventScene(void) {
   if (gState.primary == STATE_4 || gState.primary == STATE_25 || gState.primary == STATE_16 ||
       gState.primary == STATE_17) {
      return 1;
   } else {
      return 0;
   }
}

void SetupMapExtras(void) {
   extern EvtData *D_801233CC;
   EvtData *evt;
   s32 i;

   evt = Evt_GetUnused();
   evt->functionIndex = EVTF_CAMERA_TBD_277;
   D_801233CC = evt;

   if (gState.mapNum == 11) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP11;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_HEAVY_RAINFALL;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(18.375);
      evt->z1.n = CV(2.375);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(13.375);
      evt->z1.n = CV(10.625);
   }

   if (gState.mapNum == 13) {
      if (InEventScene() == 1) {
         evt = Evt_GetUnused();
         evt->functionIndex = EVTF_MAP13_BRIDGE_EXPLOSION_IN_SCENE;
      } else {
         evt = Evt_GetUnused();
         evt->functionIndex = EVTF_MAP13_BRIDGE_EXPLOSION_IN_BATTLE;
      }
   }

   if (!InEventScene() && gState.mapNum == 14) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP14_SAND;
   }

   if (gState.mapNum == 15 && !InEventScene()) {
      Map15_RaisePlank(7, 11);
      Map15_RaisePlank(14, 11);
   }

   if (gState.mapNum == 17) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP17_BUTTON;
   }

   if (gState.mapNum == 17) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP17_FLOODGATE;
   }

   if (gState.mapNum == 19) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP19;
   }

   if (gState.mapNum == 26) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP26;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;
   }

   if (gState.mapNum == 27) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP27_BUTTONS;
   }

   if (gState.mapNum == 28) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP28_BUTTON;

      for (i = 194; i < 197; i++) {
         gGfxSubTextures[GFX_MAP_TEXTURES_OFS + i][3] = 24;
      }

      if (gState.primary == STATE_4 && gState.scene == 50) {
         gState.mapState.s.field_0x0 = 1;
      }
   }

   if (gState.mapNum == 29) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP29;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_DRAWBRIDGE_BUTTON;
   }

   if (gState.mapNum == 32) {
      evt = Evt_GetUnused();
      evt->x1.n = CV(58.0);
      evt->z1.n = CV(4.0);
      evt->y1.n = CV(6.75);
      evt->functionIndex = EVTF_MAP32_SMOKESTACK;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP32_CAR_RELEASE;
   }

   if (gState.mapNum == 33) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP33;
   }

   if (gState.mapNum == 35) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP35_BUTTON;
   }

   if (gState.mapNum == 36 && !InEventScene()) {
      // Leena shielding villager
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP36_LEENA;
      evt->x1.n = CV(7.5);
      evt->z1.n = CV(6.5);
      gMapUnitsPtr[6][7].s.team = TEAM_NEUTRAL;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP36_VILLAGER;
      evt->x1.n = CV(7.5);
      evt->z1.n = CV(7.5);
      gMapUnitsPtr[7][7].s.team = TEAM_NEUTRAL;
   }

   if (gState.mapNum == 39 && !InEventScene()) {
      // Leena raising submerged platforms
      Map39_Setup();
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP39;
   }

   if (gState.mapNum == 40) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP40_BARRICADE;
      gGfxSubTextures[GFX_MAP_TEXTURE_232][3] = 40;
      gGfxSubTextures[GFX_MAP_TEXTURE_233][3] = 40;
   }

   if (gState.mapNum == 38) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_MAP38_FLOODGATE;

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(20.5);
      evt->z1.n = CV(11.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(18.5);
      evt->z1.n = CV(12.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(17.5);
      evt->z1.n = CV(12.5);
      evt->y1.n = CV(1.5);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(13.0);
      evt->z1.n = CV(2.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(11.0);
      evt->z1.n = CV(3.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(3.0);
      evt->z1.n = CV(13.0);
      evt->y1.n = CV(3.0);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_FLAME;
      evt->x1.n = CV(1.0);
      evt->z1.n = CV(14.0);
      evt->y1.n = CV(3.0);
   }

   if (gState.mapNum == 42) {
      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(2.375);
      evt->z1.n = CV(9.625);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(19.375);
      evt->z1.n = CV(3.375);

      evt = Evt_GetUnused();
      evt->functionIndex = EVTF_CHIMNEY_SMOKE;
      evt->x1.n = CV(17.625);
      evt->z1.n = CV(14.625);
   }
}

void PositionCamera(s16 x, s16 y, s16 z) {
   gCameraPos.vx = -(x >> 3);
   gCameraPos.vy = y >> 3;
   gCameraPos.vz = -(z >> 3);
}

s32 func_80095128(s32 param_1, s16 x, s16 y, s16 z) {
   // Unused?
   SVECTOR vec;

   vec.vx = -(x >> 3) + 448;
   vec.vy = y >> 3;
   vec.vz = -(z >> 3) + 448;

   return param_1;
}

void PanCamera(s16 x, s16 y, s16 z, s32 smoothness) {
   gCameraPos.vx += (-(x >> 3) - gCameraPos.vx) >> smoothness;
   gCameraPos.vz += (-(z >> 3) - gCameraPos.vz) >> smoothness;
   gCameraPos.vy += ((y >> 3) - gCameraPos.vy) >> smoothness;
}

void AdjustTileElevation(MapTileModel *tileModel, s16 amount) {
   s32 i;

   for (i = 0; i < ARRAY_COUNT(tileModel->vertices); i++) {
      tileModel->vertices[i].vy += amount;
   }
}

void AdjustFaceElevation2(MapTileModel *tileModel, s16 faceIdx, s16 amount) {
   s32 i, j;
   s32 isDupeVtx;
   u8 *pFaceVerts;

   pFaceVerts = &tileModel->faces[faceIdx][0];

   for (i = 0; i < 4; i++) {
      // Prevent moving the same vertex multiple times, presumably for triangle or edge type faces.
      isDupeVtx = 0;

      for (j = 0; j < i; j++) {
         if (pFaceVerts[i] == pFaceVerts[j]) {
            isDupeVtx = 1;
         }
      }

      if (!isDupeVtx) {
         tileModel->vertices[pFaceVerts[i]].vy += amount;
      }
   }
}

void AdjustFaceElevation(MapTileModel *tileModel, s16 faceIdx, s16 amount) {
   s32 i;
   u8 *pFaceVerts;

   pFaceVerts = &tileModel->faces[faceIdx][0];

   for (i = 0; i < 4; i++) {
      tileModel->vertices[pFaceVerts[i]].vy += amount;
   }
}

void SetFaceElevation(MapTileModel *tileModel, s16 faceIdx, s16 elevation) {
   s32 i;
   u8 *pFaceVerts;

   pFaceVerts = &tileModel->faces[faceIdx][0];

   for (i = 0; i < 4; i++) {
      tileModel->vertices[pFaceVerts[i]].vy = elevation;
   }
}

void RotateMapTile(MapTileModel *src, SVECTOR *rot, MapTileModel *dst) {
   // Unused?
   s32 i;
   MATRIX mtx;
   VECTOR vec;
   s32 flag;

   PushMatrix();
   RotMatrix(rot, &mtx);
   vec.vx = 0;
   vec.vz = 0;
   vec.vy = 0;
   TransMatrix(&mtx, &vec);
   SetTransMatrix(&mtx);
   SetRotMatrix(&mtx);

   for (i = 0; i < ARRAY_COUNT(src->vertices); i++) {
      RotTrans(&src->vertices[i], &vec, &flag);
      dst->vertices[i].vx = vec.vx;
      dst->vertices[i].vz = vec.vz;
      dst->vertices[i].vy = vec.vy;
   }

   PopMatrix();
}

void RotTransMapTile(MapTileModel *src, SVECTOR *rot, SVECTOR *tr, MapTileModel *dst) {
   s32 i;
   MATRIX mtx;
   VECTOR vec;
   s32 flag;

   PushMatrix();
   RotMatrix(rot, &mtx);
   vec.vx = tr->vx >> 3;
   vec.vz = tr->vz >> 3;
   vec.vy = -(tr->vy >> 3);
   TransMatrix(&mtx, &vec);
   SetTransMatrix(&mtx);
   SetRotMatrix(&mtx);

   for (i = 0; i < ARRAY_COUNT(src->vertices); i++) {
      RotTrans(&src->vertices[i], &vec, &flag);
      dst->vertices[i].vx = vec.vx;
      dst->vertices[i].vz = vec.vz;
      dst->vertices[i].vy = vec.vy;
   }

   PopMatrix();
}

void AdjustAreaElevation_FromOrigin(s32 x, s32 z, s16 amount) {
   // Unused?
   s32 iz, ix;

   for (ix = 0; ix < x; ix++) {
      for (iz = 0; iz < z; iz++) {
         AdjustTileElevation(&gMapRowPointers[iz][ix], amount);
      }
   }
}

#undef EVTF
#define EVTF 399
void Evtf399_Map11(EvtData *evt) {
   switch (evt->state) {
   case 0:
      gTerrainPtr[3][8].s.terrain = TERRAIN_NO_ENTRY;
      gTerrainPtr[4][8].s.terrain = TERRAIN_NO_ENTRY;
      gMapRowPointers[3][8].height = 9;
      gMapRowPointers[4][8].height = 9;
      evt->state++;

      if (gState.mapState.s.field_0x0 != 0) {
         evt->functionIndex = EVTF_NULL;
         return;
      }

   // fallthrough
   case 1:
      if (gState.mapState.s.field_0x0 == 1) {
         evt->state2 = 48;
         gCameraRotation.vy &= 0xfff;
         AssignFromMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 1;
         PerformAudioCommand(AUDIO_CMD_PREPARE_XA(33));
         evt->state++;
      }
      break;

   case 2:
      EVT.dstRotY = GetBestViewOfTarget(11, 1, 1);
      gCameraRotation.vy &= 0xfff;
      EVT.dstRotY = func_800A96A8(gCameraRotation.vy, DEG(135));
      evt->state++;

   // fallthrough
   case 3:
      if (--evt->state2 <= 0) {
         gCameraRotation.vy = EVT.dstRotY;
         gCameraRotation.vx = DEG(33.75);
         evt->state3 = 2;
         evt->state++;
         PerformAudioCommand(AUDIO_CMD_PLAY_XA(33));
      }
      PanCamera(CV(11.5), CV(2.0), CV(1.0), 2);
      gCameraRotation.vy += (EVT.dstRotY - gCameraRotation.vy) >> 2;
      gCameraRotation.vx += (DEG(33.75) - gCameraRotation.vx) >> 2;
      gCameraZoom.vz += (DEG(33.75) - gCameraZoom.vz) >> 2;
      break;

   case 4:
      AdjustFaceElevation(&gMapRowPointers[1][11], 0, 8);
      gTerrainPtr[1][11].s.elevation = -gMapRowPointers[1][11].vertices[0].vy >> 4;
      gCameraPos.vy += 8;

      if (--evt->state3 <= 0) {
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 5:
      if (--evt->state3 <= 0) {
         gState.mapState.s.field_0x0 = 2;
         evt->state++;
      }
      break;

   case 6:
      if (gState.mapState.s.field_0x0 == 3) {
         gCameraRotation.vy &= 0xfff;
         EVT.camera.rotY = func_800A96A8(gCameraRotation.vy, EVT.camera.rotY);
         evt->state3 = 32;
         evt->state++;
      }
      break;

   case 7:
      if (--evt->state3 <= 0) {
         evt->state++;
      }
      EaseOutCamera(&EVT.camera, 2);
      break;

   case 8:
      if (--evt->state3 <= 0) {
         evt->functionIndex = EVTF_NULL;
         AssignToMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 0;
         gState.signal = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 753
void Evtf753_IncrementMapState0(EvtData *evt) {
   gState.mapState.s.field_0x0++;
   evt->functionIndex = EVTF_NULL;
}
