Acronyms
========

=========== ===========================================
UART        Universal Asynchronous Receiver Transmitter
SLIP        Serial Line IP
RTU         Remote Terminal Unit
PLC         Programmable logic controller
=========== ===========================================

Description
===========
MODBUS is a serial line communication protocol used in many industrial use cases. Traditionally at has been run over RS232 and RS485 (called Modbus RTU), but is a also availeble over IP with Modbus/TCP.
There are several options for integrating RIIM with Modbus.
In this example the option is to use MODBUS locally between a mesh router(MODBUS/RTU master) and a MODBUS slave device.
This means the data sent via the RIIM network is not complete MODBUS/RTU frames. This means that MODBUS communiation is only local between the mesh router and the MODBUS slave. 
After the MODBUS responses are received in the mesh router, local intellegence decide what to do with the data. It can be processed locally and checked for threashold, but in this example the data is just forwarded in raw format to the border router.

There is a different examples availeble showing transparent modbus packet tranfered over RIIM.

The main challenges and tasks are therefore these:
   - Addressing: The MODBUS requests must include the MODBUS address of the slave and the reponse must verify that correct device reply.
   - CRC generation and verification

This example provides a possible solution to these challenges and tasks

System Overview
---------------
This image shows the system.

.. mermaid::

   graph LR
      idRPI(PC) -- UART --- idBR(BorderRouter)
      idBR(BorderRouter) -- RF --- idMR1(MeshRouter)
      idBR(BorderRouter) -- RF --- idMR2(MeshRouter)
      idMR2(MeshRouter) -- MODBUS/RTU --- idModbus1(Modbus Device)
      idMR1(MeshRouter) -- MODBUS/RTU --- idModbus2(Modbus Device)
          
For mesh router and the MODBUS communication an RS485 driver is required. 
The modbus slave need separate power.

Topics not covered in this example
----------------------------------
These topics are not covered in this example, but could be relevant in a real world deployment

- Communication to more then one MODBUS slave
- Proceccing received data (mist computing) 
- Commisioning (See separate application node for this on www.radiocrafts.com.)

RIIM network nodes
------------------

There are two ICI applications provided:

- Border Router application
   - Setup RIIM TSCH network
   - Send and receive transparent UART packets, containing the mdobus packet without address and CRC
   
- Mesh Router application
   - Join RIIM TSCH network
   - Includes function abstractions to initialize, transmit and receive modbus requests/responses
   - A dummy applicaiton is included that 
      - Received UDP packets
      - Add MODBUS address + CRC
      - Send on UART/RS485
      - Receive response and verify correct address and CRC
      - Transmit core data as UDP to border router
     
