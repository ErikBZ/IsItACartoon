// this src file is compiled into the executable that will
// categorize the images
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "Statistics.h"

#define MINCARTOON 0.7
#define NUMBEROFMODS 8
#define COLDEV 0
#define SIGCOLDEV 1
#define AVGSIZEOFBLOBS 2
#define SIGAVGSIZE 3
#define SIZEDEV 4
#define SIGSIZEDEV 5
#define LARGECOLDEV 6
#define PCLARGEBLOB 7
#define MODARRAYSIZE 20

double GetTenToHundred();
double GetHundredToTenThousand();
double GetThousandToHundThousand();
double CalcFitness(double* probablity, int num, Stats* s);
void printModArray(double* mods);

int main(int args, char** argv)
{
  FILE* file = fopen("output", "rb");
  int size;
  Stats* stats;
  srand(time(NULL));

  if(file != NULL)
  {
    fread(&size, sizeof(int), 1, file);
    stats = malloc(sizeof(Stats) * size);
    fread(stats, sizeof(Stats), size, file);
  }
  else
  {
    fprintf(stderr, "OUTPUT FILE MOST LIKELY MISSING. RUN CalcStats\n");
    exit(1);
  }

  double** modifications = malloc(sizeof(double*) * MODARRAYSIZE);

  int i;
  int j;

  for(i=0;i<MODARRAYSIZE;i++)
  {
    modifications[i] = malloc(sizeof(double) * NUMBEROFMODS);
    modifications[i][COLDEV] = GetTenToHundred();            // colorDeviationAverage
    modifications[i][SIGCOLDEV] = GetTenToHundred();            // sigColorDeviationAverage
    modifications[i][AVGSIZEOFBLOBS] = GetHundredToTenThousand();    // avgSizeOfBlobss
    modifications[i][SIGAVGSIZE] = GetHundredToTenThousand();    // sigAvgSizeOfBlobs
    modifications[i][SIZEDEV] = GetThousandToHundThousand();  // sizeDeviation
    modifications[i][SIGSIZEDEV] = GetThousandToHundThousand();  // sigSizeDeviation
    modifications[i][LARGECOLDEV] = GetHundredToTenThousand();    // largestColorDeviation
    modifications[i][PCLARGEBLOB] = GetTenToHundred();            // percentOfLargeBlobs
  }

  double** probability = malloc(sizeof(double*) * MODARRAYSIZE);
  for(i=0;i<MODARRAYSIZE;i++)
  {
    probability[i] = malloc(sizeof(double) * size);
    // calculating the probablity that stats[i] is a photo/cartoon
    for(j=0;j<size;j++)
    {
      Stats s = stats[j];
      probability[i][j] = 0;
      probability[i][j] += modifications[i][COLDEV] * s.colorDeviationAverage;
      probability[i][j] += modifications[i][SIGCOLDEV] * s.sigColorDeviationAverage;
      probability[i][j] += modifications[i][AVGSIZEOFBLOBS] * s.avgSizeOfBlobs;
      probability[i][j] += modifications[i][SIGAVGSIZE] * s.sigAvgSizeOfBlobs;
      probability[i][j] += modifications[i][SIZEDEV] * s.sizeDeviation;
      probability[i][j] += modifications[i][SIGSIZEDEV] * s.sigSizeDeviation;
      probability[i][j] += modifications[i][LARGECOLDEV] * s.largestColorDeviation;
      probability[i][j] += modifications[i][PCLARGEBLOB] * s.percentOfLargeBlobs;
    }
  }

  double* fitness = malloc(sizeof(double*) * MODARRAYSIZE);
  for(i=0;i<MODARRAYSIZE;i++)
  {
    printModArray(modifications[i]);
    fitness[i] = CalcFitness(probability[i], size, stats);
    printf("%f\n\n", fitness[i]);
  }

  fclose(file);
  exit(0);
}

double CalcFitness(double* probablity, int num, Stats* s)
{
  int i;
  double numCorrect = 0;
  for(i=0;i<num;i++)
  {
    if(MINCARTOON > probablity[i] && s[i].picType == 'd')
    {
      numCorrect += 1;
    }
  }
  return numCorrect/num;
}
void printModArray(double* mods)
{
  printf("Color dev mod: %f\n", mods[COLDEV]);
  printf("Sig color dev mod: %f\n", mods[SIGCOLDEV]);
  printf("Avg size of blobs: %f\n", mods[AVGSIZEOFBLOBS]);
  printf("Sig avg size of blobs: %f\n", mods[SIGAVGSIZE]);
  printf("Size deviation: %f\n", mods[SIZEDEV]);
  printf("Sig size deviation: %f\n", mods[SIGSIZEDEV]);
  printf("Largest col dev: %f\n", mods[LARGECOLDEV]);
  printf("Percent large blob: %f\n", mods[PCLARGEBLOB]);
  printf("\n");
}

double GetTenToHundred()
{
  double r = rand()%100;
  return r/1000;
}

double GetHundredToTenThousand()
{
  double r = rand()%100;
  return r/100000;
}

double GetThousandToHundThousand()
{
  double r = rand()%100;
  return r/1000000;
}
