#include "player.h"

void InitPlayer(Player *p)
{
    p->texture = LoadTexture("assets/Nave.png");

    // --- Posición y tamaño de dibujo ---
    p->rec.width = p->texture.width;
    p->rec.height = p->texture.height;
    p->rec.x = 420 / 2 - p->rec.width / 2;
    p->rec.y = 640 - p->rec.height - 16;

    // --- Área de colisión (más pequeña) ---
    p->collisionRec.width = p->rec.width * 0.6f;   // 60% del ancho
    p->collisionRec.height = p->rec.height * 0.6f; // 60% del alto
    p->collisionRec.x = p->rec.x + (p->rec.width - p->collisionRec.width) / 2;
    p->collisionRec.y = p->rec.y + (p->rec.height - p->collisionRec.height) / 2;

    p->speed.x = 5;
    p->speed.y = 0;

    // --- Nuevos ---
    p->lives = 3;
    p->invulnerableTime = 0;
    p->active = true;
}


void UpdatePlayer(Player *p)
{
    // Si está invulnerable, reducimos el tiempo
    if (p->invulnerableTime > 0)
        p->invulnerableTime -= GetFrameTime();

    // Movimiento
    if (IsKeyDown(KEY_RIGHT)) p->rec.x += p->speed.x;
    if (IsKeyDown(KEY_LEFT)) p->rec.x -= p->speed.x;

    // Límites
    if (p->rec.x <= 0) p->rec.x = 0;
    if (p->rec.x + p->rec.width >= 420) p->rec.x = 420 - p->rec.width;
    
    // Actualizar colisión
    p->collisionRec.x = p->rec.x + (p->rec.width - p->collisionRec.width) / 2;
    p->collisionRec.y = p->rec.y + (p->rec.height - p->collisionRec.height) / 2;
}



void DrawPlayer(Player *p)
{
    Color tint = (p->invulnerableTime > 0 && ((int)(p->invulnerableTime * 10) % 2 == 0))
                 ? Fade(WHITE, 0.3f) // parpadeo
                 : WHITE;

    Rectangle src = { 0, 0, (float)p->texture.width, (float)p->texture.height };
    Rectangle dest = p->rec;
    Vector2 origin = { 0, 0 };

    DrawTexturePro(p->texture, src, dest, origin, 0.0f, tint);

    // Borde de colisión opcional
    DrawRectangleLines(p->collisionRec.x, p->collisionRec.y,
                       p->collisionRec.width, p->collisionRec.height, RED);
}

