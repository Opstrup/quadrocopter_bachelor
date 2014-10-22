#ifndef http

#include <avr/io.h>
#include "Arduino.h"

#pragma once

class http
{
public:
	http();
	char* get_http(char request[]);
	//void post_http();
private:
	
	int8_t http_answer;
	int x, aux, http_y;
	int data_size;
	char aux_str[100];
	char data[192];
	char* request_;
	
//page info:
// 	int port;
// 	char url_[21];
 	
	
	//char http_data[32];
	};

#endif