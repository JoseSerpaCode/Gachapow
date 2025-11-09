#ifndef SHOOT_H
#define SHOOT_H

#include "common.h"

// inicializa pool de disparos
void InitShoots(Shoot shoot[]);

// update + colisión contra enemigos
// enemiesKill / score se pasan por ref para modificar valores globales
void UpdateShoots(
    Shoot shoot[],
    Enemy enemy[],
    int activeEnemies,
    EnemyWave wave,
    int *enemiesKill,
    int *score,
    int *shootRate,
    Rectangle playerRec
);

#endif
