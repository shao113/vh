#include "common.h"
#include "units.h"
#include "evt.h"
#include "window.h"
#include "battle.h"
#include "state.h"

u8 s_currentIdx_8012329c;

void Evtf596_StatusWindowMgr(EvtData *evt) {
   // Null-terminated list of gUnits-indices of each available party member
   extern u8 gPartyMemberUnitIdx[PARTY_CT];

   s32 i, j;
   s32 ct;
   UnitStatus *pUnit;
   EvtData *statusWindow;

   switch (evt->state) {
   case 0:
      ct = 0;
      for (i = 1; i < PARTY_CT; i++) {
         if (gPartyMembers[i].inParty) {
            pUnit = &gUnits[1];
            for (j = 1; j < ARRAY_COUNT(gUnits); j++) {
               if (pUnit->idx != 0 && pUnit->name == i) {
                  gPartyMemberUnitIdx[ct++] = j;
                  break;
               }
               pUnit++;
            }
         }
      }
      gPartyMemberUnitIdx[ct] = 0; //<- Null-terminator
      s_currentIdx_8012329c = 0;
      evt->state++;

   // fallthrough
   case 1:
      gSignal2 = 0;
      statusWindow = Evt_GetUnused();
      statusWindow->functionIndex = EVTF_STATUS_WINDOW;
      statusWindow->d.evtf595.unit = &gUnits[gPartyMemberUnitIdx[s_currentIdx_8012329c]];
      if (gPartyMemberUnitIdx[1] != 0) {
         // Multiple party members are available to switch between.
         statusWindow->d.evtf595.multiple = 1;
      }
      evt->state++;

   // fallthrough
   case 2:
      if (gSignal2 == 99) {
         gSignal2 = 100;
         evt->functionIndex = EVTF_NULL;
      }
      if (gSignal2 == 1) {
         // Switching left
         if (s_currentIdx_8012329c == 0) {
            // Already on first unit, so walk to last
            s_currentIdx_8012329c = 0;
            while (gPartyMemberUnitIdx[s_currentIdx_8012329c] != 0) {
               s_currentIdx_8012329c++;
            }
         }
         s_currentIdx_8012329c--;
         evt->state--;
      }
      if (gSignal2 == 2) {
         // Switching right
         if (gPartyMemberUnitIdx[s_currentIdx_8012329c + 1] == 0) {
            // Already on last unit, so loop back to first
            s_currentIdx_8012329c = 0;
         } else {
            s_currentIdx_8012329c++;
         }
         evt->state--;
      }
   }
}

