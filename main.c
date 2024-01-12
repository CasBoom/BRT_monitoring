/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.00
*/

#include "mcc_generated_files/mcc.h"
#include <string.h>
#include <stdint.h>
#include <xc.h>
#include "uart.h"
#include "sim.h"
#include "constants.h"

/*
                         Main application
 */

//staat van het hele project, is globaal 
enum state {setup, wait, getGNSS, sendData };
enum state State;
static void setupRegs(){
    setupUART();
}

static void setup(){
    setupRegs();
}

struct SIM sim;

void main(void)
{
    State = setup; 
    OSCFRQ |= 0x07; // set HFosc to 48MHz
//    OSCCON1 |= 0b01110000; // set oscillator source to HFosc
    OSCCON1 = 0b01100001; // set oscillator source to HFosc
    // Initialize the device
    
    struct SIM sim;
    setupRegs();
    setupSIM(&sim);
    // Enable the Global Interrupts
    PORTB = 0x00; // Clear PORTB
    LATB = 0x00; // Clear Data Latch
    ANSELB = 0x00; // Enable digital drivers
    TRISB = 0x00; //set all registers to OUTPUT
    while (1)
    {
        switch(state){
            case wait:
                INTERRUPT_GlobalInterruptEnable();
                __delay_ms(5000);
                state = getGNSS;
                break;
            case getGNSS:
                INTERRUPT_GlobalInterruptDisable();
                updateCoordinates(struct SIM* sim);
                state = sendData;
                break;
            case sendData:
                state = wait;
                break;

        }
        TRISB ^= BIT5; 
    }
}