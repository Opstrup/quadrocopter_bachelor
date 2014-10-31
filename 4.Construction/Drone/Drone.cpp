/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
#include "Setup.h"
//#include "http_get.h"
#include "standAloneGps.h"
#include "GPS.h"
//#include "assistedGPS.h"
#include "http_functions.h"


int main()
{
	//char gpsdata[100];
	
	init();
	
	setup_board();

	//http_get();
	
	
// 	http http_f;
// 	char* http_gps = http_f.get_http("GET /test-get-post.php?counter=1&data=2 HTTP/1.1\r\nHost: pruebas.libelium.com\r\nContent-Length: 0\r\n\r\n");
// 	
// 	Serial.println("\n\n\nGPS coordinates from the web are:");
// 	Serial.println(http_gps);
	
  	standAloneGps standAloneGPS1;
 	standAloneGPS1.initGPS();
		 
		 

	 
	
	standAloneGPS1.updateGPSPosition();

	Serial.println("\n\n\nGPS coordinates:");

	
	Serial.println("Latitude:");
	Serial.println(standAloneGPS1.getLatitude(),10);
	
	Serial.println("Longitude:");
	Serial.println(standAloneGPS1.getLongitude(),10);


    while(1)
    {
		
		
		//loop();

        //TODO:: Please write your application code 
    }
}
