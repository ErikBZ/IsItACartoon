// This will compile to the CalcStats executable
#include "Statistics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define DIRECTORY "./data/"
#define NUMBEROFFILES 20

int main(int args, char** argv)
{
  char** filenames = malloc(sizeof(char*) * NUMBEROFFILES);
  DIR *d;
  struct dirent *dir;
  int index = 0;
  d = opendir(DIRECTORY);
  int i=0;

  if(d != NULL)
  {
    while((dir = readdir(d)) != NULL && i<NUMBEROFFILES)
    {
      if(dir->d_name[0] != '.')
      {
        char* name = createFilename(DIRECTORY, dir->d_name);
        if(isPPMImage(name))
        {
          filenames[index] = malloc(strlen(name)+1);  // extra 1 for null terminator
          strcpy(filenames[index], name);
          printf("%s\n", filenames[index]);
          index++;
        }
        else
        {
          fprintf(stderr, "%s is not a PPM image and will be skipped\n", name);
        }
        free(name);
        name = NULL;
      }
      i++;
    }
  }

  Stats* stats = getAllStats(filenames, index, 20.0);

  FILE* file = fopen("output", "wb");
  if(file != NULL)
  {
    fwrite(&index, sizeof(int), 1, file);
    // lol was writing the wrong file. woops
    fwrite(stats, sizeof(Stats), index, file);
    fclose(file); // some indeces in stats aren't instantiating
  }

  free(stats);
  for(i=0;i<index;i++)
  {
    free(filenames[i]);
    filenames[i] = NULL;
  }
  free(filenames);
  filenames = NULL;
  closedir(d);
  exit(0);
}
