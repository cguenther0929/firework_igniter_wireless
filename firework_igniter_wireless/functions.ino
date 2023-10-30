/* Setup initial state conditions */
bool WiFiConnect( const char * ssid, const char * password ) {
    int i = 0;
    int timeout = (int)WIFI_CONNECT_TIMEOUT_S/0.5;
    
    
    WiFi.begin(ssid, password);

    /**
     * Print diagnostic data
     * for WiFi if logging
     * is enabled
     */
    #if defined(ENABLE_LOGGING)
        Serial.println("");
        Serial.print("\tMy MAC address is: "); Serial.println(WiFi.macAddress());
        Serial.print("\tConnecting to SSID: "); Serial.println(ssid);
        Serial.print("\tSSID password: "); Serial.println(password);
    #endif

    
    // Wait for connection
    Serial.print("\tWiFi Connecting\t");
    while ((WiFi.status() != WL_CONNECTED) && i < timeout) {
        delay(500);
        i++;
        Serial.print('.');
    }
    Serial.println("");

    if(i == timeout){
    #if defined(ENABLE_LOGGING)
        Serial.println("\tWiFi Connection timeout!");
        return false;
    }
    #endif

    #if defined(ENABLE_LOGGING)
        Serial.println("\tWiFi connected!");
        Serial.print("\tMy local IP: ");
        Serial.println(WiFi.localIP());
    #endif

    return true;

  
}

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

// void AssembleEmailMessage ( void ) {
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
