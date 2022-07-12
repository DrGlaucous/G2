#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "main.h"
#include "configuration.h"
#include "ArcMapping.h"


unsigned long MillisecondTicks{};
unsigned long MicrosecondTicks{};
unsigned long LastMillisecondTicks{};//previous values
unsigned long LastMicrosecondTicks{};

//servo values
float LeftServo{};
float RightServo{};
float BottomServo{};

//System clock
void GetTicks(void)
{
	LastMillisecondTicks = MillisecondTicks;
	LastMicrosecondTicks = MicrosecondTicks;

	MillisecondTicks = millis();
	MicrosecondTicks = micros();
}


Servo ServoLeft;
Servo ServoRight;
Servo ServoBottom;



void handlePusher(bool Fire, int RatePot, int ModePot)
{
  if(Fire)
  {
    analogWrite(PUSHER_PIN, 
    map(RatePot, POT_MIN, POT_MAX, 0, 255)
    );

  }
  else
  {
    analogWrite(PUSHER_PIN, 0);
  }
}





void setup() {
  // put your setup code here, to run once:

  ServoLeft.attach(ESC_A);
  ServoRight.attach(ESC_B);
  ServoBottom.attach(ESC_C);

#ifdef USE_PULLUPS
    pinMode(FIRE_BUTTON, INPUT_PULLUP);
    pinMode(REV_BUTTON, INPUT_PULLUP);
#else
    pinMode(FIRE_BUTTON, INPUT);
    pinMode(REV_BUTTON, INPUT);
#endif

  pinMode(RATE_POT, INPUT);
  pinMode(MODE_POT, INPUT);
  pinMode(POWER_POT, INPUT);

  pinMode(PUSHER_PIN, OUTPUT);



  //debug (and potential other things)
  Wire.begin();

}

void loop() {
  // put your main code here, to run repeatedly:

  GetTicks();

#ifdef UPSIDE_DOWN
  //reverse map the pots because everything is flipped
  DriveAllFlywheels(
    map(analogRead(X_JOYSTICK_POT), JOYSTICK_MIN, JOYSTICK_MAX, JOYSTICK_MAX, JOYSTICK_MIN),
    map(analogRead(Y_JOYSTICK_POT), JOYSTICK_MIN, JOYSTICK_MAX, JOYSTICK_MAX, JOYSTICK_MIN), 
    (digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0   , &LeftServo, &RightServo, &BottomServo);

#else
  DriveAllFlywheels(analogRead(X_JOYSTICK_POT), analogRead(Y_JOYSTICK_POT), (digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0   , &LeftServo, &RightServo, &BottomServo);
#endif


  ServoLeft.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, LeftServo));

  ServoRight.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, RightServo));

  ServoBottom.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, BottomServo));


  Wire.beginTransmission(2);
  Wire.write(char(LeftServo));
  Wire.write(char(RightServo));
  Wire.write(char(BottomServo));
  Wire.endTransmission();




  handlePusher((digitalRead(FIRE_BUTTON) == ON_STATE && digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0, analogRead(RATE_POT), analogRead(MODE_POT));

  
}