#pragma once
#include "extern/raylib.h"

void InitHUD(void);
void UnloadHUD(void);
void DrawHUD(int score, float elapsedTime, int lives);

/* setters (ajusta posiciones en px) */
void HUD_SetScorePos(int x, int y);
void HUD_SetTimePos(int x, int y);
void HUD_SetHeartPos(int x, int y);

/* controla máximo de vidas (por si cambia) */
void HUD_SetMaxLives(int maxLives);
int HUD_GetMaxLives(void);
