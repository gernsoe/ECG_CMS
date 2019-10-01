#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int frequency = 250; //samples per second

int cycle2ms(int cycle){
	int filtered = cycle*1000/250;
	int raw = filtered + 25 + 80 + 10 + 73;
	return raw;
}

int ms2cycle(int ms){
	int cycle = ms + 25 + 80 + 10 + 73;
	return cycle*250;
}

int pulse(int *Rs, int size){
	int sum = Rs[size-1]-Rs[0];
	/*for (int i = 1; i < size; i++){
		sum += Rs[i];
	}*/
	//sum = sum/250; //converts period of cycles to period of time

	sum = 250*60*size/sum; //number of R-peaks per minute
	return sum;
}

char* display(int lastPeak, int peakValue, int pulse, int warningCode){
	char warning[40];
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
	char *message;
	message = malloc(250);
	strcpy(message, "************************\n");
	char line[26];
	sprintf(line,"Last R-peak: %d ms ago\n",lastPeak);
	strcat(message,line);

	sprintf(line,"R-peak value: %d\n",peakValue);
	strcat(message,line);

	sprintf(line,"Pulse: %d\n\n",pulse);
	strcat(message,line);

	strcat(message,"Warnings:\n");
	strcat(message,warning);
	strcat(message, "************************\n");

	return message;
}

