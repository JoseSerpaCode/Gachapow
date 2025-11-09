#ifndef ENEMY_H
#define ENEMY_H

#include "common.h"

// inicializa enemigos según wave actual
void InitEnemies(Enemy enemy[], int count, EnemyWave wave);

// actualiza IA / movimiento / colisiones
void UpdateEnemies(Enemy enemy[], int activeEnemies, EnemyWave wave);

// dibuja enemigos en pantalla (debug activa bounding boxes)
void DrawEnemies(Enemy enemy[], int activeEnemies, bool debug);

// libera texturas si enemy las usa propias (no assets globales)
void UnloadEnemies(Enemy enemy[], int count);

#endif
