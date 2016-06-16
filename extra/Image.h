#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>

#define byte unsigned char
#define MINBUFFERSIZE 4096

struct Image
{
  byte isColor;
  byte *gray, *red, *green, *blue;
  int NofR, NofC; // number of rows, number of columns
};

void ReadImage(char* name, struct Image *img);
void WriteImage(char *name, struct Image Img);
#endif
