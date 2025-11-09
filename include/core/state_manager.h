#ifndef STATE_MANAGER_H
#define STATE_MANAGER_H

typedef enum {
    STATE_MENU,
    STATE_GAMEPLAY,
    STATE_GAMEOVER
} GameState;

void StateManager_Init(void);
void StateManager_Update(void);
void StateManager_Draw(void);
void StateManager_Change(GameState newState);

#endif
