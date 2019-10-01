#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>0

int cycle2ms(int cycle){
	return (cycle*1000/250) + 25 + 80 + 10 + 73;
}

int pulse(int cycles){
	return 250*60/cycles;
}


char* display(int lastPeak, int peakValue, int pulse, int warningCode){

	//INITIALIZE
	char warning[40];
	char *message;
	message = malloc(250);
	char line[26];
	//----------


	//CHOOSE WARNING CODE
	switch(warningCode){
	case 3:
		strcpy(warning, "LOW R-PEAK VALUE\n5 MISSED R-PEAKS\n");
		break;

	case 1:
		strcpy(warning, "LOW R-PEAK VALUE\n");
		break;

	case 2:
		strcpy(warning, "5 MISSED R-PEAKS\n");
		break;

	case 0:
		strcpy(warning, "None\n");
		break;

	default:
		strcpy(warning, "ERROR\n");
	}
	//-------------------



	//ASSEMBLE DISPLAY---------------------------
	strcpy(message, "************************\n");

	sprintf(line,"Last R-peak: %d ms ago\n",lastPeak);
	strcat(message,line);

	sprintf(line,"R-peak value: %d\n",peakValue);
	strcat(message,line);

	sprintf(line,"Pulse: %d\n\n",pulse);
	strcat(message,line);

	strcat(message,"Warnings:\n");
	strcat(message,warning);

	strcat(message, "************************\n");
	//-------------------------------------------

	return message;
}

