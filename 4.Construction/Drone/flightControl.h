#ifndef flightControl
#include <avr/io.h>
#include "Arduino.h"

#pragma once

class flightControl
{
public:	
	flightControl();
	float getBearingFromCompas();
	float calDistToTarget(float current_lat, float current_long, float target_lat, float target_long);
	float calBearingToTarget(float current_lat, float current_long, float target_lat, float target_long);
	
private:
	
};


#endif