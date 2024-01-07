/* Setup initial state conditions */
// bool WiFiConnect( const char * ssid, const char * password ) {
//     int i = 0;
//     int timeout = (int)WIFI_CONNECT_TIMEOUT_S/0.5;
    
    
//     WiFi.begin(ssid, password);

//     /**
//      * Print diagnostic data
//      * for WiFi if logging
//      * is enabled
//      */
//     #if defined(ENABLE_LOGGING)
//         Serial.println("");
//         Serial.print("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
//         Serial.print("\tConnecting to SSID: "); Serial.println(ssid);
//         Serial.print("\tSSID password: "); Serial.println(password);
//     #endif

    
//     // Wait for connection
//     Serial.print("\tWiFi Connecting\t");
//     while ((WiFi.status() != WL_CONNECTED) && i < timeout) {
//         delay(500);
//         i++;
//         Serial.print('.');
//     }
//     Serial.println("");

//     if(i == timeout){
//     #if defined(ENABLE_LOGGING)
//         Serial.println("\tWiFi Connection timeout!");
//         return false;
//     }
//     #endif

//     #if defined(ENABLE_LOGGING)
//         Serial.println("\tWiFi connected!");
//         Serial.print("\tMy local IP: ");
//         Serial.println(WiFi.localIP());
//     #endif

//     return true;

  
// }

// byte eRcv()
// {
//   byte respCode;
//   byte thisByte;
//   int loopCount = 0;

//   while (!client.available()) {
//     delay(1);
//     loopCount++;
//     // if nothing received for 10 seconds, timeout
//     if (loopCount > 10000) {
//       client.stop();
//       Serial.println(F("\r\nTimeout"));
//       return 0;
//     }
//   }

//   respCode = client.peek();
//   while (client.available())
//   {
//     thisByte = client.read();
//     Serial.write(thisByte);
//   }

//   if (respCode >= '4')
//   {
//     //  efail();
//     return 0;
//   }
//   return 1;
// }

// void FlushSerialRXBuffer( void ){
//     char rxchar;    
    
//     while(Serial.available() > 0) {   //Flush the buffer 
//         rxchar = Serial.read();
//     }
// }

// void SerialReadtoArray ( void ) {
//     int i;
//     char rxchar;
//     int timeout;
    
//     memset(&data_input_string,'\0', MAX_RX_BUF_ELEMENTS);

//     rxchar = '\0';
//     i = 0;
//     timeout = 0;


//     while(rxchar != '}') {

//         if(Serial.available()) {
//             rxchar = Serial.read();
//             data_input_string[i] = rxchar;
//             i++;
            
//             if(i > MAX_RX_BUF_INDEX) {
//                 #if defined(ENABLE_LOGGING)
//                     Serial.println("\tSerial overflow.");
//                 #endif
                
//                 break;
//             }

//         }
//         else{
//             timeout++;
//             delay(1);
//             if(timeout == SER_TIMEOUT_MS) {
//                 #if defined(ENABLE_LOGGING)
//                     Serial.println("\t Serial receive timeout!");      
//                 #endif
//                 break;
//             }

//         }

//     }

//     if(timeout == SER_TIMEOUT_MS) {
//         #if defined(ENABLE_LOGGING)
//             Serial.println("\t Serial receive timeout!");      
//         #endif
//     }
    
//     FlushSerialRXBuffer();

// }

//  void AssembleEmailMessage ( void ) {
    /**
     * NOTE: Email message will look 
     * something like the following
     * Hygrometer's Name: "My Name is.."
     * Humidity 1: 65.23
     * Humidity 2: 67.25
     * Temperature 1: 73.45
     * Temperature 2: 73.44
     * Battery voltage: 3.25
     * Battery Low: false
     */
    // email_message =     "Hygrometer: " + String(buf_hyg_name) + "\r\n";
    // email_message +=    "Humidity 1: " + String(humidity_1) + " % \r\n";
    // email_message +=    "Humidity 2: " + String(humidity_2) + " % \r\n";
    // email_message +=    "Temperature 1: " + String(temperature_1) + " \u00B0 F \r\n";
    // email_message +=    "Temperature 2: " + String(temperature_2) + " \u00B0 F \r\n";
    // email_message +=    "Battery Voltage: " + String(battery_v) + " V \r\n";
    // if(battery_too_low) {
    //     email_message +=    "Battery Low: True \r\n";
    // }
    // else {
    //     email_message +=    "Battery Low: False \r\n";
    // }

    
// }

    bool clear_gpio (void) {
        Wire.beginTransmission(max3725_7b_address_o8o15); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();
        
        
        Wire.beginTransmission(max3725_7b_address_p0p7); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();

        return true;

    }


/**
 * @brief Set a GPIO from number passed in
 * 
 * Value passed in shall be 1-16 
 * (not 0 to 15)
 * 
 * @return true or false
*/

    bool set_gpio (uint8_t number) {
        int address = 0;
        uint8_t shift_value=0;
        uint8_t value = 0x00;

        /**
         *  The number can't be negative 
         * Make sure the value isn't larger 
         * than 16
        */
        if(number > 16) {
            number = 0;
        }

        if(number > 8) {
            shift_value = number - 9;
        }
        else {
            shift_value = number - 1;
        }

        (number > 8)?(address = max3725_7b_address_o8o15):(address = max3725_7b_address_p0p7);

        /* The IO needs to be constrained to 0-7*/
        value = (0b00000001) << (shift_value);

        Wire.beginTransmission(address); // transmit to device #4
        Wire.write(value);        // send 0b0000 twice so the value takes
        Wire.endTransmission();

        return true;
    }