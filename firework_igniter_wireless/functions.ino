/**
 * @brief Set a GPIO from number passed in
 * 
 * Value passed in shall be 1-16 
 * (not 0 to 15)
 * 
 * @return true or false
*/

    bool clear_gpio (void) {
        Wire.beginTransmission(max3725_7b_address_o8o15); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();
        
        
        Wire.beginTransmission(max3725_7b_address_p0p7); // transmit to device #4
        Wire.write(0x00);        // send 0b0000 twice so the value takes
        Wire.endTransmission();

        return true;

    }

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

        (number > 8)?(address = max3725_7b_address_o8o15):(address = max3725_7b_address_p0p7);

        /* The IO needs to be constrained to 0-7*/
        value = (0b00000001) << (shift_value);

        Wire.beginTransmission(address); // transmit to device #4
        Wire.write(value);        // send 0b0000 twice so the value takes
        Wire.endTransmission();

        return true;
    }


    // void ICACHE_RAM_ATTR onTimerISR(){
    //     timer1_write(tmr1_write_val);

    //     Timer1msFlag = true;

    //     if(ms_ticks_1 == 50) {
    //         ms_ticks_1 = 0;
    //         Timer50msFlag = true;
    //         if(ms_ticks_50 == 10) {
    //         ms_ticks_50 = 0;               //Reset centi-seconds
    //         Timer500msFlag = true;
            
    //         if(ms_ticks_500 == 2) {         //See if we need to roll over
    //             ms_ticks_500 = 0;
    //             Timer1000msFlag = true;  
    //         }
    //         else {
    //             ms_ticks_500++;              //Increase 500ms timer
    //         }

    //         }
    //         else {
    //             ms_ticks_50++;
    //         }
            
    //     }
    //     else {
    //         ms_ticks_1++;
    //     }
  

    // }
