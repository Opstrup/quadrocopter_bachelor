/*
 * GetAndPut.cpp
 *
 * Created: 10-11-2014 18:48:39
 *  Author: Kevin
 */ 
#include "GetAndPut.h"

getAndput ::getAndput()
{
	
}

char* getAndput::getMethod(String request)
{
	char data[700];
	char aux_str[100];
	char* body;
	int8_t http_answer;
	// define values for the request:
	int port= 80;
	_url = "iha-11726.iha.dk";

	int x, aux, data_size;
	
	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
	http_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

	if (http_answer == 1)
	{

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
	
	char * header;
	header = strtok (data,"\r\n\r\n");
	for (int y = 0; y < 10; y++ )
	{
		body = strtok (NULL,"\r\n\r\n");
	}
	return body;
}

void getAndput::putMethod(String request, aJsonObject* jSON)
{
	aJsonObject* put_jsondata = aJson.createObject();
	put_jsondata = jSON;
	char aux_str[100];
	int x, data_size;
	int8_t put_answer;
//	int port = 80;
	_url = "iha-11726.iha.dk";
	String dataToPut = aJson.print(put_jsondata);
	// To find Size!
	char* put_data = aJson.print(put_jsondata);
	int p = 0;
	while (put_data[p] != '}')
	{
		p++;
	}
	p++;
	
	String putRequest = "PUT " + request + " HTTP/1.1\r\nHost: " + _url + "\r\nCache-Control: no-cache\r\nContent-Type: application/json\r\nContent-Length: " + p + "\r\n\r\n" + dataToPut + "\r\n\r\n";
	
//	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", _url, port);
//	put_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 5000);
	put_answer = 1;
	if (put_answer == 1)
	{
		Serial.print(putRequest);
		// Sends <Ctrl+Z>
		aux_str[0] = 0x1A;
		aux_str[1] = 0x00;
		Serial.print(aux_str);
//		put_answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 3000);

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

void getAndput ::putBruteForce(String request, aJsonObject* jSON)
{
	aJsonObject* put_brutedata = aJson.createObject();
	put_brutedata = jSON;
	char aux_str[100];
	int x, data_size;
	int8_t put_answer;
	int port = 80;
	_url = "iha-11726.iha.dk";
	String dataToPut = aJson.print(put_brutedata);
	// To find Size!
	char* put_data = aJson.print(put_brutedata);
	int p = 0;
	while (put_data[p] != '}')
	{
		p++;
	}
	p++;
	
	

	String putRequest = "PUT " + request + " HTTP/1.1\r\nHost: " + _url + "\r\nCache-Control: no-cache\r\nContent-Type: application/json\r\nContent-Length: " + p + "\r\n\r\n" + dataToPut + "\r\n\r\n";

	
// 	Serial.println(aux_str);
 	put_answer = 1;
// 	delay(3000);
//	put_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 5000);

	if (put_answer == 1)
	{
		Serial.println(putRequest);
		// Sends <Ctrl+Z>
		aux_str[0] = 0x1A;
		aux_str[1] = 0x00;
		Serial.print(aux_str);
		
		
//		put_answer = sendATcommand(aux_str, "+CHTTPACT: DATA,", 3000);
//		Serial.print(put_answer);
/*		
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
*/
		
			Serial.println("");
			Serial.print("After: ");
			Serial.print(millis());
			Serial.println("");
	}
	else
	{
		Serial.println("Error waiting the request");
	}
}