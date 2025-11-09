#include "extern/raylib.h"
#include "hw_api.h"     // ✅ Nueva API de hardware agnóstico
#include "game.h"
#include "hud.h"

// -----------------------------------------------------------------------------
// Entry point
// -----------------------------------------------------------------------------
int main(void)
{
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
