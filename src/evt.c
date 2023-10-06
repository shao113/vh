#include "common.h"
#include "evt.h"
#include "state.h"
#include "graphics.h"
#include "audio.h"
#include "field.h"
#include "units.h"
#include "battle.h"

void Evtf581_AudioCommand(EvtData *evt) {
   switch (evt->state) {
   case 0:
      if ((evt->d.evtf581.delay != 0) && (--evt->d.evtf581.delay != 0)) {
         return;
      }
      evt->state++;
   // fallthrough
   case 1:
      PerformAudioCommand(evt->d.evtf581.cmd);
      evt->functionIndex = EVTF_NULL;
   }
}

void Evt_ResetByFunction(s16 functionIndex) {
   s32 i;
   EvtData *evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, evt++) {
      if (evt->functionIndex == functionIndex) {
         evt->functionIndex = EVTF_NULL;
      }
   }
}

EvtData *Evt_FindByFunction(s16 functionIndex) {
   s32 i;
   gTempEvt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, gTempEvt++) {
      if (gTempEvt->functionIndex == functionIndex) {
         return gTempEvt;
      }
   }
   // return NULL;
}

void Evtf407_NoopIncState(EvtData *evt) {
   // ?
   s32 tmp;
   if (evt->state == 0) {
      tmp = (u16)evt->state;
      evt->state = tmp + 1;
   }
}

#undef EVTF
#define EVTF 405
void Evtf405_Panorama(EvtData *evt) {
   EvtData *spr;
   s32 i, iy;
   // s32 ix, gfx; //<- to match, these were combined into "i"
   s16 xOfs = 0;
   s16 yOfs = 0;

   switch (evt->state) {
   case 0:
      evt->state++;
   case 1:
      evt->x1.n += CV(1.0);
      xOfs = (EVT.yRot - GetCamRotY()) >> 4;
      xOfs += evt->x1.s.hi;
      if (evt->x1.s.hi != 0) {
         evt->x1.s.hi = 0;
      }
      yOfs = (EVT.xRot - GetCamRotX()) >> 5;
      EVT.yRot = GetCamRotY();
      EVT.xRot = GetCamRotX();
   }

   EVT.xOffset += xOfs;
   EVT.yOffset += yOfs;
   EVT.xOffset &= 0x7f;
   EVT.yOffset &= 0x7f;
   spr = Evt_GetUnused();

   switch (gState.mapNum) {
   case 10:
   case 16:
   case 24:
   case 29:
      i = GFX_PANORAMA_DEEP_BLUE;
      break;
   case 8:
   case 13:
   case 15:
   case 30:
   case 34:
   case 39:
      i = GFX_PANORAMA_2;
      break;
   case 12:
   case 19:
   case 26:
   case 35:
   case 42:
      i = GFX_PANORAMA_3;
      break;
   case 14:
   case 18:
   case 28:
   case 33:
   case 36:
      i = GFX_PANORAMA_4;
      break;
   case 22:
   case 23:
   case 37:
      i = GFX_PANORAMA_RED;
      break;
   case 11:
   case 17:
   case 25:
   case 32:
   case 38:
   case 40:
   case 41:
   case 63:
      i = GFX_PANORAMA_6;
      break;
   case 0:
   case 1:
   case 2:
   case 3:
   case 4:
   case 5:
   case 21:
   case 58:
   case 64:
   case 65:
   case 66:
   case 67:
   case 68:
      i = GFX_PANORAMA_8;
      break;
   default:
      i = GFX_PANORAMA_7;
      break;
   }

   spr->d.sprite.gfxIdx = i;

   for (i = 0; i < 4; i++) {
      for (iy = 0; iy < 3; iy++) {
         spr->x1.n = i * 128 - EVT.xOffset;
         spr->y1.n = iy * 128 + EVT.yOffset - 128;
         spr->x3.n = spr->x1.n + 128;
         spr->y3.n = spr->y1.n + 128;
         AddEvtPrim_Panorama(gGraphicsPtr->ot, spr);
      }
   }
}

void Evt_Execute(void) {
   EvtData *evt;
   s32 i;

   evt = gEvtDataArray;

   for (i = 0; i < EVT_DATA_CT; i++, evt++) {
      if (evt->functionIndex != EVTF_NULL) {
         (*gEvtFunctionPointers[evt->functionIndex])(evt);
      }
   }
}

void DecodeUnitSprites(void) {
   EvtData *evt = &gEvtData050_UnitSpritesDecoder;
   if (evt->functionIndex != EVTF_NULL) {
      (*gEvtFunctionPointers[EVTF_UNIT_SPRITES_DECODER])(evt);
   }
}

void Evtf001_Noop(EvtData *evt) { return; }

void Evt_ResetFromIdx10(void) {
   s32 i;
   for (i = 10; i < EVT_DATA_CT; i++) {
      gTempEvt = &gEvtDataArray[i];
      gTempEvt->functionIndex = EVTF_NULL;
   }
}

void Evt_ResetAll(void) {
   s32 i;
   for (i = 0; i < EVT_DATA_CT; i++) {
      gTempEvt = &gEvtDataArray[i];
      gTempEvt->functionIndex = EVTF_NULL;
   }
}

