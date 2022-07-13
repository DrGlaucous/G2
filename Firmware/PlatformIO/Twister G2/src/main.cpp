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

//i2c pack settings
unsigned char BlowerSpeed{};
unsigned char AgitatorSpeed = 255;


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


void HandlePack(bool Fire, int RatePot, int ModePot, int PowerPot)
{
  if(ModePot > 512)
  {
    //from 0x00 to 0xff, the pack will handle the re-mapping to acceptable blower values
    BlowerSpeed = map(RatePot, POT_MIN, POT_MAX, 0, 255);

    //ditto with the agitator, but this one is usually left at 255 (full speed)
    AgitatorSpeed = map(PowerPot, POT_MIN, POT_MAX, 0, 255);
  }

  Wire.beginTransmission(PACK_ADDRESS);

  Wire.write(Fire);
  Wire.write(BlowerSpeed);
  Wire.write(AgitatorSpeed);


}


void setup() {

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

  //wire is used for both of these conditions
#ifdef I2C || DEBUG_MODE
  Wire.begin();
#endif


#ifdef DEBUG_MODE
  Serial.begin(9600);
#endif




}

void loop() {

  GetTicks();



#ifdef UPSIDE_DOWN
  //reverse map the pots because everything is flipped
  unsigned int XJoystickRead = map(analogRead(X_JOYSTICK_POT), JOYSTICK_X_MIN, JOYSTICK_X_MAX, JOYSTICK_X_MAX, JOYSTICK_X_MIN);
  unsigned int YJoystickRead = map(analogRead(Y_JOYSTICK_POT), JOYSTICK_Y_MIN, JOYSTICK_Y_MAX, JOYSTICK_Y_MAX, JOYSTICK_Y_MIN);

#else
  unsigned int XJoystickRead = analogRead(X_JOYSTICK_POT);
  unsigned int YJoystickRead = analogRead(Y_JOYSTICK_POT);

#endif

    DriveAllFlywheels(XJoystickRead, YJoystickRead, (digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0   , &LeftServo, &RightServo, &BottomServo);


  ServoLeft.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, LeftServo));

  ServoRight.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, RightServo));

  ServoBottom.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, BottomServo));




  //write some important values over i2c
#ifdef DEBUG_MODE
  Wire.beginTransmission(2);
  Wire.write(char(LeftServo));
  Wire.write(char(RightServo));
  Wire.write(char(BottomServo));


 	unsigned char X_Pot_Array[2] = {
	analogRead(X_JOYSTICK_POT) & 0xFF,
	(analogRead(X_JOYSTICK_POT) >> 8) & 0xFF,
	};

  for (int i = 0; i < 2; ++i)
  {
    Wire.write(X_Pot_Array[i]);
  }

  unsigned char Y_Pot_Array[2] = {
	analogRead(Y_JOYSTICK_POT) & 0xFF,
	(analogRead(Y_JOYSTICK_POT) >> 8) & 0xFF,
	};

  for (int i = 0; i < 2; ++i)
  {
    Wire.write(Y_Pot_Array[i]);
  }

  Wire.endTransmission();
#endif


  handlePusher((digitalRead(FIRE_BUTTON) == ON_STATE && digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0, analogRead(RATE_POT), analogRead(MODE_POT));

#ifdef I2C
  //every 20th of a second to conserve cycles
  if(MillisecondTicks % 50 == 0 &&
    MillisecondTicks != LastMillisecondTicks)
    HandlePack((digitalRead(FIRE_BUTTON) == ON_STATE && digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0, analogRead(RATE_POT), analogRead(MODE_POT), analogRead(POWER_POT)   );
#endif


  
}