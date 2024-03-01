#include "common.h"
#include "units.h"
#include "object.h"
#include "window.h"
#include "battle.h"
#include "state.h"
#include "audio.h"

void CommitPartyStatus(void) {
   s32 i;
   UnitStatus *pUnit = &gUnits[1];

   for (i = 1; i < ARRAY_COUNT(gUnits); i++) {
      if (pUnit->idx != 0) {
         CommitPartyMemberStatus(pUnit);
      }
      pUnit++;
   }
}

u8 s_i_80123284;
u8 s_slot_80123288;
u8 s_delay_8012328c;
u8 s_penalized_80123290;
s32 s_totalReward_80123294;
s32 s_currentReward_80123298;

#undef OBJF
#define OBJF 594
void Objf594_BattleResults(Object *obj) {
   static u8 goldBuffer[15] = "\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x82\x66";
   s32 i;
   Object *rewardObj;

   switch (obj->state) {
   case 0:
      CommitPartyStatus();
      DrawWindow(0x3e, 0, 180, 144, 36, 76, 16, WBS_CROSSED, 0);
      DisplayCustomWindow(0x3e, 0, 1, 2, 0, 0);
      DrawText(12, 191, 25, 2, 0, "Battle results");
      DrawWindow(0x3c, 0, 0, 296, 162, 10, 39, WBS_DRAGON, 0);
      DisplayCustomWindow(0x3c, 0, 1, 2, 0, 0);
      DisplayCustomWindow(0x3d, 0, 1, 2, 0, 0);
      s_delay_8012328c = 15;
      obj->state++;
      break;

   case 1:

      switch (obj->state2) {
      case 0:
         if (--s_delay_8012328c == 0) {
            s_slot_80123288 = 0;
            s_i_80123284 = 0;
            s_penalized_80123290 = 0;
            obj->state2++;
         }
         break;

      case 1:

         while (gSlainUnits[s_i_80123284].count == 0 || gSlainUnits[s_i_80123284].reward == 0) {
            s_i_80123284++;
            if (s_i_80123284 == 20) {
               obj->state2 += 2;
               return;
            }
         }

         if (!s_penalized_80123290) {
            i = 0;
            while (gSlainUnits[s_i_80123284].unitId != gSpriteStripUnitIds[i]) {
               i++;
            }
            if (gBattleUnitRewards[gState.mapNum][i] < 0) {
               s_penalized_80123290 = 1;
               i = s_slot_80123288 % 8;
               if (i != 0) {
                  // Penalties are shown on a separate row
                  s_slot_80123288 -= (i - 8);
               }
            }
         }
         gSlainUnits[s_i_80123284].count -= 1;
         if (s_slot_80123288 >= 32) {
            // Clear existing if too many
            s_slot_80123288 = 0;
            Obj_ResetByFunction(OBJF_BATTLE_RESULTS_UNIT);
         }
         rewardObj = Obj_GetUnused();
         rewardObj->functionIndex = OBJF_BATTLE_RESULTS_UNIT;
         rewardObj->d.objf593.unitId = gSlainUnits[s_i_80123284].unitId;
         rewardObj->d.objf593.slot = s_slot_80123288++;
         if (s_penalized_80123290) {
            rewardObj->d.objf593.isPenalty = 1;
         }
         s_currentReward_80123298 = gSlainUnits[s_i_80123284].reward;
         s_delay_8012328c = 10;
         obj->state2++;

      // fallthrough
      case 2:
         if (--s_delay_8012328c == 0) {
            obj->state2--;
         }
         break;

      case 3:
         if (!s_penalized_80123290) {
            i = s_slot_80123288 % 8;
            if (i != 0) {
               s_slot_80123288 -= (i - 8);
            }
         }
         s_i_80123284 = 0;
         obj->state2++;

      // fallthrough
      case 4:
         while (!gPartyMemberSlain[s_i_80123284]) {
            s_i_80123284++;
            if (s_i_80123284 == 12) {
               obj->state2 += 2;
               return;
            }
         }
         gPartyMemberSlain[s_i_80123284] = 0;
         if (s_slot_80123288 >= 32) {
            // Clear existing if too many
            s_slot_80123288 = 0;
            Obj_ResetByFunction(OBJF_BATTLE_RESULTS_UNIT);
         }
         rewardObj = Obj_GetUnused();
         rewardObj->functionIndex = OBJF_BATTLE_RESULTS_UNIT;
         rewardObj->d.objf593.unitId = gSpriteStripUnitIds[s_i_80123284];
         rewardObj->d.objf593.slot = s_slot_80123288++;
         rewardObj->d.objf593.isPenalty = 1;
         s_currentReward_80123298 -= gBattlePenalties[gState.mapNum];
         s_delay_8012328c = 10;
         obj->state2++;

      // fallthrough
      case 5:
         if (--s_delay_8012328c == 0) {
            obj->state2--;
         }
         break;

      case 6:
         gState.gold += s_totalReward_80123294;
         obj->state++;

      // fallthrough
      case 7:
         gSignal2 = 1;
         break;
      } // switch (obj->state2) (via state:1)

      break;
   } // switch (obj->state)

   switch (obj->state3) {
   case 0:
      s_totalReward_80123294 = 0;
      s_currentReward_80123298 = 0;
      DrawWindow(0x3f, 0, 218, 104, 36, 188, 188, WBS_CROSSED, 0);
      EmbedIntAsSjis(s_totalReward_80123294, goldBuffer, 6);
      DrawSjisText(12, 229, 10, 2, 0, goldBuffer);
      DisplayCustomWindow(0x3f, 0, 1, 2, 0, 0);
      obj->state3++;
   // fallthrough
   case 1:
      if (s_currentReward_80123298 != 0) {
         s_totalReward_80123294 += s_currentReward_80123298;
         if (s_totalReward_80123294 < 0) {
            s_totalReward_80123294 = 0;
         }
         s_currentReward_80123298 = 0;
         EmbedIntAsSjis(s_totalReward_80123294, goldBuffer, 6);
         DrawSjisText(12, 229, 10, 2, 0, goldBuffer);
      }
   }
}

