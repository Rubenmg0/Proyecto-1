#include "Textures and Fonts.h"

// Bloques
Texture2D bloque_int;
Texture2D bloque_int_a;
Texture2D ladrillo;
Texture2D escalera;
Texture2D ladrillo_cueva;
Texture2D suelo_cueva;
Texture2D suelo_m;
Texture2D roto1;
Texture2D roto2;
Texture2D roto3;
Texture2D roto4;

// Interactive Structures
Texture2D flagTexture;
Texture2D tuberia;
Texture2D meta;
Texture2D banderin;

// Map
Texture2D castle;
Texture2D fondo;
Texture2D azul;
Texture2D negro;
Texture2D puntos;

// Pipes
Texture2D tuberia_s;
Texture2D tuberia_m;
Texture2D tuberia_b;
Texture2D tubo;
Texture2D tuberia_cueva;

// Objects
Texture2D money;
Texture2D coin_ui;

// Other Textures
Texture2D logoTexture;
Texture2D UI;
Texture2D Level1;
Texture2D icon_lifes;
Texture2D icon_money;

// Enemigos
Texture2D Goomba;
Texture2D Goomba_chafado;
Texture2D Goomba_invertido;
Texture2D Koopa;
Texture2D Shell;
Texture2D goomba_sprite1;
Texture2D goomba_sprite2;
Texture2D goomba_sprite3;
Texture2D goomba_sprite4;
Texture2D goomba_sprite5;
Texture2D goomba_sprite6;
Texture2D goomba_sprite7;
Texture2D goomba_sprite8;
Texture2D goomba_sprite9;
Texture2D goomba_sprite10;
Texture2D goomba_sprite11;
Texture2D goomba_sprite12;
Texture2D goomba_sprite13;
Texture2D goomba_sprite14;
Texture2D goomba_sprite15;
Texture2D goomba_sprite16;

// Mario
Texture2D Mario_Right;
Texture2D Mario_Left;
Texture2D Mario_Fire_Right;
Texture2D Mario_Fire_Left;
Texture2D mario_sprite;

// Items
Texture2D Mooshroom;
Texture2D FireFlower;
Texture2D Star;
Texture2D FireBall;
Texture2D FireBallTick1;
Texture2D FireBallTick2;

// Typography
Font marioFont;

