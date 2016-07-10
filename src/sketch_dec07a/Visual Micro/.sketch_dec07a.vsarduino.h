/* 
	Editor: http://www.visualmicro.com
	        arduino debugger, visual micro +, free forum and wiki
	
	Hardware: Arduino Uno, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

void setup(void);
void loop(void);
float readDataFromSensor(const uint8_t addr[8]);
void sendSMS(char remoteNum[20], String txtMsg);
void doUpdateSensors(void *context);
void receiveSms(void *context);
void toggleLED(int ledNumber);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\cores\arduino\arduino.h"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\sketch_dec07a.ino"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\Event.cpp"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\Event.h"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\OneWire.cpp"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\OneWire.h"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\Timer.cpp"
#include "K:\mein ordner\arduino\temp\sketch_dec07a\Timer.h"
