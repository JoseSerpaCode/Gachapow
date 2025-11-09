#include "assets.h"

// asset bank estático (solo texturas por ahora)
static Texture2D textures[TEX_COUNT];

// carga todos los assets gráficos del juego al inicio
// se llama 1 sola vez en el boot de hw_init()
// (luego las escenas solo piden con GetTextureAsset)
void Assets_Init(void) 
{
    textures[TEX_BG_STARS_CLOSE] = LoadTexture("assets/images/background/stars_close.png");
    textures[TEX_BG_STARS_FAR]   = LoadTexture("assets/images/background/stars_far.png");
    textures[TEX_PLAYER]         = LoadTexture("assets/images/player.png");
    textures[TEX_ENEMY]          = LoadTexture("assets/images/enemies/enemy.png");
    textures[TEX_LOGO]           = LoadTexture("assets/images/logo.png");
    textures[TEX_HUD]            = LoadTexture("assets/images/hud/hud.png");
}

// retorna referencia a texture ya cargada
Texture2D GetTextureAsset(TextureID id) 
{
    return textures[id];
}

// libera memoria GPU al cerrar el juego
void Assets_Unload(void) 
{
    for (int i = 0; i < TEX_COUNT; ++i)
        UnloadTexture(textures[i]);
}