void LoadGameTextures() {
	/*--------------------------------------------------------------------------*/
	/*                        Textures and Typography                           */
	/*--------------------------------------------------------------------------*/

	logoTexture = LoadTexture("Resources/Images/HOME/LogoProyecto1.png");
	UI = LoadTexture("Resources/Images/Seleccion Modo/Pantalla_Intro.png");
	money = LoadTexture("Resources/Sprites/Items/Monedas.png");
	Level1 = LoadTexture("Resources/Images/Seleccion Modo/World 1-1.png");
	icon_lifes = LoadTexture("Resources/Images/Player/Icon_Lifes.png");
	icon_money = LoadTexture("Resources/Images/Player/Icon_Money.png");
	coin_ui = LoadTexture("Resources/Sprites/Items/Coin_ui.png");


	/*--------------------------------------------------------------------------*/
	/*                           SpriteSheet Mario                              */
	/*--------------------------------------------------------------------------*/

	Mario_Right = LoadTexture("Resources/Sprites/MARIO/Mario_Right.png");
	Mario_Left = LoadTexture("Resources/Sprites/MARIO/Mario_Left.png");
	Mario_Fire_Right = LoadTexture("Resources/Sprites/MARIO/Fuego_Right.png");
	Mario_Fire_Left = LoadTexture("Resources/Sprites/MARIO/Fuego_Left.png");
	mario_sprite = Mario_Right;

	/*--------------------------------------------------------------------------*/
	/*                          SpriteSheet Enemigos                            */
	/*--------------------------------------------------------------------------*/

	Goomba = LoadTexture("Resources/Sprites/Enemies/Goomba.png");
	Goomba_chafado = LoadTexture("Resources/Sprites/Enemies/Goomba_chafado.png");
	Goomba_invertido = LoadTexture("Resources/Sprites/Enemies/Goomba_invertido.png");
	goomba_sprite1 = Goomba;
	goomba_sprite2 = Goomba;
	goomba_sprite3 = Goomba;
	goomba_sprite4 = Goomba;
	goomba_sprite5 = Goomba;
	goomba_sprite6 = Goomba;
	goomba_sprite7 = Goomba;
	goomba_sprite8 = Goomba;
	goomba_sprite9 = Goomba;
	goomba_sprite10 = Goomba;
	goomba_sprite11 = Goomba;
	goomba_sprite12 = Goomba;
	goomba_sprite13 = Goomba;
	goomba_sprite14 = Goomba;
	goomba_sprite15 = Goomba;
	goomba_sprite16 = Goomba;

	Koopa = LoadTexture("Resources/Sprites/Enemies/Koopa.png");
	Shell = LoadTexture("Resources/Sprites/Enemies/Shell.png");


	/*------------------------------------------------------------*/
	/*                           FONDO                            */
	/*------------------------------------------------------------*/
	fondo = LoadTexture("Resources/Sprites/Background/Fondo.png");
	azul = LoadTexture("Resources/Sprites/Background/Azul.png");
	negro = LoadTexture("Resources/Sprites/Background/negro.png");
	puntos = LoadTexture("Resources/Sprites/Background/Puntuaciones.png");
	suelo_m = LoadTexture("Resources/Sprites/Bloques/suelo.png");

	/*------------------------------------------------------------*/
	/*                           Items                            */
	/*------------------------------------------------------------*/
	Mooshroom = LoadTexture("Resources/Sprites/Items/Mushroom.png");
	FireFlower = LoadTexture("Resources/Sprites/Items/Power-ups.png");
	Star = LoadTexture("Resources/Sprites/Items/estrella.png");

	FireBall = LoadTexture("Resources/Sprites/Items/Bolas_Fuego.png");
	FireBallTick1 = LoadTexture("Resources/Sprites/Items/Explosion1.png");
	FireBallTick2 = LoadTexture("Resources/Sprites/Items/Explosion2.png");


	/*------------------------------------------------------------*/
	/*                          Bloques                           */
	/*------------------------------------------------------------*/
	bloque_int = LoadTexture("Resources/Sprites/Bloques/Bloque_int.png");
	bloque_int_a = LoadTexture("Resources/Sprites/Bloques/Bloque_int_a.png");

	ladrillo = LoadTexture("Resources/Sprites/Bloques/Ladrillo.png");
	escalera = LoadTexture("Resources/Sprites/Bloques/escalera.png");
	ladrillo_cueva = LoadTexture("Resources/Sprites/Bloques/Ladrillo_cueva.png");
	suelo_cueva = LoadTexture("Resources/Sprites/Bloques/Suelo_cueva.png");

	roto1 = LoadTexture("Resources/Sprites/Bloques/break1.png");
	roto1 = LoadTexture("Resources/Sprites/Bloques/break2.png");
	roto1 = LoadTexture("Resources/Sprites/Bloques/break3.png");
	roto1 = LoadTexture("Resources/Sprites/Bloques/break4.png");

	/*------------------------------------------------------------*/
	/*                          Tileset                           */
	/*------------------------------------------------------------*/

	flagTexture = LoadTexture("Resources/Sprites/Tileset/Flag.png");
	meta = LoadTexture("Resources/Sprites/Tileset/Meta.png");
	castle = LoadTexture("Resources/Sprites/Tileset/Castle.png");
	banderin = LoadTexture("Resources/Sprites/Tileset/banderin.png");
	tuberia_s = LoadTexture("Resources/Sprites/Tileset/Tuberia1.png");
	tuberia_m = LoadTexture("Resources/Sprites/Tileset/Tuberia2.png");
	tuberia_b = LoadTexture("Resources/Sprites/Tileset/Tuberia3.png");
	tuberia_cueva = LoadTexture("Resources/Sprites/Tileset/Tuberia_cueva.png");
	tubo = LoadTexture("Resources/Sprites/Tileset/Tubo.png");
	
}
void LoadGameTypography() {
	marioFont = LoadFont("Resources/Fonts/MarioFont.ttf");
}
