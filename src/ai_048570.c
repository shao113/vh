#include "common.h"
#include "evt.h"
#include "field.h"
#include "units.h"
#include "battle.h"
#include "state.h"

#include "PsyQ/kernel.h"

// TODO: Merge back into ai.c once Evtf402_AI_TBD is matching.

extern u8 D_80123410, D_80123480, D_80123484;
extern s16 D_8012F63C[40];
extern s16 D_8017DF50[27][64];
extern u8 D_80123484;

// ai.c:
void Evtf570_AI_TBD(EvtData *);
s32 func_800560F8(UnitStatus *);
s32 IsLagging(void);
void Evtf400_AI_TBD(EvtData *);
void func_80056760(UnitStatus *);
void func_800569A0(UnitStatus *);
s32 func_80056C30(UnitStatus *, UnitStatus *, u8, u8, u8, u8);
s32 func_80056F94(UnitStatus *, u8, u8, u8, u8);
void Evtf401_AI_TBD(EvtData *);
void Evtf402_AI_TBD(EvtData *);

// This file:
void Evtf589_AI_TBD(EvtData *);
void Evtf404_AI_TBD(EvtData *);
void Evtf403_AI_TBD(EvtData *);

s32 s_x1_801232f8;
s32 s_z1_801232fc;
s32 s_pref_80123300;
s32 s_z2_80123304;
s32 s_x2_80123308;
s32 s_x3_8012330c;
s32 s_z3_80123310;

#undef EVTF
#define EVTF 403
void Evtf403_AI_TBD(EvtData *evt) {
   UnitStatus *unit1;
   UnitStatus *unit2;
   EvtData *sprite;
   EvtData *newEvt;
   s32 i;
   s32 iz2, ix2;

   unit1 = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
   sprite = unit1->evtSprite;

   switch (evt->state) {
   case 0:
      if (IsLagging()) {
         return;
      }

      D_80123480 = 0;
      func_800569A0(unit1);
      gDir_80123470 = 0xffff;
      ClearGrid(0);
      ClearGrid(1);
      evt->state++;

   // fallthrough
   case 1:
      if (IsLagging()) {
         return;
      }

      PopulateMovementGrid(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 0xff, 2);
      evt->state++;

   // fallthrough
   case 2:
      if (IsLagging()) {
         return;
      }

      i = (unit1->field7_0x1c + 1) * unit1->travelRange;
      if (i > 0xff) {
         i = 0xff;
      }
      if (unit1->hp != unit1->maxHp) {
         i = 0xff;
      }
      if (func_800560F8(unit1) != 0) {
         i = 0xff;
      }

      func_8002B3A8(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), i, 3);
      evt->state++;

   // fallthrough
   case 3:
      if (IsLagging()) {
         return;
      }

      func_8002B3A8(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), unit1->travelRange, 4);
      evt->state++;

   // fallthrough
   case 4:
      if (IsLagging()) {
         return;
      }

      newEvt = Evt_GetLastUnused();
      newEvt->functionIndex = EVTF_AI_TBD_401;
      HI(newEvt->d.evtf401.x) = HI(EVT.x);
      HI(newEvt->d.evtf401.z) = HI(EVT.z);
      D_80123484 = 0;
      evt->state++;
      break;

   case 5:
      if (D_80123484 != 0) {
         EVT.resumeZ = gMapMinZ;
         EVT.resumeX = gMapMinX;
         s_pref_80123300 = 0;
         evt->state++;
      }
      break;

   case 6:
      if (IsLagging()) {
         return;
      }

      s_z1_801232fc = EVT.resumeZ;
      s_x1_801232f8 = EVT.resumeX;

      while (s_z1_801232fc <= gMapMaxZ) {
         while (s_x1_801232f8 <= gMapMaxX) {

            if ((s_x1_801232f8 == HI(sprite->d.sprite.x1) &&
                 s_z1_801232fc == HI(sprite->d.sprite.z1)) ||
                (gPathGrid3_Ptr[s_z1_801232fc][s_x1_801232f8] != PATH_STEP_UNSET &&
                 gMapUnitsPtr[s_z1_801232fc][s_x1_801232f8].s.unitIdx == 0)) {

               if (GetRCnt(RCntCNT1) > 450) {
                  EVT.resumeX = s_x1_801232f8;
                  EVT.resumeZ = s_z1_801232fc;
                  return;
               }

               if (unit1->class == CLASS_ARCHER) {
                  PopulateRangedAttackGrid(s_z1_801232fc, s_x1_801232f8, unit1->attackRange, 5);
               } else {
                  PopulateMeleeAttackGrid(s_z1_801232fc, s_x1_801232f8, 5, 1);
               }

               for (iz2 = gMapMinZ; iz2 <= gMapMaxZ; iz2++) {
                  for (ix2 = gMapMinX; ix2 <= gMapMaxX; ix2++) {
                     if (gPathGrid5_Ptr[iz2][ix2] != PATH_STEP_UNSET &&
                         gMapUnitsPtr[iz2][ix2].s.unitIdx != 0) {

                        unit2 = &gUnits[gMapUnitsPtr[iz2][ix2].s.unitIdx];
                        if (unit1->team != unit2->team) {
                           i = func_80056C30(unit1, unit2, s_z1_801232fc, s_x1_801232f8, iz2, ix2);
                           if (i > s_pref_80123300) {
                              s_pref_80123300 = i;
                              s_z2_80123304 = s_z1_801232fc;
                              s_x2_80123308 = s_x1_801232f8;
                              s_z3_80123310 = iz2;
                              s_x3_8012330c = ix2;
                           }
                        }
                     }
                  }
               }
            }

            s_x1_801232f8++;
         }

         s_z1_801232fc++;
         s_x1_801232f8 = gMapMinX;

         if (GetRCnt(RCntCNT1) > 450) {
            EVT.resumeX = s_x1_801232f8;
            EVT.resumeZ = s_z1_801232fc;
            return;
         }
      }
      if (s_pref_80123300 == 0) {
         s_x2_80123308 = HI(EVT.x);
         s_z2_80123304 = HI(EVT.z);
      }
      evt->state++;
      break;

   case 7:
      if (IsLagging()) {
         return;
      }

      gX_801233d8 = s_x2_80123308;
      gZ_801233dc = s_z2_80123304;

      if (HI(EVT.x) == s_x2_80123308 && HI(EVT.z) == s_z2_80123304) {
         if (s_pref_80123300 != 0) {
            D_8012337C = 1;
            gTargetX_80123414 = s_x3_8012330c;
            gTargetZ_80123418 = s_z3_80123310;
         } else {
            D_8012337C = 0;
            func_80056760(unit1);
         }
         evt->state = 99;
      } else if (gPathGrid4_Ptr[s_z2_80123304][s_x2_80123308] != PATH_STEP_UNSET) {
         D_8012337C = 1;
         gX_801233d8 = s_x2_80123308;
         gZ_801233dc = s_z2_80123304;
         gTargetX_80123414 = s_x3_8012330c;
         gTargetZ_80123418 = s_z3_80123310;
         evt->state = 99;
      } else {
         evt->state++;
      }

      break;

   case 8:

      while (gPathGrid4_Ptr[s_z2_80123304][s_x2_80123308] == PATH_STEP_UNSET ||
             gMapUnitsPtr[s_z2_80123304][s_x2_80123308].s.unitIdx != 0) {

         switch (gPathGrid2_Ptr[s_z2_80123304][s_x2_80123308]) {
         case PATH_STEP_SOUTH:
            s_z2_80123304--;
            continue;
         case PATH_STEP_WEST:
            s_x2_80123308--;
            continue;
         case PATH_STEP_NORTH:
            s_z2_80123304++;
            continue;
         case PATH_STEP_EAST:
            s_x2_80123308++;
            continue;
         }

         break;
      }

      D_8012337C = 0;
      gX_801233d8 = s_x2_80123308;
      gZ_801233dc = s_z2_80123304;
      func_80056760(unit1);
      evt->state = 99;
      break;

   case 99:
      D_80123480 = 1;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

