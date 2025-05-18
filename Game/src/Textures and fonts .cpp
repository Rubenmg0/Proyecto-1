#include "Textures and Fonts.h"

// Bloques
Texture2D bloque_int;
Texture2D bloque_int_a;
Texture2D ladrillo;
Texture2D escalera;
Texture2D ladrillo_cueva;
Texture2D suelo_cueva;

// Interactive Structures
Texture2D flagTexture;
Texture2D tuberia;

// Map
Texture2D castle;
Texture2D fondo;
Texture2D azul;
Texture2D negro;

// Pipes
Texture2D tuberia_s;
Texture2D tuberia_m;
Texture2D tuberia_b;
Texture2D tubo;
Texture2D tuberia_cueva;

// Objects
Texture2D money;

// Other Textures
Texture2D logoTexture;
Texture2D UI;
Texture2D Level1;
Texture2D icon_lifes;
Texture2D icon_money;

// Enemigos
Texture2D Goomba;
Texture2D Goomba_chafado;
Texture2D Koopa;
Texture2D Shell;
Texture2D goomba_sprite;

// Mario
Texture2D Mario_Right;
Texture2D Mario_Left;
Texture2D Mario_Fire_Right;
Texture2D Mario_Fire_Left;
Texture2D mario_sprite;

// Items
Texture2D Mooshroom;
Texture2D FireFlower;
Texture2D FireBall;

//typography
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
	goomba_sprite = Goomba;
	Koopa = LoadTexture("Resources/Sprites/Enemies/Koopa.png");
	Shell = LoadTexture("Resources/Sprites/Enemies/Shell.png");


	/*------------------------------------------------------------*/
	/*                           FONDO                            */
	/*------------------------------------------------------------*/
	fondo = LoadTexture("Resources/Sprites/Background/Fondo.png");
	azul = LoadTexture("Resources/Sprites/Background/Azul.png");
	negro = LoadTexture("Resources/Sprites/Background/negro.png");

	/*------------------------------------------------------------*/
	/*                           Items                            */
	/*------------------------------------------------------------*/
	Mooshroom = LoadTexture("Resources/Sprites/Items/Mushroom.png");
	FireFlower = LoadTexture("Resources/Sprites/Items/Power-ups.png");

	FireBall = LoadTexture("Resources/Sprites/Items/Bolas_Fuego.png");

	/*------------------------------------------------------------*/
	/*                          Bloques                           */
	/*------------------------------------------------------------*/
	bloque_int = LoadTexture("Resources/Sprites/Bloques/Bloque_int.png");
	bloque_int_a = LoadTexture("Resources/Sprites/Bloques/Bloque_int_a.png");

	ladrillo = LoadTexture("Resources/Sprites/Bloques/Ladrillo.png");
	escalera = LoadTexture("Resources/Sprites/Bloques/escalera.png");
	ladrillo_cueva = LoadTexture("Resources/Sprites/Bloques/Ladrillo_cueva.png");
	suelo_cueva = LoadTexture("Resources/Sprites/Bloques/Suelo_cueva.png");

	/*------------------------------------------------------------*/
	/*                          Tileset                           */
	/*------------------------------------------------------------*/

	castle = LoadTexture("Resources/Sprites/Tileset/Castle.png");
	flagTexture = LoadTexture("Resources/Sprites/Tileset/Flag.png");
	tuberia_s = LoadTexture("Resources/Sprites/Tileset/Tuberia1.png");
	tuberia_m = LoadTexture("Resources/Sprites/Tileset/Tuberia2.png");
	tuberia_b = LoadTexture("Resources/Sprites/Tileset/Tuberia3.png");
	tuberia_cueva = LoadTexture("Resources/Sprites/Tileset/Tuberia_cueva.png");
	tubo = LoadTexture("Resources/Sprites/Tileset/Tubo.png");
}
void LoadGameTypography() {

	marioFont = LoadFont("Resources/Fonts/MarioFont.ttf");

}
