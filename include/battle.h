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

typedef enum AttackResult {
   ATK_RES_HIT = 0,
   ATK_RES_BLOCKED = 1,
   ATK_RES_DEFEATED = 2
} AttackResult;

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
   TA_XB = 11,
   TA_MAG_DEFEAT = 12,
   TA_MAG_HIT = 13,
   TA_CRATE_PUSHED_PRE = 15,
   TA_CRATE_PUSHED = 16,
   TA_PUSHING_CRATE = 17,
   TA_CHEST_1 = 18, // TBD
   TA_CHEST_2 = 19,
   TA_CHEST_3 = 20,
   TA_LEVEL_UP = 21,
   TA_X16 = 22,
   TA_PHYS_DEFEAT_MSG = 23,
   TA_MAG_DEFEAT_MSG = 24,
   TA_X19 = 25, // ?: Spawning
   TA_PUSHING_BOULDER = 26,
   TA_BOULDER_PUSHED = 27,
   TA_BOULDER_HIT = 28,
   TA_X1D = 29,
   TA_X1E = 30,
   TA_X1F = 31,
} TileAction;

typedef struct TileState {
   s8 action;
   s8 cachedByte;
   s16 cachedShort;
} TileState;

typedef struct BattleBGM {
   s16 seqSetIdx;
   s16 seqId;
} BattleBGM;

extern BattleBGM gBattleBGM[BATTLE_CT];
extern s16 gBattleEvaluator[BATTLE_CT];
extern s8 gBattleExpScalingLevels[BATTLE_CT];
extern s16 gBattleUnitRewards[BATTLE_CT][20];
extern u8 gBattleSceneId[50];

extern u8 gIsEnemyTurn;
extern SlainUnit gSlainUnits[20];
extern u8 gPartyMemberSlain[PARTY_CT];
extern s16 gTargetX, gTargetZ;
extern u8 gTargetCoords[72];
extern s16 gCurrentSpell;
extern TileState (*gTileStateGridPtr)[65];

extern s8 gZ_801233dc, gX_801233d8, gTargetZ_80123418, gTargetX_80123414;
extern u16 gDir_80123470;
extern u8 D_80123468, D_8012337C;

extern u8 gAdvantage[49][49];
extern u8 gLevelDisparityBonus[160];

// struct UnitStatus *FindUnitByNameIdx(s16); //FIXME
s32 CountUnitsOfType(s16);
s32 CountUnitsOfTeam(s16);

void QueueImpededStep(s16, s16, s32, s32 *, s16, u8);
void PlotPathBackToUnit(s32, s32);
void PopulateMeleeAttackGridInDir(s16, s16, s16, s16, s32);
void PopulateMeleeAttackGrid(s16, s16, s32, u8);
void PopulateCastingGrid(s16, s16, s32, s32);
void ClearGrid(s32);
void ClearBlueMovementGrid(void);
void PopulateRangedAttackGrid(s16, s16, s32, s32);

#endif
