#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

typedef enum {
    MUSIC_MENU,
    MUSIC_GAME,
    MUSIC_BOSS,
} MusicID;

typedef enum {
    SFX_JUMP,
    SFX_HIT,
    SFX_COIN,
} SFXID;

void AudioManager_Init();
void AudioManager_Update();

void AudioManager_PlayMusic(MusicID id);
void AudioManager_StopMusic();
void AudioManager_PauseMusic();
void AudioManager_ResumeMusic();

void AudioManager_PlaySFX(SFXID id);

#endif
