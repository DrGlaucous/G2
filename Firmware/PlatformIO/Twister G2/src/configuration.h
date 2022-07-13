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









