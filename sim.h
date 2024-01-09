/* 
 * File:   sim.h
 * Author: casbo
 *
 * Created on December 3, 2023, 1:54 PM
 */

#include <stdint.h>
#include "constants.h"
#include "uart.h"
#ifndef SIM_H
#define	SIM_H

#ifdef	__cplusplus
extern "C" {
#endif
    
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

struct SIM{
    uint8_t card;
    uint8_t connection;
    uint8_t gnss;
    float lat;
    float lon;
};

struct Location {
	float lat;
	float lon;
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
    if( 1 ){ //parseLatLon(loc*, response*, sizeof(response)/sizeof(char) )
         sim->gnss=0; // no gnss update
         return 1; //return 1 if an error occured
    }
    sim->lat=loc.lat;
    sim->lon=loc.lon;
    sim->gnss=1; // gnss updated
    return 0;
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
    
    if(updateCoordinates(sim)) return 3;
    //if(pingServer()) return 4;
    
    return 0;
    
}

#ifdef	__cplusplus
}
#endif

#endif	/* SIM_H */

