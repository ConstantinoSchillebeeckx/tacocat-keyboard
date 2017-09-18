//  MasterNotifier.cpp
//  Created by Will Yager on 7/3/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Tells the master (either USB or I2C) about any key changes we've seen lately

#include "MasterNotifier.h"
#include <stdio.h>


MasterNotifier::MasterNotifier(USBCommunicator& usb, I2CCommunicator& i2c):usb_communicator(usb),i2c_communicator(i2c){
    // while(!usb_communicator.is_connected()){}

    //We are connected via USB as master
    if (usb_communicator.is_master()){
        this->is_master = true;
    }
	//We are connected via I2C only as slave
	else {
		this->is_master = false;
	}

}

//Updates the master about the relevant key changes. Master could be a keyboard over I2C or a computer over USB.
uint8_t MasterNotifier::notify(const KeysDelta& key_changes, const KeysDelta& slave_key_changes){
    uint8_t led_status;

    //If we are connected via USB
    // led_status = this->usb_communicator.update(key_changes, slave_key_changes);

    //If we are connected via USB
    if(this->is_master) {
		led_status = this->usb_communicator.update(key_changes, slave_key_changes);
	} else {
	    //If we are only connected via I2C
		led_status = this->i2c_communicator.send_packet(key_changes);
	}

    return led_status;
}