EvtData *Evt_GetUnused(void) {
   s32 i;
   EvtData *p;
   // TBD What is the significance of first 20 entries? reserved?
   p = &gEvtDataArray[20];
   for (i = 20; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

EvtData *Evt_GetFirstUnused(void) {
   s32 i;
   EvtData *p;
   p = &gEvtDataArray[0];
   for (i = 0; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

EvtData *Evt_GetLastUnused(void) { return Evt_GetLastUnusedSkippingTail(0); }

EvtData *Evt_GetLastUnusedSkippingTail(s32 tailEntriesToSkip) {
   s32 i;
   EvtData *p;
   p = gEvtDataArray + EVT_DATA_LAST_IDX - tailEntriesToSkip;
   // Ensures scan doesn't enter first 20 entries (reserved?)
   for (i = 0; i < (EVT_DATA_CT - 20) - tailEntriesToSkip; i++, p--) {
      if (p->functionIndex == EVTF_NULL) {
         ((u32 *)p)[2] = 0;
         ((u32 *)p)[3] = 0;
         ((u32 *)p)[4] = 0;
         ((u32 *)p)[5] = 0;
         ((u32 *)p)[6] = 0;
         ((u32 *)p)[7] = 0;
         ((u32 *)p)[8] = 0;
         ((u32 *)p)[9] = 0;
         ((u32 *)p)[10] = 0;
         ((u32 *)p)[11] = 0;
         ((u32 *)p)[12] = 0;
         ((u32 *)p)[13] = 0;
         ((u32 *)p)[14] = 0;
         ((u32 *)p)[15] = 0;
         ((u32 *)p)[16] = 0;
         ((u32 *)p)[17] = 0;
         ((u32 *)p)[18] = 0;
         ((u32 *)p)[19] = 0;
         ((u32 *)p)[20] = 0;
         ((u32 *)p)[21] = 0;
         ((u32 *)p)[22] = 0;
         ((u32 *)p)[23] = 0;
         return p;
      }
   }
   return NULL;
}

s32 Evt_CountUnused(void) {
   s32 i, ct;
   EvtData *p;
   ct = 0;
   p = &gEvtDataArray[20];
   for (i = 20; i < EVT_DATA_CT; i++, p++) {
      if (p->functionIndex == EVTF_NULL) {
         ct++;
      }
   }
   return ct;
}

extern MATRIX gMatrix_800f2b04;
extern MATRIX gMatrix_800f2b24;
extern MATRIX gMatrix_800f2b44;
extern MATRIX gMatrix_800f2b64;
extern MATRIX gMatrix_800f2b84;
extern MATRIX gMatrix_800f2ba4;
extern MATRIX gMatrix_800f2bc4;
extern MATRIX gMatrix_800f2be4;

void AddEvtPrim3(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   Quad quad;
   s32 p, flag;
   u32 otIdx;
   s32 otz;
   s32 gfx;

   if (!evt->d.sprite.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      evt->vec.vx = evt->x1.n >> 3;
      evt->vec.vz = evt->z1.n >> 3;
      evt->vec.vy = -(evt->y1.n >> 3);

      otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
      otIdx = OT_SIZE - otz + 6;
      if (otIdx - 1 < OT_SIZE - 1) {
         quad[0].vx = evt->d.sprite.coords[0].x >> 3;
         quad[0].vz = evt->d.sprite.coords[0].z >> 3;
         quad[0].vy = -(evt->d.sprite.coords[0].y >> 3);
         quad[1].vx = evt->d.sprite.coords[1].x >> 3;
         quad[1].vz = evt->d.sprite.coords[1].z >> 3;
         quad[1].vy = -(evt->d.sprite.coords[1].y >> 3);
         quad[2].vx = evt->d.sprite.coords[2].x >> 3;
         quad[2].vz = evt->d.sprite.coords[2].z >> 3;
         quad[2].vy = -(evt->d.sprite.coords[2].y >> 3);
         quad[3].vx = evt->d.sprite.coords[3].x >> 3;
         quad[3].vz = evt->d.sprite.coords[3].z >> 3;
         quad[3].vy = -(evt->d.sprite.coords[3].y >> 3);

         gfx = evt->d.sprite.gfxIdx;
         if (evt->d.sprite.clut == CLUT_NULL) {
            poly->clut = gGfxClutIds[gfx];
         } else {
            poly->clut = gClutIds[evt->d.sprite.clut];
         }

         setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
                 gGfxSubTextures[gfx].h);
         RotTransPers4(&quad[0], &quad[1], &quad[2], &quad[3], &poly->x0, &poly->x1, &poly->x2,
                       &poly->x3, &p, &flag);
         setRGB0(poly, 0x80, 0x80, 0x80);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }
   }
}

void AddEvtPrim4(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   Quad quad;
   s32 p, flag;
   u32 otIdx;
   s32 gfx;
   s32 otz;

   if (!evt->d.sprite.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];

      quad[0].vx = evt->d.sprite.coords[0].x >> 3;
      quad[0].vz = evt->d.sprite.coords[0].z >> 3;
      quad[0].vy = -(evt->d.sprite.coords[0].y >> 3);
      quad[1].vx = evt->d.sprite.coords[1].x >> 3;
      quad[1].vz = evt->d.sprite.coords[1].z >> 3;
      quad[1].vy = -(evt->d.sprite.coords[1].y >> 3);
      quad[2].vx = evt->d.sprite.coords[2].x >> 3;
      quad[2].vz = evt->d.sprite.coords[2].z >> 3;
      quad[2].vy = -(evt->d.sprite.coords[2].y >> 3);
      quad[3].vx = evt->d.sprite.coords[3].x >> 3;
      quad[3].vz = evt->d.sprite.coords[3].z >> 3;
      quad[3].vy = -(evt->d.sprite.coords[3].y >> 3);

      gfx = evt->d.sprite.gfxIdx;
      if (evt->d.sprite.clut == CLUT_NULL) {
         poly->clut = gGfxClutIds[gfx];
      } else {
         poly->clut = gClutIds[evt->d.sprite.clut];
      }

      setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
              gGfxSubTextures[gfx].h);
      otz = RotAverage4(&quad[0], &quad[1], &quad[2], &quad[3], &poly->x0, &poly->x1, &poly->x2,
                        &poly->x3, &p, &flag);
      otIdx = OT_SIZE - otz + evt->d.sprite.otOfs;

      if (otIdx - 1 < OT_SIZE - 1) {
         setRGB0(poly, 0x80, 0x80, 0x80);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }
   }
}

void AddEvtPrim5(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   Quad quad;
   s32 p, flag;
   u32 otIdx;
   s32 otz;
   s32 gfx;

   if (!evt->d.sprite.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      evt->vec.vx = evt->x1.n >> 3;
      evt->vec.vz = evt->z1.n >> 3;
      evt->vec.vy = -(evt->y1.n >> 3);

      otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
      otIdx = OT_SIZE - otz + 6 + evt->d.sprite.otOfs;
      if (otIdx - 1 < OT_SIZE - 1) {
         quad[0].vx = evt->d.sprite.coords[0].x >> 3;
         quad[0].vz = evt->d.sprite.coords[0].z >> 3;
         quad[0].vy = -(evt->d.sprite.coords[0].y >> 3);
         quad[1].vx = evt->d.sprite.coords[1].x >> 3;
         quad[1].vz = evt->d.sprite.coords[1].z >> 3;
         quad[1].vy = -(evt->d.sprite.coords[1].y >> 3);
         quad[2].vx = evt->d.sprite.coords[2].x >> 3;
         quad[2].vz = evt->d.sprite.coords[2].z >> 3;
         quad[2].vy = -(evt->d.sprite.coords[2].y >> 3);
         quad[3].vx = evt->d.sprite.coords[3].x >> 3;
         quad[3].vz = evt->d.sprite.coords[3].z >> 3;
         quad[3].vy = -(evt->d.sprite.coords[3].y >> 3);

         gfx = evt->d.sprite.gfxIdx;
         if (evt->d.sprite.clut == CLUT_NULL) {
            poly->clut = gGfxClutIds[gfx];
         } else {
            poly->clut = gClutIds[evt->d.sprite.clut];
         }

         setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
                 gGfxSubTextures[gfx].h);
         RotTransPers4(&quad[0], &quad[1], &quad[2], &quad[3], &poly->x0, &poly->x1, &poly->x2,
                       &poly->x3, &p, &flag);
         setRGB0(poly, 0x80, 0x80, 0x80);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }
   }
}

