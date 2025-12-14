#include "shoot.h"
#include "raylib.h"
#include "common.h"

// Función interna para crear un disparo
static void SpawnShoot(Shoot shoot[], Rectangle playerRec)
{
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        if (!shoot[i].active)
        {
            shoot[i].rec.x = playerRec.x + playerRec.width / 2 - shoot[i].rec.width / 2;
            shoot[i].rec.y = playerRec.y - shoot[i].rec.height - 2;
            shoot[i].active = true;
            break;
        }
    }
}

void InitShoots(Shoot shoot[])
{
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.width  = 4;
        shoot[i].rec.height = 8;
        shoot[i].speed = (Vector2){0, 8};
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

void UpdateShoots(
    Shoot shoot[], 
    Enemy enemy[], 
    int activeEnemies,
    EnemyWave wave, 
    int *enemiesKill, 
    int *score,
    int *shootRate,
    Rectangle playerRec)
{
    (*shootRate)++;

    // -------------------------
    //   DISPARO MANUAL (solo SPACE)
    // -------------------------
    if (IsKeyPressed(KEY_SPACE))
    {
        if (*shootRate >= 5)     // cooldown
        {
            SpawnShoot(shoot, playerRec);
            *shootRate = 0;
        }
    }

    // -------------------------
    //   MOVIMIENTO + COLISIONES
    // -------------------------
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        if (!shoot[i].active) continue;

        shoot[i].rec.y -= shoot[i].speed.y;

        for (int j = 0; j < activeEnemies; j++)
        {
            if (enemy[j].active && CheckCollisionRecs(shoot[i].rec, enemy[j].collisionRec))
            {
                shoot[i].active = false;

                // Respawn del enemigo
                enemy[j].rec.x = GetRandomValue(0, GetScreenWidth() - enemy[j].rec.width);
                enemy[j].rec.y = GetRandomValue(-200, -40);

                if (wave == FIRST)      enemy[j].speed.y = GetRandomValue(2, 4);
                else if (wave == SECOND) enemy[j].speed.y = GetRandomValue(4, 7);
                else                    enemy[j].speed.y = GetRandomValue(7, 10);

                enemy[j].speed.x = (GetRandomValue(0,100) < 40)
                                    ? GetRandomValue(-3,3)
                                    : 0.0f;

                (*enemiesKill)++;
                (*score) += 100;
                break;
            }
        }

        // Se fue de pantalla
        if (shoot[i].active && shoot[i].rec.y + shoot[i].rec.height < 0)
            shoot[i].active = false;
    }
}
