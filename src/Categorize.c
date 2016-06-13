// this src file is compiled into the executable that will
// categorize the images
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Statistics.h"

int main(int args, char** argv)
{
  FILE* file = fopen("output", "rb");
  int size;
  Stats* stats;

  if(file != NULL)
  {
    fread(&size, sizeof(int), 1, file);
    printf("%d\n", size);
    stats = malloc(sizeof(Stats) * size);
    fread(stats, sizeof(Stats), size, file);
  }

  double colDevMod = 0.01;
  double sigColDevMod = 0.01;
  double avgSizeMod = 0.001;
  double sigAvgSizeMod = 0.001;
  double sizeDevMod = 0.0001;
  double sigSizeDevMod = 0.0001;
  double largeColDevMod = 0.001;
  double percentSizeMod = 0.01;

  double* dub = malloc(sizeof(double) * size);
  int i;
  for(i=0;i<size;i++)
  {
    Stats s = stats[i];
    dub[i] = 0;
    dub[i] += colDevMod * s.colorDeviationAverage;
    dub[i] += sigColDevMod * s.sigColorDeviationAverage;
    dub[i] += avgSizeMod * s.avgSizeOfBlobs;
    dub[i] += sigAvgSizeMod * s.sigAvgSizeOfBlobs;
    dub[i] += sizeDevMod * s.sizeDeviation;
    dub[i] += sigSizeDevMod * s.sigSizeDeviation;
    dub[i] += largeColDevMod * s.largestColorDeviation;
    dub[i] += percentSizeMod * s.percentOfLargeBlobs;
  }

  for(i=0;i<size;i++)
  {
    printStats(stats[i]);
    printf("%f\n", dub[i]);
  }

  fclose(file);
  exit(0);
}
