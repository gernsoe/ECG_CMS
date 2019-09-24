#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>

int getNextData(FILE *file)
{
	int value;

	if(!(fscanf(file,"%i",&value) == 1)){
		printf("End of file");
		return -9999;
	}

    return value; // return sensor value
}

FILE* openfile(const char *filename)
{
	FILE* file;
	if((file = fopen(filename,"r")) == 0){
		printf("File not found");
	}
	return file;
}
