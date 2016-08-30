#include <stdlib.h>
#include <stdio.h>
#include "Statistics.h"
#include "BlobLL.h"

void TestCalcBlob();
void ReadData();

void main(int argc, char** argv)
{
	ReadData();
	exit(0);
}

void ReadData()
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
}

void TestCalcBlob()
{
	struct Image img;
	img.NofC = 0;
	img.NofR = 0;
	img.isColor = 0;
	img.gray = NULL;
	img.blue = NULL;
	img.green = NULL;
	img.red = NULL;

	ReadImage("./data/d_jojo.ppm", &img);
	int blobsSize = 0;
	Blob* blobs = GetAllBlobsInImage(&img, 20.0, &blobsSize);
	printf("Hello");
}