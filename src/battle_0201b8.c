#include "common.h"
#include "evt.h"
#include "battle.h"
#include "units.h"
#include "state.h"
#include "field.h"
#include "window.h"
#include "graphics.h"

typedef struct EnemyEventSpawn {
   u8 z, x, stripIdx, level, expMulti;
} EnemyEventSpawn;

typedef struct OromeLakeEnemySpawn {
   u8 z, x, stripIdx, level;
} OromeLakeEnemySpawn;

extern MATRIX gMatrix_8012c068;

HiddenItem gMapHiddenItems[BATTLE_CT][2] = {
    [0] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [1] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [2] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [3] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [4] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [5] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [6] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [7] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [8] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [9] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [10] = {{0, 1, ITEM_MAD_BOOK}, {14, 0, ITEM_L_SWORD}},
    [11] = {{11, 11, ITEM_MUSHROOM}, {8, 19, ITEM_L_BOW}},
    [12] = {{17, 3, ITEM_FIRE_GEM}, {99, 99, ITEM_NULL}},
    [13] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [14] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [15] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [16] = {{0, 12, ITEM_MUSHROOM}, {9, 13, ITEM_MOON_PIE}},
    [17] = {{17, 22, ITEM_MAD_BOOK}, {0, 22, ITEM_MACROMAN}},
    [18] = {{26, 5, ITEM_BANANA}, {99, 99, ITEM_NULL}},
    [19] = {{8, 24, ITEM_IRONBOOT}, {0, 8, ITEM_CHAIN}},
    [20] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [21] = {{2, 0, ITEM_GOLDCOIN}, {19, 0, ITEM_MAGE_GEM}},
    [22] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [23] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [24] = {{1, 10, ITEM_GOLDCOIN}, {10, 23, ITEM_UNICORN}},
    [25] = {{3, 8, ITEM_IRONBOOT}, {17, 1, ITEM_TAROT}},
    [26] = {{6, 1, ITEM_M_STAFF}, {99, 99, ITEM_NULL}},
    [27] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [28] = {{20, 7, ITEM_GOLDCOIN}, {6, 16, ITEM_MANA_KEY}},
    [29] = {{1, 6, ITEM_GOLD_AXE}, {99, 99, ITEM_NULL}},
    [30] = {{1, 2, ITEM_KINGFOIL}, {18, 8, ITEM_AURA_GEM}},
    [31] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [32] = {{4, 52, ITEM_RAMEN}, {99, 99, ITEM_NULL}},
    [33] = {{1, 13, ITEM_KINGFOIL}, {21, 4, ITEM_MITHRIL}},
    [34] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [35] = {{3, 2, ITEM_HELSTONE}, {8, 31, ITEM_MITHRIL}},
    [36] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [37] = {{6, 8, ITEM_LOGOS_K_}, {99, 99, ITEM_NULL}},
    [38] = {{10, 0, ITEM_SHIVBOOK}, {14, 21, ITEM_MITHRIL}},
    [39] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [40] = {{5, 3, ITEM_NCKLACE}, {99, 99, ITEM_NULL}},
    [41] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}},
    [42] = {{12, 5, ITEM_GOLD_AXE}, {9, 18, ITEM_MOJOROBE}},
    [43] = {{99, 99, ITEM_NULL}, {99, 99, ITEM_NULL}}};

static EnemyEventSpawn sEnemyEventSpawns[6] = {{2, 19, 17, 29, 20}, {4, 19, 18, 25, 12},
                                               {3, 18, 18, 25, 12}, {3, 20, 18, 25, 12},
                                               {1, 18, 19, 24, 12}, {1, 20, 19, 24, 12}};

static OromeLakeEnemySpawn sOromeLakeEnemySpawns1[7] = {
    {6, 13, 13, 23}, {7, 13, 13, 23}, {3, 20, 12, 24}, {2, 19, 12, 24},
    {2, 21, 12, 24}, {1, 20, 12, 24}, {0, 0, 0, 0}};

static OromeLakeEnemySpawn sOromeLakeEnemySpawns2[3] = {
    {9, 11, 12, 24}, {7, 10, 12, 24}, {0, 0, 0, 0}};

void ShowReceivedItemDialog(u8 item, u8 windowId, u8 param_3) {
   s32 len;

   gClearSavedPadState = 1;
   len = GetItemNameLength(item);

   DrawWindow(windowId, 0, 0, 240, 36, 40, 90, WBS_CROSSED, 0);
   DrawText(80, 11, 25, 2, 0, gItemNames[item]);
   DrawText(80 + len * 8, 11, 25, 2, 0, "!");
   DrawText(16, 11, 25, 2, 0, "you got ");

   // This references param_3 just enough to get the prologue/epilogue correct without otherwise
   // affecting codegen
   if (param_3) {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 0);
   } else {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 0);
   }
}

void ShowDepotReceivedItemDialog(u8 item, u8 windowId, u8 param_3) {
   s32 len;

   gClearSavedPadState = 1;
   len = GetItemNameLength(item);

   DrawWindow(windowId, 0, 0, 240, 72, 40, 90, WBS_CROSSED, 0);
   DrawText(80, 11, 25, 2, 0, gItemNames[item]);
   DrawText(80 + len * 8, 11, 25, 2, 0, "!");
   DrawText(16, 11, 25, 2, 0, "you got ");
   DrawText(16, 28, 25, 2, 0, "Can't hold anymore.");
   DrawText(16, 45, 25, 2, 0, "Sent to supply wagon.");

   if (param_3) {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 0);
   } else {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 0);
   }
}

u8 s_padHoldCounter_801231cc;

void UpdateInput(void) {
   static u32 previousPadState = 0;
   static u32 previousPad2State = 0;

   previousPadState = gPadState;
   gPadState = PadRead(0);

   if (previousPadState == gPadState) {
      s_padHoldCounter_801231cc++;
   } else {
      s_padHoldCounter_801231cc = 0;
   }

   gPadStateNewPresses = (previousPadState ^ gPadState) & gPadState;

   if (gState.vsyncMode == 2) {
      if (s_padHoldCounter_801231cc == 10) {
         s_padHoldCounter_801231cc = 8;
         gPadStateNewPresses = gPadState & PAD_DPAD;
      }
   } else {
      if (s_padHoldCounter_801231cc == 20) {
         s_padHoldCounter_801231cc = 17;
         gPadStateNewPresses = gPadState & PAD_DPAD;
      }
   }

   previousPad2State = gPad2State;

   gPad2State = PadRead(0) >> 0x10;
   gPad2StateNewPresses = (previousPad2State ^ gPad2State) & gPad2State;

   if (gClearSavedPadState == 0) {
      gSavedPadState = gPadState;
      gSavedPadStateNewPresses = gPadStateNewPresses;
      gSavedPad2State = gPad2State;
      gSavedPad2StateNewPresses = gPad2StateNewPresses;
   } else {
      gSavedPadState = 0;
      gSavedPadStateNewPresses = 0;
      gSavedPad2State = 0;
      gSavedPad2StateNewPresses = 0;
   }
}

s32 D_801231D0;
s32 D_801231D4;

void CenterCamera(u8 cursor) {
   s32 dstX, dstZ;

   if ((!gPlayerControlSuppressed && !gIsEnemyTurn && !gMapCursorSuppressed) || cursor) {

      if (gMapCursorX > (gMapMaxX - 1) - gMapMarginX) {
         D_801231D0 = (gMapMaxX - gMapCursorX) - gMapMarginX;
      }
      if (gMapCursorX < gMapMinX + gMapMarginX + 1) {
         D_801231D0 = (gMapMinX + gMapMarginX) - gMapCursorX;
      }
      if (gMapCursorZ > (gMapMaxZ - 1) - gMapMarginZ) {
         D_801231D4 = (gMapMaxZ - gMapCursorZ) - gMapMarginZ;
      }
      if (gMapCursorZ < gMapMinZ + gMapMarginZ + 1) {
         D_801231D4 = (gMapMinZ + gMapMarginZ) - gMapCursorZ;
      }

      dstX = (-D_80122E28 - (gMapSizeX / 2) + D_801231D0) * 32;
      dstZ = (-D_80122E2C - (gMapSizeZ / 2) + D_801231D4) * 32;

      gCameraPos.vx += (dstX - gCameraPos.vx) >> 2;
      gCameraPos.vz += (dstZ - gCameraPos.vz) >> 2;
   } else {
      D_80122E28 = ((-gMapSizeX / 2) * 32 - gCameraPos.vx) / 32;
      D_80122E2C = ((-gMapSizeZ / 2) * 32 - gCameraPos.vz) / 32;

      D_80122E28 = CLAMP(D_80122E28, gMapMinX, (gMapMaxX + 1) - gMapSizeX);
      D_80122E2C = CLAMP(D_80122E2C, gMapMinZ, (gMapMaxZ + 1) - gMapSizeZ);
   }
}

void UpdateCamera(void) {
   if (gPlayerControlSuppressed || gIsEnemyTurn || gMapCursorSuppressed) {
      UpdateNonPlayerCamera();
   } else {
      UpdatePlayerCamera();
   }
}

void UpdatePlayerCamera(void) {
   static s16 mapYRotationSpeed = 0;
   static s16 mapXRotationSpeed = 0;
   s32 tmp;

   if (gSavedPadStateNewPresses & PAD_L2) {
      mapXRotationSpeed = -0x20;
   }
   if (gSavedPadStateNewPresses & PAD_R2) {
      mapXRotationSpeed = 0x20;
   }

   gCameraRotation.vx += 0x80;
   gCameraRotation.vx += mapXRotationSpeed;
   if ((gCameraRotation.vx & 0xff) == 0) {
      mapXRotationSpeed = 0;
   }

   gCameraRotation.vx -= 0x80;
   gCameraRotation.vx = CLAMP(gCameraRotation.vx, 0x80, 0x380);

   gCameraRotation.vy += ANGLE_45_DEGREES;
   gCameraRotation.vy += mapYRotationSpeed;
   if ((gCameraRotation.vy & 0x3ff) == 0) {
      mapYRotationSpeed = 0;
   }
   gCameraRotation.vy -= ANGLE_45_DEGREES;

   if (gSavedPadState & PAD_R1) {
      mapYRotationSpeed = 0x80;
   }
   if (gSavedPadState & PAD_L1) {
      mapYRotationSpeed = -0x80;
   }

   if (gMapCursorX <= gMapMarginX + D_80122E28) {
      D_80122E28 = gMapCursorX - gMapMarginX;
   }
   if (gMapCursorZ <= gMapMarginZ + D_80122E2C) {
      D_80122E2C = gMapCursorZ - gMapMarginZ;
   }
   // Odd use of temps here; better match? (tmp = 1 also works)
   if (gMapCursorX >= (gMapSizeX + D_80122E28) - (tmp = gMapMarginX + 1)) {
      D_80122E28 = (gMapCursorX - gMapSizeX) + tmp;
   }
   if (gMapCursorZ >= (gMapSizeZ + D_80122E2C) - (tmp = gMapMarginZ + 1)) {
      D_80122E2C = (gMapCursorZ - gMapSizeZ) + tmp;
   }

   D_80122E28 = CLAMP(D_80122E28, gMapMinX, (gMapMaxX + 1) - gMapSizeX);
   D_80122E2C = CLAMP(D_80122E2C, gMapMinZ, (gMapMaxZ + 1) - gMapSizeZ);

   RotMatrix(&gCameraRotation, &gCameraMatrix);
   TransMatrix(&gCameraMatrix, &gCameraZoom);
   RotMatrix(&gLightRotation, &gMatrix_8012c068);

   if (gState.enableMapScaling) {
      ScaleMatrix(&gCameraMatrix, &gMapScale);
   }

   SetRotMatrix(&gCameraMatrix);
   SetTransMatrix(&gCameraMatrix);
}

void UpdateNonPlayerCamera(void) {
   RotMatrix(&gCameraRotation, &gCameraMatrix);
   TransMatrix(&gCameraMatrix, &gCameraZoom);
   RotMatrix(&gLightRotation, &gMatrix_8012c068);

   if (gState.enableMapScaling) {
      ScaleMatrix(&gCameraMatrix, &gMapScale);
   }

   SetRotMatrix(&gCameraMatrix);
   SetTransMatrix(&gCameraMatrix);
}

s16 s_introYRotSpeed_801231dc;
s16 s_introXRotSpeed_801231e0;
s16 s_introDstCamRotY_801231e4;

