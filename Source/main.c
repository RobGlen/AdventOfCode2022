#include <stdio.h>

#include "Day.h"

int main(int argc, char* argv[])
{
  printf("Advent of Code 2022: Starting up.\n");

  Program* program = NewProgram();

  ParseDayData(program);

  program->m_DayToRun = 10;

  RunDay(program);

  DeleteProgram(program);

  return 0;
} 