#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

typedef enum {
    STATE_INTRO,
    STATE_GAMEPLAY
} GameState;


void StateManager_Init(GameState startState);
void StateManager_UpdateDraw(void);
void StateManager_Change(GameState newState);
void StateManager_Unload(void);

#endif
