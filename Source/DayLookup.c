#include "DayLookup.h" 
#include "Day.h"

#include <stdlib.h>

#include "Days/Day01.h"
#include "Days/Day02.h"
#include "Days/Day03.h"
#include "Days/Day04.h"
#include "Days/Day05.h"
#include "Days/Day06.h"
#include "Days/Day07.h"
#include "Days/Day08.h"
#include "Days/Day09.h"
#include "Days/Day10.h"
#include "Days/Day11.h"
#include "Days/Day12.h"
#include "Days/Day13.h"

#define DAY(i) i - 1

DayFuncPair* CreateDayFuncPairList()
{
    DayFuncPair* dayFuncPairs = malloc(sizeof(DayFuncPair) * NUM_OF_DAYS);

    for (int i = 0; i < NUM_OF_DAYS; ++i)
    {
        dayFuncPairs[i].m_dayFunc1 = NULL;
        dayFuncPairs[i].m_dayFunc2 = NULL;
    }

    dayFuncPairs[DAY(1)].m_dayFunc1 = &ExecuteDay01_Part1;
    dayFuncPairs[DAY(1)].m_dayFunc2 = &ExecuteDay01_Part2;

    dayFuncPairs[DAY(2)].m_dayFunc1 = &ExecuteDay02_Part1;
    dayFuncPairs[DAY(2)].m_dayFunc2 = &ExecuteDay02_Part2;

    dayFuncPairs[DAY(3)].m_dayFunc1 = &ExecuteDay03_Part1;
    dayFuncPairs[DAY(3)].m_dayFunc2 = &ExecuteDay03_Part2;

    dayFuncPairs[DAY(4)].m_dayFunc1 = &ExecuteDay04_Part1;
    dayFuncPairs[DAY(4)].m_dayFunc2 = &ExecuteDay04_Part2;

    dayFuncPairs[DAY(5)].m_dayFunc1 = &ExecuteDay05_Part1;
    dayFuncPairs[DAY(5)].m_dayFunc2 = &ExecuteDay05_Part2;

    dayFuncPairs[DAY(6)].m_dayFunc1 = &ExecuteDay06_Part1;
    dayFuncPairs[DAY(6)].m_dayFunc2 = &ExecuteDay06_Part2;

    dayFuncPairs[DAY(7)].m_dayFunc1 = &ExecuteDay07_Part1;
    dayFuncPairs[DAY(7)].m_dayFunc2 = &ExecuteDay07_Part2;

    dayFuncPairs[DAY(8)].m_dayFunc1 = &ExecuteDay08_Part1;
    dayFuncPairs[DAY(8)].m_dayFunc2 = &ExecuteDay08_Part2;

	dayFuncPairs[DAY(9)].m_dayFunc1 = &ExecuteDay09_Part1;
	dayFuncPairs[DAY(9)].m_dayFunc2 = &ExecuteDay09_Part2;

    dayFuncPairs[DAY(10)].m_dayFunc1 = &ExecuteDay10_Part1;
	dayFuncPairs[DAY(10)].m_dayFunc2 = &ExecuteDay10_Part2;

    dayFuncPairs[DAY(11)].m_dayFunc1 = &ExecuteDay11_Part1;
	dayFuncPairs[DAY(11)].m_dayFunc2 = &ExecuteDay11_Part2;

    dayFuncPairs[DAY(12)].m_dayFunc1 = &ExecuteDay12_Part1;
	dayFuncPairs[DAY(12)].m_dayFunc2 = &ExecuteDay12_Part2;

    dayFuncPairs[DAY(13)].m_dayFunc1 = &ExecuteDay13_Part1;
	dayFuncPairs[DAY(13)].m_dayFunc2 = &ExecuteDay13_Part2;

    return dayFuncPairs;
}

void FreeDayFuncPairList(DayFuncPair* const dayFuncPairList)
{
    if (dayFuncPairList != NULL)
    {
        free(dayFuncPairList);
    }
}

#undef DAY