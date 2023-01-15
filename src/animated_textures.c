#include "common.h"
#include "evt.h"
#include "graphics.h"

// Identifying these as MapObjects since they're in the MapObject arrays.
// (See: Predefined MapObject arrays @800f6378; Evtf043_SetupMapObjects @800515c8)

void Evtf564_565_566_MapObject_Water(EvtData *evt) {
   extern u32 s_distortedTex_80123490[32][4];
   extern u32 s_originalTex_80123690[32][4];
   RECT rect;
   s16 i;
   s16 shift;
   s16 phase;

   if (evt->functionIndex == EVTF_MAP_OBJECT_WATER_1) {
      rect.x = 328;
      rect.y = 48;
      rect.w = 8;
      rect.h = 32;

      switch (evt->state) {
      case 0:
         StoreImage(&rect, s_originalTex_80123690);
         DrawSync(0);
         evt->state++;
         break;
      case 1:
         for (i = 0; i < 128; i++) {
            ((u32 *)s_distortedTex_80123490)[i] = ((u32 *)s_originalTex_80123690)[i];
         }

         evt->d.evtf564.phase += 130;
         phase = evt->d.evtf564.phase;

         for (i = 0; i < 32; i++) {
            shift = ((rcos(phase & 0xfff) * 2 >> 12) + 4) * 4;

            s_distortedTex_80123490[i][0] = (s_distortedTex_80123490[i][0] >> shift) +
                                            (s_distortedTex_80123490[i][1] << (32 - shift));
            s_distortedTex_80123490[i][1] = (s_distortedTex_80123490[i][1] >> shift) +
                                            (s_distortedTex_80123490[i][2] << (32 - shift));
            s_distortedTex_80123490[i][2] = (s_distortedTex_80123490[i][2] >> shift) +
                                            (s_distortedTex_80123490[i][3] << (32 - shift));
            s_distortedTex_80123490[i][3] = (s_distortedTex_80123490[i][3] >> shift) +
                                            (s_distortedTex_80123490[i][0] << (32 - shift));
            phase += 0x100;
         }

         rect.x = 328;
         rect.y = 48;
         LoadImage(&rect, s_distortedTex_80123490);
         break;
      }

      gGfxSubTextures[GFX_MAP_TEXTURE_51].y--;
      if (gGfxSubTextures[GFX_MAP_TEXTURE_51].y <= 48) {
         gGfxSubTextures[GFX_MAP_TEXTURE_51].y = 64;
      }
      gGfxSubTextures[GFX_MAP_TEXTURE_52].y = gGfxSubTextures[GFX_MAP_TEXTURE_51].y;

   } else if (evt->functionIndex == EVTF_MAP_OBJECT_WATER_2) {
      rect.x = 332;
      rect.y = 0;
      rect.w = 8;
      rect.h = 32;

      switch (evt->state) {
      case 0:
         StoreImage(&rect, s_originalTex_80123690);
         DrawSync(0);
         evt->state++;
         break;
      case 1:
         for (i = 0; i < 128; i++) {
            ((u32 *)s_distortedTex_80123490)[i] = ((u32 *)s_originalTex_80123690)[i];
         }

         evt->d.evtf564.phase += 130;
         phase = evt->d.evtf564.phase;

         for (i = 0; i < 32; i++) {
            shift = ((rcos(phase & 0xfff) * 2 >> 12) + 4) * 4;

            s_distortedTex_80123490[i][0] = (s_distortedTex_80123490[i][0] >> shift) +
                                            (s_distortedTex_80123490[i][1] << (32 - shift));
            s_distortedTex_80123490[i][1] = (s_distortedTex_80123490[i][1] >> shift) +
                                            (s_distortedTex_80123490[i][2] << (32 - shift));
            s_distortedTex_80123490[i][2] = (s_distortedTex_80123490[i][2] >> shift) +
                                            (s_distortedTex_80123490[i][3] << (32 - shift));
            s_distortedTex_80123490[i][3] = (s_distortedTex_80123490[i][3] >> shift) +
                                            (s_distortedTex_80123490[i][0] << (32 - shift));
            phase += 0x100;
         }

         rect.x = 332;
         rect.y = 0;
         LoadImage(&rect, s_distortedTex_80123490);
         break;
      }
   } else {
      // EVTF_MAP_OBJECT_LAVA_1:
      rect.x = 324;
      rect.y = 0;
      rect.w = 8;
      rect.h = 32;

      switch (evt->state) {
      case 0:
         StoreImage(&rect, s_originalTex_80123690);
         DrawSync(0);
         evt->state++;
         break;
      case 1:
         for (i = 0; i < 128; i++) {
            ((u32 *)s_distortedTex_80123490)[i] = ((u32 *)s_originalTex_80123690)[i];
         }

         evt->d.evtf564.phase += 130;
         phase = evt->d.evtf564.phase;

         for (i = 0; i < 32; i++) {
            shift = ((rcos(phase & 0xfff) * 2 >> 12) + 4) * 4;

            s_distortedTex_80123490[i][0] = (s_distortedTex_80123490[i][0] >> shift) +
                                            (s_distortedTex_80123490[i][1] << (32 - shift));
            s_distortedTex_80123490[i][1] = (s_distortedTex_80123490[i][1] >> shift) +
                                            (s_distortedTex_80123490[i][2] << (32 - shift));
            s_distortedTex_80123490[i][2] = (s_distortedTex_80123490[i][2] >> shift) +
                                            (s_distortedTex_80123490[i][3] << (32 - shift));
            s_distortedTex_80123490[i][3] = (s_distortedTex_80123490[i][3] >> shift) +
                                            (s_distortedTex_80123490[i][0] << (32 - shift));
            phase += 0x100;
         }

         rect.x = 324;
         rect.y = 0;
         LoadImage(&rect, s_distortedTex_80123490);
         break;
      }
   }
}

