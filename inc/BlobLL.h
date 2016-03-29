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
  struct Node* dummy;
  struct Node* tail;
  int color[3];
  int size;
  int maxSize;
} BlobLL;

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

// if node is a dummy node then it points to 0
// else all new nodes point to -1
typedef struct Node
{
  struct Node* next;
  struct Node* prev;
  LineBlob* data;
  BlobLL* list;
} Node;

/*
// things for building the Blob
void AddLineBlobToArray(struct LineBlob** lbArray, struct LineBlob* lb,
                        int* size, int* maxSize);
void AddNodeToNodePool(Node** nodePool, Node* n, int* size, int* maxSize);
// adds all the nodes that contain data similar to the given LineBlob
void CheckAbove(Node* rowDummyNode, Node* thisDummy, LineBlob* lb,
                double tol);
byte IsAdjacent(LineBlob* lbChecking, LineBlob* curr);
byte IsSimilarColor(LineBlob* lb1, LineBlob* lb2, double tol);
*/
//stuff for the Linked List
// same as addLast
void add(BlobLL* dummy, struct Node* newNode);
void addHead(BlobLL* dummy, struct Node* newNode);
void addData(BlobLL* dummy, LineBlob* data);
void addHeadData(BlobLL* dummy, LineBlob* data);
BlobLL* getListPointer(Node* n);
Node* getTail(BlobLL* dummy);
// add the two linked lists and free dummy1
// add 1 to 2 and get rid of 1
void mergeLinkedLists(BlobLL* dummy1, BlobLL* dummy2);
void printLinkedList(BlobLL* dummy);
void printBackwards(Node* n);
void printNode(Node* n);
void printData(LineBlob* lb);

LineBlob* createLine(int s, int e, int r, byte* averages);
Node* createNode(int s, int e, int r, byte* averages);
Node* putInNode(LineBlob* lb);
Node createDummy();

// keep this at the bottom
#endif
