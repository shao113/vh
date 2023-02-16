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

extern EnemyEventSpawn gEnemyEventSpawns[6];
extern MATRIX gMatrix_8012c068;

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
         DrawWindow(66, 0, 0, 320, 72, 330, 80, WBS_DRAGON, 0);
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
         DrawText(0x14, 24, 20, 0, 0, "Begin Battle");
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
         pSpawn = &gEnemyEventSpawns[0];

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

////// TODO: Temporary //////
const s8 RData_Temp1[] = "Action\nDone\nExamine\nPush";
const s8 RData_Temp2[] = "you didn't find anything!";
const s8 RData_Temp3[] = "Attack\nMagic\nItem";
const s8 RData_Temp4[] = "Attack\nItem";
const s8 RData_Temp5[] = "Move\nAction\nDone\nExamine\nPush";
const s8 RData_Temp6[] = "BATTLE CONDITION\nTURN OVER\nCHANGE ZOOM\nSTATUS\nOPTIONS\nSAVE\nLOAD";
const s8 RData_Temp7[] = "Close\nMedium\nDistant";
const s8 RData_Temp8[] =
    "\x82\x64\x82\x6d\x82\x64\x82\x6c\x82\x78\x81\x40\x82\x73\x82\x74\x82\x71\x82\x6d";
const s8 RData_Temp9[] =
    "\x82\x6f\x82\x6b\x82\x60\x82\x78\x82\x64\x82\x71\x81\x40\x82\x73\x82\x74\x82\x71\x82\x6d";
////// TODO: Temporary //////
