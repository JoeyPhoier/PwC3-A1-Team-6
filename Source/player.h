#pragma once
#include "Inventory.h"
#include "raylib.h"


enum Direction { Down, Up, Left, Right };

class Player {
public:
    Vector2 position = Vector2(0, 0);
    Texture2D sprite;
    float walkingspeed = 5.0f;
    Inventory inventory;
    Direction facingDir = Down;
    bool isMoving = false;
    int animCont = 0;
    bool allowInput = true;
    float SpriteCont = 0;
    int animMax = 25;
    Texture2D spriteSheet = LoadTexture("assets/Basic Character Spritesheet2.png");
    float spriteSize = 72;

};
