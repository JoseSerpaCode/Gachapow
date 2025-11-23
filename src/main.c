#include "extern/raylib.h"
#include "hw_api.h"
#include "state_manager.h"

int main(void)
{
    hw_init();

    StateManager_Init(STATE_INTRO);

    while (!WindowShouldClose())
        StateManager_UpdateDraw();

    StateManager_Unload();

    CloseAudioDevice();
    CloseWindow();
}
