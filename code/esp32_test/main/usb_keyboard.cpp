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

// Version 1.0: Initial Release
// Version 1.1: Add support for Teensy 2.0

#define USB_SERIAL_PRIVATE_INCLUDE
#include <esp_log.h>
#include <stdio.h>
#include "usb_keyboard.h"
#include <bitset>
#include <iostream>


// zero when we are not configured, non-zero when enumerated
static volatile uint8_t usb_configuration=0;


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

// return 0 if the USB is not configured, or the configuration
// number selected by the HOST
uint8_t usb_configured(void) {
	return 1;
	// return usb_configuration;
}



// send the contents of keyboard_keys and keyboard_modifier_keys
// https://folk.uio.no/jeanra/Microelectronics/TransmitStructArduinoPython.html
int8_t usb_keyboard_send(void)
{

	std::cout<<std::bitset<8>(to_send.modifier).to_string();
	for(unsigned char i=0; i<6; i++){
		std::string binary = std::bitset<8>(to_send.keyboard_keys[i]).to_string();
		std::cout<<binary;
		// printf("%i", to_send.keyboard_keys[i]);
	}
	printf("\n");

	return 0;
}


// Misc functions to wait for ready and send/receive packets
static inline void usb_wait_in_ready(void)
{
	// while (!(UEINTX & (1<<TXINI))) ;
}
static inline void usb_send_in(void)
{
	// UEINTX = ~(1<<TXINI);
}
static inline void usb_wait_receive_out(void)
{
	// while (!(UEINTX & (1<<RXOUTI))) ;
}
static inline void usb_ack_out(void)
{
	// UEINTX = ~(1<<RXOUTI);
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
			// printf("press %i\n", key);
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
