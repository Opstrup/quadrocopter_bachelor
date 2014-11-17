/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "http_functions.h"
#include "DistanceSensor.h"
#include "flightControl.h"
#include "sendAT.h"
#include "aJSon.h"
#include "stringbuffer.h"
#include "3G_Module.h"
#include "GetAndPut.h"
#include "EventHandler.h"
#include "WayPointsHandler.h"
#include "Communication.h"

void init_3G_GPS_Module();

getAndput gap;
communication com;
waypoint* waypoints;
standAloneGps standAloneGPS1;


int main()
{
	float startingLAT, startingLONG;
	long puttimer, requestDelay, httpWait;
	int firstrun;
	
	init();
	init_3G_GPS_Module();
	standAloneGPS1.initGPS();

	 for (int i = 0; i< 10;i++)
	 {
		 standAloneGPS1.updateGPSPosition();
	 }
	startingLAT = standAloneGPS1.getLat();
	startingLONG = standAloneGPS1.getLong(); 
	
	com.initialGet();
	delay(40000);
	
	int values = com.putURL();
	delay(4000);
	
	com.putDroneStatus(startingLAT,startingLONG);
	delay(40000);
	puttimer millis();
	do 
	{
		
		
		if (millis()-puttimer > 300000)
		{
			int values = com.putURL();
			delay(4000);
			com.putDroneStatus(startingLAT,startingLONG);
			puttimer = millis();
		}
		// check for nextevent, if nextevent == 0 - stay in loop
		waypoints = com.getwayPoints();
	} while (waypoints == NULL);

	

// 		float lat = waypoints[1].getlat();
// 		float longi = waypoints[1].getlong();
// 		float height = waypoints[1].getheight();
// 		char* photo = waypoints[1].getphoto();
// 		Serial.println("test");
// 		Serial.println(lat,10);
// 		Serial.println(longi,10);
// 		Serial.println(height);
// 		Serial.println(photo);
	
	firstrun = 0;
	
	Serial.println("Drone is ready for takeoff");

	
//	aJson.deleteItem(put_resetevent);
	requestDelay = millis();
	httpWait = millis();
	
    while(1)
    {
		if ((millis() -  httpWait > 40000))
		{
			firstrun = com.putURL();
			requestDelay = millis();
			httpWait = millis();
		}
		
		
		if ((millis() -  requestDelay > 3000) && firstrun == 1)
		{
				firstrun = 0;
				com.putDroneStatus(40,10);
				requestDelay = millis();
		}
    }
}


void init_3G_GPS_Module(){
	int8_t answer;
	int onModulePin= 2;
	pinMode(onModulePin, OUTPUT);
	Serial.begin(115200);
	Serial1.begin(115200);
	Serial.println("Starting...");
	answer = sendATcommand("AT", "OK", 2000);
	if (answer == 0)
	{
		// power on pulse
		digitalWrite(onModulePin,HIGH);
		delay(3000);
		digitalWrite(onModulePin,LOW);

		// waits for an answer from the module
		while(answer == 0){
			// Send AT every two seconds and wait for the answer
			answer = sendATcommand("AT", "OK", 2000);
		}
	}
	
	delay(3000);

	Serial.println("Connecting to the network...");

	while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
	sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
	
	Serial.println("Connected!");
	// sets APN, user name and password
	sendATcommand("AT+CGSOCKCONT=1,\"IP\",\"internet\"", "OK", 2000);
	sendATcommand("AT+CSOCKAUTH=1,1,\"user_name\",\"password\"", "OK", 2000);
}
