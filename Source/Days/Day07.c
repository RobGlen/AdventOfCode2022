#include "Day07.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Dir
{
    char* name;
    int size;

    int numChildren;
    struct Dir** children;

    struct Dir* parent;

    BOOL isDir;
} Dir;

void Zero(Dir* dir)
{
	dir->name = NULL;
	dir->size = 0;
	dir->numChildren = 0;
	dir->children = NULL;
    dir->parent = NULL;
	dir->isDir = FALSE;
}

void CountDirsUnderSize(Dir* currentDir, int* const count, const int sizeTarget)
{
    if (currentDir == NULL)
    {
        return;
    }

	for (int i = 0; i < currentDir->numChildren; ++i)
	{
        CountDirsUnderSize(currentDir->children[i], count, sizeTarget);
	}

    if (currentDir->size < sizeTarget)
    {
        (*count)++;
    }
}

void FreeDir(Dir* currentDir)
{
	for (int i = 0; i < currentDir->numChildren; ++i)
	{
        FreeDir(currentDir->children[i]);
	}

    if (currentDir->children != NULL)
    {
        free(currentDir->children);
    }
    
    free(currentDir->name);
    free(currentDir);
}

void ExecuteDay07_Part1(DayData* dayData)
{
    //char* currentDir = NULL;
    Dir* rootDir = NULL;
    Dir* currentDir = NULL;
    BOOL isInLSMode = FALSE;
    int test = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        test++;
        char* str = dayData->m_Data[i];
        if (str[0] == '$')
        {
            isInLSMode = FALSE;
            if (str[2] == 'c' && str[3] == 'd')
            {
                const int cdOffset = sizeof("$ cd ");

                if (str[5] == '.' && str[6] == '.')
                {
                    currentDir = currentDir->parent;
                }
                else if (currentDir != NULL)
                {
                    for (int i = 0; i < currentDir->numChildren; ++i)
                    {
                        if (strcmp(currentDir->children[i]->name, str + cdOffset - 1) == 0)
                        {
                            currentDir = currentDir->children[i];
                            break;
                        }
                    }
                }
                else
                {
                    currentDir = malloc(sizeof(Dir));
                    Zero(currentDir);

                    currentDir->name = malloc(strlen(str - cdOffset) + 1);
					strcpy(currentDir->name, str + cdOffset - 1);

                    rootDir = currentDir;
                }
            }
			else if (str[2] == 'l' && str[3] == 's')
			{
                isInLSMode = TRUE;
			}
        }
        else if (isInLSMode)
        {
			Dir* childDir = malloc(sizeof(Dir));
            Zero(childDir);

            childDir->parent = currentDir;
            childDir->isDir = str[0] == 'd' && str[1] == 'i' && str[2] == 'r';
            
            if (childDir->isDir)
            {
				const int cdOffset = sizeof("dir ");
				childDir->name = malloc(strlen(str - cdOffset) + 1);
				strcpy(childDir->name, str + cdOffset - 1);
            }
            else
            {
                char temp[20];
                strcpy(temp, str);

                int nameStartIndex = 0;
                while (str[nameStartIndex] != '\0')
                {
                    if (str[nameStartIndex++] == ' ')
                    {
                        break;
                    }
                }

                strtok(temp, " ");
                childDir->name = malloc(strlen(str + nameStartIndex) + 1);
                strcpy(childDir->name, str + nameStartIndex);
                childDir->size = atoi(temp);
                childDir->parent->size += childDir->size;
            }
            
            if (currentDir->numChildren == 0)
            {
                currentDir->children = malloc(++currentDir->numChildren * sizeof(Dir*));
            }
            else
            {
                currentDir->children = realloc(currentDir->children, ++currentDir->numChildren * sizeof(Dir*));
            }
			
			currentDir->children[currentDir->numChildren - 1] = childDir;
        }
    }

    int count = 0;
    const int sizeTarget = 100000;
    CountDirsUnderSize(rootDir, &count, sizeTarget);

    FreeDir(rootDir);

    printf("Number of directories under %i: %i.", sizeTarget, count);
}

void ExecuteDay07_Part2(DayData* dayData)
{
    
}