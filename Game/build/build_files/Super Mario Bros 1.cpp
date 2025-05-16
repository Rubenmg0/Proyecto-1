﻿#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm> 
#include <iostream>
using namespace std;

/*--------------------------------------------------------------------------*/
/*                                LIBRARIES                                 */
/*--------------------------------------------------------------------------*/

#include "Music and Sounds.h"

/*--------------------------------------------------------------------------*/
/*                               GAMESTATES                                 */
/*--------------------------------------------------------------------------*/

enum class GameScreen { LOGO, TITLE, LEVEL1, GAMEPLAY, TIMEOUT, DEATH, ENDING };

/*--------------------------------------------------------------------------*/
/*                           CONST AND VARIABLES                            */
/*--------------------------------------------------------------------------*/

constexpr float PLAYER_JUMP_SPD = 350.0f;
constexpr float PLAYER_HOR_SPD = 250.0f;
constexpr float PLAYER_RUN_SPD = 250.0f;


const float suelo = 600;
int GRAVITY = 500;
int fireContador = 0;

int Timer;
int Score = 000000;
int Money = 00;
float elapsedTime = 0.0f;
int contmuerte = 0;
int conttiempo = 0;

/*--------------------------------------------------------------------------*/
/*                            STRUCTS DEFINITION                            */
/*--------------------------------------------------------------------------*/

struct Mario {
	Vector2 position;
	Rectangle mario_hitbox;
	Vector2 speed;
	const float saltoFuerza = -10.0f;

	bool canJump;
	bool canJump2;
	float jumpTime;
	float invulnerableTimer = 0.0f;
	const float invulnerableDuration = 2.0f;

	int alive = 1; //If alive = 0 --> Mario is death
	int lifes = 3;
	bool big;
	bool side; //If side = 0 (Right) ; If side = 1 (Left)
	bool fire;
	bool invencible;
	bool visible;

	Mario(float x, float y) : position{ x, y }, speed{ 0, 0 }, canJump(false), big(false), fire(false), invencible(false) {}
};

struct Enemy {
	Vector2 position;
	Rectangle goomba_hitbox;

	Vector2 speed;
	bool activated;
	bool alive;
	bool death;
	bool side; //If side = 0 (Right) ; If side = 1 (Left)

	Enemy(float x, float y) : position{ x, y }, activated(false), alive(true), death(false), side(true) {}
};
struct PowerUp {
	Vector2 position;
	Rectangle powerup_hitbox;

	Vector2 speed;
	bool active;
	bool side; //If side = 0 (Right) ; If side = 1 (Left)

	PowerUp(float x, float y) : position{ x, y }, active(false), side(true) {}
};

struct Projectile {
	Vector2 position;
	Rectangle projectile_hitbox;

	Vector2 speed;
	bool active;

	Projectile(float x, float y) : position{ x,y }, active(false), speed{ 0,0 } {}
};


//Structure for objects in the environment
struct EnvElement {
	Rectangle rect;
	Color color;

	EnvElement(float x, float y, float width, float height, Color col)
		: rect{ x, y, width, height, }, color(col) {
	} //Esto da los datos pa los bloques del juego
};

struct Flag {
	Vector2 position;
	bool reached;

	Flag(float x, float y) : position{ x, y }, reached(false) {}
};

//PIPE
struct Pipe {
	Rectangle pipe1 = { 2600, 350, 100, 200, };
	Rectangle pipe2 = { 565, -585, 20, 90 };
	bool enteringPipe1 = false; //No entra
	bool enteringPipe2 = false; //No entra 
};

/*--------------------------------------------------------------------------*/
/*                             MAIN CLASS GAME                              */
/*--------------------------------------------------------------------------*/
class Game {
private:
	//Screen
	constexpr static int screenWidth = 1024;
	constexpr static int screenHeight = 792;

	//Game
	GameScreen currentScreen;
	int framesCounter;
	bool hurryMusicPlayed = false;

	Camera2D camera;
	unsigned int frameCounter;
	unsigned int playFrameCounter;
	unsigned int currentPlayFrame;

	//Player
	Mario player;
	Texture2D mario_sprite;
	Texture2D Mario_Right;
	Texture2D Mario_Left;
	Texture2D Mario_Fire_Right;
	Texture2D Mario_Fire_Left;

	//Enemies
	Enemy goomba;
	Texture2D goomba_sprite;
	Texture2D Goomba;
	Texture2D Goomba_chafado;

	Enemy koopa;
	Texture2D Koopa;
	Enemy shell;
	Texture2D Shell;

	//PowerUp
	PowerUp mooshroom;
	Texture2D Mooshroom;
	PowerUp fireFlower;
	Texture2D FireFlower;

	//Projectiles
	Projectile fireBall;
	Texture2D FireBall;

	//Blocks
	Texture2D bloque_int;
	Texture2D bloque_int_a;

	Texture2D ladrillo;
	Texture2D escalera;

	Texture2D ladrillo_cueva;
	Texture2D suelo_cueva;

	//Interactive Structures
	Flag flag;
	Texture2D flagTexture;
	Texture2D tuberia;
	Pipe pipe;

	//Map
	Texture2D castle;
	Texture2D fondo;
	Texture2D azul;
	Texture2D negro;

	//Pipes
	Texture2D tuberia_s;
	Texture2D tuberia_m;
	Texture2D tuberia_b;
	Texture2D tubo;
	Texture2D tuberia_cueva;
	//Objects
	Texture2D money;

	//Other Textures
	Texture2D logoTexture;
	Texture2D UI;
	Texture2D Level1;
	Texture2D icon_lifes;
	Texture2D icon_money;

	//Typography
	Font marioFont;

