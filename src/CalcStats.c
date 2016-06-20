// This will compile to the CalcStats executable
#include "Statistics.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#define DIRECTORY "./data/"
#define NUMBEROFFILES 14

int main(int args, char** argv)
{
  char** filenames = malloc(sizeof(char*) * NUMBEROFFILES);
  DIR *d;
  struct dirent *dir;
  int index = 0;
  d = opendir(DIRECTORY);
  if(d)
  {
    while((dir = readdir(d)) != NULL)
    {
      if(dir->d_name[0] != '.')
      {
        char* name = createFilename(DIRECTORY, dir->d_name);
        if(isPPMImage(name))
        {
          filenames[index] = name;
          printf("%s\n", filenames[index]);
          index++;
        }
        else
        {
          fprintf(stderr, "%s is not a PPM image and will be skipped\n", name);
        }
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

  closedir(d);
  exit(0);
}
