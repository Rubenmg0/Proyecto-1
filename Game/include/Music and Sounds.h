#ifndef MUSIC_AND_SOUNDS_H
#define MUSIC_AND_SOUNDS_H

#include "raylib.h"

//Efectos de sonido
extern Sound sfxJumpSmall;
extern Sound sfxJumpSuper;
extern Sound sfxMushroom;
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