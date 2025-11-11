// src/states/hud.c
#include "states/hud.h"
#include "assets.h"
#include "extern/raylib.h"
#include <stdio.h>

/* Posiciones por defecto (basadas en la ventana, se usan si no cambias con setters) */
static int scorePosX = 520; /* X del borde derecho */
static int scorePosY = 8;
static int timePosX  = 270; /* centro X */
static int timePosY  = 12;  /* justo debajo del top bar; ajusta si quieres más abajo */
static int heartPosX = 16;  /* izquierda */
static int heartPosY = 8;   /* arriba */

/* Tamaños de fuente */
static int hudFontSizeScore = 22;
static int hudFontSizeTime  = 18;
static int hudFontSizeFallback = 14;

/* Texturas de 3 estados (las tienes en assets) */
static Texture2D tex_full  = {0};
static Texture2D tex_half  = {0};
static Texture2D tex_empty = {0};

/* Max vidas (por defecto 3) */
static int hud_max_lives = 3;

/* Estado para evitar recalcular cada frame */
static int lastLives = -1;
static Texture2D *currentHeartTex = NULL;

/* ---------------- helpers ---------------- */
static Texture2D *pick_heart_texture(int lives)
{
    if (lives >= 3) return (tex_full.id ? &tex_full : NULL);
    if (lives == 2) return (tex_half.id ? &tex_half : NULL);
    return (tex_empty.id ? &tex_empty : NULL);
}

/* Dibuja texto con sombra para legibilidad */
static void draw_text_shadowed(const char *text, int x, int y, int fontSize, Color fg)
{
    DrawText(text, x+1, y+1, fontSize, BLACK);
    DrawText(text, x,   y,   fontSize, fg);
}

/* Right align: x = right edge */
static void draw_score_right_aligned(int score, int xRight, int y, int fontSize)
{
    char buf[64];
    snprintf(buf, sizeof(buf), "%d", score);
    int w = MeasureText(buf, fontSize);
    /* si xRight está fuera de pantalla, recalcula para anchura de pantalla */
    if (xRight <= 0) xRight = GetScreenWidth() - 8;
    draw_text_shadowed(buf, xRight - w, y, fontSize, RAYWHITE);
}

static void draw_time_mmss(float elapsed, int centerX, int y, int fontSize)
{
    int seconds = (int)elapsed;
    int mm = seconds / 60;
    int ss = seconds % 60;
    char buf[64];
    snprintf(buf, sizeof(buf), "%02d:%02d", mm, ss);
    int w = MeasureText(buf, fontSize);
    if (centerX <= 0) centerX = GetScreenWidth() / 2;
    draw_text_shadowed(buf, centerX - w/2, y, fontSize, LIGHTGRAY);
}

/* ---------------- Init / Unload ---------------- */
void InitHUD(void)
{
    /* Cargar las 3 texturas de vida */
    tex_full  = GetTextureAsset(TEX_HUD_FULLHEART);
    tex_half  = GetTextureAsset(TEX_HUD_HALFHEART);
    tex_empty = GetTextureAsset(TEX_HUD_EMPTYHEART);

    lastLives = -1;
    currentHeartTex = pick_heart_texture(hud_max_lives);

    /* Ajuste inicial basado en pantalla size (opcional) */
    int sw = GetScreenWidth();
    scorePosX = sw - 12;
    scorePosY = 8;
    timePosX = sw / 2;
    timePosY = 12;
}

/* No liberamos texturas: Asset manager se encarga */
void UnloadHUD(void)
{
    lastLives = -1;
    currentHeartTex = NULL;
}

/* ---------------- Setters ---------------- */
void HUD_SetScorePos(int x, int y) { scorePosX = x; scorePosY = y; }
void HUD_SetTimePos(int x, int y)  { timePosX  = x; timePosY  = y; }
void HUD_SetHeartPos(int x, int y) { heartPosX = x; heartPosY = y; }
void HUD_SetMaxLives(int maxLives) { hud_max_lives = (maxLives > 0 ? maxLives : 3); }
int HUD_GetMaxLives(void) { return hud_max_lives; }

/* ---------------- DrawHUD ---------------- */
void DrawHUD(int score, float elapsedTime, int lives)
{
    /* Score (solo número) - a la derecha */
    draw_score_right_aligned(score, scorePosX, scorePosY, hudFontSizeScore);

    /* Time (centrado en timePosX) */
    draw_time_mmss(elapsedTime, timePosX, timePosY, hudFontSizeTime);

    /* Heart (imagen) */
    if (lives != lastLives) {
        currentHeartTex = pick_heart_texture(lives);
        lastLives = lives;
    }

    if (currentHeartTex && currentHeartTex->id != 0) {
        /* Dibuja el sprite en la esquina/posición indicada */
        DrawTexture(*currentHeartTex, 0, 0, WHITE);
    } else {
        /* Fallback visible si no hay imagen */
        char buf[32];
        snprintf(buf, sizeof(buf), "LIVES: %d", lives);
        draw_text_shadowed(buf, heartPosX, heartPosY, hudFontSizeFallback, RED);
    }
}
