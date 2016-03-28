/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 * image.c: A program for manipulating images                           *
 * Author: Zoran Duric, 10-7-2010.                                      *
 *++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "image.h"

void PGMtoPPM(struct Image *Img)
{
  int i;
  if (Img->iscolor) return;

  Img->iscolor = 1;
  if (Img->red == NULL) Img->red = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
  if (Img->green == NULL) Img->green = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
  if (Img->blue == NULL) Img->blue = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);

  for (i=0; i<Img->NofR*Img->NofC; i++)
    {
      Img->red[i] = Img->gray[i];
      Img->green[i] = Img->gray[i]; 
      Img->blue[i] = Img->gray[i];
    }
  free(Img->gray);
}

// Read PPM & PGM files (P2, P3, P5, P6)
void ReadImage(char *name, struct Image *Img)
{
  int i,j,pixel,k;
  FILE *fp;
  char s[256];
  int filetype, nlevels;
  byte *in;

  printf("\nReading input file '%s'\n",name);
  if ((fp = fopen(name, "rb")) == NULL) {
    fprintf(stderr,"Can't open input file %s\n",name);
    exit(1);
  } 
  label1 : 
    if (fgets(s, 80, fp)==NULL)
      {
	fprintf(stderr,"<EOF> character found in input file %s\n", name);
	exit(1);
      }
    
  if (strcmp("P2\n",s)==0) filetype = 2;
  else if (strcmp("P3\n",s)==0) filetype = 3;
  else if (strcmp("P5\n",s)==0) filetype = 5;
  else if (strcmp("P6\n",s)==0) filetype = 6;
  else {
    if (s[0] == '#') goto label1;
    fprintf(stderr,"Input file %s is of wrong type %s\n",name,s);
    exit(1);
  }

 label2:
  if (fgets(s, 80, fp)==NULL) {
    fprintf(stderr,"Size line does not exist in input file %s\n", name);
    exit(1);
  }
  if (s[0] == '#') goto label2;
  sscanf(s,"%d %d",&(Img->NofC),&(Img->NofR));

 label3:
  if (fgets(s, 80, fp)==NULL) {
    fprintf(stderr,"<EOF> character found in input file %s\n", name);
    exit(1);
  }
  if (s[0] == '#') goto label3;
  sscanf(s,"%d",&nlevels);

  switch (filetype) { // read pixels
  case 2:  // P2 - ascii file, gray level image
    Img->iscolor = 0;
    if (Img->gray == NULL) Img->gray = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    k = 0;
    for (i=0; i<Img->NofR; i++) for (j=0; j<Img->NofC; j++) {
      fscanf(fp,"%d",&pixel);
      Img->gray[k++] = pixel;
    }
    break;
  case 3:  // P3 - ascii file, color image
    Img->iscolor = 1;
    if (Img->red == NULL) Img->red = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    if (Img->green == NULL) Img->green = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    if (Img->blue == NULL) Img->blue = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    k = 0;
    for (i=0; i<Img->NofR; i++) for (j=0; j<Img->NofC; j++) {
      fscanf(fp,"%d",&pixel);      Img->red[k] = pixel;
      fscanf(fp,"%d",&pixel);      Img->green[k] = pixel;
      fscanf(fp,"%d",&pixel);      Img->blue[k++] = pixel;
    }
    break;
  case 5:  // P5 - raw file, gray level image
    Img->iscolor = 0;
    if (Img->gray == NULL) Img->gray = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    k = 0;
    in = (byte *)malloc(sizeof(byte)*Img->NofC);
    for (i=0; i<Img->NofR; i++) {
      j = fread((void *)in,sizeof(byte),Img->NofC,fp);
      if (j < Img->NofC) 
       {  fprintf(stderr,
              "Error: input file %s contains only %1d objects (< %1d)\n",
               name,i*Img->NofC+j,Img->NofC*Img->NofR);
          exit(1);
       }
      for (j=0; j<Img->NofC; j++) Img->gray[k++] = in[j];
    }
    free(in);
    break;
  case 6:   // P6 - raw file, color image
    Img->iscolor = 1;
    if (Img->red == NULL) Img->red = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    if (Img->green == NULL) Img->green = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    if (Img->blue == NULL) Img->blue = (byte *)malloc(sizeof(byte)*Img->NofC*Img->NofR);
    k = 0;
    in = (byte *)malloc(sizeof(byte)*Img->NofC*3);;
    for (i=0; i<Img->NofR; i++) {
      j = fread((void *)in,sizeof(byte),Img->NofC*3,fp);
      if (j < (Img->NofC*3)) 
       {  fprintf(stderr,
              "Error: input file %s contains only %1d objects (< %1d)\n",
               name,i*Img->NofC+(j+1)/3,Img->NofC*Img->NofR);
          exit(1);
       }
      for (j=0; j<Img->NofC; j++) {
	Img->red[k] = in[3*j];
	Img->green[k] = in[3*j+1];
	Img->blue[k++] = in[3*j+2];
      }
    }
    free(in);
    break;
  otherwise: break;
  }

  fclose(fp);
}

