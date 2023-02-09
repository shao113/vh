#include "common.h"
#include "evt.h"
#include "battle.h"
#include "units.h"
#include "state.h"
#include "field.h"
#include "window.h"

void EmbedIntAsSjis(s32 num, u8 *dst, u8 numDigits) {
   const s32 powersOfTen[] = {10, 100, 1000, 10000, 100000};
   s32 i, power, units;
   u8 idx;
   s32 started = 0;

   if (numDigits > 1) {
      idx = numDigits - 2;
      for (i = idx; i >= 0; i--, idx--) {
         // SJIS 8140: Space
         dst[0] = 0x81;
         dst[1] = 0x40;

         if ((num > powersOfTen[idx] - 1) || started) {
            power = powersOfTen[idx];
            units = num / power;
            num = num % power;
            // SJIS 824F: Zero
            *dst++ = 0x82;
            *dst++ = 0x4f + units;
            started = 1;
         } else {
            // Leading spaces
            *dst++ = 0x81;
            *dst++ = 0x40;
         }
      }
   }
   // Final digit
   *dst++ = 0x82;
   *dst++ = 0x4f + num;
}

void EmbedExp(s32 num, s8 *dst, u8 numDigits) {
   // This will share rdata with EmbedIntAsSjis
   const s32 powersOfTen[] = {10, 100, 1000, 10000, 100000};
   s32 i, power, units;
   u8 idx;
   s32 started = 0;

   if (numDigits > 1) {
      idx = numDigits - 2;
      for (i = idx; i >= 0; idx--, i--) {
         *dst = '0';

         if ((num > powersOfTen[idx] - 1) || started) {
            power = powersOfTen[idx];
            units = num / power;
            num = num % power;
            *dst++ = '0' + units;
            started = 1;
         } else {
            dst++;
         }
      }
   }
   *dst++ = '0' + num;
   *dst++ = ' ';
   *dst++ = ' ';
   *dst++ = ' ';
   *dst++ = ' ';
}

u8 GetItemNameLength(u8 item) {
   u8 *p;
   s32 count;

   count = 0;
   p = gItemNames[item];
   while (1) {
      if (*p++ == '\0') {
         return count;
      }
      count++;
   }
}

void ShowExpDialog(s32 exp, u8 windowId) {
   u8 numDigits;
   s8 *pLine1 = "You got            ";
   s8 *pLine2 = "  experience points.";

   numDigits = 1;
   if (exp >= 10) {
      numDigits = 2;
   }
   if (exp >= 100) {
      numDigits = 3;
   }
   if (exp >= 1000) {
      numDigits = 4;
   }
   EmbedExp(exp, &pLine1[8], numDigits); // Writes to rdata?
   DrawWindow(windowId, 0, 0, 192, 45, 60, 82, WBS_CROSSED, 0);
   DrawText(12, 9, 20, 2, 0, pLine1);
   DrawText(12, 21, 20, 2, 0, pLine2);
   DisplayBasicWindow(windowId);
}

s32 s_hp_801231c4;
s32 s_maxHp_801231c8;

void SyncGainedHp(UnitStatus *unit, u8 after) {
   s32 targetHp;

   if (!after) {
      // Save values for next call (after:1)
      s_hp_801231c4 = unit->hp;
      s_maxHp_801231c8 = unit->maxHp;
   } else {
      if (unit->hpFrac != 0) {
         targetHp = s_hp_801231c4 + unit->maxHp - s_maxHp_801231c8;
         while (unit->hp < targetHp) {
            unit->hpFrac++;
            unit->hp = (gClassBaseMaxHp[unit->class] * 2 +
                        gClassBaseMaxHp[unit->class] * (unit->level - 2) +
                        (gClassBaseMaxHp[unit->class] * unit->hpVar100 / 100)) *
                       unit->hpFrac / 10000 / 100;
         }
      }
   }
}

