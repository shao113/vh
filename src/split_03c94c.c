#include "common.h"
#include "evt.h"
#include "window.h"
#include "units.h"
#include "battle.h"
#include "field.h"
#include "state.h"
#include "graphics.h"

void ShowMsgBoxForSprite(EvtData *sprite, u8 lower, u8 omitTail) {
   EvtData *tail;

   if (!lower) {
      DrawWindow(0x41, 0, 0, 296, 64, 12, 16, WBS_ROUNDED, 0);
      DisplayCustomWindow(0x41, 0, 1, 3, 0, 25);
      DisplayCustomWindow(0x42, 0, 1, 3, 0, 25);
      if (!omitTail) {
         tail = Evt_GetUnused();
         tail->functionIndex = EVTF_UPPER_MSGBOX_TAIL;
         tail->d.evtf421.sprite = sprite;
      }
   } else {
      DrawWindow(0x43, 0, 100, 296, 64, 12, 161, WBS_ROUNDED, 0);
      DisplayCustomWindow(0x43, 0, 1, 3, 0, 25);
      DisplayCustomWindow(0x44, 0, 1, 3, 0, 25);
      if (!omitTail) {
         tail = Evt_GetUnused();
         tail->functionIndex = EVTF_LOWER_MSGBOX_TAIL;
         tail->d.evtf421.sprite = sprite;
      }
   }
}

void SetMsgBoxPortrait(short portraitId, u8 lower) {
   s32 i;
   EvtData *evt1;

   evt1 = gEvtDataArray;
   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt1->functionIndex == EVTF_MSGBOX_PORTRAIT && evt1->d.evtf413.flipped == lower) {
         evt1->d.evtf413.faceSprite->functionIndex = EVTF_NULL;
         evt1->d.evtf413.speakSprite->functionIndex = EVTF_NULL;
         evt1->d.evtf413.blinkSprite->functionIndex = EVTF_NULL;
         break;
      }
      evt1++;
   }
   if (i == EVT_DATA_CT) {
      evt1 = Evt_GetUnused();
   }

   evt1->functionIndex = EVTF_MSGBOX_PORTRAIT;
   evt1->state = 0;
   evt1->state2 = 0;
   evt1->d.evtf413.blinkState = 0;
   evt1->d.evtf413.flipped = lower;
   evt1->d.evtf413.portrait.id = portraitId;

   if (!lower) {
      evt1->d.evtf413.x = 26;
      evt1->d.evtf413.y = 25;
   } else {
      evt1->d.evtf413.x = 246;
      evt1->d.evtf413.y = 170;
   }
}

void CloseMsgBox(u8 lower) {
   s32 i;
   EvtData *evt1;

   if (!lower) {
      CloseWindow(0x41);
      CloseWindow(0x42);
      evt1 = gEvtDataArray;
      for (i = 0; i < EVT_DATA_CT; i++) {
         if (evt1->functionIndex == EVTF_UPPER_MSGBOX_TAIL) {
            evt1->functionIndex = EVTF_NULL;
         }
         evt1++;
      }
   } else {
      CloseWindow(0x43);
      CloseWindow(0x44);
      evt1 = gEvtDataArray;
      for (i = 0; i < EVT_DATA_CT; i++) {
         if (evt1->functionIndex == EVTF_LOWER_MSGBOX_TAIL) {
            evt1->functionIndex = EVTF_NULL;
         }
         evt1++;
      }
   }
   evt1 = gEvtDataArray;
   for (i = 0; i < EVT_DATA_CT; i++) {
      if (evt1->functionIndex == EVTF_MSGBOX_PORTRAIT && evt1->d.evtf413.flipped == lower) {
         evt1->state = 99;
         break;
      }
      evt1++;
   }
}

