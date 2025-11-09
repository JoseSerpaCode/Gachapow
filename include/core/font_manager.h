#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "raylib.h"

// inicializa fuente global única
void FontManager_Init(const char *fontPath, int fontSize);

// retorna la fuente global
Font GetGlobalFont(void);

// libera la fuente cargada
void FontManager_Unload(void);

#endif
