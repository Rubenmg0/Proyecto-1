#ifndef TEXTURES_AND_FONTS_H
#define TEXTURES_AND_FONTS_H

#include "raylib.h"

// Bloques
extern Texture2D bloque_int;
extern Texture2D bloque_int_a;
extern Texture2D ladrillo;
extern Texture2D escalera;
extern Texture2D ladrillo_cueva;
extern Texture2D suelo_cueva;
extern Texture2D suelo_m;

extern Texture2D roto1;
extern Texture2D roto2;
extern Texture2D roto3;
extern Texture2D roto4;

// Interactive Structures
extern Texture2D flagTexture;
extern Texture2D tuberia;
extern Texture2D meta;
extern Texture2D banderin;

// Map
extern Texture2D castle;
extern Texture2D fondo;
extern Texture2D azul;
extern Texture2D negro;
extern Texture2D puntos;


// Pipes
extern Texture2D tuberia_s;
extern Texture2D tuberia_m;
extern Texture2D tuberia_b;
extern Texture2D tubo;
extern Texture2D tuberia_cueva;

// Objects
extern Texture2D money;

// Other Textures
extern Texture2D logoTexture;
extern Texture2D UI;
extern Texture2D Level1;
extern Texture2D icon_lifes;
extern Texture2D icon_money;
extern Texture2D coin_ui;


// Enemigos
extern Texture2D Goomba;
extern Texture2D Goomba_chafado;
extern Texture2D Goomba_invertido;
extern Texture2D Koopa;
extern Texture2D Shell;
extern Texture2D goomba_sprite1;
extern Texture2D goomba_sprite2;
extern Texture2D goomba_sprite3;
extern Texture2D goomba_sprite4;
extern Texture2D goomba_sprite5;
extern Texture2D goomba_sprite6;
extern Texture2D goomba_sprite7;
extern Texture2D goomba_sprite8;
extern Texture2D goomba_sprite9;
extern Texture2D goomba_sprite10;
extern Texture2D goomba_sprite11;
extern Texture2D goomba_sprite12;
extern Texture2D goomba_sprite13;
extern Texture2D goomba_sprite14;
extern Texture2D goomba_sprite15;
extern Texture2D goomba_sprite16;


// Mario
extern Texture2D Mario_Right;
extern Texture2D Mario_Left;
extern Texture2D Mario_Fire_Right;
extern Texture2D Mario_Fire_Left;
extern Texture2D mario_sprite;

// Items
extern Texture2D Mooshroom;
extern Texture2D FireFlower;
extern Texture2D Star;
extern Texture2D FireBall;
extern Texture2D FireBallTick1;
extern Texture2D FireBallTick2;

extern Font marioFont;

// Carga
void LoadGameTextures();
void LoadGameTypography();

#endif 