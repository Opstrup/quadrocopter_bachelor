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

void flightControl :: initPWM()
{
	Serial.println("InitPWM");
	
	// INIT TIMER_1
	pinMode(11, OUTPUT);
	pinMode(12, OUTPUT);
	
	// Amount of steps in counter
	ICR1 = 0xFFFF;
	
	// Timer/Counter is defined by 
	TCCR1A = 0xAA;
	TCCR1B = 0x19;
	TCCR3C = 0x00;
	
	// At what step output a and b should change
	OCR1A = 24000;
	OCR1B = 24000;
	
	
	// INIT TIMER_3
	pinMode(5, OUTPUT);
	pinMode(2, OUTPUT);
	
	// Amount of steps in counter
	ICR3 = 0xFFFF;
	
	// Timer/Counter is defined by 
	TCCR3A = 0xAA;
	TCCR3B = 0x19;
	TCCR3C = 0x00;

	// At what step output a and b should change
	OCR3A = 24000;
	OCR3B = 24000;
	
	
	// INIT TIMER_4
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	
	// Amount of steps in counter
	ICR4 = 0xFFFF;
	
	// Timer/Counter is defined by 
	TCCR4A = 0xAA;
	TCCR4B = 0x19;
	TCCR3C = 0x00;
	
	// At what step output a and b should react
	OCR4A = 24000;
	OCR4B = 24000;
	
	delay(2500);
}

void flightControl :: setPWM(int change_value, String mode)
{
	int tempint;
	Serial.print("Change value: ");
	Serial.println(change_value);
	Serial.print("Mode: ");
	Serial.println(mode);
	
	if(mode == "Throttlecontrol")
	{			
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR1B;
				if(tempint + 160 <= 32000)
				{
					OCR1B = tempint + 160;
					tempint = OCR1B;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR1B;
				if(tempint - 160 >= 16000)
				{
					OCR1B = tempint - 160;
					tempint = OCR1B;
				}
			}
		}
	}
	
	else if(mode == "Yawcontrol")
	{			
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR1A;
				if(tempint + 160 <= 32000)
				{
					OCR1A = tempint + 160;
					tempint = OCR1A;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR1A;
				if(tempint - 160 >= 16000)
				{
					OCR1A = tempint - 160;
					tempint = OCR1A;
				}
			}
		}
	}
		
	else if(mode == "Pitchcontrol")
	{
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR4B;
				if(tempint + 160 <= 32000)
				{
					OCR4B = tempint + 160;
					tempint = OCR4B;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR4B;
				if(tempint - 160 >= 16000)
				{
					OCR4B = tempint - 160;
					tempint = OCR4B;
				}
			}
		}
	}
		
	else if(mode == "Rollcontrol")
	{
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR3A;
				if(tempint + 160 <= 32000)
				{
					OCR3A = tempint + 160;
					tempint = OCR3A;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR3A;
				if(tempint - 160 >= 16000)
				{
					OCR3A = tempint - 160;
					tempint = OCR3A;
				}
			}
		}
	}
		
	else if(mode == "Flightmodecontrol")
	{		
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR3B;
				if(tempint + 160 <= 32000)
				{
					OCR3B = tempint + 160;
					tempint = OCR3B;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR3B;
				if(tempint - 160 >= 16000)
				{
					OCR3B = tempint - 160;
					tempint = OCR3B;					
				}
			}
		}
	}
		
	else if(mode == "Alttitudeholdcontrol")
	{		
		if(change_value > 0)
		{
			for(int i = 0; i < change_value; i++)
			{
				tempint = OCR4A;
				if(tempint + 160 <= 32000)
				{
					OCR4A = tempint + 160;
					tempint = OCR4A;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = OCR4A;
				if(tempint - 160 >= 16000)
				{
					OCR4A = tempint - 160;
					tempint = OCR4A;
				}
			}
		}
	}
	
	Serial.print("New value : ");
	Serial.println(tempint);
	Serial.println();
	Serial.println();
		
}

void flightControl :: armMotors()
{
	Serial.println("Arm motors");
	
	while(OCR1B > 16001 || OCR1A < 31999)
	{
		if(OCR1B > 16001)
		{
			throttle(-1);
		}
		
		if(OCR1A < 31999)
		{
			yaw(1);
		}
		
		delay(20);
	}
	
}

void flightControl :: disarmMotors()
{
	Serial.println("Disarm motors");
	
	while(OCR1B > 16001 || OCR1A > 16001)
	{
		if(OCR1B > 16001)
		{
			throttle(-1);
		}
		
		if(OCR1A > 16001)
		{
			yaw(-1);
		}
		
		delay(20);
	}
	
}

void flightControl :: calibrateMotors()
{
	Serial.println("Calibrate motors");
	
	// 	throttle		OCR1B
	// 	yaw				OCR1A
	// 	pitch			OCR4B
	// 	roll			OCR3A
	// 	flightmode		OCR3B
	// 	alttitudehold 	OCR4A
	
	
	// Calibrate left side - throttle and yaw
	OCR1B = 16000;
	OCR1A = 16000;
	delay(1000);
	
	OCR1B = 16000;
	OCR1A = 32000;
	delay(1000);
	
	OCR1B = 32000;
	OCR1A = 32000;
	delay(1000);
	
	OCR1B = 32000;
	OCR1A = 16000;
	delay(1000);
	
	OCR1B = 24000;
	OCR1A = 24000;
	delay(1000);
		
	// Calibrate right side - pitch and roll
	OCR4B = 16000;
	OCR3A = 16000;
	delay(1000);
	
	OCR4B = 16000;
	OCR3A = 32000;
	delay(1000);
	
	OCR4B = 32000;
	OCR3A = 32000;
	delay(1000);
	
	OCR4B = 32000;
	OCR3A = 16000;
	delay(1000);
	
	OCR4B = 24000;
	OCR3A = 24000;
	delay(1000);
	
	// Calibrate flightmode and alttitudehold
	OCR3B = 32000;
	delay(1000);
	OCR3B = 16000;
	delay(1500);
	OCR3B = 24000;
	delay(1000);
	
	OCR4A = 32000;
	delay(1000);
	OCR4A = 16000;
	delay(1000);
	OCR4A = 24000;
	
}

void flightControl :: throttle(int value_)
{
	String Throttlecontrol = "Throttlecontrol";
	setPWM(value_, Throttlecontrol);
}

void flightControl :: yaw(int value_)
{
	String Yawcontrol = "Yawcontrol";
	setPWM(value_, Yawcontrol);
}

void flightControl :: pitch(int value_)
{
	String Pitchcontrol = "Pitchcontrol";
	setPWM(value_, Pitchcontrol);
}

void flightControl :: roll(int value_)
{
	String Rollcontrol = "Rollcontrol";
	setPWM(value_, Rollcontrol);
}

void flightControl :: flightmode(int value_)
{
	String Flightmodecontrol = "Flightmodecontrol";
	setPWM(value_, Flightmodecontrol);
}

void flightControl :: alttitudehold(int value_)
{
	String Alttitudeholdcontrol = "Alttitudeholdcontrol";
	setPWM(value_, Alttitudeholdcontrol);
}