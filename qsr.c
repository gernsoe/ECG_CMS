#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

int peakIndexCounter = 0;				// Used to navigate forward when storing peaks
int searchBackCounter = 0;				// Used to track searchBacks

void peakDetection(QRS_params *params, FILE *fRpeak, FILE *fSearchBack, int loopCount) {
	if (params->search[0]<params->search[1] && params->search[1]>params->search[2]  // If x_n-1 < x_n < x_n+1 and 200 ms then add to peaks
				&& 50<=params->RtoRcounter) {

		params->peaks[peakIndexCounter] = params->search[1];	// Add x_n to peaks
		params->peakToPeak[peakIndexCounter] = params->PtoPcounter;	// Add the time the peak was found

		if (params->peaks[peakIndexCounter]>params->THRESHOLD1) {	// If found x_n > Threshhold1

			fprintf(fRpeak,"%d\n",loopCount);  // Add current place in data to rPeak file

			params->Rpeaks[params->RpeakIndexCounter] = params->peaks[peakIndexCounter]; // Store x_n in the R peak array
			params->RpeakToRpeak[params->RpeakIndexCounter] = params->RtoRcounter; // Add the time the R peak was found
			addRecentRR(params);

			if (params->RR_low < params->RtoRcounter && params->RtoRcounter<params->RR_high) {  // check if RR should be in RecentRR_OK
				addRecentRR_OK(params);
				averageRR(params);
				updateRRboundaries(params);
				updateThresholds(params);
				params->RR_WarningCounter = 0;
			} else {
				params->RR_WarningCounter++;
			}


			params->RtoRcounter = 0;
			params->RpeakIndexCounter = increment(params->RpeakIndexCounter, 1, 100);

		} else {	// x_n was not an R peak and is added as a noise peak
			params->NPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->NPKF;	// Update NPFK
			updateThresholds(params);
		}


		peakIndexCounter = increment(peakIndexCounter, 1, 100);
		params->PtoPcounter=0;
	}

	if (params->RtoRcounter > params->RR_miss) {	// If the interval between heartbeats get too large
		searchBack(params,fSearchBack,loopCount);
	}
}


void updateThresholds(QRS_params *params) {		// Updates the thresholds in params
	params->THRESHOLD1 = params->NPKF + 0.25*(params->SPKF-params->NPKF);
	params->THRESHOLD2 = 0.5*params->THRESHOLD1;
}

void addRecentRR(QRS_params *params) {	// Add recent RR interval to the RecentRR array
	for(int i = 1; i < 8; i++){
		params->RecentRR[8-i] = params->RecentRR[7-i];
	}
	params->RecentRR[0] = params->RpeakToRpeak[params->RpeakIndexCounter];
}

void addRecentRR_OK(QRS_params *params) {	// Add recent RR interval, which was between RR_low and RR_high, to the RecentRR_OK array
	for(int i = 1; i < 8; i++){
		params->RecentRR_OK[8-i] = params->RecentRR_OK[7-i];
	}
	params->RecentRR_OK[0] = params->RpeakToRpeak[params->RpeakIndexCounter];
}

void updateRRboundaries(QRS_params *params) {	// Update the interval for an accepted RR interval
	params->SPKF = 0.125*params->peaks[peakIndexCounter] + 0.875*params->SPKF;
	params->RR_low = 0.92*params->RR_Average2;
	params->RR_high = 1.16*params->RR_Average2;
	params->RR_miss = 1.66*params->RR_Average2;
}

void updateRRboundariesSearchBack(QRS_params *params) {		// When an Rpeak is found with searchback, parameters are updates
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

void averageRR(QRS_params *params){		// Calculates the average RR interval, based on the previous 8 measured RR intervals
	params->RR_Average1 = 0;
	params->RR_Average2 = 0;

	int devisionCounter = 0;
	for(int i = 0; i < 8; i++){
		if (params->RecentRR[i] != 0) {
			params->RR_Average1 += params->RecentRR[i];
			devisionCounter++;
		}
	}
	params->RR_Average1 = params->RR_Average1/devisionCounter;

	devisionCounter = 0;
	for(int i = 0; i < 8; i++){
			if (params->RecentRR_OK[i] != 0) {
				params->RR_Average2 += params->RecentRR_OK[i];
				devisionCounter++;
			}
		}
	params->RR_Average2 = params->RR_Average2/devisionCounter;
}

void searchBack(QRS_params *params,FILE *output,int loopCount) {
	searchBackCounter++;
	int peakCycleSum = 0;

	for (int i = peakIndexCounter; i > 0; i--){ // loops through peaks to find a missed peak2
		if (params->peaks[i] > params->THRESHOLD2 && params->peaks[i] < params->THRESHOLD1) { 	// if peak2 is higher than threshold2, then it's an Rpeak
			params->Rpeaks[params->RpeakIndexCounter] = params->peaks[i];
			params->SPKF = 0.25*params->Rpeaks[params->RpeakIndexCounter] + 0.75*params->SPKF;
			for (int j = peakIndexCounter; j > i; j--) {
				peakCycleSum = params->peakToPeak[j];
			}
			params->RpeakToRpeak[params->RpeakIndexCounter] = params->RtoRcounter - (peakCycleSum + params->PtoPcounter);
			addRecentRR(params);
			updateRRboundariesSearchBack(params);
			params->RtoRcounter = params->RtoRcounter-params->RpeakToRpeak[params->RpeakIndexCounter];
			fprintf(output,"%d\n",(loopCount-params->RtoRcounter));
			break;
		} else if (params->peaks[i] > params->THRESHOLD1) {		// If the Rpeak has already been found, searchback is stopped
			break;
		}
	}
}

int increment(int index, int incrementValue, int maxSize) {
	return (index + incrementValue) % maxSize;
}
