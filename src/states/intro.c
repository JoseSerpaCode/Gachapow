#include "states/intro.h"
#include "core/state_manager.h"
#include "raylib.h"
#include <stdio.h>

#define MAX_FRAMES 2000
#define FRAME_INTERVAL 0.21f

#define FADE_DURATION 2.0f     // Fade in y fade out
#define HOLD_DURATION 3.2f     // Tiempo pleno del logo

static Texture2D frames[MAX_FRAMES];
static int totalFrames = 0;

static Texture2D logo1;
static Texture2D logo2;

static int step = 0; // 0=video, 1=logo1, 2=logo2, 3=fin
static float timer = 0;
static int currentFrame = 0;

void Intro_Init()
{
    // ---- STEP 0: cargar frames del video ----
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "assets/images/sprites/pixil-frame-%d.png", i);

        Texture2D t = LoadTexture(name);
        if (t.id == 0)
            break;

        frames[totalFrames++] = t;
    }

    // ---- Logos ----
    logo1 = LoadTexture("assets/images/logo.png");
    logo2 = LoadTexture("assets/images/player.png");

    step = 0;
    timer = 0;
    currentFrame = 0;

    // Espera inicial en negro
    double waitStart = GetTime();
    while (GetTime() - waitStart < 1.5)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
}

void Intro_Update()
{
    float dt = GetFrameTime();

    switch (step)
    {
    case 0: // Video
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

    case 1: // Logo 1 animación completa
    case 2: // Logo 2 animación completa
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

    case 3:
        StateManager_Change(STATE_GAMEPLAY);
        break;
    }
}

void Intro_Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    switch (step)
    {
    case 0: // Video centrado
    {
        Texture2D tex = frames[currentFrame];

        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        // Escala proporcional
        float scaleX = (float)screenW / tex.width;
        float scaleY = (float)screenH / tex.height;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;

        float dstW = tex.width * scale;
        float dstH = tex.height * scale;

        float posX = (screenW - dstW) * 0.5f;
        float posY = (screenH - dstH) * 0.5f;

        Rectangle src = {0, 0, tex.width, tex.height};
        Rectangle dst = {posX, posY, dstW, dstH};
        Vector2 origin = {0, 0};

        DrawTexturePro(tex, src, dst, origin, 0.0f, WHITE);
    }
    break;

    case 1:
    case 2:
    {
        Texture2D tex = (step == 1) ? logo1 : logo2;

        float alpha = 1.0f;

        if (timer < FADE_DURATION)
        {
            alpha = timer / FADE_DURATION; // Fade-in
        }
        else if (timer < FADE_DURATION + HOLD_DURATION)
        {
            alpha = 1.0f; // Pleno
        }
        else
        {
            float t = timer - (FADE_DURATION + HOLD_DURATION);
            alpha = 1.0f - (t / FADE_DURATION); // Fade-out
        }

        // Centrado del logo
        int screenW = GetScreenWidth();
        int screenH = GetScreenHeight();

        float posX = (screenW - tex.width) * 0.5f;
        float posY = (screenH - tex.height) * 0.5f;

        Color tint = WHITE;
        tint.a = (unsigned char)(alpha * 255);

        DrawTexture(tex, posX, posY, tint);
    }
    break;
    }

    EndDrawing();
}

void Intro_Unload()
{
    for (int i = 0; i < totalFrames; i++)
        UnloadTexture(frames[i]);

    UnloadTexture(logo1);
    UnloadTexture(logo2);
}
