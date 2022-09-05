#pragma once


void StatusLED(int MilTime = 0, bool IsOn = false);
extern unsigned long MillisecondTicks;
extern unsigned long MicrosecondTicks;
extern unsigned long LastMillisecondTicks;
extern unsigned long LastMicrosecondTicks;