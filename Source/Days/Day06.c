#include "Day06.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void FindNonRepeatingString(char* const string, const int num)
{
	int i = 0;
	while (string[i] != '\0')
	{
		int h = 0;
		BOOL hasDupeBeenFound = FALSE;
		for (int j = i; j < i + num; ++j)
		{
			const int x = 1 << (string[j] - 'a');

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

	printf("It took %i to find %i non-repeating characters.\n", i + num, num);
}

void ExecuteDay06_Part1(DayData* dayData)
{ 
	FindNonRepeatingString(dayData->m_Data[0], 4);
}

void ExecuteDay06_Part2(DayData* dayData)
{
	FindNonRepeatingString(dayData->m_Data[0], 14);
}