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
                goto reGame;
            }
        }
        else
        {
            int key = GetKeyPressed();
            switch (key)
            {
            case KEY_LEFT:
                tet::TryMove(LEFT);
                break;

            case KEY_RIGHT:
                tet::TryMove(RIGHT);
                break;

            case KEY_A:
                tet::TryRotate(LEFT);
                break;

            case KEY_D:
                tet::TryRotate(RIGHT);
                break;

            case KEY_S:
                tet::TryRotate(HALF);
                break;

            case KEY_DOWN:
                tet::SoftDrop();
                break;

            case KEY_UP:
                tet::HardDrop();
                break;
            
            default:
                break;
            }

            if (IsKeyPressedRepeat(KEY_LEFT))
            {
                tet::TryMove(LEFT);
            }
            else if (IsKeyPressedRepeat(KEY_RIGHT))
            {
                tet::TryMove(RIGHT);
            }

            if (IsKeyPressedRepeat(KEY_DOWN))
            {
                tet::SoftDrop();
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

            auto highScore = tet::GetHighScore(), score = tet::GetScore();

            char highScoreText[21], scoreText[16];
            sprintf(highScoreText, "High Score\n%lld", highScore);
            sprintf(scoreText, "Score\n%lld", score);

            DrawText(highScoreText, 475, 25, 20, GRAY);
            DrawText(scoreText, 475, 75, 20, GRAY);

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