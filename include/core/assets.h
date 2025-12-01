#pragma once
#include "extern/raylib.h"
#include <stdbool.h>

typedef enum {
    TEX_BG_STARS_CLOSE = 0,
    TEX_BG_STARS_FAR,
    TEX_PLAYER,
    TEX_ENEMY,
    TEX_LOGO,
    TEX_HUD_FULLHEART,
    TEX_HUD_HALFHEART,
    TEX_HUD_EMPTYHEART,
    TEX_LOGO_GACHAPOW,
    TEX_MENU_BACKGROUND,
    TEX_COUNT,
} TextureID;

void Assets_Init(void);
Texture2D GetTextureAsset(TextureID id);
bool Assets_IsLoaded(TextureID id);
bool Assets_LoadByPath(TextureID id, const char *path);
void Assets_Unload(void);
