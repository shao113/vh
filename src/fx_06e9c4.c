#include "common.h"
#include "evt.h"
#include "graphics.h"
#include "battle.h"
#include "field.h"

extern void ApplyMaskEffect(s16, s16, s16, s16, s16, s16, s16, s16, s16, s16);

static s16 sImpactAnimData_800ff18c[18] = {5, GFX_IMPACT_1, 2, GFX_IMPACT_2, 2, GFX_IMPACT_3,
                                           2, GFX_IMPACT_4, 2, GFX_IMPACT_5, 2, GFX_IMPACT_6,
                                           2, GFX_IMPACT_7, 2, GFX_NULL,     0, GFX_NULL};

// Making global until I resolve the premature split at Evtf211:
s16 gSmokeAnimData_800ff1b0[24] = {7, GFX_PUFF_1, 1, GFX_PUFF_2,  1, GFX_PUFF_3, 1, GFX_PUFF_4,
                                   1, GFX_PUFF_5, 1, GFX_PUFF_6,  1, GFX_PUFF_7, 1, GFX_PUFF_8,
                                   1, GFX_PUFF_9, 1, GFX_PUFF_10, 1, GFX_NULL,   0, GFX_NULL};

static s16 sSalamanderAnimData_800ff1e0[6] = {0, GFX_SALAMANDER_S, 1, GFX_NULL, 1, GFX_NULL};

static s16 sFlameAnimData_800ff1ec[20] = {
    7, GFX_FLAME_1, 1, GFX_FLAME_2, 1, GFX_FLAME_3, 1, GFX_FLAME_4, 1, GFX_FLAME_5,
    1, GFX_FLAME_6, 1, GFX_FLAME_7, 1, GFX_FLAME_8, 1, GFX_NULL,    1, GFX_NULL};

static s16 sExplosionAnimData_800ff214[26] = {
    7, GFX_EXPLOSION_1, 1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,  1, GFX_EXPLOSION_4,
    1, GFX_EXPLOSION_5, 1, GFX_EXPLOSION_6,  1, GFX_EXPLOSION_7,  2, GFX_EXPLOSION_8,
    2, GFX_EXPLOSION_9, 2, GFX_EXPLOSION_10, 2, GFX_EXPLOSION_11, 2, GFX_NULL,
    0, GFX_NULL};

static s16 sExplosionAnimData_800ff248[26] = {
    7, GFX_EXPLOSION_1, 1, GFX_EXPLOSION_2,  1, GFX_EXPLOSION_3,  1, GFX_EXPLOSION_4,
    1, GFX_EXPLOSION_5, 1, GFX_EXPLOSION_6,  1, GFX_EXPLOSION_7,  1, GFX_EXPLOSION_8,
    1, GFX_EXPLOSION_9, 1, GFX_EXPLOSION_10, 1, GFX_EXPLOSION_11, 1, GFX_NULL,
    0, GFX_NULL};

static s16 sFaerieAnimData_800ff27c[8] = {4, GFX_FAERIE_1, 2, GFX_FAERIE_2,
                                          2, GFX_NULL,     1, GFX_NULL};

static s16 sSparkleAnimData_800ff28c[22] = {7, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3,
                                            1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5, 1, GFX_SPARKLE_4,
                                            1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_1,
                                            1, GFX_NULL,      0, GFX_NULL};

static s16 sSparkleAnimData_800ff2b8[70] = {
    7, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5,
    1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_1, 1, GFX_SPARKLE_2,
    1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3,
    1, GFX_SPARKLE_2, 1, GFX_SPARKLE_1, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4,
    1, GFX_SPARKLE_5, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_1,
    1, GFX_SPARKLE_2, 1, GFX_SPARKLE_3, 1, GFX_SPARKLE_4, 1, GFX_SPARKLE_5, 1, GFX_SPARKLE_4,
    1, GFX_SPARKLE_3, 1, GFX_SPARKLE_2, 1, GFX_SPARKLE_1, 1, GFX_NULL,      0, GFX_NULL};

