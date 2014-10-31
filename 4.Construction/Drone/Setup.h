#include <avr/io.h>
#include "Arduino.h"
//#include "sendATCommand.h"
#include "sendAT.h"

void setup_board();
void power_on();
int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout);