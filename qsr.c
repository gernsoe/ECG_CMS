#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakIndexCounter = 0;

void peakDetection(QRS_params *params) {
	if (params->search[0]<params->search[1] && params->search[1]>params->search[2]) {
		params->peaks[peakIndexCounter]=params->search[1];
		if (params->peaks[peakIndexCounter]>params->THRESHOLD1) {
			// store Rpeak
			params->Rpeak=params->peaks[peakIndexCounter];

			addRRinterval(&params);
			printf("first peak %i\n", params->Rpeak);
		} else {
			updateThresholds(&params);
		}
		peakIndexCounter++;
	}
}

void updateThresholds(QRS_params *params) {
	params->NPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->NPKF;
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF-params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void addRRinterval(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RR[8-i] = params->RR[7-i];
	}
	params->RR[0] = params->intervalCounter;
	params->intervalCounter = 0;
}



	/*
	printf("NPKF = %i\n", params->NPKF);
	printf("SPKF = %i\n", params->SPKF);
	printf("T1 = %i\n", params->THRESHOLD1);
	printf("T2 = %i\n", params->THRESHOLD2);
	*/
