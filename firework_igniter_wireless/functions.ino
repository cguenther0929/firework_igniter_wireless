
/* I2C Pins */
#define I2C_SDA                                 4
#define I2C_SCL                                 5
#define I2C_CLK_SPEED                           10000

/**
 * Addresses for ADC
 */
#define     ADC_CONFIG_REG_ADDRESS              0x00
#define     ADC_INT_STAT_REG_ADDR               0x01
#define     ADC_CONV_RATE_REG_ADDR              0x07
#define     ADC_CH_DISABLE_REG_ADDR             0x08
#define     ADC_ONE_SHOT_REG_ADDR               0x09
#define     ADC_ADV_CONFIG_REG_ADDR             0x0B
#define     ADC_BUSY_REG_ADDR                   0x0C
#define     ADC_CH1TO8_ADDRESS                  0x1D
#define     ADC_CH_READ_START_ADDR              0x20
#define     ADC_CH9TO16_ADDRESS                 0x37

#define     ADC_MFGID_REG_ADDR                  0x3E
#define     ADC_REVID_REG_ADDR                  0x3F

#define     ADC_IN0_HIGH_LIM_REG                0x2A    
#define     ADC_IN0_LOW_LIM_REG                 0x2B

#define     ADC_IN1_HIGH_LIM_REG                0x2C
#define     ADC_IN1_LOW_LIM_REG                 0x2D

#define     ADC_IN2_HIGH_LIM_REG                0x2E
#define     ADC_IN2_LOW_LIM_REG                 0x2F

#define     ADC_IN3_HIGH_LIM_REG                0x30
#define     ADC_IN3_LOW_LIM_REG                 0x31

#define     ADC_IN4_HIGH_LIM_REG                0x32
#define     ADC_IN4_LOW_LIM_REG                 0x33

#define     ADC_IN5_HIGH_LIM_REG                0x34
#define     ADC_IN5_LOW_LIM_REG                 0x35

#define     ADC_IN6_HIGH_LIM_REG                0x36
#define     ADC_IN6_LOW_LIM_REG                 0x37

#define     ADC_IN7_HIGH_LIM_REG                0x38
#define     ADC_IN7_LOW_LIM_REG                 0x39
/**
 * I2C Addresses of IO 
 * Expander
 */
#define   IO_EXPANDER_ADDR_P0TO7                0x68
#define   IO_EXPANDER_ADDR_O8TO15               0x58

/**
 * I2C Addresses of analog 
 * switches
 */
#define   ANLG_SW_CH1to8_ADDRESS                0x4C
#define   ANLG_SW_CH9to16_ADDRESS               0x4E

/**
 * I2C Address for DAC 
 */
#define   DAC_ADDRESS                           0x0D

/**
 * ADC raw value shall be above 
 * this to consider a fuse being okay.  
 */
#define   FUSE_OK_DIG_THRESHOLD                 500

/**
 * @brief Print uint8_t binary representation of a number
 * 
 * 
 * @return nothing
*/
void print8b_bin(byte aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--){
    Serial.print(bitRead(aByte, aBit) ? '1' : '0');
  }
  Serial.print('\n');
}

/**
 * @brief Print uint16_t binary representation of a number
 * 
 * @return nothing
*/
void print16b_bin(byte aByte) {
  for (int8_t aBit = 15; aBit >= 0; aBit--){
    Serial.print(bitRead(aByte, aBit) ? '1' : '0');
  }
  Serial.print('\n');
}

