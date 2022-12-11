#include "Day11.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
typedef struct Item
{
    unsigned long long worryLevel;
    //unsigned usigned long long worryLevel;
    struct Item* next;
} Item;

typedef void (*OpFunc)(Item* const, const int);

typedef struct Monkey
{
    Item* items;
    OpFunc opFunc;
    int opRhs;
    int divisibleTest;
    struct Monkey* trueMonkey;
    struct Monkey* falseMonkey;
    int inspectCount;
} Monkey;

void SetupMonkey(Monkey** monkeyToSetup, Monkey* monkeys, char* const str)
{
    const int monkeyToSetupIdx = atoi(str + sizeof("Monkey"));
    Monkey* newMonkeyToSetup = &monkeys[monkeyToSetupIdx];
    newMonkeyToSetup->items = NULL;
    newMonkeyToSetup->opFunc = NULL;
    newMonkeyToSetup->opRhs = 0;
    newMonkeyToSetup->divisibleTest = 0;
    newMonkeyToSetup->trueMonkey = NULL;
    newMonkeyToSetup->falseMonkey = NULL;
    newMonkeyToSetup->inspectCount = 0;
    *monkeyToSetup = newMonkeyToSetup;
}

void CreateItem(Monkey* monkeyToSetup, char* const str, int start)
{
    strtok(str + start, ", ");
    Item* item = malloc(sizeof(Item));
    item->worryLevel = atoi(str + start);
    item->next = NULL;
    Item* currentItem = monkeyToSetup->items;
    if (currentItem == NULL)
    {
        monkeyToSetup->items = item;
    }
    else
    {
        while(currentItem->next != NULL)
        {
            currentItem = currentItem->next;
        }
        currentItem->next = item;
    }
}

void AddOp(Item* const item, const int value)
{
    item->worryLevel += value;
}

void MultiplyOp(Item* const item, const int value)
{
    item->worryLevel *= value;
}

void Power2Op(Item* const item, const int value)
{
    item->worryLevel *= item->worryLevel;
}

void ParseMonkeys(DayData* const dayData, Monkey* const monkeys)
{
    Monkey* monkeyToSetup = NULL;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        char* const str = strdup(dayData->m_Data[i]);

        if (strstr(str, "Monkey"))
        {
            SetupMonkey(&monkeyToSetup, monkeys, str);
        }
        else if (strstr(str, "  Starting items:"))
        {
            int start = sizeof("  Starting items:");
            int i = start;
            while(str[i] != '\0')
            {
                if (str[i] == ',')
                {
                    CreateItem(monkeyToSetup, str, start);
                    start = i + 1;
                    i = start;
                }
                else
                {
                    ++i;
                }
            }

            CreateItem(monkeyToSetup, str, start);
        }
        else if (strstr(str, "  Operation:"))
        {
            const int startIndex = sizeof("  Operation: new = old");
            if (str[startIndex] == '+')
            {
                monkeyToSetup->opFunc = &AddOp;
                monkeyToSetup->opRhs = atoi(str + startIndex + 1);
            }
            else if (str[startIndex] == '*')
            {
                if (strstr(str + startIndex, "old"))
                {
                    monkeyToSetup->opFunc = &Power2Op;
                }
                else
                {
                    monkeyToSetup->opFunc = &MultiplyOp;
                    monkeyToSetup->opRhs = atoi(str + startIndex + 1);
                }
            }
        }
        else if (strstr(str, "  Test:"))
        {
            const int startIndex = sizeof("  Test: divisible by");
            monkeyToSetup->divisibleTest = atoi(str + startIndex);
        }
        else if (strstr(str, "    If true"))
        {
            const int startIndex = sizeof("    If true: throw to monkey");
            monkeyToSetup->trueMonkey = &monkeys[atoi(str + startIndex)];
        }
        else if (strstr(str, "    If false"))
        {
            const int startIndex = sizeof("    If false: throw to monkey");
            monkeyToSetup->falseMonkey = &monkeys[atoi(str + startIndex)];
        }

        free(str);
    }
}

