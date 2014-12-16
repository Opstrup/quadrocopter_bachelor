#ifndef flightControl
#include <avr/io.h>
#include "Arduino.h"

#pragma once

class flightControl
{
public:	
	flightControl();
	
	boolean checkIfControllerIsOn();
	
	float getBearingFromCompas();
	float calDistToTarget(float current_lat, float current_long, float target_lat, float target_long);
	float calBearingToTarget(float current_lat, float current_long, float target_lat, float target_long);
	
	void initMotors();
	void calibrateMotors();
	
	void setPWM(int change_value, String mode);
	void armMotors();
	void disarmMotors();
		
	void throttle(int);
	void yaw(int);
	void pitch(int);
	void roll(int);
	void flightmode(int);
	void alttitudehold(int);
	
private:
	
};


#endif