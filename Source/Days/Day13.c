#include "Day13.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct PacketElement
{
    int value;
    struct PacketElement* next;
    struct PacketElement* list;
    struct PacketElement* parent;
    int listLength;
} PacketElement;

typedef struct PacketPair
{
    PacketElement* firstPacket;
    PacketElement* secondPacket;
} PacketPair;

void InitPacketElement(PacketElement* const element)
{
    element->value = -1;
    element->next = NULL;
    element->list = NULL;
    element->parent = NULL;
    element->listLength = 0;
}

void InitPacketPair(PacketPair* const packetPair)
{
    packetPair->firstPacket = malloc(sizeof(PacketElement));
    packetPair->secondPacket = malloc(sizeof(PacketElement));

    InitPacketElement(packetPair->firstPacket);
    InitPacketElement(packetPair->secondPacket);
}

void FreePackets(PacketElement* element)
{
    if (element == NULL)
    {
        return;
    }

    if (element->list != NULL)
    {
        FreePackets(element->list);
    }
    
    FreePackets(element->next);

    free(element);
}

void PrintTree(PacketElement* element)
{
    if (element == NULL)
    {
        return;
    }

    if (element->list != NULL)
    {
        printf("(");
    }

    if (element->value != -1)
    {
        printf("%i,", element->value);
    }

    PrintTree(element->list);

    if (element->list != NULL)
    {
        printf(")");
    }

    PrintTree(element->next);
}

void ParsePacket(char* const str, PacketElement* rootElement)
{
    PacketElement* currentElement = rootElement;
    int i = 0;
    BOOL wasLastItemList = FALSE;
    while(str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            char* newStr = strdup(str);
            strtok(newStr, ",");
            strtok(newStr, "]");
            
            PacketElement* newNumElement = NULL;
            if (wasLastItemList)
            {
                newNumElement = currentElement;
            }
            else
            {
                newNumElement = malloc(sizeof(PacketElement));
                InitPacketElement(newNumElement);
                currentElement->next = newNumElement;    
                newNumElement->parent = currentElement->parent;
            }

            if (newNumElement->parent != NULL)
            {
                newNumElement->parent->listLength++;
            }

            newNumElement->value = atoi(newStr + i);

            if (currentElement->next)
            {
                currentElement = currentElement->next;    
            }

            wasLastItemList = FALSE;

            free(newStr);
            while(str[i + 1] != ',' && str[i + 1] != ']')
            {
                ++i;
            }
        }
        else if (str[i] == '[')
        {
            PacketElement* newListElement = malloc(sizeof(PacketElement));
            InitPacketElement(newListElement);

            //newListElement->list = malloc(sizeof(PacketElement));
            //InitPackeInitPacketPairtElement(newListElement->list);
            if (currentElement->value != -1)
            {
                PacketElement* newElement = malloc(sizeof(PacketElement));
                InitPacketElement(newElement);
                currentElement->next = newElement;
                newElement->parent = currentElement->parent;
                currentElement = currentElement->next;
            }
            
            currentElement->list = newListElement;
            newListElement->parent = currentElement;

            //currentElement->next = newListElement;

            currentElement = newListElement;
            wasLastItemList = TRUE;
        }
        else if (str[i] == ']')
        {
            if (currentElement != NULL)
            {
                currentElement = currentElement->parent;
                if (currentElement != NULL && (str[i + 1] == ','))
                {
                    PacketElement* newElement = malloc(sizeof(PacketElement));
                    InitPacketElement(newElement);
                    currentElement->next = newElement;
                    newElement->parent = currentElement->parent;
                    currentElement = currentElement->next;
                    wasLastItemList = TRUE;
                }
            }
        }
        
        ++i;
        //PrintTree(rootElement);
        //printf("\n");
    }
}

void ParsePackets(DayData* const dayData, PacketPair* const packets)
{
    for(int i = 0; i < dayData->m_DataLength; i += 3)
    {
        const int packetPairIndex = i / 3;
        PacketPair* const packetPair = &packets[packetPairIndex];
        InitPacketPair(packetPair);

        ParsePacket(dayData->m_Data[i], packetPair->firstPacket);
        ParsePacket(dayData->m_Data[i + 1], packetPair->secondPacket);
    }
}

//void ComparePacketElement(PacketElement* left, PacketElement* right, BOOL* isRightOrder)
// {
//     if (left == NULL || right == NULL)
//     {
//         (*isRightOrder) = left == NULL && right != NULL;
//         if (left != NULL && right == NULL)
//         {
//             (*isRightOrder) = FALSE;
//         }

//         return;
//     }

//     if (left->value != -1 && right->value != -1)
//     {
//         if (left->value != right->value)
//         {
//             (*isRightOrder) = left->value < right->value;
//             return;
//         }

