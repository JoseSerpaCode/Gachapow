#include "raylib.h"
#include "hw_api.h"
#include "config.h"
#include "font_manager.h"
#include "assets.h"
#include <stdio.h>

// Config cargada desde JSON
extern GameConfig config;

static double startTime = 0;

// Simulación del dispensador físico
static bool dispensing = false;
static double dispenseStart = 0;

// ========================================================
// Inicialización de HW PC (pantalla, audio, fuentes, etc.)
// ========================================================
void hw_init(void)
{
    if (!LoadConfig(CONFIG_PATH, &config))
        printf("⚠️ Config falló, se usarán valores default.\n");

    int display = GetCurrentMonitor();
    int monitorW = GetMonitorWidth(display);
    int monitorH = GetMonitorHeight(display);
    float aspect = 16.0f / 9.0f;

    int targetW = monitorW;
    int targetH = (int)(monitorW / aspect);

    if (targetH > monitorH) {
        targetH = monitorH;
        targetW = (int)(monitorH * aspect);
    }

    config.screen.width = targetW;
    config.screen.height = targetH;
    config.screen.fullscreen = true;

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(config.screen.width, config.screen.height, config.title);

    InitAudioDevice();
    SetMasterVolume(config.audio.master_volume);
    SetTargetFPS(config.screen.fps);

    FontManager_Init(config.font.fontPath, config.font.fontSize);
    Assets_Init();

    startTime = GetTime();
}

// ========================================================
// Config
// ========================================================
const GameConfig *hw_get_config(void)
{
    return &config;
}

// ========================================================
// Tiempo en ms desde init()
// ========================================================
uint32_t hw_get_time_ms(void)
{
    return (uint32_t)((GetTime() - startTime) * 1000.0);
}

// ========================================================
// Botones abstractos
// ========================================================
bool hw_button_pressed(int id)
{
    switch (id) {
        case 0: return IsKeyPressed(KEY_SPACE);  // botón 1
        case 1: return IsKeyPressed(KEY_ENTER);  // botón 2
        case 2: return IsKeyPressed('P');        // pausa, servicio, etc
        default: return false;
    }
}

// ========================================================
// COIN MECH SIMULADO
// tecla "C" = moneda insertada
// ========================================================
bool hw_coin_inserted(void)
{
    return IsKeyPressed(KEY_C);
}

// ========================================================
// Dispensador simulado (2s)
// ========================================================
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

bool hw_any_button_pressed(void)
{
    for (int i = 0; i < 3; i++)
        if (hw_button_pressed(i))
            return true;

    if (IsGamepadAvailable(0))
        for (int b = 0; b < 16; b++)
            if (IsGamepadButtonPressed(0, b))
                return true;

    return false;
}
