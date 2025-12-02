#ifndef INTRO_STATE_H
#define INTRO_STATE_H

// ============================================================================
// Estado: INTRO
// Encargado de reproducir el video inicial, el logo con animación de fade y
// finalmente el texto legal antes de pasar al estado MENÚ.
// ============================================================================

void Intro_Init(void);      // Cargar recursos y preparar el estado
void Intro_Update(void);    // Lógica del intro (video, logo, texto)
void Intro_Draw(void);      // Render del contenido del intro
void Intro_Unload(void);    // Liberar recursos usados por el intro

#endif // INTRO_STATE_H
