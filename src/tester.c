/*
  This will be the main file that is used to test out code and stuff
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "image.h"
#include "ColorClamp.h"
#include "BlobLL.h"
#include "BlobManager.h"

int main(int argc, char** argv)
{
  Node* newNode = malloc(sizeof(Node));
  newNode->next = NULL;
  newNode->prev = NULL;

  LineBlob* lb1 = malloc(sizeof(LineBlob));
  lb1->startIndex = 10;
  lb1->endIndex = 456;
  lb1->row = 0;
  lb1->averages[0] = 100;
  lb1->averages[1] = 100;
  lb1->averages[2] = 100;


  LineBlob* lb2 = malloc(sizeof(LineBlob));
  lb2->startIndex = 0;
  lb2->endIndex = 10;
  lb2->row = 1;
  lb2->averages[0] = 98;
  lb2->averages[1] = 98;
  lb2->averages[2] = 90;

  double tol = 10;

  byte b = IsSimilarColor(lb1, lb2, tol);
  printf("%d\n", b);
}
