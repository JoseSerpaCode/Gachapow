#include "background.h"
#include "assets.h"
#include "hw_api.h"     // para GetScreenWidth / config si necesitás
#include "raylib.h"

static Texture2D bg1;
static Texture2D bg2;

static float bgScroll1 = 0.0f;
static float bgScroll2 = 0.0f;

// velocidades constantes (no números sueltos mágicos)
#define BG_SCROLL_NEAR   30.0f
#define BG_SCROLL_FAR    60.0f

void InitBackground(void)
{
    bg1 = GetTextureAsset(TEX_BG_STARS_CLOSE);
    bg2 = GetTextureAsset(TEX_BG_STARS_FAR);
}

void UpdateBackground(void)
{
    float dt = GetFrameTime();

    bgScroll1 += BG_SCROLL_NEAR * dt;
    bgScroll2 += BG_SCROLL_FAR  * dt;

    if (bgScroll1 >= bg1.height) bgScroll1 = 0;
    if (bgScroll2 >= bg2.height) bgScroll2 = 0;
}

void DrawBackground(void)
{
    ClearBackground((Color){29, 0, 71, 255});

    const int screenWidth  = GetScreenWidth();
    const int screenHeight = GetScreenHeight();

    Rectangle dst = { 0, 0, screenWidth, screenHeight };

    Rectangle src1  = { 0, bgScroll1,          bg1.width, bg1.height };
    Rectangle src1b = { 0, bgScroll1-bg1.height, bg1.width, bg1.height };

    DrawTexturePro(bg1, src1,  dst, (Vector2){0,0}, 0, WHITE);
    DrawTexturePro(bg1, src1b, dst, (Vector2){0,0}, 0, WHITE);

    Rectangle src2  = { 0, bgScroll2,          bg2.width, bg2.height };
    Rectangle src2b = { 0, bgScroll2-bg2.height, bg2.width, bg2.height };

    DrawTexturePro(bg2, src2,  dst, (Vector2){0,0}, 0, WHITE);
    DrawTexturePro(bg2, src2b, dst, (Vector2){0,0}, 0, WHITE);
}

void UnloadBackground(void)
{
    // las texturas son assets, no se descargan acá
}

//------------------------------------------------------------------------------------
// Placeholders para compatibilidad con estados (Gameplay, Menú, etc.)
//------------------------------------------------------------------------------------
void InitBackgroundGameplay(void)  { InitBackground(); }
void UpdateBackgroundGameplay(void){ UpdateBackground(); }
void DrawBackgroundGameplay(void)  { DrawBackground(); }
void UnloadBackgroundGameplay(void){ UnloadBackground(); }
