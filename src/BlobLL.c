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
#include "Queue.h"

// i may not need anything above but we'll see
int GetPixelUp(int i, int rows, int cols)
{
  if(i>cols)
    return i - cols;
  return -1;
}
int GetPixelDown(int i, int rows, int cols)
{
  if(i< (rows*cols) - cols)
    return i+cols;
  return -1;
}
int GetPixelLeft(int i, int rows, int cols)
{
  if(i%cols)
    return i-1;
  return -1;
}
int GetPixelRight(int i, int rows, int cols)
{
  if((i+1)%cols)
    return i+1;
  return -1;
}

// finds the distance between two colors
double geometricDistance(double* arr1, byte* arr2)
{
  int red = arr1[0] - arr2[0];
  int green =  arr1[1] - arr2[1];
  int blue =  arr1[2] - arr2[2];

  double dist = sqrt(red*red + green*green + blue*blue);
  return dist;
}

// need this method because img has seperate arrays for rgb
double geometricDistanceWithNoArrays(double* arr1, byte red, byte blue, byte green)
{
  byte arr2[3] = {red, blue, green};
  return geometricDistance(arr1, arr2);
}

// takes an initial start index and calculates a blob with it returning
// the calculated blob
// visitedArray is 1 if it has been visited and 0 otherwise
Blob CalculateBlob(struct Image* img, double tol, byte* visited, int start)
{
  if(start < 0)
  {
    fprintf(stderr, "Calculate Blob called with %d. Number not acceptable\n", start);
    return;
  }

  // instantiating the blob to return
  Blob b;
  b.size = 0;
  b.max = 10;
  b.radAvg = 0;
  b.indeces = malloc(sizeof(int) * b.max);
  b.color = malloc(sizeof(byte)*3);
  b.color[0] = img->red[start];
  b.color[1] = img->green[start];
  b.color[2] = img->blue[start];

  // setting up the queue
  Queue intQueue;
  intQueue.tail = NULL;
  intQueue.head = NULL;
  intQueue.size = 0;
  enqueue(&intQueue, start);
  visited[0] = 1;

  while(intQueue.size != 0)
  {
    int next = dequeue(&intQueue);
    AddDataToArray(&b, next);
    AverageColors(&b, img->red[next], img->green[next], img->blue[next]);

    int up = GetPixelUp(next, img->NofR, img->NofC);
    int down = GetPixelDown(next, img->NofR, img->NofC);
    int left = GetPixelLeft(next, img->NofR, img->NofC);
    int right = GetPixelRight(next, img->NofR, img->NofC);

    if(up != null && !visited[up] &&
      tol > geometricDistanceWithNoArrays(b.color, img->red[up], img->green[up], img->blue[up]))
    {
      enqueue(&intQueue, up);
      visited[up] = 1;        // this index has now been visited
    }
    if(down != null && !visited[down] &&
      tol > geometricDistanceWithNoArrays(b.color, img->red[down], img->green[down], img->blue[down]))
    {
      enqueue(&intQueue, down);
      visited[down] = 1;        // this index has now been visited
    }
    if(left != null && !visited[left] &&
      tol > geometricDistanceWithNoArrays(b.color, img->red[left], img->green[left], img->blue[left]))
    {
      enqueue(&intQueue, left);
      visited[left] = 1;        // this index has now been visited
    }
    if(right != null && !visited[right] &&
      tol > geometricDistanceWithNoArrays(b.color, img->red[right], img->green[right], img->blue[right]))
    {
      enqueue(&intQueue, right);
      visited[right] = 1;        // this index has now been visited
    }
  }
  return b;
}

// this returns the final array of blobs that contains all blobs in the image
Blob* GetAllBlobsInImage(struct Image* img, double tol, int* size)
{
  byte* visitedArray = malloc(sizeof(int) * img->NofC * img->NofR);
  *size = 0;
  int max = 10;
  Blob* blobs = malloc(sizeof(Blob)*max);

  int i;
  for(i=0;i<img->NofC * img->NofR;i++)
  {
    visitedArray[i] = 0;
  }

  for(i=0;i<img->NofC * img->NofR;i++)
  {
    if(!visitedArray[i])
    {
      Blob b = CalculateBlob(img, tol, visitedArray, i);
      blobs = AddBlobToArray(blobs, b, size, &max);
    }
  }

  return blobs;
}

// adds an integers to an integer array
void AddDataToArray(Blob* blob, int data)
{
  int* arr = blob->indeces;
  if(blob->size >= blob->max)
  {
    blob->max = blob->size * 2;
    int* newArr = malloc(sizeof(int) * blob->max);
    memcpy(newArr, arr, blob->size * sizeof(int));
    free(arr);
    blob->indeces = newArr;
  }
  (blob->indeces)[blob->size] = data;
  blob->size = blob->size + 1;
}

Blob* AddBlobToArray(Blob* blob, Blob b, int* sizeP, int* maxP)
{
  if(*sizeP >= *maxP)
  {
    *maxP = *maxP * 2;
    Blob* newArr = malloc(sizeof(Blob) * *maxP);
    memcpy(newArr, blob, *sizeP * sizeof(Blob));
    free(blob);
    blob = newArr;
  }
  blob[*sizeP] = b;
  *sizeP = *sizeP + 1;
  return blob;
}

void AverageColors(Blob* b, byte red, byte green, byte blue)
{
  // average keeps going down, need to fix that
  // its because of how ints and floats are rounded
  float n = b->size;
  b->color[0] = (b->color[0]) * (n-1)/n + red/n;
  b->color[1] = (b->color[1]) * (n-1)/n + green/n;
  b->color[2] = (b->color[2]) * (n-1)/n + blue/n;
  double rad = geometricDistanceWithNoArrays(b->color, red, green, blue);
  b->radAvg = b->radAvg * (n-1)/n + rad/n;
}
