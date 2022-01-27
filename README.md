# RIIM_MODBUS_MASTER_Example

## Description
This is an example on how a mesh router in RIIM can act as an modbus master and read out data from one or more modbus/rtu slave and send read data through the riim network.
It is differnet than the "transparent modbus" example where the riim network just forward the modbus packet transparently ([https://radiocrafts.github.io/TRANSPARENT_MODBUS_Example]).
In this example the addressing, CRC generation and CRC check is handled locally in the mesh router. This means that a local intellegence in the mesh router is needed to determine which data to send, how often to read the sensor etc. The current example read a Tibbo BP#01 Modbus temperature sensor. The data can be sent as a COAP packet to the border router or a gateway. But currently the example only include a simple UDP transmssion to the border router.

---

This is not a full modbus master implementation, but the basic low level functions to handle adressing and CRC.

---

## Requirements
|                   |                   |
| ----------------- | ----------------- |
| RIIM SDK          | 3.1.0 or later    |
| RS485 driver      | RS485 breakout board from Sparkfun  |
| Modbus device     | Tibbo BP#01 or similar |
| External power supply | For RS485 driver and modbus device  |


## Downloading
Download the source, either by cloning the repository like this:

`
git clone https://github.com/radiocrafts/RIIM_Modbus_Master_Example
`

Copy the catalog \Example_MODBUS_MASTER\ to you SDK under ICI_Applications\


## Full Documentation
Please see the documentation located here:
[https://radiocrafts.github.io/MODBUS_MASTER_Example]

## Contact
Contact Radiocrafts AS at [https://radiocrafts.com/contact/]