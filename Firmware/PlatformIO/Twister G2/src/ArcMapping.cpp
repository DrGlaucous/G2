//this page handles all the backend calculations that are needed to map the XY joystick to the triwheels
#include <Arduino.h>
#include <Servo.h>
#include "main.h"
#include "configuration.h"
#include "ArcMapping.h"




//X and Y values of the points on the hypothetical triangle, where each point will correspond
//to one of the 3 ESCs, and depending on distance they are from the joystick XY coordinates, will vary in speed

#define C_A float( float(800 / sqrtf(3)) / 2)   //x value of the top 2 points
#define C_B float(400 / 3 )
#define C_C float(-800 / 3)
#define SLOPE_CONSTANT float( (C_C - C_B) /  (0.0 - C_A) )
#define SIDE_LENGTH float(800 / sqrtf(3))

/*
         y
(-A,B)   |       (A,B)
_________|_________
\        |        /
 \       |       /
  \  view from  /
___\__ front __/___________x
    \    |    /
     \   |   /
      \  |  /
       \ | /
        \|/
        (0,C)

*/

//so I can map with floats
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}




void MapJoystick(int JoystickX, int JoystickY, float* X, float* Y)
{
    //handling the upsideDown inversions outside this function in main
    //currently written in rightside up notation
    *Y = mapfloat(JoystickY, JOYSTICK_MAX, JOYSTICK_MIN, C_C, C_B);

    *X = mapfloat(JoystickX, JOYSTICK_MAX, JOYSTICK_MIN, (-1.0 * C_A), C_A);

    //this limits X values bsed on where vertically on the triange it is (because the triangle gets smaller the further down you go)
    float XSlopeLimit = (  (*Y - C_C) / SLOPE_CONSTANT);

    //if X exceeds this, it will be snapped to it.
    if(*X < (XSlopeLimit * -1.0))
      *X = (XSlopeLimit * -1.0);
    else if (*X > XSlopeLimit)
      *X = XSlopeLimit;

}

void FindServoValue(float X, float Y, float *LeftServo, float* RightServo, float* BottomServo)
{
  //distance finding formula and mapping formula
  *BottomServo = mapfloat(
    sqrtf(   pow(0 - X, 2) + pow(C_C - Y, 2)   ),//(0, C)
    0, SIDE_LENGTH,
    ESC_MIN, ESC_MAX
    );

  *LeftServo = mapfloat(
    sqrtf(   pow( (-1.0 * C_A) - X, 2) + pow(C_B - Y, 2)   ),//(-A, B)
    0, SIDE_LENGTH,
    ESC_MIN, ESC_MAX
    );

  *RightServo = mapfloat(
    sqrtf(   pow(C_A - X, 2) + pow(C_B - Y, 2)   ),//(A, B)
    0, SIDE_LENGTH,
    ESC_MIN, ESC_MAX
    );

}


//the functions that will be exposed to the main program, drives servos directly with X and Y coordinates
void DriveAllFlywheels (int JoystickX, int JoystickY, bool Fire, float* LeftServo, float* RightServo, float* BottomServo)
{
    

    static int SlowCounter;

    //we need the second set of floats so we can rev down at a linear rate for each flywheel
    static float RawLeftServo{};
    static float RawRightServo{};
    static float RawBottomServo{};

    if(Fire)
    {
      float X{};
      float Y{};

      MapJoystick(JoystickX, JoystickY, &X, &Y);
      FindServoValue(X, Y, &RawLeftServo, &RawRightServo, &RawBottomServo);
      *LeftServo = RawLeftServo;
      *RightServo = RawRightServo;
      *BottomServo = RawBottomServo;
      SlowCounter = REV_DOWN_RATE;

    }
    else
    {
      
      *LeftServo = mapfloat(SlowCounter, 0, REV_DOWN_RATE, ESC_MIN, RawLeftServo);
      *RightServo = mapfloat(SlowCounter, 0, REV_DOWN_RATE, ESC_MIN, RawRightServo);
      *BottomServo = mapfloat(SlowCounter, 0, REV_DOWN_RATE, ESC_MIN, RawBottomServo);

      if(SlowCounter > 0 && MillisecondTicks != LastMillisecondTicks)
      {
        SlowCounter -= 1;
      }


    }




    

}





