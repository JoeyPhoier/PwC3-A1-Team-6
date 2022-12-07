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
#include "player.h"


//----------------------------------------------------------------------------------
// Custom Classes declaration
//----------------------------------------------------------------------------------



class Screen {
public:
    int windowedWidth;
    int windowedHeight;
    int currWidth;
    int currHeight;
    float unscalezoom = 1.0f;
    float maxzoom = 3.0f;
    float minzoom = 0.25f;
};

//----------------------------------------------------------------------------------
// Module functions declaration
//----------------------------------------------------------------------------------

void PlayerUpdate(Player& player, Map& map, Camera2D& camera);
void CameraUpdate(Camera2D& camera, Player& player, Screen& screen);
void DrawGui(Screen& screen, Player& player);

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------

    Screen screen{
        .windowedWidth = 960,
        .windowedHeight = 540,
    };

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

    player.inventory.AddItem(201);
    player.inventory.AddItem(202);
    player.inventory.AddItem(2, 5);
    player.inventory.AddItem(102, 10);



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

        

        PlayerUpdate(player, map, camera);
        CameraUpdate(camera, player, screen);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        map.render(&camera);
        DrawRectangle(player.position.x-10, player.position.y-10, 20,20, RED);
        
        EndMode2D();

        DrawGui(screen, player);

        EndDrawing();
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

void PlayerUpdate(Player& player, Map& map, Camera2D& camera) {     //Could eventually add deltaTime if performance is affected
    Vector2 movDir = Vector2(0, 0);                                 //Have to test whether local var and player stored var with
    if (IsKeyDown(KEY_A)) { movDir.x--; player.facingDir = Left; }  //point lookup is more efficient.
    if (IsKeyDown(KEY_D)) { movDir.x++; player.facingDir = Right; }
    if (IsKeyDown(KEY_W)) { movDir.y--; player.facingDir = Up; }
    if (IsKeyDown(KEY_S)) { movDir.y++; player.facingDir = Down; }
    if (movDir.x != 0 || movDir.y != 0) {
        NormalVect(movDir);                            
        player.position.x += movDir.x * player.walkingspeed;
        player.position.y += movDir.y * player.walkingspeed;
    }

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

    if (IsMouseButtonPressed(0)) {
        if (player.inventory.slot[player.inventory.selectedIndex]) {
            player.inventory.slot[player.inventory.selectedIndex]->UseItem(camera, map, player.facingDir);
            if (player.inventory.slot[player.inventory.selectedIndex]->currStack == 0) {
                delete player.inventory.slot[player.inventory.selectedIndex];
                player.inventory.slot[player.inventory.selectedIndex] = nullptr;
            }


        }
    }

    if(IsKeyUp(KEY_LEFT_CONTROL)) player.inventory.selectedIndex -= GetMouseWheelMove();  //Probably needs some smoothing
    if (player.inventory.selectedIndex > 9) player.inventory.selectedIndex = 0;
    else if (player.inventory.selectedIndex < 0) player.inventory.selectedIndex = 9;
}

void CameraUpdate(Camera2D& camera, Player& player, Screen& screen) {
    camera.offset = Vector2(screen.currWidth/2, screen.currHeight / 2);
    camera.target = player.position;
    
    if(IsKeyDown(KEY_LEFT_CONTROL)) screen.unscalezoom += (GetMouseWheelMove() * 0.05f);
    float screenscale = (screen.currWidth / screen.windowedWidth);
    if (screen.unscalezoom > screen.maxzoom) screen.unscalezoom = screen.maxzoom;
    else if (screen.unscalezoom < screen.minzoom) screen.unscalezoom = screen.minzoom;

    camera.zoom = screen.unscalezoom * screenscale;               //Breaking down zoom into unscale zoom and screenscale ensures
}                                                                 //the same viewport is rendered independent of window size.

void DrawGui(Screen& screen, Player& player) {
    for(int i = 0; i < 10; i++) {
        if (player.inventory.selectedIndex == i) {
            DrawRectangle(screen.currWidth / 2 + (60 * i) - 295, screen.currHeight - 60, 50, 50, ORANGE);
            DrawRectangle(screen.currWidth / 2 + (60 * i) - 290, screen.currHeight - 55, 40, 40, BROWN);
        }   
        else{
            DrawRectangle(screen.currWidth / 2 + (60 * i) - 295, screen.currHeight - 60, 50, 50, BROWN);
            DrawRectangle(screen.currWidth / 2 + (60 * i) - 290, screen.currHeight - 55, 40, 40, DARKBROWN);
        }
        if (player.inventory.slot[i] != nullptr) {
            DrawTexture(player.inventory.slot[i]->sprite, screen.currWidth / 2 + (60 * i) - 290, screen.currHeight - 55, WHITE);
            if (player.inventory.slot[i]->currStack > 1) {
                int spacing = 0;                                //Might be better to show text as img, in order to indent it to the right.
                if (player.inventory.slot[i]->currStack > 100) spacing = 2;     //Using log() would make the code simpler, but slower.
                else if (player.inventory.slot[i]->currStack > 10) spacing = 1;
                DrawText(TextFormat("%i", player.inventory.slot[i]->currStack),screen.currWidth / 2 + (60 * i) - 259 - (7 * spacing), screen.currHeight - 28, 12, WHITE);
            }
        }
    }
   

}