#include <iostream>
#include "raylib.h"
#include "tetray.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

const int FPS = 30;

const int BLOCK_SIZE = 20;
const int OFFSET_X = 50;
const int OFFSET_Y = 0;

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);
    SetTargetFPS(FPS);

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
        else
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

            if (IsKeyDown(KEY_DOWN))
            {
                std::cout << "Soft" << std::endl;
                tet::SoftDrop();
            }

            if (IsKeyPressed(KEY_UP))
            {
                std::cout << "Hard" << std::endl;
                tet::HardDrop();
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

            // ui
            DrawText("[Left] : move left",  275, 405, 20, GRAY);
            DrawText("[Right] : move Righ", 275, 430, 20, GRAY);
            DrawText("[Up] : hard drop",    275, 455, 20, GRAY);
            DrawText("[Down] : soft drop",  275, 480, 20, GRAY);

            DrawText("[A] : spin left",  475, 405, 20, GRAY);
            DrawText("[S] : spin half",  475, 430, 20, GRAY);
            DrawText("[D] : spin right", 475, 455, 20, GRAY);
            DrawText("[R] : replay (when game over)",  475, 480, 20, GRAY);

            // board
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