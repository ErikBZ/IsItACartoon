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

void testFunction(void* thing);

int main(int argc, char** argv)
{
  // array of Linked Lists representing blobs
  BlobLL* blobs = malloc(sizeof(BlobLL * 4));
  blobsSize = 0;
  blobsMaxSize = 4;

  // used for horizontal lines
  BlobLL row;

  printf("\n");
}

void testFunction(void* thing)
{
  BlobLL* t = thing;
  printf("%d\n", t->size);
}
