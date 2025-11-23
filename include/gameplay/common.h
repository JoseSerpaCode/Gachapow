#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdbool.h>

#define NUM_SHOOTS 10
#define NUM_MAX_ENEMIES 22
#define FIRST_WAVE 10
#define SECOND_WAVE 20
#define THIRD_WAVE 22

typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;

// definición del jugador
typedef struct Player {
    Texture2D texture;

    Rectangle rec;          // para dibujar
    Rectangle collisionRec; // hitbox más pequeña

    Vector2 speed;

    int lives;
    float invulnerableTime;
    bool active;
} Player;

typedef struct Enemy {
    Rectangle rec;           // área usada para dibujar
    Rectangle collisionRec;  // área usada para colisiones
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
