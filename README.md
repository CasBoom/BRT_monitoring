# BRT_monitoring
Monitoring system for the Blue Racing Team

## Overview
This project is made out of 3 core parts
- a CAN-bus interface
- calls to a SIM7600 module to gather GNSS data
- calls to the same SIM7600 module to transmit both the CAN-bus data and the GNSS data

## CAN-bus interface
The CAN-bus interface hasn't yet been implemented yet.
The CAN-bus should be set up so that on interrupt, the right CAN-bus struct is written to.
Outside of the wait loop, interrupts should be disabled.

## Getting GNSS data
In order to get GNSS data, firstly you'll need to turn on the GNSS submodule in the SIM7600.
This can be done with the "AT+CGNSPWR=1" command. 
Next you can get a NMAE string with the "AT+CGNSINF" command.
After parsing this NMAE string you'll be able to write the latitude and longitude to the SIM struct.
Due to a bug in the UART driver, this feature hasn't been implemented.

## Transmitting data
In order to transmit data you'll need to a handfull off commands.
To setup the SIM7600 to be able to send POST requests you'll first need to send
- “AT+CPIN” 		checks if SIM-card available
- “AT+CNMP=13” 	    Enables “GSM only” mode
- “AT+CMNB=1” 		Turns on NBIOT
- “AT+SAPBR=1,1” 	Turns on GPRS

transmitData() function, this function takes in a list of CanData structs and the length of this list.
NOTE: This has not been tested due to a hardware error.

## Files
Short description of the files
- main.c        Contains basic control flow
- uart.h        UART driver and base functions
- sim.h         Contains basic SIM calls and data types
- constants.h   Containts nice to have constants to make code more readable

