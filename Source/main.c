#include <stdio.h>
#include <stdlib.h>

#include "Day.h"

int main(int argc, char* argv[])
{
    printf("Advent of Code 2022: Starting up.\n");

    Program* program = NewProgram();

    ParseDayData(program);

    if (argc >= 2)
    {
        program->m_DayToRun = atoi(argv[1]);
    }

    RunDay(program);

    DeleteProgram(program);

    return 0;
} 