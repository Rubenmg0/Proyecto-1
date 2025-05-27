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
	Enemy goomba1;
	Enemy goomba2;
	Enemy goomba3;
	Enemy goomba4;
	Enemy goomba5;
	Enemy goomba6;
	Enemy goomba7;
	Enemy goomba8;
	Enemy goomba9;
	Enemy goomba10;
	Enemy goomba11;
	Enemy goomba12;
	Enemy goomba13;
	Enemy goomba14;
	Enemy goomba15;
	Enemy goomba16;
	Enemy koopa;
	Enemy shell;
	PowerUp mooshroom;
	PowerUp fireFlower;
	PowerUp star;
	Projectile fireBall;
	Flag flag;
	Flag bandera;
	Flag banderinn;
	Pipe pipe;
	CoinPop moneda;

	FloatingScore scores[MAX_FLOATING_SCORES];

	vector<EnvElement> initialBlocks = CreateBlocks();
	vector<EnvElement> blocks = initialBlocks;

	void InitFloatingScores() {
		for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
			scores[i].active = false;
		}
	}

public:
	//Initialise the game
	Game() : currentScreen(GameScreen::LOGO), framesCounter(0), player(50, -600), frameCounter(0),
		playFrameCounter(0), currentPlayFrame(0), goomba1(950, 600), goomba2(1400, 600), goomba3(1400, 600), goomba4(1400, 600), goomba5(1400, 600),
		goomba6(1400, 600), goomba7(1400, 600),goomba8(1400, 600), goomba9(1400, 600), goomba10(1400, 600), goomba11(1400, 600), goomba12(1400, 600), 
		goomba13(1400, 600), goomba14(1400, 600), goomba15(1400, 600), goomba16(1400, 600), koopa(1600, 600), flag(9375, 264), mooshroom(-100, 2000),
		fireFlower(-150, 2000), star(-125, 2000), fireBall(0, 9000), shell(0, 9000), banderinn(9775, 320), bandera(9350, flag.position.y - flagTexture.height + 5){

		InitWindow(screenWidth, screenHeight, "Super Mario + Screen Manager");
		InitAudioDevice(); // Initialize audio device
		InitFloatingScores();
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
	//Función Texto
	void SpawnFloatingScore(Vector2 pos, int value) {
		for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
			if (!scores[i].active) {
				scores[i].position = pos;
				scores[i].value = value;
				scores[i].lifetime = 1.0f;
				scores[i].speedY = -30.0f;
				scores[i].active = true;
				break;
			}
		}
	}

	//funciones Goomba
	void Goomba_Activation(Enemy& goomba, Mario& player)
	{
		if (player.position.x >= goomba.position.x - 450 && !goomba.death && !goomba.death2 && player.alive != 0 && Timer > 0) {
			goomba.activated = true;
		}
		if ((goomba.activated && goomba.position.x <= camera.target.x - 480) || (goomba.activated && goomba.position.x >= camera.target.x + 550)) {
			goomba.activated = false;
		}
	}

	void Goomba_Movement(Enemy& goomba, Mario& player, float& deltaTime)
	{
		if (goomba.activated && !goomba.death && !goomba.death2 && player.alive != 0 && goomba.side && Timer > 0) {
			goomba.position.x += -120 * deltaTime;
		}
		if (goomba.activated && !goomba.death && !goomba.death2 && player.alive != 0 && !goomba.side && Timer > 0) {
			goomba.position.x += 120 * deltaTime;
		}
	}

	void Goomba_Death(Enemy& goomba, Mario& player) {
		if (goomba.death) {
			if (player.position.y >= goomba.position.y) {
				goomba.position.y = 1000;
			}
		}
	}
	void Goomba_Death2(Enemy& goomba, float& deltaTime)
	{
		if (goomba.death2) {
			goomba.speed.y += GRAVITY * deltaTime;
			goomba.position.x += goomba.speed.x * deltaTime;
			goomba.position.y += goomba.speed.y * deltaTime;
		}
	}

	void Goomba_InTheMoon(Enemy& goomba)
	{
		if (goomba.position.y >= 1000) {
			goomba.alive = false;
		}
	}

	void Goomba_MarioCollision(Enemy& goomba, Mario& player)
	{
		if (goomba.alive && !goomba.death && !goomba.death2 && player.position.x + player.mario_hitbox.width + 10 >= goomba.position.x &&
			player.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= goomba.position.y && player.position.y <= goomba.position.y + goomba.goomba_hitbox.height)
		{
			if (player.position.y + player.mario_hitbox.height <= goomba.position.y && player.alive && !star.Invincible) {
				PlaySound(sfxStomp);
				goomba.death = true;
				Score += 100;
				SpawnFloatingScore({ goomba.position.x - 4, goomba.position.y - 100 }, 100);
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (player.big && player.fire && !player.invencible && !star.Invincible) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;

			}
			else if (player.big && !player.fire && !player.invencible && !star.Invincible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (!player.big && !player.fire && !player.invencible && !star.Invincible) {
				player.alive = 0;
			}
			else if (star.Invincible) {
				PlaySound(sfxKick);
				goomba.death2 = true;
				Score += 100;
				SpawnFloatingScore({ goomba.position.x - 4, goomba.position.y - 100 }, 100);
			}
		}
	}

	void Goomba_Fuego(Enemy& goomba, Projectile& fireBall) {
		if (goomba.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= goomba.position.x && fireBall.active &&
			fireBall.position.x <= goomba.position.x + goomba.goomba_hitbox.width + 20 && !goomba.death2 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= goomba.position.y && fireBall.position.y <= goomba.position.y + goomba.goomba_hitbox.height)
		{
			PlaySound(sfxKick);
			Score += 100;
			SpawnFloatingScore({ goomba.position.x - 4, goomba.position.y - 100 }, 100);
			goomba.death2 = true;
			fireBall.active = false;
			fireBall.position.y = 2000;
			goomba.speed.x = goomba.side ? -8.0f : 8.0f;
			goomba.speed.y = -20.0f;
		}
	}

	void Goomba_Suelo(Enemy& goomba, Mario& player, EnvElement& block, bool& onGroundEnemy)
	{
		if (Timer > 0 && player.alive != 0 && !goomba.death2
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

	void Goomba_fall(Enemy& goomba, Mario& player, bool& onGroundEnemy, float& deltaTime)
	{
		if (!onGroundEnemy && !goomba.death2 && player.alive && Timer > 0) {
			goomba.position.y += (GRAVITY - 300) * deltaTime;
			if (goomba.position.y > 0)
			{
				goomba.position.y += (GRAVITY - 400) * 2.0f * deltaTime; //Increase gravity in fall
			}
			else
			{
				goomba.position.y += (GRAVITY - 300) * deltaTime; //Normal upward gravity
			}
		}
	}

	void Goomba_Lados(Enemy& goomba, Mario& player, EnvElement& block, float& nextXE)
	{
		//Derecha
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
		//Izquierda
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

	void Goomba_Goomba(Enemy& goomba, Mario& player, Enemy& goomba2, float& nextXE)
	{
		//Derecha
		if (Timer > 0 && player.alive != 0 && goomba.alive &&
			goomba.activated && !goomba.side && goomba.position.y >= 500 &&
			goomba.position.y >= goomba2.position.y &&
			goomba.position.y < (goomba2.position.y + goomba2.goomba_hitbox.height) &&
			goomba.position.x - 5 <= goomba2.position.x &&
			(nextXE + goomba.goomba_hitbox.width) >= goomba2.position.x - 20)
		{
			goomba.side = true;
			goomba2.side = false;
		}
		//Izquierda
		if (Timer > 0 && player.alive != 0 &&
			goomba.activated && goomba2.side && goomba.position.y >= 500 &&
			goomba.position.y >= goomba2.position.y &&
			goomba.position.y < (goomba2.position.y + goomba2.goomba_hitbox.height) &&
			goomba.position.x + 5 >= (goomba2.position.x + goomba2.goomba_hitbox.width) &&
			(nextXE) <= (goomba2.position.x + goomba2.goomba_hitbox.width))
		{
			goomba.side = false;
			goomba2.side = true;
		}
	}

	void Shell_Goomba(Enemy& shell, Mario& player, Enemy& goomba2, float& nextShell)
	{
		//Derecha
		if (Timer > 0 && player.alive != 0 && shell.alive &&
			shell.activated && !shell.side &&
			shell.position.y >= goomba2.position.y &&
			shell.position.y < (goomba2.position.y + goomba2.goomba_hitbox.height) &&
			shell.position.x - 5 <= goomba2.position.x &&
			(nextShell + shell.goomba_hitbox.width) >= goomba2.position.x - 20)
		{
			PlaySound(sfxFireBall);
			goomba2.death2 = true;
			Score += 100;
			SpawnFloatingScore({ goomba2.position.x - 4, goomba2.position.y - 100 }, 100);
		}
		//Izquierda
		if (Timer > 0 && player.alive != 0 &&
			shell.activated && goomba2.side &&
			shell.position.y >= goomba2.position.y &&
			shell.position.y < (goomba2.position.y + goomba2.goomba_hitbox.height) &&
			shell.position.x + 5 >= (goomba2.position.x + goomba2.goomba_hitbox.width) &&
			(nextShell) <= (goomba2.position.x + goomba2.goomba_hitbox.width))
		{
			PlaySound(sfxFireBall);
			goomba2.death2 = true;
			Score += 100;
			SpawnFloatingScore({ goomba2.position.x - 4, goomba2.position.y - 100 }, 100);
		}
	}

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
				star.active = false;
				star.position = { -110, 1400 };
				koopa.position = { 5100, 600 };
				shell.position = { 0, 1000 };
				Timer = 400;
				Score = 000000;
				Money = 00;
				player.alive = 1;
				player.fire = 0;

				goomba1.side = true;
				goomba1.death = false;
				goomba1.death2 = false;
				goomba1.alive = true;
				goomba1.activated = false;
				goomba1.position = { 950, 600 };

				goomba2.side = true;
				goomba2.death = false;
				goomba2.death2 = false;
				goomba2.alive = true;
				goomba2.activated = false;
				goomba2.position = { 1850, 600 };

				goomba3.side = true;
				goomba3.death = false;
				goomba3.death2 = false;
				goomba3.alive = true;
				goomba3.activated = false;
				goomba3.position = { 2300, 600 };

				goomba4.side = true;
				goomba4.death = false;
				goomba4.death2 = false;
				goomba4.alive = true;
				goomba4.activated = false;
				goomba4.position = { 2400, 600 };

				goomba5.side = true;
				goomba5.death = false;
				goomba5.death2 = false;
				goomba5.alive = true;
				goomba5.activated = false;
				goomba5.position = { 3800, 150 };

				goomba6.side = true;
				goomba6.death = false;
				goomba6.death2 = false;
				goomba6.alive = true;
				goomba6.activated = false;
				goomba6.position = { 3900, 150 };

				goomba7.side = true;
				goomba7.death = false;
				goomba7.death2 = false;
				goomba7.alive = true;
				goomba7.activated = false;
				goomba7.position = { 4550, 600 };

				goomba8.side = true;
				goomba8.death = false;
				goomba8.death2 = false;
				goomba8.alive = true;
				goomba8.activated = false;
				goomba8.position = { 4620, 600 };

				goomba9.side = true;
				goomba9.death = false;
				goomba9.death2 = false;
				goomba9.alive = true;
				goomba9.activated = false;
				goomba9.position = { 5410, 600 };

				goomba10.side = true;
				goomba10.death = false;
				goomba10.death2 = false;
				goomba10.alive = true;
				goomba10.activated = false;
				goomba10.position = { 5480, 600 };

				goomba11.side = true;
				goomba11.death = false;
				goomba11.death2 = false;
				goomba11.alive = true;
				goomba11.activated = false;
				goomba11.position = { 5860, 600 };

				goomba12.side = true;
				goomba12.death = false;
				goomba12.death2 = false;
				goomba12.alive = true;
				goomba12.activated = false;
				goomba12.position = { 5930, 600 };

				goomba13.side = true;
				goomba13.death = false;
				goomba13.death2 = false;
				goomba13.alive = true;
				goomba13.activated = false;
				goomba13.position = { 6050, 600 };

				goomba14.side = true;
				goomba14.death = false;
				goomba14.death2 = false;
				goomba14.alive = true;
				goomba14.activated = false;
				goomba14.position = { 6120, 600 };

				goomba15.side = true;
				goomba15.death = false;
				goomba15.death2 = false;
				goomba15.alive = true;
				goomba15.activated = false;
				goomba15.position = { 8300, 600 };

				goomba16.side = true;
				goomba16.death = false;
				goomba16.death2 = false;
				goomba16.alive = true;
				goomba16.activated = false;
				goomba16.position = { 8370, 600 };

				koopa.death = false;
				koopa.alive = true;
				koopa.side = true;
				koopa.activated = false;
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
					camera.target.x = 4200;
					goomba1.death = true;
					goomba2.death = true;
					goomba3.death = true;
					goomba4.death = true;
					goomba5.death = true;
					goomba6.death = true;
				}
				camera.target.x = 333;
				camera.target.y = 350;
				mooshroom.position = { -110, 1400 };
				mooshroom.active = false;
				mooshroom.side = false;
				fireFlower.active = false;
				fireFlower.position = { -110, 1400 };
				star.active = false;
				star.position = { -110, 1400 };
				koopa.position = { 5100, 600 };
				shell.position = { 0, 1000 };
				flag.reached = false;
				bandera.position.y = flag.position.y - flagTexture.height + 5;
				Timer = 400;
				player.alive = 1;
				player.fire = 0;
				goomba1.side = true;
				goomba1.death = false;
				goomba1.death2 = false;
				goomba1.alive = true;
				goomba1.activated = false;
				goomba1.position = { 950, 600 };

				goomba2.side = true;
				goomba2.death = false;
				goomba2.death2 = false;
				goomba2.alive = true;
				goomba2.activated = false;
				goomba2.position = { 1850, 600 };

				goomba3.side = true;
				goomba3.death = false;
				goomba3.death2 = false;
				goomba3.alive = true;
				goomba3.activated = false;
				goomba3.position = { 2300, 600 };

				goomba4.side = true;
				goomba4.death = false;
				goomba4.death2 = false;
				goomba4.alive = true;
				goomba4.activated = false;
				goomba4.position = { 2400, 600 };

				goomba5.side = true;
				goomba5.death = false;
				goomba5.death2 = false;
				goomba5.alive = true;
				goomba5.activated = false;
				goomba5.position = { 3800, 150 };

				goomba6.side = true;
				goomba6.death = false;
				goomba6.death2 = false;
				goomba6.alive = true;
				goomba6.activated = false;
				goomba6.position = { 3900, 150 };

				goomba7.side = true;
				goomba7.death = false;
				goomba7.death2 = false;
				goomba7.alive = true;
				goomba7.activated = false;
				goomba7.position = { 4550, 600 };

				goomba8.side = true;
				goomba8.death = false;
				goomba8.death2 = false;
				goomba8.alive = true;
				goomba8.activated = false;
				goomba8.position = { 4620, 600 };

				goomba9.side = true;
				goomba9.death = false;
				goomba9.death2 = false;
				goomba9.alive = true;
				goomba9.activated = false;
				goomba9.position = { 5410, 600 };

				goomba10.side = true;
				goomba10.death = false;
				goomba10.death2 = false;
				goomba10.alive = true;
				goomba10.activated = false;
				goomba10.position = { 5480, 600 };

				goomba11.side = true;
				goomba11.death = false;
				goomba11.death2 = false;
				goomba11.alive = true;
				goomba11.activated = false;
				goomba11.position = { 5860, 600 };

				goomba12.side = true;
				goomba12.death = false;
				goomba12.death2 = false;
				goomba12.alive = true;
				goomba12.activated = false;
				goomba12.position = { 5930, 600 };

				goomba13.side = true;
				goomba13.death = false;
				goomba13.death2 = false;
				goomba13.alive = true;
				goomba13.activated = false;
				goomba13.position = { 6050, 600 };

				goomba14.side = true;
				goomba14.death = false;
				goomba14.death2 = false;
				goomba14.alive = true;
				goomba14.activated = false;
				goomba14.position = { 6120, 600 };

				goomba15.side = true;
				goomba15.death = false;
				goomba15.death2 = false;
				goomba15.alive = true;
				goomba15.activated = false;
				goomba15.position = { 8300, 600 };

				goomba16.side = true;
				goomba16.death = false;
				goomba16.death2 = false;
				goomba16.alive = true;
				goomba16.activated = false;
				goomba16.position = { 8370, 600 };

				koopa.death = false;
				koopa.alive = true;
				koopa.side = true;
				koopa.activated = false;
				shell.death = false;
				shell.activated = false;
				elapsedTime = 0.0f;
				contmuerte = 0;
				conttiempo = 0;
				blocks = CreateBlocks();

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

			for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
				if (scores[i].active) {
					scores[i].lifetime -= GetFrameTime();
					scores[i].position.y += scores[i].speedY * GetFrameTime();

					if (scores[i].lifetime <= 0) {
						scores[i].active = false;
					}
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
				bandera.position.y = flag.position.y - flagTexture.height + 5;

				// Reset enemies
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

				star.active = false;
				star.position = { -110, 1400 };
				star.emerging = false;
				star.emergeOffset = 0.0f;

				// Reset fireball
				fireBall.position = { 0, 9000 };
				fireBall.active = false;
				fireBall.timer = 0;

				// Reset pipes
				pipe.enteringPipe1 = false;
				pipe.enteringPipe2 = false;

				// Reset all blocks
				blocks = CreateBlocks();

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
		goomba1.goomba_hitbox = { goomba1.position.x, goomba1.position.y, 16, 16 };
		goomba2.goomba_hitbox = { goomba2.position.x, goomba2.position.y, 16, 16 };
		goomba3.goomba_hitbox = { goomba3.position.x, goomba3.position.y, 16, 16 };
		goomba4.goomba_hitbox = { goomba4.position.x, goomba4.position.y, 16, 16 };
		goomba5.goomba_hitbox = { goomba5.position.x, goomba5.position.y, 16, 16 };
		goomba6.goomba_hitbox = { goomba6.position.x, goomba6.position.y, 16, 16 };
		goomba7.goomba_hitbox = { goomba7.position.x, goomba7.position.y, 16, 16 };
		goomba8.goomba_hitbox = { goomba8.position.x, goomba8.position.y, 16, 16 };
		goomba9.goomba_hitbox = { goomba9.position.x, goomba9.position.y, 16, 16 };
		goomba10.goomba_hitbox = { goomba10.position.x, goomba10.position.y, 16, 16 };
		goomba11.goomba_hitbox = { goomba11.position.x, goomba11.position.y, 16, 16 };
		goomba12.goomba_hitbox = { goomba12.position.x, goomba12.position.y, 16, 16 };
		goomba13.goomba_hitbox = { goomba13.position.x, goomba13.position.y, 16, 16 };
		goomba14.goomba_hitbox = { goomba14.position.x, goomba14.position.y, 16, 16 };
		goomba15.goomba_hitbox = { goomba15.position.x, goomba15.position.y, 16, 16 };
		goomba16.goomba_hitbox = { goomba16.position.x, goomba16.position.y, 16, 16 };
		player.mario_hitbox.x += player.speed.x;
		player.mario_hitbox.y += player.speed.y;
		mooshroom.powerup_hitbox = { mooshroom.position.x, mooshroom.position.y, 16,16 };
		fireFlower.powerup_hitbox = { fireFlower.position.x, fireFlower.position.y, 16, 16 };
		star.powerup_hitbox = { star.position.x, star.position.y, 16, 16 };
		fireBall.projectile_hitbox = { fireBall.position.x, fireBall.position.y, 4, 4 };
		koopa.goomba_hitbox = { koopa.position.x, koopa.position.y, 16, 24 };
		shell.goomba_hitbox = { shell.position.x,shell.position.y,16,16 };

		bool hitObstacleFloor = false;
		bool hitObstacleWall = false;
		bool onGroundGoomba1 = false;
		bool onGroundGoomba2 = false;
		bool onGroundGoomba3 = false;
		bool onGroundGoomba4 = false;
		bool onGroundGoomba5 = false;
		bool onGroundGoomba6 = false;
		bool onGroundGoomba7 = false;
		bool onGroundGoomba8 = false;
		bool onGroundGoomba9 = false;
		bool onGroundGoomba10 = false;
		bool onGroundGoomba11 = false;
		bool onGroundGoomba12 = false;
		bool onGroundGoomba13 = false;
		bool onGroundGoomba14 = false;
		bool onGroundGoomba15 = false;
		bool onGroundGoomba16 = false;
		bool onGroundKoopa = false;
		bool onGroundShell = false;
		bool onGroundPowerUp = false;
		bool onGroundPowerUpF = false;
		bool onGroundPowerUpS = false;
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
		if (player.position.x > camera.target.x && camera.target.x < 9795 && player.position.y >= -500)
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
		if ((fireBall.active && fireBall.position.x <= camera.target.x - 480) || (fireBall.active && fireBall.position.x >= camera.target.x + 480)) {
			fireBall.active = false;
		}
		if ((shell.activated && shell.position.x <= camera.target.x - 480) || (shell.activated && shell.position.x >= camera.target.x + 480)) {
			shell.activated = false;
		}
		if ((koopa.activated && koopa.position.x <= camera.target.x - 480) || (koopa.activated && koopa.position.x >= camera.target.x + 480)) {
			koopa.activated = false;
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

		if ((mooshroom.active && mooshroom.position.x <= camera.target.x - 480) || (mooshroom.active && mooshroom.position.x >= camera.target.x + 550)) {
			mooshroom.active = false;
		}

		//FIRE FLOWER
		if (player.position.x >= fireFlower.position.x - 100 && player.alive != 0) {
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

		//STAR
		star.speed.x = 1.0f;
		if (star.active && !star.emerging && player.alive != 0 && star.side) {
			star.position.x += -400 * deltaTime;
		}

		if (star.active && !star.emerging && player.alive != 0 && !star.side) {
			star.position.x += 400 * deltaTime;
		}

		if (star.emerging) {
			float emergeSpeed = 30.0f;  // píxeles por segundo
			star.emergeOffset += emergeSpeed * deltaTime;
			star.position.y -= emergeSpeed * deltaTime;

			if (star.emergeOffset >= star.maxEmergeOffset) {
				star.emerging = false;
				star.active = true;
				star.position.y = star.position.y + star.emergeOffset - star.maxEmergeOffset; // asegura corrección de posición
			}
		}

		//GOOMBA
		Goomba_Activation(goomba1, player);
		Goomba_Activation(goomba2, player);
		Goomba_Activation(goomba3, player);
		Goomba_Activation(goomba4, player);
		Goomba_Activation(goomba5, player);
		Goomba_Activation(goomba6, player);
		Goomba_Activation(goomba7, player);
		Goomba_Activation(goomba8, player);
		Goomba_Activation(goomba9, player);
		Goomba_Activation(goomba10, player);
		Goomba_Activation(goomba11, player);
		Goomba_Activation(goomba12, player);
		Goomba_Activation(goomba13, player);
		Goomba_Activation(goomba14, player);
		Goomba_Activation(goomba15, player);
		Goomba_Activation(goomba16, player);

		goomba1.speed.x = 1.0f;
		goomba2.speed.x = 1.0f;
		goomba3.speed.x = 1.0f;
		goomba4.speed.x = 1.0f;
		goomba5.speed.x = 1.0f;
		goomba6.speed.x = 1.0f;
		goomba7.speed.x = 1.0f;
		goomba8.speed.x = 1.0f;
		goomba9.speed.x = 1.0f;
		goomba10.speed.x = 1.0f;
		goomba11.speed.x = 1.0f;
		goomba12.speed.x = 1.0f;
		goomba13.speed.x = 1.0f;
		goomba14.speed.x = 1.0f;
		goomba15.speed.x = 1.0f;
		goomba16.speed.x = 1.0f;

		Goomba_Movement(goomba1, player, deltaTime);
		Goomba_Movement(goomba2, player, deltaTime);
		Goomba_Movement(goomba3, player, deltaTime);
		Goomba_Movement(goomba4, player, deltaTime);
		Goomba_Movement(goomba5, player, deltaTime);
		Goomba_Movement(goomba6, player, deltaTime);
		Goomba_Movement(goomba7, player, deltaTime);
		Goomba_Movement(goomba8, player, deltaTime);
		Goomba_Movement(goomba9, player, deltaTime);
		Goomba_Movement(goomba10, player, deltaTime);
		Goomba_Movement(goomba11, player, deltaTime);
		Goomba_Movement(goomba12, player, deltaTime);
		Goomba_Movement(goomba13, player, deltaTime);
		Goomba_Movement(goomba14, player, deltaTime);
		Goomba_Movement(goomba15, player, deltaTime);
		Goomba_Movement(goomba16, player, deltaTime);

		Goomba_Death(goomba1, player);
		Goomba_Death(goomba2, player);
		Goomba_Death(goomba3, player);
		Goomba_Death(goomba4, player);
		Goomba_Death(goomba5, player);
		Goomba_Death(goomba6, player);
		Goomba_Death(goomba7, player);
		Goomba_Death(goomba8, player);
		Goomba_Death(goomba9, player);
		Goomba_Death(goomba10, player);
		Goomba_Death(goomba11, player);
		Goomba_Death(goomba12, player);
		Goomba_Death(goomba13, player);
		Goomba_Death(goomba14, player);
		Goomba_Death(goomba15, player);
		Goomba_Death(goomba16, player);

		Goomba_Death2(goomba1, deltaTime);
		Goomba_Death2(goomba2, deltaTime);
		Goomba_Death2(goomba3, deltaTime);
		Goomba_Death2(goomba4, deltaTime);
		Goomba_Death2(goomba5, deltaTime);
		Goomba_Death2(goomba6, deltaTime);
		Goomba_Death2(goomba7, deltaTime);
		Goomba_Death2(goomba8, deltaTime);
		Goomba_Death2(goomba9, deltaTime);
		Goomba_Death2(goomba10, deltaTime);
		Goomba_Death2(goomba11, deltaTime);
		Goomba_Death2(goomba12, deltaTime);
		Goomba_Death2(goomba13, deltaTime);
		Goomba_Death2(goomba14, deltaTime);
		Goomba_Death2(goomba15, deltaTime);
		Goomba_Death2(goomba16, deltaTime);

		Goomba_InTheMoon(goomba1);
		Goomba_InTheMoon(goomba2);
		Goomba_InTheMoon(goomba3);
		Goomba_InTheMoon(goomba4);
		Goomba_InTheMoon(goomba5);
		Goomba_InTheMoon(goomba6);
		Goomba_InTheMoon(goomba7);
		Goomba_InTheMoon(goomba8);
		Goomba_InTheMoon(goomba9);
		Goomba_InTheMoon(goomba10);
		Goomba_InTheMoon(goomba11);
		Goomba_InTheMoon(goomba12);
		Goomba_InTheMoon(goomba13);
		Goomba_InTheMoon(goomba14);
		Goomba_InTheMoon(goomba15);
		Goomba_InTheMoon(goomba16);

		//Koopa
		if (player.position.x >= koopa.position.x - 450 && !koopa.death && !koopa.death2 && player.alive != 0 && Timer > 0) {
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


		if (koopa.position.y >= 1000) {
			koopa.alive = false;
		}

		//SHELL
		if (player.position.x >= shell.position.x - 50 && player.alive != 0) {
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

		// ANIMACIÓN DE REBOTE DE BLOQUES
		for (EnvElement& block : blocks) {
			if (block.bouncing) {
				block.bounceOffset += block.bounceSpeed;
				block.rect.y += block.bounceSpeed;

				if (block.bounceOffset <= -10.0f) {
					block.bounceSpeed *= -1;
				}

				if (block.bounceOffset >= 0.0f) {
					block.rect.y = block.originalY;
					block.bouncing = false;
					block.bounceOffset = 0.0f;
					block.bounceSpeed = -2.0f;
				}
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

				if (!block.bouncing && !block.hit && ColorToInt(block.color) != ColorToInt(GREEN) && ColorToInt(block.color) != ColorToInt(LIGHTGRAY)) {
					PlaySound(sfxBump);
					block.bouncing = true;
					block.originalY = block.rect.y;
					block.bounceOffset = 0.0f;
				}

				if (ColorToInt(block.color) == ColorToInt(GREEN) || (ColorToInt(block.color) == ColorToInt(LIGHTGRAY) && block.rect.y <= -1900)) {
					PlaySound(sfxBreakBlock);
					Score += 50;
					block.rect.y = 3000;
					player.speed.y = 0;
				}

				if (ColorToInt(block.color) == ColorToInt(RED) && !block.hit) { //Moneda
					PlaySound(sfxCoin_Block);
					Money++;
					SpawnFloatingScore({ block.rect.x + 4, block.rect.y - 30 }, 200);
					moneda.active = true;
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
					star.side = false;
					star.position = { block.rect.x + 22, block.rect.y + 35 };
					star.emerging = true;
					star.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(MAGENTA) && !block.hit && !desactived) { //Moneda repetida
					PlaySound(sfxCoin_Block);
					Money++;
					SpawnFloatingScore({ block.rect.x + 4, block.rect.y - 30 }, 200);
					contador++;
					moneda.active = true;
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
				if (!block.bouncing && !block.hit && ColorToInt(block.color) != ColorToInt(LIGHTGRAY)) {
					PlaySound(sfxBump);
					block.bouncing = true;
					block.originalY = block.rect.y;
					block.bounceOffset = 0.0f;
				}

				player.speed.y = 0.0f;
				player.position.y = block.rect.y + block.rect.height + block.rect.height;

				if (ColorToInt(block.color) == ColorToInt(RED) && !block.hit) { //Moneda
					PlaySound(sfxCoin_Block);
					Money++;
					moneda.active = true;
					SpawnFloatingScore({ block.rect.x + 4, block.rect.y - 30 }, 200);
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
					star.side = false;
					star.position = { block.rect.x + 22, block.rect.y + 35 };
					star.emerging = true;
					star.emergeOffset = 0.0f;
					block.hit = true;
				}
				if (ColorToInt(block.color) == ColorToInt(MAGENTA) && !block.hit && !desactived) { //Moneda repetida
					PlaySound(sfxCoin_Block);
					Money++;
					SpawnFloatingScore({ block.rect.x + 4, block.rect.y - 30 }, 200);
					moneda.active = true;
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
		//Goomba
		Goomba_MarioCollision(goomba1, player);
		Goomba_MarioCollision(goomba2, player);
		Goomba_MarioCollision(goomba3, player);
		Goomba_MarioCollision(goomba4, player);
		Goomba_MarioCollision(goomba5, player);
		Goomba_MarioCollision(goomba6, player);
		Goomba_MarioCollision(goomba7, player);
		Goomba_MarioCollision(goomba8, player);
		Goomba_MarioCollision(goomba9, player);
		Goomba_MarioCollision(goomba10, player);
		Goomba_MarioCollision(goomba11, player);
		Goomba_MarioCollision(goomba12, player);
		Goomba_MarioCollision(goomba13, player);
		Goomba_MarioCollision(goomba14, player);
		Goomba_MarioCollision(goomba15, player);
		Goomba_MarioCollision(goomba16, player);

		//Koopa
		if (koopa.alive && !koopa.death && !koopa.death2 && player.position.x + player.mario_hitbox.width + 10 >= koopa.position.x &&
			player.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			player.position.y + player.mario_hitbox.height + 16 >= koopa.position.y && player.position.y <= koopa.position.y + koopa.goomba_hitbox.height)
		{
			if (player.position.y + player.mario_hitbox.height <= koopa.position.y && player.alive && !star.Invincible) {
				PlaySound(sfxStomp);
				shell.position = koopa.position;
				shell.side = koopa.side;
				koopa.death = true;
				Score += 100;
				SpawnFloatingScore({ koopa.position.x + 4, koopa.position.y - 100 }, 100);
				player.speed.y = -PLAYER_JUMP_SPD + 100;
				player.canJump = false;
				player.canJump2 = true;
				player.jumpTime = 0.0f;
			}
			else if (player.big && player.fire && !player.invencible && !star.Invincible) {
				player.invencible = true;
				player.fire = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (player.big && !player.fire && !player.invencible && !star.Invincible) {
				player.invencible = true;
				player.big = 0;
				player.invulnerableTimer = 0.0f;
			}
			else if (!player.big && !player.fire && !player.invencible && !star.Invincible) {
				player.alive = 0;
			}
			else if (star.Invincible) {
				PlaySound(sfxKick);
				koopa.death2 = true;
				Score += 100;
				SpawnFloatingScore({ koopa.position.x - 4, koopa.position.y - 100 }, 100);
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

		if (player.invencible && !star.Invincible) {
			player.invulnerableTimer += GetFrameTime();
			player.visible = fmod(player.invulnerableTimer, 0.15f) < 0.1f;
			if (player.invulnerableTimer >= player.invulnerableDuration) {
				player.invencible = false;
				player.invulnerableTimer = 0.0f;
			}
		}
		if (star.Invincible) {
			player.invencible = true;
			player.invulnerableTimer += GetFrameTime();
			player.visible = fmod(player.invulnerableTimer, 0.15f) < 0.1f;
			if (player.invulnerableTimer >= 10.0f) {
				player.invencible = false;
				star.Invincible = false;
				player.invulnerableTimer = 0.0f;
				StopMusicStream;
				PlayMusicStream(musicOverworld);
			}
		}
		if (shell.speed.x != 0) {
			shell.shell += GetFrameTime();
		}

		//Con bola de fuego
		Goomba_Fuego(goomba1, fireBall);
		Goomba_Fuego(goomba2, fireBall);
		Goomba_Fuego(goomba3, fireBall);
		Goomba_Fuego(goomba4, fireBall);
		Goomba_Fuego(goomba5, fireBall);
		Goomba_Fuego(goomba6, fireBall);
		Goomba_Fuego(goomba7, fireBall);
		Goomba_Fuego(goomba8, fireBall);
		Goomba_Fuego(goomba9, fireBall);
		Goomba_Fuego(goomba10, fireBall);
		Goomba_Fuego(goomba11, fireBall);
		Goomba_Fuego(goomba12, fireBall);
		Goomba_Fuego(goomba13, fireBall);
		Goomba_Fuego(goomba14, fireBall);
		Goomba_Fuego(goomba15, fireBall);
		Goomba_Fuego(goomba16, fireBall);

		if (koopa.alive && fireBall.position.x + fireBall.projectile_hitbox.width + 10 >= koopa.position.x && fireBall.active &&
			fireBall.position.x <= koopa.position.x + koopa.goomba_hitbox.width + 20 &&
			fireBall.position.y + fireBall.projectile_hitbox.height + 16 >= koopa.position.y && fireBall.position.y <= koopa.position.y + koopa.goomba_hitbox.height)
		{
			PlaySound(sfxKick);
			Score += 200;
			SpawnFloatingScore({ koopa.position.x + 4, koopa.position.y - 100 }, 200);
			koopa.death2 = true;
			fireBall.active = false;
			koopaDeathInit = true;
			fireBall.position.y = 2000;
			koopa.speed.x = koopa.side ? -8.0f : 8.0f;
			koopa.speed.y = -20.0f;
		}

		//Con el suelo
		for (EnvElement block : blocks) {
			Goomba_Suelo(goomba1, player, block, onGroundGoomba1);
			Goomba_Suelo(goomba2, player, block, onGroundGoomba2);
			Goomba_Suelo(goomba3, player, block, onGroundGoomba3);
			Goomba_Suelo(goomba4, player, block, onGroundGoomba4);
			Goomba_Suelo(goomba5, player, block, onGroundGoomba5);
			Goomba_Suelo(goomba6, player, block, onGroundGoomba6);
			Goomba_Suelo(goomba7, player, block, onGroundGoomba7);
			Goomba_Suelo(goomba8, player, block, onGroundGoomba8);
			Goomba_Suelo(goomba9, player, block, onGroundGoomba9);
			Goomba_Suelo(goomba10, player, block, onGroundGoomba10);
			Goomba_Suelo(goomba11, player, block, onGroundGoomba11);
			Goomba_Suelo(goomba12, player, block, onGroundGoomba12);
			Goomba_Suelo(goomba13, player, block, onGroundGoomba13);
			Goomba_Suelo(goomba14, player, block, onGroundGoomba14);
			Goomba_Suelo(goomba15, player, block, onGroundGoomba15);
			Goomba_Suelo(goomba16, player, block, onGroundGoomba16);

		}

		Goomba_fall(goomba1, player, onGroundGoomba1, deltaTime);
		Goomba_fall(goomba2, player, onGroundGoomba2, deltaTime);
		Goomba_fall(goomba3, player, onGroundGoomba3, deltaTime);
		Goomba_fall(goomba4, player, onGroundGoomba4, deltaTime);
		Goomba_fall(goomba5, player, onGroundGoomba5, deltaTime);
		Goomba_fall(goomba6, player, onGroundGoomba6, deltaTime);
		Goomba_fall(goomba7, player, onGroundGoomba7, deltaTime);
		Goomba_fall(goomba8, player, onGroundGoomba8, deltaTime);
		Goomba_fall(goomba9, player, onGroundGoomba9, deltaTime);
		Goomba_fall(goomba10, player, onGroundGoomba10, deltaTime);
		Goomba_fall(goomba11, player, onGroundGoomba11, deltaTime);
		Goomba_fall(goomba12, player, onGroundGoomba12, deltaTime);
		Goomba_fall(goomba13, player, onGroundGoomba13, deltaTime);
		Goomba_fall(goomba14, player, onGroundGoomba14, deltaTime);
		Goomba_fall(goomba15, player, onGroundGoomba15, deltaTime);
		Goomba_fall(goomba16, player, onGroundGoomba16, deltaTime);

		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
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

		if (!onGroundShell && player.alive && Timer > 0) {
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
		float nextXE1 = goomba1.position.x + goomba1.speed.x * deltaTime;
		float nextXE2 = goomba2.position.x + goomba2.speed.x * deltaTime;
		float nextXE3 = goomba3.position.x + goomba3.speed.x * deltaTime;
		float nextXE4 = goomba4.position.x + goomba4.speed.x * deltaTime;
		float nextXE5 = goomba5.position.x + goomba5.speed.x * deltaTime;
		float nextXE6 = goomba6.position.x + goomba6.speed.x * deltaTime;
		float nextXE7 = goomba7.position.x + goomba7.speed.x * deltaTime;
		float nextXE8 = goomba8.position.x + goomba8.speed.x * deltaTime;
		float nextXE9 = goomba9.position.x + goomba9.speed.x * deltaTime;
		float nextXE10 = goomba10.position.x + goomba10.speed.x * deltaTime;
		float nextXE11 = goomba11.position.x + goomba11.speed.x * deltaTime;
		float nextXE12 = goomba12.position.x + goomba12.speed.x * deltaTime;
		float nextXE13 = goomba13.position.x + goomba13.speed.x * deltaTime;
		float nextXE14 = goomba14.position.x + goomba14.speed.x * deltaTime;
		float nextXE15 = goomba15.position.x + goomba15.speed.x * deltaTime;
		float nextXE16 = goomba16.position.x + goomba16.speed.x * deltaTime;


		//Derecha
		for (EnvElement block : blocks) {
			Goomba_Lados(goomba1, player, block, nextXE1);
			Goomba_Lados(goomba2, player, block, nextXE2);
			Goomba_Lados(goomba3, player, block, nextXE3);
			Goomba_Lados(goomba4, player, block, nextXE4);
			Goomba_Lados(goomba5, player, block, nextXE5);
			Goomba_Lados(goomba6, player, block, nextXE6);
			Goomba_Lados(goomba7, player, block, nextXE7);
			Goomba_Lados(goomba8, player, block, nextXE8);
			Goomba_Lados(goomba9, player, block, nextXE9);
			Goomba_Lados(goomba10, player, block, nextXE10);
			Goomba_Lados(goomba11, player, block, nextXE11);
			Goomba_Lados(goomba12, player, block, nextXE12);
			Goomba_Lados(goomba13, player, block, nextXE13);
			Goomba_Lados(goomba14, player, block, nextXE14);
			Goomba_Lados(goomba15, player, block, nextXE15);
			Goomba_Lados(goomba16, player, block, nextXE16);
		}

		Goomba_Goomba(goomba3, player, goomba4, nextXE3);
		Goomba_Goomba(goomba6, player, goomba5, nextXE5);
		Goomba_Goomba(goomba7, player, goomba8, nextXE7);
		Goomba_Goomba(goomba8, player, goomba9, nextXE8);
		Goomba_Goomba(goomba9, player, goomba10, nextXE9);
		Goomba_Goomba(goomba10, player, goomba11, nextXE9);
		Goomba_Goomba(goomba11, player, goomba12, nextXE11);
		Goomba_Goomba(goomba12, player, goomba13, nextXE12);
		Goomba_Goomba(goomba13, player, goomba14, nextXE13);
		Goomba_Goomba(goomba15, player, goomba16, nextXE15);

		

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

		Shell_Goomba(shell, player, goomba9, nextshell);
		Shell_Goomba(shell, player, goomba10, nextshell);
		Shell_Goomba(shell, player, goomba11, nextshell);
		Shell_Goomba(shell, player, goomba12, nextshell);
		Shell_Goomba(shell, player, goomba13, nextshell);
		Shell_Goomba(shell, player, goomba14, nextshell);

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
			SpawnFloatingScore({ mooshroom.position.x + 4, mooshroom.position.y - 100 }, 1000);
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
				SpawnFloatingScore({ fireFlower.position.x + 4, fireFlower.position.y - 100 }, 1000);
				fireFlower.position.y = 1000;
			}
			else {
				player.big = true;
				fireFlower.active = false;
				Score += 1000;
				SpawnFloatingScore({ fireFlower.position.x + 4, fireFlower.position.y - 100 }, 1000);
				fireFlower.position.y = 1000;
			}
		}

		//Star
		if (star.active && player.position.x + player.mario_hitbox.width + 10 >= star.position.x &&
			player.position.x <= star.position.x + star.powerup_hitbox.width + 20 &&
			player.position.y >= star.position.y && player.position.y <= star.position.y + star.powerup_hitbox.height &&
			!star.emerging)
		{
			
			PlaySound(sfxPowerUpTaken);
			StopMusicStream;
			PlayMusicStream(musicInvencible);
			star.Invincible = true;
			star.active = false;
			Score += 1000;
			SpawnFloatingScore({ fireFlower.position.x + 4, fireFlower.position.y - 100 }, 1000);
			star.position.y = 1000;
		}

		//Con el suelo
		//Seta
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

		//Flor de Fuego
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

		//Star
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0
				&& block.rect.x <= star.position.x + star.powerup_hitbox.width - 5
				&& block.rect.x + block.rect.width + 10 >= star.position.x
				&& block.rect.y + block.rect.height >= star.position.y
				&& block.rect.y <= star.position.y && !star.emerging
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW)) {
				onGroundPowerUpS = true;
				star.speed.y = 300.0f * deltaTime;
				onGroundPowerUpS = false;

				star.position.y = block.rect.y + 5;
				star.position.y = block.rect.y - 100;
			}
		}

		//Bola de fuego
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
				fireBall.position.y = block.rect.y - 40;

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
				mooshroom.position.y += (GRAVITY - 400) * 2.0f * deltaTime;
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
		if (!onGroundPowerUpS && player.alive && Timer > 0 && !star.emerging) {
			star.position.y += (GRAVITY - 400) * deltaTime;
			if (mooshroom.position.y > 0)
			{
				star.position.y += (GRAVITY - 400) * 2.0f * deltaTime;
			}
			else
			{
				star.position.y += (GRAVITY - 400) * deltaTime;
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

		//Los lados Star
		float nextS = star.position.x + star.speed.x * deltaTime;

		//Derecha
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				star.active && !star.side &&
				star.position.y > block.rect.y &&
				star.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				star.position.x - 10 <= block.rect.x &&
				(nextS + star.powerup_hitbox.width) >= block.rect.x - 15
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				star.side = true;
			}
		}

		//Izquierda
		for (EnvElement block : blocks) {
			if (Timer > 0 && player.alive != 0 &&
				star.active && star.side &&
				star.position.y > block.rect.y &&
				star.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				star.position.x + 10 >= (block.rect.x + block.rect.width) &&
				(nextS) <= (block.rect.x + block.rect.width + 20)
				&& ColorToInt(block.color) != ColorToInt(BLUE) && ColorToInt(block.color) != ColorToInt(YELLOW))
			{
				star.side = false;
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
				fireBall.active && fireBall.position.y > block.rect.y - 12 &&
				fireBall.position.y < (block.rect.y + block.rect.height + block.rect.height) &&
				fireBall.position.x >= (block.rect.x + block.rect.width) &&
				(nextF) <= (block.rect.x + block.rect.width + 15)
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
				Score += 200;
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
				Score += 200;
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
				Score += 200;
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
				Score += 200;
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
				Score += 200;
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
				Score += 200;
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
				Score += 200;
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
				player.position.y += 3 * 0.50;
			} 
			else if (hitObstacleFloor && bandera.position.y >= 640) {
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

		if (IsKeyPressed(KEY_R) && !pipe.enteringPipe1 && !pipe.enteringPipe2) {
			player.position = { 50, 600 };
			camera.target.x = 333;
			camera.target.y = 350;
			mooshroom.position = { -110, 1400 };
			mooshroom.active = false;
			mooshroom.side = false;
			fireFlower.active = false;
			fireFlower.position = { -110, 1400 };
			star.active = false;
			star.position = { -110, 1400 };
			goomba1.position = { 1400, 600 };
			goomba2.position = { 1400, 600 };
			goomba3.position = { 1400, 600 };
			goomba4.position = { 1400, 600 };
			goomba5.position = { 1400, 600 };
			goomba6.position = { 1400, 600 };
			goomba7.position = { 1400, 600 };
			goomba8.position = { 1400, 600 };
			goomba9.position = { 1400, 600 };
			goomba10.position = { 1400, 600 };
			goomba11.position = { 1400, 600 };
			goomba12.position = { 1400, 600 };
			goomba13.position = { 1400, 600 };
			goomba14.position = { 1400, 600 };
			goomba15.position = { 1400, 600 };
			goomba16.position = { 1400, 600 };
			koopa.position = { 5100, 600 };
			shell.position = { 0, 1000 };
			Timer = 400;
			Money = 00;
			Score = 000000;
			flag.reached = false;
			bandera.position.y = flag.position.y - flagTexture.height + 5;
			player.alive = 1;
			player.lifes = 3;
			player.big = 0;
			player.fire = 0;
			goomba1.side = true;
			goomba1.death = false;
			goomba1.death2 = false;
			goomba1.alive = true;
			goomba1.activated = false;
			goomba1.position = { 950, 600 };

			goomba2.side = true;
			goomba2.death = false;
			goomba2.death2 = false;
			goomba2.alive = true;
			goomba2.activated = false;
			goomba2.position = { 1850, 600 };

			goomba3.side = true;
			goomba3.death = false;
			goomba3.death2 = false;
			goomba3.alive = true;
			goomba3.activated = false;
			goomba3.position = { 2300, 600 };

			goomba4.side = true;
			goomba4.death = false;
			goomba4.death2 = false;
			goomba4.alive = true;
			goomba4.activated = false;
			goomba4.position = { 2400, 600 };

			goomba5.side = true;
			goomba5.death = false;
			goomba5.death2 = false;
			goomba5.alive = true;
			goomba5.activated = false;
			goomba5.position = { 3800, 150 };

			goomba6.side = true;
			goomba6.death = false;
			goomba6.death2 = false;
			goomba6.alive = true;
			goomba6.activated = false;
			goomba6.position = { 3900, 150 };

			goomba7.side = true;
			goomba7.death = false;
			goomba7.death2 = false;
			goomba7.alive = true;
			goomba7.activated = false;
			goomba7.position = { 4550, 600 };

			goomba8.side = true;
			goomba8.death = false;
			goomba8.death2 = false;
			goomba8.alive = true;
			goomba8.activated = false;
			goomba8.position = { 4620, 600 };

			goomba9.side = true;
			goomba9.death = false;
			goomba9.death2 = false;
			goomba9.alive = true;
			goomba9.activated = false;
			goomba9.position = { 5410, 600 };

			goomba10.side = true;
			goomba10.death = false;
			goomba10.death2 = false;
			goomba10.alive = true;
			goomba10.activated = false;
			goomba10.position = { 5480, 600 };

			goomba11.side = true;
			goomba11.death = false;
			goomba11.death2 = false;
			goomba11.alive = true;
			goomba11.activated = false;
			goomba11.position = { 5860, 600 };

			goomba12.side = true;
			goomba12.death = false;
			goomba12.death2 = false;
			goomba12.alive = true;
			goomba12.activated = false;
			goomba12.position = { 5930, 600 };

			goomba13.side = true;
			goomba13.death = false;
			goomba13.death2 = false;
			goomba13.alive = true;
			goomba13.activated = false;
			goomba13.position = { 6050, 600 };

			goomba14.side = true;
			goomba14.death = false;
			goomba14.death2 = false;
			goomba14.alive = true;
			goomba14.activated = false;
			goomba14.position = { 6120, 600 };

			goomba15.side = true;
			goomba15.death = false;
			goomba15.death2 = false;
			goomba15.alive = true;
			goomba15.activated = false;
			goomba15.position = { 8300, 600 };

			goomba16.side = true;
			goomba16.death = false;
			goomba16.death2 = false;
			goomba16.alive = true;
			goomba16.activated = false;
			goomba16.position = { 8370, 600 };
			koopa.death = false;
			koopa.death2 = false;
			koopa.side = true;
			koopa.alive = true;
			koopa.activated = false;
			shell.death = false;
			shell.activated = false;
			elapsedTime = 0.0f;
			contmuerte = 0;
			blocks = CreateBlocks();
		}

		if (IsKeyPressed(KEY_P)) {
			player.alive = 0;
		}
		if (IsKeyPressed(KEY_L)) {
			Score += 50;
			SpawnFloatingScore({ player.position.x + 4, player.position.y - 100 }, 50);
		}
		if (IsKeyPressed(KEY_I)) {
			Score += 1000;
			SpawnFloatingScore({ player.position.x + 4, player.position.y - 100 }, 1000);
		}
		if (IsKeyPressed(KEY_O)) {
			PlaySound(sfxCoin_Block);
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
			goomba1.death = false;
			goomba1.death2 = false;
			goomba1.alive = true;
			goomba1.side = true;
			goomba1.position.x = player.position.x + 200;
			goomba1.position.y = player.position.y;
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
		if (IsKeyPressed(KEY_J) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Seta
			star.active = true;
			star.side = true;
			star.position.x = player.position.x + 200;
			star.position.y = player.position.y;
		}
		if (IsKeyPressed(KEY_F1) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Seta
			player.position = { 3950, 600 };
			camera.target.x = player.position.x;
		}
		if (IsKeyPressed(KEY_F2) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Seta
			player.position = { 6900, 600 };
			camera.target.x = player.position.x;
		}
		if (IsKeyPressed(KEY_F3) && Timer > 0 && player.alive == 1 && !flag.reached) { //Generar Seta
			player.position = { 9100, 600 };
			camera.target.x = player.position.x;
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
		if (!star.Invincible) {
		UpdateMusicStream(musicOverworld);
		}
		if (star.Invincible) {
			UpdateMusicStream(musicInvencible);
		}
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
		float deltaTime = GetFrameTime();
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

		if (!player.side && !player.fire && !star.Invincible) mario_sprite = Mario_Right;
		else if (player.side && !player.fire && !star.Invincible) mario_sprite = Mario_Left;
		else if (!player.side && player.fire && player.big && !star.Invincible) mario_sprite = Mario_Fire_Right;
		else if (player.side && player.fire && player.big && !star.Invincible) mario_sprite = Mario_Fire_Left;
		else if (!player.side && star.Invincible) mario_sprite = Mario_Star_Right;
		else if (player.side && star.Invincible) mario_sprite = Mario_Star_Left;


		Rectangle sourceRec = { 0, 0, (float)frameWidthP, (float)frameHeightP };

		if (star.Invincible) {
			Rectangle sourceRec = { 0, 48, (float)frameWidthP, (float)frameHeightP };
		}

		static float frameTime = 0.0f;
		static int currentFrame = 0;
		frameTime += GetFrameTime();
		float frameSpeed = 0.1f; //Velocity animation

		//Enemies
		/*--Goomba--*/
		int frameWidthG = 16;
		int frameHeightG = 16;

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
			if (!player.fire && !star.Invincible) mario_sprite = Mario_Right;
			else if (player.fire && !star.Invincible) mario_sprite = Mario_Fire_Right;
			else if (star.Invincible) mario_sprite = Mario_Star_Right;

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
			if (!player.fire && !star.Invincible) mario_sprite = Mario_Left;
			else if (player.fire && !star.Invincible) mario_sprite = Mario_Fire_Left;
			else if (star.Invincible) mario_sprite = Mario_Star_Left;
			
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
		// Goomba 1
		if (goomba1.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba1.death && !goomba1.death2) goomba_sprite1 = Goomba;
		if (goomba1.death) goomba_sprite1 = Goomba_chafado;
		if (goomba1.death2) goomba_sprite1 = Goomba_invertido;

		// Goomba 2
		if (goomba2.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba2.death && !goomba2.death2) goomba_sprite2 = Goomba;
		if (goomba2.death) goomba_sprite2 = Goomba_chafado;
		if (goomba2.death2) goomba_sprite2 = Goomba_invertido;

		// Goomba 3
		if (goomba3.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba3.death && !goomba3.death2) goomba_sprite3 = Goomba;
		if (goomba3.death) goomba_sprite3 = Goomba_chafado;
		if (goomba3.death2) goomba_sprite3 = Goomba_invertido;

		// Goomba 4
		if (goomba4.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba4.death && !goomba4.death2) goomba_sprite4 = Goomba;
		if (goomba4.death) goomba_sprite4 = Goomba_chafado;
		if (goomba4.death2) goomba_sprite4 = Goomba_invertido;

		// Goomba 5
		if (goomba5.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba5.death && !goomba5.death2) goomba_sprite5 = Goomba;
		if (goomba5.death) goomba_sprite5 = Goomba_chafado;
		if (goomba5.death2) goomba_sprite5 = Goomba_invertido;

		// Goomba 6
		if (goomba6.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba6.death && !goomba6.death2) goomba_sprite6 = Goomba;
		if (goomba6.death) goomba_sprite6 = Goomba_chafado;
		if (goomba6.death2) goomba_sprite6 = Goomba_invertido;

		// Goomba 7
		if (goomba7.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba7.death && !goomba7.death2) goomba_sprite7 = Goomba;
		if (goomba7.death) goomba_sprite7 = Goomba_chafado;
		if (goomba7.death2) goomba_sprite7 = Goomba_invertido;

		// Goomba 8
		if (goomba8.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba8.death && !goomba8.death2) goomba_sprite8 = Goomba;
		if (goomba8.death) goomba_sprite8 = Goomba_chafado;
		if (goomba8.death2) goomba_sprite8 = Goomba_invertido;

		// Goomba 9
		if (goomba9.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba9.death && !goomba9.death2) goomba_sprite9 = Goomba;
		if (goomba9.death) goomba_sprite9 = Goomba_chafado;
		if (goomba9.death2) goomba_sprite9 = Goomba_invertido;

		// Goomba 10
		if (goomba10.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba10.death && !goomba10.death2) goomba_sprite10 = Goomba;
		if (goomba10.death) goomba_sprite10 = Goomba_chafado;
		if (goomba10.death2) goomba_sprite10 = Goomba_invertido;

		// Goomba 11
		if (goomba11.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba11.death && !goomba11.death2) goomba_sprite11 = Goomba;
		if (goomba11.death) goomba_sprite11 = Goomba_chafado;
		if (goomba11.death2) goomba_sprite11 = Goomba_invertido;

		// Goomba 12
		if (goomba12.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba12.death && !goomba12.death2) goomba_sprite12 = Goomba;
		if (goomba12.death) goomba_sprite12 = Goomba_chafado;
		if (goomba12.death2) goomba_sprite12 = Goomba_invertido;

		// Goomba 13
		if (goomba13.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba13.death && !goomba13.death2) goomba_sprite13 = Goomba;
		if (goomba13.death) goomba_sprite13 = Goomba_chafado;
		if (goomba13.death2) goomba_sprite13 = Goomba_invertido;

		// Goomba 14
		if (goomba14.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba14.death && !goomba14.death2) goomba_sprite14 = Goomba;
		if (goomba14.death) goomba_sprite14 = Goomba_chafado;
		if (goomba14.death2) goomba_sprite14 = Goomba_invertido;

		// Goomba 15
		if (goomba15.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba15.death && !goomba15.death2) goomba_sprite15 = Goomba;
		if (goomba15.death) goomba_sprite15 = Goomba_chafado;
		if (goomba15.death2) goomba_sprite15 = Goomba_invertido;

		// Goomba 16
		if (goomba16.activated && player.alive != 0 && Timer > 0) {
			if (frameTimeE >= frameSpeedE) {
				frameTimeE = 0.0f;
				currentFrameE = (currentFrameE + 1) % 3;
			}
			sourceRec2.x = (float)(currentFrameE * frameWidthG);
		}
		if (!goomba16.death && !goomba16.death2) goomba_sprite16 = Goomba;
		if (goomba16.death) goomba_sprite16 = Goomba_chafado;
		if (goomba16.death2) goomba_sprite16 = Goomba_invertido;

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
		// Wall Bug Fix
		//1 stair
		DrawTextureEx(escalera, { 6460, 550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6460, 500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6460, 450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6460, 400 }, 0.0f, 3.2f, WHITE);
		//2 stair
		DrawTextureEx(escalera, { 6590, 550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6590, 500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6590, 450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 6590, 400 }, 0.0f, 3.2f, WHITE);
		//3 stair
		DrawTextureEx(escalera, { 7170, 550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7170, 500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7170, 450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7170, 400 }, 0.0f, 3.2f, WHITE);
		//4 stair
		DrawTextureEx(escalera, { 7320, 550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7320, 500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7320, 450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 7320, 400 }, 0.0f, 3.2f, WHITE);
		//5 stair
		DrawTextureEx(escalera, { 8970, 550 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 500 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 450 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 400 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 350 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 300 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 250 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(escalera, { 8970, 200 }, 0.0f, 3.2f, WHITE);


		DrawTexturePro(Mooshroom, sourceRec2, { mooshroom.position.x - 20, mooshroom.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(FireFlower, sourceRec2, { fireFlower.position.x - 20, fireFlower.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		DrawTexturePro(Star, sourceRec2, { star.position.x - 20, star.position.y - 48, sourceRec.width * 3, sourceRec2.height * 3 }, { 0,0 }, 0, WHITE);
		// Monedas 

		for (EnvElement& block : blocks) {
			if (moneda.active) {
				if (moneda.position < 140) {
					moneda.position += 0.03;
				}
				if (block.rect.x - moneda.position3 <= player.position.x + player.mario_hitbox.width - 5 && block.rect.x + block.rect.width + moneda.position3 + 10 >= player.position.x
					&& block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2 >= player.position.y - 100 + player.speed.y * deltaTime 
					&& block.rect.y + block.rect.height + block.rect.height <= player.position.y && ColorToInt(block.color) == ColorToInt(RED)
					|| block.rect.x - moneda.position3 <= player.position.x + player.mario_hitbox.width - 5 && block.rect.x + block.rect.width + moneda.position3 + 10 >= player.position.x
					&& block.rect.y + block.rect.height + block.rect.height + block.rect.height - 2 >= player.position.y - 100 + player.speed.y * deltaTime
					&& block.rect.y + block.rect.height + block.rect.height <= player.position.y && ColorToInt(block.color) == ColorToInt(MAGENTA)) {
					DrawTexturePro(money_b, sourceRec4, { block.rect.x, block.rect.y - moneda.position + moneda.position2, sourceRec4.width * 3, sourceRec4.height * 3 }, { 0, 0 }, 0, WHITE);
					if (player.position.x <= 4800) {
						moneda.position3 = 90;
					}
					if (player.position.x > 4800 && player.position.x <= 6000) {
						moneda.position3 = 20;
					}
				}
				if (moneda.position >= 140)
				{
					moneda.position2 += 0.03;
				}
				if (moneda.position2 >= 140) {
					moneda.position = 0;
					moneda.position2 = 0;
					moneda.active = false;
				}
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
		//Wall bug fix

		DrawTextureEx(ladrillo_cueva, { -112, -2000 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1950 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1900 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1850 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1800 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1750 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1700 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1650 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1600 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(ladrillo_cueva, { -112, -1550 }, 0.0f, 3.2f, WHITE);


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

		DrawTexturePro(goomba_sprite1, sourceRec2, { goomba1.position.x - 20, goomba1.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite2, sourceRec2, { goomba2.position.x - 20, goomba2.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite3, sourceRec2, { goomba3.position.x - 20, goomba3.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite4, sourceRec2, { goomba4.position.x - 20, goomba4.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite5, sourceRec2, { goomba5.position.x - 20, goomba5.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite6, sourceRec2, { goomba6.position.x - 20, goomba6.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite7, sourceRec2, { goomba7.position.x - 20, goomba7.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite8, sourceRec2, { goomba8.position.x - 20, goomba8.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite9, sourceRec2, { goomba9.position.x - 20, goomba9.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite10, sourceRec2, { goomba10.position.x - 20, goomba10.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite11, sourceRec2, { goomba11.position.x - 20, goomba11.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite12, sourceRec2, { goomba12.position.x - 20, goomba12.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite13, sourceRec2, { goomba13.position.x - 20, goomba13.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite14, sourceRec2, { goomba14.position.x - 20, goomba14.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite15, sourceRec2, { goomba15.position.x - 20, goomba15.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);
		DrawTexturePro(goomba_sprite16, sourceRec2, { goomba16.position.x - 20, goomba16.position.y - 48, sourceRec2.width * 3, sourceRec2.height * 3 }, { 0, 0 }, 0, WHITE);

		
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
		if (!flag.reached) {
			DrawTextureEx(meta, { 9350, flag.position.y - flagTexture.height + 5 }, 0, 3, WHITE);
		}
		if (flag.reached) {
			DrawTextureEx(meta, { 9350, bandera.position.y - flagTexture.height + 5 }, 0, 3, WHITE);
			if (bandera.position.y < 640) {
				bandera.position.y += 6 * 0.50;
			}
		}
		DrawTextureEx(castle, { (9675), (360) }, 0.0f, 3, WHITE);

		for (int i = 0; i < MAX_FLOATING_SCORES; i++) {
			if (scores[i].active) {
				DrawTextEx(marioFont, TextFormat("%d", scores[i].value), { scores[i].position.x, scores[i].position.y }, 20, 1, WHITE);
			}
		}

		//Mario
		if (!player.big && !player.invencible && !star.Invincible) {
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}
		else if (!player.big && player.visible && player.invencible && !star.Invincible) {
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}

		if (player.big && !player.fire && !player.invencible && !star.Invincible) {
			sourceRec.y = 16;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}
		else if (player.big && !player.fire && player.visible && player.invencible && !star.Invincible) {
			sourceRec.y = 16;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}


		if (!player.big && !player.fire && star.Invincible) {
			sourceRec.y = 0;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
			star.variant += 2 * 0.5;
			if (star.variant >= 10.0f) {
				sourceRec.y = 48;
				DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
				star.variant += 2 * 0.5;
				if (star.variant >= 20.0f) {
					sourceRec.y = 96;
					DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
					star.variant += 2 * 0.5;
					if (star.variant >= 30.0f) {
						sourceRec.y = 144;
						DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 48, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
						star.variant += 2 * 0.5;
						if (star.variant >= 40.0f) {
							star.variant = 0;
						}
					}
				}
			}
		}
		if (player.big && !player.fire && star.Invincible) {
			sourceRec.y = 16;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
			star.variant += 2 * 0.5;
			if (star.variant >= 10.0f) {
				sourceRec.y = 64;
				DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
				star.variant += 2 * 0.5;
				if (star.variant >= 20.0f) {
					sourceRec.y = 112;
					DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
					star.variant += 2 * 0.5;
					if (star.variant >= 30.0f) {
						sourceRec.y = 160;
						DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
						star.variant += 2 * 0.5;
						if (star.variant >= 40.0f) {
							star.variant = 0;
						}
					}
				}
			}
		}


		if (player.big && player.fire && !star.Invincible) {
			sourceRec.y = 32;
			DrawTexturePro(mario_sprite, sourceRec, { player.position.x - 20, player.position.y - 96, sourceRec.width * 3, sourceRec.height * 3 }, { 0, 0 }, 0, WHITE);
		}
		DrawTextureEx(tuberia_b, { (2592), (399) }, 0.0f, 1.2, WHITE);
		DrawTextureEx(suelo_m, { 2570, 599 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_m, { 2620, 599 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(suelo_m, { 2670, 599 }, 0.0f, 3.2f, WHITE);
		DrawTextureEx(tuberia_cueva, { 579, -1700 }, 0.0f, 3.2f, WHITE);

		if (player.position.x >= 9795) { //Mario arrived to the flag
			camera.target.x = 9795;
			DrawTextureEx(banderin, { (9775), (320) }, 0.0f, 3, WHITE);
			DrawTextureEx(castle, { (9675), (360) }, 0.0f, 3, WHITE);
			player.big = 0;
			player.fire = 0;
			UnloadTexture(mario_sprite);
		}
		
		DrawTexturePro(coin_ui, sourceRec4, { camera.target.x - 165, camera.target.y - 310, sourceRec4.width * 4, sourceRec4.height * 4 }, { 0, 0 }, 0, WHITE);

		EndMode2D();
		UItest();

	}
};

int main() {
	Game game;
	game.Run();
	return 0;
}