//         if (left->value > right->value)
//         {
//             (*isRightOrder) = FALSE;
//             return;
//         }
//     }

//     if (left->parent != NULL && left->next == NULL && right->next != NULL)
//     {
//         (*isRightOrder) = TRUE;
//     }

//     if (left->list != NULL && right->list != NULL)
//     {
//         ComparePacketElement(left->list, right->list, isRightOrder);
//     }

//     if (left->value != -1 && right->list != NULL)
//     {
//         ComparePacketElement(left, right->list, isRightOrder);
//     }

//     if (left->list != NULL && right->value != -1)
//     {
//         ComparePacketElement(left->list, right, isRightOrder);
//     }

//     if (left->list != NULL && right->next)
//     {

//     }

//     ComparePacketElement(left->list, right->list, isRightOrder);
//     ComparePacketElement(left, right->list, isRightOrder);
//     ComparePacketElement(left->list, right, isRightOrder);

//     if (left->list != NULL && right->list == NULL)
//     {
//         (*isRightOrder) = FALSE;
//         return;
//     })


//     ComparePacketElement(left->next, right->next, isRightOrder);
// }

BOOL CompareElement(PacketElement* left, PacketElement* right, BOOL* isRightOrder, BOOL isList)
{
    if (left == NULL || right == NULL)
    {
        if (left != right)
        {
            (*isRightOrder) = left == NULL;
            return TRUE;
        }

        return FALSE;
    }

    // if (!isList && (left->list != NULL || right->list != NULL))
    // {
    //     return FALSE;
    // }

    if ((left->value != right->value) && (left->value != -1 || left->list == NULL) && (right->value != -1 || right->list == NULL))
    {
        (*isRightOrder) = left->value < right->value;
        return TRUE;
    }

    //if (isList)
    {
        if (left->parent != NULL && right->parent == NULL && left->value <= right->value && left->parent->listLength != 1)
        {
            (*isRightOrder) = left->parent->listLength < 1;
            return TRUE;
        }

        if (right->parent != NULL && left->parent == NULL&& left->value <= right->value && right->parent->listLength != 1)
        {
            (*isRightOrder) = right->parent->listLength > 1;
            return TRUE;
        }
    }
    // if (left->parent != NULL && right->parent != NULL)
    // {
    //     if (left->parent->listLength != right->parent->listLength)
    //     {
    //         (*isRightOrder) = left->parent->listLength < right->parent->listLength;
    //         return TRUE;
    //     }
    // }

    return FALSE;
}

BOOL ComparePacketElement(PacketElement* left, PacketElement* right, BOOL* isRightOrder)
{
    if (CompareElement(left, right, isRightOrder, FALSE))
    {
         return TRUE;
    }

    if (left == NULL || right == NULL)
    {
        return FALSE;
    }

    if (left->list != NULL && right->list != NULL)
    {
        if (ComparePacketElement(left->list, right->list, isRightOrder))
        {
            return TRUE;
        }
    }

    if (left->value != -1 && right->list != NULL)
    {
        if (CompareElement(left, right->list, isRightOrder, TRUE))
        {
            return TRUE;
        }
    }

    if (left->list != NULL && right->value != -1)
    {
        if (CompareElement(left->list, right, isRightOrder, TRUE))
        {
            return TRUE;
        }
    }

    return ComparePacketElement(left->next, right->next, isRightOrder);
}

int ProcessPackets(PacketPair* const packets, const int length)
{
    int sumOfIndices = 0;
    for (int i = 0; i < length; ++i)
    {
        BOOL isRightOrder = FALSE;
        ComparePacketElement(packets[i].firstPacket, packets[i].secondPacket, &isRightOrder);
        if (isRightOrder)
        {
            printf("%i is in right order\n", i + 1);
            PrintTree(packets[i].firstPacket);
            printf("\n");
            PrintTree(packets[i].secondPacket);
            printf("\n\n");
            sumOfIndices += i + 1;
        }
        else
        {
            printf("%i is wrong:\n", i + 1);
            PrintTree(packets[i].firstPacket);
            printf("\n");
            PrintTree(packets[i].secondPacket);
            printf("\n\n");
        }
    }

    return sumOfIndices;
}

void ExecuteDay13_Part1(DayData* dayData)
{
    const int packetsLength = (dayData->m_DataLength / 3) + 1;
    PacketPair* packets = malloc(sizeof(PacketPair) * packetsLength);
    ParsePackets(dayData, packets);

    const int sumOfIndices = ProcessPackets(packets, packetsLength);
    printf("Sum of packets in right order: %i\n", sumOfIndices);

    for(int i = 0; i < packetsLength; ++i)
    {
        FreePackets(packets[i].firstPacket);
        FreePackets(packets[i].secondPacket);
    }
    free(packets);
}

void ExecuteDay13_Part2(DayData* dayData)
{
   
}