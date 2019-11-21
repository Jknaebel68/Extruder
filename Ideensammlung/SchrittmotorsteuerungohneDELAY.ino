/*
  Einstellungen am TB6600 für 6400 Schritte pro Umdrehung
  und für den Nema 17 Motor mit 1,0 bis 1,2 Amp.
  Dippschalter 6   5   4   3   2   1
  Stellungen   0n  off on  off off off
  und für den Nema 17 Motor mit 1,5 bis 1,7 Amp.
  Dippschalter 6   5   4   3   2   1
  Stellungen   off on  on  off off off
*/



//------Steuerung für den ersten Schrittmotor------------------
word x1 = 0;
word schritte1 = 19200;
byte richtung1 = HIGH;
byte schrittposition1 = 1;
// Der Microwert für pwzeit & whzeit nicht unter 200 !
unsigned long pwablauf1 = 0;// Pulswechselablaufzeit1
unsigned long whablauf1 = 0;// Wiederhohlablaufzeit1
unsigned long rampenstart1 = 500;// Microsekundenx2 pro Impuls bei Anfahrt
unsigned long rampenende1 = 50;//   Microsekundenx2 pro Impuls bei Voller Fahrt
unsigned long stopbeginn1 = -5000;
unsigned long stopzeit1 = 5000;// Pausezeit von Stopp bis Anfahrt
word rampenrichtung1 = 1;
unsigned long mini1 = rampenstart1;
unsigned long maxi1 = rampenende1;
byte enabl1 = 6;
byte dir1 = 5;
byte puls1 = 4;
//--------------------------------------------------------------
//------Steuerung für den zweiten Schrittmotor------------------
word x2 = 0;
word schritte2 = 19200;
byte richtung2 = HIGH;
byte schrittposition2 = 1;
// Der Microwert für pwzeit & whzeit nicht unter 200 !!
unsigned long pwablauf2 = 0;// Pulswechselablaufzeit2
unsigned long whablauf2 = 0;// Wiederhohlablaufzeit2
unsigned long rampenstart2 = 500;
unsigned long rampenende2 = 50;
unsigned long stopbeginn2 = -5000;
unsigned long stopzeit2 = 5000;
word rampenrichtung2 = 1;
unsigned long mini2 = rampenstart2;
unsigned long maxi2 = rampenende2;
byte enabl2 = 10;
byte dir2 = 9;
byte puls2 = 8;

void setup()
{
	
	Serial.begin(9600);
	//----------Steuerpins für den ersten Treiber aktivieren-------
	pinMode(enabl1, OUTPUT); // Enable1
	pinMode(dir1, OUTPUT); // Step1
	pinMode(puls1, OUTPUT); // Dir1
	digitalWrite(enabl1, HIGH); // Set Enable1 High
	//---------------------------------------------------------------
	//----------Steuerpins für den zweiten Treiber aktivieren--------
	pinMode(enabl2, OUTPUT); // Enable2
	pinMode(dir2, OUTPUT); // Step2
	pinMode(puls2, OUTPUT); // Dir2
	digitalWrite(enabl2, HIGH); // Set Enable2 High
	
}