#undef EVTF
#define EVTF 014
void Evtf014_BattleUnit(EvtData *evt) {
   s32 i_s0; // + portraitId, samex
   s32 textPtrIdx;
   u8 origAnimIdx;
   UnitStatus *unit;
   EvtData *sprite;
   u8 elevation;
   u8 tmpz, tmpx;
   s32 nextz, nextx;
   EvtData *evt1;
   EvtData *evt2;
   Quad *qswap;
   u8 **animSet;

   i_s0 = 0;
   unit = EVT.unit;
   sprite = EVT.sprite;
   origAnimIdx = EVT.animIdx;
   textPtrIdx = 0;

   switch (evt->state) {
   case 0:

      switch (evt->state2) {
      case 0:
         unit->team = EVT.team;
         CalculateUnitStats(unit);
         sprite = Evt_GetUnused();
         sprite->functionIndex = EVTF_NOOP;
         sprite->d.sprite.coords[0].z = rand() % 64 - 32;
         sprite->d.sprite.coords[0].x = rand();
         sprite->d.sprite.stripIdx = unit->stripIdx;
         unit->evtSprite = sprite;
         sprite->d.sprite.direction = unit->direction;
         EVT.animSet = gSpriteStripAnimSets[sprite->d.sprite.stripIdx];
         sprite->d.sprite.x1 = EVT.x;
         sprite->d.sprite.z1 = EVT.z;
         sprite->d.sprite.x3 = EVT.x;
         sprite->d.sprite.z3 = EVT.z;
         EVT.sprite = sprite;
         SPR_MAP_UNIT(sprite).s.unitIdx = EVT.unitIdx;
         SPR_MAP_UNIT(sprite).s.team = EVT.team;
         EVT.timer = rand() % 12 + 1;
         if (SPR_TILE_STATE(sprite).action == TA_X19) {
            SPR_TILE_STATE(sprite).action = TA_NONE;
            SetElevationFromTerrain(sprite);
            sprite->d.sprite.hidden = 1;
         }
         evt->state2++;
         break;

      case 1:
         if (--EVT.timer == 0) {
            sprite->d.sprite.animInitialized = 0;
            evt->state++;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:0)

      break;

   case 1:
      EVT.animIdx = ANIM_IDLE_B;
      if (SPR_TILE_STATE(sprite).action == TA_NONE) {
         break;
      }
      if (SPR_TILE_STATE(sprite).action == TA_X6) {
         EVT.pathIdx = 4;
         while (gPathBackToUnit[EVT.pathIdx] != PATH_STEP_INVALID) {
            EVT.pathIdx += 2;
         }
         LO(sprite->d.sprite.z3) = 0x80;
         LO(sprite->d.sprite.x3) = 0x80;
         evt->state2 = 0;
         evt->state++;
      }
      if (SPR_TILE_STATE(sprite).action == TA_MELEE_ATK) {
         evt->state = 8;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_PUSHING_CRATE) {
         evt->state = 15;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_PUSHING_BOULDER) {
         evt->state = 21;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_RANGED_ATK) {
         evt->state = 12;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_BLOCK) {
         evt->state = 9;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_PHYS_HIT) {
         evt->state = 10;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_MAG_HIT) {
         evt->state = 5;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_BOULDER_HIT) {
         evt->state = 22;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_PHYS_DEFEAT) {
         evt->state = 7;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_PHYS_DEFEAT_MSG) {
         evt->state = 18;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_MAG_DEFEAT_MSG) {
         evt->state = 19;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_MAG_DEFEAT) {
         evt->state = 13;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_XB) {
         evt->state = 3;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_CHOOSING_DIRECTION) {
         gSignal4 = sprite->d.sprite.direction;
         EVT.direction = sprite->d.sprite.direction;
         evt->state = 4;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_CAST) {
         evt->state = 6;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_LEVEL_UP) {
         evt->state = 16;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_X16) {
         evt->state = 17;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_X1D) {
         evt->state = 23;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_X1E) {
         evt->state = 24;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_X1F) {
         evt->state = 25;
         evt->state2 = 0;
      } else if (SPR_TILE_STATE(sprite).action == TA_X20) {
         SPR_TILE_STATE(sprite).action = TA_NONE;
         gState.msgFinished = 0;
         if (unit->name == UNIT_KURTZ) {
            SetupBattleMsgBox(UNIT_KURTZ, PORTRAIT_KURTZ_511, 0x22);
         } else if (unit->name == UNIT_DALLAS) {
            SetupBattleMsgBox(UNIT_DALLAS, PORTRAIT_DALLAS_516, 0x22);
         } else if (unit->name == UNIT_SABINA) {
            SetupBattleMsgBox(UNIT_SABINA, PORTRAIT_SABINA_509, 0x22);
         } else if (unit->name == UNIT_KANE) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_ANGRY, 0x23);
            gState.mapState.n.field_0x12 = 1;
         } else if (unit->name > UNIT_END_OF_PARTY) {
            gState.msgFinished = 1;
         } else {
            // Party member
            if (gState.mapNum == 13 || gState.mapNum == 35) {
               // Injured portrait when party member caught in bridge destruction.
               SetupBattleMsgBox(unit->name, (unit->name - 1) * 7 + 2, unit->name + 21);
            } else {
               // Neutral portrait when being washed away by river etc.
               SetupBattleMsgBox(unit->name, (unit->name - 1) * 7 + 1, unit->name + 21);
            }
         }
      } else if (SPR_TILE_STATE(sprite).action == TA_X9) {
         SPR_TILE_STATE(sprite).action = TA_NONE;
         gMapUnitsPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].raw = 0;
         HI(sprite->d.sprite.z1) = gPathBackToUnit[0];
         HI(sprite->d.sprite.x1) = gPathBackToUnit[1];
         sprite->d.sprite.z3 = sprite->d.sprite.z1;
         sprite->d.sprite.x3 = sprite->d.sprite.x1;
         SPR_MAP_UNIT(sprite).s.unitIdx = EVT.unitIdx;
         SPR_MAP_UNIT(sprite).s.team = EVT.team;
         evt->state = 3;
         evt->state2 = 0;
      }

      break;

   case 2:

      switch (evt->state2) {
      case 0:
         sprite->d.sprite.x2 = 0x20;
         sprite->d.sprite.z2 = 0x20;
         if (unit->class == CLASS_AIRMAN) {
            elevation = SPR_TERRAIN(sprite).elevation;
            i_s0 = EVT.pathIdx;
            while (gPathBackToUnit[i_s0 - 1] != PATH_STEP_INVALID) {
               tmpz = gPathBackToUnit[i_s0 - 2];
               tmpx = gPathBackToUnit[i_s0 - 1];
               if (gTerrainPtr[tmpz][tmpx].elevation > elevation) {
                  elevation = gTerrainPtr[tmpz][tmpx].elevation;
               }
               i_s0 -= 2;
            }
            sprite->d.sprite.y3 = elevation * 128;
         }
         SPR_MAP_UNIT(sprite).raw = 0;

      LAB_8004cfb8: //<- from state2:1 after decrement
         SPR_TILE_STATE(sprite).action = TA_NONE;
         tmpx = gPathBackToUnit[--EVT.pathIdx];
         tmpz = gPathBackToUnit[--EVT.pathIdx];
         if (tmpz != PATH_STEP_INVALID) {
            if (unit->class == CLASS_AIRMAN) {
               if (tmpx == HI(sprite->d.sprite.x1)) {
                  i_s0 = 1;
               } else {
                  i_s0 = 0;
               }
               while (gPathBackToUnit[EVT.pathIdx - 1] != PATH_STEP_INVALID) {
                  nextx = gPathBackToUnit[EVT.pathIdx - 1];
                  nextz = gPathBackToUnit[EVT.pathIdx - 2];
                  if (i_s0) {
                     if (tmpx != nextx) {
                        break;
                     }
                  } else {
                     if (tmpz != nextz) {
                        break;
                     }
                  }
                  EVT.pathIdx -= 2;
                  tmpx = nextx;
                  tmpz = nextz;
               }
            }
            HI(sprite->d.sprite.z3) = tmpz;
            HI(sprite->d.sprite.x3) = tmpx;
            sprite->d.sprite.finishedMoving = 0;
            evt->state2++;
         } else {
            evt->state = 1;
            gSignal3 = 1;
            gMapUnitsPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].s.unitIdx = EVT.unitIdx;
            gMapUnitsPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].s.team = EVT.team;
         }

         break;

      case 1:
         if (sprite->d.sprite.finishedMoving) {
            evt->state2--;
            goto LAB_8004cfb8;
         }

      } // switch (evt->state2) (via state:2)

      break;

   case 3:
      sprite->d.sprite.direction = unit->direction;
      SPR_TILE_STATE(sprite).action = TA_NONE;
      evt->state = 1;
      evt->state2 = 0;
      break;

   case 4:
      SPR_TILE_STATE(sprite).action = TA_NONE;
      EVT.animIdx = ANIM_IDLE_B;
      sprite->d.sprite.direction = gSignal4;
      if (gSignal2 == 1) {
         unit->direction = gSignal4;
         evt->state = 1;
         evt->state2 = 0;
      }
      if (gSignal2 == 2) {
         sprite->d.sprite.direction = EVT.direction;
         evt->state = 1;
         evt->state2 = 0;
      }
      break;

   case 5:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_STRUCK_END_B;
         gSignal3 = 0;
         evt->state2++;
         break;
      case 1:
         if (gSignal3 == 1) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 6:

      switch (evt->state2) {
      case 0:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_CASTING_FX;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_CASTING_B;
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            if (gSpellSounds[gCurrentSpell] != 0) {
               PerformAudioCommand(gSpellSounds[gCurrentSpell]);
            }
            gSignal3 = 1;
            evt->state2++;
         }
         break;

      case 2:
         gSignal3 = 2;
         evt->state = 1;
         evt->state2 = 0;
         break;
      }

      break;

   case 7:

      switch (evt->state2) {
      case 0:
         TallySlainUnit(unit);
         CommitPartyMemberStatus(unit);
         // Prevent premature looting of kurtz, sabina, etc. during village escape
         if (gState.mapNum != 29) {
            gState.droppedItem = unit->droppedItem;
         }
         EVT.animIdx = ANIM_STRUCK_END_B;
         EVT.timer = 35;
         evt1 = Evt_GetUnused();
         if (UnitIsRocky(unit)) {
            evt1->functionIndex = EVTF_ROCK_SPURT;
         } else {
            evt1->functionIndex = EVTF_BLOOD_SPURT;
         }
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            evt2 = Evt_GetUnused();
            evt2->functionIndex = EVTF_SLAY_UNIT;
            // TODO replace
            HI(evt2->d.sprite.x1) = HI(sprite->d.sprite.x1);
            HI(evt2->d.sprite.z1) = HI(sprite->d.sprite.z1);
            gSignal3 = 0;
            evt->state2++;
         }
         break;

      case 2:
         if (gSignal3 != 0) {
            gTileStateGridPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].action = TA_NONE;
            SPR_MAP_UNIT(sprite).raw = 0;
            sprite->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
            unit->idx = 0;
         }
         break;
      } // switch (evt->state2) (via state:7)

      break;

   case 8:

      switch (evt->state2) {
      case 0:
         if (gUnitSounds_Attacking1[unit->unitId][0] != 0) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_AUDIO_CMD;
            evt1->d.evtf581.cmd = gUnitSounds_Attacking1[unit->unitId][0];
            evt1->d.evtf581.delay = gUnitSoundDelays_Attacking1[unit->unitId][0];
            if (gUnitSoundDelays_Attacking1[unit->unitId][1] != 0) {
               evt1 = Evt_GetUnused();
               evt1->functionIndex = EVTF_AUDIO_CMD;
               evt1->d.evtf581.cmd = gUnitSounds_Attacking1[unit->unitId][1];
               evt1->d.evtf581.delay = gUnitSoundDelays_Attacking1[unit->unitId][1] +
                                       gUnitSoundDelays_Attacking1[unit->unitId][0];
            }
         }
         if (gUnitSounds_Attacking2[unit->unitId][0] != 0) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_AUDIO_CMD;
            evt1->d.evtf581.cmd = gUnitSounds_Attacking2[unit->unitId][0];
            evt1->d.evtf581.delay = gUnitSoundDelays_Attacking2[unit->unitId][0];
            if (gUnitSoundDelays_Attacking2[unit->unitId][1] != 0) {
               evt1 = Evt_GetUnused();
               evt1->functionIndex = EVTF_AUDIO_CMD;
               evt1->d.evtf581.cmd = gUnitSounds_Attacking2[unit->unitId][1];
               evt1->d.evtf581.delay = gUnitSoundDelays_Attacking2[unit->unitId][1] +
                                       gUnitSoundDelays_Attacking2[unit->unitId][0];
            }
         }
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_ATTACKING_B;
         if (unit->unitId == UNIT_ID_BAHAMUT || unit->unitId == UNIT_ID_SALAMANDER) {
            evt2 = Evt_GetUnused();
            evt2->functionIndex = EVTF_FLAME_BREATH;
            // TODO replace
            evt2->d.sprite.x1 = sprite->d.sprite.x1;
            evt2->d.sprite.z1 = sprite->d.sprite.z1;
         }
         if (unit->unitId == UNIT_ID_ASH_VANDALIER || unit->unitId == UNIT_ID_DOLF_DARK_ANGEL) {
            evt2 = Evt_GetUnused();
            evt2->functionIndex = EVTF_ELITE_MELEE_SPARKLES;
            // TODO replace
            evt2->d.sprite.x1 = sprite->d.sprite.x1;
            evt2->d.sprite.z1 = sprite->d.sprite.z1;
         }
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            EVT.animIdx = ANIM_ATTACKING_END_B;
            gSignal3 = 1;
            evt->state2++;
         }
         break;

      case 2:
         if (sprite->d.sprite.animFinished) {
            gSignal4 = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:8)

      break;

   case 9:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(gUnitSounds_Blocking[unit->unitId]);
         EVT.animIdx = ANIM_BLOCKING_B;
         SPR_TILE_STATE(sprite).action = TA_NONE;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_UNIT_BLOCKING;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            gSignal3 = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 10:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(gUnitSounds_Struck[unit->unitId]);
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_STRUCK_B;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_UNIT_STRUCK;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            gSignal3 = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 11:
      if (SPR_TILE_STATE(sprite).action != TA_X4) {
         evt->state = 3;
         evt->state2 = 0;
      } else {
         sprite->d.sprite.direction = SPR_TILE_STATE(sprite).cachedShort;
      }
      break;

   case 12:

      switch (evt->state2) {
      case 0:
         if (gUnitSounds_Attacking1[unit->unitId][0] != 0) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_AUDIO_CMD;
            evt1->d.evtf581.cmd = gUnitSounds_Attacking1[unit->unitId][0];
            evt1->d.evtf581.delay = gUnitSoundDelays_Attacking1[unit->unitId][0];
            if (gUnitSoundDelays_Attacking1[unit->unitId][1] != 0) {
               evt1 = Evt_GetUnused();
               evt1->functionIndex = EVTF_AUDIO_CMD;
               evt1->d.evtf581.cmd = gUnitSounds_Attacking1[unit->unitId][1];
               evt1->d.evtf581.delay = gUnitSoundDelays_Attacking1[unit->unitId][1] +
                                       gUnitSoundDelays_Attacking1[unit->unitId][0];
            }
         }
         SPR_TILE_STATE(sprite).action = TA_NONE;
         if (CalculateProjectileHeight(HI(sprite->d.sprite.x1), HI(sprite->d.sprite.z1), gTargetX,
                                       gTargetZ) > 0x20000) {
            EVT.animIdx = ANIM_ATTACKING_FAR_B;
         } else {
            EVT.animIdx = ANIM_ATTACKING_B;
         }
         if (gState.mapNum == 13 && unit->name == UNIT_KIRA && gState.mapState.n.field_0x13 == 0) {
            evt->state2++;
            gState.mapState.n.field_0x13 = 1;
         } else {
            evt->state2 = 4;
         }
         break;

      case 1:
         ShowMsgBoxForSprite(sprite, 0, 0);
         SetMsgBoxPortrait(PORTRAIT_KIRA_ANGRY, 0);
         SetMsgBoxText(1, 0x14, 0x100);
         evt->state2++;
         break;

      case 2:
         if (gState.msgBoxFinished) {
            ShowMsgBoxForSprite(sprite, 1, 1);
            SetMsgBoxPortrait(PORTRAIT_ASH, 1);
            SetMsgBoxText(2, 0x15, 0x100);
            evt->state2++;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            CloseMsgBox(0);
            CloseMsgBox(1);
            evt->state2++;
         }
         break;

      case 4:
         if (sprite->d.sprite.animFinished) {
            EVT.animIdx += 2;
            if (gUnitSounds_Attacking2[unit->unitId][0] != 0) {
               evt1 = Evt_GetUnused();
               evt1->functionIndex = EVTF_AUDIO_CMD;
               evt1->d.evtf581.cmd = gUnitSounds_Attacking2[unit->unitId][0];
               evt1->d.evtf581.delay = 0;
            }
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_PROJECTILE;
            if (gMapUnitsPtr[gTargetZ][gTargetX].s.team == TEAM_CHEST) {
               evt1->functionIndex = EVTF_PROJECTILE_INDIRECT;
            }
            evt1->d.evtf022.x = sprite->d.sprite.x1;
            evt1->d.evtf022.z = sprite->d.sprite.z1;
            evt1->d.evtf022.y = sprite->d.sprite.y1 + 0x80;

            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_RANGED_TARGET_CAMERA;
            evt1->d.evtf023.targetSprite = GetUnitSpriteAtPosition(gTargetZ, gTargetX);
            evt->state2++;
         }
         break;

      case 5:
         if (sprite->d.sprite.animFinished) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:12)

      break;

   case 13:

      switch (evt->state2) {
      case 0:
         TallySlainUnit(unit);
         CommitPartyMemberStatus(unit);
         if (gState.mapNum != 29) {
            gState.droppedItem = unit->droppedItem;
         }
         gSignal3 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (gSignal3 == 1) {
            EVT.timer = 5;
            evt->state2++;
         } else {
            EVT.animIdx = ANIM_STRUCK_END_B;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            SPR_TILE_STATE(sprite).action = TA_NONE;
            SPR_MAP_UNIT(sprite).raw = 0;
            sprite->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
            unit->idx = 0;
         }
         break;
      }

      break;

   case 15:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_HIGH_STEP_B;
         if (unit->class == CLASS_AIRMAN) {
            EVT.timer = 11;
         } else {
            EVT.timer = 4;
         }
         evt->state2++;
         break;

      case 1:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x5e9);
            gSignal3 = 1;
            evt->state2++;
         }
         break;

      case 2:
         if (sprite->d.sprite.animFinished) {
            gSignal4 = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 16:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_LEVEL_UP_FX;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         EVT.animIdx = ANIM_HOP_B;
         if (unit->class == CLASS_AIRMAN) {
            EVT.animIdx = ANIM_IDLE_B;
            evt->state2 = 5;
         } else {
            evt->state2++;
         }
         break;

      case 1:
      case 2:
      case 3:
         if (sprite->d.sprite.animFinished) {
            sprite->d.sprite.animInitialized = 0;
            evt->state2++;
         }
         break;

      case 4:
         if (sprite->d.sprite.animFinished) {
            EVT.animIdx = ANIM_IDLE_B;
            evt->state2++;
         }

      // fallthrough
      case 5:
         if (gSignal3 != 0) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:16)

      break;

   case 17:
      TallySlainUnit(unit);
      CommitPartyMemberStatus(unit);
      if (gState.mapNum != 29) {
         gState.droppedItem = unit->droppedItem;
      }
      SPR_TILE_STATE(sprite).action = TA_NONE;
      SPR_MAP_UNIT(sprite).raw = 0;
      sprite->functionIndex = EVTF_NULL;
      evt->functionIndex = EVTF_NULL;
      unit->idx = 0;
      return;

   case 18:

      switch (evt->state2) {
      case 0:
         TallySlainUnit(unit);
         CommitPartyMemberStatus(unit);
         if (gState.mapNum != 29) {
            gState.droppedItem = unit->droppedItem;
         }
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_STRUCK_B;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_UNIT_STRUCK;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         evt1 = Evt_GetUnused();
         if (UnitIsRocky(unit)) {
            evt1->functionIndex = EVTF_ROCK_SPURT;
         } else {
            evt1->functionIndex = EVTF_BLOOD_SPURT;
         }
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            evt->state = 20;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 19:

      switch (evt->state2) {
      case 0:
         TallySlainUnit(unit);
         CommitPartyMemberStatus(unit);
         if (gState.mapNum != 29) {
            gState.droppedItem = unit->droppedItem;
         }
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_STRUCK_END_B;
         gSignal3 = 0;
         evt->state2++;
         break;

      case 1:
         if (gSignal3 == 1) {
            evt->state = 20;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 20:

      switch (evt->state2) {
      case 0:
         EVT.animIdx = ANIM_STRUCK_END_B;
         EVT.timer = 10;
         evt->state2++;
         break;

      case 1:
         evt->state2 = 99;
         if (unit->unitType == UNIT_TYPE_MAGE_TOWER) {
            ShowMsgBoxForSprite(sprite, 1, 1);
            if (gState.mapState.n.field_0x13 < 2) {
               SetMsgBoxPortrait(PORTRAIT_ASH_UPSET, 1);
            } else {
               SetMsgBoxPortrait(PORTRAIT_ASH_ANGRY, 1);
            }
            SetMsgBoxText(2, gState.mapState.n.field_0x13 + 15, 0x100);
            gState.mapState.n.field_0x13++;
            evt->state2 = 98;
         } else {
            ShowMsgBoxForSprite(sprite, 0, 0);
            if (unit->name <= UNIT_END_OF_PARTY) {
               i_s0 = (unit->name - 1) * 7 + 2;
               textPtrIdx = unit->name;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_ZOOT) {
               i_s0 = PORTRAIT_ZOOT_559;
               evt->state2 = 98;
               if (gState.mapNum == 10) {
                  i_s0 = PORTRAIT_ZOOT_449;
                  evt->state2 = 96;
               }
               textPtrIdx = 13;
            }
            if (unit->name == UNIT_HASSAN) {
               i_s0 = PORTRAIT_HASSAN_INJURED;
               textPtrIdx = 13;
               evt->state2 = 96;
            }
            if (unit->unitType == UNIT_TYPE_VILLAGER) {
               i_s0 = gUnitPortraitIds[unit->unitId] + 1;
               evt->state2 = 98;
               gState.mapState.n.field_0x13++;
               textPtrIdx = 13;
               if (gState.mapState.n.field_0x13 == 1) {
                  evt->state2 = 2;
               }
               if (gState.mapState.n.field_0x13 == 9) {
                  evt->state2 = 3;
               }
            }
            if (unit->name == UNIT_MAGNUS) {
               i_s0 = PORTRAIT_MAGNUS_DARK_LORD_492;
               textPtrIdx = 13;
               evt->state2 = 96;
            }
            if (unit->name == UNIT_CLIVE) {
               i_s0 = PORTRAIT_CLIVE_INJURED;
               textPtrIdx = 14;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_LANDO) {
               i_s0 = PORTRAIT_LANDO_INJURED;
               textPtrIdx = 20;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_DUMAS) {
               i_s0 = PORTRAIT_DUMAS_562;
               textPtrIdx = 21;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_DALLAS) {
               i_s0 = PORTRAIT_DALLAS_518;
               textPtrIdx = 20;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_KURTZ) {
               i_s0 = PORTRAIT_KURTZ_511;
               textPtrIdx = 13;
               evt->state2 = 98;
            }
            if (unit->unitType == UNIT_TYPE_SALAMANDER) {
               i_s0 = PORTRAIT_SALAMANDER;
               textPtrIdx = 13;
               evt->state2 = 98;
            }
            if (unit->name == UNIT_KANE && gState.mapNum == 40) {
               if (gState.mapNum == 40) {
                  i_s0 = PORTRAIT_KANE_BLK_KNIGHT_INJURED;
                  textPtrIdx = 27;
                  evt->state2 = 98;
               }
            }
            if (unit->name == UNIT_XENO) {
               i_s0 = PORTRAIT_XENO_INJURED;
               textPtrIdx = 23;
               evt->state2 = 98;
            }
            if (unit->unitType == UNIT_TYPE_DOOM_LORD) {
               PerformAudioCommand(0x1386);
               i_s0 = PORTRAIT_DOLF_495;
               textPtrIdx = 13;
               evt->state2 = 7;
            }
            if (unit->unitType == UNIT_TYPE_DARK_ANGEL) {
               i_s0 = PORTRAIT_DOLF_581;
               textPtrIdx = 14;
               evt->state2 = 96;
            }
            if (gState.mapNum == 38) {
               if (unit->name == UNIT_KANE) {
                  i_s0 = PORTRAIT_KANE_INJURED;
                  textPtrIdx = 16;
                  gState.mapState.n.field_0x12 = 1;
                  evt->state2 = 4;
               }
               if (unit->name == UNIT_SABINA) {
                  i_s0 = PORTRAIT_SABINA_509;
                  evt->state2 = 98;
                  textPtrIdx = 18;
                  if (gState.mapState.n.field_0x13 != 0 && gState.mapState.n.field_0x12 == 0) {
                     evt->state2 = 5;
                  }
               }
            }
            if (gState.mapNum == 29) {
               if (unit->name == UNIT_KANE) {
                  i_s0 = PORTRAIT_KANE_INJURED;
                  textPtrIdx = 13;
                  evt->state2 = 98;
               }
               if (unit->name == UNIT_SABINA) {
                  i_s0 = PORTRAIT_SABINA_508;
                  textPtrIdx = 14;
                  evt->state2 = 98;
               }
               if (unit->name == UNIT_DALLAS) {
                  i_s0 = PORTRAIT_DALLAS_517;
                  textPtrIdx = 15;
                  evt->state2 = 98;
               }
               if (unit->name == UNIT_KURTZ) {
                  i_s0 = PORTRAIT_KURTZ_511;
                  textPtrIdx = 16;
                  evt->state2 = 98;
               }
            }
            if (unit->name == UNIT_LEENA) {
               i_s0 = PORTRAIT_LEENA_573;
               textPtrIdx = 13;
               evt->state2 = 6;
            }
            SetMsgBoxPortrait(i_s0, 0);
            SetMsgBoxText(1, textPtrIdx, 0x100);
            gState.msgBoxFinished = 0;
         }
         break;

      case 2:
         if (gState.msgBoxFinished) {
            ShowMsgBoxForSprite(sprite, 1, 1);
            CloseWindow(0x38);
            SetMsgBoxPortrait(PORTRAIT_ASH_UPSET, 1);
            textPtrIdx = 14;
            SetMsgBoxText(2, textPtrIdx, 0x100);
            evt->state2 = 98;
         }
         break;

      case 3:
         if (gState.msgBoxFinished) {
            ShowMsgBoxForSprite(sprite, 1, 1);
            SetMsgBoxPortrait(PORTRAIT_ASH_UPSET, 1);
            CloseWindow(0x38);
            textPtrIdx = 15;
            SetMsgBoxText(2, textPtrIdx, 0x100);
            evt->state2 = 98;
         }
         break;

      case 4:
         if (gState.msgBoxFinished) {
            CloseWindow(0x38);
            ShowMsgBoxForSprite(sprite, 1, 1);
            SetMsgBoxPortrait(PORTRAIT_SABINA, 1);
            textPtrIdx = 17;
            SetMsgBoxText(2, textPtrIdx, 0x100);
            evt->state2 = 98;
         }
         break;

      case 5:
         if (gState.msgBoxFinished) {
            CloseWindow(0x38);
            ShowMsgBoxForSprite(sprite, 1, 1);
            SetMsgBoxPortrait(PORTRAIT_KANE_ANGRY, 1);
            textPtrIdx = 19;
            SetMsgBoxText(2, textPtrIdx, 0x100);
            evt->state2 = 98;
         }
         break;

      case 6:
         if (gState.msgBoxFinished) {
            ShowMsgBoxForSprite(sprite, 1, 1);
            SetMsgBoxPortrait(PORTRAIT_ASH_UPSET, 1);
            textPtrIdx = 14;
            SetMsgBoxText(2, textPtrIdx, 0x100);
            evt->state2 = 98;
         }
         break;

      case 7:
         if (gState.msgBoxFinished) {
            PerformAudioCommand(0x386);
            CloseMsgBox(0);
            EVT.timer = 90;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_TBD_732;
            // TODO replace
            HI(evt1->d.sprite.x1) = HI(sprite->d.sprite.x1);
            HI(evt1->d.sprite.z1) = HI(sprite->d.sprite.z1);
            gState.D_801405A4 = 0;
            evt->state2++;
         }
         break;

      case 8:
         if (--EVT.timer == 0) {
            SetupBattleUnit(13, HI(sprite->d.sprite.z1), HI(sprite->d.sprite.x1), 32, TEAM_ENEMY,
                            DIR_WEST, 100, 60, 99);
            SPR_MAP_UNIT(sprite).raw = 0;
            sprite->d.sprite.hidden = 1;
            EVT.timer = 90;
            evt->state2++;
         }
         break;

      case 9:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x31);
            gState.D_801405A4 = 1;
            /*gSignal3 = 1;
            gSignal4 = 1;
            sprite->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
            unit->idx = 0;*/
            //?
            goto LAB_8004ec50;
         }
         break;

      case 96:
         if (gState.msgBoxFinished) {
            CloseMsgBox(0);
            gTileStateGridPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].action = TA_NONE;
            unit->name = UNIT_NULL;
            gSignal3 = 1;
            gSignal4 = 1;
            evt->state++;
         }
         break;

      case 98:
         if (gState.msgBoxFinished) {
            PerformAudioCommand(0x5e8);
            CloseMsgBox(0);
            CloseMsgBox(1);
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_STRETCH_WARP_SPRITE;
            evt1->d.evtf062.x = sprite->d.sprite.x1;
            evt1->d.evtf062.z = sprite->d.sprite.z1;
            EVT.timer = 20;
            evt->state2 = 99;
         }
         break;

      case 99:
         if (--EVT.timer == 0) {
            gTileStateGridPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].action = TA_NONE;
            SPR_MAP_UNIT(sprite).raw = 0;
         //?
         LAB_8004ec50:
            gSignal3 = 1;
            gSignal4 = 1;
            sprite->functionIndex = EVTF_NULL;
            evt->functionIndex = EVTF_NULL;
            unit->idx = 0;
         }
         break;
      } // switch (evt->state2) (via state:20)

      break;

   case 21:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_PUSHED_BOULDER;
         // TODO replace w/ 020
         evt1->d.evtf017.sprite = sprite;
         evt->state2++;
         break;

      case 1:
         if (gSignal5 == 1) {
            EVT.animIdx = ANIM_HIGH_STEP_B;
            if (unit->class == CLASS_AIRMAN) {
               EVT.timer = 11;
            } else {
               EVT.timer = 4;
            }
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x5e9);
            gSignal3 = 1;
            gSignal5 = 2;
            evt->state2++;
         }
         break;

      case 3:
         if (sprite->d.sprite.animFinished) {
            gSignal4 = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:21)

      break;

   case 22:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x5e4);
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_STRUCK_B;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_UNIT_STRUCK;
         // TODO replace
         evt1->d.sprite.x1 = sprite->d.sprite.x1;
         evt1->d.sprite.z1 = sprite->d.sprite.z1;
         unit->hpFrac /= 2;
         CalculateUnitStats(unit);
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 23:
   case 24:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = SPR_TILE_STATE(sprite).cachedByte;
         evt->state2++;
         break;

      case 1:
         if (sprite->d.sprite.animFinished) {
            gSignal3 = 1;
            evt->state2++;
         }
         break;

      case 2:
         if (SPR_TILE_STATE(sprite).cachedByte == 0 || evt->state == 24) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 25:

      switch (evt->state2) {
      case 0:
         SPR_TILE_STATE(sprite).action = TA_NONE;
         EVT.animIdx = ANIM_BLOCKING_END_B;
         evt->state2++;
      // fallthrough
      case 1:
         if (!gState.shielding) {
            evt->state = 1;
            evt->state2 = 0;
         }
         break;
      }

      break;

   } // switch (evt->state)

   if (unit->class == CLASS_AIRMAN) {
      sprite->d.sprite.y1 -= (120 + (rcos(sprite->d.sprite.coords[0].x & 0xfff) * 50 >> 12));
      UpdateAirmanUnitSpriteMovement(sprite, evt);
   } else {
      UpdateUnitSpriteMovement(sprite, evt);
   }
   UpdateUnitSpriteOrientation(sprite);
   if (origAnimIdx != EVT.animIdx) {
      sprite->d.sprite.animInitialized = 0;
   }
   animSet = EVT.animSet;
   sprite->d.sprite.animData = animSet[EVT.animIdx + sprite->d.sprite.facingFront];
   UpdateUnitSpriteAnimation(sprite);

   if (unit->class != CLASS_AIRMAN) {
      if (EVT.animIdx != ANIM_IDLE_B) {
         if (gOverheadMapState != 0) {
            RenderOverheadMapUnitMarker(gGraphicsPtr->ot, sprite, 0, unit->team);
         } else {
            RenderUnitSprite(gGraphicsPtr->ot, sprite, 0);
            RenderUnitHelpers(gGraphicsPtr->ot, sprite, unit->team, unit->done, unit->poisoned,
                              unit->paralyzed, unit->hideMarker);
         }
      } else {
         if (gOverheadMapState != 0) {
            RenderOverheadMapUnitMarker(gGraphicsPtr->ot, sprite, 1, unit->team);
         } else {
            RenderUnitSprite(gGraphicsPtr->ot, sprite, 1);
            RenderUnitHelpers(gGraphicsPtr->ot, sprite, unit->team, unit->done, unit->poisoned,
                              unit->paralyzed, unit->hideMarker);
         }
      }
   } else {
      qswap = gSpriteBoxQuads[0];
      gSpriteBoxQuads[0] = gSpriteBoxQuads[8];
      gSpriteBoxQuads[8] = qswap;

      qswap = gSpriteBoxQuads[1];
      gSpriteBoxQuads[1] = gSpriteBoxQuads[9];
      gSpriteBoxQuads[9] = qswap;

      qswap = gSpriteBoxQuads[2];
      gSpriteBoxQuads[2] = gSpriteBoxQuads[11];
      gSpriteBoxQuads[11] = qswap;

      gQuad_800fe65c[0].vx =
          -((rcos((sprite->d.sprite.coords[0].x + ANGLE_180_DEGREES) & 0xfff) * 5 >> 12) + 15);
      gQuad_800fe65c[1].vx = -gQuad_800fe65c[0].vx;
      gQuad_800fe65c[2].vx = gQuad_800fe65c[0].vx;
      gQuad_800fe65c[3].vx = -gQuad_800fe65c[0].vx;

      gQuad_800fe65c[0].vy =
          -((rcos((sprite->d.sprite.coords[0].x + ANGLE_180_DEGREES) & 0xfff) >> 12) + 4);
      gQuad_800fe65c[1].vy = gQuad_800fe65c[0].vy;
      gQuad_800fe65c[2].vy = -gQuad_800fe65c[0].vy;
      gQuad_800fe65c[3].vy = -gQuad_800fe65c[0].vy;

      // Shadow (for flying unit)
      evt1 = gTempGfxEvt;
      evt1->d.sprite.hidden = 0;
      evt1->d.sprite.semiTrans = 0;
      evt1->d.sprite.x1 = sprite->d.sprite.x1;
      evt1->d.sprite.z1 = sprite->d.sprite.z1;
      evt1->d.sprite.gfxIdx = sprite->d.sprite.gfxIdx;
      evt1->d.sprite.stripIdx = sprite->d.sprite.stripIdx;
      evt1->d.sprite.facingLeft = sprite->d.sprite.facingLeft;
      evt1->d.sprite.clut = 7;

      if (gOverheadMapState == 0) {
         RenderUnitSprite(gGraphicsPtr->ot, evt1, 1);
      }

      qswap = gSpriteBoxQuads[0];
      gSpriteBoxQuads[0] = gSpriteBoxQuads[8];
      gSpriteBoxQuads[8] = qswap;

      qswap = gSpriteBoxQuads[1];
      gSpriteBoxQuads[1] = gSpriteBoxQuads[9];
      gSpriteBoxQuads[9] = qswap;

      qswap = gSpriteBoxQuads[2];
      gSpriteBoxQuads[2] = gSpriteBoxQuads[11];
      gSpriteBoxQuads[11] = qswap;

      if (EVT.animIdx == ANIM_IDLE_B) {
         SetElevationFromTerrain(sprite);
      }
      sprite->d.sprite.coords[0].x += 150;
      sprite->d.sprite.y1 += (120 + (rcos(sprite->d.sprite.coords[0].x & 0xfff) * 50 >> 12));
      if (gOverheadMapState != 0) {
         RenderOverheadMapUnitMarker(gGraphicsPtr->ot, sprite, 0, unit->team);
      } else {
         RenderUnitSprite(gGraphicsPtr->ot, sprite, 0);
         RenderUnitHelpers(gGraphicsPtr->ot, sprite, unit->team, unit->done, unit->poisoned,
                           unit->paralyzed, unit->hideMarker);
      }
   }
}

