#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int getNextData(FILE *file)
{
	int value;
	if(!(fscanf(file,"%i",&value) == 1)){
		printf("End of file\n");
		return INT_MIN; //Magic value
	}
    return value;
}

FILE* openfile(const char *filename)
{
	FILE* file;
	if((file = fopen(filename,"r")) == 0){
		printf("File not found\n");
		return NULL;
	}
	return file;
}
