/**
 * TODO: Nice to have ... Push the fuse current from the web interface
 * 
 * 
 * TODO: A lot of button progress was made, but it still doesn't 
 * TODO: appear to submit the value.  In the <form /> definition, 
 * TODO: it seems to crash if we make the method a post instead of get.
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
// #define ENABLE_LOGGING

/**
 * Uncomment the following 
 * if you wish to to see logging 
 * data as it pertains to the fuse
 * array
 */
#define ENABLE_LOGGING_ADC_RELATED

/**
 * Define an array for the purpose
 * of tracking the health of the fuses
 */ 
#define NUM_OF_FUSES                16
uint8_t fuse_array[NUM_OF_FUSES]    = {0};

/* Define Outputs */
#define LOCAL_HB_LED    2

/* Define 8266 IO */
#define BAT_MON         A0
#define PBTN_1          12
#define PBTN_2          13

/* Define GPIO Expander IO */
#define EXT_GRN_LED     9   //Represents the IO number
#define EXT_RED_LED     10  
#define EXT_YEL_LED     11

/**
 * Volts per bit for
 * the eight bit DAC.
*/
const float MV_PER_BIT        = 12.9412;   // 3V3/255 steps for the DAC

/**
 * Fuse current global
*/
#define FUSE_CURRENT_MA_MIN     150
#define FUSE_CURRENT_MA_MAX     800
#define FUSE_OK_DIG_THRESHOLD  50
uint16_t fuse_current_ma      = FUSE_CURRENT_MA_MIN; //Value is in mA

/**
 * Define version string constants
*/
String version_string = "";
String SW_VERSION_STRING = "0.3.1.a";
String HW_VERSION_STRING = "A03";

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
 * 
 * I2C (Wire) functions want 
 * 16bit addresses, thus the width 
 * of these addresses is set to 16bits
*/
const uint16_t io_expander_7b_address_p0p7    = 0b1101000;
const uint16_t io_expander_7b_address_o8o15   = 0b1011000;

const uint16_t anlg_sw_ch1to8_address         = 0b1001100;
const uint16_t anlg_sw_ch9to16_address        = 0b1001110;

const uint16_t adc_ch1to8_address             = 0b0011101;
const uint16_t adc_ch9to16_address            = 0b0110111;

const uint16_t dac_address                    = 0b0001101;

const uint16_t eeprom_address                 = 0b1010000;


/**
 * Internal Addresses 
 * of ADC registers
*/
const uint8_t adc_config_reg_addr             = 0x00;
const uint8_t adc_int_stat_reg_addr           = 0x01;
const uint8_t adc_conv_rate_reg_addr          = 0x07;
const uint8_t adc_ch_disable_reg_addr         = 0x08;
const uint8_t adc_adv_config_reg_addr         = 0x0B;
const uint8_t adc_busy_register_addr          = 0x0C;
const uint8_t adc_channel_read_start_addr     = 0x20;

const uint8_t adc_in0_high_limit_reg_addr     = 0x2A;
const uint8_t adc_in0_low_limit_reg_addr      = 0x2B;
const uint8_t adc_in1_high_limit_reg_addr     = 0x2C;
const uint8_t adc_in1_low_limit_reg_addr      = 0x2D;
const uint8_t adc_in2_high_limit_reg_addr     = 0x2E;
const uint8_t adc_in2_low_limit_reg_addr      = 0x2F;
const uint8_t adc_in3_high_limit_reg_addr     = 0x30;
const uint8_t adc_in3_low_limit_reg_addr      = 0x31;
const uint8_t adc_in4_high_limit_reg_addr     = 0x32;
const uint8_t adc_in4_low_limit_reg_addr      = 0x33;
const uint8_t adc_in5_high_limit_reg_addr     = 0x34;
const uint8_t adc_in5_low_limit_reg_addr      = 0x35;
const uint8_t adc_in6_high_limit_reg_addr     = 0x36;
const uint8_t adc_in6_low_limit_reg_addr      = 0x37;
const uint8_t adc_in7_high_limit_reg_addr     = 0x38;
const uint8_t adc_in7_low_limit_reg_addr      = 0x39;

const uint8_t adc_mfgid_reg_addr              = 0x3E;
const uint8_t adc_revid_reg_addr              = 0x3F;


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
const char* ssid          = "firework_igniter";
const char* password      = "123456789"; //Minimum of eight characters here
IPAddress assigned_ip_str;

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
uint8_t current_oled_row        = 0;
const uint8_t oled_column       = 1;
const uint8_t first_row         = 0;
const uint8_t lcd_row_spacing   = 2;

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

enum screen {
  SCREEN_1,
  SCREEN_2
};

/**
 * State tracker 
 */
state current_state = UNKNOWN;

/**
 * Screen tracker 
 */