	//Blocks
	vector<EnvElement> blocks = {

	{-200, 600, 3350, 200, GREEN}, // SUELO
	{3250, 600, 760, 200, GREEN}, // SUELO
	{4160, 600, 3063, 200, GREEN}, // SUELO
	{7320, 600, 3000, 200, GREEN}, // SUELO


	{650, 400, 50, 50, GREEN},	//PRIMER ? /MONEDAS

	//Conjuto de bloques
	{850, 400, 50, 50, GREEN},
	{900, 400, 50, 50, GREEN}, // TIENE UN CHAMPIÑON
	{950, 400, 50, 50, GREEN},
	{1000, 400, 50, 50, GREEN}, // MONEDAS
	{1050, 400, 50, 50, GREEN},
	//Bloque superior
{950, 200, 50, 50, GREEN}, //MONEDAS

//Tuberias
{1250, 500, 100, 100, GREEN},
{1675, 450, 100, 150, GREEN},
{2075, 400, 100, 200, GREEN},
{2600, 400, 100, 200, GREEN},

//Bloque oculto (ya se hará) -Vida extra-
//Boquete 1
{3150, 600, 50, 50, BLUE},
{3200, 600, 50, 50, BLUE},
{3150, 650, 50, 50, BLUE},
{3200, 650, 50, 50, BLUE},
//Segundo conjuto de bloques
	//Zona inferior
{3550, 400, 50, 50, GREEN},
{3600, 400, 50, 50, GREEN}, //Champiñon
{3650, 400, 50, 50, GREEN},
//Zona superior
{3700, 200, 50, 50, GREEN},
{3750, 200, 50, 50, GREEN},
{3800, 200, 50, 50, GREEN},
{3850, 200, 50, 50, GREEN},
{3900, 200, 50, 50, GREEN},
{3950, 200, 50, 50, GREEN},
{4000, 200, 50, 50, GREEN},
{4050, 200, 50, 50, GREEN},
//Boquete 2
{4010, 600, 50, 50, BLUE},
{4060, 600, 50, 50, BLUE},
{4110, 600, 50, 50, BLUE},

{4010, 650, 50, 50, BLUE},
{4060, 650, 50, 50, BLUE},
{4110, 650, 50, 50, BLUE},

{4010, 700, 50, 50, BLUE},
{4060, 700, 50, 50, BLUE},
{4110, 700, 50, 50, BLUE},


//Zona superior 2
{4250, 200, 50, 50, GREEN},
{4300, 200, 50, 50, GREEN},
{4350, 200, 50, 50, GREEN},
{4400, 200, 50, 50, GREEN},//Monedas
//Bloque inferior
{4400, 400, 50, 50, GREEN}, //Moneda

//Bloques extrella
{4700, 400, 50, 50, GREEN},
{4750, 400, 50, 50, GREEN}, //Estrella

//Bloques ?
	//Inferior
{5000, 400, 50, 50, GREEN}, //Monedas
{5125, 400, 50, 50, GREEN}, //Monedas
{5250, 400, 50, 50, GREEN}, //Monedas
//Superior
{5125, 200, 50, 50, GREEN}, //Champiñon

//Secuencia de bloques
{5550, 400, 50, 50, GREEN},

{5700, 200, 50, 50, GREEN},
{5750, 200, 50, 50, GREEN},
{5800, 200, 50, 50, GREEN},

{6000, 200, 50, 50, GREEN},
{6050, 200, 50, 50, GREEN},
{6100, 200, 50, 50, GREEN},
{6150, 200, 50, 50, GREEN},

{6050, 400, 50, 50, GREEN},
{6100, 400, 50, 50, GREEN},

//Primera escalera
//Primer escalon
{6310, 550, 50, 50, GREEN},
{6360, 550, 50, 50, GREEN},
{6410, 550, 50, 50, GREEN},
{6460, 550, 50, 50, GREEN},
//Segundo escalon
{6360, 500, 50, 50, GREEN},
{6410, 500, 50, 50, GREEN},
{6460, 500, 50, 50, GREEN},
//Tercer escalon
{6410, 450, 50, 50, GREEN},
{6460, 450, 50, 50, GREEN},
//Quarto escalon
{6460, 400, 50, 50, GREEN},
//-Invertida-//
//Primer escalón
{6590, 550, 50, 50, GREEN},
{6640, 550, 50, 50, GREEN},
{6690, 550, 50, 50, GREEN},
{6740, 550, 50, 50, GREEN},
//Segundo escalón
{6590, 500, 50, 50, GREEN},
{6640, 500, 50, 50, GREEN},
{6690, 500, 50, 50, GREEN},
//Tercer escalón
{6590, 450, 50, 50, GREEN},
{6640, 450, 50, 50, GREEN},
//Quarto escalón
{6590, 400, 50, 50, GREEN},

//Segunda escalera
//Primer escalon
{6970, 550, 50, 50, GREEN},
{7020, 550, 50, 50, GREEN},
{7070, 550, 50, 50, GREEN},
{7120, 550, 50, 50, GREEN},
{7170, 550, 50, 50, GREEN},

//Segundo escalon
{7020, 500, 50, 50, GREEN},
{7070, 500, 50, 50, GREEN},
{7120, 500, 50, 50, GREEN},
{7170, 500, 50, 50, GREEN},

//Tercer escalon
{7070, 450, 50, 50, GREEN},
{7120, 450, 50, 50, GREEN},
{7170, 450, 50, 50, GREEN},

//Quarto escalon
{7120, 400, 50, 50, GREEN},
{7170, 400, 50, 50, GREEN},

//BOQUETE 3

//-Invertida-//
//Primer escalón
{7320, 550, 50, 50, GREEN},
{7370, 550, 50, 50, GREEN},
{7420, 550, 50, 50, GREEN},

//Segundo escalón
{7320, 500, 50, 50, GREEN},
{7370, 500, 50, 50, GREEN},
{7420, 500, 50, 50, GREEN},

//Tercer escalón
{7320, 450, 50, 50, GREEN},
{7370, 450, 50, 50, GREEN},

//Quarto escalón
{7320, 400, 50, 50, GREEN},

//Tuberias finales
{7700, 500, 100, 100, GREEN},
//Bloques intermedios
{7950, 400, 50, 50, GREEN},
{8000, 400, 50, 50, GREEN},
{8050, 400, 50, 50, GREEN},
{8100, 400, 50, 50, GREEN},
{8460, 500, 100, 100, GREEN},

//ESCALERA FINAL
// Primer escalón (base de 9 bloques)
	{8570, 550, 50, 50, GREEN},
	{8620, 550, 50, 50, GREEN},
	{8670, 550, 50, 50, GREEN},
	{8720, 550, 50, 50, GREEN},
	{8770, 550, 50, 50, GREEN},
	{8820, 550, 50, 50, GREEN},
	{8870, 550, 50, 50, GREEN},
	{8920, 550, 50, 50, GREEN},
	{8970, 550, 50, 50, GREEN},

	// Segundo escalón (8 bloques)
	{8620, 500, 50, 50, GREEN},
	{8670, 500, 50, 50, GREEN},
	{8720, 500, 50, 50, GREEN},
	{8770, 500, 50, 50, GREEN},
	{8820, 500, 50, 50, GREEN},
	{8870, 500, 50, 50, GREEN},
	{8920, 500, 50, 50, GREEN},
	{8970, 500, 50, 50, GREEN},

	// Tercer escalón (7 bloques)
	{8670, 450, 50, 50, GREEN},
	{8720, 450, 50, 50, GREEN},
	{8770, 450, 50, 50, GREEN},
	{8820, 450, 50, 50, GREEN},
	{8870, 450, 50, 50, GREEN},
	{8920, 450, 50, 50, GREEN},
	{8970, 450, 50, 50, GREEN},

	// Cuarto escalón (6 bloques)
	{8720, 400, 50, 50, GREEN},
	{8770, 400, 50, 50, GREEN},
	{8820, 400, 50, 50, GREEN},
	{8870, 400, 50, 50, GREEN},
	{8920, 400, 50, 50, GREEN},
	{8970, 400, 50, 50, GREEN},

	// Quinto escalón (5 bloques)
	{8770, 350, 50, 50, GREEN},
	{8820, 350, 50, 50, GREEN},
	{8870, 350, 50, 50, GREEN},
	{8920, 350, 50, 50, GREEN},
	{8970, 350, 50, 50, GREEN},

	// Sexto escalón (4 bloques)
	{8820, 300, 50, 50, GREEN},
	{8870, 300, 50, 50, GREEN},
	{8920, 300, 50, 50, GREEN},
	{8970, 300, 50, 50, GREEN},

	// Séptimo escalón (3 bloques)
	{8870, 250, 50, 50, GREEN},
	{8920, 250, 50, 50, GREEN},
	{8970, 250, 50, 50, GREEN},

	// Octavo escalón (2 bloques)
	{8920, 200, 50, 50, GREEN},
	{8970, 200, 50, 50, GREEN},

	//Flag
	{9375, 550, 50, 50, GREEN},

	//Cueba escenario
		//Suelo
	{ -112, -500, 850, 100, GREEN },
	//Paredes-
{ -112, -1000, 50, 500, GREEN },
{ 688, -1000, 50, 500, GREEN },

//techo
{ 88, -1000, 350, 50, GREEN },//1

//Zona monedas
{ 88, -650, 350, 150, GREEN },//1
//tuberia
{ 578, -600, 100, 100, GREEN },//1

	};

public:
	//Initialise the game
	Game() : currentScreen(GameScreen::LOGO), framesCounter(0), player(50, -600), frameCounter(0),
		playFrameCounter(0), currentPlayFrame(0), goomba(1400, 600), koopa(1600, 600), flag(9375, 264), mooshroom(900, 350),
		fireFlower(450, 600), fireBall(0, 9000), shell(0, 9000) {

		InitWindow(screenWidth, screenHeight, "Super Mario + Screen Manager");
		InitAudioDevice();              // Initialize audio device
		SetTargetFPS(60);

		LoadGameSounds(); //Call all sounds
		LoadGameMusic(); //Call all music

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
		Shell = LoadTexture("Resources/Sprites/Enemies/Koopa.png");


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
		/*                          Bloques Cueva                     */
		/*------------------------------------------------------------*/



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

		/*--------------------------------------------------------------------------*/
		/*                            Text font										*/
		/*--------------------------------------------------------------------------*/

		marioFont = LoadFont("Resources/Fonts/MarioFont.ttf");

		//Camera of the game
		camera.target = player.position;
		camera.offset = { screenWidth / 2.0f, screenHeight / 2.0f };
		camera.rotation = 0.0f;
		camera.zoom = 1.15f;
	}

	~Game() {
		//Finalize textures and typography
		UnloadTexture(logoTexture);
		UnloadTexture(UI);
		UnloadTexture(Level1);
		UnloadTexture(mario_sprite);
		UnloadFont(marioFont);
		UnloadAudioAssets();

		CloseWindow();
	}


