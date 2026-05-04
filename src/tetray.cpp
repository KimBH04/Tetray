#include <iostream>

#include <queue>
#include <span>

#include <thread>
#include <atomic>
#include <mutex>

#include "tetray.hpp"

namespace tet
{
    namespace shape
    {
        static constexpr sbyte SHAPE_COUNT = 8;
        static constexpr sbyte SHAPE_ROTATE_COUNT = 4;
        static constexpr sbyte SHAPE_SIZE = 5;
        static constexpr sbyte SHAPE_OFFSET = SHAPE_SIZE / 2;
        static constexpr sbyte SHAPE_LINE_MASK = 0b11111;

        static const Color colors[] = { BLACK, RED, ORANGE, YELLOW, GREEN, BLUE, DARKBLUE, PURPLE };
        static constexpr int shapes[SHAPE_COUNT][SHAPE_ROTATE_COUNT] =
        {
            { 0, 0, 0, 0, }, // NONE
            { // Z
                0b00000'00010'00110'00100'00000,
                0b00000'00000'01100'00110'00000,
                0b00000'00100'01100'01000'00000,
                0b00000'01100'00110'00000'00000,
            },
            { // L
                0b00000'00100'00100'00110'00000,
                0b00000'00000'01110'01000'00000,
                0b00000'01100'00100'00100'00000,
                0b00000'00010'01110'00000'00000,
            },
            { // O
                0b00000'00000'00110'00110'00000,
                0b00000'00000'01100'01100'00000,
                0b00000'01100'01100'00000'00000,
                0b00000'00110'00110'00000'00000,
            },
            { // S
                0b00000'00100'00110'00010'00000,
                0b00000'00000'00110'01100'00000,
                0b00000'01000'01100'00100'00000,
                0b00000'00110'01100'00000'00000,
            },
            { // I
                0b00000'00100'00100'00100'00100,
                0b00000'00000'11110'00000'00000,
                0b00100'00100'00100'00100'00000,
                0b00000'00000'01111'00000'00000,
            },
            { // J
                0b00000'00110'00100'00100'00000,
                0b00000'00000'01110'00010'00000,
                0b00000'00100'00100'01100'00000,
                0b00000'01000'01110'00000'00000,
            },
            { // T
                0b00000'00100'00110'00100'00000,
                0b00000'00000'01110'00100'00000,
                0b00000'00100'01100'00100'00000,
                0b00000'00100'01110'00000'00000,
            },
        };
    
        // Super Rotation System offsets
        static constexpr std::pair<sbyte, sbyte> JLSTZOffset[][5] =
        {   // J L S T Z
            {   // l
                {  0,  0 }, { -1,  0 }, { -1, -1 }, {  0,  2 }, { -1,  2 },
            },
            {   // 0
                {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
            },
            {   // r
                {  0,  0 }, {  1,  0 }, {  1, -1 }, {  0,  2 }, {  1,  2 },
            },
            {   // 2
                {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 }, {  0,  0 },
            },
        };
        
        static constexpr std::pair<sbyte, sbyte> OOffset[][1] =
        {   // O
            {   // l
                { -1,  0 },
            },
            {   // 0
                {  0,  0 },
            },
            {   // r
                {  0, -1 },
            },
            {   // 2
                { -1, -1 },
            },
        };
    
        static constexpr std::pair<sbyte, sbyte> IOffset[][5] =
        {   // I
            {   // l
                {  0,  1 }, {  0,  1 }, {  0,  1 }, {  0, -1 }, {  0,  2 },
            },
            {   // 0
                {  0,  0 }, { -1,  0 }, {  2,  0 }, { -1,  0 }, {  2,  0 },
            },
            {   // r
                { -1,  0 }, {  0,  0 }, {  0,  0 }, {  0,  1 }, {  0, -2 },
            },
            {   // 2
                { -1,  1 }, {  1,  1 }, { -2,  1 }, {  1,  0 }, { -2,  0 },
            },
        };

    } // namespace shapes

