#include "raylib.h"
#include "common.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "shoot.h"
#include "hud.h"
#include "hw_api.h"
#include "background.h"
#include <stdio.h>

//------------------------------------------------------------------------------------
// Estado del gameplay
//------------------------------------------------------------------------------------
static bool gameOver       = false;
static bool pause          = false;
static bool victory        = false;

static int score           = 0;
static int enemiesKill     = 0;
static int shootRate       = 0;
static int activeEnemies   = 0;

static float alpha         = 0.0f;
static float elapsedTime   = 0.0f;
static bool smooth         = false;

static Player player;
static Enemy enemy[NUM_MAX_ENEMIES];
static Shoot shoot[NUM_SHOOTS];
static EnemyWave wave;

static bool rewardDispensed = false;
static prize_status_t prizeState = PRIZE_IDLE;

//------------------------------------------------------------------------------------
static void DestroyAllEnemies(Enemy enemies[], int maxEnemies)
{
    for (int i = 0; i < maxEnemies; i++)
        enemies[i].active = false;
}

//------------------------------------------------------------------------------------
void InitGame(void)
{
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

    rewardDispensed = false;
    prizeState = PRIZE_IDLE;
}

//------------------------------------------------------------------------------------
void UpdateGame(void)
{
    UpdateBackground();

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

    if (IsKeyPressed('P'))
        pause = !pause;
    if (pause) return;

    float dt = GetFrameTime();
    elapsedTime += dt;

    // límite de tiempo
    if (elapsedTime >= 180.0f)
    {
        gameOver = true;
        return;
    }

    UpdatePlayer(&player);
    UpdateEnemies(enemy, activeEnemies, wave);
    UpdateShoots(shoot, enemy, activeEnemies, wave, &enemiesKill, &score, &shootRate, player.rec);

    // collision jugador vs enemigo
    for (int i = 0; i < activeEnemies; i++)
    {
        if (enemy[i].active && CheckCollisionRecs(player.collisionRec, enemy[i].collisionRec))
        {
            if (player.invulnerableTime <= 0)
            {
                player.lives--;
                player.invulnerableTime = 2.0f;

                if (player.lives <= 0)
                {
                    gameOver = true;
                    return;
                }
            }
            enemy[i].active = false;
        }
    }

    // oleadas
    switch (wave)
    {
        case FIRST:
        case SECOND:
        case THIRD:
        {
            if (!smooth) { alpha += 0.02f; if (alpha >= 1.0f) smooth = true; }
            else alpha -= 0.02f;
        }
        break;
    }

    if (wave == FIRST && enemiesKill >= activeEnemies)
    {
        enemiesKill = 0; wave = SECOND; activeEnemies = SECOND_WAVE;
        smooth = false; alpha = 0;
        DestroyAllEnemies(enemy, NUM_MAX_ENEMIES);
        InitEnemies(enemy, activeEnemies, wave);
    }
    else if (wave == SECOND && enemiesKill >= activeEnemies)
    {
        enemiesKill = 0; wave = THIRD; activeEnemies = THIRD_WAVE;
        smooth = false; alpha = 0;
        DestroyAllEnemies(enemy, NUM_MAX_ENEMIES);
        InitEnemies(enemy, activeEnemies, wave);
    }
    else if (wave == THIRD && enemiesKill >= activeEnemies)
    {
        victory = true;
    }
}

//------------------------------------------------------------------------------------
void DrawGame(void)
{
    const GameConfig *cfg = hw_get_config();

    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawBackground();

    const int sw = cfg->screen.width;
    const int sh = cfg->screen.height;

    if (!gameOver)
    {
        DrawPlayer(&player);
        DrawEnemies(enemy, activeEnemies, true);

        for (int i = 0; i < NUM_SHOOTS; i++)
            if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);

        // wave text
        const char *txt = (wave == FIRST) ? "FIRST WAVE" :
                          (wave == SECOND)? "SECOND WAVE" : "THIRD WAVE";

        DrawText(txt, sw/2 - MeasureText(txt,40)/2, sh/2 - 40, 40, Fade(BLACK, alpha));

        DrawHUD(score, elapsedTime, player.lives);

        if (victory)
        {
            if (!rewardDispensed)
            {
                prizeState = hw_dispense_request();
                if (prizeState == PRIZE_DONE) rewardDispensed = true;
            }

            DrawText("YOU WIN!", sw/2 - MeasureText("YOU WIN!",40)/2, sh/2 - 40, 40, BLACK);

            const char *status = (!rewardDispensed)? "DISPENSING REWARD..." : "REWARD DISPENSED!";
            DrawText(status, sw/2 - MeasureText(status,20)/2, sh/2 + 20, 20, DARKGRAY);
        }
        else if (pause)
        {
            DrawText("GAME PAUSED", sw/2 - MeasureText("GAME PAUSED",40)/2, sh/2 - 40, 40, GRAY);
        }
    }
    else
    {
        int finalScore = score + (int)(elapsedTime * 5.0f);
        DrawText("GAME OVER", sw/2 - MeasureText("GAME OVER",40)/2, sh/2 - 60, 40, BLACK);

        char buff[64];
        sprintf(buff,"FINAL SCORE: %i",finalScore);
        DrawText(buff, sw/2 - MeasureText(buff,20)/2, sh/2, 20, GRAY);

        DrawText("PRESS [ENTER] TO PLAY AGAIN",
                 sw/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN",20)/2,
                 sh/2 + 40, 20, GRAY);
    }

    EndDrawing();
}

//------------------------------------------------------------------------------------
void UnloadGame(void)
{
    UnloadTexture(player.texture);
    for (int i = 0; i < NUM_MAX_ENEMIES; i++) UnloadTexture(enemy[i].texture);
    UnloadBackground();
}

//------------------------------------------------------------------------------------
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
