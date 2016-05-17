#include <stdlib.h>
#include <stdio.h>
#include "HeadBlobLL.h"
#include "string.h"

// TODO put stuff for HeadBlobLL
void addHeadNode(HeadLL* headLinkedList, HeadNode* node)
{
  if(headLinkedList->head == NULL)
  {
    headLinkedList->head = node;
    headLinkedList->tail = node;
  }
  else
  {
    headLinkedList->tail->next = node;
    headLinkedList->tail = node;
  }
  node->next = NULL;
  headLinkedList->size++;
}

void addHeadNodeData(HeadLL* headLinkedList, Node* node)
{
  HeadNode* headNode = malloc(sizeof(HeadNode));
  headNode->data = node;
  addHeadNode(headLinkedList, headNode);
}

void printHeadList(HeadLL headLinkedList)
{

}

void printHeadData(HeadNode* data)
{

}

void resetHeadLL(HeadLL* headLinkedList)
{
  headLinkedList->size = 0;
  headLinkedList->head = NULL;
  headLinkedList->tail = NULL;
}