void AddEvtPrim6(u32 *ot, EvtData *evt, s32 useMapElevation) {
   POLY_FT4 *poly;
   Quad *quadp;
   MATRIX *matrix;
   s32 p, flag;
   u32 otIdx;
   s32 gfx;
   s32 otz;

   if (!evt->d.sprite.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      matrix = evt->d.sprite.facingLeft ? &gMatrix_800f2b24 : &gMatrix_800f2b04;
      PushMatrix();

      evt->vec.vx = evt->x1.n >> 3;
      evt->vec.vz = evt->z1.n >> 3;

      if (useMapElevation) {
         if ((evt->x1.s.hi >= D_80122E28) && (evt->x1.s.hi <= gMapSizeX + D_80122E28 - 1) &&
             (evt->z1.s.hi >= D_80122E2C) && (evt->z1.s.hi <= gMapSizeZ + D_80122E2C - 1)) {
            evt->vec.vy = OBJ_TILE_MODEL(evt).vertices[0].vy;
            evt->y1.n = -(evt->vec.vy << 3);
         }
      } else {
         evt->vec.vy = -(evt->y1.n >> 3) - evt->d.sprite.animYOfs;
      }

      RotTrans(&evt->vec, (VECTOR *)&matrix->t, &flag);

      otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
      otIdx = OT_SIZE - otz + 8;

      if (otIdx - 1 < OT_SIZE - 1) {
         SetRotMatrix(matrix);
         SetTransMatrix(matrix);

         gfx = evt->d.sprite.gfxIdx;
         if (evt->d.sprite.clut == CLUT_NULL) {
            poly->clut = gGfxClutIds[gfx];
         } else {
            poly->clut = gClutIds[evt->d.sprite.clut];
         }

         setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
                 gGfxSubTextures[gfx].h);
         quadp = gSpriteBoxQuads[evt->d.sprite.boxIdx];
         RotTransPers4(&(*quadp)[0], &(*quadp)[1], &(*quadp)[2], &(*quadp)[3], &poly->x0, &poly->x1,
                       &poly->x2, &poly->x3, &p, &flag);
         setRGB0(poly, 0x80, 0x80, 0x80);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }

      PopMatrix();
   }
}

void AddEvtPrim7(u32 *ot, EvtData *evt, s32 useMapElevation) {
   POLY_FT4 *poly;
   Quad *quadp;
   MATRIX *matrix;
   s32 p, flag;
   u32 otIdx;
   s32 gfx;
   s32 otz;

   if (!evt->d.sprite.hidden && !IsSpriteOutsideVisibleRange(evt)) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      matrix = evt->d.sprite.facingLeft ? &gMatrix_800f2b64 : &gMatrix_800f2b44;
      PushMatrix();

      evt->vec.vx = evt->x1.n >> 3;
      evt->vec.vz = evt->z1.n >> 3;

      if (useMapElevation) {
         evt->vec.vy = OBJ_TILE_MODEL(evt).vertices[0].vy;
         evt->y1.n = -(evt->vec.vy << 3);
      } else {
         evt->vec.vy = -(evt->y1.n >> 3);
      }

      RotTrans(&evt->vec, (VECTOR *)&matrix->t, &flag);

      if (gCameraRotation.vx > 0x113) {
         otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
         otIdx = OT_SIZE - otz + 5 + ((gCameraRotation.vx - 0x113) >> 6);
      } else {
         otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
         otIdx = OT_SIZE - otz + 5;
      }

      if (otIdx - 1 < OT_SIZE - 1) {
         SetRotMatrix(matrix);
         SetTransMatrix(matrix);

         gfx = evt->d.sprite.gfxIdx;
         if (evt->d.sprite.clut == CLUT_NULL) {
            poly->clut = gGfxClutIds[gfx];
         } else {
            poly->clut = gClutIds[evt->d.sprite.clut];
         }

         setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
                 gGfxSubTextures[gfx].h);
         quadp = gSpriteBoxQuads[evt->d.sprite.boxIdx];
         RotTransPers4(&(*quadp)[0], &(*quadp)[1], &(*quadp)[2], &(*quadp)[3], &poly->x0, &poly->x1,
                       &poly->x2, &poly->x3, &p, &flag);
         setRGB0(poly, 0x80, 0x80, 0x80);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }

      PopMatrix();
   }
}