s32 CheckForSupporterBonus(u8 z, u8 x, u8 attackerZ, u8 attackerX, u8 team) {
   if ((z != attackerZ) || (x != attackerX)) {
      if (gMapUnitsPtr[z][x].s.unitIdx != 0 && gMapUnitsPtr[z][x].s.team == team) {
         return 2;
      }
   }
   return 0;
}

void DisplaySupporterBonus(u8 z, u8 x, u8 attackerZ, u8 attackerX, u8 team) {
   EvtData *marker;

   if (CheckForSupporterBonus(z, x, attackerZ, attackerX, team)) {
      marker = Evt_GetUnused();
      marker->functionIndex = EVTF_ATTACK_INFO_MARKER;
      marker->d.evtf052.type = 2;
      HI(marker->d.evtf052.x) = x;
      HI(marker->d.evtf052.z) = z;
   }
}

s16 CalculateAttackDamage(UnitStatus *attacker, UnitStatus *defender) {
   // FIXME: Fake match (forced register)
   // Also gExperienceLevels[n-2] etc results in negative addends, requiring the same fixup as large
   // addends to match
   EvtData *attackerSprite, *defenderSprite;
   u8 ax, az, dx, dz;
   s16 resist;
   s32 power;
   s8 levelDiff;
   s32 damage;
   s32 blockChance;
   BigInt expBuffer1;
   BigInt expBuffer2;
   BigInt expBuffer3;
   s32 rnd;
   s16 dirBonus = 0;

   attackerSprite = attacker->evtSprite;
   defenderSprite = defender->evtSprite;
   resist = gAdvantage[attacker->advantage][defender->advantage];

   dx = HI(defenderSprite->d.sprite.x1);
   dz = HI(defenderSprite->d.sprite.z1);
   az = HI(attackerSprite->d.sprite.z1);
   ax = HI(attackerSprite->d.sprite.x1);

   resist -= CheckForSupporterBonus(dz - 1, dx, az, ax, attacker->team);
   resist -= CheckForSupporterBonus(dz, dx - 1, az, ax, attacker->team);
   resist -= CheckForSupporterBonus(dz + 1, dx, az, ax, attacker->team);
   resist -= CheckForSupporterBonus(dz, dx + 1, az, ax, attacker->team);

   if (attacker->team == TEAM_PLAYER) {
      resist -= gItemEquipmentPower[attacker->weapon];
   }
   if (defender->team == TEAM_PLAYER) {
      resist += gItemEquipmentPower[defender->helmet];
      resist += gItemEquipmentPower[defender->armor];
   }

   // High-ground bonus
   if (attacker->class == CLASS_ARCHER) {
      resist -= (SPR_TERRAIN(attackerSprite).elevation - SPR_TERRAIN(defenderSprite).elevation);
   } else {
      resist -= (SPR_TERRAIN(attackerSprite).elevation - SPR_TERRAIN(defenderSprite).elevation) * 3;
   }

   // Direction bonus
   switch (defender->direction >> 10) {
   case DIR_SOUTH:
      if (HI(attackerSprite->d.sprite.x1) != HI(defenderSprite->d.sprite.x1)) {
         dirBonus += 1;
      } else if (HI(attackerSprite->d.sprite.z1) < HI(defenderSprite->d.sprite.z1)) {
         dirBonus += 2;
      }
      break;
   case DIR_WEST:
      if (HI(attackerSprite->d.sprite.z1) != HI(defenderSprite->d.sprite.z1)) {
         dirBonus += 1;
      } else if (HI(attackerSprite->d.sprite.x1) < HI(defenderSprite->d.sprite.x1)) {
         dirBonus += 2;
      }
      break;
   case DIR_NORTH:
      if (HI(attackerSprite->d.sprite.x1) != HI(defenderSprite->d.sprite.x1)) {
         dirBonus += 1;
      } else if (HI(attackerSprite->d.sprite.z1) > HI(defenderSprite->d.sprite.z1)) {
         dirBonus += 2;
      }
      break;
   default:
      if (HI(attackerSprite->d.sprite.z1) != HI(defenderSprite->d.sprite.z1)) {
         dirBonus += 1;
      } else if (HI(attackerSprite->d.sprite.x1) > HI(defenderSprite->d.sprite.x1)) {
         dirBonus += 2;
      }
      break;
   }

   resist -= dirBonus * 2;

   // Level disparity bonus
   levelDiff = (attacker->level + attacker->atkBoosted) - (defender->level + defender->defBoosted);
   {
      // FIXME
      register s16 tmp asm("v0");
      // s16 tmp;

      if (levelDiff > 0) {
         tmp = -gLevelDisparityBonus[levelDiff] + resist;
      } else {
         tmp = gLevelDisparityBonus[-levelDiff] + resist;
      }
      resist = tmp;

      //@a3c
      if (tmp < 1) {
         resist = 1;
      }
   }

   power = attacker->atkVar10000 + (100000 / resist);
   damage = power - (defender->defVar10000 + gTerrainBonus[SPR_TERRAIN(defenderSprite).terrain]);

   if (damage <= 0) {
      damage = 100;
   }

   if (defender->hpFrac - damage < 100) {
      damage = defender->hpFrac;
   }

   if (!(gState.mapNum == 21 && defender->team == TEAM_ENEMY) && !defender->paralyzed) {
      rnd = rand() & 0x7fff;
      if (defender->level > attacker->level) {
         blockChance = (defender->level - attacker->level) * 32;
      } else {
         blockChance = 0;
      }
      blockChance += gClassBlockChance[defender->class];
      blockChance -= (blockChance / 2) * dirBonus;
      if (blockChance > rnd) {
         damage = 0;
      }
      if (defender->unitId == UNIT_ID_JUGGERNAUT && dirBonus != 2) {
         damage = 0;
      }
   }

   if (defender->aglBoosted) {
      defender->aglBoosted = 0;
      damage = 0;
   }

   if (damage != 0) {
      if (attacker->unitType == UNIT_TYPE_HELL_BAT || attacker->unitType == UNIT_TYPE_GUARDDOG ||
          attacker->unitType == UNIT_TYPE_ACID_GHOST) {
         TryInflictingAilment(attacker, defender, AILMENT_POISON);
      }
      if (attacker->unitType == UNIT_TYPE_ANT_ARM || attacker->unitType == UNIT_TYPE_BLOOD_BAT) {
         TryInflictingAilment(attacker, defender, AILMENT_PARAYLYZE);
      }
   }

   // Calculate experience
   if (attacker->team == TEAM_PLAYER) {
      s32 i, j;

      if (defender->team == TEAM_PLAYER) {
         for (i = 0; i < 8; i++) {
            gState.experience[i] = 0;
         }
      }

      if (attacker->team == TEAM_PLAYER && defender->team != TEAM_PLAYER) {
         for (i = 0; i < 8; i++) {
            gState.experience[i] = 0;
            expBuffer2[i] = 0;
            expBuffer3[i] = 0;
         }
         if (damage != 0) {
            // expBuffer1 = SLm1
            for (i = 0; i < 8; i++) {
               expBuffer1[i] = gExperienceLevels[gState.expScalingLevel - 1][i];
            }
            // expBuffer1 -= SLm2
            BigIntSubtract(expBuffer1, gExperienceLevels[gState.expScalingLevel - 2]);
            // expBuffer2 = expBuffer1 / 20
            BigIntDivide(expBuffer1, expBuffer2, 20);
            // expBuffer3 = expBuffer2 * expMulti:
            for (i = 0; i < defender->expMulti; i++) {
               BigIntAdd(expBuffer3, expBuffer2);
            }
            // expBuffer1 = expBuffer3 / 15
            BigIntDivide(expBuffer3, expBuffer1, 15);
            // ?: expBuffer1 = (((SLm1 - SLm2) / 20) * multi) / 15

            i = 1;
            for (j = damage; j > 1000; j -= 1000) {
               i++;
            }
            if (i < 3) {
               i = 3;
            }
            if (i > 10) {
               i = 10;
            }

            if (defender->hpFrac == damage) {
               // Extra EXP for a kill
               i += 5;
            }

            for (j = 0; j < i; j++) {
               // gState.experience += expBuffer1 * i
               BigIntAdd(gState.experience, expBuffer1);
            }
         }
      }
   }

   return damage;
}

