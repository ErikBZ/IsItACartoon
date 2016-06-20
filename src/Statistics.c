#include "Statistics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "image.h"
#include "BlobLL.h"

#define NOTVISISTED 0
#define PICTURETYPE 7

char* createFilename(char* d, char* f)
{
  char *result = malloc(strlen(d) + strlen(f) + 1);
  strcpy(result, d);
  strcat(result, f);
  return result;
}

Stats* getAllStats(char** files, int size, double tol)
{
  int i;
  int j;
  byte* visited;

  struct Image* img;
  Stats* stats = malloc(sizeof(Stats) * size);

  for(i=0;i<size;i++)
  {
    // reading in the next image
    img = malloc(sizeof(struct Image));
    img->gray = NULL;
    img->red = NULL;
    img->green = NULL;
    img->blue = NULL;
    
    ReadImage(files[i], img);
    // reallocing the visited array so that it matches the size
    // of the current image, then reiniting it
    visited = malloc(sizeof(byte) * img->NofC * img->NofR);
    for(j=0;j<size;j++)
    {
      visited[j] = NOTVISISTED;
    }

    int blobSize = 0;
    Blob* blobs = GetAllBlobsInImage(img, tol, &blobSize);
    Stats s = findStatsOfAnImage(img, blobs, blobSize, files[i]);
    stats[i] = s;

    printf("Image statistics calculated sucessfully\n");

    free(visited);
    free(img);
    visited = NULL;
    img = NULL;
  }
  return stats;
}

byte isPPMImage(char* file)
{
  byte size = strlen(file);
  if(file[size-4] == '.' && file[size-3] == 'p' && file[size-2] == 'p'
     && file[size-1] == 'm')
      return 1;
  return 0;
}

double deviation(struct Image* img, Blob b)
{
  double dev = 0;
  int i;
  double sum = 0;
  for(i=0;i<b.size;i++)
  {
    double rad = geometricDistanceWithNoArrays(b.color, img->red[b.indeces[i]],
                  img->green[b.indeces[i]], img->blue[b.indeces[i]]);
    sum += (rad - b.radAvg) * (rad - b.radAvg);
  }

  sum = sum/b.size;

  return sqrt(sum);
}

double averageSizeOfBlobs(Blob* blobs, int size)
{
  int i=0;
  double sum = 0;
  for(i=0;i<size;i++)
  {
    sum += blobs[i].size;
  }
  return sum/size;
}

// average devaiton for color
double averageDeviation(struct Image* img, Blob* blobs, int size)
{
  int i;
  double sum = 0;
  for(i=0;i<size;i++)
  {
    sum += deviation(img, blobs[i]);
  }
  return sum/size;
}

// largest deviation for color
double findLargestColorDeviation(struct Image* img, Blob* blobs, int size)
{
  int i;
  double large = 0;
  double dev = 0;
  for(i=0;i<size;i++)
  {
    dev = deviation(img, blobs[i]);
    if(dev > large)
    {
      large = dev;
    }
  }
  return large;
}

double sizeDeviation(Blob* blobs, int size)
{
  int i;
  double avg = averageSizeOfBlobs(blobs, size);
  double sum = 0;
  for(i=0;i<size;i++)
  {
    sum += (blobs[i].size - avg) * (blobs[i].size - avg);
  }
  sum = sum/size;
  return sqrt(sum);
}

int findLargestBlob(Blob* blobs, int size)
{
  int large = 0;
  int i;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size > large)
    {
      large = blobs[i].size;
    }
  }
  return large;
}

int numberOfInsignificantBlobs(Blob* blobs, int size)
{
  int i = 0;
  int sum = 0;
  for(i=0; i<size;i++)
  {
    if(blobs[i].size == 1)
      sum++;
  }
  return sum;
}

int numberOfBlobs(Blob* blobs, int size)
{
  return size;
}

// the "significant" calculations can be found here
double averageSizeOfBlobsWithSig(Blob* blobs, int size)
{
  int i;
  int numberOfSigBlobs = size - numberOfInsignificantBlobs(blobs, size);
  double sum;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size != 1)
      sum += blobs[i].size;
  }
  return sum/numberOfSigBlobs;
}

// for color
double averageDeviationWithSig(struct Image* img, Blob* blobs, int size)
{
  int i;
  int numberOfSigBlobs = size - numberOfInsignificantBlobs(blobs, size);
  double sum = 0;
  double large = 0;
  double dev = 0;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size != 1)
    {
      dev = deviation(img, blobs[i]);
      sum += dev;
    }
  }
  return sum/numberOfSigBlobs;
}

double sizeDeviationWithSig(Blob* blobs, int size)
{
  double avg = averageSizeOfBlobsWithSig(blobs, size);
  int numberOfSigBlobs = size - numberOfInsignificantBlobs(blobs, size);
  int i;
  double sum = 0;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size != 1)
    {
      sum += (blobs[i].size - avg) * (blobs[i].size - avg) ;
    }
  }
  sum = sum/size;
  return sqrt(sum);
}

double percentTakenByLargeBlobs(Blob* blobs, int size, double imgSize)
{
  int i;
  double percent = 0;
  double sum = 0;
  double large = imgSize * 0.01;
  for(i=0; i<size;i++)
  {
    if(blobs[i].size > large)
    {
      sum += blobs[i].size;
    }
  }
  return sum/imgSize;
}

Stats findStatsOfAnImage(struct Image* img, Blob* blobs, int size, char* t)
{
  Stats stats;
  memset(stats.name, 0, 30);
  strcpy(stats.name, t);
  stats.picType = t[PICTURETYPE];
  stats.colorDeviationAverage = averageDeviation(img, blobs, size);
  stats.sigColorDeviationAverage = averageDeviationWithSig(img, blobs, size);
  stats.largestColorDeviation = findLargestColorDeviation(img, blobs, size);
  stats.avgSizeOfBlobs = averageSizeOfBlobs(blobs, size);
  stats.sigAvgSizeOfBlobs = averageSizeOfBlobsWithSig(blobs, size);
  stats.sizeDeviation = sizeDeviation(blobs, size);
  stats.sigSizeDeviation = sizeDeviationWithSig(blobs, size);
  stats.percentOfLargeBlobs = percentTakenByLargeBlobs(blobs, size, img->NofC * img->NofR);
  stats.largestBlob = findLargestBlob(blobs, size);
  stats.insignBlobs = numberOfInsignificantBlobs(blobs, size);
  stats.numOfBlobs = numberOfBlobs(blobs, size);
  return stats;
}

void printStats(Stats s)
{
  printf("%s\n", s.name);
  printf("Picture Type: %c\n", s.picType);
  printf("Color Deviation Average: %lf\n", s.colorDeviationAverage);
  printf("Significant Color Deviation Average: %lf\n", s.sigColorDeviationAverage);
  printf("Largest Color Deviation: %lf\n", s.largestColorDeviation);
  printf("Average Size of Blobs: %lf\n", s.avgSizeOfBlobs);
  printf("Significant Average Size of Blobs: %lf\n", s.sigAvgSizeOfBlobs);
  printf("Size Deviation: %lf\n", s.sizeDeviation);
  printf("Significant Size Deviation: %lf\n", s.sigSizeDeviation);
  printf("Percent Taken By Large Blobs: %lf\n", s.percentOfLargeBlobs);
  printf("Largest Blob: %d\n", s.largestBlob);
  printf("Number of Insignificat Blobs: %d\n", s.insignBlobs);
  printf("Number of Blobs: %d\n\n", s.numOfBlobs);
}
