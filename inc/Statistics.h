#ifndef STATS_H
#define STATS_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "BlobLL.h"
#include "image.h"

typedef struct Stats
{
  // actually the deviation of the deviations
  // may have to change it to variance, idk i'm not a statistician
  // should have paid attention in class more woops
  double colorDeviationAverage;
  double sigColorDeviationAverage;
  double avgSizeOfBlobs;
  double sigAvgSizeOfBlobs;
  // deviation of average sizes
  double sizeDeviation;
  double sigSizeDeviation;
  double largestColorDeviation;
  double percentOfLargeBlobs;
  int largestBlob;
  int insignBlobs;
  int numOfBlobs;
} Stats;

// gets the deviation of a blobs color
double deviation(struct Image* img, Blob b);
// gets the average size of the blobs
double averageSizeOfBlobs(Blob* blobs, int size);
// gets the average std Devation of the whole image
double averageDeviation(struct Image* img, Blob* blobs, int size);
// gets the largest color deviation
double findLargestColorDeviation(struct Image* img, Blob* blobs, int size);

int findLargestBlob(Blob* blobs, int size);
int numberOfInsignificantBlobs(Blob* blobs, int size);
// gets the number of insignificant blobs IE blobs that are 1 pixel big
int numberOfBlobs(Blob* blobs, int size);
// deviation of the average sizes
double sizeDeviation(Blob* blobs, int size);

// these functions calculate stats without taking into consideration
// the 1 pixel blobs
double averageSizeOfBlobsWithSig(Blob* blobs, int size);
double averageDeviationWithSig(struct Image* img, Blob* blobs, int size);
double sizeDeviationWithSig(Blob* blobs, int size);
double percentTakenByLargeBlobs(Blob* blobs, int size, double imgSize);

Stats findStatsOfAnImage(struct Image* img, Blob* blobs, int size);

// using this for printing info
void printStats(Stats s);
#endif
