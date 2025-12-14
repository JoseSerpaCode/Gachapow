#pragma once
#include "extern/raylib.h"
#include <stdbool.h>

// ======================================================
// TEXTURAS NORMALES DEL PROYECTO
// ======================================================

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

// Inicialización y control
void Assets_Init(void);
Texture2D GetTextureAsset(TextureID id);
bool Assets_IsLoaded(TextureID id);
bool Assets_LoadByPath(TextureID id, const char *path);
void Assets_Unload(void);

// ======================================================
// SISTEMA DE ANIMACIONES
// ======================================================

// Limite máximo de frames por animación
#define MAX_ANIM_FRAMES 512

extern Texture2D intro1_frames[MAX_ANIM_FRAMES];
extern int intro1_frames_count;

extern Texture2D intro2_frames[MAX_ANIM_FRAMES];
extern int intro2_frames_count;

// Carga automática de frames numerados dentro de una carpeta
bool Assets_LoadAnimation(const char *folder, Texture2D outFrames[], int *outCount);

// Descarga un array de texturas
void Assets_UnloadAnimation(Texture2D frames[], int count);
