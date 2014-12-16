/*
 * WayPointsHandler.h
 *
 * Created: 11-11-2014 12:31:39
 *  Author: Kevin
 */ 


#ifndef WAYPOINTSHANDLER_H_
#define WAYPOINTSHANDLER_H_
#include "GetAndPut.h"
#include "waypoint.h"

class waypointsHandler
{
	
public:
	waypointsHandler();
	waypoint* getWayPoints(String request);

private:
	String _request;
	waypoint createWayPoint(char* lat,char* longi,char* height,char* takePhoto);
	};



#endif /* WAYPOINTSHANDLER_H_ */