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

#ifndef CAN1_H
#define CAN1_H

/**
  Section: Included Files
*/

#include <stdbool.h>
#include <stdint.h>
#include "can_types.h"



/**
  Section: CAN Module APIs
*/

/**
  @Summary
    Initialization routine that takes inputs from the CAN GUI.

  @Description
    This routine initializes the CAN module.
    This routine must be called before any other CAN routine is called.
    This routine should only be called once during system initialization.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Example
    <code>
    void main(void)
    {
        SYSTEM_Initialize();
   
        while (1)
        {
            //User Application code
        }      
    }
    </code> 
*/
void CAN1_Initialize(void);

/**
  @Summary
    Set the CAN operation mode.

  @Description
    This function sets the CAN operation mode.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function.
 
  @Param
    reqestMode - CAN operation mode

  @Returns
    CAN_OP_MODE_REQUEST_SUCCESS - Requested Operation mode set successfully
    CAN_OP_MODE_REQUEST_FAIL - Requested Operation mode set failure. Set configuration 
                               mode before setting CAN normal or debug operation mode.
    CAN_OP_MODE_SYS_ERROR_OCCURED - System error occurred while setting Operation mode.

  @Example
    <code>
    void main(void)
    {
        SYSTEM_Initialize();

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                //User Application code
            }
        }

        while (1)
        {
        }
    }
    </code> 
*/
CAN_OP_MODE_STATUS CAN1_OperationModeSet(const CAN_OP_MODES reqestMode);

/**
  @Summary
    Get the CAN operation mode.

  @Description
    This function gets the CAN operation mode.

  @Preconditions
    CAN1_Initialize() function should be called before calling this function. 
 
  @Param
    None

  @Returns
    Return the present CAN operation mode. 

  @Example
    <code>
    void main(void)
    {        
        SYSTEM_Initialize();
            
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                //User Application code
            }
        }
    
        while (1)
        {      
        }
    }
    </code> 
*/
CAN_OP_MODES CAN1_OperationModeGet(void);



/**
  @Summary
    Checks whether the transmitter is in bus off state.

  @Description
    This routine checks whether the transmitter is in bus off state.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Bus Off state
    false   - Transmitter not in Bus Off state

  @Example
    <code>
    #define CAN_TX_BUFF  "TEMPERATURE SENSOR ON"

    void main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[32] = CAN_TX_BUFF;
        
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_32;
                msg.data = data;
            
                if(CAN1_IsBusOff() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while(1);
    }
    </code>
*/
bool CAN1_IsBusOff(void);

/**
  @Summary
    Checks whether the transmitter is in the error passive state.

  @Description
    This routine checks whether the transmitter is in the error passive state.
    If Transmitter error counter is above 127, then the transmitter error passive 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error Passive state
    false   - Transmitter not in Error Passive state

  @Example
    <code>
    #define CAN_TX_BUFF  "TEMPERATURE SENSOR ON"

    void main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[32] = CAN_TX_BUFF;
        
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_32;
                msg.data = data;
            
                if(CAN1_IsTxErrorPassive() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while(1);
    }
    </code>
*/
bool CAN1_IsTxErrorPassive(void);

/**
  @Summary
    Checks whether the transmitter is in the error warning state.

  @Description
    This routine checks whether the transmitter is in the error warning state.
    If Transmitter error counter is above 95 to below 128, then transmitter error 
    warning state is set.

  @Preconditions
    CAN1_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error warning state
    false   - Transmitter not in Error warning state

  @Example
    <code>
    #define CAN_TX_BUFF  "TEMPERATURE SENSOR ON"

    void main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[32] = CAN_TX_BUFF;
        
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_32;
                msg.data = data;
            
                if(CAN1_IsTxErrorWarning() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while(1);
    }
    </code>
*/
bool CAN1_IsTxErrorWarning(void);