s32 TryInflictingAilment(UnitStatus *source, UnitStatus *target, u8 ailment) {
   // Unused non-void return type is to resolve a swapped instruction
   s32 chance;
   s32 i;
   BigInt expBuffer1;
   BigInt expBuffer2;
   BigInt expBuffer3;

   if (source->team == TEAM_PLAYER) {
      chance = (source->level - target->level) * 0x7fff / 100 + 0x5994;
   } else {
      chance = (source->level - target->level) * 0x7fff / 100 + 0x3fff;
   }

   if (target->ailmentSusceptibility == 1) {
      chance = 0;
   }
   if (target->ailmentSusceptibility == 2) {
      chance /= 2;
   }

   if (chance > (rand() & 0x7fff)) {
      // Calculate experience
      // Bug? I think the OR should've been bracketed differently
      if (((target->team != TEAM_PLAYER) && (ailment == AILMENT_POISON && !target->poisoned)) ||
          (ailment != AILMENT_POISON && !target->paralyzed)) {

         for (i = 0; i < 8; i++) {
            expBuffer1[i] = gExperienceLevels[gState.expScalingLevel - 1][i];
            expBuffer3[i] = 0;
         }
         BigIntSubtract(expBuffer1, gExperienceLevels[gState.expScalingLevel - 2]);
         BigIntDivide(expBuffer1, expBuffer2, 20);
         for (i = 0; i < target->expMulti; i++) {
            BigIntAdd(expBuffer3, expBuffer2);
         }
         BigIntDivide(expBuffer3, expBuffer1, 15);
         for (i = 0; i < 5; i++) {
            BigIntAdd(gState.experience, expBuffer1);
         }
      }
      // Apply ailment
      if (ailment == AILMENT_POISON) {
         target->poisoned++;
      } else {
         target->paralyzed = 1;
      }
   }
}

