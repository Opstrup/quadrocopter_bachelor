#include <avr/io.h>
#include "Arduino.h"
#include "Setup.h"
#include "flightControl.h"
#include "DistanceSensor.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "http_functions.h"
#include "sendAT.h"
#include "aJSon.h"
#include "stringbuffer.h"
#include "3G_Module.h"
#include "GetAndPut.h"
#include "EventHandler.h"
#include "WayPointsHandler.h"
#include "Communication.h"

#define THROTTLE		OCR1B	// pin 12
#define YAW				OCR1A	// pin 11
#define PITCH			OCR4B	// pin 7
#define ALTTITUDEHOLD	OCR4A	// pin 6
#define ROLL			OCR3A	// pin 5
#define FLIGHTMODE		OCR3B	// pin 2

void adjustHeight(float minFlyHeight, float maxFlyHeight);
void flyForward(bool heightFLAG, bool bearingFLAG);
void adjustBearing(float currLAT, float currLONG, float waypoint_1_LAT, float waypoint_1_LONG, bool heightFLAG);
void init3G_GPS_Module();


flightControl drone;
standAloneGps GPSInfo;
DistanceSensor distSensor(46,48);
getAndput gap;
communication com;
waypoint* waypoints;


bool bearingFLAG = 0, heightFLAG = 0;
int RightHeightCounter = 0;

int main()
{
	bool all_waypoint_reached = 0, waypoint_1_reached = 0, waypoint_2_reached = 0, waypoint_3_reached = 0, startingpoint_reached = 0; 
	float waypoint_1_LAT = 0, waypoint_1_LONG = 0, minFlyHeight, maxFlyHeight, distanceToWaypoint = 20;
	float startingLAT, startingLONG, currLAT = 0, currLONG = 0; 
	long millisCount = 0, puttimer, requestDelay, httpWait;
	int NextEvent = 0,firstrun;
	
	init();
	init3G_GPS_Module();
	
	// Init GPS module
	GPSInfo.initGPS();

	for (int i = 0; i< 10;i++)
	{
		GPSInfo.updateGPSPosition();
	}
	startingLAT = GPSInfo.getLat();
	startingLONG = GPSInfo.getLong();
	
	com.initialGet();
	delay(40000);
	
	int values = com.putURL();
	delay(4000);
	
	com.putDroneStatus(startingLAT,startingLONG);
	delay(40000);
	puttimer = millis();
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
	
	// Drone is ready for takeoff.
	
				
	drone.initMotors();
	drone.armMotors();
	THROTTLE = 22000;
	minFlyHeight = 10;
	maxFlyHeight = 40;
	
	while(all_waypoint_reached != true)
	{
		millisCount = millis();
		
		while(waypoint_1_reached != true)
		{
			
			while(drone.checkIfControllerIsOn() == true)	// Check for controller
			{
					
			}
						
// 			GPSInfo.updateGPSPosition();
// 			currLAT = GPSInfo.getLat();
// 			currLONG = GPSInfo.getLong();
			waypoint_1_LAT = waypoints[0].getlat();
			waypoint_1_LONG = waypoints[0].getlong();
 			distanceToWaypoint = drone.calDistToTarget(startingLAT, startingLONG, waypoint_1_LAT, waypoint_1_LONG);
		
			if(distanceToWaypoint < 15)
			{
				waypoint_1_reached = true;
			}
		
			else
			{			
				
				adjustHeight(minFlyHeight, maxFlyHeight);
				heightFLAG = 1;
 				adjustBearing(startingLAT, startingLONG, waypoint_1_LAT, waypoint_1_LONG, heightFLAG);	

// 				flyForward(heightFLAG, bearingFLAG);			
// 				
// 				if(millis() - millisCount > 40000)
// 				{
// 					communication.put_isOnline_CurrPos(currLAT, currLONG);
// 					millisCount = millis();
// 				}

			}
		}
		
		all_waypoint_reached = true;
	}	
}       
                                                                     


