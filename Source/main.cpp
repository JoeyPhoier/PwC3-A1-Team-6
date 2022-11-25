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

Stage stage1;


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

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------
    
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        if ( IsKeyPressed(KEY_F11) || (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))) {
            int currDisplay = GetCurrentMonitor();
            
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
        stage1.groundscroll-= 3;
        if (stage1.groundscroll <= -groundtexture.width) stage1.groundscroll = 0;
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        DrawTexture(groundtexture, stage1.groundscroll, 0, WHITE);
        DrawTexture(groundtexture, groundtexture.width + stage1.groundscroll, 0, WHITE);
        
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}