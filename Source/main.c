#include <stdio.h>

#include "Day.h"

int main(int argc, char* argv[])
{
  printf("Advent of Code 2022: Starting up.\n");

  Program* program = NewProgram();

  ParseDayData(program);

  program->m_DayToRun = 2;

  RunDay(program);

  printf("Press any key to continue!\n");
  getchar();

  DeleteProgram(program);
  return 0;
} 