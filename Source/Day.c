#include <stdio.h>
//#include <cstdio.h>
#include <stdlib.h>

#include "Day.h"

Program* NewProgram()
{
    Program* program = malloc(sizeof(Program));
    return program;
}

void DeleteProgram(Program* const program)
{
    if (program != NULL)
    {
        if (program->m_days != NULL)
        {
            free(program->m_days);
        }
        
        free(program);
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