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
 @file
 @brief This is a demonstration of a MODBUS loacl master in a mesh router. It is not a complete mester, but show how the underlying MODBUS_RTU drived can be used to access an MODBUS slave.
 @version 1.00 : First revision
 @date 2022.01.25
*/

#include "RIIM_UAPI.h"
#include "MODBUS_RTU.h"


#define MaxUDPPayloadLength 128
#define RadioChannel 32
#define UDPport 12345
#define OutputPower 14

/**
 * @brief When you receive "MaxPayloadLength" bytes or when you stop writing for "UARTtimeOut" milli seconds, payload is transmitted to the Border Router and its printed there via UART. Also prints whatever is received via UART
 */

static uint8_t payload[MaxUDPPayloadLength];

void rxHandler(uint8_t *buffer, uint8_t len);

static uint8_t blinkTimerHandler;

static void startup()
{
    // Wait until we are part of network
    if(Network.getNetworkState() != ONLINE){
        GPIO.toggle(GPIO_6);
        return;
    } else {
        GPIO.setValue(GPIO_6, LOW);
        Timer.stop(blinkTimerHandler);
        return;
    }
}

void modbusTxHandler(uint8_t *buffer, uint8_t len,uint8_t status)
{
    uint8_t i;
    //Util.printf("# RXD triggered\n");
    IPAddr Addr;
    if ((len>0)&&(status == 0))
    {
    if(Network.getNetworkState() != ONLINE){
        return;
        }

    if(Network.getBorderRouterAddress(&Addr)!=UAPI_OK)
        {
        return;
        }
     
    for(i=0;i<len;i++) 
        {
            payload[i]=buffer[i];
        }
         UDP.send6(Addr, UDPport, payload, len);
     }
    else
    {
    }
    
}

/**
 * @brief When we receive a UDP packet, print it out on UART
 */
void udpCallback(IPAddr src_ipAddr, uint8_t *payload, uint8_t payloadSize)
{
    transmitModbus(64,payload,payloadSize,modbusTxHandler);
    return;
}

/**
 * @brief This is the entry point of the user application. It is
 *        called only once during startup. It is responsible for
 *        setting up callbacks, timers etc.
 */


RIIM_SETUP()
{
    initModbus();
    const uint8_t nwKey[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    Network.setNWKey((uint8_t*)nwKey);
    Network.setPanId(3333);
    Network.startMeshRouter();
    //Network.setChannel(RadioChannel);
    Network.setTxPower(OutputPower);

    GPIO.setDirection(GPIO_6, OUTPUT);
    GPIO.setValue(GPIO_6, HIGH);
    blinkTimerHandler=Timer.create(PERIODIC, 500, startup);
    Timer.start(blinkTimerHandler);

    UDP.setup(udpCallback,UDPport);
    return UAPI_OK;
}
