#include "Statistics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "Image.h"
#include "BlobLL.h"

#define NOTVISISTED 0
#define PICTURETYPE 7

// I think I'll create a method that does everying

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
    Stats s = findStatsOfAnImage_Version2(img, blobs, blobSize, files[i]);
    stats[i] = s;

    printf("Image statistics calculated sucessfully\n");

    free(visited);
    FreeImage(img);
    free(img);
    FreeBlobs(blobs, blobSize);
    free(blobs);

    visited = NULL;
    img = NULL;
    blobs = NULL;
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

// gets the color deviation in a single blob of an image
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
  int i=1;
  double sum = 0;
  for(i=0;i<size;i++)
  {
    if(blobs[i].size != 1)
    {
      // valgrind error. some indeces in the blobs array aren't initialized
      // but that's fine, since they won't be used in the calculations since all
      // uninitialized blobs are at indeces > size
      sum += (blobs[i].size - avg) * (blobs[i].size - avg);
    }
  }
  sum = sum/size;
  double ans = sqrt(sum);
  return ans;
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

// I'm retarded. Why am was I dividing color by size?
Stats findStatsOfAnImage(struct Image* img, Blob* blobs, int size, char* t)
{
  Stats stats;
  memset(stats.name, 0, 30);
  strcpy(stats.name, t);
  stats.picType = t[PICTURETYPE];
  stats.colorDeviationAverage = averageDeviation(img, blobs, size);
  stats.sigColorDeviationAverage = averageDeviationWithSig(img, blobs, size);
  stats.largestColorDeviation = findLargestColorDeviation(img, blobs, size);
  stats.avgSizeOfBlobs = averageSizeOfBlobs(blobs, size)/size;
  stats.sigAvgSizeOfBlobs = averageSizeOfBlobsWithSig(blobs, size)/size;
  stats.sizeDeviation = sizeDeviation(blobs, size)/size;
  stats.sigSizeDeviation = sizeDeviationWithSig(blobs, size)/size;
  stats.percentOfLargeBlobs = percentTakenByLargeBlobs(blobs, size, img->NofC * img->NofR);
  // there seems to be an error here. This should theoritcally always be less than 1
  // but some pics of have larger than 1 blobs. I'll have to check this out
  stats.largestBlob = (double)findLargestBlob(blobs, size)/size;
  stats.insignBlobs = numberOfInsignificantBlobs(blobs, size);
  stats.numOfBlobs = numberOfBlobs(blobs, size);
  return stats;
}

/*
  A faster version of find stats of an image. This should replace it once it's complete
	this method is cancer
*/
Stats findStatsOfAnImage_Version2(struct Image* img, Blob* blobs,
  int bArraySize, char* name)
{
  Stats s;
  memset(s.name, 0, 30);
  strcpy(s.name, name);
  s.picType = name[PICTURETYPE];
  int res1, res2;
  res1 = getImageResolution(img);
  s.resolution = res1;

  int i=0;
  double sum = 0;
	int numOfBlobsCalculated = 0;
  double largeSum = 0;
	int numOfLargeBlobsCalculated = 0;
  double large = 0;  
  double dev = 0;
	int sumOfSize = 0;
	int sigSumOfSize = 0;
	int sumOfLargeBlobs = 0;
	int largestBlobTracker = 0;
	int insignBlobsTracker = 0;

	// packing a bunch of shit in this one for loop
	// I need one more after this and all the stats will be done
	// until i find out that i did this completely incorrectly
  for(i=0;i<bArraySize;i++)
  {
		sumOfSize += blobs[i].size;
    dev = deviation(img, blobs[i]);
    // summing up all deviations that are not of size 1
		if(blobs[i].size > 1)
		{
    	sum += dev;
			numOfBlobsCalculated++;
			sigSumOfSize += blobs[i].size;
		}
		else
		{
			insignBlobsTracker++;
		}
		// gets the sum of the deviations that were not calculated
		// with small blobs
		if(blobs[i].size > 10)
		{
			largeSum += dev;
			numOfLargeBlobsCalculated++;
		}
		if(dev > large)
		{
			large = dev;
		}
		if(blobs[i].size > (double)s.resolution * 0.01)
		{
			sumOfLargeBlobs += blobs[i].size;
		}
		if(blobs[i].size > largestBlobTracker)
		{
				largestBlobTracker = blobs[i].size;
		}
  }
	s.colorDeviationAverage = sum/numOfBlobsCalculated;
	s.sigColorDeviationAverage = largeSum/numOfLargeBlobsCalculated;
	s.largestColorDeviation = large;
  s.avgSizeOfBlobs = ((double)sumOfSize/bArraySize)/s.resolution;
  s.sigAvgSizeOfBlobs = ((double)sigSumOfSize/numOfBlobsCalculated)/s.resolution;
  s.percentOfLargeBlobs = (double)sumOfLargeBlobs/s.resolution;
  s.largestBlob = largestBlobTracker/s.resolution;
  s.numOfBlobs = bArraySize;
  s.insignBlobs = insignBlobsTracker;

	double avg = s.avgSizeOfBlobs;
	double newSum = 0;
	double sigSum = 0;
	int sigTracker = 0;
  for(i=0;i<bArraySize;i++)
  {
    newSum += (blobs[i].size - avg) * (blobs[i].size - avg);
		if(blobs[i].size > 1)
		{
			sigTracker++;
			sigSum += (blobs[i].size - avg) * (blobs[i].size - avg);
		}
  }
	newSum = newSum/bArraySize;
	sigSum = sigSum/sigTracker;
  s.sizeDeviation = sqrt(newSum)/s.resolution;
  s.sigSizeDeviation = sqrt(sigSum)/s.resolution;
  
  return s;
}

int getResolution(Blob* blobs, int size)
{
  int i, sum;
  sum = 0;
  for(i=0;i<size;i++)
  {
    sum += blobs[i].size;
  }
  return sum;
}

int getImageResolution(struct Image* img)
{
  return img->NofR * img->NofC;
}

void printStats(Stats s)
{
  printf("%s\n", s.name);
  printf("Picture Type: %c\n", s.picType);
  printf("Resolution: %d\n", s.resolution);
  printf("Color Deviation Average: %e\n", s.colorDeviationAverage);
  printf("Significant Color Deviation Average: %e\n", s.sigColorDeviationAverage);
  printf("Largest Color Deviation: %e\n", s.largestColorDeviation);
  printf("Average Size of Blobs: %e\n", s.avgSizeOfBlobs);
  printf("Significant Average Size of Blobs: %e\n", s.sigAvgSizeOfBlobs);
  printf("Size Deviation: %e\n", s.sizeDeviation);
  printf("Significant Size Deviation: %e\n", s.sigSizeDeviation);
  printf("Percent Taken By Large Blobs: %e\n", s.percentOfLargeBlobs);
  printf("Largest Blob: %e\n", s.largestBlob);
  printf("Number of Insignificat Blobs: %d\n", s.insignBlobs);
  printf("Number of Blobs: %d\n\n", s.numOfBlobs);
}
