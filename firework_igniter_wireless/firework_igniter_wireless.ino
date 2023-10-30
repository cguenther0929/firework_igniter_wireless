/*
 * This is the main source file for the ESP8266 WiFi SOM
 * 
*/

#include <ESP8266WiFi.h>
// #include <ArduinoJson.h>
#include <pins_arduino.h>
#include <Ticker.h>
// #include <WiFiClientSecure.h>
// #include "EMailSender.h"

/**
 * Uncomment the following 
 * if you wish to have logging 
 * data printed to the screen
 */
#define ENABLE_LOGGING

/**
 * JSON and input
 * data constructors
 */
// #define MAX_RX_BUF_INDEX          511
// #define MAX_RX_BUF_ELEMENTS       MAX_RX_BUF_INDEX + 1
// StaticJsonDocument<256> json_doc;
// DeserializationError json_err;
// char data_input_string[MAX_RX_BUF_ELEMENTS];

// FSM states
enum state {
  UNKNOWN,
  STATE_1,
  STATE_2,
  STATE_3,
  STATE_4,
  STATE_5,
  STATE_6
};

/**
 * State tracker 
 */
state current_state = UNKNOWN;

/**
 * The Sleep bit, driven
 * by the application processor,
 * defines when the WiFi module
 * shall go into deep sleep mode.  
 * Most likely, this bit will only
 * be looked at after booting.  
 */
// #define SLEEP_BIT                 4

/**
 * WiFi Parameters
 */
#define WIFI_CONNECT_TIMEOUT_S    10
WiFiClient client;

/**
 * Serial port 
 * parameters
 */
#define SER_TIMEOUT_MS            500

/**
 * Error LED
*/
#define WIFI_ERR_LED              12


/**
 * Data imported 
 * from JSON string
 */
char buf_wifi_password[32];
char buf_wifi_ssid[32];

// char buf_hyg_name[32];                  // Email address hygrometer used to send emails
// char buf_hyg_smtp2go_account[64];       // Email address hygrometer used to send emails
// char buf_hyg_smtp2go_password[64];      // Account password for aforementioned email address 
// char buf_recipient_email_addr[64];      // Email address of recipient
// char buf_sender_email_addr[64];         // Email address of sender

// #define TEMP_BUF_SIZE             64    // Size of the temporary buffer
// char buf_temp[TEMP_BUF_SIZE];           // Temporary buffer that can be used for building strings

// float humidity_1        = 0;
// float humidity_2        = 0;
// float temperature_1     = 0;
// float temperature_2     = 0;
// float battery_v         = 0;
// bool battery_too_low    = false; 

/**
 * Email related parameters
 */
// String email_message = "";
// char server[] = "mail.smtp2go.com";   //CJG added this line 7/27/22


/**
 * Timer parameters
 */
long            tmr1_write_val = 3030;   // Empirically derived to generate a 1ms tick timer.
unsigned int    ms_ticks_1;
unsigned int    ms_ticks_50;
unsigned int    ms_ticks_500;
unsigned int    ms_ticks_1000;

bool            Timer1msFlag = false;
bool            Timer50msFlag = false;
bool            Timer500msFlag = false;
bool            Timer1000msFlag = false;

long            seconds_counter = 0;        //32bit value 4.264....e9



void ICACHE_RAM_ATTR onTimerISR(){
  timer1_write(tmr1_write_val);

  Timer1msFlag = true;

  if(ms_ticks_1 == 50) {
    ms_ticks_1 = 0;
    Timer50msFlag = true;
    if(ms_ticks_50 == 10) {
      ms_ticks_50 = 0;               //Reset centi-seconds
      Timer500msFlag = true;
    
      if(ms_ticks_500 == 2) {         //See if we need to roll over
        ms_ticks_500 = 0;
        Timer1000msFlag = true;  
      }
      else {
        ms_ticks_500++;              //Increase 500ms timer
      }

    }
    else {
        ms_ticks_50++;
    }
    
  }
  else {
    ms_ticks_1++;
  }
  

}


void loop(void)
{

  if(Timer1msFlag == true) {
    Timer1msFlag = false;
    StateEvaluation();
  }
  
  if(Timer50msFlag == true) {
    Timer50msFlag = false;
  }

  if(Timer500msFlag == true) {
    Timer500msFlag = false;
  }

  if(Timer1000msFlag == true) {
    Timer1000msFlag = false;
    // digitalWrite(LED_1,!(digitalRead(LED_1)));  //Toggle LED Pin
    (seconds_counter == 300000)?(seconds_counter = 0):(seconds_counter++);
    
  }
}

void setup(void) {
  
  /**
   * Setup pin directions
   */
  // pinMode(SLEEP_BIT, INPUT);
  // pinMode(WIFI_ERR_LED, OUTPUT);

  // digitalWrite(WIFI_ERR_LED, LOW);

  #if defined(ENABLE_LOGGING)
    Serial.begin(9600);
    Serial.setTimeout(50);    //Timeout value in ms -- max is 1000
  #endif
  
  current_state = STATE_1;

  //Initialize Ticker every 0.05s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(tmr1_write_val);        //.05s 

  #if defined(ENABLE_LOGGING)
    delay(2000);
    Serial.println("Module just rebooted.");
  #endif

}
