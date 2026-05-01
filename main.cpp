#include <iostream>
#include "raylib.h"
#include "tetray.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    tet::Init();
    tet::Run();

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(BLACK);

        DrawLine(0, tet::BOARD_DEPTH * 10, tet::BOARD_WIDTH * 10, tet::BOARD_DEPTH * 10, WHITE);

        for (byte r = 0; r < tet::BOARD_DEPTH; r++)
        {
            for (byte c = 0; c < tet::BOARD_WIDTH; c++)
            {
                DrawRectangle(c * 10, r * 10, 10, 10, tet::GetColor(r, c));
            }
        }

        EndDrawing();
    }

    CloseWindow();
    tet::Stop();
    return 0;
}