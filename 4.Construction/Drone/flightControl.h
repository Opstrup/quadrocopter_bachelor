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
	char* getGPS();
	
	
	
	private:
		
		int standAlonemode_;
	 	int getGPS_;
	 	char gps_data[100];
		//char* p_gps_data[sizeof(gps_data)];
 		char * Version;
	 	char * expected_answer;
	 	int waitTime;
	};


#endif