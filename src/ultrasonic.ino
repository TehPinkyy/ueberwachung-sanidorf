#include <NewPing.h>
#include <Defines.h>

NewPing UltraSonicSensor(ULTRASONIC_PIN_TRIGGER, ULTRASONIC_PIN_ECHO, ULTRASONIC_MAX_DISTANCE);

void setup()
{
	Serial.begin(9600);
}

int distance;

void loop()
{
	distance = UltraSonicSensor.ping_cm();
	Serial.print(distance);
	Serial.println();
}
