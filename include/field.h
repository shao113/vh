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

typedef struct MapTileModel {
   SVECTOR vertices[22];
   s16 gfx[18];
   s8 faces[18][4];
   s8 shades[18]; // TBD
   s8 faceCt;
   u8 height;
} MapTileModel;

extern s16 gTerrainBonus[];
extern TerrainTile (*gTerrainPtr)[65];
extern MapUnit (*gMapUnitsPtr)[65];
extern u8 *gMapDataPtr;
extern MapTileModel *gMapRowPointers[28];
extern s16 gMapCursorX, gMapCursorZ;
extern s16 gMapMinX, gMapMinZ, gMapMaxX, gMapMaxZ;
extern u8 gOverheadMapState;
extern BVectorZXY gMapCursorStartingPos[BATTLE_CT];

#endif
