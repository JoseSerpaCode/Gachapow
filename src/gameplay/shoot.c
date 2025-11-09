#include "shoot.h"
#include "raylib.h"
#include "common.h"

// Inicializa pool de disparos (pool pattern)
void InitShoots(Shoot shoot[])
{
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.width  = 4;
        shoot[i].rec.height = 8;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 8;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

// Actualiza disparos, mueve, chequea colisiones con collisionRec de enemigos,
// respawnea enemigos y actualiza score / kills / shootRate.
// - enemiesKill, score y shootRate se pasan por referencia para modificar datos globales.
void UpdateShoots(Shoot shoot[], Enemy enemy[], int activeEnemies, EnemyWave wave, int *enemiesKill, int *score, int *shootRate, Rectangle playerRec)
{
    (*shootRate)++;

    // Auto-shoot cada X frames (tuning)
    if ((*shootRate) % 12 == 0)
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

    // Movimiento y colisiones
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        if (!shoot[i].active) continue;

        shoot[i].rec.y -= shoot[i].speed.y;

        // Colisión con enemigos usando collisionRec (más preciso)
        for (int j = 0; j < activeEnemies; j++)
        {
            if (enemy[j].active && CheckCollisionRecs(shoot[i].rec, enemy[j].collisionRec))
            {
                shoot[i].active = false;

                // Respawnear enemigo arriba (manteniendo pool activo)
                enemy[j].rec.x = GetRandomValue(0, GetScreenWidth() - (int)enemy[j].rec.width);
                enemy[j].rec.y = GetRandomValue(-200, -40);

                // Ajustar velocidad según oleada
                if (wave == FIRST)      enemy[j].speed.y = (float)GetRandomValue(2, 4);
                else if (wave == SECOND)enemy[j].speed.y = (float)GetRandomValue(4, 7);
                else                    enemy[j].speed.y = (float)GetRandomValue(7, 10);

                if (GetRandomValue(0, 100) < 40)
                    enemy[j].speed.x = (float)GetRandomValue(-3, 3);
                else
                    enemy[j].speed.x = 0.0f;

                (*shootRate) = 0;
                (*enemiesKill)++;
                (*score) += 100;
                break; // el disparo ya fue procesado, salir del bucle enemies
            }
        }

        // Si el disparo sale de pantalla, lo desactivamos
        if (shoot[i].active && (shoot[i].rec.y + shoot[i].rec.height < 0))
        {
            shoot[i].active = false;
            (*shootRate) = 0;
        }
    }
}
