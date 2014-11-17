/*
 * Communication.h
 *
 * Created: 12-11-2014 11:45:00
 *  Author: Kevin
 */ 



#include "WayPointsHandler.h"
#include "EventHandler.h"
#include "GetAndPut.h"
#include "Arduino.h"
#include <avr/io.h>

class communication
{
	
public:
//	communication(getAndput gap, eventHandler eHandler,waypointsHandler wpHandler);
	waypoint* getwayPoints();
	void putDroneStatus(float latitude, float longitude);
	void initialGet();
	int putURL();
private:

	
	waypoint* wpObject;
	getAndput _gap;
	waypointsHandler _wpHandler;
	eventHandler _eHandler;
	float _put_latitude;
	float _put_longitude;
	int _nEvent;
};



