
# Github repository for the G2 Twister
![image](https://raw.githubusercontent.com/DrGlaucous/G2/main/Images/Twister.PNG)

This is an arm or traditional handle mounted nerf rival blaster, capable of hooking up to the [G3](https://github.com/DrGlaucous/G3) or taking rival stick magazines.

It contains a set of 3 brushless flywheels, designed to be able to put spin on the projectiles and arc them in any direction at the discretion of the user.
___
# Firmware
The firmware for this blaster is designed to be widely configurable and uses platformIO as the development platform (though the arduino IDE could be used in a pinch with few modifications). All settings are handled and explained in the "configuration.h" header file.


### little explanation on how the blaster's options should work

This blaster is heavily designed to operate with the [G3](https://github.com/DrGlaucous/G3), an open-source rival ball hopper that can be worn or carried with the user and expands the blaster's capacity from roughly 20 rounds to about 1500.
Being as such, it is capable of plugging into the G3 and controlling it with i2C commands with the knobs on the Twister itself.
**Here is how to set the blower and agitator speed on the pack with the Blaster's on-board controls.**


* If the SETTINGS knob is in the forward position, it puts the blaster into *"pack settings mode"*

* In this mode, pressing the buttons on the handle will not control anything on the blaster itself; revving ONLY revs the supercell *(so the user can set the blower correctly without launching balls everywhere)*.

* The POWER knob (originally for flywheels) sets the blower speed, and the RATE knob sets the agitator speed (originally for the pusher wheel).

* When the user is done, moving the SETTINGS knob back will keep their settings on the pack and return the blaster to normal operation, though with the pack using the values they just set.

* With a power cycle, all setting changes are lost unless the user holds down the FIRE BUTTON when exiting SETTINGS mode, in which the blaster will store the values to EEPROM.

* When turning on the blaster, EEPROM settings will be loaded back into memory. If the blaster was turned on for the first time, hard-coded defaults will be loaded and stored to the EEPROM.

* If the user holds down the FIRE BUTTON when turning the blaster on, hard-coded defaults will be loaded and stored to the EEPROM
