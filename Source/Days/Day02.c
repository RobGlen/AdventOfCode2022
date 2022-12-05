 #include "Day02.h"

 #include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const int ROCK = 1;
static const int PAPER = 2;
static const int SCISSORS = 3;

static const int WIN = 6;
static const int DRAW = 3;
static const int LOSE = 0;

typedef struct
{
    char id1;
    char id2;
    int value;
    int scoreModifier;
} RPSInfo;

static const int RPS_SIZE = 3;
static const RPSInfo rpsInfoList[3] = 
{
    { 'A', 'X', 1, -1 },
    { 'B', 'Y', 2, 0 },
    { 'C', 'Z', 3, 1 }
};


void ExecuteDay02_Part1(struct DayData* dayData)
{  
    int totalScore = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        int myScore = 0;
        int enemyScore = 0;
        for (int j = 0; j < RPS_SIZE; ++j)
        {
            if (dayData->m_Data[i][0] == rpsInfoList[j].id1)
            {
                enemyScore = rpsInfoList[j].value;
            }

            if (dayData->m_Data[i][2] == rpsInfoList[j].id2)
            {
                myScore = rpsInfoList[j].value;
            }
        }

        const int scoreDiff = myScore - enemyScore;
        if (scoreDiff == 1 || scoreDiff == -2)
        {
            totalScore += WIN;
        }
        else if (scoreDiff == 0)
        {
            totalScore += DRAW;
        }
        else
        {
            totalScore += LOSE;
        }

        totalScore += myScore;
    }

    printf("Total rock paper scissors score: %i\n", totalScore);
}

void ExecuteDay02_Part2(struct DayData* dayData)
{
    int totalScore = 0;
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        int myScoreModifier = 0;
        int enemyScore = 0;
        for (int j = 0; j < RPS_SIZE; ++j)
        {
            if (dayData->m_Data[i][0] == rpsInfoList[j].id1)
            {
                enemyScore = rpsInfoList[j].value;
            }

            if (dayData->m_Data[i][2] == rpsInfoList[j].id2)
            {
                myScoreModifier = rpsInfoList[j].scoreModifier;
            }
        }

        int myScore = (enemyScore + myScoreModifier);
        const int range = RPS_SIZE - 1 + 1;
        myScore += ((myScore < 1) ? range * ((1 - myScore) / range + 1) : 0);
        myScore = 1 + (myScore - 1) % range;

        const int scoreDiff = myScore - enemyScore;
        if (scoreDiff == 1 || scoreDiff == -2)
        {
            totalScore += WIN;
        }
        else if (scoreDiff == 0)
        {
            totalScore += DRAW;
        }
        else
        {
            totalScore += LOSE;
        }

        totalScore += myScore;
    }

    printf("Total rock paper scissors score: %i\n", totalScore);
}