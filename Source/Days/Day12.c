#include "Day12.h"

#include "../Day.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Pos
{
    int x;
    int y;
} Pos;

typedef struct Node
{
    unsigned int id;

    Pos pos;
    unsigned int value;
    BOOL isVisited;
    char element;
    
    struct Node* parent;
    struct Node* left;
    struct Node* right;
} Node;

void DetermineGrid(DayData* const dayData, unsigned int** grid, const int width, Pos* start, Pos* end)
{
    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        grid[i] = malloc(sizeof(int) * width);

        for (int j = 0; j < width; ++j)
        {
            const char curChar = dayData->m_Data[i][j];
            if (curChar == 'S')
            {
                (*start).x = j;
                (*start).y = i;
                grid[i][j] = 0;
            }
            else if (curChar == 'E')
            {
                (*end).x = j;
                (*end).y = i;
                grid[i][j] = 'z' - 'a';
            }
            else
            {
                grid[i][j] = curChar - 'a'; 
            }
        }
    }
}

void InitNode(Node* const node, const int x, const int y, const int width)
{
    node->id = y * width + x;
    node->pos.x = x;
    node->pos.y = y;
    node->value = 0;
    node->isVisited = FALSE;
    node->element = 0;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
}

void FindAndAppendToNode(Node* const node, Node* const nodeToAdd)
{
    if ((node == NULL) || (nodeToAdd == NULL))
    {
        return;
    }

    if (node == nodeToAdd)
    {
        printf("Hello there.\n");
    }

    if (node->left == NULL)
    {
        node->left = nodeToAdd;
        nodeToAdd->parent = node;
        return;
    }
    else if (node->left->id > nodeToAdd->id && node->right == NULL)
    {
        node->right = node->left;
        node->left = nodeToAdd;
        nodeToAdd->parent = node;
        return;
    }
    else if (node->right == NULL)
    {
        node->right = nodeToAdd;
        nodeToAdd->parent = node;
        return;
    }

    if (node->id < nodeToAdd->id)
    {
        FindAndAppendToNode(node->left, nodeToAdd);
        return;
    }
    
    FindAndAppendToNode(node->right, nodeToAdd);
}

Node* FindNode(Node* const node, const unsigned int id)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->id == id)
    {
        return node;
    }

    Node* foundNode = FindNode(node->left, id);

    if (foundNode == NULL)
    {
        foundNode = FindNode(node->right, id);
    }
    
    return foundNode;
}

void FindNodeWithSmallestDistance(Node* const node, Node** smallest)
{
    if (node == NULL)
    {
        return;
    }

    if ((*smallest) == NULL || node->value < (*smallest)->value)
    {
        (*smallest) = node;
    }

    FindNodeWithSmallestDistance(node->left, smallest);
    FindNodeWithSmallestDistance(node->right, smallest);
}

int FindSmallestId(Node* node)
{
    if (node->left != NULL)
    {
        return FindSmallestId(node->left);
    }

    return node->id;
}

void PrintGrid(Node* rootUnvisitedNode, Node* rootVisitedNode, const int width, const int height, Pos start, Pos end)
{
    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            Node* node = FindNode(rootUnvisitedNode, i * width + j);

            if (node != NULL)
            {
                if (node->value == -1)
                {
                    printf("\x1B[31m.");
                }
                else if ((start.x == j && start.y == i) || (end.x == j && end.y == i))
                {
                    printf("\x1B[33m%i", node->value);
                }
                else
                {
                    printf("\x1B[31m%i", node->value);
                }
                
                //printf("\x1B[32m"); 
                //printf("\033[0m");
                //printf("\x1B[31m%i", tree);
            }
            else
            {
                node = FindNode(rootVisitedNode, i * width + j);

                if (node != NULL)
                {
                    if ((start.x == j && start.y == i) || (end.x == j && end.y == i))
                    {
                        printf("\x1B[33m%c", node->element);
                    }
                    else
                    {
                        printf("\x1B[32m%c", node->element);
                    }
                }
                else
                {
                    printf("\x1B[31m[x]");
                }
            }
        }
        printf("\033[0m\n");
    }
    printf("\n");
}

