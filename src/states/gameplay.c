#include "common.h"
#include "gameplay/player.h"
#include "gameplay/enemy.h"
#include "states/hud.h"

static Player player;
static Enemy enemies[NUM_MAX_ENEMIES];

static int activeEnemies = 0;
static int currentWave = 0;
static int firstWave = FIRST_WAVE;

static int score = 0;
static float elapsedTime = 0.0f;

void Gameplay_Enter(void) {
    InitBackgroundGameplay();       // si existe
    InitPlayer(&player);
    InitEnemies(enemies, NUM_MAX_ENEMIES, firstWave);
}

void Gameplay_Update(void) {
    UpdateBackgroundGameplay();     // si existe
    UpdatePlayer(&player);
    UpdateEnemies(enemies, activeEnemies, currentWave);
    UpdateHUD();
}

void Gameplay_Draw(void) {
    DrawBackgroundGameplay();       // si existe
    DrawPlayer(&player);
    DrawEnemies(enemies, activeEnemies, false);
    DrawHUD(score, elapsedTime, player.lives);
}

void Gameplay_Exit(void) {
    UnloadPlayer(&player);
    UnloadBackgroundGameplay();
}
