#ifndef BATTLE_H
#define BATTLE_H

#include "common.h"
#include "units.h"

typedef enum BattleEvalState {
   BATTLE_EVAL_NONE = 0,
   BATTLE_EVAL_DEFEAT = 1,
   BATTLE_EVAL_VICTORY = 2
} BattleEvalState;

extern s8 gIsEnemyTurn;

UnitStatus *FindUnitByNameIdx(s16);
s32 CountUnitsOfType(s16);
s32 CountUnitsOfTeam(s16);

#endif
