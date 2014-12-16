/*
 * waypoint.h
 *
 * Created: 11-11-2014 15:08:13
 *  Author: Kevin
 */ 


#ifndef WAYPOINT_H_
#define WAYPOINT_H_

#include <avr/io.h>
#include "Arduino.h"
#include <string.h>

class waypoint
{
	
public:
	waypoint();
	float getlat();
	float getlong();
	float getheight();
	char* getphoto();
	
	void setlat(char* lat);
	void setlong(char* longi);
	void setheight(char* height);
	void setphoto(char* photo);
	
private:

	char* _latitude_char;
	char* _longitude_char;
	float _latitude;
	float _longitude;
	float _height;
	char* _takePhoto;

	};



#endif /* WAYPOINT_H_ */