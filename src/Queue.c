#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>

void enqueue(Queue* q, int data)
{
  QueueNode* newNode = malloc(sizeof(QueueNode));
  newNode->data = data;
  newNode->next = NULL;

  if(q->size == 0)
  {
    q->tail = newNode;
    q->head = newNode;
  }
  else
  {
    q->tail->next = newNode;
    q->tail = newNode;
  }
  q->size++;
}

int dequeue(Queue* q)
{
  if(q->size == 0)
    return null;

  int head = q->head->data;
  QueueNode* hat = q->head;
  q->head = q->head->next;
  q->size--;

  // free hat free hat
  // he killed those babies in self defense
  free(hat);

  return head;
}

void printQueue(Queue* q)
{
  QueueNode* curr = q->head;
  while(curr != NULL)
  {
    printf("%d\n", curr->data);
    curr = curr->next;
  }
  printf("\n");
}
