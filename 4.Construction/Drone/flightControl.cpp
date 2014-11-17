#include <avr/io.h>
#include <string.h>
#include "Arduino.h"
#include "flightControl.h"

#define THROTTLE		OCR1B	// pin 12
#define YAW				OCR1A	// pin 11
#define PITCH			OCR4B	// pin 7
#define ALTTITUDEHOLD	OCR4A	// pin 6
#define ROLL			OCR3A	// pin 5
#define FLIGHTMODE		OCR3B	// pin 2

flightControl :: flightControl()
{
	
}

boolean flightControl :: checkIfControllerIsOn()
{
	bool ControllerIsOn = 0;
	int pin = 53;
	pinMode(pin, OUTPUT);
	
	char *Roll, *Pitch, *Yaw, *Throttle, *Mode, *AUX1, *AUX2, *AUX3;
	char currentAUX1[100];
	int counter = 0;
	
	// send command to flightcontrolboard "vehicle attitude"
	Serial1.println('t');
	
	counter = 0;
	do{
		// wait for available data
		while(Serial1.available() == 0);
		
		// save data in currentBearing
		currentAUX1[counter] = Serial1.read();
		counter++;
	}
	while(currentAUX1[counter - 1] != '\r');
	currentAUX1[counter] = '\0';
	
	Serial.println("Read Processed Transmitter Values");
	Serial.println(currentAUX1);
	Serial.println();
	
	// vehicle attitude data looks like: "12.1,27.3,1.4\r\n"
	// We are only interested in the last part.
	Roll = strtok (currentAUX1,",");
	Pitch = strtok (NULL, ",");
	Yaw = strtok (NULL, ",");
	Throttle = strtok (NULL, ",");
	Mode = strtok (NULL, ",");
	AUX1 = strtok (NULL, ",");
	AUX2 = strtok (NULL, ",");
	
	// convert bearing from radian to degrees
	float AUX2_Value = atof(AUX2);
	
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.println();
	Serial.print("AUX2 Value: ");
	Serial.println(AUX2);
	Serial.println();
	
	//AUX2_Value = 1000;
		
	if(AUX2_Value < 1500)
	{
		ControllerIsOn = true;
		digitalWrite(pin, HIGH);
		Serial.println("Drone styres nu manuelt");
		Serial.println();
	}
	
	else if(AUX2_Value > 1500)
	{
		ControllerIsOn = false;
		digitalWrite(pin, LOW);
		Serial.println("Drone styres af arduino");
		Serial.println();
	}
	
	return ControllerIsOn;
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
	
	//Serial.println(Z);
		
	// convert bearing from radian to degrees
	float bearing = atof(Z);
	bearing = bearing / 0.0175;
	
	Serial.println(bearing);
	Serial.println();
	
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

void flightControl :: initMotors()
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
	YAW = 24000;
	THROTTLE = 16000;
	
	
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
	ROLL = 24000-120;
	FLIGHTMODE = 24000;
	
	
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
	ALTTITUDEHOLD = 24000;
	PITCH = 24000 + 240;
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
				tempint = THROTTLE;
				if(tempint + 40 <= 32000)
				{
					THROTTLE = tempint + 40;
					tempint = THROTTLE;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = THROTTLE;
				if(tempint - 40 >= 16000)
				{
					THROTTLE = tempint - 40;
					tempint = THROTTLE;
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
				tempint = YAW;
				if(tempint + 40 <= 32000)
				{
					YAW = tempint + 40;
					tempint = YAW;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = YAW;
				if(tempint - 40 >= 16000)
				{
					YAW = tempint - 40;
					tempint = YAW;
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
				tempint = PITCH;
				if(tempint + 40 <= 32000)
				{
					PITCH = tempint + 40;
					tempint = PITCH;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = PITCH;
				if(tempint - 40 >= 16000)
				{
					PITCH = tempint - 40;
					tempint = PITCH;
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
				tempint = ROLL;
				if(tempint + 40 <= 32000)
				{
					ROLL = tempint + 40;
					tempint = ROLL;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = ROLL;
				if(tempint - 40 >= 16000)
				{
					ROLL = tempint - 40;
					tempint = ROLL;
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
				tempint = FLIGHTMODE;
				if(tempint + 40 <= 32000)
				{
					FLIGHTMODE = tempint + 40;
					tempint = FLIGHTMODE;
				}
			}
		}
			
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = FLIGHTMODE;
				if(tempint - 40 >= 16000)
				{
					FLIGHTMODE = tempint - 40;
					tempint = FLIGHTMODE;					
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
				tempint = ALTTITUDEHOLD;
				if(tempint + 40 <= 32000)
				{
					ALTTITUDEHOLD = tempint + 40;
					tempint = ALTTITUDEHOLD;
				}
			}
		}
		
		else if(change_value < 0)
		{
			for(int i = 0; i > change_value; i--)
			{
				tempint = ALTTITUDEHOLD;
				if(tempint - 40 >= 16000)
				{
					ALTTITUDEHOLD = tempint - 40;
					tempint = ALTTITUDEHOLD;
				}
			}
		}
	}
	
	Serial.print("New value : ");
	Serial.println(tempint);
	Serial.println();
		
}

void flightControl :: armMotors()
{
	Serial.println("Arm motors");
	Serial.println();
	
	THROTTLE = 16000;
	YAW = 32000;
	
	delay(100);
	
	THROTTLE = 32000;	
	YAW = 24000;
}

void flightControl :: disarmMotors()
{
	Serial.println("Disarm motors");
	
	THROTTLE = 16000;
	YAW = 16000;
		
	delay(100);
	
	THROTTLE = 16000;
	YAW = 24000;
}

void flightControl :: calibrateMotors()
{
	Serial.println("Calibrate motors");
	
	// Calibrate left side - throttle and yaw
	THROTTLE = 32000;
	YAW = 16000;
	delay(1000);
	
	THROTTLE = 32000;
	YAW = 32000;
	delay(1000);
	
	THROTTLE = 16000;
	YAW = 32000;
	delay(1000);
	
	THROTTLE = 16000;
	YAW = 16000;
	delay(1000);
			
	THROTTLE = 24000;
	YAW = 24000;
	delay(1000);
		
		
	// Calibrate right side - pitch and roll
	PITCH = 32000;
	ROLL = 16000;
	delay(1000);
	
	PITCH = 32000;
	ROLL = 32000;
	delay(1000);
	
	PITCH = 16000;
	ROLL = 32000;
	delay(1000);
	
	PITCH = 16000;
	ROLL = 16000;
	delay(1000);
	
	PITCH = 24000;
	ROLL = 24000;
	delay(1000);
	
	
	// Calibrate flightmode and alttitudehold
	FLIGHTMODE = 32000;
	delay(1000);
	FLIGHTMODE = 16000;
	delay(1500);
	FLIGHTMODE = 24000;
	delay(1000);
	
	ALTTITUDEHOLD = 32000;
	delay(1000);
	ALTTITUDEHOLD = 16000;
	delay(1000);
	ALTTITUDEHOLD = 24000;
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