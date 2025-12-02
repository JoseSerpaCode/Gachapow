#include "menu.h"
#include "state_manager.h"
#include "raylib.h"
#include "assets.h"
#include "config.h"
#include "hw_api.h"
#include <stdio.h>
#include <math.h>

#define FRAME_INTERVAL_1 0.10f
#define FRAME_INTERVAL_2 0.20f
#define FRAME_INTERVAL_3 0.20f

#define MENU_TIMEOUT 6.0f

static float timer = 0.0f;
static float globalTimer = 0.0f;
static int step = 0;
static int currentFrame = 0;

static Texture2D menu_background;

extern Texture2D intro1_frames[];
extern Texture2D intro2_frames[];
extern Texture2D intro3_frames[];

extern int intro1_frames_count;
extern int intro2_frames_count;
extern int intro3_frames_count;


// =====================================================
// Frame centrado y escalado estable
// =====================================================
static void DrawFrameCentered(Texture2D tex)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    float sx = (float)sw / tex.width;
    float sy = (float)sh / tex.height;
    float scale = (sx < sy) ? sx : sy;

    Rectangle dst = {
        (sw - tex.width * scale) * 0.5f,
        (sh - tex.height * scale) * 0.5f,
        tex.width * scale,
        tex.height * scale
    };

    DrawTexturePro(tex, (Rectangle){0,0,tex.width,tex.height}, dst,
                   (Vector2){0,0}, 0, WHITE);
}


// =====================================================
void Menu_Init(void)
{
    menu_background = GetTextureAsset(TEX_MENU_BACKGROUND);
    timer = 0.0f;
    globalTimer = 0.0f;
    step = 0;
    currentFrame = 0;
}


// =====================================================
void Menu_Update(void)
{
    float dt = GetFrameTime();
    timer += dt;

    // Cuando ya mostramos menú final → permitimos loop del attract mode
    if (step == 13)
    {
        globalTimer += dt;
        if (globalTimer >= MENU_TIMEOUT)
        {
            step = 0;
            timer = 0;
            currentFrame = 0;
            globalTimer = 0;
        }
    }
    else {
        globalTimer = 0;
    }

    // Máquina de estados del attract mode
    switch (step)
    {
        case 0:
            currentFrame = 0;
            step = 1;
            break;

        case 1:
            if (timer >= FRAME_INTERVAL_1)
            {
                timer = 0;
                currentFrame++;
                if (currentFrame >= intro1_frames_count)
                {
                    currentFrame = 0;
                    step = 5;
                }
            }
            break;

        case 5:
            if (timer >= FRAME_INTERVAL_2)
            {
                timer = 0;
                currentFrame++;
                if (currentFrame >= intro2_frames_count)
                {
                    currentFrame = 0;
                    step = 9;
                }
            }
            break;

        case 9:
            if (timer >= FRAME_INTERVAL_3)
            {
                timer = 0;
                currentFrame++;
                if (currentFrame >= intro3_frames_count)
                {
                    currentFrame = 0;
                    step = 12;
                }
            }
            break;

        case 12:
            if (timer >= 2.0f)
            {
                timer = 0;
                step = 13;
            }
            break;

        case 13:
            // Aquí ya estamos en menú final
            // COIN MECH UNIVERSAL
            if (hw_coin_inserted())
            {
                StateManager_Change(STATE_GAMEPLAY);
            }
            break;
    }
}


// =====================================================
void Menu_Draw(void)
{
    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    if (step == 13)
        BeginDrawing(), ClearBackground(BLACK);
    else
        BeginDrawing();

    switch (step)
    {
        case 1:  DrawFrameCentered(intro1_frames[currentFrame]); break;
        case 5:  DrawFrameCentered(intro2_frames[currentFrame]); break;
        case 9:  DrawFrameCentered(intro3_frames[currentFrame]); break;
        case 12: DrawFrameCentered(intro3_frames[intro3_frames_count - 1]); break;
        case 13: DrawFrameCentered(menu_background); break;
    }

    // ================================
    // INSERT COIN animado
    // ================================
    if (step == 13)
    {
        float pulse = 1.0f + 0.06f * sinf(GetTime() * 6.0f);
        int size = (int)(42 * pulse);

        const char* msg = "INSERT COIN";
        int tw = MeasureText(msg, size);
        int x = sw/2 - tw/2;
        int y = sh - 95;

        Color glow = {255, 255, 180, 200};
        Color main = {255, 255, 120, 255};

        DrawText(msg, x+3, y+3, size, glow);
        DrawText(msg, x,   y,   size, main);
    }

    EndDrawing();
}


// =====================================================
void Menu_Unload(void) {}
