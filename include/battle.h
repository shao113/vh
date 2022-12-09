#ifndef BATTLE_H
#define BATTLE_H

#include "common.h"

typedef enum BattleEvalState {
   BATTLE_EVAL_NONE = 0,
   BATTLE_EVAL_DEFEAT = 1,
   BATTLE_EVAL_VICTORY = 2
} BattleEvalState;

typedef struct SlainUnit {
   s16 unitId;
   s16 count;
   s16 reward;
} SlainUnit;

extern u8 gIsEnemyTurn;
extern SlainUnit gSlainUnits[20];
extern u8 gPartyMemberSlain[PARTY_CT];

UnitStatus *FindUnitByNameIdx(s16);
s32 CountUnitsOfType(s16);
s32 CountUnitsOfTeam(s16);

#endif
