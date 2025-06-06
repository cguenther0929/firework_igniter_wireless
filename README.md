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

For the OLED, it is imperative to install the U8g2 library *which was written by Oliver*. This library can be installed from within the Arduino tool suite by simply navigating to *Sketch > Include Library > Manage Libraries...*.  Then one has to simple search for an install the U8g2 library.  
<br>

Inside the sketch (Arduino IDE opened), click the option to include a library by navigating to the following option: *Sketch > Include Library > Add .ZIP Library...*  This will need to be repeated for each library we wish to add.  
The folders, which should be included at `\Documents\Arduino\libraries`, may have the -master suffix, but that's okay, as the sketch appears to compile and function fine.


### COMPILING ###
The ESP12 Test Board utilizes a bare ESP8266 WiFi SOM.  For proper compilation, the target board shall be  *Generic ESP8266 Module*.  

### VERSIONS ###
* v0.0.1.a -- This firework igniter code was revamped for the actual igniter board (before this, it was intended for the WiFi test board).   The GPIO expander routines were revamped and tested by toggling a front LED over the LAN connection.   

* v0.0.2.a -- Still testing.  The LED will come on only if the web-switch transitions from OFF to ON. The IO number, for each of the external LEDs, has been defined.  The SW string was updated. It has been verified that the OLED is properly driven.   

* 0.1.0.a -- Multiple screens are now functioning.  The DAC output appears to be working, but the algorithms to set the output are not correct.   

* 0.1.1.a -- The function to set the fuse current (in mA) is functioning.  In general, routines involving the DAC are functioning well.  

* 0.1.2.a -- Functions to set and clear GPIO are functioning.  Only the bit of interest is set or cleared in functions set_ioxpander_gpio and clear_ioxpander_gpio. 

* 0.2.0.a -- Enabling an output by way of enabling the appropriate analog switch is working.  LEDs 1-16 were illuminated on the tester (DAC was already working).  This version has a hard-coded analog current of 800mA. Aside from a hard-coded fuse current, this version could by utilized to run the show.  

* 0.2.1.a -- The "ARM" switch was implemented that allows the user to quickly toggle between MIN and MAX fuse current.  When the system is "armed" the RED LED at, the front of the enclosure, will illuminate. 

* 0.3.2.a -- Fuse verification features have been rolled in, but don't seem to be working.  All logging features were turned off.  This version can be used for the show.  

* 0.3.3.a -- A timing issue related to fuse ignition was corrected. Said issue was that the dwell time for how long a fuse igniter was energized was too short due to the fuse checking routine.  The long-term solution will be to put the fuse check routine inside a conditional so that it doesn't run when a fuse is being ignited.  In the interim and in this version, however, the fuse checking routine has been disabled. 

* 0.3.4.a -- The fuse checking routine is not in perfect working order, but this version is being tagged since the ADCs appear to be working decent. Also, there has been enough changes to justify a tag.   

* 0.3.4.b -- The fuse checking routine appears to be working.  Code was cleaned up.  The third display screen shows the health of the fuses (as a sixteen bit binary value).  The act of checking fuses (which is a blocking function) was causing incoming HTML message to get missed, thus causing the device to miss the command of lighting a fuse.  This was mitigated by introducing a check count.  The fuses are checked ten times upon boot, but that's it.  The act of arming the device will no longer start the fuse disable timer.  