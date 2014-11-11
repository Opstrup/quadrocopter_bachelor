#include <avr/io.h>
#include "Arduino.h"
#include "3G_Module.h"

Module_3G::Module_3G(char* url, String requestDrone,String requestWaypoints)
{
	_url = url;
	_requestDrone = requestDrone;
	_requestWaypoints = requestWaypoints;
	
}

int Module_3G::get_FlightSetup()
{
	char aux_str[100];
	int8_t http_answer;
	// define values for the request:
	int port= 80;
	
	// If nextevent == 0
	String request = _requestDrone;

	int x, aux, data_size;
	
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
	http_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

	if (http_answer == 1)
	{

		if (_nextEvent > 0)
		{
			request = _requestWaypoints + _nextEvent + "/";
			
		}
		String getRequest = "GET " + request + " HTTP/1.1\r\nHost: " + _url + "\r\nContent-Length: 0\r\n\r\n";
		Serial.print(getRequest);
		// Sends <Ctrl+Z>
		aux_str[0] = 0x1A;
		aux_str[1] = 0x00;
		http_answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 60000);

		x=0;
		if (http_answer == 1)
		{
			data_size = 0;
			while(Serial.available()==0);
			aux = Serial.read();
			do{
				data_size *= 10;
				data_size += (aux-0x30);
				while(Serial.available()==0);
				aux = Serial.read();
			}
			while(aux != 0x0D);
			Serial.print("Data received: ");
			Serial.println(data_size);
			
			if (data_size > 0)
			{
				for (int y= 0; y < data_size; y++){
					data[y] = Serial.read();
					if ( (data[y] <127 && data[y] > 31) || data[y] == 13 || data[y] == 10){
						x=y;
					}
					else{
						y--;
					}
				}
				x++;
				data[x] = '\0';
			}
			else
			{
				Serial.println("Download finished");
			}
		}
		else
		{
			Serial.println("Error getting the url");
			data_size = 0;
		}
	}
	else
	{
		Serial.println("Error waiting the request");
	}
	
	if (_nextEvent > 0)
	{
		char * header;
		header = strtok (data,"\r\n\r\n");
		for (int y = 0; y < 10; y++ )
		{
			bodyWaypoints = strtok (NULL,"\r\n\r\n");
		}
		findNumberOfWayPoints();
		data_size = 0;
		setNextEvent = 1;
		return numberOfWaypoints;
	}
	else
	{
		char * header;
		header = strtok (data,"\r\n\r\n");
		for (int y = 0; y < 10; y++ )
		{
			bodyEvent = strtok (NULL,"\r\n\r\n");
		}
		data_size = 0;
		sortJSON();
		return _nextEvent;
	}
	
	
}

aJsonObject* Module_3G :: findNumberOfWayPoints()
{
	int w = 0;
 	String strBodyWayPoints = bodyWaypoints;

	
	if (strBodyWayPoints[0] == '[')
	{
		while (strBodyWayPoints[w] != ']')
		{
			if (strBodyWayPoints[w] == '{')
			{
				numberOfWaypoints++;
			}
			w++;
		}
	}
	else
	{
		numberOfWaypoints = 1;
	}
	
}

