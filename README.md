# tacocat keyboard

<img src="https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/raw/master/assets/img/tacocat.png" width=150/>



:construction: this repo is currently on *hold*; once I started writing the firmware, I realized the choice of microcontroller was not a good one. For a keyboard, it's super useful to have a USB to serial chip that acts like a [HID](https://en.wikipedia.org/wiki/Human_interface_device), that's why the Teensy or the ATmega32U4 are popular options. That being said, I started developing the firmware in such a way that required a python script to be running on the host to listen to the keystrokes being sent. :sweat: Furthermore, I'm not convinced that the physical layout is that great. Anywho, I'm leaving this here for reference. :construction:

<br>
<br>

Another split ergonomic keyboard with features:
- two symmetric keyboard halves staggered in opposite directions
- wireless or powered operation
- wifi & bluetooth connected
- can be used as two halves with a TRRS audio cable or as single joined keyboard (no cable needed)
- audio passthrough

Please visit the [wiki](https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/wiki) for full documentation and build logs.

<img src="https://github.com/ConstantinoSchillebeeckx/tacocat-keyboard/raw/master/assets/img/render_full.png" />