void UpdateUnitSpriteOrientation(EvtData *sprite) {
   s32 dir;

   do {
      dir = ((gCameraRotation.vy + sprite->d.sprite.direction + sprite->d.sprite.coords[0].z) &
             0xfff) >>
            10;
   } while (0);

   sprite->d.sprite.facingLeft = !(dir == 0 || dir == 1);
   sprite->d.sprite.facingFront = (dir == 1 || dir == 2);
}

void UpdateUnitSpriteMovement(EvtData *sprite, EvtData *battler) {
   switch (sprite->state) {
   case 0:
      sprite->d.sprite.finishedMoving = 0;
      // Do nothing if already at destination
      if (sprite->d.sprite.x1 == sprite->d.sprite.x3 &&
          sprite->d.sprite.z1 == sprite->d.sprite.z3) {
         return;
      }
      // Face destination
      if (sprite->d.sprite.x1 < sprite->d.sprite.x3) {
         sprite->d.sprite.direction = ANGLE_WEST;
      }
      if (sprite->d.sprite.x1 > sprite->d.sprite.x3) {
         sprite->d.sprite.direction = ANGLE_EAST;
      }
      if (sprite->d.sprite.z1 < sprite->d.sprite.z3) {
         sprite->d.sprite.direction = ANGLE_SOUTH;
      }
      if (sprite->d.sprite.z1 > sprite->d.sprite.z3) {
         sprite->d.sprite.direction = ANGLE_NORTH;
      }
      sprite->state++;
      break;

   case 1:
      if (SmoothStepTo(sprite, sprite->d.sprite.z3, sprite->d.sprite.x3, 0) == 0) {
         // Terrain is not level, so initiate a jump
         battler->d.evtf014.animIdx = ANIM_JUMPING_B;
         sprite->state += 2;
      } else {
         battler->d.evtf014.animIdx = ANIM_WALKING_B;
         sprite->state++;
      }
      break;

   case 2:
      if (SmoothStepTo(sprite, sprite->d.sprite.z3, sprite->d.sprite.x3, 1) != 0) {
         sprite->d.sprite.finishedMoving = 1;
         sprite->state = 0;
      }
      break;

   case 3:
      if (!sprite->d.sprite.animFinished) {
         break;
      }
      sprite->state++;

   // fallthrough
   case 4:
      if (SmoothStepTo(sprite, sprite->d.sprite.z3, sprite->d.sprite.x3, 1) != 0) {
         // Finish landing jump with a crouch
         battler->d.evtf014.animIdx = ANIM_CROUCHING_B;
         sprite->state++;
      }
      break;

   case 5:
      if (sprite->d.sprite.animFinished) {
         sprite->d.sprite.finishedMoving = 1;
         sprite->state = 0;
      }
      break;
   }
}

