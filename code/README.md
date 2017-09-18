hardware.update() [HardwareController]:
- scans all the physical buttons checking for whether any are pressed down; returns an array, with each index representing a button state (either pressed or not).

debouncer.update() [ButtonDebouncer]:
- debounce the raw button states by requiring a certain number of samples (of the on state) for any given key.

push_detector.update() [ButtonDeltaDetector]:
- translates individual key presses into key press deltas (changes in key press state: 1 for key pressed, -1 for key released)

key_mapper.resolve() [KeyMapper]:
- translate the array of pressed key indexes into an array of pressed key button codes (see keylayouts.h)

slave.update() [SlaveNotifier]:
- if the keyboard acts as a slave, this will just generate an empty key array
- if the keyboard is a master (and has a slave), this will query the connected slave keyboard for its key array using i2c_communicator.request_packet()

master.notify() [MasterNotifier]:
- if acting as the master connected to host PC, this will call usb_communicator.update();
	- usb_communicator.update() will iterate over the button codes currently pressed and put together a tidy package of the the keys (on the left & right keyboard) currently pressed. It allows a modifier key as well as 6 other keys pressed concurrently.
	- package of pressed keys is then "sent" (printed to serial for now) to the host computer.
- if acting as the slave connected to master through I2C, this will call i2c_communicator.update():
	-
