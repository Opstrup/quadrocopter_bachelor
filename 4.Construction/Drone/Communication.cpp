#include "Communication.h"

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
		delay(50000);
		_eHandler.putResetNextEvent(requestDrone, _put_latitude, _put_longitude);	
		return wpObject;
	}
	delay(40000);
	return NULL;
	
}
void communication:: putDroneStatus( float latitude, float longitude)
{
	_put_longitude = longitude;
	_put_latitude = latitude;
	String request = "/api/drones/1/?format=json";
	_eHandler.setOnlineLocation(request,_put_latitude,_put_longitude);
}
void communication:: initialGet()
{
	String request = "/api/drones/1/?format=json";
	int nextEvent = _eHandler.getNextEvent(request);
}
int communication :: putURL()
{
	char* _url = "iha-11726.iha.dk";
	int port = 80;
	char aux_str[100];
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
	Serial.println(aux_str);
	
	return 1;
	
}

