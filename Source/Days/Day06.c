#include "Day06.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ExecuteDay06_Part1(DayData* dayData)
{ 
    int i = 0;
    while (dayData->m_Data[0][i] != '\0')
    {
        int h = 0;
        BOOL hasDupeBeenFound = FALSE;
		for (int j = i; j < i + 4; ++j)
		{
            const int x = 1 << (dayData->m_Data[0][j] - 'a');

            if (h & x)
            {
                hasDupeBeenFound = TRUE;
                break;
            }
            else
            {
                h |= x;
            }
		}

        if (!hasDupeBeenFound)
        {
            break;
        }

        ++i;
    }

    printf("It took %i to find 4 repeating characters.\n", i+4);
}

void ExecuteDay06_Part2(DayData* dayData)
{
    
}