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

  // initializing tolerance
  // i'll probably change it some
  double tol = 40;

  FindBlobsInImage(&pool, img, tol);

  int i;
  for(i=0;i<pool.size;i++)
  {
    if(pool.blobPool[i].head != NULL)
    {
      printf("%d\n", pool.blobPool[i].size);
    }
  }
}
