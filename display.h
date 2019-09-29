#ifndef SENSOR_H
#define SENSOR_H
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

//Frequency of measurement: 250/second

//Takes cycle number and returns millisecond, counting chronologically
int cycle2ms(int cycle);
//returns filtered cycle given timestamp, counting chronologically
int ms2cycle(int ms);

//returns pulse given an array of n of cycles between "size" previous R-peaks
int pulse(int *Rs, int size);

//Returns string containing output given data
char* display(int lastPeak, int peakValue, int pulse, char warningCode);


#endif
