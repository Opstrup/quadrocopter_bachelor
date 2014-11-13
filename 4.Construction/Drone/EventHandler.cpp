/*
 * EventHandler.cpp
 *
 * Created: 11-11-2014 11:43:51
 *  Author: Kevin
 */ 
#include "EventHandler.h"

eventHandler::eventHandler()
{
	
}

int eventHandler::getNextEvent(String request)
{
	String _request;
	char* nextEventBody;
	_request = request;
	
	nextEventBody = _getAndput.getMethod(request);
	
	// Char* to JSON
	data_object = aJson.parse(nextEventBody);
	// Items in JSON seperated:

	aJsonObject* next_event_json = aJson.getObjectItem(data_object, "next_event");

	_nextEvent = atoi(aJson.print(next_event_json));
	
	return _nextEvent;
}

void eventHandler::setOnlineLocation(String request, float _latitude, float _longitude)
{
	aJsonObject* put_isOnline_location = aJson.createObject();
	
	//int _id = atoi(aJson.print(aJson.getObjectItem(data_object, "id")));
	
	
	//char* _model = aJson.print(aJson.getObjectItem(data_object, "model"));
	//_model = strtok(_model,"\"");
	//_model = strtok(_model,"\"");
	//int _putnextEvent = atoi(aJson.print(aJson.getObjectItem(data_object, "next_event")));

	
	aJson.addNumberToObject(put_isOnline_location,"id",1);
	aJson.addStringToObject(put_isOnline_location,"is_online" ,"true");
	aJson.addStringToObject(put_isOnline_location,"model", "aeroquad");
	aJson.addNumberToObject(put_isOnline_location,"next_event",_nextEvent);
	aJson.addNumberToObject(put_isOnline_location,"latitude",_latitude);
	aJson.addNumberToObject(put_isOnline_location,"longitude",_longitude);
	
	
	
	
	_getAndput.putMethod(request,put_isOnline_location);
	aJson.deleteItem(put_isOnline_location);
}


void eventHandler :: putResetNextEvent(String request, float _latitude, float _longitude, int nEvent)
{
	
	String _request;
	
	
	aJsonObject* put_object = aJson.createObject();
	
	int _id = atoi(aJson.print(aJson.getObjectItem(data_object, "id")));
	
	
	char* _model = aJson.print(aJson.getObjectItem(data_object, "model"));
	_model = strtok(_model,"\"");
	_model = strtok(_model,"\"");
	int _nextEvent = atoi(aJson.print(aJson.getObjectItem(data_object, "next_event")));

	
	aJson.addNumberToObject(put_object,"id",_id);
	aJson.addStringToObject(put_object,"is_online" ,"true");
	aJson.addStringToObject(put_object,"model", _model);
	aJson.addNumberToObject(put_object,"next_event",nEvent);
	
	
	aJson.addNumberToObject(put_object,"latitude",_latitude);
	aJson.addNumberToObject(put_object,"longitude",_longitude);
	
	
	_request = request;
	
	_getAndput.putMethod(_request,put_object);	
}
