// used to protect against multiple includes
#ifndef BLOBLLH_
#define BLOBLLH_

#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "ColorClamp.h"

#define null -1

typedef struct BlobLL
{
  struct Node* head;
  struct Node* tail;
  byte occupied;
  int* color;
  int nextEmptyBlobLL;
  int size;
} BlobLL;

// contains all the BlobLL that were created
// meaning that this has all the heads of the blobs of the image
typedef struct BlobPool
{
  struct BlobLL* blobPool;
  int firstEmptyBlobList;
  int size;
  int maxSize;
} BlobPool;

// startIndex and endIndex will be the index for that row
// in order to get the real index you'll have to mutliply it by "row"
// which will give you the index in the big array created by image.h
typedef struct LineBlob
{
  int startIndex;
  int endIndex;
  int row;
  // 0 is red, 1 is green, 2 is blue
  byte* averages;
} LineBlob;

// if node is a head node then it points to 0
// else all new nodes point to -1
// I CAN JUST USE *VOID YAYYYY
typedef struct Node
{
  struct Node* next;
  struct Node* prev;
  LineBlob* data;
  BlobLL* list;
} Node;

//stuff for the Linked List
// same as addLast
void add(BlobLL* blob, struct Node* newNode);
void addHead(BlobLL* blob, struct Node* newNode);
void addData(BlobLL* blob, LineBlob* data);
void addHeadData(BlobLL* blob, LineBlob* data);
BlobLL* getListPointer(Node* n);
BlobLL createBlobLL(Node* n);
Node* getTail(BlobLL* blob);
// add the two linked lists and free blob1
// add 1 to 2 and get rid of 1
void mergeLinkedLists(BlobLL* blob1, BlobLL* blob2);
void printLinkedList(BlobLL* blob);
void printBackwards(Node* n);
void printNode(Node* n);
void printData(LineBlob* lb);
void reset(BlobLL* ll);
// since data is LineBlob* then this will be the best thing to
// use to create line blobs
LineBlob* createLine(int s, int e, int r, byte* averages);
Node* createNode(int s, int e, int r, byte* averages);
Node* putInNode(LineBlob* lb);
Node createDummy();
// keep this at the bottom
#endif