#undef EVTF
#define EVTF 597
void Evtf597_BattleIntro(EvtData *evt) {
   s16 diff;

   gCameraRotation.vy -= s_introYRotSpeed_801231dc;
   gCameraRotation.vx -= s_introXRotSpeed_801231e0;

   switch (evt->state) {
   case 0:
      gSignal1 = 1;
      s_introDstCamRotY_801231e4 =
          (gMapCursorStartingPos[gState.mapNum].y * ANGLE_90_DEGREES) + ANGLE_45_DEGREES;
      s_introYRotSpeed_801231dc = 8;
      s_introXRotSpeed_801231e0 = 5;
      gCameraZoom.vz = 800;
      gCameraRotation.vx = ANGLE_90_DEGREES;
      gPlayerControlSuppressed = 1;
      EVT.timer = 30;
      evt->state++;
   // fallthrough
   case 1:
      if (--EVT.timer == 0) {
         // Display battle conditions
         DrawWindow(0x42, 0, 0, 320, 72, 330, 80, WBS_DRAGON, 0);
         DrawText(20, 20, 40, 2, 0, gState.currentTextPointers[0]);
         DisplayBasicWindow(0x42);
         DisplayBasicWindow(0x43);
         SlideWindowTo(0x42, 1, 80);
         SlideWindowTo(0x43, 249, 80);
         EVT.timer = 80;
         evt->state++;
      }
      break;
   case 2:
      if (--EVT.timer == 0 || PressedCircleOrX()) {
         SlideWindowTo(0x42, -350, 80);
         SlideWindowTo(0x43, -102, 80);
         EVT.timer = 10;
         evt->state++;
      }
      break;
   case 3:
      if (--EVT.timer == 0) {
         CloseWindow(0x42);
         CloseWindow(0x43);
         EVT.timer = 10;
         evt->state++;
      }
      break;
   case 4:
      if (--EVT.timer == 0) {
         DrawWindow(0x42, 0, 0, 144, 64, 84, 90, WBS_DRAGON, 0);
         DrawText(20, 24, 20, 0, 0, "Begin Battle");
         DisplayCustomWindow(0x42, 2, 1, 0, 0, 0);
         EVT.timer = 30;
         evt->state++;
      }
      break;
   case 5:
      if (--EVT.timer == 0 || PressedCircleOrX()) {
         CloseWindow(0x42);
         EVT.timer = 20;
         evt->state++;
      }
      break;
   case 6:
      if (--EVT.timer == 0) {
         s_introYRotSpeed_801231dc = 0;
         s_introXRotSpeed_801231e0 = 0;
         gPlayerControlSuppressed = 0;

         gCameraRotation.vy &= 0xfff;

         diff = s_introDstCamRotY_801231e4 - gCameraRotation.vy;
         if (diff > 0) {
            if (diff > ANGLE_180_DEGREES) {
               s_introDstCamRotY_801231e4 -= ANGLE_360_DEGREES;
            }
         } else if (diff < -ANGLE_180_DEGREES) {
            s_introDstCamRotY_801231e4 += ANGLE_360_DEGREES;
         }

         EVT.timer = 20;
         evt->state++;
      }
      break;
   case 7:
      if (--EVT.timer != 0) {
         gCameraRotation.vy += (s_introDstCamRotY_801231e4 - gCameraRotation.vy) >> 2;
         gCameraRotation.vx += (0x180 - gCameraRotation.vx) >> 2;
         gCameraZoom.vz += (0x280 - gCameraZoom.vz) >> 2;
      } else {
         gCameraRotation.vy = s_introDstCamRotY_801231e4;
         gCameraRotation.vx = 0x180;
         gCameraZoom.vz = 0x280;
         gClearSavedPadState = 0;
         evt->functionIndex = EVTF_NULL;
         gSignal1 = 0;
      }
      break;
   }
}

#undef EVTF
#define EVTF 587
void Evtf587_BattleEnemyEvent(EvtData *evt) {
   s32 i;
   EvtData *sprite, *fx;
   EnemyEventSpawn *pSpawn;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   switch (evt->state) {
   case 0:
      gPlayerControlSuppressed = 1;
      if (gState.mapNum == 40) {
         if (gState.mapState.n.field_0x0 == 1) {
            evt->state = 16;
            return;
         }
         if (gState.turn == 1 && FindUnitByNameIdx(UNIT_KANE)) {
            evt->state = 10;
            return;
         }
      }
      if (gState.mapNum == 26) {
         if (gState.mapState.n.field_0x13 == 1 && FindUnitByNameIdx(UNIT_LANDO)) {
            gState.mapState.n.field_0x13 = 2;
            evt->state = 11;
            return;
         }
      }
      if (gState.mapNum == 32) {
         if (gState.turn == 2) {
            evt->state = 12;
            return;
         }
         if (gState.turn == 5) {
            evt->state = 13;
            return;
         }
         if (gState.turn == 8) {
            evt->state = 14;
            return;
         }
         if (gState.turn == 11) {
            evt->state = 15;
            return;
         }
      }
      if (gState.mapNum == 38) {
         if (gState.turn == 3) {
            evt->state = 17;
            return;
         }
      }
      gPlayerControlSuppressed = 0;
      evt->state = 99;
      return;

   case 10:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_ANGRY, 15);
            evt->state2++;
         }
         break;
      case 3:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:10)

      return;

   case 11:

      switch (evt->state2) {
      case 0:
         gState.mapState.n.field_0x13 = 2;
         SetupBattleMsgBox(UNIT_LANDO, PORTRAIT_LANDO_514, 19);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:11)

      return;

   case 12:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_DALLAS, PORTRAIT_C_KNIGHT, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DALLAS, PORTRAIT_DALLAS_516, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            gState.field_0x96 = 1;
            gState.mapState.n.field_0x0++;
            evt->state2++;
         }
         break;
      case 3:
         if (gState.field_0x96 == 0) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:12)

      return;

   case 13:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_DALLAS, PORTRAIT_C_KNIGHT, 15);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DALLAS, PORTRAIT_DALLAS_516, 16);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            gState.field_0x96 = 1;
            gState.mapState.n.field_0x0++;
            evt->state2++;
         }
         break;
      case 3:
         if (gState.field_0x96 == 0) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:13)

      return;

   case 14:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_DALLAS, PORTRAIT_C_KNIGHT, 17);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DALLAS, PORTRAIT_DALLAS_516, 18);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            gState.field_0x96 = 1;
            gState.mapState.n.field_0x0++;
            evt->state2++;
         }
         break;
      case 3:
         if (gState.field_0x96 == 0) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:14)

      return;

   case 15:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_DALLAS, PORTRAIT_DALLAS_516, 19);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            gState.field_0x96 = 1;
            gState.mapState.n.field_0x0++;
            evt->state2++;
         }
         break;
      case 2:
         if (gState.field_0x96 == 0) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:15)

      return;

   case 16:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x131c);
         gState.mapState.n.field_0x0 = 2;
         SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_HAPPY, 16);

         // Find an open tile for Xeno to spawn onto
         i = 12;
         while (gMapUnitsPtr[i][2].s.unitIdx != 0) {
            i--;
         }

         EVT.xenoSpawnZ = i;
         SetupBattleUnit(17, EVT.xenoSpawnZ, 2, 10, TEAM_ENEMY, DIR_NORTH, 1, 20, 1);
         gTileStateGridPtr[EVT.xenoSpawnZ][2].action = TA_X19;
         evt->state2++;
         break;

      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_XENO, PORTRAIT_XENO, 17);
            evt->state2++;
         }
         break;

      case 2:
      case 3:
         evt->state2++;
         break;

      case 4:
         PerformAudioCommand(0x31c);
         sprite = GetUnitSpriteAtPosition(EVT.xenoSpawnZ, 2);
         fx = Evt_GetUnused();
         fx->functionIndex = EVTF_STRETCH_WARP_SPRITE;
         fx->d.evtf062.sprite = sprite;
         fx->mem = 1;
         evt->state2++;
         break;

      case 5:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE, 18);
            evt->state2++;
         }
         break;

      case 6:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_XENO, PORTRAIT_XENO, 19);
            evt->state2++;
         }
         break;

      case 7:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_ANGRY, 20);
            evt->state2++;
         }
         break;

      case 8:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_XENO, PORTRAIT_XENO, 21);
            evt->state2++;
         }
         break;

      case 9:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_HAPPY, 22);
            evt->state2++;
         }
         break;

      case 10:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_XENO, PORTRAIT_XENO, 23);
            evt->state2++;
         }
         break;

      case 11:
         if (gState.msgFinished) {
            PerformAudioCommand(0x20);
            EVT.timer = 20;
            evt->state2++;
         }
         break;

      case 12:
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            LoadSeqSet(0x19);
            FinishLoadingSeq();
            PerformAudioCommand(0x5e6);
            sprite = FindUnitSpriteByNameIdx(UNIT_KANE);
            fx = Evt_GetUnused();
            fx->functionIndex = EVTF_TBD_732;
            // TODO replace
            fx->d.sprite.x1 = sprite->d.sprite.x1;
            fx->d.sprite.z1 = sprite->d.sprite.z1;
            gState.D_801405A4 = 0;
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_CURSED, 24);
            evt->state2++;
            EVT.timer = 200;
         }
         break;

      case 13:
         if (--EVT.timer == 0) {
            gState.msgBoxFinished = 1;
         }
         if (gState.msgFinished) {
            PerformAudioCommand(0x5e7);
            gState.D_801405A4 = 1;
            PerformAudioCommand(0x131c);
            SetupBattleMsgBox(UNIT_XENO, PORTRAIT_XENO, 25);
            PerformAudioCommand(0x20b);
            evt->state2++;
         }
         break;

      case 14:
         if (gState.msgFinished) {
            sprite = FindUnitSpriteByNameIdx(UNIT_KANE);
            SPR_TILESTATE(sprite).action = TA_X16;
            EVT.spawnZ = HI(sprite->d.sprite.z1);
            EVT.spawnX = HI(sprite->d.sprite.x1);
            PerformAudioCommand(0x31c);
            sprite = GetUnitSpriteAtPosition(EVT.xenoSpawnZ, 2);
            fx = Evt_GetUnused();
            fx->functionIndex = EVTF_STRETCH_WARP_SPRITE;
            fx->d.evtf062.sprite = sprite;
            EVT.timer = 30;
            evt->state2++;
         }
         break;

      case 15:
         if (--EVT.timer == 0) {
            SetupBattleUnit(13, EVT.spawnZ, EVT.spawnX, 30, TEAM_ENEMY, DIR_WEST, 1, 20, 4);
            gTileStateGridPtr[EVT.xenoSpawnZ][2].action = TA_X16;
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 26);
            evt->state2++;
         }
         break;

      case 16:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE_BLK_KNIGHT, 29);
            evt->state2++;
         }
         break;

      case 17:
         if (gState.msgFinished) {
            PerformAudioCommand(0x21);
            EVT.timer = 75;
            evt->state2++;
         }
         break;

      case 18:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x201);
            evt->state = 99;
         }
         break;

      } // switch (evt->state2) (via state:16)

      return;

   case 17:

      switch (evt->state2) {
      case 0:
         gState.mapState.n.field_0x13 = 1;
         SetupBattleMsgBox(UNIT_SABINA, PORTRAIT_SABINA, 20);
         evt->state2++;
         break;

      case 1:
      case 2:
      case 3:
         evt->state2++;
         break;

      case 4:
         pSpawn = &sEnemyEventSpawns[0];

         for (i = 0; i < 6; i++) {
            s32 iz, ix;

            iz = pSpawn->z;
            ix = pSpawn->x;

            while (gMapUnitsPtr[iz][ix].s.team != TEAM_NULL ||
                   gTerrainPtr[iz][ix].terrain >= TERRAIN_BARREN) {
               iz++;
               if (iz == 16) {
                  iz = 0;
                  ix++;
               }
            }

            SetupBattleUnit(pSpawn->stripIdx, iz, ix, pSpawn->level, TEAM_ENEMY, DIR_SOUTH, 100,
                            pSpawn->expMulti, 16);
            gMapUnitsPtr[iz][ix].s.team = TEAM_ENEMY;
            pSpawn++;
         }

         evt->state2++;
         break;

      case 5:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KANE, PORTRAIT_KANE, 13);
            evt->state2++;
         }
         break;

      case 6:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_SABINA, PORTRAIT_SABINA, 14);
            evt->state2++;
         }
         break;

      case 7:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 15);
            do {
               // do-while fixes regalloc; can't get this function to match without at least one of
               // these; not sure if this implies something should be a macro or if it just happens
               // to nudge the regs into place
               evt->state2++;
            } while (0);
         }
         break;

      case 8:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;

      } // switch (evt->state2) (via state:17)

      return;

   case 99:
      gState.field_0x98 = 0;
      gPlayerControlSuppressed = 0;
      evt->functionIndex = EVTF_NULL;
      return;

   } // switch (evt->state)
}

