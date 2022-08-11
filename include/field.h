#ifndef FIELD_H
#define FIELD_H

#include "common.h"

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
   u8 elevation;
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

extern s16 gTerrainBonus[];
extern TerrainTile (*gTerrainPtr)[65];
extern MapUnit (*gMapUnitsPtr)[65];
extern s16 gMapCursorX, gMapCursorZ;

#endif
