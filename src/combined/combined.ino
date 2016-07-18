#include <inttypes.h>

#include "Defines.h"
#include "NewPing.h"



// Schacht - Ultraschall
#if SCHACHT_ULTRASONIC_ON
NewPing SchachtUltraSonicSensor(SCHACHT_ULTRASONIC_PIN_TRIGGER, SCHACHT_ULTRASONIC_PIN_ECHO, SCHACHT_ULTRASONIC_MAX_DISTANCE);
int schachtUltrasonicSensorDistance;
#endif

// Schacht - Schwimmer
#if SCHACHT_SCHWIMMER_ON
const int PIN_SCHWIMMER = 14;
int schwimmerStatus;
#endif

// Schacht - Pumpe
#if SCHACHT_PUMPE_ON
const int PIN_PUMPE_PEGEL = 17;
int pegelStand;
#endif

// Sack - Ultraschall
#if SACK_ULTRASONIC_ON
NewPing SackUltraSonicSensor(SACK_ULTRASONIC_PIN_TRIGGER, SACK_ULTRASONIC_PIN_ECHO, SACK_ULTRASONIC_MAX_DISTANCE);
int sackUltrasonicSensorDistance;
#endif


void setup()
{

  // Serial Communication
  #if COMMUNICATION_SERIAL_ON
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  #endif

  // GSM Communication
  #if COMMUNICATION_GSM_ON
  //TODO
  #endif
}



void loop()
{
  
  // Strings
  char eof = "/";
  String tab = "\t";
  String serialString = "";

  // Schacht - Ultraschall
  #if SCHACHT_ULTRASONIC_ON
  schachtUltrasonicSensorDistance = SchachtUltraSonicSensor.ping_cm();
  serialString = "[Schacht - Ultraschall]: "
  Serial.print(serialString + schachtUltrasonicSensorDistance);
  Serial.println();
  //TODO WARNING DETECTION
  //TODO ALERT DETECTION
  #endif

  
  // Schacht - Schwimmer
  #if SCHACHT_SCHWIMMER_ON
      pinMode(SCHACHT_SCHWIMMER_PIN, INPUT);
    
    if(digitalRead(SCHACHT_SCHWIMMER_PIN) == HIGH) {
      //ALAAARM!!
      serialString = "[ALARM] "
      Serial.println(serialString + LOCATION);
      //Serial.println(eof);
    } else {
      //no ALARM!
      ;
    }
  #endif

  //Serial.println(digitalRead(PIN_SCHWIMMER));

  // Schacht - Pumpe
  #if SCHACHT_PUMPE_ON
  pegelStand = analogRead(SCHACHT_PUMPE_PEGEL_PIN);
  //TODO: Normalisierung der Werte + Mapping auf Entfernungswerte via defines

  //ADC Resolution: 10bit -> 4.9mV/Unit with [0..5V] Input
  #endif


  // Sack - Ultraschall
  #if SACK_ULTRASONIC_ON
  //TODO
  #endif
}
