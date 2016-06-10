/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "image.h"
#include "BlobLL.h"
#include "Queue.h"
#include "Statistics.h"

#define NOTVISISTED 0
#define PICTURETYPE 7

char* createFilename(char* d, char* f)
{
  char *result = malloc(strlen(d) + strlen(f) + 1);
  strcpy(result, d);
  strcat(result, f);
  return result;
}

Stats* getAllStats(char** files, int size, double tol)
{
  int i;
  int j;
  byte* visited;

  struct Image* img;
  Stats* stats = malloc(sizeof(Stats) * size);

  for(i=0;i<size;i++)
  {
    // reading in the next image
    img = malloc(sizeof(struct Image));
    ReadImage(files[i], img);
    // reallocing the visited array so that it matches the size
    // of the current image, then reiniting it
    visited = malloc(sizeof(byte) * img->NofC * img->NofR);
    for(j=0;j<size;j++)
    {
      visited[j] = NOTVISISTED;
    }

    int blobSize = 0;
    Blob* blobs = GetAllBlobsInImage(img, tol, &blobSize);
    Stats s = findStatsOfAnImage(img, blobs, blobSize, files[i][PICTURETYPE]);
    stats[i] = s;

    printf("Image statistics calculated sucessfully\n");

    free(visited);
    free(img);
    visited = NULL;
    img = NULL;
  }
  return stats;
}


int main(int argc, char** argv)
{
  // will find the stats and output it
  if(argc != 2)
  {
    printf("Invalid input. Exiting\n");
    exit(0);
  }

  char** filenames = malloc(sizeof(char*) * 4);
  DIR *d;
  struct dirent *dir;
  int index = 0;
  d = opendir("./data");
  if(d)
  {
    while((dir = readdir(d)) != NULL)
    {
      if(dir->d_name[0] != '.')
      {
        filenames[index] = createFilename(argv[1], dir->d_name);
        printf("%s\n", filenames[index]);
        index++;
      }
    }
  }

  Stats* stats = getAllStats(filenames, index, 20.0);

  FILE* file = fopen("output", "wb");
  if(file != NULL)
  {
    fwrite(&index, sizeof(int), 1, file);
    // lol was writing the wrong file. woops
    fwrite(stats, sizeof(Stats), index, file);
    fclose(file);
  }

  Stats* st = malloc(sizeof(Stats) * index);
  FILE* file2 = fopen("output", "rb");
  Stats temp;
  int size;

  if(file2 != NULL)
  {
    fread(&size, sizeof(int), 1, file2);
    fread(st, sizeof(Stats), index, file2);
    fclose(file2);
  }

  int i;
  for(i=0;i<size;i++ )
  {
    printf("Hello\n");
    printStats(st[i]);
  }

  closedir(d);
  exit(0);
}
