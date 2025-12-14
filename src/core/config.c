#include "config.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

GameConfig config;

// Macro segura para copiar strings sin desbordar el buffer
#define SAFE_STRCPY(dest, src) strncpy(dest, src, sizeof(dest)-1)

// ============================================================================
// LoadConfig
// Lee un archivo JSON y llena la estructura GameConfig.
// Si algo falla, se usan valores por defecto.
// ============================================================================
bool LoadConfig(const char *filename, GameConfig *config)
{
    // =========================================================================
    // VALORES POR DEFECTO (fallback si no existe el archivo)
    // =========================================================================
    config->screen.width = 1280;
    config->screen.height = 720;
    config->screen.fullscreen = false;
    config->screen.fps = 60;

    config->audio.master_volume = 1.0f;
    config->audio.music_volume = 1.0f;
    config->audio.sfx_volume = 1.0f;

    config->intro.enabled = true;
    SAFE_STRCPY(config->intro.musicPath, "assets/music/intro.ogg");

    SAFE_STRCPY(config->font.fontPath, "assets/fonts/arcade.ttf");
    config->font.fontSize = 24;

    SAFE_STRCPY(config->title, "Gachapow");

    // =========================================================================
    // INTENTAR ABRIR EL ARCHIVO DE CONFIGURACIÓN
    // =========================================================================
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️ No se encontró config, usando valores por defecto.\n");
        return false; // No es error grave, solo no se cargó nada.
    }

    // Obtener el tamaño del archivo
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    // Reservar memoria para leer el contenido completo
    char *data = malloc(size + 1);
    if (!data) {
        fclose(file);
        return false;
    }

    // Leer archivo completo
    fread(data, 1, size, file);
    data[size] = '\0'; // Null-terminator obligatorio para texto JSON
    fclose(file);

    // =========================================================================
    // PARSEAR JSON
    // =========================================================================
    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("⚠️ Error parseando JSON: %s\n", cJSON_GetErrorPtr());
        free(data);
        return false;
    }

    // =========================================================================
    // LEER SECCIÓN: SCREEN
    // =========================================================================
    cJSON *screen = cJSON_GetObjectItemCaseSensitive(json, "screen");
    if (cJSON_IsObject(screen)) {

        cJSON *w   = cJSON_GetObjectItem(screen, "width");
        cJSON *h   = cJSON_GetObjectItem(screen, "height");
        cJSON *fs  = cJSON_GetObjectItem(screen, "fullscreen");
        cJSON *fps = cJSON_GetObjectItem(screen, "fps");

        if (cJSON_IsNumber(w))   config->screen.width = w->valueint;
        if (cJSON_IsNumber(h))   config->screen.height = h->valueint;
        if (cJSON_IsBool(fs))    config->screen.fullscreen = fs->valueint;
        if (cJSON_IsNumber(fps)) config->screen.fps = fps->valueint;
    }

    // =========================================================================
    // LEER SECCIÓN: AUDIO
    // =========================================================================
    cJSON *audio = cJSON_GetObjectItemCaseSensitive(json, "audio");
    if (cJSON_IsObject(audio)) {

        cJSON *mv  = cJSON_GetObjectItem(audio, "master_volume");
        cJSON *mus = cJSON_GetObjectItem(audio, "music_volume");
        cJSON *sfx = cJSON_GetObjectItem(audio, "sfx_volume");

        if (cJSON_IsNumber(mv))  config->audio.master_volume = mv->valuedouble;
        if (cJSON_IsNumber(mus)) config->audio.music_volume = mus->valuedouble;
        if (cJSON_IsNumber(sfx)) config->audio.sfx_volume = sfx->valuedouble;
    }

    // =========================================================================
    // LEER SECCIÓN: INTRO
    // =========================================================================
    cJSON *intro = cJSON_GetObjectItemCaseSensitive(json, "intro");
    if (cJSON_IsObject(intro)) {

        cJSON *en  = cJSON_GetObjectItem(intro, "enabled");
        cJSON *mus = cJSON_GetObjectItem(intro, "music");

        if (cJSON_IsBool(en))    config->intro.enabled = en->valueint;
        if (cJSON_IsString(mus)) SAFE_STRCPY(config->intro.musicPath, mus->valuestring);
    }

    // =========================================================================
    // LEER SECCIÓN: FONT
    // =========================================================================
    cJSON *font = cJSON_GetObjectItemCaseSensitive(json, "font");
    if (cJSON_IsObject(font)) {

        cJSON *path = cJSON_GetObjectItem(font, "path");
        cJSON *size = cJSON_GetObjectItem(font, "size");

        if (cJSON_IsString(path)) SAFE_STRCPY(config->font.fontPath, path->valuestring);
        if (cJSON_IsNumber(size)) config->font.fontSize = size->valueint;
    }

    // =========================================================================
    // LEER: window_title
    // =========================================================================
    cJSON *title = cJSON_GetObjectItemCaseSensitive(json, "window_title");
    if (cJSON_IsString(title))
        SAFE_STRCPY(config->title, title->valuestring);

    // =========================================================================
    // LIMPIEZA
    // =========================================================================
    cJSON_Delete(json);
    free(data);

    printf("✅ Config cargada desde %s\n", filename);
    return true;
}
