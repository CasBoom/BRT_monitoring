/* 
 * File:   sim.h
 * Author: casbo
 *
 * Created on December 3, 2023, 1:54 PM
 */

#include "constants.h"
#include "uart.h"
#ifndef SIM_H
#define	SIM_H

#ifdef	__cplusplus
extern "C" {
#endif

// send a simple AT command
bool sendCommand(char* command, int length){
    char buffer[32];
    printLn(command, length);
    read(&buffer, sizeof(buffer)/sizeof(char));
    return (buffer[0]=='O' && buffer[1]=='K'); //returns true if OK, otherwise returns false;
}

// send AT command with non-standard expected response
bool sendCommand(char* command, int tLength, char* response, int rLength){
    char buffer[32];
    printLn(command, tLength);
    read(&buffer, sizeof(buffer)/sizeof(char));
    for(int i=0; i<rLength; i++){
        if(response+i != buffer+i)return false;
    }
    return true;
}

//sends response to buffer for more involved response flows
void bufferCommand(char* command, int tLength, char* buffer, int bLength){
    printLn(command, tLength);
    read(&buffer, sizeof(buffer)/sizeof(char));
}

struct SIM{
    uint8_t card;
    uint8_t connection;
    uint8_t gnss;
    uint16_t lat;
    uint16_t lon;
};
    
uint8_t SIMCardExists(){
    // check if simcard exists
    // return 1 if it does
    // return 0 if not
    char* command="AT+CDIN=?";
    char* response="READY";
    return sendCommand(command, sizeof(command)/sizeof(char), response, sizeof(response)/sizeof(char));
}

uint8_t makeConnection(){
    // connect to GSM/LTE/Whatever
    return 0; // if successful
    //return 1 if failed;
}

uint8_t updateCoordinates(struct SIM* sim){
    // ask for GNSS coordinates
    // if successful, 
    // &sim.lat=latitude;
    // &sim.lon=longitude;
    // &sim.gnss=1;
    return 0;
    //if unsuccessful
    //&sim.gnss=0;
    //return 1 (don't change lat or lon, because the old values still give information)
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

