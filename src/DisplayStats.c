#include <stdlib.h>
#include <stdio.h>
#include "Statistics.h"

void main(int argc, char** argv)
{
	FILE* file = fopen("output", "rb");
	int size;
	Stats* stats;

	if(file != NULL)
	{
		fread(&size, sizeof(int), 1, file);
		stats = malloc(sizeof(Stats) * size);
		fread(stats, sizeof(Stats), size, file);
	}

	int i = 0;
	for(i=0;i<size;i++)
	{
		printStats(stats[i]);
	}

	free(stats);
	fclose(file);
	exit(0);
}