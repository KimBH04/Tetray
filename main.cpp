#include <iostream>
#include "raylib.h"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground((Color){ 10, 20, 30 });

        DrawText("Hello, Raylib!", 100, 100, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}