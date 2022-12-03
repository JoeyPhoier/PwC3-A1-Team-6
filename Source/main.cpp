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
#include "JCC_tools.h"
#include "Inventory.h"
#include "map.h"


//----------------------------------------------------------------------------------
// Custom Classes declaration
//----------------------------------------------------------------------------------

class Player {
public:
    Vector2 position = Vector2(0, 0);
    Texture2D sprite;
    float walkingspeed = 5.0f;
    
};

class Screen {
public:
    int windowedWidth;
    int windowedHeight;
    int currWidth;
    int currHeight;
    float unscalezoom;
};

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------

void PlayerUpdate(Player* player);
void CameraUpdate(Camera2D* camera, Player * player, Screen* screen);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------

    Screen screen{
        screen.windowedWidth = 960,
        screen.windowedHeight = 540,
    };
    
    screen.unscalezoom = 1.0f;


    InitWindow(screen.windowedWidth, screen.windowedHeight, "Team 6 - Temp Title");

    screen.currWidth = screen.windowedWidth;
    screen.currHeight = screen.windowedHeight;

    Image test = LoadImage("assets/test.png");
    Texture2D groundtexture = LoadTexture("assets/ground.png");
    Texture2D grid2D = LoadTexture("assets/gridbackground.png");

    SetWindowIcon(test);

    Map map;
    

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------
    
    Camera2D camera = { 0 };
    Player player;

    camera.zoom = 1.0f;

    
   
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {   
        
        if ( IsKeyPressed(KEY_F11) || (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))) {
            int currDisplay = GetCurrentMonitor();
            if (!IsWindowFullscreen()) {
                SetWindowMonitor(currDisplay);
                screen.currWidth = GetMonitorWidth(currDisplay);
                screen.currHeight = GetMonitorHeight(currDisplay);
                ToggleFullscreen();
            }
            else {
                ToggleFullscreen();
                SetWindowSize(screen.windowedWidth, screen.windowedHeight);
                screen.currWidth = screen.windowedWidth;
                screen.currHeight = screen.windowedHeight;
                SetWindowPosition((GetMonitorWidth(currDisplay) - screen.windowedWidth) / 2,
                                (GetMonitorHeight(currDisplay) - screen.windowedHeight) / 2);
            }
            
        }

        

        PlayerUpdate(&player);
        CameraUpdate(&camera, &player, &screen);
        
        BeginDrawing();
        BeginMode2D(camera);
        
        ClearBackground(RAYWHITE);
        map.render(&camera);
        DrawRectangle(player.position.x-10.0f, player.position.y-10.0f, 20,20, RED);

        EndMode2D();
        EndDrawing();
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void PlayerUpdate(Player* player) {                                 //Could eventually add deltaTime if performance is affected
    Vector2 movDir = Vector2(0, 0);                                 //Have to test whether local var and player stored var with
    if (IsKeyDown(KEY_A)) movDir.x--;                               //point lookup is more efficient.
    if (IsKeyDown(KEY_D)) movDir.x++;
    if (IsKeyDown(KEY_W)) movDir.y--;
    if (IsKeyDown(KEY_S)) movDir.y++;
    if (movDir.x != 0 || movDir.y != 0) {
        movDir = NormalVect(movDir);
        player->position.x += movDir.x * player->walkingspeed;
        player->position.y += movDir.y * player->walkingspeed;
    }
}

void CameraUpdate(Camera2D* camera, Player* player, Screen* screen) {
    camera->offset = Vector2(screen->currWidth/2, screen->currHeight / 2);
    camera->target = player->position;
            
    screen->unscalezoom += (GetMouseWheelMove() * 0.05f);
    float screenscale = (screen->currWidth / screen->windowedWidth);
    if (screen->unscalezoom > 3.0f) screen->unscalezoom = 3.0f;
    else if (screen->unscalezoom < 0.25f) screen->unscalezoom = 0.25f;

    camera->zoom = screen->unscalezoom * screenscale;
}