//...

#undef EVTF
#define EVTF 118
void Evtf118_Faerie_Target(EvtData *evt) {
   EvtData *targetSprite;
   EvtData *evt_s2;
   EvtData *evt_s3;
   s32 spriteX, spriteY, spriteW, spriteH;
   POLY_FT4 *poly;

   FntPrint("w1 :%d\n", EVT.timer);
   FntPrint("w10:%d\n", EVT.fade);

   switch (evt->state) {
   case 0:
      targetSprite = EVT.targetSprite;
      targetSprite->d.sprite.hidden = 1;
      evt->state++;

   // fallthrough
   case 1:
      targetSprite = EVT.targetSprite;
      evt->x1.n = targetSprite->x1.n;
      evt->z1.n = targetSprite->z1.n;
      evt->y1.n = targetSprite->y1.n;

      evt_s2 = Evt_GetUnused();
      CopyEvtData(targetSprite, evt_s2);
      evt_s3 = Evt_GetUnused();
      CopyEvtData(targetSprite, evt_s3);
      evt_s2->d.sprite.hidden = 0;
      evt_s3->d.sprite.hidden = 0;
      GetUnitSpriteVramRect(targetSprite, &spriteX, &spriteY, &spriteW, &spriteH);
      ApplyMaskEffect(spriteX, spriteY, spriteW + 1, spriteH + 1, 400 << 2, 320, 0,
                      -(evt->state3++ % 0x40), GFX_TBD_25, 0);
      evt_s2->d.sprite.gfxIdx = GFX_TBD_25;
      evt_s2->d.sprite.clut = EVT.clut;
      evt_s2->d.sprite.semiTrans = 2;

      if (targetSprite->d.sprite.gfxIdx >= 21 && targetSprite->d.sprite.gfxIdx <= 30) {
         evt_s2->d.sprite.boxIdx = 1;
      } else {
         evt_s2->d.sprite.boxIdx = 0;
      }

      AddEvtPrim6(gGraphicsPtr->ot, evt_s2, 0);
      poly = &gGraphicsPtr->quads[gQuadIndex - 1];
      setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);

      RenderUnitSprite(gGraphicsPtr->ot, evt_s3, 0);
      evt_s2->functionIndex = EVTF_NULL;
      evt_s3->functionIndex = EVTF_NULL;

      switch (evt->state2) {
      case 0:
         EVT.fade += 4;
         if (EVT.fade > 0x40) {
            evt->state2++;
         }
         break;
      case 1:
         if (++EVT.timer == 88) {
            evt->state2++;
            EVT.unused_0x2C = 0x40;
         }
         break;
      case 2:
         EVT.fade -= 4;
         if (EVT.fade < 0) {
            targetSprite = EVT.targetSprite;
            targetSprite->d.sprite.hidden = 0;
            evt->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

#undef EVTF
#define EVTF 117
void Evtf117_Faerie_SparkleTrail(EvtData *evt) {
   EvtData *sprite;
   s32 rnd;
   s16 halfSize;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sSparkleAnimData_800ff2b8;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;
      EVT.fade = 0x180;
      evt->y2.n = evt->y1.n;
      evt->y3.n = -((rand() >> 2) % 15) - 2;
      evt->state2 = (rand() >> 2) % 3 + 1;
      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);

      if (sprite->d.sprite.animFinished) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      } else {
         sprite->x1.n = evt->x1.n;
         sprite->z1.n = evt->z1.n;
         sprite->y1.n = evt->y2.n;
         evt->y2.n += evt->y3.n;

         halfSize = evt->state2;
         gQuad_800fe63c[0].vx = -halfSize;
         gQuad_800fe63c[0].vy = -halfSize;
         gQuad_800fe63c[1].vx = halfSize;
         gQuad_800fe63c[1].vy = -halfSize;
         gQuad_800fe63c[2].vx = -halfSize;
         gQuad_800fe63c[2].vy = halfSize;
         gQuad_800fe63c[3].vx = halfSize;
         gQuad_800fe63c[3].vy = halfSize;

         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade / 2, EVT.fade / 2, EVT.fade / 2);

         EVT.fade += evt->y3.n;
         if (EVT.fade <= 0) {
            EVT.fade = 0;
         }
      }

      break;
   }
}

