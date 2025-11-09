#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "raylib.h"

// Inicializa la fuente global
void FontManager_Init(const char *fontPath, int fontSize);

// Obtiene la fuente global
Font GetGlobalFont(void);

// Libera recursos al cerrar
void FontManager_Unload(void);

#endif