#undef EVTF
#define EVTF 585
void Evtf585_BattlePlayerEvent(EvtData *evt) {
   s32 i;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   switch (evt->state) {
   case 0:
      gPlayerControlSuppressed = 1;
      if (gState.mapNum == 11) {
         if (gState.turn == 2) {
            // Only show dialog if all 3 party members are still alive
            s32 numIncapacitated = 3;
            for (i = 1; i <= 3; i++) {
               if (FindUnitByNameIdx(i)) {
                  numIncapacitated--;
               }
            }
            if (numIncapacitated == 0) {
               evt->state = 10;
               return;
            }
         }
         if (gState.turn == 3 && gState.mapState.n.field_0x0 == 0) {
            // Call attention to drawbridge switch
            evt->state = 11;
            return;
         }
      }
      if (gState.mapNum == 13) {
         if (gState.turn == 2 && FindUnitByNameIdx(UNIT_DIEGO) && FindUnitByNameIdx(UNIT_ELENI)) {
            evt->state = 12;
            return;
         }
         if (gState.turn == 3 && FindUnitByNameIdx(UNIT_ASH)) {
            evt->state = 28;
            return;
         }
      }
      if (gState.mapNum == 14) {
         if (gState.turn == 2) {
            evt->state = 13;
            return;
         }
      }
      if (gState.mapNum == 18) {
         if (gState.turn == 2) {
            evt->state = 14;
            return;
         }
      }
      if (gState.mapNum == 21) {
         if (gState.turn == 2) {
            evt->state = 15;
            return;
         }
         if (gState.turn == 4) {
            evt->state = 16;
            return;
         }
      }
      if (gState.mapNum == 23) {
         if (gState.turn == 3) {
            evt->state = 17;
            return;
         }
      }
      if (gState.mapNum == 26) {
         if (gState.turn == 2 && gState.mapState.n.field_0x13 == 0) {
            evt->state = 18;
            return;
         }
         if (gState.turn == 3 && gState.mapState.n.field_0x13 == 0) {
            evt->state = 19;
            return;
         }
         if (gState.turn == 4 && gState.mapState.n.field_0x13 == 0) {
            evt->state = 20;
            return;
         }
      }
      if (gState.mapNum == 42) {
         if (gState.turn == 2 && FindUnitByNameIdx(UNIT_DOLAN) && FindUnitByNameIdx(UNIT_AMON) &&
             FindUnitByNameIdx(UNIT_SARA)) {
            evt->state = 23;
            return;
         }
         if (gState.turn == 3 && FindUnitByNameIdx(UNIT_CLINT) && FindUnitByNameIdx(UNIT_KIRA)) {
            evt->state = 24;
            return;
         }
      }
      if (gState.mapNum == 41) {
         if (gState.turn == 2) {
            evt->state = 25;
            return;
         }
         if (gState.turn == 3) {
            evt->state = 26;
            return;
         }
         if (gState.turn == 4) {
            evt->state = 27;
            return;
         }
      }
      if (gState.mapNum == 28) {
         if (gState.turn == 2) {
            evt->state = 29;
            return;
         }
      }
      gPlayerControlSuppressed = 0;
      evt->state = 99;
      return;

   case 10:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_DIEGO, PORTRAIT_DIEGO, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_CLINT, PORTRAIT_CLINT, 15);
            evt->state2++;
         }
         break;
      case 3:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DIEGO, PORTRAIT_DIEGO, 16);
            evt->state2++;
         }
         break;
      case 4:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:10)

      return;

   case 11:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 17);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:11)

      return;

   case 12:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_DIEGO, PORTRAIT_DIEGO, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ELENI, PORTRAIT_ELENI_25, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DIEGO, PORTRAIT_DIEGO, 15);
            evt->state2++;
         }
         break;
      case 3:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ELENI, PORTRAIT_ELENI_25, 16);
            evt->state2++;
         }
         break;
      case 4:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:12)

      return;

   case 13:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 14:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 15:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 16:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH_UPSET, 14);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 17:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 13);
         evt->state2++;
         if (!FindUnitByNameIdx(UNIT_ZOHAR)) {
            evt->state2++;
         }
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ZOHAR, PORTRAIT_ZOHAR, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 18:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 14);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 19:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 15);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 20:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 16);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 23:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_SARA, PORTRAIT_SARA_HAPPY, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_AMON, PORTRAIT_AMON_ANGRY, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DOLAN, PORTRAIT_DOLAN, 15);
            evt->state2++;
         }
         break;
      case 3:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_SARA, PORTRAIT_SARA, 16);
            evt->state2++;
         }
         break;
      case 4:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_AMON, PORTRAIT_AMON_HAPPY, 17);
            evt->state2++;
         }
         break;
      case 5:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_SARA, PORTRAIT_SARA_HAPPY, 18);
            evt->state2++;
         }
         break;
      case 6:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:23)

      return;

   case 24:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBox(UNIT_CLINT, PORTRAIT_CLINT, 19);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KIRA, PORTRAIT_KIRA, 20);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_CLINT, PORTRAIT_CLINT, 21);
            evt->state2++;
         }
         break;
      case 3:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KIRA, PORTRAIT_KIRA, 22);
            evt->state2++;
         }
         break;
      case 4:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:24)

      return;

   case 25:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_ASH, PORTRAIT_APPARITION_675, 13);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 14);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 26:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_ASH, PORTRAIT_KANE_APPARITION, 15);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH_UPSET, 16);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 27:

      switch (evt->state2) {
      case 0:
         SetupBattleMsgBoxWithoutTail(UNIT_ASH, PORTRAIT_APPARITION_681, 17);
         evt->state2++;
         break;
      case 1:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH_6, 18);
            evt->state2++;
         }
         break;
      case 2:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      }

      return;

   case 28:

      switch (evt->state2) {
      case 0:
         SetupPartyBattleUnit(UNIT_KIRA, 9, 8, DIR_NORTH);
      // fallthrough
      case 1:
      case 2:
      case 3:
         evt->state2++;
         break;
      case 4:
         SetupBattleMsgBox(UNIT_KIRA, PORTRAIT_KIRA, 17);
         evt->state2++;
         break;
      case 5:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 18);
            evt->state2++;
         }
         break;
      case 6:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KIRA, PORTRAIT_KIRA, 19);
            evt->state2++;
         }
         break;
      case 7:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;
      } // switch (evt->state2) (via state:28)

      return;

   case 29:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x134d);
         SetupBattleMsgBox(UNIT_DUMAS, PORTRAIT_DUMAS, 14);
         evt->state2++;
         break;

      case 1:
         SetupBattleUnit(18, 19, 13, 10, TEAM_ENEMY, DIR_NORTH, 1, 20, 1);
         evt->state2++;
         break;

      case 2:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_KIRA_2, PORTRAIT_JUGGERNAUT, 15);
            evt->state2++;
         }
         break;

      case 3:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DUMAS, PORTRAIT_DUMAS_562, 16);
            evt->state2++;
         }
         break;

      case 4:
         if (gState.msgFinished) {
            SetupBattleMsgBoxWithoutTail(UNIT_DUMAS, PORTRAIT_JUGGERNAUT_INJURED, 22);
            evt->state2++;
         }
         break;

      case 5:
      case 6:
         evt->state2++;
         break;

      case 7:
         gTileStateGridPtr[19][13].action = TA_X16;
         evt->state2++;
         break;

      case 8:
      case 9:
      case 10:
         evt->state2++;
         break;

      case 11:
         SetupPartyBattleUnit(UNIT_ASH, 19, 13, DIR_EAST);
         SetupPartyBattleUnit(UNIT_DIEGO, 20, 13, DIR_EAST);
         SetupPartyBattleUnit(UNIT_HUXLEY, 21, 14, DIR_EAST);
         SetupPartyBattleUnit(UNIT_GROG, 20, 14, DIR_EAST);
         SetupPartyBattleUnit(UNIT_DOLAN, 18, 13, DIR_EAST);
         SetupPartyBattleUnit(UNIT_SARA, 18, 14, DIR_EAST);
         SetupPartyBattleUnit(UNIT_ZOHAR, 19, 14, DIR_EAST);
         PerformAudioCommand(0x34d);
         evt->state2++;
         break;

      case 12:
         if (gState.msgFinished) {
            PerformAudioCommand(0x38);
            EVT.timer = 20;
            evt->state2++;
         }
         break;

      case 13:
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            LoadSeqSet(23);
            FinishLoadingSeq();
            EVT.timer = 1;
            evt->state2++;
         }
         break;

      case 14:
         if (--EVT.timer == 0) {
            EVT.timer = 10;
            evt->state2++;
         }
         break;

      case 15:
         if (--EVT.timer == 0) {
            EvtData *bloodSpurt = Evt_GetUnused();
            bloodSpurt->functionIndex = EVTF_BLOOD_SPURT;
            // TODO replace
            HI(bloodSpurt->d.sprite.z1) = 19;
            HI(bloodSpurt->d.sprite.x1) = 13;
            EVT.timer = 30;
            evt->state2++;
         }
         break;

      case 16:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x202);
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 17);
            evt->state2++;
         }
         break;

      case 17:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_CLINT, PORTRAIT_CLINT, 18);
            evt->state2++;
         }
         break;

      case 18:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_ASH, PORTRAIT_ASH, 19);
            evt->state2++;
         }
         break;

      case 19:
         if (gState.msgFinished) {
            SetupBattleMsgBox(UNIT_DUMAS, PORTRAIT_DUMAS_562, 20);
            evt->state2++;
         }
         break;

      case 20:
         if (gState.msgFinished) {
            evt->state = 99;
         }
         break;

      } // switch (evt->state2) (via state:29)

      return;

   case 99:
      gState.field_0x98 = 0;
      gPlayerControlSuppressed = 0;
      evt->functionIndex = EVTF_NULL;
      return;

   } // switch (evt->state)
}

void SetupBattleMsgBox(u8 nameIdx, s16 portrait, s16 textIdx) {
   EvtData *msgbox;

   gState.msgFinished = 0;

   msgbox = Evt_GetUnused();
   msgbox->functionIndex = EVTF_BATTLE_MSGBOX;
   msgbox->d.evtf586.nameIdx = nameIdx;
   msgbox->d.evtf586.portrait = portrait;
   msgbox->d.evtf586.textIdx = textIdx;
}

void SetupBattleMsgBoxWithoutTail(u8 nameIdx, s16 portrait, s16 textIdx) {
   EvtData *msgbox;

   gState.msgFinished = 0;

   msgbox = Evt_GetUnused();
   msgbox->functionIndex = EVTF_BATTLE_MSGBOX;
   msgbox->d.evtf586.nameIdx = nameIdx;
   msgbox->d.evtf586.portrait = portrait;
   msgbox->d.evtf586.textIdx = textIdx;
   msgbox->d.evtf586.omitTail = 1;
}

#undef EVTF
#define EVTF 586
void Evtf586_BattleMsgBox(EvtData *evt) {
   EvtData *sprite, *camEvt;

   switch (evt->state) {
   case 0:
      sprite = FindUnitSpriteByNameIdx(EVT.nameIdx);
      camEvt = Evt_GetUnused();
      camEvt->functionIndex = EVTF_CAMERA_TBD_588;
      camEvt->d.evtf588.sprite = sprite;
      EVT.timer = 20;
      evt->state++;
      break;
   case 1:
      if (--EVT.timer == 0) {
         sprite = FindUnitSpriteByNameIdx(EVT.nameIdx);
         ShowMsgBoxForSprite(sprite, 0, EVT.omitTail);
         SetMsgBoxPortrait(EVT.portrait, 0);
         SetMsgBoxText(1, EVT.textIdx, 0x100);
         evt->state++;
      }
      break;
   case 2:
      if (gState.msgBoxFinished) {
         CloseMsgBox(0);
         gState.msgFinished = 1;
         evt->functionIndex = EVTF_NULL;
         return;
      }
      break;
   }
}

void DisplaySpellStatusWindow(UnitStatus *unit, u8 windowId) {
   s32 numSpells, i;

   gWindowChoiceHeight = 18;
   gWindowChoicesTopMargin = 9;

   numSpells = 0;
   while (unit->spells[numSpells] != SPELL_NULL) {
      numSpells++;
      if (numSpells == 10) {
         break;
      }
   }

   DrawWindow(windowId, 0, 50, 144, numSpells * 18 + 18, 70, (10 - numSpells) * 9 + 10, WBS_CROSSED,
              0);

   for (i = 0; i < numSpells; i++) {
      DrawText(12, (i * 8 + i) * 2 + 60, 20, 0, 0, gSpellNames[unit->spells[i]]);
   }

   if (unit->team == TEAM_PLAYER) {
      DisplayBasicWindow(windowId);
   } else {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 23);
   }
}

