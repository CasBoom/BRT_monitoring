/**
  CAN1 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    can1.c

  @Summary
    This is the generated driver implementation file for the CAN1 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides implementations for driver APIs for CAN1.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  1.1.1
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above or later
        MPLAB             :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#include <xc.h>
#include <stdint.h>
#include <string.h>
#include "can1.h"


#define SID_LOW_WIDTH                   (8U)
#define SID_HIGH_MASK                   (0x07U)
#define EID_LOW_WIDTH                   (5U)
#define EID_LOW_POSN                    (3U)
#define EID_LOW_MASK                    (0xF8U)
#define EID_MID_WIDTH                   (8U)
#define EID_HIGH_WIDTH                  (5U)
#define EID_HIGH_MASK                   (0x1FU)
#define IDE_POSN                        (4U)
#define RTR_POSN                        (5U)
#define BRS_POSN                        (6U)
#define FDF_POSN                        (7U)

#define DLCToPayloadBytes(x)            (DLC_BYTES[(x)])
#define PLSIZEToPayloadBytes(x)         (DLCToPayloadBytes(8u + (x)))

struct CAN_FIFOREG
{
    uint8_t CONL;
    uint8_t CONH;
    uint8_t CONU;
    uint8_t CONT;
    uint8_t STAL;
    uint8_t STAH;
    uint8_t STAU;
    uint8_t STAT;
    uint32_t UA;
};

static volatile struct CAN_FIFOREG * const FIFO = (struct CAN_FIFOREG *)&C1TXQCONL;
static const uint8_t DLC_BYTES[] = {0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U};




static void CAN1_BitRateConfiguration(void)
{
    // SJW 1; 
    C1NBTCFGL = 0x01;
    
    // TSEG2 1; 
    C1NBTCFGH = 0x01;
    
    // TSEG1 4; 
    C1NBTCFGU = 0x04;
    
    // BRP 0; 
    C1NBTCFGT = 0x00;
    
}


void CAN1_Initialize(void)
{
    /* Enable the CAN module */
    C1CONHbits.ON = 1;
    
    if (CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
    {
        /* Initialize the C1FIFOBA with the start address of the CAN FIFO message object area. */
        C1FIFOBA = 0x3800;
        
        // CLKSEL0 disabled; PXEDIS enabled; ISOCRCEN enabled; DNCNT 0; 
        C1CONL = 0x60;

        // ON enabled; FRZ disabled; SIDL disabled; BRSDIS enabled; WFT T11 Filter; WAKFIL enabled; 
        C1CONH = 0x97;

        // TXQEN disabled; STEF disabled; SERR2LOM disabled; ESIGM disabled; RTXAT disabled; 
        C1CONU = 0x00;

        CAN1_BitRateConfiguration();
        CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE);    
    }
}

CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES requestMode)
{
    CAN_OP_MODE_STATUS status = CAN_OP_MODE_REQUEST_SUCCESS;
    CAN_OP_MODES opMode = CAN1_OperationModeGet();

    if (CAN_CONFIGURATION_MODE == opMode
            || CAN_DISABLE_MODE == requestMode
            || CAN_CONFIGURATION_MODE == requestMode)
    {
        C1CONTbits.REQOP = requestMode;
        
        while (C1CONUbits.OPMOD != requestMode)
        {
            //This condition is avoiding the system error case endless loop
            if (1 == C1INTHbits.SERRIF)
            {
                status = CAN_OP_MODE_SYS_ERROR_OCCURED;
                break;
            }
        }
    }
    else
    {
        status = CAN_OP_MODE_REQUEST_FAIL;
    }

    return status;
}

CAN_OP_MODES CAN1_OperationModeGet(void)
{
    return C1CONUbits.OPMOD;
}



bool CAN1_IsBusOff(void)
{
    return C1TRECUbits.TXBO;
}

bool CAN1_IsRxErrorPassive(void)
{
    return C1TRECUbits.RXBP;
}

bool CAN1_IsRxErrorWarning(void)
{
    return C1TRECUbits.RXWARN;
}

bool CAN1_IsRxErrorActive(void)
{
    return !CAN1_IsRxErrorPassive();
}

bool CAN1_IsTxErrorPassive(void)
{
    return C1TRECUbits.TXBP;
}

bool CAN1_IsTxErrorWarning(void)
{
    return C1TRECUbits.TXWARN;
}

bool CAN1_IsTxErrorActive(void)
{
    return !CAN1_IsTxErrorPassive();
}

void CAN1_Sleep(void)
{
    C1INTHbits.WAKIF = 0;
    C1INTTbits.WAKIE = 1;
    
    CAN1_OperationModeSet(CAN_DISABLE_MODE);
}




