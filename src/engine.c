#include "common.h"
#include "audio.h"
#include "state.h"
#include "evt.h"
#include "units.h"
#include "window.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"
#include "card.h"

#include "PsyQ/kernel.h"

void UpdateEngine(void) {
   u32 osc;
   s32 flag;

   UpdateAudio();
   rand();
   if (gState.vsyncMode == 2) {
      gState.frameCounter += 2;
   } else {
      gState.frameCounter += 1;
   }
   if (gState.debug) {
      /* while holding 2P Circle to freeze, press 2P R1 to frame-step */
      while ((PadRead(1) >> 0x10) & PAD_CIRCLE) {
         UpdateInput();
         if (gPad2StateNewPresses & PAD_R1)
            break;
      }
      /* 1P Select: setup transition to state 98 (debug menu) */
      if (gState.debug && gPadStateNewPresses & PAD_SELECT) {
         PerformAudioCommand(6);
         gState.primary = 1;
      }
      /* 2P Start: win battle */
      if (gState.debug && (gPad2StateNewPresses & PAD_START)) {
         gState.battleEval = BATTLE_EVAL_VICTORY;
      }
   }
   gState.unitMarkerSpin += 0x20;
   osc = (gOscillation + 0x100) & 0xfff;
   gOscillation = osc;
   gGridColorOscillation = rcos(osc) * 100 >> 12;
   gGridColorOscillation += 155U;
   gQuadIndex = 0;
   UpdateInput();
   UpdateCamera();
   UpdateLight(0x38);
   gGraphicsPtr = (gGraphicsPtr == &gGraphicBuffers[0]) ? &gGraphicBuffers[1] : &gGraphicBuffers[0];
   ClearOTag(gGraphicsPtr->ot, OT_SIZE);
   CenterCamera(0);
   RotTrans(&gCameraPos, (VECTOR *)gCameraMatrix.t, &flag);
   SetTransMatrix(&gCameraMatrix);
   if (!gState.fieldRenderingDisabled) {
      RenderField();
   }
   Evt_Execute();
   DrawSync(1);
   FntPrint("VSYNC : %d\n", GetRCnt(RCntCNT1));
   DrawSync(0);
   DecodeUnitSprites();
   if (gState.vsyncNoWait && (gPad2State & PAD_L1)) {
      VSync(1);
   } else {
      VSync(gState.vsyncMode);
   }
   ResetRCnt(RCntCNT1);
   PutDrawEnv(&gGraphicsPtr->drawEnv);
   PutDispEnv(&gGraphicsPtr->dispEnv);
   DrawOTag(gGraphicsPtr->ot);
   FntFlush(-1);
}

u16 s_mapCursorGhostY;

