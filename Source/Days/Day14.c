#include "Day14.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const int CAVE_BITFIELD_SIZE = 8;

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
    const PairedPos pairedPos = { (unsigned long long)1 << (szudzik), szudzik / 64 };
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

void ParseVectors(char* const str, unsigned long long* caveBitfield, int* escapeZone, int* largestOffset)
{
    // YOU NEED TO ITERATE THROUGH THE POINTS YOU FOOL.
    int start = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        const char* arrowStr = strstr(str + i, "->");
        if ((str + i) == arrowStr)
        {
            char* newStr = strdup(str + start);
            strtok(newStr + start, "->");

            int commaPos = start;
            for(commaPos; commaPos < i; ++commaPos)
            {
                if (newStr[commaPos] == ',')
                {
                    break;
                }
            }

            strtok(newStr, ",");

            Pos pos;
            pos.x = atoi(newStr) - 500;
            pos.y = atoi(newStr + commaPos + 1);

            if (pos.y > (*escapeZone))
            {
                (*escapeZone) = pos.y;
            }

            PairedPos pairedPos = PairPos(pos);

            if (pairedPos.pairingArrayOffset > (*largestOffset))
            {
                (*largestOffset) = pairedPos.pairingArrayOffset;
            }

           //[cantorArrayOffset]
            if (!(caveBitfield[pairedPos.pairingArrayOffset] & pairedPos.x))
            {
                //[cantorArrayOffset]
                caveBitfield[pairedPos.pairingArrayOffset] |= pairedPos.x;
            }

            free(newStr);
            start = i + 2;
        }
        ++i;
    }
}

#define MoveAndTestSand \
if (TestPos(sandTest, caveBitfield) && TestPos(sandTest, sandBitfield))\
{\
    sand.x = sandTest.x;\
    continue;\
}\

void ExecuteDay14_Part1(DayData* dayData)
{
    unsigned long long caveBitfield[8];
    unsigned long long sandBitfield[8];

    for (int i = 0; i < CAVE_BITFIELD_SIZE; ++i)
    {
        caveBitfield[i] = 0;
        sandBitfield[i] = 0;
    }

    int escapeZone = 0;
    int largestOffset = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        ParseVectors(dayData->m_Data[i], caveBitfield, &escapeZone, &largestOffset);
    }

    if (largestOffset > CAVE_BITFIELD_SIZE)
    {
        printf("\nWarning offset is larger than size, array needs to be BIGGER! %i.\n\n", largestOffset);
    }

    int sandCount = 0;
    BOOL hasSandEscaped = FALSE;
    while(!hasSandEscaped)
    {
        // 500 x, but we offset by 500 so sshhh..
        Pos sand = { 0, 0 };
        
        BOOL isSandPlaced = FALSE;

        while(!isSandPlaced)
        {
            //sand.y -= 1;

            Pos sandTest = sand;
            sandTest.y += 1;

            PairedPos testPairedPos = PairPos(sandTest);

            if (!(caveBitfield[testPairedPos.pairingArrayOffset] & testPairedPos.x))
            {
                if (sandBitfield[testPairedPos.pairingArrayOffset] & testPairedPos.x)
                {
                    //left.
                    sandTest.x -= 1;
                    MoveAndTestSand;
                    
                    //right.
                    sandTest.x += 2;
                    MoveAndTestSand;

                    //left left.
                    sandTest.x -= 3;
                    MoveAndTestSand;

                    //right right.
                    sandTest.y += 4;
                    MoveAndTestSand;

                    isSandPlaced = PlacePos(sand, sandBitfield);
                
                    if (isSandPlaced)
                    {
                        ++sandCount;
                    }
                }
                else
                {
                    // no rock and no sand, continue;
                }
            }
            else
            {
                // there is a rock next. stop here.
                isSandPlaced = PlacePos(sand, sandBitfield);
                
                if (isSandPlaced)
                {
                    ++sandCount;
                }
            }

            sand = sandTest;

            hasSandEscaped = sand.y > escapeZone;

            if (hasSandEscaped)
            {
                break;
            }
        }
    }

    printf("Sand count after escape: %i\n", sandCount);
}

void ExecuteDay14_Part2(DayData* dayData)
{
    
}

#undef MoveAndTestSand