//  HardwareController.cpp
//  Created by Will Yager on 7/3/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Controls hardware peripherals and sets the LEDs

#include "HardwareController.h"
#include "ets_sys.h"
#include "driver/gpio.h"

//For nifty LED fade effects
/*
const uint8_t PROGMEM half_sin_curve[] = {0, 8, 16, 23, 31, 39, 47, 55, 63, 71,\
 78, 86, 93, 101, 108, 115, 122, 129, 136, 143, 149, 156, 162, 168,\
  174, 180, 185, 191, 196, 201, 206, 210, 215, 219, 223, 227, 230,\
   234, 237, 239, 242, 244, 246, 248, 250, 251, 252, 253, 254, 254,\
    255, 254, 254, 253, 252, 251, 250, 248, 246, 244, 242, 239, 237,\
     234, 230, 227, 223, 219, 215, 210, 206, 201, 196, 191, 185, 180,\
      174, 168, 162, 156, 149, 143, 136, 129, 122, 115, 108, 101, 93, \
       86, 78, 71, 63, 55, 47, 39, 31, 23, 16, 8, 0};
*/


//These functions map a left-to-right, bottom-to-top row/column
//number to an actual pin number and then do operations on them
#define row_size 5
const gpio_num_t row_pin_numbers[row_size] = {
	GPIO_NUM_19,
	GPIO_NUM_23,
	GPIO_NUM_18,
	GPIO_NUM_2,
	GPIO_NUM_16
};
#define GPIO_ROWS ((1<<row_pin_numbers[0]) | (1<<row_pin_numbers[1]) | (1<<row_pin_numbers[2]) | (1<<row_pin_numbers[3]) |(1<<row_pin_numbers[4]));

#define col_size 7
const gpio_num_t column_pin_numbers[col_size] = {
	GPIO_NUM_12,
	GPIO_NUM_33,
	GPIO_NUM_25,
	GPIO_NUM_26,
	GPIO_NUM_27,
	GPIO_NUM_14,
	GPIO_NUM_32
};
#define GPIO_COLS ((1ULL<<column_pin_numbers[0]) | (1ULL<<column_pin_numbers[1]) | (1ULL<<column_pin_numbers[2]) | (1ULL<<column_pin_numbers[3]) | (1ULL<<column_pin_numbers[4]) | (1ULL<<column_pin_numbers[5]) | (1ULL<<column_pin_numbers[6]));

//Simply iterates through all PHYSICAL keys and sets its corresponding value
//in the returned ButtonsState to true if the key is pressed.
ButtonsState HardwareController::update(){
	ButtonsState result = {};

    //Step 1: Pull row up (Weak) [done in config?]
    //Step 2: Pull column down (Strong)
    //Step 3: Mesure row pin. If low, key is pressed.
    //Step 4: Reset all things to in, low.
	// see: https://github.com/Chris--A/Keypad/blob/master/src/Keypad.cpp#L83
    for(uint8_t col = 0; col < col_size; col++){
		gpio_set_level(column_pin_numbers[col], 0);
        ets_delay_us(5); //Let signals propogate
		for(uint8_t row = 0; row < row_size; row++){
			if (gpio_get_level(row_pin_numbers[row]) == 0) {
				unsigned char button_number = row*col_size + col;
                result.states[button_number] = true;//Record the button as being pressed
			} //Scanned specific key
		} // Scanned all keys in row
		gpio_set_level(column_pin_numbers[col], 1);
    } //Scanned all keys

    return result;
}

HardwareController::HardwareController(){
	gpio_config_t io_conf;

	// setup ROWS
    io_conf.mode = GPIO_MODE_INPUT; //set as output mode
    io_conf.pin_bit_mask = GPIO_ROWS; 	//bit mask of the pins that you want to set,e.g. GPIO19
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down mode
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE; //enable pull-up mode
	io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf); //configure GPIO with the given settings

	// setup COLS
	io_conf.mode = GPIO_MODE_OUTPUT; //set as output mode
	io_conf.pin_bit_mask = GPIO_COLS; 	//bit mask of the pins that you want to set,e.g. GPIO12
	io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; //disable pull-down mode
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE; //disable pull-up mode
	io_conf.intr_type = GPIO_INTR_DISABLE;
	gpio_config(&io_conf); //configure GPIO with the given settings

	for (uint8_t col = 0; col < col_size; ++col) {
		gpio_set_level(column_pin_numbers[col], 1); // set all COLS high
	}

}
