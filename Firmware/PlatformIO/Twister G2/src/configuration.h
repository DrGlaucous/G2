//configuration options for the twister G2
#pragma once

//the state of a pin when it is "high" or "clicked"
#define ON_STATE 1
#define USE_PULLUPS//tell the board to use its onboard pullup resistors

//pins for the inputs
#define REV_BUTTON 4
#define FIRE_BUTTON 7


#define X_JOYSTICK_POT 3
#define Y_JOYSTICK_POT 6

#define JOYSTICK_MIN 0
#define JOYSTICK_MAX 800


#define POWER_POT 2
#define RATE_POT 1
#define MODE_POT 0

#define POT_MIN 0
#define POT_MAX 1023

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









