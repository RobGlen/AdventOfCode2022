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
    BOOL isList;
} PacketElement;

typedef struct PacketPair
{
    PacketElement* firstPacket;
    PacketElement* secondPacket;
} PacketPair;

typedef struct Packet
{
    PacketElement* packet;
    struct Packet* prev;
    struct Packet* next;
} Packet;

void InitPacketElement(PacketElement* const element)
{
    element->value = -1;
    element->next = NULL;
    element->list = NULL;
    element->parent = NULL;
    element->listLength = 0;
    element->isList = FALSE;
}

void InitPacketPair(PacketPair* const packetPair)
{
    packetPair->firstPacket = NULL;    
    packetPair->secondPacket = NULL;
}

void InitPacket(Packet* const packet)
{
    packet->packet = NULL;
    packet->prev = NULL;
    packet->next = NULL;
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

    if (element->isList)
    {
        printf("(");
    }

    if (element->value != -1)
    {
        printf("%i", element->value);
    }

    PrintTree(element->list);

    if (element->isList)
    {
        printf(")");
    }

    if (element->next != NULL)
    {
        printf(",");
    }

    PrintTree(element->next);
}

void ParsePacket(char* const str, PacketElement** rootElement)
{
    PacketElement* currentElement = *rootElement;
    PacketElement* currentList = NULL;
    int i = 0;
    BOOL wasLastItemList = FALSE;
    while(str[i] != '\0')
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            char* newStr = strdup(str + i);
            strtok(newStr, ",");
            strtok(newStr, "]");
            
            PacketElement* newNumElement = malloc(sizeof(PacketElement));
            InitPacketElement(newNumElement);

            if (wasLastItemList)
            {
                currentElement->list = newNumElement;    
                newNumElement->parent = currentElement;
            }
            else
            {
                currentElement->next = newNumElement;    
                newNumElement->parent = currentElement->parent;
            }

            if (newNumElement->parent != NULL)
            {
                newNumElement->parent->listLength++;
            }

            newNumElement->value = atoi(newStr);

            currentElement = newNumElement;

            wasLastItemList = FALSE;

            free(newStr);
            while(str[i + 1] != ',' && str[i + 1] != ']')
            {
                ++i;
            }
        }
        else if (str[i] == '[')
        {
            PacketElement* newElement = malloc(sizeof(PacketElement));
            InitPacketElement(newElement);
            if (currentElement == NULL)
            {
                currentElement = newElement;
                (*rootElement) = currentElement;
            }
            else if (wasLastItemList)
            {
                currentElement->list = newElement;
                newElement->parent = currentElement;
            }
            else
            {
                currentElement->next = newElement;
                newElement->parent = currentElement->parent;
            }

            if (newElement->parent != NULL)
            {
                newElement->parent->listLength++;
            }
                 
            currentElement = newElement;
            currentElement->isList = TRUE;
            wasLastItemList = TRUE;
            currentList = currentElement;
        }
        else if (str[i] == ']')
        {
            if (currentElement != NULL && !currentElement->isList || currentElement->list != NULL)
            {
                if (currentElement->parent != NULL)
                {
                    currentElement = currentElement->parent;
                }
            }
            wasLastItemList = FALSE;
        }
        
        ++i;
        //PrintTree(rootElement);
        //printf("\n");
    }
    //PrintTree(*rootElement);
    //printf("\n\n");
}

void ParsePackets(DayData* const dayData, PacketPair* const packets)
{
    for(int i = 0; i < dayData->m_DataLength; i += 3)
    {
        const int packetPairIndex = i / 3;
        PacketPair* const packetPair = &packets[packetPairIndex];
        InitPacketPair(packetPair);

        ParsePacket(dayData->m_Data[i], &packetPair->firstPacket);
        ParsePacket(dayData->m_Data[i + 1], &packetPair->secondPacket);
    }
}

BOOL CompareElement(PacketElement* left, PacketElement* right, BOOL* isRightOrder)
{
    //PrintTree(left);
    //printf("\n");
    //PrintTree(right);
    //printf("\n\n");

    if (left == NULL || right == NULL)
    {
        if (left != right)
        {
            (*isRightOrder) = left == NULL;
            return TRUE;
        }

        return FALSE;
    }

    if(!left->isList && !right->isList)
    {
        if(left->value != right->value)
        {
            (*isRightOrder) = left->value < right->value;
            return TRUE;
        }
        
    }

    if(left->isList && !right->isList)
    {
        if (left->list == NULL)
        {
            (*isRightOrder) = TRUE;
            return TRUE;
        }

        if (left->list->isList)
        {
            return CompareElement(left->list, right, isRightOrder);
        }

        if (left->list->value != right->value)
        {
            (*isRightOrder) = left->list->value < right->value;
            return TRUE;
        }

        if(left->list->value == right->value && left->listLength != 1)
        {
            (*isRightOrder) = left->listLength < 1;
            return TRUE;
        }
        
    }

    if(!left->isList && right->isList)
    {
        if (right->list == NULL)
        {
            (*isRightOrder) = FALSE;
            return TRUE;
        }

        if (right->list->isList)
        {
            return CompareElement(left, right->list, isRightOrder);
        }

        if (left->value != right->list->value)
        {
            (*isRightOrder) = left->value < right->list->value;
            return TRUE;
        }

        if(left->value == right->list->value && right->listLength != 1)
        {
            (*isRightOrder) = right->listLength > 1;
            return TRUE;
        }
        
    }

    return FALSE;
}

