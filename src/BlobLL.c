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
void add(BlobLL* dummy, Node* newNode)
{
  Node* curr = dummy->dummy;
  if(curr->next != NULL)
  {
    curr = curr->next;
  }
  curr->next = newNode;

  newNode->next = NULL;
  newNode->prev = curr;

  dummy->tail = newNode;
}

void addHead(BlobLL* dummy, Node* newNode)
{
  Node* nextNode = dummy->dummy->next;
  if(nextNode != NULL)
  {
    nextNode->prev = newNode;
    newNode->prev = dummy->dummy;
    newNode->next = nextNode;
  }
  else
  {
    newNode->next = NULL;
    newNode->prev = dummy->dummy;
  }
  dummy->dummy->next = newNode;
}

void addData(BlobLL* dummy, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(dummy, newNode);
}

void addHeadData(BlobLL* dummy, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(dummy, newNode);
}

BlobLL* getListPointer(struct Node* n)
{
  Node* curr = n;

  while(curr->prev != NULL)
  {
    curr = curr->prev;
  }
  // previous is null and data is null then there is no dummy for some reaosn
  // dummies should be the node with previous as null AND data as null
  return curr->list;
}

Node* getTail(BlobLL* dummy)
{
  return dummy->tail;
}

// assuming that dummy1 and dummy2 are non empty
void mergeLinkedLists(BlobLL* dummy1, BlobLL* dummy2)
{
}

void printLinkedList(BlobLL* dummy)
{
  Node* curr = dummy->dummy;
  while(curr->next != NULL)
  {
    curr = curr->next;
    printNode(curr);
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
    printf("There is no dummy node");
}

void printNode(Node* curr)
{
  printf("This pointer:%p\n", curr);
  printf("Previous Pointer: %p\n", curr->prev);
  printf("Next Pointer: %p\n", curr->next);
  printData(curr->data);
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
