#ifndef __DAYLOOKUP_H__
#define __DAYLOOKUP_H__

struct DayData;

typedef void (*DayFunc)(struct DayData* dayData);

typedef struct DayFuncPair
{
    DayFunc m_dayFunc1;
    DayFunc m_dayFunc2;
} DayFuncPair;

DayFuncPair* CreateDayFuncPairList();
void FreeDayFuncPairList(DayFuncPair* const dayFuncPairList);

#endif