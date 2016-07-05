/* Ultrasonic driver for US-015

Wiring:
VCC => +5V
GND => GND
TRIG/ECHO => digital pin 2 (Pin 2)
*/

const int TRIGGER_PIN = 2;

long duration, distance;

void setup(){
	Serial.begin(9600);
}

void loop(){
  //short LOW Pulse to guarantee correct HIGH Pulse
	pinMode(TRIGGER_PIN, OUTPUT);
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(20);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIGGER_PIN, LOW);

  //input
	pinMode(TRIGGER_PIN, INPUT);
	duration = pulseIn(TRIGGER_PIN, HIGH);			//[ms]
	distance = durationToDistance(duration);	//[cm]

  //output data
	Serial.print(distance);
  Serial.println();
	delay(100);
}

long durationToDistance(long duration){
//sound travels at a velocity of ~340 m/s = ~29ms/cm
	return (duration / (29 * 2));
}
