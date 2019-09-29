#ifndef FILTERS_H
#define FILTERS_H

//*ys: a pointer to an array of length 33, containing previous datapoints already passed through the filter
//*xs: a pointer to an array of length 33, containing previous datapoints not passed through the filter

//each function returns the filtered datapoint corresponding to *(xs + 0)

int lowPass(int *ys, int *xs);
int highPass(int *ys, int *xs);
int derivative(int *xs);
int square(int *xs);
int moveWindow(int *xs);

 
#endif // FILTERS_H
