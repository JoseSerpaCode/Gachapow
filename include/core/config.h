#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_PATH "config.json"

#include <stdbool.h>

// ----------------------
// Estructuras de Config
// ----------------------

typedef struct {
    int width;
    int height;
    bool fullscreen;
    int fps;
} ScreenConfig;

typedef struct {
    float master_volume;   // volumen general
    float music_volume;    // volumen música
    float sfx_volume;      // volumen efectos
} AudioConfig;

typedef struct {
    char fontPath[256];
    int fontSize;
} FontConfig;

typedef struct {
    bool enabled;          // si se usa intro
    char musicPath[256];   // ruta a música del intro
} IntroConfig;

typedef struct {
    ScreenConfig screen;
    AudioConfig  audio;
    FontConfig   font;
    IntroConfig  intro;

    char title[128];
} GameConfig;

// ----------------------
// Prototipos
// ----------------------

bool LoadConfig(const char *filename, GameConfig *config);

#endif
