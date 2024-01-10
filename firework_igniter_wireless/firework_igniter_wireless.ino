/*
 * This is the main source file for the ESP8266 WiFi SOM
 * 
 * TODO: make this less manual <h2>Firework Igniter 0.0.1</h2>
 * TODO: need to figure out how to prevent certain IO from defaulting to ON
*/

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <pins_arduino.h>
#include <Ticker.h>
#include <U8x8lib.h>
#include <Wire.h>
// #include <Hash.h>
// #include <ArduinoJson.h>
// #include <WiFiClientSecure.h>
// #include "EMailSender.h"

/**
 * Uncomment the following 
 * if you wish to have logging 
 * data printed to the screen
 */
#define ENABLE_LOGGING

/* Definitions for LEDs */
#define IND_1   2
#define IND_2   12


/**
 * Define version string constants
 * TODO: need to remove HTML_SW_STRING ??
*/
String version_string = "";
String SW_VERSION_STRING = "0.0.2.a";
String HTML_SW_STRING = "<h2>Firework Igniter" + SW_VERSION_STRING + "</h2>";
String HW_VERSION_STRING = "A01";

/**
 * Constants (slave addresses)
 * for the MAX3725 GPIO expander
 * The "p" port represents the eight 
 * IO that are of teh open-drain type
 * the "o" port represents the eight 
 * IO that are of the push-pull type
 * 
 * The seven-bit addresses shall
 * be defined here, as the Wire.h
 * library will automatically add the 
 * R/#W bit.
*/
const uint8_t max3725_7b_address_p0p7  = 0b1101000;
const uint8_t max3725_7b_address_o8o15 = 0b1011000;

/**
 * Parameters for soft 
 * access point
*/
const char* ssid     = "firework_igniter";
const char* password = "123456789"; //Minimum of eight characters here

/**
 * Constant parameters to 
 * support server activity
*/
const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";


/**
 * Webserver object
*/
AsyncWebServer server(80);

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




// START OF TEST CODE PASTED IN FROM https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Firework Igniter</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    p {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 6px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 3px}
    input:checked+.slider {background-color: #b30000}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
  </style>
</head>
<body>
  <h2>Firework Igniter 0.0.2.a</h2>
  %BUTTONPLACEHOLDER%
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?output="+element.id+"&state=1", true); }
  else { xhr.open("GET", "/update?output="+element.id+"&state=0", true); }
  xhr.send();
}
</script>
</body>
</html>
)rawliteral";


// String processor_sw_version(const String& var) {
//   if(var == "SWVER"){
//     String sw_ver = "";
//     // <h2>Firework Igniter</h2>
//     sw_ver += "Firework Igniter ";
//     sw_ver += SW_VERSION_STRING;
//     // sw_ver += " 0.1.0.b ";
//   }
//   return String();
// }

// Replaces placeholder with button section in your web page
//TODO: need to make the following IDs and output values correct.
//TODO: just an example header string :::  <title>Firework Igniter + " " + %SWVER%</title>
String processor(const String& var){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<h4>FUSE 1</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"2\" " + outputState(2) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 3</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"3\" " + outputState(3) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"4\" " + outputState(4) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"5\" " + outputState(5) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 6</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"6\" " + outputState(6) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 7</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"7\" " + outputState(7) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 8</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"8\" " + outputState(8) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 9</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"9\" " + outputState(9) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 10</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"10\" " + outputState(10) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 11</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"11\" " + outputState(11) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 12</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"12\" " + outputState(12) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 13</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"13\" " + outputState(13) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 14</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"14\" " + outputState(14) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 15</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"15\" " + outputState(15) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 16</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"16\" " + outputState(16) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  // else if(var == "SWVER"){
  //   String sw_ver = "";
  //   // <h2>Firework Igniter</h2>
  //   sw_ver += "<h2>Firework Igniter ";
  //   sw_ver += SW_VERSION_STRING;
  //   sw_ver += "</h2>";
  //   // sw_ver += " 0.1.0.b ";
  // }
  return String();
}

