//  USBCommunicator.cpp
//  Created by William Yager on 10/12/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Responsible for communicating with the host PC (if one is attached)

#include "USBCommunicator.h"
#include <stdio.h>
#include "usb_keyboard.h"

USBCommunicator::USBCommunicator(bool is_master){
	this->_is_master = is_master;
    usb_init(); //Turn on USB hardware
}

//Sends all deltas in key_changes and slave_key_changes to the host PC.
//Assumes the non-zero deltas are packed in the lowest indices
uint8_t USBCommunicator::update(const KeysDelta& key_changes, const KeysDelta& slave_key_changes){
    uint8_t i = 0;
    while(key_changes.deltas[i].delta != 0){
        this->send_key_event(key_changes.deltas[i]);
        i++;
    }
	i = 0;
	while(slave_key_changes.deltas[i].delta != 0){
		this->send_key_event(slave_key_changes.deltas[i]);
		i++;
	}
	usb_keyboard_send();
    return keyboard_leds;
}

//Returns true ESP32 is master (connected to PC)
// we can't check whether the usb
// is properly connected, so instead
// we return 1 if the keyboard is the master
// or 0 if we are the slave
bool USBCommunicator::is_master(){
	return this->_is_master;
}

void USBCommunicator::send_key_event(const KeyDelta& event){
    if(event.delta > 0) usb_press(event.key);
    if(event.delta < 0) usb_release(event.key);
}