void DisplayItemsStatusWindow(UnitStatus *unit, u8 windowId) {
   EvtData *icon;
   s32 y, numItems = 0;

   gWindowChoicesTopMargin = 9;

   if (unit->items[0] != ITEM_NULL) {
      numItems++;
   }
   if (unit->items[1] != ITEM_NULL) {
      numItems++;
   }

   DrawWindow(windowId, 0, 50, 144, numItems * 18 + 18, 70, (10 - numItems) * 9 + 10, WBS_CROSSED,
              0);

   y = 0;
   if (unit->items[0] != ITEM_NULL) {
      DrawSjisText(28, y * 18 + 60, 20, 0, 0, gItemNamesSjis[unit->items[0]]);
      y++;
   }
   if (unit->items[1] != ITEM_NULL) {
      DrawSjisText(28, y * 18 + 60, 20, 0, 0, gItemNamesSjis[unit->items[1]]);
      y++;
   }

   if (unit->team == TEAM_PLAYER) {
      DisplayBasicWindow(windowId);
   } else {
      DisplayCustomWindow(windowId, 0, 1, 0, 0, 23);
   }

   icon = Evt_GetUnused();
   icon->functionIndex = EVTF_DISPLAY_ICON;
   icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->items[0];
   icon->d.sprite.x1 = 79;
   icon->d.sprite.y1 = (10 - numItems) * 9 + 20;
   y = (unit->items[0] != ITEM_NULL) ? 18 : 0;

   icon = Evt_GetUnused();
   icon->functionIndex = EVTF_DISPLAY_ICON;
   icon->d.sprite.gfxIdx = GFX_ITEM_ICONS_OFS + unit->items[1];
   icon->d.sprite.x1 = 79;
   icon->d.sprite.y1 = (10 - numItems) * 9 + 20 + y;
}

u8 s_unitX_801231e8;
u8 s_unitZ_801231ec;
u8 s_hiddenItem_801231f0;
u8 s_hiddenItemSentToDepot_801231f4;

