#pragma once

#include "entities.h"
#include "Inventory.h"
#include "raylib.h"


enum Direction { Down, Up, Left, Right };

class Player : public Entity {
public:
    Texture2D sprite;
    float walkingspeed = 5.0f;
    Inventory inventory;
    int facingDir = Down;
    bool isMoving = false;
    int animCont = 0;
    bool allowInput = true;
    int SpriteCont = 0;
    int animMax = 25;
    Texture2D spriteSheet = LoadTexture("assets/Basic Character Spritesheet.png");
    float spriteSize = 192;
    int currMoney = 0;
    int preMoney = 0;


    void GetMoney(int& amount);
    void GetPreMoney(int& amount);
    void LoseMoney(int& amount);
    void LosePreMoney(int& amount);

    void Update(Map& map, Camera2D& camera);
    void Render();
    

};