    using namespace shape;

    //  -         +
    //  ---------->
    //  width       | - depth
    //  (bit)       |
    //              |
    //              v +

    static constexpr sbyte CREATE_DEPTH = 5;
    static constexpr sbyte MASK_COUNT = 3;
    static constexpr int COLOR_MASK = 0b111;
    static constexpr int INITIALIZE = 0b11'000'000'000'000'000'000'000'000'000'000;

    static std::mutex dataMutex;
    static int coloredBoard[BOARD_DEPTH];

    static std::jthread runner;

    static constexpr unsigned long long RATE = 5ULL;
    static std::atomic<bool> isRun(false);
    static std::atomic<unsigned long long> elapsed(0ULL);

    static byte currentShape = 1;
    static byte currentRotate = 1;
    static sbyte currentBitCount = BOARD_WIDTH / 2;
    static sbyte currentDepth = CREATE_DEPTH;

    static byte next = 1;

    static inline long long getBitSlicing(long long value, const byte &begin, const byte &end)
    {
        if (begin >= end)
            return 0LL;

        value >>= begin;

        if (end >= sizeof(long long) * 8UL)
            return value;

        return value & ((1LL << (end - begin)) - 1LL);
    }

    static inline std::span<const std::pair<sbyte, sbyte>> getSRSOffset(const byte &minoType, const byte &rotate)
    {
        if (rotate >= 4)
        {
            std::cerr << "[getSRSOffset] : " << rotate << " is 4 or more";
            return {};
        }

        switch (minoType)
        {
        case J_MINO:
        case L_MINO:
        case S_MINO:
        case T_MINO:
        case Z_MINO:
            return JLSTZOffset[rotate];
        
        case O_MINO:
            return OOffset[rotate];
        
        case I_MINO:
            return IOffset[rotate];

        default:
            std::cerr << "[getSRSOffset] : " << minoType << " is not defined";
            return {};
        }
    }

    static bool placeable(const sbyte &bitCount, const sbyte &depth, const byte &shapeIndex, const byte &shapeRotateIndex)
    {
        if (bitCount < 0 || bitCount >= BOARD_WIDTH || depth < 0 || depth >= BOARD_DEPTH)
            return false;

        int shape = shapes[shapeIndex][shapeRotateIndex];
        for (sbyte i = -SHAPE_OFFSET; i <= SHAPE_OFFSET; i++)
        {
            // if (depth + i < CREATE_DEPTH)
            //     continue;

            auto shapeLine = getBitSlicing(shape, (SHAPE_OFFSET + i) * SHAPE_SIZE, (SHAPE_OFFSET + i + 1) * SHAPE_SIZE);
            if (shapeLine == 0)
                continue;

            // v  when shape line is not empty  v

            if (depth + i < 0 || depth + i >= BOARD_DEPTH)
                return false;

            int line = coloredBoard[depth + i];
            for (sbyte j = -SHAPE_OFFSET; j <= SHAPE_OFFSET; j++)
            {
                auto shapeBit = (shapeLine >> (SHAPE_OFFSET + j)) & 1;
                if (shapeBit == 0)
                    continue;

                // v  when shape bit is not empty  v

                if (bitCount + j < 0 || bitCount + j >= BOARD_WIDTH)
                    return false;

                auto block = getBitSlicing(line, (bitCount + j) * MASK_COUNT, (bitCount + j + 1) * MASK_COUNT);
                if (block != 0 && shapeBit != 0)
                    return false;
            }
        }

        return true;
    }

