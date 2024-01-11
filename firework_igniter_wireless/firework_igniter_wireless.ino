/*
 * This is the main source file for the ESP8266 WiFi SOM
 * 
 * TODO: buttons 4,5, 14, and 16 default to ON
*/

#include <ESPAsyncWebServer.h>
#include <ESPAsyncTCP.h>
#include <ESP8266WiFi.h>
#include <pins_arduino.h>
#include <Ticker.h>
#include <U8x8lib.h>
#include <Wire.h>

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
String SW_VERSION_STRING = "0.1.0.a";
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
  %HTMLPLACEHOLDER%
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


// Replaces HTML placeholder an generate button list
//TODO: need to make the following IDs and output values correct.
//TODO:  Example H2 title :: <h2>Firework Igniter 0.0.2.a</h2>
String processor(const String& var){
  //Serial.println(var);
  if(var == "HTMLPLACEHOLDER"){
    String buttons = "";
    buttons += "<h2>FIREWORK IGNITER ";
    buttons += SW_VERSION_STRING;
    buttons += "</h2>";
    buttons += "<h4>FUSE 1</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"101\" " + outputState(101) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 2</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"102\" " + outputState(102) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 3</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"103\" " + outputState(103) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 4</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"104\" " + outputState(104) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 5</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"105\" " + outputState(105) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 6</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"106\" " + outputState(106) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 7</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"107\" " + outputState(107) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 8</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"108\" " + outputState(108) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 9</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"109\" " + outputState(109) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 10</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"110\" " + outputState(110) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 11</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"111\" " + outputState(111) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 12</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"112\" " + outputState(112) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 13</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"113\" " + outputState(113) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 14</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"114\" " + outputState(114) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 15</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"115\" " + outputState(115) + "><span class=\"slider\"></span></label>";
    buttons += "<h4>FUSE 16</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"116\" " + outputState(116) + "><span class=\"slider\"></span></label>";
    return buttons;
  }
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


/**
 * Data imported 
 * from JSON string
 */
char buf_wifi_password[32];
char buf_wifi_ssid[32];


/**
 * Timer parameters
 */
long            tmr1_write_val  = 3030;   // Empirically derived to generate a 1ms tick timer.
unsigned int    ms_ticks_1      =0;
unsigned int    ms_ticks_50     =0;
unsigned int    ms_ticks_500    =0;
unsigned int    ms_ticks_1000   =0;

bool            Timer1msFlag        = false;
bool            Timer50msFlag       = false;
bool            Timer500msFlag      = false;
bool            Timer1000msFlag     = false;
bool            timer_running       = false;
// bool            timer_expired       = false;

long            seconds_counter     = 0;        //32bit value 4.264....e9
long            tick_1ms_counter   = 0;        //32bit value.  At 20ms, this can count to 8.5899e7 seconds

float           timeout_seconds     = 3.0;
// float           timeout_1ms_ticks   = timeout_seconds/0.001;
float           timeout_1ms_ticks   = 3000.0;


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
 /**
  * @brief BEGINNING OF SETUP ROUTINE
 */

void setup(void) {
  
  /**
   * Setup pin directions
   */
  pinMode(IND_1, OUTPUT);
  pinMode(IND_2, OUTPUT);
  
  // digitalWrite(WIFI_ERR_LED, LOW);
  digitalWrite(IND_1, LOW);  // The indicator is active high //IO 12
  digitalWrite(IND_2, LOW);
  
  #if defined(ENABLE_LOGGING)
    Serial.begin(9600);
    Serial.setTimeout(50);    //Timeout value in ms -- max is 1000
  #endif
  
  current_state = STATE_1;

  /* BEGINNING OF DISPLAY ROUTINE*/

  u8x8.begin();
  lcdBold(true); // You MUST make this call here to set a Font
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

  /* END OF DISPLAY ROUTINE*/


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



  // server.querySelector("#stop").addEventListener("click", () => {
  //   const switches = server.querySelectorAll(".switch input");
  //   for (let s of switches) {
  //     s.checked = false;
  //   }
  // });
  // document.querySelector("#stop").addEventListener("click", () => {
  //   const switches = document.querySelectorAll(".switch input");
  //   for (let s of switches) {
  //     s.checked = false;
  //   }
  // });


    /**
     * GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
     * Here is where we set the output based on the the fuse value selected
    */
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value();   //TODO: Temp info PARAM_INPUT_1 == "output"
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();   //TODO: temp info PARAM_INPUT_2 == "state"
      input_message1_value = inputMessage1.toInt();
      input_message2_value = inputMessage2.toInt();
      
      //TODO: cjg put the following in so we only 
      //TODO: write to the digital IO if the output 
      //TODO: is correct.
      input_message1_value -= 100;
      if(input_message1_value == 12) {
        Serial.println("Starting timer and setting output.");
        digitalWrite(input_message1_value, input_message2_value);     //This is the function where output states are changed
        timer_running = true;
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


//TODO: this following is for testing only
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

  //Initialize Ticker every 0.05s
  timer1_attachInterrupt(onTimerISR);
  timer1_enable(TIM_DIV16, TIM_EDGE, TIM_SINGLE);
  timer1_write(tmr1_write_val);        //.05s 

} /* END SETUP ROUTINE*/

/**
 * @brief Main application loop
*/
void loop(void) {

  if(Timer1msFlag == true) {
    Timer1msFlag = false;
    // StateEvaluation();
    if(timer_running){
      tick_1ms_counter++;
    }
    else {
      tick_1ms_counter = 0;
    }
    
    if(tick_1ms_counter >= timeout_1ms_ticks) {
      Serial.println("Timeout occurred.");
      clear_gpio();
      //TODO: the following is in to test the timeout function
      digitalWrite(IND_2, LOW);     //This is the function where output states are changed
      // timer_expired = true;
      tick_1ms_counter = 0;
      timer_running = false;  //TODO: do we want to turn this off here?
    }
  }
  
  if(Timer50msFlag == true) {
    Timer50msFlag = false;
    digitalWrite(IND_1,!(digitalRead(IND_1)));  //Toggle the health LED
  }

  if(Timer500msFlag == true) {
    Timer500msFlag = false;
    // digitalWrite(IND_1,!(digitalRead(IND_1)));  //Toggle the health LED
  }

  if(Timer1000msFlag == true) {
    Timer1000msFlag = false;
    (seconds_counter == 300000)?(seconds_counter = 0):(seconds_counter++);
    
  }
}  /*END MAIN LOOP*/