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
 @brief Glueing layer for MODBUS transmission and reception for RIIM
*/

/**
 * @brief User callback when transmission complete
 *
 * Callback to be implemented by user and passed into transmitModbus().
 * The callback is triggered when the transmission is complete.
 *
 * @param[in] len: the number of bytes transmitted
 */
typedef void (*MODBUSTransmitCallback)(uint8_t *buffer, uint8_t len,uint8_t status);

/**
 * @brief User callback when bytes are received
 *
 *
 * Callback to be implemented by user, and passed in to startRXModbus().
 * The callback is triggered when the wanted number of bytes are received.
 *
 * @param[in] buffer: the buffer containing the bytes read
 * @param[in] len: the number of bytes read. May differ from the number of bytes wanted to read
 */
typedef void (*MODBUSReceiveCallback)(uint8_t *buffer, uint8_t len,uint8_t status);

/**
 * @brief Function to transmit a MODBUS request via UART/RS485
 * 
 * 
 * @param[in] slave_address: modbus address
 * @param[in] PDU: the buffer for the modbus frame (excluding address and CRC)
 * @param[in] length: the number of bytes transmitted
 * @param[in] txCallback: callback to be run when transmission is done
 */

void transmitModbus(uint8_t slave_address,uint8_t *PDU,uint8_t length,MODBUSTransmitCallback txCallback);

/**
 * @brief Function to setup reception of a MODBUS packet via UART/RS485
 * 
 *
 * @param[in] rxCallback: callback to be run when reception is done
 */
void startRXModbus(MODBUSReceiveCallback rxCallback);

/**
 * @brief Function to initialize a UART for MODBUS commands via UART/RS485
 * 
 */
void initModbus();