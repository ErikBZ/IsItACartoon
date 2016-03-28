/*
  Author: Erik Zapata
  ColorClamp takes in a ppm image and clamps the color of an area to the first
  pixel it found
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ColorClamp.h"
#include "image.h"

// int main(int argc, char** argv)
// {
//   if(argc != 2)
//     printf("Need 2 arguments. ColorClamp xxx.ppm\n");
//
//   struct Image* img = malloc(sizeof(struct Image));
//   struct Image* clampedImage;
//   clampedImage = malloc(sizeof(struct Image));
//   byte* tolerances = malloc(3 * sizeof(byte));
//   tolerances[0] = 37;
//   tolerances[1] = 37;
//   tolerances[2] = 37;
//   ReadImage(argv[1], img);
//
//   printf("%d\n", img->NofC);
//   printf("%d\n", img->NofR);
//   printf("%d\n", img->NofC * img->NofR);
//
//   ClampColor(img, clampedImage, tolerances);
//
//   WriteImage("out.ppm", *clampedImage);
//
//   free(clampedImage);
//   free(img);
// }

void ClampColor(struct Image* img, struct Image* clampedImage, byte* tolerances)
{
  int i = 0;
  struct Pixel** pixArray = malloc(sizeof(struct Pixel*) * 10);
  struct Pixel* pix = malloc(sizeof(struct Pixel));
  int size = 0;
  int maxSize = 10;

  pix->red = img->red[0];
  pix->green = img->green[0];
  pix->blue = img->blue[0];

  clampedImage->red = malloc(sizeof(byte) * img->NofC * img->NofR);
  clampedImage->green = malloc(sizeof(byte) * img->NofC * img->NofR);
  clampedImage->blue = malloc(sizeof(byte) * img->NofC * img->NofR);

  clampedImage->NofC = img->NofC;
  clampedImage->NofR = img->NofR;

  clampedImage->iscolor = 1;

  AddPixelPointerToArray(pixArray, pix, &size, &maxSize);

  for(i=0; i<img->NofC*img->NofR; i++)
  {
    pix = GetPixelFromArray(pixArray, img, i, size, tolerances);
    if(pix == 0)
    {
      free(pix);
      pix = GetNewPixel(img->red[i], img->green[i], img->blue[i]);
      AddPixelPointerToArray(pixArray, pix, &size, &maxSize);
    }

    clampedImage->red[i] = pix->red;
    clampedImage->green[i] = pix->green;
    clampedImage->blue[i] = pix->blue;

  }
}

struct Pixel* GetNewPixel(byte red, byte green, byte blue)
{
  struct Pixel* pix = malloc(sizeof(struct Pixel));

  pix->red = red;
  pix->green = green;
  pix->blue = blue;
  pix->num = 0;

  return pix;
}

struct Pixel* GetPixelFromArray(struct Pixel** pixArray, struct Image* img,
    int x, int size, byte* tolerances)
{
  int i = 0;
  for(i=0; i<size;i++)
  {
    if(abs(pixArray[i]->red - img->red[x]) <= tolerances[0] &&
       abs(pixArray[i]->green - img->green[x]) <= tolerances[1] &&
       abs(pixArray[i]->blue - img->blue[x]) <= tolerances[2])
      {
        return pixArray[i];
      }
  }

  return 0;
}

void AddPixelPointerToArray(struct Pixel** pixelArray, struct Pixel* pix, int* size, int* maxSize)
{
  if(size == maxSize)
  {
    *maxSize = (*maxSize) * 2;
    struct Pixel** newPixelArray = malloc(*maxSize * sizeof(struct Pixel*));
    memcpy(newPixelArray, pixelArray, (*size)-1 * sizeof(struct Pixel*));
    free(pixelArray);
    pixelArray = newPixelArray;
  }
  pixelArray[(*size)] = pix;
  (*size) = (*size) + 1;
}
