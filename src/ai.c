#include "common.h"
#include "evt.h"
#include "field.h"
#include "units.h"
#include "battle.h"
#include "state.h"

#include "PsyQ/kernel.h"

void Evtf570_AI_TBD(EvtData *);
s32 func_800560F8(UnitStatus *);
s32 IsLagging(void);
void Evtf400_AI_TBD(EvtData *);
void func_80056760(UnitStatus *);
void func_800569A0(UnitStatus *);
s32 func_80056C30(UnitStatus *, UnitStatus *, u8, u8, u8, u8);
s32 func_80056F94(UnitStatus *, u8, u8, u8, u8);
void Evtf401_AI_TBD(EvtData *);
//...

extern u8 D_80123410, D_80123480, D_80123484;
extern s16 D_8012F63C[40];
extern s16 D_8017DF50[27][64];
extern u8 D_80123484;

#undef EVTF
#define EVTF 570
void Evtf570_AI_TBD(EvtData *evt) {
   EvtData *newEvt;
   UnitStatus *unit;
   u8 spellEffectA, spellEffectB;

   unit = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];

   switch (evt->state) {
   case 0:
      D_80123480 = 0;
      if (gState.mapNum == 21) {
         evt->state = 5;
         evt->state2 = 0;
         break;
      }
      if (unit->name == UNIT_LEENA) {
         evt->state = 6;
         evt->state2 = 0;
         break;
      }
      if (gState.mapNum == 26) {
         evt->state = 6;
         evt->state2 = 0;
         break;
      }

      spellEffectA = 0;
      spellEffectB = 0;

      if (unit->spells[0] == SPELL_NULL) {
         evt->state = 1;
         evt->state2 = 0;
      } else {
         spellEffectA = gSpellsEx[unit->spells[0]][SPELL_EX_EFFECT] + 1;
         if (unit->mp < gSpells[unit->spells[0]].mpCost) {
            spellEffectA = 0;
         }
         if (unit->spells[1] != SPELL_NULL) {
            spellEffectB = gSpellsEx[unit->spells[0]][SPELL_EX_EFFECT] + 1; //? Why spells[0]?
            if (unit->mp < gSpells[unit->spells[1]].mpCost) {
               spellEffectB = 0;
            }
         }
         if (spellEffectA == 0) {
            spellEffectA = spellEffectB;
            spellEffectB = 0;
            gCurrentSpell = unit->spells[1];
         } else {
            gCurrentSpell = unit->spells[0];
         }
         if (spellEffectB == 0) {
            if (spellEffectA == 0) {
               evt->state = 1;
               evt->state2 = 0;
            } else if (spellEffectA != SPELL_EFFECT_DAMAGE + 1) {
               if (gSpells[gCurrentSpell].range == 0 && gSpells[gCurrentSpell].fieldSize == 0) {
                  if (unit->hpFrac >= 5000) {
                     evt->state = 1;
                     evt->state2 = 0;
                  } else {
                     evt->state = 3;
                     evt->state2 = 0;
                  }
               } else {
                  if (unit->class != CLASS_PRIEST && unit->hpFrac >= 5000) {
                     evt->state = 1;
                     evt->state2 = 0;
                  } else {
                     evt->state = 4;
                     evt->state2 = 0;
                  }
               }
            } else {
               evt->state = 2;
               evt->state2 = 0;
            }
         } else if (spellEffectA == SPELL_EFFECT_DAMAGE + 1 &&
                    spellEffectB == SPELL_EFFECT_DAMAGE + 1) {
            if (rand() % 2 == 0) {
               gCurrentSpell = unit->spells[0];
            } else {
               gCurrentSpell = unit->spells[1];
            }
            evt->state = 2;
            evt->state2 = 0;
         } else if (unit->hpFrac < 5000) {
            gCurrentSpell = unit->spells[0];
            if (gSpells[unit->spells[0]].range == 0 && gSpells[unit->spells[0]].fieldSize == 0) {
               evt->state = 3;
               evt->state2 = 0;
            } else {
               evt->state = 4;
               evt->state2 = 0;
            }
         } else {
            gCurrentSpell = unit->spells[1];
            evt->state = 2;
            evt->state2 = 0;
         }
      }

      break;

   case 1:

      switch (evt->state2) {
      case 0:
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_403;
         evt->state2++;

      // fallthrough
      case 1:
         if (D_80123480) {
            evt->state = 99;
         }
         break;
      }

      break;

   case 2:

      switch (evt->state2) {
      case 0:
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_402;
         evt->state2++;

      // fallthrough
      case 1:
         if (D_80123480) {
            evt->state = 99;
         }
         break;
      }

      break;

   case 3:

      switch (evt->state2) {
      case 0:
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_404;
         evt->state2++;

      // fallthrough
      case 1:
         if (D_80123480) {
            D_8012337C = 2;
            gTargetX_80123414 = gX_801233d8;
            gTargetZ_80123418 = gZ_801233dc;
            evt->state = 99;
         }
         break;
      }

      break;

   case 4:

      switch (evt->state2) {
      case 0:
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_402;
         evt->state2++;

      // fallthrough
      case 1:
         if (D_80123480) {
            if (D_80123410) {
               D_80123480 = 0;
               newEvt = Evt_GetUnused();
               // todo replace
               HI(newEvt->d.sprite.x1) = HI(EVT.x);
               HI(newEvt->d.sprite.z1) = HI(EVT.z);
               newEvt->functionIndex = EVTF_AI_TBD_404;
               evt->state2++;
            } else {
               evt->state = 99;
            }
         }
         break;

      case 2:
         if (D_80123480) {
            D_8012337C = 2;
            gTargetX_80123414 = gX_801233d8;
            gTargetZ_80123418 = gZ_801233dc;
            evt->state = 99;
         }
         break;
      }

      break;

   case 5:

      switch (evt->state2) {
      case 0:
         D_80123480 = 0;
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_404;
         evt->state2++;
         break;

      case 1:
         if (D_80123480) {
            evt->state = 99;
         }
         break;
      }

      break;

   case 6:

      switch (evt->state2) {
      case 0:
         D_80123480 = 0;
         newEvt = Evt_GetUnused();
         // todo replace
         HI(newEvt->d.sprite.x1) = HI(EVT.x);
         HI(newEvt->d.sprite.z1) = HI(EVT.z);
         newEvt->functionIndex = EVTF_AI_TBD_589;
         evt->state2++;
         break;

      case 1:
         if (D_80123480) {
            evt->state = 99;
         }
         break;
      }

      break;

   case 99:
      D_80123468 = 1;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

s32 func_800560F8(UnitStatus *unit) {
   s32 ix, iz;
   u8 team = unit->team;
   s32 ct = 0;

   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         if (gMapUnitsPtr[iz][ix].s.unitIdx != 0 && gMapUnitsPtr[iz][ix].s.team == team) {
            ct++;
         }
      }
   }

   return (ct <= unit->field8_0x1d);
}

