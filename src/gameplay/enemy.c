#include "enemy.h"
#include "assets.h"
#include "raylib.h" // GetScreenWidth/GetScreenHeight, GetRandomValue

// Inicializa `count` enemigos en el array proporcionado.
// - No carga texturas: toma la referencia desde Assets (TEX_ENEMY).
// - Ajusta posición, colisión y velocidad según la oleada.
void InitEnemies(Enemy enemy[], int count, EnemyWave wave)
{
    // Desactivar solo los elementos que vamos a manejar
    for (int i = 0; i < count; i++) {
        enemy[i].active = false;
    }

    // Referencia única a la textura (asset manager la carga)
    Texture2D enemyTexture = GetTextureAsset(TEX_ENEMY);

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    for (int i = 0; i < count; i++)
    {
        enemy[i].texture = enemyTexture;

        // Tamaño fijo o derivado de la textura si está disponible
        enemy[i].rec.width  = (enemyTexture.width  > 0) ? enemyTexture.width  : 32;
        enemy[i].rec.height = (enemyTexture.height > 0) ? enemyTexture.height : 24;

        enemy[i].rec.x = GetRandomValue(0, screenW - (int)enemy[i].rec.width);
        enemy[i].rec.y = GetRandomValue(-1000, -40);
        enemy[i].active = true;
        enemy[i].color = GRAY;

        // Colisión ligeramente más pequeña que el sprite visual
        enemy[i].collisionRec.x      = enemy[i].rec.x + 4;
        enemy[i].collisionRec.y      = enemy[i].rec.y + 3;
        enemy[i].collisionRec.width  = enemy[i].rec.width  - 8;
        enemy[i].collisionRec.height = enemy[i].rec.height - 6;

        // Velocidades verticales según la oleada
        if (wave == FIRST)  enemy[i].speed.y = (float)GetRandomValue(2, 4);
        else if (wave == SECOND) enemy[i].speed.y = (float)GetRandomValue(4, 7);
        else enemy[i].speed.y = (float)GetRandomValue(7, 10);

        // Velocidad horizontal ocasional
        if (GetRandomValue(0, 100) < 40)
            enemy[i].speed.x = (float)GetRandomValue(-3, 3);
        else
            enemy[i].speed.x = 0.0f;
    }
}

// Actualiza posición, re-spawn cuando sale de pantalla y actualiza collisionRec
void UpdateEnemies(Enemy enemy[], int activeEnemies, EnemyWave wave)
{
    (void)wave; // por si la IA se complica luego

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    for (int i = 0; i < activeEnemies; i++)
    {
        if (!enemy[i].active) continue;

        enemy[i].rec.y += enemy[i].speed.y;
        enemy[i].rec.x += enemy[i].speed.x;

        // Rebote horizontal dentro de los límites
        if (enemy[i].rec.x <= 0 || enemy[i].rec.x + enemy[i].rec.width >= screenW)
            enemy[i].speed.x *= -1;

        // Si se salió por abajo, re-spawneamos por arriba con nueva X
        if (enemy[i].rec.y > screenH)
        {
            enemy[i].rec.x = GetRandomValue(0, screenW - (int)enemy[i].rec.width);
            enemy[i].rec.y = GetRandomValue(-200, -40);
        }

        // actualizar posición del rect de colisión
        enemy[i].collisionRec.x = enemy[i].rec.x + 4;
        enemy[i].collisionRec.y = enemy[i].rec.y + 3;
    }
}

// Dibuja enemigos; si debug=true dibuja bounding box de colisión.
void DrawEnemies(Enemy enemy[], int activeEnemies, bool debug)
{
    for (int i = 0; i < activeEnemies; i++)
    {
        if (!enemy[i].active) continue;

        // DrawTexturePro permite escalado/control si lo necesitas.
        Rectangle src = { 0, 0, (float)enemy[i].texture.width, (float)enemy[i].texture.height };
        Rectangle dst = enemy[i].rec;
        Vector2 origin = { 0, 0 };
        DrawTexturePro(enemy[i].texture, src, dst, origin, 0.0f, WHITE);

        if (debug)
        {
            DrawRectangleLines((int)enemy[i].collisionRec.x,
                               (int)enemy[i].collisionRec.y,
                               (int)enemy[i].collisionRec.width,
                               (int)enemy[i].collisionRec.height, RED);
        }
    }
}

// No hacemos UnloadTexture aquí: Assets_Unload() se encarga de liberar todo.
// Esta función queda como placeholder si en el futuro enemigos tuvieran assets locales.
void UnloadEnemies(Enemy enemy[], int count)
{
    (void)enemy;
    (void)count;
    // NO UnloadTexture: assets son gestionados por Assets_Unload()
}