void UpdateAirmanUnitSpriteMovement(EvtData *sprite, EvtData *battler) {
   switch (sprite->state) {
   case 0:
      sprite->d.sprite.finishedMoving = 0;
      sprite->state++;

   // fallthrough
   case 1:
      if (sprite->d.sprite.x1 == sprite->d.sprite.x3 &&
          sprite->d.sprite.z1 == sprite->d.sprite.z3) {
         return;
      }
      battler->d.evtf014.animIdx = ANIM_WALKING_B;
      sprite->d.sprite.y2 = 32;
      if (sprite->d.sprite.x1 < sprite->d.sprite.x3) {
         sprite->d.sprite.direction = ANGLE_WEST;
      }
      if (sprite->d.sprite.x1 > sprite->d.sprite.x3) {
         sprite->d.sprite.direction = ANGLE_EAST;
      }
      if (sprite->d.sprite.z1 < sprite->d.sprite.z3) {
         sprite->d.sprite.direction = ANGLE_SOUTH;
      }
      if (sprite->d.sprite.z1 > sprite->d.sprite.z3) {
         sprite->d.sprite.direction = ANGLE_NORTH;
      }
      if (sprite->d.sprite.y1 < sprite->d.sprite.y3) {
         sprite->state++;
      } else {
         if ((gTerrainPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].elevation * 128 <
              sprite->d.sprite.y1) &&
             (gPathBackToUnit[battler->d.evtf014.pathIdx - 1] == PATH_STEP_INVALID)) {
            sprite->state += (sprite->d.sprite.direction >> 9) + 3;
         } else {
            sprite->state += (sprite->d.sprite.direction >> 9) + 2;
         }
      }
      break;

   case 2:
      sprite->d.sprite.y1 += sprite->d.sprite.y2;
      sprite->d.sprite.y2 += 8;
      if (sprite->d.sprite.y1 >= sprite->d.sprite.y3) {
         sprite->d.sprite.y1 = sprite->d.sprite.y3;
         sprite->state += (sprite->d.sprite.direction >> 9) + 1;
         if (gPathBackToUnit[battler->d.evtf014.pathIdx - 1] == PATH_STEP_INVALID) {
            sprite->state++;
         }
      }
      break;

   case 3:
   case 4:
      sprite->d.sprite.z2 += 4;
      sprite->d.sprite.z1 += sprite->d.sprite.z2;
      if (sprite->d.sprite.z1 >= sprite->d.sprite.z3) {
         sprite->d.sprite.z1 = sprite->d.sprite.z3;
         if (sprite->state == 3) {
            sprite->d.sprite.finishedMoving = 1;
            sprite->state = 1;
         } else {
            sprite->state = 11;
            goto HandleState11;
         }
      }
      break;

   case 5:
   case 6:
      sprite->d.sprite.x2 += 4;
      sprite->d.sprite.x1 += sprite->d.sprite.x2;
      if (sprite->d.sprite.x1 >= sprite->d.sprite.x3) {
         sprite->d.sprite.x1 = sprite->d.sprite.x3;
         if (sprite->state == 5) {
            sprite->d.sprite.finishedMoving = 1;
            sprite->state = 1;
         } else {
            sprite->state = 11;
            goto HandleState11;
         }
      }
      break;

   case 7:
   case 8:
      sprite->d.sprite.z2 += 4;
      sprite->d.sprite.z1 -= sprite->d.sprite.z2;
      if (sprite->d.sprite.z1 <= sprite->d.sprite.z3) {
         sprite->d.sprite.z1 = sprite->d.sprite.z3;
         if (sprite->state == 7) {
            sprite->d.sprite.finishedMoving = 1;
            sprite->state = 1;
         } else {
            sprite->state = 11;
            goto HandleState11;
         }
      }
      break;

   case 9:
   case 10:
      sprite->d.sprite.x2 += 4;
      sprite->d.sprite.x1 -= sprite->d.sprite.x2;
      if (sprite->d.sprite.x1 <= sprite->d.sprite.x3) {
         sprite->d.sprite.x1 = sprite->d.sprite.x3;
         if (sprite->state == 9) {
            sprite->d.sprite.finishedMoving = 1;
            sprite->state = 1;
         } else {
            sprite->state = 11;
            goto HandleState11;
         }
      }
      break;

   case 11:
   HandleState11:
      sprite->d.sprite.y3 =
          gTerrainPtr[HI(sprite->d.sprite.z3)][HI(sprite->d.sprite.x3)].elevation * 128;
      sprite->state++;
      break;

   case 12:
      sprite->d.sprite.y2 += 8;
      sprite->d.sprite.y1 -= sprite->d.sprite.y2;
      if (sprite->d.sprite.y1 <= sprite->d.sprite.y3) {
         sprite->d.sprite.y1 = sprite->d.sprite.y3;
         sprite->d.sprite.finishedMoving = 1;
         sprite->state = 1;
      }
      break;
   }
}

