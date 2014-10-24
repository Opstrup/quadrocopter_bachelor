#ifndef MotorControl
#include <avr/io.h>
#include "Arduino.h"

class MotorControl
{
public:

	void armMotors(int);
	int setPWM();
	void calMotors(int);
	void throttle(int);
	void yaw(int);
	void pitch(int);
	void roll(int);
	

private:	

	int pin_;
	
	};




#endif