#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakIndexCounter = 0;

void peakDetection(QRS_params *params) {
	if (params->search[0]<params->search[1] && params->search[1]>params->search[2] && params->RtoR>=50) {
		params->peaks[peakIndexCounter]=params->search[1];
		params->peakToPeak[peakIndexCounter]=params->RtoR+params->peakToPeak[peakIndexCounter-1];
		if (params->peaks[peakIndexCounter]>params->THRESHOLD1) {
			// store Rpeak
			params->Rpeak=params->peaks[peakIndexCounter];
			// add RR to RecentRR
			addRecentRR(params);
			// print test

			//printf("first peak %i\n", params->RecentRR[0]);

			// check if RR should be in RecentRR_OK
			if (params->RR_low<params->RecentRR[0] && params->RecentRR[0]<params->RR_high) {
				addRecentRR_OK(params);
				averageRR(params);
				updateRRboundaries(params);
				updateThresholds(params);

			} else {
				if (params->RtoR > params->RR_miss) {
					searchBack(params);
				}
			}

			//printf("peaks[3] = %d\n",  params->peaks[3]);
			/*
			for (int i = 0; i < peakIndexCounter; i++) {
				printf("peaks[%i] = %d\n",i ,  params->peaks[i]);
			}
			*/

			params->RtoR = 0;
		} else {
			// Update NPFK, when a peak is found
			params->NPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->NPKF;
			updateThresholds(params);
		}
		//printf("RR = %d, peak = %d\n", params->peakToPeak[peakIndexCounter], params->peaks[peakIndexCounter]);
		peakIndexCounter++;

	}
}

void updateNPKF(QRS_params *params){

}

void updateThresholds(QRS_params *params) {
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF-params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void addRecentRR(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR[8-i] = params->RecentRR[7-i];
	}
	params->RecentRR[0] = params->RtoR;
}

void addRecentRR_OK(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR_OK[8-i] = params->RecentRR_OK[7-i];
	}
	params->RecentRR_OK[0] = params->RtoR;
}

void updateRRboundaries(QRS_params *params) {
	params->SPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->SPKF;
	params->RR_low = 0.92*params->RR_Average2;
	params->RR_high = 1.16*params->RR_Average2;
	params->RR_miss = 1.66*params->RR_Average2;
}

void averageRR(QRS_params *params){
	int devisionCounter = 0;

	for(int i = 0; i < 8; i++){
		if (params->RecentRR[i] != 0) {
			params->RR_Average1 += params->RecentRR[i];
			devisionCounter++;
		}
	}
	params->RR_Average1 = params->RR_Average1/devisionCounter;
	//printf("RR average 1 = %d\n", params->RR_Average1);
	params->RR_Average1 = 0;
	devisionCounter = 0;

	for(int i = 0; i < 8; i++){
			if (params->RecentRR_OK[i] != 0) {
				params->RR_Average2 += params->RecentRR_OK[i];
				devisionCounter++;
			}
		}
	params->RR_Average2 = params->RR_Average2/devisionCounter;
	//printf("RR average 2 = %d\n", params->RR_Average2);
	params->RR_Average2 = 0;

}

void searchBack(QRS_params *params) {
	for (int i = peakIndexCounter; i > 0; i--){
		if (params->peaks[i] > params->THRESHOLD2) {
			params->Rpeak = params->peaks[i];
			params->SPKF = 0.25*params->Rpeak + 0.75*params->SPKF;

		}
	}
}

	/*
	printf("NPKF = %i\n", params->NPKF);
	printf("SPKF = %i\n", params->SPKF);
	printf("T1 = %i\n", params->THRESHOLD1);
	printf("T2 = %i\n", params->THRESHOLD2);
	*/
