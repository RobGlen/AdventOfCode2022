 #include "Day03.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int OffsetChar(const char element)
{
    const int offset = ((element >= 97 && element <= 122) ? 'a' : ('A' - ('z' - 'a') - 1)) - 1;
    return element - offset;
}

void ExecuteDay03_Part1(struct DayData* dayData)
{ 
    int sum = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        int length = 0;

        // find length;
        while(dayData->m_Data[i][length] != '\0')
        {
            ++length;
        }

        BOOL isFinished = FALSE;
        for (int j = 0; j < length / 2; ++j)
        {
            for (int k = length / 2; k < length; ++k)
            {
                if (dayData->m_Data[i][j] == dayData->m_Data[i][k])
                {
                    sum += OffsetChar(dayData->m_Data[i][j]);
                    isFinished = TRUE;
                    break;
                }
            }

            if (isFinished)
            {
                break;
            }
        }
    }

    printf("Repeating Elements in rucksack sum: %i\n", sum);
}

void ExecuteDay03_Part2(struct DayData* dayData)
{
    int sum = 0;
    const int groupSize = 3;
    for (int i = 0; i < dayData->m_DataLength; i += groupSize)
    {
        const int numOfItems = 53;
        int commonItemsCount[53];
        for (int j = 0; j < numOfItems; ++j)
        {
            commonItemsCount[j] = 0;
        }

        for (int j = i; j < (i + groupSize); ++j)
        {
            int k = 0;
            while (dayData->m_Data[j][k] != '\0')
            {
                const int charIndex = OffsetChar(dayData->m_Data[j][k]);
                if (commonItemsCount[charIndex] == j - i)
                {
                    commonItemsCount[charIndex]++;
                }
                ++k;
            }
        }

        BOOL wasHit = FALSE;
        for (int j = 0; j < numOfItems; ++j)
        {
            if (commonItemsCount[j] == groupSize)
            {
                sum += j;
                wasHit = TRUE;
                break;
            }
        }

        if (!wasHit)
        {
            printf("%i - FAIL: %s, %s, %s.\n", i, dayData->m_Data[i], dayData->m_Data[i+1], dayData->m_Data[i+2]);
        }
    }

    printf("Group badge sum: %i\n", sum);
}