#include "raylib.h"
#include "hud.h"
#include <math.h>

static Texture2D hud;

void InitHUD(void) {
    hud = LoadTexture("assets/images/player.png");
}

void DrawHUD(int score, float elapsedTime, int lives) {
    // Solo dibuja la textura directamente
    DrawTexture(hud, 0, 0, WHITE);
}

void UnloadHUD(void) {
    UnloadTexture(hud);
}

void UpdateHUD(void) {
    // Si más adelante el HUD tiene animaciones, efectos o timers, van acá.
}
