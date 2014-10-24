#include <avr/io.h>
#include <string.h>
#include "Arduino.h"
#include "flightControl.h"

flightControl :: flightControl()
{
	
}

float flightControl :: getBearingFromCompas()
{
	char *X, *Y, *Z;
	char currentBearing[100];
	int counter = 0;
	
	// send command to flightcontrolboard "vehicle attitude"
	Serial1.println('r');
		
	counter = 0;
	do{
		// wait for available data
		while(Serial1.available() == 0);
		
		// save data in currentBearing
		currentBearing[counter] = Serial1.read();	
		counter++;
	}
	while(currentBearing[counter - 1] != '\r');
	currentBearing[counter] = '\0';
	
	// vehicle attitude data looks like: "12.1,27.3,1.4\r\n"
	// We are only interested in the last part.
	X = strtok (currentBearing,",");
	Y = strtok (NULL, ",");
	Z = strtok (NULL, ",");	
		
	// convert bearing from radian to degrees
	float bearing = atof(Z);
	bearing = bearing / 0.0175;
	
	return bearing;
}

float flightControl :: calDistToTarget(float current_lat, float current_long, float target_lat, float target_long)
{
	float dist, a, b, jord_radius = 6371000;
	float lat1, lat2, dlat, lon1, lon2, dlon;
		
	lat1 = current_lat*(3.14/180);
	lat2 = target_lat*(3.14/180);
	
	lon1 = current_long*(3.14/180);
	lon2 = target_long*(3.14/180);
	
	dlat = lat2 - lat1;
	dlon = lon2 - lon1;
	
	a = sin(dlat/2)*sin(dlat/2)+sin(dlon/2)*sin(dlon/2)*cos(lat1)*cos(lat2);
	b = 2*atan2(sqrt(a),sqrt(1-a));
	
	dist = jord_radius * b;
	
	return dist;
}

float flightControl :: calBearingToTarget(float current_lat, float current_long, float target_lat, float target_long)
{
	float bearing, x, y;
	float lat1, lat2, dlat, lon1, lon2, dlon;
	
	lat1 = current_lat*(3.14/180);
	lat2 = target_lat*(3.14/180);
	
	lon1 = current_long*(3.14/180);
	lon2 = target_long*(3.14/180);
	
	dlat = lat2 - lat1;
	dlon = lon2 - lon1;
	
	x = sin(dlon)*cos(lat2);
	y = cos(lat1)*sin(lat2)-sin(lat1)*cos(lat2)*cos(dlon);
	
	bearing = (atan2(x,y))*(180/PI);
		
	if(bearing <0)
	{
		bearing = bearing + 360;
	}
	
	return bearing;
}