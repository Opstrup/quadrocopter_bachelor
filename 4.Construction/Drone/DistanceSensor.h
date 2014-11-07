#ifndef DistanceSensor
#include <avr/io.h>
#include "Arduino.h"

class DistanceSensor
{
public:	
	DistanceSensor(int tpin,int epin);
	long getDistance();
	
	long microsecondsToCentimeters(long microseconds);
	
		
private:

	int trigPin;
	int echoPin;
	long duration;
	long cm;
	long cm_return;
	
	char totalData[];
	
	};

#endif