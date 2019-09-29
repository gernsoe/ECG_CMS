#ifndef SENSOR_H
#define SENSOR_H
#include <stdio.h>
#include <stdlib.h>


//Returns pointer to file with filename
FILE* openfile(const char* filename);

//Takes pointer to file and returns next integer
int getNextData(FILE *file);

#endif
