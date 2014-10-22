/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
#include "Setup.h"
#include "http_get.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "assistedGPS.h"
#include "http_functions.h"


int main()
{
	//char gpsdata[100];
	
	init();
	
	setup_board();
	
	
	//GPS StandaloneGPS;
	//StandaloneGPS.standAlonemode();
	
	
		
	//http_get();
	
	
	http http_f;
	char* http_gps = http_f.get_http("GET /test-get-post.php?counter=1&data=2 HTTP/1.1\r\nHost: pruebas.libelium.com\r\nContent-Length: 0\r\n\r\n");
	
	Serial.println("\n\n\nGPS coordinates from the web are:");
	Serial.println(http_gps);
	
 	standAloneGps sagps;
 	sagps.initGPS();
 	char* gpsinfo = sagps.getGPS();
	Serial.println("\n\n\nGPS coordinates from the GPS are:");
	Serial.println(gpsinfo);
	
// 	
// 	Serial.println("data fra drone filen.");
// 	Serial.println(gpsinfo);
// 	//Serial.println(gpsdata);
	
	
	/*
	Serial.println("assisted gps loades:");
	assistedGPS assistedModeGPS;
	assistedModeGPS.initGPS();
	
	char * gpsinfo = assistedModeGPS.getGPS();
	Serial.println(gpsinfo);	
	*/
	
	
	
	
	
	
    while(1)
    {
		
		
		//loop();
		
		//getStandAGPS();
		//loop();
        //TODO:: Please write your application code 
    }
}
