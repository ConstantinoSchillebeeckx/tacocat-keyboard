//  SlaveNotifier.cpp
//  Created by William Yager on 10/12/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.

#include "SlaveNotifier.h"

KeysDelta SlaveNotifier::update(uint8_t led_status){

    KeysDelta result = {}; //Initialize to 0

    //No slave. Just return the empty KeysDelta
	if(!i2c_communicator.is_master()) return result;

    result = i2c_communicator.request_packet();

    return result;
}
