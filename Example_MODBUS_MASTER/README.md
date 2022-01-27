<!-- pandoc -V geometry:margin=1cm -V fontsize=12pt -V fontfamily=utopia README.md -o README.pdf -->

# RIIM UART example

## Description
This example demonstrates how to send and receive UDP packets. Only the DB (Mesh Router)
board and the Border Router are needed for this example.



## To use this example:
- Set up the computer as described in the user manual
- Use the **Compile_And_Upload** script or use MAKE manually to generate and upload the images
- Connect the boards to serial terminal (default baudrate 115200, 8N1)
- Observe the outputs:
    - Border Router will print out all messages received along the source IPv6 address
    - Mesh Router will print out all messages sent and received