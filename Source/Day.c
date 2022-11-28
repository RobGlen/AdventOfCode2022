#include <stdio.h>
//#include <cstdio.h>
#include <stdlib.h>

#include "Day.h"

Program* NewProgram()
{
    Program* program = malloc(sizeof(Program));
    AllocateDays(program);
    return program;
}

void DeleteProgram(Program* const program)
{
    if (program != NULL)
    {
        if (program->m_days != NULL)
        {    
            for (int i = 0; i < NUM_OF_DAYS; ++i)
            {
                if (program->m_days->m_dayName != NULL)
                {
                    free(program->m_days->m_dayName);
                }
            }

            free(program->m_days);
        }

        free(program);
    }
}
void AllocateDays(Program* const program)
{
    program->m_days = malloc(NUM_OF_DAYS * sizeof(DayData));
    for (int i = 0; i < NUM_OF_DAYS; ++i)
    {
        program->m_days[i].m_dayName = malloc(sizeof("Day25\0"));
        sprintf(program->m_days[i].m_dayName, "Day%i\0", i+1);
    }
}

void ParseInput(const char* const dayName, DayData* const dayData)
{
    const char* const filename = "";
    FILE* const file = fopen(filename, "");
}



void DetermineFilenameFromDay(char* const outString, const char* const dayName)
{
}