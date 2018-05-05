/*
 * radiation.cpp
 *
 *  Created on: 21.04.2018
 *      Author: Jonathan
 */
#include "radhand.h"



RadHand::RadHand(uint16_t deadTimeInput, uint16_t conversionFactor, uint32_t intervals[]){

	deadTime = deadTimeInput;
	convFactor = conversionFactor;
	arrLen = sizeof(intervals)/sizeof(uint32_t);

	for(uint8_t i=0; i<arrLen; i++ ){
		timeInterval[i] = intervals[i];
		lastConversion[i] = 0;
		currentCPM[i] = 0;
		progress[i] = 0;
		updated[i] = false;
		pulseCount[i] = 0;

	}




	totalPulseCount = 0;




}

//****************************************************
//Calculates the dose rate eg 0.1uSv/h from a given
//CPM value (uSv/h is the SI unit)
//****************************************************

float RadHand::calculateDoseRateSi(uint32_t cpm){

	//calculates the dose Rate in SI-Unit (uSv/h);

	float pulseCount = cpm * 1.00;						//convert CPM to a float type
	float cps = pulseCount / 60;						//calculate CPS from CPM
	float dt = deadTime / 1000000.0;
	float realcpm = (cps / (1- (cps* dt)) *60);	//adjust cpm count with dead time correction factor
														//without correction display is wrong 0.9% (LND712) at 5000cpm and 11% at 80k CPM!!

	float doseRate = realcpm / convFactor;				//calculate Sievert value from CPM

	return doseRate;
}

//****************************************************
//Calculates the dose rate eg 1uR/h from a given
//CPM value (uR/h is the american unit)
//****************************************************

float RadHand::calculateDoesRateUs(uint32_t cpm){

	//calculates the dose Rate in US unit (uR/h)
	//1 R/h =  2.77777777777778E-06 Sv/h
	//0.1uSv/h = 10.00008 uR/h  :)

	float doseRate = calculateDoseRateSi(cpm) * 10.00008;
	return doseRate;

}

//****************************************************
//Calculates the true CPM value based on dead time
//For LND712 the values will be 1-15% better than raw
//****************************************************

uint32_t RadHand::calculateRealCPM(uint32_t cpm){

	float pulseCount = cpm * 1.00;						//convert CPM to a float type
	float cps = pulseCount / 60;						//calculate CPS from CPM
	float realcpm = (cps / (1- (cps* deadTime)) *60);	//adjust cpm count with dead time correction factor
														//without correction display is wrong 0.9% (LND712) at 5000cpm and 11% at 80k CPM!!

	uint32_t returncpm = realcpm;
	return returncpm;

}


//****************************************************
//Returns the real cpm value of the current CPM reading
//
//****************************************************


uint32_t RadHand::getCPM(uint8_t id){
	return calculateRealCPM(currentCPM[id]);
}





uint32_t RadHand::calculateCPM(uint32_t pulseCnt,uint32_t time){

	float cpmvar = (pulseCnt*60000.0)/ (time * 1.0);
	return  cpmvar;
}






//****************************************************
//This is called when a pulse of the GM tube arrives
//it will register the pulse to pulseCount variable
//*****************************************************

void RadHand::event(){
	for(uint8_t i=0; i<arrLen;i++){
	pulseCount[i]=pulseCount[i]+1;
	}
}


void RadHand::service(uint32_t milliseconds){


	for(uint8_t i=0; i<arrLen;i++){

		if(milliseconds >= (lastConversion[i]+timeInterval[i])){
			currentCPM[i] = calculateCPM(pulseCount[i], timeInterval[i]);
			pulseCount[i]=0;
			lastConversion[i] = milliseconds;
			updated[i] = true;
		}

	uint32_t time_gone = milliseconds-lastConversion[i];
	progress[i] = float(100.0/timeInterval[i])*time_gone;

	}



}


bool RadHand::checkUpdate(uint8_t id){
	if(updated[id] == true){
		updated[id]= false;
		return true;
	}
	return false;
}