s32 IsLagging(void) { return (GetRCnt(RCntCNT1) > 450); }

#undef EVTF
#define EVTF 400
void Evtf400_AI_TBD(EvtData *evt) {
   UnitStatus *unit;
   UnitStatus *targetUnit;
   s8 team;
   // s32 i;
   s32 iz, ix;
   s32 iz2, ix2;
   s32 sVar6;

   unit = EVT.unit;
   team = unit->team;

   switch (evt->state) {
   case 0:
      if (IsLagging()) {
         return;
      }

      EVT.resumeZ = gMapMinZ;
      EVT.resumeX = gMapMinX;

      if (gSpells[gCurrentSpell].targeting < SPELL_TARGET_ENEMY_GROUP) {
         evt->state += 2;
      } else {
         ClearGrid(6);

         for (iz = 1; iz < UNIT_CT; iz++) {
            targetUnit = &gUnits[iz];
            if (targetUnit->idx != 0) {

               if (gSpells[gCurrentSpell].targeting == SPELL_TARGET_ENEMY_GROUP) {
                  if (targetUnit->team == team) {
                     continue;
                  }
               } else if (unit == targetUnit || targetUnit->team != team) {
                  continue;
               }

               func_8002ADCC(HI(targetUnit->evtSprite->d.sprite.z1),
                             HI(targetUnit->evtSprite->d.sprite.x1),
                             gSpells[gCurrentSpell].fieldSize, 6);
            }
         }
         evt->state++;
         break;
      }
      break;

   case 1:
      iz = EVT.resumeZ;
      ix = EVT.resumeX;

      while (iz <= gMapMaxZ) {
         while (ix <= gMapMaxX) {
            sVar6 = 0;
            if (gPathGrid6_Ptr[iz][ix] != PATH_STEP_UNSET) {
               PopulateCastingGrid(iz, ix, gSpells[gCurrentSpell].fieldSize, 5);

               for (iz2 = gMapMinZ; iz2 <= gMapMaxZ; iz2++) {
                  for (ix2 = gMapMinX; ix2 <= gMapMaxX; ix2++) {
                     if (gPathGrid5_Ptr[iz2][ix2] != PATH_STEP_UNSET &&
                         gMapUnitsPtr[iz2][ix2].s.unitIdx != 0) {
                        sVar6 += D_8012F63C[gUnits[gMapUnitsPtr[iz2][ix2].s.unitIdx].idx];
                     }
                  }
               }

               D_8017DF50[iz][ix] = sVar6;
            } else {
               D_8017DF50[iz][ix] = 0;
            }
            //@b9c
            ix++;
            if (IsLagging()) {
               EVT.resumeX = ix;
               EVT.resumeZ = iz;
               return;
            }
         }
         iz++;
         ix = gMapMinX;
      }

      D_80123484++;
      evt->functionIndex = EVTF_NULL;
      break;

   case 2:
      iz = EVT.resumeZ;
      ix = EVT.resumeX;

      while (iz <= gMapMaxZ) {
         while (ix <= gMapMaxX) {
            sVar6 = 0;
            if (gMapUnitsPtr[iz][ix].s.unitIdx != 0) {
               sVar6 = D_8012F63C[gUnits[gMapUnitsPtr[iz][ix].s.unitIdx].idx];
            }
            D_8017DF50[iz][ix] = sVar6;
            ix++;
            if (GetRCnt(RCntCNT1) > 450) {
               EVT.resumeX = ix;
               EVT.resumeZ = iz;
               return;
            }
         }
         iz++;
         ix = gMapMinX;
      }

      D_80123484++;
      evt->functionIndex = EVTF_NULL;
      break;
   }
}

