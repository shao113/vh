#include "common.h"
#include "battle.h"
#include "units.h"
#include "field.h"

// This file should probably be merged with the preceding functions (starting from func_8002ADCC?)
// into something like path_grids.c, once I tackle the missing path grid functions.
// The path grid functions are very similar, but there's clearly some missing ingredient keeping
// them from matching cleanly.

void func_8002C108(s16, s16, s16, s32 *, s16, s16);
void func_8002C1A0(s16, s16, s32, s32);
void func_8002C1D8(s16, s16, s32, s32, s16, s16);
u8 func_8002CEE4(s16, s16, s16, s32 *, s16);
void func_8002CF88(s16, s16, s32, s32, s32);
void PopulateBlueMovementGrid(s16, s16, s32);
void PopulateMovementGrid(s16, s16, s32, s32);
void QueueImpededStep(s32, s32, s32, s32 *, s16, u8);
void PlotPathBackToUnit(s32, s32);
void PopulateMeleeAttackGridInDir(s16, s16, s16, s16, s32);
void PopulateMeleeAttackGrid(s16, s16, s32, u8);
void PopulateCastingGrid(s16, s16, s32, s32);
void ClearGrid(s32);
void ClearBlueMovementGrid(void);
void PopulateRangedAttackGrid(s16, s16, s32, s32);
void NoopForever_8002f9b0(void);

extern void ApplyAirmanAdjustments(u8);
extern void RevertAirmanAdjustments(void);

void func_8002C108(s16 z, s16 x, s16 rem, s32 *pCount, s16 cost, s16 range) {
   s32 i;
   u8 *p;

   rem -= cost;
   if (rem >= 0) {
      p = gImpededStepsQueue[cost];
      i = 0;
      while (p[i + 0] != PATH_STEP_INVALID) {
         i += 4;
      }
      p[i + 0] = z;
      p[i + 1] = x;
      p[i + 2] = range - rem;
      p[i + 3] = rem;
      (*pCount)++;
   }
}

void func_8002C1A0(s16 z, s16 x, s32 range, s32 gridNum) {
   func_8002C1D8(z, x, range, gridNum, 100, 100);
}