/**
  @Summary
    Checks whether the transmitter is in the error active state.

  @Description
    This routine checks whether the transmitter is in the error active state.
    If Transmitter error counter is above 0 to below 128, then transmitter error 
    active state is set.

  @Preconditions
    CAN1_Initialize function should be called 
    before calling this function.

  @Param
    None

  @Returns
    true    - Transmitter in Error active state
    false   - Transmitter not in Error active state

  @Example
    <code>
    #define CAN_TX_BUFF  "TEMPERATURE SENSOR ON"

    void main(void) 
    {
        CAN_MSG_OBJ msg;
        uint8_t data[32] = CAN_TX_BUFF;
        
        SYSTEM_Initialize();

        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                msg.msgId = 0x1FFFF;
                msg.field.formatType = CAN_FD_FORMAT;
                msg.field.brs = CAN_NON_BRS_MODE;
                msg.field.frameType = CAN_FRAME_DATA;
                msg.field.idType = CAN_FRAME_EXT;
                msg.field.dlc = DLC_32;
                msg.data = data;
            
                if(CAN1_IsTxErrorActive() == false)
                {
                    if(CAN_TX_FIFO_AVAILABLE == (CAN1_TransmitFIFOStatusGet(CAN1_TX_FIFO1) & CAN_TX_FIFO_AVAILABLE))
                    {
                        CAN1_Transmit(CAN1_TX_FIFO1, &msg);
                    }
                }
            }
        }
        
        while(1);
    }
    </code>
*/
bool CAN1_IsTxErrorActive(void);

/**
  @Summary
    Checks whether the Receiver is in the error passive state.

  @Description
    This routine checks whether the receive is in the error passive state.
    If Receiver error counter is above 127, then receiver error passive 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error passive state
    false   - Receiver not in Error passive state

  @Example
    <code>
    void main(void) 
    {
        CAN_MSG_OBJ msg;
        
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorPassive() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }
                }
            }
        }
    }
    </code>
*/
bool CAN1_IsRxErrorPassive(void);

/**
  @Summary
    Checks whether the Receiver is in the error warning state.

  @Description
    This routine checks whether the receive is in the error warning state.
    If Receiver error counter is above 95 to below 128, then receiver error warning 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error warning state
    false   - Receiver not in Error warning state

  @Example
    <code>
    void main(void) 
    {
        CAN_MSG_OBJ msg;
     
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorWarning() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }
                }
            }
        }
    }
    </code>
*/
bool CAN1_IsRxErrorWarning(void);

/**
  @Summary
    Checks whether the Receiver is in the error active state.

  @Description
    This routine checks whether the receive is in the error active state.
    If Receiver error counter is above 0 to below 128, then receiver error active 
    state is set.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    true    - Receiver in Error active state
    false   - Receiver not in Error active state

  @Example
    <code>
    void main(void) 
    {
        CAN_MSG_OBJ msg;
     
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                while(1) 
                {
                    if(CAN1_IsRxErrorActive() == false)
                    {
                        if(CAN1_ReceivedMessageCountGet() > 0) 
                        {
                            CAN1_Receive(&msg);
                        }
                    }
                }
            }
        }
    }
    </code>
*/
bool CAN1_IsRxErrorActive(void);

/**
  @Summary
    Puts CAN module in sleep mode.

  @Description
    This routine puts CAN module in sleep mode.

  @Preconditions
    CAN1_Initialize function should be called before calling this function.

  @Param
    None

  @Returns
    None

  @Example
    <code>
    void main(void) 
    {     
        SYSTEM_Initialize();
        
        if(CAN_CONFIGURATION_MODE == CAN1_OperationModeGet())
        {
            if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
            {
                CAN1_Sleep();
                
                if(CAN_DISABLE_MODE == CAN1_OperationModeGet())
                {
                    Sleep(); //Call sleep instruction
                    C1INTLbits.WAKIF = 0; // Clear Wake-Up interrupt flag
                    
                    // Recover to Normal mode
                    if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_CONFIGURATION_MODE))
                    {
                        if(CAN_OP_MODE_REQUEST_SUCCESS == CAN1_OperationModeSet(CAN_NORMAL_2_0_MODE))
                        {
                            //User Application code                            
                        }
                    }
                }
            }
        }
        
        while(1);
    }
    </code>
*/
void CAN1_Sleep(void);





#endif  //CAN1_H
