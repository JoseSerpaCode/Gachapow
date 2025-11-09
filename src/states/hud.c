#include "extern/raylib.h"
#include "hud.h"
#include "assets.h"  // <- importante

static Texture2D hud;

void InitHUD(void) {
    hud = GetTextureAsset(TEX_HUD);
}

void DrawHUD(int score, float elapsedTime, int lives) {
    (void)score;
    (void)elapsedTime;
    (void)lives;
    DrawTexture(hud, 0, 0, WHITE);
}


void UnloadHUD(void) {
    // No hace falta liberar nada, ya lo maneja Assets_Unload()
}
