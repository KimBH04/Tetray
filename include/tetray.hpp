#pragma once

#include "raylib.h"

#define MINO_NONE   (byte)0
#define Z_MINO      (byte)1
#define L_MINO      (byte)2
#define O_MINO      (byte)3
#define S_MINO      (byte)4
#define I_MINO      (byte)5
#define J_MINO      (byte)6
#define T_MINO      (byte)7

#define LEFT    (sbyte)-1
#define RIGHT   (sbyte) 1
#define HALF    (sbyte) 2

typedef unsigned char byte;
typedef signed char sbyte;

namespace tet
{
    static constexpr sbyte BOARD_DEPTH = 25;
    static constexpr sbyte BOARD_WIDTH = 10;

    void Init();

    void Run();

    void Stop();

    void TryMove(sbyte to);

    void TryRotate(sbyte to);

    Color GetColor(sbyte row, sbyte column);
} // namespace tet
