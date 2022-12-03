#ifndef __DAY_H__
#define __DAY_H__

#define TRUE 1
#define FALSE 0
#define BOOL int

typedef struct DayData
{
    char* m_dayName;
    char* m_fileName;

    char** m_Data;
    int m_DataLength;
    // execute func?
} DayData;

static const int NUM_OF_DAYS = 25;

typedef struct Program
{    
    DayData* m_days;
    int m_DayToRun;
} Program;

Program* NewProgram();
void DeleteProgram(Program* const program);
void AllocateDays(Program* const program);
void ParseDayData(Program* const program);
void ParseInputForDay(DayData* const dayData);

void RunDay(Program* const program);

#endif