#include "shoot.h"

void InitShoots(Shoot shoot[])
{
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.width = 4;
        shoot[i].rec.height = 8;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = 8;
        shoot[i].active = false;
        shoot[i].color = MAROON;
    }
}

void UpdateShoots(Shoot shoot[], Enemy enemy[], int activeEnemies, EnemyWave wave, int *enemiesKill, int *score, int *shootRate, Rectangle playerRec)
{
    (*shootRate)++;

    // Disparar automáticamente cada cierto tiempo
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
        if (shoot[i].active)
        {
            shoot[i].rec.y -= shoot[i].speed.y;

            for (int j = 0; j < activeEnemies; j++)
            {
                if (enemy[j].active && CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                {
                    shoot[i].active = false;
                    enemy[j].rec.x = GetRandomValue(0, 420 - enemy[j].rec.width);
                    enemy[j].rec.y = GetRandomValue(-200, -40);

                    if (wave == FIRST) enemy[j].speed.y = GetRandomValue(2, 4);
                    else if (wave == SECOND) enemy[j].speed.y = GetRandomValue(4, 7);
                    else enemy[j].speed.y = GetRandomValue(7, 10);

                    if (GetRandomValue(0, 100) < 40)
                        enemy[j].speed.x = GetRandomValue(-3, 3);
                    else
                        enemy[j].speed.x = 0;

                    (*shootRate) = 0;
                    (*enemiesKill)++;
                    (*score) += 100;
                }
            }

            if (shoot[i].rec.y + shoot[i].rec.height < 0)
            {
                shoot[i].active = false;
                (*shootRate) = 0;
            }
        }
    }
}
