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

  // malloc and initializing the components of BlobPool
  BlobPool pool;
  pool.blobPool = malloc(sizeof(BlobLL)*2);
  pool.size = 0;
  pool.maxSize = 2;

  double tol = 15;
  int i;

  HeadLL* firstRow = malloc(sizeof(HeadLL));
  firstRow = calculateRow(img, 0, firstRow, tol);
  MergeRows(NULL, firstRow, &pool, tol, img->NofC);
  HeadLL* secondRow = malloc(sizeof(HeadLL));
  secondRow = calculateRow(img, 1, secondRow, tol);
  MergeRows(firstRow, secondRow, &pool, tol, img->NofC);

  for(i=0;i<pool.size;i++)
  {
    if(pool.blobPool[i].occupied)
    {
      printLinkedList(&pool.blobPool[i]);
    }
  }

  printf("\n");
  printf("%d\n", firstRow->size);
  printf("%d\n", secondRow->size);
  printf("%d\n", pool.size);

  exit(0);
}
