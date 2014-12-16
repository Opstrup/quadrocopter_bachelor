/*
 * Drone.cpp
 *
 * Created: 06-10-2014 08:25:04
 *  Author: Kevin
 */ 


#include <avr/io.h>
#include "Arduino.h"
#include "sendAT.h"


int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout){

	uint8_t x=0,  answer=0;
	char response[100];
	unsigned long previous;

	memset(response, '\0', 100);    // Initialice the string
	
	delay(100);
	
	while( Serial.available() > 0) Serial.read();    // Clean the input buffer
	
	Serial.println(ATcommand);    // Send the AT command


	x = 0;
	previous = millis();

	// this loop waits for the answer
	do{
		// if there are data in the UART input buffer, reads it and checks for the asnwer
		if(Serial.available() != 0){
			response[x] = Serial.read();
			x++;
			// check if the desired answer is in the response of the module
			if (strstr(response, expected_answer) != NULL)
			{
				answer = 1;
			}
		}
		// Waits for the asnwer with time out
	}while((answer == 0) && ((millis() - previous) < timeout));

	return answer;
}

int8_t sendATcommand2(char* ATcommand, char* expected_answer1,
char* expected_answer2, unsigned int timeout){

	 uint8_t x=0,  answer=0;
	 char response[100];
	 unsigned long previous;

	 memset(response, '\0', 100);    // Initialize the string

	 delay(100);

	 while( Serial.available() > 0) Serial.read();    // Clean the input buffer

	 Serial.println(ATcommand);    // Send the AT command


	 x = 0;
	 previous = millis();

	 // this loop waits for the answer
	 do{

		 if(Serial.available() != 0){
			 response[x] = Serial.read();
			 x++;
			 // check if the desired answer is in the response of the module
			 if (strstr(response, expected_answer1) != NULL)
			 {
				 answer = 1;
			 }
			 // check if the desired answer is in the response of the module
			 if (strstr(response, expected_answer2) != NULL)
			 {
				 answer = 2;
			 }
		 }
		 // Waits for the asnwer with time out
	 }
	 while((answer == 0) && ((millis() - previous) < timeout));

	 return answer;
}