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

const uint8_t sensor1[8] = {
  0x10, 0x6D, 0x3F, 0xB5, 0x2, 0x8, 0x0, 0xB0
};
const uint8_t sensor2[8] = {
  0x10, 0xAB, 0x3F, 0xB5, 0x2, 0x8, 0x0, 0x9D
};
unsigned long firstAlarmCold = 0,
              firstAlarmHot = 0;

char **remoteNumbers = new char*[5]();
int numbersArrayIndex = 0;
boolean isSentCold = false,
        isSentHot = false;

void setup(void) {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);

  Serial.println(freeMemory());

  //add alarm receipiant numbers and set the initial state of whats the last index of the array
  char number1[] = {
    '+', '4', '9', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0', '0', '0', '0', '0', '0'
  };
  char number2[] = {
    '+', '4', '9', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0', '0', '0', '0', '0', '0'
  };

  Serial.println(number1);
  Serial.println(number2);
  remoteNumbers[0] = number1;
  remoteNumbers[1] = number2;
  numbersArrayIndex = 2;

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
  t.every(5000, doUpdateSensors, (void*)0);
  t.every(10000, receiveSms, (void*)0);

  doUpdateSensors((void*)0);
  Serial.println(freeMemory());
}

void loop(void) {
  unsigned long currentMillis = millis();

  //Device Working LED on
  digitalWrite(11, HIGH);

  if (currentMillis - previousMillis > 1000)
  {
    previousMillis = currentMillis;
  }

  t.update();

  //check if cold
  if ((temp1 < 2.0) | (temp2 < 2.0))
  {
    Serial.println("Alarm cold!");
    unsigned long timeDiffCold = millis() - firstAlarmCold;

    Serial.print(timeDiffCold / 1000);

    //Alarm LED on
    digitalWrite(12, HIGH);

    if (timeDiffCold > SETALARMBACKTIME)
    {
      firstAlarmCold = 0;
      isSentCold = false;
    }

    if (firstAlarmCold == 0)
    {
      firstAlarmCold = millis();
    }
    else
    {
      if (timeDiffCold > ALARMTIMEDELAY)
      {
        if (!isSentCold)
        {
          Serial.println("Sending cold message");
          String msg1 = String("Kaelte Alarm! Aktuelle Temperatur betraegt: Sensor 1 ");
          String msg = String(msg1 + (int)temp1  + " Grad, Sensor 2 " + (int)temp2 + " Grad");
          Serial.println(msg);

          for (int n = 0; numbersArrayIndex - 1; n++)
          {
            sendSMS(remoteNumbers[n], msg);
          }
          isSentCold = true;
        }
      }
    }

    //Alarm Led off
    digitalWrite(12, LOW);
  }

  //check if hot
  if ((temp1 > 60.0) | (temp2 > 60.0))
  {
    Serial.print("Alarm hot!");
    unsigned long timeDiffHot = millis() - firstAlarmHot;

    Serial.print(timeDiffHot / 1000);

    //Alarm Led on
    digitalWrite(12, HIGH);

    if (timeDiffHot > SETALARMBACKTIME)
    {
      firstAlarmHot = 0;
      isSentHot = false;
    }

    if (firstAlarmHot == 0)
    {
      firstAlarmHot = millis();
    }
    else
    {
      if (timeDiffHot > ALARMTIMEDELAY)
      {
        if (!isSentHot)
        {
          Serial.print("Sending cold message");
          String msg1 = String("Hitze Alarm! Aktuelle Temperatur betraegt: Sensor 1 ");
          String msg =  String(msg1 + (int)temp1 + " Grad, Sensor 2 " + (int)temp2 + " Grad");
          Serial.println(msg);

          for (int n = 0; numbersArrayIndex - 1; n++)
          {
            sendSMS(remoteNumbers[n], msg);
          }
          isSentHot = true;

        }
      }
    }

    //Alarm Led off
    digitalWrite(12, LOW);
  }
  //Device Working LED off
  digitalWrite(11, LOW);
}

float readDataFromSensor(const uint8_t addr[8])
{
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  float celsius;

  /**
     Serial.println();
     Serial.print("Attempting read from ROM =");
     for( i = 0; i < 8; i++) {
     Serial.write(' ');
     Serial.print(addr[i], HEX);
     }

     if (OneWire::crc8(addr, 7) != addr[7])
     {
     Serial.println("CRC is not valid!");
     return;
     }
   **/
  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end

  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.

  present = ds.reset();
  ds.select(addr);
  ds.write(0xBE);         // Read Scratchpad

  /**
     Serial.print("  Data = ");
     Serial.print(present, HEX);
     Serial.print(" ");
   **/
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  /**Serial.print(" CRC=");
     Serial.print(OneWire::crc8(data, 8), HEX);
     Serial.println();
   **/
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];

  raw = raw << 3; // 9 bit resolution default
  if (data[7] == 0x10) {
    // "count remain" gives full 12 bit resolution
    raw = (raw & 0xFFF0) + 12 - data[6];
  }

  celsius = (float)raw / 16.0;
  Serial.print("  Temperature = ");
  Serial.print(celsius);
  Serial.print(" Celsius, ");
  Serial.print("\n");

  return  celsius;
}

void sendSMS(char remoteNum[20], String txtMsg)
{
  sms.beginSMS(remoteNum);
  sms.print(txtMsg);
  sms.endSMS();
}

void doUpdateSensors(void *context)
{
  Serial.print("Sensor 1: \n");
  temp1 = readDataFromSensor(sensor1);
  Serial.print("Sensor 2: \n");
  temp2 = readDataFromSensor(sensor2);
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

    if (c == 'T') //requesting actual temperature
    {
      Serial.println("Entering T part");
      Serial.println(freeMemory());
      int t1 = (int)temp1;
      int t2 = (int)temp2;
      String temper1 = String(t1);
      String temper2 = String(t2);
      String message1 = String("Temp1 :" + temper1 + " Grad; Temp2: " + temper2 + " Grad");
      Serial.println(temper1);
      Serial.println(temper2);
      Serial.println(message1);
      Serial.println(freeMemory());

      sendSMS(requestNr, message1);

      Serial.println("Exiting T part");
    }
    else if (c == 'A') //requesting adding of cell phone number to alarm receipiants list
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