void Evtf412_EventCamera(EvtData *evt) {
   s32 diff;
   s32 oneEighth;
   EvtData *focus;

   if (!gState.inEvent) {
      return;
   }

   if (gState.debug && (gPadStateNewPresses & PAD_START)) {
      gState.primary = STATE_SET_SCENE_STATE;
      gState.secondary = 0;
      gState.state3 = 0;
      gState.state4 = 0;
   }

   if (evt->state == 0) {
      gState.eventCameraRot = gCameraRotation.vy;
      evt->state++;
   }

   diff = gState.eventCameraRot - gCameraRotation.vy;
   oneEighth = diff >> 3; // a0
   if (abs(oneEighth) > 2) {
      gCameraRotation.vy += oneEighth;
   } else {
      if (abs(diff) > 2) {
         gCameraRotation.vy += (oneEighth < 0) ? -2 : +2;
      } else {
         gCameraRotation.vy += diff;
      }
   }

   focus = gState.evtFocus;
   if (focus != NULL) {
      gCameraPos.vx += (-(focus->d.sprite.x1 >> 3) - gCameraPos.vx) >> 3;
      gCameraPos.vz += (-(focus->d.sprite.z1 >> 3) - gCameraPos.vz) >> 3;
      gCameraPos.vy +=
          (((focus->d.sprite.y1 + 0x100 + gState.eventCameraHeight) >> 3) - gCameraPos.vy) >> 3;
   } else {
      // x
      diff = -(gState.eventCameraPan.x >> 3) - gCameraPos.vx;
      oneEighth = diff >> 3;
      if (abs(oneEighth) > 2) {
         gCameraPos.vx += oneEighth;
      } else {
         if (abs(diff) > 2) {
            gCameraPos.vx += (oneEighth < 0) ? -2 : +2;
         } else {
            gCameraPos.vx += diff;
         }
      }
      // z
      diff = -(gState.eventCameraPan.z >> 3) - gCameraPos.vz;
      oneEighth = diff >> 3;
      if (abs(oneEighth) > 2) {
         gCameraPos.vz += oneEighth;
      } else {
         if (abs(diff) > 2) {
            gCameraPos.vz += (oneEighth < 0) ? -2 : +2;
         } else {
            gCameraPos.vz += diff;
         }
      }
      // y
      diff = ((gState.eventCameraPan.y + 0x100 + gState.eventCameraHeight) >> 3) - gCameraPos.vy;
      oneEighth = diff >> 3;
      if (abs(oneEighth) > 2) {
         gCameraPos.vy += oneEighth;
      } else {
         if (abs(diff) > 2) {
            gCameraPos.vy += (oneEighth < 0) ? -2 : +2;
         } else {
            gCameraPos.vy += diff;
         }
      }
   }
}

