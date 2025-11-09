#include "config.h"
#include "font_manager.h"
#include "game.h"
#include "hw_api.h"
#include <stdio.h>

int main(void)
{
    hw_init();
    InitGame();

    while (!WindowShouldClose())
        UpdateDrawFrame();   // 👈 Nombre correcto de la función

    UnloadGame();             // 👈 También corrige aquí

    return 0;
}
