#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "BlobLL.h"
#include "string.h"
#include "ColorClamp.h"
#include "BlobManager.h"
#include "HeadBlobLL.h"

void AddLineBlobToArray(LineBlob** lbArray, LineBlob* lbp,
                        int* size, int* maxSize)
{
  (*size) = (*size) + 1;
  if(size >= maxSize)
  {
    (*maxSize) = (*maxSize) * 2;
    LineBlob** newLineLBlobArray = malloc(sizeof(LineBlob*) * (*maxSize));
    memcpy(newLineLBlobArray, lbArray, (*size)-1 * sizeof(LineBlob));
    free(lbArray);
    lbArray = newLineLBlobArray;
  }

  lbArray[*size] = lbp;
}


HeadLL* calculateRow(struct Image* img, int row, HeadLL* currLinkedList, double tol)
{
  resetHeadLL(currLinkedList);

  int i=0;

  // calculates the index where the row starts in the
  // 1D byte array
  int rowStartIndex = row * img->NofC;
  byte* color = malloc(sizeof(byte)*3);
  LineBlob* newLineBlob = NULL;
  int index = rowStartIndex + i;

  newLineBlob = malloc(sizeof(LineBlob));
  newLineBlob->startIndex = index;
  newLineBlob->row = row;
  newLineBlob->averages = malloc(sizeof(byte)*3);
  newLineBlob->averages[0] = img->red[index];
  newLineBlob->averages[1] = img->green[index];
  newLineBlob->averages[2] = img->blue[index];

  for(i=0;i<=img->NofC;i++)
  {
    index = rowStartIndex + i;
    color[0] = img->red[index];
    color[1] = img->green[index];
    color[2] = img->blue[index];

    if(newLineBlob == NULL)
    {
      newLineBlob = malloc(sizeof(LineBlob));
      newLineBlob->startIndex = index-1;
      newLineBlob->row = row;
      newLineBlob->averages = malloc(sizeof(byte)*3);
      newLineBlob->averages[0] = img->red[index-1];
      newLineBlob->averages[1] = img->green[index-1];
      newLineBlob->averages[2] = img->blue[index-1];
    }

    double rad = threeVarRadius(newLineBlob->averages, color);

    if(rad>tol || i==img->NofC)
    {
      newLineBlob->endIndex = i + rowStartIndex-1;
      Node* lbNode = malloc(sizeof(Node));
      lbNode->data = newLineBlob;
      HeadNode* headNode = malloc(sizeof(HeadNode));
      headNode->data = lbNode;
      addHeadNode(currLinkedList, headNode);
      newLineBlob = NULL;
    }
  }
  return currLinkedList;
}

HeadLL* calculateRowWithAverage(struct Image* img, int row, HeadLL* currLinkedList,
                                double tol)
{
  resetHeadLL(currLinkedList);

  int i=0;

  // calculates the index where the row starts in the
  // 1D byte array
  int rowStartIndex = row * img->NofC;
  byte* color = malloc(sizeof(byte)*3);
  LineBlob* newLineBlob = NULL;
  int index = rowStartIndex + i;

  newLineBlob = malloc(sizeof(LineBlob));
  newLineBlob->startIndex = index;
  newLineBlob->row = row;
  newLineBlob->averages = malloc(sizeof(byte)*3);
  newLineBlob->averages[0] = img->red[index];
  newLineBlob->averages[1] = img->green[index];
  newLineBlob->averages[2] = img->blue[index];

  for(i=0;i<=img->NofC;i++)
  {
    index = rowStartIndex + i;
    color[0] = img->red[index];
    color[1] = img->green[index];
    color[2] = img->blue[index];

    if(newLineBlob == NULL)
    {
      newLineBlob = malloc(sizeof(LineBlob));
      newLineBlob->startIndex = index-1;
      newLineBlob->row = row;
      newLineBlob->averages = malloc(sizeof(byte)*3);
      newLineBlob->averages[0] = img->red[index-1];
      newLineBlob->averages[1] = img->green[index-1];
      newLineBlob->averages[2] = img->blue[index-1];
    }

    double rad = threeVarRadius(newLineBlob->averages, color);

    if(rad>tol || i==img->NofC)
    {
      newLineBlob->endIndex = i + rowStartIndex-1;
      Node* lbNode = malloc(sizeof(Node));
      lbNode->data = newLineBlob;
      HeadNode* headNode = malloc(sizeof(HeadNode));
      headNode->data = lbNode;
      addHeadNode(currLinkedList, headNode);
      newLineBlob = NULL;
    }
    else
    {
      float n = 1 + index - newLineBlob->startIndex;
      newLineBlob->averages[0] = (newLineBlob->averages[0]) * ((n-1)/n) + color[0]/n;
      newLineBlob->averages[1] = (newLineBlob->averages[1]) * ((n-1)/n) + color[1]/n;
      newLineBlob->averages[2] = (newLineBlob->averages[2]) * ((n-1)/n) + color[2]/n;
    }
  }
  return currLinkedList;
}

