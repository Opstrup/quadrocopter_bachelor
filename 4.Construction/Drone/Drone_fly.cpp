#include <avr/io.h>
#include "Arduino.h"
#include "Setup.h"
#include "flightControl.h"
#include "DistanceSensor.h"
#include "standAloneGps.h"
#include "GPS.h"
#include "3G_Module.h"
#include "http_functions.h"

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
Module_3G communication;

bool bearingFLAG = 0, heightFLAG = 0;
int RightHeightCounter = 0;

int main()
{
	bool all_waypoint_reached = 0, waypoint_1_reached = 0, waypoint_2_reached = 0, waypoint_3_reached = 0, startingpoint_reached = 0; 
	float waypoint_1_LAT = 0, waypoint_1_LONG = 0, minFlyHeight, maxFlyHeight, distanceToWaypoint = 20;
	float startingLAT, startingLONG, currLAT = 0, currLONG = 0; 
	long millisCount = 0;
	int NextEvent = 0 ;
	
	init();
	
	Serial.begin(115200);
	Serial1.begin(115200);
	
	delay(5000);
				
	drone.initMotors();
	drone.armMotors();
	
	minFlyHeight = 100;
	maxFlyHeight = 200;
	
	while(all_waypoint_reached != true)
	{
		millisCount = millis();
		
		while(waypoint_1_reached != true)
		{
			
			while(drone.checkIfControllerIsOn() == true)
			{
					
			}
						
// 			GPSInfo.updateGPSPosition();
// 			currLAT = GPSInfo.getLat();
// 			currLONG = GPSInfo.getLong();
// 			distanceToWaypoint = drone.calDistToTarget(currLAT, currLONG, waypoint_1_LAT, waypoint_1_LONG);
		
			if(distanceToWaypoint < 15)
			{
				waypoint_1_reached = true;
			}
		
			else
			{			
				Serial.print("RightHeightCounter =  ");
				Serial.println(RightHeightCounter);
				Serial.println();
				
				adjustHeight(minFlyHeight, maxFlyHeight);					
								
// 				adjustBearing(currLAT, currLONG, waypoint_1_LAT, waypoint_1_LONG, heightFLAG);	
// 				
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
		
		drone.throttle(3);
		
		heightFLAG = 0;
	}
	
	else if(currFlyHeight > maxFlyHeight)
	{
		Serial.println("FlyHeight > maxFlyHeight");
		Serial.println();
		
		drone.throttle(-1);
		
		heightFLAG = 0;
	}
	
	else
	{
		delay(200);
		newFlyHeight = distSensor.getDistance();
		
		Serial.print("Forskel på currFlyHeight og newFlyHeight =  ");
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
	
	currBearing = drone.getBearingFromCompas();
	
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
		Serial.println("Current bearing is perfect");
		Serial.println();
		bearingFLAG = 1;
	}
	
	else if((currBearing >= targetBearing + 10 || currBearing <= targetBearing - 10) && heightFLAG == 1)
	{
		Serial.println("Adjusting bearing");
		Serial.println();
		
		drone.yaw(48);
		delay(200);
		drone.yaw(-48);
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
