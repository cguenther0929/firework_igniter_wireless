/**
 * TODO: A lot of button progress was made, but it still doesn't 
 * TODO: appear to submit the value.  In the <form /> definition, 
 * TODO: it seems to crash if we make the method a post instead of get.
 * 
 * This might be a good tutorial: https://www.w3schools.com/tags/tryit.asp?filename=tryhtml_form_method
 * 
 * 
*/
/*
 * This is the main source file for the ESP8266 WiFi SOM
 * 
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

/* Define Outputs */
// TODO: need to define these as 
// TODO: LED that hang off IO expander  
#define LOCAL_HB_LED    2

/* Define Inputs */
#define BAT_MON         A0
#define PBTN_1          12
#define PBTN_2          13

/**
 * Define version string constants
 * TODO: need to remove HTML_SW_STRING ??
*/
String version_string = "";
String SW_VERSION_STRING = "0.0.1.a";
String HTML_SW_STRING = "<h2>Firework Igniter" + SW_VERSION_STRING + "</h2>";
String HW_VERSION_STRING = "A02";

/**
 * Constants (slave addresses)
 * for I2C devices. For the IO expander,
 * the "p" port represents the eight 
 * IO that are of the open-drain type
 * the "o" port represents the eight 
 * IO that are of the push-pull type.
 * 
 * Only the seven-bit addresses shall
 * be defined here, as the Wire.h
 * library will automatically add the 
 * R/#W bit.
*/
const uint16_t io_expander_7b_address_p0p7   = 0b1101000;
const uint16_t io_expander_7b_address_o8o15  = 0b1011000;

const uint16_t anlg_sw_ch1to8_address        = 0b1001100;
const uint16_t anlg_sw_ch9to16_address       = 0b1001110;

const uint16_t adc_ch1to8_address            = 0b0011101;
const uint16_t adc_ch9to16_address           = 0b0110111;

const uint16_t dac_address                   = 0b0001101;

const uint16_t eeprom_address                = 0b1010000;

/**
 * The address of the display,
 * as posted in silk on the back of 
 * the unit, is 0x78 or 
 * 0b_111_1000
*/

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
 * @brief LCD parameters
*/
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
uint8_t current_lcd_row       = 0;
const uint8_t first_row       = 0;
const uint8_t lcd_row_spacing = 2;

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


/**
 * HTML character string to 
 * return upon get request from
 * client
*/
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


<FORM action="/" method="get">
<br>
Fuse Current: <INPUT type="text" name="fuse_value" size="5" value="50"<br>
<input type="submit" value="SUBMIT">
</FORM>


</body>
</html>
)rawliteral";


/**
 * Replaces HTML placeholder 
 * and generate button list
*/
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

long            seconds_counter     = 0;        //32bit value 4.264....e9
long            tick_1ms_counter    = 0;        //32bit value.  At 20ms, this can count to 8.5899e7 seconds