void AddEvtPrim8(u32 *ot, EvtData *evt, s32 useMapElevation) {
   POLY_FT4 *poly;
   Quad *quadp;
   MATRIX *matrix;
   s32 p, flag;
   u32 otIdx;
   s32 gfx;
   s32 otz;
   s16 y;

   if (!evt->d.sprite.hidden && !IsSpriteOutsideVisibleRange(evt)) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      matrix = evt->d.sprite.facingLeft ? &gMatrix_800f2ba4 : &gMatrix_800f2b84;
      PushMatrix();

      evt->vec.vx = evt->x1.n >> 3;
      evt->vec.vz = evt->z1.n >> 3;

      if (useMapElevation) {
         evt->vec.vy = OBJ_TILE_MODEL(evt).vertices[0].vy;
         evt->y1.n = -(evt->vec.vy << 3);
      } else {
         evt->vec.vy = -(evt->y1.n >> 3);
      }

      RotTrans(&evt->vec, (VECTOR *)&matrix->t, &flag);

      if (gCameraRotation.vx > 0x113) {
         otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
         otIdx = OT_SIZE - otz + 5 + ((gCameraRotation.vx - 0x113) >> 6);
      } else {
         otz = RotTransPers(&evt->vec, &poly->x0, &p, &flag);
         otIdx = OT_SIZE - otz + 5;
      }

      if (otIdx - 1 < OT_SIZE - 1) {
         SetRotMatrix(matrix);
         SetTransMatrix(matrix);

         gfx = evt->d.sprite.gfxIdx;
         if (evt->d.sprite.clut == CLUT_NULL) {
            poly->clut = gGfxClutIds[gfx];
         } else {
            poly->clut = gClutIds[evt->d.sprite.clut];
         }

         setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
                 gGfxSubTextures[gfx].h);
         quadp = gSpriteBoxQuads[evt->d.sprite.boxIdx];

         y = -(*quadp)[0].vy;
         (*quadp)[0].vy = -((rcos(gCameraRotation.vx & 0xfff) * (y >> 1) >> 12) + (y >> 1));
         (*quadp)[1].vy = (*quadp)[0].vy;

         RotTransPers4(&(*quadp)[0], &(*quadp)[1], &(*quadp)[2], &(*quadp)[3], &poly->x0, &poly->x1,
                       &poly->x2, &poly->x3, &p, &flag);
         (*quadp)[0].vy = -y;
         (*quadp)[1].vy = -y;

         setRGB0(poly, gLightColor.r, gLightColor.g, gLightColor.b);

         if (evt->d.sprite.semiTrans) {
            poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
            poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
         } else {
            poly->tpage = gGfxTPageIds[gfx];
            poly->code = GPU_CODE_POLY_FT4;
         }

         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }

      PopMatrix();
   }
}