void func_8002C1D8(s16 z, s16 x, s32 range, s32 gridNum, s16 zz, s16 xx) {
   PathGridRow *pGrid;
   s32 i, j;
   s32 pushIdx;
   s32 popIdx;
   s32 rem;
   s32 pendingCount;
   s32 elevationCost;
   s32 terrainCost;
   u8 queueOfs;
   u8 team;
   s16 diff;
   u8 stepType;
   UnitStatus *unit;
   u8 buffer[600];

   pushIdx = 0;
   popIdx = 300;
   pendingCount = 0;

   switch (gridNum) {
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

   rem = range;
   queueOfs = 0;

   if (xx == 100 && zz == 100) {
      unit = &gUnits[gMapUnitsPtr[z][x].s.unitIdx];
      stepType = unit->step;
      team = gMapUnitsPtr[z][x].s.team;
   } else {
      unit = &gUnits[gMapUnitsPtr[zz][xx].s.unitIdx];
      stepType = unit->step;
      team = gMapUnitsPtr[zz][xx].s.team;
   }

   for (i = 0; i < 5; i++) {
      for (j = 0; j < 200; j++) {
         gImpededSteps[i][j] = PATH_STEP_INVALID;
      }
   }
   for (i = 0; i < 5; i++) {
      gImpededStepsQueue[i] = &gImpededSteps[i][0];
   }

   if (z < gMapMinZ || z > gMapMaxZ || x < gMapMinX || x > gMapMaxX)
      return;

   rem--;

   buffer[pushIdx++] = PATH_STEP_INVALID;
   // pushIdx += 2; // 2
   pushIdx++;

   buffer[popIdx++] = PATH_STEP_INVALID;
   // popIdx += 2; // 302
   popIdx++;

   ClearGrid(gridNum);

   if (unit->class == CLASS_AIRMAN) {
      ApplyAirmanAdjustments(team);
   }
   pGrid[z][x] = range + 1;

   while (1) {
      while (1) {
         while (z != PATH_STEP_INVALID) {
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z - 1][x].s.terrain];
            diff = gTerrainPtr[z - 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z - 1][x].s.team == 0 || gMapUnitsPtr[z - 1][x].s.team == team) {
               if (gTerrainPtr[z - 1][x].s.terrain >= 0 && pGrid[z - 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z - 1][x] = rem + 1;
                     if (rem != 0) {
                        buffer[pushIdx++] = z - 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     func_8002CEE4(z - 1, x, rem, &pendingCount, terrainCost + elevationCost);
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z + 1][x].s.terrain];
            diff = gTerrainPtr[z + 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z + 1][x].s.team == 0 || gMapUnitsPtr[z + 1][x].s.team == team) {
               if (gTerrainPtr[z + 1][x].s.terrain >= 0 && pGrid[z + 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z + 1][x] = rem + 1;
                     if (rem != 0) {
                        buffer[pushIdx++] = z + 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     func_8002CEE4(z + 1, x, rem, &pendingCount, terrainCost + elevationCost);
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x - 1].s.terrain];
            diff = gTerrainPtr[z][x - 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x - 1].s.team == 0 || gMapUnitsPtr[z][x - 1].s.team == team) {
               if (gTerrainPtr[z][x - 1].s.terrain >= 0 && pGrid[z][x - 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z][x - 1] = rem + 1;
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x - 1;
                     }
                  } else {
                     func_8002CEE4(z, x - 1, rem, &pendingCount, terrainCost + elevationCost);
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x + 1].s.terrain];
            diff = gTerrainPtr[z][x + 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x + 1].s.team == 0 || gMapUnitsPtr[z][x + 1].s.team == team) {
               if (gTerrainPtr[z][x + 1].s.terrain >= 0 && pGrid[z][x + 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z][x + 1] = rem + 1;
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x + 1;
                     }
                  } else {
                     func_8002CEE4(z, x + 1, rem, &pendingCount, terrainCost + elevationCost);
                  }
               }
            }

            popIdx -= 2;
            z = buffer[popIdx + 0];
            x = buffer[popIdx + 1];
         }

         popIdx += 2;
         i = popIdx;
         popIdx = pushIdx;
         pushIdx = i;

         i = 0;
         while (gImpededStepsQueue[0][i + 0] != PATH_STEP_INVALID) {
            z = gImpededStepsQueue[0][i + 0]; //.z
            x = gImpededStepsQueue[0][i + 1]; //.x
            if (pGrid[z][x] == PATH_STEP_UNSET) {
               pGrid[z][x] = gImpededStepsQueue[0][i + 2]; //.step
               if (gImpededStepsQueue[0][i + 3] != 0) {    //.rem
                  buffer[popIdx++] = z;
                  buffer[popIdx++] = x;
               }
            }
            gImpededStepsQueue[0][i] = PATH_STEP_INVALID;
            pendingCount--;
            i += 4;
         }

         popIdx -= 2;
         z = buffer[popIdx + 0];
         x = buffer[popIdx + 1];

         rem--;
         queueOfs++;
         if (queueOfs == 5) {
            queueOfs = 0;
         }
         j = queueOfs;

         for (i = 0; i < 5; i++) {
            if (i + j == 5) {
               j -= 5;
            }
            gImpededStepsQueue[i] = &gImpededSteps[i + j][0];
         }

         if (pendingCount == 0 && z == PATH_STEP_INVALID) {
            if (unit->class == CLASS_AIRMAN) {
               RevertAirmanAdjustments();
               for (i = gMapMinZ; i <= gMapMaxZ; i++) {
                  for (j = gMapMinX; j <= gMapMaxX; j++) {
                     if (gTerrainPtr[i][j].s.terrain == TERRAIN_OBSTACLE) {
                        pGrid[i][j] = PATH_STEP_UNSET;
                     }
                  }
               }
            }
            return;
         }
      }
   }
}