uint8_t         timeout_seconds     = 4.0;
uint16_t        timeout_1ms_ticks   = timeout_seconds/0.001;


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
  pinMode(LOCAL_HB_LED, OUTPUT);
  pinMode(BAT_MON, INPUT);
  pinMode(PBTN_1, INPUT);
  pinMode(PBTN_2, INPUT);
  // TODO: remove unnecessary code
  // TODO: do we need to do anything for the IO expander here?
  // pinMode(LOCAL_HB_LED, OUTPUT);
  // pinMode(IND_2, OUTPUT);
  
  // digitalWrite(WIFI_ERR_LED, LOW);
  digitalWrite(LOCAL_HB_LED, LOW);  // The indicator is active high 
  
  #if defined(ENABLE_LOGGING)
    Serial.begin(9600);
    Serial.setTimeout(50);    //Timeout value in ms -- max is 1000
  #endif
  
  current_state = STATE_1;

  /**
   * OLED Setup
   */
  // TODO: we might want to try this
  // u8g2.setI2Caddress(0x078);
  // u8g2.begin();
  // u8g2.setPowerSave(0);
  u8x8.begin();
  lcdBold(true); // This call is necessary to set a font
  u8x8.clear();

  u8x8.setCursor(0,first_row);
  u8x8.print(F("FIREWORK IGNITER"));
  current_lcd_row += lcd_row_spacing;
  u8x8.setCursor(0,current_lcd_row);
  u8x8.print(F("STARTING ..."));
  
  
  // delay(2500);
  
  u8x8.clear();
 

  #if defined(ENABLE_LOGGING)
    Serial.println("Module just rebooted.");
  #endif

  /**
   * @brief setup wifi access point
  */
  Serial.println("Setting AP (Access Point)…");
  
  /**
   * Remove the password parameter, 
   * if the AP is to be open 
   * */ 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  #if defined(ENABLE_LOGGING)
    Serial.print("AP IP address: ");
    Serial.println(IP);
  #endif

  /**
   * OLED display parameters 
   * Host IP / Versions / Etc. 
  */

  u8x8.clear();
  current_lcd_row = 0; 
  u8x8.setCursor(0,first_row);
  current_lcd_row += lcd_row_spacing; 
  u8x8.print("HOSTING AT IP");
  u8x8.setCursor(0,current_lcd_row);
  current_lcd_row += lcd_row_spacing;
  u8x8.print(IP);

  u8x8.setCursor(0,current_lcd_row);
  u8x8.print(String("SW V: " + SW_VERSION_STRING));
  current_lcd_row += lcd_row_spacing;

  u8x8.setCursor(0,current_lcd_row);
  u8x8.print(String("HW V: " + HW_VERSION_STRING));
  current_lcd_row += lcd_row_spacing;

  /**
   * Print the IP address
   * of the ESP8266 to the 
   * serial terminal
   */ 
  #if defined(ENABLE_LOGGING)
    Serial.print("Local IP address: ");
    Serial.println(WiFi.localIP());
  #endif

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  /**
   * Send a GET request to <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
  */
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage1;
    String inputMessage2;
    uint8_t input_message1_value = 0;
    uint8_t input_message2_value = 0;

   #if defined(ENABLE_LOGGING)
      Serial.print("GPIO: ");
      Serial.print(input_message1_value);
      Serial.print(" - Set to: ");
      Serial.println(input_message2_value);
    #endif

    // TODO: This is an old message.
    // TODO: Need to clean the following up if it works
    if(request->hasParam("fuse_value")){
      #if defined(ENABLE_LOGGING)
        Serial.println("A fuse value was submitted.");
      #endif
      
      inputMessage1 = request->getParam("fuse_value")->value();
      input_message1_value = inputMessage1.toInt();
      #if defined(ENABLE_LOGGING)
        Serial.print("Fuse Current Submitted: ");
        Serial.print(input_message1_value);
      #endif
    }
    
    /**
     * GET input1 value on <ESP_IP>/update?output=<inputMessage1>&state=<inputMessage2>
     * Here is where we set the output based on the the fuse value selected
     * This is in the SETUP routine 
    */
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      /* PARAM_INPUT_1 = "output" */
      inputMessage1 = request->getParam(PARAM_INPUT_1)->value(); 
      /* PARAM_INPUT_2 = "state" */ 
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();   
      input_message1_value = inputMessage1.toInt();
      input_message2_value = inputMessage2.toInt();
      
      
      /**
       * Each IO had 100 added to it
       * in order to avoid IO values 
       * within the Arduino, so
       * we must remove 100 from 
       * the value to get back to 
       * the originally intended IO
       * value.
      */
      input_message1_value -= 100;
      //TODO: clean up superfluous code 
      // if(input_message1_value == 12) {
      set_gpio(input_message1_value); //TODO: this line is in just for testing.
      // digitalWrite(input_message1_value, input_message2_value);     //This is the function where output states are changed //TODO: we need to remove the digital write
      
      /* The follow is true when a switch transitions from OFF to ON */
      if(input_message2_value == 1) {     
        #if defined(ENABLE_LOGGING)
          Serial.println("Timeout timer started.");
        #endif
        timer_running = true;

      }
      // }

    }
    else {
      inputMessage1 = "Invalid request";
      inputMessage2 = "Invalid request";
    }
    
    #if defined(ENABLE_LOGGING)
      Serial.print("GPIO: ");
      Serial.print(input_message1_value);
      Serial.print(" - Set to: ");
      Serial.println(input_message2_value);
    #endif

    request->send(200, "text/plain", "OK");
  });

  // Start server
  server.begin();

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
      // TODO: This entire section might need work
      // TODO: need to remove superfluous code
      // Serial.println("Timeout occurred.");
      clear_gpio();  
      // digitalWrite(IND_2, LOW);     //This is the function where output states are changed
      #if defined(ENABLE_LOGGING)
        Serial.println("Timeout occurred");
      #endif
      
      tick_1ms_counter = 0;
      timer_running = false;  
    }
  }
  
  if(Timer50msFlag == true) {
    Timer50msFlag = false;
  }

  if(Timer500msFlag == true) {
    Timer500msFlag = false;
  }

  if(Timer1000msFlag == true) {
    Timer1000msFlag = false;
    (seconds_counter == 300000)?(seconds_counter = 0):(seconds_counter++);
    digitalWrite(LOCAL_HB_LED,!(digitalRead(LOCAL_HB_LED)));  //Toggle the health LED
    
  }
}  /*END MAIN LOOP*/
