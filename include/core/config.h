#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// configuración de video (resolución, fullscreen, FPS target etc)
typedef struct {
    int width;
    int height;
    bool fullscreen;
    int fps;
} ScreenConfig;

// configuración de audio lógica global
typedef struct {
    float master_volume;
} AudioConfig;

// configuración tipográfica global
typedef struct {
    char fontPath[256];
    int fontSize;
} FontConfig;

// config global entera del juego
typedef struct {
    ScreenConfig screen;
    AudioConfig audio;
    FontConfig font;
    char title[128];
} GameConfig;

// archivo literal relativo al ejecutable final
#define CONFIG_PATH "assets/data/config.json"

// parsea config.json → struct GameConfig
bool LoadConfig(const char *filename, GameConfig *config);

#endif
