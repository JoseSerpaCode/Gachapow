#include "raylib.h"
#include "hw_api.h"
#include "config.h"
#include "font_manager.h"
#include "assets.h"

static GameConfig config;
static double startTime = 0;
static bool dispensing = false;
static double dispenseStart = 0;

void hw_init(void)
{
    // Load config JSON
    if (!LoadConfig(CONFIG_PATH, &config)) {
        printf("Error al cargar configuración.\n");
        return -1;
    }

    InitWindow(config.screen.width, config.screen.height, config.title);
    InitAudioDevice();
    SetMasterVolume(config.audio.master_volume);
    SetTargetFPS(config.screen.fps);

    FontManager_Init(config.font.fontPath, config.font.fontSize);
    Assets_Init();

    startTime = GetTime();
}

const GameConfig *hw_get_config(void)
{
    return &config;
}

uint32_t hw_get_time_ms(void)
{
    return (uint32_t)((GetTime() - startTime) * 1000);
}

bool hw_button_pressed(int id)
{
    switch (id) {
        case 0: return IsKeyPressed(KEY_SPACE);
        case 1: return IsKeyPressed(KEY_ENTER);
        case 2: return IsKeyPressed('P');
        default: return false;
    }
}

prize_status_t hw_dispense_request(void)
{
    if (!dispensing) {
        dispensing = true;
        dispenseStart = GetTime();
        return PRIZE_DISPENSING;
    }

    if (dispensing && (GetTime() - dispenseStart) > 2.0) {
        dispensing = false;
        return PRIZE_DONE;
    }

    return PRIZE_DISPENSING;
}

void hw_reset_dispense(void)
{
    dispensing = false;
    dispenseStart = 0;
}
