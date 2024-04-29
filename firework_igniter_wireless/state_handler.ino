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

void screen_evaluation( void ) {
    switch (current_screen) {
        /**
         * @brief Screen one is defined
         * 
         */
        case SCREEN_1:
            current_screen = SCREEN_2;

        break;

        case SCREEN_2:
            current_screen = SCREEN_1;

        break;

        default:
            current_screen = SCREEN_1;
    }
}

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
            
        
        }
        break;
        
        /**
         * @brief STATE NETWORK CONNECTION
         * 
         */
        case STATE_4:
            

            current_state = STATE_5;

        break;

        /**
         * @brief SEND EMAIL STATE
         * 
         */
        case STATE_5:
        {



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