u8 func_8002CEE4(s16 z, s16 x, s16 rem, s32 *pCount, s16 cost) {
   s32 i;
   u8 *p;

   rem -= cost;
   if (rem < 0) {
      return 0;
   } else {
      p = gImpededStepsQueue[cost];
      i = 0;
      while (p[i + 0] != PATH_STEP_INVALID) {
         i += 4;
      }
      p[i + 0] = z;
      p[i + 1] = x;
      p[i + 2] = rem + 1;
      p[i + 3] = rem;
      (*pCount)++;
      return rem + 1;
   }
}

void func_8002CF88(s16 z, s16 x, s32 range, s32 gridNum1, s32 gridNum2) {
   // FIXME: Fake match (do-while)

   s32 i, j;
   s32 pushIdx;
   s32 popIdx;
   s32 rem;
   s32 pendingCount;
   s32 elevationCost;
   s32 terrainCost;
   u8 queueOfs;
   u8 team;
   u8 stepType;
   s16 diff;
   u8 buffer[600];
   UnitStatus *unit;
   PathGridRow *pGrid1;
   PathGridRow *pGrid2;
   s32 b;

   pushIdx = 0;
   popIdx = 300;
   pendingCount = 0;

   switch (gridNum2) {
   case 0:
      pGrid2 = &gPathGrid0[1];
      break;
   case 1:
      pGrid2 = &gPathGrid1[1];
      break;
   case 2:
      pGrid2 = &gPathGrid2[1];
      break;
   case 3:
      pGrid2 = &gPathGrid3[1];
      break;
   case 4:
      pGrid2 = &gPathGrid4[1];
      break;
   case 5:
      pGrid2 = &gPathGrid5[1];
      break;
   case 6:
      pGrid2 = &gPathGrid6[1];
      break;
   }

   switch (gridNum1) {
   case 0:
      pGrid1 = &gPathGrid0[1];
      break;
   case 1:
      pGrid1 = &gPathGrid1[1];
      break;
   case 2:
      pGrid1 = &gPathGrid2[1];
      break;
   case 3:
      pGrid1 = &gPathGrid3[1];
      break;
   case 4:
      pGrid1 = &gPathGrid4[1];
      break;
   case 5:
      pGrid1 = &gPathGrid5[1];
      break;
   case 6:
      pGrid1 = &gPathGrid6[1];
      break;
   }

   rem = range;
   queueOfs = 0;
   unit = &gUnits[gMapUnitsPtr[z][x].s.unitIdx];
   stepType = unit->step;

   team = gMapUnitsPtr[z][x].s.team;

   for (i = 0; i < 5; i++) {
      for (j = 0; j < 200; j++) {
         gImpededSteps[i][j] = PATH_STEP_INVALID;
      }
   }

   for (i = 0; i < 5; i++) {
      gImpededStepsQueue[i] = &gImpededSteps[i][0];
   }
   if (z < gMapMinZ || z > gMapMaxZ || x < gMapMinX || x > gMapMaxX) {
      return;
   }

   rem--;
   buffer[pushIdx++] = PATH_STEP_INVALID;
   pushIdx++; // 2

   buffer[popIdx++] = PATH_STEP_INVALID;
   popIdx++; // 302

   ClearGrid(gridNum1);

   if (unit->class == CLASS_AIRMAN) {
      ApplyAirmanAdjustments(team);
   }
   pGrid1[z][x] = range;

   while (1) {
      while (1) {
         while (z != PATH_STEP_INVALID) {
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z - 1][x].s.terrain];
            diff = gTerrainPtr[z - 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z - 1][x].s.team == 0 || gMapUnitsPtr[z - 1][x].s.team == team) {
               if (gTerrainPtr[z - 1][x].s.terrain >= 0 && pGrid1[z - 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid1[z - 1][x] = rem + 1;
                     b = rem + 1;
                     b &= 0xff; //? u8?
                     if (pGrid2[z - 1][x] < b) {
                        pGrid2[z - 1][x] = b;
                     }
                     if (rem != 0) {
                        buffer[pushIdx++] = z - 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     b = func_8002CEE4(z - 1, x, rem, &pendingCount, terrainCost + elevationCost);
                     if (pGrid2[z - 1][x] < b) {
                        pGrid2[z - 1][x] = b;
                     }
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z + 1][x].s.terrain];
            diff = gTerrainPtr[z + 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z + 1][x].s.team == 0 || gMapUnitsPtr[z + 1][x].s.team == team) {
               if (gTerrainPtr[z + 1][x].s.terrain >= 0 && pGrid1[z + 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid1[z + 1][x] = rem + 1;
                     b = rem + 1;
                     b &= 0xff;
                     if (pGrid2[z + 1][x] < b) {
                        pGrid2[z + 1][x] = b;
                     }
                     if (rem != 0) {
                        buffer[pushIdx++] = z + 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     b = func_8002CEE4(z + 1, x, rem, &pendingCount, terrainCost + elevationCost);
                     if (pGrid2[z + 1][x] < b) {
                        pGrid2[z + 1][x] = b;
                     }
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x - 1].s.terrain];
            diff = gTerrainPtr[z][x - 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x - 1].s.team == 0 || gMapUnitsPtr[z][x - 1].s.team == team) {
               if (gTerrainPtr[z][x - 1].s.terrain >= 0 && pGrid1[z][x - 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid1[z][x - 1] = rem + 1;
                     b = rem + 1;
                     b &= 0xff;
                     if (pGrid2[z][x - 1] < b) {
                        pGrid2[z][x - 1] = b;
                     }
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x - 1;
                     }
                  } else {
                     b = func_8002CEE4(z, x - 1, rem, &pendingCount, terrainCost + elevationCost);
                     if (pGrid2[z][x - 1] < b) {
                        pGrid2[z][x - 1] = b;
                     }
                  }
               }
            }

            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x + 1].s.terrain];
            diff = gTerrainPtr[z][x + 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x + 1].s.team == 0 || gMapUnitsPtr[z][x + 1].s.team == team) {
               if (gTerrainPtr[z][x + 1].s.terrain >= 0 && pGrid1[z][x + 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid1[z][x + 1] = rem + 1;
                     b = rem + 1;
                     b &= 0xff;
                     if (pGrid2[z][x + 1] < b) {
                        pGrid2[z][x + 1] = b;
                     }
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x + 1;
                        do {
                        } while (0); // FIXME
                     }
                  } else {
                     b = func_8002CEE4(z, x + 1, rem, &pendingCount, terrainCost + elevationCost);
                     if (pGrid2[z][x + 1] < b) {
                        pGrid2[z][x + 1] = b;
                     }
                  }
               }
            }

            popIdx -= 2;
            z = buffer[popIdx + 0];
            x = buffer[popIdx + 1];
         }

         popIdx += 2;
         i = popIdx;
         popIdx = pushIdx;
         pushIdx = i;

         i = 0;
         while (gImpededStepsQueue[0][i + 0] != PATH_STEP_INVALID) {
            z = gImpededStepsQueue[0][i + 0]; //.z
            x = gImpededStepsQueue[0][i + 1]; //.x
            if (pGrid1[z][x] == PATH_STEP_UNSET) {
               pGrid1[z][x] = gImpededStepsQueue[0][i + 2]; //.step
               if (gImpededStepsQueue[0][i + 3] != 0) {     //.rem
                  buffer[popIdx++] = z;
                  buffer[popIdx++] = x;
               }
            }
            gImpededStepsQueue[0][i] = PATH_STEP_INVALID;
            pendingCount--;
            i += 4;
         }

         popIdx -= 2;
         // popIdx--; popIdx--;
         z = buffer[popIdx + 0];
         x = buffer[popIdx + 1];

         rem--;
         queueOfs++;
         if (queueOfs == 5) {
            queueOfs = 0;
         }
         j = queueOfs;

         for (i = 0; i < 5; i++) {
            if (i + j == 5) {
               j -= 5;
            }
            gImpededStepsQueue[i] = &gImpededSteps[i + j][0];
         }

         if (pendingCount == 0 && z == PATH_STEP_INVALID) {
            if (unit->class == CLASS_AIRMAN) {
               RevertAirmanAdjustments();
               for (i = gMapMinZ; i <= gMapMaxZ; i++) {
                  for (j = gMapMinX; j <= gMapMaxX; j++) {
                     if (gTerrainPtr[i][j].s.terrain == TERRAIN_OBSTACLE) {
                        pGrid1[i][j] = PATH_STEP_UNSET;
                     }
                  }
               }
            }
            return;
         }
      }
   }
}