BOOL ComparePacketElement(PacketElement* left, PacketElement* right, BOOL* isRightOrder)
{
    if (CompareElement(left, right, isRightOrder))
    {
        return TRUE;
    }

    if (left == NULL || right == NULL)
    {
        return FALSE;
    }

    if (left->isList && right->isList)
    {
        if (ComparePacketElement(left->list, right->list, isRightOrder))
        {
            return TRUE;
        }
    }

     if (!left->isList && right->isList)
     {
         if (CompareElement(left->next, right->list, isRightOrder))
         {
             return TRUE;
         }
     }

     if (left->isList && !right->isList)
     {
         if (CompareElement(left->list, right->next, isRightOrder))
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
        //printf("%i:\n", i + 1);
        BOOL isRightOrder = FALSE;
        ComparePacketElement(packets[i].firstPacket, packets[i].secondPacket, &isRightOrder);
        if (isRightOrder)
        {
            printf("%i is in right order\n", i + 1);
            //PrintTree(packets[i].firstPacket);
            //printf("\n");
            //PrintTree(packets[i].secondPacket);
            //printf("\n\n");
            sumOfIndices += i + 1;
        }
        else
        {
            printf("%i is wrong:\n", i + 1);
            //PrintTree(packets[i].firstPacket);
            //printf("\n");
            //PrintTree(packets[i].secondPacket);
            //printf("\n\n");
        }
    }

    return sumOfIndices;
}

Packet* SortPacket(Packet* const currentPacket, Packet* const packetToAdd, BOOL* shouldAddAfter)
{
	if (currentPacket == NULL)
	{
		return NULL;
	}

	BOOL isPacketAfter = FALSE;
	ComparePacketElement(currentPacket->packet, packetToAdd->packet, &isPacketAfter);

	if (isPacketAfter)
	{
		Packet* packet = SortPacket(currentPacket->next, packetToAdd, shouldAddAfter);
		if (packet != NULL)
		{
			return packet;
		}
		else
		{
			(*shouldAddAfter) = TRUE;
			return currentPacket;
		}
	}
	else
	{
		(*shouldAddAfter) = FALSE;
		return currentPacket;
	}
}

void InsertPacket(Packet** head, Packet* const newPacket, PacketElement* const packetElement)
{
	InitPacket(newPacket);
	newPacket->packet = packetElement;

	BOOL shouldAddAfter = FALSE;
	Packet* packetToConnectTo = SortPacket(*head, newPacket, &shouldAddAfter);

	if (shouldAddAfter)
	{
		packetToConnectTo->next = newPacket;
		newPacket->prev = packetToConnectTo;
	}
	else
	{
		Packet* prevPacket = packetToConnectTo->prev;
		if (packetToConnectTo != (*head))
		{
			prevPacket->next = newPacket;
		}
		else
		{
			(*head) = newPacket;
		}
		newPacket->next = packetToConnectTo;
        newPacket->prev = prevPacket;
		packetToConnectTo->prev = newPacket;
	}
}

Packet* ProcessAndSortPackets(Packet* packetsPool, PacketPair* packetPairs, const int packetsLength)
{
    printf("Sorting packets...\n");
	int currentPacketIndex = 0;
	Packet* head = &packetsPool[currentPacketIndex++];
	InitPacket(head);
	Packet* currentPacket = head;
	for (int i = 0; i < packetsLength; ++i)
	{
        printf("Sorting packet %i.\n", i);
		if (head->packet == NULL)
		{
			head->packet = packetPairs[i].firstPacket;
		}
		else
		{
			InsertPacket(&head, &packetsPool[currentPacketIndex++], packetPairs[i].firstPacket);
        }

        InsertPacket(&head, &packetsPool[currentPacketIndex++], packetPairs[i].secondPacket);
	}

	return head;
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
	const int originalPacketsLength = (dayData->m_DataLength / 3) + 1;
    const int packetsLength = originalPacketsLength + 1;
	PacketPair* packetPairs = malloc(sizeof(PacketPair) * packetsLength);
	ParsePackets(dayData, packetPairs);

	PacketPair* const dividerPackets = &packetPairs[packetsLength - 1];
	InitPacketPair(dividerPackets);

	ParsePacket("[[2]]", &dividerPackets->firstPacket);
	ParsePacket("[[6]]", &dividerPackets->secondPacket);

    Packet* packets = malloc(sizeof(Packet) * packetsLength * 2);

    Packet* head = ProcessAndSortPackets(packets, packetPairs, packetsLength);

    int i = 0;
    int i2 = 0;
    int i6 = 0;
    while (head != NULL)
    {
        PrintTree(head->packet);
        printf("\n");
        if (head->packet == dividerPackets->firstPacket)
        {
            i2 = i + 1;
        }
        else if (head->packet == dividerPackets->secondPacket)
        {
            i6 = i + 1;
        }

        head = head->next;
        ++i;
    }

    printf("decoder key: %i\n", i2 * i6);

    free(packets);
	for (int i = 0; i < packetsLength; ++i)
	{
		FreePackets(packetPairs[i].firstPacket);
		FreePackets(packetPairs[i].secondPacket);
	}
	free(packetPairs);
}