String outputState(int output){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
}

// END OF TEST CODE PASTED IN FROM https://randomnerdtutorials.com/esp8266-nodemcu-access-point-ap-web-server/


//TODO: clean up the following?
/**
 * JSON and input
 * data constructors
 */
// #define MAX_RX_BUF_INDEX          511
// #define MAX_RX_BUF_ELEMENTS       MAX_RX_BUF_INDEX + 1
// StaticJsonDocument<256> json_doc;
// DeserializationError json_err;
// char data_input_string[MAX_RX_BUF_ELEMENTS];


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
// #define WIFI_ERR_LED              12

/**
 * @brief LCD parameters
*/
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
uint8_t current_lcd_row       = 0;
const uint8_t first_row       = 0;
const uint8_t lcd_row_spacing = 2;

// String SW_VERSION_STRING = "0.0.b";
// String HW_VERSION_STRING = "A01";
// String version_string = "";


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
unsigned int    ms_ticks_1=0;
unsigned int    ms_ticks_50=0;
unsigned int    ms_ticks_500=0;
unsigned int    ms_ticks_1000=0;

bool            Timer1msFlag = false;
bool            Timer50msFlag = false;
bool            Timer500msFlag = false;
bool            Timer1000msFlag = false;

long            seconds_counter = 0;        //32bit value 4.264....e9


void lcdBold(bool aVal) {
  if (aVal) {
    u8x8.setFont(u8x8_font_victoriabold8_r); // BOLD
  } 
  else {
    u8x8.setFont(u8x8_font_victoriamedium8_r); // NORMAL
  }
}

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

// void lcdRun() {
//   lcdBold(false);
//   u8x8.clear(); // ("0123456789012345")
//   u8x8.setCursor(0,rowCups); u8x8.print(F("Cup: Run:OFF"));
//   u8x8.setCursor(0,rowState); u8x8.print(F("Now:STOPPED "));
//   u8x8.setCursor(0,rowTemp); u8x8.print(F("Tmp: Rly: "));
//   u8x8.setCursor(0,rowTime); u8x8.print(F("Tim: Min: "));

// }

