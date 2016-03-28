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

void AddNodeToNodePool(Node** nodePool, Node* n, int* size, int* maxSize)
{
  (*size) = (*size) + 1;
  if(size >= maxSize)
  {
    (*maxSize) = (*maxSize) * 2;
    Node** newNodePool = malloc(sizeof(Node*) * (*maxSize));
    memcpy(newNodePool, nodePool, (*size)-1 * sizeof(Node*));
    free(nodePool);
    nodePool = newNodePool;
  }

  nodePool[*size] = n;
}

void CheckAbove(Node* rowDummyNode, Node* thisDummy, LineBlob* lb,
                double tol)
{
  Node* curr = rowDummyNode->next;
  while(curr != NULL)
  {
    byte t = IsSimilarColor(curr->data, lb, tol);
    if(t)
    {

    }
    curr = curr->next;
  }
}

byte IsSimilarColor(LineBlob* lb1, LineBlob* lb2, double tol)
{
  byte similarColor = 0;

  int red = lb1->averages[0] - lb2->averages[0];
  int green = lb1->averages[1] - lb2->averages[1];
  int blue = lb1->averages[2 ] - lb2->averages[2];

  double dist = sqrt(red*red + green*green + blue*blue);

  if(dist < tol)
    similarColor = 1;

  return similarColor;
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
