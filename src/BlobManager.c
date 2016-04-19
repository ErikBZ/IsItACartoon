#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "BlobLL.h"
#include "string.h"
#include "ColorClamp.h"
#include "BlobManager.h"

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

// now i just gott test and and find that this doesn't work whatsoever
void FindBlobsInImage(BlobPool* pool, struct Image* img, double tol)
{
   int i,j;
   byte color[3];

   // the rows don't need a color
   BlobLL* oldRow = malloc(sizeof(BlobLL));
   oldRow->head = NULL;
   oldRow->tail = NULL;
   oldRow->color = NULL;
   oldRow->size = 0;

   BlobLL* thisRow = malloc(sizeof(BlobLL));
   thisRow->head = NULL;
   thisRow->tail = NULL;
   thisRow->color = NULL;
   thisRow->size = 0;

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

     for(j=0;img->NofC;j++)
     {
       int index = i*img->NofR + j;
       // comparing this color to "color" above
       byte nextColor[3];
       nextColor[0] = img->red[i*img->NofR];
       nextColor[1] = img->red[i*img->NofR];
       nextColor[2] = img->red[i*img->NofR];
       double rad = threeVarRadius(newLB->averages, nextColor);

       // if the colors are different by more than the tolerances allowed
       if(rad > tol)
       {
         newLB->endIndex = j-1;
         Node* newLBNode = malloc(sizeof(Node));
         newLBNode->data = newLB;
         CheckAbove(oldRow, newLBNode, tol);
         // adding a node that points to nweLBNode as data
         // to this row
         addData(thisRow, newLBNode);

         // mallocing a new LineBlob since the previous one is now closed
         newLB = malloc(sizeof(LineBlob));
         newLB->startIndex = j;
         newLB->row = i;
         newLB->averages[0] = nextColor[0];
         newLB->averages[1] = nextColor[1];
         newLB->averages[2] = nextColor[2];
       }
     }
     // closing the final lineblob
     newLB->endIndex = j;
     Node* newLBNode = malloc(sizeof(Node));
     newLBNode->data = newLB;
     CheckAbove(oldRow, newLBNode, tol);
     addData(thisRow, newLBNode);

     // have to free oldRow then set this row to old row
     // and malloc a new head or free everything else except the
     // LL so that i can reuse it. I r smat
     reset(oldRow);
     BlobLL* temp = oldRow;
     oldRow = thisRow;
     thisRow = temp;
  }
}

// BlobLL has to be a double pointer so that the real pointer
// can be changed. now i can also free the malloc'd pointer prior
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
int CheckAbove(BlobLL* rowLL, Node* lbNode, double tol)
{
  int LLFound = 0;
  if(rowLL->head == NULL)
    return LLFound;

  Node* curr = rowLL->head;
  while(curr != NULL)
  {
    // curr holds a node to a node, so we need a doubel data Pointer
    // yay for nodes of nodes
    Node* currLBNode = curr->data;
    byte c = IsSimilarColor(currLBNode->data, lbNode->data, tol);
    byte a = IsAdjacent(currLBNode->data, lbNode->data);

    if(a && c)
    {
      Node* blobNode = curr->data;
      BlobLL* llPointer = getListPointer(blobNode);

      if(lbNode->list == NULL)
      {
        if(llPointer != NULL)
          addHead(llPointer, lbNode);
        else
        {
          llPointer = malloc(sizeof(BlobLL));
          addHead(llPointer, lbNode);
        }
      }
      else
      {
        BlobLL* oldBlobP = getListPointer(lbNode);
        mergeLinkedLists(oldBlobP, llPointer);
      }
      LLFound = 1;
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

byte IsAdjacent(LineBlob* lbChecking, LineBlob* curr)
{
  byte adjacent = 0;

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