	void Run() {
		while (!WindowShouldClose()) {
			Update();
			Draw();
		}
	}

private:
	void Update() {

		//All Screens of the Game
		switch (currentScreen) {
		case GameScreen::LOGO:
			framesCounter++;
			if (framesCounter > 250) {
				currentScreen = GameScreen::TITLE;
			}
			break;

		case GameScreen::TITLE:
			framesCounter = 0;
			if (IsKeyPressed(KEY_ENTER)) {
				currentScreen = GameScreen::LEVEL1;
			}
			break;

		case GameScreen::LEVEL1:
			framesCounter++;
			if (framesCounter >= 120) {
				currentScreen = GameScreen::GAMEPLAY;
				player.position = { 50, 600 };
				camera.target.x = 333;
				camera.target.y = 350;
				goomba.position = { 1400, 600 };
				mooshroom.position = { 900, 350 };
				fireFlower.position = { 900, 600 };
				Timer = 400;
				Score = 000000;
				Money = 00;
				player.alive = 1;
				player.fire = 0;
				goomba.death = false;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;
				mario_sprite = Mario_Right;

				PlayMusicStream(musicOverworld);
			}
			break;

		case GameScreen::TIMEOUT:
			if (framesCounter == 0) {
				player.lifes--;
				framesCounter++;
				if (player.lifes <= 0) {
					while (framesCounter < 2999999999) {
						framesCounter++;
					}
					if (framesCounter >= 2999999999) {
						currentScreen = GameScreen::ENDING;
						PlaySound(sfxGameOver);
					}
				}
			}
			elapsedTime += GetFrameTime();

			if (elapsedTime >= 3.0f) {
				currentScreen = GameScreen::DEATH;
				elapsedTime = 0.0f;
			}
			break;

		case GameScreen::DEATH:
			elapsedTime += GetFrameTime();

			if (elapsedTime >= 3.0f) {
				currentScreen = GameScreen::GAMEPLAY;
				player.position = { 50, -600 };
				camera.target.x = 333;
				camera.target.y = 350;
				goomba.position = { 1400, 600 };
				mooshroom.position = { 900, 350 };
				fireFlower.position = { 900, 600 };
				Timer = 400;
				player.alive = 1;
				player.fire = 0;
				goomba.death = false;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;

				PlayMusicStream(musicOverworld);
			}
			break;

		case GameScreen::GAMEPLAY:
			framesCounter = 0;
			UpdateGameplay();

			if (player.position.y >= 1000) player.alive = 0;

			if (player.alive == 0) {
				if (contmuerte == 0)
				{
					StopMusicStream(musicOverworld);
					StopMusicStream(musicOverworld_hurry);
					PlaySound(sfxDeath);
					player.speed.y = -PLAYER_JUMP_SPD * 1.2f;
					player.canJump = false;
					player.canJump2 = true;
					player.jumpTime = 0.0f;
					if (framesCounter == 0) {
						player.lifes--;
						framesCounter++;
					}
					contmuerte++;
				}
				elapsedTime += GetFrameTime();
				if (elapsedTime >= 15.0f) {
					if (player.lifes <= 0) {
						while (elapsedTime < 0.0f) {
							elapsedTime += GetFrameTime();
						}
						if (elapsedTime >= 0.0f) {
							currentScreen = GameScreen::ENDING;
							PlaySound(sfxGameOver);
							break;
						}
					}
					currentScreen = GameScreen::DEATH;
					elapsedTime = 0.0f;
				}
			}
			if (flag.reached) {
				StopMusicStream(musicOverworld);
				StopMusicStream(musicOverworld_hurry);
				if (conttiempo == 0 && player.position.y == 600) {
					PlaySound(sfxCompleted);
					conttiempo++;
				}
			}

			if (Timer < 100 && !hurryMusicPlayed) {
				StopMusicStream(musicOverworld);
				PlayMusicStream(musicOverworld_hurry);
				hurryMusicPlayed = true;
			}

			if (Timer <= 0) {
				if (contmuerte == 0)
				{
					StopMusicStream(musicOverworld_hurry);
					PlaySound(sfxDeath);
					player.speed.y = -PLAYER_JUMP_SPD * 1.2f;
					player.canJump = false;
					player.canJump2 = true;
					player.jumpTime = 0.0f;
					contmuerte++;
				}

				elapsedTime += GetFrameTime();
				if (elapsedTime >= 15.0f) {
					currentScreen = GameScreen::TIMEOUT;
					elapsedTime = 0.0f;
				}
			}
			break;

		case GameScreen::ENDING:

			if (IsKeyPressed(KEY_ENTER)) {
				player.lifes = 3;
				flag.reached = false;
				currentScreen = GameScreen::TITLE;
			}
			break;
		}
	}

