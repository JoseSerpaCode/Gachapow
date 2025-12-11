#include "raylib.h"
#include "states/gameover.h"

void GameOver_Enter(void) {
    // Inicializa elementos de game over
}

void GameOver_Update(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        // Luego: StateManager_Change(STATE_MENU);
    }
}

void GameOver_Draw(void) {
    ClearBackground(MAROON);
    DrawText("GAME OVER", 180, 300, 30, WHITE);
    DrawText("Presiona ENTER para volver al menú", 80, 360, 20, GRAY);
}

void GameOver_Exit(void) {
    // Limpieza (si aplica)
}