/**
 * @brief Display the first screen on the OLED
 * 
 * @return true or false
*/
uint8_t display_screen ( void )
{
    switch (current_screen){
        case SCREEN_1:
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(OLED_COLUMN,FIRST_ROW);
            u8x8.print("HOSTING AT IP");
            current_oled_row += LCD_ROW_SPACING; 
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            u8x8.print(assigned_ip_str);
            current_oled_row += LCD_ROW_SPACING; 
            
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            u8x8.print(String("SW V: " + SW_VERSION_STRING));
            current_oled_row += LCD_ROW_SPACING;

            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            u8x8.print(String("HW V: " + HW_VERSION_STRING));
            current_oled_row += LCD_ROW_SPACING;
        break;

        case SCREEN_2:
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(OLED_COLUMN,FIRST_ROW);
            u8x8.print("CURRENT (mA)");
            current_oled_row += LCD_ROW_SPACING; 
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            u8x8.print(String(fuse_current_ma));
        break;
        
        case SCREEN_3:  
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(OLED_COLUMN,FIRST_ROW);
            
            u8x8.print("Chk Count:");
            u8x8.print(fuse_check_counter);
            current_oled_row += LCD_ROW_SPACING; 
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            

            u8x8.print("Fuse health:");
            current_oled_row += LCD_ROW_SPACING; 
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            
            /**
             * build the fuse health 
             * string
             */
            fuse_health = "";
            for(uint8_t i=16; i > 0; i--) 
            {
                fuse_health += String(fuse_array[i-1]);
            }
            
            u8x8.print(fuse_health);
            current_oled_row += LCD_ROW_SPACING; 
            


            
            u8x8.setCursor(OLED_COLUMN,current_oled_row);
            u8x8.print(String("-CJG IGNITER-"));
        break;

        default:
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(OLED_COLUMN,FIRST_ROW);
            u8x8.print("FSM ISSUE WITH DISPLAY");

    }
    

    return true;
}

/**
 * @brief Function to define bold text on the OLED
 * 
 * @return nothing
*/
void lcdBold(bool aVal) {
  if (aVal) {
    u8x8.setFont(u8x8_font_victoriabold8_r); // BOLD
  } 
  else {
    u8x8.setFont(u8x8_font_victoriamedium8_r); // NORMAL
  }
}

/**
 * @brief The fuse current will be the value represented by maval
 * 
 * @param maval mA value of the fuse current
 * 
 * @return nothing
*/
void set_fuse_current_ma (uint16_t maval) {
    uint16_t bit_value;
    float mv_voltage_target=0.0;

    mv_voltage_target = maval * 0.5;   // Simply Ohms law V = I*R

    bit_value = (uint8_t)(mv_voltage_target/MV_PER_BIT);
    
    set_dac_value(bit_value);
}

/**
 * @brief Set DAC per binary value that was passed in
 * 
 * @param binval binary value for the DAC setting (0-255)
 * 
 * @return nothing
*/
void set_dac_value (uint8_t binval) {
    /**
     * 8b DAC which uses the supply 
     * voltage as the reference (3V3).
     * This yields an output of 12.89mV/bit. 
     * Quick references:
     * 1V       = 78
     * 1.5V     = 116
     * 2V       = 155
     * 
     * The register for the DAC is structured 
     * as follows:
     * ┌─┬─┬───┬───┬──┬──┬──┬──┬──┬──┬──┬──┬─┬─┬─┬─┐
     * │0│0│PD1│PD0│D7│D6│D5│D4│D3│D2│D1│D0│0│0│0│0│
     * └─┴─┴───┴───┴──┴──┴──┴──┴──┴──┴──┴──┴─┴─┴─┴─┘
     * For PD1 and PD0
     * [0,0] = Normal Mode
     * [0,1] = 2.5k to GND
     * [1,0] = 100k to GND
     * [1,1] = High Impedance
     * 
     * 
     * 
     * So, if operating in normal mode
     * the register for the DAC would look like
     * this.
     * ┌─┬─┬─┬─┬──┬──┬──┬──┬──┬──┬──┬──┬─┬─┬─┬─┐
     * │0│0│0│0│D7│D6│D5│D4│D3│D2│D1│D0│0│0│0│0│
     * └─┴─┴─┴─┴──┴──┴──┴──┴──┴──┴──┴──┴─┴─┴─┴─┘
     *                          
    */

    uint8_t high_nibble = 0x00 | ((binval & 0xF0) >> 4);
    uint8_t low_nibble = 0x00 | ((binval & 0x0F) << 4);

    Wire.beginTransmission(DAC_ADDRESS); 
    Wire.write(high_nibble);        
    Wire.write(low_nibble);        
    Wire.endTransmission();

}

void  one_shot_reading_adc128d_ch1to8(void) 
{
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_ONE_SHOT_REG_ADDR);        
    Wire.write(0x01);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
}

void one_shot_reading_adc128d_ch9to16(void) 
{
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_ONE_SHOT_REG_ADDR);        
    Wire.write(0x01);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
}