void loop()
{
	//---------------------------------------------------------------------
	//----------Treiber 1 aktivieren---------------------------------------
	//---------------------------------------------------------------------
	if (millis() - stopbeginn1 >= stopzeit1) {
		digitalWrite(enabl1, LOW); // Treiber Motor1 ein
		digitalWrite(dir1, richtung1); // Set Dir
		if (x1 < schritte1)    // Ziel noch nicht erreicht?
		{
			//----------------------Motor 1----------------------------------------
			//---------------------Schritt 1---------------------------------------
			if ((schrittposition1 == 1) && (micros() - whablauf1 >= mini1)) {
				digitalWrite(puls1, HIGH); // Step auf high
				schrittposition1 = 2;  // Steuerung für Schritt 1 einstellen
				pwablauf1 = micros(); // Timer für Schritt 1 starten
			}
			//----------------------Motor 1----------------------------------------
			//---------------------Schritt 2---------------------------------------
			if ((schrittposition1 == 2) && (micros() - pwablauf1 >= mini1)) {
				digitalWrite(puls1, LOW); // Step auf low
				schrittposition1 = 1;  // Steuerung für Schritt 0 einstellen
				whablauf1 = micros(); // Timer für Schritt 0 starten
				x1++;
				//-----------------------------------------------------------------
				//-----------------Rampensteuerung Motor1--------------------------
				if (x1 <= rampenstart1) {
					rampenrichtung1 = HIGH;
				}
				else {
					rampenrichtung1 = LOW;
				}
				if (rampenrichtung1 == HIGH) {
					if (mini1 > maxi1) {
						mini1 = mini1 - 2; // Rampe für Motor 1 Gas geben
					}
				}
				if (rampenrichtung1 == LOW) {
					if (x1 + rampenstart1 > schritte1) {
						mini1 = mini1 + 2; // Rampe für Motor 1 bremsen
					}
				}
				//-----------------------------------------------------------------
			}
		}
		//-------------------------Motor 1-------------------------------------
		//----------------------Ziel erreicht ?--------------------------------
		if (x1 == schritte1) {
			x1 = 0;
			richtung1 = !richtung1; // Dir Richtung wechseln
			mini1 = rampenstart1; //Rampe für Motor 1 Neustart
			digitalWrite(enabl1, HIGH);//Motor1 aus
			stopbeginn1 = millis();
		}
	}
	//---------------------------------------------------------------------


	//---------------------------------------------------------------------
	//----------Treiber 2 aktivieren---------------------------------------
	//---------------------------------------------------------------------
	if (millis() - stopbeginn2 >= stopzeit2) {
		digitalWrite(enabl2, LOW); // Treiber Motor2 ein
		digitalWrite(dir2, richtung2); // Set Dir
		if (x2 < schritte2)    // Ziel noch nicht erreicht?
		{
			//----------------------Motor 2--------------------------------------
			//---------------------Schritt 1-------------------------------------
			if ((schrittposition2 == 1) && (micros() - whablauf2 >= mini2)) {
				digitalWrite(puls2, HIGH); // Step auf HIGH
				schrittposition2 = 2;  // Steuerung für Schritt 1 einstellen
				pwablauf2 = micros(); // Timer für Schritt 1 starten
			}
			//----------------------Motor 2--------------------------------------
			//---------------------Schritt 2-------------------------------------
			if ((schrittposition2 == 2) && (micros() - pwablauf2 >= mini2)) {
				digitalWrite(puls2, LOW); // Step auf low
				schrittposition2 = 1;  // Steuerung für Schritt 0 einstellen
				whablauf2 = micros(); // Timer für Schritt 0 starten
				x2++;
				//-----------------------------------------------------------------
				//-----------------Rampensteuerung Motor2--------------------------
				if (x2 <= rampenstart2) {
					rampenrichtung2 = HIGH;
				}
				else {
					rampenrichtung2 = LOW;
				}
				if (rampenrichtung2 == HIGH) {
					if (mini2 > maxi2) {
						mini2 = mini2 - 2; // Rampe für Motor 1 Gas geben
					}
				}
				if (rampenrichtung2 == LOW) {
					if (x2 + rampenstart2 > schritte2) {
						mini2 = mini2 + 2; // Rampe für Motor 1 bremsen
					}
				}
				//----------------------------------------------------------------
			}
		}
		//-------------------------Motor 2------------------------------------
		//----------------------Ziel erreicht ?-------------------------------
		if (x2 == schritte2) {
			x2 = 0;
			mini2 = rampenstart2; //Rampe für Motor 2 Neustart
			richtung2 = !richtung2; // Dir Richtung wechseln
			digitalWrite(enabl2, HIGH);//Motor2 aus  
			stopbeginn2 = millis();
		}
	}
	

}