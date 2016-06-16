#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Image.h"

int main(int args, char** argv)
{
  if(args != 2)
  {
    fprintf(stderr, "Arguments invalid. Need name of file to open");
    exit(1);
  }
  struct Image img;
  ReadImage(argv[1], &img);
}

void ReadImage(char* name, struct Image* img)
{
  int i, j, k, pixel;
  FILE *file;
  char buffer[256];
  int nlevels;
  byte *in;
  
  printf("Reading ppm image %s\n", name);
  file = fopen(name, "rb");
  if(file == NULL)
  {
    fprintf(stderr, "Cannot read file %s. Skipping", name);
    return; 
  }
  
  if(fgets(buffer, 80, file) == NULL)
  {
    fprintf(stderr, "<EOF> character found in file. File %s maybe corrupt, skipping\n", name);
    return;
  }
  
  fprintf(stdout, "The ppm type for image %s is %s", name, buffer);
  fprintf(stdout, "The size of the first line is %zu\n", strlen(buffer));

  // checking for comments in the image
  do
  {
    if(fgets(buffer, 80, file) == NULL)
    {
      fprintf(stderr, "No size for image. Check to make sure file %s is correct", name);
      return;
    }
  }while(buffer[0] == '#'); 

  sscanf(buffer, "%d %d", &(img->NofC), &(img->NofR));
  printf("Number of Columns: %d\nNumber of Rows: %d\n", img->NofC, img->NofR);

  do
  {
    if(fgets(buffer, 80, file) == NULL)
    {
      fprintf(stderr, "<EOF> character found in file. File %s maybe corrupt, skipping\n", name);
      return;
    }
  }while(buffer[0] == '#'); 
  sscanf(buffer, "%d", &nlevels);
  printf("N levels??: %d\n", nlevels);

  // in the other image.c with jagerbomb and madcos it does not properly
  // allocate the correct size. i'll fix that here  

  img->iscolor = 1;   // I thin i'll only be dealing with type 6 ppm
  if(img->red == NULL)
    img->red = malloc(sizeof(byte) * img->NofC * img->NofR);
  if(img->green == NULL)
    img->green = malloc(sizeof(byte) * img->NofC * img->NofR);
  if(img->blue == NULL)
    img->blue = malloc(sizeof(byte) * img->NofC * img->NofR); 
  
}

