// pegelstand.ino

/*
	Pegelspannung(Pumpe) -> 0.5 Spannungsteiler -> A4 (Pin)
	VCC(5V[Arduino]) -> Schwimmschalter -> Widerstand -> A3 (Pin 17)
	max leserate vom ADC ist: 10kHz
*/

const int PIN_PUMPE_PEGEL = 17;
int pegelStand;


void setup() {
	Serial.begin(9600);
}

void loop() {
	pegelStand = analogRead(PIN_PUMPE_PEGEL);
	//TODO: Normalisierung der Werte + Mapping auf Entfernungswerte via defines

	
}