// Read binary file
void ReadBinaryFile(char *name, struct Buffer *b)
{
  int i,j,k;
  FILE *fp;
  byte *in, *aux;

  printf("\nReading binary file '%s'\n",name);
  if ((fp = fopen(name, "rb")) == NULL) {
    fprintf(stderr,"Can't open input file %s\n",name);
    exit(1);
  } 

  // make space for data
  b->data = (byte *)malloc(sizeof(byte)*MIN_BUFFER_SIZE);
  b->size = 0;
  b->max_size = MIN_BUFFER_SIZE;

  in = (byte *)malloc(sizeof(byte)*MIN_BUFFER_SIZE);
  while ((j = fread((void *)in,sizeof(byte),MIN_BUFFER_SIZE,fp)) > 0)
    {
      if ((j + b->size) > b->max_size)
	{
	  aux = (byte *)malloc(sizeof(byte)*b->max_size*2);
	  b->max_size *=2;
	  for (i=0; i<b->size; i++) aux[i] = b->data[i];
	  free(b->data);
	  b->data = aux;
	}
      for (i=0; i<j; i++) b->data[b->size+i] = in[i];
      b->size += j;
      if (j < MIN_BUFFER_SIZE) break;
    }
  free(in);
  fclose(fp);
}

// Write gray or color images
void WriteImage(char *file, struct Image Img)
{
  int i, j, k;
  FILE *fo;
  byte *out;

  printf("\nWriting file '%s'\n",file);

  if ((fo = fopen(file, "wb")) == NULL) {
    fprintf(stderr,"Can't create output file \'%s\'\n",file);
    exit(1);
  } 
  if (!Img.iscolor) {  // write P5 - raw, gray image
    out =  (byte *)malloc(sizeof(byte)*Img.NofC);;
    fprintf(fo,"P5\n %3d %3d\n 255\n",Img.NofC,Img.NofR);  
    for (i=0; i<Img.NofR; i++) {
      k = Img.NofC*i;
      for (j=0; j<Img.NofC; j++) out[j] = Img.gray[k++];
      fwrite(out, sizeof(byte), Img.NofC, fo);
    }
    free(out);
  } // !iscolor
  else {  // P6 - raw, color image
    out =  (byte *)malloc(sizeof(byte)*Img.NofC*3);;
    fprintf(fo,"P6\n %3d %3d\n 255\n",Img.NofC,Img.NofR);  
    for (i=0; i<Img.NofR; i++) {
      k = Img.NofC*i;
      for (j=0; j<Img.NofC; j++) {
	out[3*j] = Img.red[k];
	out[3*j+1] = Img.green[k];
	out[3*j+2] = Img.blue[k++];
      }
      fwrite(out, sizeof(byte), Img.NofC*3, fo);
    }
    free(out);
  } // iscolor
  fclose(fo);
}


// Write gray or color images
void WriteBinaryFile(char *name, struct Buffer b)
{
  FILE *fo;

  printf("\nWriting file '%s'\n",name);

  if ((fo = fopen(name, "wb")) == NULL) {
    fprintf(stderr,"Can't create output file \'%s\'\n",name);
    exit(1);
  } 

  fwrite(b.data, sizeof(byte), b.size, fo);
  fclose(fo);
}


