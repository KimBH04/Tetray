#include <iostream>
#include "raylib.h"
#include "tetray.hpp"

const int WIDTH = 800;
const int HEIGHT = 600;
const char *TITLE = "Tetray";

int main()
{
    InitWindow(WIDTH, HEIGHT, TITLE);

    for (int i = 0; i < tet::SHAPE_COUNT; i++)
    {
        for (int j = 0; j < tet::SHAPE_SIZE; j++)
        {
            for (int k = 0; k < tet::SHAPE_ROTATE_COUNT; k++)
            {
                for (int l = tet::SHAPE_SIZE - 1; l >= 0; l--)
                {
                    std::cout << ((tet::shapes[i][k][j] >> l) & 1 ? "#" : ".");
                }
                std::cout << '\t';
            }
            std::cout << std::endl;
        }
        std::cout << "-----------------------------------------------------" << std::endl;
    }

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