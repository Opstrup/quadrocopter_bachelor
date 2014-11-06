#ifndef DistanceSensor
#include <avr/io.h>
#include "Arduino.h"

class DistanceSensor
{
public:	
	DistanceSensor();
	long getDistance();
	
// 	long microsecondsToCentimeters(long microseconds)
// 	{
// 		return microseconds / 74 / 2;
// 	}
		
private:

	int trigPin;
	int echoPin;
	long duration;
	long cm;
	
	};

#endif