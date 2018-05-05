/*
 * radiation.h
 *
 *  Created on: 21.04.2018
 *      Author: Jonathan
 */

#ifndef RADHAND_H_
#define RADHAND_H_
#include "Arduino.h"



class RadHand {

public:


	RadHand(uint16_t deadTimeInput, uint16_t conversionFactor,uint32_t intervals[]);

	void service(uint32_t milliseconds);
	void event();
	uint32_t calculateCPM(uint32_t cpm, uint32_t time);

	bool checkUpdate(uint8_t id);


	float calculateDoseRateSi(uint32_t cpm);
	float calculateDoesRateUs(uint32_t cpm);
	uint32_t calculateRealCPM(uint32_t cpm);
	uint32_t getCPM(uint8_t id);


	uint32_t currentCPM[];
	uint32_t totalPulseCount;
	uint8_t progress[];
	uint8_t  arrLen;


private:
	uint16_t convFactor;
	uint16_t deadTime;
	uint32_t pulseCount[];
	uint32_t timeInterval[];
	uint32_t lastConversion[];


	bool updated[];

};







#endif /* RADHAND_H_ */
