#include "font_manager.h"
#include <stddef.h>

static Font globalFont; // fuente global estática

void FontManager_Init(const char *fontPath, int fontSize) {
    globalFont = LoadFontEx(fontPath, fontSize, NULL, 0);
}

Font GetGlobalFont(void) {
    return globalFont;
}

void FontManager_Unload(void) {
    UnloadFont(globalFont);
}