    static void draw(const sbyte &bitCount, const sbyte &depth, const byte &shapeIndex, const byte &shapeRotateIndex)
    {
        for (sbyte i = -SHAPE_OFFSET; i <= SHAPE_OFFSET; i++)
        {
            if (depth + i < 0 || depth + i >= BOARD_DEPTH)
                continue;

            auto shapeLine =
                getBitSlicing(
                    shapes[shapeIndex][shapeRotateIndex],
                    (SHAPE_OFFSET + i) * SHAPE_SIZE,
                    (SHAPE_OFFSET + i + 1) * SHAPE_SIZE
                );
            if (shapeLine == 0)
                continue;

            for (sbyte j = -SHAPE_OFFSET; j <= SHAPE_OFFSET; j++)
            {
                if (bitCount + j < 0 || bitCount + j >= BOARD_WIDTH)
                    continue;

                if (((shapeLine >> (SHAPE_OFFSET + j)) & 1) == 0)
                    continue;

                // if (erase)
                //     coloredBoard[depth + i] &= 0x3FFFFFFF ^ ((int)COLOR_MASK << ((bitCount + j) * MASK_COUNT));
                // else
                    coloredBoard[depth + i] |= (int)shapeIndex << ((bitCount + j) * MASK_COUNT);
            }
        }
    }

    static void run()
    {
        while (isRun)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000 / RATE));

            std::lock_guard<std::mutex> lock(dataMutex);

            if (placeable(currentBitCount, currentDepth + 1, currentShape, currentRotate))
            {
                currentDepth++;
            }
            else
            {
                draw(currentBitCount, currentDepth, currentShape, currentRotate);

                currentShape = std::rand() % 7 + 1;
                currentRotate = 1;
                currentBitCount = BOARD_WIDTH / 2;
                currentDepth = CREATE_DEPTH;
            }

            elapsed++;
        }
    }

    void Init()
    {
        elapsed = 0ULL;
        for (int i = 0; i < BOARD_DEPTH; i++)
        {
            coloredBoard[i] = INITIALIZE;
        }
    }

    void Run()
    {
        if (isRun)
        {
            return;
        }

        isRun = true;
        runner = std::jthread(run);
        std::cout << "Start running" << std::endl;
    }

    void Stop()
    {
        isRun = false;
    }

    void TryMove(sbyte to)
    {
        std::lock_guard<std::mutex> lock(dataMutex);
        if (placeable(currentBitCount + to, currentDepth, currentShape, currentRotate))
        {
            currentBitCount += to;
        }
    }

    void TryRotate(sbyte rotate)
    {
        std::lock_guard<std::mutex> lock(dataMutex);

        byte rotateTo = (currentRotate + 4 + rotate) & 0b11;
        auto from = getSRSOffset(currentShape, currentRotate);
        auto to   = getSRSOffset(currentShape, rotateTo);
        auto size = from.size();
        for (size_t i = 0; i < size; i++)
        {
            sbyte x = from[i].first  - to[i].first;
            sbyte y = -from[i].second + to[i].second;
            if (placeable(currentBitCount + x, currentDepth + y, currentShape, rotateTo))
            {
                currentBitCount += x;
                currentDepth += y;
                currentRotate = rotateTo;
                break;
            }
        }
    }

    Color GetColor(sbyte row, sbyte column)
    {
        if (row < 0 || column < 0 || row >= BOARD_DEPTH || column >= BOARD_WIDTH)
        {
            return BLACK;
        }

        std::lock_guard<std::mutex> b_lock(dataMutex);
        auto value = getBitSlicing(coloredBoard[row], column * MASK_COUNT, (column + 1) * MASK_COUNT);
        if (value == 0)
        {
            auto rowDiff = row - currentDepth, colDiff = column - currentBitCount;
            if (std::abs(rowDiff) <= 2 && std::abs(colDiff) <= 2)
            {
                auto line =
                    getBitSlicing(
                        shapes[currentShape][currentRotate],
                        (SHAPE_OFFSET + rowDiff) * SHAPE_SIZE,
                        (SHAPE_OFFSET + rowDiff + 1) * SHAPE_SIZE
                    );
                
                value = ((line >> (SHAPE_OFFSET + colDiff)) & 1) * currentShape;
            }
        }
        return colors[value];
    }
} // namespace tet
