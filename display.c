#include "display.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int frequency = 250; //samples per second

int cycle2ms(int cycle){
	int raw = cycle/250;
	int filtered = raw + 25 + 80 + 10 + 73;
	return filtered;
}

int ms2cycle(int ms){
	int raw = ms - 25 - 80 - 10 - 73;
	int seconds = raw*250;
	return seconds;
}

int pulse(int *Rs, int size){
	int sum = 0;
	for (int i = 0; i < size; i++){
		sum += Rs[i];
	}
	return sum/size;
}

char* display(int lastPeak, int peakValue, int pulse, char warningCode){
	char warning[60];
	switch(warningCode){
	case 'B':
		strcpy(warning, "LOW R-PEAK VALUE\n5 MISSED R-PEAKS\n");
		break;

	case 'L':
		strcpy(warning, "LOW R-PEAK VALUE\n");
		break;

	case 'M':
		strcpy(warning, "5 MISSED R-PEAKS\n");
		break;

	case 'N':
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

