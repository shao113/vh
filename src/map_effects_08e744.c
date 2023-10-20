#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "field.h"
#include "state.h"
#include "audio.h"

#undef EVTF
#define EVTF 662
void Evtf662_Map28_OpenDoor(EvtData *evt) {
   s32 i;
   s32 ix, iz;
   EvtData *smoke;
   s32 tmp;

   gCameraRotation.vy -= DEG(0.703125);

   switch (evt->state) {
   case 0:
      evt->state++;
      evt->mem = 48;
      evt->y2.n = gCameraPos.vy;
      evt->z3.n = 3;
      evt->state2 = 1;

   // fallthrough
   case 1:
      if (evt->mem >= evt->state2) {
         ix = 13;
         for (iz = 13; iz <= 15; iz++) {
            AdjustFaceElevation(&gMapRowPointers[iz][ix], 0, evt->state2);
         }
         evt->y2.n -= evt->state2;
         evt->mem -= evt->state2;
      } else {
         ix = 13;
         for (iz = 13; iz <= 15; iz++) {
            AdjustFaceElevation(&gMapRowPointers[iz][ix], 0, evt->mem);
         }
         evt->y2.n -= evt->mem;
         evt->mem = 0;
         evt->state++;
      }

      if (--evt->z3.n <= 0) {
         for (i = 0; i < 7; i++) {
            smoke = Evt_GetUnused();
            smoke->functionIndex = EVTF_SMOKE;
            smoke->x1.n = CV(13.0);
            smoke->z1.n = CV(13.0) + i * CV(0.5);
            smoke->y1.n = CV(0.5);
            smoke->x2.n = -rand() % CV(0.25);
            smoke->z2.n = rand() % CV(0.25) - CV(0.125);
            smoke->y2.n = rand() % CV(0.25);

            smoke = Evt_GetUnused();
            smoke->functionIndex = EVTF_SMOKE;
            smoke->x1.n = CV(14.0);
            smoke->z1.n = CV(13.0) + i * CV(0.5);
            smoke->y1.n = CV(0.5);
            smoke->x2.n = rand() % CV(0.25);
            smoke->z2.n = rand() % CV(0.25) - CV(0.125);
            smoke->y2.n = rand() % CV(0.25);
         }
         evt->z3.n += (rand() % 5 + 3);
      }

      gCameraPos.vy = evt->y2.n + rand() % 4;

      for (i = 194; i < 197; i++) {
         gGfxSubTextures[GFX_MAP_TEXTURES_OFS + i][3] = evt->mem >> 1;
      }
      break;

   case 2:
      gCameraPos.vy = evt->y2.n + rand() % 2;
      if (++evt->mem >= 32) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 3:
      //?
      rand();
      tmp = evt->y2.n;
      gCameraPos.vy = tmp;
      if (++evt->mem >= 32) {
         evt->mem = 0;
         evt->state++;
      }
      break;

   case 4:
      for (iz = 13; iz <= 15; iz++) {
         gTerrainPtr[iz][13].s.elevation = -gMapRowPointers[iz][13].vertices[0].vy >> 4;
      }
      evt->functionIndex = EVTF_NULL;
      gCameraPos.vy = evt->y2.n;
      break;

   case 5:
      break;
   }
}

void Map28_RemoveDoor(void) {
   s32 iz, ix;

   ix = 13;
   for (iz = 13; iz <= 15; iz++) {
      AdjustFaceElevation(&gMapRowPointers[iz][ix], 0, 48);
      gTerrainPtr[iz][ix].s.elevation = -gMapRowPointers[iz][ix].vertices[0].vy >> 4;
   }
}

