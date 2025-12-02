#include "states/intro.h"
#include "core/state_manager.h"
#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include "config.h"

#define MAX_FRAMES 2000           // Máximo de frames que puede tener el video
#define FRAME_INTERVAL 0.20f      // Tiempo entre frames del video (en segundos)

#define FADE_DURATION 2.7f        // Duración de la animación de fade-in y fade-out
#define HOLD_DURATION 3.7f        // Tiempo que se mantiene el logo o texto en pantalla

// ============================================================================
// VARIABLES GLOBALES
// ============================================================================

extern GameConfig config;         // Configuración global del juego

// Array donde se almacenarán los frames del video animado
static Texture2D frames[MAX_FRAMES];
static int totalFrames = 0;       // Total de frames cargados

// Logo principal a mostrar después del video
static Texture2D logo1;

// Estado actual del intro:
// 0 = mostrando video
// 1 = mostrando logo
// 2 = texto legal
// 3 = fin → pasa al gameplay
static int step = 0;

static float timer = 0;           // Temporizador general
static int currentFrame = 0;      // Frame actual del video

// Texto legal que se muestra al final del intro
static const char *legalText =
    "Este juego utiliza el lenguaje C junto a la libreria Raylib.\n"
    "El sistema es desarrollado por el equipo 'GP Environment'.\n"
    "Todos los derechos reservados.\n\n"
    "Los personajes, historias, nombres y eventos representados en este juego\n"
    "son ficticios. Cualquier similitud con personas reales es coincidencia.\n\n"
    "El software está protegido por leyes internacionales de derechos de autor.\n"
    "La reproducción total o parcial sin autorización está prohibida.\n\n"
    "© 2026 Gachapow. Todos los derechos reservados.";


// ============================================================================
// MEDIR TEXTO MULTILÍNEA
// Calcula el ancho máximo de un bloque de texto con saltos de línea.
// Útil para centrar el texto legal.
// ============================================================================
int MeasureMultilineText(const char *text, int fontSize)
{
    int maxWidth = 0;
    int width = 0;

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '\n')
        {
            // Línea terminada → actualizar maxWidth
            if (width > maxWidth)
                maxWidth = width;

            width = 0; // reinicia conteo para la siguiente línea
        }
        else
        {
            // Se mide como si cada carácter fuese del tamaño de "A"
            width += MeasureText("A", fontSize);
        }
    }

    // Última línea si no terminó en \n
    if (width > maxWidth)
        maxWidth = width;

    return maxWidth;
}


// ============================================================================
// INIT
// Inicializa el intro, carga frames del video y el logo.
// ============================================================================
void Intro_Init()
{
    // ----------------------------------------
    // CARGAR FRAMES DEL VIDEO
    // ----------------------------------------
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        char name[64];
        snprintf(name, sizeof(name), "assets/images/sprites/pixil-frame-%d.png", i);

        Texture2D t = LoadTexture(name);

        // Si no existe el archivo → se detiene la carga
        if (t.id == 0)
            break;

        frames[totalFrames++] = t;
    }

    // -------------------
    // CARGAR EL LOGO
    // -------------------
    logo1 = GetTextureAsset(TEX_LOGO_GACHAPOW);

    // Filtro pixelado (mejor para logos estilo retro)
    SetTextureFilter(logo1, TEXTURE_FILTER_POINT);

    // Resetear variables
    step = 0;
    timer = 0;
    currentFrame = 0;

    // ------------------------------
    // Pantalla negra inicial (1.5 s)
    // ------------------------------
    double waitStart = GetTime();
    while (GetTime() - waitStart < 1.5)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
}


// ============================================================================
// UPDATE
// Lógica principal del intro, avanza pasos y controla animaciones.
// ============================================================================
void Intro_Update()
{
    float dt = GetFrameTime();

    switch (step)
    {
    // --------------------------------------------------------------------
    case 0: // Video frame por frame
        // --------------------------------------------------------------------
        timer += dt;

        if (timer >= FRAME_INTERVAL)
        {
            timer -= FRAME_INTERVAL;
            currentFrame++;

            // Si se acabó el video → pasar al logo
            if (currentFrame >= totalFrames)
            {
                step = 1;
                timer = 0;
            }
        }
        break;

    // --------------------------------------------------------------------
    case 1: // Logo
    case 2: // Texto legal
        // --------------------------------------------------------------------
        {
            timer += dt;

            // Tiempo total de fade-in → hold → fade-out
            float totalTime = FADE_DURATION + HOLD_DURATION + FADE_DURATION;

            // Al completar el ciclo → pasar al siguiente step
            if (timer >= totalTime)
            {
                step++;
                timer = 0;
            }
        }
        break;

    // --------------------------------------------------------------------
    case 3: // Finaliza intro → Gameplay
        StateManager_Change(STATE_GAMEPLAY);
        break;
    }
}


