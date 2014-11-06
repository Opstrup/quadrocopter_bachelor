/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
//#include "http_get.h"
#include "standAloneGps.h"
#include "GPS.h"
//#include "assistedGPS.h"
#include "http_functions.h"
#include "DistanceSensor.h"
#include "flightControl.h"
#include "sendAT.h"
#include "aJSon.h"
#include "stringbuffer.h"
#include "3G_Module.h"

void init3G_GPS_Module();

int main()
{
	//char gpsdata[100];
	
	init();
	init3G_GPS_Module();

	
// 	float longitude, latitude;
// 	//http_get();
// 	standAloneGps standAloneGPS1;
// 	standAloneGPS1.initGPS();
// 	standAloneGPS1.updateGPSPosition();
// 	
// 	latitude = standAloneGPS1.getLat();
// 	longitude = standAloneGPS1.getLong();
// 	
// 	
// 	Serial.println("GPS coordinates");
// 	Serial.println(longitude,10);
// 	Serial.println(latitude,10);
	
// 	float curr_height;
// 	DistanceSensor Height(53,51);
//  	flightControl Drone;
// 	
	//Drone.initPWM();
	 
	//Drone.armMotors();
	
	//Drone.initPWM();

	
	
	

//   	char* http_gps = http_f.get_http("GET /api/drones/1/?format=json HTTP/1.1\r\nHost: iha-11726.iha.dk\r\nContent-Length: 0\r\n\r\n");
// // // 	
//  	Serial.println("\n\n\nGPS coordinates from the web are:");
//  	Serial.println(http_gps);

	float latitude, longitude;
	
	char * flightdata;
	Module_3G getandput;
	
	flightdata = getandput.get_FlightSetup();
	Serial.println(flightdata);
	
	delay(40000);

	flightdata = getandput.get_FlightSetup();
	Serial.println(flightdata);
// 	standAloneGps standAloneGPS1;
// 	standAloneGPS1.initGPS();
// 	standAloneGPS1.updateGPSPosition();
// 	
// 	latitude = standAloneGPS1.getLat();
// 	longitude = standAloneGPS1.getLong(); 
// 	
	

    while(1)
    {
		
		//Serial.println(Height.getDistance());
// 		while ((Height.getDistance()) < 10)
// 		{
// 			Drone.throttle(1);
// 			Serial.println(Height.getDistance());
// 			
// 			//Serial.println(OCR1B);
// 		}
// 		while ((Height.getDistance()) > 20)
// 		{
// 			Drone.throttle(-1);
// 			Serial.println(Height.getDistance());
// 		}
		
		//
		
		//distance = nysensor.getDistance();
		//Serial.println(distance);
		//loop();

        //TODO:: Please write your application code 
    }
}


void init3G_GPS_Module(){
	int8_t answer;
	int onModulePin= 2;
	pinMode(onModulePin, OUTPUT);
	Serial.begin(115200);
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
