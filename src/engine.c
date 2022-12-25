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

void UpdateEngine(void);
void Evtf047_BattleMapCursor(EvtData *);
void Evtf049_BattleMapCursorControl(EvtData *);
void Evtf019_Compass(EvtData *);
void Evtf025_OverheadMapView(EvtData *);
void Noop_800190c0(void);
void CalculateSubstepDeltas(s32, s32, s32, s32, s32, u32, s32 *, s32 *);
s32 SmoothStepTo(EvtData *, s32, s32, s32);
void CalculateProjectileDeltas(s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 ArcProjectileTo(EvtData *, s32, s32, s32, s32);
void RotateProjectile(s16, s16, s16);
void Evtf022_029_Projectile(EvtData *);
void Evtf023_Camera_RangedTarget(EvtData *);
s32 CalculateProjectileHeight(s8, s8, s8, s8);

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

      HI(evt->d.sprite.x1) = x;
      HI(evt->d.sprite.z1) = z;

      if (gTerrainPtr[z][x].terrain != TERRAIN_NO_ENTRY) {
         SetElevationFromTerrain(evt);
      } else {
         evt->d.sprite.y1 = gMapRowPointers[z][x].height * 128;
      }

      evt->d.sprite.coords[0].y = evt->d.sprite.y1;
      evt->d.sprite.coords[1].y = evt->d.sprite.y1;
      evt->d.sprite.coords[2].y = evt->d.sprite.y1;
      evt->d.sprite.coords[3].y = evt->d.sprite.y1;

      LO(evt->d.sprite.x1) = 0x80;
      LO(evt->d.sprite.z1) = 0x80;

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
      HI(evt->d.sprite.x1) = gMapCursorStartingPos[gState.mapNum].x;
      HI(evt->d.sprite.z1) = gMapCursorStartingPos[gState.mapNum].z;

      if (gState.primary == STATE_LOAD_IN_BATTLE_SAVE) {
         HI(evt->d.sprite.x1) = gDeferredInBattleSaveData.mapCursorX;
         HI(evt->d.sprite.z1) = gDeferredInBattleSaveData.mapCursorZ;
      }
      // snap to tile center
      LO(evt->d.sprite.x1) = 0x80;
      LO(evt->d.sprite.z1) = 0x80;

      gMapCursorX = 0;
      gMapCursorZ = 0;

      for (gMapCursorX = 0; gMapCursorX <= HI(evt->d.sprite.x1); gMapCursorX++) {
         CenterCamera(1);
      }
      for (gMapCursorZ = 0; gMapCursorZ <= HI(evt->d.sprite.z1); gMapCursorZ++) {
         CenterCamera(1);
      }

      evt->state++;
      break;
   case 1:
      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_SOUTH:
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
      case CAM_DIR_EAST:
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
      case CAM_DIR_NORTH:
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
      case CAM_DIR_WEST:
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

   if (HI(evt->d.sprite.x1) < gMapMinX) {
      HI(evt->d.sprite.x1) = gMapMinX;
   }
   if (HI(evt->d.sprite.x1) > gMapMaxX) {
      HI(evt->d.sprite.x1) = gMapMaxX;
   }
   if (HI(evt->d.sprite.z1) < gMapMinZ) {
      HI(evt->d.sprite.z1) = gMapMinZ;
   }
   if (HI(evt->d.sprite.z1) > gMapMaxZ) {
      HI(evt->d.sprite.z1) = gMapMaxZ;
   }

   gMapCursorX = HI(evt->d.sprite.x1);
   gMapCursorZ = HI(evt->d.sprite.z1);

   if (!gPlayerControlSuppressed && !gIsEnemyTurn) {
      gCameraPos.vy += ((evt->d.sprite.y1 >> 3) - gCameraPos.vy) >> 2;
   }
}

