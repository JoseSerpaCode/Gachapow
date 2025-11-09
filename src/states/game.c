#include "extern/raylib.h"
#include "common.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "shoot.h"
#include "hud.h"
#include "hw_api.h"
#include "background.h"
#include "assets.h"

//------------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------------
static const int screenWidth = 540;
static const int screenHeight = 700;

bool gameOver = false;
bool pause = false;
bool victory = false;

int score = 0;
int enemiesKill = 0;
int shootRate = 0;
int activeEnemies = 0;

float alpha = 0.0f;
float elapsedTime = 0.0f;
bool smooth = false;

Player player;
Enemy enemy[NUM_MAX_ENEMIES];
Shoot shoot[NUM_SHOOTS];
EnemyWave wave;

static bool rewardDispensed = false;
static prize_status_t prizeState = PRIZE_IDLE;
//------------------------------------------------------------------------------------
// Limpieza de enemigos (al cambiar de ola)
//------------------------------------------------------------------------------------
void DestroyAllEnemies(Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; i++)
        enemies[i].active = false;
}

//------------------------------------------------------------------------------------
void InitGame(void)
{
    Assets_Init();

    gameOver = false;
    pause = false;
    victory = false;
    score = 0;
    enemiesKill = 0;
    smooth = false;
    alpha = 0;
    elapsedTime = 0;
    shootRate = 0;
    wave = FIRST;
    activeEnemies = FIRST_WAVE;

    InitPlayer(&player);
    InitEnemies(enemy, activeEnemies, wave);
    InitShoots(shoot);
    InitBackground();

    InitHUD();
}

//------------------------------------------------------------------------------------
void UpdateGame(void)
{
    UpdateBackground();
    // --- Reinicio de juego ---
    if (gameOver || victory)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            hw_reset_dispense();
            rewardDispensed = false;
            prizeState = PRIZE_IDLE;
            InitGame();
        }
        return;
    }

    if (IsKeyPressed(KEY_F1))
        victory = true;

    // --- Pausa ---
    if (IsKeyPressed('P'))
        pause = !pause;
    if (pause)
        return;

    float dt = GetFrameTime();
    elapsedTime += dt;

    // --- Tiempo límite ---
    if (elapsedTime >= 180.0f)
    {
        gameOver = true;
        return;
    }

    // --- Lógica principal ---
    UpdatePlayer(&player);
    UpdateEnemies(enemy, activeEnemies, wave);
    UpdateShoots(shoot, enemy, activeEnemies, wave, &enemiesKill, &score, &shootRate, player.rec);

    // --- Colisiones jugador vs enemigos ---
    for (int i = 0; i < activeEnemies; i++)
    {
        if (enemy[i].active && CheckCollisionRecs(player.collisionRec, enemy[i].collisionRec))
        {
            if (player.invulnerableTime <= 0)
            {
                player.lives--;
                player.invulnerableTime = 2.0f; // 👈 2 segundos invulnerable

                if (player.lives <= 0)
                {
                    gameOver = true;
                    return;
                }
            }

            enemy[i].active = false;
        }
    }

    // --- Control de oleadas ---
    switch (wave)
    {
    case FIRST:
    {
        if (!smooth)
        {
            alpha += 0.02f;
            if (alpha >= 1.0f)
                smooth = true;
        }
        if (smooth)
            alpha -= 0.02f;

        // ✅ Corrige condición: cuando mata a todos los enemigos activos
        if (enemiesKill >= activeEnemies)
        {
            enemiesKill = 0;
            wave = SECOND;
            activeEnemies = SECOND_WAVE;
            smooth = false;
            alpha = 0.0f;

            // 🔥 Limpia enemigos anteriores
            DestroyAllEnemies(enemy, NUM_MAX_ENEMIES);
            InitEnemies(enemy, activeEnemies, wave);
        }
    }
    break;

    case SECOND:
    {
        if (!smooth)
        {
            alpha += 0.02f;
            if (alpha >= 1.0f)
                smooth = true;
        }
        if (smooth)
            alpha -= 0.02f;

        if (enemiesKill >= activeEnemies)
        {
            enemiesKill = 0;
            wave = THIRD;
            activeEnemies = THIRD_WAVE;
            smooth = false;
            alpha = 0.0f;

            DestroyAllEnemies(enemy, NUM_MAX_ENEMIES);
            InitEnemies(enemy, activeEnemies, wave);
        }
    }
    break;

    case THIRD:
    {
        if (!smooth)
        {
            alpha += 0.02f;
            if (alpha >= 1.0f)
                smooth = true;
        }
        if (smooth)
            alpha -= 0.02f;

        // 🏆 Cuando mata todos los enemigos de la tercera wave
        if (enemiesKill >= activeEnemies)
        {
            victory = true;
        }
    }
    break;
    }
}

//------------------------------------------------------------------------------------
void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawBackground();

    if (!gameOver)
    {
        DrawPlayer(&player);
        DrawEnemies(enemy, activeEnemies, true);

        for (int i = 0; i < NUM_SHOOTS; i++)
            if (shoot[i].active)
                DrawRectangleRec(shoot[i].rec, shoot[i].color);

        // --- Texto de waves ---
        if (wave == FIRST)
            DrawText("FIRST WAVE", screenWidth / 2 - MeasureText("FIRST WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == SECOND)
            DrawText("SECOND WAVE", screenWidth / 2 - MeasureText("SECOND WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));
        else if (wave == THIRD)
            DrawText("THIRD WAVE", screenWidth / 2 - MeasureText("THIRD WAVE", 40) / 2, screenHeight / 2 - 40, 40, Fade(BLACK, alpha));

        DrawHUD(score, elapsedTime, player.lives);

        // --- 🏆 Victoria ---
        if (victory)
        {
            if (!rewardDispensed)
            {
                prizeState = hw_dispense_request();

                if (prizeState == PRIZE_DONE)
                    rewardDispensed = true;
            }

            DrawText("YOU WIN!", screenWidth / 2 - MeasureText("YOU WIN!", 40) / 2,
                     screenHeight / 2 - 40, 40, BLACK);

            if (!rewardDispensed)
                DrawText("DISPENSING REWARD...",
                         screenWidth / 2 - MeasureText("DISPENSING REWARD...", 20) / 2,
                         screenHeight / 2 + 20, 20, GRAY);
            else
                DrawText("REWARD DISPENSED!",
                         screenWidth / 2 - MeasureText("REWARD DISPENSED!", 20) / 2,
                         screenHeight / 2 + 20, 20, DARKGREEN);
        }

        // --- Pausa ---
        else if (pause)
        {
            DrawText("GAME PAUSED", screenWidth / 2 - MeasureText("GAME PAUSED", 40) / 2, screenHeight / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        // --- Game Over ---
        int finalScore = score + (int)(elapsedTime * 5.0f);
        DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 60, 40, BLACK);
        DrawText(TextFormat("FINAL SCORE: %i", finalScore),
                 screenWidth / 2 - MeasureText(TextFormat("FINAL SCORE: %i", finalScore), 20) / 2,
                 screenHeight / 2, 20, GRAY);
        DrawText("PRESS [ENTER] TO PLAY AGAIN",
                 screenWidth / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2,
                 screenHeight / 2 + 40, 20, GRAY);
    }

    EndDrawing();
}

//------------------------------------------------------------------------------------
void UnloadGame(void)
{
    UnloadBackground();
}

//------------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
