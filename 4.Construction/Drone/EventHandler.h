/*
 * EventHandler.h
 *
 * Created: 11-11-2014 11:41:56
 *  Author: Kevin
 */ 


#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_

#include "GetAndPut.h"

class eventHandler
{
	
public:
	eventHandler();
	int getNextEvent(String request);
	void setOnlineLocation(String request, float _latitude, float _longitude);
	void putResetNextEvent(String request, float _latitude, float _longitude, int nEvent);
		

private:
	int _nextEvent;
	aJsonObject* data_object;
	getAndput _getAndput;
	};




#endif /* EVENTHANDLER_H_ */