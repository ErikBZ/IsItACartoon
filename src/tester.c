/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "Statistics.h"

int main(int argc, char** argv)
{
  Stats s;
  s.numOfBlobs = 5;
  printf("%d\n", s.numOfBlobs);
}
