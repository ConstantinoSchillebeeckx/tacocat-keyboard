#ifndef usb_serial_h__
#define usb_serial_h__

#include <stdint.h>

void usb_init(void);			// initialize everything
uint8_t is_master(void);		// is the USB port configured

//int8_t usb_keyboard_press(uint8_t key, uint8_t modifier);
int8_t usb_keyboard_send(void);
extern uint8_t keyboard_modifier_keys;
extern uint8_t keyboard_keys[6];
extern volatile uint8_t keyboard_leds;

void usb_press(uint16_t key);
void usb_press_modifier_key(uint8_t key);
void usb_press_standard_key(uint8_t key);

void usb_release(uint16_t key);
void usb_release_modifier_key(uint8_t key);
void usb_release_standard_key(uint8_t key);

#include "keylayouts.h"

#endif
