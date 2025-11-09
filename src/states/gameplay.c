#include "gameplay.h"
#include "background_gameplay.h"
#include "player.h"
#include "enemy.h"
#include "hud.h"

static Enemy enemies[MAX_ENEMIES];

void Gameplay_Enter(void)
{
    InitBackgroundGameplay();
    InitPlayer();
    InitEnemies(enemies, MAX_ENEMIES, firstWave);
    InitHUD();
}

void Gameplay_Update(void)
{
    UpdateBackgroundGameplay();
    UpdatePlayer();
    UpdateEnemies(enemies, activeEnemies, currentWave);
    UpdateHUD();
}

void Gameplay_Draw(void)
{
    DrawBackgroundGameplay();
    DrawPlayer();
    DrawEnemies(enemies, activeEnemies, false);
    DrawHUD();
}

void Gameplay_Exit(void)
{
    UnloadEnemies(enemies, MAX_ENEMIES);
    UnloadPlayer();
    UnloadBackgroundGameplay();
    UnloadHUD();
}