void MergeRows(HeadLL* prevRow, HeadLL* currRow, BlobPool* blobPool, double tol, int NofC)
{
  // this should only happen on the first row created
  if(prevRow == NULL)
  {
    HeadNode* node = currRow->head;
    while(node != NULL)
    {
      Node* lbNode = node->data;
      BlobLL blobList;
      blobList.occupied = 1;
      blobList.size = 0;
      blobList.color = NULL;
      blobList.tail = NULL;
      blobList.head = NULL;
      addHead(&blobList, lbNode);
      AddBlobLLToListPool(blobPool, blobList);

      node = node->next;
    }
  }
  else
  {
    HeadNode* currRowNode = currRow->head;
    while(currRowNode != NULL)
    {
      int found = CheckAbove(prevRow, currRowNode->data, tol, NofC);
      // no blobLL were found in the previous row for this node
      if(!found)
      {
        BlobLL blobList;
        blobList.occupied = 1;
        blobList.size = 0;
        blobList.color = NULL;
        blobList.tail = NULL;
        blobList.head = NULL;
        addHead(&blobList, currRowNode->data);
        AddBlobLLToListPool(blobPool, blobList);
      }
      // there is no else because CheckAbove deals with the found cases
      currRowNode = currRowNode->next;
    }
  }
}

// now i just gott test and and find that this doesn't work whatsoever
// i'll need to recreate this later
void FindBlobsInImage(BlobPool* pool, struct Image* img, double tol)
{
  int i,j;
  byte color[3];

  // the rows don't need a color
  HeadLL* oldRow = malloc(sizeof(HeadLL));
  oldRow->head = NULL;
  oldRow->tail = NULL;

  HeadLL* thisRow = malloc(sizeof(HeadLL));
  thisRow->head = NULL;
  thisRow->tail = NULL;

  for(i=0;i<img->NofR;i++)
  {
    LineBlob* newLB = malloc(sizeof(LineBlob));
    newLB->startIndex = 0;
    newLB->row = i;
    newLB->averages = malloc(sizeof(byte)*3);

    // starting color to compare othLier colors too
    newLB->averages[0] = img->red[i*img->NofR];
    newLB->averages[1] = img->green[i*img->NofR];
    newLB->averages[2] = img->blue[i*img->NofR];

    for(j=0;j<img->NofC;j++)
    {
      int index = i*img->NofR + j;
      // comparing this color to "color" above
      byte nextColor[3];
      nextColor[0] = img->red[i*img->NofR + j];
      nextColor[1] = img->green[i*img->NofR + j];
      nextColor[2] = img->blue[i*img->NofR + j];
      double rad = threeVarRadius(newLB->averages, nextColor);

      // printf("%lf\n", rad);

      // if the colors are different by more than the tolerances allowed
      if(rad > tol)
      {
        newLB->endIndex = j-1;
        Node* newLBNode = malloc(sizeof(Node));
        newLBNode->data = newLB;
        newLBNode->prev = NULL;
        newLBNode->next = NULL;
        newLBNode->list = NULL;

        byte found = CheckAbove(oldRow, newLBNode, tol, 0);

        if(found == 0)
        {
          BlobLL b = createBlobLL(newLBNode);
          addHead(&b, newLBNode);
          AddBlobLLToListPool(pool, b);
        }
        // adding a node that points to nweLBNode as data
        // to this row

        // debugging, checking to see how thisRow is being created
        // printLinkedList(thisRow);
        // printf("\n");

        // mallocing a new LineBlob since the previous one is now closed
        newLB = malloc(sizeof(LineBlob));
        newLB->startIndex = j;
        newLB->row = i;
        newLB->averages = malloc(sizeof(byte)*3);
        newLB->averages[0] = nextColor[0];
        newLB->averages[1] = nextColor[1];
        newLB->averages[2] = nextColor[2];
      }
    }
    // closing the final lineblob
    newLB->endIndex = j;
    Node* newLBNode = malloc(sizeof(Node));
    newLBNode->data = newLB;
    newLBNode->prev = NULL;
    newLBNode->next = NULL;
    newLBNode->list = NULL;

    byte found = CheckAbove(oldRow, newLBNode, tol, 0);
    if(found == 0 )
    {
      BlobLL b = createBlobLL(newLBNode);
      addHead(&b, newLBNode);
      AddBlobLLToListPool(pool, b);
    }
    // add row to HeadNodes

    // have to free oldRow then set this row to old row
    // and malloc a new head or free everything else except the
    // LL so that i can reuse it. I r smat
    // for some reason at row 2 one of the heads has a prev adderess
    // when it should be null
    resetHeadLL(oldRow);
    HeadLL* temp = oldRow;
    oldRow = thisRow;
    thisRow = temp;
  }
}

