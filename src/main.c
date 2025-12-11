#include "extern/raylib.h"
#include "hw_api.h"
#include "state_manager.h"
#include "assets.h"

#include <direct.h>
#include <stdio.h>

int main(void)
{
    // Verificación de ruta (tu código)
    FILE *file = fopen("assets/data/config.json", "r");
    if (!file) _chdir("..");
    else fclose(file);

    hw_init();
    StateManager_Init(STATE_INTRO);

    int sw = GetScreenWidth();
    int sh = GetScreenHeight();

    // === nuevo: render texture donde se dibuja TODO el juego ===
    RenderTexture2D screen = LoadRenderTexture(sw, sh);

    // === nuevo: cargar shader CRT ===
    Shader crt = LoadShader(0, "assets/shaders/crt.fs");

    // uniforms del shader
    int locResolution = GetShaderLocation(crt, "resolution");
    int locTime       = GetShaderLocation(crt, "time");

    Vector2 resolution = { (float)sw, (float)sh };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // --- 1) DIBUJAR EL JUEGO A LA TEXTURA ---
        BeginTextureMode(screen);      // redirige el dibujo al framebuffer
            ClearBackground(BLACK);    // fondo
            StateManager_UpdateDraw(); // tus estados dibujan aquí
        EndTextureMode();

        // --- 2) MOSTRAR LA TEXTURA EN PANTALLA CON EL SHADER CRT ---
        BeginDrawing();
            ClearBackground(BLACK);

            // Actualizar uniformes
            SetShaderValue(crt, locResolution, &resolution, SHADER_UNIFORM_VEC2);
            float t = GetTime();
            SetShaderValue(crt, locTime, &t, SHADER_UNIFORM_FLOAT);

            BeginShaderMode(crt);
                // IMPORTANTE: invertir Y (Raylib lo requiere)
                DrawTextureRec(
                    screen.texture,
                    (Rectangle){0, 0, (float)screen.texture.width, (float)-screen.texture.height},
                    (Vector2){0, 0},
                    WHITE
                );
            EndShaderMode();

        EndDrawing();
    }

    // Liberar
    UnloadRenderTexture(screen);
    UnloadShader(crt);
    StateManager_Unload();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
