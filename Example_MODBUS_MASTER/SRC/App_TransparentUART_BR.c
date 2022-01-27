
/*****************************************************************************
 * Copyright ©2020. Radiocrafts AS (Radiocrafts).  All Rights Reserved.
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
  * @brief 
  * 
  *
 @brief This is a demonstration of a MODBUS loacl master in a mesh router. This file is a copy of the ICI application for transparent UART in the SDK, but with different PAN ID.
 @version 1.00 : First revision
 @date 2022.01.25
*/


#include "RIIM_UAPI.h"
#define MaxPayloadLength 10
#define UARTtimeOut 300
#define RadioChannel 32
#define UDPport 12345
#define OutputPower 0
#define UARTBaudRate 115200


/**
 * @brief When you receive "MaxPayloadLength" bytes or when you stop writing for "UARTtimeOut" milliseconds, payload is transmitted to the Border Router and its printed there via UART. Also prints whatever is received via UART
 */

uint8_t					payload[MaxPayloadLength];
static uint8_t StartupTimerHandler;

const IPAddr 			Addr = UAPI_UDP_ALL_LOCAL_NODES;

void rxHandler(uint8_t *buffer, uint8_t len);

static void startup()
{
    Network.setTschMaxBroadcastRate(64);
}


void rxHandler(uint8_t *buffer, uint8_t len)
{
    int i;
    if(len>0)
    {
       for(i=0;i<len;i++)
       {
        payload[i]=buffer[i];
       }
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    UDP.send6(Addr, UDPport, payload, len);
    }
    else
    {
     UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    }
}

/**
 * @brief When we receive a UDP packet, print it out on UART
 */

void udpCallback(IPAddr src_ipAddr, uint8_t *payload, uint8_t payloadSize)
{
    Util.printf("%.*s", payloadSize, payload);
}


/**
 * @brief This is the entry point of the user application. It is
 *        called only once during startup. It is responsible for
 *        setting up callbacks, timers etc.
 */
RIIM_SETUP()
{
    UART.init(UARTBaudRate, UART_PARITY_NONE, UART_DATA_8_BITS, UART_STOP_1_BIT);
    //Util.printf("Starting RIIM Border Router Node for UART Transparent Mode\n");
    const uint8_t nwKey[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Network.setNWKey((uint8_t*)nwKey);
    Network.setPanId(3333);
    StartupTimerHandler=Timer.create(ONE_SHOT, 60000, startup);
    Timer.start(StartupTimerHandler);
    
    Network.startBorderRouter(NULL, NULL, NULL, NULL);
    Network.setChannel(RadioChannel);
    Network.setTxPower(OutputPower);
    Network.setTschMaxBroadcastRate(1); 
    UDP.setup(udpCallback,UDPport);
    UART.startReceive_Adv(rxHandler,MaxPayloadLength,UARTtimeOut,UART_Timeout_Infinite,UART_No_Termination_Character,false);   
    return UAPI_OK;
}


