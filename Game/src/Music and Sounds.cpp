#include "Music and Sounds.h"

//Efectos de sonido
Sound sfxJumpSmall;
Sound sfxJumpSuper;
Sound sfxMushroom;
Sound sfxGameOver;
Sound sfxFlagpole;
Sound sfxDeath;
Sound sfxCompleted;

//Musica
Music musicOverworld;
Music musicOverworld_hurry;
Music musicInvencible;
Music musicInvencible_hurry;

void LoadGameSounds() {
    sfxJumpSmall = LoadSound("Resources/Audio/FX/smb_jump-small.wav");
    sfxJumpSuper = LoadSound("Resources/Audio/FX/smb_jump-super.wav");
    sfxMushroom = LoadSound("Resources/Audio/FX/smb_mushroom.wav");
    sfxGameOver = LoadSound("Resources/Audio/FX/smb_gameover.wav");
    sfxFlagpole = LoadSound("Resources/Audio/FX/smb_flagpole.wav");
    sfxDeath = LoadSound("Resources/Audio/Music/Muerte.ogg");
    sfxCompleted = LoadSound("Resources/Audio/FX/smb_stage_clear.wav");
}

void LoadGameMusic() {
    musicOverworld = LoadMusicStream("Resources/Audio/Music/Cancion.ogg");
    musicOverworld_hurry = LoadMusicStream("Resources/Audio/Music/CancionHurry.ogg");
    musicInvencible = LoadMusicStream("Resources/Audio/Music/Invencible.ogg");
    musicInvencible_hurry = LoadMusicStream("Resources/Audio/Music/InvencibleHurry.ogg");

    SetMusicVolume(musicOverworld, 0.5f);
    SetMusicVolume(musicInvencible, 0.5f);
    SetMusicVolume(musicOverworld_hurry, 0.5f);
}