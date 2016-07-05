/* Ultrasonic driver for US-015

Wiring:
VCC => +5V
GND => GND
TRIG => digital pin 12 (Pin 12)
ECHO => digital pin 11 (Pin 11)
*/

const int TRIGGER_PIN = 12;
const int ECHO_PIN = 11;

void setup(){
	Serial.begin(9600);
}

void loop(){
	long = duration, distance;

//short LOW Pulse to guarantee correct HIGH Pulse
	pinMode(TRIGGER_PIN, OUTPUT);
	digitalWrite(TRIGGER_PIN, LOW);
	delayMicroseconds(20);
	digitalWrite(TRIGGER_PIN, HIGH);
	delayMicroseconds(20);
	digitalWrite(TRIGGER_PIN, LOW);

//input
	pinMode(ECHO_PIN, INPUT);
	duration = pulseIn(ECHO_PIN, HIGH);			//[ms]
	distance = durationToDistance(duration);	//[cm]

	Serial.print(distance);
	delayMicroseconds(500);
}

long durationToDistance(long duration){
//sound travels at a velocity of ~340 m/s = ~29ms/cm
	return (duration / (29 * 2);
}