#include "Music and Sounds.h"

//Efectos de sonido
Sound sfxJumpSmall;
Sound sfxJumpSuper;
Sound sfxPowerUpTaken;
Sound sfxFireBall;
Sound sfxFireBallWall;
Sound sfxKick;
Sound sfxPowerUpAppear;
Sound sfxStomp;
Sound sfxCoin_Block;
Sound sfxPipe;
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
    sfxPowerUpTaken = LoadSound("Resources/Audio/FX/smb_mushroom.wav");
    sfxFireBall = LoadSound("Resources/Audio/FX/smb_fireball.wav");
    sfxFireBallWall = LoadSound("Resources/Audio/FX/smb_bump.wav");
    sfxKick = LoadSound("Resources/Audio/FX/smb_kick.wav");
    sfxPowerUpAppear = LoadSound("Resources/Audio/FX/smb_powerup_appears.wav");
    sfxStomp = LoadSound("Resources/Audio/FX/smb_stomp.wav");
    sfxCoin_Block = LoadSound("Resources/Audio/FX/smb_coin.wav");
    sfxPipe = LoadSound("Resources/Audio/FX/smb_pipe.wav");
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


