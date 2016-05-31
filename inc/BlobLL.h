// used to protect against multiple includes
#ifndef BLOBLLH_
#define BLOBLLH_

#include <stdlib.h>
#include <stdio.h>
#include "image.h"

typedef struct Blob
{
  int* indeces;
  int size;
  int max;
  byte* color;
} Blob;

// if this works i don't need anything above
int GetPixelUp(int i, int rows, int cols);
int GetPixelDown(int i, int rows, int cols);
int GetPixelLeft(int i, int rows, int cols);
int GetPixelRight(int i, int rows, int cols);
double geometricDistance(byte* arr1, byte* arr2);
double geometricDistanceWithNoArrays(byte* arr1, byte red, byte green, byte blue);
Blob CalculateBlob(struct Image* img, double tol, byte* visitedArray, int start);
void AddDataToArray(Blob* blob, int data);
void AverageColors(Blob* b, byte red, byte green, byte blue);
Blob* AddBlobToArray(Blob* blob, Blob b, int* sizeP, int* maxP);
Blob* GetAllBlobsInImage(struct Image* img, double tol, int* size);

// keep this at the bottom
#endif
