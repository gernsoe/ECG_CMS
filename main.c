#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>



int main()
{	
	//Define stuff-----------------
	char* filename = "ECG.txt";
	FILE *fValues = fopen("values.txt","w");
	FILE *fThreshold1 = fopen("threshold1.txt","w");
	FILE *fThreshold2 = fopen("threshold2.txt","w");
	FILE *fRpeak = fopen("Rpeak.txt","w");
	FILE *fSearchBack = fopen("searchBack.txt","w");
	FILE *fWarning = fopen("warning.txt","w");
	FILE *file = openfile(filename);
    QRS_params qsr_params;  // Instance of the made avaiable through: #include "qsr.h"
    int outValue;
    int warning;
    //-----------------------------

    //Initialise values----------------
    int loopCounter = 0;
    qsr_params.NPKF = 0;
    qsr_params.SPKF = 0;
    qsr_params.THRESHOLD1 = 0;
    qsr_params.THRESHOLD2 = 0;
    qsr_params.RR_high = 10000;
    qsr_params.RR_low = 0;
    qsr_params.RtoRcounter=0;
    qsr_params.PtoPcounter=0;
    qsr_params.RR_miss = 300;
    qsr_params.RR_Average1 = 0;
    qsr_params.RR_Average2 = 0;
    qsr_params.RR_WarningCounter = 0;
    qsr_params.RpeakIndexCounter = 0;
    memset(qsr_params.RecentRR, 0, sizeof(int)*8);
    memset(qsr_params.RecentRR_OK, 0, sizeof(int)*8);
    memset(qsr_params.peaks, 0, sizeof(int)*100);
    memset(qsr_params.Rpeaks, 0, sizeof(int)*100);
    memset(qsr_params.peakToPeak, 0, sizeof(int)*100);
    memset(qsr_params.RpeakToRpeak, 0, sizeof(int)*100);
    int raw[33] = {0};
    int low[33] = {0};
    int high[33] = {0};
    int der[33] = {0};
    int sqr[33] = {0};
    //-----------------------------------

    //Exit if no input------
	if(file == NULL){
		fclose(file);
		fclose(fValues);
		fclose(fThreshold1);
		fclose(fThreshold2);
		fclose(fWarning);
		fclose(fRpeak);
		fclose(fSearchBack);
		return -1;
	}
	//----------------------



	while(1){
			warning = 0;

			//pushes down queues
			for (int i = 1; i < 33; i++){
				raw[33-i] = raw[32-i];
				low[33-i] = low[32-i];
				high[33-i] = high[32-i];
				der[33-i] = der[32-i];
				sqr[33-i] = sqr[32-i];
			}
			//------------------


			//Get next data point
			raw[0] = getNextData(file);
			if(raw[0] == INT_MIN){ //End if magic number is returned (meaning end of file)
				break;
			}
			//-------------------



			//Apply filters
			low[0] = lowPass(low,raw);
			high[0] = highPass(high,low);
			der[0] = derivative(high);
			sqr[0] = square(der);
			outValue = moveWindow(sqr);
			//-------------


			// push down on peak search array
			for(int i = 1; i < 3; i++) {
				qsr_params.search[3-i] = qsr_params.search[2-i];
			}
			qsr_params.search[0] = outValue;
			//-------------------------------


			//Perform peak detection
			if (loopCounter > 2) {
				peakDetection(&qsr_params,fRpeak,fSearchBack,loopCounter); // Perform Peak Detection
			}
			//----------------------


			//Choose error warning
			if (qsr_params.RR_WarningCounter >= 5) {
				warning += 2;
			}
			if (qsr_params.Rpeaks[qsr_params.RpeakIndexCounter-1] < 2000) {
				warning += 1;
			}
			//---------------------


			//Display-------------
			if(qsr_params.RpeakIndexCounter > 0){
				printf(display(cycle2ms(qsr_params.RtoRcounter),qsr_params.Rpeaks[qsr_params.RpeakIndexCounter-1],pulse(qsr_params.RpeakToRpeak[qsr_params.RpeakIndexCounter-1]),warning));
			}
			//--------------------


			//Write to output files
			fprintf(fValues,"%d\n",outValue);
			fprintf(fThreshold1,"%d\n",qsr_params.THRESHOLD1);
			fprintf(fThreshold2,"%d\n",qsr_params.THRESHOLD2);
			fprintf(fWarning,"%d\n",warning);
			//rPeaks and searchBack is handled in qsr.c
			//---------------------


			//Increment indexes
			loopCounter++;
			qsr_params.PtoPcounter++;
			qsr_params.RtoRcounter++;
			//-----------------

	}

	//Close files-------
	fclose(file);
	fclose(fValues);
	fclose(fThreshold1);
	fclose(fThreshold2);
	fclose(fWarning);
	fclose(fRpeak);
	fclose(fSearchBack);
	//------------------

	return 0;
}



