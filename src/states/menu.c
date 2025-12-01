#include "raylib.h"
#include "menu.h"

void Menu_Enter(void) {
    // Inicialización del menú
}

void Menu_Update(void) {
    // Ejemplo: si presionas Enter, cambia al gameplay
    if (IsKeyPressed(KEY_ENTER)) {
        // Aquí luego se hará: StateManager_Change(STATE_GAMEPLAY);
    }
}

void Menu_Draw(void) {
    BeginDrawing();
    ClearBackground(DARKBLUE);
    DrawText("MENÚ PRINCIPAL", 140, 300, 30, WHITE);
    DrawText("Presiona ENTER para jugar", 120, 360, 20, GRAY);
    EndDrawing();
}

void Menu_Exit(void) {
    // Limpieza del menú (si hace falta)
}
