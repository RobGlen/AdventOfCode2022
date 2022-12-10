#include "Day10.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Command
{
    int cyclesLeft;
    int amountToAdd;
    struct Command* next;
} Command;

typedef void (*OpFunc)(const int, void*);
typedef void (*SignalFunc)(void*);

void AddCommandToQueue(Command** queue, Command* command)
{
    if (*queue == NULL)
    {
        *queue = command;
    }
    else
    {
        Command* commandToAppendTo = *queue;
        while(commandToAppendTo->next != NULL)
        {
            commandToAppendTo = commandToAppendTo->next;
        }

        commandToAppendTo->next = command;
    }
}

void AllocateOrReuseCommand(Command** commandQueue, Command** reuseQueue, const int cyclesToCompleteCommand, const int amountToAdd)
{
    Command* command = NULL;
    if (*reuseQueue == NULL)
    {
        command = malloc(sizeof(Command));
    }
    else
    {
        command = *reuseQueue;
        (*reuseQueue) = (*reuseQueue)->next;
    }

    command->cyclesLeft = cyclesToCompleteCommand;
    command->amountToAdd = amountToAdd;
    command->next = NULL;

    AddCommandToQueue(commandQueue, command);    
}

void FreeQueue(Command* command)
{
    Command* nextCommand = NULL;
    int i = 0;
    while(command != NULL)
    {
        nextCommand = command->next;
        free(command);
        command = nextCommand;
        i++;
    }
}

void ExecuteSignal(DayData* dayData,
    const BOOL shouldPrintRegister,
    const int startingSignal,
    const OpFunc opFunc,
    const SignalFunc signalFunc,
    void* funcData)
{
    int x = 1;
    Command* commandQueue = NULL;
    Command* reuseQueue = NULL;
    int nextSignal = startingSignal;
    int signalSum = 0;
    static const int CYCLES_FOR_ADDX = 2;

    for(int i = 0; i < dayData->m_DataLength || commandQueue != NULL; ++i)
    {
        if (i < dayData->m_DataLength)
        {
            char* const str = dayData->m_Data[i];
            strtok(str, " ");

            if (strcmp(str, "addx") == 0)
            {
                AllocateOrReuseCommand(&commandQueue, &reuseQueue, CYCLES_FOR_ADDX, atoi(str + sizeof("addx")));
            }
            else if (strcmp(str, "noop") == 0)
            {
                AllocateOrReuseCommand(&commandQueue, &reuseQueue, 1, 0);
            }
        }
        
        if (opFunc != NULL)
        {
            opFunc(x, funcData);
        }

        if (i + 1 == nextSignal)
        {
            signalSum += nextSignal * x;
            nextSignal += 40;

            if (shouldPrintRegister)
            {
                printf("\x1B[32m");
            }

            if (signalFunc != NULL)
            {
                signalFunc(funcData);
            }   
        }
        else
        {
            if (shouldPrintRegister)
            {
                printf("\033[0m");
            }
            
        }

        if (shouldPrintRegister)
        {
            printf("i: %i, x: %i\n", i, x);
        }

        Command* currentCommand = commandQueue;
        Command* prevCommand = NULL;

        currentCommand->cyclesLeft--;
        currentCommand = commandQueue;
        if (currentCommand != NULL)
        {
            if (currentCommand->cyclesLeft <= 0)
            {
                x += currentCommand->amountToAdd;
                commandQueue = commandQueue->next;
                currentCommand->next = NULL;
                AddCommandToQueue(&reuseQueue, currentCommand);
            }
        }
    }
    
    FreeQueue(commandQueue);
    FreeQueue(reuseQueue);

    printf("\033[0mSignal sum: %i\n", signalSum);
}

void OnOp(const int x, void* data)
{    
    int* counter = data;   

    if ((*counter) >= x && (*counter) < x + 3)
    {
        printf("█");
    }
    else
    {
        printf(" ");
    }
    (*counter)++;
}

void OnSignal(void* data)
{
    int* counter = data;
    (*counter) = 1;
    printf("\n");
}

void ExecuteDay10_Part1(DayData* dayData)
{
    const BOOL shouldPrint = TRUE;
    const int startingSignal = 20;
    ExecuteSignal(dayData, shouldPrint, startingSignal, NULL, NULL, NULL);
}

void ExecuteDay10_Part2(DayData* dayData)
{
    const BOOL shouldPrint = FALSE;
    const int startingSignal = 40;
    int counter = 1;
    ExecuteSignal(dayData, shouldPrint, startingSignal, &OnOp, &OnSignal, &counter);
}