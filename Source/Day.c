#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                if (program->m_days[i].m_dayName != NULL)
                {
                    free(program->m_days[i].m_dayName);
                }

                if (program->m_days[i].m_Data != NULL)
                {
                    for (int j = 0; j < program->m_days[i].m_DataLength; ++j)
                    {
                        free(program->m_days[i].m_Data[j]);
                    }

                    free(program->m_days[i].m_Data);
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

    static const int STR_LEN = 6;

    for (int i = 0; i < NUM_OF_DAYS; ++i)
    {
        const int currentDay = i + 1;
        program->m_days[i].m_dayName = malloc(STR_LEN * sizeof(char));

        if (currentDay < 10)
        {
            sprintf(program->m_days[i].m_dayName, "Day0%i\0", currentDay); 
        }
        else
        {
            sprintf(program->m_days[i].m_dayName, "Day%i\0", currentDay);
        }

        program->m_days[i].m_Data = NULL;
        program->m_days[i].m_DataLength = 0;
    }
}

void ParseDayData(Program* const program)
{
    for (int i = 0; i < NUM_OF_DAYS; ++i)
    {
        ParseInputForDay(&program->m_days[i]);
    }
}

void ParseInputForDay(DayData* const dayData)
{
    //const char* const filename = "";
    char filename[21];
    sprintf(filename, "Data/Input/%s.txt\0", dayData->m_dayName);

    FILE* const file = fopen(filename, "r");

    if (file != NULL)
    {
        char str[99];
        int i = 0;

        while (fscanf(file, "%s", str) != EOF)
        {
            i++;
        }
        
        dayData->m_DataLength = i + 1;
        rewind(file);

        dayData->m_Data = malloc(dayData->m_DataLength * sizeof(char*));
      
        i = 0;
        while (fscanf(file, "%s", str) != EOF)
        {
            dayData->m_Data[i] = malloc(sizeof(str));
            strcpy(dayData->m_Data[i], str);
            i++;
        }
        
        fclose(file);
    }
}