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

void testFunction(void* thing);

int main(int argc, char** argv)
{
  // array of Linked Lists representing blobs
  BlobLL* blobs = malloc(sizeof(BlobLL) * 4);
  int blobsSize = 0;
  int blobsMaxSize = 4;

  // used for horizontal lines
  // make sure row is initialized with something
  BlobLL row;
  row.dummy = malloc(sizeof(Node));
  Node* n = malloc(sizeof(Node));
  n->data = malloc(sizeof(Node));
  Node* n2 = malloc(sizeof(Node));
  n2->data = malloc(sizeof(Node));
  add(&row, n);
  add(&row, n2);
  printLinkedList(&row);

  printf("\n");
}

void testFunction(void* thing)
{
  BlobLL* t = thing;
  printf("%d\n", t->size);
}