void ExecuteMonkeyBusiness(Monkey* const monkeys, const int numOfMonkeys, const int roundCount, const BOOL doesWorryReduce)
{
    for (int i = 0; i < roundCount; ++i)
    {
        int lcm = 1;
        for(int j = 0; j < numOfMonkeys; ++j)
        {
            lcm *= monkeys[j].divisibleTest;
        }

        for(int j = 0; j < numOfMonkeys; ++j)
        {
            Monkey* currentMonkey = &monkeys[j];

            Item* item = currentMonkey->items;

            while(item != NULL)
            {
                Item* nextItem = item->next;

                currentMonkey->opFunc(item, currentMonkey->opRhs);

                currentMonkey->items = item->next;
                item->next = NULL;

                const unsigned long long mod1 = item->worryLevel % currentMonkey->divisibleTest;

                //for(int k; k < numOfMonkeys; ++k)
                item->worryLevel %= lcm;

                const unsigned long long mod2 = item->worryLevel % currentMonkey->divisibleTest;

                if (mod1 != mod2)
                {
                    printf("FAIL\n");
                }

                if (doesWorryReduce)
                {
                    item->worryLevel /= 3;
                }

                Monkey* monkeyToThrowTo = NULL;
                if (item->worryLevel % currentMonkey->divisibleTest == 0)
                {              
                    monkeyToThrowTo = currentMonkey->trueMonkey;
                }
                else
                {
                    monkeyToThrowTo = currentMonkey->falseMonkey;
                }

                Item* itemToAppendTo = monkeyToThrowTo->items;
                if (itemToAppendTo != NULL)
                {
                    while(itemToAppendTo->next != NULL)
                    {
                        itemToAppendTo = itemToAppendTo->next;
                    }

                    itemToAppendTo->next = item;
                }
                else
                {
                    monkeyToThrowTo->items = item;
                }
                
                currentMonkey->inspectCount++;
                item = nextItem;
            }
        }
    }
}

unsigned long long FindHighestMonkeyBusiness(Monkey* monkeys, const int numOfMonkeys)
{
    unsigned long long highest = 0;
    unsigned long long secondHighest = 0;
    for (int i = 0; i < numOfMonkeys; ++i)
    {
        Monkey* currentMonkey = &monkeys[i];
        
        const unsigned long long inspectCount = currentMonkey->inspectCount;

        if (inspectCount > highest)
        {
            secondHighest = highest;
            highest = inspectCount;
        }
        else if (inspectCount > secondHighest)
        {
            secondHighest = inspectCount;
        }
    }

    return highest * secondHighest;
}

void FreeItems(Monkey* monkeys, const int numOfMonkeys)
{
    for (int i = 0; i < numOfMonkeys; ++i)
    {
        Item* currentItem = monkeys[i].items;
        while(currentItem != NULL)
        {
            Item* nextItem = currentItem->next;
            free(currentItem);
            currentItem = nextItem;
        }
    }
}

void ExecuteDay11_Part1(DayData* dayData)
{
    static const int NUM_OF_MONKEYS = 8;
    Monkey monkeys[8];

    ParseMonkeys(dayData, monkeys);

    static const int ROUND_COUNT = 20;
    static const BOOL DOES_WORRY_REDUCE = TRUE;
    ExecuteMonkeyBusiness(monkeys, NUM_OF_MONKEYS, ROUND_COUNT, DOES_WORRY_REDUCE);

    const int monkeyBusiness = FindHighestMonkeyBusiness(monkeys, NUM_OF_MONKEYS);
    printf("Monkey business: %i\n", monkeyBusiness);

    FreeItems(monkeys, NUM_OF_MONKEYS);
}

void ExecuteDay11_Part2(DayData* dayData)
{
    static const int NUM_OF_MONKEYS = 8;
    Monkey monkeys[8];

    ParseMonkeys(dayData, monkeys);

    static const int ROUND_COUNT = 10000;
    static const BOOL DOES_WORRY_REDUCE = FALSE;
    ExecuteMonkeyBusiness(monkeys, NUM_OF_MONKEYS, ROUND_COUNT, DOES_WORRY_REDUCE);

    const unsigned long long monkeyBusiness = FindHighestMonkeyBusiness(monkeys, NUM_OF_MONKEYS);
    printf("Monkey business: %llu\n", monkeyBusiness);

    FreeItems(monkeys, NUM_OF_MONKEYS);
}