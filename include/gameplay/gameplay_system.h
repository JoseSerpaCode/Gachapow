#ifndef GAME_H
#define GAME_H

// inicializa el estado del gameplay (player, enemigos, shoots, etc)
// se llama cuando se entra a la escena gameplay
void InitGame(void);

// actualiza lógica del gameplay (no dibuja)
// se llama cada frame en la escena gameplay
void UpdateGame(void);

// dibuja gameplay (no HUD, no menú)
void DrawGame(void);

// libera recursos usados solo por gameplay
void UnloadGame(void);

// función comodín de raylib style → Update + Draw
// usada por main cuando estamos en modo gameplay
// evita error humano de orden de llamado
void UpdateDrawFrame(void);

#endif
