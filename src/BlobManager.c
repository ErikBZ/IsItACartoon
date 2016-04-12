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

// BlobLL has to be a double pointer so that the real pointer
// can be changed. now i can also free the malloc'd pointer prior
void AddBlobLLToListPool(BlobLL** blobPool, BlobLL n, int* size, int* maxSize)
{
  if((*size) >= (*maxSize))
  {
    (*maxSize) = (*maxSize) * 2;
    BlobLL* newNodePool = malloc(sizeof(BlobLL) * (*maxSize));
    memcpy(newNodePool, *blobPool, (*size) * sizeof(BlobLL));
    free(*blobPool);
    *blobPool = newNodePool;
  }
  (*blobPool)[*size] = n;
  (*size) = (*size) + 1;
}

// assume that the head of lb is this dummy
void CheckAbove(BlobLL* rowLL, BlobLL* thisDummy, LineBlob* lb,
                double tol)
{
  Node* curr = rowLL->head;
  while(curr != NULL)
  {
    byte c = IsSimilarColor(curr->data, lb, tol);
    byte a = IsAdjacent(curr->data, lb);

    if(a && c)
    {
      Node* blobNode = curr->data;
      BlobLL* llPointer = getListPointer(blobNode);
      mergeLinkedLists(thisDummy, llPointer);
    }
    curr = curr->next;
  }
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