void CalculateSupportSpellExp(UnitStatus *target) {
   u8 level;
   s32 i;
   BigInt expBuffer1;
   BigInt expBuffer2;
   BigInt expBuffer3;

   if (target->team == TEAM_PLAYER) {
      level = target->level;
      if (level < 2) {
         level = 2;
      }

      for (i = 0; i < 8; i++) {
         expBuffer1[i] = gExperienceLevels[level - 1][i];
         expBuffer3[i] = 0;
      }
      BigIntSubtract(expBuffer1, gExperienceLevels[level - 2]);
      BigIntDivide(expBuffer1, expBuffer2, 20);
      for (i = 0; i < 20; i++) {
         BigIntAdd(expBuffer3, expBuffer2);
      }
      BigIntDivide(expBuffer3, expBuffer1, 15);
      for (i = 0; i < 5; i++) {
         BigIntAdd(gState.experience, expBuffer1);
      }
   }
}

s16 CalculateSpellPowerAndExp(UnitStatus *caster, UnitStatus *target) {
   s32 damage = 0;
   s32 power;
   s8 level;
   s16 resist = 10;
   s16 half;
   s32 i, j;
   BigInt expBuffer1;
   BigInt expBuffer2;
   BigInt expBuffer3;

   if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] == SPELL_EFFECT_DAMAGE) {
      level = gSpells[gCurrentSpell].power - target->level;
      if (level > 0) {
         resist -= level * 2;
         if (resist < 1) {
            resist = 1;
         }
      } else {
         resist -= level * 2;
      }

      half = resist >> 1;

      switch (target->magicSusceptibility) {
      case 1:
         resist += half;
         break;
      case 2:
         resist += half >> 1;
         break;
      case 4:
         resist -= half >> 1;
         break;
      case 5:
         resist -= half;
         break;
      }

      if (resist < 1) {
         resist = 1;
      }

      power = caster->atkVar10000 + (100000 / resist);
      damage = power - target->defVar10000;

      if (damage <= 0) {
         damage = 100;
      }

      if (target->hpFrac - damage < 100) {
         damage = target->hpFrac;
      }

   } else if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] == SPELL_EFFECT_RESTORE_HP) {
      //@144c
      level = gSpells[gCurrentSpell].power - target->level;
      if (level >= 0) {
         damage = 10000 - target->hpFrac;
      } else {
         damage = level * 500 + 10000;
         if (damage < 500) {
            damage = 500;
         }
         if (damage + target->hpFrac > 10000) {
            damage = 10000 - target->hpFrac;
         }
      }
   }

   // Calculate experience
   if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] == SPELL_EFFECT_DAMAGE) {
      if (caster->team == TEAM_PLAYER && target->team != TEAM_PLAYER) {
         for (i = 0; i < 8; i++) {
            expBuffer2[i] = 0;
            expBuffer3[i] = 0;
         }
         if (damage != 0) {
            for (i = 0; i < 8; i++) {
               expBuffer1[i] = gExperienceLevels[gState.expScalingLevel - 1][i];
            }
            BigIntSubtract(expBuffer1, gExperienceLevels[gState.expScalingLevel - 2]);
            BigIntDivide(expBuffer1, expBuffer2, 20);
            for (i = 0; i < target->expMulti; i++) {
               BigIntAdd(expBuffer3, expBuffer2);
            }
            BigIntDivide(expBuffer3, expBuffer1, 15);

            i = 1;
            for (j = damage; j > 1000; j -= 1000) {
               i++;
            }
            if (i > 10) {
               i = 10;
            }
            if (target->hpFrac == damage) {
               // Extra EXP for a kill
               i += 5;
            }
            for (j = 0; j < i; j++) {
               BigIntAdd(gState.experience, expBuffer1);
            }
         }
      }
   } else if (gSpellsEx[gCurrentSpell][SPELL_EX_EFFECT] == SPELL_EFFECT_RESTORE_HP) {
      if (caster->team == TEAM_PLAYER && target->team == TEAM_PLAYER) {
         //@16e8
         level = target->level;
         if (level < 2) {
            level = 2;
         }

         for (i = 0; i < 8; i++) {
            expBuffer2[i] = 0;
            expBuffer3[i] = 0;
         }

         if (damage != 0) {
            for (i = 0; i < 8; i++) {
               expBuffer1[i] = gExperienceLevels[level - 1][i];
            }
            BigIntSubtract(expBuffer1, gExperienceLevels[level - 2]);
            BigIntDivide(expBuffer1, expBuffer2, 20);
            for (i = 0; i < 20; i++) {
               BigIntAdd(expBuffer3, expBuffer2);
            }
            BigIntDivide(expBuffer3, expBuffer1, 15);

            i = 1;
            for (j = damage; j > 1000; j -= 1000) {
               i++;
            }
            if (i > 10) {
               i = 10;
            }
            if (target->hpFrac == damage) {
               i += 5;
            }
            for (j = 0; j < i; j++) {
               BigIntAdd(gState.experience, expBuffer1);
            }
         }
      }
   }

   return damage;
}