void PopulateBlueMovementGrid(s16 z, s16 x, s32 range) { PopulateMovementGrid(z, x, range, 10); }

void PopulateMovementGrid(s16 z_, s16 x_, s32 range, s32 gridNum) {
   // FIXME: Fake match (forced register, do-while)

   PathGridRow *pGrid;
   s32 i, j;
   s32 pushIdx;
   s32 popIdx;
   s32 pendingCount;
   s32 elevationCost;
   s32 terrainCost;
   u8 queueOfs;
   u8 team;
   s16 diff;
   u8 stepType;
   UnitStatus *unit;
   u8 buffer[600];
   // s32 rem;
   register s32 rem asm("s7");
   s16 x;
   s16 z;

   rem = range;
   z = z_;
   x = x_;
   pushIdx = 0;
   popIdx = 300;
   pendingCount = 0;

   switch (gridNum) {
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
   case 10:
      pGrid = &gPathGrid10[1];
      break;
   }

   queueOfs = 0;
   unit = &gUnits[gMapUnitsPtr[z][x].s.unitIdx];
   stepType = unit->step;
   team = gMapUnitsPtr[z][x].s.team;

   for (i = 0; i < 5; i++) {
      for (j = 0; j < 200; j++) {
         gImpededSteps[i][j] = PATH_STEP_INVALID;
      }
   }
   for (i = 0; i < 5; i++) {
      gImpededStepsQueue[i] = &gImpededSteps[i][0];
   }

   if (z < gMapMinZ || z > gMapMaxZ || x < gMapMinX || x > gMapMaxX) {
      return;
   }

   buffer[pushIdx++] = PATH_STEP_INVALID;
   pushIdx++; // 2

   buffer[popIdx++] = PATH_STEP_INVALID;
   popIdx++; // 302

   ClearGrid(gridNum);
   pGrid[z][x] = PATH_STEP_INVALID;

   if (rem == 0) {
      return;
   }
   rem--;

   if (unit->class == CLASS_AIRMAN) {
      ApplyAirmanAdjustments(team);
   }

   while (1) {
      while (1) {
         while (z != PATH_STEP_INVALID) {
            // North
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z - 1][x].s.terrain];
            diff = gTerrainPtr[z - 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               do {
                  elevationCost = gTravelDescentCost[stepType][-diff];
               } while (0); // FIXME
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z - 1][x].s.team == 0 || gMapUnitsPtr[z - 1][x].s.team == team) {
               if (gTerrainPtr[z - 1][x].s.terrain >= 0 && pGrid[z - 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     // Not impeded by elevation or terrain; place step onto grid immediately.
                     pGrid[z - 1][x] = PATH_STEP_NORTH;
                     if (rem != 0) {
                        buffer[pushIdx++] = z - 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     // Impeded: defer placement onto the grid (via gImpededStepsQueue)
                     QueueImpededStep(z - 1, x, rem, &pendingCount, terrainCost + elevationCost,
                                      PATH_STEP_NORTH);
                  }
               }
            }

            // South
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z + 1][x].s.terrain];
            diff = gTerrainPtr[z + 1][x].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z + 1][x].s.team == 0 || gMapUnitsPtr[z + 1][x].s.team == team) {
               if (gTerrainPtr[z + 1][x].s.terrain >= 0 && pGrid[z + 1][x] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z + 1][x] = PATH_STEP_SOUTH;
                     if (rem != 0) {
                        buffer[pushIdx++] = z + 1;
                        buffer[pushIdx++] = x;
                     }
                  } else {
                     QueueImpededStep(z + 1, x, rem, &pendingCount, terrainCost + elevationCost,
                                      PATH_STEP_SOUTH);
                  }
               }
            }

            // East
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x - 1].s.terrain];
            diff = gTerrainPtr[z][x - 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x - 1].s.team == 0 || gMapUnitsPtr[z][x - 1].s.team == team) {
               if (gTerrainPtr[z][x - 1].s.terrain >= 0 && pGrid[z][x - 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z][x - 1] = PATH_STEP_EAST;
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x - 1;
                     }
                  } else {
                     QueueImpededStep(z, x - 1, rem, &pendingCount, terrainCost + elevationCost,
                                      PATH_STEP_EAST);
                  }
               }
            }

            // West
            terrainCost = gTravelTerrainCost[stepType][gTerrainPtr[z][x + 1].s.terrain];
            diff = gTerrainPtr[z][x + 1].s.elevation - gTerrainPtr[z][x].s.elevation;
            if (diff < 0) {
               elevationCost = gTravelDescentCost[stepType][-diff];
            } else {
               elevationCost = gTravelAscentCost[stepType][diff];
            }

            if (gMapUnitsPtr[z][x + 1].s.team == 0 || gMapUnitsPtr[z][x + 1].s.team == team) {
               if (gTerrainPtr[z][x + 1].s.terrain >= 0 && pGrid[z][x + 1] == PATH_STEP_UNSET) {
                  if (terrainCost == 0 && elevationCost == 0) {
                     pGrid[z][x + 1] = PATH_STEP_WEST;
                     if (rem != 0) {
                        buffer[pushIdx++] = z;
                        buffer[pushIdx++] = x + 1;
                     }
                  } else {
                     QueueImpededStep(z, x + 1, rem, &pendingCount, terrainCost + elevationCost,
                                      PATH_STEP_WEST);
                  }
               }
            }

            // Pop next coord; If no more are available, fall through to swap push/pop indices.
            popIdx -= 2;
            z = buffer[popIdx + 0];
            x = buffer[popIdx + 1];
         }

         // Pops exhausted; Swap push/pop indices.
         popIdx += 2;
         i = popIdx;
         popIdx = pushIdx;
         pushIdx = i;

         // Adopt any impeded steps that have shifted down to the front of the queue.
         i = 0;
         while (gImpededStepsQueue[0][i + 0] != PATH_STEP_INVALID) {
            z = gImpededStepsQueue[0][i + 0]; //.z
            x = gImpededStepsQueue[0][i + 1]; //.x
            if (pGrid[z][x] == PATH_STEP_UNSET) {
               pGrid[z][x] = gImpededStepsQueue[0][i + 2]; //.step
               if (gImpededStepsQueue[0][i + 3] != 0) {    //.rem
                  buffer[popIdx++] = z;
                  buffer[popIdx++] = x;
               }
            }
            gImpededStepsQueue[0][i] = PATH_STEP_INVALID;
            pendingCount--;
            i += 4;
         }

         popIdx -= 2;
         // popIdx--; popIdx--;
         z = buffer[popIdx + 0];
         x = buffer[popIdx + 1];

         // Having consumed some range, rotate gImpededStepsQueue. Steps in gImpededStepsQueue[0]
         // are ready to be imported. Impeded steps were placed into the queue at the index
         // corresponding to their cost, costing more range to process.
         rem--;
         queueOfs++;
         if (queueOfs == 5) {
            queueOfs = 0;
         }
         j = queueOfs;

         for (i = 0; i < 5; i++) {
            if (i + j == 5) {
               j -= 5;
            }
            gImpededStepsQueue[i] = &gImpededSteps[i + j][0];
         }

         if (pendingCount == 0 && z == PATH_STEP_INVALID) {
            if (unit->class == CLASS_AIRMAN) {
               RevertAirmanAdjustments();
            }
            return;
         }
      }
   }
}

