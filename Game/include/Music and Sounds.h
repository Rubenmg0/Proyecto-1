#ifndef MUSIC_AND_SOUNDS_H
#define MUSIC_AND_SOUNDS_H

#include "raylib.h"

//Efectos de sonido
extern Sound sfxJumpSmall;
extern Sound sfxJumpSuper;
extern Sound sfxPowerUpTaken;
extern Sound sfxFireBall;
extern Sound sfxKick;
extern Sound sfxPowerUpAppear;
extern Sound sfxStomp;
extern Sound sfxCoin_Block;
extern Sound sfxPipe;
extern Sound sfxGameOver;
extern Sound sfxFlagpole;
extern Sound sfxDeath;
extern Sound sfxCompleted;

//Music
extern Music musicOverworld;
extern Music musicOverworld_hurry;
extern Music musicInvencible;
extern Music musicInvencible_hurry;

void LoadGameSounds();
void LoadGameMusic();

#endif