#undef EVTF
#define EVTF 663
void Evtf663_Map28_Button(EvtData *evt) {
   EvtData *child;

   switch (evt->state) {
   case 0:
      evt->x1.n = CV(13.5);
      evt->z1.n = CV(14.5);
      evt->y1.n = GetTerrainElevation(14, 13);
      evt->state++;
      if (gState.mapState.s.field_0x0 != 0) {
         Map28_RemoveDoor();
         DepressButton(2, 19);
         evt->functionIndex = EVTF_NULL;
      }
      break;

   case 1:
      if (gState.mapState.s.field_0x0 != 0) {
         evt->state2 = 0;
         evt->state++;
      }
      break;

   case 2:

      switch (evt->state2) {
      case 0:
         AssignFromMainCamera(&EVT.camera);
         gPlayerControlSuppressed = 1;
         evt->state2++;
         break;

      case 1:
         child = Evt_GetUnused();
         child->functionIndex = EVTF_BUTTON_DEPRESS;
         child->x1.s.hi = 2;
         child->z1.s.hi = 19;
         EVT.child = child;
         evt->state2++;
         break;

      case 2:
         child = EVT.child;
         if (child->state == 99) {
            evt->state3 = 0;
            PerformAudioCommand(AUDIO_CMD_PREPARE_XA(42));
            evt->state2++;
         }
         break;

      case 3:
         PanCamera(evt->x1.n, evt->y1.n, evt->z1.n, 3);
         if (++evt->state3 >= 48) {
            PerformAudioCommand(AUDIO_CMD_PLAY_XA(42));
            evt->state2++;
         }
         break;

      case 4:
         child = Evt_GetUnused();
         child->functionIndex = EVTF_MAP28_OPEN_DOOR;
         EVT.child = child;
         evt->state2++;
         break;

      case 5:
         child = EVT.child;
         if (child->functionIndex != EVTF_MAP28_OPEN_DOOR) {
            evt->state2++;
            evt->state3 = 0;
         }
         break;

      case 6:
         evt->state2++;

      // fallthrough
      case 7:
         EaseOutCamera(&EVT.camera, 2);
         if (++evt->state3 >= 32) {
            AssignToMainCamera(&EVT.camera);
            evt->state2++;
         }
         break;

      case 8:
         gPlayerControlSuppressed = 0;
         evt->state2 = 0;
         evt->state++;
         break;
      }

      break;

   case 3:
      gState.mapState.s.field_0x0 = 1;
      gState.signal = 0;
      evt->state++;
      break;

   case 4:
      break;
   }
}

