/*
 * WayPointsHandler.cpp
 *
 * Created: 11-11-2014 12:33:09
 *  Author: Kevin
 */ 
#include "WayPointsHandler.h"


waypointsHandler::waypointsHandler()
{
	
}

waypoint* waypointsHandler::getWayPoints(String request)
{
	getAndput _getAndput;
	_request = request;
	char* waypoints;
	
	
	int numberOfWaypoints = 0;
	
	waypoints = _getAndput.getMethod(_request);
	
	int w = 0;
	String strBodyWayPoints = waypoints;

		
	if (strBodyWayPoints[0] == '[')
	{
		while (strBodyWayPoints[w] != ']')
		{
			if (strBodyWayPoints[w] == '{')
				{
					numberOfWaypoints++;
				}
				w++;
			}
	}
	else
	{
		numberOfWaypoints = 1;
	}
	char* trash;
	String devide_waypoints;
	waypoint strwayPoints[numberOfWaypoints];
	Serial.println(numberOfWaypoints);
	devide_waypoints = strtok (waypoints,"{");
	
		if (numberOfWaypoints > 1)
		{
			
			//int i = 0;
			//while (i < numberOfWaypoints)
			for(int i = 0; i <numberOfWaypoints; i++)
			{
				devide_waypoints = strtok (NULL,"}");
				devide_waypoints = "{" + devide_waypoints + "}";		
				Serial.println(devide_waypoints);	
				char wp[500];

				devide_waypoints.toCharArray(wp,500);	
				//JSON
				devide_waypoints = strtok (NULL,"{");	
				aJsonObject* waypoints_object = aJson.parse(wp);
				char* _latitude_char = aJson.print(aJson.getObjectItem(waypoints_object, "latitude"));
				char* _longitude_char = aJson.print(aJson.getObjectItem(waypoints_object, "longitude"));
				
				char* _height_char = aJson.print(aJson.getObjectItem(waypoints_object, "height"));					
				char* _takePhoto = aJson.print(aJson.getObjectItem(waypoints_object, "take_photo"));
				waypoint aWayPoint;
				
				delay(1000);
 				aWayPoint = createWayPoint(_latitude_char,_longitude_char,_height_char,_takePhoto);
 				Serial.println(aWayPoint.getheight());
				strwayPoints[i] = aWayPoint;

			
				
				
				
				aJson.deleteItem(waypoints_object);
				
	
			}
		}
		else if (numberOfWaypoints == 1)
		{
			Serial.println("numberofWaypoints = 1");
// 			devide_waypoints = strtok (NULL,"}");
// 			devide_waypoints = devide_waypoints + "}";
// 			strwayPoints[0] = devide_waypoints;
		}
	return strwayPoints;
}

waypoint waypointsHandler::createWayPoint(char* lat,char* longi,char* height,char* takePhoto)
{
	
	waypoint returnwaypoint;
	returnwaypoint.setlat(lat);
	returnwaypoint.setlong(longi);
	returnwaypoint.setheight(height);
	returnwaypoint.setphoto(takePhoto);
	return returnwaypoint;
	
	
}