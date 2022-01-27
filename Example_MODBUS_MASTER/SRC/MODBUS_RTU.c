/*****************************************************************************
 * Copyright ©2021. Radiocrafts AS (Radiocrafts).  All Rights Reserved. 
 * Permission to use, copy, modify, and distribute this software and 
 * its documentation, without fee and without a signed licensing 
 * agreement, is hereby granted, provided that the above copyright 
 * notice, this paragraph and the following two paragraphs appear in 
 * all copies, modifications, and distributions.
 * 
 * IN NO EVENT SHALL RADIOCRFTS BE LIABLE TO ANY PARTY FOR DIRECT, 
 * INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING 
 * LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS 
 * DOCUMENTATION, EVEN IF RADIOCRAFTS HAS BEEN ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE. 
 * 
 * RADIOCRAFTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT 
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING 
 * DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". 
 * RADIOCRAFTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, 
 * UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
****************************************************************************/
/**
 @file
 @brief This is an MODBUS_RTU library files RIIM
 @version 0.01 : First revision
 @date 2021.11.23
*/


#include "RIIM_UAPI.h"
#include "MODBUS_RTU.h"


#define MaxPayloadLength 255
#define UARTtimeOut 3000
#define UARTBaudRate 38400
#define MODBUStimeOut 5000
#define RS485_TX_RX_control true
#define mySlaveAddress 0x40
#define master true

static MODBUSReceiveCallback rxCallback;
static MODBUSTransmitCallback txCallback;

static uint8_t	PDU_TX[MaxPayloadLength];
static uint8_t	PDU_RX[MaxPayloadLength];

static const uint32_t   timerPeriod=UARTtimeOut;

static uint8_t lastSlaveAddress;


/*** local functions ***/
void txHandler(uint8_t len);

void rxHandler(uint8_t *ADU, uint8_t ADU_length);
uint16_t calculateCRC(uint8_t *buffer, uint16_t len);

/*** local functions ***/
uint16_t calculateCRC(uint8_t *buffer, uint16_t len)
{
  uint16_t crc = 0xFFFF;
  uint16_t byte_number;
  uint16_t bit_counter;


  for (byte_number = 0; byte_number < len; byte_number++) {
    crc ^= (uint16_t)buffer[byte_number];         
  
    for (bit_counter = 0; bit_counter <8; bit_counter++) {    
      if ((crc & 0x0001) != 0) {      
        crc >>= 1;                    
        crc ^= 0xA001;
      }
      else                            
        crc >>= 1;                   
    }
  }
   return crc;  
}

void initModbus()
{
    UART.enableRXTX(RS485_TX_RX_control);  
    UART.init(UARTBaudRate, UART_PARITY_NONE, UART_DATA_8_BITS, UART_STOP_1_BIT);
}

void transmitModbus(uint8_t slave_address,uint8_t *PDU,uint8_t length, MODBUSTransmitCallback callback)
{    
    uint8_t ADU[255];
    uint8_t byte_counter;
    uint16_t CRC;
    uint8_t ADU_length;

    ADU_length=length+3;  //add 1 address byte and 2 CRC bytes
    
    //Use incoming slave address parameter if master, otherwise use the slave's own address
    if (master==true)    
    {
        ADU[0] = slave_address;
        lastSlaveAddress = slave_address;
    }
    else
    {
        ADU[0]=mySlaveAddress;
    }

    for (byte_counter=0;byte_counter<length;byte_counter++)
    {   
        ADU[byte_counter+1]=PDU[byte_counter];
    }

    CRC=calculateCRC(ADU,length+1);

    ADU[length + 2] = (uint8_t)(CRC>>8);
    ADU[length + 1] = (uint8_t)CRC;

    UART.startTransmit(ADU,ADU_length,txHandler);
    txCallback = callback;
}


void startRXModbus(MODBUSReceiveCallback callback)
{     
      UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
      rxCallback=callback;
}


void txHandler(uint8_t len)
{
    if ((!(master == true))||(lastSlaveAddress==0)) 
        {
            txCallback(NULL,0,0);
            return;
        }
    
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,MODBUStimeOut,UART_No_Termination_Character,false);   
}

void rxHandler(uint8_t *ADU, uint8_t ADU_length)
{
    uint16_t CRC_RX; 
    uint16_t local_CRC;
    uint8_t status;
    uint8_t rxAddress;

    rxAddress=ADU[0];

    if (ADU_length == 0)
    {
        return;
    }


    if (!(((rxAddress == mySlaveAddress)&&!(master==true))||(rxAddress==0)||((rxAddress == lastSlaveAddress)&&(master==true))))
    {
        status = 1;
        if (master==true)
        {
            txCallback(ADU,ADU_length,status); 
        }
        else
        {
            rxCallback(ADU,ADU_length,status);
        }
        return;
    }

    if (ADU_length<4)
    {
        status = 2;
        if (master==true)
        {
            txCallback(ADU,ADU_length,status); 
        }
        else
        {
            rxCallback(ADU,ADU_length,status);
        }
        return;
    }

    CRC_RX = (((uint16_t)(ADU[ADU_length-1]))<<8)|((uint16_t)(ADU[ADU_length-2]));
   
    local_CRC = calculateCRC(ADU,ADU_length-2);

    if (!(local_CRC==CRC_RX))
    {
        status = 3;
       
        if (master==true)
        {
            txCallback(ADU,ADU_length,status); 
        }
        else
        {
            rxCallback(ADU,ADU_length,status);
        }
        return;
    }

                
    for(uint16_t i=0;i<ADU_length;i++) 
        {
         ADU[i]=ADU[i+1];
        }

    status = 0;
    if (master==true)
    {  
        txCallback(ADU, ADU_length-3, status);
    }
    else
    {
        rxCallback(ADU, ADU_length-3, status);
    } 
}