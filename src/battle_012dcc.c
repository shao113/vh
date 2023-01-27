#include "common.h"
#include "evt.h"
#include "battle.h"
#include "units.h"
#include "state.h"
#include "field.h"
#include "window.h"

#undef EVTF
#define EVTF 027
void Evtf027_TargetingSpell(EvtData *evt) {
   UnitStatus *caster;
   s32 unitIdx;
   s32 ix, iz;
   EvtData *newEvt;

   if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != 0) {
      gState.mapCursorOutOfRange = 0;
   } else {
      gState.mapCursorOutOfRange = 1;
   }

   caster = EVT.caster;

   switch (evt->state) {
   case 0:
      caster = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].unitIdx];
      EVT.caster = caster;

      if (gSpells[gCurrentSpell].area == SPELL_AREA_NULL) {
         gTargetX = HI(EVT.x);
         gTargetZ = HI(EVT.z);
         evt->state = 100;
         return;
      }
      evt->state++;

   // fallthrough
   case 1:
      ClearGrid(1);
      gTargetZ = HI(EVT.z);
      gTargetX = HI(EVT.x);
      PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].range, 0);
      gYellowTargetGridPtr[gMapCursorZ][gMapCursorX] = 1;
      evt->state++;
      return;

   case 2:
      if (!(gSavedPadStateNewPresses & PAD_X)) {
         if (gSpells[gCurrentSpell].area == SPELL_AREA_AOE) {
            evt->state++;
            evt->state2 = 6;
            return;
         }
         if (gTargetX != gMapCursorX || gTargetZ != gMapCursorZ) {
            gTargetX = gMapCursorX;
            gTargetZ = gMapCursorZ;
            ClearGrid(1);
            if (gRedAttackGridPtr[gTargetZ][gTargetX] != 0) {
               gYellowTargetGridPtr[gTargetZ][gTargetX] = 1;
            }
         }
         if (gSavedPadStateNewPresses & PAD_CIRCLE) {
            evt->state++;
            evt->state2 = gSpells[gCurrentSpell].targeting;
         }
      } else {
         evt->state = 99;
      }
      return;

   case 3:

      switch (evt->state2) {
      case SPELL_TARGET_ENEMY:
         if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != 0) {
            unitIdx = gMapUnitsPtr[gMapCursorZ][gMapCursorX].unitIdx;
            if (unitIdx != 0 && caster->team != gUnits[unitIdx].team) {
               gTargetX = gMapCursorX;
               gTargetZ = gMapCursorZ;
               evt->state = 4;
               return;
            }
         }
         evt->state = 2;
         evt->state2 = 0;
         return;
      case SPELL_TARGET_ALLY:
         if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != 0) {
            unitIdx = gMapUnitsPtr[gMapCursorZ][gMapCursorX].unitIdx;
            if (unitIdx != 0 && caster->team == gUnits[unitIdx].team) {
               gTargetX = gMapCursorX;
               gTargetZ = gMapCursorZ;
               evt->state = 4;
               return;
            }
         }
         evt->state = 2;
         evt->state2 = 0;
         return;
      case 2:
         return;
      case SPELL_TARGET_ENEMY_GROUP:
         evt->state = 2;
         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != 0) {
                  unitIdx = gMapUnitsPtr[iz][ix].unitIdx;
                  if (unitIdx != 0 && caster->team != gUnits[unitIdx].team) {
                     gTargetX = HI(EVT.x);
                     gTargetZ = HI(EVT.z);
                     evt->state = 4;
                  }
               }
            }
         }
         evt->state2 = 0;
         return;
      case SPELL_TARGET_ALLY_GROUP:
         evt->state = 2;
         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gRedAttackGridPtr[iz][ix] != 0) {
                  unitIdx = gMapUnitsPtr[iz][ix].unitIdx;
                  if (unitIdx != 0 && caster->team == gUnits[unitIdx].team) {
                     gTargetX = HI(EVT.x);
                     gTargetZ = HI(EVT.z);
                     evt->state = 4;
                  }
               }
            }
         }
         evt->state2 = 0;
         return;
      case 5:
         return;
      case 6:
      TargetingType6_80022da4:
         if (gRedAttackGridPtr[gMapCursorZ][gMapCursorX] != 0) {
            gTargetX = gMapCursorX;
            gTargetZ = gMapCursorZ;
            evt->state = 4;
            break; // -> state:4
         }
         ClearGrid(1);
         if (gSavedPadStateNewPresses & PAD_X) {
            evt->state = 99;
            return;
         }
         evt->state = 2;
         evt->state2 = 0;
         return;
      case 7:
      case 8:
      case 9:
         gTargetX = HI(EVT.x);
         gTargetZ = HI(EVT.z);
         evt->state = 4;
         return;
      default:
         return;
      } // switch (evt->state2) (via state:3)

   // fallthrough
   case 4:
      if (gSpells[gCurrentSpell].area == SPELL_AREA_SINGLE) {
         evt->state = 100;
         return;
      }
      PopulateCastingGrid(gTargetZ, gTargetX, gSpells[gCurrentSpell].fieldSize, 1);
      evt->state++;

   // fallthrough
   case 5:
      if (gTargetX != gMapCursorX || gTargetZ != gMapCursorZ) {
         evt->state = 3;
         evt->state2 = 6;

         // Might explain the misidentified while loop and buggy analysis:
         goto TargetingType6_80022da4;
      }
      if (gSavedPadStateNewPresses & PAD_X) {
         evt->state = 99;
         return;
      }
      if (!(gSavedPadStateNewPresses & PAD_CIRCLE)) {
         return;
      }
      evt->state2 = gSpells[gCurrentSpell].targeting;
      evt->state++;

   // fallthrough
   case 6:

      switch (evt->state2) {
      case SPELL_TARGET_ENEMY:
         unitIdx = gMapUnitsPtr[gTargetZ][gTargetX].unitIdx;
         if (unitIdx != 0 && caster->team != gUnits[unitIdx].team) {
            evt->state = 100;
            return;
         }
         evt->state = 5;
         evt->state2 = 0;
         return;
      case SPELL_TARGET_ALLY:
         unitIdx = gMapUnitsPtr[gTargetZ][gTargetX].unitIdx;
         if (unitIdx != 0 && caster->team == gUnits[unitIdx].team) {
            evt->state = 100;
            return;
         }
         evt->state = 5;
         evt->state2 = 0;
         return;
      case SPELL_TARGET_ENEMY_GROUP:
         evt->state = 5;
         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gYellowTargetGridPtr[iz][ix] != 0) {
                  unitIdx = gMapUnitsPtr[iz][ix].unitIdx;
                  if (unitIdx != 0 && caster->team != gUnits[unitIdx].team) {
                     evt->state = 100;
                  }
               }
            }
         }
         evt->state2 = 0;
         return;
      case SPELL_TARGET_ALLY_GROUP:
         evt->state = 5;
         for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (gYellowTargetGridPtr[iz][ix] != 0) {
                  unitIdx = gMapUnitsPtr[iz][ix].unitIdx;
                  if (unitIdx != 0 && caster->team == gUnits[unitIdx].team) {
                     evt->state = 100;
                  }
               }
            }
         }
         evt->state2 = 0;
         return;
      case 6:
         evt->state = 100;
         return;
      default:
         return;
      } // switch (evt->state2) (via state:6)

      return;

   case 99:
      gSignal2 = 1;
      ClearGrid(0);
      ClearGrid(1);
      evt->functionIndex = EVTF_NULL;
      return;

   case 100:
      CloseWindow(0x1e);
      ClearGrid(0);
      ClearGrid(1);
      gClearSavedPadState = 1;
      gSignal2 = 2;

      newEvt = Evt_GetLastUnused();
      newEvt->functionIndex = EVTF_UNIT_CASTING;
      /// TODO: Replace with d.evtf028 once defined:
      newEvt->d.sprite.x1 = EVT.x;
      newEvt->d.sprite.z1 = EVT.z;

      evt->state++;
      return;

   case 101:
      if (gSignal2 == 99) {
         gState.mapCursorOutOfRange = 0;
         evt->functionIndex = EVTF_NULL;
      }
      return;

   } // switch (evt->state)
}
