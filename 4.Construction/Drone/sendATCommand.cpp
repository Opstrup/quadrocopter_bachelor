#include "sendATCommand.h"

int ATcommand::ATComSend(char* ATcommand, char* expected_answer, unsigned int timeout)
{
	expected_answer_ = expected_answer;
	timeout_ = timeout;
	memset(response, '\0', 100);
	
	delay(1000);
	
	while( Serial.available() > 0) Serial.read();
	
	ATcommand_ = ATcommand;
	Serial.println(ATcommand_);
	
	x = 0;
	previous = millis();
	// this loop waits for the answer
	do{
		// if there are data in the UART input buffer, reads it and checks for the asnwer
		if(Serial.available() != 0){
			response[x] = Serial.read();
			x++;
			// check if the desired answer is in the response of the module
			if (strstr(response, expected_answer_) != NULL)
			{
				answer = 1;
			}
		}
		// Waits for the asnwer with time out
	}while((answer == 0) && ((millis() - previous) < timeout_));

	return answer;
}