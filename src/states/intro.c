#include "states/intro.h"
#include "core/state_manager.h"
#include "raylib.h"
#include "assets.h"
#include "config.h"
#include "hw_api.h"
#include <stdio.h>
#include <math.h>

// ============================================================================
// CONSTANTES
// ============================================================================
#define MAX_FRAMES       2000
#define FRAME_INTERVAL   0.15f

#define FADE_DURATION    1.5f
#define HOLD_DURATION    2.5f

// ============================================================================
// ENUMERACIONES Y ESTRUCTURAS
// ============================================================================
typedef enum {
    INTRO_VIDEO = 0,
    INTRO_LOGO,
    INTRO_LEGAL,
    INTRO_END
} IntroStep;

// ============================================================================
// VARIABLES
// ============================================================================
extern GameConfig config;

static Texture2D frames[MAX_FRAMES];
static int totalFrames = 0;

static Texture2D logoGachapow;

static IntroStep step = INTRO_VIDEO;
static float timer = 0;
static int currentFrame = 0;

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
// FUNCIONES AUXILIARES
// ============================================================================

// Devuelve un alpha (0–1) según timer para fade-in, hold y fade-out
static float GetFadeValue(float t)
{
    if      (t < FADE_DURATION)                   return t / FADE_DURATION;
    else if (t < FADE_DURATION + HOLD_DURATION)   return 1.0f;
    else                                          return 1.0f - ((t - (FADE_DURATION + HOLD_DURATION)) / FADE_DURATION);
}

// Centrado automático de un Texture2D escalado a pantalla
static Rectangle GetCenteredScale(Texture2D tex, int screenW, int screenH)
{
    float scale = fminf((float)screenW / tex.width, (float)screenH / tex.height);

    return (Rectangle){
        (screenW - tex.width * scale) * 0.5f,
        (screenH - tex.height * scale) * 0.5f,
        tex.width * scale,
        tex.height * scale
    };
}

// Mide texto multilinea para centrarlo
static int MeasureMultilineText(const char *text, int fontSize)
{
    int maxWidth = 0, width = 0;
    const int charWidth = MeasureText("A", fontSize);

    for (int i = 0; text[i] != '\0'; i++)
    {
        if (text[i] == '\n') { maxWidth = (width > maxWidth ? width : maxWidth); width = 0; }
        else                 { width += charWidth; }
    }

    if (width > maxWidth) maxWidth = width;
    return maxWidth;
}

// ============================================================================
// INIT
// ============================================================================
void Intro_Init()
{
    // Cargar frames del video
    for (int i = 0; i < MAX_FRAMES; i++)
    {
        char path[64];
        snprintf(path, sizeof(path), "assets/animations/arcadeOpening/pixil-frame-%d.png", i);

        Texture2D t = LoadTexture(path);
        if (t.id == 0) break;

        frames[totalFrames++] = t;
    }

    // Logo
    logoGachapow = GetTextureAsset(TEX_LOGO_GACHAPOW);
    SetTextureFilter(logoGachapow, TEXTURE_FILTER_POINT);

    // Reset
    step = INTRO_VIDEO;
    timer = 0;
    currentFrame = 0;

    // Pantalla negra inicial
    double start = GetTime();
    while (GetTime() - start < 1.5)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }
}

// ============================================================================
// UPDATE
// ============================================================================
void Intro_Update()
{
    float dt = GetFrameTime();
    timer += dt;

    // ----------------------------------------------------------------
    // SKIP INTRO: si se presiona cualquier botón, salta al menú
    // ----------------------------------------------------------------
    if (hw_any_button_pressed() || IsKeyPressed(KEY_ENTER))
    {
        StateManager_Change(STATE_MENU);
        return;
    }
    // ----------------------------------------------------------------

    switch (step)
    {
        case INTRO_VIDEO:
            if (timer >= FRAME_INTERVAL)
            {
                timer -= FRAME_INTERVAL;
                currentFrame++;

                if (currentFrame >= totalFrames)
                {
                    step = INTRO_LOGO;
                    timer = 0;
                }
            }
            break;

        case INTRO_LOGO:
        case INTRO_LEGAL:
        {
            float full = FADE_DURATION + HOLD_DURATION + FADE_DURATION;
            if (timer >= full)
            {
                step++;
                timer = 0;
            }
        }
        break;

        case INTRO_END:
            StateManager_Change(STATE_MENU);
            break;
    }
}

// ============================================================================
// DRAW
// ============================================================================
void Intro_Draw()
{
    ClearBackground(BLACK);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    switch (step)
    {
        case INTRO_VIDEO:
        {
            Texture2D *tex = &frames[currentFrame];
            Rectangle dst = GetCenteredScale(*tex, sw, sh);

            DrawTexturePro(*tex,
                           (Rectangle){0,0,tex->width,tex->height},
                           dst, (Vector2){0,0}, 0, WHITE);
        }
        break;

        case INTRO_LOGO:
        {
            float alpha = GetFadeValue(timer);
            Color c = {255, 255, 255, (unsigned char)(alpha * 255)};

            Rectangle dst = GetCenteredScale(logoGachapow, sw, sh);

            DrawTexturePro(logoGachapow,
                           (Rectangle){0,0,logoGachapow.width,logoGachapow.height},
                           dst, (Vector2){0,0}, 0, c);
        }
        break;

        case INTRO_LEGAL:
        {
            float alpha = GetFadeValue(timer);
            Color c = {255,255,255,(unsigned char)(alpha * 255)};

            int fontSize = 20, spacing = 5;
            int textW = MeasureMultilineText(legalText, fontSize);

            int x = (sw - textW) / 2;
            int y = sh * 0.15f;

            const char *p = legalText;
            char line[512];
            int idx = 0;

            while (*p)
            {
                if (*p == '\n')
                {
                    line[idx] = '\0';
                    DrawText(line, x, y, fontSize, c);
                    y += fontSize + spacing;
                    idx = 0;
                }
                else line[idx++] = *p;

                p++;
            }

            line[idx] = '\0';
            DrawText(line, x, y, fontSize, c);
        }
        break;

        default:
            break;
    }
}

// ============================================================================
// UNLOAD
// ============================================================================
void Intro_Unload()
{
    for (int i = 0; i < totalFrames; i++)
        UnloadTexture(frames[i]);

    UnloadTexture(logoGachapow);
}