void QueueImpededStep(s32 z, s32 x, s32 rem, s32 *pCount, s16 cost, u8 dir) {
   s32 i;
   u8 *p;

   rem -= cost;

   if (rem >= 0) {
      p = gImpededStepsQueue[cost];

      i = 0;
      while (p[i] != PATH_STEP_INVALID) {
         i += 4;
      }

      // TBD array or struct?
      p[i + 0] = z;
      p[i + 1] = x;
      p[i + 2] = dir;
      p[i + 3] = rem;

      (*pCount)++;
   }
}

void PlotPathBackToUnit(s32 z, s32 x) {
   s32 pathX, pathZ;
   u8 idx;

   pathX = 0;
   pathZ = 0;
   idx = 2;
   gPathBackToUnit[idx++] = PATH_STEP_INVALID;
   gPathBackToUnit[idx++] = PATH_STEP_INVALID;
   gPathBackToUnit[idx++] = z;
   gPathBackToUnit[idx++] = x;

   while (gBlueMovementGridPtr[z][x] != PATH_STEP_INVALID) {

      switch (gBlueMovementGridPtr[z][x]) {
      case PATH_STEP_SOUTH:
         pathZ = z - 1;
         pathX = x;
         break;
      case PATH_STEP_WEST:
         pathZ = z;
         pathX = x - 1;
         break;
      case PATH_STEP_NORTH:
         pathZ = z + 1;
         pathX = x;
         break;
      case PATH_STEP_EAST:
         pathZ = z;
         pathX = x + 1;
         break;
      }

      gPathBackToUnit[idx++] = pathZ;
      gPathBackToUnit[idx++] = pathX;

      x = pathX;
      z = pathZ;
   }

   idx -= 2;
   gPathBackToUnit[idx++] = PATH_STEP_INVALID;
   gPathBackToUnit[idx] = PATH_STEP_INVALID;

   gPathBackToUnit[0] = z;
   gPathBackToUnit[1] = x;
}

