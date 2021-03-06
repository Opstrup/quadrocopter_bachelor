﻿#include <avr/io.h>
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
	int _ATanswer;
	Serial.println("Start of GPS setup");
	delay(2000);
	_ATanswer = sendATcommand("AT+CGPS=1,1","OK",1000);

	if (_ATanswer != 1)
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
	int _ATanswer;
	char gps_data[100];
	_ATanswer = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
	int counter;
	if (_ATanswer == 1)
	{
		do
		{
			_ATanswer = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
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
	char * latitude;
	char* longitude;
	latitude = strtok (gps_data2,",");
	longitude = strtok (NULL, ",");
	longitude = strtok (NULL, ",");

	float_latitude =atof(latitude);
	float_longitude =atof(longitude);
	
	int int_latitude = int(float_latitude);
	int int_longitude = int(float_longitude);
	
}

float standAloneGps :: getLat()
{
	float_latitude = float_latitude/100;
	int int_latitude = int(float_latitude);
	float_latitude = (float_latitude -(float_latitude - int_latitude))+(((float_latitude - int_latitude)*100)/60);
 	return float_latitude;
}

float standAloneGps :: getLong()
{
	float_longitude = float_longitude/100;
	int int_longitude = int(float_longitude);
	float_longitude = (float_longitude -(float_longitude - int_longitude))+(((float_longitude - int_longitude)*100)/60);
	return float_longitude;
}