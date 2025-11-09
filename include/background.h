#ifndef BACKGROUND_H
#define BACKGROUND_H

//
// Este background es SOLO el background del gameplay
// (intro y menu tendrán sus propios backgrounds / videos / imágenes)
//

// Init solo obtiene referencias a assets ya cargados en Assets_Init()
// y resetea scroll interno
void InitBackground(void);

// update del scroll (parallax)
void UpdateBackground(void);

// draw del background del gameplay
void DrawBackground(void);

// NO descarga texturas acá (son globales del asset manager)
void UnloadBackground(void);

#endif