void func_80056760(UnitStatus *unit) {
   EvtData *sprite;
   UnitStatus *otherUnit;
   s32 i;
   s16 xDist, zDist;
   s16 largest;
   s16 a = 0;
   s16 b = 0;
   s16 c = 0;
   s16 d = 0;

   for (i = 1; i < UNIT_CT; i++) {
      otherUnit = &gUnits[i];
      sprite = otherUnit->evtSprite;

      if (otherUnit->idx != 0 && unit->team != otherUnit->team) {
         xDist = (gX_801233d8 - HI(sprite->d.sprite.x1));
         if (xDist < 0) {
            xDist = -xDist;
         }
         zDist = gZ_801233dc - HI(sprite->d.sprite.z1);
         if (zDist < 0) {
            zDist = -zDist;
         }
         if (xDist >= zDist) {
            if (HI(sprite->d.sprite.x1) < gX_801233d8) {
               d += ((0x40 - xDist) * 2) + (0x40 - zDist);
            } else {
               b += ((0x40 - xDist) * 2) + (0x40 - zDist);
            }
         }
         if (zDist >= xDist) {
            if (HI(sprite->d.sprite.z1) < gZ_801233dc) {
               c += ((0x40 - zDist) * 2) - (xDist - 0x40);
            } else {
               a += ((0x40 - zDist) * 2) - (xDist - 0x40);
            }
         }
      }
   }

   if (a == 0 && b == 0 && c == 0 && d == 0) {
      gDir_80123470 = 0xffff;
   } else {
      gDir_80123470 = ANGLE_SOUTH;
      largest = a;
      if (largest < b) {
         gDir_80123470 = ANGLE_WEST;
         largest = b;
      }
      if (largest < c) {
         gDir_80123470 = ANGLE_NORTH;
         largest = c;
      }
      if (largest < d) {
         gDir_80123470 = ANGLE_EAST;
         largest = d;
      }
   }
}

void func_800569A0(UnitStatus *unit) {
   s16 i;
   UnitStatus *otherUnit;
   EvtData *sprite;
   s16 sVar4;

   for (i = 1; i < UNIT_CT; i++) {
      otherUnit = &gUnits[i];
      sprite = otherUnit->evtSprite;
      sVar4 = 0;
      if (otherUnit->idx != 0) {

         switch (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT]) {
         case SPELL_EFFECT_DAMAGE:
            if ((gSpells[gCurrentSpell].targeting == SPELL_TARGET_ENEMY ||
                 gSpells[gCurrentSpell].targeting == SPELL_TARGET_ENEMY_GROUP) &&
                unit->team != otherUnit->team) {
               sVar4 += (unit->level - otherUnit->level) * 10;
               sVar4 += 280;
               sVar4 -= (otherUnit->hpFrac / 125);
               sVar4 -= gAdvantage[unit->advantage][otherUnit->advantage];
               sVar4 -= gTerrainPreference[SPR_TERRAIN(sprite).terrain] / 100;
            }
            break;

         case SPELL_EFFECT_RESTORE_HP:
            if ((gSpells[gCurrentSpell].targeting == SPELL_TARGET_ALLY ||
                 gSpells[gCurrentSpell].targeting == SPELL_TARGET_ALLY_GROUP) &&
                unit->team == otherUnit->team && unit != otherUnit && otherUnit->hpFrac < 7000) {
               sVar4 += 400;
               sVar4 -= (otherUnit->hpFrac / 25);
            }
            break;
         }

         D_8012F63C[i] = sVar4;
      }
   }
}

