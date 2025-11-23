// src/game.c
#include "extern/raylib.h"
#include "common.h"
#include "gameplay_system.h"
#include "player.h"
#include "enemy.h"
#include "shoot.h"
#include "hud.h"
#include "hw_api.h"
#include "background.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 540
#define SCREEN_HEIGHT 700

/* Constants */
static const float MAX_GAME_TIME = 180.0f;
static const float INVULNERABLE_SEC = 2.0f;
static const float WAVE_FADE_SPEED = 0.02f; /* unidades de alpha por frame (puedes multiplicar por dt si quieres) */

/* Game-level aggregated state -- evita globals dispersos */
typedef struct
{
    bool gameOver;
    bool pause;
    bool victory;

    int score;
    int enemiesKill;
    int shootRate;
    int activeEnemies;

    float alpha;
    float elapsedTime;
    bool smooth;

    Player player;
    Enemy enemies[NUM_MAX_ENEMIES];
    Shoot shoots[NUM_SHOOTS];
    EnemyWave wave;

    bool rewardDispensed;
    prize_status_t prizeState;
} GameState;

/* Un único state interno (static para visibilidad limitada) */
static GameState GS;

/* Helpers */
static void reset_enemies(GameState *g)
{
    for (int i = 0; i < NUM_MAX_ENEMIES; ++i)
        g->enemies[i].active = false;
}

static void start_wave(GameState *g, EnemyWave newWave, int newActiveCount)
{
    g->enemiesKill = 0;
    g->wave = newWave;
    g->activeEnemies = newActiveCount;
    g->smooth = false;
    g->alpha = 0.0f;
    reset_enemies(g);
    InitEnemies(g->enemies, g->activeEnemies, g->wave);
}

/* Init / Unload */
void InitGame(void)
{
    Assets_Init();

    memset(&GS, 0, sizeof(GS));
    GS.gameOver = false;
    GS.pause = false;
    GS.victory = false;
    GS.score = 0;
    GS.enemiesKill = 0;
    GS.smooth = false;
    GS.alpha = 0.0f;
    GS.elapsedTime = 0.0f;
    GS.shootRate = 0;
    GS.wave = FIRST;
    GS.activeEnemies = FIRST_WAVE;
    GS.rewardDispensed = false;
    GS.prizeState = PRIZE_IDLE;

    InitPlayer(&GS.player);
    InitEnemies(GS.enemies, GS.activeEnemies, GS.wave);
    InitShoots(GS.shoots);
    InitBackground();
    InitHUD();
}

/* Update input-only (separable para tests) */
static void process_input(GameState *g) {
    if (IsKeyPressed(KEY_F1)) g->victory = true;
    if (IsKeyPressed('P')) g->pause = !g->pause;

    if ((g->gameOver || g->victory) && IsKeyPressed(KEY_ENTER)) {
        hw_reset_dispense();
        g->rewardDispensed = false;
        g->prizeState = PRIZE_IDLE;
        InitGame();
    }
}


/* Manage wave fade animation (centraliza lógica para no repetir) */
static void update_wave_fade(GameState *g, float dt)
{
    (void)dt; /* si quieres usar velocidad dependiente de tiempo, multiplícalo aquí */
    if (!g->smooth)
    {
        g->alpha += WAVE_FADE_SPEED;
        if (g->alpha >= 1.0f)
        {
            g->alpha = 1.0f;
            g->smooth = true;
        }
    }
    else
    {
        g->alpha -= WAVE_FADE_SPEED;
        if (g->alpha <= 0.0f)
        {
            g->alpha = 0.0f;
            /* Podrías decidir si smooth vuelve a false o no; así se hace un único ciclo */
        }
    }
}

