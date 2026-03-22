#pragma once

#include "raylib.h"

#define MINO_NONE   0
#define Z_MINO      1
#define L_MINO      2
#define O_MINO      3
#define S_MINO      4
#define I_MINO      5
#define J_MINO      6
#define T_MINO      7

typedef unsigned char byte;

namespace tet
{
    namespace shape
    {
        constexpr byte SHAPE_COUNT = 8;
        constexpr byte SHAPE_ROTATE_COUNT = 4;
        constexpr byte SHAPE_SIZE = 5;

        const Color colors[] = { BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, DARKBLUE, PURPLE };
        constexpr byte shapes[SHAPE_COUNT][SHAPE_ROTATE_COUNT][SHAPE_SIZE] =
        {
            { { 0, }, { 0, }, { 0, }, { 0, }, }, // NONE
            { // Z
                {
                    0b00000,
                    0b01100,
                    0b00110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00010,
                    0b00110,
                    0b00100,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b01100,
                    0b00110,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b01100,
                    0b01000,
                    0b00000,
                },
            },
            { // L
                {
                    0b00000,
                    0b00010,
                    0b01110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b00100,
                    0b00110,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b01110,
                    0b01000,
                    0b00000,
                },
                {
                    0b00000,
                    0b01100,
                    0b00100,
                    0b00100,
                    0b00000,
                },
            },
            { // O
                {
                    0b00000,
                    0b00110,
                    0b00110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b00110,
                    0b00110,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b01100,
                    0b01100,
                    0b00000,
                },
                {
                    0b00000,
                    0b01100,
                    0b01100,
                    0b00000,
                    0b00000,
                },
            },
            { // S
                {
                    0b00000,
                    0b00110,
                    0b01100,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b00110,
                    0b00010,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b00110,
                    0b01100,
                    0b00000,
                },
                {
                    0b00000,
                    0b01000,
                    0b01100,
                    0b00100,
                    0b00000,
                },
            },
            { // I
                {
                    0b00000,
                    0b00000,
                    0b01111,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b00100,
                    0b00100,
                    0b00100,
                },
                {
                    0b00000,
                    0b00000,
                    0b11110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00100,
                    0b00100,
                    0b00100,
                    0b00100,
                    0b00000,
                },
            },
            { // J
                {
                    0b00000,
                    0b01000,
                    0b01110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00110,
                    0b00100,
                    0b00100,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b01110,
                    0b00010,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b00100,
                    0b01100,
                    0b00000,
                },
            },
            { // T
                {
                    0b00000,
                    0b00100,
                    0b01110,
                    0b00000,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b00110,
                    0b00100,
                    0b00000,
                },
                {
                    0b00000,
                    0b00000,
                    0b01110,
                    0b00100,
                    0b00000,
                },
                {
                    0b00000,
                    0b00100,
                    0b01100,
                    0b00100,
                    0b00000,
                },
            },
        };
    } // namespace shapes

    using namespace shape;

} // namespace tet
