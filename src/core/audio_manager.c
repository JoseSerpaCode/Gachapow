#include "raylib.h"
#include "audio_manager.h"

static Music musicTracks[3];
static Sound sfxTracks[3];

static Music *currentMusic = NULL;

void AudioManager_Init() {
    InitAudioDevice();

    // Cargar musicas
    musicTracks[MUSIC_MENU] = LoadMusicStream("assets/game_music.ogg");
    //musicTracks[MUSIC_GAME] = LoadMusicStream("assets/game.ogg");
    //musicTracks[MUSIC_BOSS] = LoadMusicStream("assets/boss.ogg");

    /* Cargar sfx
    sfxTracks[SFX_JUMP] = LoadSound("assets/jump.wav");
    sfxTracks[SFX_HIT]  = LoadSound("assets/hit.wav");
    sfxTracks[SFX_COIN] = LoadSound("assets/coin.wav");
    */
}

void AudioManager_Update() {
    if (currentMusic)
        UpdateMusicStream(*currentMusic);
}

void AudioManager_PlayMusic(MusicID id) {
    if (currentMusic != NULL)
        StopMusicStream(*currentMusic);

    currentMusic = &musicTracks[id];
    PlayMusicStream(*currentMusic);
}

void AudioManager_StopMusic() {
    if (currentMusic)
        StopMusicStream(*currentMusic);
}

void AudioManager_PauseMusic() {
    if (currentMusic)
        PauseMusicStream(*currentMusic);
}

void AudioManager_ResumeMusic() {
    if (currentMusic)
        ResumeMusicStream(*currentMusic);
}

void AudioManager_PlaySFX(SFXID id) {
    PlaySound(sfxTracks[id]);
}
