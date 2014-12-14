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
	
	aJson.addNumberToObject(put_isOnline_location,"id",1);
	aJson.addStringToObject(put_isOnline_location,"is_online" ,"true");
	aJson.addStringToObject(put_isOnline_location,"model", "aeroquad");
	aJson.addNumberToObject(put_isOnline_location,"next_event",_nextEvent);
	aJson.addNumberToObject(put_isOnline_location,"latitude",_latitude);
	aJson.addNumberToObject(put_isOnline_location,"longitude",_longitude);
	
	
	
	
	_getAndput.putMethod(request,put_isOnline_location);
	aJson.deleteItem(put_isOnline_location);
}


void eventHandler :: putResetNextEvent(String request, float _latitude, float _longitude)
{
	aJsonObject* put_resetevent = aJson.createObject();
	
	aJson.addNumberToObject(put_resetevent,"id",1);
	aJson.addStringToObject(put_resetevent,"is_online" ,"true");
	aJson.addStringToObject(put_resetevent,"model", "aeroquad");
	aJson.addNumberToObject(put_resetevent,"next_event",0);
	
	
	aJson.addNumberToObject(put_resetevent,"latitude",_latitude);
	aJson.addNumberToObject(put_resetevent,"longitude",_longitude);
	
	char* _url = "iha-11726.iha.dk";
	int port = 80;
	char aux_str[100];
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
	Serial.println(aux_str);
	
	delay(4000);
	
	_getAndput.putMethod(request,put_resetevent);
	aJson.deleteItem(put_resetevent);	
}
