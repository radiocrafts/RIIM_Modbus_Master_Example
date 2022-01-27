Troubleshooting
===============

How do I find the serial port used for the Border Router
--------------------------------------------------------

In Linux, the drivers for the USB-to-UART IC (FTDI FT-series) is built into the kernel. When a new board is connected, a new device will automatically appear in the **/dev/** folder. The name will be ttyUSB followed by a number, which start at zero for the first device enumerated. Therefore:

- The first device enumerated will get the device **/dev/ttyUSB0**
- The next will get the device **/dev/ttyUSB1**
- ....and so on.
