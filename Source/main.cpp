/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   Example originally created with raylib 1.0, last time updated with raylib 1.0
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2013-2022 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"
#include <iostream>

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

class Player {
    Vector2 pos;
};

class Stage {
public:
    int groundscroll=0;
    int backgroundscroll=0;
};

class Entity {
public:
    int health = 0;
    Vector2 pos = { 0,0 };
    Vector2 speed = { 0,0 };
    Vector2 accel = { 0,0 };
    const Vector2 maxaccel = { 3,3 };
    const Vector2 maxspeed = { 40,50 };

    void movement(){
        speed.x += accel.x;
        if (speed.x > maxspeed.x)speed.x = maxspeed.x;
        else if (speed.x < -maxspeed.x)speed.x = -maxspeed.x;
        speed.y += accel.y;
        pos.x += speed.x;
        pos.y += speed.y;
    }
};

Stage stage1;

void PlayerInput(Entity* player);

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------

    const int appWidth = 800;
    const int appHeight = 450;

    InitWindow(appWidth, appHeight, "Team 6 - Temp Title");

    Image test = LoadImage("assets/test.png");
    Texture2D groundtexture = LoadTexture("assets/ground.png");
    SetWindowIcon(test);

    SetTargetFPS(60);

    Entity Rob;
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        int currDisplay = GetCurrentMonitor();
        if ( IsKeyPressed(KEY_F11) || (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))) {
            
            if (!IsWindowFullscreen()) {
                SetWindowMonitor(currDisplay);
                ToggleFullscreen();
            }
            else {
                ToggleFullscreen();
                const int monitorWidth = GetMonitorWidth(currDisplay);
                const int monitorHeight = GetMonitorHeight(currDisplay);
                SetWindowSize(appWidth, appHeight);
                SetWindowPosition((monitorWidth - appWidth) / 2, (monitorHeight - appHeight) / 2);
            }
            
        }
        stage1.groundscroll-= 10;
        if (stage1.groundscroll <= -groundtexture.width) stage1.groundscroll = 0;

        PlayerInput(&Rob);

        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        

        Rectangle source = { Rob.pos.x, 0, groundtexture.width, groundtexture.height };
        Rectangle dest = { 0, 680 * (GetScreenHeight() / 1080.0f), (GetScreenWidth() / 1920.0f) * groundtexture.width, (GetScreenHeight() / 1080.0f) * groundtexture.height};

        DrawTexturePro(groundtexture, source, dest, Vector2{0,0}, 0, WHITE);


        //DrawTexture(groundtexture, stage1.groundscroll, 0, WHITE);
        //DrawTexture(groundtexture, groundtexture.width + stage1.groundscroll, 0, WHITE);
        
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void PlayerInput(Entity* player){
    if (IsKeyDown(KEY_A) && !IsKeyDown(KEY_D)) {
        player->accel.x = -player->maxaccel.x;
        if (player->speed.x > player->maxspeed.x * .35) player->speed.x = -player->maxspeed.x * .35;
    }
    else if (IsKeyDown(KEY_D) && !IsKeyDown(KEY_A)) {
        player->accel.x = player->maxaccel.x;
        if (player->speed.x < -player->maxspeed.x * .35) player->speed.x = player->maxspeed.x * .35;
    }
    else {
        if (player->speed.x > 0)
        {
            if (player->maxaccel.x/2 > player->speed.x)player->accel.x = -player->speed.x;
            else player->accel.x = -player->maxaccel.x / 2;

        }
        else if (player->speed.x < 0)
        {
            if (player->maxaccel.x / 2 > -player->speed.x)player->accel.x = -player->speed.x;
            else player->accel.x = player->maxaccel.x / 2;
        }
        else player->accel.x = 0;
    }
    player->movement();
}