#ifndef Module_3G

#include <avr/io.h>
#include "Arduino.h"
#include "sendAT.h"
#include "aJSon.h"
#include "stringbuffer.h"

#pragma once

class Module_3G
{
public:

	Module_3G();		
	char* get_FlightSetup();
	//void get_Nextevent();
	void put_isOnline_CurrPos(float longitude, float latitude);
	
	
private:
	char aux_str[100];
	char data[420];
	char * body_data;
	char data_v2[500];
	char data_body[50];
	char* body1;
	char * body;
	
	aJsonObject* sortJSON();
	char* _id_char;
	int _id = 0;
	char* _nextEvent_char;
	int _nextEvent = 0;
	char* _latitude_char;
	char* _longitude_char;
	float _latitude;
	float _longitude;
	char* _status;
	char* _is_online;
	char* _model;
	
	aJsonObject* id_json;
	aJsonObject* status_json;
	aJsonObject* is_online_json;
	aJsonObject* model_json;
	aJsonObject* next_event_json;
	aJsonObject* latitude_json;
	aJsonObject* longitude_json;
};


#endif