void FindSmallestNodeOfChar(Node* const node, unsigned int* smallestSteps, Node** smallestNode, const char charToFind)
{
    if (node == NULL)
    {
        return;
    }

    FindSmallestNodeOfChar(node->left, smallestSteps, smallestNode, charToFind);
    FindSmallestNodeOfChar(node->right, smallestSteps, smallestNode, charToFind);

    if ((node->element == charToFind) && (node->value < (*smallestSteps)))
    {
        (*smallestSteps) = node->value;
        (*smallestNode) = node;
    }
}

Node* ExecuteDijkstra(Node* nodes,
    unsigned int** grid,
    const int width,
    const int height,
    const Pos start,
    const Pos end,
    const BOOL isReverse,
    const BOOL findLowestSteps,
    const char targetChar)
{
    //Mark all nodes unvisited. Create a set of all the unvisited nodes called the unvisited set.
    //Assign to every node a tentative distance value: set it to zero for our initial node and to infinity for all other nodes.
    //During the run of the algorithm, the tentative distance of a node v is the length of the shortest path discovered so far between
    // the node v and the starting node. Since initially no path is known to any other vertex than the source itself (which is a path of length zero),
    // all other tentative distances are initially set to infinity. Set the initial node as current.[16]
    int nodeUseIndex = 0;
    int iteration = 0;

    Node* rootUnvisitedNode = &nodes[nodeUseIndex++];
    Node* rootVisitedNode = NULL;
    InitNode(rootUnvisitedNode, width / 2, height / 2, width);
    if (rootUnvisitedNode->id != start.y * width + start.x)
    {
        rootUnvisitedNode->value = -1; // (max int)
    }
    //Node* nodeToAppendTo = rootUnvisitedNode;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (rootUnvisitedNode->pos.x == j && rootUnvisitedNode->pos.y == i)
            {
                continue;
            }

            Node* const newNode = &nodes[nodeUseIndex++];
            InitNode(newNode, j, i, width);
            newNode->element = grid[i][j] + 'a';

            if (newNode->id != start.y * width + start.x)
            {
                newNode->value = -1; // (max int)
            }

            FindAndAppendToNode(rootUnvisitedNode, newNode);
        }
    }

    //For the current node, consider all of its unvisited neighbors and calculate their tentative distances through the current node.
    
    //Compare the newly calculated tentative distance to the one currently assigned to the neighbor and assign it the smaller one.
    //For example, if the current node A is marked with a distance of 6, and the edge connecting it with a neighbor B has length 2,
    //then the distance to B through A will be 6 + 2 = 8. If B was previously marked with a distance greater than 8 then change it to 8.
    //Otherwise, the current value will be kept.
    Pos adjacents[4] = 
        {
            { -1, 0 },
            { 1, 0 },
            { 0, -1 },
            { 0, 1 },
        };

    Node* destinationNode = FindNode(rootUnvisitedNode, end.y * width + end.x);

    while(rootUnvisitedNode != NULL && !destinationNode->isVisited)
    {
        Node* node = NULL;
        FindNodeWithSmallestDistance(rootUnvisitedNode, &node);
        
        for(int i = 0; i < 4; ++i)
        {
            Pos curPos = node->pos;
            curPos.x += adjacents[i].x;
            curPos.y += adjacents[i].y;

            if (curPos.x < 0 || curPos.y < 0 || curPos.x >= width || curPos.y >= height)
            {
                continue;
            }

            Node* neighbourNode = FindNode(rootUnvisitedNode, curPos.y * width + curPos.x);
            if (neighbourNode != NULL)
            {
                const int diff = grid[curPos.y][curPos.x] - grid[node->pos.y][node->pos.x];

                if ((!isReverse && diff > 1) || (isReverse && diff < -1))
                {
                    continue;
                }

                const int newValue = node->value + 1;
                if (neighbourNode->value > newValue)
                {
                    neighbourNode->value = newValue;
                }
            }
        }

        //When we are done considering all of the unvisited neighbors of the current node, mark the current node as visited and remove it from the unvisited set.
        //A visited node will never be checked again (this is valid and optimal in connection with the behavior in step 6.:
        //that the next nodes to visit will always be in the order of 'smallest distance from initial node first' so any visits after would have a greater distance).
        
        node->isVisited = TRUE;

        Node* parent = node->parent;

        node->parent = NULL;

        if (node == rootUnvisitedNode)
        {
            rootUnvisitedNode = NULL;
        }

        if (parent != NULL)
        {
            if (parent->left == node)
            {
                parent->left = NULL;
            }
            else
            {
                parent->right = NULL;
            }
        }

        if (rootUnvisitedNode == NULL)
        {
            rootUnvisitedNode = node->left;
        }
        else
        {
            FindAndAppendToNode(rootUnvisitedNode, node->left);
        }
        FindAndAppendToNode(rootUnvisitedNode, node->right);

        node->left = NULL;
        node->right = NULL;

        if (rootVisitedNode != NULL)
        {
            FindAndAppendToNode(rootVisitedNode, node);
        }
        else
        {
            rootVisitedNode = node;
        }

        //If the destination node has been marked visited (when planning a route between two specific nodes) or if the smallest tentative distance among the nodes
        //in the unvisited set is infinity (when planning a complete traversal; occurs when there is no connection between the initial node and remaining unvisited nodes),
        //then stop. The algorithm has finished.



        //Otherwise, select the unvisited node that is marked with the smallest tentative distance, set it as the new current node, and go back to step 3.
        iteration++;

        //PrintGrid(rootUnvisitedNode, rootVisitedNode, width, height, start, end);
    }

    PrintGrid(rootUnvisitedNode, rootVisitedNode, width, height, start, end);

    if (findLowestSteps)
    {
        unsigned int smallestSteps = -1;
        Node* smallestNode = NULL;
        FindSmallestNodeOfChar(rootVisitedNode, &smallestSteps, &smallestNode, 'a');
        return smallestNode;
    }

    return destinationNode;
}