void DetermineMaxMpAndStatVariance(UnitStatus *unit) {
   s32 unitClass;

   if (unit->team == TEAM_PLAYER) {
      unitClass = unit->class;
      if (unit->name == UNIT_ASH) {
         unitClass = CLASS_7;
      }
      unit->maxMp = gClassMpMultiplier[unitClass] * unit->level;
      if (unit->class == CLASS_MONK) {
         unit->maxMp += gPartyMembers[unit->name].advLevelFirst;
      }
      if (unit->maxMp > 99) {
         unit->maxMp = 99;
      }
   }
   // 80..119
   unit->hpVar100 = (rand() % 40) + 80;
   unit->atkVar100 = (rand() % 40) + 80;
   unit->defVar100 = (rand() % 40) + 80;
   unit->agiVar100 = (rand() % 40) + 80;
}

void CalculateUnitStats(UnitStatus *unit) {
   // s32 level;
   s32 i;
   u32 a0;
   u32 a2;
   BigInt expBuffer1;
   BigInt expBuffer2;

   // 9700..10285 (assuming var100 of 80..119)
   unit->atkVar10000 = (unit->atkVar100 * 15) + 8500;
   unit->defVar10000 = (unit->defVar100 * 15) + 8500;
   unit->agiVar10000 = (unit->agiVar100 * 15) + 8500;
   // += -200..190
   unit->defVar10000 += (unit->hpVar100 * 10) - 1000;

   i = 0;
   while (BigIntCompare(unit->experience, gExperienceLevels[i]) < 2) {
      i++;
   }
   unit->level = i;

   if (unit->level > 1) {
      unit->maxHp =
          (gClassBaseMaxHp[unit->class] * 2 + gClassBaseMaxHp[unit->class] * (unit->level - 2) +
           (gClassBaseMaxHp[unit->class] * unit->hpVar100) / 100) /
          100;

      unit->hp =
          ((gClassBaseMaxHp[unit->class] * 2 + gClassBaseMaxHp[unit->class] * (unit->level - 2) +
            (gClassBaseMaxHp[unit->class] * unit->hpVar100) / 100) *
           unit->hpFrac / 10000) /
          100;

      if (unit->hp == 0 && unit->hpFrac != 0) {
         unit->hp = 1;
      }

      unit->attack =
          (gClassBaseAttack[unit->class] + (gClassBaseAttack[unit->class] / 3) * (unit->level - 2) +
           ((gClassBaseAttack[unit->class] / 3) * unit->atkVar100) / 100) /
          100;

      unit->defense = (gClassBaseDefense[unit->class] +
                       (gClassBaseDefense[unit->class] / 3) * (unit->level - 2) +
                       ((gClassBaseDefense[unit->class] / 3) * unit->defVar100) / 100) /
                      100;

      unit->agility = (gClassBaseAgility[unit->class] +
                       (gClassBaseAgility[unit->class] / 3) * (unit->level - 2) +
                       ((gClassBaseAgility[unit->class] / 3) * unit->agiVar100) / 100) /
                      100;

   } else {
      unit->maxHp = gClassBaseMaxHp[unit->class] / 50;
      unit->hp = (gClassBaseMaxHp[unit->class] * ((unit->hpFrac / 100) << 1)) / 10000;
      if (unit->hp == 0 && unit->hpFrac != 0) {
         unit->hp = 1;
      }
      unit->attack = gClassBaseAttack[unit->class] / 100;
      unit->defense = gClassBaseDefense[unit->class] / 100;
      unit->agility = gClassBaseAgility[unit->class] / 100;
   }

   for (i = 0; i < 8; i++) {
      expBuffer1[i] = unit->experience[i];
   }

   BigIntSubtract(expBuffer1, gExperienceLevels[unit->level - 1]);

   for (i = 0; i < 8; i++) {
      expBuffer2[i] = gExperienceLevels[unit->level][i];
   }

   BigIntSubtract(expBuffer2, gExperienceLevels[unit->level - 1]);

   i = 0;
   while (1) {
      if (expBuffer2[i] == 0) {
         i++;
      } else {
         break;
      }
   }

   if (i == 7) {
      i = 6;
   }

   if (expBuffer1[i] < 100) {
      a2 = expBuffer2[i + 1] + (expBuffer2[i] << 16);
      a0 = expBuffer1[i + 1] + (expBuffer1[i] << 16);
   } else {
      a2 = expBuffer2[i];
      a0 = expBuffer1[i];
   }

   a0 *= 100;
   unit->exp = (unit->level - 1) * 100;
   if (a2 != 0) {
      unit->exp += a0 / a2;
   }
}

