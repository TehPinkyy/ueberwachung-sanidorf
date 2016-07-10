//#include <GSM.h>
#include <GSM3ShieldV1AccessProvider.h>
#include <GSM3SMSService.h>
#include <stdlib.h>
#include "OneWire.h"
#include "Timer.h"
#include "MemoryFree.h"

//ROM adresses
//Sensor 1:  10 6D 3F B5 2 8 0 B0
//Sensor 2:  10 AB 3F B5 2 8 0 9D
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library

#define PINNUMBER ""
#define ALARMTIMEDELAY 60000
#define SETALARMBACKTIME 86400000
#define GSM_SMS GSM3SMSService
#define GSM GSM3ShieldV1AccessProvider

OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)

GSM gsmAccess(true);
GSM_SMS sms;

Timer t;
float temp1, temp2;
unsigned long previousMillis = 0;
unsigned long alarmMillis = 0;

char **remoteNumbers = new char*[5]();
int numbersArrayIndex = 0;
boolean isSentCold = false,
        isSentHot = false;

void setup(void) {
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(8, INPUT);


  Serial.println(freeMemory());

  //add alarm receipiant numbers and set the initial state of whats the last index of the array
  char number1[] = {
    '+', '4', '9', '1', '7', '5', '8', '0', '1', '7', '6', '5', '5', '\0', '0', '0', '0', '0', '0', '0'
  };

  Serial.println(number1);
  remoteNumbers[0] = number1;
  numbersArrayIndex = 1;

  Serial.println(freeMemory());
  Serial.println("Initing GSM");
  boolean notConnected = true;

  //connect GSM Shield with network loop until connection is established
  while (notConnected)
  {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else
    {
      Serial.println("Not connected");
      delay(1000);
    }
  }

  Serial.println("GSM initialized");

  //init interrupts to read sensors every 5 and check received sms every 10 secs
  t.every(10000, receiveSms, (void*)0);

  Serial.println(freeMemory());
}

void loop(void) {
  unsigned long currentMillis = millis();

  Serial.println("Loop act");
  //Device Working LED on
  digitalWrite(11, HIGH);

  if (currentMillis - previousMillis > 1000)
  {
    previousMillis = currentMillis;
  }

  //Pegelalarm ausgelöst
  if (digitalRead(8) && (currentMillis - alarmMillis) > 30000) {
    alarmMillis = millis();
    String message = "Pegelalarm an Ost ausgelöst!";
    sendSMS(remoteNumbers[0], message);
  }

  if (!digitalRead(8)) {
    alarmMillis = millis();
  }

  t.update();

  //Device Working LED off
  digitalWrite(11, LOW);

  delay (1000);
}

void sendSMS(char remoteNum[20], String txtMsg)
{
  Serial.println("Sending msg");
  Serial.println(txtMsg);
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
  Serial.println("Sending succeeded");
}

void receiveSms(void *context)
{
  Serial.println("Checking Sms");

  if (sms.available())
  {
    char requestNr[20];
    char c;

    Serial.println("Sms available!");

    sms.remoteNumber(requestNr, 20);

    c = sms.read();

    Serial.println(c);

    if (c == 'A') //requesting adding of cell phone number to alarm receipiants list
    {
      Serial.println("Entering A part");
      addNumber(requestNr);
      sendSMS(remoteNumbers[numbersArrayIndex - 1], "Deine Nummer wurde erfolgreich zur Alarmliste hinzugefuegt");
      Serial.println("Exiting A part");
    }

    sms.flush();
  }
}

void addNumber(char number[20])
{
  remoteNumbers[numbersArrayIndex] = number;
  numbersArrayIndex++;
}




