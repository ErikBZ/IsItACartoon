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
  struct Image* img = malloc(sizeof(struct Image));
  char* filename = argv[1];
  ReadImage(filename, img);
  BlobPool pool;
  pool.blobPool = malloc(sizeof(BlobLL)*10);
  pool.size = 0;
  pool.maxSize = 10;

}
