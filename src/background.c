#include "background.h"
#include "assets.h"

static Texture2D bg1;
static Texture2D bg2;
static float bgScroll1 = 0.0f;
static float bgScroll2 = 0.0f;

void InitBackground(void)
{
    bg1 = GetTextureAsset(TEX_BG_STARS_CLOSE);
    bg2 = GetTextureAsset(TEX_BG_STARS_FAR);
}

void UpdateBackground(void)
{
    float dt = GetFrameTime();

    // movimiento hacia abajo
    bgScroll1 -= 30 * dt;
    bgScroll2 -= 60 * dt;

    if (bgScroll1 >= bg1.height) bgScroll1 = 0;
    if (bgScroll2 >= bg2.height) bgScroll2 = 0;
}

void DrawBackground(void)
{
    ClearBackground((Color){29, 0, 71, 255});

    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    // Escalado proporcional para cubrir la pantalla
    Rectangle src1 = { 0, bgScroll1, bg1.width, bg1.height };
    Rectangle src1b = { 0, bgScroll1 - bg1.height, bg1.width, bg1.height };
    Rectangle dst = { 0, 0, screenWidth, screenHeight };

    DrawTexturePro(bg1, src1, dst, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(bg1, src1b, dst, (Vector2){0, 0}, 0.0f, WHITE);

    Rectangle src2 = { 0, bgScroll2, bg2.width, bg2.height };
    Rectangle src2b = { 0, bgScroll2 - bg2.height, bg2.width, bg2.height };

    DrawTexturePro(bg2, src2, dst, (Vector2){0, 0}, 0.0f, WHITE);
    DrawTexturePro(bg2, src2b, dst, (Vector2){0, 0}, 0.0f, WHITE);
}

void UnloadBackground(void)
{
}
