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

  return new;
}

Node createDummy()
{
  Node new;
  new.next = NULL;
  new.prev = NULL;
  new.data = NULL;

  return new;
}


// same as add last
void add(Node* dummy, Node* newNode)
{
  Node* curr = dummy;
  if(curr->next != NULL)
  {
    curr = curr->next;
  }
  curr->next = newNode;

  newNode->next = NULL;
  newNode->prev = curr;
}

void addHead(Node* dummy, Node* newNode)
{
  Node* nextNode = dummy->next;
  if(nextNode != NULL)
  {
    nextNode->prev = newNode;
    newNode->prev = dummy;
    newNode->next = nextNode;
  }
  else
  {
    newNode->next = NULL;
    newNode->prev = dummy;
  }
  dummy->next = newNode;
}

void addData(Node* dummy, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(dummy, newNode);
}

void addHeadData(Node* dummy, LineBlob* data)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->data = data;
  add(dummy, newNode);
}

Node* getDummy(struct Node* n)
{
  Node* curr = n;

  while(curr->prev != NULL)
  {
    curr = curr->prev;
  }
  // previous is null and data is null then there is no dummy for some reaosn
  // dummies should be the node with previous as null AND data as null
  if(curr->data != NULL)
    return NULL;
  return curr;
}


void printLinkedList(Node* dummy)
{
  Node* curr = dummy;
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
