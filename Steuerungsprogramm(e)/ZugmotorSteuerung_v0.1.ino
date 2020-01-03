
    //Name:       ZugmotorSteuerung.ino
    //Created:	02.01.2020 08:54:18
    //Author:     Joachim Rüber & Jörg Knaebel


#include <Wire.h>

#define pMotortakt 4
#define pRichtung 5
#define pFreigabe 6

int iZugmotor = 0;						// Zugmotor ein bei 1
uint16_t ivZugmotor = 1;      // Vorschub Zugmotor
//------Steuerung f�r den ersten Schrittmotor------------------
int schrittposition1 = 1;
			// Der Microwert für pwzeit & whzeit nicht unter 200 !
unsigned long pwablauf1 = 0;							// Pulswechselablaufzeit1
unsigned long whablauf1 = 0;							// Wiederhohlablaufzeit1
unsigned long lSchaltzeit = 0;						// Zeit wird in Microsekunden übertragen berechnung der Schaltzeit macht der Mega
uint8_t x[4];




void setup() {
	pinMode(pMotortakt, OUTPUT);
	pinMode(pFreigabe, OUTPUT);
	pinMode(pRichtung, OUTPUT);

	digitalWrite(pFreigabe, HIGH); 
	digitalWrite(pRichtung, HIGH);
	Wire.begin(3);
	Wire.onReceive(get_Motor_Parameter);
//  Serial.begin(9600);
//  Serial.println("Bereit");
}



void loop()
{
	if (lSchaltzeit >= 1 && iZugmotor == 1)
	{
    digitalWrite(pFreigabe, LOW);
		//----------------------Motor 1----------------------------------------
		//---------------------Schritt 1---------------------------------------
		if ((schrittposition1 == 1) && (micros() - whablauf1 >= lSchaltzeit))
		{
			digitalWrite(pMotortakt, HIGH);								// Step auf high
			schrittposition1 = 2;										// Steuerung f�r Schritt 1 einstellen
			pwablauf1 = micros();										// Timer f�r Schritt 1 starten
		}
		//----------------------Motor 1----------------------------------------
		//---------------------Schritt 2---------------------------------------
		if ((schrittposition1 == 2) && (micros() - pwablauf1 >= lSchaltzeit))
		{
			digitalWrite(pMotortakt, LOW);								// Step auf low
			schrittposition1 = 1;										// Steuerung f�r Schritt 0 einstellen
			whablauf1 = micros();										// Timer f�r Schritt 0 starten
		}
	}
}

void get_Motor_Parameter(int howMany) 
	{
        for (int i=0; i<4;i++)
        {
            x[i] = Wire.read();
            
        }
    //int x1 = 0;
    //int x2 = 0;
    //int x3 = 0;
    //int x4 = 0;
    float y = 0;
    float xx = 0;
    float z = 0;
    //x1 = Wire.read();
    //x2 = Wire.read();
    //x3 = Wire.read();
    //x4 = Wire.read();
		iZugmotor = Wire.read();
    delay(1);
    ivZugmotor = x[0] + x[1] + x[2] + x[3];
    xx = ivZugmotor;
    y = xx/97,69;
    z = y * 1600;
    lSchaltzeit = 30000000/z;
//    Serial.println("Erhalten");
//    Serial.println(ivZugmotor);
//    Serial.println(x);
//    Serial.println(y);
//    Serial.println(z);
//    Serial.println(lSchaltzeit);
	}
