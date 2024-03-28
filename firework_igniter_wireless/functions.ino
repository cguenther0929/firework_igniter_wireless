/**
 * @brief Set a GPIO from number passed in
 * 
 * Value passed in shall be 1-16 
 * (not 0 to 15)
 * 
 * @return true or false
*/

    bool clear_gpio (void) {
        Wire.beginTransmission(io_expander_7b_address_o8o15); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();
        
        
        Wire.beginTransmission(io_expander_7b_address_p0p7); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();

        return true;

    }

    /**
     * @brief Set an IO of the GPIO expander
     * 
     * @param number This is a value of 1 to 16.
     * where 1:8 is the lower bank of the expander (p0p7)
     * while 9:16 maps to the upper bank of the expander
     * o8o15
    */
    bool set_gpio (uint8_t number) {
        int address = 0;
        uint8_t shift_value=0;
        uint8_t value = 0x00;

        /**
         *  The number can't be negative 
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
        value = (0b00000001) << (shift_value);

        Wire.beginTransmission(address); 
        Wire.write(value);        // The value has to be sent twice to guarantee a submittal
        Wire.endTransmission();

        return true;
    }