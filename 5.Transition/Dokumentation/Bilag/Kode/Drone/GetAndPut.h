/*
 * GetAndPut.h
 *
 * Created: 10-11-2014 18:45:36
 *  Author: Kevin
 */ 


#ifndef GETANDPUT_H_
#define GETANDPUT_H_
#include "Arduino.h"
#include <avr/io.h>
#include "aJSon.h"
#include "stringbuffer.h"
#include "sendAT.h"

class getAndput
{
public:
	getAndput();
	char* getMethod(String request);
	void putMethod(String request, aJsonObject* jSON);
	void putBruteForce(String request, aJsonObject* jSON);
	
private:	
	
	String _request;
	char* _url;
	
	};



#endif /* GETANDPUT_H_ */