#undef EVTF
#define EVTF 116
void Evtf116_Faerie_Sparkle(EvtData *evt) {
   EvtData *sprite;
   POLY_FT4 *poly;

   switch (evt->state) {
   case 0:
      sprite = Evt_GetUnused();
      sprite->functionIndex = EVTF_NOOP;
      sprite->d.sprite.animData = sSparkleAnimData_800ff28c;
      sprite->d.sprite.semiTrans = 2;
      EVT.sprite = sprite;

      EVT.xOfs = 0x40 - rand() % 0x81;
      EVT.zOfs = 0x40 - rand() % 0x81;
      EVT.yOfs = 0x40 - rand() % 0x81;
      EVT.fade = 0x80;

      evt->state++;

   // fallthrough
   case 1:
      sprite = EVT.sprite;
      UpdateEvtAnimation(sprite);
      if (sprite->d.sprite.animFinished) {
         sprite->functionIndex = EVTF_NULL;
         evt->functionIndex = EVTF_NULL;
      } else {
         sprite->x1.n = evt->x1.n + EVT.xOfs;
         sprite->z1.n = evt->z1.n + EVT.zOfs;
         sprite->y1.n = evt->y1.n + EVT.yOfs;

         gQuad_800fe63c[0].vx = -4;
         gQuad_800fe63c[0].vy = -4;
         gQuad_800fe63c[1].vx = 4;
         gQuad_800fe63c[1].vy = -4;
         gQuad_800fe63c[2].vx = -4;
         gQuad_800fe63c[2].vy = 4;
         gQuad_800fe63c[3].vx = 4;
         gQuad_800fe63c[3].vy = 4;

         AddEvtPrim6(gGraphicsPtr->ot, sprite, 0);
         poly = &gGraphicsPtr->quads[gQuadIndex - 1];
         setRGB0(poly, EVT.fade, EVT.fade, EVT.fade);
         EVT.fade -= 14;
      }

      break;
   }
}

s32 s_unused_80123340;

