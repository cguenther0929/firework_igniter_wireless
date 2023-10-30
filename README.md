# Firework Igniter, Wireless 

This repository contains source code that is to run on the ESP8266 SOM as found on the firework igniter.  

### ARCHITECTURE ###
The ESP8266 SOM can be bootloaded over the USB-to-serial interface. The Arduino IDE shall be used during develpment.   

### TOOL SUITE ###

The source file(s) are to be compiled using Arduino IDE v1.8.8, or newer.    

It is necessary to include esp8266 libraries. First, it must be verified that the correct board manager URL is included under the preference window.  Navigate to *File > Preferences*, and add the following URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json

After including the necessary board manager URL, navigate to *tools > board > boards manager*, and install "esp8266".  This package will be considered to be *written by the "community"*.

### COMPILING ###
The ESP12 Test Board utilizes a bare ESP8266 WiFi SOM.  For proper compilation, the target board shall be  *Generic ESP8266 Module*.  

### VERSIONS ###
* V0.0.1 -- TBD
