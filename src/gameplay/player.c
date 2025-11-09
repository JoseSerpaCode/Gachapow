#include "player.h"
#include "assets.h"
#include "raylib.h"

void InitPlayer(Player *p)
{
    p->texture = GetTextureAsset(TEX_PLAYER);

    p->rec.width  = (p->texture.width  > 0) ? p->texture.width  : 32;
    p->rec.height = (p->texture.height > 0) ? p->texture.height : 32;
    p->rec.x = GetScreenWidth() / 2 - p->rec.width / 2;
    p->rec.y = GetScreenHeight() - p->rec.height - 16;

    p->collisionRec.width  = p->rec.width * 0.6f;
    p->collisionRec.height = p->rec.height * 0.6f;
    p->collisionRec.x = p->rec.x + (p->rec.width - p->collisionRec.width) / 2;
    p->collisionRec.y = p->rec.y + (p->rec.height - p->collisionRec.height) / 2;

    p->speed.x = 5;
    p->speed.y = 0;

    p->lives = 3;
    p->invulnerableTime = 0.0f;
    p->active = true;
}

void UpdatePlayer(Player *p)
{
    if (p->invulnerableTime > 0.0f)
        p->invulnerableTime -= GetFrameTime();

    if (IsKeyDown(KEY_RIGHT)) p->rec.x += p->speed.x;
    if (IsKeyDown(KEY_LEFT))  p->rec.x -= p->speed.x;

    if (p->rec.x <= 0) p->rec.x = 0;
    if (p->rec.x + p->rec.width >= GetScreenWidth())
        p->rec.x = GetScreenWidth() - p->rec.width;

    p->collisionRec.x = p->rec.x + (p->rec.width - p->collisionRec.width) / 2;
    p->collisionRec.y = p->rec.y + (p->rec.height - p->collisionRec.height) / 2;
}

void DrawPlayer(Player *p)
{
    Color tint = WHITE;
    if (p->invulnerableTime > 0.0f)
    {
        if (((int)(p->invulnerableTime * 10.0f) % 2) == 0)
            tint = Fade(WHITE, 0.3f);
    }

    Rectangle src = { 0, 0, (float)p->texture.width, (float)p->texture.height };
    Rectangle dst = p->rec;
    Vector2 origin = { 0, 0 };
    DrawTexturePro(p->texture, src, dst, origin, 0.0f, tint);
}

void UnloadPlayer(Player *p) {
    UnloadTexture(p->texture);
}
