 #include "Day05.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Crate
{
    char id;
    struct Crate* next;
    struct Crate* prev;
} Crate;

void ParseCrates(DayData* const dayData, Crate** crateStack, Crate* allCrates, const int crateCount, int* outInstructionsIndex)
{
    int crateIndex = 0;

    int i = 0;
    while (dayData->m_Data[i][0] != '\0')
    {
        ++i;
    }
    *outInstructionsIndex = i + 1;

    int stackIndices[9];
    int j = 0;
    --i;
    int stackIndex = 0;
    while (dayData->m_Data[i][j] != '\0')
    {
        if (dayData->m_Data[i][j] != ' ')
        {
            stackIndices[stackIndex] = j;
            ++stackIndex;
        }
        j++;
    }

    for (i--; i >= 0; --i)
    {
        dayData->m_Data[i];
        j = 1;
     /*   int stackIndex = 0;*/

        for (j = 0; j < 9; ++j)
        {
            stackIndex = stackIndices[j];

            if (dayData->m_Data[i][stackIndex] == ' ')
            {
                continue;
            }

			allCrates[crateIndex].id = dayData->m_Data[i][stackIndex];
			if (crateStack[j] != NULL)
			{
				Crate* currentCrate = crateStack[j];
				while (currentCrate->next != NULL)
				{
					currentCrate = currentCrate->next;
				}

				currentCrate->next = &allCrates[crateIndex];
				currentCrate->next->prev = currentCrate;
				crateIndex++;
			}
            else
            {
                crateStack[j] = &allCrates[crateIndex];
				crateIndex++;
			}

        }
        /*while (dayData->m_Data[i][j + 1] != '\0')
        {
            if (dayData->m_Data[i][j - 1] == '[' && dayData->m_Data[i][j + 1] == ']')
            {

				++stackIndex;
            }
            ++j;
        }*/
    }
}

void ParseInstructions(DayData* dayData, Crate** crateStack, int instructionsStartIndex, BOOL shouldKeepOrder)
{
    int rules[3];

    for (int i = instructionsStartIndex; i < dayData->m_DataLength; ++i)
    {
        int j = 0;
        for (int k = 0; k < 3; ++k)
        {
			while (dayData->m_Data[i][j] != ' ')
			{
                ++j;
			}

			++j;

			int len = j;

            char str[4];
			while (dayData->m_Data[i][len] != ' ' && dayData->m_Data[i][len] != '\0')
			{
                str[len - j] = dayData->m_Data[i][len];
                ++len;
			}

            str[len - j] = '\0';

            dayData->m_Data[i][len] = '\0';

			//char str[10];
			//strcpy(str, dayData->m_Data[i] + j);
            rules[k] = atoi(str);

            j = len + 1;
        }

        const int moveAmount = rules[0];
        const int moveFrom = rules[1] - 1;
        const int moveTo = rules[2] - 1;

		Crate* crateToMove = crateStack[moveFrom];

        if (crateToMove == NULL)
        {
            continue;
        }

		while (crateToMove->next != NULL)
		{
			crateToMove = crateToMove->next;
		}
        
        if (shouldKeepOrder)
        {
			Crate* startCrate = crateToMove;
			for (j = 0; j < moveAmount - 1; ++j)
			{
				if (startCrate != NULL)
				{
					startCrate = startCrate->prev;
				}
			}

			Crate* prevCrateInMoveStack = startCrate->prev;

			if (prevCrateInMoveStack != NULL)
			{
				prevCrateInMoveStack->next = NULL;
			}

			Crate* crateTarget = crateStack[moveTo];

			if (crateStack[moveTo] == NULL)
			{
				crateStack[moveTo] = startCrate;
			}
			else
			{
				while (crateTarget->next != NULL)
				{
					crateTarget = crateTarget->next;
				}

				crateTarget->next = startCrate;
			}

			startCrate->prev = crateTarget;
			crateToMove->next = NULL;

			if (startCrate == crateStack[moveFrom])
			{
				crateStack[moveFrom] = NULL;
			}
        }
        else
        {
			for (j = 0; j < moveAmount; ++j)
			{
				Crate* prevCrateInMoveStack = crateToMove->prev;

				if (prevCrateInMoveStack != NULL)
				{
					prevCrateInMoveStack->next = NULL;
				}

				Crate* crateTarget = crateStack[moveTo];

				if (crateStack[moveTo] == NULL)
				{
					crateStack[moveTo] = crateToMove;
				}
				else
				{
					while (crateTarget->next != NULL)
					{
						crateTarget = crateTarget->next;
					}

					crateTarget->next = crateToMove;
				}

				crateToMove->prev = crateTarget;
				crateToMove->next = NULL;

				if (crateToMove == crateStack[moveFrom])
				{
					crateStack[moveFrom] = NULL;
				}

				crateToMove = prevCrateInMoveStack;

				if (crateToMove == NULL)
				{
					break;
				}
			}
        }
    }
}

void SortoutCrates(DayData* dayData, BOOL shouldKeepOrder)
{
	Crate* crateStack[9];
	Crate allCrates[65];

	for (int i = 0; i < 65; ++i)
	{
		allCrates[i].id = 0;
		allCrates[i].next = NULL;
		allCrates[i].prev = NULL;
	}

	for (int i = 0; i < 9; ++i)
	{
		crateStack[i] = NULL;
	}
	int instructionsIndex = 0;
	ParseCrates(dayData, crateStack, allCrates, 65, &instructionsIndex);

	for (int i = 0; i < 9; ++i)
	{
		Crate* crate = crateStack[i];
		while (crate != NULL)
		{
			printf("[%c]", crate->id);
			crate = crate->next;
		}
		printf("\n");
	}

	ParseInstructions(dayData, crateStack, instructionsIndex, shouldKeepOrder);

	for (int i = 0; i < 9; ++i)
	{
		Crate* crate = crateStack[i];
		if (crate != NULL)
		{
			while (crate->next != NULL)
			{
				crate = crate->next;
			}
			printf("%c", crate->id);
		}
	}

	printf("\n");
}

void ExecuteDay05_Part1(DayData* dayData)
{ 
	SortoutCrates(dayData, FALSE);
}

void ExecuteDay05_Part2(DayData* dayData)
{
    SortoutCrates(dayData, TRUE);
}