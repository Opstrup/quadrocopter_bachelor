#include "DistanceSensor.h"

#define ARRAY_SIZE 10

DistanceSensor::DistanceSensor(int tpin,int epin)
{
	trigPin = tpin;
	echoPin = epin;
}

long DistanceSensor::getDistance()
{
	cm_return = 0;
	for (int x = 0; x<1;x++)
	{
		pinMode(trigPin, OUTPUT);
		digitalWrite(trigPin, LOW);
		delayMicroseconds(2);
		digitalWrite(trigPin, HIGH);
		delayMicroseconds(10);
		digitalWrite(trigPin, LOW);
		
		// Read the signal from the sensor: a HIGH pulse whose
		// duration is the time (in microseconds) from the sending
		// of the ping to the reception of its echo off of an object.
		pinMode(echoPin, INPUT);
		duration = pulseIn(echoPin, HIGH);
		
		// convert the time into a distance
		cm = microsecondsToCentimeters(duration);
		cm_return = cm_return + cm;
	}
	
	cm_return = cm_return;//10;
	return cm_return;
}

long DistanceSensor :: microsecondsToCentimeters(long microseconds)
{
	return microseconds / 29 / 2;
}