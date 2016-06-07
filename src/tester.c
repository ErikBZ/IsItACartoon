/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "image.h"
#include "BlobLL.h"
#include "Queue.h"
#include "Statistics.h"

#define DATADIRSIZE 7

char* createFilename(char* d, char* f)
{
  char *result = malloc(strlen(d) + strlen(f) + 1);
  strcpy(result, d);
  strcat(result, f);
  return result;
}

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf("Invalid input. Exiting\n");
    exit(0);
  }

  char** filenames = malloc(sizeof(char*) * 4);

  DIR *d;
  struct dirent *dir;
  int index = 0;
  d = opendir("./data");
  if(d)
  {
    while((dir = readdir(d)) != NULL)
    {
      if(dir->d_name[0] != '.')
      {
        filenames[index] = createFilename(argv[1], dir->d_name);
        printf("%s\n", filenames[index]);
        index++;
      }
    }
  }


  // reading in the ppm file into img
  struct Image* img = malloc(sizeof(struct Image));
  ReadImage(filenames[0], img);

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

  Stats stats;
  stats.colorDeviationAverage = averageDeviation(img, blobArr, size);
  stats.sigColorDeviationAverage = averageDeviationWithSig(img, blobArr, size);
  stats.largestColorDeviation = findLargestColorDeviation(img, blobArr, size);
  stats.avgSizeOfBlobs = averageSizeOfBlobs(blobArr, size);
  stats.sigAvgSizeOfBlobs = averageSizeOfBlobsWithSig(blobArr, size);
  stats.sizeDeviation = sizeDeviation(blobArr, size);
  stats.sigSizeDeviation = sizeDeviationWithSig(blobArr, size);
  stats.percentOfLargeBlobs = percentTakenByLargeBlobs(blobArr, size, img->NofC * img->NofR);
  stats.largestBlob = findLargestBlob(blobArr, size);
  stats.insignBlobs = numberOfInsignificantBlobs(blobArr, size);
  stats.numOfBlobs = numberOfBlobs(blobArr, size);

  printStats(stats);

  FILE* file = fopen("output", "wb");
  if(file != NULL)
  {
    fwrite(&stats, sizeof(Stats), 1, file);
    fclose(file);
  }

  Stats st;
  FILE* file2 = fopen("output", "rb");
  if(file != NULL)
  {
    fread(&st, sizeof(Stats), 1, file2);
    fclose(file2);
  }

  printStats(st);

  closedir(d);
  exit(0);
}
