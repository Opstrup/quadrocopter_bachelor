/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
#include "Setup.h"


//Prototypes:



int8_t answer;
int onModulePin= 2;


void setup_board(){

	
	pinMode(onModulePin, OUTPUT);
	Serial.begin(115200);
	
	Serial.println("Starting...");
	power_on();
	
	delay(3000);
	
	// sets the PIN code
	//sendATcommand("AT+CPIN=6932", "OK", 2000);
	
	//delay(3000);
	
	Serial.println("Connecting to the network...");

	while( (sendATcommand("AT+CREG?", "+CREG: 0,1", 500) ||
	sendATcommand("AT+CREG?", "+CREG: 0,5", 500)) == 0 );
	
	Serial.println("Connected!");	
	
	// sets APN, user name and password
	sendATcommand("AT+CGSOCKCONT=1,\"IP\",\"internet\"", "OK", 2000);
	sendATcommand("AT+CSOCKAUTH=1,1,\"user_name\",\"password\"", "OK", 2000);
}

void power_on(){

	uint8_t power=0;

	// checks if the module is started
	power = sendATcommand("AT", "OK", 2000);
	if (power == 0)
	{
		// power on pulse
		digitalWrite(onModulePin,HIGH);
		delay(3000);
		digitalWrite(onModulePin,LOW);

		// waits for an answer from the module
		while(power == 0){
			// Send AT every two seconds and wait for the answer
			power = sendATcommand("AT", "OK", 2000);
		}
	}
	
}

void loop()
{
		
}