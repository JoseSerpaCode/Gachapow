#include "background.h"
#include "assets.h"
#include "extern/raylib.h"

//------------------------------------------------------------------------------
// Variables estáticas (solo visibles en este módulo)
//------------------------------------------------------------------------------
static Texture2D bgFar;   // capa lejana (más lenta)
static Texture2D bgNear;  // capa cercana (más rápida)
static float scrollFar = 0.0f;
static float scrollNear = 0.0f;

// Velocidades del parallax (ajusta según se vea bien en tu juego)
static const float BG_SPEED_FAR  = 30.0f;
static const float BG_SPEED_NEAR = 60.0f;

//------------------------------------------------------------------------------
// Inicialización: se llama una sola vez al iniciar el juego
//------------------------------------------------------------------------------
void InitBackground(void)
{
    bgFar  = GetTextureAsset(TEX_BG_STARS_CLOSE);
    bgNear = GetTextureAsset(TEX_BG_STARS_FAR);

    scrollFar  = 0.0f;
    scrollNear = 0.0f;
}

//------------------------------------------------------------------------------
// Actualización: mueve el fondo cada frame (efecto scroll hacia ABAJO)
//------------------------------------------------------------------------------
void UpdateBackground(void)
{
    float dt = GetFrameTime();

    // La nave sube → el fondo se desplaza hacia abajo
    scrollFar  -= BG_SPEED_FAR  * dt;
    scrollNear -= BG_SPEED_NEAR * dt;

    // Reinicio continuo (cuando la textura termina de pasar)
    if (scrollFar <= -bgFar.height)   scrollFar  = 0.0f;
    if (scrollNear <= -bgNear.height) scrollNear = 0.0f;
}

//------------------------------------------------------------------------------
// Dibujo: renderiza ambas capas del fondo con efecto de scroll continuo
//------------------------------------------------------------------------------
void DrawBackground(void)
{
    // Color base del cielo
    ClearBackground((Color){29, 0, 71, 255});

    int screenWidth  = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Rectangle dst = { 0, 0, (float)screenWidth, (float)screenHeight };

    // --- Capa lejana ---
    Rectangle srcFar  = { 0, scrollFar,  (float)bgFar.width,  (float)bgFar.height };
    Rectangle srcFar2 = { 0, scrollFar + (float)bgFar.height, (float)bgFar.width, (float)bgFar.height };
    DrawTexturePro(bgFar, srcFar,  dst, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(bgFar, srcFar2, dst, (Vector2){0, 0}, 0.0f, WHITE);

    // --- Capa cercana ---
    Rectangle srcNear  = { 0, scrollNear,  (float)bgNear.width,  (float)bgNear.height };
    Rectangle srcNear2 = { 0, scrollNear + (float)bgNear.height, (float)bgNear.width, (float)bgNear.height };
    DrawTexturePro(bgNear, srcNear,  dst, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(bgNear, srcNear2, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

//------------------------------------------------------------------------------
// Liberación: (nada que hacer, los assets se descargan en Assets_Unload())
//------------------------------------------------------------------------------
void UnloadBackground(void)
{
    // Nada: las texturas son manejadas por el asset manager global
}
