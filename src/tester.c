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

// TODO. TEST THE FUNCTIONS IN BLOBMANAGER TO MAKE SURE THEY WORK PROPERLY

int main(int argc, char** argv)
{
  BlobLL blob1;
  blob1.dummy = NULL;
  blob1.tail = NULL;
  blob1.color = NULL;
  blob1.size = 0;
  blob1.maxSize = 0;

  BlobLL blob2;
  blob2.dummy = NULL;
  blob2.tail = NULL;
  blob2.color = NULL;
  blob2.size = 2;
  blob2.maxSize = 4;

  BlobLL blob3;
  blob3.dummy = NULL;
  blob3.tail = NULL;
  blob3.color = NULL;
  blob3.size = 4;
  blob3.maxSize = 8;

  int size = 0;
  int maxSize = 2;

  BlobLL* blobs = malloc(maxSize * (sizeof(BlobLL)));

  int i;
  for(i=0;i<20;i++)
  {
      BlobLL* b = malloc(sizeof(BlobLL));
      b->dummy = malloc(sizeof(Node));
      b->tail = malloc(sizeof(Node));
      b->color = malloc(sizeof(int) * 3);
      b->size = i;
      b->maxSize = i;

      AddBlobLLToListPool(&blobs, *b, &size, &maxSize);
  }

  for(i=0;i<size;i++)
  {
    printf("%d\n", blobs[i].size);
  }
  printf("\n");
}
