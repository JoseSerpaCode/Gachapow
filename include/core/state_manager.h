#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

#include <raylib.h>

// ================================
// TIPOS DE FUNCIÓN DE LOS ESTADOS
// ================================
typedef void (*StateInitFunc)(void);
typedef void (*StateUpdateFunc)(void);
typedef void (*StateDrawFunc)(void);
typedef void (*StateUnloadFunc)(void);

// ================================
// ENUM DE ESTADOS DEL JUEGO
// ================================
typedef enum {
    STATE_INTRO = 0,
    STATE_MENU,
    STATE_GAMEPLAY,
    // STATE_GAMEOVER
} GameState;

// ================================
// API DEL STATE MANAGER
// ================================
void StateManager_Init(GameState startState);
void StateManager_UpdateDraw(void);
void StateManager_Change(GameState newState);
void StateManager_Unload(void);

#endif
