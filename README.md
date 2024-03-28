# Firework Igniter, Wireless 

This repository contains source code that is to run on the ESP8266 SOM as found on the firework igniter.  Initially, the code contained within this repository was intended for the WiFi Test Board.  This code now 'lives' in a separate repository called `wifi_test_board_firework_igniter`.  

### ARCHITECTURE ###
The ESP8266 SOM can be bootloaded over the USB-to-serial interface. The Arduino IDE shall be used during development.   

### TOOL SUITE ###

The source file(s) are to be compiled using Arduino IDE v1.8.8, or newer.    

It is necessary to include esp8266 libraries. First, it must be verified that the correct board manager URL is included under the preference window.  Navigate to *File > Preferences*, and add the following URL: http://arduino.esp8266.com/stable/package_esp8266com_index.json

After including the necessary board manager URL, navigate to *tools > board > boards manager*, and install "esp8266".  This package will be considered to be *written by the "community"*.

For the asynchronous web sever, two libraries need to be manually downloaded and added so that <ESPAsyncWebServer.h> and <ESPAsyncTCP.h> can be included.  These libraries cannot be added through the board manager using conventional methods.

For ESPAsyncWebServer, download the file here.  A .ZIP file will be downloaded: <br>
https://github.com/me-no-dev/ESPAsyncWebServer/archive/master.zip <br>


For ESPAsyncTCP library, download the file here. A .ZIP file will be downloaded: <br>>
https://github.com/me-no-dev/ESPAsyncTCP/archive/master.zip <br>

Inside the sketch (Arduino IDE opened), click the option to include a library by navigating to the following option: *Sketch > Include Library > Add .ZIP Library...*  This will need to be repeated for each library we wish to add.  
The folders, which should be included at `\Documents\Arduino\libraries`, may have the -master suffix, but that's okay, as the sketch appears to compile and function fine.


### COMPILING ###
The ESP12 Test Board utilizes a bare ESP8266 WiFi SOM.  For proper compilation, the target board shall be  *Generic ESP8266 Module*.  

### VERSIONS ###
* V0.0.1.a -- This firework igniter code was revamped for the actual igniter board (before this, it was intended for the WiFi test board).   The GPIO expander routines were revamped and tested by toggling a front LED over the LAN connection.   