void Evtf449_MapObject_FlowingWater(EvtData *evt) {
   gGfxSubTextures[GFX_MAP_TEXTURE_5].y++;
   if (gGfxSubTextures[GFX_MAP_TEXTURE_5].y >= 16) {
      gGfxSubTextures[GFX_MAP_TEXTURE_5].y = 0;
   }
}

void Evtf568_MapObject_Rail(EvtData *evt) {
   // Simulated train movement
   s32 i;

   gLightRotation.vy = GetLightRotY() + 0xc0;

   gGfxSubTextures[GFX_MAP_TEXTURE_7].y += 8;
   if (gGfxSubTextures[GFX_MAP_TEXTURE_7].y >= 32) {
      gGfxSubTextures[GFX_MAP_TEXTURE_7].y = 0;
   }

   gGfxSubTextures[GFX_MAP_TEXTURE_23].y += 8;
   if (gGfxSubTextures[GFX_MAP_TEXTURE_23].y >= 48) {
      gGfxSubTextures[GFX_MAP_TEXTURE_23].y = 16;
   }

   for (i = 8; i < 19; i++) {
      gGfxSubTextures[GFX_MAP_TEXTURES_OFS + i].y = gGfxSubTextures[GFX_MAP_TEXTURE_7].y;
   }
   for (i = 24; i < 35; i++) {
      gGfxSubTextures[GFX_MAP_TEXTURES_OFS + i].y = gGfxSubTextures[GFX_MAP_TEXTURE_23].y;
   }

   switch (evt->mem) {
   case 0:
      gGfxSubTextures[GFX_MAP_TEXTURE_90].x = 224;
      evt->mem++;
      break;
   case 1:
      gGfxSubTextures[GFX_MAP_TEXTURE_90].x = 240;
      evt->mem = 0;
      break;
   }
}

void Evtf411_MapObject_VileBog(EvtData *evt) {
   switch (evt->state) {
   case 0:
   case 4:
   case 8:
   case 12:
      gGfxSubTextures[GFX_MAP_TEXTURE_21].x = evt->state2 * 16 + 80;
      evt->state2++;
      break;
   case 15:
      evt->state = 0;
      evt->state2 = 0;
      return;
   }
   evt->state++;
}

void Evtf569_572_MapObject_Lava(EvtData *evt) {
   switch (evt->state) {
   case 0:
   case 3:
   case 6:
   case 9:
   case 12:
   case 15:
   case 18:
   case 21:
      if (evt->functionIndex == EVTF_MAP_OBJECT_LAVA_2) {
         gGfxSubTextures[GFX_MAP_TEXTURE_198].x = evt->state2 * 16 + 48;
      } else {
         gGfxSubTextures[GFX_MAP_TEXTURE_4].x = evt->state2 * 16 + 16;
         // shadowed area
         gGfxSubTextures[GFX_MAP_TEXTURE_128].x = evt->state2 * 16 + 112;
      }
      evt->state2++;
      break;
   case 23:
      evt->state = 0;
      evt->state2 = 0;
      return;
   }
   evt->state++;
}

void Evtf042_MapObject_FlowingSand(EvtData *evt) {
   gGfxSubTextures[GFX_MAP_TEXTURE_1].y--;
   if (gGfxSubTextures[GFX_MAP_TEXTURE_1].y == 0) {
      gGfxSubTextures[GFX_MAP_TEXTURE_1].y = 16;
   }
}
