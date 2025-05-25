#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm> 
#include <iostream>

using namespace std;
/*--------------------------------------------------------------------------*/
/*                                LIBRARIES                                 */
/*--------------------------------------------------------------------------*/
#include "Music and Sounds.h"
#include "Textures and Fonts.h"
#include "Structures.h"
#include "MapData.h"

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
int contador = 0;

int Timer;
int Score = 000000;
int Money = 00;
float elapsedTime = 0.0f;
int contmuerte = 0;
int conttiempo = 0;
#define MAX_FLOATING_SCORES 20


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

	//Structs definitions
	Mario player;
	Enemy goomba;
	Enemy koopa;
	Enemy shell;
	PowerUp mooshroom;
	PowerUp fireFlower;
	Projectile fireBall;
	Flag flag;
	Pipe pipe;
	vector<FloatingScore> floatingScores;

public:
	//Initialise the game
	Game() : currentScreen(GameScreen::LOGO), framesCounter(0), player(50, -600), frameCounter(0),
		playFrameCounter(0), currentPlayFrame(0), goomba(1400, 600), koopa(1600, 600), flag(9375, 264), mooshroom(-100, 2000),
		fireFlower(-150, 2000), fireBall(0, 9000), shell(0, 9000) {

		InitWindow(screenWidth, screenHeight, "Super Mario + Screen Manager");
		InitAudioDevice(); // Initialize audio device
		SetTargetFPS(60);

		
		LoadGameSounds();
		LoadGameMusic();
		LoadGameTextures();
		LoadGameTypography();

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
				mooshroom.position = { -110, 1400 };
				mooshroom.active = false;
				mooshroom.side = false;
				fireFlower.active = false;
				fireFlower.position = { -110, 1400 };
				goomba.position = { 1400, 600 };
				koopa.position = { 1600, 600 };
				shell.position = { 0, 1000 };
				Timer = 400;
				Score = 000000;
				Money = 00;
				player.alive = 1;
				player.fire = 0;
				goomba.side = true;
				goomba.death = false;
				goomba.alive = true;
				koopa.death = false;
				koopa.alive = true;
				koopa.side = true;
				shell.death = false;
				shell.activated = false;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;
				mario_sprite = Mario_Right;
				for (EnvElement& block : blocks) {
					block.hit = false;
				}

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
				if (player.position.x < 3950) {
					player.position = { 50, 600 };
				}
				if (player.position.x >= 3950) {
					player.position = { 3850, 600 };
				}
				camera.target.x = 333;
				camera.target.y = 350;
				mooshroom.position = { -110, 1400 };
				mooshroom.active = false;
				mooshroom.side = false;
				fireFlower.active = false;
				fireFlower.position = { -110, 1400 };
				goomba.position = { 1400, 600 };
				koopa.position = { 1600, 600 };
				shell.position = { 0, 1000 };
				Timer = 400;
				player.alive = 1;
				player.fire = 0;
				goomba.side = true;
				goomba.death = false;
				goomba.alive = true;
				koopa.death = false;
				koopa.alive = true;
				koopa.side = true;
				shell.death = false;
				shell.activated = false;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;
				for (EnvElement& block : blocks) {
					block.hit = false;
				}

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
				// Reset player state completely
				player.lifes = 3;
				player.position = { 50, 600 };
				player.alive = 1;
				player.big = 0;
				player.fire = 0;
				player.invencible = false;
				player.invulnerableTimer = 0.0f;
				player.visible = true;
				player.canJump = true;
				player.canJump2 = false;
				player.jumpTime = 0.0f;
				player.speed = { 0, 0 };
				player.side = false;

				// Reset camera
				camera.target.x = 333;
				camera.target.y = 350;

				// Reset game variables
				Timer = 400;
				Score = 000000;
				Money = 00;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;
				hurryMusicPlayed = false;

				// Reset flag
				flag.reached = false;

				// Reset enemies
				goomba.position = { 1400, 600 };
				goomba.side = true;
				goomba.death = false;
				goomba.death2 = false;
				goomba.alive = true;
				goomba.activated = false;

				koopa.position = { 1600, 600 };
				koopa.death = false;
				koopa.death2 = false;
				koopa.alive = true;
				koopa.side = true;
				koopa.activated = false;

				shell.position = { 0, 1000 };
				shell.death = false;
				shell.death2 = false;
				shell.activated = false;
				shell.speed.x = 0;
				shell.alive = true;
				shell.side = true;
				shell.shell = 0;

				// Reset power-ups
				mooshroom.position = { -110, 1400 };
				mooshroom.active = false;
				mooshroom.side = false;
				mooshroom.emerging = false;
				mooshroom.emergeOffset = 0.0f;

				fireFlower.active = false;
				fireFlower.position = { -110, 1400 };
				fireFlower.emerging = false;
				fireFlower.emergeOffset = 0.0f;

				// Reset fireball
				fireBall.position = { 0, 9000 };
				fireBall.active = false;
				fireBall.timer = 0;

				// Reset pipes
				pipe.enteringPipe1 = false;
				pipe.enteringPipe2 = false;

				// Reset all blocks
				for (EnvElement& block : blocks) {
					block.hit = false;
				}

				// Reset mario sprite
				mario_sprite = Mario_Right;

				// Reset frame counters
				framesCounter = 0;
				frameCounter = 0;
				playFrameCounter = 0;
				currentPlayFrame = 0;

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
		bool onGroundPowerUpF = false;
		bool projectileHitObstacleFloor = false;
		bool desactived = false;
		bool goombaDeathInit = false;
		bool koopaDeathInit = false;

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
				PlaySound(sfxFireBall);
				fireBall.position = { player.position.x, player.position.y + -40 };
				fireBall.timer = 0;
				fireBall.active = true;
				projectileHitObstacleFloor = true;
				fireBall.speed.y = 300.0f * deltaTime;
				projectileHitObstacleFloor = false;
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
		mooshroom.speed.x = 1.0f;
		if (mooshroom.active && !mooshroom.emerging && player.alive != 0 && mooshroom.side) {
			mooshroom.position.x += -120 * deltaTime;
		}

		if (mooshroom.active && !mooshroom.emerging && player.alive != 0 && !mooshroom.side) {
			mooshroom.position.x += 120 * deltaTime;
		}

		if (mooshroom.emerging) {
			float emergeSpeed = 30.0f;  // píxeles por segundo
			mooshroom.emergeOffset += emergeSpeed * deltaTime;
			mooshroom.position.y -= emergeSpeed * deltaTime;

			if (mooshroom.emergeOffset >= mooshroom.maxEmergeOffset) {
				mooshroom.emerging = false;
				mooshroom.active = true;
				mooshroom.position.y = mooshroom.position.y + mooshroom.emergeOffset - mooshroom.maxEmergeOffset; // asegura corrección de posición
			}
		}

		//FIRE FLOWER
		if (player.position.x >= fireFlower.position.x && player.alive != 0) {
			fireFlower.active = true;
		}
		if (fireFlower.emerging) {
			float emergeSpeed = 30.0f;  // píxeles por segundo
			fireFlower.emergeOffset += emergeSpeed * deltaTime;
			fireFlower.position.y -= emergeSpeed * deltaTime;

			if (fireFlower.emergeOffset >= fireFlower.maxEmergeOffset) {
				fireFlower.emerging = false;
				fireFlower.active = true;
				fireFlower.position.y = fireFlower.position.y + fireFlower.emergeOffset - fireFlower.maxEmergeOffset; // asegura corrección de posición
			}
		}

		//GOOMBA
		if (player.position.x - goomba.position.x <= -200 && !goomba.death && !goomba.death2 && player.alive != 0 && Timer > 0) {
			goomba.activated = true;
		}

		goomba.speed.x = 1.0f;
		if (goomba.activated && !goomba.death && !goomba.death2 && player.alive != 0 && goomba.side && Timer > 0) {
			goomba.position.x += -120 * deltaTime;
		}
		if (goomba.activated && !goomba.death && !goomba.death2 && player.alive != 0 && !goomba.side && Timer > 0) {
			goomba.position.x += 120 * deltaTime;
		}

		if (goomba.death) {
			if (player.position.y >= goomba.position.y) {
				goomba.position.y = 1000;
			}
		}
		if (goomba.death2) {
			goomba.speed.y += GRAVITY * deltaTime;
			goomba.position.x += goomba.speed.x * deltaTime;
			goomba.position.y += goomba.speed.y * deltaTime;
			if (goomba.position.y >= goomba.goomba_hitbox.y + 100) {
				goombaDeathInit = false;
			}
		}

		if (goomba.position.y >= 1000) {
			goomba.alive = false;
		}

		//Koopa
		if (player.position.x - koopa.position.x <= -200 && !koopa.death && !koopa.death2 && player.alive != 0 && Timer > 0) {
			koopa.activated = true;
		}
		koopa.speed.x = 1.0f;
		if (koopa.activated && !koopa.death && !koopa.death2 && player.alive != 0 && koopa.side && Timer > 0) {
			koopa.position.x += -120 * deltaTime;
		}
		if (koopa.activated && !koopa.death && !koopa.death2 && player.alive != 0 && !koopa.side && Timer > 0) {
			koopa.position.x += 120 * deltaTime;
		}

		if (koopa.death) {
			koopa.position.y = 1000;
		}
		if (koopa.death2) {
			shell.position = koopa.position;
			koopa.alive = false;
			
			koopa.speed.y += GRAVITY * deltaTime;
			koopa.position.x += koopa.speed.x * deltaTime;
			koopa.position.y += koopa.speed.y * deltaTime;
			if (koopa.position.y >= koopa.goomba_hitbox.y + 100) {
				koopaDeathInit = false;
			}
		}

		if (koopa.position.y >= 1000) {
			koopa.alive = false;
		}

		//SHELL
		if (player.position.x >= shell.position.x && player.alive != 0) {
			shell.activated = true;
		}

		if (shell.activated && !shell.death && player.alive != 0 && shell.side && Timer > 0) {
			shell.position.x += shell.speed.x * -deltaTime;
		}
		if (shell.activated && !shell.death && player.alive != 0 && !shell.side && Timer > 0) {
			shell.position.x += shell.speed.x * deltaTime;
		}
		
		if (shell.death2) {
			shell.speed.y += GRAVITY * deltaTime;
			shell.position.x += shell.speed.x * deltaTime;
			shell.position.y += shell.speed.y * deltaTime;
		}

		//--------Colisiones de Mario--------\\

		//Suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y >= player.position.y
				&& block.rect.y <= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				hitObstacleFloor = true;
				player.speed.y = 0.0f;
				player.position.y = block.rect.y;
			}
		}

		//Techo
		for (EnvElement& block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 && player.speed.y < 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 10
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2 >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				player.speed.y = 0.0f;
				player.position.y = block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2;

				if (ColorToInt(block.color) == ColorToInt(RED) && !block.hit) { //Moneda
					PlaySound(sfxCoin_Block);
					Money++;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(BROWN) && !block.hit) { //Flor de fuego
					PlaySound(sfxPowerUpAppear);
					fireFlower.position = { block.rect.x + 22, block.rect.y + 35 };
					fireFlower.emerging = true;
					fireFlower.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(PINK) && !block.hit) { //Estrella
					PlaySound(sfxPowerUpAppear);
					//mooshroom.side = false;
					//mooshroom.position = { block.rect.x + 22, block.rect.y + 35 };
					//mooshroom.emerging = true;
					//mooshroom.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(MAGENTA) && !block.hit && !desactived) { //Moneda repetida
					PlaySound(sfxCoin_Block);
					Money++;
					contador++;
					desactived = true;
					if (contador == 8) {
						block.hit = true;
						contador = 0;
					}
				}
			}
			else if (!player.big && Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				player.speed.y = 0.0f;
				player.position.y = block.rect.y + block.rect.height + block.rect.height;
				if (ColorToInt(block.color) == ColorToInt(RED) && !block.hit) { //Moneda
					PlaySound(sfxCoin_Block);
					Money++;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(BROWN) && !block.hit) { //Seta
					PlaySound(sfxPowerUpAppear);
					mooshroom.side = false;
					mooshroom.position = { block.rect.x + 22, block.rect.y + 35 };
					mooshroom.emerging = true;
					mooshroom.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(PINK) && !block.hit) { //Estrella
					PlaySound(sfxPowerUpAppear);
					//mooshroom.side = false;
					//mooshroom.position = { block.rect.x + 22, block.rect.y + 35 };
					//mooshroom.emerging = true;
					//mooshroom.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(MAGENTA) && !block.hit && !desactived) { //Moneda repetida
					PlaySound(sfxCoin_Block);
					Money++;
					contador++;
					desactived = true;
					if (contador == 8) {
						block.hit = true;
						contador = 0;
					}
				}				
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				hitObstacleWall = true;
				player.speed.x = 0;
				player.position.x = block.rect.x + block.rect.width + player.mario_hitbox.width - 7;
			}
		}

		if (!hitObstacleFloor) {
			player.position.y += player.speed.y * deltaTime;
			if (player.speed.y > 0) {
				player.speed.y += GRAVITY * 3.0f * deltaTime; //Increase gravity in fall
			}
			else {
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
		if (goomba.alive && !goomba.death && !goomba.death2 && player.position.x + player.mario_hitbox.width + 10 >= goomba.position.x &&
			player.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= goomba.position.y && player.position.y <= goomba.position.y + goomba.goomba_hitbox.height)
		{
			if (player.position.y + player.mario_hitbox.height <= goomba.position.y && player.alive) {
				PlaySound(sfxStomp);
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

			}
			else if (player.big && !player.fire && !player.invencible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (!player.big && !player.fire && !player.invencible) {
				player.alive = 0;
			}
		}

		if (koopa.alive && !koopa.death && !koopa.death2 && player.position.x + player.mario_hitbox.width + 10 >= koopa.position.x &&
			player.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= koopa.position.y && player.position.y <= koopa.position.y + koopa.goomba_hitbox.height)
		{
			if (player.position.y + player.mario_hitbox.height <= koopa.position.y && player.alive) {
				PlaySound(sfxStomp);
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
			}
			else if (player.big && !player.fire && !player.invencible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (!player.big && !player.fire && !player.invencible) {
				player.alive = 0;
			}
		}

		if (shell.alive && !shell.death && player.position.x + player.mario_hitbox.width + 10 >= shell.position.x &&
			player.position.x <= shell.position.x + shell.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= shell.position.y && player.position.y <= shell.position.y + shell.goomba_hitbox.height)
		{
			if (player.position.y + player.mario_hitbox.height <= shell.position.y && player.alive) {
				shell.speed.x = 0;
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (!player.invencible && shell.speed.x <= 0 && player.side) {
				shell.speed.x = 500;
				shell.side = true;
			}
			else if (!player.invencible && shell.speed.x <= 0 && !player.side) {
				shell.speed.x = 500;
				shell.side = false;
			}
			else if (player.big && player.fire && !player.invencible && shell.speed.x > 0) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (player.big && !player.fire && !player.invencible && shell.speed.x > 0) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
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
		if (shell.speed.x != 0) {
			shell.shell += GetFrameTime();
		}

		//Con bola de fuego
		if (goomba.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= goomba.position.x && fireBall.active &&
			fireBall.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= goomba.position.y && fireBall.position.y <= goomba.position.y + goomba.goomba_hitbox.height)
		{
			PlaySound(sfxKick);
			Score += 100;
			goomba.death2 = true;
			fireBall.active = false;
			goombaDeathInit = true;
			fireBall.position.y = 2000;
			goomba.speed.x = goomba.side ? -8.0f : 8.0f;
			goomba.speed.y = -20.0f;
		}

		if (koopa.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= koopa.position.x && fireBall.active &&
			fireBall.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= koopa.position.y && fireBall.position.y <= koopa.position.y + koopa.goomba_hitbox.height)
		{
			PlaySound(sfxKick);
			Score += 200;
			koopa.death2 = true;
			fireBall.active = false;
			koopaDeathInit = true;
			fireBall.position.y = 2000;
			koopa.speed.x = koopa.side ? -8.0f : 8.0f;
			koopa.speed.y = -20.0f;
		}

		//Con el suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && goomba.activated && !goomba.death2
				&& block.rect.x <= goomba.position.x + goomba.goomba_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= goomba.position.x
				&& block.rect.y + block.rect.height >= goomba.position.y
				&& block.rect.y <= goomba.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundEnemy = true;
				goomba.speed.y = 0.0f;
				goomba.position.y = block.rect.y;
			}
		}

		if (!onGroundEnemy && !goomba.death2 && player.alive && Timer > 0 && !goombaDeathInit) {
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundKoopa = true;
				koopa.speed.y = 0.0f;
				koopa.position.y = block.rect.y;
			}
		}

		if (!onGroundKoopa && !koopa.death2 && player.alive && Timer > 0 && !koopaDeathInit) {
			koopa.position.y += (GRAVITY - 300) * deltaTime;
			if (koopa.position.y > 0)
			{
				koopa.position.y += (GRAVITY - 300) * 2.0f * deltaTime;
			}
			else
			{
				koopa.position.y += (GRAVITY - 300) * deltaTime;
			}
		}

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && shell.activated
				&& block.rect.x <= shell.position.x + shell.goomba_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= shell.position.x
				&& block.rect.y + block.rect.height >= shell.position.y
				&& block.rect.y <= shell.position.y
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundShell = true;
				shell.speed.y = 0.0f;
				shell.position.y = block.rect.y;
			}
		}

		if (!onGroundShell && !shell.death2 && player.alive && Timer > 0) {
			shell.position.y += (GRAVITY - 300) * deltaTime;
			if (shell.position.y > 0)
			{
				shell.position.y += (GRAVITY - 300) * 2.0f * deltaTime;
			}
			else
			{
				shell.position.y += (GRAVITY - 300) * deltaTime;
			}
		}

		//Los lados Goomba
		float nextXE = goomba.position.x + goomba.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && goomba.alive &&
				goomba.activated && !goomba.side &&
				goomba.position.y > block.rect.y &&
				goomba.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				goomba.position.x - 10 <= block.rect.x &&
				(nextXE + goomba.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				goomba.side = false;
			}
		}

		//Los lados Koopa
		float nextK = koopa.position.x + koopa.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && koopa.alive &&
				koopa.activated && !koopa.side &&
				koopa.position.y > block.rect.y &&
				koopa.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				koopa.position.x - 10 <= block.rect.x &&
				(nextK + koopa.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
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
				(nextK) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				koopa.side = false;
			}
		}
		//Los lados Shell
		float nextshell = shell.position.x + shell.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && shell.alive &&
				shell.activated && !shell.side &&
				shell.position.y > block.rect.y &&
				shell.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				shell.position.x - 10 <= block.rect.x &&
				(nextshell + shell.goomba_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
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
				shell.position.x + 10 >(block.rect.x + block.rect.width) &&
				(nextshell) <= (block.rect.x + block.rect.width + 30)
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				shell.side = false;
			}
		}
		//--------Colisiones de Power-Ups--------\\

		//Con Mario
		//Mushroom
		if (mooshroom.active && player.position.x + player.mario_hitbox.width + 10 >= mooshroom.position.x &&
			player.position.x <= mooshroom.position.x + mooshroom.powerup_hitbox.width + 20 &&
			player.position.y >= mooshroom.position.y && player.position.y <= mooshroom.position.y + mooshroom.powerup_hitbox.height &&
			!mooshroom.emerging)
		{
			PlaySound(sfxPowerUpTaken);
			if (!player.big) player.big = true;
			mooshroom.active = false;
			Score += 1000;
			mooshroom.position.y = 1000;
		}

		//Fire Flower
		if (fireFlower.active && player.position.x + player.mario_hitbox.width + 10 >= fireFlower.position.x &&
			player.position.x <= fireFlower.position.x + fireFlower.powerup_hitbox.width + 20 &&
			player.position.y >= fireFlower.position.y - 10 && player.position.y <= fireFlower.position.y + fireFlower.powerup_hitbox.height &&
			!fireFlower.emerging)
		{
			PlaySound(sfxPowerUpTaken);
			if (!player.fire && player.big) {
				player.fire = true;
				fireFlower.active = false;
				Score += 1000;
				for (const auto& score : floatingScores) {
					DrawTextEx(marioFont, TextFormat("%d", 1000), { player.position.x, player.position.y }, 9, 1, WHITE);
				}
				for (auto& score : floatingScores) {
					score.y -= 0.5f; // Movimiento hacia arriba
					score.lifetime--;
				}
				fireFlower.position.y = 1000;
			}
			else {
				player.big = true;
				fireFlower.active = false;
				Score += 1000;
				fireFlower.position.y = 1000;
			}
		}

		//Con el suelo
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= mooshroom.position.x + mooshroom.powerup_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= mooshroom.position.x
				&& block.rect.y + block.rect.height >= mooshroom.position.y
				&& block.rect.y <= mooshroom.position.y && !mooshroom.emerging
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundPowerUp = true;
				mooshroom.speed.y = 0.0f;
				mooshroom.position.y = block.rect.y;
			}
		}
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= fireFlower.position.x + fireFlower.powerup_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= fireFlower.position.x
				&& block.rect.y + block.rect.height >= fireFlower.position.y
				&& block.rect.y <= fireFlower.position.y && !fireFlower.emerging
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundPowerUpF = true;
				fireFlower.speed.y = 0.0f;
				fireFlower.position.y = block.rect.y;
			}
		}

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 && fireBall.active
				&& block.rect.x <= fireBall.position.x + fireBall.projectile_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= fireBall.position.x
				&& block.rect.y + block.rect.height >= fireBall.position.y
				&& block.rect.y <= fireBall.position.y && ColorToInt(block.color) != ColorToInt(BLUE)
				&& ColorToInt(block.color) != ColorToInt(YELLOW)) {
				projectileHitObstacleFloor = true;
				fireBall.speed.y = 300.0f * deltaTime;
				projectileHitObstacleFloor = false;

				fireBall.position.y = block.rect.y + 5;
				fireBall.position.y = block.rect.y - 50;

			}
		}
		if (!projectileHitObstacleFloor && player.alive && Timer > 0 && fireBall.active) {
			fireBall.position.y += (10) * deltaTime;
			if (fireBall.position.y > 0)
			{
				fireBall.position.y += (10) * 2.0f * deltaTime;
			}
			else
			{
				fireBall.position.y += (10) * deltaTime;
			}
		}

		if (!onGroundPowerUp && player.alive && Timer > 0 && !mooshroom.emerging) {
			mooshroom.position.y += (GRAVITY - 300) * deltaTime;
			if (mooshroom.position.y > 0)
			{
				mooshroom.position.y += (GRAVITY - 300) * 2.0f * deltaTime;
			}
			else
			{
				mooshroom.position.y += (GRAVITY - 300) * deltaTime;
			}
		}
		if (!onGroundPowerUpF && player.alive && Timer > 0 && !fireFlower.emerging) {
			fireFlower.position.y += (GRAVITY - 300) * deltaTime;
			if (fireFlower.position.y > 0)
			{
				fireFlower.position.y += (GRAVITY - 300) * 2.0f * deltaTime;
			}
			else
			{
				fireFlower.position.y += (GRAVITY - 300) * deltaTime;
			}
		}

		//Los lados Mushroom
		float nextXP = mooshroom.position.x + mooshroom.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				mooshroom.active && !mooshroom.side &&
				mooshroom.position.y > block.rect.y &&
				mooshroom.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				mooshroom.position.x - 10 <= block.rect.x &&
				(nextXP + mooshroom.powerup_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
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
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				mooshroom.side = false;
			}
		}

		//Los lados bola de fuego
		float nextF = fireBall.position.x + fireBall.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				fireBall.active && fireBall.position.y > block.rect.y + 7 &&
				fireBall.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				fireBall.position.x - 10 <= block.rect.x &&
				(nextF + fireBall.projectile_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				PlaySound(sfxFireBallWall);
				fireBall.active = false;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				fireBall.active && fireBall.position.y > block.rect.y + 7 &&
				fireBall.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				fireBall.position.x >= (block.rect.x + block.rect.width) &&
				(nextF) <= (block.rect.x + block.rect.width + 12)
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				PlaySound(sfxFireBallWall);
				fireBall.active = false;
			}
		}
		
		//Moneda
		for (EnvElement& block : blocks) { //Por arriba
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y >= player.position.y
				&& block.rect.y <= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
		}

		for (EnvElement& block : blocks) { //Por abajo
			if (player.big && Timer > 0 && player.alive != 0 && player.speed.y < 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 10
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2 >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
			else if (!player.big && Timer > 0 && player.alive != 0
				&& block.rect.x <= player.position.x + player.mario_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= player.position.x
				&& block.rect.y + block.rect.height + block.rect.height <= player.position.y
				&& block.rect.y + block.rect.height + block.rect.height >= player.position.y + player.speed.y * deltaTime
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
		}

		//--- COLISIÓN POR LA DERECHA (Mario viene de la izquierda) ---
		for (EnvElement& block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x > 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height + block.rect.height - 4) &&
				player.position.x - 10 <= block.rect.x &&
				(nextX + player.mario_hitbox.width) >= block.rect.x
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
			else if (!player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x > 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				player.position.x - 10 <= block.rect.x &&
				(nextX + player.mario_hitbox.width) >= block.rect.x
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
		}

		//--- COLISIÓN POR LA IZQUIERDA (Mario viene de la derecha) ---
		for (EnvElement& block : blocks) {
			if (player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x < 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height + block.rect.height - 4) &&
				player.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextX) <= (block.rect.x + block.rect.width + 14)
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
			else if (!player.big && Timer > 0 && player.alive != 0 &&
				player.speed.x < 0 &&
				player.position.y > block.rect.y &&
				player.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				player.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextX) <= (block.rect.x + block.rect.width + 14)
				&& ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit)
			{
				PlaySound(sfxCoin_Block);
				Money++;
				
				block.hit = true;
			}
		}
		
		//--------Colision Tuberia--------\\
			// PIPES 
		if (!pipe.enteringPipe1 && player.position.x + player.mario_hitbox.width - 5 >= pipe.pipe1.x && player.position.x <= pipe.pipe1.x + pipe.pipe1.width + 10
			&& player.position.y == 400 && IsKeyDown(KEY_DOWN)) {
			pipe.enteringPipe1 = true;
		}

		if (pipe.enteringPipe1) {
			PlaySound(sfxPipe);
			player.position.y += 0.1;

			if (player.position.y >= 450) {
				// Por ejemplo, cambia de zona
				player.position.x = -10;
				player.position.y = -1950;

				camera.target.x = 333;
				camera.target.y = -1750;
				pipe.enteringPipe1 = false;
			}
		}

		if (!pipe.enteringPipe2 && player.position.x >= pipe.pipe2.x - 20 && player.position.y >= pipe.pipe2.y
			&& player.position.y < -800 && IsKeyDown(KEY_RIGHT) && player.position.x < 700) {
			DrawTextureEx(tuberia_cueva, { 579, -1700 }, 0.0f, 3.2f, WHITE);
			PlaySound(sfxPipe);
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
			mooshroom.position = { -110, 1400 };
			mooshroom.active = false;
			mooshroom.side = false;
			fireFlower.active = false;
			fireFlower.position = { -110, 1400 };
			goomba.position = { 1400, 600 };
			koopa.position = { 1600, 600 };
			shell.position = { 0, 1000 };
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
			goomba.death2 = false;
			goomba.alive = true;
			koopa.death = false;
			koopa.death2 = false;
			koopa.side = true;
			koopa.alive = true;
			shell.death = false;
			shell.activated = false;
			elapsedTime = 0.0f;
			contmuerte = 0;
			for (EnvElement& block : blocks) {
				block.hit = false;
			}
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
		}

		if (elapsedTime >= 1.0f && Timer > 0 && player.alive == 1 && !flag.reached) {
			Timer--;
			elapsedTime = 0.0f;
		}

		if (IsKeyPressed(KEY_B) && Timer > 0 && player.alive == 1 && !flag.reached) {
			player.big = 1;
		}
		if (IsKeyPressed(KEY_G) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Goomba
			goomba.death = false;
			goomba.death2 = false;
			goomba.alive = true;
			goomba.side = true;
			goomba.position.x = player.position.x + 200;
			goomba.position.y = player.position.y;
		}
		if (IsKeyPressed(KEY_K) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Koopa
			koopa.death = false;
			koopa.death2 = false;
			koopa.alive = true;
			koopa.side = true;
			koopa.position.x = player.position.x + 200;
			koopa.position.y = player.position.y;
		}
		if (IsKeyPressed(KEY_M) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Seta
			mooshroom.active = true;
			mooshroom.side = true;
			mooshroom.position.x = player.position.x + 200;
			mooshroom.position.y = player.position.y;
		}
		if (IsKeyPressed(KEY_F) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Flor de Fuego
			fireFlower.active = true;
			fireFlower.position.x = player.position.x + 200;
			fireFlower.position.y = player.position.y;
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
		UnloadSound(sfxPowerUpTaken);
		UnloadSound(sfxFireBall);
		UnloadSound(sfxKick);
		UnloadSound(sfxPowerUpAppear);
		UnloadSound(sfxStomp);
		UnloadSound(sfxCoin_Block);
		UnloadSound(sfxPipe);
		UnloadSound(sfxGameOver);
		UnloadSound(sfxFlagpole);
		UnloadSound(sfxDeath);
		UnloadSound(sfxCompleted);

		CloseAudioDevice();
	}

	void DrawGameplay() {
		BeginMode2D(camera);
		ClearBackground(BLUE);
		DrawTextureEx(negro, { -200, -2500 }, 0.0f, 1000.0f, WHITE);

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

		if (!goomba.death && !goomba.death2) {
			int frameWidthG = 16;
			int frameHeightG = 16;
		}
		if (goomba.death || goomba.death2) {
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
		if (!goomba.death && !goomba.death2) goomba_sprite = Goomba;
		if (goomba.death) goomba_sprite = Goomba_chafado;
		if (goomba.death2) goomba_sprite = Goomba_invertido;

		if (koopa.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec3.x = (float)(currentFrameE * frameWidthG);
		}

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

		//for (EnvElement block : blocks) //Para ver la hitbox de cada rectangulo
		//{
		//	DrawRectangle(block.rect.x, block.rect.y, block.rect.width, block.rect.height, block.color);
		//}

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

		//All voids
		for (const EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(BLUE)) {
				DrawTextureEx(azul, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All bricks
		for (const EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(GREEN)) {
				DrawTextureEx(ladrillo, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All stairs
		for (const EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(GRAY)) {
				DrawTextureEx(escalera, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All blocks ? and star
		for (const EnvElement& block : blocks) {
			Texture2D textura = block.hit ? bloque_int_a : bloque_int;
			Texture2D textura2 = block.hit ? bloque_int_a : ladrillo;
			if (ColorToInt(block.color) == ColorToInt(RED)) {
				DrawTexturePro(textura, sourceRec4, { block.rect.x, block.rect.y, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
			}
			if (ColorToInt(block.color) == ColorToInt(BROWN)) {
				DrawTexturePro(textura, sourceRec4, { block.rect.x, block.rect.y, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
			}
			if (ColorToInt(block.color) == ColorToInt(MAGENTA)) {
				DrawTextureEx(textura2, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
			if (ColorToInt(block.color) == ColorToInt(PINK)) {
				DrawTextureEx(textura2, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All cave_suelo	
		for (const EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(BLACK)) {
				DrawTextureEx(suelo_cueva, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All Cave_walls
		for (const EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(LIGHTGRAY)) {
				DrawTextureEx(ladrillo_cueva, { block.rect.x, block.rect.y }, 0.0f, 3.2f, WHITE);
			}
		}

		//All Coins
		for (EnvElement& block : blocks) {
			if (ColorToInt(block.color) == ColorToInt(YELLOW) && !block.hit) {
				DrawTexturePro(money, sourceRec4, { block.rect.x - 8, block.rect.y, sourceRec4.width * 3.2f, sourceRec4.height * 3.2f }, { 0, 0 }, 0, WHITE);
			}
			else if (ColorToInt(block.color) == ColorToInt(YELLOW) && block.hit) {
				block.rect.y = block.rect.y - 1000;
			}
		}

		// Paredes Cueva (Derecha)
		
		
		DrawTextureEx(tubo, { 688, -1750 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -1800 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -1850 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -1900 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -1950 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tubo, { 688, -2000 }, 0.0f, 3.2f, WHITE);

		DrawTexturePro(goomba_sprite, sourceRec2, { goomba.position.x - 20, goomba.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(Mooshroom, sourceRec2, { mooshroom.position.x - 20, mooshroom.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(FireFlower, sourceRec2, { fireFlower.position.x - 20, fireFlower.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		if (fireBall.active) {
			DrawTexturePro(FireBall, sourceRec2, { fireBall.position.x - 20, fireBall.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		}
		else if (!fireBall.active) {
			DrawTextureEx(FireBallTick1, { fireBall.position.x - 20, fireBall.position.y - 48 }, 0.0f, 3.2f, WHITE);
			fireBall.timer += 2.0f * 0.8;
			if (fireBall.timer >= 5.0f) {
				DrawTextureEx(FireBallTick2, { fireBall.position.x - 20, fireBall.position.y - 48 }, 0.0f, 3.2f, WHITE);
				if (fireBall.timer >= 10.0f) {
					fireBall.position.y = 2000;
					fireBall.timer = 0;
				}
			}
		}
		if (!koopa.death && !koopa.death2) {
			DrawTexturePro(Koopa, sourceRec3, { koopa.position.x - 20, koopa.position.y - 72, sourceRec3.width * 3, sourceRec3.height * 3 }, { 0,0 }, 0, WHITE);
		}
		if (koopa.death || koopa.death2) {
			DrawTexturePro(Shell, sourceRec2, { shell.position.x - 20, shell.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		}

		//META Y CASTILLO//
		DrawTextureEx(flagTexture, { 9375, flag.position.y - flagTexture.height }, 0, 3, WHITE);
		DrawTextureEx(castle, { (9675), (360) }, 0.0f, 3, WHITE);

		//Mario
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
		DrawTextureEx(tuberia_b, { (2592), (399) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(tuberia_cueva, { 579, -1700 }, 0.0f, 3.2f, WHITE);
		


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