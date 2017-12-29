//  KeyMapper.cpp
//  Created by Will Yager on 7/3/13.
//  Copyright (c) 2013 Will Yager. All rights reserved.
//  Maps physical buttons to USB spec key codes.

#include "KeyMapper.h"
#include "keylayouts.h"
#include <stdio.h>

//Ties a physical button number to a USB key spec number
struct key_mapping {
    //Physical button
    uint8_t button;
    //USB key data
    uint16_t key;
};

//The following should be changed to reflect the actual physical key number/key value pairs
//So {1, KEY_A} means physical button 1 is the 'a' key.
//The following layouts mimic their physical layout. The button number is, in cartesian coordinates, x+6y
/*
28 29 30 31 32 33 34
21 22 23 24 25 26
14 15 16 17 18 19
7 8 9 10 11 12 13
0 1 2 3 4 5
*/

//Left keyboard
// KEY_MENU are unassigned keys for now
const struct key_mapping keys[] =
{
	{28,KEY_ESC},{29,KEY_TILDE},{30,KEY_1},{31,KEY_2},{32,KEY_3},{33,KEY_4},{34,KEY_5},
	{21,KEY_TAB},{22,KEY_Q},{23,KEY_W},{24,KEY_E},{25,KEY_R},{26,KEY_T},
	{14,MODIFIERKEY_LEFT_SHIFT},{15,KEY_A},{16,KEY_S},{17,KEY_D},{18,KEY_F},{19,KEY_G},
	{7,MODIFIERKEY_LEFT_SHIFT},{8,KEY_Z},{9,KEY_X},{10,KEY_C},{11,KEY_V},{12,KEY_B},{13,KEY_MENU},
	{0,MODIFIERKEY_LEFT_CTRL},{1,MODIFIERKEY_LEFT_ALT},{2,MODIFIERKEY_LEFT_GUI},{3,KEY_MENU},{4,KEY_BACKSPACE},{5,KEY_SPACE}
};

//Right keyboard (slave)
// const struct key_mapping keys[] =
// {
// 	{28,KEY_MENU},{29,KEYPAD_PLUS},{30,KEY_0},{31,KEY_9},{32,KEY_8},{33,KEY_7},{34,KEY_6},
// 	{21,KEY_MENU},{22,KEY_P},{23,KEY_O},{24,KEY_I},{25,KEY_U},{26,KEY_Y},
// 	{14,KEY_QUOTE},{15,KEY_SEMICOLON},{16,KEY_L},{17,KEY_K},{18,KEY_J},{19,KEY_H},
// 	{7,KEY_SLASH},{8,KEY_UP},{9,KEY_PERIOD},{10,KEY_COMMA},{11,KEY_M},{12,KEY_N},{13,KEY_MENU},
// 	{0,KEY_RIGHT},{1,KEY_DOWN},{2,KEY_LEFT},{3,KEY_MENU},{4,KEY_DELETE},{5,KEY_ENTER}
// };
//
//Physical button number => Virtual button code
uint16_t KeyMapper::map_button_to_key(uint8_t button){
    for (uint8_t i = 0; i < (sizeof(keys)/sizeof(key_mapping)); ++i)
    {
        if(keys[i].button == button){
            return keys[i].key;
        }
    }
    return 0; //No button assignment found
}

//Takes a lot of button-number-indexed deltas and puts the relevant
//key-code-associated delta in the first free space of the result
//put data in highest indices because USBCommunicator::update
//assumes the non-zero deltas are packed in the lowest indices
KeysDelta KeyMapper::resolve(const ButtonsDelta& button_changes){
    KeysDelta result;
    uint8_t count = 0;
    for (int i = 0; i < 35; ++i)
    {
		result.deltas[i].delta = 0;
        if (button_changes.deltas[i] != 0)
        {
            result.deltas[count].delta = button_changes.deltas[i];
			result.deltas[count].key   = this->map_button_to_key(i);
            count++;
        }
    }
    return result;
}
