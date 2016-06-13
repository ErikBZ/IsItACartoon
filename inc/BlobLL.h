// used to protect against multiple includes
#ifndef BLOBLLH_
#define BLOBLLH_

#include <stdlib.h>
#include <stdio.h>
#include "image.h"

#define null -1

typedef struct Blob
{
  int* indeces;
  int size;
  int max;
  double radAvg;
  double* color;
} Blob;

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

// queue stuff

void enqueue(Queue* q, int data);
int dequeue(Queue* q);
void printQueue(Queue* q);

// if this works i don't need anything above
int GetPixelUp(int i, int rows, int cols);
int GetPixelDown(int i, int rows, int cols);
int GetPixelLeft(int i, int rows, int cols);
int GetPixelRight(int i, int rows, int cols);
double geometricDistance(double* arr1, byte* arr2);
double geometricDistanceWithNoArrays(double* arr1, byte red, byte green, byte blue);
Blob CalculateBlob(struct Image* img, double tol, byte* visitedArray, int start);
void AddDataToArray(Blob* blob, int data);
void AverageColors(Blob* b, byte red, byte green, byte blue);
Blob* AddBlobToArray(Blob* blob, Blob b, int* sizeP, int* maxP);
Blob* GetAllBlobsInImage(struct Image* img, double tol, int* size);

// keep this at the bottom
#endif
