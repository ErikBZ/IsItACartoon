#ifndef HEADBLOBLL_H
#define HEADBLOBLL_H

#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "BlobLL.h"

typedef struct HeadLL
{
  struct HeadNode* head;
  struct HeadNode* tail;
  int size;
} HeadLL;

typedef struct HeadNode
{
  struct HeadNode* next;
  Node* data;

} HeadNode;

// Methods for the LinkedList
// add will add to the end
void addHeadNode(HeadLL* headLinkedList, HeadNode* node);
void addHeadNodeData(HeadLL* headLinkedList, Node* node);
void printHeadList(HeadLL headLinkedList);
void printHeadData(HeadNode* data);
void resetHeadLL(HeadLL* headLinkedList);

#endif
