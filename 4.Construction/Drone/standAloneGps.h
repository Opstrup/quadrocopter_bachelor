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
		
	float getLong();
	float getLat();
	
	
	
	private:
		float float_latitude;
		float float_longitude;
	};


#endif