#undef EVTF
#define EVTF 115
void Evtf115_Faerie_FX2(EvtData *evt) {
   EvtData *evt_s1;

   switch (evt->state) {
   case 0:
      s_unused_80123340 = 0;
      evt_s1 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      EVT.targetSprite = evt_s1;
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi);
      evt->x1.n = evt_s1->x1.n;
      evt->z1.n = evt_s1->z1.n;

      evt_s1 = Evt_GetUnused();
      evt_s1->functionIndex = EVTF_NOOP;
      evt_s1->d.sprite.animData = sFaerieAnimData_800ff27c;
      EVT.faerieSprite = evt_s1;

      EVT.todo_x26 = 0x200;
      evt->mem = 3;
      evt->state++;

   // fallthrough
   case 1:
      evt_s1 = EVT.faerieSprite;
      evt_s1->x1.n = evt->x1.n + (EVT.todo_x26 + 0xa0) * rcos(EVT.theta) / ONE;
      evt_s1->z1.n = evt->z1.n + (EVT.todo_x26 + 0xa0) * rsin(EVT.theta) / ONE;
      evt_s1->y1.n = evt->y1.n + 0x180 + EVT.todo_x26;
      UpdateEvtAnimation(evt_s1);
      AddEvtPrim6(gGraphicsPtr->ot, evt_s1, 0);

      CreatePositionedEvt(evt_s1, EVTF_FAERIE_SPARKLE);

      if (--evt->mem <= 0) {
         CreatePositionedEvt(evt_s1, EVTF_FAERIE_SPARKLE_TRAIL);
         evt->mem = (rand() >> 2) % 2 + 1;
      }

      EVT.theta += 0x60;

      switch (evt->state2) {
      case 0:
         EVT.todo_x26 -= 10;
         if (EVT.todo_x26 < 0) {
            EVT.todo_x26 = 0;
            evt->state2++;
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_FAERIE_TARGET;
            evt_s1->x1.n = evt->x1.n;
            evt_s1->z1.n = evt->z1.n;
            evt_s1->y1.n = evt->y1.n;
            evt_s1->d.evtf118.targetSprite = EVT.targetSprite;
            evt_s1->d.evtf118.clut = 4;
         }
         break;

      case 1:
         if (++EVT.timer == 51) {
            evt->state2++;
         }
         break;

      case 2:
         EVT.todo_x26 += 10;
         if (EVT.todo_x26 == 220) {
            evt_s1 = Evt_GetUnused();
            evt_s1->functionIndex = EVTF_DISPLAY_DAMAGE_2;
            evt_s1->x1.s.hi = evt->x1.s.hi;
            evt_s1->z1.s.hi = evt->z1.s.hi;
         }
         if (EVT.todo_x26 > 0x200) {
            evt->functionIndex = EVTF_NULL;
            gSignal3 = 1;
            evt_s1->functionIndex = EVTF_NULL;
         }
         break;
      }

      break;
   }
}

