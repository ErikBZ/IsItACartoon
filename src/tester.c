/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "BlobLL.h"
#include "Queue.h"

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

  int size = 0;
  // this seems to work just fine
  Blob* blobArr = GetAllBlobsInImage(img, tol, &size);

  exit(0);
}
