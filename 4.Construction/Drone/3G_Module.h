#ifndef Module_3G

#include <avr/io.h>
#include "Arduino.h"
#include "sendAT.h"
#include "aJSon.h"
#include "stringbuffer.h"

#pragma once

class Module_3G
{
public:

	Module_3G(char* url,String requestDrone, String requestWaypoints);		
	int get_FlightSetup();
	void put_isOnline_CurrPos(float longitude, float latitude);
	
	void sortWayPoints(int wayPointNumber);
	float getLong();
	float getLat();
	float getHeight();
	char* getTakePhoto();
	
	
private:
	// PUT and GET attributes
	char data[700];
	char * bodyEvent;
	char* bodyWaypoints;
	int numberOfWaypoints = 0;
	int allowModify;
	
	//For the contructor
	char* _url;
	String _requestDrone;
	String _requestWaypoints;
	
	aJsonObject* sortJSON();	
	int _id = 0;
	int _nextEvent = 0;

	char* _is_online;
	char* _model;
	int setNextEvent;
	
	aJsonObject* findNumberOfWayPoints();
	String safeWayPoints[10];
	char* _takePhoto;
	float _height;
	float _latitude;
	float _longitude;
	
};


#endif