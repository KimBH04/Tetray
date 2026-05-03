#include <iostream>
#include "raylib.h"
#include "tetray.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

const int BLOCK_SIZE = 20;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    tet::Init();
    tet::Run();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        {
        ClearBackground(BLACK);

            DrawLine(0, tet::BOARD_DEPTH * BLOCK_SIZE, tet::BOARD_WIDTH * BLOCK_SIZE, tet::BOARD_DEPTH * BLOCK_SIZE, WHITE);

        for (byte r = 0; r < tet::BOARD_DEPTH; r++)
        {
            for (byte c = 0; c < tet::BOARD_WIDTH; c++)
            {
                    DrawRectangle(c * BLOCK_SIZE, r * BLOCK_SIZE, BLOCK_SIZE, BLOCK_SIZE, tet::GetColor(r, c));
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    tet::Stop();
    return 0;
}