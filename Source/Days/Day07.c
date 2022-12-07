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

Dir* ParseDirs(DayData* dayData)
{
    Dir* rootDir = NULL;
    Dir* currentDir = NULL;
    BOOL isInLSMode = FALSE;

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        char* str = dayData->m_Data[i];
        if (str[0] == '$')
        {
            isInLSMode = FALSE;
            if (str[2] == 'c' && str[3] == 'd')
            {
                const int cdOffset = sizeof("$ cd ") - 1;

                if (str[5] == '.' && str[6] == '.')
                {
                    currentDir = currentDir->parent;
                }
                else if (currentDir != NULL)
                {
                    for (int i = 0; i < currentDir->numChildren; ++i)
                    {
                        if (strcmp(currentDir->children[i]->name, str + cdOffset) == 0)
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

                    currentDir->name = malloc(strlen(str + cdOffset) + 1);
                    strcpy(currentDir->name, str + cdOffset);

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
                const int cdOffset = sizeof("dir ") - 1;
                childDir->name = malloc(strlen(str + cdOffset) + 1);
                strcpy(childDir->name, str + cdOffset);
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

                Dir* parent = childDir->parent;
                while (parent != NULL)
                {
                    parent->size += childDir->size;
                    parent = parent->parent;
                }
                //childDir->parent->size += childDir->size;
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

    return rootDir;
}

void CountDirsUnderSize(Dir* currentDir, int* const count, int* sum, const int sizeTarget)
{
    if (currentDir == NULL)
    {
        return;
    }

    for (int i = 0; i < currentDir->numChildren; ++i)
    {
        CountDirsUnderSize(currentDir->children[i], count, sum, sizeTarget);
    }

    if (currentDir->size <= sizeTarget && currentDir->isDir)
    {
        (*count)++;
        (*sum) += currentDir->size;
    }
}

void FindSmallestDirToFreeup(Dir* currentDir, Dir** smallestDir, const int freeupTarget)
{
    for (int i = 0; i < currentDir->numChildren; ++i)
    {
        FindSmallestDirToFreeup(currentDir->children[i], smallestDir, freeupTarget);
    }

    if (currentDir->size >= freeupTarget && currentDir->size < (*smallestDir)->size)
    {
        *smallestDir = currentDir;
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
    Dir* rootDir = ParseDirs(dayData);

    int count = 0;
    int sum = 0;
    const int sizeTarget = 100000;
    CountDirsUnderSize(rootDir, &count, &sum, sizeTarget);

    FreeDir(rootDir);

    printf("Number of directories under %i: %i. Sum: %i\n", sizeTarget, count, sum);
}

void ExecuteDay07_Part2(DayData* dayData)
{
    Dir* rootDir = ParseDirs(dayData);
    
    const int totalSpace = 70000000;
    const int freeUpTarget = 30000000;
    const int sizeLeft = 70000000 - rootDir->size;
    const int sizeToFreeUp = freeUpTarget - sizeLeft;

    Dir* smallestDirToFreeup = rootDir;
    
    FindSmallestDirToFreeup(rootDir, &smallestDirToFreeup, sizeToFreeUp);

    printf("Size of dir to freeup: %i\n", smallestDirToFreeup->size);

    FreeDir(rootDir);
}