void RenderUnitSprite(u32 *ot, EvtData *sprite, s32 useMapElevation) {
   Quad *quadp;
   SVECTOR screenXY;
   POLY_FT4 *poly;
   MATRIX *matrix;
   s32 p, flag;
   u32 otIdx;
   s32 gfx;
   s32 otz;
   s32 stripIdx;
   s16 xOfs = 0;
   s16 zOfs = 0;
   u8 view = 0;
   s16 sxOfs = 0;
   s16 szOfs = 0;

   if (IsSpriteOutsideVisibleRange(sprite)) {
      return;
   }

   poly = &gGraphicsPtr->quads[gQuadIndex];
   matrix = sprite->d.sprite.facingLeft ? &gMatrix_800f2be4 : &gMatrix_800f2bc4;
   PushMatrix();

   if (sprite->d.sprite.animSingleAxis) {

      switch ((sprite->d.sprite.direction & 0xfff) >> 10) {
      case DIR_SOUTH:
         zOfs = sprite->d.sprite.animRelativeOfs;
         xOfs = 0;
         break;
      case DIR_WEST:
         zOfs = 0;
         xOfs = sprite->d.sprite.animRelativeOfs;
         break;
      case DIR_NORTH:
         zOfs = -sprite->d.sprite.animRelativeOfs;
         xOfs = 0;
         break;
      case DIR_EAST:
         zOfs = 0;
         xOfs = -sprite->d.sprite.animRelativeOfs;
         break;
      }

   } else {

      switch ((gCameraRotation.vy & 0xfff) >> 10) {
      case CAM_DIR_NORTH:
      case CAM_DIR_SOUTH:
         view = 1;
         break;
      case CAM_DIR_EAST:
      case CAM_DIR_WEST:
         view = 0;
         break;
      }

      switch ((sprite->d.sprite.direction & 0xfff) >> 10) {
      case DIR_SOUTH:
         zOfs = sprite->d.sprite.animRelativeOfs;
         if (view) {
            xOfs = sprite->d.sprite.animRelativeOfs;
         } else {
            xOfs = -sprite->d.sprite.animRelativeOfs;
         }
         break;
      case DIR_WEST:
         if (view) {
            zOfs = sprite->d.sprite.animRelativeOfs;
         } else {
            zOfs = -sprite->d.sprite.animRelativeOfs;
         }
         xOfs = sprite->d.sprite.animRelativeOfs;
         break;
      case DIR_NORTH:
         zOfs = -sprite->d.sprite.animRelativeOfs;
         if (view) {
            xOfs = -sprite->d.sprite.animRelativeOfs;
         } else {
            xOfs = sprite->d.sprite.animRelativeOfs;
         }
         break;
      case DIR_EAST:
         if (view) {
            zOfs = -sprite->d.sprite.animRelativeOfs;
         } else {
            zOfs = sprite->d.sprite.animRelativeOfs;
         }
         xOfs = -sprite->d.sprite.animRelativeOfs;
         break;
      }
   }

   sprite->vec.vx = (sprite->x1.n >> 3) + xOfs;
   sprite->vec.vz = (sprite->z1.n >> 3) + zOfs;

   if (useMapElevation != 0) {
      if (useMapElevation == 1) {
         sprite->vec.vy = OBJ_TILE_MODEL(sprite).vertices[0].vy -
                          gCrateGrid_Ptr[sprite->z1.s.hi][sprite->x1.s.hi] * 32 -
                          sprite->d.sprite.animYOfs;
         sprite->y1.n = -(sprite->vec.vy << 3);
      } else {
         sprite->vec.vy +=
             (OBJ_TILE_MODEL(sprite).vertices[0].vy - sprite->d.sprite.animYOfs - sprite->vec.vy) >>
             2;
         sprite->y1.n = -(sprite->vec.vy << 3);
      }
   } else {
      sprite->vec.vy = -(sprite->y1.n >> 3) - sprite->d.sprite.animYOfs;
   }

   RotTrans(&sprite->vec, (VECTOR *)&matrix->t, &flag);

   if (gCameraRotation.vx > 0x113) {
      otz = RotTransPers(&sprite->vec, &poly->x0, &p, &flag);
      otIdx = OT_SIZE - otz + 4 + ((gCameraRotation.vx - 0x113) >> 6);
   } else {
      otz = RotTransPers(&sprite->vec, &poly->x0, &p, &flag);
      otIdx = OT_SIZE - otz + 5;
   }

   if (otIdx - 1 < OT_SIZE - 1) {
      SetRotMatrix(matrix);
      SetTransMatrix(matrix);

      gfx = sprite->d.sprite.gfxIdx;
      stripIdx = sprite->d.sprite.stripIdx;

      if (sprite->d.sprite.clut == CLUT_NULL) {
         poly->clut = gSpriteStripClutIds[stripIdx];
      } else {
         poly->clut = gClutIds[sprite->d.sprite.clut];
      }

      if (sprite->d.sprite.semiTrans) {
         if (gfx >= 45) {
            if (gfx < 55) {
               // 45..54
               poly->tpage = gTPageIds[(sprite->d.sprite.semiTrans - 1) * 32 + 11];
               gfx -= 45;
            } else {
               // 55..64*
               gfx -= 55;
               poly->tpage = gTPageIds[(sprite->d.sprite.semiTrans - 1) * 32 + 27];
            }
            setUVWH(poly, gTexwWideSpriteSetFrames[gfx].x, gTexwWideSpriteSetFrames[gfx].y,
                    gTexwWideSpriteSetFrames[gfx].w, gTexwWideSpriteSetFrames[gfx].h);
         } else {
            if (gfx < 35) {
               if (gfx < 26) {
                  // 0..25
                  poly->tpage = gTPageIds[(sprite->d.sprite.semiTrans - 1) * 32 + 11];
               } else {
                  // 26..34
                  poly->tpage = gTPageIds[(sprite->d.sprite.semiTrans - 1) * 32 + 27];
               }
               setUVWH(poly, gTexwSpriteSetFrames[gfx].x, gTexwSpriteSetFrames[gfx].y,
                       gTexwSpriteSetFrames[gfx].w, gTexwSpriteSetFrames[gfx].h);
            } else {
               // 35..44: Unit sprite strip, consisting of ten 48x48 pixel frames, spanning
               // across two TPages (with gaps reserved for item icons)
               if (gfx < 40) {
                  // 35..39
                  poly->tpage = gTPageIds[gSpriteStripTPageCells[stripIdx * 2] +
                                          (sprite->d.sprite.semiTrans - 1) * 32];
                  gfx += (stripIdx % 5) * 5;
                  gfx -= 35;
               } else {
                  // 40..44
                  poly->tpage = gTPageIds[gSpriteStripTPageCells[stripIdx * 2 + 1] +
                                          (sprite->d.sprite.semiTrans - 1) * 32];
                  gfx += (stripIdx % 5) * 5;
                  gfx -= 40;
               }
               setUVWH(poly, gTexwSpriteStripFrames[gfx].x, gTexwSpriteStripFrames[gfx].y,
                       gTexwSpriteStripFrames[gfx].w, gTexwSpriteStripFrames[gfx].h);
            }
         }
         poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
      } else {
         // semiTrans:0
         if (gfx >= 45) {
            if (gfx < 55) {
               // 45..54
               poly->tpage = gTPageIds[11];
               gfx -= 45;
            } else {
               // 55..64*
               gfx -= 55;
               poly->tpage = gTPageIds[27];
            }
            setUVWH(poly, gTexwWideSpriteSetFrames[gfx].x, gTexwWideSpriteSetFrames[gfx].y,
                    gTexwWideSpriteSetFrames[gfx].w, gTexwWideSpriteSetFrames[gfx].h);
         } else {
            if (gfx < 35) {
               if (gfx < 26) {
                  // 0..25
                  poly->tpage = gTPageIds[11];
               } else {
                  // 26..34
                  poly->tpage = gTPageIds[27];
               }
               setUVWH(poly, gTexwSpriteSetFrames[gfx].x, gTexwSpriteSetFrames[gfx].y,
                       gTexwSpriteSetFrames[gfx].w, gTexwSpriteSetFrames[gfx].h);
            } else {
               if (gfx < 40) {
                  // 35..39
                  poly->tpage = gSpriteStripTPageIds[stripIdx * 2];
                  gfx += (stripIdx % 5) * 5;
                  gfx -= 35;
               } else {
                  // 40..44
                  poly->tpage = gSpriteStripTPageIds[stripIdx * 2 + 1];
                  gfx += (stripIdx % 5) * 5;
                  gfx -= 40;
               }
               setUVWH(poly, gTexwSpriteStripFrames[gfx].x, gTexwSpriteStripFrames[gfx].y,
                       gTexwSpriteStripFrames[gfx].w, gTexwSpriteStripFrames[gfx].h);
            }
         }
         poly->code = GPU_CODE_POLY_FT4;
      }

      if (sprite->d.sprite.gfxIdx >= 45) {
         // 45..
         sprite->d.sprite.boxIdx = 2;
      } else if (sprite->d.sprite.gfxIdx >= 21 && sprite->d.sprite.gfxIdx <= 30) {
         // 21..30
         sprite->d.sprite.boxIdx = 1;
      } else {
         // ..20, 31..44
         sprite->d.sprite.boxIdx = 0;
      }

      quadp = gSpriteBoxQuads[sprite->d.sprite.boxIdx];
      RotTransPers4(&(*quadp)[0], &(*quadp)[1], &(*quadp)[2], &(*quadp)[3], &poly->x0, &poly->x1,
                    &poly->x2, &poly->x3, &p, &flag);

      if (gState.primary == STATE_4 && gState.preciseSprites) {
         PopMatrix();
         PushMatrix();
         RotTransPers(&sprite->vec, &screenXY, &flag, &flag);

         switch (sprite->d.sprite.boxIdx) {
         case 0:
            if (!sprite->d.sprite.facingLeft) {
               sxOfs = -23;
               szOfs = 24;
            } else {
               szOfs = -23;
               sxOfs = 24;
            }
            break;
         case 1:
            if (!sprite->d.sprite.facingLeft) {
               sxOfs = -31;
               szOfs = 32;
            } else {
               szOfs = -31;
               sxOfs = 32;
            }
            break;
         case 2:
            if (!sprite->d.sprite.facingLeft) {
               sxOfs = -47;
               szOfs = 48;
            } else {
               szOfs = -47;
               sxOfs = 48;
            }
            break;
         }

         //?: Setting x/y with single sw; was this done manually or by some compiler optimization?
         *(u32 *)(&poly->x0) = ((screenXY.vy - 40) << 16) + sxOfs + screenXY.vx;
         *(u32 *)(&poly->x1) = ((screenXY.vy - 40) << 16) + szOfs + screenXY.vx;
         *(u32 *)(&poly->x2) = ((screenXY.vy + 7) << 16) + sxOfs + screenXY.vx;
         *(u32 *)(&poly->x3) = ((screenXY.vy + 7) << 16) + szOfs + screenXY.vx;
      }

      setRGB0(poly, 0x80, 0x80, 0x80);
      if (!sprite->d.sprite.hidden) {
         AddPrim(&ot[otIdx], poly);
         gQuadIndex++;
      }
   }

   PopMatrix();
}

