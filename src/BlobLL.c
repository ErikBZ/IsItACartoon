/*
  Author: Erik Zapata
  BlobFinder is my 3rd time trying to make a blob finding algorithmn
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "image.h"
#include "BlobLL.h"
#include "string.h"

LineBlob* createLine(int s, int e, int r, byte* averages)
{
  // astericks go after the struct woops
  LineBlob* newBlob = malloc(sizeof(LineBlob));
  newBlob->startIndex = s;
  newBlob->endIndex = e;
  newBlob->row = r;
  newBlob->averages = averages;

  return newBlob;
}

Node* createNode(int s, int e, int r, byte* averages)
{
  LineBlob* lb = createLine(s, e, r, averages);
  Node* new = putInNode(lb);
  return new;
}

Node* putInNode(LineBlob* lb)
{
  Node* new = malloc(sizeof(Node));
  new->next = NULL;
  new->prev = NULL;
  new->data = lb;
  new->list = NULL;

  return new;
}

Node createDummy()
{
  Node new;
  new.next = NULL;
  new.prev = NULL;
  new.data = NULL;
  new.list = NULL;

  return new;
}


// same as add last
void add(BlobLL* blob, Node* newNode)
{
  if(blob->size == 0)
  {
    blob->head = newNode;
    blob->tail = newNode;
  }
  else
  {
    blob->tail->next = newNode;
    newNode->prev = blob->tail;
    blob->tail = newNode;
    newNode->next = NULL;
  }
  blob->size += 1;
}

void addHead(BlobLL* blob, Node* newNode)
{
  if(blob->size == 0)
  {
    blob->head = newNode;
    blob->tail = newNode;
  }
  else
  {
    newNode->next = blob->head;
    blob->head = newNode;
  }
  blob->size += 1;
}

void addData(BlobLL* blob, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(blob, newNode);
}

void addHeadData(BlobLL* blob, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(blob, newNode);
}

BlobLL* getListPointer(struct Node* n)
{
  Node* curr = n;

  while(curr->prev != NULL)
  {
    curr = curr->prev;
  }
  // previous is null and data is null then there is no blob for some reaosn
  // dummies should be the node with previous as null AND data as null
  return curr->list;
}

Node* getTail(BlobLL* blob)
{
  return blob->tail;
}

// assuming that blob1 and blob2 are non empty
void mergeLinkedLists(BlobLL* blob1, BlobLL* blob2)
{
}

void printLinkedList(BlobLL* blob)
{
  Node* curr = blob->head;
  while(curr != NULL)
  {
    printNode(curr);
    curr = curr->next;
  }
}

void printBackwards(Node* n)
{
  Node* curr = n;
  while(curr->next != NULL)
  {
    printNode(curr);
    curr= curr->prev;
  }
  if(curr->data == NULL)
    printNode(curr);
  else
    printf("There is no head node");
}

void printNode(Node* curr)
{
  printf("This pointer:%p\n", curr);
  printf("Previous Pointer: %p\n", curr->prev);
  printf("Next Pointer: %p\n", curr->next);
}

void printData(LineBlob* lb)
{
  if(lb == NULL)
    printf("NODE IS EMPTY\n\n");
  else
  {
      printf("LineBlob start index: %d\n", lb->startIndex);
      printf("LineBlob end index: %d\n\n", lb->endIndex);
  }
}