void Module_3G :: sortWayPoints(int wayPointNumber)
{
	aJsonObject* latitude_json;
	aJsonObject* longitude_json;
	aJsonObject* height_json;
	aJsonObject* takePhoto_json;
	char* _latitude_char;
	char* _longitude_char;
	String devide_waypoints;
	char* trash;

	if (wayPointNumber <= numberOfWaypoints)
	{
		String wayPoints[numberOfWaypoints];
		
		if (allowModify == 1)
		{
			
			if (numberOfWaypoints > 1)
			{
				trash = strtok (bodyWaypoints,"{");
				for (int i = 0 ; i < numberOfWaypoints; i++)
				{
					devide_waypoints = strtok (NULL,"}");
					trash = strtok (NULL,"{");
					devide_waypoints = "{" + devide_waypoints + "}";
					wayPoints[i] = devide_waypoints;
					//wayPoints[i] = devide_waypoints;
					}
				}
			else if (numberOfWaypoints == 1)
			{
				devide_waypoints = strtok (NULL,"}");
				devide_waypoints = devide_waypoints + "}";
				wayPoints[0] = devide_waypoints;
			}
			for (int u = 0; u < numberOfWaypoints; u++)
			{
				safeWayPoints[u] = wayPoints[u];
			}
		}
		
		
		char chosenWayPoint[150];
		safeWayPoints[wayPointNumber].toCharArray(chosenWayPoint,150);
		
		aJsonObject* waypoints_object = aJson.parse(chosenWayPoint);

	
	
	
		latitude_json = aJson.getObjectItem(waypoints_object, "latitude");
		longitude_json = aJson.getObjectItem(waypoints_object, "longitude");
		takePhoto_json = aJson.getObjectItem(waypoints_object, "take_photo");
		height_json = aJson.getObjectItem(waypoints_object, "height");
		
		
		_latitude_char = aJson.print(latitude_json);
		_longitude_char = aJson.print(longitude_json);
		for (int y = 0; y < 1;y++ )
		{
			_latitude_char = strtok(_latitude_char,"\"");
		}
		for (int y = 0; y < 1;y++ )
		{
			_longitude_char = strtok(_longitude_char,"\"");
		}
		_latitude = atof(_latitude_char);
		_longitude = atof(_longitude_char);
		_takePhoto = aJson.print(takePhoto_json);
		_height = atof(aJson.print(height_json));
		
		allowModify = 0;

	}
		
	else
	{
		
	}
}


void Module_3G ::put_isOnline_CurrPos(float longitude,float latitude)
{
	char aux_str[100];
	int x, data_size;
	int8_t put_answer;
	int port = 80;
	aJsonObject* put_object = aJson.createObject();

	
	aJson.addNumberToObject(put_object,"id",_id);
 	aJson.addStringToObject(put_object,"is_online" ,"true");
	aJson.addStringToObject(put_object,"model", _model);
	if (setNextEvent > 0)
	{
		aJson.addNumberToObject(put_object,"next_event",0);
		setNextEvent = 0;
	}
	else
	{
		aJson.addNumberToObject(put_object,"next_event",_nextEvent);
	}
	
	aJson.addNumberToObject(put_object,"latitude",latitude);
	aJson.addNumberToObject(put_object,"longitude",longitude);
	
	String dataToPut = aJson.print(put_object);
	char* put_data = aJson.print(put_object);
	
	int p = 0;
	while (put_data[p] != '}')
	{
	p++;
	}
	p++;

	String putRequest = "PUT " + _requestDrone + " HTTP/1.1\r\nHost: " + _url + "\r\nCache-Control: no-cache\r\nContent-Type: application/json\r\nContent-Length: " + p + "\r\n\r\n" + dataToPut + "\r\n\r\n";
	
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
	put_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 5000);

	if (put_answer == 1)
	{
		Serial.print(putRequest);
		// Sends <Ctrl+Z>
		aux_str[0] = 0x1A;
		aux_str[1] = 0x00;
		put_answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 3000);

		x=0;
		if (put_answer == 1)
		{
			Serial.println("sent");
		}
		else
		{
			Serial.println("Error getting the url");
			data_size = 0;
		}

	}
	else
	{
		Serial.println("Error waiting the request");
	}
}

aJsonObject* Module_3G :: sortJSON()
{
		
	// Char* to JSON
	aJsonObject* data_object = aJson.parse(bodyEvent);
	// Items in JSON seperated:
	
	aJsonObject* id_json = aJson.getObjectItem(data_object, "id");
	aJsonObject* is_online_json = aJson.getObjectItem(data_object, "is_online");
	aJsonObject* model_json = aJson.getObjectItem(data_object, "model");
	aJsonObject* next_event_json = aJson.getObjectItem(data_object, "next_event");
	aJsonObject* latitude_json = aJson.getObjectItem(data_object, "latitude");
	aJsonObject* longitude_json = aJson.getObjectItem(data_object, "longitude");
	
	_id = atoi(aJson.print(id_json));
	_is_online = aJson.print(is_online_json);
	_model = aJson.print(model_json);
	_model = strtok(_model,"\"");
	_model = strtok(_model,"\"");
	_nextEvent = atoi(aJson.print(next_event_json));

}

float Module_3G::getLat()
{
	return _latitude;
}
float Module_3G::getLong()
{
	return _longitude;
}

float Module_3G::getHeight()
{
	return _height;
}

char* Module_3G :: getTakePhoto()
{
	return _takePhoto;
}