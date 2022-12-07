#pragma once
#include "Inventory.h"
#include "raylib.h"


enum Direction { Left, Up, Right, Down };

class Player {
public:
    Vector2 position = Vector2(0, 0);
    Texture2D sprite;
    float walkingspeed = 5.0f;
    Inventory inventory;
    Direction facingDir = Down;

};
