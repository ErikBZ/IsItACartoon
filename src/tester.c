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

int main(int argc, char** argv)
{
  // struct Image* img = malloc(sizeof(struct Image));
  // char* filename = argv[1];
  // ReadImage(filename, img);

  BlobPool pool;
  pool.blobPool = malloc(sizeof(BlobLL)*2);
  pool.size = 0;
  pool.maxSize = 2;

  BlobLL b1;
  b1.size = 1;
  BlobLL b2;
  b2.size = 2;
  BlobLL b3;
  b3.size = 3;
  BlobLL b4;
  b4.size = 4;

  AddBlobLLToListPool(&pool, b1);
  AddBlobLLToListPool(&pool, b2);
  AddBlobLLToListPool(&pool, b3);
  AddBlobLLToListPool(&pool, b4);

  printf("%d\n", pool.size);
  printf("%d\n", pool.maxSize);

  int i;
  for(i=0;i<4;i++)
  {
    printf("%d\n", pool.blobPool[i].size);
  }
}
