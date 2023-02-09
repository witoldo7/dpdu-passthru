# dpdu-passthru
D-PDU API driver for J2534 compatible devices. Currently supports only GM Tech2Win software.

# Currently working
* XHorse Mini-VCI J2534 device
* Tech2Win
  * Saab 9-5 with K-Line diagnostics (pre-2006 year models). The pins 7 and 8 of the DLC need to be shorted in order to access all diagnostic units in the car. Tested with MY2000.
  * Possibly other Saab models which have only K-Line diagnostics

# Todo
* Abstraction for J2534 devices (+ support for other devices)
* Find out why Saab 9-5 post-2006 fails communication with DICE through the K-Line 
* ISO15765 / CAN support

# Instructions
1. Build the project with Visual Studio 2022, using the Release build configuration
2. Install the driver by running the .bat installer as administrator: https://github.com/JohnJocke/dpdu-passthru/blob/master/installer/install.bat