#undef EVTF
#define EVTF 003
void Evtf003_BattleActions(EvtData *evt) {
   // TODO: Eliminate gotos?
   UnitStatus *unit, *cursorUnit;
   EvtData *evt1, *evt2;
   s32 i; // iz, mapNum
   s32 ix;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   unit = EVT.unit;

   switch (evt->state) {
   case 0:
      if (evt->state2 == 0) {
         gState.droppedItem = ITEM_NULL;
         s_unitX_801231e8 = HI(EVT.unitX) = HI(EVT.x);
         s_unitZ_801231ec = HI(EVT.unitZ) = HI(EVT.z);
         EVT.unit = unit = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
         gState.lastSelectedUnit = unit->name + 1;
         unit->hideMarker = 1;
         EVT.range = unit->travelRange;
         EVT.remainingRange = EVT.range;
         UpdateUnitInfoWindow(unit);
         DisplayBasicWindow(0x1f);
         PopulateBlueMovementGrid(HI(EVT.z), HI(EVT.x), unit->travelRange);
         gShowBlueMovementGrid = 0;
         evt2 = unit->evtSprite;
         StartUnitSpritesDecoder(evt2->d.sprite.stripIdx);
         evt->state = 16;
         evt->state2 = 0;
         goto BattleActions_State16;
      }

      break; // -> switch(cursorState)

   case 1:
      gState.mapCursorOutOfRange = 0;
      gWindowChoiceHeight = 17;
      gWindowChoicesTopMargin = 10;
      DrawWindow(0x35, 316, 0, 80, 90, 115, 80, WBS_CROSSED, 4);
      DrawText(316, 11, 20, 2, 0, "Action\nDone\nExamine\nPush");
      DisplayBasicWindow(0x35);
      gWindowActiveIdx = 0x35;
      gClearSavedPadState = 1;
      evt->state++;
      break;

   case 2:
      if (gWindowChoice.raw == 0x3501) {
         // Action
         CloseWindow(0x35);
         if (unit->spells[0] != SPELL_NULL) {
            evt->state = 10;
         } else {
            evt->state = 11;
         }
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3502) {
         // Done
         CloseWindow(0x35);
         evt->state = 8;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3504) {
         // Push
         CloseWindow(0x35);
         evt->state = 15;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x3503) {
         // Examine
         CloseWindow(0x35);
         evt->state = 9;
         evt->state2 = 0;
      } else if (gWindowChoice.raw == 0x35ff) {
         // Cancel
         if (EVT.range != 0) {
            CloseWindow(0x35);
            if (HI(EVT.unitX) != HI(EVT.x) || HI(EVT.unitZ) != HI(EVT.z)) {
               gTileStateGridPtr[HI(EVT.unitZ)][HI(EVT.unitX)].action = TA_X9;
               HI(EVT.unitX) = HI(EVT.x);
               HI(EVT.unitZ) = HI(EVT.z);
            }
            evt->state = 7;
            evt->state2 = 0;
         }
      }
      break;

   case 7:
      if (gBlueMovementGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_UNSET) {
         gState.mapCursorOutOfRange = 0;
      } else {
         gState.mapCursorOutOfRange = 1;
      }

      switch (evt->state2) {
      case 0:
         DrawWindow(0x34, 316, 0, 56, 36, 10, 152, WBS_CROSSED, 0);
         DrawText(316, 11, 25, 0, 0, "Move");
         DisplayCustomWindow(0x34, 2, 1, 0, 0, 0);
         evt->state2++;

      // fallthrough
      case 1:
         gShowBlueMovementGrid = 1;
         gClearSavedPadState = 0;

         if (!gDecodingSprites && (gSavedPadStateNewPresses & PAD_CIRCLE) &&
             gBlueMovementGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_INVALID &&
             gBlueMovementGridPtr[gMapCursorZ][gMapCursorX] != PATH_STEP_UNSET &&
             gMapUnitsPtr[gMapCursorZ][gMapCursorX].raw == 0) {
            func_8002B3A8(HI(EVT.z), HI(EVT.x), EVT.range, 2);
            EVT.remainingRange = EVT.range - gPathGrid2_Ptr[gMapCursorZ][gMapCursorX];
            HI(EVT.unitX) = gMapCursorX;
            HI(EVT.unitZ) = gMapCursorZ;
            PlotPathBackToUnit(gMapCursorZ, gMapCursorX);
            gTileStateGridPtr[HI(EVT.z)][HI(EVT.x)].action = TA_X6;
            gSignal3 = 0;
            gClearSavedPadState = 1;
            evt->state2++;
         } else if ((gSavedPadStateNewPresses & PAD_CIRCLE) &&
                    gBlueMovementGridPtr[gMapCursorZ][gMapCursorX] == PATH_STEP_INVALID) {
            EVT.remainingRange = EVT.range;
            CloseWindow(0x34);
            gShowBlueMovementGrid = 0;
            gState.mapCursorOutOfRange = 0;
            evt->state = 1;
         } else if (gSavedPadStateNewPresses & PAD_X) {
            EVT.remainingRange = EVT.range;
            CloseWindow(0x34);
            gShowBlueMovementGrid = 0;
            gState.mapCursorOutOfRange = 0;
            evt->state = 16;
            evt->state2 = 0;
         }
         break;

      case 2:
         if (gSignal3 != 0) {
            CloseWindow(0x34);
            gShowBlueMovementGrid = 0;
            gState.mapCursorOutOfRange = 0;
            evt->state = 1;
         }
         break;
      } // switch (evt->state2) (via state:7)

      break;

   case 8:

      switch (evt->state2) {
      case 0:
         gTileStateGridPtr[HI(EVT.unitZ)][HI(EVT.unitX)].action = TA_CHOOSING_DIRECTION;
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_CHOOSE_DONE_DIRECTION;
         evt2->d.evtf016.unit = EVT.unit;
         gSignal3 = 0;
         gSignal2 = 0;
         evt->state2++;
      // fallthrough
      case 1:
         if (gSignal2 == 1) {
            evt->state = 99;
            evt->state2 = 0;
         }
         if (gSignal2 == 2) {
            if (EVT.range != 0) {
               evt->state = 16;
               evt->state2 = 0;
            } else {
               evt->state = 1;
            }
            if (HI(EVT.unitX) != HI(EVT.x) || HI(EVT.unitZ) != HI(EVT.z)) {
               evt->state = 1;
            }
         }
         break;
      } // switch (evt->state2) (via state:8)

      break;

   case 9:
      // Examine

      switch (evt->state2) {
      case 0:
         CloseWindow(0x1e);
         EVT.performedSubaction = 1;
         gState.searchX = HI(EVT.unitX);
         gState.searchZ = HI(EVT.unitZ);
         s_hiddenItem_801231f0 = ITEM_NULL;
         i = gState.mapNum;
         if (!gState.mapState.n.foundHiddenItem1 && gState.searchZ == gMapHiddenItems[i][0].z &&
             gState.searchX == gMapHiddenItems[i][0].x) {
            s_hiddenItem_801231f0 = gMapHiddenItems[i][0].item;
            if (unit->items[0] == ITEM_NULL) {
               unit->items[0] = s_hiddenItem_801231f0;
               s_hiddenItemSentToDepot_801231f4 = 0;
            } else if (unit->items[1] == ITEM_NULL) {
               unit->items[1] = s_hiddenItem_801231f0;
               s_hiddenItemSentToDepot_801231f4 = 0;
            } else {
               gState.depot[s_hiddenItem_801231f0]++;
               s_hiddenItemSentToDepot_801231f4 = 1;
            }
            gState.mapState.n.foundHiddenItem1 = 1;
            evt->state2++;
         } else if (!gState.mapState.n.foundHiddenItem2 &&
                    gState.searchZ == gMapHiddenItems[i][1].z &&
                    gState.searchX == gMapHiddenItems[i][1].x) {
            s_hiddenItem_801231f0 = gMapHiddenItems[i][1].item;
            if (unit->items[0] == ITEM_NULL) {
               unit->items[0] = s_hiddenItem_801231f0;
               s_hiddenItemSentToDepot_801231f4 = 0;
            } else if (unit->items[1] == ITEM_NULL) {
               unit->items[1] = s_hiddenItem_801231f0;
               s_hiddenItemSentToDepot_801231f4 = 0;
            } else {
               gState.depot[s_hiddenItem_801231f0]++;
               s_hiddenItemSentToDepot_801231f4 = 1;
            }
            gState.mapState.n.foundHiddenItem2 = 1;
            evt->state2++;
         } else {
            evt->state2 += 4;
         }
         break;

      case 1:
         PerformAudioCommand(0x374);
         gSignal3 = 0;
         evt2 = unit->evtSprite;
         evt1 = Evt_GetUnused();
         evt1->d.evtf294.x = evt2->d.sprite.x1;
         evt1->d.evtf294.z = evt2->d.sprite.z1;
         evt1->d.evtf294.y = evt2->d.sprite.y1;
         evt1->functionIndex = EVTF_REVEAL_HIDDEN_ITEM;
         evt1->d.evtf294.gfxIdx = GFX_ITEM_ICONS_OFS + s_hiddenItem_801231f0;
         EVT.timer = 80;
         evt->state2++;
         break;

      case 2:
         if (--EVT.timer == 0) {
            if (!s_hiddenItemSentToDepot_801231f4) {
               ShowReceivedItemDialog(s_hiddenItem_801231f0, 0x3c, 1);
            } else {
               ShowDepotReceivedItemDialog(s_hiddenItem_801231f0, 0x3c, 1);
            }
            EVT.timer = 80;
            gSignal3 = 1;
            evt->state2++;
         }
         break;

      case 3:
         if (PressedCircleOrX() || --EVT.timer == 0) {
            CloseWindow(0x3c);
            evt->state2++;
         }
         break;

      case 4:
         gState.needEval = 1;
         gState.signal = 1;
         EVT.timer = 0;
         evt->state2++;
         break;

      case 5:
         if (++EVT.timer > 100) {
            EVT.timer = 100;
         }
         if (gState.signal == 0) {
            if (EVT.timer < 20 && s_hiddenItem_801231f0 == ITEM_NULL) {
               gClearSavedPadState = 1;
               DrawWindow(0x3c, 0, 0, 232, 36, 44, 90, WBS_CROSSED, 0);
               DrawText(12, 11, 25, 2, 0, "you didn't find anything!");
               DisplayCustomWindow(0x3c, 0, 1, 0, 0, 0);
               EVT.timer = 30;
               evt->state2++;
            } else {
               evt->state2 += 2;
            }
         }
         break;

      case 6:
         if (PressedCircleOrX() || --EVT.timer == 0) {
            CloseWindow(0x3c);
            evt->state2++;
         }
         break;

      case 7:
         EVT.range = EVT.remainingRange;
         HI(EVT.x) = HI(EVT.unitX);
         HI(EVT.z) = HI(EVT.unitZ);
         PopulateBlueMovementGrid(HI(EVT.z), HI(EVT.x), EVT.range);
         evt->state2++;
         break;

      case 8:
         if (EVT.range != 0) {
            evt->state = 16;
            evt->state2 = 0;
         } else {
            evt->state = 1;
         }
         break;
      } // switch (evt->state2) (via state:9)

      break;

   case 10:

      switch (evt->state2) {
      case 0:
         gWindowChoiceHeight = 17;
         gWindowChoicesTopMargin = 10;
         DrawWindow(0x36, 316, 0, 80, 72, 115, 80, WBS_CROSSED, 3);
         DrawText(316, 11, 20, 2, 0, "Attack\nMagic\nItem");
         DisplayBasicWindow(0x36);
         gWindowActiveIdx = 0x36;
         gClearSavedPadState = 1;
         evt->state2++;
         break;

      case 1:
         if (gWindowChoice.raw == 0x3601) {
            // Attack
            CloseWindow(0x36);
            gClearSavedPadState = 0;
            evt->state = 12;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3602) {
            // Magic
            CloseWindow(0x1e);
            CloseWindow(0x36);
            evt->state = 13;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3603) {
            // Item
            CloseWindow(0x1e);
            CloseWindow(0x36);
            evt->state = 17;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x36ff) {
            // Cancel
            //@3e44
            CloseWindow(0x36);
         LAB_80033808:
            if (HI(EVT.unitX) == HI(EVT.x) && HI(EVT.unitZ) == HI(EVT.z)) {
               evt->state = 16;
               evt->state2 = 0;
               if (EVT.range == 0) {
                  evt->state = 1;
               }
            } else {
               evt->state = 1;
            }
         }
         break;
      } // switch (evt->state2) (via state:10)

      break;

   case 11:

      switch (evt->state2) {
      case 0:
         gWindowChoiceHeight = 17;
         gWindowChoicesTopMargin = 10;
         DrawWindow(0x37, 316, 0, 80, 54, 150, 100, WBS_CROSSED, 2);
         DrawText(316, 11, 20, 2, 0, "Attack\nItem");
         DisplayBasicWindow(0x37);
         gWindowActiveIdx = 0x37;
         gClearSavedPadState = 1;
         evt->state2++;
         break;

      case 1:
         if (gWindowChoice.raw == 0x3701) {
            // Attack
            CloseWindow(0x37);
            gClearSavedPadState = 0;
            evt->state = 12;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3702) {
            // Item
            CloseWindow(0x37);
            evt->state = 17;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x37ff) {
            // Cancel
            //@3fb0
            CloseWindow(0x37);
            goto LAB_80033808; //?: could match via tail merge?
         }
         break;
      } // switch (evt->state2) (via state:11)

      break;

   case 12:

      switch (evt->state2) {
      case 0:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_TARGETING_ATTACK;
         gSignal2 = 0;
         evt2->d.evtf015.x = EVT.unitX;
         evt2->d.evtf015.z = EVT.unitZ;
         evt->state2++;

      // fallthrough
      case 1:
         if (gSignal2 == 1) {
            gState.mapCursorOutOfRange = 0;
            if (HI(EVT.unitX) == HI(EVT.x) && HI(EVT.unitZ) == HI(EVT.z)) {
               evt->state = 16;
               evt->state2 = 0;
               if (EVT.range == 0) {
                  evt->state = 1;
               }
            } else {
               evt->state = 1;
            }
         }
         if (gSignal2 == 2) {
            for (i = gMapMinZ; i <= gMapMaxZ; i++) {
               for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
                  gBlueMovementGridPtr[i][ix] = PATH_STEP_UNSET;
               }
            }
            evt->state2++;
         }
         break;

      case 2:
         if (gSignal2 == 99) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:12)

      break;

   case 13:
   case 17:

      switch (evt->state2) {
      case 0:
         SlideWindowTo(0x39, 8, 268);
         evt2 = Evt_GetUnused();
         if (evt->state == 13) {
            evt2->functionIndex = EVTF_BATTLE_SPELLS_LIST;
         } else {
            evt2->functionIndex = EVTF_BATTLE_ITEMS_LIST;
         }
         evt2->d.evtf031.unit = EVT.unit;
         gSignal2 = 0;
         evt->state2++;

      // fallthrough
      case 1:
         if (gSignal2 == 1) {
            gState.mapCursorOutOfRange = 0;
            SlideWindowTo(0x39, 8, 188);
            if (HI(EVT.unitX) == HI(EVT.x) && HI(EVT.unitZ) == HI(EVT.z)) {
               evt->state = 16;
               evt->state2 = 0;
               if (EVT.range == 0) {
                  evt->state = 1;
               }
            } else {
               evt->state = 1;
            }
         }
         if (gSignal2 == 2) {
            gState.mapCursorOutOfRange = 0;
            evt->state2++;
         }
         break;

      case 2:
         evt2 = Evt_GetLastUnused();
         evt2->functionIndex = EVTF_TARGETING_SPELL;
         gSignal2 = 0;
         evt2->d.evtf027.x = EVT.unitX;
         evt2->d.evtf027.z = EVT.unitZ;
         evt->state2++;

      // fallthrough
      case 3:
         if (gSignal2 == 1) {
            evt->state2 = 1;
            if (evt->state == 17) {
               // Restore item on cancel
               unit->items[gState.activeItemSlot] = gState.activeItem;
            }
         }
         if (gSignal2 == 2) {
            gClearSavedPadState = 1;
            evt2 = unit->evtSprite;
            unit->direction = evt2->d.sprite.direction;
            for (i = gMapMinZ; i <= gMapMaxZ; i++) {
               for (ix = gMapMinX; ix <= gMapMaxX; ix++) {
                  gBlueMovementGridPtr[i][ix] = PATH_STEP_UNSET;
               }
            }
            evt->state2++;
         }
         break;

      case 4:
         if (gSignal2 == 99) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:13/17)

      break;

   case 15:
      // Push

      switch (evt->state2) {
      case 0:
         gTileStateGridPtr[HI(EVT.unitZ)][HI(EVT.unitX)].action = TA_CHOOSING_DIRECTION;
         evt->state2++;
         break;

      case 1:
         evt2 = Evt_GetUnused();
         evt2->functionIndex = EVTF_PUSH;
         // TODO replace with evtf048
         evt2->d.evtf031.unit = EVT.unit;
         gSignal3 = 0;
         gSignal2 = 0;
         evt->state2++;

      // fallthrough
      case 2:
         if (gSignal2 == 99) {
            EVT.performedSubaction = 1;
            EVT.range = EVT.remainingRange;
            PopulateBlueMovementGrid(HI(EVT.unitZ), HI(EVT.unitX), EVT.range);
            HI(EVT.x) = HI(EVT.unitX);
            HI(EVT.z) = HI(EVT.unitZ);
            if (EVT.range != 0) {
               evt->state = 16;
               evt->state2 = 0;
            } else {
               evt->state = 1;
            }
         } else if (gSignal2 == 2) {
            if (HI(EVT.unitX) == HI(EVT.x) && HI(EVT.unitZ) == HI(EVT.z)) {
               if (EVT.range != 0) {
                  evt->state = 16;
                  evt->state2 = 0;
               } else {
                  evt->state = 1;
               }
            } else {
               evt->state = 1;
            }
         }
         break;
      } // switch (evt->state2) (via state:15)

      break;

   case 16:
   BattleActions_State16:
      gState.mapCursorOutOfRange = 0;

      switch (evt->state2) {
      case 0:
         gWindowChoiceHeight = 0x11;
         gWindowChoicesTopMargin = 10;
         DrawWindow(0x35, 316, 0, 80, 108, 115, 60, WBS_CROSSED, 5);
         DrawText(316, 11, 20, 2, 0, "Move\nAction\nDone\nExamine\nPush");
         DisplayBasicWindow(0x35);
         gWindowActiveIdx = 0x35;
         gClearSavedPadState = 1;
         evt->state2++;
         break;

      case 1:
         if (gWindowChoice.raw == 0x3501) {
            // Move
            CloseWindow(0x1f);
            CloseWindow(0x35);
            evt->state = 7;
            evt->state2 = 0;
         }
         if (gWindowChoice.raw == 0x3502) {
            // Action
            CloseWindow(0x1f);
            CloseWindow(0x35);
            if (unit->spells[0] != SPELL_NULL) {
               evt->state = 10;
            } else {
               evt->state = 11;
            }
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3503) {
            // Done
            CloseWindow(0x1f);
            CloseWindow(0x35);
            evt->state = 8;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3505) {
            // Push
            CloseWindow(0x1f);
            CloseWindow(0x35);
            evt->state = 15;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x3504) {
            // Examine
            CloseWindow(0x1f);
            CloseWindow(0x35);
            evt->state = 9;
            evt->state2 = 0;
         } else if (gWindowChoice.raw == 0x35ff) {
            // Cancel
            if (!EVT.performedSubaction) {
               CloseWindow(0x1f);
               CloseWindow(0x35);
               evt->state = 100;
               evt->state2 = 0;
            }
         }
         break;
      } // switch (evt->state2) (via state:16)

      break;

   case 99:

      switch (evt->state2) {
      case 0:
         gClearSavedPadState = 1;
         gState.mapCursorOutOfRange = 0;
         evt2 = unit->evtSprite;
         if (SPR_TERRAIN(evt2).terrain == TERRAIN_VILE_BOG) {
            unit->poisoned = 1;
         }
         if (gState.droppedItem != ITEM_NULL) {
            ShowReceivedItemDialog(gState.droppedItem, 0x3c, 0);
            gState.depot[gState.droppedItem]++;
            EVT.timer = 50;
            evt->state2++;
         } else {
            evt->state2 += 2;
         }
         break;

      case 1:
         if (PressedCircleOrX() || --EVT.timer == 0) {
            CloseWindow(0x3c);
            evt->state2++;
         }
         break;

      case 2:
         unit->done++;
         if (gState.mapNum == 26 && gState.mapState.n.field_0x13 == 0) {
            // Moved during ambush (Reed Highway)
            gState.mapState.n.field_0x13 = 1;
         }
         evt->state++;
         break;
      } // switch (evt->state2) (via state:99)

      break;

   case 100:
      gState.needEval = 1;
      evt->state++;
      break;

   case 101:
      if (gState.battleEval != BATTLE_EVAL_NONE) {
         return;
      }
      unit->hideMarker = 0;
      if (EVT.performedSubaction) {
         unit->done++;
      }
      gClearSavedPadState = 0;
      gSignal1 = 0;
      ClearBlueMovementGrid();
      evt->functionIndex = EVTF_NULL;
      CloseWindow(0x1e);
      break;
   } // switch (evt->state)

   switch (EVT.cursorState) {
   case 0:
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == unit->idx) {
         break;
      }
      EVT.cursorState++;

   // fallthrough
   case 1:
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == 0) {
         break;
      }
      EVT.cursorState++;

   // fallthrough
   case 2:
   BattleActions_CursorState2:
      EVT.cursorUnitIdx = gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx;
      cursorUnit = &gUnits[gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx];
      UpdateCompactUnitInfoWindow(cursorUnit, cursorUnit, 1);
      CloseWindow(0x1e);
      if (cursorUnit->team == TEAM_PLAYER) {
         DisplayBasicWindow(0x1e);
      } else {
         DisplayCustomWindow(0x1e, 0, 1, 0, 0, 23);
      }
      EVT.cursorState++;
      break;

   case 3:
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == 0) {
         CloseWindow(0x1e);
         EVT.cursorUnitIdx = 0;
         EVT.cursorState = 1;
         break;
      }
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == EVT.cursorUnitIdx) {
         break;
      }
      EVT.cursorState--;
      goto BattleActions_CursorState2;
   } // switch (cursorState)
}

s16 s_newZoom_801231f8;
u8 s_menuMem_battleOptions_801231fc;

