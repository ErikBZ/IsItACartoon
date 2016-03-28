// used to protect against multiple includes

#ifndef COLORCLAMPH_
#define COLORCLAMPH_

#include <stdlib.h>
#include <stdio.h>
#include "image.h"

struct Pixel
{
  byte red;
  byte green;
  byte blue;

  int num;
};

void ClampColor(struct Image* img, struct Image* clampedImage, byte* tolerances);
struct Pixel* GetNewPixel(byte red, byte green, byte blue);
struct Pixel* GetPixelFromArray(struct Pixel** pixArray, struct Image* img,
                                int x, int size, byte* tolerances);
void AddPixelPointerToArray(struct Pixel** pixelArray, struct Pixel* pix, int* size,
                                     int* maxSize);

// keep this at the bottom
#endif