void Evtf047_BattleMapCursor(EvtData *evt) {
   // static u16 mapCursorGhostY = 0;
   // extern s16 gMapCursorAnimData[];
   static s16 mapCursorAnimData[] = {0, GFX_MAP_CURSOR_1,
                                     1, GFX_MAP_CURSOR_2,
                                     1, GFX_MAP_CURSOR_3,
                                     1, GFX_MAP_CURSOR_4,
                                     1, GFX_MAP_CURSOR_5,
                                     1, GFX_MAP_CURSOR_6,
                                     1, 0,
                                     1, 0};
   s16 i;
   s16 x, z;

   if (!gIsEnemyTurn && !gPlayerControlSuppressed && !gClearSavedPadState &&
       !gMapCursorSuppressed) {

      if (gState.mapCursorOutOfRange) {
         evt->d.sprite.clut = 3;
      } else {
         evt->d.sprite.clut = 0;
      }

      evt->d.sprite.animData = mapCursorAnimData;

      z = gMapCursorZ;
      x = gMapCursorX;

      evt->d.sprite.coords[0].x = x * 0x100;
      evt->d.sprite.coords[2].x = x * 0x100;
      evt->d.sprite.coords[1].x = x * 0x100 + 0xff;
      evt->d.sprite.coords[3].x = x * 0x100 + 0xff;

      evt->d.sprite.coords[0].z = z * 0x100;
      evt->d.sprite.coords[2].z = z * 0x100 + 0xff;
      evt->d.sprite.coords[1].z = z * 0x100;
      evt->d.sprite.coords[3].z = z * 0x100 + 0xff;

      H_HI(evt->d.sprite.x1) = x;
      H_HI(evt->d.sprite.z1) = z;

      if (gTerrainPtr[z][x].terrain != TERRAIN_NO_ENTRY) {
         SetElevationFromTerrain(evt);
      } else {
         evt->d.sprite.y1 = gMapRowPointers[z][x].height * 128;
      }

      evt->d.sprite.coords[0].y = evt->d.sprite.y1;
      evt->d.sprite.coords[1].y = evt->d.sprite.y1;
      evt->d.sprite.coords[2].y = evt->d.sprite.y1;
      evt->d.sprite.coords[3].y = evt->d.sprite.y1;

      H_LO(evt->d.sprite.x1) = 0x80;
      H_LO(evt->d.sprite.z1) = 0x80;

      evt->d.sprite.otOfs = -2;
      evt->d.sprite.semiTrans = 0;
      UpdateEvtAnimation(evt);
      AddEvtPrim5(gGraphicsPtr->ot, evt);

      if (gCameraRotation.vx <= 0x300) {
         evt->d.sprite.semiTrans = 4;

         s_mapCursorGhostY += 18;
         s_mapCursorGhostY &= 0xff;

         evt->d.sprite.coords[0].y += s_mapCursorGhostY;
         evt->d.sprite.coords[1].y += s_mapCursorGhostY;
         evt->d.sprite.coords[2].y += s_mapCursorGhostY;
         evt->d.sprite.coords[3].y += s_mapCursorGhostY;

         for (i = 0; i < 13; i++) {
            evt->d.sprite.y1 = evt->d.sprite.coords[0].y;
            AddEvtPrim5(gGraphicsPtr->ot, evt);
            evt->d.sprite.coords[0].y += 0x100;
            evt->d.sprite.coords[1].y += 0x100;
            evt->d.sprite.coords[2].y += 0x100;
            evt->d.sprite.coords[3].y += 0x100;
         }
      }
   }
}

