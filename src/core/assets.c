// ============================================================================
// assets.c
// Sistema centralizado de carga, acceso y descarga de texturas del juego.
// Usa un banco estático donde se almacenan todas las Texture2D cargadas.
// ============================================================================

#include "assets.h"
#include "extern/raylib.h"
#include <stdio.h>
#include <string.h>     // memset
#include <assert.h>

// ============================================================================
// VARIABLES ESTÁTICAS
// ============================================================================

// Banco de texturas (índice = TextureID del enum)
static Texture2D textures[TEX_COUNT];

// Tabla de rutas asociadas a cada TextureID.
// DEBEN mantener el mismo orden que el enum en assets.h
// NOTA: HUD apunta temporalmente al mismo archivo en 3 entradas.
static const char *asset_paths[TEX_COUNT] = {
    [TEX_BG_STARS_CLOSE] = "assets/images/game/background/stars_close.png",
    [TEX_BG_STARS_FAR]   = "assets/images/game/background/stars_far.png",
    [TEX_PLAYER]         = "assets/images/game/player.png",
    [TEX_ENEMY]          = "assets/images/game/enemies/enemy.png",
    [TEX_LOGO]           = "assets/images/logo.png",

    // TODO: HUD → actualmente los 3 apuntan al mismo archivo.
    // Más adelante cada uno debería tener su sprite real.
    [TEX_HUD_FULLHEART]  = "assets/images/game/hud/hud.png",
    [TEX_HUD_HALFHEART]  = "assets/images/game/hud/hud.png",
    [TEX_HUD_EMPTYHEART] = "assets/images/game/hud/hud.png",

    [TEX_LOGO_GACHAPOW]  = "assets/images/logo.png",
    [TEX_MENU_BACKGROUND]= "assets/images/menu_background.png",
};

// ============================================================================
// HELPER INTERNO
// Carga una textura individual si aún no se ha cargado.
// ============================================================================

static bool Assets_LoadTextureInternal(TextureID id, const char *path)
{
    if (id < 0 || id >= TEX_COUNT || path == NULL)
        return false;

    // Ya cargada → nada que hacer
    if (textures[id].id != 0)
        return true;

    // Intentar cargar
    Texture2D t = LoadTexture(path);

    if (t.id == 0)
    {
        // Error al cargar
        fprintf(stderr,
                "Assets: fallo cargando texture '%s' (id=%d)\n",
                path, id);
        return false;
    }

    textures[id] = t;
    return true;
}

// ============================================================================
// INIT
// Inicializa el sistema de assets cargando todo el banco inicial.
// ============================================================================

void Assets_Init(void)
{
    // Limpiar banco de texturas
    memset(textures, 0, sizeof(textures));

    // Cargar cada textura definida en asset_paths
    for (int i = 0; i < TEX_COUNT; ++i)
    {
        if (asset_paths[i])
            Assets_LoadTextureInternal((TextureID)i, asset_paths[i]);
    }
}

// ============================================================================
// GETTER
// Devuelve la textura pedida. Si no está cargada, intenta carga "lazy".
// ============================================================================

Texture2D GetTextureAsset(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT)
        return (Texture2D){ 0 };

    // Intento carga lazy solo si está vacía
    if (textures[id].id == 0)
    {
        const char *path = asset_paths[id];
        if (path)
        {
            bool ok = Assets_LoadTextureInternal(id, path);
            if (!ok)
                return (Texture2D){ 0 };
        }
        else
        {
            // No existe ruta asociada
            return (Texture2D){ 0 };
        }
    }

    return textures[id];
}

// ============================================================================
// CHECKER
// Indica si un asset está cargado en memoria.
// ============================================================================

bool Assets_IsLoaded(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT)
        return false;

    return textures[id].id != 0;
}

// ============================================================================
// UNLOAD
// Descarga TODAS las texturas en el banco.
// ============================================================================

void Assets_Unload(void)
{
    for (int i = 0; i < TEX_COUNT; ++i)
    {
        if (textures[i].id != 0)
        {
            UnloadTexture(textures[i]);
            textures[i] = (Texture2D){ 0 }; // evitar double free
        }
    }
}

// ============================================================================
// UTILIDAD OPCIONAL
// Cargar un asset manualmente desde una ruta específica.
// ============================================================================

bool Assets_LoadByPath(TextureID id, const char *path)
{
    if (id < 0 || id >= TEX_COUNT || path == NULL)
        return false;

    return Assets_LoadTextureInternal(id, path);
}