void AddEvtPrim_Gui(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   u32 otIdx;
   s32 gfx;

   if (!evt->d.sprite.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      gfx = evt->d.sprite.gfxIdx;

      if (evt->d.sprite.clut == CLUT_NULL) {
         poly->clut = gGfxClutIds[gfx];
      } else {
         poly->clut = gClutIds[evt->d.sprite.clut];
      }

      setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
              gGfxSubTextures[gfx].h);

      poly->x0 = evt->x1.n;
      poly->x1 = evt->x3.n;
      poly->x2 = evt->x1.n;
      poly->x3 = evt->x3.n;
      poly->y0 = evt->y1.n;
      poly->y1 = evt->y1.n;
      poly->y2 = evt->y3.n;
      poly->y3 = evt->y3.n;

      if (evt->d.sprite.otOfs != 0) {
         otIdx = OT_SIZE - 1 - evt->d.sprite.otOfs;
      } else {
         otIdx = OT_SIZE - 1;
      }

      setRGB0(poly, 0x80, 0x80, 0x80);

      if (evt->d.sprite.semiTrans) {
         poly->tpage = gTPageIds[(evt->d.sprite.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
         poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
      } else {
         poly->tpage = gGfxTPageIds[gfx];
         poly->code = GPU_CODE_POLY_FT4;
      }

      AddPrim(&ot[otIdx], poly);
      gQuadIndex++;
   }
}

void AddEvtPrim2(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   u32 otIdx;
   s32 gfx;

   if (!evt->d.sprite2.hidden) {
      poly = &gGraphicsPtr->quads[gQuadIndex];
      gfx = evt->d.sprite2.gfxIdx;

      if (evt->d.sprite2.clut == CLUT_NULL) {
         poly->clut = gGfxClutIds[gfx];
      } else {
         poly->clut = gClutIds[evt->d.sprite2.clut];
      }

      setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
              gGfxSubTextures[gfx].h);

      poly->x0 = evt->d.sprite2.coords[0].x;
      poly->x1 = evt->d.sprite2.coords[1].x;
      poly->x2 = evt->d.sprite2.coords[2].x;
      poly->x3 = evt->d.sprite2.coords[3].x;
      poly->y0 = evt->d.sprite2.coords[0].y;
      poly->y1 = evt->d.sprite2.coords[1].y;
      poly->y2 = evt->d.sprite2.coords[2].y;
      poly->y3 = evt->d.sprite2.coords[3].y;

      if (evt->d.sprite2.otOfs != 0) {
         otIdx = OT_SIZE - 1 - evt->d.sprite2.otOfs;
      } else {
         otIdx = OT_SIZE - 1;
      }

      setRGB0(poly, 0x80, 0x80, 0x80);

      if (evt->d.sprite2.semiTrans) {
         poly->tpage = gTPageIds[(evt->d.sprite2.semiTrans - 1) * 32 + gGfxTPageCells[gfx]];
         poly->code = GPU_CODE_POLY_FT4 | GPU_CODE_SEMI_TRANS;
      } else {
         poly->tpage = gGfxTPageIds[gfx];
         poly->code = GPU_CODE_POLY_FT4;
      }

      AddPrim(&ot[otIdx], poly);
      gQuadIndex++;
   }
}

void AddEvtPrim_Panorama(u32 *ot, EvtData *evt) {
   POLY_FT4 *poly;
   s32 gfx;

   poly = &gGraphicsPtr->quads[gQuadIndex];
   gfx = evt->d.sprite.gfxIdx;

   if (evt->d.sprite.clut == CLUT_NULL) {
      poly->clut = gGfxClutIds[gfx];
   } else {
      poly->clut = gClutIds[evt->d.sprite.clut];
   }

   setUVWH(poly, gGfxSubTextures[gfx].x, gGfxSubTextures[gfx].y, gGfxSubTextures[gfx].w,
           gGfxSubTextures[gfx].h);

   poly->x0 = evt->x1.n;
   poly->x1 = evt->x3.n;
   poly->x2 = evt->x1.n;
   poly->x3 = evt->x3.n;
   poly->y0 = evt->y1.n;
   poly->y1 = evt->y1.n;
   poly->y2 = evt->y3.n;
   poly->y3 = evt->y3.n;

   setRGB0(poly, 0x80, 0x80, 0x80);
   poly->tpage = gGfxTPageIds[gfx];
   poly->code = GPU_CODE_POLY_FT4;

   AddPrim(&ot[0], poly);
   gQuadIndex++;
}

void RenderOverheadMapUnitMarker(u32 *ot_unused, EvtData *unitSprite, s32 showElevation, u8 team) {
   EvtData *marker;
   s32 osc;
   s16 n, w, s, e;

   marker = Evt_GetUnused();
   if (showElevation) {
      unitSprite->y1.n = OBJ_TERRAIN(unitSprite).s.elevation * CV(0.5) + CV(1.0);
   }

   marker->d.sprite.gfxIdx = (team == TEAM_PLAYER) ? GFX_BLUE_GEM : GFX_RED_GEM;
   marker->x1.s.hi = unitSprite->x1.s.hi;
   marker->x1.s.lo = CV(0.5);
   marker->z1.s.hi = unitSprite->z1.s.hi;
   marker->z1.s.lo = CV(0.5);
   marker->y1.n = unitSprite->y1.n;

   osc = ((rcos(((gOscillation * 2) & 0xfff)) << 6) >> 12) + CV(0.5);

   e = marker->x1.n - osc;
   w = marker->x1.n + osc;
   n = marker->z1.n - osc;
   s = marker->z1.n + osc;

   marker->d.sprite.coords[0].y = marker->y1.n;
   marker->d.sprite.coords[1].y = marker->y1.n;
   marker->d.sprite.coords[2].y = marker->y1.n;
   marker->d.sprite.coords[3].y = marker->y1.n;

   marker->d.sprite.coords[0].x = e;
   marker->d.sprite.coords[1].x = w;
   marker->d.sprite.coords[2].x = e;
   marker->d.sprite.coords[3].x = w;

   marker->d.sprite.coords[0].z = n;
   marker->d.sprite.coords[1].z = n;
   marker->d.sprite.coords[2].z = s;
   marker->d.sprite.coords[3].z = s;

   AddEvtPrim3(gGraphicsPtr->ot, marker);
}

