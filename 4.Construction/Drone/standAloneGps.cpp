#include <avr/io.h>
#include "Arduino.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "sendATCommand.h"
#include "sendAT.h"

standAloneGps :: standAloneGps()
{
}

void standAloneGps :: initGPS()
{
	Serial.println("Start of GPS setup");
	delay(2000);
	standAlonemode_ = sendATcommand("AT+CGPS=1,1","OK",1000);

	if (standAlonemode_ != 1)
	{
		Serial.println("Cannot set GPS mode 1.");
	}
	else
	{
		Serial.println("CGPSINFO okay!");
	}
}

void standAloneGps :: updateGPSPosition()
{
	getGPS_ = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
	int counter;
	if (getGPS_ == 1)
	{
		do
		{
			getGPS_ = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
			counter = 0;
			do{
				while(Serial.available() == 0);
				gps_data[counter] = Serial.read();
				counter++;
			}
			while(gps_data[counter - 1] != '\r');
			gps_data[counter] = '\0';
			Serial.println("Ingen data.");
			delay(1200);
			
		}while (gps_data[0] == ',');

	}
	else
	{
		Serial.println("Could not start GPSINFO.");
	}
	
	char gps_data2[100];
	for (int x = 0; x< sizeof(gps_data);x++)
	{
		gps_data2[x] = gps_data[x];
	}
	
	latitude = strtok (gps_data2,",");
	longitude = strtok (NULL, ",");
	longitude = strtok (NULL, ",");

	float_latitude =atof(latitude);
	float_longitude =atof(longitude);
	
	int_latitude = int(float_latitude);
	int_longitude = int(float_longitude);
	
}

float standAloneGps :: getLatitude()
{
	float_latitude = float_latitude/100;
	int_latitude = int(float_latitude);
	float_latitude = (float_latitude -(float_latitude - int_latitude))+(((float_latitude - int_latitude)*100)/60);
 	return float_latitude;
}

float standAloneGps :: getLongitude()
{
	float_longitude = float_longitude/100;
	int_longitude = int(float_longitude);
	float_longitude = (float_longitude -(float_longitude - int_longitude))+(((float_longitude - int_longitude)*100)/60);
	return float_longitude;
}