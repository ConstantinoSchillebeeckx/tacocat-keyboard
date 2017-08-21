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
	{28,KEY_MENU},{29,KEY_TILDE},{30,KEY_1},{31,KEY_2},{32,KEY_3},{33,KEY_4},{33,KEY_5},
	{21,KEY_TAB},{22,KEY_Q},{23,KEY_W},{24,KEY_E},{25,KEY_R},{26,KEY_T},
	{14,MODIFIERKEY_LEFT_SHIFT},{15,KEY_A},{16,KEY_S},{17,KEY_D},{18,KEY_F},{19,KEY_G},
	{7,MODIFIERKEY_LEFT_SHIFT},{8,KEY_Z},{9,KEY_X},{10,KEY_C},{11,KEY_V},{14,KEY_B},{13,KEY_MENU},
	{0,MODIFIERKEY_LEFT_CTRL},{1,MODIFIERKEY_LEFT_ALT},{2,MODIFIERKEY_LEFT_GUI},{3,KEY_MENU},{4,KEY_BACKSPACE},{5,KEY_SPACE}
};

//Right keyboard
/*
const struct key_mapping keys[] =
{
    {30, KEY_MUTE}, {31, KEY_VOLUME_DOWN}, {32, KEY_VOLUME_UP}, {33, KEY_VOLUME_DOWN}, {34, KEY_VOLUME_UP},
    {24,KEY_6},{25,KEY_7},{26,KEY_8},{27,KEY_9},{28,KEY_0},{29,KEY_MINUS},
    {18,KEY_Y},{19,KEY_U},{20,KEY_I},{21,KEY_O},{22,KEY_P},{23,KEY_EQUAL},
    {12,KEY_H},{13,KEY_J},{14,KEY_K},{15,KEY_L},{16,KEY_SEMICOLON},{17,KEY_BACKSLASH},
    {6,KEY_N}, {7,KEY_M}, {8,KEY_COMMA},{9,KEY_PERIOD},{10,KEY_UP},{11,KEY_QUOTE},
    {0,KEY_SPACE},{1,KEY_BACKSPACE},{2,KEY_ENTER},{3,KEY_LEFT},{4,KEY_DOWN},{5,KEY_RIGHT}
};
*/


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
    for (int i = 0; i < 64; ++i)
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