// woo this works just fine!
void AddBlobLLToListPool(BlobPool* blobPool, BlobLL n)
{
  BlobLL* blobArray = blobPool->blobPool;

  if((blobPool->size) >= (blobPool->maxSize))
  {
    (blobPool->maxSize) = (blobPool->maxSize) * 2;
    BlobLL* newNodePool = malloc(sizeof(BlobLL) * (blobPool->maxSize));
    memcpy(newNodePool, blobArray, blobPool->size * sizeof(BlobLL));
    free(blobArray);
    blobPool->blobPool = newNodePool;

    int i;
    for(i=0;i<blobPool->size;i++)
    {
      if((blobPool->blobPool)[i].head != NULL)
      (blobPool->blobPool)[i].head->list = &((blobPool->blobPool)[i]);
    }
  }
  (blobPool->blobPool)[blobPool->size] = n;
  blobPool->size = blobPool->size + 1;
}

// assume that the head of lb is this dummy
// i'll be slightly changing this function
// returns false if there were no linblobs with colors similar
// to "lb" and returns true if there were
// if two or more line blobs have similar color but different
// LinkedList, lists are merged and the latter lists are set to NULL
int CheckAbove(HeadLL* rowLL, Node* lbNode, double tol, int NofC)
{
  int LLFound = 0;
  if(rowLL->head == NULL)
    return LLFound;

  HeadNode* curr = rowLL->head;
  while(curr != NULL)
  {
    // curr holds a node to a node, so we need a doubel data Pointer
    // yay for nodes of nodes
    // creating a new struct for HeadNodes
    Node* currLBNode = curr->data;

    byte c = IsSimilarColor(currLBNode->data, lbNode->data, tol);
    byte a = IsAdjacent(currLBNode->data, lbNode->data, NofC);

    if(a && c)
    {
      BlobLL* llPointer = getListPointer(currLBNode);

      if(lbNode->list == NULL)
      {
        if(llPointer != NULL)
        {
          addHead(llPointer, lbNode);
          LLFound = 1;
        }
        else
        {
          LLFound = 0;
        }
      }
      else
      {
        BlobLL* oldBlobP = getListPointer(lbNode);
        mergeLinkedLists(oldBlobP, llPointer);
        LLFound = 1;
      }
    }
    curr = curr->next;
  }
  return LLFound;
}

byte IsSimilarColor(LineBlob* lb1, LineBlob* lb2, double tol)
{
  byte similarColor = 0;

  double dist = threeVarRadius(lb1->averages, lb2->averages);

  if(dist < tol)
    similarColor = 1;

  return similarColor;
}

double threeVarRadius(byte* arr1, byte* arr2)
{
  int red = arr1[0] - arr2[0];
  int green =  arr1[1] - arr2[1];
  int blue =  arr1[2] - arr2[2];

  double dist = sqrt(red*red + green*green + blue*blue);
  return dist;
}

byte IsAdjacent(LineBlob* lbChecking, LineBlob* curr, int NofC)
{
  byte adjacent = 0;
  // normalizing the indeces to positoin from its index in the array
  int prevLbStart = lbChecking->startIndex - (lbChecking->row * NofC);
  int prevLbNed;
  int currLbStart;
  int currLbNed;

  // checking to see if it is adjacent to the line blob above it
  if((curr->startIndex >= lbChecking->startIndex
      && curr->startIndex <= lbChecking->endIndex) ||
     (curr->endIndex >= lbChecking->startIndex
      && curr->endIndex <= lbChecking->endIndex) ||
     (curr->startIndex < lbChecking->startIndex
      && curr->endIndex > lbChecking->endIndex))
  {
    adjacent = 1;
  }
  return adjacent;
}