// ============================================================================
// DRAW
// Dibuja lo que corresponda al estado actual (video, logo o texto).
// ============================================================================
void Intro_Draw()
{
    BeginDrawing();
    ClearBackground(BLACK);

    int screenW = GetScreenWidth();
    int screenH = GetScreenHeight();

    switch (step)
    {
    // --------------------------------------------------------------------
    case 0: // VIDEO
        // --------------------------------------------------------------------
        {
            Texture2D tex = frames[currentFrame];

            // Mantener proporción al hacer escala
            float scaleX = (float)screenW / tex.width;
            float scaleY = (float)screenH / tex.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            // Rectángulo destino centrado
            Rectangle dst = {
                (screenW - tex.width * scale) * 0.5f,
                (screenH - tex.height * scale) * 0.5f,
                tex.width * scale,
                tex.height * scale};

            DrawTexturePro(tex,
                           (Rectangle){0, 0, tex.width, tex.height},
                           dst,
                           (Vector2){0, 0},
                           0.0f,
                           WHITE);
        }
        break;

    // --------------------------------------------------------------------
    case 1: // LOGO
        // --------------------------------------------------------------------
        {
            Texture2D tex = logo1;

            // Calcular alpha según el tiempo (fade-in/hold/fade-out)
            float alpha;

            if (timer < FADE_DURATION)
                alpha = timer / FADE_DURATION;                   // fade-in
            else if (timer < FADE_DURATION + HOLD_DURATION)
                alpha = 1.0f;                                     // hold
            else
                alpha = 1.0f - ((timer - (FADE_DURATION + HOLD_DURATION)) / FADE_DURATION); // fade-out

            // Escalar el logo manteniendo proporción
            float scaleX = (float)screenW / tex.width;
            float scaleY = (float)screenH / tex.height;
            float scale = (scaleX < scaleY) ? scaleX : scaleY;

            Rectangle dst = {
                (screenW - tex.width * scale) * 0.5f,
                (screenH - tex.height * scale) * 0.5f,
                tex.width * scale,
                tex.height * scale};

            // Aplicar alpha al color
            Color tint = WHITE;
            tint.a = (unsigned char)(alpha * 255);

            DrawTexturePro(tex,
                           (Rectangle){0, 0, tex.width, tex.height},
                           dst,
                           (Vector2){0, 0},
                           0.0f,
                           tint);
        }
        break;

    // --------------------------------------------------------------------
    case 2: // TEXTO LEGAL
        // --------------------------------------------------------------------
        {
            // Calcular alpha igual que el logo
            float alpha;

            if (timer < FADE_DURATION)
                alpha = timer / FADE_DURATION;
            else if (timer < FADE_DURATION + HOLD_DURATION)
                alpha = 1.0f;
            else
                alpha = 1.0f - ((timer - (FADE_DURATION + HOLD_DURATION)) / FADE_DURATION);

            Color tint = WHITE;
            tint.a = (unsigned char)(alpha * 255);

            int fontSize = 20;
            int spacing = 5;

            // Centrado horizontal
            int textW = MeasureMultilineText(legalText, fontSize);
            int posX = (screenW - textW) / 2;
            int posY = screenH * 0.15f;

            // Separar líneas manualmente
            char line[512];
            int idx = 0;
            int y = posY;

            const char *p = legalText;

            while (*p)
            {
                if (*p == '\n')
                {
                    line[idx] = '\0';
                    DrawText(line, posX, y, fontSize, tint);
                    y += fontSize + spacing;
                    idx = 0;
                }
                else
                {
                    line[idx++] = *p;
                }
                p++;
            }

            // Última línea
            line[idx] = '\0';
            DrawText(line, posX, y, fontSize, tint);
        }
        break;
    }

    EndDrawing();
}


// ============================================================================
// UNLOAD
// Libera todos los recursos del intro.
// ============================================================================
void Intro_Unload()
{
    // Liberar todos los frames del video
    for (int i = 0; i < totalFrames; i++)
        UnloadTexture(frames[i]);

    // Liberar el logo
    UnloadTexture(logo1);
}
