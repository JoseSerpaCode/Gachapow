#include "extern/raylib.h"
#include "hw_api.h"
#include "state_manager.h"

#include <direct.h>
#include <stdio.h>
#include "assets.h"

int main(void)
{
    // Comprobamos si el archivo de config existe
    FILE *file = fopen("assets/data/config.json", "r");
    if (!file) {
        // No existe: estamos probablemente en /build, así que subimos un nivel
        _chdir("..");
    } else {
        fclose(file);
    }

    hw_init();

    StateManager_Init(STATE_INTRO);

    while (!WindowShouldClose())
        StateManager_UpdateDraw();

    StateManager_Unload();

    CloseAudioDevice();
    CloseWindow();
}
