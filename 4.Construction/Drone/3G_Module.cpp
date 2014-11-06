#include <avr/io.h>
#include "Arduino.h"
#include "3G_Module.h"

Module_3G::Module_3G()
{
	
}

char* Module_3G::get_FlightSetup()
{
	
	int8_t http_answer;
	// define values for the request:
	char url[ ]="iha-11726.iha.dk";
	int port= 80;
	
//	char request[]  = "GET /api/drones/1/?format=json HTTP/1.1\r\nHost: iha-11726.iha.dk\r\nContent-Length: 0\r\n\r\n";
	char* request = "GET /api/";
	char* request_path = "drones/";
	char* request_event = "1/?format=json";
	char* http_part = " HTTP/1.1\r\nHost: iha-11726.iha.dk\r\nContent-Length: 0\r\n\r\n";
	
	int x, aux, data_size;
	
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
	http_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

	if (http_answer == 1)
	{
		if (_nextEvent > 0)
		{
			request_path = "drones/";
			request_event = _nextEvent_char;
		}
		Serial.print(request);
		Serial.print(request_path);
		Serial.print(request_event);
		Serial.print(http_part);
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
	char * header;
	header = strtok (data,"\r\n\r\n");
	for (int y = 0; y < 10; y++ )
	{
		body = strtok (NULL,"\r\n\r\n");
	}

	sortJSON();
	return body;
}

void Module_3G ::put_isOnline_CurrPos(float longitude,float latitude)
{
	delay(1000);
	
	int x,aux, data_size;
	int8_t put_answer;
	char url[] = "iha-11726.iha.dk";
	int port = 80;
	float _longitude,_latitude;
	char * putlink_;
	// For testing!
	//body1 = "{\"id\":1,\"status\":\"Ready\",\"is_online\":false,\"model\":\"aeroquad\",\"next_event\":2,\"latitude\":60.171,\"longitude\":20.191}";
	//Serial.println(body1);
		
	aJsonObject* put_object = aJson.createObject();

	
	aJson.addNumberToObject(put_object,"id",_id);
	aJson.addStringToObject(put_object,"status","Ready");
 	aJson.addStringToObject(put_object,"is_online" ,"true");
	aJson.addStringToObject(put_object,"model","Aeroquad");
	aJson.addNumberToObject(put_object,"next_event",_nextEvent);
	aJson.addNumberToObject(put_object,"latitude",latitude);
	aJson.addNumberToObject(put_object,"longitude",longitude);
	
	char * put_data = aJson.print(put_object);
	
	int put_size = sizeof(body);
	int p = 0;
	while (put_data[p] != '}')
	{
	p++;
	}
	p++;

	putlink_ = "PUT /api/drones/1/ HTTP/1.1\r\nHost: iha-11726.iha.dk\r\nCache-Control: no-cache\r\nContent-Type: application/json\r\nContent-Length: ";
	
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
	put_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 5000);

	if (put_answer == 1)
	{
		Serial.print(putlink_);
		Serial.print(p);
		Serial.print("\r\n\r\n");
		Serial.print(put_data);
		Serial.print("\r\n\r\n");

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
	aJsonObject* data_object = aJson.parse(body);
	// Items in JSON seperated:
	
	id_json = aJson.getObjectItem(data_object, "id");
	status_json = aJson.getObjectItem(data_object, "status");
	is_online_json = aJson.getObjectItem(data_object, "is_online");
	model_json = aJson.getObjectItem(data_object, "model");
	next_event_json = aJson.getObjectItem(data_object, "next_event");
	latitude_json = aJson.getObjectItem(data_object, "latitude");
	longitude_json = aJson.getObjectItem(data_object, "longitude");
	
	_id_char = aJson.print(id_json);
	_is_online = aJson.print(is_online_json);
	_model = aJson.print(model_json);
	_nextEvent_char = aJson.print(next_event_json);
	_status = aJson.print(status_json);
	_latitude_char = aJson.print(latitude_json);
	_longitude_char = aJson.print(longitude_json);
	
	
	_id = atoi(_id_char);
	_nextEvent = atoi(_nextEvent_char);
	_latitude = atof(_latitude_char);
	_longitude = atof(_longitude_char);
	
// 	
// 	Serial.println(_id);
// 	Serial.println(_nextEvent);
// 	Serial.println(_status);
// 	Serial.println(_model);
// 	Serial.println(_is_online);
	
}