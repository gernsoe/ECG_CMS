#include "sensor.h"
#include "filters.h"
#include "qsr.h"
#include <stdio.h>
#include <stdlib.h>

// Main function for organizing the program execution.
// The functions and object predefined are just for inspiration.
// Please change orden,names arguments to fit your solution.

int main()
{	
    QRS_params qsr_params;  // Instance of the made avaiable through: #include "qsr.h"
    qsr_params.NPKF = 0;
    qsr_params.SPKF = 0;
    qsr_params.THRESHOLD1 = 0;
    qsr_params.THRESHOLD2 = 0;

    int loopCounter = 0;

	FILE *file = openfile("ECG.txt");
	int raw[33] = {0};
	int low[33] = {0};
	int high[33] = {0};
	int der[33] = {0};
	int sqr[33] = {0};

	int outValue;

	while(1){
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

			for(int i = 1; i < 3; i++) {
				qsr_params.search[3-i] = qsr_params.search[2-i];
			}

			qsr_params.search[0] = outValue;

			if (loopCounter > 2) {
				peakDetection(&qsr_params); // Perform Peak Detection
			}
			loopCounter++;

	}


	return 0;
}


