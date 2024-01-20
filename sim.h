/* 
 * File:   sim.h
 * Author: casbo
 *
 * Created on December 3, 2023, 1:54 PM
 */

#include <stdint.h>
#include <math.h>
#include "constants.h"
#include "uart.h"
#ifndef SIM_H
#define	SIM_H

#ifdef	__cplusplus
extern "C" {
#endif
   
#define URL "basically-clean-alpaca.ngrok-free.app/value/update/"
#define TOKEN "55f201a9df707c12d8012ba2b111c2d3"
//sends response to buffer for more involved response flows
void bufferCommand(char* command, int tLength, char* buffer, int bLength){
    printLn(command, tLength);
    read(buffer, sizeof(buffer)/sizeof(char));
}

// send a simple AT command
uint8_t sendCommand(char* command, int length){
    char buffer[32];
    bufferCommand(command, length, buffer, sizeof(buffer)/sizeof(char));
    return (buffer[0]=='O' && buffer[1]=='K'); //returns true if OK, otherwise returns false;
}

// send AT command with non-standard expected response
uint8_t checkCommand(char* command, int tLength, char* response, int rLength){
    char buffer[32];
    bufferCommand(command, tLength, buffer, sizeof(buffer)/sizeof(char));
    for(int i=0; i<rLength; i++){
        if(*(response+i) != *(buffer+i))return 0;
    }
    return 1;
}

// sim struct, stores status of the SIM and the GNSS location
struct SIM{
    uint8_t card;
    uint8_t connection;
    uint8_t gnss;
    float lat;
    float lon;
};

// struct to temporarily store location data in the parser
struct Location {
	float lat;
	float lon;
};

// data object to be transmitted
struct CanData{
    int canId;
    int valueId;
    int value;
};

uint8_t parseLatLon(struct Location* loc, char* string, int string_length) {
	int field = 0; // NMAE field
	int i = 0; // string index
	int buffer_size = 0; // buffer index
	char c = ' '; // current character
	char buffer[128];
	while (field <= 4 && buffer_size <= 128-1) {
		c = *(i + string);
		*(buffer_size + buffer) = c;
		i++;
		buffer_size++;
		if (c == ',') {
			if (field == 3) {
				if (buffer_size <= 1) return 1;
				loc->lat = strtof(buffer, NULL);
			}
			if (field == 4) {
				if (buffer_size <= 1) return 1;
				loc->lon = strtof(buffer, NULL);
			}

			for (int m = buffer_size; m <= 0; m--) { // clear buffer
				*(m + buffer) = ' ';
			}
			buffer_size = 0;
			field++;
		}
	}
	if (field < 4) return 2; // out of space
	return 0;
}

bool updateCoordinates(struct SIM* sim){
    struct Location loc;
    char response[128];
    // ask for GNSS coordinates
    char command[] = "AT+CGNSINF";
    bufferCommand(command, sizeof(command)/sizeof(char), response, sizeof(response)/sizeof(char));
    if( parseLatLon(&loc, response, sizeof(response)/sizeof(char) ) ){ 
         sim->gnss=0; // no gnss update
         print(response, sizeof(response)/sizeof(char));
         return 1; //return 1 if an error occured
    }
    sim->lat=loc.lat;
    sim->lon=loc.lon;
    print(response, sizeof(response)/sizeof(char));
    sim->gnss=1; // gnss updated
    return 0;
}

void setPostData(struct CanData *CanData){
    unsigned int dataSize = 0;
    char dataSizeString[8];
    char postData[256];
    char token[] = TOKEN;
    char timeoutString[] = "9999";
    sprintf(postData,"token=%s&value=%d",TOKEN, CanData->value);
    dataSize = strlen(postData);
    
    // set data size
    sprintf(dataSizeString,"%u", dataSize);
    print(dataSizeString, strlen(dataSizeString)+1);
    transmit(',');
    printLn(timeoutString,sizeof(timeoutString)/sizeof(char));
    printLn(postData, strlen(postData)+1);
}

void transmitData(struct CanData *CanData, int dataListLength){
    //this is ugly
    char url[] = URL;
    char id[5];
    char init[] = "AT+HTTPINIT";
    char para[] = "AT+HTTPPARA=\"URL\",";
    char data[] = "AT+HTTPDATA=";
    char send[] = "AT+HTTPACTION=1";
    char term[] = "AT+HTTPTERM";
    
    for(int i =0; i < dataListLength; i++){
        sprintf(id, "%d", (CanData+i)->valueId);
        printLn(init, sizeof(init)/sizeof(char)); // init http
        print(para, sizeof(para)/sizeof(char));   // setup params
        print(url, sizeof(url)/sizeof(char));   // append URL
        printLn(id, strlen(id)+1);
        print(data, sizeof(data)/sizeof(char));   // beginning of start data string
        setPostData((CanData+i));                     // set post data
        printLn(send, sizeof(send)/sizeof(char)); // send post request
        printLn(term, sizeof(term)/sizeof(char));
    }
    
}

uint8_t SIMCardExists(){
    // check if simcard exists
    // return 1 if it does
    // return 0 if not
    char* command="AT+CPIN=?";
    char* response="READY";
    return checkCommand(command, sizeof(command)/sizeof(char), response, sizeof(response)/sizeof(char));
}

uint8_t makeConnection(){
    // connect to GSM/LTE/Whatever
    return 0; // if successful
    //return 1 if failed;
}

uint8_t setupSIM(struct SIM* sim){
    //attempt to setup sim
    sim->card=0;
    sim->connection=0;
    if(!SIMCardExists()) return 1; // return 1 if the SimCard doesn't exist
    sim->card=1;
    // turn on gnss with AT commands, once OK ask update
    //if(pingServer()) return 4;
    
    return 0;
    
}

#ifdef	__cplusplus
}
#endif

#endif	/* SIM_H */

