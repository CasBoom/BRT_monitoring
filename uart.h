/* 
 * File:   uart.h
 * Author: casbo
 *
 * Created on December 3, 2023, 1:52 PM
 */

#include "constants.h"
#include "mcc_generated_files/mcc.h"

#include <stdint.h>

#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

static void setupUART(){
    //U5CON0 
    //Fosc/[16(UxBRG+1)]
    // baudrate must be 1125000 bits/s
    U5BRG = 12; //divide by 12
    RC3PPS = 0x2C;//set pin 18 to Tx
    //RA5PPS = 5;
    // 1 startbit, 8 databits, 1 end bits, no parity check: dit is de default
    U5CON0 |= BIT5 | BIT4; //enable RX and TX
    U5CON1 |= BIT7; //enable UART
}

void transmit(char byte){
    while(U5FIFO&BIT4); // polling till available
    U5TXB = byte;
}

void print(char* string, int length){
    for(int i=0; i<length-1; i++) // minus one for the string terminator
    {
        transmit(*(string+i));
    }
}

void printLn(char* string, int length){
    print(string, length);
    transmit('\r');
    transmit('\n');
}

void read(char* buffer, int buffer_size){
    char input=0;
    int i=0;
    while(input != '\n' && i<buffer_size){ //while last char not endline and buffer_size not exceeded
        while(U5FIFO&BIT1); //while empty wait till bit available
        input = U5TXB; //add bit to buffer
        *(i+buffer) = input;
        transmit(input);
        i++;
        U5FIFO |= BIT1; // clear buffer
    }
}

#endif	/* UART_H */

