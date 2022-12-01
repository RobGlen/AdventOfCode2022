#include "DayLookup.h" 
#include "Day.h"

#include <stdlib.h>

#include "Days/Day01.h"

DayFuncPair* CreateDayFuncPairList()
{
    DayFuncPair* dayFuncPairs = malloc(sizeof(DayFuncPair) * NUM_OF_DAYS);

    for (int i = 0; i < NUM_OF_DAYS; ++i)
    {
        dayFuncPairs[i].m_dayFunc1 = NULL;
        dayFuncPairs[i].m_dayFunc2 = NULL;
    }

    // dayFuncPairs[0] = malloc(sizeof(DayFuncPair));
    dayFuncPairs[0].m_dayFunc1 = &ExecuteDay01_Part1;
    dayFuncPairs[0].m_dayFunc2 = &ExecuteDay01_Part2;

    return dayFuncPairs;
}

void FreeDayFuncPairList(DayFuncPair* const dayFuncPairList)
{
    if (dayFuncPairList != NULL)
    {
        free(dayFuncPairList);
    }
}