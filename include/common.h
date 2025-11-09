#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdbool.h>
#include "gameplay/player.h"   // ✅ aquí traemos la definición de Player

// --------------------
// Constantes del gameplay
// --------------------

// máximo de disparos activos en pantalla al mismo tiempo
#define NUM_SHOOTS 10

// total absoluto hard limit de enemigos cargados en RAM
#define NUM_MAX_ENEMIES 22

// cantidad de enemigos por oleada (tuning gameplay)
#define FIRST_WAVE 10
#define SECOND_WAVE 20
#define THIRD_WAVE 22

// tiempos y valores varios de gameplay
#define MAX_PLAY_TIME_SEC            180.0f
#define PLAYER_INVULNERABLE_TIME     2.0f
#define WAVE_ALPHA_SPEED             0.02f
#define TEXT_BIG_SIZE                40
#define TEXT_SMALL_SIZE              20
#define FINAL_SCORE_TIME_MULTIPLIER  5.0f


// --------------------
// INPUT ABSTRACCION (no raylib directo dentro de game)
// --------------------
#define BUTTON_SHOOT   0
#define BUTTON_START   1
#define BUTTON_PAUSE   2


// wave actual (balas y AI pueden cambiar por esto)
typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;


// --------------------
// ENTIDADES (ECS-like)
// --------------------

// Solo los que no existen en gameplay/headers específicos

typedef struct Enemy {
    Rectangle rec;
    Rectangle collisionRec;
    Vector2 speed;
    bool active;
    Color color;
    Texture2D texture;
} Enemy;

typedef struct Shoot {
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
} Shoot;

#endif
