#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"
/*------------------------------------------------------------*/
/*                      Mario Structure                       */
/*------------------------------------------------------------*/
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

    int alive = 1; // If alive = 0 --> Mario is dead
    int lifes = 3;
    bool big;
    bool side;      // 0 = Right, 1 = Left
    bool fire;
    bool invencible;
    bool visible;

    Mario(float x, float y)
        : position{ x, y }, speed{ 0, 0 }, canJump(false), big(false), fire(false), invencible(false) {
    }
};

/*------------------------------------------------------------*/
/*                      Enemy Structure                       */
/*------------------------------------------------------------*/
struct Enemy {
    Vector2 position;
    Rectangle goomba_hitbox;
    Vector2 speed;
    bool activated;
    bool alive;
    bool death;
    bool death2;
    bool side; // 0 = Right, 1 = Left
    int shell = 0;
    bool shell2;

    Enemy(float x, float y)
        : position{ x, y }, activated(false), alive(true), death(false), death2(false), side(true), shell2(false) {
    }
};

/*------------------------------------------------------------*/
/*                      PowerUp Structure                     */
/*------------------------------------------------------------*/
struct PowerUp {
    Vector2 position;
    Rectangle powerup_hitbox;
    Vector2 speed;
    bool active;
    bool side; // 0 = Right, 1 = Left
    bool emerging;
    float emergeOffset;
    float maxEmergeOffset = 30.0f;
    bool Invincible;
    float variant = 0;

    PowerUp(float x, float y)
        : position{ x, y }, active(false), side(false), emerging(false) , Invincible(false){
    }
};

/*------------------------------------------------------------*/
/*                    Projectile Structure                    */
/*------------------------------------------------------------*/
struct Projectile {
    Vector2 position;
    Rectangle projectile_hitbox;
    Vector2 speed;
    bool active;
    int timer = 0;

    Projectile(float x, float y)
        : position{ x, y }, active(false), speed{ 0, 0 } {
    }
};

/*------------------------------------------------------------*/
/*                Enviroment Element Structure                */
/*------------------------------------------------------------*/
struct EnvElement {
    Rectangle rect;
    Color color;
    bool hit;
    //Mario hit while small state
    bool bouncing = false;
    float bounceOffset = 0.0f;
    float bounceSpeed = -2.0f;
    float originalY = 0.0f;

    EnvElement(float x, float y, float width, float height, Color col, bool hit)
        : rect{ x, y, width, height }, color(col), hit(hit),
        bouncing(false), bounceOffset(0.0f), originalY(y) {
    }
};

/*------------------------------------------------------------*/
/*                       Flag Structure                       */
/*------------------------------------------------------------*/
struct Flag {
    Vector2 position;
    bool reached;
    bool emerging;
    float emergeOffset;
    float maxEmergeOffset = 30.0f;
    bool pause;

    Flag(float x, float y)
        : position{ x, y }, reached(false), emerging(false), pause(false) {
    }
};

/*------------------------------------------------------------*/
/*                       Pipe Structure                       */
/*------------------------------------------------------------*/
struct Pipe {
    Rectangle pipe1 = { 2600, 350, 100, 200 };
    Rectangle pipe2 = { 565, -1585, 20, 90 };
    bool enteringPipe1 = false;
    bool enteringPipe2 = false;
};

/*------------------------------------------------------------*/
/*                    CoinPop Structure                       */
/*------------------------------------------------------------*/
struct CoinPop {
    float position = 0;
    float position2 = 0;
    float position3 = 0;
    bool active = false;
    bool altura = false;
};

/*------------------------------------------------------------*/
/*                      Text Puntuation                       */
/*------------------------------------------------------------*/

struct FloatingScore {
    Vector2 position;
    int value;
    float lifetime;
    float speedY;
    bool active;
};


#endif 