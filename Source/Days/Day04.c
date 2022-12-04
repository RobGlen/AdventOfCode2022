 #include "Day04.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void DetermineRanges(char* const sourceStr, int* ranges)
{
    int i = 0;
    int commaIndex = 0;
    int hyphenIndex[2] = {0, 0};
    int hyphenToSet = 0;
    while(sourceStr[i] != '\0')
    {
        if (sourceStr[i] == '-')
        {
            hyphenIndex[hyphenToSet] = i;
            hyphenToSet++;
        }
        else if (sourceStr[i] == ',')
        {
            commaIndex = i;
        }
        ++i;
    }

    char str[20];
    strcpy(str, sourceStr);
    strtok(str, ",\0");
    strtok(str, "-\0");
    strtok(str + commaIndex + 1, "-\0");
    
    ranges[0] = atoi(str);
    ranges[1] = atoi(str + hyphenIndex[0] + 1);
    ranges[2] = atoi(str + commaIndex + 1);
    ranges[3] = atoi(str + hyphenIndex[1] + 1);
}

void ExecuteDay04_Part1(struct DayData* dayData)
{ 
    int countOfOverlap = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        int ranges[4];
        DetermineRanges(dayData->m_Data[i], ranges);

        if (((ranges[0] <= ranges[2]) && (ranges[1] >= ranges[3])) ||
            ((ranges[0] >= ranges[2]) && (ranges[1] <= ranges[3])))
        {
            countOfOverlap++;
        }
    }
    printf("Num of overlaps: %i\n", countOfOverlap);
}

void ExecuteDay04_Part2(struct DayData* dayData)
{
    int countOfOverlap = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        int ranges[4];
        DetermineRanges(dayData->m_Data[i], ranges);

        if (((ranges[0] <= ranges[2]) && (ranges[1] >= ranges[3])) ||
            ((ranges[0] >= ranges[2]) && (ranges[1] <= ranges[3])) ||
            ((ranges[0] <= ranges[2]) && (ranges[1] >= ranges[2])) ||
            ((ranges[0] <= ranges[3]) && (ranges[1] >= ranges[3])))
        {
            countOfOverlap++;
        }
    }
    printf("Num of overlaps: %i\n", countOfOverlap);
}