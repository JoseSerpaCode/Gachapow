#include "menu.h"
#include "state_manager.h"
#include "raylib.h"
#include "assets.h"
#include "config.h"
#include "hw_api.h"
#include <stdio.h>
#include <math.h>

// =============================================
// Intervalos de animación para los 2 intros
// =============================================
#define FRAME_INTERVAL_1 0.06f
#define FRAME_INTERVAL_2 0.15f

// Tiempo antes de reiniciar attract mode cuando está en menú
#define MENU_TIMEOUT 35.0f //

// =============================================
// Variables internas
// =============================================
static float timer = 0.0f;
static float globalTimer = 0.0f;
static int step = 0;
static int currentFrame = 0;

static Texture2D menu_background;

// Estado del sistema de monedas
static bool coinInserted = false;   // Se insertó al menos una moneda
static float coinFlashTimer = 0.0f; // Parpadeo rápido 1s
static bool readyToStart = false;   // Mostrar el mensaje final

// =====================================================
// Referencias externas a las animaciones
// =====================================================
extern Texture2D intro1_frames[];
extern Texture2D intro2_frames[];

extern int intro1_frames_count;
extern int intro2_frames_count;

// =====================================================
// Dibuja un frame escalado y centrado a pantalla
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
        tex.height * scale};

    DrawTexturePro(tex, (Rectangle){0, 0, tex.width, tex.height}, dst,
                   (Vector2){0, 0}, 0, WHITE);
}

// =====================================================
void Menu_Init(void)
{
    menu_background = GetTextureAsset(TEX_MENU_BACKGROUND);

    timer = 0;
    globalTimer = 0;
    step = 0;
    currentFrame = 0;

    coinInserted = false;
    coinFlashTimer = 0;
    readyToStart = false;
}

// =====================================================
void Menu_Update(void)
{
    float dt = GetFrameTime();
    timer += dt;

    // =====================================================
    // 1) Detectar moneda EN CUALQUIER ETAPA
    // =====================================================
    if (!coinInserted && hw_coin_inserted())
    {
        coinInserted = true;
        coinFlashTimer = 1.0f; // parpadeo rápido 1s
        step = 13;             // forzar menú estático al instante
        timer = 0;
        globalTimer = 0;
    }

    // =====================================================
    // 2) Manejo del parpadeo rápido
    // =====================================================
    if (coinInserted && coinFlashTimer > 0)
    {
        coinFlashTimer -= dt;
        if (coinFlashTimer <= 0)
            readyToStart = true;
    }

    // =====================================================
    // 3) Si ya está listo, cualquier input inicia el juego
    // =====================================================
    if (readyToStart)
    {
        if (hw_any_button_pressed() || IsKeyPressed(KEY_ENTER))
        {
            StateManager_Change(STATE_GAMEPLAY);
            return;
        }
    }

    // =====================================================
    // 4) Manejo del timeout para reiniciar attract mode
    // =====================================================
    if (step == 13) // solo cuando estamos en el menú final
    {
        globalTimer += dt;

        if (globalTimer >= MENU_TIMEOUT)
        {
            // Reinicio total de animaciones
            step = 0;
            timer = 0;
            currentFrame = 0;
            globalTimer = 0;

            // Si NO hay moneda, reiniciamos mensajes
            if (!coinInserted)
            {
                readyToStart = false;
                coinFlashTimer = 0;
            }
        }
    }
    else
    {
        globalTimer = 0; // no cuenta si estamos en animaciones
    }

    // =====================================================
    // 5) Secuencia clásica del attract mode
    // =====================================================
    switch (step)
    {
    case 0:
        step = 1;
        currentFrame = 0;
        break;

    case 1: // intro1
        if (timer >= FRAME_INTERVAL_1)
        {
            timer = 0;
            if (++currentFrame >= intro1_frames_count)
            {
                step = 5;
                currentFrame = 0;
            }
        }
        break;

    case 5: // intro2
        if (timer >= FRAME_INTERVAL_2)
        {
            timer = 0;
            if (++currentFrame >= intro2_frames_count)
            {
                step = 13; // ← salto directo al menú estático
                timer = 0;
            }
        }
        break;

    case 13: // menú estático
        // (el timeout del attract mode se maneja arriba)
        break;
    }
}

// =====================================================
void Menu_Draw(void)
{
    ClearBackground(BLACK);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Dibujo del attract mode
    switch (step)
    {
    case 1:
        DrawFrameCentered(intro1_frames[currentFrame]);
        break;
    case 5:
        DrawFrameCentered(intro2_frames[currentFrame]);
        break;
    case 13:
        DrawFrameCentered(menu_background);
        break;
    }

    // =====================================================
    // TEXTO: INSERT COIN / PRESS BUTTON TO START
    // =====================================================
    if (true) // SIEMPRE visible en menú estático
    {
        float pulse = sinf(GetTime() * (coinFlashTimer > 0 ? 16.0f : 6.0f));
        float scale = 1.0f + 0.06f * pulse;
        int size = (int)(42 * scale);

        const char *msg;

        if (!coinInserted)
            msg = "INSERT COIN";
        else if (!readyToStart)
            msg = "INSERT COIN"; // pero parpadeo rápido
        else
            msg = "PRESIONA UN BOTON PARA CONTINUAR";

        Color glow, main;

        if (!readyToStart)
        {
            glow = (Color){255, 255, 180, 200};
            main = (Color){255, 255, 120, 255};
        }
        else
        {
            glow = (Color){255, 140, 40, 255};
            main = (Color){255, 90, 0, 255};
        }

        int tw = MeasureText(msg, size);
        int x = sw / 2 - tw / 2;
        int y = sh - 95;

        DrawText(msg, x + 3, y + 3, size, glow);
        DrawText(msg, x, y, size, main);
    }
}

// =====================================================
void Menu_Unload(void)
{
    // El sistema de assets gestiona las texturas
}
