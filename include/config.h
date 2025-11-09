// Archivo de configuración global del juego (ruta relativa a la raíz del ejecutable)

#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

typedef struct {
    int width;
    int height;
    bool fullscreen;
    int fps;
} ScreenConfig;

typedef struct {
    float master_volume;
} AudioConfig;

typedef struct {
    char fontPath[256];
    int fontSize;
} FontConfig;

typedef struct {
    ScreenConfig screen;
    AudioConfig audio;
    FontConfig font;
    char title[128];
} GameConfig;

#define CONFIG_PATH "assets/data/config.json"
bool LoadConfig(const char *filename, GameConfig *config);

#endif
