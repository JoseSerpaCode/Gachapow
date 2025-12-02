#include "state_manager.h"

#include "states/intro.h"
#include "states/menu.h"
#include "states/gameplay.h"
#include "states/gameover.h"

static GameState current;
static GameState next;

// Punteros a funciones del estado activo
static void (*InitFunc)(void);
static void (*UpdateFunc)(void);
static void (*DrawFunc)(void);
static void (*UnloadFunc)(void);

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
    // Cambió el estado → descarga el actual y carga el nuevo
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
