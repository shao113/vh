#ifndef FIELD_H
#define FIELD_H

#include "common.h"
#include "graphics.h"

typedef enum TerrainType {
   TERRAIN_PLAINS = 0,
   TERRAIN_PRAIRIE = 1,
   TERRAIN_THICKET = 2,
   TERRAIN_BARREN = 3,
   TERRAIN_WATER = 4,
   TERRAIN_VILE_BOG = 5,
   TERRAIN_LAVA = 6,
   TERRAIN_BOUNDARY = 7,
   TERRAIN_OBSTACLE = 8,
   TERRAIN_NO_ENTRY = 9
} TerrainType;

typedef struct TerrainTile {
   s8 elevation;
   s8 terrain;
} TerrainTile;

/* Crates, Boulders, ... */
typedef struct MapObject2 {
   s8 z;
   s8 x;
} MapObject2;

/* Chests, ... */
typedef struct MapObject3 {
   s8 z;
   s8 x;
   s8 item;
} MapObject3;

typedef struct MapUnit {
   s8 unitIdx;
   s8 team;
} MapUnit;

typedef struct MapTileModel {
   SVECTOR vertices[22];
   s16 gfx[18];
   s8 faces[18][4];
   s8 shades[18]; // TBD
   s8 faceCt;
   u8 height;
} MapTileModel;

typedef enum PathStep {
   PATH_STEP_UNSET = 0x0,
   PATH_STEP_SOUTH = 0x1,
   PATH_STEP_WEST = 0x2,
   PATH_STEP_NORTH = 0x3,
   PATH_STEP_EAST = 0x4,
   PATH_STEP_INVALID = 0xff
} PathStep;

// ?: a PathGrid can contain PathStep (direction), num steps from start, ...
typedef u8 PathGridRow[65];
extern PathGridRow gPathGrid0[30];
extern PathGridRow gPathGrid1[30];
// extern PathGridRow gPathGrid0_1[29];
// extern PathGridRow gPathGrid1_1[29];
extern PathGridRow gPathGrid2[30];
extern PathGridRow gPathGrid3[30];
extern PathGridRow gPathGrid4[30];
extern PathGridRow gPathGrid5[30];
extern PathGridRow gPathGrid6[30];

extern s16 gTerrainBonus[];
extern TerrainTile (*gTerrainPtr)[65];
extern MapUnit (*gMapUnitsPtr)[65];
extern u8 *gMapDataPtr;
extern MapTileModel *gMapRowPointers[28];
extern s16 gMapCursorX, gMapCursorZ;
extern s16 gMapMinX, gMapMinZ, gMapMaxX, gMapMaxZ;
extern s16 gMapSizeX, gMapSizeZ;
extern u8 gOverheadMapState;
extern BVectorZXY gMapCursorStartingPos[BATTLE_CT];

extern PathGridRow *gRedAttackGridPtr;
extern PathGridRow *gYellowTargetGridPtr;
extern PathGridRow *gBlueMovementGridPtr;

#endif
