// before BlobLL and this were gonna be the same but i'm
// splitting them up so that its easier to manage *budum tsss*
#ifndef BLOBMANAGER_H
#define  BLOBMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "string.h"
#include "ColorClamp.h"
#include "BlobLL.h"
#include "image.h"
#include "HeadBlobLL.h"

byte IsSimilarColor(LineBlob* lb1, LineBlob* lb2, double tol);
// things for building the Blob
double threeVarRadius(byte* arr1, byte* arr2);
void AddLineBlobToArray(struct LineBlob** lbArray, struct LineBlob* lb,
                        int* size, int* maxSize);
// now that i have the BlobPool struct i can change this to something
// different. I can chage blobPool, size, and maxsize into one struct
// and be done with it. I'll need to do some testing to make sure it'll add up
// properly though
void AddBlobLLToListPool(BlobPool* blobPool, BlobLL n);
// main function that finds the blobs in image
// time to test it out. RIP
void FindBlobsInImage(BlobPool* pool, struct Image* img, double tol);
// adds all the nodes that contain data similar to the given LineBlob
int CheckAbove(HeadLL* rowLL, Node* lbNode, double tol);
byte IsAdjacent(LineBlob* lbChecking, LineBlob* curr);
HeadLL* calculateRow(struct Image* img, int row, HeadLL* headLinkedList,
                     BlobPool* blobPool, double tol);

#endif
