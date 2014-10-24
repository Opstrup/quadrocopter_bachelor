#ifndef standAloneGps
#include <avr/io.h>
#include "Arduino.h"
#include "GPS.h"

#pragma once

class standAloneGps : public GPS
{
public:	
	standAloneGps();
	void initGPS();
	void updateGPSPosition();
		
	float getLongitude();
	float getLatitude();
	
	
	
	private:
		
		int standAlonemode_;
	 	int getGPS_;
	 	char gps_data[100];
		char* longitude;
		char* latitude;
		float float_latitude;
		float float_longitude;
		int int_latitude;
		int int_longitude;
	};


#endif