/*
 * waypoint.cpp
 *
 * Created: 11-11-2014 15:12:37
 *  Author: Kevin
 */ 
#include "waypoint.h"


waypoint::waypoint()
{
	
}

void waypoint::setlat(char* lat)
{

	_latitude_char = lat;
}
void waypoint::setlong(char* longi)
{

	_longitude_char = longi;
}
void waypoint::setheight(char* height)
{
	_height = atof(height);
}
void waypoint::setphoto(char* photo)
{
	_takePhoto = photo;
}
float waypoint::getlat()
{
	for (int y = 0; y < 1;y++ )
	{
		_latitude_char = strtok(_latitude_char,"\"");
	}
	return _latitude = atof(_latitude_char);
}
float waypoint::getlong()
{
	for (int y = 0; y < 1;y++ )
	{
		_longitude_char = strtok(_longitude_char,"\"");
	}
	return _longitude = atof(_longitude_char);
}
float waypoint::getheight()
{
	
	return _height;
}
char* waypoint::getphoto()
{
	return _takePhoto;
}