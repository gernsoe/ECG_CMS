#ifndef SENSOR_H
#define SENSOR_H
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int frequency;
int cycle2ms(int cycle);
int ms2cycle(int ms);
int pulse(int *Rs, int size);
char* display(int lastPeak, int peakValue, int pulse, char warningCode);


#endif
