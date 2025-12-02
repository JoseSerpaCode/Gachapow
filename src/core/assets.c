// ============================================================================
// assets.c
// Sistema centralizado de carga, acceso y descarga de texturas + animaciones.
// ============================================================================

#include "assets.h"
#include "extern/raylib.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// VARIABLES DEL BANCO DE TEXTURAS
// ============================================================================

static Texture2D textures[TEX_COUNT];

static const char *asset_paths[TEX_COUNT] = {
    [TEX_BG_STARS_CLOSE] = "assets/images/game/background/stars_close.png",
    [TEX_BG_STARS_FAR]   = "assets/images/game/background/stars_far.png",
    [TEX_PLAYER]         = "assets/images/game/player.png",
    [TEX_ENEMY]          = "assets/images/game/enemies/enemy.png",
    [TEX_LOGO]           = "assets/images/logo.png",

    [TEX_HUD_FULLHEART]  = "assets/images/game/hud/hud.png",
    [TEX_HUD_HALFHEART]  = "assets/images/game/hud/hud.png",
    [TEX_HUD_EMPTYHEART] = "assets/images/game/hud/hud.png",

    [TEX_LOGO_GACHAPOW]  = "assets/images/logo.png",
    [TEX_MENU_BACKGROUND]= "assets/images/menu_background.png",
};

// ============================================================================
// VARIABLES PARA ANIMACIONES
// ============================================================================

Texture2D intro1_frames[MAX_ANIM_FRAMES];
int intro1_frames_count = 0;

Texture2D intro2_frames[MAX_ANIM_FRAMES];
int intro2_frames_count = 0;

Texture2D intro3_frames[MAX_ANIM_FRAMES];
int intro3_frames_count = 0;

// ============================================================================
// CARGA INTERNA DE TEXTURA
// ============================================================================

static bool Assets_LoadTextureInternal(TextureID id, const char *path)
{
    if (!path || id < 0 || id >= TEX_COUNT) return false;

    if (textures[id].id != 0)
        return true;

    Texture2D t = LoadTexture(path);

    if (t.id == 0)
    {
        fprintf(stderr, "ERROR: No se pudo cargar textura '%s'\n", path);
        return false;
    }

    textures[id] = t;
    return true;
}

// ============================================================================
// CARGA DE ANIMACIONES AUTOMÁTICAS
// ============================================================================
// Busca archivos numerados: 000.png, 001.png, 002.png, ...
// ============================================================================

bool Assets_LoadAnimation(const char *folder, Texture2D outFrames[], int *outCount)
{
    if (!folder || !outFrames || !outCount) return false;

    char path[512];
    *outCount = 0;

    for (int i = 0; i < MAX_ANIM_FRAMES; i++)
    {
        snprintf(path, sizeof(path), "%s/pixil-frame-%d.png", folder, i);
        if (!FileExists(path))
            break;

        Texture2D t = LoadTexture(path);

        if (t.id == 0)
        {
            fprintf(stderr, "ERROR: falló frame %s\n", path);
            return false;
        }

        outFrames[*outCount] = t;
        (*outCount)++;
    }

    printf("Cargados %d frames desde %s\n", *outCount, folder);
    return true;
}

// ============================================================================
// UNLOAD ANIMATION
// ============================================================================

void Assets_UnloadAnimation(Texture2D frames[], int count)
{
    for (int i = 0; i < count; i++)
    {
        if (frames[i].id != 0)
        {
            UnloadTexture(frames[i]);
            frames[i] = (Texture2D){0};
        }
    }
}

// ============================================================================
// INIT - Inicializa todo el sistema de assets
// ============================================================================

void Assets_Init(void)
{
    memset(textures, 0, sizeof(textures));

    for (int i = 0; i < TEX_COUNT; i++)
        if (asset_paths[i])
            Assets_LoadTextureInternal(i, asset_paths[i]);

    // Cargar animaciones
    Assets_LoadAnimation("assets/animations/intro1", intro1_frames, &intro1_frames_count);
    Assets_LoadAnimation("assets/animations/intro2", intro2_frames, &intro2_frames_count);
    Assets_LoadAnimation("assets/animations/intro3", intro3_frames, &intro3_frames_count);
}

// ============================================================================
// GETTERS
// ============================================================================

Texture2D GetTextureAsset(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT) return (Texture2D){0};
    return textures[id];
}

bool Assets_IsLoaded(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT) return false;
    return textures[id].id != 0;
}

bool Assets_LoadByPath(TextureID id, const char *path)
{
    return Assets_LoadTextureInternal(id, path);
}

// ============================================================================
// UNLOAD TOTAL
// ============================================================================

void Assets_Unload(void)
{
    for (int i = 0; i < TEX_COUNT; i++)
    {
        if (textures[i].id != 0)
        {
            UnloadTexture(textures[i]);
            textures[i] = (Texture2D){0};
        }
    }

    Assets_UnloadAnimation(intro1_frames, intro1_frames_count);
    Assets_UnloadAnimation(intro2_frames, intro2_frames_count);
    Assets_UnloadAnimation(intro3_frames, intro3_frames_count);
}
