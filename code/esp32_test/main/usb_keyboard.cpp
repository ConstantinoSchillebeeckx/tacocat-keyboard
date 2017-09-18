/* USB Keyboard Example for Teensy USB Development Board
 * http://www.pjrc.com/teensy/usb_keyboard.html
 * Copyright (c) 2009 PJRC.COM, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <esp_log.h>
#include <stdio.h>
#include <bitset>
#include <iostream>
#include "usb_keyboard.h"
#include "KeyMapper.h"




// we send this to the host, to be processed by python script
struct package {
	// which modifier keys are currently pressed
	// 1=left ctrl,    2=left shift,   4=left alt,    8=left gui
	// 16=right ctrl, 32=right shift, 64=right alt, 128=right gui
	uint8_t modifier;
	uint8_t keyboard_keys[6]; // which keys are currently pressed, up to 6 keys may be down at once
};

// instanciate one struct
package to_send = {};


// 1=num lock, 2=caps lock, 4=scroll lock, 8=compose, 16=kana
volatile uint8_t keyboard_leds=0;




/**************************************************************************
 *
 *  Public Functions - these are the API intended for the user
 *
 **************************************************************************/


// initialize USB
void usb_init(void)
{
}


// send the contents of keyboard_keys and keyboard_modifier_keys
// to the host computer to be parsed by the python script
int8_t usb_keyboard_send(void)
{

	std::cout<<std::bitset<8>(to_send.modifier).to_string();
	for(unsigned char i=0; i<6; i++){
		std::string binary = std::bitset<8>(to_send.keyboard_keys[i]).to_string();
		std::cout<<binary;
	}
	printf("\n");

	return 0;
}



void usb_press(uint16_t key){
    uint8_t msB = key >> 8;
    if(msB == 0x80){//modifier key
        usb_press_modifier_key(key & 0xFF);
    }
    else if(msB == 0x40){//standard key
        usb_press_standard_key(key & 0xFF);
    }
}

void usb_press_modifier_key(uint8_t key){
    if((to_send.modifier & key) != key){
        to_send.modifier |= key;
    }
}

void usb_press_standard_key(uint8_t key){
    for(unsigned char i=0; i<6; i++){
        if(to_send.keyboard_keys[i] == key) return; //Already pressed
    }
    for(unsigned char i=0; i<6; i++){
        if(to_send.keyboard_keys[i] == 0){ //Found a free space to put the key in
            to_send.keyboard_keys[i] = key;
            return;
        }
    }
}

void usb_release(uint16_t key){
    uint8_t msB = key >> 8;
    if(msB == 0x80){//modifier key
        usb_release_modifier_key(key & 0xFF);
    }
    else if(msB == 0x40){//standard key
        usb_release_standard_key(key & 0xFF);
    }
}

void usb_release_modifier_key(uint8_t key){
    if((to_send.modifier & key) != 0){
        to_send.modifier &= ~key;
    }
}

void usb_release_standard_key(uint8_t key){
    for(unsigned char i=0; i<6; i++){
        if(to_send.keyboard_keys[i] == key){
            to_send.keyboard_keys[i] = 0;
            return;
        }
    }
}
