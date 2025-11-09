#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

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

// inicializa datos base del jugador
void InitPlayer(Player *p);

// actualiza input / movimiento / invulnerabilidad etc
void UpdatePlayer(Player *p);

// dibuja player en pantalla
void DrawPlayer(Player *p);

// libera recursos del jugador
void UnloadPlayer(Player *p);

#endif
