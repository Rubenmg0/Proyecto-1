#ifndef STRUCTURES_H
#define STRUCTURES_H

#include "raylib.h"

// ----------------------------
// Mario Structure
// ----------------------------
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

// ----------------------------
// Enemy Structure
// ----------------------------
struct Enemy {
    Vector2 position;
    Rectangle goomba_hitbox;
    Vector2 speed;
    bool activated;
    bool alive;
    bool death;
    bool side; // 0 = Right, 1 = Left
    int shell = 0;
    bool shell2;

    Enemy(float x, float y)
        : position{ x, y }, activated(false), alive(true), death(false), side(true), shell2(false) {
    }
};

// ----------------------------
// PowerUp Structure
// ----------------------------
struct PowerUp {
    Vector2 position;
    Rectangle powerup_hitbox;
    Vector2 speed;
    bool active;
    bool side; // 0 = Right, 1 = Left

    PowerUp(float x, float y)
        : position{ x, y }, active(false), side(true) {
    }
};

// ----------------------------
// Projectile Structure
// ----------------------------
struct Projectile {
    Vector2 position;
    Rectangle projectile_hitbox;
    Vector2 speed;
    bool active;

    Projectile(float x, float y)
        : position{ x, y }, active(false), speed{ 0, 0 } {
    }
};

// ----------------------------
// Environment Element Structure
// ----------------------------
struct EnvElement {
    Rectangle rect;
    Color color;

    EnvElement(float x, float y, float width, float height, Color col)
        : rect{ x, y, width, height }, color(col) {
    }
};

// ----------------------------
// Flag Structure
// ----------------------------
struct Flag {
    Vector2 position;
    bool reached;

    Flag(float x, float y)
        : position{ x, y }, reached(false) {
    }
};

// ----------------------------
// Pipe Structure
// ----------------------------
struct Pipe {
    Rectangle pipe1 = { 2600, 350, 100, 200 };
    Rectangle pipe2 = { 565, -585, 20, 90 };
    bool enteringPipe1 = false;
    bool enteringPipe2 = false;
};

#endif 