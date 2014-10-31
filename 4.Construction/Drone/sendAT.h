#include <avr/io.h>
#include "Arduino.h"

int8_t sendATcommand(char* ATcommand, char* expected_answer, unsigned int timeout);
int8_t sendATcommand2(char* ATcommand, char* expected_answer1,
char* expected_answer2, unsigned int timeout);