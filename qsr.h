#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{ // Structure for QRS parameters
   int SPKF;					// Estimated average Rpeak value
   int NPKF; 					// Estimated average noise peak value
   int THRESHOLD1;				// All peaks higher than this is classified as an Rpeak
   int THRESHOLD2;				// Used in searchback to find missed Rpeaks
   int peaks[1000];				// All found peaks are stored here
   int Rpeaks[1000];			// All found Rpeaks are stored here
   int peakToPeak[1000];		// Contains timestamp from every peak
   int RpeakToRpeak[1000];		// Contains timestamp from every Rpeak
   int RecentRR[8];				// Contains the 8 most recent RR intervals
   int RecentRR_OK[8];			// Contains the 8 most recent RR intervals, where RR_low < RR < RR_high
   int search[3];				// Stores 3 datapoints used for determining if a peak is found
   int RR_high;					// Upper limit of normal RR interval
   int RR_low;					// Lower limit of normal RR interval
   int RR_miss;					// Start searchback if RR > RR_miss
   int RR_Average1;				// The average RR interval based on RecentRR
   int RR_Average2;				// The average RR interval based on RecentRR_OK

   int dataPointCounter;		// Counts every data input
   int RtoRcounter;				// Counts data input between Rpeaks
   int RR_WarningCounter;		// Counts when RR < RR_low or RR_miss < RR

} QRS_params;

void peakDetection(QRS_params *params);

void updateThresholds(QRS_params *params);

void addRRinterval(QRS_params *params);

void updateRRboundaries(QRS_params *params);

void updateRRboundariesSearchBack(QRS_params *params);

void addRecentRR_OK(QRS_params *params);

void averageRR(QRS_params *params);

void addRecentRR(QRS_params *params);

void searchBack(QRS_params *params);


#endif // QSR_H
