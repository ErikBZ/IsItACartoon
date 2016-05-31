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
    newNode->list = blob;
    newNode->prev = NULL;
    newNode->next = NULL;
  }
  else
  {
    blob->tail->next = newNode;
    newNode->prev = blob->tail;
    blob->tail = newNode;
    newNode->next = NULL;
    newNode->list = NULL;
  }
  blob->size = blob->size + 1;
}

void addHead(BlobLL* blob, Node* newNode)
{
  if(blob->size == 0)
  {
    blob->head = newNode;
    blob->tail = newNode;
    newNode->list = blob;
    newNode->prev = NULL;
    newNode->next = NULL;
  }
  else
  {
    newNode->list = blob;
    newNode->prev = NULL;
    newNode->next = blob->head;
    blob->head->prev = newNode;
    blob->head->list = NULL;
    blob->head = newNode;
    blob->tail->next = NULL;
  }
  blob->size = blob->size + 1;
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

BlobLL createBlobLL(Node* n)
{
  BlobLL blob;
  blob.size = 0;
  blob.head = NULL;
  blob.tail = NULL;
  blob.color = malloc(sizeof(byte)*3);
  add(&blob, n);
  return blob;
}

Node* getTail(BlobLL* blob)
{
  return blob->tail;
}

// assuming that blob1 and blob2 are non empty
// blob2 is added to blob1
void mergeLinkedLists(BlobLL* blob1, BlobLL* blob2)
{
  Node* blob2Head = blob2->head;
  blob1->tail->next = blob2Head;
  blob2Head->prev = blob1->tail;
  blob1->tail = blob2->tail;
  // probably a really bad way to calculate the average of 2 averages

  /*
  blob1->color[0] = (blob1->color[0] + blob2->color[0])/2;
  blob1->color[1] = (blob1->color[1] + blob2->color[1])/2;
  blob1->color[2] = (blob1->color[2] + blob2->color[2])/2;
  */
  blob1->size = blob2->size + blob1->size;

  //freeing and reseting most things in blob2
  // i don't know when i'm going to free blob2 though.
  // maybe once the blobs are finshed
  blob2Head->list = NULL;
  blob2->head = NULL;
  blob2->tail = NULL;
  free(blob2->color);
  blob2->color = NULL;
  blob2->occupied = 0;
  blob2->size = 0;
}

void reset(BlobLL* ll)
{
  // free everything from tail to head
  Node* curr = ll->tail;
  while(curr != NULL)
  {
    free(curr);
    curr = curr->prev;
  }
  ll->tail = NULL;
  ll->head = NULL;
  ll->size = 0;
  ll->color = NULL;
}

// all the printing functions are located here
void printLinkedList(BlobLL* blob)
{
  Node* curr = blob->head;
  while(curr != NULL)
  {
    printNode(curr);
    curr = curr->next;
  }
  printf("\n");
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
  printData(curr->data);
}

void printData(LineBlob* lb)
{
  if(lb == NULL)
    printf("NODE IS EMPTY\n\n");
  else
  {
      printf("LineBlob start index: %d\n", lb->startIndex);
      printf("LineBlob end index: %d\n", lb->endIndex);
      printf("%d %d %d\n", lb->averages[0], lb->averages[1], lb->averages[2]);
  }
}

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
double geometricDistance(byte* arr1, byte* arr2)
{
  int red = arr1[0] - arr2[0];
  int green =  arr1[1] - arr2[1];
  int blue =  arr1[2] - arr2[2];

  double dist = sqrt(red*red + green*green + blue*blue);
  return dist;
}

// need this method because img has seperate arrays for rgb
double geometricDistanceWithNoArrays(byte* arr1, byte red, byte blue, byte green)
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

void AverageColors(Blob* b, byte red, byte green, byte blue)
{
  float n = b->size;
  b->color[0] = (b->color[0]) * (n-1)/n + red/n;
  b->color[1] = (b->color[1]) * (n-1)/n + green/n;
  b->color[2] = (b->color[2]) * (n-1)/n + blue/n;
}
