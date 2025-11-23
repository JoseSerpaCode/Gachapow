// src/assets.c
#include "assets.h"
#include "extern/raylib.h"
#include <stdio.h>
#include <string.h> /* memset */
#include <assert.h>

/* Banco estático de textures */
static Texture2D textures[TEX_COUNT];
/* Rutas asociadas a cada TextureID (mantener en el mismo orden que el enum) */
static const char *asset_paths[TEX_COUNT] = {
    [TEX_BG_STARS_CLOSE] = "assets/images/background/stars_close.png",
    [TEX_BG_STARS_FAR]   = "assets/images/background/stars_far.png",
    [TEX_PLAYER]         = "assets/images/player.png",
    [TEX_ENEMY]          = "assets/images/enemies/enemy.png",
    [TEX_LOGO]           = "assets/images/logo.png",
    [TEX_HUD_FULLHEART]  = "assets/images/hud/hud.png",
    [TEX_HUD_HALFHEART]  = "assets/images/hud/hud.png",
    [TEX_HUD_EMPTYHEART] = "assets/images/hud/hud.png",
    [TEX_LOGO_GACHAPOW]  = "assets/images/logo.png",
};

/* Helper: carga una textura y la almacena en el banco */
static bool Assets_LoadTextureInternal(TextureID id, const char *path)
{
    if (id < 0 || id >= TEX_COUNT || path == NULL) return false;

    /* Si ya está cargada, no hacer nada */
    if (textures[id].id != 0) return true;

    Texture2D t = LoadTexture(path);
    if (t.id == 0) {
        /* fallo al cargar */
        fprintf(stderr, "Assets: fallo cargando texture '%s' (id=%d)\n", path, id);
        return false;
    }

    textures[id] = t;
    return true;
}

/* Inicializa el sistema de assets.
   Si prefieres carga eager, descomenta el bucle para precargar todo.
*/
void Assets_Init(void)
{
    memset(textures, 0, sizeof(textures));
    /* Si quieres precargar todo en el init, descomenta:
    for (int i = 0; i < TEX_COUNT; ++i) {
        if (asset_paths[i]) Assets_LoadTextureInternal((TextureID)i, asset_paths[i]);
    }
    */
}

/* Retorna una copia de Texture2D (como antes).
   Si el asset no está cargado intenta cargarlo de forma lazy usando asset_paths.
   Si tampoco hay ruta o falla, devuelve texture.id == 0 (texture vacía).
*/
Texture2D GetTextureAsset(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT) {
        /* devuelvo textura vacía */
        return (Texture2D){ 0 };
    }

    if (textures[id].id == 0) {
        /* intentar carga lazy desde la tabla de rutas */
        const char *path = asset_paths[id];
        if (path) {
            bool ok = Assets_LoadTextureInternal(id, path);
            if (!ok) {
                /* fallo: devolver textura vacía */
                return (Texture2D){ 0 };
            }
        } else {
            /* no hay ruta conocida */
            return (Texture2D){ 0 };
        }
    }

    return textures[id];
}

/* Comprueba si un texture está cargada */
bool Assets_IsLoaded(TextureID id)
{
    if (id < 0 || id >= TEX_COUNT) return false;
    return textures[id].id != 0;
}

/* Descargar todos los assets cargados de forma segura */
void Assets_Unload(void)
{
    for (int i = 0; i < TEX_COUNT; ++i) {
        if (textures[i].id != 0) {
            UnloadTexture(textures[i]);
            /* dejarlo en cero para evitar doble free accidental */
            textures[i] = (Texture2D){ 0 };
        }
    }
}

/* Funciones de utilidad: cargar manualmente un asset si necesitas cargar por ruta arbitraria */
bool Assets_LoadByPath(TextureID id, const char *path)
{
    if (id < 0 || id >= TEX_COUNT || path == NULL) return false;
    return Assets_LoadTextureInternal(id, path);
}
