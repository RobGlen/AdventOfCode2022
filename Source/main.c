#include <stdio.h>

#include "Day.h"

int main()
{
  printf("Advent of Code 2022: Starting up.\n");

  //ParseInput(NULL, NULL);
  Program* program = NewProgram();


  printf("Press any key to continue!\n");
  getchar();

  DeleteProgram(program);
  return 0;
} 