void Noop_8003bb24() {}

void RenderUnitHelpers(u32 *ot_unused, EvtData *unitSprite, u8 team, u8 done, u8 poisoned,
                       u8 paralyzed, u8 hideMarker) {
   EvtData *marker;
   s16 spin;
   s16 slot = 0;

   if ((unitSprite->x1.s.hi >= D_80122E28) && (unitSprite->x1.s.hi <= gMapSizeX + D_80122E28 - 1) &&
       (unitSprite->z1.s.hi >= D_80122E2C) && (unitSprite->z1.s.hi <= gMapSizeZ + D_80122E2C - 1)) {

      if (poisoned) {
         RenderStatusEffectText(gGraphicsPtr->ot, unitSprite, 0, slot++);
      }
      if (paralyzed) {
         RenderStatusEffectText(gGraphicsPtr->ot, unitSprite, 1, slot++);
      }
      if (!hideMarker && !gIsEnemyTurn && !gPlayerControlSuppressed) {
         marker = Evt_GetUnused();
         marker->y1.n = OBJ_TERRAIN(unitSprite).s.elevation * CV(0.5);
         marker->d.sprite.gfxIdx = (team == TEAM_PLAYER) ? GFX_PLAYER_CIRCLE : GFX_ENEMY_CIRCLE;
         marker->x1.n = unitSprite->x1.n;
         marker->z1.n = unitSprite->z1.n;
         marker->d.sprite.coords[0].y = marker->y1.n;
         marker->d.sprite.coords[1].y = marker->y1.n;
         marker->d.sprite.coords[2].y = marker->y1.n;
         marker->d.sprite.coords[3].y = marker->y1.n;

         if (!done) {
            spin = gState.unitMarkerSpin;
         } else {
            if (team == TEAM_PLAYER) {
               spin = DEG(45);
            } else {
               spin = gState.unitMarkerSpin;
            }
         }

         marker->d.sprite.coords[0].x = unitSprite->x1.n + (rcos(spin & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[1].x =
             unitSprite->x1.n + (rcos((spin + DEG(90)) & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[2].x =
             unitSprite->x1.n + (rcos((spin + DEG(270)) & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[3].x =
             unitSprite->x1.n + (rcos((spin + DEG(180)) & 0xfff) * 0xaa >> 12);

         marker->d.sprite.coords[0].z =
             unitSprite->z1.n + (rcos((spin + DEG(90)) & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[1].z =
             unitSprite->z1.n + (rcos((spin + DEG(180)) & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[2].z = unitSprite->z1.n + (rcos(spin & 0xfff) * 0xaa >> 12);
         marker->d.sprite.coords[3].z =
             unitSprite->z1.n + (rcos((spin + DEG(270)) & 0xfff) * 0xaa >> 12);

         marker->d.sprite.otOfs = -2;
         AddEvtPrim5(gGraphicsPtr->ot, marker);
      }
   }
}

void RenderStatusEffectText(u32 *ot_unused, EvtData *unitSprite, u8 paralyzed, u8 slot) {
   EvtData *text;
   s16 osc;
   s16 oneThird, twoThirds, oneAndOneThird;

   osc = ((rcos(gOscillation) * 4) >> 12) + 12;
   oneThird = osc / 3;
   twoThirds = oneThird * 2;
   oneAndOneThird = osc + oneThird;

   gQuad_800fe63c[0].vx = -oneAndOneThird;
   gQuad_800fe63c[0].vy = -twoThirds;

   gQuad_800fe63c[1].vx = oneAndOneThird;
   gQuad_800fe63c[1].vy = -twoThirds;

   gQuad_800fe63c[2].vx = -oneAndOneThird;
   gQuad_800fe63c[2].vy = twoThirds;

   gQuad_800fe63c[3].vx = oneAndOneThird;
   gQuad_800fe63c[3].vy = twoThirds;

   text = Evt_GetUnused();
   text->d.sprite.boxIdx = 7;

   if (paralyzed) {
      text->d.sprite.gfxIdx = GFX_PARALYZED;
   } else {
      text->d.sprite.gfxIdx = GFX_POISONED;
   }

   text->y1.n = unitSprite->y1.n + CV(1.375) + (slot * CV(0.625));
   text->x1.n = unitSprite->x1.n;
   text->z1.n = unitSprite->z1.n;

   AddEvtPrim6(gGraphicsPtr->ot, text, 0);
}

void SetElevationFromTerrain(EvtData *evt) {
   if (evt->x1.s.hi < gMapMinX || evt->x1.s.hi > gMapMaxX || evt->z1.s.hi < gMapMinZ ||
       evt->z1.s.hi > gMapMaxZ) {
      evt->y1.n = 0;
   } else {
      evt->y1.n = OBJ_TERRAIN(evt).s.elevation * CV(0.5);
   }
}

s16 GetTerrainElevation(s8 z, s8 x) {
   if (x < gMapMinX || x > gMapMaxX || z < gMapMinZ || z > gMapMaxZ) {
      return 0;
   } else {
      return gTerrainPtr[z][x].s.elevation * CV(0.5);
   }
}

s32 GetMapModelElevation(s8 z, s8 x) {
   if (x < gMapMinX || x > gMapMaxX || z < gMapMinZ || z > gMapMaxZ) {
      return 0;
   } else {
      return gMapRowPointers[z][x].height * CV(0.5);
   }
}

void UpdateEvtAnimation(EvtData *evt) {
   s16 *animData;

   /* AnimData shorts:
    boxIdx,
    (gfxIdx, delay), (gfxIdx, delay), ... */

   animData = (s16 *)evt->d.sprite.animData;

   if (!evt->d.sprite.animInitialized) {
      evt->state3 = 1;
   }

   switch (evt->state3) {
   case 0:
      break;
   case 1:
      evt->d.sprite.animDataIdx = 1;
      evt->d.sprite.animFinished = 0;
      evt->d.sprite.animInitialized = 1;
      evt->d.sprite.boxIdx = animData[0];
      evt->d.sprite.currentFrameDelay = animData[2];
      evt->state3++;
      break;
   case 2:
      if (--evt->d.sprite.currentFrameDelay == 0) {
         evt->d.sprite.animDataIdx += 2;
         if (animData[evt->d.sprite.animDataIdx] == 0) {
            if (animData[evt->d.sprite.animDataIdx + 1] != 0) {
               // gfxIdx is zero, but frameDelay is non-zero; reset loop
               evt->d.sprite.animDataIdx = 1;
               evt->d.sprite.currentFrameDelay = animData[2];
            } else {
               // gfxIdx and frameDelay are both zero; rewind to final frame
               evt->d.sprite.animFinished++;
               evt->d.sprite.animDataIdx -= 2;
               evt->state3++;
            }
         } else {
            // gfxIdx is non-zero; assign associated delay, then break to set frame
            evt->d.sprite.currentFrameDelay = animData[evt->d.sprite.animDataIdx + 1];
         }
      }
      break;
   }

   evt->d.sprite.gfxIdx = animData[evt->d.sprite.animDataIdx];
}

void UpdateMultisizeEvtAnimation(EvtData *evt) {
   s16 *animData;

   /* Multisize AnimData shorts:
    (gfxIdx, delay, boxIdx), ... */

   animData = (s16 *)evt->d.sprite.animData;

   if (!evt->d.sprite.animInitialized) {
      evt->state3 = 1;
   }

   switch (evt->state3) {
   case 0:
      break;
   case 1:
      evt->d.sprite.animDataIdx = 0;
      evt->d.sprite.animFinished = 0;
      evt->d.sprite.animInitialized = 1;
      evt->d.sprite.currentFrameDelay = animData[1];
      evt->d.sprite.boxIdx = animData[2];
      evt->state3++;
      break;
   case 2:
      if (--evt->d.sprite.currentFrameDelay == 0) {
         evt->d.sprite.animDataIdx += 3;
         if (animData[evt->d.sprite.animDataIdx] == 0) {
            if (animData[evt->d.sprite.animDataIdx + 1] != 0) {
               // gfxIdx is zero, but frameDelay is non-zero; reset loop
               evt->d.sprite.animDataIdx = 0;
               evt->d.sprite.currentFrameDelay = animData[1];
               evt->d.sprite.boxIdx = animData[2];
            } else {
               // gfxIdx and frameDelay are both zero; rewind to final frame
               evt->d.sprite.animFinished++;
               evt->d.sprite.animDataIdx -= 3;
               evt->state3++;
            }
         } else {
            // gfxIdx is non-zero; assign associated delay/box, then break to set frame
            evt->d.sprite.currentFrameDelay = animData[evt->d.sprite.animDataIdx + 1];
            evt->d.sprite.boxIdx = animData[evt->d.sprite.animDataIdx + 2];
         }
      }
      break;
   }

   evt->d.sprite.gfxIdx = animData[evt->d.sprite.animDataIdx];
}

void UpdateUnitSpriteAnimation(EvtData *evt) {
   // Unit anim data consists of bytes instead of shorts
   s8 *animData = (s8 *)evt->d.sprite.animData;

   if (!evt->d.sprite.animInitialized) {
      evt->state3 = 1;
   }

   switch (evt->state3) {
   case 0:
      break;

   case 1:
      evt->d.sprite.animFinished = 0;
      evt->d.sprite.animInitialized = 1;

      if (animData[0] >= 0x3b) {
         evt->d.sprite.animRelativeOfs = animData[1];
         evt->d.sprite.animYOfs = animData[2];
         evt->d.sprite.animDataIdx = 3;
         evt->d.sprite.currentFrameDelay = animData[4];
         if (animData[0] == 0x3c) {
            evt->d.sprite.animSingleAxis = 1;
         } else {
            evt->d.sprite.animSingleAxis = 0;
         }
      } else {
         evt->d.sprite.animDataIdx = 0;
         evt->d.sprite.currentFrameDelay = animData[1];
         //? can sep? union?
         *(u32 *)&evt->d.sprite.animRelativeOfs = 0;
         // evt->d.sprite.animYOfs = 0;
         evt->d.sprite.animSingleAxis = 0;
      }

      evt->state3++;

      if (evt->d.sprite.currentFrameDelay == 1 && animData[evt->d.sprite.animDataIdx + 2] == 0 &&
          animData[evt->d.sprite.animDataIdx + 3] == 0) {
         evt->d.sprite.animFinished++;
         evt->state3 = 3;
      }

      break;

   case 2:
      if (--evt->d.sprite.currentFrameDelay == 0) {
         evt->d.sprite.animDataIdx += 2;

         if (animData[evt->d.sprite.animDataIdx] >= 0x3b) {
            evt->d.sprite.animRelativeOfs = animData[evt->d.sprite.animDataIdx + 1];
            evt->d.sprite.animYOfs = animData[evt->d.sprite.animDataIdx + 2];
            if (animData[evt->d.sprite.animDataIdx] == 0x3c) {
               evt->d.sprite.animSingleAxis = 1;
            } else {
               evt->d.sprite.animSingleAxis = 0;
            }
            evt->d.sprite.animDataIdx += 3;
         }

         if (animData[evt->d.sprite.animDataIdx] == 0) {
            if (animData[evt->d.sprite.animDataIdx + 1] != 0) {
               if (animData[0] >= 0x3b) {
                  evt->d.sprite.animRelativeOfs = animData[1];
                  evt->d.sprite.animYOfs = animData[2];
                  if (animData[0] == 0x3c) {
                     evt->d.sprite.animSingleAxis = 1;
                  } else {
                     evt->d.sprite.animSingleAxis = 0;
                  }
                  evt->d.sprite.animDataIdx = 3;
                  evt->d.sprite.currentFrameDelay = animData[4];
               } else {
                  evt->d.sprite.animDataIdx = 0;
                  evt->d.sprite.currentFrameDelay = animData[1];
                  //? can sep?
                  *(u32 *)&evt->d.sprite.animRelativeOfs = 0;
                  // evt->d.sprite.animYOfs = 0;
                  evt->d.sprite.animSingleAxis = 0;
               }
            }
         } else {
            evt->d.sprite.currentFrameDelay = animData[evt->d.sprite.animDataIdx + 1];
         }
      }

      if (evt->d.sprite.currentFrameDelay == 1 && animData[evt->d.sprite.animDataIdx + 2] == 0 &&
          animData[evt->d.sprite.animDataIdx + 3] == 0) {
         evt->d.sprite.animFinished++;
         evt->state3++;
      }

      break;
   }

   evt->d.sprite.gfxIdx = animData[evt->d.sprite.animDataIdx];
}
