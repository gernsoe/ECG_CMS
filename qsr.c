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
			// add RR to RecentRR
			addRecentRR(params);
			// print test
			printf("first peak %i\n", params->RecentRR[0]);
			// check if RR should be in RecentRR_OK
			if (params->RR_low<params->RecentRR[0] && params->RecentRR[0]<params->RR_high) {
				addRecentRR_OK(params);
				updateRRboundaries(params);

			}
			params->intervalCounter = 0;
		} else {
			updateThresholds(params);
		}

		peakIndexCounter++;
	}
}

void updateThresholds(QRS_params *params) {
	params->NPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->NPKF;
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF-params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void addRecentRR(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR[8-i] = params->RecentRR[7-i];
	}
	params->RecentRR[0] = params->intervalCounter;
}

void addRecentRR_OK(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR_OK[8-i] = params->RecentRR_OK[7-i];
	}
	params->RecentRR_OK[0] = params->intervalCounter;
}

void updateRRboundaries(QRS_params *params) {
	params->SPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->SPKF;

}

	/*
	printf("NPKF = %i\n", params->NPKF);
	printf("SPKF = %i\n", params->SPKF);
	printf("T1 = %i\n", params->THRESHOLD1);
	printf("T2 = %i\n", params->THRESHOLD2);
	*/