s32 BigIntCompare(BigInt lhs, BigInt rhs) {
   s32 i;

   i = 0;
   while (1) {
      if (*lhs > *rhs) {
         return 0;
      }
      if (*lhs++ < *rhs++) {
         return 2;
      }
      i++;
      if (i == 8) {
         return 1;
      }
   }
}

void BigIntAdd(BigInt lhs, BigInt rhs) {
   // lhs += rhs
   s32 i;
   s32 value;
   s32 *pLhs, *pRhs;
   s32 lhsBuf[8];
   s32 rhsBuf[8];

   pLhs = &lhsBuf[7];
   pRhs = &rhsBuf[7];

   for (i = 0; i < 8; i++) {
      lhsBuf[i] = (s32)lhs[i];
      rhsBuf[i] = (s32)rhs[i];
   }

   for (i = 0; i < 8; i++) {
      value = *pLhs + *pRhs;

      if (value > 0xffff) {
         value -= 0x10000;
         pLhs[-1] += 1;
      }
      *pLhs-- = value;
      pRhs--;
   }

   for (i = 0; i < 8; i++) {
      lhs[i] = (u16)lhsBuf[i];
   }
}

void BigIntSubtract(BigInt lhs, BigInt rhs) {
   // lhs -= rhs
   s32 i;
   s32 *pLhs, *pRhs;
   s32 lhsBuf[8];
   s32 rhsBuf[8];

   pLhs = &lhsBuf[7];
   pRhs = &rhsBuf[7];

   for (i = 0; i < 8; i++) {
      lhsBuf[i] = (s32)lhs[i];
      rhsBuf[i] = (s32)rhs[i];
   }

   for (i = 0; i < 8; i++) {
      if (*pLhs >= *pRhs) {
         *pLhs-- = *pLhs - *pRhs;
      } else {
         *pLhs-- = 0x10000 - (*pRhs - *pLhs);
         pRhs[-1] += 1;
      }
      pRhs--;
   }

   for (i = 0; i < 8; i++) {
      lhs[i] = (u16)lhsBuf[i];
   }
}

