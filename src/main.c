#include "config.h"
#include "font_manager.h"
#include <stdio.h>

int main(void) {
    GameConfig config;
    if (!LoadConfig(CONFIG_PATH, &config)) {
        printf("Error al cargar configuración.\n");
        return -1;
    }

    InitWindow(config.screen.width, config.screen.height, config.title);
    FontManager_Init(config.font.fontPath, config.font.fontSize);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTextEx(GetGlobalFont(), "Hola Mundo", (Vector2){100,100}, 24, 2, RAYWHITE);

        EndDrawing();
    }

    FontManager_Unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
