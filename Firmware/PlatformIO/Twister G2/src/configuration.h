//configuration options for the twister G2
#pragma once

//the state of a pin when it is "high" or "clicked"
#define ON_STATE 1
#define USE_PULLUPS//tell the board to use its onboard pullup resistors

//pins for the inputs
#define REV_BUTTON 7
#define FIRE_BUTTON 4


#define X_JOYSTICK_POT 3
#define Y_JOYSTICK_POT 6


//both joysticks like to sit around 615 in value even though their range extends the full 0-1023 range
//center for them should be around 512, this is about 100 off in value, so I decided to go off the center range + radius
//instead of the min/max range

#define JOYSTICK_CENTER_X 615
#define JOYSTICK_CENTER_Y 615

//should be the value of the smallest differece between center and one of the walls (in my case, 1023)
#define JOYSTICK_X_RAD 410
#define JOYSTICK_Y_RAD 410

//no need to touch these: they're automatically configured by the prcompiler using the values above
#define JOYSTICK_X_MIN (JOYSTICK_CENTER_X - JOYSTICK_X_RAD)
#define JOYSTICK_X_MAX (JOYSTICK_CENTER_X + JOYSTICK_X_RAD)
#define JOYSTICK_Y_MIN (JOYSTICK_CENTER_Y - JOYSTICK_Y_RAD)
#define JOYSTICK_Y_MAX (JOYSTICK_CENTER_Y + JOYSTICK_Y_RAD)



#define POWER_POT 2
#define RATE_POT 1
#define MODE_POT 0

#define POT_MIN 0
#define POT_MAX 1023//overshooting this value will decrease the overall max FPS the blaster will shoot at

//pins for the outputs

#define ESC_A 9//MUST be PWM
#define ESC_B 6
#define ESC_C 5
//min and max values for the servos
#define ESC_MIN 37
#define ESC_MAX 180
/*

VIEW FROM FRONT

A      B
\     /
 \   /
  \ /
   |
   |
   C


*/

//controls the speed at which the ESCs slow down, larger numbers = slower rev down time
#define REV_DOWN_RATE 200


//for the push wheel that regulates ball flow
#define PUSHER_PIN 3//MUST be PWM

//tells the firmware if the blaster is being used with the arm sling or with the handle (arm sling is upside down)
#define UPSIDE_DOWN

#define I2C//use I2C to communicate with the superCell
#define PACK_ADDRESS 2//address of the pack on the i2c line
#define I2C_TIMEOUT 5000//microseconds until the blaster gives up communicating with the pack for that cycle
//(good if you want to switch between running the blaster with and without the pack, commenting this out will make it unusable without an i2c peripheral)

//enables several different serial and i2c debug messages
//(i2c is used when the blaster is assembled, and it is not possible to get to the USB port)
//#define DEBUG_MODE
#define DEBUG_I2C_ADDRESS 8

//between 0 and 255, if the fire (top) button is held down on power cycle, these values are stored to the EEPROM (or if nothing is there in the first place)
#define PACK_DEFAULT_BLOWER_VALUE 150
#define PACK_DEFAULT_AGITATOR_VALUE 255


//little explanation on how the blaster's options should work
//If the SETTINGS pot is in the forward position, it puts the blaster into "pack settings mode"
//in this mode, rev and fire do not do anything on the blaster itself
//revving ONLY revs the supercell (so the user can set the blower correctly without launching balls everywhere)
//the POWER pot (originally for flywheels) sets the blower speed, and the PUSH_SPEED pot sets the agitator speed (originally for the pusher wheel)
//when the user is done, moving the SETTINGS pot back will keep their settings on the pack and return the blaster to normal operation, though with the pack using the values they just set
//with a power cycle, all settings are lost unless the user holds down the FIRE_BUTTON when exiting settings mode, in which the blaster will store the values to EEPROM
//When turning on the blaster, EEPROM settings will be loaded back into memory
//if the blaster was turned on for the first time, hard-coded defaults will be loaded and stored to the EEPROM
//if the user holds down the FIRE_BUTTON when turning the blaster on, hard-coded defaults will be loaded and stored to the EEPROM





