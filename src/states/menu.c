// ============================================================================
// menu.c – Menú principal estilo arcade
// ============================================================================
#include "menu.h"
#include "state_manager.h"
#include "raylib.h"
#include "assets.h"
#include "config.h"
#include <stdio.h>

extern GameConfig config;

// Textura del fondo del menú
static Texture2D bg;

// Timer para parpadeo de "INSERT COIN"
static float blinkTimer = 0.0f;
static bool blinkVisible = true;

// Velocidad del parpadeo
#define BLINK_INTERVAL 0.65f

// ============================================================================
// INIT
// ============================================================================
void Menu_Init()
{
    bg = GetTextureAsset(TEX_MENU_BACKGROUND);

    // Filtro estilo pixel art
    SetTextureFilter(bg, TEXTURE_FILTER_POINT);

    blinkTimer = 0;
    blinkVisible = true;
}


// ============================================================================
// UPDATE
// ============================================================================
void Menu_Update()
{
    float dt = GetFrameTime();
    blinkTimer += dt;

    // Alternar visibilidad
    if (blinkTimer >= BLINK_INTERVAL)
    {
        blinkTimer = 0;
        blinkVisible = !blinkVisible;
    }

    // Si el jugador presiona START / CONFIRM / SPACE → empieza el juego
    if (IsKeyPressed(KEY_ENTER) ||
        IsKeyPressed(KEY_SPACE) ||
        IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
    {
        StateManager_Change(STATE_GAMEPLAY);
    }
}


// ============================================================================
// DRAW
// ============================================================================
void Menu_Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // Dibujar fondo a pantalla completa
    DrawTexturePro(
        bg,
        (Rectangle){0, 0, bg.width, bg.height},
        (Rectangle){0, 0, sw, sh},
        (Vector2){0, 0},
        0.0f,
        WHITE
    );

    // -------------------------
    // TEXTO: "INSERT COIN"
    // -------------------------
    const char* msg = "INSERT COIN";

    int fontSize = 50;
    int tw = MeasureText(msg, fontSize);

    if (blinkVisible)
    {
        DrawText(
            msg,
            (sw - tw) / 2,
            sh * 0.78f,
            fontSize,
            (Color){255, 255, 0, 255}   // amarillo arcade
        );
    }

    EndDrawing();
}


// ============================================================================
// UNLOAD
// ============================================================================
void Menu_Unload()
{
    // Fondo está en Assets.c, NO se debe descargar aquí.
    // Lo maneja Assets_Unload() globalmente.
}
