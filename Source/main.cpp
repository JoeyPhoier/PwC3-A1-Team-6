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

        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}