void PopulateMeleeAttackGridInDir(s16 attackerZ, s16 attackerX, s16 targetZ, s16 targetX,
                                  s32 gridNum) {
   PathGridRow *pGrid;
   s8 elevationDiff;

   switch (gridNum) {
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

   if ((pGrid[targetZ][targetX] == PATH_STEP_UNSET) &&
       (targetZ >= gMapMinZ && targetZ <= gMapMaxZ && targetX >= gMapMinX && targetX <= gMapMaxX)) {

      elevationDiff =
          gTerrainPtr[targetZ][targetX].s.elevation - gTerrainPtr[attackerZ][attackerX].s.elevation;
      if (elevationDiff < 0) {
         elevationDiff = -elevationDiff;
      }
      if (elevationDiff <= 1) {
         pGrid[targetZ][targetX] = 1;
      }
   }
}

void PopulateMeleeAttackGrid(s16 z, s16 x, s32 gridNum, u8 range) {
   PathGridRow *pGrid;
   s32 iz, ix;

   switch (gridNum) {
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

   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         pGrid[iz][ix] = PATH_STEP_UNSET;
      }
   }

   if (range != 0) {
      PopulateMeleeAttackGridInDir(z, x, z - 1, x, gridNum);
      PopulateMeleeAttackGridInDir(z, x, z, x - 1, gridNum);
      PopulateMeleeAttackGridInDir(z, x, z + 1, x, gridNum);
      PopulateMeleeAttackGridInDir(z, x, z, x + 1, gridNum);
   }
}

