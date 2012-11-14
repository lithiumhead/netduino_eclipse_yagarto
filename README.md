netduino_eclipse_yagarto
========================

Simple C programs for netduino using YAGARTO+Eclipse on Windows.
This repository is an Eclipse Workspace.
I have used the following software tools to develop and test programs present in this repository:
- A Windows PC with 3 free USB ports (I used Windows 7 Home Premium 64 bit SP1)
- Eclipse IDE for Automotive Software Developers (I used juno SR1  32 bit for Windows)
- Atmel SAM-BA for Windows (I used 2.12)
- YAGARTO GNU ARM toolchain (I used 13.10.2012)
- YAGARTO Tools (I used 18.10.2012)

Instructions:
-------------
Instructions on setting up the hardware and software tools environment is documented on my blog here: 
The blog post above documents how to create your own Eclipse workspace and run blinky on your netduino.
After you have done that, just delete that Eclipse workspace and clone a copy of this repository on your Windows PC using "Github for Windows" and try out all the programs contained here in.

Dscription of the programs contained in this repository
-------------------------------------------------------

- **schematic** This folder does not contain any source code. It contains a PDF and Fritzing source of the connection diagram of my hardware setup that I used to develop these programs.
- **blinky** Blinks the SMD LED present on the netduino board. Uses a delay loop.
- **serial** Sends "Hello World" out on USART0 @ 115200 8-N-1. A USB to UART transceiver (e.g. FT232) must be connected to appropriate pins of USART0. (Read blog post linked to above)
- **serial_printf** Sends "Hello World" out on USART0 @ 115200 8-N-1. Uses printf() to do so. The out put of printf() is directed to USART0. A USB to UART transceiver (e.g. FT232) must be connected to appropriate pins of USART0. (Read blog post linked to above)
- **SW_LED** Toggle the SMD LED present on the netduino board with the switch which is also present on board the netduino.




