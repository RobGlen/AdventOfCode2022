typedef struct
{
    char* m_dayName;
    char* m_fileName;

    char** m_Data;
    int m_DataLength;
    // execute func?
} DayData;

static const int NUM_OF_DAYS = 25;

typedef struct
{    
    DayData* m_days;
} Program;

Program* NewProgram();
void DeleteProgram(Program* const program);
void AllocateDays(Program* const program);
void ParseDayData(Program* const program);
void ParseInputForDay(DayData* const dayData);