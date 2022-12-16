#include "player.h"
#include "JCC_tools.h"

void Player::Update(Map& map, Camera2D& camera) {

    //First the functions that ignore "allowInput".

    if (IsKeyPressed(KEY_ONE)) inventory.selectedIndex = 0;
    else if (IsKeyPressed(KEY_TWO)) inventory.selectedIndex = 1;
    else if (IsKeyPressed(KEY_THREE)) inventory.selectedIndex = 2;
    else if (IsKeyPressed(KEY_FOUR)) inventory.selectedIndex = 3;
    else if (IsKeyPressed(KEY_FIVE)) inventory.selectedIndex = 4;
    else if (IsKeyPressed(KEY_SIX)) inventory.selectedIndex = 5;
    else if (IsKeyPressed(KEY_SEVEN)) inventory.selectedIndex = 6;
    else if (IsKeyPressed(KEY_EIGHT)) inventory.selectedIndex = 7;
    else if (IsKeyPressed(KEY_NINE)) inventory.selectedIndex = 8;
    else if (IsKeyPressed(KEY_ZERO)) inventory.selectedIndex = 9;


    if (IsKeyUp(KEY_LEFT_CONTROL)) inventory.selectedIndex -= GetMouseWheelMove();  //Probably needs some smoothing
    if (inventory.selectedIndex > 9) inventory.selectedIndex = 0;
    else if (inventory.selectedIndex < 0) inventory.selectedIndex = 9;

    if (!allowInput) return; //Anything below this point will be skipped if mid animation.

    Vector2 movDir = Vector2(0, 0);                                 //Could eventually add deltaTime if performance is affected
    if (IsKeyDown(KEY_A)) { movDir.x--; facingDir = Left; }  //Have to test whether local var and player stored var with
    if (IsKeyDown(KEY_D)) { movDir.x++; facingDir = Right; } //point lookup is more efficient.
    if (IsKeyDown(KEY_W)) { movDir.y--; facingDir = Up; }
    if (IsKeyDown(KEY_S)) { movDir.y++; facingDir = Down; }
    if (IsKeyUp(KEY_A) && IsKeyUp(KEY_S) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W)) isMoving = false;
    else isMoving = true;

    if ((movDir.x != 0 || movDir.y != 0) && isMoving) {
        NormalVect(movDir);
        bool canMoveX = true;
        bool canMoveY = true;
        for(Rectangle i : map.collisions){
            if (CheckCollisionPointRec(Vector2(position.x + movDir.x * walkingspeed, position.y), i)){
                canMoveX = false;
                isMoving = false;
            };
            if (CheckCollisionPointRec(Vector2(position.x, position.y + movDir.y * walkingspeed), i)){
                canMoveY = false;
                isMoving = false;
            };
        }
        if (position.x + movDir.x * walkingspeed < spriteSize * 0.15f || position.x + movDir.x * walkingspeed > map.tilesX * map.tileSize - spriteSize * 0.15f) {
            canMoveX = false;
            isMoving = false;
        }
        if (position.y + movDir.y * walkingspeed < spriteSize * 0.3f || position.y + movDir.y * walkingspeed > map.tilesY * map.tileSize - spriteSize * 0.05f) {
            canMoveY = false;
            isMoving = false;
        }
        if (canMoveX && movDir.x != 0)
        {
            position.x += movDir.x * walkingspeed;
            isMoving = true;
        }
        if (canMoveY && movDir.y != 0)
        {
            position.y += movDir.y * walkingspeed;
            isMoving = true;
        }
    }
    
    if (IsMouseButtonPressed(0)) {
        if (inventory.slot[inventory.selectedIndex]) {
            switch (inventory.slot[inventory.selectedIndex]->type) {
            case Hoe:
                allowInput = false;
                animCont = animMax;
                SpriteCont = 4;
                break;
            case WateringCan:
                allowInput = false;
                animCont = animMax;
                SpriteCont = 8;
                break;
            }
            inventory.slot[inventory.selectedIndex]->UseItem(camera, position, map, &facingDir);
            if (inventory.slot[inventory.selectedIndex]->currStack == 0) {
                delete inventory.slot[inventory.selectedIndex];
                inventory.slot[inventory.selectedIndex] = nullptr;
            }
        }
    }
    else if (IsMouseButtonPressed(1)) {
        map.tiles[TileIndex(camera, position, map, &facingDir, true)].Interact(&inventory, &map);
    }

}

void Player::Render() {
    if (allowInput) {
        if (isMoving && SpriteCont < 2)
        {
            SpriteCont = 2;
            animCont = animMax;
        }
        if (!isMoving && SpriteCont > 1)
        {
            SpriteCont = 0;
            animCont = animMax;
        }
    }

    if (animCont > 0) {
        animCont--;
        //std::cout << "animCont decrease" << std::endl;
    }
    else {

        if (SpriteCont % 2 == 0)
        {
            SpriteCont++;
        }
        else if (SpriteCont < 4)
        {
            SpriteCont--;
            
        }
        else 
        {
            SpriteCont = 0;
            allowInput = true;
        }
        animCont = animMax;
    }

    DrawTextureRec(spriteSheet, Rectangle{ SpriteCont * spriteSize,float(facingDir * spriteSize),spriteSize,spriteSize }, Vector2{ position.x - spriteSize / 2,position.y - spriteSize * 0.62f }, WHITE);
    //render
    
}

