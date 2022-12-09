#include "Day09.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Pos
{
    int x;
    int y;
} Pos;

void ExecuteDay09_Part1(DayData* dayData)
{
    Pos headPos = { 50, 50 };
    Pos tailPos = { 50, 50 };
    static const int TAIL_UNIQUE_BITFIELD_SIZE = 2000;
    long long tailUniquePosBitfield[2000];
    for (int i = 0; i < TAIL_UNIQUE_BITFIELD_SIZE; ++i)
    {
        tailUniquePosBitfield[i] = 0;
    }

    int uniqueLocations = 0;

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        const char* const str = dayData->m_Data[i];

        const int amount = atoi(str + 2);


        for (int j = 0; j < amount; ++j)
        {
            if (str[0] == 'U')
            {
                headPos.y += 1;
            }
            else if (str[0] == 'D')
            {
                headPos.y -= 1;
            }
            else if (str[0] == 'L')
            {
                headPos.x -= 1;
            }
            else if (str[0] == 'R')
            {
                headPos.x += 1;
            }

            if (tailPos.x == headPos.x)
            {
                const int tailHeadDelta = tailPos.y - headPos.y;
                const int tailHeadDeltaNormal = (tailHeadDelta != 0) ? (tailHeadDelta / abs(tailHeadDelta)) : 0;
                tailPos.y = headPos.y + tailHeadDeltaNormal;
            }
            else if (tailPos.y == headPos.y)
            {
                const int tailHeadDelta = tailPos.x - headPos.x;
                const int tailHeadDeltaNormal = (tailHeadDelta != 0) ? (tailHeadDelta / abs(tailHeadDelta)) : 0;

                tailPos.x = headPos.x + tailHeadDeltaNormal;
            }
            else
            {
                const Pos tailHeadDelta =
                {
                    (tailPos.x - headPos.x),
                    (tailPos.y - headPos.y)
                };

                const Pos tailHeadDeltaNormal =
                 {
                     (tailHeadDelta.x != 0) ? (tailHeadDelta.x / abs(tailHeadDelta.x)) : 0,
                     (tailHeadDelta.y != 0) ? (tailHeadDelta.y / abs(tailHeadDelta.y)) : 0
                 };

                if (abs(tailHeadDelta.x) < abs(tailHeadDelta.y))
                {
                    tailPos.x = headPos.x;
                    tailPos.y = headPos.y + tailHeadDeltaNormal.y;
                }
                else if (abs(tailHeadDelta.x) > abs(tailHeadDelta.y))
                {
                    tailPos.x = headPos.x + tailHeadDeltaNormal.x;
                    tailPos.y = headPos.y;
                }
            }

            
            const long long cantor = (long long)(0.5f * (tailPos.x + tailPos.y) * (tailPos.x + tailPos.y + 1) + tailPos.y);

            const long long x = (long long)1 << (cantor);

            const int cantorArrayOffset = cantor / 64;

            if (!(tailUniquePosBitfield[cantorArrayOffset] & x))
            {
                uniqueLocations++;
                tailUniquePosBitfield[cantorArrayOffset] |= x;
            }
        }
    }

    printf("Unique tail locations: %i\n", uniqueLocations);
}
void ExecuteDay09_Part2(DayData* dayData)
{
    Pos ropePosList[10];
    static const int ROPE_LENGTH = 10;
    for (int i = 0; i < ROPE_LENGTH; ++i)
    {
        ropePosList[i].x = 50;
        ropePosList[i].y = 50;
    }

    static const int TAIL_UNIQUE_BITFIELD_SIZE = 2000;
    long long tailUniquePosBitfield[2000];
    for (int i = 0; i < TAIL_UNIQUE_BITFIELD_SIZE; ++i)
    {
        tailUniquePosBitfield[i] = 0;
    }

    int uniqueLocations = 0;

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        const char* const str = dayData->m_Data[i];

        int amount = atoi(str + 2);

        while(amount > 0)
        {
            amount--;
            if (str[0] == 'U')
            {
                ropePosList[0].y += 1;
            }
            else if (str[0] == 'D')
            {
                ropePosList[0].y -= 1;
            }
            else if (str[0] == 'L')
            {
                ropePosList[0].x -= 1;
            }
            else if (str[0] == 'R')
            {
                ropePosList[0].x += 1;
            }

            for (int j = 1; j < ROPE_LENGTH; ++j)
            {
                if (ropePosList[j].x == ropePosList[j - 1].x)
                {
                    const int tailHeadDelta = ropePosList[j].y - ropePosList[j - 1].y;
                    const int tailHeadDeltaNormal = (tailHeadDelta != 0) ? (tailHeadDelta / abs(tailHeadDelta)) : 0;
                    ropePosList[j].y = ropePosList[j - 1].y + tailHeadDeltaNormal;
                }
                else if (ropePosList[j].y == ropePosList[j - 1].y)
                {
                    const int tailHeadDelta = ropePosList[j].x - ropePosList[j - 1].x;
                    const int tailHeadDeltaNormal = (tailHeadDelta != 0) ? (tailHeadDelta / abs(tailHeadDelta)) : 0;

                    ropePosList[j].x = ropePosList[j - 1].x + tailHeadDeltaNormal;
                }
                else
                {
                    const Pos tailHeadDelta =
                    {
                        (ropePosList[j].x - ropePosList[j - 1].x),
                        (ropePosList[j].y - ropePosList[j - 1].y)
                    };

                    const Pos tailHeadDeltaNormal =
                    {
                        (tailHeadDelta.x != 0) ? (tailHeadDelta.x / abs(tailHeadDelta.x)) : 0,
                        (tailHeadDelta.y != 0) ? (tailHeadDelta.y / abs(tailHeadDelta.y)) : 0
                    };

                    if (abs(tailHeadDelta.x) < abs(tailHeadDelta.y))
                    {
                        ropePosList[j].x = ropePosList[j - 1].x;
                        ropePosList[j].y = ropePosList[j - 1].y + tailHeadDeltaNormal.y;
                    }
                    else if (abs(tailHeadDelta.x) > abs(tailHeadDelta.y))
                    {
                        ropePosList[j].x = ropePosList[j - 1].x + tailHeadDeltaNormal.x;
                        ropePosList[j].y = ropePosList[j - 1].y;
                    }
                    else
                    {
                        ropePosList[j].x = ropePosList[j - 1].x + tailHeadDeltaNormal.x;
                        ropePosList[j].y = ropePosList[j - 1].y + tailHeadDeltaNormal.y;
                    }
                }
            }
            
            const long long cantor = (long long)(0.5f * (ropePosList[ROPE_LENGTH - 1].x + ropePosList[ROPE_LENGTH - 1].y) * (ropePosList[ROPE_LENGTH - 1].x + ropePosList[ROPE_LENGTH - 1].y + 1) + ropePosList[ROPE_LENGTH - 1].y);

            const long long x = (long long)1 << (cantor);

            const int cantorArrayOffset = cantor / 64;

            if (!(tailUniquePosBitfield[cantorArrayOffset] & x))
            {
                uniqueLocations++;
                tailUniquePosBitfield[cantorArrayOffset] |= x;
            }

        //	for (int j = 0; j < 6; ++j)
        //	{
        //		for (int k = 0; k < 6; ++k)
        //		{
        //			BOOL hasPrintedRope = FALSE;
        //			for (int l = 0; l < 10; ++l)
        //			{
        //				if (ropePosList[l].x == k && ropePosList[l].y == 5 - j)
        //				{
        //					printf("%i", l);
        //					hasPrintedRope = TRUE;
        //					break;
        //				}
        //			}

        //			if (!hasPrintedRope)
        //			{
        //				printf(".");
        //			}
        //			//if (headPos.x == k && headPos.y == 4 - j)
        //			//{
        //			//    printf("H");
        //			//}
        //			//else if (tailPos.x == k && tailPos.y == 4 - j)
        //			//{
        //			//	printf("T");
        //			//}
        //			//else
        //			//{
        //			//    printf(".");
        //			//}
        //		}
        //		printf("\n");
        //	}

        //	printf("\n");
        }
    }

    printf("Unique tail locations: %i\n", uniqueLocations);
}