s32 func_80056C30(UnitStatus *unit1, UnitStatus *unit2, u8 z1, u8 x1, u8 z2, u8 x2) {
   s32 result;

   if (unit2->name == UNIT_LEENA) {
      result = 5000;
   } else {
      result = 10000;
   }

   switch (unit2->direction >> 10) {
   case DIR_SOUTH:
      if (x1 != x2) {
         result += 20;
      } else if (z1 < z2) {
         result += 30;
      }
      break;
   case DIR_WEST:
      if (z1 != z2) {
         result += 20;
      } else if (x1 < x2) {
         result += 30;
      }
      break;
   case DIR_NORTH:
      if (x1 != x2) {
         result += 20;
      } else if (z1 > z2) {
         result += 30;
      }
      break;
   default:
      if (z1 != z2) {
         result += 20;
      } else if (x1 > x2) {
         result += 30;
      }
      break;
   }

   result += D_8012F63C[unit2->idx];
   result -= gPathGrid6_Ptr[z1][x1] * 3;
   result -= (gPathGrid3_Ptr[z1][x1] - 1) / unit1->travelRange * 1000;
   if (result < 1) {
      result = 1;
   }
   result += (gTerrainPtr[z1][x1].elevation - gTerrainPtr[z2][x2].elevation) * 10;
   result += gTerrainPreference[gTerrainPtr[z1][x1].terrain] / 100;

   if (unit1->class == CLASS_ARCHER) {
      if (unit2->class == CLASS_ARCHER) {
         PopulateRangedAttackGrid(z2, x2, unit2->attackRange, 0);
         if (gPathGrid0_Ptr[z1][x1] != PATH_STEP_UNSET) {
            result -= 200;
         }
      } else {
         PopulateMeleeAttackGrid(z2, x2, 0, 1);
         if (gPathGrid0_Ptr[z1][x1] != PATH_STEP_UNSET) {
            result -= 200;
         }
      }
      result += 200;
      result -= (unit2->hpFrac / 50);
      ClearGrid(0);
   }

   return result;
}

s32 func_80056F94(UnitStatus *unit, u8 z1, u8 x1, u8 z2, u8 x2) {
   s32 result = D_8017DF50[z2][x2];

   if (result >= 1) {
      result += 10000;
      result -= (gPathGrid3_Ptr[z1][x1] - 1) / unit->travelRange * 1000;
      result += gTerrainPreference[gTerrainPtr[z1][x1].terrain] / 100;
      result -= gPathGrid6_Ptr[z1][x1] * 3;
   } else {
      result = 0;
   }

   return result;
}

#undef EVTF
#define EVTF 401
void Evtf401_AI_TBD(EvtData *evt) {
   UnitStatus *unit1;
   UnitStatus *unit2;
   EvtData *sprite1;
   EvtData *sprite2;
   s16 i;

   unit1 = &gUnits[EVT.unitIdx];
   sprite1 = unit1->evtSprite;

   switch (evt->state) {
   case 0:
      unit1 = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
      sprite1 = unit1->evtSprite;
      ClearGrid(0);
      ClearGrid(6);

      EVT.team = SPR_MAP_UNIT(sprite1).s.team;
      EVT.unitIdx = SPR_MAP_UNIT(sprite1).s.unitIdx;
      SPR_MAP_UNIT(sprite1).raw = 0;
      EVT.unitIter = 0;
      evt->state++;
      break;

   case 1:
      i = EVT.unitIter;

      while (++i != UNIT_CT) {
         unit2 = &gUnits[i];
         sprite2 = unit2->evtSprite;

         if (unit2->idx != 0 && unit1->team != unit2->team) {
            func_8002CF88(HI(sprite2->d.sprite.z1), HI(sprite2->d.sprite.x1), 0xff, 0, 6);
            ClearGrid(0);
            if (GetRCnt(RCntCNT1) > 450) {
               EVT.unitIter = i;
               return;
            }
         }
      }

      SPR_MAP_UNIT(sprite1).s.team = EVT.team;
      SPR_MAP_UNIT(sprite1).s.unitIdx = EVT.unitIdx;
      ClearGrid(0);
      D_80123484++;
      evt->functionIndex = EVTF_NULL;
      return;
   }
}
