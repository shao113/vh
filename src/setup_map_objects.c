#include "common.h"
#include "evt.h"
#include "units.h"
#include "graphics.h"
#include "state.h"
#include "card.h"

/*typedef struct MapObject {
   s16 evtf;
   s16 z;
   s16 x;
   s16 param; // Item for chests, gfx for trees
} MapObject;*/

typedef s16 MapObject[4];

static MapObject sMapObjects_None[] = {
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map9[] = {
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map0[] = {
    {EVTF_MAP_OBJECT_CHEST, 7, 11, ITEM_NOVA_P_},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map1[] = {
    {EVTF_MAP_OBJECT_LAVA_3, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 8, 11, ITEM_ICE_P_},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map2[] = {
    {EVTF_MAP_OBJECT_CHEST, 1, 1, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 7, 1, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 13, 1, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 19, 1, ITEM_MANA_P_},
    {EVTF_MAP_OBJECT_CHEST, 4, 4, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 10, 4, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 16, 4, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 1, 7, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 7, 7, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 13, 7, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 19, 7, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 4, 10, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 10, 10, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 16, 10, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 1, 13, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 7, 13, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 13, 13, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 19, 13, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 4, 16, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 16, 16, ITEM_NULL},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map3[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CRATE, 14, 12, 0},
    {EVTF_MAP_OBJECT_CRATE, 14, 1, 0},
    {EVTF_MAP_OBJECT_CRATE, 13, 6, 0},
    {EVTF_MAP_OBJECT_CHEST, 1, 6, ITEM_CHAOS_P_},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map4[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 17, 2, ITEM_LOGO_P_},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map5[] = {
    {EVTF_MAP_OBJECT_CHEST, 0, 1, ITEM_HEAVEN_P},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map8[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},      {EVTF_MAP_OBJECT_TREE, 0, 5, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 9, 1, GFX_TREE},  {EVTF_MAP_OBJECT_TREE, 9, 6, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 22, 5, GFX_TREE}, {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map10[] = {
    {EVTF_MAP_OBJECT_TREE, 1, 2, GFX_TREE},   {EVTF_MAP_OBJECT_TREE, 13, 5, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 7, 9, GFX_TREE},   {EVTF_MAP_OBJECT_TREE, 12, 10, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 12, 13, GFX_TREE}, {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map11[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},       {EVTF_MAP_OBJECT_CHEST, 10, 22, ITEM_HERB},
    {EVTF_MAP_OBJECT_CHEST, 1, 1, ITEM_HERB}, {EVTF_MAP_OBJECT_LAMP_POST, 8, 10, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 3, 16, 0},    {EVTF_MAP_OBJECT_LAMP_POST, 7, 17, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 4, 21, 0},    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map12[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 1, 2, ITEM_HERB},
    {EVTF_MAP_OBJECT_CHEST, 16, 9, ITEM_ELIXIR},
    {EVTF_MAP_OBJECT_CRATE, 2, 2, 0},
    {EVTF_MAP_OBJECT_BOULDER, 10, 3, 0},
    {EVTF_MAP_OBJECT_BOULDER, 2, 12, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map13_65[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},       {EVTF_MAP_OBJECT_TREE, 7, 1, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 1, 2, GFX_TREE},   {EVTF_MAP_OBJECT_TREE, 9, 10, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 10, 16, GFX_TREE}, {EVTF_MAP_OBJECT_TREE, 9, 22, GFX_TREE},
    {EVTF_MAP_OBJECT_BOULDER, 10, 7, 0},      {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map14[] = {
    {EVTF_MAP_OBJECT_FLOWING_SAND, 0, 0, 0},      {EVTF_MAP_OBJECT_TREE, 9, 3, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 2, 4, GFX_TREE_BARE},  {EVTF_MAP_OBJECT_TREE, 16, 4, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 7, 8, GFX_TREE_BARE},  {EVTF_MAP_OBJECT_TREE, 15, 8, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 9, 12, GFX_TREE_BARE}, {EVTF_MAP_OBJECT_TREE, 15, 13, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 2, 16, GFX_TREE_BARE}, {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map15[] = {
    {EVTF_MAP_OBJECT_WATER_2, 0, 0, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map16[] = {
    {EVTF_MAP_OBJECT_CRATE, 2, 4, 0},
    {EVTF_MAP_OBJECT_CRATE, 2, 4, 0},
    {EVTF_MAP_OBJECT_CHEST, 5, 1, ITEM_MAGE_OIL},
    {EVTF_MAP_OBJECT_CHEST, 5, 14, ITEM_HERB},
    {EVTF_MAP_OBJECT_CRATE, 9, 15, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map17[] = {
    {EVTF_MAP_OBJECT_CRATE, 16, 2, 0},
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 15, 1, ITEM_MOODRING},
    {EVTF_MAP_OBJECT_CHEST, 1, 1, ITEM_ELIXIR},
    {EVTF_MAP_OBJECT_BOULDER, 3, 6, 0},
    {EVTF_MAP_OBJECT_BOULDER, 15, 17, 0},
    {EVTF_MAP_OBJECT_TREE, 2, 2, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 1, 3, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 12, 3, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 1, 14, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 16, 16, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 12, 19, GFX_TREE_PALM},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map18_66[] = {
    {EVTF_MAP_OBJECT_VILE_BOG, 0, 0, 0},
    {EVTF_MAP_OBJECT_TREE, 7, 2, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 26, 1, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 20, 4, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 20, 10, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 11, 11, GFX_TREE_BARE},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map19[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_BOULDER, 3, 14, 0},
    {EVTF_MAP_OBJECT_BOULDER, 12, 14, 0},
    {EVTF_MAP_OBJECT_CHEST, 1, 16, ITEM_MOODRING},
    {EVTF_MAP_OBJECT_CHEST, 14, 16, ITEM_MAGE_OIL},
    {EVTF_MAP_OBJECT_TREE, 1, 12, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 5, 10, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 4, 22, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 11, 29, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 13, 21, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 14, 13, GFX_TREE_PALM},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map21_67[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_TREE, 4, 4, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 18, 4, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 4, 9, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 15, 12, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 4, 15, GFX_TREE_PALM},
    {EVTF_MAP_OBJECT_TREE, 9, 15, GFX_TREE_PALM},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map22[] = {
    {EVTF_MAP_OBJECT_LAVA_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_GRAVE_MARKER, 8, 8, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map23[] = {
    {EVTF_MAP_OBJECT_LAVA_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_GRAVE_MARKER, 8, 8, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map24[] = {
    {EVTF_MAP_OBJECT_CHEST, 0, 1, ITEM_MEGAHERB},
    {EVTF_MAP_OBJECT_CHEST, 16, 17, ITEM_ELIXIR},
    {EVTF_MAP_OBJECT_BOULDER, 16, 2, 0},
    {EVTF_MAP_OBJECT_BOULDER, 16, 16, 0},
    {EVTF_MAP_OBJECT_BOULDER, 6, 5, 0},
    {EVTF_MAP_OBJECT_CRATE, 3, 2, 0},
    {EVTF_MAP_OBJECT_CRATE, 3, 2, 0},
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map25[] = {
    {EVTF_MAP_OBJECT_CHEST, 1, 1, ITEM_MAGE_GEM}, {EVTF_MAP_OBJECT_CHEST, 15, 6, ITEM_MEGAHERB},
    {EVTF_MAP_OBJECT_BOULDER, 1, 7, 0},           {EVTF_MAP_OBJECT_BOULDER, 15, 7, 0},
    {EVTF_MAP_OBJECT_BOULDER, 12, 12, 0},         {EVTF_MAP_OBJECT_BOULDER, 13, 4, 0},
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},           {EVTF_MAP_OBJECT_TREE, 18, 2, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 5, 4, GFX_TREE},       {EVTF_MAP_OBJECT_TREE, 6, 9, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 18, 12, GFX_TREE},     {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map26[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CRATE, 4, 42, 0},
    {EVTF_MAP_OBJECT_TREE, 4, 2, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 7, 6, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 12, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 2, 13, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 4, 13, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 3, 15, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 3, 17, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 2, 18, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 21, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 7, 22, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 24, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 1, 32, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 11, 32, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 1, 35, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 43, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 1, 46, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 4, 46, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 3, 14, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 2, 16, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 7, 19, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 20, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 7, 23, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 8, 25, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 9, 30, GFX_TREE},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map27[] = {
    {EVTF_MAP_OBJECT_TORCH, 5, 7, 0},
    {EVTF_MAP_OBJECT_TORCH, 10, 15, 0},
    {EVTF_MAP_OBJECT_CHEST, 7, 3, ITEM_MEGAHERB},
    {EVTF_MAP_OBJECT_CHEST, 14, 1, ITEM_GRIMHELM},
    {EVTF_MAP_OBJECT_CHEST, 3, 6, ITEM_S_STAFF},
    {EVTF_MAP_OBJECT_CHEST, 3, 8, ITEM_NULL},
    {EVTF_MAP_OBJECT_CHEST, 3, 14, ITEM_MEGAHERB},
    {EVTF_MAP_OBJECT_CHEST, 4, 16, ITEM_NULL},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map28[] = {
    {EVTF_MAP_OBJECT_BOULDER, 18, 11, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map29[] = {
    {EVTF_MAP_OBJECT_CHEST, 6, 1, ITEM_AURA_GEM}, {EVTF_MAP_OBJECT_CHEST, 12, 1, ITEM_HOLY_H2O},
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},           {EVTF_MAP_OBJECT_CRATE, 8, 6, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 2, 5, 0},         {EVTF_MAP_OBJECT_LAMP_POST, 11, 7, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 19, 8, 0},        {EVTF_MAP_OBJECT_LAMP_POST, 22, 8, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 19, 11, 0},       {EVTF_MAP_OBJECT_LAMP_POST, 22, 11, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 11, 12, 0},       {EVTF_MAP_OBJECT_LAMP_POST, 1, 14, 0},
    {EVTF_MAP_OBJECT_FOUNTAIN, 3, 17, 0},         {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map30[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_BOULDER, 3, 6, 0},
    {EVTF_MAP_OBJECT_BOULDER, 21, 4, 0},
    {EVTF_MAP_OBJECT_TREE, 1, 1, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 6, 1, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 12, 0, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 11, 3, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 22, 5, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 18, 7, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 10, 10, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 22, 9, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 2, 11, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 6, 12, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 17, 11, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 18, 13, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 22, 13, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 0, 15, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 7, 16, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 17, 16, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 22, 17, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 19, 18, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 9, 19, GFX_TREE_EVERGREEN},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map32[] = {
    {EVTF_MAP_OBJECT_RAIL, 0, 0, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map33_68[] = {
    {EVTF_MAP_OBJECT_LAVA_2, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 5, 1, ITEM_L_MASK},
    {EVTF_MAP_OBJECT_CHEST, 21, 19, ITEM_LIFE_ORB},
    {EVTF_MAP_OBJECT_TREE, 1, 2, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 6, 3, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 13, 3, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 17, 1, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 20, 6, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 1, 8, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 18, 14, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 3, 16, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 7, 21, GFX_TREE_BARE},
    {EVTF_MAP_OBJECT_TREE, 18, 21, GFX_TREE_BARE},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map34[] = {
    {EVTF_MAP_OBJECT_WATER_2, 0, 0, 0},
    {EVTF_MAP_OBJECT_BOULDER, 20, 13, 0},
    {EVTF_MAP_OBJECT_CHEST, 1, 4, ITEM_PLATE},
    {EVTF_MAP_OBJECT_CHEST, 1, 13, ITEM_NULL},
    {EVTF_MAP_OBJECT_CRATE, 18, 7, 0},
    {EVTF_MAP_OBJECT_TREE, 0, 3, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 2, 9, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 2, 14, GFX_TREE_EVERGREEN},
    {EVTF_MAP_OBJECT_TREE, 0, 17, GFX_TREE_EVERGREEN},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map35[] = {
    {EVTF_MAP_OBJECT_FLOWING_WATER, 0, 0, 0},       {EVTF_MAP_OBJECT_TREE, 1, 1, GFX_TREE_SNOWY},
    {EVTF_MAP_OBJECT_TREE, 13, 4, GFX_TREE_SNOWY},  {EVTF_MAP_OBJECT_TREE, 4, 5, GFX_TREE_SNOWY},
    {EVTF_MAP_OBJECT_TREE, 10, 6, GFX_TREE_SNOWY},  {EVTF_MAP_OBJECT_TREE, 14, 8, GFX_TREE_SNOWY},
    {EVTF_MAP_OBJECT_TREE, 3, 18, GFX_TREE_SNOWY},  {EVTF_MAP_OBJECT_TREE, 12, 18, GFX_TREE_SNOWY},
    {EVTF_MAP_OBJECT_TREE, 11, 22, GFX_TREE_SNOWY}, {EVTF_MAP_OBJECT_TREE, 2, 25, GFX_TREE_SNOWY},
    {EVTF_MAP_OBJECT_TREE, 8, 28, GFX_TREE_SNOWY},  {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map36[] = {
    {EVTF_MAP_OBJECT_CHEST, 11, 11, ITEM_NULL},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map37[] = {
    {EVTF_MAP_OBJECT_LAVA_3, 0, 0, 0},
    {EVTF_MAP_OBJECT_CRATE, 14, 10, 0},
    {EVTF_MAP_OBJECT_CRATE, 14, 10, 0},
    {EVTF_MAP_OBJECT_CRATE, 14, 10, 0},
    {EVTF_MAP_OBJECT_CRATE, 16, 11, 0},
    {EVTF_MAP_OBJECT_CHEST, 13, 10, ITEM_D_SPEAR},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map38[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_BOULDER, 1, 28, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map39[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_CHEST, 12, 9, ITEM_WYRMFANG},
    {EVTF_MAP_OBJECT_CHEST, 0, 6, ITEM_GOLD_AXE},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map40[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},  {EVTF_MAP_OBJECT_BOULDER, 6, 22, 0},
    {EVTF_MAP_OBJECT_BOULDER, 7, 22, 0}, {EVTF_MAP_OBJECT_FLAG, 2, 2, 0},
    {EVTF_MAP_OBJECT_FLAG, 9, 4, 0},     {EVTF_MAP_OBJECT_FLAG, 9, 8, 0},
    {EVTF_MAP_OBJECT_FLAG, 12, 12, 0},   {EVTF_MAP_OBJECT_FLAG, 8, 25, 0},
    {EVTF_MAP_OBJECT_FLAG, 5, 25, 0},    {EVTF_MAP_OBJECT_FLAG, 0, 33, 0},
    {EVTF_MAP_OBJECT_FLAG, 13, 33, 0},   {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map41[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},           {EVTF_MAP_OBJECT_CHEST, 2, 3, ITEM_RAGNAROK},
    {EVTF_MAP_OBJECT_CHEST, 8, 5, ITEM_HOLY_H2O}, {EVTF_MAP_OBJECT_CHEST, 13, 20, ITEM_LIFE_ORB},
    {EVTF_MAP_OBJECT_BOULDER, 10, 11, 0},         {EVTF_MAP_OBJECT_BOULDER, 11, 11, 0},
    {EVTF_MAP_OBJECT_BOULDER, 6, 12, 0},          {EVTF_MAP_OBJECT_BOULDER, 9, 13, 0},
    {EVTF_MAP_OBJECT_BOULDER, 4, 14, 0},          {EVTF_MAP_OBJECT_BOULDER, 7, 14, 0},
    {EVTF_MAP_OBJECT_TREE, 6, 1, GFX_TREE},       {EVTF_MAP_OBJECT_TREE, 9, 3, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 5, 7, GFX_TREE},       {EVTF_MAP_OBJECT_TREE, 13, 10, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 11, 16, GFX_TREE},     {EVTF_MAP_OBJECT_TREE, 3, 17, GFX_TREE},
    {EVTF_MAP_OBJECT_TREE, 5, 20, GFX_TREE},      {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map42[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_MAP_OBJECT_FOUNTAIN, 8, 12, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 5, 6, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 15, 5, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 6, 19, 0},
    {EVTF_MAP_OBJECT_LAMP_POST, 12, 19, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map43[] = {
    {EVTF_MAP_OBJECT_TORCH, 6, 2, 0},
    {EVTF_MAP_OBJECT_TORCH, 10, 2, 0},
    {EVTF_MAP_OBJECT_TORCH, 3, 8, 0},
    {EVTF_MAP_OBJECT_TORCH, 13, 8, 0},
    {EVTF_MAP_OBJECT_TORCH, 3, 22, 0},
    {EVTF_MAP_OBJECT_TORCH, 13, 22, 0},
    {EVTF_NULL, 0, 0, 0},
};

static MapObject sMapObjects_Map63_64[] = {
    {EVTF_MAP_OBJECT_WATER_1, 0, 0, 0},
    {EVTF_NULL, 0, 0, 0},
};

void Evtf043_SetupMapObjects(EvtData *evt) {
   s16 *p;
   EvtData *newEvt;
   s32 i;

   switch (gState.mapNum) {
   case 0:
      p = sMapObjects_Map0[0];
      break;
   case 1:
      p = sMapObjects_Map1[0];
      break;
   case 2:
      p = sMapObjects_Map2[0];
      break;
   case 3:
      p = sMapObjects_Map3[0];
      break;
   case 4:
      p = sMapObjects_Map4[0];
      break;
   case 5:
      p = sMapObjects_Map5[0];
      break;
   case 8:
      p = sMapObjects_Map8[0];
      break;
   case 9:
      p = sMapObjects_Map9[0];
      break;
   case 10:
      p = sMapObjects_Map10[0];
      break;
   case 11:
      p = sMapObjects_Map11[0];
      break;
   case 12:
      p = sMapObjects_Map12[0];
      break;
   case 13:
   case 65:
      p = sMapObjects_Map13_65[0];
      break;
   case 14:
      p = sMapObjects_Map14[0];
      break;
   case 15:
      p = sMapObjects_Map15[0];
      break;
   case 16:
      p = sMapObjects_Map16[0];
      break;
   case 17:
      p = sMapObjects_Map17[0];
      break;
   case 18:
   case 66:
      p = sMapObjects_Map18_66[0];
      break;
   case 19:
      p = sMapObjects_Map19[0];
      break;
   case 21:
   case 67:
      p = sMapObjects_Map21_67[0];
      break;
   case 22:
      p = sMapObjects_Map22[0];
      break;
   case 23:
      p = sMapObjects_Map23[0];
      break;
   case 24:
      p = sMapObjects_Map24[0];
      break;
   case 25:
      p = sMapObjects_Map25[0];
      break;
   case 26:
      p = sMapObjects_Map26[0];
      break;
   case 27:
      p = sMapObjects_Map27[0];
      break;
   case 28:
      p = sMapObjects_Map28[0];
      break;
   case 29:
      p = sMapObjects_Map29[0];
      break;
   case 30:
      p = sMapObjects_Map30[0];
      break;
   case 32:
      p = sMapObjects_Map32[0];
      break;
   case 33:
   case 68:
      p = sMapObjects_Map33_68[0];
      break;
   case 34:
      p = sMapObjects_Map34[0];
      break;
   case 35:
      p = sMapObjects_Map35[0];
      break;
   case 36:
      p = sMapObjects_Map36[0];
      break;
   case 37:
      p = sMapObjects_Map37[0];
      break;
   case 38:
      p = sMapObjects_Map38[0];
      break;
   case 39:
      p = sMapObjects_Map39[0];
      break;
   case 40:
      p = sMapObjects_Map40[0];
      break;
   case 41:
      p = sMapObjects_Map41[0];
      break;
   case 42:
      p = sMapObjects_Map42[0];
      break;
   case 43:
      p = sMapObjects_Map43[0];
      break;
   case 63:
   case 64:
      p = sMapObjects_Map63_64[0];
      break;
   default:
      p = sMapObjects_None[0];
      break;
   }

   while (*p != EVTF_NULL) {
      newEvt = Evt_GetUnused();
      newEvt->functionIndex = *p++;
      newEvt->z1.s.hi = *p++;
      newEvt->x1.s.hi = *p++;
      newEvt->d.mapObj.param = *p++;

      if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
         i = newEvt->functionIndex;
         if (i == EVTF_MAP_OBJECT_CHEST || i == EVTF_MAP_OBJECT_CRATE ||
             i == EVTF_MAP_OBJECT_BOULDER) {
            newEvt->functionIndex = EVTF_NULL;
         }
      }

      if (gState.primary == STATE_4 || gState.primary == STATE_25 || gState.primary == STATE_16 ||
          gState.primary == STATE_17) {
         i = newEvt->functionIndex;
         if ((gState.scene == 12 || gState.scene == 13) && i == EVTF_MAP_OBJECT_BOULDER) {
            newEvt->functionIndex = EVTF_NULL;
         }
         if (gState.scene == 9 && (i == EVTF_MAP_OBJECT_CHEST || i == EVTF_MAP_OBJECT_CRATE ||
                                   i == EVTF_MAP_OBJECT_BOULDER)) {
            newEvt->functionIndex = EVTF_NULL;
         }
         if (gState.scene == 50 && i == EVTF_MAP_OBJECT_BOULDER) {
            newEvt->functionIndex = EVTF_NULL;
         }
         if (gState.scene == 75 && i == EVTF_MAP_OBJECT_CHEST) {
            newEvt->functionIndex = EVTF_NULL;
         }
         if (gState.scene == 35 && i == EVTF_MAP_OBJECT_GRAVE_MARKER) {
            newEvt->functionIndex = EVTF_NULL;
         }
         if (gState.scene == 7 && (i == EVTF_MAP_OBJECT_CHEST || i == EVTF_MAP_OBJECT_CRATE)) {
            newEvt->functionIndex = EVTF_NULL;
         }
      }
   }

   if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
      for (i = 0; i < ARRAY_COUNT(gDeferredInBattleSaveData.chests); i++) {
         if (gDeferredInBattleSaveData.chests[i].z != 99) {
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_MAP_OBJECT_CHEST;
            newEvt->z1.s.hi = gDeferredInBattleSaveData.chests[i].z;
            newEvt->x1.s.hi = gDeferredInBattleSaveData.chests[i].x;
            newEvt->d.mapObj.param = gDeferredInBattleSaveData.chests[i].item;
         }
      }
      for (i = 0; i < ARRAY_COUNT(gDeferredInBattleSaveData.crates); i++) {
         if (gDeferredInBattleSaveData.crates[i].z != 99) {
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_MAP_OBJECT_CRATE;
            newEvt->z1.s.hi = gDeferredInBattleSaveData.crates[i].z;
            newEvt->x1.s.hi = gDeferredInBattleSaveData.crates[i].x;
         }
      }
      for (i = 0; i < ARRAY_COUNT(gDeferredInBattleSaveData.boulders); i++) {
         if (gDeferredInBattleSaveData.boulders[i].z != 99) {
            newEvt = Evt_GetUnused();
            newEvt->functionIndex = EVTF_MAP_OBJECT_BOULDER;
            newEvt->z1.s.hi = gDeferredInBattleSaveData.boulders[i].z;
            newEvt->x1.s.hi = gDeferredInBattleSaveData.boulders[i].x;
         }
      }
   }

   evt->functionIndex = EVTF_NULL;
}