/**
 * @brief To determine if the ADC (CH 1 through 8) is OKAY
 * 
 * @return true of false 
*/
bool adc128d_ch1to8_okay ( void ) {
    uint8_t sensor_value    = 0x00;

    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.write(ADC_BUSY_REG_ADDR);        
    Wire.endTransmission();

    Wire.requestFrom(ADC_CH1TO8_ADDRESS, 1);    // Request 1 byte from the address   
    sensor_value = Wire.read();
    Wire.endTransmission();

    if((sensor_value & 0x03) > 0) {
        return false;
    }
    
    return true;
}

/**
 * @brief To determine if the ADC (CH 9 through 16) is OKAY
 * 
 * @return true of false 
 * 
 */
bool adc128d_ch9to16_okay ( void ) {
    uint8_t sensor_value    = 0x00;
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS); 
    Wire.write(ADC_BUSY_REG_ADDR);        
    Wire.endTransmission();
    
    
    Wire.requestFrom(ADC_CH9TO16_ADDRESS, 1);    // Request 1 byte from the address   
    sensor_value = Wire.read();
    Wire.endTransmission();
    
    if((sensor_value & 0x03) > 0) {
        return false;
    }

    return true;
}

/**
 * @brief Check to verify the fuses are healthy
 * 
 * @return TODO: TBD 
 * 
 * With 100mA of current flowing through
 * the 0.5OHM sense resistor, 50mV will 
 * be developed across the resistor.  
 * The ADC is 12-bit with an 
 * internal reference of 2.56V. This relationship 
 * will yield 625uV per bit.  This will result 
 * in a bit value of ~80 when the input voltage 
 * is 50mV.
 * 
 */
void check_fuses (int ary[], uint8_t count) {
    uint8_t i                           = 0x00;
    uint16_t adc_raw_value              = 0x0000;
    
    
    /**
    * Set the fuse current 
    * to a low value so the 
    * fuses can be checked without 
    * setting off fireworks.
    * 
    * Keep this low since we're going
    * to turn them all on
    */
    set_fuse_current_ma(100);       //Value is in mA

    /**
     * Turn all fuses on before taking readings
     */
    enable_all_anlgsw();
    delay(200);
    
    /**
     * Stop the ADC
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);        
    Wire.write(0x00);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);       
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    delay(10);

    /**
     * Check to see if the ADCs are okay
     */
    if (!adc128d_ch1to8_okay()) 
    {        
        if(ENABLE_LOGGING_ADC_RELATED)
        {
            Serial.println("ADC CH1-8 NOTOK.");
        }
    }
    
    if (!adc128d_ch9to16_okay()) 
    {  
        if(ENABLE_LOGGING_ADC_RELATED)
        {
            Serial.println("ADC CH9-16 NOTOK.");
        }
    }
    
    /* Read the associated value from the ADC */
    for (i=1; i <= count; i++) {
        adc_raw_value = get_adc_value(i);
        
        if(ENABLE_LOGGING_ADC_RELATED)
        {
            if(i<10)
            {
                Serial.print(" CH ");
            }
            else 
            {
                Serial.print("CH ");
            }
            
            Serial.print(i);
            Serial.print(" -- Raw analog value:  ");
            Serial.println(adc_raw_value);
        }
        
        if (adc_raw_value > FUSE_OK_DIG_THRESHOLD){
            ary[i-1] = 1;
        }
        else {
            ary[i-1] = 0;
        }
    }


    /**
     * Kill all the analog switches 
     * before exiting
    */
    disable_all_anlgsw();
    
    /**
     * Put the fuse current back
     * before exiting
    */
    set_fuse_current_ma(fuse_current_ma);

    /**
     * Put the ADCs back into constant 
     * conversion mode
    */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);        
    Wire.write(0x01);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);       
    Wire.write(0x01);                   
    Wire.endTransmission();
    
}

/**
 * @brief Initialize I2C
 * 
 * @return nothing
 * 
*/
void init_i2c ( void ) 
{
    // The following is required to enable I2C lines
    Wire.begin(I2C_SDA, I2C_SCL);

    //          Frequency of clock speed in Hz
    //             |
    Wire.setClock(I2C_CLK_SPEED);
}

