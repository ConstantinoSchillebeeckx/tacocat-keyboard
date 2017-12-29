//  HardwareController.h
//  Created by Will Yager on 7/3/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Controls the various non-communication hardware peripherals

#ifndef __keyboard2__HardwareController__
#define __keyboard2__HardwareController__

#include "Types.h"

class HardwareController {
private:
public:
    HardwareController();
    //Simply iterates through all PHYSICAL keys (i.e. buttons) and
    //sets their corresponding bit value to true if they are pressed
    //and then returns the corresponding ButtonsState object
    ButtonsState update();
};

#endif /* defined(__keyboard2__HardwareController__) */
