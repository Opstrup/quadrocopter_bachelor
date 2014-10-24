#include <avr/io.h>
#include "Arduino.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "sendATCommand.h"

standAloneGps :: standAloneGps()
{
	initGPS();
	getGPS();
}

void standAloneGps :: initGPS()
{
	Serial.println("Start of GPS setup");
	delay(2000);
	//standAlonemode_ = sendATcommand("AT+CGPS=1,1","OK",2000);
	ATcommand sendAT;
	
	Version = "AT+CGPS=1,1";
	expected_answer = "OK";
	waitTime = 2000;
	//
	standAlonemode_ = sendAT.ATComSend(Version,expected_answer,waitTime);	

	if (standAlonemode_ != 1)
	{
		Serial.println("Cannot set GPS mode 1.");
	}
	else
	{
		Serial.println("CGPSINFO okay!");
	}
}

char* standAloneGps :: getGPS()
{
	//char gps_data[100];
	//char* p_gps_data[sizeof(gps_data)];
	ATcommand sendAT;
	getGPS_ = sendAT.ATComSend("AT+CGPSINFO", "+CGPSINFO:",1000);
	int counter;
	if (getGPS_ == 1)
	{
		
		counter = 0;
		do{
			while(Serial.available() == 0);
			gps_data[counter] = Serial.read();
			counter++;
		}
		while(gps_data[counter - 1] != '\r');
		gps_data[counter] = '\0';
		
		if(gps_data[0] == ',')
		{
			Serial.println("Ingen GPS data - section 1.");
		}
		else
		{
			Serial.println("Der er data.");
		}
		
		while (gps_data[0] == ',')
		{
			getGPS_ = sendAT.ATComSend("AT+CGPSINFO", "+CGPSINFO:",1000);
			do{
				while(Serial.available() == 0);
				gps_data[counter] = Serial.read();
				counter++;
			}
			while(gps_data[counter - 1] != '\r');
			gps_data[counter] = '\0';
			Serial.println("Ingen data.");
			delay(3000);
		}
		
		
		Serial.print("GPS data:");
		Serial.println(gps_data);
		Serial.println("");
		
		
	}
	else
	{
		Serial.println("Could not start GPSINFO.");
	}
	
	return gps_data;
	//p_gps_data[sizeof(gps_data)];
	
	//return p_gps_data;
	
	char * longitude;
	char * latitude;
	char * NorthSouth;
	char * WestEast;
	
	latitude = strtok (gps_data,",");
	NorthSouth = strtok (NULL, ",");
	longitude = strtok (NULL, ",");
	WestEast = strtok (NULL, ",");
	
	
	Serial.println(latitude);
	Serial.println(NorthSouth);
	Serial.println(longitude);
	Serial.println(WestEast);
}