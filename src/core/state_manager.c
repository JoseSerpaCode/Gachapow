#include "state_manager.h"

#include "states/intro.h"
#include "states/menu.h"
#include "states/gameplay.h"
#include "states/gameover.h"

static GameState current;
static GameState next;

// Funciones del estado activo
static StateInitFunc InitFunc;
static StateUpdateFunc UpdateFunc;
static StateDrawFunc DrawFunc;
static StateUnloadFunc UnloadFunc;

static void LoadState(GameState s)
{
    switch (s)
    {
    case STATE_INTRO:
        InitFunc   = Intro_Init;
        UpdateFunc = Intro_Update;
        DrawFunc   = Intro_Draw;
        UnloadFunc = Intro_Unload;
        break;

    case STATE_MENU:
        InitFunc   = Menu_Init;
        UpdateFunc = Menu_Update;
        DrawFunc   = Menu_Draw;
        UnloadFunc = Menu_Unload;
        break;

    case STATE_GAMEPLAY:
        InitFunc   = Gameplay_Init;
        UpdateFunc = Gameplay_Update;
        DrawFunc   = Gameplay_Draw;
        UnloadFunc = Gameplay_Unload;
        break;

    /*
    case STATE_GAMEOVER:
        InitFunc   = GameOver_Init;
        UpdateFunc = GameOver_Update;
        DrawFunc   = GameOver_Draw;
        UnloadFunc = GameOver_Unload;
        break;
    */
    }
}

void StateManager_Init(GameState startState)
{
    current = startState;
    next = startState;
    LoadState(current);
    InitFunc();
}

void StateManager_UpdateDraw(void)
{
    if (next != current)
    {
        UnloadFunc();
        current = next;
        LoadState(current);
        InitFunc();
    }

    UpdateFunc();
    DrawFunc();
}

void StateManager_Change(GameState newState)
{
    next = newState;
}

void StateManager_Unload(void)
{
    UnloadFunc();
}
