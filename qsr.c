#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakIndexCounter = 0;				// Used to navigate forward when storing peaks
int RpeakIndexCounter = 0;				// Used to navigate forward when storing Rpeaks
int searchBackCounterTest = 0;

void peakDetection(QRS_params *params) {
	if (params->search[0]<params->search[1] && params->search[1]>params->search[2]
				&& 50<=params->RtoRcounter) {

		params->peaks[peakIndexCounter] = params->search[1];
		params->peakToPeak[peakIndexCounter] = params->dataPointCounter;

		if (params->peaks[peakIndexCounter]>params->THRESHOLD1) {
			params->Rpeaks[RpeakIndexCounter] = params->peaks[peakIndexCounter]; // Store the R peak in the R peak array
			params->RpeakToRpeak[RpeakIndexCounter] = params->dataPointCounter; // Store the time between the R peaks
			addRecentRR(params);

			/*
			printf("RR = %d, R[%d] = %d, T1 = %d, T2 = %d\n", params->RpeakToRpeak[RpeakIndexCounter],
								RpeakIndexCounter, params->Rpeaks[RpeakIndexCounter], params->THRESHOLD1, params->THRESHOLD2);
								*/

			// check if RR should be in RecentRR_OK
			//printf("Low = %d, RR = %d, high = %d, miss = %d\n", params->RR_low, params->RtoRcounter, params->RR_high, params->RR_miss );
			if (params->RR_low < params->RtoRcounter && params->RtoRcounter<params->RR_high) {
				addRecentRR_OK(params);
				averageRR(params);
				updateRRboundaries(params);
				updateThresholds(params);
			}
			/*
			for (int i = 0; i < 35;i++) {
				printf("RR = %d, peak = %d, SB = %d\n",
					params->RtoRcounter, params->RecentRR_OK[0], params->RR_miss);
			}
			*/
			RpeakIndexCounter++;
			params->RtoRcounter = 0;
		} else {
			// Update NPFK, when a peak is found
			params->NPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->NPKF;
			updateThresholds(params);
		}

		peakIndexCounter++;
	}

	if (params->RtoRcounter > params->RR_miss) {
		searchBack(params);
	}

	printf("RR = %d, Rpeak = %d, total = %d, SB = %d\n", params->RtoRcounter, params->RpeakToRpeak[RpeakIndexCounter-1], params->dataPointCounter, searchBackCounterTest);  //This shows the when searchBack should happen
}


void updateThresholds(QRS_params *params) {
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF-params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void addRecentRR(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR[8-i] = params->RecentRR[7-i];
	}
	params->RecentRR[0] = params->RpeakToRpeak[RpeakIndexCounter] - params->RpeakToRpeak[RpeakIndexCounter - 1];
}

void addRecentRR_OK(QRS_params *params) {
	for(int i = 1; i < 8; i++){
		params->RecentRR_OK[8-i] = params->RecentRR_OK[7-i];
	}
	params->RecentRR_OK[0] = params->RpeakToRpeak[RpeakIndexCounter] - params->RpeakToRpeak[RpeakIndexCounter - 1];
}

void updateRRboundaries(QRS_params *params) {
	params->SPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->SPKF;
	params->RR_low = 0.92*params->RR_Average2;
	params->RR_high = 1.16*params->RR_Average2;
	params->RR_miss = 1.66*params->RR_Average2;
}

void updateRRboundariesSearchBack(QRS_params *params) {
	int devCounter = 0;
	params->RR_Average1 = 0;
	for(int i = 0; i < 8; i++){									// Update RR_average 1
		if (params->RecentRR[i] != 0) {
			params->RR_Average1 += params->RecentRR[i];
			devCounter++;
		}
	}
	params->RR_Average1 = params->RR_Average1/devCounter;
	params->RR_low = 0.92*params->RR_Average1;
	params->RR_high = 1.16*params->RR_Average1;
	params->RR_miss = 1.66*params->RR_Average1;
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF - params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void averageRR(QRS_params *params){
	int devisionCounter = 0;
	params->RR_Average1 = 0;
	params->RR_Average2 = 0;

	for(int i = 0; i < 8; i++){
		if (params->RecentRR[i] != 0) {
			params->RR_Average1 += params->RecentRR[i];
			devisionCounter++;
		}
	}
	params->RR_Average1 = params->RR_Average1/devisionCounter;
	//printf("RR average 1 = %d\n", params->RR_Average1);

	devisionCounter = 0;

	for(int i = 0; i < 8; i++){
			if (params->RecentRR_OK[i] != 0) {
				params->RR_Average2 += params->RecentRR_OK[i];
				devisionCounter++;
			}
		}
	params->RR_Average2 = params->RR_Average2/devisionCounter;
	//printf("RR average 2 = %d\n", params->RR_Average2);

}

void searchBack(QRS_params *params) {
	for (int i = peakIndexCounter; i > 0; i--){ 					// loops through peaks to find a missed peak2
		if (params->peaks[i] > params->THRESHOLD2) { 				// if peak2 is higher than threshold2, then it's an Rpeak
			params->Rpeaks[RpeakIndexCounter] = params->peaks[i];
			params->SPKF = 0.25*params->Rpeaks[RpeakIndexCounter] + 0.75*params->SPKF;
			params->RpeakToRpeak[RpeakIndexCounter] = 										// RR is calculated between found peak and
					params->peakToPeak[i];	// and the latest Rpeak TODO wrong value
			addRecentRR(params);
			updateRRboundariesSearchBack(params);
			searchBackCounterTest++;
			params->RtoRcounter = 0;

			/*
			printf("Search: RR-1 = %d, ptop = %d, peak = %d\n", params->RpeakToRpeak[RpeakIndexCounter-1], params->peakToPeak[i], params->peaks[peakIndexCounter]);
			printf("Search: RR = %d, Rpeak = %d, total = %d\n", params->RpeakToRpeak[RpeakIndexCounter],
					params->Rpeaks[RpeakIndexCounter], params->dataPointCounter);
					*/
			break;
		} else if (params->peaks[i] > params->THRESHOLD1) {
			break;
		}
	}
}

