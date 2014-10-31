#ifndef sendAT
#include <avr/io.h>
#include "Arduino.h"

class ATcommand
{
public:
	int ATComSend(char* ATcommand, char* expected_answer, unsigned int timeout);
	

private:

	uint8_t x;
	uint8_t answer;
	char response[100];
	unsigned long previous;
	char * ATcommand_;
	char * expected_answer_;
	unsigned int timeout_;

};
#endif