void BigIntDivide(BigInt dividend, BigInt quotient, u16 divisor) {
   s32 i;
   s32 *pQuotient, *pDividend;
   s32 dividendBuf[8];
   s32 quotientBuf[8];

   pDividend = &dividendBuf[0];
   pQuotient = &quotientBuf[0];

   for (i = 0; i < 8; i++) {
      dividendBuf[i] = (s32)dividend[i];
      quotientBuf[i] = 0;
   }

   for (i = 0; i < 7; i++) {
      if (*pDividend >= divisor) {
         *pQuotient++ = *pDividend / divisor;
         pDividend[1] += (*pDividend % divisor) * 0x10000;
      } else {
         *pQuotient++ = 0;
         pDividend[1] += *pDividend * 0x10000;
      }
      pDividend++;
   }

   if (*pDividend >= divisor) {
      *pQuotient = *pDividend / divisor;
   } else {
      *pQuotient = 0;
   }

   for (i = 0; i < 8; i++) {
      quotient[i] = (u16)quotientBuf[i];
   }
}

void func_8002ADCC(s16 z, s16 x, s32 fieldSize, s32 whichGrid) {
   PathGridRow *pGrid;
   s32 push_t0;
   s32 pop_t1;
   u8 abStack602[600];
   s32 swap;

   push_t0 = 0;
   pop_t1 = 300;

   if (z >= gMapMinZ && z <= gMapMaxZ && x >= gMapMinX && x <= gMapMaxX) {

      switch (whichGrid) {
      case 0:
         pGrid = &gPathGrid0[1];
         break;
      case 1:
         pGrid = &gPathGrid1[1];
         break;
      case 2:
         pGrid = &gPathGrid2[1];
         break;
      case 3:
         pGrid = &gPathGrid3[1];
         break;
      case 4:
         pGrid = &gPathGrid4[1];
         break;
      case 5:
         pGrid = &gPathGrid5[1];
         break;
      case 6:
         pGrid = &gPathGrid6[1];
         break;
      }

      if (fieldSize == 0) {
         pGrid[z][x] = 1;
         return;
      }

      fieldSize--;
      abStack602[push_t0] = PATH_STEP_INVALID;
      push_t0 += 2; // 2
      abStack602[pop_t1] = PATH_STEP_INVALID;
      pop_t1 += 2; // 302
      pGrid[z][x] = 1;

      do {
         while (z != PATH_STEP_INVALID) {

            if (pGrid[z - 1][x] == PATH_STEP_UNSET) {
               pGrid[z - 1][x]++;
               if (fieldSize != 0) {
                  abStack602[push_t0++] = z - 1;
                  abStack602[push_t0++] = x;
               }
            }
            if (pGrid[z + 1][x] == PATH_STEP_UNSET) {
               pGrid[z + 1][x]++;
               if (fieldSize != 0) {
                  abStack602[push_t0++] = z + 1;
                  abStack602[push_t0++] = x;
               }
            }
            if (pGrid[z][x - 1] == PATH_STEP_UNSET) {
               pGrid[z][x - 1]++;
               if (fieldSize != 0) {
                  abStack602[push_t0++] = z;
                  abStack602[push_t0++] = x - 1;
               }
            }
            if (pGrid[z][x + 1] == PATH_STEP_UNSET) {
               pGrid[z][x + 1]++;
               if (fieldSize != 0) {
                  abStack602[push_t0++] = z;
                  abStack602[push_t0++] = x + 1;
               }
            }

            // pop_t1 -= 2;
            pop_t1--;
            pop_t1--;
            z = abStack602[pop_t1 + 0];
            x = abStack602[pop_t1 + 1];

         } // inner

         pop_t1 += 2;
         swap = pop_t1;
         pop_t1 = push_t0;
         push_t0 = swap;

         pop_t1--;
         pop_t1--;
         z = abStack602[pop_t1 + 0];
         x = abStack602[pop_t1 + 1];

         fieldSize--;

      } while (z != PATH_STEP_INVALID); // outer
   }
}