/**
 * @brief Initialize the ADC
 * 
 * @return nothing
 * 
*/
void init_adc ( void ) {
    uint8_t adc_value;

    /**
     * The initialization steps for using this 
     * ADC can be found on page 33 or 49 of the datasheet 
     * under section "Quick Start".  
     * 
     * Here's an abbreviated description of what needs to happen
     * 1) Wait for the device to be ready;
     * 2) Progarm the Advanced Configuration Register
     * 3) Program the conversion Rate Resister
     * 4) Enable / disable channels (all enabled by default)
     * 6) Mask interrups (all masked by default)
     * 7)Program limit registers (mainly if you intend to take advantage of the interrupt bit)
     * 8) Set the 'START' bit in the configuration register
     * 9) Clear interrupts
     */

    /**
     * Wait for the device to be ready 
     * before writing to the advanced configuration 
     * register.  
     * 
     * Calling functions "ADC OKAY" functions will 
     * cause the applications to crash 
     */
    delay(300);
    
    
    /**
     * PROGRAM ADVANCED CONFIGURATION REGISTER (ADDRESS 0x0B)
     * 
     * The device needs to be placed into mode 1.
     * Mode 1 defines that all channels are ADC inputs. 
     * The default mode only allows for channels 0 to 6
     * to be ADC input channels, while one "channel 7" is reserved
     * for taking a temperature measurement (see page 25 in the 
     * datasheet for more informaiton).
     *  
     * Here is a breakdown of that register (p24/49)
     *                                         
     *     BIT 7 (RESERVED)                                    
     *       |     BIT 6 (RESERVED)                              
     *       |     |     BIT 5 (RESERVED)                       
     *       |     |     |     BIT 4 (RESERVED)               
     *       |     |     |     |     BIT 3 (RESERVED)            
     *       |     |     |     |     |     BIT 2 (MODE SELECT [1])       
     *       |     |     |     |     |     |     BIT 1 (MODE SELECT [0])     
     *       |     |     |     |     |     |     |     BIT 0 (EXTERNAL REFERENCE ENABLE) 
     *       |     |     |     |     |     |     |     |
     *   0b  0     0     0     0     0     0     1     0    ===   0x02
     *                                      
     *       ┌───────┬─────────┬──────┐         
     *       │MODE[1]│ MODE[0] │ MODE │         
     *       ├───────┼─────────┼──────┤         
     *       │   0   │    0    │ MODE0│         
     *       │   0   │    1    │ MODE1│         
     *       │   1   │    0    │ MODE2│         
     *       │   1   │    1    │ MODE3│         
     *       └───────┴─────────┴──────┘         
     *                                 
     * The ADC shall operate in mode one, which means all channels will 
     * be ADC inputs
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_ADV_CONFIG_REG_ADDR);        
    Wire.write(0x02);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_ADV_CONFIG_REG_ADDR);        
    Wire.write(0x02);                   // Enable the device and clear interrupts 
    Wire.endTransmission();

    /**
     * PROGRAM CONVERSION RATE REGISTER (ADDRESS 0x07)
     * 
     * The conversion rate can only be modified when 
     * the chip is disabled!!
     * 
     *     BIT 7 (RESERVED)                                    
     *       |     BIT 6 (RESERVED)                              
     *       |     |     BIT 5 (RESERVED)                       
     *       |     |     |     BIT 4 (RESERVED)               
     *       |     |     |     |     BIT 3 (RESERVED)            
     *       |     |     |     |     |     BIT 2 (RESERVED)       
     *       |     |     |     |     |     |     BIT 1 (RESERVED)     
     *       |     |     |     |     |     |     |     BIT 0 (CONVERSION RATE) 
     *       |     |     |     |     |     |     |     |
     *   0b  0     0     0     0     0     0     0     1    ===   0x01
     *   
     *    CONVERSION RATE BIT = 0 = Low Power Conversion Mode 
     *    CONVERSION RATE BIT = 1 = Continuous Conversion Mode
     *              ** Note, this register can only be programmed 
     *                  when the device is in shutdown mode, 
     *                  that is, when the 'START' bit of the "CONFIGURATION REGISTER' is 0 
     * 
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_CONV_RATE_REG_ADDR);        
    Wire.write(0x01);                   // Request continuous conversion rate 
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_CONV_RATE_REG_ADDR);        
    Wire.write(0x01);                   
    Wire.endTransmission();
    
    /**
     * All channels, by default, are enabled
     * See register 0x08 (channel disable register) for more information
     */
    
     /**
     * All channels, by default, are masked from 
     * creating an interrupt.  
     * See register 0x03 (interrupt mask register on p21) for more information
     */
     
     /**
     * PROGRAM LIMIT REGISTERS 0x2A to 0x39 (p26 ot 49)
     * 
     * See register 0x2A-0x39 (limit registers on p26) for more information
     * 
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN0_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN0_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN1_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN1_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN2_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN2_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN3_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN3_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN4_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN4_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN5_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN5_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN6_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN6_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN7_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_IN7_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN0_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN0_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN1_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN1_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN2_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN2_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN3_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN3_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN4_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN4_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN5_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN5_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN6_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN6_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN7_HIGH_LIM_REG);        
    Wire.write(0xFF);                   
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_IN7_LOW_LIM_REG);        
    Wire.write(0x00);                   
    Wire.endTransmission();
       
    /**
     * WRITE TO THE CONFIGURATION REGISTER TO START THE ADC (ADDRESS 0x00)
     * 
     * NOTE: THIS SHALL BE DONE BEFORE CLEARING THE INIT BIT
     * 
     *     BIT 7 (INITIALIZATION)                                    
     *       |     BIT 6 (RESERVED)                              
     *       |     |     BIT 5 (RESERVED)                       
     *       |     |     |     BIT 4 (RESERVED)               
     *       |     |     |     |     BIT 3 (#INT_CLEAR)            
     *       |     |     |     |     |     BIT 2 (RESERVED)       
     *       |     |     |     |     |     |     BIT 1 (#INT_ENABLE)     
     *       |     |     |     |     |     |     |     BIT 0 (START) 
     *       |     |     |     |     |     |     |     |
     *   0b  0     0     0     0     0     0     0     1    === 0x01
     * 
     * 
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);        
    Wire.write(0x01);                   // Enable the device and clear interrupts 
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_CONFIG_REG_ADDRESS);       
    Wire.write(0x01);                   
    Wire.endTransmission();
    
    /**
     * READ THE CONTENTS OF THE INTERRUPT STATUS REGISTER TO CLEAR ANY INTERRUPTS (ADDRESS 0x01)
     * 
     *     BIT 7 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)                                    
     *       |     BIT 6 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)                              
     *       |     |     BIT 5 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)                       
     *       |     |     |     BIT 4 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)               
     *       |     |     |     |     BIT 3 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)            
     *       |     |     |     |     |     BIT 2 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)       
     *       |     |     |     |     |     |     BIT 1 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT)     
     *       |     |     |     |     |     |     |     BIT 0 (A HI OR LO LIMIT ON THIS CHANNEL TRIGGERED AN INTERRUPT) 
     *       |     |     |     |     |     |     |     |
     *   0b  0     0     0     0     1     0     0     1    >> READ ONLY
     */

    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.write(ADC_INT_STAT_REG_ADDR);        
    Wire.endTransmission();

    Wire.requestFrom(ADC_CH1TO8_ADDRESS, 1);   
    adc_value = Wire.read();
    Wire.endTransmission();
    
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS); 
    Wire.write(ADC_INT_STAT_REG_ADDR);        
    Wire.endTransmission();

    Wire.requestFrom(ADC_CH9TO16_ADDRESS, 1);   
    adc_value = Wire.read();
    Wire.endTransmission();
    
}

