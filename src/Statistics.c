#include "Statistics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "BlobLL.h"

double deviation(struct Image* img, Blob b)
{
  double dev = 0;
  int i;
  double sum;
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
  double sum;
  for(i=0;i<size;i++)
  {
    sum += blobs[i].size;
  }
  return sum/size;
}

double averageDeviation(struct Image* img, Blob* blobs, int size)
{
  int i;
  double sum;
  for(i=0;i<size;i++)
  {
    sum += deviation(img, blobs[i]);
  }
  return sum/size;
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

int numberOfInsignificantBlobs(Blob* blobs, int size)
{
  int i = 0;
  int sum;
  for(i=0; i<size;i++)
  {
    if(blobs[i].size == 1)
      sum++;
  }
  return sum;
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

double averageDeviationWithSig(struct Image* img, Blob* blobs, int size)
{
  int i;
  int numberOfSigBlobs = size - numberOfInsignificantBlobs(blobs, size);
  double sum;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size != 1)
    {
      sum += deviation(img, blobs[i]);
    }
  }
  return sum/numberOfSigBlobs;
}

double sizeDeviationWithSig(Blob* blobs, int size)
{
  double avg = averageSizeOfBlobsWithSig(blobs, size);
  int numberOfSigBlobs = size - numberOfInsignificantBlobs(blobs, size);
  int i;
  double sum;
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
