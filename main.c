#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include "display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.
int loopCounter = 0;

int main()
{	
    QRS_params qsr_params;  // Instance of the made avaiable through: #include "qsr.h"
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

    memset(qsr_params.RecentRR, 0, sizeof(int)*8);
    memset(qsr_params.RecentRR_OK, 0, sizeof(int)*8);
    memset(qsr_params.peaks, 0, sizeof(int)*100);
    memset(qsr_params.Rpeaks, 0, sizeof(int)*100);
    memset(qsr_params.peakToPeak, 0, sizeof(int)*100);
    memset(qsr_params.RpeakToRpeak, 0, sizeof(int)*100);

	FILE *file = openfile("ECG.txt");
	int raw[33] = {0};
	int low[33] = {0};
	int high[33] = {0};
	int der[33] = {0};
	int sqr[33] = {0};

	int outValue;

	while(1){
		/*
		if (loopCounter > 3000) {
			break;
		}
		*/

			//pushes down queues
			for (int i = 1; i < 33; i++){
				raw[33-i] = raw[32-i];
				low[33-i] = low[32-i];
				high[33-i] = high[32-i];
				der[33-i] = der[32-i];
				sqr[33-i] = sqr[32-i];
			}
			raw[0] = getNextData(file);
			if(raw[0] == -9999){
				break;
			}
			//printf("-------------\n");
			//printf("%d\n",raw[0]);
			low[0] = lowPass(low,raw);
			//printf("%d\n",low[0]);
			high[0] = highPass(high,low);
			//printf("%d\n",high[0]);
			der[0] = derivative(high);
			//printf("%d\n",der[0]);
			sqr[0] = square(der);
			//printf("%d\n",sqr[0]);
			outValue = moveWindow(sqr);
			//printf("%d\n",outValue);

			// push down on peak search array
			for(int i = 1; i < 3; i++) {
				qsr_params.search[3-i] = qsr_params.search[2-i];
			}

			// counter for RR intervals

			qsr_params.search[0] = outValue;

			if (loopCounter > 2) {
				peakDetection(&qsr_params); // Perform Peak Detection
			}
			qsr_params.PtoPcounter++;
			qsr_params.RtoRcounter++;
			loopCounter++;
			//incrementCounters(qsr_params);

			if (qsr_params.RR_WarningCounter >= 5) {

			}
	}

	return 0;
}

void incrementCounters(QRS_params *params) {
	//params.dataPointCounter++;
	//params.RtoRcounter++;
	loopCounter++;
}


