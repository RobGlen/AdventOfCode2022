 #include "Day04.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ExecuteDay04_Part1(struct DayData* dayData)
{ 
    int countOfOverlap = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        dayData->m_Data[i];

        int j = 0;
        int commaIndex = 0;
        int hyphenIndex[2] = {0, 0};
        int hyphenToSet = 0;
        while(dayData->m_Data[i][j] != '\0')
        {
            if (dayData->m_Data[i][j] == '-')
            {
                hyphenIndex[hyphenToSet] = j;
                hyphenToSet++;
            }
            else if (dayData->m_Data[i][j] == ',')
            {
                commaIndex = j;
            }
            ++j;
        }

        char str[20];
        char* strptr = str;
        strcpy(str, dayData->m_Data[i]);
        strtok(str, ",\0");
        strtok(str, "-\0");
        strtok(strptr + commaIndex + 1, "-\0");
        int ranges[4] = 
        {
            atoi(str),
            atoi(str + hyphenIndex[0] + 1),
            atoi(str + commaIndex + 1),
            atoi(str + hyphenIndex[1] + 1),
        };

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
        dayData->m_Data[i];

        int j = 0;
        int commaIndex = 0;
        int hyphenIndex[2] = {0, 0};
        int hyphenToSet = 0;
        while(dayData->m_Data[i][j] != '\0')
        {
            if (dayData->m_Data[i][j] == '-')
            {
                hyphenIndex[hyphenToSet] = j;
                hyphenToSet++;
            }
            else if (dayData->m_Data[i][j] == ',')
            {
                commaIndex = j;
            }
            ++j;
        }

        char str[20];
        char* strptr = str;
        strcpy(str, dayData->m_Data[i]);
        strtok(str, ",\0");
        strtok(str, "-\0");
        strtok(strptr + commaIndex + 1, "-\0");
        int ranges[4] = 
        {
            atoi(str),
            atoi(str + hyphenIndex[0] + 1),
            atoi(str + commaIndex + 1),
            atoi(str + hyphenIndex[1] + 1),
        };

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