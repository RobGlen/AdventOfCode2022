#include "Day08.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void CompareAndCountTrees(const int treeToConsider, const int treeToCompare, int* tallestTree, int* treeCount, BOOL* shouldExit)
{
    if (treeToCompare >= treeToConsider)
    {
        (*shouldExit) = TRUE;
    }

    (*treeCount)++;
}

void ExecuteDay08_Part1(DayData* dayData)
{
    int width = 0;
    while (dayData->m_Data[0][width] != '\0')
    {
        ++width;
    }

    int visibleTreesCount = 0;

    int* tallestHeightsOnTop = malloc(sizeof(int) * width);
    for (int i = 0; i < width; ++i)
    {
        tallestHeightsOnTop[i] = -1;
    }
    
    for (int y = 0; y < dayData->m_DataLength - 0; ++y)
    {
        int tallestWidthOnLeft = -1;

        for (int x = 0; x < width - 0; ++x)
        {
            const int tree = dayData->m_Data[y][x] - '0';

            BOOL isVisibleFoundFromCached = FALSE;
            if (tree > tallestWidthOnLeft)
            {
                isVisibleFoundFromCached = TRUE;
                tallestWidthOnLeft = tree;
                ++visibleTreesCount;
                printf("\x1B[32m%i", tree);
            }

            if (tree > tallestHeightsOnTop[x])
            {
                tallestHeightsOnTop[x] = tree;
                if (!isVisibleFoundFromCached)
                {
                    ++visibleTreesCount;
                    printf("\x1B[32m%i", tree);
                    isVisibleFoundFromCached = TRUE;
                }
            }

            if (isVisibleFoundFromCached)
            {
                continue;
            }

            BOOL foundTallerTree = FALSE;
            for (int i = y + 1; i < dayData->m_DataLength; ++i)
            {
                if (tree <= dayData->m_Data[i][x] - '0')
                {
                    foundTallerTree = TRUE;
                    break;
                }
            }

            if (!foundTallerTree)
            {
                ++visibleTreesCount;
                printf("\x1B[32m%i", tree);
                continue;
            }
            
            foundTallerTree = FALSE;
            for (int i = x + 1; i < width; ++i)
            {
                if (tree <= dayData->m_Data[y][i] - '0')
                {
                    foundTallerTree = TRUE;
                    break;
                }
            }

            if (!foundTallerTree)
            {
                ++visibleTreesCount;
                printf("\x1B[32m%i", tree);
            }
            else
            {
                printf("\x1B[31m%i", tree);
            }
        }

        printf("\n");
    }

    printf("\033[0m\nVisible trees: %i\n", visibleTreesCount);

    free(tallestHeightsOnTop);
}

void ExecuteDay08_Part2(DayData* dayData)
{
    int width = 0;
    while (dayData->m_Data[0][width] != '\0')
    {
        ++width;
    }

    int heighestTreeScore = -1;
    for (int y = 0; y < dayData->m_DataLength - 0; ++y)
    {
        for (int x = 0; x < width - 0; ++x)
        {
            const int tree = dayData->m_Data[y][x] - '0';

            int treesFound[4] = {0, 0, 0, 0};
            int tallestTree = -1;
            for (int i = y - 1; i >= 0; --i)
            {
                const int currentTree = dayData->m_Data[i][x] - '0';
                BOOL shouldExit = FALSE;
                CompareAndCountTrees(tree, currentTree, &tallestTree, &treesFound[0], &shouldExit);

                if (shouldExit)
                {
                    break;
                }
            }

            tallestTree = -1;
            for (int i = y + 1; i < dayData->m_DataLength; ++i)
            {
                const int currentTree = dayData->m_Data[i][x] - '0';
                BOOL shouldExit = FALSE;
                CompareAndCountTrees(tree, currentTree, &tallestTree, &treesFound[1], &shouldExit);

                if (shouldExit)
                {
                    break;
                }
            }

            tallestTree = -1;
            for (int i = x - 1; i >= 0; --i)
            {
                const int currentTree = dayData->m_Data[y][i] - '0';
                BOOL shouldExit = FALSE;
                CompareAndCountTrees(tree, currentTree, &tallestTree, &treesFound[2], &shouldExit);

                if (shouldExit)
                {
                    break;
                }
            }

            tallestTree = -1;
            for (int i = x + 1; i < width; ++i)
            {
                const int currentTree = dayData->m_Data[y][i] - '0';
                BOOL shouldExit = FALSE;
                CompareAndCountTrees(tree, currentTree, &tallestTree, &treesFound[3], &shouldExit);

                if (shouldExit)
                {
                    break;
                }
            }

            const int treeScore = treesFound[0] * treesFound[1] * treesFound[2] * treesFound[3];
            if (treeScore > heighestTreeScore)
            {
                heighestTreeScore = treeScore;
            }
        }
    }

    printf("Highest tree scenic score: %i\n", heighestTreeScore);
}