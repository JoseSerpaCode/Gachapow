#ifndef HUD_H
#define HUD_H

// inicializa recursos usados por el HUD (fuente, gráficos, etc)
void InitHUD(void);

// dibuja UI overlay independiente del gameplay (score, tiempo, vidas)
// *** NO modifica estado del juego ***
void DrawHUD(int score, float elapsedTime, int lives);

// libera recursos reservados por HUD
void UnloadHUD(void);

#endif