void ApplyAirmanAdjustments(u8 team) {
   s32 iz, ix;

   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         gTileStateGridPtr[iz][ix].cachedShort = gMapUnitsPtr[iz][ix].raw;
         gTileStateGridPtr[iz][ix].cachedByte = gTerrainPtr[iz][ix].terrain;

         if (gMapUnitsPtr[iz][ix].s.unitIdx != 0 && gMapUnitsPtr[iz][ix].s.team != team) {
            gMapUnitsPtr[iz][ix].raw = 0;

            if (gTerrainPtr[iz][ix].terrain == TERRAIN_BOUNDARY) {
               gTerrainPtr[iz][ix].terrain = TERRAIN_10;
            } else {
               gTerrainPtr[iz][ix].terrain = TERRAIN_OBSTACLE;
            }
         }
         if (gTerrainPtr[iz][ix].terrain == TERRAIN_OBSTACLE) {
            gMapUnitsPtr[iz][ix].s.team = TEAM_NULL;
         }
      }
   }
}

void RevertAirmanAdjustments(void) {
   s32 iz, ix;

   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         gMapUnitsPtr[iz][ix].raw = gTileStateGridPtr[iz][ix].cachedShort;
         gTerrainPtr[iz][ix].terrain = gTileStateGridPtr[iz][ix].cachedByte;
      }
   }
}
