#include "config.h"
#include "cJSON.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool LoadConfig(const char *filename, GameConfig *config) {
    FILE *file = fopen(filename, "r");
    if (!file) return false;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    rewind(file);

    char *data = malloc(size + 1);
    fread(data, 1, size, file);
    data[size] = '\0';
    fclose(file);

    cJSON *json = cJSON_Parse(data);
    if (!json) {
        free(data);
        return false;
    }

    // Pantalla
    cJSON *screen = cJSON_GetObjectItem(json, "screen");
    config->screen.width = cJSON_GetObjectItem(screen, "width")->valueint;
    config->screen.height = cJSON_GetObjectItem(screen, "height")->valueint;
    config->screen.fullscreen = cJSON_GetObjectItem(screen, "fullscreen")->valueint;
    config->screen.fps = cJSON_GetObjectItem(screen, "fps")->valueint;

    // Audio
    cJSON *audio = cJSON_GetObjectItem(json, "audio");
    config->audio.master_volume = (float)cJSON_GetObjectItem(audio, "master_volume")->valuedouble;

    // Fuente
    cJSON *font = cJSON_GetObjectItem(json, "font");
    strcpy(config->font.fontPath, cJSON_GetObjectItem(font, "path")->valuestring);
    config->font.fontSize = cJSON_GetObjectItem(font, "size")->valueint;

    // Título
    strcpy(config->title, cJSON_GetObjectItem(json, "title")->valuestring);

    cJSON_Delete(json);
    free(data);
    return true;
}
