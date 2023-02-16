#include "common.h"
#include "battle.h"
#include "units.h"
#include "field.h"

// This file should probably be merged with the preceding functions (starting from func_8002ADCC?)
// into something like path_grids.c, once I tackle the missing path grid functions.

void QueueImpededStep(s16, s16, s32, s32 *, s16, u8);
void PlotPathBackToUnit(s32, s32);
void PopulateMeleeAttackGridInDir(s16, s16, s16, s16, s32);
void PopulateMeleeAttackGrid(s16, s16, s32, u8);
void PopulateCastingGrid(s16, s16, s32, s32);
void ClearGrid(s32);
void ClearBlueMovementGrid(void);
void PopulateRangedAttackGrid(s16, s16, s32, s32);
void NoopForever_8002f9b0(void);

void QueueImpededStep(s16 z, s16 x, s32 rem, s32 *pCount, s16 imped, u8 dir) {
   s32 i;
   u8 *p;
   ImpededStep *pStep;

   rem -= imped;

   if (rem >= 0) {
      p = (u8 *)gImpededStepsQueue[imped];

      i = 0;
      while (p[i] != PATH_STEP_INVALID) {
         i += sizeof(ImpededStep);
      }

      // TBD array or struct?
      pStep = (ImpededStep *)&p[i];
      pStep->z = z;
      pStep->x = x;
      pStep->step = dir;
      pStep->rem = rem;

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
          gTerrainPtr[targetZ][targetX].elevation - gTerrainPtr[attackerZ][attackerX].elevation;
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

      srcElevation = gTerrainPtr[z][x].elevation;

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
                  elevationDiff = gTerrainPtr[i][ix].elevation - srcElevation;
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
