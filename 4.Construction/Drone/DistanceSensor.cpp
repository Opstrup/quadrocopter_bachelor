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
	for (int x = 0; x<10;x++)
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
	
	cm_return = cm_return/10;
	return cm_return;
}
long DistanceSensor :: microsecondsToCentimeters(long microseconds)
{
	return microseconds / 29 / 2;
}



/*


long data[ARRAY_SIZE] = {10,12,30,9,11,34,10,11,9,13};

for (int fejl = 0; fejl < ARRAY_SIZE; fejl++)
{
	Serial.print(data[fejl]);
	Serial.print(" : ");
}
Serial.println(" ");


int antalfejl = 0;
int fejlved[32];
int plads = 0;
for (int y = 0;y < ARRAY_SIZE ;y++)
{
	for (int z = 0;z < ARRAY_SIZE;z++)
	{
		if (!(data[y] < (data[z]+10) && data[y] > (data[z]-10)))
		{
			fejlved[plads] = z;
			antalfejl++;
			plads++;
		}
		
	}
	
	
}

for (int t = 0; t<antalfejl;t++)	{
	data[fejlved[t]] = 10;
}

for (int fejl = 0; fejl < ARRAY_SIZE; fejl++)
{
	Serial.print(data[fejl]);
	Serial.print(" : ");
}
cm_return = cm_return/ARRAY_SIZE;
Serial.println("");



delay(200);
*/