void func_8004FDB0(EvtData *sprite, EvtData *entity) {
   // TBD: Is this y coord used as state anywhere else?
   switch (HI(sprite->d.sprite.coords[0].y)) {
   case 0:
      if (sprite->d.sprite.x1 == sprite->d.sprite.x3 &&
          sprite->d.sprite.z1 == sprite->d.sprite.z3) {
         return;
      }
      sprite->d.sprite.finishedMoving = 0;
      if (sprite->d.sprite.x1 < sprite->d.sprite.x3) {
         HI(sprite->d.sprite.coords[0].y) += 1;
      }
      if (sprite->d.sprite.x1 > sprite->d.sprite.x3) {
         HI(sprite->d.sprite.coords[0].y) += 2;
      }
      if (sprite->d.sprite.z1 < sprite->d.sprite.z3) {
         HI(sprite->d.sprite.coords[0].y) += 3;
      }
      if (sprite->d.sprite.z1 > sprite->d.sprite.z3) {
         HI(sprite->d.sprite.coords[0].y) += 4;
      }
      if (entity->d.evtf409.maintainDirection == 0) {
         if (sprite->d.sprite.x1 < sprite->d.sprite.x3) {
            sprite->d.sprite.direction = ANGLE_WEST;
         }
         if (sprite->d.sprite.x1 > sprite->d.sprite.x3) {
            sprite->d.sprite.direction = ANGLE_EAST;
         }
         if (sprite->d.sprite.z1 < sprite->d.sprite.z3) {
            sprite->d.sprite.direction = ANGLE_SOUTH;
         }
         if (sprite->d.sprite.z1 > sprite->d.sprite.z3) {
            sprite->d.sprite.direction = ANGLE_NORTH;
         }
      }
      break;

   case 1:
      sprite->d.sprite.z1 = sprite->d.sprite.z3;
      sprite->d.sprite.x1 += sprite->d.sprite.x2;
      if (sprite->d.sprite.x1 >= sprite->d.sprite.x3) {
         sprite->d.sprite.x1 = sprite->d.sprite.x3;
         sprite->d.sprite.finishedMoving = 1;
         HI(sprite->d.sprite.coords[0].y) = 0;
      }
      break;

   case 2:
      sprite->d.sprite.z1 = sprite->d.sprite.z3;
      sprite->d.sprite.x1 -= sprite->d.sprite.x2;
      if (sprite->d.sprite.x1 <= sprite->d.sprite.x3) {
         sprite->d.sprite.x1 = sprite->d.sprite.x3;
         sprite->d.sprite.finishedMoving = 1;
         HI(sprite->d.sprite.coords[0].y) = 0;
      }
      break;

   case 3:
      sprite->d.sprite.x1 = sprite->d.sprite.x3;
      sprite->d.sprite.z1 += sprite->d.sprite.z2;
      if (sprite->d.sprite.z1 >= sprite->d.sprite.z3) {
         sprite->d.sprite.z1 = sprite->d.sprite.z3;
         sprite->d.sprite.finishedMoving = 1;
         HI(sprite->d.sprite.coords[0].y) = 0;
      }
      break;

   case 4:
      sprite->d.sprite.x1 = sprite->d.sprite.x3;
      sprite->d.sprite.z1 -= sprite->d.sprite.z2;
      if (sprite->d.sprite.z1 <= sprite->d.sprite.z3) {
         sprite->d.sprite.z1 = sprite->d.sprite.z3;
         sprite->d.sprite.finishedMoving = 1;
         HI(sprite->d.sprite.coords[0].y) = 0;
      }
      break;
   }
}

void SetupEventEntity_SingleSet(u8 **baseAnimSet, s16 *p, u8 stripIdxA, u8 stripIdxB) {
   EvtData *entity;

   entity = Evt_GetUnused();
   entity->functionIndex = EVTF_EVENT_ENTITY;
   entity->d.evtf409.baseAnimSet = baseAnimSet;
   entity->d.evtf409.pNextCommand = p;
   // Adding 2 since the area where strips 0 and 1 would be is used for other purposes (font, etc)
   entity->d.evtf409.stripIdxA = stripIdxA + 2;
   entity->d.evtf409.stripIdxB = stripIdxB + 2;
}

void SetupEventEntity(u8 **baseAnimSet, u8 **altAnimSet, s16 *p, u8 stripIdxA, u8 stripIdxB) {
   EvtData *entity;

   entity = Evt_GetUnused();
   entity->functionIndex = EVTF_EVENT_ENTITY;
   entity->d.evtf409.baseAnimSet = baseAnimSet;
   entity->d.evtf409.altAnimSet = altAnimSet;
   entity->d.evtf409.pNextCommand = p;
   entity->d.evtf409.stripIdxA = stripIdxA + 2;
   entity->d.evtf409.stripIdxB = stripIdxB + 2;
}