#undef EVTF
#define EVTF 304
void Evtf304_661_Flame(EvtData *evt) {
   static s16 flameAnimData_Fast[20] = {
       7, GFX_FLAME_1, 1, GFX_FLAME_2, 1, GFX_FLAME_3, 1, GFX_FLAME_4, 1, GFX_FLAME_5,
       1, GFX_FLAME_6, 1, GFX_FLAME_7, 1, GFX_FLAME_8, 1, GFX_NULL,    1, GFX_NULL};
   static s16 flameAnimData_Med[20] = {
       7, GFX_FLAME_1, 2, GFX_FLAME_2, 2, GFX_FLAME_3, 2, GFX_FLAME_4, 2, GFX_FLAME_5,
       2, GFX_FLAME_6, 2, GFX_FLAME_7, 2, GFX_FLAME_8, 2, GFX_NULL,    1, GFX_NULL};
   static s16 flameAnimData_Slow[20] = {
       7, GFX_FLAME_1, 3, GFX_FLAME_2, 3, GFX_FLAME_3, 3, GFX_FLAME_4, 3, GFX_FLAME_5,
       3, GFX_FLAME_6, 3, GFX_FLAME_7, 3, GFX_FLAME_8, 3, GFX_NULL,    1, GFX_NULL};
   static Quad quad = {{-16, -64, 0, 0}, {16, -64, 0, 0}, {-16, 0, 0, 0}, {16, 0, 0, 0}};
   static Quad quad_unused = {{-8, -32, 0, 0}, {8, -32, 0, 0}, {-8, 0, 0, 0}, {8, 0, 0, 0}};
   static s16 smokeAnimData_unused[24] = {
       4, GFX_PUFF_1, 2, GFX_PUFF_2, 2, GFX_PUFF_3, 2, GFX_PUFF_4,  2, GFX_PUFF_5, 2, GFX_PUFF_6,
       2, GFX_PUFF_7, 2, GFX_PUFF_8, 2, GFX_PUFF_9, 2, GFX_PUFF_10, 2, GFX_NULL,   1, GFX_NULL};

   void *animations[3] = {flameAnimData_Fast, flameAnimData_Med, flameAnimData_Slow};
   s32 i, skip;
   s32 prevQuadIdx;
   POLY_FT4 *poly;
   Quad *qswap;
   u8 fade;

   switch (evt->state) {
   case 0:
      evt->mem = rand() % 5;
      evt->state++;

   // fallthrough
   case 1:
      if (--evt->mem <= 0) {
         evt->state++;
      }
      break;

   case 2:
      if (evt->functionIndex == EVTF_MAP31_SCN61_XENO_FLAME) {
         EVT.animData = animations[rand() % 3];
         EVT.quadp = &quad;
         skip = rand() % 5;
         for (i = 0; i < skip; i++) {
            UpdateEvtAnimation(evt);
         }
      } else {
         EVT.animData = animations[rand() % 3];
         EVT.quadp = &quad;
         EVT.semiTrans = 2;
      }
      evt->state++;
      break;

   default:
   case 3:
      if (evt->functionIndex == EVTF_MAP31_SCN61_XENO_FLAME) {
         if (gState.mapState.s.field_0x0 == 1) {
            EVT.radius--;
            if (EVT.radius <= CV(3.0))
               EVT.radius = CV(3.0);
         }
         evt->x1.n = evt->x2.n + (EVT.radius * rcos(EVT.theta) >> 12);
         evt->z1.n = evt->z2.n + (EVT.radius * rsin(EVT.theta) >> 12);
         evt->y1.n = evt->y2.n;
         if (gState.mapState.s.field_0x0 == 2) {
            evt->mem = 128;
            evt->state++;
         }
      }
      qswap = gSpriteBoxQuads[7];
      gSpriteBoxQuads[7] = EVT.quadp;
      UpdateEvtAnimation(evt);
      AddEvtPrim7(gGraphicsPtr->ot, evt, 0);
      gSpriteBoxQuads[7] = qswap;
      break;

   case 4:
      prevQuadIdx = gQuadIndex;
      qswap = gSpriteBoxQuads[7];
      gSpriteBoxQuads[7] = EVT.quadp;
      UpdateEvtAnimation(evt);
      AddEvtPrim7(gGraphicsPtr->ot, evt, 0);
      gSpriteBoxQuads[7] = qswap;
      if (prevQuadIdx != gQuadIndex) {
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         fade = evt->mem;
         setRGB0(poly, fade, fade, fade);
      }
      evt->mem -= 4;
      if (evt->mem <= 0) {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

#undef EVTF
#define EVTF 303
void Evtf303_Map31_Scn61_XenoFlames(EvtData *evt) {
   EvtData *entitySprite;
   EvtData *flame;
   s32 i;
   s32 rnd;

   switch (evt->state) {
   case 0:
      EVT.theta = DEG(0);
      evt->state++;
      entitySprite = EVT.entitySprite;
      evt->x1.n = entitySprite->x1.n;
      evt->z1.n = entitySprite->z1.n;
      evt->y1.n = entitySprite->y1.n;
      gState.mapState.s.field_0x0 = 0;
      evt->y3.n = gCameraPos.vy;

   // fallthrough
   case 1:
      for (i = 0; i < 1; i++) {
         flame = Evt_GetUnused();
         flame->functionIndex = EVTF_MAP31_SCN61_XENO_FLAME;
         flame->x1.n = evt->x1.n + (CV(4.0) * rcos(EVT.theta + DEG(180)) >> 12);
         flame->z1.n = evt->z1.n + (CV(4.0) * rsin(EVT.theta + DEG(180)) >> 12);
         flame->y1.n = GetTerrainElevation(flame->z1.s.hi, flame->x1.s.hi);
         flame->x2.n = evt->x1.n;
         flame->z2.n = evt->z1.n;
         flame->y2.n = evt->y1.n;
         flame->d.evtf304.semiTrans = 2;
         flame->d.evtf304.radius = CV(4.0);
         flame->d.evtf304.theta = EVT.theta + DEG(180);
      }
      EVT.theta += DEG(8.4375);
      rnd = rand();
      gCameraPos.vy = evt->y3.n - 12;
      gCameraPos.vy += rnd % 24;
      if (EVT.theta > DEG(360)) {
         gCameraPos.vy = evt->y3.n;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}