s32 s_pref_80123314;
s32 s_z_80123318;
s32 s_x_8012331c;

#undef EVTF
#define EVTF 404
void Evtf404_AI_TBD(EvtData *evt) {
   UnitStatus *unit;
   EvtData *sprite;
   EvtData *newEvt;
   s32 i;
   s32 iz, ix;

   unit = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
   sprite = unit->evtSprite;

   switch (evt->state) {
   case 0:
      if (IsLagging()) {
         return;
      }

      D_80123480 = 0;
      gDir_80123470 = 0xffff;
      ClearGrid(0);
      ClearGrid(1);
      evt->state++;

   // fallthrough
   case 1:
      if (IsLagging()) {
         return;
      }

      func_8002B3A8(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), unit->travelRange, 4);
      evt->state++;

   // fallthrough
   case 2:
      if (IsLagging()) {
         return;
      }

      newEvt = Evt_GetLastUnused();
      newEvt->functionIndex = EVTF_AI_TBD_401;
      HI(newEvt->d.evtf401.x) = HI(EVT.x);
      HI(newEvt->d.evtf401.z) = HI(EVT.z);
      D_80123484 = 0;
      evt->state++;
      break;

   case 3:
      if (D_80123484 == 0) {
         break;
      }

      EVT.resumeZ = gMapMinZ;
      EVT.resumeX = gMapMinX;
      s_pref_80123314 = 10000;
      evt->state++;

   // fallthrough
   case 4:
      if (IsLagging()) {
         return;
      }

      iz = EVT.resumeZ;
      ix = EVT.resumeX;

      while (iz <= gMapMaxZ) {
         while (ix <= gMapMaxX) {

            if ((ix == HI(sprite->d.sprite.x1) && iz == HI(sprite->d.sprite.z1)) ||
                (gPathGrid4_Ptr[iz][ix] != PATH_STEP_UNSET &&
                 gMapUnitsPtr[iz][ix].s.unitIdx == 0)) {

               if (GetRCnt(RCntCNT1) > 450) {
                  EVT.resumeX = ix;
                  EVT.resumeZ = iz;
                  return;
               }

               i = (gPathGrid6_Ptr[iz][ix] * 10) -
                   (gTerrainPreference[gTerrainPtr[iz][ix].s.terrain] / 100) +
                   -(gTerrainPtr[iz][ix].s.elevation * 10);
               if (s_pref_80123314 > i) {
                  s_pref_80123314 = i;
                  s_z_80123318 = iz;
                  s_x_8012331c = ix;
               }
            }

            ix++;
         }

         iz++;
         ix = gMapMinX;

         if (GetRCnt(RCntCNT1) > 450) {
            EVT.resumeX = ix;
            EVT.resumeZ = iz;
            return;
         }
      }

      evt->state++;
      break;

   case 5:
      D_8012337C = 0;
      gX_801233d8 = s_x_8012331c;
      gZ_801233dc = s_z_80123318;
      func_80056760(unit);
      evt->state = 99;
      break;

   case 99:
      D_80123480 = 1;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

s32 s_pref_80123328;
s32 s_z_8012332c;
s32 s_x_80123330;

#undef EVTF
#define EVTF 589
void Evtf589_AI_TBD(EvtData *evt) {
   UnitStatus *unit;
   EvtData *sprite;
   s32 i;
   s32 iz, ix;
   s32 PVar4;

   unit = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
   sprite = unit->evtSprite;

   switch (evt->state) {
   case 0:
      if (IsLagging()) {
         return;
      }

      D_80123480 = 0;
      gDir_80123470 = 0xffff;
      ClearGrid(0);
      ClearGrid(1);
      evt->state++;

   // fallthrough
   case 1:
      if (IsLagging()) {
         return;
      }

      func_8002B3A8(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), unit->travelRange, 4);
      func_8002C1A0(HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 0xfe, 6);
      evt->state++;

   // fallthrough
   case 2:
      if (IsLagging()) {
         return;
      }

      PVar4 = 0;

      if (gState.mapNum == 39) {
         iz = 11;
         ix = 16;
         if (gTerrainPtr[2][10].s.terrain == TERRAIN_PLAINS) {
            iz = 2;
            ix = 10;
         }
         if (gTerrainPtr[7][7].s.terrain == TERRAIN_PLAINS) {
            iz = 6;
            ix = 3;
         }
      } else {
         iz = HI(sprite->d.sprite.z1);
         ix = HI(sprite->d.sprite.x1);

         for (i = 0; i < 12; i++) {
            if (PVar4 < gPathGrid6_Ptr[i][47]) {
               iz = i;
               ix = 47;
               PVar4 = gPathGrid6_Ptr[i][47];
            }
         }
         if (gState.mapState.s.field_0x13 != 0) {
            for (i = 0; i < 12; i++) {
               if (PVar4 < gPathGrid6_Ptr[i][0]) {
                  iz = i;
                  ix = 0;
                  PVar4 = gPathGrid6_Ptr[i][0];
               }
            }
         }
      }

      func_8002C1D8(iz, ix, 0xfe, 6, HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1));
      evt->state++;
      break;

   case 3:
      s_z_8012332c = HI(EVT.z);
      s_x_80123330 = HI(EVT.x);
      EVT.resumeZ = gMapMinZ;
      EVT.resumeX = gMapMinX;
      s_pref_80123328 = 0;
      evt->state++;

   // fallthrough
   case 4:
      if (IsLagging()) {
         return;
      }

      iz = EVT.resumeZ;
      ix = EVT.resumeX;

      while (iz <= gMapMaxZ) {
         while (ix <= gMapMaxX) {

            if ((ix == HI(sprite->d.sprite.x1) && iz == HI(sprite->d.sprite.z1)) ||
                (gPathGrid4_Ptr[iz][ix] != PATH_STEP_UNSET &&
                 gMapUnitsPtr[iz][ix].s.unitIdx == 0)) {

               if (GetRCnt(RCntCNT1) > 450) {
                  EVT.resumeX = ix;
                  EVT.resumeZ = iz;
                  return;
               }

               i = gPathGrid6_Ptr[iz][ix];
               if (s_pref_80123328 < i) {
                  s_pref_80123328 = i;
                  s_z_8012332c = iz;
                  s_x_80123330 = ix;
               }
            }

            ix++;
         }

         iz++;
         ix = gMapMinX;

         if (GetRCnt(RCntCNT1) > 450) {
            EVT.resumeX = ix;
            EVT.resumeZ = iz;
            return;
         }
      }

      evt->state++;
      break;

   case 5:
      D_8012337C = 0;
      gX_801233d8 = s_x_80123330;
      gZ_801233dc = s_z_8012332c;
      gDir_80123470 = 0xffff;
      evt->state = 99;
      break;

   case 99:
      D_80123480 = 1;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}
