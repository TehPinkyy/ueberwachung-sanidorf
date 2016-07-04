// schwimmschalter.ino

/*
	VCC(5V[Arduino]) -> Schwimmschalter -> Widerstand -> A0 (Pin 14)
	max leserate vom ADC ist: 10kHz
*/

const int PIN_SCHWIMMER = 14;
int schwimmerStatus;


void setup() {
	Serial.begin(9600);
}

void loop() {
	pinMode(PIN_SCHWIMMER, INPUT);
	
	if(digitalRead(PIN_SCHWIMMER) == HIGH) {

		//ALAAARM!!
	} else {

		//no ALARM!
	}

	Serial.println(digitalRead(PIN_SCHWIMMER));

}