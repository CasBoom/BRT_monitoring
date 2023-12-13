/* 
 * File:   sim.h
 * Author: casbo
 *
 * Created on December 3, 2023, 1:54 PM
 */

#include "constants.h"
#ifndef SIM_H
#define	SIM_H

#ifdef	__cplusplus
extern "C" {
#endif


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
    return 1;
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
    if(!SIMCardExists()) return 1; // return 1 if the 
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

