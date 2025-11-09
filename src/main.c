#include "config.h"
#include "font_manager.h"
#include <stdio.h>

int main(void)
{
    hw_init();
    Game_Init();

    while (!WindowShouldClose())
        Game_UpdateDraw();

    Game_Shutdown();
    return 0;
}