	void UpdateGameplay() {

		if (player.big) player.mario_hitbox = { player.position.x, player.position.y, 23,32 };
		if (!player.big) player.mario_hitbox = { player.position.x, player.position.y, 23,16 };
		goomba.goomba_hitbox = { goomba.position.x, goomba.position.y, 16,16 };
		player.mario_hitbox.x += player.speed.x;
		player.mario_hitbox.y += player.speed.y;
		mooshroom.powerup_hitbox = { mooshroom.position.x, mooshroom.position.y, 16,16 };
		fireFlower.powerup_hitbox = { fireFlower.position.x, fireFlower.position.y, 16, 16 };
		fireBall.projectile_hitbox = { fireBall.position.x, fireBall.position.y, 4, 4 };
		koopa.goomba_hitbox = { koopa.position.x, koopa.position.y, 16, 24 };
		shell.goomba_hitbox = { shell.position.x,shell.position.y,16,16 };

		bool hitObstacleFloor = false;
		bool hitObstacleWall = false;
		bool onGroundEnemy = false;
		bool onGroundKoopa = false;
		bool onGroundShell = false;
		bool onGroundPowerUp = false;
		bool projectileHitObstacleFloor = false;

		float deltaTime = GetFrameTime();
		elapsedTime += deltaTime * 2.5;

		//In-game controls and conditions
		if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_DOWN) && !flag.reached && Timer > 0 && player.alive != 0 && !pipe.enteringPipe1 && !pipe.enteringPipe2)
		{
			if (IsKeyDown(KEY_LEFT_SHIFT) && !flag.reached && Timer > 0 && !hitObstacleWall)
			{
				player.position.x += PLAYER_RUN_SPD * deltaTime;
				player.speed.x = 2.0f;
			}
			player.position.x += PLAYER_HOR_SPD * deltaTime;
			player.speed.x = 2.0f;
		}

		if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_DOWN) && player.position.x > camera.target.x - screenWidth / 2.3f && !flag.reached && Timer > 0 && player.alive != 0 && !pipe.enteringPipe1 && !pipe.enteringPipe2)
		{
			if (IsKeyDown(KEY_LEFT_SHIFT) && player.position.x > camera.target.x - screenWidth / 2.0f && !flag.reached && Timer > 0 && !hitObstacleWall)
			{
				player.position.x -= PLAYER_RUN_SPD * deltaTime;
				player.speed.x = -2.0f;
			}
			player.position.x -= PLAYER_HOR_SPD * deltaTime;
			player.speed.x = -2.0f;
		}
		if (player.position.x > camera.target.x && camera.target.x < 9795 && player.position.y >= 0)
		{
			camera.target.x = player.position.x;
		}

		static constexpr float MAX_JUMP_TIME = 0.3f;
		static constexpr float JUMP_HOLD_FORCE = 500.0f;

		if (IsKeyPressed(KEY_SPACE) && player.canJump && !flag.reached && Timer > 0 && player.alive != 0 && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			player.speed.y = -PLAYER_JUMP_SPD;
			player.canJump = false;
			player.canJump2 = true;
			player.jumpTime = 0.0f;
			PlaySound(sfxJumpSmall);
		}

		if (IsKeyDown(KEY_SPACE) && player.canJump2 && player.jumpTime < MAX_JUMP_TIME && !flag.reached && Timer > 0 && player.alive != 0 && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			player.speed.y -= JUMP_HOLD_FORCE * deltaTime;
			player.jumpTime += deltaTime;
		}

		if (IsKeyReleased(KEY_SPACE) && !flag.reached && Timer > 0 && player.alive != 0 && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			player.canJump2 = false;  //Cut the jump when releasing the key
			player.speed.y += JUMP_HOLD_FORCE - 300;
		}

		if (player.fire && player.big) {
			if (IsKeyPressed(KEY_X) && !pipe.enteringPipe1 && !pipe.enteringPipe2 && !flag.reached && player.alive && Timer > 0) {
				fireBall.position = { player.position.x, player.position.y + -40 };
				fireBall.active = true;
				if (player.side) fireBall.speed.x = -650 * deltaTime;
				else if (!player.side) fireBall.speed.x = 650 * deltaTime;
			}
		}
		if (fireBall.active) {
			fireBall.position += fireBall.speed;
		}

		if (Timer <= 0 || player.alive == 0) {
			hitObstacleFloor = false;
		}
		//MOOSHROOM
		if (player.position.x >= mooshroom.position.x && player.alive != 0) {
			mooshroom.active = true;
		}
		if (!mooshroom.active && !player.big) mooshroom.position = { 900, 350 };

		mooshroom.speed.x = 1.0f;
		if (mooshroom.active && player.alive != 0 && mooshroom.side) {
			mooshroom.position.x += -120 * deltaTime;
		}

		if (mooshroom.active && player.alive != 0 && !mooshroom.side) {
			mooshroom.position.x += 120 * deltaTime;
		}

		//FIRE FLOWER
		if (player.position.x >= fireFlower.position.x && player.alive != 0) {
			fireFlower.active = true;
		}

		//SHELL
		if (player.position.x >= shell.position.x && player.alive != 0) {
			shell.activated = true;
		}

		if (shell.activated && shell.death == false && player.alive != 0 && shell.side && Timer > 0) {
			shell.position.x += shell.speed.x * -deltaTime;
		}
		if (shell.activated && shell.death == false && player.alive != 0 && !shell.side && Timer > 0) {
			shell.position.x += shell.speed.x * deltaTime;
		}

		if (shell.death == true) {
			if (player.position.y >= shell.position.y) {
				shell.position.y = 1000;
			}
		}

		//GOOMBA
		if (player.position.x - goomba.position.x <= -200 && goomba.death == false && player.alive != 0 && Timer > 0) {
			goomba.activated = true;
		}

		goomba.speed.x = 1.0f;
		if (goomba.activated && goomba.death == false && player.alive != 0 && goomba.side && Timer > 0) {
			goomba.position.x += -120 * deltaTime;
		}
		if (goomba.activated && goomba.death == false && player.alive != 0 && !goomba.side && Timer > 0) {
			goomba.position.x += 120 * deltaTime;
		}

		if (goomba.death == true) {
			if (player.position.y >= goomba.position.y) {
				goomba.position.y = 1000;
			}
		}

		//Koopa
		if (player.position.x - koopa.position.x <= -200 && koopa.death == false && player.alive != 0 && Timer > 0) {
			koopa.activated = true;
		}
		koopa.speed.x = 1.0f;
		if (koopa.activated && koopa.death == false && player.alive != 0 && koopa.side && Timer > 0) {
			koopa.position.x += -120 * deltaTime;
		}
		if (koopa.activated && koopa.death == false && player.alive != 0 && !koopa.side && Timer > 0) {
			koopa.position.x += 120 * deltaTime;
		}

		if (koopa.death == true) {
			if (player.position.y >= koopa.position.y) {
				shell.position = koopa.position;
				koopa.position.y = 1000;
			}

		}

		//--------Colisiones de Mario--------\\

		//Suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y >= player.position.y
				&& block.rect.y <= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				hitObstacleFloor = true;
				player.speed.y = 0.0f;
				player.position.y = block.rect.y;
			}
		}

		//Techo
		for (EnvElement block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 && player.speed.y < 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 10
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2 >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				player.speed.y = 0.0f;
				player.position.y = block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2;
			}
			else if (!player.big && Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				player.speed.y = 0.0f;
				player.position.y = block.rect.y + block.rect.height + block.rect.height;
			}
		}

		//Lados
		float nextX = player.position.x + player.speed.x * deltaTime; //Calcula la posición futura en X

		//--- COLISIÓN POR LA DERECHA (Mario viene de la izquierda) ---
		for (EnvElement block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x > 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height + block.rect.height - 4) &&
				player.position.x - 10 <= block.rect.x &&
				(nextX + player.mario_hitbox.width) >= block.rect.x
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				//¡Choque! Ajustamos posición y detenemos velocidad horizontal
				hitObstacleWall = true;
				player.speed.x = 0;
				player.position.x = block.rect.x - player.mario_hitbox.width;
			}
			else if (!player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x > 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				player.position.x - 10 <= block.rect.x &&
				(nextX + player.mario_hitbox.width) >= block.rect.x
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				//¡Choque! Ajustamos posición y detenemos velocidad horizontal
				hitObstacleWall = true;
				player.speed.x = 0;
				player.position.x = block.rect.x - player.mario_hitbox.width;
			}
		}

		//--- COLISIÓN POR LA IZQUIERDA (Mario viene de la derecha) ---
		for (EnvElement block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x < 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height + block.rect.height - 4) &&
				player.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextX) <= (block.rect.x + block.rect.width + 14)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				//¡Choque! Ajustamos posición y detenemos velocidad horizontal
				hitObstacleWall = true;
				player.speed.x = 0;
				player.position.x = block.rect.x + block.rect.width + player.mario_hitbox.width - 7;
			}
			else if (!player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x < 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				player.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextX) <= (block.rect.x + block.rect.width + 14)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				//¡Choque! Ajustamos posición y detenemos velocidad horizontal
				hitObstacleWall = true;
				player.speed.x = 0;
				player.position.x = block.rect.x + block.rect.width + player.mario_hitbox.width - 7;
			}
		}

		if (!hitObstacleFloor) {
			player.position.y += player.speed.y * deltaTime;
			if (player.speed.y > 0)
			{
				player.speed.y += GRAVITY * 3.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				player.speed.y += GRAVITY * deltaTime; //Normal upward gravity
			}
			player.canJump = false;
			player.canJump2 = true;
		}
		else {
			player.canJump = true;
		}

		//--------Colisiones de Enemigos--------\\

		//Con Mario
		if (goomba.alive && !goomba.death && player.position.x + player.mario_hitbox.width + 10 >= goomba.position.x &&
			player.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= goomba.position.y && player.position.y <= goomba.position.y + goomba.goomba_hitbox.height
			&& !player.invencible)
		{
			if (player.position.y + player.mario_hitbox.height <= goomba.position.y && player.alive) {
				goomba.death = true;
				Score += 100;
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (player.big && player.fire && !player.invencible) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (player.big && !player.fire && !player.invencible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (!player.big && !player.fire && !player.invencible) {
				player.alive = 0;
			}
		}

		if (koopa.alive && !koopa.death && player.position.x + player.mario_hitbox.width + 10 >= koopa.position.x &&
			player.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= koopa.position.y && player.position.y <= koopa.position.y + koopa.goomba_hitbox.height
			&& !player.invencible)
		{
			if (player.position.y + player.mario_hitbox.height <= koopa.position.y && player.alive) {
				shell.position = koopa.position;
				shell.side = koopa.side;
				koopa.death = true;
				Score += 100;
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (player.big && player.fire && !player.invencible) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (player.big && !player.fire && !player.invencible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (!player.big && !player.fire && !player.invencible) {
				player.alive = 0;
			}
		}

		if (shell.alive && !shell.death && player.position.x + player.mario_hitbox.width + 10 >= shell.position.x &&
			player.position.x <= shell.position.x + shell.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= shell.position.y && player.position.y <= shell.position.y + shell.goomba_hitbox.height
			&& !player.invencible)
		{
			if (player.position.y + player.mario_hitbox.height <= shell.position.y && player.alive) {
				shell.speed.x = 0;
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (player.big && player.fire && !player.invencible && shell.speed.x <= 0 && player.side) {
				shell.speed.x = 300;
				shell.side = true;
			}
			else if (player.big && !player.fire && !player.invencible && shell.speed.x <= 0 && player.side) {
				shell.speed.x = 300;
				shell.side = true;
			}
			else if (!player.big && !player.fire && !player.invencible && shell.speed.x <= 0 && player.side) {
				shell.speed.x = 300;
				shell.side = true;
			}
			else if (player.big && player.fire && !player.invencible && shell.speed.x <= 0 && !player.side) {
				shell.speed.x = 300;
				shell.side = false;
			}
			else if (player.big && !player.fire && !player.invencible && shell.speed.x <= 0 && !player.side) {
				shell.speed.x = 300;
				shell.side = false;
			}
			else if (!player.big && !player.fire && !player.invencible && shell.speed.x <= 0 && !player.side) {
				shell.speed.x = 300;
				shell.side = false;
			}
			else if (player.big && player.fire && !player.invencible && shell.speed.x > 0) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (player.big && !player.fire && !player.invencible && shell.speed.x > 0) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
				player.speed.y = -PLAYER_JUMP_SPD * 0.5f;
			}
			else if (!player.big && !player.fire && !player.invencible && shell.speed.x > 0) {
				player.alive = 0;
			}
		}


		if (player.invencible) {
			player.invulnerableTimer += GetFrameTime();
			player.visible = fmod(player.invulnerableTimer, 0.15f) < 0.1f;
			if (player.invulnerableTimer >= player.invulnerableDuration) {
				player.invencible = false;
				player.invulnerableTimer = 0.0f;
			}
		}

		//Con bola de fuego
		if (goomba.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= goomba.position.x &&
			fireBall.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= goomba.position.y && fireBall.position.y <= goomba.position.y + goomba.goomba_hitbox.height)
		{
			Score += 100;
			goomba.death = true;
			fireBall.active = false;
			fireBall.position.y = 9000;
		}

		if (koopa.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= koopa.position.x &&
			fireBall.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= koopa.position.y && fireBall.position.y <= koopa.position.y + koopa.goomba_hitbox.height)
		{
			Score += 100;
			koopa.death = true;
			fireBall.active = false;
			fireBall.position.y = 9000;
		}


		//Con el suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && goomba.activated
				&& block.rect.x <= goomba.position.x + goomba.goomba_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= goomba.position.x
				&& block.rect.y + block.rect.height >= goomba.position.y
				&& block.rect.y <= goomba.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE)) {
				onGroundEnemy = true;
				goomba.speed.y = 0.0f;
				goomba.position.y = block.rect.y;
			}
		}

		if (!onGroundEnemy && player.alive && Timer > 0) {
			goomba.position.y += (GRAVITY - 300) * deltaTime;
			if (goomba.position.y > 0)
			{
				goomba.position.y += (GRAVITY - 300) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				goomba.position.y += (GRAVITY - 300) * deltaTime; //Normal upward gravity
			}
		}

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && koopa.activated
				&& block.rect.x <= koopa.position.x + koopa.goomba_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= koopa.position.x
				&& block.rect.y + block.rect.height >= koopa.position.y
				&& block.rect.y <= koopa.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE)) {
				onGroundKoopa = true;
				koopa.speed.y = 0.0f;
				koopa.position.y = block.rect.y;
			}
		}

		if (!onGroundKoopa && player.alive && Timer > 0) {
			koopa.position.y += (GRAVITY - 300) * deltaTime;
			if (koopa.position.y > 0)
			{
				koopa.position.y += (GRAVITY - 300) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				koopa.position.y += (GRAVITY - 300) * deltaTime; //Normal upward gravity
			}
		}

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && shell.activated
				&& block.rect.x <= shell.position.x + shell.goomba_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= shell.position.x
				&& block.rect.y + block.rect.height >= shell.position.y
				&& block.rect.y <= shell.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE)) {
				onGroundShell = true;
				shell.speed.y = 0.0f;
				shell.position.y = block.rect.y;
			}
		}

		if (!onGroundShell && player.alive && Timer > 0) {
			shell.position.y += (GRAVITY - 300) * deltaTime;
			if (shell.position.y > 0)
			{
				shell.position.y += (GRAVITY - 300) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				shell.position.y += (GRAVITY - 300) * deltaTime; //Normal upward gravity
			}
		}


		//Los lados
		float nextXE = goomba.position.x + goomba.speed.x * deltaTime; //Calcula la posición futura en X

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && goomba.alive &&
				goomba.activated && !goomba.side &&
				goomba.position.y > block.rect.y &&
				goomba.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				goomba.position.x - 10 <= block.rect.x &&
				(nextXE + goomba.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				goomba.side = true;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				goomba.activated && goomba.side &&
				goomba.position.y > block.rect.y &&
				goomba.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				goomba.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextXE) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				goomba.side = false;
			}
		}

		//Los lados
		float next = koopa.position.x + koopa.speed.x * deltaTime; //Calcula la posición futura en X

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && koopa.alive &&
				koopa.activated && !koopa.side &&
				koopa.position.y > block.rect.y &&
				koopa.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				koopa.position.x - 10 <= block.rect.x &&
				(next + koopa.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				koopa.side = true;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				koopa.activated && koopa.side &&
				koopa.position.y > block.rect.y &&
				koopa.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				koopa.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(next) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				koopa.side = false;
			}
		}
		//Los lados
		float nextshell = shell.position.x + shell.speed.x * deltaTime; //Calcula la posición futura en X

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && shell.alive &&
				shell.activated && !shell.side &&
				shell.position.y > block.rect.y &&
				shell.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				shell.position.x - 10 <= block.rect.x &&
				(nextshell + shell.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				shell.side = true;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				shell.activated && shell.side &&
				shell.position.y > block.rect.y &&
				shell.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				shell.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextshell) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				shell.side = false;
			}
		}
		//--------Colisiones de Power-Ups--------\\

		//Con Mario
		//Mushroom
		if (mooshroom.active && player.position.x + player.mario_hitbox.width + 10 >= mooshroom.position.x &&
			player.position.x <= mooshroom.position.x + mooshroom.powerup_hitbox.width + 20 &&
			player.position.y >= mooshroom.position.y && player.position.y <= mooshroom.position.y + mooshroom.powerup_hitbox.height)
		{
			PlaySound(sfxMushroom);
			if (!player.big) player.big = true;
			mooshroom.active = false;
			Score += 1000;
			mooshroom.position.y = 1000;
		}

		//Fire Flower
		if (fireFlower.active && player.position.x + player.mario_hitbox.width + 10 >= fireFlower.position.x &&
			player.position.x <= fireFlower.position.x + fireFlower.powerup_hitbox.width + 20 &&
			player.position.y >= fireFlower.position.y && player.position.y <= fireFlower.position.y + fireFlower.powerup_hitbox.height)
		{
			if (!player.fire && player.big) {
				player.fire = true;
				fireFlower.active = false;
				Score += 1000;
				fireFlower.position.y = 1000;
			}
			else {
				player.big = true;
				fireFlower.active = false;
				Score += 1000;
				fireFlower.position.x = 10000;
			}
		}

		//Con el suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && mooshroom.active
				&& block.rect.x <= mooshroom.position.x + mooshroom.powerup_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= mooshroom.position.x
				&& block.rect.y + block.rect.height >= mooshroom.position.y
				&& block.rect.y <= mooshroom.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE)) {
				onGroundPowerUp = true;
				mooshroom.speed.y = 0.0f;
				mooshroom.position.y = block.rect.y;
			}
		}

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && fireBall.active
				&& block.rect.x <= fireBall.position.x + fireBall.projectile_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= fireBall.position.x
				&& block.rect.y + block.rect.height >= fireBall.position.y
				&& block.rect.y <= fireBall.position.y) {
				projectileHitObstacleFloor = true;
				fireBall.speed.y = 500.0f * deltaTime;
				projectileHitObstacleFloor = false;

				fireBall.position.y = block.rect.y;

			}
		}
		if (!projectileHitObstacleFloor && player.alive && Timer > 0) {
			fireBall.position.y += (10) * deltaTime;
			if (fireBall.position.y > 0)
			{
				fireBall.position.y += (10) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				fireBall.position.y += (10) * deltaTime; //Normal upward gravity
			}
		}

		if (!onGroundPowerUp && player.alive && Timer > 0) {
			mooshroom.position.y += (GRAVITY - 300) * deltaTime;
			if (mooshroom.position.y > 0)
			{
				mooshroom.position.y += (GRAVITY - 300) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				mooshroom.position.y += (GRAVITY - 300) * deltaTime; //Normal upward gravity
			}
		}

		//Los lados
		float nextXP = mooshroom.position.x + mooshroom.speed.x * deltaTime; //Calcula la posición futura en X

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				mooshroom.active && !mooshroom.side &&
				mooshroom.position.y > block.rect.y &&
				mooshroom.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				mooshroom.position.x - 10 <= block.rect.x &&
				(nextXP + mooshroom.powerup_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				mooshroom.side = true;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				mooshroom.active && mooshroom.side &&
				mooshroom.position.y > block.rect.y &&
				mooshroom.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				mooshroom.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextXP) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE))
			{
				mooshroom.side = false;
			}
		}
		//--------Colision Tuberia--------\\
			// PIPES 
		if (!pipe.enteringPipe1 && player.position.x + player.mario_hitbox.width - 5 >= pipe.pipe1.x && player.position.x <= pipe.pipe1.x + pipe.pipe1.width + 10
			&& player.position.y == 400 && IsKeyDown(KEY_DOWN)) {
			pipe.enteringPipe1 = true;
		}

		if (pipe.enteringPipe1) {
			player.position.x += 0.5;

			if (player.position.y >= 350) {
				// Por ejemplo, cambia de zona
				player.position.x = -10;
				player.position.y = -950;

				camera.target.x = 333;
				camera.target.y = -750;
				pipe.enteringPipe1 = false;
			}
		}

		if (!pipe.enteringPipe2 && player.position.x >= pipe.pipe2.x - 20 && player.position.y >= pipe.pipe2.y
			&& player.position.y < 0 && IsKeyDown(KEY_RIGHT)) {
			pipe.enteringPipe2 = true;
		}
		if (pipe.enteringPipe2) {
			player.position.x += 0.5;

			if (player.position.x >= 600) {
				// Por ejemplo, cambia de zona
				player.position.x = 7750;
				player.position.y = 500;

				camera.target.x = 8000;
				camera.target.y = 350;
				pipe.enteringPipe2 = false;
			}
		}

		//--------Colision Bandera--------\\
		
		if (!flag.reached && player.position.x >= flag.position.x - 20) {
			PlaySound(sfxFlagpole);
			player.position.x = flag.position.x;
			player.speed.y = 0;
			elapsedTime = 0;
			flag.reached = true;
		}

		if (flag.reached) {
			if (!hitObstacleFloor && player.position.y != 550) {
				player.position.y += 1 * 0.01;
			}
			else if (hitObstacleFloor) {
				if (player.position.y >= flag.position.y + 50) {
					float playerMovementSpeed = 120.0f * GetFrameTime();
					player.position.x += playerMovementSpeed;
				}

				if (player.position.x >= flag.position.x + 800) {
					currentScreen = GameScreen::ENDING;
					Mario_Right = LoadTexture("Resources/Sprites/MARIO/Mario_Right.png");
					Mario_Fire_Right = LoadTexture("Resources/Sprites/MARIO/Fuego_Right.png");
				}
			}
		}

		// FUNCIONES DE PRUEBA //

		if (IsKeyPressed(KEY_R)) {
			player.position = { 50, 600 };
			camera.target.x = 333;
			camera.target.y = 350;
			goomba.position = { 1400, 600 };
			koopa.position = { 1600, 600 };
			mooshroom.position = { 900, 350 };
			fireFlower.position = { 900, 600 };
			shell.position = { 0, 10000 };
			Timer = 400;
			Money = 00;
			Score = 000000;
			flag.reached = false;
			player.alive = 1;
			player.lifes = 3;
			player.big = 0;
			player.fire = 0;
			goomba.side = true;
			goomba.death = false;
			koopa.side = true;
			koopa.death = false;
			elapsedTime = 0.0f;
			contmuerte = 0;
		}

		if (IsKeyPressed(KEY_P)) {
			player.alive = 0;
		}
		if (IsKeyPressed(KEY_L)) {
			Score += 50;
		}
		if (IsKeyPressed(KEY_I)) {
			Score += 10000;
		}
		if (IsKeyPressed(KEY_O)) {
			Money++;
			player.invencible = true;
		}

		if (elapsedTime >= 1.0f && Timer > 0 && player.alive == 1 && !flag.reached) {
			Timer--;
			elapsedTime = 0.0f;
		}

		if (IsKeyPressed(KEY_K)) {
			player.lifes = 0;
			currentScreen = GameScreen::ENDING;
		}

		if (IsKeyPressed(KEY_B)) {
			player.big = 1;
		}
		if (IsKeyPressed(KEY_G)) {
			goomba.death = false;
			goomba.position.x = player.position.x + 200;
			goomba.position.y = player.position.y;
		}
	}

	void Draw() {
		BeginDrawing();
		ClearBackground(WHITE);

		int frameWidth = 16; //Each frame measures 16x16 pixels
		int frameHeight = 16;
		Rectangle sourceRec = { 0, 0, (float)frameWidth, (float)frameHeight };

		static float frameTime = 0.0f;
		static int currentFrame = 0;
		float frameSpeed = 1.0f; //Velocity of animation

		//Draw all the Screens
		switch (currentScreen) {
		case GameScreen::LOGO:

			DrawTextureEx(logoTexture, { (screenWidth - logoTexture.width - logoTexture.width + 1000) / 9.0f, (screenHeight - logoTexture.height + 700) / 10.0f }, 0.0f, 1.2f, WHITE);
			DrawText("Project 1 - GDD - CITM", 330, 420, 30, GRAY);
			DrawText("   Members", 420, 500, 25, GRAY);
			DrawText(" Marc Jimenez", 408, 550, 25, GRAY);
			DrawText(" Ruben Mateo", 410, 600, 25, GRAY);
			DrawText("Sauc Pellejero", 410, 650, 25, GRAY);
			DrawText("Professor: Alejandro Gomez Paris", 310, 725, 25, GRAY);
			DrawText("Replica Super Mario Bros 1 con Raylib", 220, 100, 30, GRAY);

			break;

		case GameScreen::TITLE:

			DrawTextureEx(UI, { (0), (0) }, 0.0f, 1.65f, WHITE);
			break;

		case GameScreen::LEVEL1:

			DrawTextureEx(Level1, { (0), (0) }, 0.0f, 4.0f, WHITE);
			break;

		case GameScreen::GAMEPLAY:

			DrawGameplay();
			AudioGameplay();


			break;

		case GameScreen::TIMEOUT:

			DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
			UItest();
			DrawTextEx(marioFont, TextFormat("TIME UP"), { screenWidth / 2 - 110, screenHeight / 2 }, 30, 1, WHITE);
			DrawTextureEx(icon_money, { 343, 50 }, 0, 1, WHITE);
			break;

		case GameScreen::DEATH:

			DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
			UItest();
			DrawTextEx(marioFont, TextFormat("World 1-1"), { screenWidth / 2 - 150, screenHeight / 2 - 100 }, 35, 1, WHITE);
			DrawTextureEx(icon_lifes, { screenWidth / 2 - 120, screenHeight / 2 - 30 }, 0.0f, 1.5f, WHITE);
			DrawTextEx(marioFont, TextFormat(" x  %d", player.lifes), { screenWidth / 2 - 40, screenHeight / 2 }, 30, 1, WHITE);
			DrawTextureEx(icon_money, { 343, 50 }, 0, 1, WHITE);
			break;
		case GameScreen::ENDING:

			DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);
			UItest();
			DrawTextEx(marioFont, TextFormat("GAME OVER"), { screenWidth / 2 - 150, screenHeight / 2 }, 30, 1, WHITE);
			break;
		}
		EndDrawing();
	}

	void UItest() {
		if (Score < 50) {
			DrawTextEx(marioFont, TextFormat("MARIO\n00000%d", Score), { 100, 30 }, 32, 1, WHITE);
		}
		if (Score >= 50 && Score < 100) {
			DrawTextEx(marioFont, TextFormat("MARIO\n0000%d", Score), { 100, 30 }, 32, 1, WHITE);
		}
		if (Score >= 100 && Score < 1000) {
			DrawTextEx(marioFont, TextFormat("MARIO\n000%d", Score), { 100, 30 }, 32, 1, WHITE);
		}
		if (Score >= 1000 && Score < 10000) {
			DrawTextEx(marioFont, TextFormat("MARIO\n00%d", Score), { 100, 30 }, 32, 1, WHITE);
		}
		if (Score >= 10000 && Score < 100000) {
			DrawTextEx(marioFont, TextFormat("MARIO\n0%d", Score), { 100, 30 }, 32, 1, WHITE);
		}
		if (Score > 100000) {
			DrawTextEx(marioFont, TextFormat("MARIO\n%d", Score), { 100, 30 }, 32, 1, WHITE);
		}

		if (Money < 10) {

			DrawTextEx(marioFont, TextFormat("\n x", Money), { 360, 30 }, 32, 1, WHITE);
			DrawTextEx(marioFont, TextFormat("\n  0%d", Money), { 365, 30 }, 32, 1, WHITE);
		}
		if (Money >= 10 && Money < 100) {
			DrawTextEx(marioFont, TextFormat("\n x", Money), { 360, 30 }, 32, 1, WHITE);
			DrawTextEx(marioFont, TextFormat("\n  %d", Money), { 365, 30 }, 32, 1, WHITE);
		}
		if (Money == 100) {
			Money = 0;
			player.lifes++;
		}
		DrawTextEx(marioFont, TextFormat("WORLD\n 1-1 "), { 580, 30 }, 32, 1, WHITE);
		DrawTextEx(marioFont, TextFormat("TIME"), { 800, 30 }, 32, 1, WHITE);
		if (currentScreen == GameScreen::GAMEPLAY)
		{
			if (Timer >= 100) {
				DrawTextEx(marioFont, TextFormat("\n %d", Timer), { 800, 30 }, 32, 1, WHITE);
			}
			if (Timer < 100 && Timer >= 10) {
				DrawTextEx(marioFont, TextFormat("\n 0%d", Timer), { 800, 30 }, 32, 1, WHITE);
			}
			if (Timer < 10 && Timer > 0) {
				DrawTextEx(marioFont, TextFormat("\n 00%d", Timer), { 800, 30 }, 32, 1, WHITE);
			}
			if (Timer == 0) {
				DrawTextEx(marioFont, TextFormat("\n 000", Timer), { 800, 30 }, 32, 1, WHITE);
			}
		}
	}

	void AudioGameplay() {
		UpdateMusicStream(musicOverworld);
		UpdateMusicStream(musicOverworld_hurry);
	}

	void UnloadAudioAssets() {
		UnloadMusicStream(musicOverworld);
		UnloadMusicStream(musicOverworld_hurry);
		UnloadMusicStream(musicInvencible);
		UnloadMusicStream(musicInvencible_hurry);

		UnloadSound(sfxJumpSmall);
		UnloadSound(sfxJumpSuper);
		UnloadSound(sfxMushroom);
		UnloadSound(sfxGameOver);
		UnloadSound(sfxFlagpole);
		UnloadSound(sfxDeath);
		UnloadSound(sfxCompleted);

		CloseAudioDevice();
	}

	void DrawGameplay() {
		BeginMode2D(camera);
		ClearBackground(BLUE);

		//Player
		int frameWidthP;
		int frameHeightP;

		if (!player.fire) {
			if (player.big == 0) { //Small Mode
				frameWidthP = 16; //Each frame mesure 16x16 pixels
				frameHeightP = 16;
			}
			else if (player.big == 1) { //Big Mode
				frameWidthP = 16;
				frameHeightP = 32;
			}
		}
		else if (player.fire && player.big) {
			frameWidthP = 16;
			frameHeightP = 32;
		}

		if (!player.side && !player.fire) mario_sprite = Mario_Right;
		else if (player.side && !player.fire) mario_sprite = Mario_Left;
		else if (!player.side && player.fire && player.big) mario_sprite = Mario_Fire_Right;
		else if (player.side && player.fire && player.big) mario_sprite = Mario_Fire_Left;

		Rectangle sourceRec = { 0, 0, (float)frameWidthP, (float)frameHeightP };

		static float frameTime = 0.0f;
		static int currentFrame = 0;
		frameTime += GetFrameTime();
		float frameSpeed = 0.1f; //Velocity animation

		//Enemies
		/*--Goomba--*/
		int frameWidthG = 16;
		int frameHeightG = 16;

		if (!goomba.death) {
			int frameWidthG = 16;
			int frameHeightG = 16;
		}
		if (goomba.death) {
			int frameWidthG = 16;
			int frameHeightG = 16;
		}

		Rectangle sourceRec2 = { 0, 0, (float)frameWidthG, (float)frameHeightG };

		/*--Koopa--*/
		int frameWidthK = 16;
		int frameHeightK = 24;
		Rectangle sourceRec3 = { 0, 0, (float)frameWidthK, (float)frameHeightK };

		static float frameTimeE = 0.0f;
		static int currentFrameE = 0;
		frameTimeE += GetFrameTime();
		float frameSpeedE = 0.1f; //Velocity animation

		//Blocks
		/*--?--*/
		int frameWidthInt = 16;
		int frameHeightInt = 16;
		Rectangle sourceRec4 = { 0, 0, (float)frameWidthInt, (float)frameHeightInt };

		static float frameTimeInt = 0.0f;
		static int currentFrameInt = 0;
		frameTimeInt += GetFrameTime();
		float frameSpeedInt = 0.5f; //Velocity animation
		int variant = 1;


		//Animation of Mario
		if (IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && Timer > 0 && player.alive != 0 && !flag.reached && !pipe.enteringPipe1 && !pipe.enteringPipe2 || flag.reached && camera.target.x < 9795 && (player.position.y == 600 || player.position.y == 550)) {
			if (!player.fire) mario_sprite = Mario_Right;
			else if (player.fire) mario_sprite = Mario_Fire_Right;
			player.side = 0;
			if (IsKeyDown(KEY_LEFT_SHIFT) && !flag.reached) {
				frameSpeed = 0.05f; //Increases running speed
			}
			if (IsKeyDown(KEY_DOWN) && Timer > 0 && player.alive != 0 && !flag.reached && !player.big) currentFrame = 0;
			if (frameTime >= frameSpeed && !IsKeyDown(KEY_DOWN)) {
				frameTime = 0.0f;
				currentFrame = (currentFrame + 1) % 4; //Cycling between the 3 walk/run frames
			}
			if (IsKeyDown(KEY_DOWN) && Timer > 0 && player.alive != 0 && !flag.reached && player.big)
			{
				currentFrame = 0;
				sourceRec.x = frameWidthP * 6;
			}
			sourceRec.x = (float)(currentFrame * frameWidthP); //Change frame
		}

		if (IsKeyDown(KEY_LEFT) && !IsKeyDown(KEY_RIGHT) && Timer > 0 && player.alive != 0 && !flag.reached && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			if (!player.fire) mario_sprite = Mario_Left;
			else if (player.fire) mario_sprite = Mario_Fire_Left;
			player.side = 1;
			if (IsKeyDown(KEY_LEFT_SHIFT) && !flag.reached) {
				frameSpeed = 0.05f; //Increases running speed
			}
			if (IsKeyDown(KEY_DOWN) && Timer > 0 && player.alive != 0 && !flag.reached && !player.big) currentFrame = 0;
			if (frameTime >= frameSpeed && !IsKeyDown(KEY_DOWN)) {
				frameTime = 0.0f;
				currentFrame = (currentFrame + 1) % 4; //Cycling between the 3 walk/run frames
			}
			if (IsKeyDown(KEY_DOWN) && Timer > 0 && player.alive != 0 && !flag.reached && player.big)
			{
				currentFrame = 0;
				sourceRec.x = frameWidthP * 6;
			}
			sourceRec.x = (float)(currentFrame * frameWidthP); //Change frame
		}

		if (IsKeyDown(KEY_DOWN) && !IsKeyDown(KEY_RIGHT) && !IsKeyDown(KEY_LEFT) && Timer > 0 && player.alive != 0 && !flag.reached && player.big && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			sourceRec.x = frameWidthP * 6; //Agacharse
		}

		//Animation of Enemies
		if (goomba.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba.death) goomba_sprite = Goomba;
		if (goomba.death) goomba_sprite = Goomba_chafado;

		//Animation of Blocks
		if (player.alive != 0 && Timer > 0) {
			if (currentFrameInt != 0)
			{
				frameSpeedInt = 0.16;
			}
			if (frameTimeInt >= frameSpeedInt) {
				frameTimeInt = 0.0f;
				currentFrameInt = (currentFrameInt + variant) % 4;
			}
			sourceRec4.x = (float)(currentFrameInt * frameWidthInt);
		}

		else {
			currentFrame = 0; //Return to the first frame if still
			sourceRec.x = 0;
		}

		if (!player.canJump && !flag.reached) {
			sourceRec.x = frameWidthP * 5;
		}

		if (Timer <= 0 || player.alive == 0) {
			player.big = 0;
			player.fire = 0;
			sourceRec.x = frameWidthP * 6;
		}
		// COLORES DE BLOQUES
		for (EnvElement block : blocks)
		{
			DrawRectangle(block.rect.x, block.rect.y, block.rect.width, block.rect.height, block.color);
		}

		//Draw all entities, structures and objetcs
		//Tuberias 
		DrawTextureEx(tuberia_s, { (1242), (500) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_m, { (1667), (450) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_b, { (2067), (399) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_b, { (2592), (399) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_s, { (7692), (500) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_s, { (8452), (500) }, 0.0f, 1.2, WHITE);

		//All background
		DrawTextureEx(fondo, { (-113), (72) }, 0.0f, 3, WHITE);
		DrawTextureEx(fondo, { (2190), (72) }, 0.0f, 3, WHITE);
		DrawTextureEx(fondo, { (4490), (72) }, 0.0f, 3, WHITE);
		DrawTextureEx(fondo, { (6790), (72) }, 0.0f, 3, WHITE);
		DrawTextureEx(fondo, { (9090), (72) }, 0.0f, 3, WHITE);

		//Boquete 1
		DrawTextureEx(azul, { (3150), (590) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (3200), (590) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (3150), (640) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (3200), (640) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (3150), (650) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (3200), (650) }, 0.0f, 3.2, WHITE);


		//Boquete 2
		DrawTextureEx(azul, { (4010), (590) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4060), (590) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4110), (590) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (4010), (640) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4060), (640) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4110), (640) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (4010), (650) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4060), (650) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (4110), (650) }, 0.0f, 3.2, WHITE);

		//Boquete 3
		DrawTextureEx(azul, { (7222), (590) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (7270), (590) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (7222), (640) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (7270), (640) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(azul, { (7222), (650) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(azul, { (7270), (650) }, 0.0f, 3.2, WHITE);
		//Bloques ? y ladrillos
		DrawTextureEx(ladrillo, { (850), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (950), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (1050), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (3550), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3650), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (3700), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3750), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3800), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3850), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3900), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (3950), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4000), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4050), (200) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (4250), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4300), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4350), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4400), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (4700), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (4750), (400) }, 0.0f, 3.2, WHITE); // Estrella

		DrawTextureEx(ladrillo, { (5550), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (5700), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (5750), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (5800), (200) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (6000), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (6050), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (6100), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (6150), (200) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(ladrillo, { (6050), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (6100), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(escalera, { (6310), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6360), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6410), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6460), (550) }, 0.0f, 3.2, WHITE);
		/*----*/
		DrawTextureEx(escalera, { (6360), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6410), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6460), (500) }, 0.0f, 3.2, WHITE);
		/*---*/
		DrawTextureEx(escalera, { (6410), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6460), (450) }, 0.0f, 3.2, WHITE);
		/*--*/
		DrawTextureEx(escalera, { (6460), (400) }, 0.0f, 3.2, WHITE);
		/*-*/

		DrawTextureEx(escalera, { (6590), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6640), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6690), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6740), (550) }, 0.0f, 3.2, WHITE);
		/*----*/
		DrawTextureEx(escalera, { (6590), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6640), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6690), (500) }, 0.0f, 3.2, WHITE);
		/*---*/
		DrawTextureEx(escalera, { (6590), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (6640), (450) }, 0.0f, 3.2, WHITE);
		/*--*/
		DrawTextureEx(escalera, { (6590), (400) }, 0.0f, 3.2, WHITE);
		/*-*/

		DrawTextureEx(escalera, { (6970), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7020), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7070), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7120), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7170), (550) }, 0.0f, 3.2, WHITE);

		/*----*/
		DrawTextureEx(escalera, { (7020), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7070), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7120), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7170), (500) }, 0.0f, 3.2, WHITE);

		/*---*/
		DrawTextureEx(escalera, { (7070), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7120), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7170), (450) }, 0.0f, 3.2, WHITE);

		/*--*/
		DrawTextureEx(escalera, { (7120), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7170), (400) }, 0.0f, 3.2, WHITE);

		/*-*/

		DrawTextureEx(escalera, { (7320), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7370), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7420), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7470), (550) }, 0.0f, 3.2, WHITE);
		/*----*/
		DrawTextureEx(escalera, { (7320), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7370), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7420), (500) }, 0.0f, 3.2, WHITE);
		/*---*/
		DrawTextureEx(escalera, { (7320), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (7370), (450) }, 0.0f, 3.2, WHITE);
		/*--*/
		DrawTextureEx(escalera, { (7320), (400) }, 0.0f, 3.2, WHITE);
		/*-*/

		DrawTextureEx(ladrillo, { (7950), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (8000), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(ladrillo, { (8100), (400) }, 0.0f, 3.2, WHITE);

		DrawTextureEx(escalera, { (8570), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8620), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8670), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8720), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8770), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8820), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (550) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (550) }, 0.0f, 3.2, WHITE);
		/*---------*/
		DrawTextureEx(escalera, { (8620), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8670), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8720), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8770), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8820), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (500) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (500) }, 0.0f, 3.2, WHITE);
		/*--------*/
		DrawTextureEx(escalera, { (8670), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8720), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8770), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8820), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (450) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (450) }, 0.0f, 3.2, WHITE);
		/*-------*/
		DrawTextureEx(escalera, { (8720), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8770), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8820), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (400) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (400) }, 0.0f, 3.2, WHITE);
		/*------*/
		DrawTextureEx(escalera, { (8770), (350) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8820), (350) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (350) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (350) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (350) }, 0.0f, 3.2, WHITE);
		/*-----*/
		DrawTextureEx(escalera, { (8820), (300) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8870), (300) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (300) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (300) }, 0.0f, 3.2, WHITE);
		/*----*/
		DrawTextureEx(escalera, { (8870), (250) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8920), (250) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (250) }, 0.0f, 3.2, WHITE);
		/*---*/
		DrawTextureEx(escalera, { (8920), (200) }, 0.0f, 3.2, WHITE);
		DrawTextureEx(escalera, { (8970), (200) }, 0.0f, 3.2, WHITE);
		/*--*/
		DrawTextureEx(escalera, { (8970), (250) }, 0.0f, 3.2, WHITE);
		/*-*/

		DrawTexturePro(bloque_int, sourceRec4, { 650, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 900, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 1000, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 950, 200, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 3600, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 4400, 200, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 5000, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 5125, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 5250, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 5125, 200, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 6050, 200, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 6100, 200, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(bloque_int, sourceRec4, { 8050, 400, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
		// Fondo negro
		DrawTextureEx(negro, { -200, -1500 }, 0.0f, 1500.0f, WHITE);
		//Cueba
	// Suelo
		DrawTextureEx(suelo_cueva, { -112, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { -112, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { -62, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { -62, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { -12, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { -12, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 38, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 38, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 88, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 88, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 138, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 138, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 188, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 188, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 238, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 238, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 288, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 288, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 338, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 338, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 388, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 388, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 438, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 438, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 488, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 488, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 538, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 538, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 588, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 588, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 638, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 638, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 688, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 688, -450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 738, -500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_cueva, { 738, -450 }, 0.0f, 3.2f, WHITE);

		// Paredes (Izquierda)
		DrawTextureEx(ladrillo_cueva, { -112, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -700 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -750 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -800 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -850 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -900 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -950 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1000 }, 0.0f, 3.2f, WHITE);

		// Paredes (Derecha)
		DrawTextureEx(tuberia_cueva, { 579, -700 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -750 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -800 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -850 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -900 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -950 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -1000 }, 0.0f, 3.2f, WHITE);

		// Techo
		DrawTextureEx(ladrillo_cueva, { 88,  -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 138, -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 188, -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 238, -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 288, -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 338, -1000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 388, -1000 }, 0.0f, 3.2f, WHITE);

		// Zona de Monedas
		DrawTextureEx(ladrillo_cueva, { 88,  -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 138, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 188, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 238, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 288, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 338, -550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 388, -550 }, 0.0f, 3.2f, WHITE);

		DrawTextureEx(ladrillo_cueva, { 88,  -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 138, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 188, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 238, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 288, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 338, -600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 388, -600 }, 0.0f, 3.2f, WHITE);

		DrawTextureEx(ladrillo_cueva, { 88,  -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 138, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 188, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 238, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 288, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 338, -650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { 388, -650 }, 0.0f, 3.2f, WHITE);

		DrawTexturePro(goomba_sprite, sourceRec2, { goomba.position.x - 20, goomba.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(Mooshroom, sourceRec2, { mooshroom.position.x - 20, mooshroom.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(FireFlower, sourceRec2, { fireFlower.position.x - 20, fireFlower.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(FireBall, sourceRec2, { fireBall.position.x - 20, fireBall.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(Koopa, sourceRec2, { koopa.position.x - 20, koopa.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(Shell, sourceRec2, { shell.position.x - 20, shell.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);

		//META Y CASTILLO//
		DrawTextureEx(flagTexture, { 9375, flag.position.y - flagTexture.height }, 0, 3, WHITE);
		DrawTextureEx(castle, { (9675), (360) }, 0.0f, 3, WHITE);
		if (!player.big && !player.invencible) {
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}
		else if (!player.big && player.visible && player.invencible) {
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}

		if (player.big && !player.fire && !player.invencible) {
			sourceRec.y = 16;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}
		else if (player.big && !player.fire && player.visible && player.invencible) {
			sourceRec.y = 16;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}

		if (player.big && player.fire) {
			sourceRec.y = 32;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}

		if (player.position.x >= 9795) { //Mario arrived to the flag
			camera.target.x = 9795;
			DrawTextureEx(castle, { (9675), (360) }, 0.0f, 3, WHITE);
			player.big = 0;
			player.fire = 0;
			UnloadTexture(mario_sprite);
		}

		EndMode2D();
		UItest();
	}
};

int main() {
	Game game;
	game.Run();
	return 0;
}