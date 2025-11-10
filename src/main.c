#include "extern/raylib.h"
#include "hw_api.h"
#include "game.h"
#include "hud.h"
#include <direct.h>
#include <stdio.h>

// -----------------------------------------------------------------------------
// Entry point
// -----------------------------------------------------------------------------
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

    // 🔧 Inicialización del hardware (Raylib, ventana, audio, FPS, etc.)
    hw_init();

    // 🔧 Inicialización del juego y HUD
    InitGame();
    InitHUD();

    // 🔁 Bucle principal del juego
    while (!WindowShouldClose())   // Puedes mantenerlo así, o crear hw_window_should_close()
    {
        // ⚙️ Actualiza y dibuja todo (esta función ya llama UpdateGame + DrawGame)
        UpdateDrawFrame();
    }

    // 🧹 Limpieza
    UnloadHUD();
    UnloadGame();

    Assets_Unload();
    CloseAudioDevice();  // Se cierra en hw_init(), pero puedes mantenerlo aquí
    CloseWindow();

    return 0;
}
