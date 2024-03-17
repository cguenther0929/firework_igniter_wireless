# Firework Igniter, Wireless 

This repository contains source code that is to run on the ESP8266 SOM as found on the firework igniter.  

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
* V0.0.1.a -- The GPIO expander is working well. Functions have been defined that will clear all outputs, or allow the user to define (1 through 16) which output shall be active.  The asynchronous server is working, but many more switches need to be defined.  

* v0.0.2.a -- All 16 fuses now show up on the browser.  Need to figure out how to make sure they default to LOW.  Also, it needs to be determined how to display the SW version on the webpage in a way that is more automatic.  

* v0.1.0.a -- A value of 100 was added to all IO so as to get away from built in IO.  It doesn't seem obvious as to what's going on under the hood, but sliders seem to be heavily tied to Arduino IO.  Thus if an output is on, the slider is on.  Taking GPIO 1 as an example, GPIO 1 is now 101 (avoiding built-in IO). This allows the operation to focus on GPIO expander IO.  All sliders now default to OFF.  The timeout feature kind of works, but needs to be improved.  

* v0.1.1.a -- The timeout feature was improved -- now the timer only starts running when an output transitions from inactive to active.  There is now a form on the bottom of the webpage that allows the user to enter a value, however, the code needs to be written to accept an updated value.  When submit is clicked, it seems the webpage goes awry.  

*v0.1.2.a -- A lot of button progress was made, but clicking on SUBMIT still doesn't properly submit the value.  In the form definition, it seems the page will become unresponsive if the method is defined to be post instead of get.

