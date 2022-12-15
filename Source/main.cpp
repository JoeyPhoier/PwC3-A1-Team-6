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

//void PlayerUpdate(Player& player, Map& map, Camera2D& camera);
void CameraUpdate(Camera2D& camera, Player& player, Screen& screen);
void DrawGui(Screen& screen, Player& player);
//void RenderPlayer(Player& player);

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

    

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------
    
    Camera2D camera = { 0 };
    Player player;
    player.position = Vector2{ (map.tilesX - 1.0f) * map.tileSize/2,(map.tilesY - 1.0f) * map.tileSize/2 };

    player.inventory.AddItem(201);
    player.inventory.AddItem(202);
    player.inventory.AddItem(2, 5);
    player.inventory.AddItem(102, 10);

    Map map(&player);

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
        

        player.Update(map, camera);
        CameraUpdate(camera, player, screen);
        map.RemoveDeadEntities();


        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        map.RenderGround(&camera);
        map.RenderEntities(&camera);
        //player.Render();

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