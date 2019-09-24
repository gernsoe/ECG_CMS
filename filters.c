#include "filters.h"

//variables for filter-functions are named after the offset from current data point
int lowPass(int *ys, int *xs){
	return (2*(*(ys + 1)) - *(ys + 2) + (*(xs + 0) - 2*(*(xs + 6)) + *(xs + 12))/32);
}

int highPass(int *ys, int *xs){
	return (*(ys + 1) - (*(xs + 0))/32 + *(xs + 16) - *(xs + 17) + (*(xs + 32))/32);
}

int derivative(int *xs){
	return ((2*(*(xs + 0)) + *(xs + 1) - *(xs + 3) - 2*(*(xs + 4)))/8);
}

int square(int *xs){
	return (*xs * (*xs));
}

int moveWindow(int *xs){
	int sum = 0;
	for(int i = 0; i < 30; i++){
		sum += *(xs + i);
	}
	return (sum/30);
}
