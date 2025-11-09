#include "config.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// lee config JSON y carga parámetros globales (resolución, audio, fuente, título)
bool LoadConfig(const char *filename, GameConfig *config) 
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("⚠️ No se pudo abrir config: %s\n", filename);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *data = malloc(size + 1);
    if (!data) { fclose(file); return false; }

    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    if (!json) {
        printf("⚠️ Error JSON config: %s\n", cJSON_GetErrorPtr());
        free(data);
        return false;
    }

    // --- pantalla ---
    cJSON *screen = cJSON_GetObjectItem(json, "screen");
    if (screen) {
        config->screen.width      = cJSON_GetObjectItem(screen, "width")       ? cJSON_GetObjectItem(screen, "width")->valueint : 800;
        config->screen.height     = cJSON_GetObjectItem(screen, "height")      ? cJSON_GetObjectItem(screen, "height")->valueint : 600;
        config->screen.fullscreen = cJSON_GetObjectItem(screen, "fullscreen")  ? cJSON_GetObjectItem(screen, "fullscreen")->valueint : false;
        config->screen.fps        = cJSON_GetObjectItem(screen, "fps")         ? cJSON_GetObjectItem(screen, "fps")->valueint : 60;
    }

    // --- audio ---
    cJSON *audio = cJSON_GetObjectItem(json, "audio");
    config->audio.master_volume = (audio && cJSON_GetObjectItem(audio, "master_volume")) ?
                                  (float)cJSON_GetObjectItem(audio, "master_volume")->valuedouble : 1.0f;

    // --- fuente ---
    cJSON *font = cJSON_GetObjectItem(json, "font");
    if (font) {
        cJSON *path = cJSON_GetObjectItem(font, "path");
        cJSON *size = cJSON_GetObjectItem(font, "size");

        strcpy(config->font.fontPath, path ? path->valuestring : "assets/fonts/default.ttf");
        config->font.fontSize = size ? size->valueint : 20;
    }

    // --- título ventana ---
    cJSON *title = cJSON_GetObjectItem(json, "title");
    strcpy(config->title, title ? title->valuestring : "Untitled Game");

    cJSON_Delete(json);
    free(data);

    printf("✅ Config cargurada desde: %s\n", filename);
    return true;
}
