#include "player.h"
#include "JCC_tools.h"

void Player::PlayerUpdate(Player& player, Map& map, Camera2D& camera) {

    //First the functions that ignore "allowInput".

    if (IsKeyPressed(KEY_ONE)) player.inventory.selectedIndex = 0;
    else if (IsKeyPressed(KEY_TWO)) player.inventory.selectedIndex = 1;
    else if (IsKeyPressed(KEY_THREE)) player.inventory.selectedIndex = 2;
    else if (IsKeyPressed(KEY_FOUR)) player.inventory.selectedIndex = 3;
    else if (IsKeyPressed(KEY_FIVE)) player.inventory.selectedIndex = 4;
    else if (IsKeyPressed(KEY_SIX)) player.inventory.selectedIndex = 5;
    else if (IsKeyPressed(KEY_SEVEN)) player.inventory.selectedIndex = 6;
    else if (IsKeyPressed(KEY_EIGHT)) player.inventory.selectedIndex = 7;
    else if (IsKeyPressed(KEY_NINE)) player.inventory.selectedIndex = 8;
    else if (IsKeyPressed(KEY_ZERO)) player.inventory.selectedIndex = 9;


    if (IsKeyUp(KEY_LEFT_CONTROL)) player.inventory.selectedIndex -= GetMouseWheelMove();  //Probably needs some smoothing
    if (player.inventory.selectedIndex > 9) player.inventory.selectedIndex = 0;
    else if (player.inventory.selectedIndex < 0) player.inventory.selectedIndex = 9;

    if (!player.allowInput) return; //Anything below this point will be skipped if mid animation.

    Vector2 movDir = Vector2(0, 0);                                 //Could eventually add deltaTime if performance is affected
    if (IsKeyDown(KEY_A)) { movDir.x--; player.facingDir = Left; }  //Have to test whether local var and player stored var with
    if (IsKeyDown(KEY_D)) { movDir.x++; player.facingDir = Right; } //point lookup is more efficient.
    if (IsKeyDown(KEY_W)) { movDir.y--; player.facingDir = Up; }
    if (IsKeyDown(KEY_S)) { movDir.y++; player.facingDir = Down; }
    if (IsKeyUp(KEY_A) && IsKeyUp(KEY_S) && IsKeyUp(KEY_D) && IsKeyUp(KEY_W)) player.isMoving = false;
    else player.isMoving = true;

    if ((movDir.x != 0 || movDir.y != 0) && player.isMoving) {
        NormalVect(movDir);                            
        player.position.x += movDir.x * player.walkingspeed;
        player.position.y += movDir.y * player.walkingspeed;
    }
    
    if (IsMouseButtonPressed(0)) {
        if (player.inventory.slot[player.inventory.selectedIndex]) {
            switch (player.inventory.slot[player.inventory.selectedIndex]->type) {
            case Hoe:
                player.allowInput = false;
                player.animCont = player.animMax;
                player.SpriteCont = 4;
                break;
            case WateringCan:
                player.allowInput = false;
                player.animCont = player.animMax;
                player.SpriteCont = 8;
                break;
            }
            player.inventory.slot[player.inventory.selectedIndex]->UseItem(camera, map, &player.facingDir);
            if (player.inventory.slot[player.inventory.selectedIndex]->currStack == 0) {
                delete player.inventory.slot[player.inventory.selectedIndex];
                player.inventory.slot[player.inventory.selectedIndex] = nullptr;
            }
        }
    }

}

void Player::RenderPlayer(Player& player) {
    if (player.allowInput) {
        if (player.isMoving && player.SpriteCont < 2)
        {
            player.SpriteCont = 2;
            player.animCont = player.animMax;
        }
        if (!player.isMoving && player.SpriteCont > 1)
        {
            player.SpriteCont = 0;
            player.animCont = player.animMax;
        }
    }

    if (player.animCont > 0) {
        player.animCont--;
        //std::cout << "animCont decrease" << std::endl;
    }
    else {

        if (player.SpriteCont % 2 == 0)
        {
            player.SpriteCont++;
        }
        else if (player.SpriteCont < 4)
        {
            player.SpriteCont--;
            
        }
        else 
        {
            player.SpriteCont = 0;
            player.allowInput = true;
        }
        player.animCont = player.animMax;
    }

    DrawTextureRec(player.spriteSheet, Rectangle{ player.SpriteCont * player.spriteSize,float(player.facingDir * player.spriteSize),player.spriteSize,player.spriteSize }, Vector2{ player.position.x - player.spriteSize / 2,player.position.y - player.spriteSize * 0.62f }, WHITE);
    //render
    
}