/*
#undef EVTF
#define EVTF 211
void Evtf211_Avalanche_Boulder(EvtData *evt) {

   // WIP: Butchered this pretty badly while trying (unsuccessfully) to eke out a fake match.

   extern SVECTOR s_vertices_80123b0c[11][9];

   EvtData *evt_s0;
   EvtData *evt_s2;
   POLY_FT4 *poly;
   s32 i, j;
   s32 rnd1;
   s32 rnd2;
   s16 iVar5s;
   s16 iVar8s;
   s16 iVar10s;
   s32 shade;
   s16 a;
   s16 b;
   s16 c;
   s32 temp_s8;
   s16 local_38;
   s32 local_40;
   s32 flag;
   s32 onstack;
   MATRIX matrix;
   VECTOR rotated[11][9];
   SVECTOR rotation;

   s8 unused[16];

   switch (evt->state) {
   case 0:
      evt_s0 = GetUnitSpriteAtPosition(evt->z1.s.hi, evt->x1.s.hi);
      evt->y1.n = GetTerrainElevation(evt->z1.s.hi, evt->x1.s.hi) + 0x200;
      evt->x1.n = evt_s0->x1.n;
      evt->z1.n = evt_s0->z1.n;

      temp_s8 = 0x300; //?

      // Intentionally wrong, for easier comparison:
      i = 0;
      *&onstack = 0x100;

      for (; i < 11; i++) {
         //? Getting this part right is probably essential for a match; another temp? s16?
         b = ((j = onstack) * rsin(i * 0xcd)) / ONE;
         a = (u16)(temp_s8 >> 1) * rcos(i * 0xcd + 0x800) / ONE;
         for (j = 0; j < 9; j++) {
            s_vertices_80123b0c[i][j].vx = b * rcos(j * 0x200) / ONE;
            s_vertices_80123b0c[i][j].vy = b * rsin(j * 0x200) / ONE;
            s_vertices_80123b0c[i][j].vz = a;
         }
      }

      for (local_40 = 0; local_40 < 0x100; local_40++) {
         rnd1 = rand() % 11;
         j = rand() % 8;
         a = 0x140 * rsin(rnd1 * 0xcd) / ONE;

         if (rnd1 == 0) {
            iVar5s = 8 - rand() % 17;
            iVar8s = 8 - rand() % 17;
            iVar10s = 8 - rand() % 17;
            b = rand() % 8;

            for (j = 0; j < 8; j++) {
               s_vertices_80123b0c[rnd1][j].vx =
                   (s_vertices_80123b0c[rnd1][j].vx * 7 + s_vertices_80123b0c[rnd1 + 1][b].vx) / 8 +
                   iVar5s;
               s_vertices_80123b0c[rnd1][j].vz =
                   (s_vertices_80123b0c[rnd1][j].vz * 7 + s_vertices_80123b0c[rnd1 + 1][b].vz) / 8 +
                   iVar8s;
               s_vertices_80123b0c[rnd1][j].vy =
                   (s_vertices_80123b0c[rnd1][j].vy * 7 + s_vertices_80123b0c[rnd1 + 1][b].vy) / 8 +
                   iVar10s;
            }
         } else if (rnd1 == 10) {
            iVar5s = 8 - rand() % 17;
            iVar8s = 8 - rand() % 17;
            iVar10s = 8 - rand() % 17;
            b = rand() % 8;

            for (j = 0; j < 8; j++) {
               s_vertices_80123b0c[rnd1][j].vx =
                   (s_vertices_80123b0c[rnd1][j].vx * 7 + s_vertices_80123b0c[rnd1 - 1][b].vx) / 8 +
                   iVar5s;
               s_vertices_80123b0c[rnd1][j].vz =
                   (s_vertices_80123b0c[rnd1][j].vz * 7 + s_vertices_80123b0c[rnd1 - 1][b].vz) / 8 +
                   iVar8s;
               s_vertices_80123b0c[rnd1][j].vy =
                   (s_vertices_80123b0c[rnd1][j].vy * 7 + s_vertices_80123b0c[rnd1 - 1][b].vy) / 8 +
                   iVar10s;
            }
         } else {
            s_vertices_80123b0c[rnd1][j].vx =
                (s_vertices_80123b0c[rnd1][j].vx * 4 + s_vertices_80123b0c[rnd1 + 1][j].vx +
                 s_vertices_80123b0c[rnd1 - 1][j].vx + s_vertices_80123b0c[rnd1][j + 1].vx +
                 s_vertices_80123b0c[rnd1][j - 1].vx) /
                    8 +
                (a * (0x30 - rand() % 0x61) / 0x100);

            s_vertices_80123b0c[rnd1][j].vy =
                (s_vertices_80123b0c[rnd1][j].vy * 4 + s_vertices_80123b0c[rnd1 + 1][j].vy +
                 s_vertices_80123b0c[rnd1 - 1][j].vy + s_vertices_80123b0c[rnd1][j + 1].vy +
                 s_vertices_80123b0c[rnd1][j - 1].vy) /
                    8 +
                (a * (0x30 - rand() % 0x61) / 0x100);

            s_vertices_80123b0c[rnd1][j].vz =
                (s_vertices_80123b0c[rnd1][j].vz * 4 + s_vertices_80123b0c[rnd1 + 1][j].vz +
                 s_vertices_80123b0c[rnd1 - 1][j].vz + s_vertices_80123b0c[rnd1][j + 1].vz +
                 s_vertices_80123b0c[rnd1][j - 1].vz) /
                    8 +
                (a * (0x30 - rand() % 0x61) / 0x100);
         }
      }

      for (i = 0; i < 11; i++) {
         s_vertices_80123b0c[i][8].vx = s_vertices_80123b0c[i][0].vx;
         s_vertices_80123b0c[i][8].vy = s_vertices_80123b0c[i][0].vy;
         s_vertices_80123b0c[i][8].vz = s_vertices_80123b0c[i][0].vz;
      }

      evt->state++;

   // fallthrough
   case 1:
      evt_s2 = Evt_GetUnused();
      evt_s2->functionIndex = EVTF_NOOP;
      evt_s2->d.sprite.gfxIdx = GFX_TILED_STONE;
      evt_s2->d.sprite.clut = 5;

      iVar8s = EVT.todo_x4c;
      PushMatrix();
      rotation.vx = EVT.rotX;
      rotation.vz = EVT.rotZ;
      rotation.vy = EVT.rotY;
      RotMatrix(&rotation, &matrix);
      matrix.t[0] = 0;
      matrix.t[1] = 0;
      matrix.t[2] = 0;
      SetRotMatrix(&matrix);
      SetTransMatrix(&matrix);
      do {
         for (i = 0; i < 11; i++) {
            for (j = 0; j < 9; j++) {
               RotTrans(&s_vertices_80123b0c[i][j], &rotated[i][j], &flag);
            }
         }
      } while (0);
      PopMatrix();

      for (rnd2 = 0; rnd2 < 10; rnd2++) {
         for (j = 0; j < 8; j++) {
            evt_s2->d.sprite.coords[0].x = evt->x1.n + iVar8s * rotated[rnd2 + 1][j].vx / 0x80;
            evt_s2->d.sprite.coords[0].y = evt->y1.n + iVar8s * rotated[rnd2 + 1][j].vy / 0x80;
            evt_s2->d.sprite.coords[0].z = evt->z1.n + iVar8s * rotated[rnd2 + 1][j].vz / 0x80;
            evt_s2->d.sprite.coords[1].x = evt->x1.n + iVar8s * rotated[rnd2 + 1][j + 1].vx / 0x80;
            evt_s2->d.sprite.coords[1].y = evt->y1.n + iVar8s * rotated[rnd2 + 1][j + 1].vy / 0x80;
            evt_s2->d.sprite.coords[1].z = evt->z1.n + iVar8s * rotated[rnd2 + 1][j + 1].vz / 0x80;
            evt_s2->d.sprite.coords[2].x = evt->x1.n + iVar8s * rotated[rnd2][j].vx / 0x80;
            evt_s2->d.sprite.coords[2].y = evt->y1.n + iVar8s * rotated[rnd2][j].vy / 0x80;
            evt_s2->d.sprite.coords[2].z = evt->z1.n + iVar8s * rotated[rnd2][j].vz / 0x80;
            evt_s2->d.sprite.coords[3].x = evt->x1.n + iVar8s * rotated[rnd2][j + 1].vx / 0x80;
            evt_s2->d.sprite.coords[3].y = evt->y1.n + iVar8s * rotated[rnd2][j + 1].vy / 0x80;
            evt_s2->d.sprite.coords[3].z = evt->z1.n + iVar8s * rotated[rnd2][j + 1].vz / 0x80;

            a = 0x80;
            if (evt_s2->d.sprite.coords[0].x - evt_s2->d.sprite.coords[1].x < 0) {
               a -= 0x10;
            }
            if (evt_s2->d.sprite.coords[2].x - evt_s2->d.sprite.coords[3].x < 0) {
               a -= 0x10;
            }
            if (evt_s2->d.sprite.coords[0].z - evt_s2->d.sprite.coords[1].z < 0) {
               a -= 0x10;
            }
            if (evt_s2->d.sprite.coords[2].z - evt_s2->d.sprite.coords[3].z < 0) {
               a -= 0x10;
            }
            if (evt_s2->d.sprite.coords[0].y - evt_s2->d.sprite.coords[2].y < 0) {
               a -= 0x10;
            }
            if (evt_s2->d.sprite.coords[1].y - evt_s2->d.sprite.coords[3].y < 0) {
               a -= 0x10;
            }

            AddEvtPrim4(gGraphicsPtr->ot, evt_s2);
            poly = &gGraphicsPtr->quads[gQuadIndex - 1];
            setRGB0(poly, a, a, a);
         }
      }

      evt_s2->functionIndex = EVTF_NULL;
      break;
   }
}
*/
