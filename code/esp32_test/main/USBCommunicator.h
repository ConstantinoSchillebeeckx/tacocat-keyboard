//  USBCommunicator.h
//  Created by William Yager on 10/12/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Talks to a USB master.

#ifndef ____USBCommunicator__
#define ____USBCommunicator__

#include "Types.h"

class USBCommunicator {
private:
	bool _is_master;
public:
    USBCommunicator(bool is_master);
    //Sends any USB key press events to the master. Returns the LED status.
    uint8_t update(const KeysDelta& key_changes, const KeysDelta& slave_key_changes);
    bool is_master();
    void send_key_event(const KeyDelta& event);
};

#endif /* defined(____USBCommunicator__) */
