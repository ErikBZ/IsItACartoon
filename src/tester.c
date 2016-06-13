/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "image.h"
#include "BlobLL.h"
#include "Statistics.h"

#define NOTVISISTED 0
#define PICTURETYPE 7

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
  d = opendir(argv[1]);
  if(d)
  {
    while((dir = readdir(d)) != NULL)
    {
      if(dir->d_name[0] != '.')
      {
        char* name = createFilename(argv[1], dir->d_name);
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
