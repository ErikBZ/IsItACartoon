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
// creates a child mod array that is roughly half of one parent, half the other
double* CreateChild(double* parent1, double* parent2);
// creates a child that is roughly half 1 parent and half random
double* CreateBastard(double* parent1);
// creates a new generation;
double** CreateNewGeneration(double** currentGen, double* fitness);

void CalculateProbability(double** probability, double** modifications,
                          Stats* stats, int statSize);
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

  // the first generation
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
  }

  // calculating the probablity that stats[i] is a photo/cartoon
  CalculateProbability(probability, modifications, stats, size);

  double* fitness = malloc(sizeof(double*) * MODARRAYSIZE);
  for(i=0;i<MODARRAYSIZE;i++)
  {
    printModArray(modifications[i]);
  }

  // throwing out the trash
  for(i=0;i<MODARRAYSIZE;i++)
  {
    free(modifications[i]);
  }
  free(modifications);
  for(i=0;i<MODARRAYSIZE;i++)
  {
    free(probability[i]);
  }
  free(stats);
  free(fitness);
  free(probability);
  fclose(file);
  exit(0);
}

void CalculateProbability(double** probability, double** modifications,
                          Stats* stats, int statSize)
{
  int i,j;
  for(i=0;i<MODARRAYSIZE;i++)
  {
    for(j=0;j<statSize;j++)
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
}

double** CreateNewGeneration(double** currentGen, double* fitness)
{
  double* firstBest = malloc(sizeof(double) * NUMBEROFMODS);
  double* secondBest = malloc(sizeof(double)* NUMBEROFMODS);
  double* child;
  double largestFitness = 0;
  int i;

  for(i=0;i<MODARRAYSIZE;i++)
  {
    if(fitness[i] > largestFitness)
    {
      memcpy(secondBest, firstBest, sizeof(double) * NUMBEROFMODS);
      memcpy(firstBest, currentGen[i], sizeof(double) * NUMBEROFMODS);
      largestFitness = fitness[i];
    }
  }
  // first best and second best are now set
  // the new generation will have the first 7 be children (2 parents)
  // the second 7 be bastards (1 parent)
  // and the last 6 be immigrants(completely random)
  for(i=0;i<MODARRAYSIZE;i++)
  {
    if(i<7)
    {
      child = CreateChild(firstBest, secondBest);
      memcpy(currentGen[i], child, sizeof(double) * NUMBEROFMODS);
      free(child);
      child = NULL;
    }
    else if(i<14)
    {
      child = CreateBastard(firstBest);
      memcpy(currentGen[i], child, sizeof(double) * NUMBEROFMODS);
      free(child);
      child = NULL;
    }
  }
}

double* CreateChild(double* parent1, double* parent2)
{
  int i=0;
  int passdown = rand()%2;
  double* child = malloc(sizeof(double) * NUMBEROFMODS);
  for(i=0;i<NUMBEROFMODS;i++)
  {
    if(passdown)
    {
      child[i] = parent1[i];
    }
    else
    {
      child[i] = parent2[i];
    }
  }
  return child;
}

double* CreateBastard(double* parent1)
{
  int i=0;
  int passdown = rand()%2;
  double* child = malloc(sizeof(double) * NUMBEROFMODS);
  for(i=0;i<NUMBEROFMODS;i++)
  {
    if(passdown)
      child[i] = parent1[i];
    // equal to coldev, SIGCOLDEV, PCLARGEBLOB
    else if(i == COLDEV || i==SIGCOLDEV || i==PCLARGEBLOB)
      child[i] = GetTenToHundred();
    else if(i==AVGSIZEOFBLOBS || i== SIGAVGSIZE || i==LARGECOLDEV)
      child[i] = GetHundredToTenThousand();
    else
      child[i] = GetThousandToHundThousand();
  }
  return child;
}

double CalcFitness(double* probablity, int num, Stats* s)
{
  int i;
  double numCorrect = 0;
  for(i=0;i<num;i++)
  {
    if(MINCARTOON > probablity[i] && s[i].picType == 'p')
    {
      numCorrect += 1;
    }
    else if(MINCARTOON < probablity[i] && s[i].picType == 'd')
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
