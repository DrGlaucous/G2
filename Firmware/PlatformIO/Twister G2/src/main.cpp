#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <EEPROM.h>
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
bool InSettingsMode{};
unsigned char BlowerSpeed = PACK_DEFAULT_BLOWER_VALUE;
unsigned char AgitatorSpeed = PACK_DEFAULT_AGITATOR_VALUE;


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

//saves user values to EEPROM
void EEPROMSave(unsigned char BlowerS, unsigned char AgiS)
{
  EEPROM.update(0, BlowerS);
  EEPROM.update(1, AgiS);
  EEPROM.update(2, 0x50);
}

//writes hard-coded defaults to the EEPROM
void EEPROMFactoryReset(void)
{
  //will use adress 0 and 1 for storing PPack values
  EEPROM.update(0, PACK_DEFAULT_BLOWER_VALUE);
  EEPROM.update(1, PACK_DEFAULT_AGITATOR_VALUE);
  EEPROM.update(2, 0x50);//redundancy check: if this address is anything but 0x50, the board knows to put factory defaults in here
}

//pulls the relavent values from the controller's EEPROM
bool EEPROMRead(unsigned char* BlowerS, unsigned char* AgiS)
{
  *BlowerS = EEPROM.read(0);//blower speed
  *AgiS = EEPROM.read(1);//agitator speed
  //the validation byte used to determine if the EEPROM has been stored to before
  if(EEPROM.read(2) == 0x50)
  {
    return true;
  }
  else
  {
    return false;
  }

}

//pusher sub-function
void HandlePusher(bool Fire, int RatePot, int ModePot)
{
  if( Fire == 0 || ModePot > 512)//will not fire in settings mode
  {
    analogWrite(PUSHER_PIN, 0);
  }
  else
  {
    analogWrite(PUSHER_PIN, 
    map(RatePot, POT_MIN, POT_MAX, 0, 255)
    );



  }
}

//handles the control of the SuperCell
void HandlePack(bool Fire, int RatePot, int ModePot, int PowerPot)
{


  if(ModePot > 512)
  {
    InSettingsMode = true;
    //from 0x00 to 0xff, the pack will handle the re-mapping to acceptable blower values
    BlowerSpeed = map(PowerPot, POT_MIN, POT_MAX, 0, 255);

    //ditto with the agitator, but this one is usually left at 255 (full speed)
    AgitatorSpeed = map(RatePot, POT_MIN, POT_MAX, 0, 255);
  }
  else if(InSettingsMode == true && digitalRead(FIRE_BUTTON) == ON_STATE)//only true for the first tick that the ModePot is switched back
  {
    InSettingsMode = false;
    EEPROMSave(BlowerSpeed, AgitatorSpeed);//save pack values to EEPROM

  }
  else
  {
    InSettingsMode = false;
  }

  digitalWrite(13, Fire);

  Wire.beginTransmission(PACK_ADDRESS);

  Wire.write(Fire);
  Wire.write(BlowerSpeed);
  Wire.write(AgitatorSpeed);

  Wire.endTransmission();


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


  //builtin LED for debugging
  pinMode(13, OUTPUT);

  //wire is used for both of these conditions
#if defined(I2C) || defined(DEBUG_MODE)
  Wire.begin();
#endif


  //if restoring settings from EEPROM show that it has not been written to or
  //if the fire button is held down on power up, this will restore EEPROM pack settings back to default ones in the code
  if(!EEPROMRead(&BlowerSpeed, &AgitatorSpeed) || digitalRead(FIRE_BUTTON) == ON_STATE)
  {
    EEPROMFactoryReset();

    //little visual verification that the EEPROM has been factory reset when power-cycled
    digitalWrite(13, HIGH);
  }



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

    DriveAllFlywheels(XJoystickRead, YJoystickRead, (digitalRead(REV_BUTTON) == ON_STATE  &&   analogRead(MODE_POT) <= 512   )? 1 : 0   , &LeftServo, &RightServo, &BottomServo);


  ServoLeft.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, LeftServo));

  ServoRight.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, RightServo));

  ServoBottom.write(
    map(analogRead(POWER_POT), POT_MIN, POT_MAX,
    ESC_MIN, BottomServo));



  HandlePusher((digitalRead(FIRE_BUTTON) == ON_STATE && digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0, analogRead(RATE_POT), analogRead(MODE_POT));


#ifdef I2C
  //every 20th of a second to conserve cycles
  if(MillisecondTicks % 50 == 0 &&
    MillisecondTicks != LastMillisecondTicks)
    HandlePack((digitalRead(REV_BUTTON) == ON_STATE)? 1 : 0, analogRead(RATE_POT), analogRead(MODE_POT), analogRead(POWER_POT)   );
#endif



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


  
}