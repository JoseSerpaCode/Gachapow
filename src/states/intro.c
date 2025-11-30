#include "states/intro.h"
#include "core/state_manager.h"
#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include "config.h"

#define MAX_FRAMES 2000
#define FRAME_INTERVAL 0.20f

#define FADE_DURATION 2.7f
#define HOLD_DURATION 3.7f

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

extern GameConfig config;

// Frames del video
static Texture2D frames[MAX_FRAMES];
static int totalFrames = 0;

// Logo principal
static Texture2D logo1;

// Estado general del intro
// 0 = video
// 1 = logo
// 2 = texto legal
// 3 = fin
static int step = 0;

static float timer = 0;
static int currentFrame = 0;

// Texto legal multilínea
static const char *legalText =
    "Este juego utiliza el lenguaje C junto a la libreria Raylib.\n"
    "El sistema es desarrollado por el equipo 'GP Environment'.\n"
    "Todos los derechos reservados.\n\n"
    "Los personajes, historias, nombres y eventos representados en este juego\n"
    "son ficticios. Cualquier similitud con personas reales es coincidencia.\n\n"
    "El software está protegido por leyes internacionales de derechos de autor.\n"
    "La reproducción total o parcial sin autorización está prohibida.\n\n"
    "© 2026 Gachapow. Todos los derechos reservados.";


// ============================================================================
// MEDIR TEXTO MULTILÍNEA
// ============================================================================
int MeasureMultilineText(const char *text, int fontSize)
{
    int maxWidth = 0;
    int width = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '\n')
        {
            if (width > maxWidth)
                maxWidth = width;
            width = 0;
        }
        else
        {
            width += MeasureText("A", fontSize);
        }
    }

    if (width > maxWidth)
        maxWidth = width;
    return maxWidth;
}

// ============================================================================
// INIT
// ============================================================================
void Intro_Init()
{
    // Cargar frames del video
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "assets/images/sprites/pixil-frame-%d.png", i);

        Texture2D t = LoadTexture(name);
        if (t.id == 0)
            break;

        frames[totalFrames++] = t;
    }

    // Cargar logo
    logo1 = GetTextureAsset(TEX_LOGO_GACHAPOW);
    SetTextureFilter(logo1, TEXTURE_FILTER_POINT);

    // Reset de variables
    step = 0;
    timer = 0;
    currentFrame = 0;

    // Pantalla negra inicial
    double waitStart = GetTime();
    while (GetTime() - waitStart < 1.5)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
}

// ============================================================================
// UPDATE
// ============================================================================
void Intro_Update()
{
    float dt = GetFrameTime();

    switch (step)
    {
    // --------------------------------------------------------------------
    case 0: // Video frame por frame
        // --------------------------------------------------------------------
        timer += dt;

        if (timer >= FRAME_INTERVAL)
        {
            timer -= FRAME_INTERVAL;
            currentFrame++;

            if (currentFrame >= totalFrames)
            {
                step = 1;
                timer = 0;
            }
        }
        break;

    // --------------------------------------------------------------------
    case 1: // Logo
    case 2: // Texto legal
        // --------------------------------------------------------------------
        {
            timer += dt;
            float totalTime = FADE_DURATION + HOLD_DURATION + FADE_DURATION;

            if (timer >= totalTime)
            {
                step++;
                timer = 0;
            }
        }
        break;

    // --------------------------------------------------------------------
    case 3:
        StateManager_Change(STATE_GAMEPLAY);
        break;
    }
}

// ============================================================================
// DRAW
// ============================================================================
void Intro_Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    switch (step)
    {
    // --------------------------------------------------------------------
    case 0: // VIDEO
        // --------------------------------------------------------------------
        {
            Texture2D tex = frames[currentFrame];

            float scaleX = (float)screenW / tex.width;
            float scaleY = (float)screenH / tex.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            Rectangle dst = {
                (screenW - tex.width * scale) * 0.5f,
                (screenH - tex.height * scale) * 0.5f,
                tex.width * scale,
                tex.height * scale};

            DrawTexturePro(tex,
                           (Rectangle){0, 0, tex.width, tex.height},
                           dst,
                           (Vector2){0, 0},
                           0.0f,
                           WHITE);
        }
        break;

    // --------------------------------------------------------------------
    case 1: // LOGO
        // --------------------------------------------------------------------
        {
            Texture2D tex = logo1;

            float alpha;
            if (timer < FADE_DURATION)
                alpha = timer / FADE_DURATION;
            else if (timer < FADE_DURATION + HOLD_DURATION)
                alpha = 1.0f;
            else
                alpha = 1.0f - ((timer - (FADE_DURATION + HOLD_DURATION)) / FADE_DURATION);

            float scaleX = (float)screenW / tex.width;
            float scaleY = (float)screenH / tex.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            Rectangle dst = {
                (screenW - tex.width * scale) * 0.5f,
                (screenH - tex.height * scale) * 0.5f,
                tex.width * scale,
                tex.height * scale};

            Color tint = WHITE;
            tint.a = (unsigned char)(alpha * 255);

            DrawTexturePro(tex,
                           (Rectangle){0, 0, tex.width, tex.height},
                           dst,
                           (Vector2){0, 0},
                           0.0f,
                           tint);
        }
        break;

    // --------------------------------------------------------------------
    case 2: // TEXTO LEGAL
        // --------------------------------------------------------------------
        {
            float alpha;
            if (timer < FADE_DURATION)
                alpha = timer / FADE_DURATION;
            else if (timer < FADE_DURATION + HOLD_DURATION)
                alpha = 1.0f;
            else
                alpha = 1.0f - ((timer - (FADE_DURATION + HOLD_DURATION)) / FADE_DURATION);

            Color tint = WHITE;
            tint.a = (unsigned char)(alpha * 255);

            int fontSize = 20;
            int spacing = 5;

            int textW = MeasureMultilineText(legalText, fontSize);
            int posX = (screenW - textW) / 2;
            int posY = screenH * 0.15f;

            char line[512];
            int idx = 0;
            int y = posY;

            const char *p = legalText;

            while (*p)
            {
                if (*p == '\n')
                {
                    line[idx] = '\0';
                    DrawText(line, posX, y, fontSize, tint);
                    y += fontSize + spacing;
                    idx = 0;
                }
                else
                {
                    line[idx++] = *p;
                }
                p++;
            }

            line[idx] = '\0';
            DrawText(line, posX, y, fontSize, tint);
        }
        break;
    }

    EndDrawing();
}

// ============================================================================
// UNLOAD
// ============================================================================
void Intro_Unload()
{
    for (int i = 0; i < totalFrames; i++)
        UnloadTexture(frames[i]);

    UnloadTexture(logo1);
}
