#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

void Map27_DepressButton(s32 x, s32 z) {
   AdjustFaceElevation(&gMapRowPointers[z][x], 0, 16);
   gTerrainPtr[z][x].s.elevation = -gMapRowPointers[z][x].vertices[0].vy >> 4;
}

void Map27_RemoveCellDoor(s32 x, s32 z) {
   MapTileModel *tileModel;

   tileModel = &gMapRowPointers[z][x];
   AdjustFaceElevation(tileModel, 0, 64);
   tileModel->vertices[tileModel->faces[7][0]].vy += 64;
   tileModel->vertices[tileModel->faces[7][1]].vy += 64;
   gTerrainPtr[z][x].s.elevation = -gMapRowPointers[z][x].vertices[0].vy >> 4;
   gTerrainPtr[z][x].s.terrain = TERRAIN_PLAINS;
   tileModel->gfx[0] = GFX_MAP_TEXTURE_25;

   tileModel = &gMapRowPointers[z + 1][x];
   AdjustFaceElevation(tileModel, 0, 64);
   tileModel->vertices[tileModel->faces[7][0]].vy += 64;
   tileModel->vertices[tileModel->faces[7][1]].vy += 64;
   tileModel->gfx[0] = GFX_MAP_TEXTURE_25;
   gTerrainPtr[z + 1][x].s.elevation = -gMapRowPointers[z][x].vertices[0].vy >> 4;
   gTerrainPtr[z + 1][x].s.terrain = TERRAIN_PLAINS;
}

#undef EVTF
#define EVTF 664
void Evtf664_Map27_OpenCellDoor(EvtData *evt) {
   s32 tileZ, tileX;
   MapTileModel *tileModel;

   tileModel = &OBJ_TILE_MODEL(evt);
   tileX = evt->x1.s.hi;
   tileZ = evt->z1.s.hi;

   switch (evt->state) {
   case 0:
      evt->mem = 64;
      evt->state2 = 2;
      evt->state3 = 1;
      evt->state++;

   // fallthrough
   case 1:
      if (evt->mem > evt->state2) {
         AdjustFaceElevation(&gMapRowPointers[tileZ][tileX], 0, evt->state2);
         AdjustFaceElevation(&gMapRowPointers[tileZ + 1][tileX], 0, evt->state2);

         tileModel = &gMapRowPointers[tileZ][tileX];
         tileModel->vertices[tileModel->faces[7][0]].vy += evt->state2;
         tileModel->vertices[tileModel->faces[7][1]].vy += evt->state2;

         tileModel = &gMapRowPointers[tileZ + 1][tileX];
         tileModel->vertices[tileModel->faces[7][0]].vy += evt->state2;
         tileModel->vertices[tileModel->faces[7][1]].vy += evt->state2;

         evt->mem -= evt->state2;
      } else {
         SetFaceElevation(&gMapRowPointers[tileZ][tileX], 0, -16);
         SetFaceElevation(&gMapRowPointers[tileZ][tileX], 1, -16);
         SetFaceElevation(&gMapRowPointers[tileZ + 1][tileX], 0, -16);
         SetFaceElevation(&gMapRowPointers[tileZ + 1][tileX], 1, -16);
         evt->mem = 0;
         evt->state++;
      }
      UpdateMapTileLighting(tileModel);
      gTerrainPtr[tileZ][tileX].s.elevation = -gMapRowPointers[tileZ][tileX].vertices[0].vy >> 4;
      gTerrainPtr[tileZ][tileX].s.terrain = TERRAIN_PLAINS;
      gTerrainPtr[tileZ + 1][tileX].s.elevation =
          -gMapRowPointers[tileZ][tileX].vertices[0].vy >> 4;
      gTerrainPtr[tileZ + 1][tileX].s.terrain = TERRAIN_PLAINS;
      break;

   case 2:
      if (++evt->mem >= 32) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 665
void Evtf665_Map27_Buttons(EvtData *evt) {
   static BVectorXZ buttonLocations[4] = {{22, 14}, {9, 9}, {15, 12}, {1, 1}};
   static u8 buttonDoorIdx[4] = {0, 1, 2, 4};
   static BVectorXZ doorLocations[5] = {{11, 1}, {19, 13}, {3, 1}, {11, 13}, {7, 9}};

   s32 i;
   s32 doorIdx;
   EvtData *child;
   BVectorXZ *p;

   switch (evt->state) {
   case 0:
      for (i = 0; i < ARRAY_COUNT(buttonLocations); i++) {
         if (gState.mapState.bytes[i] != 0) {
            Map27_DepressButton(buttonLocations[i].x, buttonLocations[i].z);
            doorIdx = buttonDoorIdx[i];
            Map27_RemoveCellDoor(doorLocations[doorIdx].x, doorLocations[doorIdx].z);
            if (doorIdx == 2) {
               // Doors 2 and 3 open together
               p = &doorLocations[3];
               Map27_RemoveCellDoor(p->x, p->z);
            }
            EVT.buttonPressed[i] = 1;
         }
      }
      evt->state++;

   // fallthrough
   case 1:
      for (i = 0; i < ARRAY_COUNT(buttonLocations); i++) {
         if (!EVT.buttonPressed[i] && gState.mapState.bytes[i] != 0) {
            // Button i currently being pressed. (mapState set by Evtf443_EvaluateBattle27)
            evt->mem = i;
            evt->state++;
            evt->state2 = 0;
            break;
         }
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         AssignFromMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 1;
         evt->z3.n = buttonDoorIdx[evt->mem];
         evt->state2++;
         break;

      case 1:
         child = Evt_GetUnused();
         child->functionIndex = EVTF_BUTTON_DEPRESS;
         child->x1.s.hi = buttonLocations[evt->mem].x;
         child->z1.s.hi = buttonLocations[evt->mem].z;
         EVT.child = child;
         evt->state2++;
         break;

      case 2:
         child = EVT.child;
         if (child->state == 99) {
            evt->state3 = 0;
            evt->state2++;
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(40));
         }
         break;

      case 3:
         PanCamera(doorLocations[evt->z3.n].x * CV(1.0) + CV(0.5),
                   GetTerrainElevation(doorLocations[evt->z3.n].z, doorLocations[evt->z3.n].x),
                   doorLocations[evt->z3.n].z * CV(1.0) + CV(0.5), 3);
         if (++evt->state3 >= 48) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(40));
            evt->state2++;
         }
         break;

      case 4:
         child = Evt_GetUnused();
         child->functionIndex = EVTF_MAP27_OPEN_CELL_DOOR;
         child->x1.s.hi = doorLocations[evt->z3.n].x;
         child->z1.s.hi = doorLocations[evt->z3.n].z;
         EVT.child = child;
         evt->state2++;
         break;

      case 5:
         child = EVT.child;
         if (child->functionIndex != EVTF_MAP27_OPEN_CELL_DOOR) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 6:
         if (evt->z3.n == 2) {
            evt->z3.n++;
            evt->state2 = 3;
         } else {
            evt->state2++;
         }
         break;

      case 7:
         EaseOutCamera(&EVT.camera, 2);
         if (++evt->state3 >= 48) {
            AssignToMainCamera(&EVT.camera);
            evt->state2++;
         }
         break;

      case 8:
         gPlayerControlSuppressed = 0;
         gState.signal = 0;
         evt->state2 = 0;
         evt->state++;
         break;
      }

      break;

   case 3:
      EVT.buttonPressed[evt->mem] = 1;
      evt->state = 1;
      break;
   }
}
