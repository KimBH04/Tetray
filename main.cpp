#include <iostream>
#include "raylib.h"
#include "tetray.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

const int BLOCK_SIZE = 20;
const int OFFSET_X = 50;
const int OFFSET_Y = 0;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);

reGame:
    tet::Init();
    tet::Run();

    while (!WindowShouldClose())
    {
        if (tet::IsEnd())
        {
            if (IsKeyPressed(KEY_R))
            {
                std::cout << "Re" << std::endl;
                goto reGame;
            }
        }

        {
            if (IsKeyPressed(KEY_LEFT))
            {
                std::cout << "Left" << std::endl;
                tet::TryMove(LEFT);
            }

            if (IsKeyPressed(KEY_RIGHT))
            {
                std::cout << "Right" << std::endl;
                tet::TryMove(RIGHT);
            }

            if (IsKeyPressed(KEY_A))
            {
                std::cout << "Left rotate" << std::endl;
                tet::TryRotate(LEFT);
            }

            if (IsKeyPressed(KEY_S))
            {
                std::cout << "Half rotate" << std::endl;
                tet::TryRotate(HALF);
            }

            if (IsKeyPressed(KEY_D))
            {
                std::cout << "Right rotate" << std::endl;
                tet::TryRotate(RIGHT);
            }
        }

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            for (byte r = 0; r < tet::BOARD_DEPTH; r++)
            {
                for (byte c = 0; c < tet::BOARD_WIDTH; c++)
                {
                    DrawRectangle(c * BLOCK_SIZE + OFFSET_X, r * BLOCK_SIZE + OFFSET_Y, BLOCK_SIZE, BLOCK_SIZE, tet::GetColor(r, c));
                }
            }

            // DrawLine(0, tet::BOARD_DEPTH * BLOCK_SIZE, tet::BOARD_WIDTH * BLOCK_SIZE, tet::BOARD_DEPTH * BLOCK_SIZE, WHITE);
            // DrawLine(tet::BOARD_WIDTH * BLOCK_SIZE, 0, tet::BOARD_WIDTH * BLOCK_SIZE, tet::BOARD_DEPTH * BLOCK_SIZE, WHITE);
        }
        EndDrawing();
    }

    CloseWindow();
    tet::Stop();
    return 0;
}