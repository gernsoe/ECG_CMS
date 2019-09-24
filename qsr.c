#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakIndexCounter = 0;
void peakDetection(QRS_params *params)
{
	if (params->search[0]<params->search[1] && params->search[1]<params->search[2]) {
		params->peaks[peakIndexCounter]=params->search[1];
		peakIndexCounter++;
	}

	printf("%d",params->peaks[0]);
	/*
	printf("NPKF = %i\n", params->NPKF);
	printf("SPKF = %i\n", params->SPKF);
	printf("T1 = %i\n", params->THRESHOLD1);
	printf("T2 = %i\n", params->THRESHOLD2);
	*/
}
