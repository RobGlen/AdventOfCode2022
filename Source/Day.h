typedef struct
{
    char* m_dayName;
    char* m_fileName;

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