#undef EVTF
#define EVTF 425
void Evtf425_BattleOptions(EvtData *evt) {
   // TODO: eliminate goto; earlier attempt to re-write in a more natural way threw off a bunch of
   // stuff; need to try other variations
   static u8 turnBuffer[13] = "\x82\x73\x82\x74\x82\x71\x82\x6d\x81\x40\x81\x40";
   s32 i;
   UnitStatus *unit;
   s16 dx, dz;
   s16 unitX, unitZ;
   EvtData *evt1;
   s16 turn;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   switch (evt->state) {
   case 0:
      s_menuMem_battleOptions_801231fc = 0;
      if (gSignal1 == 0) {
         if ((gSavedPadStateNewPresses & PAD_SQUARE) && !gPlayerControlSuppressed &&
             !gClearSavedPadState) {
            // Pressing Square selects next available unit
            i = 0;
            if (gState.lastSelectedUnit > 12) {
               gState.lastSelectedUnit = 1;
            }

            while ((unit = FindUnitByNameIdx(gState.lastSelectedUnit)) == 0 || unit->done) {
               gState.lastSelectedUnit++;
               if (gState.lastSelectedUnit > 12) {
                  gState.lastSelectedUnit = 1;
               }
               if (i++ > 12) {
                  // FIXME
                  goto LAB_800347e4; // -> to next if
               }
            }

            dx = 0;
            dz = 0;
            gState.lastSelectedUnit++;
            unitX = HI(unit->evtSprite->d.sprite.x1);
            unitZ = HI(unit->evtSprite->d.sprite.z1);

            if (gMapCursorX < unitX) {
               dx = 1;
            }
            if (gMapCursorX > unitX) {
               dx = -1;
            }
            if (gMapCursorZ < unitZ) {
               dz = 1;
            }
            if (gMapCursorZ > unitZ) {
               dz = -1;
            }

            for (; gMapCursorX != unitX; gMapCursorX += dx) {
               CenterCamera(1);
            }
            for (; gMapCursorZ != unitZ; gMapCursorZ += dz) {
               CenterCamera(1);
            }

            evt1 = Evt_FindByFunction(EVTF_BATTLE_MAP_CURSOR_CONTROL);
            HI(evt1->d.sprite.x1) = gMapCursorX;
            HI(evt1->d.sprite.z1) = gMapCursorZ;
            // break; // -> to next if
         } // Square

      LAB_800347e4:
         if (gSignal1 == 0 && (gSavedPadStateNewPresses & PAD_CIRCLE) &&
             gMapUnitsPtr[gMapCursorZ][gMapCursorX].raw == 0 && !gPlayerControlSuppressed) {
            // Pressing Circle on an unoccupied tile opens battle options
            gClearSavedPadState = 1;
            gSignal1 = 1;
            evt->state++;
            gWindowChoiceHeight = 17;
            gWindowChoicesTopMargin = 10;
            CloseWindow(0x35);
            DrawWindow(0x35, 256, 120, 96, 36, 16, 16, WBS_CROSSED, 0);

            turn = gState.turn;
            if (turn > 99) {
               turn %= 100;
            }
            // SJIS 8140: Space, 824F: Zero
            turnBuffer[8] = 0x81;
            turnBuffer[9] = 0x40;
            turnBuffer[10] = 0x82;
            turnBuffer[11] = 0x4f + (turn % 10);

            if (turn > 9) {
               i = (turn / 10);
               turnBuffer[8] = 0x82;
               turnBuffer[9] = 0x4f + i;
            }

            DrawSjisText(260, 131, 10, 2, 0, turnBuffer);
            DisplayBasicWindow(0x35);
         } // Circle
      }
      break;

   case 1:
      gWindowChoiceHeight = 17;
      gWindowChoicesTopMargin = 10;
      DrawWindow(0x34, 0, 0, 176, 144, 62, 50, WBS_CROSSED, 7);
      DrawText(12, 11, 17, 2, 0,
               "BATTLE CONDITION\nTURN OVER\nCHANGE ZOOM\nSTATUS\nOPTIONS\nSAVE\nLOAD");
      DisplayBasicWindowWithSetChoice(0x34, s_menuMem_battleOptions_801231fc);
      gWindowActiveIdx = 0x34;
      evt->state++;
      break;

   case 2:
      if (gWindowChoice.raw == 0x3402) {
         // Turn over
         s_menuMem_battleOptions_801231fc = 1;
         CloseWindow(0x34);
         CloseWindow(0x35);
         evt->state = 3;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3403) {
         // Change zoom
         s_menuMem_battleOptions_801231fc = 2;
         CloseWindow(0x34);
         evt->state = 5;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3404) {
         // Status
         s_menuMem_battleOptions_801231fc = 3;
         CloseWindow(0x34);
         evt->state = 0xb;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3401) {
         // Battle condition
         s_menuMem_battleOptions_801231fc = 0;
         CloseWindow(0x34);
         evt->state = 6;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3405) {
         // Options
         s_menuMem_battleOptions_801231fc = 4;
         CloseWindow(0x34);
         evt->state = 7;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x34ff) {
         // Cancel
         CloseWindow(0x34);
         evt->state = 99;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3406) {
         // Save
         CloseWindow(0x34);
         CloseWindow(0x35);
         evt->state = 12;
         evt->state2 = 0;
      }
      if (gWindowChoice.raw == 0x3407) {
         // Load
         CloseWindow(0x34);
         CloseWindow(0x35);
         evt->state = 13;
         evt->state2 = 0;
      }
      break;

   case 3:

      switch (evt->state2) {
      case 0:
         gIsEnemyTurn = 1;
         evt->state2++;
         break;
      case 1:
         if (!gIsEnemyTurn) {
            gSignal1 = 0;
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 5:

      switch (evt->state2) {
      case 0:
         DrawWindow(0x34, 256, 0, 88, 74, 105, 80, WBS_CROSSED, 3);
         DrawText(260, 11, 10, 2, 0, "Close\nMedium\nDistant");
         DisplayBasicWindowWithSetChoice(0x34, gState.zoom);
         gWindowActiveIdx = 0x34;
         evt->state2++;
         break;

      case 1:
         EVT.timer = 15;
         if (gWindowChoice.raw == 0x3401) {
            CloseWindow(0x34);
            gState.zoom = 0;
            s_newZoom_801231f8 = 450;
            evt->state2++;
         } else if (gWindowChoice.raw == 0x3402) {
            CloseWindow(0x34);
            gState.zoom = 1;
            s_newZoom_801231f8 = 640;
            evt->state2++;
         } else if (gWindowChoice.raw == 0x3403) {
            CloseWindow(0x34);
            gState.zoom = 2;
            s_newZoom_801231f8 = 896;
            evt->state2++;
         } else if (gWindowChoice.raw == 0x34ff || gWindowChoice.raw == 0x3404) {
            CloseWindow(0x34);
            evt->state = 1;
            evt->state2 = 0;
         }
         break;

      case 2:
         if (--EVT.timer != 0) {
            gCameraZoom.vz += (s_newZoom_801231f8 - gCameraZoom.vz) >> 2;
         } else {
            gCameraZoom.vz = s_newZoom_801231f8;
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 6:
      // Battle conditions
      switch (evt->state2) {
      case 0:
         DrawWindow(0x34, 0, 0, 320, 72, 1, 80, WBS_DRAGON, 0);
         DrawText(20, 20, 34, 2, 0, gState.currentTextPointers[0]);
         DisplayBasicWindow(0x34);
         DisplayBasicWindow(0x35);
         gWindowActiveIdx = 0x34;
         evt->state2++;
         break;
      case 1:
         if ((PadRead(1) >> 0x10) & PAD_SELECT) {
            // 2P Select: Enable fast-motion cheat
            gState.vsyncNoWait = 1;
         }
         if (gPadStateNewPresses & PAD_X) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 7:
      ShowConfigMenu(evt);
      break;

   case 8:
      ShowTextSpeedSettings(evt);
      break;

   case 9:
      ShowAudioSettings(evt);
      break;

   case 10:
      ShowCameraSettings(evt);
      break;

   case 11:

      switch (evt->state2) {
      case 0:
         CloseWindow(0x34);
         CloseWindow(0x35);
         gSignal2 = 0;
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_STATUS_WINDOW_MGR;
         evt->state2++;
      // fallthrough
      case 1:
         if (gSignal2 == 100) {
            gClearSavedPadState = 0;
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 12:

      switch (evt->state2) {
      case 0:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_FILE_SAVE_DIALOG_IBS;
         gState.D_8014053E = 0;
         evt->state2++;
         break;
      case 1:
         if (gState.D_8014053E != 0) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 13:

      switch (evt->state2) {
      case 0:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_FILE_LOAD_DIALOG_IBS;
         gState.D_8014053E = 0;
         evt->state2++;
         break;
      case 1:
         if (gState.D_8014053E != 0) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 99:
      CloseWindow(0x34);
      CloseWindow(0x35);
      gSignal1 = 0;
      gClearSavedPadState = 0;
      evt->state = 0;
      evt->state2 = 0;
      break;
   } // switch (evt->state)
}

void FieldInfo_HandleCursorUnit(EvtData *evt) {
   UnitStatus *cursorUnit;

   switch (evt->mem) {
   case 0:
      // Bug: cursorUnit is uninitialized here; presumably because this function's code was lifted
      // from Evtf003_BattleActions
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == cursorUnit->idx) {
         break;
      }
      evt->mem++;

   // fallthrough
   case 1:
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == 0) {
         break;
      }
      evt->mem++;

   // fallthrough
   case 2:
   FieldInfo_CursorState2:
      evt->d.evtf030.unitIdx = gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx;
      cursorUnit = &gUnits[gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx];
      UpdateCompactUnitInfoWindow(cursorUnit, cursorUnit, 1);
      CloseWindow(0x1e);

      if (cursorUnit->team == TEAM_PLAYER) {
         DisplayBasicWindow(0x1e);
      } else {
         DisplayCustomWindow(0x1e, 0, 1, 0, 0, 23);
      }
      evt->mem++;
      break;

   case 3:
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == 0) {
         CloseWindow(0x1e);
         evt->d.evtf030.unitIdx = 0;
         evt->mem = 1;
         break;
      }
      if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx == evt->d.evtf030.unitIdx) {
         break;
      }
      evt->mem--;
      goto FieldInfo_CursorState2;
   }
}

u8 s_unitX_80123200;
u8 s_unitZ_80123204;

#undef EVTF
#define EVTF 030
void Evtf030_FieldInfo(EvtData *evt) {
   static s8 terrainText[10][12] = {"Plains   0%", "Prairie  5%", "Thicket 15%", "Barren   0%",
                                    "Water   20%", "Vile bog 0%", "Lava     0%", "Boundary30%",
                                    "Obstacle   ", "No entry   "};
   EvtData *evt1;
   UnitStatus *unit;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   if (!gIsEnemyTurn) {
      switch (evt->state) {
      case 0:
         FieldInfo_HandleCursorUnit(evt);

         if ((gSavedPadStateNewPresses & PAD_CIRCLE) && gSignal1 == 0 &&
             gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx != 0) {
            CloseWindow(0x1e);

            // Debug mode allows controlling units regardless of team, done state, etc
            if (gState.debug == 0) {
               unit = &gUnits[gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx];
               if (gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.team != TEAM_PLAYER ||
                   unit->name == 30 || unit->name == 31 || unit->unitType == UNIT_TYPE_MAGE_TOWER) {
                  s_unitX_80123200 = gMapCursorX;
                  s_unitZ_80123204 = gMapCursorZ;
                  gSignal1 = 1;
                  evt->state = 4;
                  evt->state2 = 0;
                  break;
               } else if (unit->done || unit->paralyzed) {
                  s_unitX_80123200 = gMapCursorX;
                  s_unitZ_80123204 = gMapCursorZ;
                  gSignal1 = 1;
                  evt->state = 4;
                  evt->state2 = 0;
                  break;
               }
            }

            gSignal1 = 1;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_ACTIONS;
            HI(evt1->d.evtf003.x) = gMapCursorX;
            HI(evt1->d.evtf003.z) = gMapCursorZ;
            gClearSavedPadState = 1;

         } else if ((gSavedPadStateNewPresses & PAD_X) && gSignal1 == 0 &&
                    gMapUnitsPtr[gMapCursorZ][gMapCursorX].s.unitIdx != 0) {
            CloseWindow(0x1e);
            HI(EVT.x) = gMapCursorX;
            HI(EVT.z) = gMapCursorZ;
            gClearSavedPadState = 1;
            gSignal1 = 1;
            evt->state += 2;
            evt->state2 = 0;
         } else if ((gSavedPadStateNewPresses & PAD_TRIANGLE) && gSignal1 == 0 &&
                    !gPlayerControlSuppressed) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_OVERHEAD_MAP_VIEW;
            gClearSavedPadState = 1;
            gSignal1 = 2;
            evt->state += 3;
         }
         break;

      case 1:
         break;

      case 2:

         switch (evt->state2) {
         case 0:
            gSignal2 = 0;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_STATUS_WINDOW;
            // TODO replace
            evt1->d.evtf031.unit = unit = &gUnits[gMapUnitsPtr[HI(EVT.z)][HI(EVT.x)].s.unitIdx];
            evt->state2++;

         // fallthrough
         case 1:
            if (gSignal2 == 99) {
               gClearSavedPadState = 0;
               gSignal1 = 0;
               evt->state = 0;
            }
            break;
         }

         break;

      case 3:
         if (gSignal1 == 99) {
            gClearSavedPadState = 0;
            gPlayerControlSuppressed = 0;
            gSignal1 = 0;
            evt->state = 0;
         }
         break;

      case 4:
         // Preview range for selected enemy or inactive unit
         switch (evt->state2) {
         case 0:
            gState.previewingRange = 1;
            unit = &gUnits[gMapUnitsPtr[s_unitZ_80123204][s_unitX_80123200].s.unitIdx];
            UpdateUnitInfoWindow(unit);
            if (unit->team != TEAM_ENEMY) {
               DisplayBasicWindow(0x1f);
            } else {
               DisplayCustomWindow(0x1f, 0, 1, 0, 0, 23);
            }
            PopulateBlueMovementGrid(s_unitZ_80123204, s_unitX_80123200, unit->travelRange);
            gShowBlueMovementGrid = 1;
            evt->state2++;
            break;

         case 1:
            if (gPadStateNewPresses & PAD_X) {
               gState.previewingRange = 0;
               ClearBlueMovementGrid();
               CloseWindow(0x1f);
               gSignal1 = 0;
               evt->state = 0;
               evt->state2 = 0;
            }
         }

         break;
      } // switch (evt->state)
   }

   switch (EVT.terrainInfoState) {
   case 0:
      DrawWindow(0x39, 396, 0, 112, 36, 8, 188, WBS_CROSSED, 0);
      DisplayCustomWindow(0x39, 2, 1, 0, 0, 0);
      EVT.previousTerrain = -1;
      EVT.terrainInfoState++;
      break;

   case 1:
      if (EVT.previousTerrain != gTerrainPtr[gMapCursorZ][gMapCursorX].terrain) {
         EVT.previousTerrain = gTerrainPtr[gMapCursorZ][gMapCursorX].terrain;
         DrawText(396, 11, 25, 0, 0, terrainText[EVT.previousTerrain]);
      }
      if (gPlayerControlSuppressed || gIsEnemyTurn || gState.inEvent || gClearSavedPadState) {
         CloseWindow(0x39);
         EVT.terrainInfoState++;
      }
      break;

   case 2:
      if (!gPlayerControlSuppressed && !gIsEnemyTurn && !gState.inEvent && !gClearSavedPadState) {
         gWindowChoicesCount = 0;
         DisplayCustomWindow(0x39, 2, 1, 0, 0, 0);
         EVT.previousTerrain = -1;
         EVT.terrainInfoState--;
      }
      break;
   }
}

u16 s_camRotXPhase_80123208;
u16 s_camZoomPhase_8012320c;
s16 s_dstCamRotX_80123210;
s16 s_dstCamRotY_80123214;
s16 s_dstZoom_80123218;
s16 s_dstCamX_8012321c;
s16 s_dstCamY_80123220;
s16 s_dstCamZ_80123224;
s32 s_origMapSizeX_80123228;
s32 s_origMapSizeZ_8012322c;

#undef EVTF
#define EVTF 013
void Evtf013_BattleMgr(EvtData *evt) {
   static s32 iUnit = 0;
   EvtData *unitSprite, *evt1;
   UnitStatus *unit;
   s32 i;
   s32 diff;
   OromeLakeEnemySpawn *pSpawn;

   if (gState.battleEval != BATTLE_EVAL_NONE) {
      evt->functionIndex = EVTF_NULL;
      return;
   }

   unitSprite = EVT.unitSprite;
   unit = EVT.unit;

   switch (evt->state) {
   case 0:
      if (gIsEnemyTurn) {
         s_origMapSizeX_80123228 = gMapSizeX;
         s_origMapSizeZ_8012322c = gMapSizeZ;
         if (gState.mapNum == 29) {
            if (gMapSizeX >= 16) {
               gMapSizeX = 16;
            }
            if (gMapSizeZ >= 16) {
               gMapSizeZ = 16;
            }
         } else if (gState.mapNum == 8) {
            gMapSizeX = 17;
            gMapSizeZ = 17;
         } else {
            if (gMapSizeX >= 18) {
               gMapSizeX = 18;
            }
            if (gMapSizeZ >= 18) {
               gMapSizeZ = 18;
            }
         }

         s_dstCamRotX_80123210 = gCameraRotation.vx;
         s_dstCamRotY_80123214 = gCameraRotation.vy & 0xfff;
         s_dstZoom_80123218 = gCameraZoom.vz;
         s_dstCamX_8012321c = gCameraPos.vx;
         s_dstCamY_80123220 = gCameraPos.vy;
         s_dstCamZ_80123224 = gCameraPos.vz;

         evt->state = 13;
         evt->state2 = 0;
         iUnit = 0;

         if (gState.mapNum == 8) {
            gPlayerControlSuppressed = 0;
            EVT.timer = 1;
            EVT.todo_x2d = 1;
            evt->state = 1;
            evt->state2 = 0;
         }
      }
      break;

   case 1:

      switch (evt->state2) {
      case 0:
         gShowBlueMovementGrid = 0;
         if (--EVT.timer == 0) {
            evt->state2++;
         }
         break;
      case 1:
         gState.needEval = 1;
         evt->state2++;
         break;
      case 2:
         if (gState.battleEval == 0) {
            evt->state++;
         } else {
            evt->state2++;
         }
         break;
      }

      break;

   case 2:
      gState.droppedItem = ITEM_NULL;

   BattleMgr_TryNextUnit:
      do {
         iUnit++;
         if (iUnit == 40) {
            if (gState.mapNum != 8) {
               EVT.todo_x2d = 0;
               gMapSizeX = s_origMapSizeX_80123228;
               gMapSizeZ = s_origMapSizeZ_8012322c;
               iUnit = 1;
               evt->state = 14;
               evt->state2 = 0;
               for (i = 1; i < 40; i++) {
                  unit = &gUnits[i];
                  unit->done = 0;
               }
               return;
            } else {
               iUnit = 0;
               gState.D_80140859 = 1;
            }
         }
         if (iUnit == 39) {
            gState.turn++;
         }

         unit = &gUnits[iUnit];

         // FIXME: More sensible way to match these jumps? Outer loop?
         if (gState.mapNum != 8) {
            if (unit->idx == 0) {
               goto BattleMgr_TryNextUnit;
            }
            if (unit->name == UNIT_LEENA && !unit->paralyzed) {
               break;
            }
            if (unit->idx == 0) {
               goto BattleMgr_TryNextUnit;
            }
            if (unit->name == UNIT_CLIVE && !unit->paralyzed) {
               break;
            }
            if (unit->idx != 0 && unit->team == TEAM_ENEMY && !unit->paralyzed) {
               if (unit->unitType == UNIT_TYPE_DEATH_DEV_) {
                  continue;
               } else {
                  break;
               }
            }
         } else {
            if (unit->idx != 0 && !unit->paralyzed) {
               break;
            }
         }
      } while (1);
      //} while (unit->idx == 0 || unit->paralyzed);

      evt1 = unit->evtSprite;
      EVT.unit = unit;
      EVT.unitSprite = evt1;
      unitSprite = evt1;
      EVT.todo_x2d = 1;
      evt->state++;
      break;

   case 3:
      PopulateBlueMovementGrid(HI(unitSprite->d.sprite.z1), HI(unitSprite->d.sprite.x1),
                               unit->travelRange);
      StartUnitSpritesDecoder(unitSprite->d.sprite.stripIdx);
      evt->state++;
      break;

   case 4:
      if (!gDecodingSprites) {
         UpdateCompactUnitInfoWindow(unit, unit, 0);
         if (unit->name == 30 || unit->name == 31) {
            DisplayCustomWindow(0x1d, 0, 1, 0, 0, 0);
         } else {
            DisplayCustomWindow(0x1d, 0, 1, 0, 0, 23);
         }
         EVT.timer = 15;
         evt->state++;
      }
      break;

   case 5:
      D_80123468 = 0;
      evt1 = Evt_GetLastUnused();
      evt1->functionIndex = EVTF_AI_TBD_570;
      // TODO replace
      HI(evt1->d.sprite.x1) = HI(unitSprite->d.sprite.x1);
      HI(evt1->d.sprite.z1) = HI(unitSprite->d.sprite.z1);
      evt->state++;
      break;

   case 6:
      if (D_80123468 != 0) {
         if (HI(unitSprite->d.sprite.x1) == gX_801233d8 &&
             HI(unitSprite->d.sprite.z1) == gZ_801233dc) {
            evt->state += 2;
            evt->state2 = 0;
         } else {
            evt->state++;
            evt->state2 = 0;
         }
      }
      break;

   case 7:

      switch (evt->state2) {
      case 0:
         PerformAudioCommand(0x1388);
         if (unit->unitType == UNIT_TYPE_DEATH_ANT) {
            SPR_TILESTATE(unitSprite).action = TA_X1D;
            SPR_TILESTATE(unitSprite).cachedByte = 16;
            gSignal3 = 0;
            evt->state2++;
         } else {
            evt->state2 += 2;
         }
         break;

      case 1:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_CLOUD;
         evt1->d.sprite.x1 = unitSprite->d.sprite.x1;
         evt1->d.sprite.z1 = unitSprite->d.sprite.z1;
         evt1->d.sprite.y1 = unitSprite->d.sprite.y1;

         if (gSignal3 != 0) {
            PerformAudioCommand(0x388);
            unitSprite->d.sprite.hidden = 1;
            SPR_TILESTATE(unitSprite).cachedByte = 0;
            gState.mapState.n.field_0x0 = 2;
            gState.field_0x96 = 1;
            evt->state2++;
         }
         break;

      case 2:
         if (gState.field_0x96 == 0) {
            PerformAudioCommand(0x1389);
            gShowBlueMovementGrid = 1;
            PlotPathBackToUnit(gZ_801233dc, gX_801233d8);
            SPR_TILESTATE(unitSprite).action = TA_X6;
            gSignal3 = 0;
            evt->state2++;
         }
         break;

      case 3:
         if (unit->unitType == UNIT_TYPE_ANT_ARM) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_CLOUD;
            evt1->d.sprite.x1 = unitSprite->d.sprite.x1;
            evt1->d.sprite.z1 = unitSprite->d.sprite.z1;
            evt1->d.sprite.y1 = unitSprite->d.sprite.y1;
         }
         if (gSignal3 == 1) {
            gShowBlueMovementGrid = 0;
            if (unit->unitType == UNIT_TYPE_DEATH_ANT) {
               SPR_TILESTATE(unitSprite).action = TA_X1D;
               SPR_TILESTATE(unitSprite).cachedByte = 22;
               EVT.timer = 2;
               gSignal3 = 0;
               evt->state2++;
            } else {
               evt->state2 += 4;
            }
         }
         break;

      case 4:
         if (--EVT.timer == 0) {
            evt->state2++;
            unitSprite->d.sprite.hidden = 0;
         }
         break;

      case 5:
         evt1 = Evt_GetUnused();
         evt1->functionIndex = EVTF_CLOUD;
         evt1->d.sprite.x1 = unitSprite->d.sprite.x1;
         evt1->d.sprite.z1 = unitSprite->d.sprite.z1;
         evt1->d.sprite.y1 = unitSprite->d.sprite.y1;

         if (gSignal3 != 0) {
            SPR_TILESTATE(unitSprite).cachedByte = 0;
            PerformAudioCommand(0x389);
            gState.field_0x96 = 1;
            gState.mapState.n.field_0x0 = 1;
            gState.mapState.n.field_0x1 = gZ_801233dc;
            gState.mapState.n.field_0x2 = gX_801233d8;
            evt->state2++;
         }
         break;

      case 6:
         if (gState.field_0x96 == 0) {
            UpdateElevation();
            evt->state2++;
         }
         break;

      case 7:
         CloseWindow(0x1d);
         unit->direction = unitSprite->d.sprite.direction;
         evt->state++;
         evt->state2 = 0;
         break;

      } // switch (evt->state2) (via state:7)

      break;

   case 8:
      EVT.todo_x2d = 0;
      ClearBlueMovementGrid();
      if (D_8012337C == 0) {
         if (gDir_80123470 == 0xffff) {
            evt->state += 1;
            evt->state2 = 0;
         } else {
            evt->state += 2;
            evt->state2 = 0;
         }
      } else {
         CloseWindow(0x1d);
         if (D_8012337C == 1) {
            evt->state += 3;
            evt->state2 = 0;
         } else {
            evt->state += 4;
            evt->state2 = 0;
         }
      }
      break;

   case 9:

      switch (evt->state2) {
      case 0:
         EVT.timer = 5;
         evt->state2++;
      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            CloseWindow(0x1d);
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 10:

      switch (evt->state2) {
      case 0:
         unit->direction = gDir_80123470;
         SPR_TILESTATE(unitSprite).action = TA_XB;
         EVT.timer = 5;
         evt->state2++;
         // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            CloseWindow(0x1d);
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      }

      break;

   case 11:

      switch (evt->state2) {
      case 0:
         ClearGrid(1);
         if (unit->class == CLASS_ARCHER) {
            PopulateRangedAttackGrid(HI(unitSprite->d.sprite.z1), HI(unitSprite->d.sprite.x1),
                                     unit->attackRange, 0);
         } else {
            PopulateMeleeAttackGrid(HI(unitSprite->d.sprite.z1), HI(unitSprite->d.sprite.x1), 0,
                                    unit->attackRange);
         }
         gYellowTargetGridPtr[gTargetZ_80123418][gTargetX_80123414] += 1;
         EVT.timer = 30;
         evt->state2++;
         break;

      case 1:
         if (--EVT.timer == 0) {
            ClearGrid(0);
            ClearGrid(1);
            evt1 = Evt_GetLastUnused();
            evt1->functionIndex = EVTF_UNIT_ATTACKING;
            HI(evt1->d.evtf021.x) = gX_801233d8;
            HI(evt1->d.evtf021.z) = gZ_801233dc;
            gTargetX = gTargetX_80123414;
            gTargetZ = gTargetZ_80123418;
            gSignal2 = 0;
            evt->state2++;
         }
         break;

      case 2:
         if (gSignal2 == 99) {
            EVT.timer = 5;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:11)

      break;

   case 12:

      switch (evt->state2) {
      case 0:
         ClearGrid(0);
         ClearGrid(1);
         if (gSpells[gCurrentSpell].area == SPELL_AREA_AOE) {
            PopulateCastingGrid(gZ_801233dc, gX_801233d8, gSpells[gCurrentSpell].range, 0);
            PopulateCastingGrid(gTargetZ_80123418, gTargetX_80123414,
                                gSpells[gCurrentSpell].fieldSize, 1);
         } else if (gSpells[gCurrentSpell].targeting >= SPELL_TARGET_ENEMY_GROUP) {
            PopulateCastingGrid(gZ_801233dc, gX_801233d8, gSpells[gCurrentSpell].range, 1);
         } else {
            PopulateCastingGrid(gZ_801233dc, gX_801233d8, gSpells[gCurrentSpell].range, 0);
            gPathGrid1_Ptr[gTargetZ_80123418][gTargetX_80123414] += 1;
         }

         EVT.timer = 50;
         evt->state2++;
         break;

      case 1:
         if (--EVT.timer == 0) {
            ClearGrid(0);
            ClearGrid(1);
            evt1 = Evt_GetLastUnused();
            evt1->functionIndex = EVTF_UNIT_CASTING;
            HI(evt1->d.evtf028.x) = gX_801233d8;
            HI(evt1->d.evtf028.z) = gZ_801233dc;
            gTargetX = gTargetX_80123414;
            gTargetZ = gTargetZ_80123418;
            gSignal2 = 0;
            evt->state2++;
         }
         break;

      case 2:
         if (gSignal2 == 99) {
            EVT.timer = 5;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            evt->state = 99;
            evt->state2 = 0;
         }
         break;
      } // switch (evt->state2) (via state:12)

      break;

   case 13:

      switch (evt->state2) {
      case 0:
         DrawWindow(0x38, 0, 0, 160, 64, 75, 90, WBS_DRAGON, 0);
         // ENEMY TURN
         DrawSjisText(
             20, 24, 20, 0, 0,
             "\x82\x64\x82\x6d\x82\x64\x82\x6c\x82\x78\x81\x40\x82\x73\x82\x74\x82\x71\x82\x6d");
         DisplayCustomWindow(0x38, 2, 1, 0, 0, 0);
         EVT.timer = 45;
         evt->state2++;

      // fallthrough
      case 1:
         if (EVT.timer == 25) {
            PerformAudioCommand(0x20);
         }
         if (--EVT.timer == 0) {
            PerformAudioCommand(6);
            CloseWindow(0x38);
            EVT.timer = 10;
            evt->state2++;
         }
         break;

      case 2:
         if (--EVT.timer == 0) {
            PerformAudioCommand(0x201);
            EVT.timer = 16;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            gState.field_0x98 = 1;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_ENEMY_EVENT;
            evt->state2++;
         }
         break;

      case 4:
         if (gState.field_0x98 == 0) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_TURN_START;
            evt1->d.evtf592.team = TEAM_ENEMY;
            gSignal2 = 0;
            evt->state2++;
         }
         break;

      case 5:
         if (gSignal2 == 99) {
            gState.needEval = 1;
            evt->state2++;
         }
         break;

      case 6:
         EVT.timer = 5;
         evt->state = 1;
         evt->state2 = 0;
         break;
      } // switch (evt->state2) (via state:13)

      break;

   case 14:

      switch (evt->state2) {
      case 0:
         EVT.timer = 1;
         evt->state2++;

      // fallthrough
      case 1:
         if (--EVT.timer == 0) {
            DrawWindow(0x38, 0, 0, 176, 64, 70, 90, WBS_DRAGON, 0);
            // PLAYER TURN
            DrawSjisText(
                20, 24, 20, 0, 0,
                "\x82\x6f\x82\x6b\x82\x60\x82\x78\x82\x64\x82\x71\x81\x40\x82\x73\x82\x74\x82"
                "\x71\x82\x6d");
            DisplayCustomWindow(0x38, 2, 1, 0, 0, 0);
            EVT.timer = 45;
            evt->state2++;
         }
         break;

      case 2:
         if (EVT.timer == 25) {
            PerformAudioCommand(0x20);
         }
         if (EVT.timer == 10) {
            PerformAudioCommand(6);
         }
         if (--EVT.timer == 0) {
            PlayBattleBGM(gState.mapNum);
            CloseWindow(0x38);
            EVT.timer = 20;
            evt->state2++;
         }
         break;

      case 3:
         if (--EVT.timer == 0) {
            gState.field_0x98 = 1;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_TURN_TICKER;
            evt->state2++;
         }
         break;

      case 4:
         if (gState.field_0x98 == 0) {
            gState.field_0x98 = 1;
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_PLAYER_EVENT;
            evt->state2++;
         }
         break;

      case 5:
         if (gState.field_0x98 == 0) {
            evt1 = Evt_GetUnused();
            evt1->functionIndex = EVTF_BATTLE_TURN_START;
            evt1->d.evtf592.team = TEAM_PLAYER;
            gSignal2 = 0;
            evt->state2++;
         }
         break;

      case 6:
         if (gSignal2 == 99) {
            EVT.timer = 20;
            gCameraRotation.vy &= 0xfff;
            diff = s_dstCamRotY_80123214 - gCameraRotation.vy;
            if (diff > 0) {
               if (diff > ANGLE_180_DEGREES) {
                  s_dstCamRotY_80123214 -= ANGLE_360_DEGREES;
               }
            } else if (diff < -ANGLE_180_DEGREES) {
               s_dstCamRotY_80123214 += ANGLE_360_DEGREES;
            }
            evt->state2++;
         }
         break;

      case 7:
         if (--EVT.timer != 0) {
            gCameraRotation.vx += (s_dstCamRotX_80123210 - gCameraRotation.vx) >> 2;
            gCameraRotation.vy += (s_dstCamRotY_80123214 - gCameraRotation.vy) >> 2;
            gCameraZoom.vz += (s_dstZoom_80123218 - gCameraZoom.vz) >> 2;
            gCameraPos.vx += (s_dstCamX_8012321c - gCameraPos.vx) >> 2;
            gCameraPos.vy += (s_dstCamY_80123220 - gCameraPos.vy) >> 2;
            gCameraPos.vz += (s_dstCamZ_80123224 - gCameraPos.vz) >> 2;
         } else {
            gCameraRotation.vx = s_dstCamRotX_80123210;
            gCameraRotation.vy = s_dstCamRotY_80123214;
            gCameraZoom.vz = s_dstZoom_80123218;
            gCameraPos.vx = s_dstCamX_8012321c;
            gCameraPos.vy = s_dstCamY_80123220;
            gCameraPos.vz = s_dstCamZ_80123224;
            evt->state2++;
         }
         return;

      case 8:
         gState.needEval = 1;
         evt->state2++;
         return;

      case 9:
         gIsEnemyTurn = 0;
         evt->state = 0;
         return;
      } // switch (evt->state2) (via state:14)

      break;

   case 99:

      switch (evt->state2) {
      case 0:
         evt1 = unit->evtSprite;
         if (SPR_TERRAIN(evt1).terrain == TERRAIN_VILE_BOG) {
            unit->poisoned = 1;
         }
         if (gState.droppedItem != ITEM_NULL) {
            ShowReceivedItemDialog(gState.droppedItem, 0x3c, 0);
            gState.depot[gState.droppedItem]++;
            EVT.timer = 50;
            evt->state2++;
         } else {
            evt->state2 += 2;
         }
         break;

      case 1:
         if (PressedCircleOrX() || --EVT.timer == 0) {
            CloseWindow(0x3c);
            evt->state2++;
         }
         break;

      case 2:
         if (unit->name != UNIT_LEENA) {
            evt->state += 1;
         } else {
            evt->state += 2;
         }
         break;
      } // switch (evt->state2) (via state:99)

      break;

   case 100:
      ClearGrid(0);
      ClearGrid(1);
      EVT.timer = 5;
      evt->state = 1;
      evt->state2 = 0;
      break;

   case 101:
      if (gState.mapState.n.field_0x0 != 2 &&
          (HI(unitSprite->d.sprite.z1) == 11 && HI(unitSprite->d.sprite.x1) == 16)) {
         SetupBattleMsgBox(UNIT_LEENA, PORTRAIT_LEENA, 17);
         evt->state++;
      } else if (gState.mapState.n.field_0x0 != 3 &&
                 (HI(unitSprite->d.sprite.z1) == 2 && HI(unitSprite->d.sprite.x1) == 10)) {
         SetupBattleMsgBox(UNIT_LEENA, PORTRAIT_LEENA, 18);
         evt->state++;
      } else {
         evt->state--;
      }
      break;

   case 102:
      gPlayerControlSuppressed = 1;
      evt->state++;
      break;

   case 103:
      if (gState.msgFinished) {
         if (HI(unitSprite->d.sprite.z1) == 11) {
            gState.mapState.n.field_0x0 = 2;
         } else {
            gState.mapState.n.field_0x0 = 3;
         }
         gState.field_0x96 = 1;
         EVT.timer = 60;
         evt->state++;
      }
      break;

   case 104:
      if (--EVT.timer == 0) {
         if (HI(unitSprite->d.sprite.z1) == 11) {
            pSpawn = &sOromeLakeEnemySpawns1[0];
         } else {
            pSpawn = &sOromeLakeEnemySpawns2[0];
         }
         while (pSpawn->z != 0) {
            SetupBattleUnit(pSpawn->stripIdx, pSpawn->z, pSpawn->x, pSpawn->level, TEAM_ENEMY,
                            DIR_WEST, 100, 12, 99);
            pSpawn++;
         }
         evt->state++;
      }
      break;

   case 105:
      if (gState.field_0x96 == 0) {
         evt->state -= 5;
      }
      break;

   } // switch (evt->state)

   if (!gPlayerControlSuppressed) {
      s16 a, b;
      if (EVT.todo_x2d) {
         gCameraPos.vx += (-(unitSprite->d.sprite.x1 >> 3) - gCameraPos.vx) >> 4;
         gCameraPos.vz += (-(unitSprite->d.sprite.z1 >> 3) - gCameraPos.vz) >> 4;
         gCameraPos.vy += (((unitSprite->d.sprite.y1 + 0x100) >> 3) - gCameraPos.vy) >> 4;
      }
      if (!gPlayerControlSuppressed && gIsEnemyTurn && gState.cameraMode == CAMERA_MODE_DYNAMIC) {
         gCameraRotation.vy -= 8;
         s_camRotXPhase_80123208 += 16;
         s_camZoomPhase_8012320c += 24;
         a = ((rcos(s_camRotXPhase_80123208 & 0xfff) * 256) >> 12) + 384;
         b = ((rcos(s_camZoomPhase_8012320c & 0xfff) * 150) >> 12) + 650;
         gCameraRotation.vx += (a - gCameraRotation.vx) >> 3;
         gCameraZoom.vz += (b - gCameraZoom.vz) >> 3;
      }
   }
}