screen current_screen = SCREEN_1;

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
Fuse Current (mA): <INPUT type="text" name="fuse_value" size="5" value="50"<br>
<input type="submit" value="Update">
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
    buttons += "<h4> ARM </h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"117\" " + outputState(117) + "><span class=\"slider\"></span></label>";
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
  
  digitalWrite(LOCAL_HB_LED, LOW);  // The indicator is active high 
  
  // #if defined (ENABLE_LOGGING) || (ENABLE_LOGGING_ADC_RELATED)
  Serial.begin(115200);
  Serial.setTimeout(50);    //Timeout value in ms -- max is 1000
  // #endif
  
  current_state = STATE_1;
  current_screen = SCREEN_1;

  #if defined(ENABLE_LOGGING)
    Serial.println("Module just rebooted.");
  #endif


  /**
   * Initialize the OLED
  */
  u8x8.begin();
  lcdBold(true); // This call is necessary to set a font
  u8x8.clear();;
  
  /**
   * @brief setup wifi access point
  */
  #if defined(ENABLE_LOGGING)
    Serial.println("Setting AP (Access Point)…");
  #endif
  
  /**
   * Remove the password parameter, 
   * if the AP is to be open 
   * */ 
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  assigned_ip_str = IP;
  
  #if defined(ENABLE_LOGGING)
    Serial.print("AP IP address: ");
    Serial.println(IP);
  #endif

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
      input_message1_value = inputMessage1.toInt(); // Output value
      input_message2_value = inputMessage2.toInt(); // State value
      
      
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
      
      /**
       * The following is true when a 
       * switch switches from ON to OFF
      */
      if(input_message2_value == 0) {     
        if(input_message1_value == 17) {
          if(fuse_current_ma != FUSE_CURRENT_MA_MIN) {
            fuse_current_ma = FUSE_CURRENT_MA_MIN;
            set_fuse_current_ma(fuse_current_ma);
          }
        }
      
      }
      
      /** 
       * The following is true when 
       * a switch transitions from OFF to ON 
       */
      if(input_message2_value == 1) {     
        
      /**
       * This is the case where the user
       * wishes to arm the ignition system  
      */
      if(input_message1_value == 17) {
        if(fuse_current_ma != FUSE_CURRENT_MA_MAX) {
          fuse_current_ma = FUSE_CURRENT_MA_MAX;
          set_fuse_current_ma(fuse_current_ma);
        }
      }

      /**
       * Enable the output (1-16) by enabling the 
       * associated analog switch. 
      */
      else {
        set_anlgsw(input_message1_value); 
      }

        #if defined(ENABLE_LOGGING)
          Serial.println("Timeout timer started.");
        #endif
        timer_running = true;

      }

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

  
  /**
   * Define default (low) fuse current 
  */
  set_fuse_current_ma(FUSE_CURRENT_MA_MIN);

  /**
  * Initialize the ADC 
  */
  init_adc();

  /**
   * Read the MFG and REV
   * IDs from the chip
   */
  #if defined(ENABLE_LOGGING_ADC_RELATED)
    Serial.print('\n');
    Serial.print('\n');

    Serial.println("MFGID SHOULD BE 1 and REVID SHOULD BE 9.");
    Serial.print("ADC 1thru8 MFGID: ");
    Serial.print(get_adc1thru8_mfgid());
    Serial.print(" ... ");
    Serial.print("ADC9thru16 MFGID: ");
    Serial.print(get_adc9thru16_mfgid());
    Serial.print('\n');
    Serial.print('\n');


    Serial.print("ADC 1thru8 REVID: ");
    Serial.print(get_adc1thru8_revid());
    Serial.print(" ... ");
    Serial.print("ADC 9thru16 REVID: ");
    Serial.print(get_adc9thru16_revid());
    Serial.print('\n');
    Serial.print('\n');
  #endif


} /* END SETUP ROUTINE*/

/**
 * @brief Main application loop
*/
void loop(void) {
  uint16_t i=0;

  if(Timer1msFlag == true) {
    Timer1msFlag = false;
    
    if(fuse_current_ma >= 800) {
      set_ioxpander_gpio(11);
    }
    else {
      clear_ioxpander_gpio(11);;
    }

    if(timer_running){
      tick_1ms_counter++;
    }
    else {
      tick_1ms_counter = 0;
    }
    
    /* Condition where timeout occurred */
    if(tick_1ms_counter >= timeout_1ms_ticks) {
      
      disable_all_anlgsw();  
      #if defined(ENABLE_LOGGING)
        Serial.println("Time expired");
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
    
    if(digitalRead(LOCAL_HB_LED)) {
      /**
       * Turn heartbeat LEDs OFF
      */
      digitalWrite(LOCAL_HB_LED,0);  
      clear_ioxpander_gpio(9); 
    }
    else {
      /**
       * Turn heartbeat LEDs ON
      */
      digitalWrite(LOCAL_HB_LED,1);  
      set_ioxpander_gpio(9); 
    }

    check_fuses(fuse_array,NUM_OF_FUSES);

    #if defined(ENABLE_LOGGING_ADC_RELATED)
        Serial.print("Fuse Array:  ");
        for(i=0; i < 16; i++) {
          Serial.print(fuse_array[i]);
        }
        Serial.print('\n');
    #endif

    
  }

  if(seconds_counter >= 3) {
    seconds_counter = 0;    //Reset the seconds counter
    screen_evaluation();
    display_screen();
  }
}  /*END MAIN LOOP*/
