#ifndef QSR_H
#define QSR_H

// Header file for QRS functionality 
// it is recommended to use the structure "QRS_parameters"
// to organize all variables and parameters

typedef struct QRS_params
{ // Structure for QRS parameters
   int SPKF;
   int NPKF; 
   int THRESHOLD1;
   int THRESHOLD2;
   int peaks[100];
   int RecentRR[8];
   int RecentRR_OK[8];
   int search[3];
   int Rpeak;
   int RR_high;
   int RR_low;
   int RR_miss;
   int RR_Average1;
   int RR_Average2;


   int RtoR;

   // Add parameters that are missing
   // or of use to you
   
} QRS_params;

// Feel free to change and add methods
void peakDetection(QRS_params *params);

void updateThresholds(QRS_params *params);

void addRRinterval(QRS_params *params);

void updateRRboundaries(QRS_params *params);

void addRecentRR_OK(QRS_params *params);

void averageRR(QRS_params *params);

void addRecentRR(QRS_params *params);

void searchBack(QRS_params *params);

#endif // QSR_H