#undef OBJF
#define OBJF 593
void Objf593_BattleResultsUnit(Object *obj) {
   s32 i;

   switch (obj->state) {
   case 0:
      PerformAudioCommand(AUDIO_CMD_PLAY_SFX(239));
      OBJ.unitClut = gUnitClutIds[OBJ.unitId];

      // Find matching sprite strip
      i = 0;
      while (gSpriteStripUnitIds[i] != OBJ.unitId) {
         i++;
      }
      OBJ.unitGfxIdx = GFX_STRIP_SPRITES_OFS + i;

      i = OBJ.slot;
      obj->x1.n = (i % 8) * 32 + 28;
      obj->y1.n = (i / 8) * 30 + 57;
      obj->x3.n = obj->x1.n + 33;
      obj->y3.n = obj->y1.n + 33;
      OBJ.otOfs = 2;
      obj->state++;

   // fallthrough
   case 1:
      if (OBJ.isPenalty) {
         OBJ.clut = CLUT_NULL;
         OBJ.gfxIdx = GFX_RED_X;
         AddObjPrim_Gui(gGraphicsPtr->ot, obj);
      }
      OBJ.gfxIdx = OBJ.unitGfxIdx;
      OBJ.clut = OBJ.unitClut;
      AddObjPrim_Gui(gGraphicsPtr->ot, obj);
   }
}

void TallySlainUnit(UnitStatus *unit) {
   s32 i;

   if (unit->unitId <= UNIT_ID_END_OF_PARTY) {
      gPartyMemberSlain[unit->name - 1] = 1;
   } else {
      for (i = 0; i < ARRAY_COUNT(gSlainUnits); i++) {
         if (unit->unitId == gSlainUnits[i].unitId) {
            gSlainUnits[i].count++;
         }
      }
   }
}
