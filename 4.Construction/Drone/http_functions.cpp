#include <avr/io.h>
#include "Arduino.h"
#include "http_functions.h"
#include "sendATCommand.h"
#include "sendAT.h"

http :: http()
{

}

char* http :: get_http(char request[])
{
	// define values for the request:
	char url[ ]="pruebas.libelium.com";
	int port= 80;
	int test=12;
	request_ = request;

	sprintf(aux_str, "AT+CHTTPACT=\"%s\",%d", url, port);
	http_answer = sendATcommand(aux_str, "+CHTTPACT: REQUEST", 60000);

	if (http_answer == 1)
	{
		Serial.println(request_);
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
					for (int y= 0; y < data_size; y++)
						{
							data[y] = Serial.read();
							if ( (data[y] <127 && data[y] > 31) || data[y] == 13 || data[y] == 10)
							{
								//Serial.print(data[x]);
								x=y;
							}
							else
							{
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

// 	int z = (x - 37);
// 	http_y = 0;
// 	for (z; z< x;z++)
// 	{
// 		
// 		http_data[http_y] = data[z];
// 		http_y++;
// 		//Serial.print(data[z]);
// 		
// 	}
// 	http_data[http_y]  = '\0'; 
	}
	else
	{
		Serial.println("Error waiting the request");
	}
	return data;
}