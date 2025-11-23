#include "raylib.h"
#include "hw_api.h"
#include "config.h"
#include "font_manager.h"
#include "assets.h"
#include <stdio.h>

// config cargada desde JSON (persistente durante todo el runtime)
extern GameConfig config;

static double startTime = 0;

// simulación de dispensador para PC proto
static bool dispensing = false;
static double dispenseStart = 0;


// inicializa subsistema de HW virtual PC
// NOTA: esta función NO decide cerrar el juego
// si falla config, main decidirá qué hacer
void hw_init(void)
{
    if (!LoadConfig(CONFIG_PATH, &config))
        printf("⚠️ Config falló, se usarán valores default internos.\n");

    // ------------------------------------------------------------
    // 📌 1. Forzar relación de aspecto 16:9 y fullscreen real
    // ------------------------------------------------------------
    int display = GetCurrentMonitor();
    int monitorW = GetMonitorWidth(display);
    int monitorH = GetMonitorHeight(display);

    float aspect = 16.0f / 9.0f;

    int targetW = monitorW;
    int targetH = (int)(monitorW / aspect);

    // si la altura excede, usamos altura y recalculamos ancho
    if (targetH > monitorH) {
        targetH = monitorH;
        targetW = (int)(monitorH * aspect);
    }

    config.screen.width = targetW;
    config.screen.height = targetH;
    config.screen.fullscreen = true;

    // ------------------------------------------------------------
    // 📌 2. Flags ANTES de InitWindow()
    // ------------------------------------------------------------
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    // ------------------------------------------------------------
    // 📌 3. Crear ventana en fullscreen escalada a 16:9
    // ------------------------------------------------------------
    InitWindow(config.screen.width, config.screen.height, config.title);

    // ------------------------------------------------------------
    // 📌 4. Audio, fuentes, assets
    // ------------------------------------------------------------
    InitAudioDevice();
    SetMasterVolume(config.audio.master_volume);
    SetTargetFPS(config.screen.fps);

    FontManager_Init(config.font.fontPath, config.font.fontSize);
    Assets_Init();

    startTime = GetTime();
}


// exposición de config global para gameplay / states
const GameConfig *hw_get_config(void)
{
    return &config;
}

// tiempo relativo desde arranque del juego en ms
uint32_t hw_get_time_ms(void)
{
    return (uint32_t)((GetTime() - startTime) * 500.0);
}

// teclado abstracto
bool hw_button_pressed(int id)
{
    switch (id) {
        case 0: return IsKeyPressed(KEY_SPACE);
        case 1: return IsKeyPressed(KEY_ENTER);
        case 2: return IsKeyPressed('P');
        default: return false;
    }
}

// comportamiento fake de la máquina física real
prize_status_t hw_dispense_request(void)
{
    if (!dispensing) {
        dispensing = true;
        dispenseStart = GetTime();
        return PRIZE_DISPENSING;
    }

    if ((GetTime() - dispenseStart) > 2.0) {
        dispensing = false;
        return PRIZE_DONE;
    }

    return PRIZE_DISPENSING;
}

void hw_reset_dispense(void)
{
    dispensing = false;
    dispenseStart = 0.0;
}