void PopulateCastingGrid(s16 z, s16 x, s32 range, s32 gridNum) {
   // FIXME: Fake match (forced register)
   PathGridRow *pGrid;
   s32 i, ix;
   s8 elevationDiff;
   s8 srcElevation;
   s32 pushIdx, popIdx;
   u8 buffer[600];

   pushIdx = 0;
   popIdx = 300;

   if (z >= gMapMinZ && z <= gMapMaxZ && x >= gMapMinX && x <= gMapMaxX) {

      srcElevation = gTerrainPtr[z][x].s.elevation;

      switch (gridNum) {
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

      if (range == 0) {
         ClearGrid(gridNum);
         pGrid[z][x] = 1;
      } else {
         range--;
         buffer[pushIdx] = PATH_STEP_INVALID;
         pushIdx += 2; // 2
         buffer[popIdx] = PATH_STEP_INVALID;
         popIdx += 2; // 302
         ClearGrid(gridNum);
         pGrid[z][x] = 1;

         do {
            // Can't think of a match to make sense of the temps here.
            register u8 *tmp asm("v0"); // Why does this even work??
            s32 tmpRange;

            while (z != PATH_STEP_INVALID) {
               if (pGrid[z - 1][x] == PATH_STEP_UNSET) {
                  tmpRange = range + 1;
                  pGrid[z - 1][x] = range + 1;
                  if (range != 0) {
                     tmp = buffer;
                     buffer[pushIdx++] = z - 1;
                     buffer[pushIdx++] = x;
                  }
               }
               if (pGrid[z + 1][x] == PATH_STEP_UNSET) {
                  tmpRange = range + 1;
                  pGrid[z + 1][x] = range + 1;
                  if (range != 0) {
                     tmp = buffer;
                     buffer[pushIdx++] = z + 1;
                     buffer[pushIdx++] = x;
                  }
               }
               if (pGrid[z][x - 1] == PATH_STEP_UNSET) {
                  tmpRange = range + 1;
                  pGrid[z][x - 1] = range + 1;
                  if (range != 0) {
                     tmp = buffer;
                     buffer[pushIdx++] = z;
                     buffer[pushIdx++] = x - 1;
                  }
               }
               if (pGrid[z][x + 1] == PATH_STEP_UNSET) {
                  tmpRange = range + 1;
                  pGrid[z][x + 1] = range + 1;
                  if (range != 0) {
                     tmp = buffer;
                     buffer[pushIdx++] = z;
                     buffer[pushIdx++] = x + 1;
                  }
               }

               popIdx--;
               popIdx--;

               z = buffer[popIdx + 0];
               x = buffer[popIdx + 1];

            } // inner

            // Swap push/pop indices:
            popIdx += 2;
            i = popIdx;
            popIdx = pushIdx;
            pushIdx = i;

            popIdx--;
            popIdx--;
            z = buffer[popIdx + 0];
            x = buffer[popIdx + 1];

            range--;

         } while (z != PATH_STEP_INVALID);

         // Prune where elevation diff exceeds available range
         for (i = gMapMinZ; i <= gMapMaxZ; i++) {
            for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
               if (pGrid[i][ix] != PATH_STEP_UNSET) {
                  elevationDiff = gTerrainPtr[i][ix].s.elevation - srcElevation;
                  if (elevationDiff < 0) {
                     elevationDiff = -elevationDiff;
                  }
                  if (pGrid[i][ix] < elevationDiff) {
                     pGrid[i][ix] = PATH_STEP_UNSET;
                  }
               }
            }
         }
      }
   }
}

