// image.h, Author: Zoran Duric, 10-7-2010

#ifndef COLORIMAGE_H
#define COLORIMAGE_H

#include <stdio.h>
#include <stdlib.h>

#define byte unsigned char
#define MIN_BUFFER_SIZE 4096

#define GetColor    (img.iscolor)
#define GetGray(i)  (img.gray[i])
#define GetRed(i)   (img.red[i])
#define GetGreen(i) (img.green[i])
#define GetBlue(i)  (img.blue[i])
#define SetGray(i,g)  (img.gray[i] = g)
#define SetRed(i,r)   (img.red[i] = r)
#define SetGreen(i,g) (img.green[i] = g)
#define SetBlue(i,b)  (img.blue[i] = b)
#define GetByte(i)      (b.data[i])
#define SetByte(i,val)  (b.data[i] = val)

struct Image
{
  byte iscolor;  // color = 1 for a color image
  byte *gray, *red, *green, *blue;
  int NofR, NofC;
};

struct Buffer
{
  byte *data; // data pointer
  int size, max_size;   // true and available size
};

void ReadImage(char *name, struct Image *Img);
void WriteImage(char *name, struct Image Img);
void PGMtoPPM(struct Image *Img);
void ReadBinaryFile(char *name, struct Buffer *b);
void WriteBinaryFile(char *name, struct Buffer b);

#endif
