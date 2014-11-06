#include "DistanceSensor.h"

DistanceSensor::DistanceSensor()
{
// 	trigPin = tpin;
// 	echoPin = epin;
}

long DistanceSensor::getDistance()
{
	//duration = dur;
	pinMode(trigPin, OUTPUT);
	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	
	pinMode(echoPin, INPUT);
	duration = pulseIn(echoPin, HIGH);
	// convert the time to cm.
	cm = microsecondsToCentimeters(duration);
	
	return cm;	
}