void setup(void) {
  
  /**
   * Setup pin directions
   */
  pinMode(IND_1, OUTPUT);
  pinMode(IND_2, OUTPUT);
  
  // pinMode(SLEEP_BIT, INPUT);
  // pinMode(WIFI_ERR_LED, OUTPUT);

  // digitalWrite(WIFI_ERR_LED, LOW);
  digitalWrite(IND_1, LOW);  // The indicator is active high
  digitalWrite(IND_2, LOW);

  #if defined(ENABLE_LOGGING)
    Serial.begin(9600);
    Serial.setTimeout(50);    //Timeout value in ms -- max is 1000
  #endif
  
  current_state = STATE_1;

  /* START ... TEST CODE PASTED IN STARTS HERE*/

  u8x8.begin();
  lcdBold(true); // You MUST make this call here to set a Font
  
  // while(true){
    
    // u8x8.setPowerSave(0); // Too lazy to find out if I need this
    u8x8.clear();
    // u8x8.setCursor(0,rowCups);

  u8x8.setCursor(0,first_row);
  u8x8.print(F("FIREWORK IGNITER"));
  // u8x8.setCursor(0,row_second);
  // u8x8.print(F("STARTING..."));
  delay(2500);
  // u8x8.setCursor(0,rowState);
  
  u8x8.clear();
  current_lcd_row = 0;
  u8x8.setCursor(0,current_lcd_row);
  current_lcd_row += lcd_row_spacing;
  // u8x8.print(__DATE__);
  // u8x8.setCursor(0,rowState);
  
  // u8x8.setCursor(0,current_lcd_row);
  u8x8.print(String("SW V: " + SW_VERSION_STRING));

  u8x8.setCursor(0,current_lcd_row);
  current_lcd_row += lcd_row_spacing;
  u8x8.print(String("HW V: " + HW_VERSION_STRING));
  delay(3500);
  

  /* END ... OF CODE PASTED IN*/

  //Initialize Ticker every 0.05s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(tmr1_write_val);        //.05s 

  #if defined(ENABLE_LOGGING)
    Serial.println("Module just rebooted.");
  #endif

  /**
   * @brief setup wifi access point
  */
  Serial.println("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);
  

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  u8x8.clear();
  current_lcd_row = 0; 
  u8x8.setCursor(0,first_row);
  current_lcd_row += lcd_row_spacing; 
  u8x8.print("HOSTING AT IP");
  u8x8.setCursor(0,current_lcd_row);
  current_lcd_row += lcd_row_spacing;
  u8x8.print(IP);
  delay(2500);

  u8x8.clear();
  current_lcd_row = 0;
  u8x8.setCursor(0,current_lcd_row);
  current_lcd_row += lcd_row_spacing;
  u8x8.print(String("FIREWORK IGNITER"));
  u8x8.setCursor(0,current_lcd_row);
  u8x8.print(String("READY ..."));

  // Print ESP8266 Local IP Address
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  //TODO: need to move these constants up top
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    uint8_t input_message1_value = 0;
    uint8_t input_message2_value = 0;

    
    /**
     * GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
     * Here is where we set the output based on the the fuse value selected
    */
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      input_message1_value = inputMessage1.toInt();
      input_message2_value = inputMessage2.toInt();
      
      //TODO: cjg put the following in so we only 
      //TODO: write to the digital IO if the output 
      //TODO: is correct.
      if(input_message1_value == 12) {
        // digitalWrite(inputMessage1.toInt(), inputMessage2.toInt());     //This is the function where output states are changed
        digitalWrite(input_message1_value, input_message2_value);     //This is the function where output states are changed
      }

    }
    else {
      inputMessage1 = "No message sent";
      inputMessage2 = "No message sent";
    }
    
    Serial.print("GPIO: ");
    Serial.print(input_message1_value);
    Serial.print(" - Set to: ");
    Serial.println(input_message2_value);
    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();

  delay(200);  //TODO: we probably want to remove the delay!
  /** TODO: This is a temporary note
   * to say that the LED will illuminate after
   * just before trying to engage the IO expander
  */
  digitalWrite(IND_1,HIGH);  
  delay(200); //TODO: we probably want to remove the delay!


  /**
   * Setup for I2C communication 
   * to the GPIO expander.
   * Wire.begin() does not have to be called
   * since the display library makes a call to 
   * it.
   * Slave address for P0-P7:   1101 000 R/#W 
   * Slave address for O8-O15:  1011 000 R/#W 
   * 
   * Do not provide an argument if/when calling
   * Wire.begin() if needing to operate in master mode.
   * 
   * The MAX7325 requires an I2C 
   * transaction before it can determine
   * its address.  This is any 
   * transaction, meaning an I2C transaction
   * doesn't necessarily need to target
   * the MAX7325. It has been confirmed that simply 
   * updating the display (before these routines) will
   * allow the MAX7325 to acquire it's I2C address. 
   * 
   * For the MAX7325, the P ports are 
   * open-drain, while the O ports are 
   * push-pull.  
  */
  clear_gpio();

  set_gpio(13);
  delay(200); 
  
  clear_gpio();
  delay(200);
}

/**
 * @brief Main application loop
*/
void loop(void) {

  if(Timer1msFlag == true) {
    Timer1msFlag = false;
    // StateEvaluation();
  }
  
  if(Timer50msFlag == true) {
    Timer50msFlag = false;
  }

  if(Timer500msFlag == true) {
    Timer500msFlag = false;
    digitalWrite(IND_1,!(digitalRead(IND_1)));  //Toggle the health LED
  }

  if(Timer1000msFlag == true) {
    Timer1000msFlag = false;
    (seconds_counter == 300000)?(seconds_counter = 0):(seconds_counter++);
    
  }
}