void ReserveSprite(u8 srcIdxWithinSheet, u8 dstStripIdx, u8 dstSubIdx) {
   DVECTOR spriteOffsets[24] = {{0, 0},    {12, 0},   {24, 0},   {36, 0},   {0, 48},   {12, 48},
                                {24, 48},  {36, 48},  {0, 96},   {12, 96},  {24, 96},  {36, 96},
                                {0, 144},  {12, 144}, {24, 144}, {36, 144}, {0, 192},  {12, 192},
                                {24, 192}, {36, 192}, {0, 256},  {12, 256}, {24, 256}, {36, 256}};
   s32 spriteX;
   s32 stripX;
   RECT srcRect;
   s32 y;

   spriteX = dstSubIdx * (48 >> 2);
   if (dstSubIdx > 4) {
      // Jump the gap (icons)
      spriteX += 4;
   }

   srcIdxWithinSheet -= 1;
   srcRect.w = 48 >> 2;
   srcRect.h = 48;
   srcRect.x = spriteOffsets[srcIdxWithinSheet].vx + 704;
   srcRect.y = spriteOffsets[srcIdxWithinSheet].vy;

   if (dstStripIdx > 4) {
      stripX = 768;
   } else {
      stripX = 640;
   }
   if (dstStripIdx > 14) {
      stripX += 128;
   }

   y = dstStripIdx % 5 * 48;
   if (dstStripIdx % 10 < 5) {
      y += 256;
   }

   MoveImage(&srcRect, stripX + spriteX, y);
   DrawSync(0);
}