/* Reemplaza la función UpdateGame actual por ésta */
void UpdateGame(void)
{
    /* Actualizamos el fondo en cada frame (esto mueve bgScroll1/bgScroll2) */
    UpdateBackground();

    process_input(&GS); /* tu función de entrada separada (si no la tienes, mantén la lógica de teclas aquí) */

    if (GS.gameOver || GS.victory)
    {
        /* Si se terminó, no actualizar más lógica de juego */
        return;
    }

    if (GS.pause)
        return;

    float dt = GetFrameTime();
    GS.elapsedTime += dt;

    /* Tiempo límite */
    if (GS.elapsedTime >= MAX_GAME_TIME)
    {
        GS.gameOver = true;
        return;
    }

    /* Lógica de entidades */
    UpdatePlayer(&GS.player);
    UpdateEnemies(GS.enemies, GS.activeEnemies, GS.wave);
    UpdateShoots(GS.shoots, GS.enemies, GS.activeEnemies, GS.wave,
                 &GS.enemiesKill, &GS.score, &GS.shootRate, GS.player.rec);

    /* Colisiones jugador vs enemigos */
    for (int i = 0; i < GS.activeEnemies; ++i)
    {
        if (GS.enemies[i].active && CheckCollisionRecs(GS.player.collisionRec, GS.enemies[i].collisionRec))
        {
            if (GS.player.invulnerableTime <= 0.0f)
            {
                GS.player.lives--;
                GS.player.invulnerableTime = INVULNERABLE_SEC;
                if (GS.player.lives <= 0)
                {
                    GS.gameOver = true;
                    return;
                }
            }
            GS.enemies[i].active = false;
        }
    }

    /* Control de oleadas */
    switch (GS.wave)
    {
    case FIRST:
        update_wave_fade(&GS, dt);
        if (GS.enemiesKill >= GS.activeEnemies)
        {
            start_wave(&GS, SECOND, SECOND_WAVE);
        }
        break;
    case SECOND:
        update_wave_fade(&GS, dt);
        if (GS.enemiesKill >= GS.activeEnemies)
        {
            start_wave(&GS, THIRD, THIRD_WAVE);
        }
        break;
    case THIRD:
        update_wave_fade(&GS, dt);
        if (GS.enemiesKill >= GS.activeEnemies)
        {
            GS.victory = true;
        }
        break;
    default:
        break;
    }
}

/* Draw */
void DrawGame(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawBackground();

    if (!GS.gameOver)
    {
        DrawPlayer(&GS.player);
        DrawEnemies(GS.enemies, GS.activeEnemies, true);

        for (int i = 0; i < NUM_SHOOTS; ++i)
            if (GS.shoots[i].active)
                DrawRectangleRec(GS.shoots[i].rec, GS.shoots[i].color);

        /* Texto de waves (usa alpha actual) */
        const char *wave_text = (GS.wave == FIRST) ? "FIRST WAVE" : (GS.wave == SECOND) ? "SECOND WAVE"
                                                                                        : "THIRD WAVE";
        DrawText(wave_text, SCREEN_WIDTH / 2 - MeasureText(wave_text, 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, Fade(BLACK, GS.alpha));

        /* Victory / Pause overlays dibujados aquí (si aplica) */
        if (GS.victory)
        {
            if (!GS.rewardDispensed)
            {
                GS.prizeState = hw_dispense_request();
                if (GS.prizeState == PRIZE_DONE)
                    GS.rewardDispensed = true;
            }

            DrawText("YOU WIN!", SCREEN_WIDTH / 2 - MeasureText("YOU WIN!", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, BLACK);

            if (!GS.rewardDispensed)
                DrawText("DISPENSING REWARD...", SCREEN_WIDTH / 2 - MeasureText("DISPENSING REWARD...", 20) / 2, SCREEN_HEIGHT / 2 + 20, 20, GRAY);
            else
                DrawText("REWARD DISPENSED!", SCREEN_WIDTH / 2 - MeasureText("REWARD DISPENSED!", 20) / 2, SCREEN_HEIGHT / 2 + 20, 20, DARKGREEN);
        }
        else if (GS.pause)
        {
            DrawText("GAME PAUSED", SCREEN_WIDTH / 2 - MeasureText("GAME PAUSED", 40) / 2, SCREEN_HEIGHT / 2 - 40, 40, GRAY);
        }
    }
    else
    {
        int finalScore = GS.score + (int)(GS.elapsedTime * 5.0f);
        char buf[64];
        snprintf(buf, sizeof(buf), "FINAL SCORE: %i", finalScore);
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - MeasureText("GAME OVER", 40) / 2, SCREEN_HEIGHT / 2 - 60, 40, BLACK);
        DrawText(buf, SCREEN_WIDTH / 2 - MeasureText(buf, 20) / 2, SCREEN_HEIGHT / 2, 20, GRAY);
        DrawText("PRESS [ENTER] TO PLAY AGAIN", SCREEN_WIDTH / 2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20) / 2, SCREEN_HEIGHT / 2 + 40, 20, GRAY);
    }

    /* --- HUD dibujado al final para que quede encima de todo --- */
    DrawHUD(GS.score, GS.elapsedTime, GS.player.lives);

    EndDrawing();
}

/* Unload */
void UnloadGame(void)
{
    UnloadBackground();
    Assets_Unload(); /* si existe */
}

/* Single-step for game loop */
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}