/**
 * @brief Get ADC value from ADC for specified channel
 * 
 * @param number this is a value in
 * the range of 1-16.
 * 
 * If the Register Address value is unknown, 
 * write to the ADC128D818 with the Serial Bus Address byte,
 * followed by the desired Register Address byte. 
 * Then restart the Serial Communication with a Read
 * consisting of the Serial Bus Address byte, followed 
 * by the Data byte read from the ADC128D818.
 * 
*/
uint16_t get_adc_value (uint8_t adc_ch1thru16) {
    uint8_t     adc_zero_based_ch_reg_addr      = 0x00;
    uint8_t     zero_based_adc_ch               = 0x00;
    uint8_t     i2c_byte_ctr                    = 0x00;
    uint8_t     adc_lower_nibble                = 0x00;
    uint8_t     adc_upper_nibble                = 0x00;
    uint16_t    address                         = 0x0000;
    uint16_t    adc_raw_value                   = 0x0000;

    /**
     * The number can't be negative 
     * Make sure the value isn't larger 
     * than 16
    */
    if(adc_ch1thru16 > 16) {
        adc_ch1thru16 = 1;
    }

    if(adc_ch1thru16 > 8) {
        zero_based_adc_ch = adc_ch1thru16 - 9;  //Zero-based
    }
    else {
        zero_based_adc_ch = adc_ch1thru16 - 1;  //Zero-based
    }

    (adc_ch1thru16 > 8)?(address = ADC_CH9TO16_ADDRESS):(address = ADC_CH1TO8_ADDRESS);
    
    /**
     * Set the address
     * of the internal register.  
     * According to the datasheet
     * for the IC, the data 
     * registers start at 0x20
     */
    adc_zero_based_ch_reg_addr  = (uint8_t)(ADC_CH_READ_START_ADDR + zero_based_adc_ch);

    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(address);
    Wire.write(adc_zero_based_ch_reg_addr);        
    Wire.endTransmission();

    /**
     * Read the data (two-bytes)
     * (12-bit) right justified
     * Here the address would be 
     * "preset" from the previous 
     * write
     */
    Wire.beginTransmission(address); 
    Wire.requestFrom(address, 2);    // Request 2 byte from the address
    
    while(Wire.available() && i2c_byte_ctr < 2) {
        if(i2c_byte_ctr == 0) {
            adc_upper_nibble = Wire.read() & 0x0F; //Only want lower four bits here.
        }
        else {
            adc_lower_nibble = Wire.read();
        }
        
        i2c_byte_ctr++;
    }
    
    Wire.endTransmission();
    
    adc_raw_value = (uint16_t)((adc_upper_nibble << 8 ) | (adc_lower_nibble)) ;
    
    if(ENABLE_LOGGING)
    {
        Serial.println("Finished getting ADC value.");
    }
    
    return adc_raw_value;
}

