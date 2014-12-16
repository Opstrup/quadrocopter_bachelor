#include "GPS.h"
#include "sendAT.h"
#include <string.h>
#include "sendATCommand.h"




// 
// void GPS ::standAlonemode()
// {
// 	Serial.println("Start of GPS setup");
// 	delay(2000);
// 	//standAlonemode_ = sendATcommand("AT+CGPS=1,1","OK",2000);
// 	
//  	ATcommand sendAT;
// 	Version = "AT+CGPS=1,1";
// 	aReturn = "OK";
//  	waitTime = 2000;
// // 	
//  	standAlonemode_ = sendAT.ATComSend(Version,aReturn,waitTime);
// // 	
// 	Serial.println("After sendAT class");
// 	
// 	
// 	if (standAlonemode_ != 1)
// 	{
// 		Serial.println("Cannot set GPS mode 1.");
// 	}
// 	else
// 	{
// 		Serial.println("CGPSINFO okay!");
// 	}
// 	
// }
// 
// void GPS ::getGPS()
// {
//  	char gps_data[100];
// 
// 	getGPS_ = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
// 	int counter;
// 	if (getGPS_ == 1)
// 	{
// 		
// 		counter = 0;
// 		do{
// 			while(Serial.available() == 0);
// 			gps_data[counter] = Serial.read();
// 			counter++;
// 		}
// 		while(gps_data[counter - 1] != '\r');
// 		gps_data[counter] = '\0';
// 		
// 		if(gps_data[0] == ',')
// 		{
// 			Serial.println("Ingen GPS data - section 1.");
// 		}
// 		else
// 		{
// 			Serial.println("Der er data.");
// 		}
// 		
// 		while (gps_data[0] == ',')
// 		{
// 			getGPS_ = sendATcommand("AT+CGPSINFO", "+CGPSINFO:",1000);
// 			do{
// 				while(Serial.available() == 0);
// 				gps_data[counter] = Serial.read();
// 				counter++;
// 			}
// 			while(gps_data[counter - 1] != '\r');
// 			gps_data[counter] = '\0';
// 			Serial.println("Ingen data.");
// 			delay(3000);
// 		}
// 		
// 		
// 		Serial.print("GPS data:");
// 		Serial.println(gps_data);
// 		Serial.println("");
// 		
// 		
// 	}
// 	else
// 	{
// 		Serial.println("Could not start GPSINFO.");
// 	}
// 	
// 	char * longitude;
// 	char * latitude;
// 	char * NorthSouth;
// 	char * WestEast;
// 	
// 	latitude = strtok (gps_data,",");	
// 	NorthSouth = strtok (NULL, ",");
// 	longitude = strtok (NULL, ",");
// 	WestEast = strtok (NULL, ",");
// 		
// 	
// 	Serial.println(latitude);
// 	Serial.println(NorthSouth);
// 	Serial.println(longitude);
// 	Serial.println(WestEast);
// 	
// 	
// }