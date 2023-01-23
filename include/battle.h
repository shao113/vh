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

typedef enum TileAction {
   TA_NONE = 0,
   TA_PHYS_HIT = 1,
   TA_BLOCK = 2,
   TA_PHYS_DEFEAT = 3,
   TA_MELEE_ATK = 5,
   TA_X6 = 6, // ?: moving
   TA_CHOOSING_DIRECTION = 7,
   TA_CAST = 8,
   TA_X9 = 9, // ?: after attacking, cancelled move dest
   TA_RANGED_ATK = 10,
   TA_MAG_DEFEAT = 12,
   TA_MAG_HIT = 13,
   TA_CRATE_PUSHED_PRE = 15,
   TA_CRATE_PUSHED = 16,
   TA_PUSHING_CRATE = 17,
   TA_LEVEL_UP = 21,
   TA_PHYS_DEFEAT_MSG = 23,
   TA_MAG_DEFEAT_MSG = 24,
   TA_PUSHING_BOULDER = 26,
   TA_BOULDER_PUSHED = 27,
   TA_BOULDER_HIT = 28,
} TileAction;

typedef struct TileState {
   u8 action;
   s8 cachedByte;
   s16 cachedWord;
} TileState;

extern u8 gIsEnemyTurn;
extern SlainUnit gSlainUnits[20];
extern u8 gPartyMemberSlain[PARTY_CT];
extern s16 gTargetX, gTargetZ;
extern s16 gCurrentSpell;
extern TileState (*gTileStateGridPtr)[65];

struct UnitStatus *FindUnitByNameIdx(s16);
s32 CountUnitsOfType(s16);
s32 CountUnitsOfTeam(s16);

#endif