/**
 * @brief Read the interrupt status register from the CH1 through 8 ADC
 * 
 * @param nothing
 * 
 * @return nothing
 * 
 * The purpose of this function is 
 * to read from the interrupt status register 
 * within the ADC.   
 * 
*/
uint8_t get_adc1thru8_int_stat_reg ( void ){
    uint8_t status_byte = 0x00;

    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_INT_STAT_REG_ADDR);        
    Wire.endTransmission();

    /**
     * Read the contents of the 
     * register  
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.requestFrom(ADC_CH1TO8_ADDRESS, 1);    // Request 1 byte from the address

    status_byte = Wire.read();

    Wire.endTransmission();

    return status_byte;

}

/**
 * @brief Read the interrupt status register from the CH9 through 16 ADC
 * 
 * @param nothing
 * 
 * @return nothing
 * 
 * The purpose of this function is 
 * to read from the interrupt status register 
 * within the ADC.   
 * 
*/
uint8_t get_adc9thru16_int_stat_reg ( void ){
    uint8_t status_byte = 0x00;

    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_INT_STAT_REG_ADDR);        
    Wire.endTransmission();


    /**
     * Read the contents of the 
     * register  
     */
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS); 
    Wire.requestFrom(ADC_CH9TO16_ADDRESS, 1);    // Request 1 byte from the address


    status_byte = Wire.read();

    Wire.endTransmission();

    return status_byte;

}

