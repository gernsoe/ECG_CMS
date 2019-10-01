#ifndef QSR_H
#define QSR_H
#include <stdio.h>

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters
int RpeakIndexCounter;
int foundRpeak;
typedef struct QRS_params
{ // Structure for QRS parameters
   int SPKF;					// Estimated average Rpeak value
   int NPKF; 					// Estimated average noise peak value
   int THRESHOLD1;				// All peaks higher than this is classified as an Rpeak
   int THRESHOLD2;				// Used in searchback to find missed Rpeaks
   int peaks[100];				// All found peaks are stored here
   int Rpeaks[100];			// All found Rpeaks are stored here
   int peakToPeak[100];		// Contains timestamp from every peak
   int RpeakToRpeak[100];		// Contains timestamp from every Rpeak
   int RecentRR[8];				// Contains the 8 most recent RR intervals
   int RecentRR_OK[8];			// Contains the 8 most recent RR intervals, where RR_low < RR < RR_high
   int search[3];				// Stores 3 datapoints used for determining if a peak is found
   int RR_high;					// Upper limit of normal RR interval
   int RR_low;					// Lower limit of normal RR interval
   int RR_miss;					// Start searchback if RR > RR_miss
   int RR_Average1;				// The average RR interval based on RecentRR
   int RR_Average2;				// The average RR interval based on RecentRR_OK

   int PtoPcounter;				// Counts data input between peaks
   int RtoRcounter;				// Counts data input between Rpeaks
   int RR_WarningCounter;		// Counts when RR < RR_low or RR_miss < RR

} QRS_params;

void peakDetection(QRS_params *params,FILE* fRpeak,FILE* fSearchBack,int loopCount);

void updateThresholds(QRS_params *params);

void addRRinterval(QRS_params *params);

void updateRRboundaries(QRS_params *params);

void updateRRboundariesSearchBack(QRS_params *params);

void addRecentRR_OK(QRS_params *params);

void averageRR(QRS_params *params);

void addRecentRR(QRS_params *params);

void searchBack(QRS_params *params,FILE* output,int loopCount);

void increment(int index, int incrementValue, int PorR);


#endif // QSR_H
