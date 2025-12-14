#include "font_manager.h"
#include <stddef.h>

static Font globalFont;

// carga fuente global usada por HUD y mensajes
void FontManager_Init(const char *fontPath, int fontSize) 
{
    globalFont = LoadFontEx(fontPath, fontSize, NULL, 0);
}

// devuelve referencia a la font del juego
Font GetGlobalFont(void) 
{
    return globalFont;
}

// libera recursos
void FontManager_Unload(void) 
{
    UnloadFont(globalFont);
}