/**
 * @brief Get the manufacture ID value from the CH1 to CH8 ADC
 * 
 * @param none
 * 
 * @return Manufacture ID of the ch1 though ch8 adc
*/
uint8_t get_adc1thru8_mfgid ( void ) {
    uint8_t mfgid = 0x00;
    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_MFGID_REG_ADDR);        
    Wire.endTransmission();


    /**
     * Read the byte back from
     * the ADC.  
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.requestFrom(ADC_CH1TO8_ADDRESS, 1);    // Request 1 byte from the address

    mfgid = Wire.read(); 
    Wire.endTransmission();

    return mfgid;
}

/**
 * @brief Get the manufacture ID value from the CH9 to CH16 ADC
 * 
 * @param none
 * 
 * @return Manufacture ID of the ch9 though ch16 adc
*/
uint8_t get_adc9thru16_mfgid ( void ) {
    uint8_t mfgid = 0x00;
    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_MFGID_REG_ADDR);        
    Wire.endTransmission();

    /**
     * Read the byte back from
     * the ADC.  
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.requestFrom(ADC_CH9TO16_ADDRESS, 1);    // Request 1 byte from the address

    mfgid = Wire.read(); 
    Wire.endTransmission();

    return mfgid;
}

/**
 * @brief Get the revision ID from the CH1 to CH8 ADC
 * 
 * @param none
 * 
 * @return Revision ID of the ch1 though ch8 adc
*/
uint8_t get_adc1thru8_revid ( void ) {
    uint8_t revid = 0x00;
    
    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS);
    Wire.write(ADC_REVID_REG_ADDR);        
    Wire.endTransmission();

    /**
     * Read the byte back from
     * the ADC.  
     */
    Wire.beginTransmission(ADC_CH1TO8_ADDRESS); 
    Wire.requestFrom(ADC_CH1TO8_ADDRESS, 1);    // Request 1 byte from the address

    revid = Wire.read(); 
    Wire.endTransmission();

    return revid;
}

/**
 * @brief Get the revision ID from the CH9 to CH16 ADC
 * 
 * @param none
 * 
 * @return Revision ID of the ch9 though ch16 adc
*/
uint8_t get_adc9thru16_revid ( void ) {
    uint8_t revid = 0x00;
    
    /** 
     * Set the address of the internal register
     */
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS);
    Wire.write(ADC_REVID_REG_ADDR);        
    Wire.endTransmission();

    /**
     * Read the byte back from
     * the ADC.  
     */
    Wire.beginTransmission(ADC_CH9TO16_ADDRESS); 
    Wire.requestFrom(ADC_CH9TO16_ADDRESS, 1);    // Request 1 byte from the address

    revid = Wire.read(); 
    Wire.endTransmission();

    return revid;
}

/**
 * @brief Set an IO of the GPIO expander
 * 
 * @param number This is a value of 1 to 16.
 * where 1:8 is the lower bank of the expander (p0p7)
 * while 9:16 maps to the upper bank of the expander
 * o8o15
*/
bool set_ioxpander_gpio (uint8_t number) {
    int address                 = 0x00;
    uint8_t gpio_read_value     = 0x00;
    uint8_t shift_value         = 0x00;
    uint8_t value_mask          = 0x00;

    /**
     * The number can't be negative 
     * Make sure the value isn't larger 
     * than 16
    */
    if(number > 16) {
        number = 1;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = IO_EXPANDER_ADDR_O8TO15):(address = IO_EXPANDER_ADDR_P0TO7);

    /* Determine the value_mask */
    value_mask = (0b00000001) << (shift_value);
    if(ENABLE_LOGGING)
    {
        Serial.print("Mask --> ");
        print8b_bin(value_mask);
    }

    /**
     * Do not want to overwrite 
     * outputs that are already 
     * set, so read from the IO 
     * expander
    */
    Wire.beginTransmission(address); 
    Wire.requestFrom(address, 1);    // Request 1 byte from the address

    gpio_read_value = Wire.read();  

    if(ENABLE_LOGGING)
    {
        Serial.print("GPIO Read --> ");
        print8b_bin(gpio_read_value);
    }

    /**
     * Modify the value read from the
     * GPIO expander so the only the value
     * of interested is modified
    */
    gpio_read_value = (value_mask) | gpio_read_value;

    /**
     * Write the modified IO value back 
     * to the IO expander 
    */
    Wire.write(gpio_read_value);        // The value has to be sent twice to guarantee a submittal
    Wire.endTransmission();

    return true;
}

