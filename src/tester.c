/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "ColorClamp.h"
#include "BlobLL.h"
#include "BlobManager.h"
#include "Queue.h"

void printHeadLL(HeadLL* linkedList)
{
  HeadNode* curr = linkedList->head;
  while(curr != NULL)
  {
    LineBlob* lb = curr->data->data;
    printf("Row: %d\n", lb->row);
    printf("Start index: %d End index: %d\n", lb->startIndex, lb->endIndex);
    printf("Red avg: %d Green avg: %d Blue avg: %d\n",
            lb->averages[0], lb->averages[1], lb->averages[2]);
    curr = curr->next;
  }

  printf("Size of Linked List: %d\n", linkedList->size);
}

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf("Invalid input. Exiting\n");
    exit(0);
  }

  // reading in the ppm file into img
  struct Image* img = malloc(sizeof(struct Image));
  ReadImage(argv[1], img);

  byte* visitedArray = malloc(sizeof(int) * img->NofC * img->NofR);
  int i;
  for(i=0; i<img->NofC * img->NofR;i++)
  {
    visitedArray[i] = 0;
  }
  double tol = 20;
  Blob b1 = CalculateBlob(img, tol, visitedArray, 0);
  printf("%d\n", b1.size);
  printf("%d %d %d\n", b1.color[0], b1.color[1], b1.color[2]);

  for(i=0; i<b1.size;i++)
  {
    printf("%d\n", b1.indeces[i]);
    img->red[b1.indeces[i]] = 255;
    img->green[b1.indeces[i]] = 255;
    img->blue[b1.indeces[i]] = 255;
  }
  WriteImage("blobbed.ppm", *img);

  exit(0);
}
