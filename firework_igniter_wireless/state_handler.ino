#define HYG_NAME_KEY            "hyg_name"
#define HYG_SMTP2GO_ACC_KEY     "hyg_smtp2go_acc"
#define HYG_SMTP2GO_PASS_KEY    "hyg_smtp2go_pass"
#define RECIPIENT_EMAIL_KEY     "email_to"
#define SENDER_EMAIL_KEY        "email_from"
#define WIFI_SSID_KEY           "wifi_ssid"
#define WIFI_PASS_KEY           "wifi_pass"
#define HUM_1_KEY               "hum1"
#define TEMP_1_KEY              "temp1"
#define HUM_2_KEY               "hum2"
#define TEMP_2_KEY              "temp2"
#define BAT_V_KEY               "bat_v"
#define BAT_SATUS_KEY           "bat_low"

void StateEvaluation( void ) {      

    switch (current_state) {
        
        /**
         * @brief STATE EVALUATE SLEEP BIT
         * 
         */
        case STATE_1:

            #if defined(ENABLE_LOGGING)
                Serial.println("\tIn Sate_1.\n");
            #endif
            /**
             * If sleep bit is set, 
             * transition to STATE_6
             * state
             */

            // if(digitalRead(SLEEP_BIT)) {     
            //     current_state = STATE_6;
            // }

            // else {
            //     current_state = STATE_2;
            // }
        
        break;

        /**
         * @brief WAITING FOR DATA STATE
         * 
         */
        case STATE_2:
            
            // if (Serial.available() > 0){
                                
            //     SerialReadtoArray ();       // Blocking

            //     #if defined(ENABLE_LOGGING)
            //         Serial.print("\Received string: "); Serial.println(data_input_string);
            //     #endif
            
            //     current_state = STATE_3;
            // }  //Wait until serial data is available

            /**
             * ELSE
             * we continue to wait for new data...
             */

        break;
        
        /**
         * @brief PARSE INPUT DATA STATE
         * 
         */
        case STATE_3:
        {
            // int temp_length = 0;
            // int b64len = 0;

            // #if defined(ENABLE_LOGGING)
            //     Serial.println("\tParsing data.\n");
            // #endif

            // json_err = deserializeJson(json_doc,data_input_string);
            
            // if(json_err) {

            //     #if defined(ENABLE_LOGGING)
            //         Serial.println("Json deserialize error.");
            //         Serial.print("Error: "); Serial.println(json_err.c_str());
            //     #endif
                
            //     FlushSerialRXBuffer();
            //     current_state = STATE_2;
            //     break;
            // }

            // /**
            //  * Retrieve the hygrometer's 
            //  * name 
            //  */
            // const char * cc_hyg_name = json_doc[HYG_NAME_KEY];
            // memcpy(buf_hyg_name, cc_hyg_name, strlen(cc_hyg_name));
            
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- Email address passed in via JSON: ");
            //     Serial.println(buf_hyg_name);
            // #endif
            
            // /**
            //  * Retrieve the hygrometer's email
            //  * address as passed in via JSON
            //  */
            // const char * cc_hyg_smtp2go_acc = json_doc[HYG_SMTP2GO_ACC_KEY];
            // memcpy(buf_hyg_smtp2go_account, cc_hyg_smtp2go_acc, strlen(cc_hyg_smtp2go_acc));
            
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- Email address passed in via JSON: ");
            //     Serial.println(buf_hyg_smtp2go_account);
            // #endif


            // /**
            //  * Retrieve the hygrometer's SMTP2GO
            //  * accounts password as passed in 
            //  * via JSON
            //  */
            // const char * cc_hyg_smtp2go_pass = json_doc[HYG_SMTP2GO_PASS_KEY];
            // memcpy(buf_hyg_smtp2go_password, cc_hyg_smtp2go_pass, strlen(cc_hyg_smtp2go_pass));
            
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- Email login password passed in via JSON: ");
            //     Serial.println(buf_hyg_smtp2go_password);
            // #endif

            // /**
            //  * Retrieve the recipient's
            //  * email address as passed in 
            //  * via JSON
            //  */
            // const char * cc_recipient_email_addr = json_doc[RECIPIENT_EMAIL_KEY];
            // memcpy(buf_recipient_email_addr, cc_recipient_email_addr, strlen(cc_recipient_email_addr));
            
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- Recipient email address passed in via JSON: ");
            //     Serial.println(buf_recipient_email_addr);
            // #endif
            
            // /**
            //  * Retrieve sender's email
            //  * address 
            //  */
            // const char * cc_sender_email_addr = json_doc[SENDER_EMAIL_KEY];
            // memcpy(buf_sender_email_addr, cc_sender_email_addr, strlen(cc_sender_email_addr));
            
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- Sender email address passed in via JSON: ");
            //     Serial.println(buf_sender_email_addr);
            // #endif

            // /**
            //  * Retrieve WiFi SSID
            //  * as passed in via JSON
            //  */
            // const char * cc_wifi_ssid = json_doc[WIFI_SSID_KEY];
            // memcpy(buf_wifi_ssid, cc_wifi_ssid, strlen(cc_wifi_ssid));

            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- WiFi SSID passed in via JSON: ");
            //     Serial.println(buf_wifi_ssid);
            // #endif

            // /**
            //  * Retrieve WiFi password
            //  * as passed in via JSON
            //  */
            // const char * cc_wifi_pass = json_doc[WIFI_PASS_KEY];
            // memcpy(buf_wifi_password, cc_wifi_pass, strlen(cc_wifi_pass));

            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- WiFi password passed in via JSON: ");
            //     Serial.println(buf_wifi_password);
            // #endif

            // /**
            //  * Retrieve the first humidity
            //  * value as passed in via JSON
            //  */
            // humidity_1 = json_doc[HUM_1_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- humidity 1 value passed in via JSON: ");
            //     Serial.println(humidity_1);
            // #endif
            
            // /**
            //  * Retrieve the second humidity
            //  * value as passed in via JSON
            //  */
            // humidity_2 = json_doc[HUM_2_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- humidity 2 value passed in via JSON: ");
            //     Serial.println(humidity_2);
            // #endif

            // /**
            //  * Retrieve the first temperature 
            //  * value as passed in via JSON
            //  */
            // temperature_1 = json_doc[TEMP_1_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- temperature 1 value passed in via JSON: ");
            //     Serial.println(temperature_1);
            // #endif
            
            // /**
            //  * Retrieve the first temperature 
            //  * value as passed in via JSON
            //  */
            // temperature_2 = json_doc[TEMP_2_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- temperature 2 value passed in via JSON: ");
            //     Serial.println(temperature_2);
            // #endif
            
            // /**
            //  * Retrieve battery voltage  
            //  * value as passed in via JSON
            //  */
            // battery_v = json_doc[BAT_V_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- battery voltage passed in via JSON: ");
            //     Serial.println(battery_v);
            // #endif
            
            // /**
            //  * Retrieve battery too low
            //  * flag as passed in via JSON
            //  */
            // battery_too_low = json_doc[BAT_SATUS_KEY];
            // #if defined(ENABLE_LOGGING)
            //     Serial.print("\tSTATE HDLR -- battery too low flag passed in via JSON: ");
            //     Serial.println(battery_too_low);
            // #endif
            
            // current_state = STATE_4;          
        
        }
        break;
        
        /**
         * @brief STATE NETWORK CONNECTION
         * 
         */
        case STATE_4:
            
            // WiFi.forceSleepWake();
            // delay(1);                       // There are claims that a non-zero delay is required after calling the wake function

            // if(!WiFiConnect(buf_wifi_ssid, buf_wifi_password)) {
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println("Deep sleep.");
            //     #endif
            //     current_state = STATE_6;
            // }

            current_state = STATE_5;

        break;

        /**
         * @brief SEND EMAIL STATE
         * 
         */
        case STATE_5:
        {
            // AssembleEmailMessage();

            /**
             * @brief Make a client connection
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Making client connection"));
            // #endif
            // if (client.connect(server, 2525) == 1) {
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("connected"));
            //     #endif
            // } else {
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("connection failed"));
            //     #endif
            //     return;
            // }
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error connecting to the client"));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;        
            //     break;
            // }


            /**
             * @brief Sending EHLO command
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Sending EHLO"));
            // #endif
            // client.println("EHLO www.example.com");
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error sending EHLO command."));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }
            

            /**
             * @brief Sending auth login command
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Sending auth login"));
            // #endif

            // client.println("auth login");
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error sending AUTH LOGIN command."));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }

            
            /**
             * @brief Send SMTP2GO User Account Credentials
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.print(F("Sending SMTP2GO B64 User Account Name: "));
            //     Serial.println(buf_hyg_smtp2go_account);
            // #endif
            
            // client.println(buf_hyg_smtp2go_account); //B64 encoded SMTP2GO username
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.print(F("\t*** Error sending SMTP2GO Username: "));
            //         Serial.println(buf_hyg_smtp2go_account);
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }


            /**
             * @brief Send SMTP2GO Password
             * 
             * THis should be the BASE64 password 
             * for you SMTP2GO account
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.print(F("Sending B64 SMTP2GO Password: "));
            //     Serial.println(buf_hyg_smtp2go_password);
            // #endif

            // client.println(buf_hyg_smtp2go_password);  
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error sending SMTP2GO password"));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }
            

            /**
             * @brief Command for MAIL From:
             * i.e.  --> client.println(F("MAIL From: clinton.debug@gmail.com"));
             * 
             */
            // memset(buf_temp, 0, TEMP_BUF_SIZE);
            // strcpy(buf_temp, "MAIL From: ");
            // strcat(buf_temp, buf_sender_email_addr);
            // client.println(buf_temp);
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.print(F("\t*** Error on command: "));
            //         Serial.println(buf_temp);
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }
            

            /**
             * @brief Enter recipient address
             * First, fill temp buffer with null characters
             * i.e.  -->  client.println(F("RCPT To: clinton.guenther@gmail.com"));
             * 
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.print(F("Sending To: "));
            //     Serial.println(buf_recipient_email_addr);
            // #endif
            
            // memset(buf_temp, 0, TEMP_BUF_SIZE);
            // strcpy(buf_temp, "RCPT To: ");
            // strcat(buf_temp, buf_recipient_email_addr);
            
            // client.println(buf_temp);
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.print(F("\t*** Error on command: "));
            //         Serial.println(buf_temp);
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }
            

            /**
             * @brief Send DATA command
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Sending DATA"));
            // #endif

            // client.println(F("DATA"));
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error on command \"DATA\"."));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }


            /**
             * @brief Sending To: command
             * i.e.  --> client.println(F("To: clinton.guenther@gmail.com"));
             */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Sending email"));
            // #endif
            // //  client.println(F("To:  clinton.guenther@gmail.com"));
            // memset(buf_temp, 0, TEMP_BUF_SIZE);
            // strcpy(buf_temp, "To: ");
            // strcat(buf_temp, buf_recipient_email_addr);
            // client.println(buf_temp);


            // /**
            //  * @brief Sending From: command
            //  * i.e. -->  client.println(F("From: clinton.debug@gmail.com"));
            //  */
            // // client.println(F("From: clinton.debug@gmail.com"));
            // memset(buf_temp, 0, TEMP_BUF_SIZE);
            // strcpy(buf_temp, "From: ");
            // strcat(buf_temp, buf_sender_email_addr);
            // client.println(buf_temp);


            // /**
            //  * @brief Send the subject
            //  */
            // client.println(F("Subject: Hygrometer Health Report\r\n"));
            // client.println(email_message);
            // client.println(F("."));
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error sending DOT to complete transaction"));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }


            // /**
            //  * @brief Sending QUIT
            //  * 
            //  */
            // #if defined(ENABLE_LOGGING)
            //     Serial.println(F("Sending QUIT"));
            // #endif
                        
            // client.println(F("QUIT"));
            // if (!eRcv()){
            //     #if defined(ENABLE_LOGGING)
            //         Serial.println(F("\t*** Error sending \"QUIT\"."));
            //     #endif
            //     FlushSerialRXBuffer( );
            //     current_state = STATE_6;
            //     break;
            // }
            
            
            /**
             * @brief Disconnecting 
             */
            //     client.stop();
                
            //     #if defined(ENABLE_LOGGING)
            //     Serial.println(F("disconnected"));
            //     #endif

            //     current_state = STATE_6;
        }
        break;

        /**
         * @brief DEEP SLEEP STATE
         */
        case STATE_6:
            // digitalWrite(WIFI_ERR_LED, LOW);
            
            // /**
            //  * Put module into deep sleep.
            //  * Only a reset can wake the module!
            //  */
            // ESP.deepSleep(0);
        break;

        default:
            delay(1);
    }
}