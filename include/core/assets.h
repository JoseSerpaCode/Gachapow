#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"

// Identificadores de texturas cargadas 1 sola vez al inicio del juego
// son la API pública para pedir texturas sin repetir rutas en código
typedef enum {
    TEX_BG_STARS_CLOSE,
    TEX_BG_STARS_FAR,
    TEX_PLAYER,
    TEX_ENEMY,
    TEX_LOGO,

    TEX_COUNT  // MUST be last → tamaño del array interno
} TextureID;

// carga TODOS los assets estáticos del juego (solo una vez)
void Assets_Init(void);

// retorna la textura asociada a un asset ID
// NO duplica recursos, solo referencia
Texture2D GetTextureAsset(TextureID id);

// libera TODOS los assets cargados por Assets_Init()
void Assets_Unload(void);

#endif