/**
 * @brief Clear an IO of the GPIO expander
 * 
 * @param number This is a value of 1 to 16.
 * where 1:8 is the lower bank of the expander (p0p7)
 * while 9:16 maps to the upper bank of the expander
 * o8o15
*/
bool clear_ioxpander_gpio (uint8_t number) {
    int address                 = 0;
    uint8_t gpio_read_value     = 0x00;
    uint8_t shift_value         = 0;
    uint8_t value_mask          = 0x00;

    /**
     * The number can't be negative 
     * Make sure the value isn't larger 
     * than 16
    */
    if(number > 16) {
        number = 1;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = IO_EXPANDER_ADDR_O8TO15):(address = IO_EXPANDER_ADDR_P0TO7);

    /* The IO needs to be constrained to 0-7*/
    value_mask = (0b00000001) << (shift_value);
    
    /**
     * Generate the value_mask
     * where the bit we want to
     * clear shall be a zero while 
     * all others shall be a one
    */
    value_mask ^= (0xFF);
    if(ENABLE_LOGGING) 
    {
        Serial.print("Mask: ");
        print8b_bin(value_mask);
    }

    Wire.beginTransmission(address); 
    
    Wire.requestFrom(address, 1);    // Request 1 byte from the address

    gpio_read_value = Wire.read();    
    
    if(ENABLE_LOGGING) 
    {
        Serial.print("GPIO Read: ");
        print8b_bin(gpio_read_value);
    }
    
    /**
     * Now clear just the io of
     * interest
    */
    gpio_read_value = (value_mask) & gpio_read_value;


    /**
     * Write the modified IO value back 
     * to the IO expander 
    */
    Wire.write(gpio_read_value);        // The value has to be sent twice to guarantee a submittal
    Wire.endTransmission();

    return true;
}

/**
 * @brief Clear IO associated by the IO expander
 * 
 * @return true or false
*/
bool clear_ioexp_io (void) {
    Wire.beginTransmission(IO_EXPANDER_ADDR_O8TO15); 
    Wire.write(0x00);        // send 0b0000 twice so the value takes
    Wire.endTransmission();
    
    
    Wire.beginTransmission(IO_EXPANDER_ADDR_P0TO7); 
    Wire.write(0x00);        // send 0b0000 twice so the value takes
    Wire.endTransmission();

    return true;
}

bool set_anlgsw (uint8_t number) {
    
    int address                 = 0x00;
    uint8_t shift_value         = 0x00;
    uint8_t anlgsw_val          = 0x00;

    /**
     * The number can't be negative 
     * Make sure the value isn't larger 
     * than 16
    */
    if(number > 16) {
        number = 1;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = ANLG_SW_CH9to16_ADDRESS):(address = ANLG_SW_CH1to8_ADDRESS);

    /* Determine the anlgsw_val */
    anlgsw_val = (0b00000001) << (shift_value);
    
    /**
     * Begin I2C tranmission to 
     * the appropriate chip
    */
    Wire.beginTransmission(address); 

     /**
     * Write the analog 
     * switch value to the IC
    */
    Wire.write(0x00);           // Register selection byte 
    Wire.write(anlgsw_val);     // The value has to be sent twice to guarantee a submittal
    Wire.endTransmission();

    return true;
}

bool enable_all_anlgsw( void )
{
    /**
     * Enable all channels of the 
     * analog switch that is associated
     * with channels 1 through 8
    */
    Wire.beginTransmission(ANLG_SW_CH1to8_ADDRESS); 
    Wire.write(0x00);           // Register selection byte 
    Wire.write(0xFF);     // Value we want to send
    Wire.endTransmission();
    
    /**
     * Enable all channels of the 
     * analog switch that is associated
     * with channels 9 through 16
    */
    Wire.beginTransmission(ANLG_SW_CH9to16_ADDRESS); 
    Wire.write(0x00);           // Register selection byte 
    Wire.write(0xFF);     // Value we want to send
    Wire.endTransmission();
    
    return true;
}

bool disable_all_anlgsw ( void ) 
{
    Wire.beginTransmission(ANLG_SW_CH1to8_ADDRESS); 
    Wire.write(0x00);        
    Wire.write(0x00);        
    Wire.endTransmission();
    
    Wire.beginTransmission(ANLG_SW_CH9to16_ADDRESS); 
    Wire.write(0x00);        
    Wire.write(0x00);        
    Wire.endTransmission();

    return true;
}