void ClearGrid(s32 gridNum) {
   PathGridRow *pGrid;
   s32 iz, ix;

   switch (gridNum) {
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
   case 10:
      pGrid = &gPathGrid10[1];
      break;
   }

   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         pGrid[iz][ix] = PATH_STEP_UNSET;
      }
   }
}

void ClearBlueMovementGrid(void) {
   s32 iz, ix;
   for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
      for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
         gBlueMovementGridPtr[iz][ix] = PATH_STEP_UNSET;
      }
   }
}

void PopulateRangedAttackGrid(s16 z, s16 x, s32 range, s32 gridNum) {
   PathGridRow *pGrid;
   s32 iz, ix;

   switch (gridNum) {
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

   if (z >= gMapMinZ && z <= gMapMaxZ && x >= gMapMinX && x <= gMapMaxX) {
      for (iz = gMapMinZ; iz <= gMapMaxZ; iz++) {
         for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
            pGrid[iz][ix] = PATH_STEP_UNSET;
         }
      }

      pGrid[z][x] = PATH_STEP_INVALID;

      PopulateRangedAttackGridInDir(z, x, 0, 1, range, gridNum);
      PopulateRangedAttackGridInDir(z, x, 0, -1, range, gridNum);
      PopulateRangedAttackGridInDir(z, x, 1, 0, range, gridNum);
      PopulateRangedAttackGridInDir(z, x, -1, 0, range, gridNum);
   }
}

void NoopForever_8002f9b0(void) {
   while (1) {
   }
}
