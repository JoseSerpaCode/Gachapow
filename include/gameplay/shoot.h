#ifndef SHOOT_H
#define SHOOT_H

#include "raylib.h"
#include "enemy.h"
#include "common.h"

void InitShoots(Shoot shoot[]);
void UpdateShoots(Shoot shoot[], Enemy enemy[], int activeEnemies, EnemyWave wave,
                  int *enemiesKill, int *score, int *shootRate, Rectangle playerRec);

#endif
