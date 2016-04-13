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

byte IsSimilarColor(LineBlob* lb1, LineBlob* lb2, double tol);
// things for building the Blob
double threeVarRadius(byte* arr1, byte* arr2);
void AddLineBlobToArray(struct LineBlob** lbArray, struct LineBlob* lb,
                        int* size, int* maxSize);
void AddBlobLLToListPool(BlobLL** blobPool, BlobLL n, int* size, int* maxSize);
// adds all the nodes that contain data similar to the given LineBlob
int CheckAbove(BlobLL* rowLL, Node* lbNode, double tol);
byte IsAdjacent(LineBlob* lbChecking, LineBlob* curr);

#endif
