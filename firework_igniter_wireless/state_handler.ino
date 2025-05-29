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
            current_screen = SCREEN_3;

        break;
        
        case SCREEN_3:
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

            if(ENABLE_LOGGING) 
            {
                Serial.println("\tIn Sate_1.\n");
            }
        
        break;

        /**
         * @brief WAITING FOR DATA STATE
         * 
         */
        case STATE_2:
            

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
        break;

        default:
            delay(1);
    }
}