void adjustHeight(float minFlyHeight, float maxFlyHeight)
{
	long currFlyHeight, newFlyHeight;
	
	currFlyHeight = distSensor.getDistance();
	Serial.print("Current FlyHeight in cm:  ");
	Serial.println(currFlyHeight);
	Serial.println();
	
	if( currFlyHeight < minFlyHeight)
	{
		Serial.println("FlyHeight < minFlyHeight");
		Serial.println(); 
		
		drone.throttle(2);
		
		heightFLAG = 0;
	}
	
	else if(currFlyHeight > maxFlyHeight)
	{
		Serial.println("FlyHeight > maxFlyHeight");
		Serial.println();
		
		drone.throttle(-2);

// 		if (THROTTLE < 23170)
// 		{
// 			THROTTLE = 23200;
// 		}
		
		heightFLAG = 0;
	}
	
	else
	{
		delay(1000);
		newFlyHeight = distSensor.getDistance();
		
		Serial.print("Forskel paa currFlyHeight og newFlyHeight =  ");
		Serial.println(currFlyHeight - newFlyHeight);
		Serial.println();
		
		if(currFlyHeight - newFlyHeight > 5)
		{
			drone.throttle(1);
			Serial.println("Faaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaalder");
			Serial.println();
		}
		
		else if(currFlyHeight - newFlyHeight < -5)
		{
			drone.throttle(-1);
			Serial.println("Gaar for huuuuuuuuuuuuuuuuuuurtigt");
			Serial.println();
		}
		
		else
		{
			heightFLAG = 1;
			RightHeightCounter ++;
		}
	}
}

void adjustBearing(float currLAT, float currLONG, float waypoint_1_LAT, float waypoint_1_LONG, bool heightFLAG)
{
	float targetBearing, currBearing;
	
	targetBearing = drone.calBearingToTarget(currLAT, currLONG, waypoint_1_LAT, waypoint_1_LONG);
	
	Serial.print("Target Bearing ");
	Serial.println(targetBearing);
	Serial.println();
	
	currBearing = drone.getBearingFromCompas();
	Serial.print("Current Bearing ");
	Serial.println(currBearing);
	Serial.println();
	
	
	
	if (targetBearing < 11)
	{
		targetBearing = targetBearing + 10;
		
		currBearing = currBearing + 10;
		
		if (currBearing > 359)
		{
			currBearing = currBearing - 360;
		}
	}
	
	if (targetBearing > 349)
	{
		targetBearing = targetBearing - 10;
		
		currBearing = currBearing - 10;
		
		if (currBearing < 0 )
		{
			currBearing = currBearing + 360;
		}
	}
	
	if(heightFLAG == 0)
	{
		Serial.println("Height need to be adjusted before bearing");
		Serial.println();
	}
	
	else if(currBearing < targetBearing + 10 && currBearing > targetBearing - 10 && heightFLAG == 1 )
	{
		Serial.println("Current bearing is in accepted range");
		Serial.println();
		bearingFLAG = 1;
	}
	
	else if((currBearing >= targetBearing + 10 || currBearing <= targetBearing - 10) && heightFLAG == 1)
	{
		Serial.println("Difference in bearing is more then 10 degrees. ");
		Serial.println("Adjusting bearing");
		Serial.println();
		
		drone.yaw(12);
		delay(200);
		drone.yaw(-12);
		bearingFLAG = 0;
	}
}

void flyForward(bool heightFLAG, bool bearingFLAG)
{
	if( heightFLAG == 1 && bearingFLAG == 1)
	{
		Serial.println("Using pitch to fly forward");
		Serial.println();
		
		drone.pitch(48);
		delay(200);
		drone.pitch(-48);
	}
	
	if( heightFLAG != 1 || bearingFLAG != 1)
	{
		Serial.println("Height and bearing need to be adjusted before using pitch");
		Serial.println();
	}
}

void init3G_GPS_Module()
{
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
		while(answer == 0)
		{
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