#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"

// inicializa datos base del jugador
void InitPlayer(Player *p);

// actualiza input / movimiento / invulnerabilidad etc
void UpdatePlayer(Player *p);

// dibuja player en pantalla
void DrawPlayer(Player *p);

#endif
