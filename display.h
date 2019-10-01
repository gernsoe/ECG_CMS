#ifndef SENSOR_H
#define SENSOR_H
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

//returns number of millisecond since an event, given number of cycles since event
int cycle2ms(int cycle);

//return pulse given number of cycles between two beats
int pulse(int cycles);

//Returns string containing output given data
char* display(int lastPeak, int peakValue, int pulse, int warningCode);


#endif
