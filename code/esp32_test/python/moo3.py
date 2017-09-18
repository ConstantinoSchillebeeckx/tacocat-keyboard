#!/usr/bin/env python
# https://folk.uio.no/jeanra/Microelectronics/TransmitStructArduinoPython.html

#=====================
# THIS IS OLD use moo2
#=====================

import numpy as np
import time, sys, struct
from pykeyboard import PyKeyboard

import serial

port = '/dev/tty.usbserial-DN02P7CS'
# port = '/dev/cu.usbserial-DN02MPFQ'
baud = 115200
kb = PyKeyboard()

# packet of key presses
# i=0 is the key modifier
# the remaining 6 are the pressed keys
packet = [0] * 7

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


count = 0
# time.sleep(2) # to let things properly connect ...

if ser.isOpen():
	print 'Serial is open'
	# time.sleep(1)
	while True:

		# wait for start of package
		if ser.read(1) == 'S':
			data = ser.read(36) # TODO confirm byte length
			if ser.read(1) == 'E': # wait for end of package to process data
				dat = struct.unpack('<fffffffff', data)
				print dat
else:
	print 'No serial'