void Evtf049_BattleMapCursorControl(EvtData *evt) {
   static s32 mapCursorSavedPadState = 0;
   static s32 mapCursorPadHoldCounter = 0;
   u32 pad;

   if (gState.signal != 0) {
      return;
   }
   if (gSavedPadState & PAD_X) {
      // holding X jump-starts the counter, accelerating cursor movement
      mapCursorPadHoldCounter = 30;
   }

   pad = gSavedPadStateNewPresses;

   if (mapCursorSavedPadState == gSavedPadState) {
      mapCursorPadHoldCounter++;
   } else {
      // input changed: save new value and reset counter
      mapCursorSavedPadState = gSavedPadState;
      mapCursorPadHoldCounter = 0;
   }
   if (mapCursorPadHoldCounter > 30) {
      // repeat saved input; UpdateInput() repeats d-pad input as well, so this just gives an
      // intermittent "bump" (besides enabling the x-button boost)
      pad = gSavedPadState;
      mapCursorPadHoldCounter = 18;
   }

   SetElevationFromTerrain(evt);

   switch (evt->state) {
   case 0:
      H_HI(evt->d.sprite.x1) = gMapCursorStartingPos[gState.mapNum].x;
      H_HI(evt->d.sprite.z1) = gMapCursorStartingPos[gState.mapNum].z;

      if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
         H_HI(evt->d.sprite.x1) = gDeferredInBattleSaveData.mapCursorX;
         H_HI(evt->d.sprite.z1) = gDeferredInBattleSaveData.mapCursorZ;
      }
      // snap to tile center
      H_LO(evt->d.sprite.x1) = 0x80;
      H_LO(evt->d.sprite.z1) = 0x80;

      gMapCursorX = 0;
      gMapCursorZ = 0;

      for (gMapCursorX = 0; gMapCursorX <= H_HI(evt->d.sprite.x1); gMapCursorX++) {
         CenterCamera(1);
      }
      for (gMapCursorZ = 0; gMapCursorZ <= H_HI(evt->d.sprite.z1); gMapCursorZ++) {
         CenterCamera(1);
      }

      evt->state++;
      break;
   case 1:
      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case DIR_SOUTH:
         if (pad & PAD_UP) {
            evt->d.sprite.z1 += 0x100;
         }
         if (pad & PAD_DOWN) {
            evt->d.sprite.z1 -= 0x100;
         }
         if (pad & PAD_LEFT) {
            evt->d.sprite.x1 -= 0x100;
         }
         if (pad & PAD_RIGHT) {
            evt->d.sprite.x1 += 0x100;
         }
         break;
      case DIR_WEST:
         if (pad & PAD_UP) {
            evt->d.sprite.x1 -= 0x100;
         }
         if (pad & PAD_DOWN) {
            evt->d.sprite.x1 += 0x100;
         }
         if (pad & PAD_LEFT) {
            evt->d.sprite.z1 -= 0x100;
         }
         if (pad & PAD_RIGHT) {
            evt->d.sprite.z1 += 0x100;
         }
         break;
      case DIR_NORTH:
         if (pad & PAD_UP) {
            evt->d.sprite.z1 -= 0x100;
         }
         if (pad & PAD_DOWN) {
            evt->d.sprite.z1 += 0x100;
         }
         if (pad & PAD_LEFT) {
            evt->d.sprite.x1 += 0x100;
         }
         if (pad & PAD_RIGHT) {
            evt->d.sprite.x1 -= 0x100;
         }
         break;
      case DIR_EAST:
         if (pad & PAD_UP) {
            evt->d.sprite.x1 += 0x100;
         }
         if (pad & PAD_DOWN) {
            evt->d.sprite.x1 -= 0x100;
         }
         if (pad & PAD_LEFT) {
            evt->d.sprite.z1 += 0x100;
         }
         if (pad & PAD_RIGHT) {
            evt->d.sprite.z1 -= 0x100;
         }
         break;
      }
   }

   if (H_HI(evt->d.sprite.x1) < gMapMinX) {
      H_HI(evt->d.sprite.x1) = gMapMinX;
   }
   if (H_HI(evt->d.sprite.x1) > gMapMaxX) {
      H_HI(evt->d.sprite.x1) = gMapMaxX;
   }
   if (H_HI(evt->d.sprite.z1) < gMapMinZ) {
      H_HI(evt->d.sprite.z1) = gMapMinZ;
   }
   if (H_HI(evt->d.sprite.z1) > gMapMaxZ) {
      H_HI(evt->d.sprite.z1) = gMapMaxZ;
   }

   gMapCursorX = H_HI(evt->d.sprite.x1);
   gMapCursorZ = H_HI(evt->d.sprite.z1);

   if (!gPlayerControlSuppressed && !gIsEnemyTurn) {
      gCameraPos.vy += ((evt->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;
   }
}

void Evtf019_Compass(EvtData *evt) {
   static VECTOR compassOffset = {0, 0, 0x300, 0};
   static SVECTOR compassVtx[] = {
       {48, 0, -48, 0}, {-48, 0, -48, 0}, {48, 0, 48, 0}, {-48, 0, 48, 0}};
   static SVECTOR compassLetterVtx[4][4] = {
       {{-48, 0, -12, 0}, {-72, 0, -12, 0}, {-48, 0, 12, 0}, {-72, 0, 12, 0}},
       {{72, 0, -12, 0}, {48, 0, -12, 0}, {72, 0, 12, 0}, {48, 0, 12, 0}},
       {{12, 0, 48, 0}, {-12, 0, 48, 0}, {12, 0, 75, 0}, {-12, 0, 72, 0}},
       {{12, 0, -60, 0}, {-12, 0, -60, 0}, {12, 0, -36, 0}, {-12, 0, -36, 0}}};

   POLY_FT4 *shadowPoly;
   POLY_FT4 *compassPoly;
   // POLY_FT4 *letterPoly;
   u8 x, y, w, h;
   u16 tpage, clut;
   s32 p, flag, i;
   s32 gfx;
   SVECTOR(*vtxp)[4];

   if (!gState.inEvent) {
      PushMatrix();
      SetGeomOffset(270, 50);
      TransMatrix(&gCameraMatrix, &compassOffset);
      SetTransMatrix(&gCameraMatrix);

      x = gGfxSubTextures[GFX_COMPASS].x;
      y = gGfxSubTextures[GFX_COMPASS].y;
      w = gGfxSubTextures[GFX_COMPASS].w;
      h = gGfxSubTextures[GFX_COMPASS].h;

      compassPoly = &gGraphicsPtr->quads[gQuadIndex++];
      shadowPoly = &gGraphicsPtr->quads[gQuadIndex++];

      compassPoly->clut = shadowPoly->clut = gGfxClutIds[GFX_COMPASS];
      compassPoly->tpage = shadowPoly->tpage = gGfxTPageIds[GFX_COMPASS];

      setUVWH(compassPoly, x, y, w, h);
      setUVWH(shadowPoly, x, y, w, h);

      RotTransPers4(&compassVtx[0], &compassVtx[1], &compassVtx[2], &compassVtx[3],
                    (s32 *)&compassPoly->x0, (s32 *)&compassPoly->x1, (s32 *)&compassPoly->x2,
                    (s32 *)&compassPoly->x3, &p, &flag);
      // setcode(compassPoly, 0x2c); setRGB0(compassPoly, 0x80, 0x80, 0x80);
      *(u32 *)&compassPoly->r0 = 0x2c808080;
      addPrim(&gGraphicsPtr->ot[OT_SIZE - 5], compassPoly);

      // Shadow
      SetGeomOffset(270, 58);
      RotTransPers4(&compassVtx[0], &compassVtx[1], &compassVtx[2], &compassVtx[3],
                    (s32 *)&shadowPoly->x0, (s32 *)&shadowPoly->x1, (s32 *)&shadowPoly->x2,
                    (s32 *)&shadowPoly->x3, &p, &flag);
      *(u32 *)&shadowPoly->r0 = 0x2c000000;
      addPrim(&gGraphicsPtr->ot[OT_SIZE - 6], shadowPoly);

      // E/W/S/N
      SetGeomOffset(270, 50);
      compassPoly = &gGraphicsPtr->quads[gQuadIndex];
      clut = gGfxClutIds[GFX_E];
      tpage = gGfxTPageIds[GFX_E];
      gfx = GFX_E;
      vtxp = &compassLetterVtx[0];
      for (i = 0; i < 4; i++, gfx++, vtxp++) {
         compassPoly->clut = clut;
         compassPoly->tpage = tpage;

         x = gGfxSubTextures[gfx].x;
         y = gGfxSubTextures[gfx].y;
         w = gGfxSubTextures[gfx].w;
         h = gGfxSubTextures[gfx].h;
         setUVWH(compassPoly, x, y, w, h);

         RotTransPers4(&(*vtxp)[0], &(*vtxp)[1], &(*vtxp)[2], &(*vtxp)[3], (s32 *)&compassPoly->x0,
                       (s32 *)&compassPoly->x1, (s32 *)&compassPoly->x2, (s32 *)&compassPoly->x3,
                       &p, &flag);
         *(u32 *)&compassPoly->r0 = 0x2c808080;
         addPrim(&gGraphicsPtr->ot[OT_SIZE - 5], compassPoly);

         compassPoly++;
         gQuadIndex++;
      }
      SetGeomOffset(gGeomOffsetX, gGeomOffsetY);
      PopMatrix();
   }
}

void Evtf025_OverheadMapView(EvtData *evt) {
   static s16 overheadMapZoomLevels[BATTLE_CT] = {
       1300, 1500, 1700, 1350, 1550, 3550, 1500, 1500, 1900, 1500, 1400, 1500, 1500, 1450, 1500,
       1650, 1450, 1600, 2200, 1900, 2400, 1700, 1400, 1400, 1600, 1650, 2750, 1500, 1850, 2350,
       1950, 2000, 3550, 1850, 2000, 1900, 2000, 1750, 1950, 1650, 2550, 1450, 1550, 1650};

   switch (evt->state) {
   case 0:
      CloseWindow(0x1e);
      gOverheadMapState = 1;
      gCameraRotation.vy &= 0xfff;
      evt->d.evtf025.camSavedRotX = gCameraRotation.vx;
      evt->d.evtf025.camSavedRotY = gCameraRotation.vy;
      evt->d.evtf025.camSavedZoom = gCameraZoom.vz;
      evt->d.evtf025.camSavedX = gCameraPos.vx;
      evt->d.evtf025.camSavedY = gCameraPos.vy;
      evt->d.evtf025.camSavedZ = gCameraPos.vz;
      evt->state++;
      // fallthrough
   case 1:
      gPlayerControlSuppressed = 1;
      evt->state++;
      break;
   case 2:
      gCameraZoom.vz += (overheadMapZoomLevels[gState.mapNum] - gCameraZoom.vz) >> 2;
      gCameraRotation.vx += (0x400 - gCameraRotation.vx) >> 2;
      gCameraRotation.vy += (0x800 - gCameraRotation.vy) >> 2;
      // TODO: better representation?
      // gCameraPos.vx += ((~gMapMaxX / 2) * 0x20 - gCameraPos.vx >> 2);
      // gCameraPos.vz += ((~gMapMaxZ / 2) * 0x20 - gCameraPos.vz >> 2);
      gCameraPos.vx += (((-(gMapMaxX + 1) / 2) * 32) - gCameraPos.vx) >> 2;
      gCameraPos.vz += (((-(gMapMaxZ + 1) / 2) * 32) - gCameraPos.vz) >> 2;
      gCameraPos.vy += -gCameraPos.vy >> 2;

      if ((gPadStateNewPresses & PAD_TRIANGLE) || (gPadStateNewPresses & PAD_X)) {
         gOverheadMapState = 0;
         evt->d.evtf025.delay = 25;
         evt->state++;
      }
      break;
   case 3:
      gCameraRotation.vx += (evt->d.evtf025.camSavedRotX - gCameraRotation.vx) >> 2;
      gCameraRotation.vy += (evt->d.evtf025.camSavedRotY - gCameraRotation.vy) >> 2;
      gCameraZoom.vz += (evt->d.evtf025.camSavedZoom - gCameraZoom.vz) >> 2;
      gCameraPos.vx += (evt->d.evtf025.camSavedX - gCameraPos.vx) >> 2;
      gCameraPos.vz += (evt->d.evtf025.camSavedZ - gCameraPos.vz) >> 2;
      gCameraPos.vy += (evt->d.evtf025.camSavedY - gCameraPos.vy) >> 2;
      if (--evt->d.evtf025.delay == 0) {
         gCameraRotation.vx = evt->d.evtf025.camSavedRotX;
         gCameraRotation.vy = evt->d.evtf025.camSavedRotY;
         gCameraZoom.vz = evt->d.evtf025.camSavedZoom;
         gCameraPos.vx = evt->d.evtf025.camSavedX;
         gCameraPos.vy = evt->d.evtf025.camSavedY;
         gCameraPos.vz = evt->d.evtf025.camSavedZ;
         gSignal1 = 99;
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

void Noop_800190c0(void) { Noop_800c72c0(); }
