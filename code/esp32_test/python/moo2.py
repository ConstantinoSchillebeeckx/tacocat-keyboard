#!/usr/bin/env python

import numpy as np
import time, sys, struct, serial
from pykeyboard import PyKeyboard

port = '/dev/tty.usbserial-DN02P7CS'
# port = '/dev/cu.usbserial-DN02MPFQ'
baud = 115200
kb = PyKeyboard()


# packet of key presses
# i=0 is the key modifier
# the remaining 6 are the pressed keys
num_members = 7
member_size = 8 # 8 bits
len_packet = member_size * num_members



# setup serial connection
ser = serial.Serial(
	port=port,
	baudrate=baud,
	bytesize = serial.EIGHTBITS,
	parity = serial.PARITY_NONE,
	stopbits = serial.STOPBITS_ONE_POINT_FIVE,
	xonxoff = False,
	rtscts = False,
	dsrdtr = False,
	timeout = 2,
)


key_map = {
	4:'a',
	5:'b',
	6:'c',
	7:'d',
	8:'e',
	9:'f',
	10:'g',
	11:'h',
	12:'i',
	13:'j',
	14:'k',
	15:'l',
	16:'m',
	17:'n',
	18:'o',
	19:'p',
	20:'q',
	21:'r',
	22:'s',
	23:'t',
	24:'u',
	25:'v',
	26:'w',
	27:'x',
	28:'y',
	29:'z',
	30:'1',
	31:'2',
	32:'3',
	33:'4',
	34:'5',
	35:'6',
	36:'7',
	37:'8',
	38:'9',
	39:'0',
	40:'return',
	41:'escape',
	42:'delete',
	43:'tab',
	44:'space',
	45:'-',
	46:'=',
	47:'',
	48:'[',
	49:']',
	50:'\\',
	51:';',
	52:"'",
	53:'`',
	54:',',
	55:'.',
	56:'/',
	57:'capslock',
	58:'KEYTYPE_BRIGHTNESS_DOWN',
	59:'KEYTYPE_BRIGHTNESS_UP',
	60:'KEYTYPE_LAUNCH_PANEL',
	61:'KEYTYPE_LAUNCH_PANEL',
	62:'KEYTYPE_CONTRAST_DOWN',
	63:'KEYTYPE_CONTRAST_UP',
	64:'KEYTYPE_PREVIOUS',
	65:'KEYTYPE_PLAY',
	66:'KEYTYPE_NEXT',
	67:'KEYTYPE_MUTE',
	68:'KEYTYPE_SOUND_DOWN',
	69:'KEYTYPE_SOUND_UP'
}

modifier_map = {
	1:'control',
	2:'shift',
	4:'option',
	8:'command',
	16:'rightcontrol',
	32:'rightshift',
	64:'rightoption',
	128:'function',
}

# arrow keys "UP_ARROW_KEY", "DOWN_ARROW_KEY"








def main():

	def reset():
		# Toggle DTR to reset ESP32
		ser.setDTR(False)
		time.sleep(1)
		ser.flushInput()
		ser.setDTR(True)


	def unpack(s):
		'''
		unpack byte string into it components made up of
		[0-7]: modifier key
		[8-55]: 6 pressed keys
		'''

		if len(s) == len_packet:
			modifier = int(s[:member_size], 2)
			keys = []
			for i,k in enumerate(xrange(member_size,len_packet, member_size)):
				binary = s[k:k+member_size]
				keys.append(int(binary, 2))
				# keys.append(binary)
			return (modifier, keys)
		else:
			return (None, None)

	def get_millis():
		return int(round(time.time() * 1000))


	# ----------------------------------------- #
	#                  START                    #
	# ----------------------------------------- #
	print 'Reseting ESP32'
	reset()

	prev_combo = None # previously pressed keys

	# time in miliseconds to wait before considering held key another press
	# this is only applicable when being_held = True
	time_between_press = 100

	# boolean is true when key combo has been held for
	# more than being_held_thershold miliseconds
	being_held = False
	being_held_thershold = 1000

	if ser.isOpen():
		print 'Serial is open'
		last_press_time = get_millis()
		time_since_last_press = 0
		while True:

			modifier, keys = unpack(ser.readline().strip())
			if modifier != None: # modifier will be None until ESP32 has reset

				# convert from int to key press code
				# keys will be an empty array if no key
				# are being pressed
				# mod will be None unless a modifier key
				# is being pressed
				keys = [key_map.get(l, None) for l in keys if l]
				mod = modifier_map.get(modifier, None)
				combo = [mod] + keys if mod else keys

				something_pressed = True if len(keys) else False
				press_change = combo != prev_combo

				if (something_pressed and press_change) or (time_since_last_press >= time_between_press and being_held):
					kb.press_keys(combo)
					# print combo
					prev_combo = combo
					last_press_time = get_millis()

				time_since_last_press = get_millis() - last_press_time;

				# reset previous keys on key release
				if not something_pressed:
					prev_combo = None
					being_held = False

				# check whether key is being pressed and held
				if something_pressed and time_since_last_press >= being_held_thershold:
					being_held = True

	else:
		print 'Serial not open'
		reset()


if __name__ == "__main__":
    main()