void Evtf019_Compass(EvtData *evt) {
   static VECTOR compassOffset = {0, 0, 0x300, 0};
   static Quad compassQuad = {{48, 0, -48, 0}, {-48, 0, -48, 0}, {48, 0, 48, 0}, {-48, 0, 48, 0}};
   static Quad compassLetterQuads[4] = {
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
   Quad *quadp;

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

      RotTransPers4(&compassQuad[0], &compassQuad[1], &compassQuad[2], &compassQuad[3],
                    (s32 *)&compassPoly->x0, (s32 *)&compassPoly->x1, (s32 *)&compassPoly->x2,
                    (s32 *)&compassPoly->x3, &p, &flag);
      // setcode(compassPoly, 0x2c); setRGB0(compassPoly, 0x80, 0x80, 0x80);
      *(u32 *)&compassPoly->r0 = 0x2c808080;
      addPrim(&gGraphicsPtr->ot[OT_SIZE - 5], compassPoly);

      // Shadow
      SetGeomOffset(270, 58);
      RotTransPers4(&compassQuad[0], &compassQuad[1], &compassQuad[2], &compassQuad[3],
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
      quadp = &compassLetterQuads[0];
      for (i = 0; i < 4; i++, gfx++, quadp++) {
         compassPoly->clut = clut;
         compassPoly->tpage = tpage;

         x = gGfxSubTextures[gfx].x;
         y = gGfxSubTextures[gfx].y;
         w = gGfxSubTextures[gfx].w;
         h = gGfxSubTextures[gfx].h;
         setUVWH(compassPoly, x, y, w, h);

         RotTransPers4(&(*quadp)[0], &(*quadp)[1], &(*quadp)[2], &(*quadp)[3],
                       (s32 *)&compassPoly->x0, (s32 *)&compassPoly->x1, (s32 *)&compassPoly->x2,
                       (s32 *)&compassPoly->x3, &p, &flag);
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

void CalculateSubstepDeltas(s32 currentXorZ, s32 currentY, s32 destXorZ, s32 destY, s32 rise,
                            u32 substeps, s32 *deltaXorZ, s32 *deltaY) {
   // rise is usually ~7000
   *deltaXorZ = (destXorZ - currentXorZ) / (s32)substeps;
   *deltaY = ((destY - currentY) + (((s32)substeps + 1) * (s32)substeps) / 2 * rise) / substeps;
}

typedef struct SubstepProperties {
   u8 substeps;
   s16 rise;
} SubstepProperties;

s32 s_rise_80123144;
s32 s_currentX_80123148;
s32 s_destX_8012314c;
s32 s_currentZ_80123150;
s32 s_destZ_80123154;
s32 s_currentY_80123158;
s32 s_destY_8012315c;
s32 s_deltaX_80123160;
s32 s_deltaY_80123164;
s32 s_deltaZ_80123168;
s16 s_isubstep_8012316c;
u8 s_substeps_80123170;

s32 SmoothStepTo(EvtData *sprite, s32 destZ, s32 destX, s32 cont) {
   // for each [elevation diff]
   static SubstepProperties ascendingSubsteps[5] = {
       {3, 7200}, {4, 7000}, {7, 7000}, {8, 7000}, {9, 7000}};
   static SubstepProperties levelOrDescendingSubsteps[5] = {
       {3, 7000}, {4, 7000}, {7, 7000}, {8, 7000}, {9, 7000}};
   s32 unused[3];
   s8 elevationDiff;
   u8 destElevation, currentElevation;
   s32 diff;

   if (cont == 0) {
      s_currentX_80123148 = sprite->d.sprite.x1 << 8;
      s_destX_8012314c = destX << 8;

      s_currentY_80123158 = sprite->d.sprite.y1 << 8;
      s_destY_8012315c = gTerrainPtr[destZ >> 8][destX >> 8].elevation << 8 << 7;

      s_currentZ_80123150 = sprite->d.sprite.z1 << 8;
      s_destZ_80123154 = destZ << 8;

      destElevation = gTerrainPtr[destZ >> 8][destX >> 8].elevation;
      currentElevation = gTerrainPtr[HI(sprite->d.sprite.z1)][HI(sprite->d.sprite.x1)].elevation;
      diff = destElevation - currentElevation;
      elevationDiff = diff;
      if (elevationDiff > 0) {
         // Climbing
         s_substeps_80123170 = ascendingSubsteps[elevationDiff].substeps;
         s_rise_80123144 = ascendingSubsteps[elevationDiff].rise;
      } else {
         // Descending or level
         elevationDiff = -elevationDiff;
         s_substeps_80123170 = levelOrDescendingSubsteps[elevationDiff].substeps;
         s_rise_80123144 = levelOrDescendingSubsteps[elevationDiff].rise;
      }

      if (s_currentX_80123148 != s_destX_8012314c) {
         // Moving along X axis
         CalculateSubstepDeltas(s_currentX_80123148, s_currentY_80123158, s_destX_8012314c,
                                s_destY_8012315c, s_rise_80123144, s_substeps_80123170,
                                &s_deltaX_80123160, &s_deltaY_80123164);
         s_deltaZ_80123168 = 0;
      } else {
         // Moving along Z axis
         CalculateSubstepDeltas(s_currentZ_80123150, s_currentY_80123158, s_destZ_80123154,
                                s_destY_8012315c, s_rise_80123144, s_substeps_80123170,
                                &s_deltaZ_80123168, &s_deltaY_80123164);
         s_deltaX_80123160 = 0;
      }
      s_isubstep_8012316c = 0;

      // For cont:0, return 1 where elevation is level
      return elevationDiff == 0;
   } else {
      s_currentX_80123148 += s_deltaX_80123160;
      s_currentZ_80123150 += s_deltaZ_80123168;

      s_deltaY_80123164 -= s_rise_80123144;
      s_currentY_80123158 += s_deltaY_80123164;

      sprite->d.sprite.x1 = s_currentX_80123148 >> 8;
      sprite->d.sprite.z1 = s_currentZ_80123150 >> 8;
      sprite->d.sprite.y1 = s_currentY_80123158 >> 8;
      s_isubstep_8012316c++;

      if (s_isubstep_8012316c == s_substeps_80123170) {
         sprite->d.sprite.x1 = s_destX_8012314c >> 8;
         sprite->d.sprite.z1 = s_destZ_80123154 >> 8;
         sprite->d.sprite.y1 = s_destY_8012315c >> 8;

         // For cont:1, return 1 when movement completes
         return 1;
      }
   }
   return 0;
}

void CalculateProjectileDeltas(s32 currentXorZ, s32 currentY, s32 destXorZ, s32 destY, s32 rise,
                               s32 height, s32 *deltaXorZ, s32 *deltaY, s32 *substeps) {
   // TODO: Better match? Got close (~25) w/o register overrides or do-while, but moving on for now.
   s32 a, b;

   do {
      currentXorZ >>= 8;
      destY >>= 8;
      rise >>= 8;
      destXorZ >>= 8;
      currentY >>= 8;
      height >>= 8;
      destY -= height;
   } while (0);

   *deltaY = -(SquareRoot12(((abs(height) * rise) >> 8) << 5) >> 4);
   b = -(*deltaY / rise);
   a = ((destY - currentY) / rise * 2) << 12;
   *substeps = b + (SquareRoot12(-a) >> 12);
   *deltaXorZ = ((destXorZ - currentXorZ) / *substeps) << 8;
   *deltaY = -(*deltaY << 8);
}

s32 s_currentX_80123174;
s32 s_destX_80123178;
s32 s_currentZ_8012317c;
s32 s_destZ_80123180;
s32 s_currentY_80123184;
s32 s_destY_80123188;
s16 s_isubstep_8012318c;

s32 ArcProjectileTo(EvtData *sprite, s32 destX, s32 destZ, s32 height, s32 cont) {
   static s32 deltaX = 0;
   static s32 deltaY = 0;
   static s32 deltaZ = 0;
   static s32 substeps = 0;
   static s32 rise = 3000;
   static const s32 unused = 0; //<- to fix sdata spacing

   if (cont == 0) {
      s_currentX_80123174 = sprite->d.sprite.x1 << 8;
      s_destX_80123178 = destX << 8;
      s_currentY_80123184 = sprite->d.sprite.y1 << 8;
      s_destY_80123188 = (-gMapRowPointers[destZ >> 8][destX >> 8].vertices[0].vy + 16) * 0x800;
      s_currentZ_8012317c = sprite->d.sprite.z1 << 8;
      s_destZ_80123180 = destZ << 8;
      if (s_currentX_80123174 != s_destX_80123178) {
         CalculateProjectileDeltas(s_currentX_80123174, s_currentY_80123184, s_destX_80123178,
                                   s_destY_80123188, rise, height, &deltaX, &deltaY, &substeps);
         deltaZ = 0;
      } else {
         CalculateProjectileDeltas(s_currentZ_8012317c, s_currentY_80123184, s_destZ_80123180,
                                   s_destY_80123188, rise, height, &deltaZ, &deltaY, &substeps);
         deltaX = 0;
      }
      s_isubstep_8012318c = 0;
      return 0;
   } else {
      s_currentX_80123174 += deltaX;
      s_currentZ_8012317c += deltaZ;
      s_currentY_80123184 += deltaY;
      deltaY -= rise;
      sprite->d.sprite.y2 = deltaY >> 2;
      if (deltaZ == 0) {
         sprite->d.sprite.x2 = deltaX >> 2;
      } else {
         sprite->d.sprite.x2 = deltaZ >> 2;
      }
      sprite->d.sprite.x1 = s_currentX_80123174 >> 8;
      sprite->d.sprite.z1 = s_currentZ_8012317c >> 8;
      sprite->d.sprite.y1 = s_currentY_80123184 >> 8;
      s_isubstep_8012318c++;
      // For cont:1, return 1 when movement completes
      return s_isubstep_8012318c == substeps;
   }
}

void RotateProjectile(s16 rotx, s16 roty, s16 rotz) {
   s32 flag;
   Quad quad;
   VECTOR transformedVtx[4];
   MATRIX matrix;
   SVECTOR rotation;
   VECTOR translation;

   // .sdata: Initialized in pieces to fit
   quad[0] = ((SVECTOR){-15, -6, 0, 0});
   quad[1] = ((SVECTOR){15, -6, 0, 0});
   quad[2] = ((SVECTOR){-15, 6, 0, 0});
   quad[3] = ((SVECTOR){15, 6, 0, 0});

   // .rdata (rhs)
   translation = ((VECTOR){0, 0, 0, 0});

   rotation.vz = rotz;
   rotation.vx = rotx;
   rotation.vy = roty;

   PushMatrix();
   RotMatrix(&rotation, &matrix);
   TransMatrix(&matrix, &translation);
   SetRotMatrix(&matrix);
   SetTransMatrix(&matrix);

   RotTrans(&quad[0], &transformedVtx[0], &flag);
   RotTrans(&quad[1], &transformedVtx[1], &flag);
   RotTrans(&quad[2], &transformedVtx[2], &flag);
   RotTrans(&quad[3], &transformedVtx[3], &flag);

   gQuad_800fe63c[0].vx = (s16)transformedVtx[0].vx;
   gQuad_800fe63c[0].vy = (s16)transformedVtx[0].vy;
   gQuad_800fe63c[0].vz = (s16)transformedVtx[0].vz;

   gQuad_800fe63c[1].vx = (s16)transformedVtx[1].vx;
   gQuad_800fe63c[1].vy = (s16)transformedVtx[1].vy;
   gQuad_800fe63c[1].vz = (s16)transformedVtx[1].vz;

   gQuad_800fe63c[2].vx = (s16)transformedVtx[2].vx;
   gQuad_800fe63c[2].vy = (s16)transformedVtx[2].vy;
   gQuad_800fe63c[2].vz = (s16)transformedVtx[2].vz;

   gQuad_800fe63c[3].vx = (s16)transformedVtx[3].vx;
   gQuad_800fe63c[3].vy = (s16)transformedVtx[3].vy;
   gQuad_800fe63c[3].vz = (s16)transformedVtx[3].vz;

   PopMatrix();
}

void Evtf022_029_Projectile(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *projectileSprite;
   EvtData *evtParticles;
   s32 unitIdx;
   s32 projectileHeight;
   UnitStatus *target, *attacker;
   s16 mdist, xmdist, zmdist;
   s8 unused[24];

   projectileSprite = evt->d.evtf022.sprite;
   target = &gUnits[gMapUnitsPtr[gTargetZ][gTargetX].unitIdx];
   unitIdx = gMapUnitsPtr[HI(evt->d.evtf022.z)][HI(evt->d.evtf022.x)].unitIdx;
   attacker = &gUnits[unitIdx];

   if (evt->functionIndex == EVTF_PROJECTILE) {
      targetSprite = target->evtSprite;
   } else {
      // EVTF_PROJECTILE_INDIRECT: Unused? maybe to open chest w/ ranged attacker?
      targetSprite = gTempGfxEvt;
      HI(targetSprite->d.sprite.x1) = LO(gTargetX);
      HI(targetSprite->d.sprite.z1) = LO(gTargetZ);
      LO(targetSprite->d.sprite.x1) = 0x80;
      LO(targetSprite->d.sprite.z1) = 0x80;
      targetSprite->d.sprite.y1 = GetTerrainHeight(LO(gTargetZ), LO(gTargetX));
   }

   switch (evt->state) {
   case 0:
      projectileSprite = Evt_GetUnused();
      projectileSprite->functionIndex = EVTF_NOOP;

      if (attacker->unitId == UNIT_ID_GRENADIER) {
         projectileSprite->d.sprite.gfxIdx = GFX_BOMB;
         projectileSprite->d.sprite.vecIdx = 4;
      } else if (attacker->unitId == UNIT_ID_BASILISK || attacker->unitId == UNIT_ID_M_CANNON ||
                 attacker->unitId == UNIT_ID_EGGWORM || attacker->unitId == UNIT_ID_BUGABOO) {
         projectileSprite->d.sprite.gfxIdx = GFX_EXPLOSION_3;
         projectileSprite->d.sprite.vecIdx = 4;
      } else {
         projectileSprite->d.sprite.gfxIdx = GFX_ARROW;
         projectileSprite->d.sprite.vecIdx = 7;
      }

      evt->d.evtf022.sprite = projectileSprite;
      projectileSprite->d.sprite.x1 = evt->d.evtf022.x;
      projectileSprite->d.sprite.z1 = evt->d.evtf022.z;
      projectileSprite->d.sprite.y1 = evt->d.evtf022.y;

      projectileHeight = CalculateProjectileHeight(
          HI(projectileSprite->d.sprite.x1), HI(projectileSprite->d.sprite.z1),
          HI(targetSprite->d.sprite.x1), HI(targetSprite->d.sprite.z1));
      ArcProjectileTo(projectileSprite, targetSprite->d.sprite.x1, targetSprite->d.sprite.z1,
                      projectileHeight, 0);
      evt->d.evtf022.xMid = evt->d.evtf022.x + ((targetSprite->d.sprite.x1 - evt->d.evtf022.x) / 2);
      evt->d.evtf022.zMid = evt->d.evtf022.z + ((targetSprite->d.sprite.z1 - evt->d.evtf022.z) / 2);

      evt->d.evtf022.xzMidDist = (targetSprite->d.sprite.x1 - evt->d.evtf022.x) / 2;
      SET_ABS(evt->d.evtf022.xzMidDist);

      evt->d.evtf022.zMidDist = (targetSprite->d.sprite.z1 - evt->d.evtf022.z) / 2;
      SET_ABS(evt->d.evtf022.zMidDist);

      evt->d.evtf022.xzMidDist += evt->d.evtf022.zMidDist;

      if (projectileSprite->d.sprite.x1 == targetSprite->d.sprite.x1) {
         evt->d.evtf022.yRotOfs = ANGLE_90_DEGREES;
         evt->d.evtf022.zRotOfs = ANGLE_180_DEGREES;
      }
      evt->state++;
      ArcProjectileTo(projectileSprite, 0, 0, 0, 1);
      break;
   case 1:
      evtParticles = Evt_GetUnused();
      evtParticles->d.sprite.x1 = projectileSprite->d.sprite.x1;
      evtParticles->d.sprite.y1 = projectileSprite->d.sprite.y1;
      evtParticles->d.sprite.z1 = projectileSprite->d.sprite.z1;

      if (attacker->unitId == UNIT_ID_DIEGO_SNIPER || attacker->unitId == UNIT_ID_AMON_SNIPER ||
          attacker->unitId == UNIT_ID_DARIUS_SNIPER || attacker->unitId == UNIT_ID_LANDO ||
          attacker->unitId == UNIT_ID_SABINA) {
         evtParticles->functionIndex = EVTF_SPARKLE_DUST;
      }
      if (attacker->unitId == UNIT_ID_DIEGO_BOWMAN || attacker->unitId == UNIT_ID_KIRA_BOWMAN ||
          attacker->unitId == UNIT_ID_AMON_BOWMAN || attacker->unitId == UNIT_ID_DARIUS_BOWMAN) {
         evtParticles->functionIndex = EVTF_PROJECTILE_TRAIL_SPARKLES;
      }
      if (attacker->unitId == UNIT_ID_BUGABOO || attacker->unitId == UNIT_ID_EGGWORM) {
         evtParticles->functionIndex = EVTF_PROJECTILE_TRAIL_POISON;
      }
      if (attacker->unitId == UNIT_ID_BASILISK || attacker->unitId == UNIT_ID_M_CANNON) {
         evtParticles->functionIndex = EVTF_PROJECTILE_TRAIL_EXPLOSION;
      }
      if (attacker->unitId == UNIT_ID_KIRA_SNIPER) {
         evtParticles->functionIndex = EVTF_PROJECTILE_TRAIL_SMOKE;
      }

      if (ArcProjectileTo(projectileSprite, 0, 0, 0, 1)) {
         gSignal3 = 1;
         gTempDirection = 1;
         evt->functionIndex = EVTF_NULL;
         projectileSprite->functionIndex = EVTF_NULL;
      }

      xmdist = evt->d.evtf022.xMid - projectileSprite->d.sprite.x1;
      SET_ABS(xmdist);
      zmdist = evt->d.evtf022.zMid - projectileSprite->d.sprite.z1;
      SET_ABS(zmdist);
      mdist = xmdist + zmdist;

      // Shifts compute 3/4ths
      gCameraZoom.vz = 250 + ((evt->d.evtf022.xzMidDist - mdist) >> 1) +
                       ((evt->d.evtf022.xzMidDist - mdist) >> 2);

      if (evt->functionIndex == EVTF_NULL) {
         gCameraZoom.vz = 250;
         gCameraRotation.vx = 0x180;
      }
      break;
   }

   gCameraPos.vx = -(projectileSprite->d.sprite.x1 >> 3);
   gCameraPos.vz = -(projectileSprite->d.sprite.z1 >> 3);
   gCameraPos.vy = projectileSprite->d.sprite.y1 >> 3;

   // x2,y2 (delta) set in ArcProjectileTo
   evt->d.evtf022.zRot = ratan2(projectileSprite->d.sprite.y2, projectileSprite->d.sprite.x2);

   if (evt->d.evtf022.yRotOfs == 0) {
      evt->d.evtf022.zRot = ANGLE_360_DEGREES - evt->d.evtf022.zRot;
   }

   evt->d.evtf022.zRot += evt->d.evtf022.zRotOfs;
   RotateProjectile(gCameraRotation.vx, gCameraRotation.vy + evt->d.evtf022.yRotOfs,
                    gCameraRotation.vz + evt->d.evtf022.zRot);
   AddEvtPrim6(gGraphicsPtr->ot, projectileSprite, 0);
}

void Evtf023_Camera_RangedTarget(EvtData *evt) {
   s16 diff;
   EvtData *targetSprite = evt->d.evtf023.targetSprite;

   switch (evt->state) {
   case 0:
      evt->d.evtf023.yRotDst =
          GetBestViewOfTarget(HI(targetSprite->d.sprite.z1), HI(targetSprite->d.sprite.x1), 1);

      diff = evt->d.evtf023.yRotDst - gCameraRotation.vy;
      if (diff > 0) {
         if (diff > ANGLE_180_DEGREES) {
            evt->d.evtf023.yRotDst -= ANGLE_360_DEGREES;
         }
      } else if (diff < -ANGLE_180_DEGREES) {
         evt->d.evtf023.yRotDst += ANGLE_360_DEGREES;
      }

      evt->d.evtf023.delay = 35;
      evt->state++;
      // fallthrough
   case 1:
      if (--evt->d.evtf023.delay != 0) {
         gCameraRotation.vy += (evt->d.evtf023.yRotDst - gCameraRotation.vy) >> 3;
      } else {
         evt->functionIndex = EVTF_NULL;
      }
      break;
   }
}

s32 CalculateProjectileHeight(s8 x1, s8 z1, s8 x2, s8 z2) {
   // TODO: Less hacky match
   s8 ox, oz;
   s32 xstep = 0, zstep = 0;
   s32 highest;
   // s32 startingElevation; // = gTerrainPtr[z1][x1].elevation;
   register s32 startingElevation asm("t6");

   ox = x1;
   oz = z1;
   startingElevation = gTerrainPtr[z1][x1].elevation;

   if (x1 > x2) {
      xstep = -1;
   }
   if (x1 < x2) {
      xstep = 1;
   }
   if (z1 > z2) {
      zstep = -1;
   }
   if (z1 < z2) {
      zstep = 1;
   }

   highest = startingElevation;

   do {
      x1 += xstep;
      z1 += zstep;

      if (gTerrainPtr[z1][x1].terrain != TERRAIN_NO_ENTRY &&
          gTerrainPtr[z1][x1].terrain != TERRAIN_OBSTACLE) {
         if (gTerrainPtr[z1][x1].elevation > highest) {
            highest = gTerrainPtr[z1][x1].elevation;
         }
      } else {
         if (gMapRowPointers[z1][x1].height > highest) {
            highest = gMapRowPointers[z1][x1].height;
         }
      }
   } while (x1 != x2 || z1 != z2);

   highest = (highest - startingElevation) << 15;

   do {
      ox = ox - x2;
      SET_ABS(ox);
      oz = oz - z2;
      SET_ABS(oz);
   } while (0);

   return highest + ((s8)(ox + oz) << 14);
}
