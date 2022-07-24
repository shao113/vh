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

typedef struct MapState {
   u32 battleEval;
   u8 needEval;
   u8 field2_0x5;
   u8 field3_0x6;
   u8 field4_0x7;
   u8 field5_0x8;
   u8 field6_0x9;
   u8 field7_0xa;
   u8 field8_0xb;
   u8 field9_0xc;
   u8 field10_0xd;
   u8 field11_0xe;
   u8 field12_0xf;
   u8 foundHiddenItem1;
   u8 foundHiddenItem2;
   u8 field15_0x12;
   u8 field16_0x13;
   u8 field17_0x14;
   u8 field18_0x15;
   u8 field19_0x16;
   u8 field20_0x17;
   u8 field21_0x18;
   u8 signal;
   u8 field23_0x1a;
   u8 preciseSprites;
   u8 field25_0x1c;
   u8 msgFinished;
   u8 searchX;
   u8 searchZ;
} MapState;

extern s16 gTerrainBonus[];
extern TerrainTile (*gTerrainPtr)[65];
extern MapState gMapState;

#endif
