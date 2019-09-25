#ifndef FILTERS_H
#define FILTERS_H

int lowPass(int *ys, int *xs);
int highPass(int *ys, int *xs);
int derivative(int *xs);
int square(int *xs);
int moveWindow(int *xs);

 
#endif // FILTERS_H
