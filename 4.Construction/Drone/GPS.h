#ifndef GPS
#include <avr/io.h>
#include "Arduino.h"
#pragma once

class GPS
{
public:
	virtual void initGPS();
	virtual char* getGPS();
private:
};	
#endif