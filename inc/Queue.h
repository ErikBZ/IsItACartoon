#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>

#define null -1

typedef struct Queue
{
  struct QueueNode* head;
  struct QueueNode* tail;
  int size;
} Queue;

typedef struct QueueNode
{
  struct QueueNode* next;
  int data;
} QueueNode;

void enqueue(Queue* q, int data);
int dequeue(Queue* q);
void printQueue(Queue* q);

#endif
