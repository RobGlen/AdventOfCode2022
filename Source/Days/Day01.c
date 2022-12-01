 #include "Day01.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ExecuteDay01_Part1(struct DayData* dayData)
{
    int count = 0;
    int largestCount = 0;

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        if ((strcmp(dayData->m_Data[i], "") == 0) || (i == dayData->m_DataLength - 1))
        {
            count = 0;
        }
        else
        {
            // this needs to get the string.
            const int dataToNum = atoi(dayData->m_Data[i]);
            count += dataToNum;
            if (largestCount < count)
            {
                largestCount = count;
            }
        }
    }

    printf("largest: %i\n", largestCount);
}

void ExecuteDay01_Part2(struct DayData* dayData)
{
    const int numOfLargest = 3;
    int count = 0;
    int largestCount[3] = {0, 0, 0};

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        const int isEmptyString = strcmp(dayData->m_Data[i], "") == 0;

        if (!isEmptyString)
        {
            const int dataToNum = atoi(dayData->m_Data[i]);
            count += dataToNum;
        }

        // only one case where this can still execute after the previous if, still worth not else-ing
        if (isEmptyString || (i == dayData->m_DataLength - 1))
        {
            for (int j = 0; j < numOfLargest; ++j)
            {
                if (largestCount[j] < count)
                {
                    if (j == 0)
                    {
                        largestCount[2] = largestCount[1];
                        largestCount[1] = largestCount[0];
                    }
                    else if (j == 1)
                    {
                        largestCount[2] = largestCount[1];
                    }

                    largestCount[j] = count;
                    break;
                }
            }

            count = 0;
        }
    }

    int totalCount = 0;
    for (int i = 0; i < numOfLargest; ++i)
    {
        totalCount += largestCount[i];
    }

    printf("largest: %i\n", totalCount);
}