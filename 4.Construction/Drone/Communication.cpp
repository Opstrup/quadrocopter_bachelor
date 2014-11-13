/*
 * Communication.cpp
 *
 * Created: 12-11-2014 12:05:21
 *  Author: Kevin
 */ 
#include "Communication.h"

// communication :: communication(getAndput gap, eventHandler eHandler,waypointsHandler wpHandler)
// {
//  	_gap = gap;
// 	_wpHandler = wpHandler;
// 	_eHandler = eHandler;
// }

waypoint* communication::getwayPoints()
{
	String requestDrone= "/api/drones/1/?format=json";
	String requestWayPoint = "/api/waypointsForEvent/";
	
	int _NextEvent;
	_NextEvent = _eHandler.getNextEvent(requestDrone);

	requestWayPoint = requestWayPoint + _NextEvent + "/";
	if (_NextEvent > 0)
	{
		delay(40000);
		wpObject = _wpHandler.getWayPoints(requestWayPoint);
		//delay(40000);
		_nEvent = 0;
		//_eHandler.putResetNextEvent(requestDrone, _latitude, _longitude, _nEvent);	
		return wpObject;
	}
	//delay(40000);
	return NULL;
	
}
void communication:: putDroneStatus( float latitude, float longitude)
{
	_put_longitude = longitude;
	__put_latitude = latitude;
	String request = "/api/drones/1/?format=json";
	_eHandler.setOnlineLocation(request,__put_latitude,_put_longitude);
}
void communication:: initialGet()
{
	String request = "/api/drones/1/?format=json";
	int nextEvent = _eHandler.getNextEvent(request);
}