#undef EVTF
#define EVTF 409
void Evtf409_EventEntity(EvtData *evt) {
   EvtData *sprite;
   EvtData *evt1;
   EvtData *evt2;
   s16 argument;
   s16 *pNextCommand;
   s16 *pCurrentCommand;
   s32 tmp;
   s32 i, j;
   u8 **animSet;

   sprite = EVT.sprite;

   switch (EVT.runState) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.coords[0].z = rand() % 256 - 128;
      sprite->d.sprite.stripIdx = EVT.stripIdxA;
      sprite->d.sprite.x1 = EVT.x;
      sprite->d.sprite.z1 = EVT.z;
      sprite->d.sprite.x3 = EVT.x;
      sprite->d.sprite.z3 = EVT.z;
      EVT.sprite = sprite;
      EVT.runState++;

   // fallthrough
   HandleRunState1:
   case 1:
      pNextCommand = EVT.pNextCommand;
      evt->mem = *pNextCommand;
      EVT.pNextCommand = pNextCommand + 2;
      EVT.runState++;
      EVT.commandState = 0;

   // fallthrough
   case 2:
      pNextCommand = EVT.pNextCommand;
      argument = pNextCommand[-1];

      switch (evt->mem) {
      case 1:
         // Yield until given location
         if (gState.eventResumeLocation >= argument) {
            EVT.runState = 1;
            goto HandleRunState1;
         }
         // Continue waiting for resume
         break;

      case 2:
         // Play base-set animation
         if ((EVT.animIdx != argument * 2) || EVT.usingAltAnimSet == 1) {
            // Need to switch
            EVT.animIdx = argument * 2;
            EVT.usingAltAnimSet = 0;
            sprite->d.sprite.animInitialized = 0;
            sprite->d.sprite.animFinished = 0;
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 3:
         sprite->d.sprite.x3 = argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 4:
         sprite->d.sprite.z3 = argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 5:
         sprite->d.sprite.x3 = argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 6:
         sprite->d.sprite.z3 = argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 7:
         sprite->d.sprite.x2 = argument;
         sprite->d.sprite.z2 = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 8:
         sprite->d.sprite.direction = argument << 10;
         EVT.runState = 1;
         goto HandleRunState1;

      case 9:
         if (sprite->d.sprite.finishedMoving) {
            EVT.runState = 1;
            goto HandleRunState1;
         }
         break;

      case 0xa:
         if (sprite->d.sprite.animFinished) {
            EVT.runState = 1;
            goto HandleRunState1;
         }
         break;

      case 0xb:

         switch (EVT.commandState) {
         case 0:
            EVT.timer = argument;
            EVT.commandState++;
            break;
         case 1:
            if (--EVT.timer == 0) {
               EVT.runState = 1;
               goto HandleRunState1;
            }
            break;
         }

         break;

      case 0xc:
         //? Branch (relative to current command)
         pCurrentCommand = EVT.pNextCommand - 2;
         EVT.pNextCommand = pCurrentCommand + argument * 2;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0xd:
         gState.evtFocus = sprite;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0xe:
         gState.evtFocus = NULL;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0xf:

         switch (EVT.commandState) {
         case 0:
            StartUnitSpritesDecoder(sprite->d.sprite.stripIdx);
            EVT.commandState++;
            break;
         case 1:
            if (!gDecodingSprites) {
               EVT.runState = 1;
               goto HandleRunState1;
            }
            break;
         }

         break;

      case 0x10:
         if (gState.eventResumeLocation >= argument) {
            EVT.pNextCommand += 2;
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x11:
      case 0x12:
         // Resume suspended entities (up to given "location"), effectively used to branch between
         // entity scripts
         if (gState.eventResumeLocation < argument) {
            gState.eventResumeLocation = argument;
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x13:
         sprite->d.sprite.x1 = sprite->d.sprite.x3;
         sprite->d.sprite.z1 = sprite->d.sprite.z3;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x14:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_EVENT_ZOOM;
         evt1->d.evtf410.zoom = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x15:
         sprite->d.sprite.x1 = argument;
         sprite->d.sprite.x3 = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x16:
         sprite->d.sprite.z1 = argument;
         sprite->d.sprite.z3 = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x17:
         sprite->d.sprite.x3 = sprite->d.sprite.x1 + argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x18:
         sprite->d.sprite.z3 = sprite->d.sprite.z1 + argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x19:
         sprite->d.sprite.x3 = sprite->d.sprite.x1 + argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1a:
         sprite->d.sprite.z3 = sprite->d.sprite.z1 + argument;
         sprite->d.sprite.finishedMoving = 0;
         EVT.maintainDirection = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1b:
         sprite->d.sprite.hidden = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1c:
         sprite->d.sprite.hidden = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1d:
         // Spawn an arbitrary evt
         evt1 = Evt_GetUnused();
         evt1->functionIndex = argument;
         // todo replace?
         evt1->d.evtf017.sprite = sprite;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1e:
         ShowMsgBoxForSprite(sprite, argument, 0);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x1f:
         CloseMsgBox(argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x20:
         SetMsgBoxText(1, argument, 0x100);
         gState.msgBoxFinished = 0;
         gState.field_0x31d = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x21:
         SetMsgBoxText(2, argument, 0x100);
         gState.msgBoxFinished = 0;
         gState.field_0x31d = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x22:
         if (gState.msgBoxFinished) {
            gState.msgBoxFinished = 0;
            EVT.runState = 1;
            goto HandleRunState1;
         }
         break;

      case 0x23:
         if (gState.field_0x31d != 0) {
            gState.field_0x31d = 0;
            EVT.runState = 1;
            goto HandleRunState1;
         }
         break;

      case 0x24:
         gState.eventCameraRot = gCameraRotation.vy = (argument << 10) | ANGLE_45_DEGREES;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x25:
         gState.eventCameraRot = tmp = argument << 10;
         gState.eventCameraRot += ANGLE_45_DEGREES;

         i = gState.eventCameraRot - gCameraRotation.vy;
         if (i > ANGLE_180_DEGREES) {
            gState.eventCameraRot -= ANGLE_360_DEGREES;
         }
         if (i < -ANGLE_180_DEGREES) {
            gState.eventCameraRot += ANGLE_360_DEGREES;
         }

         EVT.runState = 1;
         goto HandleRunState1;

      case 0x26:
         gState.eventCameraPan.x = argument;
         gCameraPos.vx = -(argument >> 3);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x27:
         gState.eventCameraPan.y = argument;
         gCameraPos.vy = (argument + gState.eventCameraHeight) >> 3;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x28:
         gState.eventCameraPan.z = argument;
         gCameraPos.vz = -(argument >> 3);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x29:
         gState.eventCameraPan.x = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2a:
         gState.eventCameraPan.y = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2b:
         gState.eventCameraPan.z = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2c:
         evt1 = gState.evtFocus;
         gState.eventCameraPan.x = evt1->d.sprite.x1;
         gState.eventCameraPan.y = evt1->d.sprite.y1;
         gState.eventCameraPan.z = evt1->d.sprite.z1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2d:
         gCameraZoom.vz = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2e:
         EVT.stripIdxA = argument + 2;
         sprite->d.sprite.stripIdx = argument + 2;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x2f:
         if (argument == 0) {
            sprite->d.sprite.stripIdx = EVT.stripIdxA;
         } else {
            sprite->d.sprite.stripIdx = EVT.stripIdxB;
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x30:
         // Play alt-set animation
         if ((EVT.animIdx != argument * 2) || !EVT.usingAltAnimSet) {
            // Need to switch
            EVT.animIdx = argument * 2;
            EVT.usingAltAnimSet = 1;
            sprite->d.sprite.animInitialized = 0;
            sprite->d.sprite.animFinished = 0;
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x31:
      case 0x32:
         SetMsgBoxPortrait(argument, evt->mem == 0x32);
         EVT.runState = 1;
         goto HandleRunState1;

      //?
      case 0x33:
         i = 0;
         goto ReserveSpriteInSubIdxI;
      case 0x34:
         i = 1;
         goto ReserveSpriteInSubIdxI;
      case 0x35:
         i = 2;
         goto ReserveSpriteInSubIdxI;
      case 0x36:
         i = 3;
         goto ReserveSpriteInSubIdxI;
      case 0x37:
         i = 4;
         goto ReserveSpriteInSubIdxI;
      case 0x38:
         i = 5;
         goto ReserveSpriteInSubIdxI;
      case 0x39:
         i = 6;
         goto ReserveSpriteInSubIdxI;
      case 0x3a:
         i = 7;
         goto ReserveSpriteInSubIdxI;
      case 0x3b:
         i = 8;
         goto ReserveSpriteInSubIdxI;
      case 0x3c:
         i = 9;
      ReserveSpriteInSubIdxI: //?: Could use something like Duff's device instead?
         ReserveSprite(argument, sprite->d.sprite.stripIdx, i);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x3d:
         PerformAudioCommand(argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x3e:
         PerformAudioCommand(3);
         LoadSeqSet(argument);
         FinishLoadingSeq();
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x3f:
      case 0x55:

         switch (EVT.commandState) {
         case 0:
            EVT.timer = 35;
            evt2 = Evt_GetUnused();
            evt2->functionIndex = EVTF_BLOOD_SPURT;
            // todo replace
            evt2->d.evtf052.sprite = sprite;
            if (evt->mem == 0x55) {
               EVT.runState = 1;
               goto HandleRunState1;
            } else {
               EVT.commandState++;
            }
            break;

         case 1:
            if (--EVT.timer == 0) {
               evt2 = Evt_GetUnused();
               evt2->functionIndex = EVTF_SLAY_UNIT;
               // todo replace
               evt2->d.evtf052.sprite = sprite;
               EVT.runState = 1;
               goto HandleRunState1;
            }
            break;
         }

         break;

      case 0x40:

         switch (EVT.commandState) {
         case 0:
            EVT.timer = 50;
            EVT.commandState++;

         // fallthrough
         case 1:
            if (--EVT.timer == 0) {
               gState.primary = STATE_SET_SCENE_STATE;
               gState.secondary = 0;
               gState.state3 = 0;
               gState.state4 = 0;
            }
            break;
         }

         break;

      case 0x41:
         gState.eventCameraHeight = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x42:
         ShowMsgBoxForSprite(sprite, argument, 1);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x43:
         FadeOutScreen(2, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x44:
         FadeInScreen(2, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x45:
         FadeOutScreen(1, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x46:
         FadeInScreen(1, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x47:
         EVT.elevationType = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x48:

         switch (EVT.commandState) {
         case 0:
            gWindowChoicesTopMargin = 10;
            gWindowChoiceHeight = 17;
            DrawWindow(0x34, 0, 170, 240, 54, 40, 93, WBS_CROSSED, 2);
            DrawText(12, 181, 25, 2, 0, gState.currentTextPointers[argument]);
            DisplayBasicWindow(0x34);
            gWindowActiveIdx = 0x34;
            EVT.commandState++;
            break;

         case 1:
            if (gWindowChoice.raw == 0x3401) {
               CloseWindow(0x34);
               gState.eventChoice = 0;
               EVT.pNextCommand += 2;
               EVT.commandState++;
            }
            if (gWindowChoice.raw == 0x3402) {
               CloseWindow(0x34);
               gState.eventChoice = 1;
               EVT.commandState++;
            }
            break;

         case 2:
            EVT.runState = 1;
            goto HandleRunState1;
         }

         break;

      case 0x49:
         gWindowChoicesTopMargin = 10;
         DrawWindow(0x34, 0, 170, 240, 36, 40, 97, WBS_CROSSED, 0);
         DrawText(12, 180, 25, 2, 0, gState.currentTextPointers[argument]);
         DisplayBasicWindow(0x34);
         gWindowActiveIdx = 0x34;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x4a:
         if (PressedCircleOrX()) {
            EVT.runState = 1;
            CloseWindow(0x34);
            EVT.runState = 1;
            goto HandleRunState1;
         }
         break;

      case 0x4b:
         gMapMinX = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x4c:
         gMapMinZ = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x4d:
         gMapSizeX = argument;
         gMapMarginX = 0;
         gMapMaxX = gMapMinX + gMapSizeX - 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x4e:
         gMapSizeZ = argument;
         gMapMarginZ = 0;
         gMapMaxZ = gMapMinZ + gMapSizeZ - 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x4f:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_STRETCH_WARP_SPRITE;
         evt2->d.evtf062.sprite = sprite;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x50:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_STRETCH_WARP_SPRITE;
         evt2->d.evtf062.sprite = sprite;
         evt2->mem = 1; // Reversed (warp in)
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x51:
         CloseWindow(0x34);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x52:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_ADJUST_FACE_ELEVATION;
         evt2->state2 = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x53:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_SLIDING_FACE;
         evt2->state2 = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x54:
         Evt_ResetByFunction(argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x56:
         gState.preciseSprites = 1;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x57:
         gState.preciseSprites = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x58:
         DrawWindow(0x43, 0, 100, 296, 64, 12, 161, WBS_ROUNDED, 0);
         DisplayCustomWindow(0x43, 0, 1, 1, 0, 25);
         DisplayCustomWindow(0x44, 0, 1, 1, 0, 25);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x59:
         gCameraPos.vy = (sprite->d.sprite.y1 + 0x100 + gState.eventCameraHeight) >> 3;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5a:
         (gState.evtScreenEffect)->state = 5;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5b:
         (gState.evtScreenEffect)->state = 0;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5c:
         SetScreenEffectOrdering(argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5d:
         (gState.evtScreenEffect)->state2 = argument; // semiTransRate
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5e:
         (gState.evtScreenEffect)->d.evtf369.color.r = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x5f:
         (gState.evtScreenEffect)->d.evtf369.color.g = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x60:
         (gState.evtScreenEffect)->d.evtf369.color.b = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x65:
         (gState.evtScreenEffect)->d.evtf369.rd = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x66:
         (gState.evtScreenEffect)->d.evtf369.gd = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x67:
         (gState.evtScreenEffect)->d.evtf369.bd = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x68:
         (gState.evtScreenEffect)->d.evtf369.rmax = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x69:
         (gState.evtScreenEffect)->d.evtf369.gmax = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6a:
         (gState.evtScreenEffect)->d.evtf369.bmax = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6b:
         gLightColor.r = gLightColor.g = gLightColor.b = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6c:
         gLightColor.r += argument;
         gLightColor.g = gLightColor.b = gLightColor.r;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6d:
         (gState.evtScreenEffect)->d.evtf369.color.r = argument;
         (gState.evtScreenEffect)->d.evtf369.color.g = argument;
         (gState.evtScreenEffect)->d.evtf369.color.b = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6e:
         (gState.evtScreenEffect)->d.evtf369.rd = argument;
         (gState.evtScreenEffect)->d.evtf369.gd = argument;
         (gState.evtScreenEffect)->d.evtf369.bd = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x6f:
         (gState.evtScreenEffect)->d.evtf369.rmax = argument;
         (gState.evtScreenEffect)->d.evtf369.gmax = argument;
         (gState.evtScreenEffect)->d.evtf369.bmax = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x70:
      case 0x71:
         (gState.evtScreenEffect)->d.evtf369.semiTrans = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x72:
         for (i = 0; i < 30; i++) {
            for (j = 0; j < 65; j++) {
               gPathGrid0[i][j] = PATH_STEP_UNSET;
            }
         }
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x73:
         gCameraRotation.vx = argument;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x74:
         gScreenFade = Evt_GetUnused();
         gScreenFade->functionIndex = EVTF_FADE;
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x75:
         // Set up fade
         func_800A93C8(1, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x76:
         // Set up fade
         func_800A93E8(1, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x77:
         // Set up fade
         func_800A93C8(2, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x78:
         // Set up fade
         func_800A93E8(2, argument);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x79:
         Evt_ResetByFunction(EVTF_FADE);
         EVT.runState = 1;
         goto HandleRunState1;

      case 0x7a:
         SetMsgBoxText2(1, argument, 0x100);
         gState.msgBoxFinished = 0;
         gState.field_0x31d = 0;
         EVT.runState = 1;
         goto HandleRunState1;
      } // switch (command) (via runState:2)

      break;
   } // switch (runState)

   // UpdateSprite_80051464:
   func_8004FDB0(sprite, evt);
   UpdateUnitSpriteOrientation(sprite);
   if (!EVT.usingAltAnimSet) {
      animSet = EVT.baseAnimSet;
   } else {
      animSet = EVT.altAnimSet;
   }
   sprite->d.sprite.animData = animSet[EVT.animIdx + sprite->d.sprite.facingFront];
   UpdateUnitSpriteAnimation(sprite);
   RenderUnitSprite(gGraphicsPtr->ot, sprite, EVT.elevationType + 1);
}

void Evtf590_BattleTurnTicker(EvtData *evt) {
   switch (evt->state) {
   case 0:
      gState.field_0x96 = 1;

      switch (gState.mapNum) {
      case 13:
         // Bridge crumbling in sections
         gState.mapState.n.field_0x0++;
         evt->state++;
         break;
      case 33:
         // Kira being gradually lowered into lava pit
         gState.mapState.n.field_0x0++;
         evt->state++;
         break;
      default:
         gState.field_0x96 = 0;
         evt->state++;
         break;
      }

      break;

   case 1:
      if (gState.field_0x96 == 0) {
         gState.field_0x98 = 0;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}
