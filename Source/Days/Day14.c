#include "Day14.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define BITFIELD 2000
static const int CAVE_BITFIELD_SIZE = BITFIELD;
static const int PRINT_REGULARITY = 1000;

typedef struct Pos
{
    int x;
    int y;
} Pos;

typedef struct PairedPos
{
    unsigned long long x;
    int pairingArrayOffset;
} PairedPos;

unsigned long long Szudzik(const Pos pos)
{
    const unsigned long long x = pos.x >= 0 ? pos.x * 2 : pos.x * -2 - 1;
    const unsigned long long y = pos.y >= 0 ? pos.y * 2 : pos.y * -2 - 1;
    const unsigned long long szudzik = (x >= y) ? (x * x + x + y) : (y * y + x);
    return szudzik;
}

PairedPos PairPos(const Pos pos)
{
    unsigned long long szudzik = Szudzik(pos);
    const PairedPos pairedPos = { (unsigned long long)1 << (szudzik), (int)(szudzik / 64) };
    return pairedPos;
}

// returns true if valid
BOOL TestPos(const Pos pos, unsigned long long* bitfield)
{
    PairedPos pairedPos = PairPos(pos);

    if (pairedPos.pairingArrayOffset >= CAVE_BITFIELD_SIZE)
    {
        printf("WARNING OFFSET TOO BIG: Size: %i, Offset: %i.\n", CAVE_BITFIELD_SIZE, pairedPos.pairingArrayOffset);
        return FALSE;
    }

    return !(bitfield[pairedPos.pairingArrayOffset] & pairedPos.x);
}

BOOL PlacePos(const Pos pos, unsigned long long* bitfield)
{
    PairedPos pairedPos = PairPos(pos);

    if (pairedPos.pairingArrayOffset >= CAVE_BITFIELD_SIZE)
    {
        printf("WARNING OFFSET TOO BIG: Size: %i, Offset: %i.\n", CAVE_BITFIELD_SIZE, pairedPos.pairingArrayOffset);
        return FALSE;
    }

    if (!(bitfield[pairedPos.pairingArrayOffset] & pairedPos.x))
    {
        bitfield[pairedPos.pairingArrayOffset] |= pairedPos.x;
        return TRUE;
    }

    return FALSE;
}

void ParseVectors(char* const str, unsigned long long* caveBitfield, int* escapeZone, int* lowestWidth, int* highestWidth)
{
    int start = 0;
    int i = 0;
    int arrowCount = 0;
    Pos lastPos = { 0, 0 };
    
    while(str[i] != '\0')
    {
        const char* arrowStr = strstr(str + i, "->");
        if ((str + i) == arrowStr || str[i + 1] == '\0')
        {
            char* newStr = strdup(str + start);
            strtok(newStr, "->");

            int commaPos = 0;
            while(newStr[commaPos] != '\0')
            {
                if (newStr[commaPos] == ',')
                {
                    break;
                }
                ++commaPos;
            }

            strtok(newStr, ",");

            Pos pos;
            pos.x = atoi(newStr) - 500;
            pos.y = atoi(newStr + commaPos + 1);

            if (pos.y > (*escapeZone))
            {
                (*escapeZone) = pos.y;
            }

            if (pos.x > (*highestWidth))
            {
                (*highestWidth) = pos.x;
            }
			else if (pos.x < (*lowestWidth))
			{
				(*lowestWidth) = pos.x;
			}


            if (lastPos.x != 0 && lastPos.y != 0)
            {
				Pos delta;
				delta.x = pos.x - lastPos.x;
				delta.y = pos.y - lastPos.y;
                delta.x = (delta.x != 0) ? (delta.x / abs(delta.x)) : delta.x;
                delta.y = (delta.y != 0) ? (delta.y / abs(delta.y)) : delta.y;

                printf("%i,%i -> ", lastPos.x + 500, lastPos.y);

                while (lastPos.x != pos.x || lastPos.y != pos.y)
                {
                    PlacePos(lastPos, caveBitfield);

                    lastPos.x += delta.x;
                    lastPos.y += delta.y;
                }
                PlacePos(lastPos, caveBitfield);
                arrowCount++;

                printf("%i,%i\n", lastPos.x + 500, lastPos.y);
            }

            free(newStr);
            start = i + 2;
            lastPos = pos;
        }
        ++i;
    }

    static int line = 0;
    printf("Arrow count for line %i: %i\n", line++, arrowCount);
}

void PrintSandAndCave(unsigned long long* caveBitfield,
    unsigned long long* sandBitfield,
    const Pos sand,
    const int widthStart,
    const int widthEnd,
    const int height)
{
    for (int i = 0; i < height + 1; ++i)
    {
		for (int j = widthStart - 1; j < widthEnd + 1; ++j)
		{
            Pos pos = { j, i };

            if (sand.x == pos.x && sand.y == pos.y)
            {
                printf("\x1B[33mo");
            }
            else if (!TestPos(pos, caveBitfield))
            {
                printf("\033[0m#");
            }
			else if (!TestPos(pos, sandBitfield))
			{
				printf("\033[0mo");
			}
            else
            {
                printf("\033[0m.");
            }
		}
        printf("\033[0m\n");
    }
    printf("\n");
}

void ExecuteDay14_Part1(DayData* dayData)
{
    unsigned long long caveBitfield[BITFIELD];
    unsigned long long sandBitfield[BITFIELD];

    for (int i = 0; i < CAVE_BITFIELD_SIZE; ++i)
    {
        caveBitfield[i] = 0;
        sandBitfield[i] = 0;
    }

    int escapeZone = 0;
    int lowestWidth = 0;
    int highestWidth = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        ParseVectors(dayData->m_Data[i], caveBitfield, &escapeZone, &lowestWidth, &highestWidth);
    }

    int sandCount = 0;
    BOOL hasSandEscaped = FALSE;
    int i = 0;
    while(!hasSandEscaped)
    {
        // 500 x, but we offset by 500 so sshhh..
        Pos sand = { 0, 0 };
        
        BOOL isSandPlaced = FALSE;

        while(!isSandPlaced)
        {
            static const int TEST_COUNT = 3;
            Pos tests[] = { { 0, 1 }, { -1, 1 }, { 1, 1 } };

            BOOL foundPlaceToGo = FALSE;
            for (int i = 0; i < TEST_COUNT; ++i)
            {
                Pos sandTest = sand;
                sandTest.x += tests[i].x;
                sandTest.y += tests[i].y;

                if (TestPos(sandTest, caveBitfield) && TestPos(sandTest, sandBitfield))
                {
                    sand = sandTest;
                    foundPlaceToGo = TRUE;
                    break;
                }
            }

            if (!foundPlaceToGo)
            {
                if (PlacePos(sand, sandBitfield))
                {
                    ++sandCount;
                    //PrintSandAndCave(caveBitfield, sandBitfield, sand, lowestWidth, highestWidth, escapeZone);
                    isSandPlaced = TRUE;
                }
            }

            hasSandEscaped = sand.y > escapeZone;

            if (hasSandEscaped)
            {
                PrintSandAndCave(caveBitfield, sandBitfield, sand, lowestWidth, highestWidth, escapeZone);
                break;
            }

            if (i % PRINT_REGULARITY == 0)
            {
                //PrintSandAndCave(caveBitfield, sandBitfield, sand, lowestWidth, highestWidth, escapeZone);
            }
            ++i;
        }
    }

    
    printf("Sand count after escape: %i\n", sandCount);
}

void ExecuteDay14_Part2(DayData* dayData)
{
    
}

#undef MoveAndTestSand
#undef BITFIELD