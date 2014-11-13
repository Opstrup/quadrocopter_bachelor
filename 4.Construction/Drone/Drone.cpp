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

int main()
{
	//char gpsdata[100];
	
	init();
//	Serial.begin(115200);
//	Serial1.begin(115200);
	init_3G_GPS_Module();


	
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

// 	float latitude, longitude;
// 	

	//Serial.println(wayPoints);

// 	flightdata = getandput.get_FlightSetup();
// 	Serial.println(flightdata);

// 	delay(40000);
// 
// 	flightdata = getandput.get_FlightSetup();
// 	Serial.println(flightdata);
// 	standAloneGps standAloneGPS1;
// 	standAloneGPS1.initGPS();
// 	standAloneGPS1.updateGPSPosition();
// 	
// 	latitude = standAloneGPS1.getLat();
// 	longitude = standAloneGPS1.getLong(); 
//
 	
	getAndput gap;
	String drone_request = "/api/waypointsForEvent/1/";
	waypointsHandler wphand;
	waypoint* onewp;
	
	communication com;
 	com.initialGet();
 	delay(40000);
	com.putDroneStatus(60,8);
	delay(40000);
	
	do 
	{
		onewp = com.getwayPoints();
	} while (onewp == NULL);

	

		float lat = onewp[1].getlat();
		float longi = onewp[1].getlong();
		float height = onewp[1].getheight();
		char* photo = onewp[1].getphoto();
		Serial.println("test");
		Serial.println(lat,10);
		Serial.println(longi,10);
		Serial.println(height);
		Serial.println(photo);

	Serial.println("done");
	
	

/*
waypoint strwayPoints[3];
char* trash;
String devide_waypoints;
char* waypoints = "[{\"id\": 1, \"latitude\": \"56.17214933633902\", \"longitude\": \"10.191493034362793\", \"height\": 2, \"take_photo\": true, \"event_id\": 1}, {\"id\": 2, \"latitude\": \"56.171838752735496\", \"longitude\": \"10.19106388092041\", \"height\": 1, \"take_photo\": false, \"event_id\": 1}, {\"id\": 3, \"latitude\": \"56.17251964427522\", \"longitude\": \"10.189926624298096\", \"height\": 3, \"take_photo\": true, \"event_id\": 1}]";
Serial.println(waypoints);
devide_waypoints = strtok (waypoints,"{");
	
	int i = 0;
	while (i < 3)
	{
	devide_waypoints = strtok (NULL,"}");
	devide_waypoints = "{" + devide_waypoints + "}";
	char wp[150];
	Serial.println(devide_waypoints);
	devide_waypoints.toCharArray(wp,150);
	//JSON
	
	aJsonObject* waypoints_object = aJson.parse(wp);
	
	char* _latitude_char = aJson.print(aJson.getObjectItem(waypoints_object, "latitude"));
	char* _longitude_char = aJson.print(aJson.getObjectItem(waypoints_object, "longitude"));
	char* _takePhoto = aJson.print(aJson.getObjectItem(waypoints_object, "take_photo"));
	float _height = atof(aJson.print(aJson.getObjectItem(waypoints_object, "height")));
	//wayPoints[i] = devide_waypoints;
	aJson.deleteItemFromObject(waypoints_object,"latitude");
	aJson.deleteItem(waypoints_object);
	
	devide_waypoints = strtok (NULL,"{");
		i++;
	
	}
*/


    while(1)
    {
		
/*		drone.checkIfControllerIsOn();*/
		
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


void init_3G_GPS_Module(){
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
