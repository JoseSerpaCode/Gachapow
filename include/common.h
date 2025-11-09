#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <stdbool.h>

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

// wave actual (balas y AI pueden cambiar por esto)
typedef enum { FIRST = 0, SECOND, THIRD } EnemyWave;


// --------------------
// ENTIDADES (ECS-like)
// --------------------
// Nota: estos structs NO tienen lógica. Solo datos.
// La lógica vive en player.c / enemy.c / shoot.c / gameplay.c
// Esto permite swap de rendering / physics / balancing sin romper todo.

typedef struct Player {
    Rectangle rec;           // rect para render (dibujo en pantalla)
    Rectangle collisionRec;  // rect para colisiones (puede ser distinto al visual)
    Vector2 speed;
    Color color;             // color fallback si no hay texture asignada
    Texture2D texture;       // asset del player (opcional, configurable)
    bool active;             // si puede actualizarse / renderizarse
    int lives;               // vidas restantes
    float invulnerableTime;  // segundos de invulnerabilidad post-hit
} Player;


typedef struct Enemy {
    Rectangle rec;           // rect para dibujo
    Rectangle collisionRec;  // rect para colisiones
    Vector2 speed;
    bool active;
    Color color;             // color fallback
    Texture2D texture;       // texture asignada a este enemy (puede variar según tipo)
} Enemy;


typedef struct Shoot {
    Rectangle rec;           // rect para dibujo y colisión (típicamente coincide)
    Vector2 speed;
    bool active;
    Color color;             // color / sprite básico. Si luego son assets → se cambia.
} Shoot;

#endif