#undef EVTF
#define EVTF 595
void Evtf595_StatusWindow(EvtData *evt) {
   EvtData *portrait;
   EvtData *arrow;
   s16 size;
   s32 hasItems;
   UnitStatus *unit = EVT.unit;

   switch (evt->state2) {
   case 0:
      evt->state2++;

   // fallthrough
   case 1:
      portrait = Evt_GetLastUnused();
      portrait->functionIndex = EVTF_STATUS_PORTRAIT;
      portrait->d.evtf575.portraitId = gUnitPortraitIds[unit->unitId];
      UpdateSkillStatusWindow(unit);
      if (unit->team == TEAM_PLAYER) {
         DisplayBasicWindow(0x20);
      } else {
         DisplayCustomWindow(0x20, 0, 1, 0, 0, 23);
      }
      gWindowChoiceHeight = 18;
      gWindowChoicesTopMargin = 10;
      gWindowActiveIdx = 0x3c;

      if (unit->items[0] != ITEM_NULL || unit->items[1] != ITEM_NULL) {
         hasItems = 1;
      } else {
         hasItems = 0;
      }

      if (unit->spells[0] == SPELL_NULL && !hasItems) {
         evt->state2 += 1;
      }
      if (unit->spells[0] != SPELL_NULL && !hasItems) {
         DrawWindow(0x3c, 316, 0, 64, 54, 241, 90, WBS_CROSSED, 2);
         DrawText(316, 11, 20, 2, 0, "Skill\nSpell");
         DisplayBasicWindow(0x3c);
         evt->state2 += 2;
      }
      if (unit->spells[0] == SPELL_NULL && hasItems) {
         DrawWindow(0x3c, 316, 0, 64, 54, 241, 90, WBS_CROSSED, 2);
         DrawText(316, 11, 20, 2, 0, "Skill\nItems");
         DisplayBasicWindow(0x3c);
         evt->state2 += 3;
      }
      if (unit->spells[0] != SPELL_NULL && hasItems) {
         DrawWindow(0x3c, 316, 0, 64, 72, 241, 90, WBS_CROSSED, 3);
         DrawText(316, 11, 20, 2, 0, "Skill\nSpell\nItems");
         DisplayBasicWindow(0x3c);
         evt->state2 += 4;
      }

      evt->mem = 0;
      break;

   case 2:
      break;

   case 3:

      switch (evt->mem) {
      case 0:
         if (GetWindowChoice(0x3c) == 2) {
            CloseWindow(0x20);
            ClearIcons();
            Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
            DisplaySpellStatusWindow(unit, 0x3d);
            evt->mem++;
         }
         break;
      case 1:
         if (GetWindowChoice(0x3c) == 1) {
            ClearIcons();
            CloseWindow(0x3d);
            evt->state2 = 1;
         }
         break;
      }

      break;

   case 4:

      switch (evt->mem) {
      case 0:
         if (GetWindowChoice(0x3c) == 2) {
            CloseWindow(0x20);
            ClearIcons();
            Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
            DisplayItemsStatusWindow(unit, 0x3d);
            evt->mem++;
         }
         break;
      case 1:
         if (GetWindowChoice(0x3c) == 1) {
            ClearIcons();
            CloseWindow(0x3d);
            evt->state2 = 1;
         }
         break;
      }

   // fallthrough
   case 5:

      switch (evt->mem) {
      case 0:
         if (GetWindowChoice(0x3c) == 2) {
            CloseWindow(0x20);
            Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
            ClearIcons();
            DisplaySpellStatusWindow(unit, 0x3d);
            evt->mem++;
         }
         if (GetWindowChoice(0x3c) == 3) {
            CloseWindow(0x20);
            Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
            ClearIcons();
            DisplayItemsStatusWindow(unit, 0x3d);
            evt->mem += 2;
         }
         break;

      case 1:
         if (GetWindowChoice(0x3c) == 1) {
            ClearIcons();
            CloseWindow(0x3d);
            evt->state2 = 1;
         }
         if (GetWindowChoice(0x3c) == 3) {
            DisplayItemsStatusWindow(unit, 0x3d);
            evt->mem++;
         }
         break;

      case 2:
         if (GetWindowChoice(0x3c) == 2) {
            ClearIcons();
            DisplaySpellStatusWindow(unit, 0x3d);
            evt->mem--;
         }
         if (GetWindowChoice(0x3c) == 1) {
            ClearIcons();
            CloseWindow(0x3d);
            evt->state2 = 1;
         }
         break;
      }

      break;
   }

   if (gPadStateNewPresses & PAD_X) {
      CloseWindow(0x20);
      Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
      ClearIcons();
      CloseWindow(0x3c);
      CloseWindow(0x3d);
      gSignal2 = 99;
      evt->functionIndex = EVTF_NULL;
   }
   if (EVT.multiple) {
      // Pulsating selection arrows
      size = (rcos(gOscillation) * 3 >> 12) + 10;
      arrow = Evt_GetUnused();
      arrow->x1.n = 20 + size;
      arrow->x3.n = 20 - size;
      arrow->y1.n = 136 - size;
      arrow->y3.n = 136 + size;
      arrow->d.sprite.gfxIdx = GFX_SELECTION_ARROW;
      AddEvtPrim_Gui(gGraphicsPtr->ot, arrow);
      arrow->x1.n = 300 - size;
      arrow->x3.n = 300 + size;
      AddEvtPrim_Gui(gGraphicsPtr->ot, arrow);
      if (gPadStateNewPresses & PAD_LEFT) {
         CloseWindow(0x20);
         Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
         ClearIcons();
         CloseWindow(0x3c);
         CloseWindow(0x3d);
         gSignal2 = 1;
         evt->functionIndex = EVTF_NULL;
      }
      if (gPadStateNewPresses & PAD_RIGHT) {
         CloseWindow(0x20);
         Evt_ResetByFunction(EVTF_STATUS_PORTRAIT);
         ClearIcons();
         CloseWindow(0x3c);
         CloseWindow(0x3d);
         gSignal2 = 2;
         evt->functionIndex = EVTF_NULL;
      }
   }
}
