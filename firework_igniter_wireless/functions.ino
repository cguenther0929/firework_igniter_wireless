/**
 * @brief Print binary representation of a number
 * 
 * @return nothing
*/
void printBin(byte aByte) {
  for (int8_t aBit = 7; aBit >= 0; aBit--){
    Serial.print(bitRead(aByte, aBit) ? '1' : '0');
  }
  Serial.print('\n');
}

/**
 * @brief Display the first screen on the OLED
 * 
 * @return true or false
*/
bool display_screen ( void ){
    switch (current_screen){
        case SCREEN_1:
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(oled_column,first_row);
            u8x8.print("HOSTING AT IP");
            current_oled_row += lcd_row_spacing; 
            u8x8.setCursor(oled_column,current_oled_row);
            u8x8.print(assigned_ip_str);
            current_oled_row += lcd_row_spacing; 

            
            u8x8.setCursor(oled_column,current_oled_row);
            u8x8.print(String("SW V: " + SW_VERSION_STRING));
            current_oled_row += lcd_row_spacing;

            u8x8.setCursor(oled_column,current_oled_row);
            u8x8.print(String("HW V: " + HW_VERSION_STRING));
            current_oled_row += lcd_row_spacing;
        
        break;

        case SCREEN_2:
            // TODO: we shoule be able to remove the following 
            // String str_fuse_current = String(fuse_current_ma);
            
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(oled_column,first_row);
            u8x8.print("FUSE (mA)");
            current_oled_row += lcd_row_spacing; 
            u8x8.setCursor(oled_column,current_oled_row);
            u8x8.print(String(fuse_current_ma));
            current_oled_row += lcd_row_spacing; 


        break;

        default:
            u8x8.clear();
            current_oled_row = 0; 
            u8x8.setCursor(oled_column,first_row);
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
    
    // TODO: for debugging only
    // Serial.print("The calculated bit value: "); Serial.println(bit_value);

    // bit_value = ((maval/1000)*0.5*255)/3.3;

    set_dac_value(bit_value);
}

/**
 * @brief Set DAC per binary value that was passed in
 * 
 * @param binval binary value for the DAC setting (0-255)
 * 
 * @return nothing
*/
void set_dac_value ( uint8_t binval) {
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

    Wire.beginTransmission(dac_address); 
    Wire.write(high_nibble);        
    Wire.write(low_nibble);        
    Wire.endTransmission();

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
        number = 0;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = io_expander_7b_address_o8o15):(address = io_expander_7b_address_p0p7);

    /* Determine the value_mask */
    value_mask = (0b00000001) << (shift_value);
    #if defined(ENABLE_LOGGING)
        Serial.print("Mask --> ");
        printBin(value_mask);
    #endif

    /**
     * Do not want to overwrite 
     * outputs that are already 
     * set, so read from the IO 
     * expander
    */
    Wire.beginTransmission(address); 
    Wire.requestFrom(address, 1);    // Request 1 byte from the address

    gpio_read_value = Wire.read();  

    #if defined(ENABLE_LOGGING)
        Serial.print("GPIO Read --> ");
        printBin(gpio_read_value);
    #endif

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
        number = 0;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = io_expander_7b_address_o8o15):(address = io_expander_7b_address_p0p7);

    /* The IO needs to be constrained to 0-7*/
    value_mask = (0b00000001) << (shift_value);
    
    /**
     * Generate the value_mask
     * where the bit we want to
     * clear shall be a zero while 
     * all others shall be a one
    */
    value_mask ^= (0xFF);
    #if defined(ENABLE_LOGGING)
        Serial.print("Mask: ");
        printBin(value_mask);
    #endif

    Wire.beginTransmission(address); 
    
    Wire.requestFrom(address, 1);    // Request 1 byte from the address

    gpio_read_value = Wire.read();    
    
    #if defined(ENABLE_LOGGING)
        Serial.print("GPIO Read: ");
        printBin(gpio_read_value);
    #endif
    
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
    Wire.beginTransmission(io_expander_7b_address_o8o15); 
    Wire.write(0x00);        // send 0b0000 twice so the value takes
    Wire.endTransmission();
    
    
    Wire.beginTransmission(io_expander_7b_address_p0p7); 
    Wire.write(0x00);        // send 0b0000 twice so the value takes
    Wire.endTransmission();

    return true;
}

// TODO: this function is just for testing
bool set_all_anlgsw(void) {

    Wire.beginTransmission(anlg_sw_ch1to8_address); 
    Wire.write(0x00);        
    Wire.write(0xFF);        
    Wire.endTransmission();
    
    Wire.beginTransmission(anlg_sw_ch9to16_address); 
    Wire.write(0x00);        
    Wire.write(0xFF);        
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
        number = 0;
    }

    if(number > 8) {
        shift_value = number - 9;
    }
    else {
        shift_value = number - 1;
    }

    (number > 8)?(address = anlg_sw_ch9to16_address):(address = anlg_sw_ch1to8_address);

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

bool disable_all_anlgsw ( void ) {
    Wire.beginTransmission(anlg_sw_ch1to8_address); 
    Wire.write(0x00);        
    Wire.write(0x00);        
    Wire.endTransmission();
    
    Wire.beginTransmission(anlg_sw_ch9to16_address); 
    Wire.write(0x00);        
    Wire.write(0x00);        
    Wire.endTransmission();



    return true;
}