void ExecuteDay12_Part1(DayData* dayData)
{
    Pos start = { 0, 0 };
    Pos end = { 0, 0 };

    int width = 0;
    {
        int j = 0;
        while(dayData->m_Data[0][j] != '\0')
        {
            ++j;
        }

        width = j;
    }

    // y * k + x
    unsigned int** grid = malloc(sizeof(int*) * dayData->m_DataLength);

    DetermineGrid(dayData, grid, width, &start, &end);

    Node* nodes = malloc(sizeof(Node) * width * dayData->m_DataLength);
    
    Node* endNode = ExecuteDijkstra(nodes, grid, width, dayData->m_DataLength, start, end, FALSE, FALSE, 0);
    printf("Lowest from a -> b: %u\n", endNode->value);


    free(nodes);

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        free(grid[i]);
    }

    free(grid);
}

void ExecuteDay12_Part2(DayData* dayData)
{
    Pos start = { 0, 0 };
    Pos end = { 0, 0 };

    int width = 0;
    {
        int j = 0;
        while(dayData->m_Data[0][j] != '\0')
        {
            ++j;
        }

        width = j;
    }

    // y * k + x
    unsigned int** grid = malloc(sizeof(int*) * dayData->m_DataLength);

    DetermineGrid(dayData, grid, width, &start, &end);

    Node* nodes = malloc(sizeof(Node) * width * dayData->m_DataLength);
    
    Node* endNode = ExecuteDijkstra(nodes, grid, width, dayData->m_DataLength, end, start, TRUE, TRUE, 'a');
    printf("Smallest steps to an 'a': %u", endNode->value);
    free(nodes);

    for (int i = 0; i < dayData->m_DataLength; ++i)
    {
        free(grid[i]);
    }

    free(grid);
}