

/*
	Name:     Extrudersteuerung.ino
	Version:  v0.9
	Created:	25.11.2019  20:12
	Author:   Joachim Rüber &	Jörg Knaebel

*/

//  Eingebundene Libarys
#include "Nextion.h"
#include "Nextion_Defs.h"
#include <MAX6675.h>
#include <SimpleTimer.h>
#include <FastPID.h>
#include <Wire.h>
#include <EEPROM.h>

// Nextion
int iNexpage = 0;				//  Merker welche Seite auf dem Display angezeigt wird, damit nur Daten der aufgerufenen Seite übertragen werden
byte bExtruder = 0;				//  Merker Extrudermotor false = aus true = ein
byte bHeizung = 0;				//  Merker Heizung false = aus true = ein
byte bKuehlung = 0;				//  Merker Kühlung false = aus true = ein
int iMaterialart = 0;			//  Auswahl Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM !! nur mal vorläufig !!
int iMaterialfarbe = 0;			//  Auswahl Materialfarbe 1=klar 2= weiß 3= gelb 4= schwarz 5= blau 6= grün 7= rot !! nur mal vorläufig !!
int iVExtruder = 1;				//  Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iTempSollHeizblock = 0;     //  Soll Temperatur Heizblock
int iTempSollDuese = 0;			//  Soll Temperatur Düse
float fAmpMotor = 0.0;			//  Iststrom Motor Extruder über ACS712 gemessen Berechnung nicht als float Nextion setzt das komma deshalb Wert mal 10 als long
float fAmpHeizblock = 0.0;		//  Iststrom Heizung Heizblock über ACS712 gemessen
float fAmpDuese = 0.0;			//  Iststrom Heizung Düse über ACS712 gemessen
char NextionWert[15];			//  Hilfsvariable um Daten aus Variablen in Textfelder anzuzeigen
char *cError_text[] = { "Übertemperatur Block", "Übertemperatur Düse", "Antrieb Extr. blockiert", "Antrieb Aufw. blockiert", "...usw..."};
int iDatensatzNum = 0;

// Strommessung Motor
int VpA1 = 185;					// Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
int Nullpunkt = 2507;			// Spannung in mV bei dem keine Stromstärke vorhanden ist
int VpA2 = 100;					// Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
//float frefAMotor, frefHeizungDuese, frefHeizungBlock;		// Referenzwerte der Ströme

// Temperaturmessung
float fTmpBlck, fTmpDs;			// gemessene float - Temperaturen
int iTmpBlck, iTmpDs;			// umgewandelte int - Temperaturen


// PID-Regler Block
float Kp_Blck = 0.1, Ki_Blck = 0.5, Kd_Blck = 0, Hz_Blck = 10;
int output_bits_Blck = 1;
bool output_signed_Blck = false;

// PID-Regler Düse
float Kp_Ds = 0.1, Ki_Ds = 0.5, Kd_Ds = 0, Hz_Ds = 10;
int output_bits_Ds = 1;
bool output_signed_Ds = false;


// EEprom
int Rezept[20][5];              // 20 Datensätze mit je 5 Feldern
int iEEMaterialart = 0;         //  Datensatz aus EEPRom Auswahl Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM !! nur mal vorläufig !!
int iEEMaterialfarbe = 0;       //  Datensatz aus EEPRom Auswahl Materialfarbe 1=klar 2= weiß 3= gelb 4= schwarz 5= blau 6= grün 7= rot !! nur mal vorläufig !!
int iEEVExtruder = 0;           //  Datensatz aus EEPRom Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iEETempSollHeizblock = 0;   //  Datensatz aus EEPRom Soll Temperatur Heizblock
int iEETempSollDuese = 0;       //  Datensatz aus EEPRom Soll Temperatur Düse
int i = 0;                      //  Hilfsvariable

// Filamentdicke und Zugmotor
uint16_t ivZugmotor = 1;			// in mm/min wird umgerechnet in 
int iZugmotor = 0;				// wird 1 bei Heizung auf Temperatur und Extruder ein
float faWertSensor1 = 0;
float faWertSensor2 = 0;
float fReferenzwertdicke = 0;
int x1 = 0;
int x2 = 0;
int x3 = 0;
int x4 = 0;

//  Listen to buttons, sliders, etc.
NexTouch *nex_listen_list[] =
{
  &b0,
  &b1,
  &b2,
  &b3,
  &b4,
  &b5,
  &b6,
  &b7,
  &b8,
  &b9,
  &b10,
  &b11,
  &b12,
  &b13,
  &b14,
  &b15,
  &b16,
  &b17,
  &b18,
  &b19,
  &b20,
  //&b21,
  //&b22,
  &b23,
  &b24,
  //&b25,
  //&b26,
  &b27,
  &bt0,
  &bt1,
  &bt2,
  &n0,
  &n1,
  &n2,
  &n3,
  &n4,
  &n5,
  &n30,
  &h0,
  &h1,
  &h2,
  &va6,
  &page0,
  &page1,
  &page2,
  &page3,
  &page4,
  &page5,
  &page6,
  &page7,
  NULL      //  String terminated
};          //  End of touch event list

MAX6675 tmpBlck(CS_PIN_BLCK);
MAX6675 tmpDs(CS_PIN_DS);
SimpleTimer timer_1;    // Timer_1 Anzeige updaten
SimpleTimer timer_2;     // Timer_2 Aufruf PID-Regler
FastPID myPID_Blck(Kp_Blck, Ki_Blck, Kd_Blck, Hz_Blck, output_bits_Blck, output_signed_Blck);
FastPID myPID_Ds(Kp_Ds, Ki_Ds, Kd_Ds, Hz_Ds, output_bits_Ds, output_signed_Ds);

//  The setup() function runs once each time the micro-controller starts
void setup()
{

	// Alle 20 Datensätze aus dem EEPROM einlesen
	for (int i = 1; i <= 20; i++)
	{
		LadenEEprom(i);
	}

	/*------------------------------------
		PinMode Definitionen
		------------------------------------*/
	pinMode(pFreigabeHeizung, OUTPUT);
	pinMode(pKuehlung, OUTPUT);

	digitalWrite(pFreigabeHeizung, LOW);
	digitalWrite(pHeizungDuese, HIGH);
	digitalWrite(pHeizungHeizblock, HIGH);
	digitalWrite(pKuehlung, LOW);

	timer_1.setInterval(1000);    // timer_1 auf 1s gesetzt
	timer_2.setInterval(100);     // timer_2 auf 100ms gesetzt

	// I2C begin
	Wire.begin();

	Serial.begin(9600);
	nexSerial.begin(9600);

	nexInit();
	b0.attachPush(b0PushCallback, &b0);     //  Button press
	b1.attachPush(b1PushCallback, &b1);
	b2.attachPush(b2PushCallback, &b2);
	b3.attachPush(b3PushCallback, &b3);
	b4.attachPush(b4PushCallback, &b4);
	b5.attachPush(b5PushCallback, &b5);
	b6.attachPush(b6PushCallback, &b6);
	b7.attachPush(b7PushCallback, &b7);
	b8.attachPush(b8PushCallback, &b8);
	b9.attachPush(b9PushCallback, &b9);
	b10.attachPush(b10PushCallback, &b10);
	b11.attachPush(b11PushCallback, &b11);
	b12.attachPush(b12PushCallback, &b12);
	b13.attachPush(b13PushCallback, &b13);
	b14.attachPush(b14PushCallback, &b14);
	b15.attachPush(b15PushCallback, &b15);
	b16.attachPush(b16PushCallback, &b16);
	b17.attachPush(b17PushCallback, &b17);
	b18.attachPush(b18PushCallback, &b18);
	b19.attachPush(b19PushCallback, &b19);
	b20.attachPush(b20PushCallback, &b20);
	//b21.attachPush(b21PushCallback, &b21);
	//b22.attachPush(b22PushCallback, &b22);
	b23.attachPush(b23PushCallback, &b23);
	b24.attachPush(b24PushCallback, &b24);
	//b25.attachPush(b25PushCallback, &b25);
	//b26.attachPush(b26PushCallback, &b26);
	b27.attachPush(b27PushCallback, &b27);


	/*------------------------------------
	  Platzhalter für weitere Buttons
	  ------------------------------------*/
	bt0.attachPush(bt0PushCallback, &bt0);
	bt1.attachPush(bt1PushCallback, &bt1);
	bt2.attachPush(bt2PushCallback, &bt2);

	/*------------------------------------
	  Platzhalter für weitere Dual State Buttons
	  ------------------------------------*/

	n0.attachPush(n0PushCallback, &n0);
	n1.attachPush(n1PushCallback, &n1);
	n2.attachPush(n2PushCallback, &n2);
    n30.attachPush(n30PushCallback, &n30);

	/*------------------------------------
	  Platzhalter für weitere Number anzeigen mit Touchfunktion
	  ------------------------------------*/

	//Serial.println("Setup done");

}

void loop()
{
	nexLoop(nex_listen_list);

	// SwitchCaseAnzeige() aufrufen
	if (timer_1.isReady())		// 1 Sekunde
	{
		SwitchCaseAnzeige();
		timer_1.reset();
	}

	// PID Heizungsregelungen aufufen
	if (timer_2.isReady())		// 100ms
	{
		getTemp();
		if (bHeizung)
		{
			if (iTmpBlck < (iTempSollHeizblock * 0.9))
			{
				digitalWrite(pHeizungHeizblock, LOW);
				myPID_Blck.clear();
			}
			else
			{
				digitalWrite(pHeizungHeizblock, !(myPID_Blck.step(iTempSollHeizblock, iTmpBlck)));
			}

			if (iTmpDs < (iTempSollDuese * 0.9))
			{
				digitalWrite(pHeizungDuese, LOW);
				myPID_Ds.clear();
			}
			else
			{
				digitalWrite(pHeizungDuese, !(myPID_Ds.step(iTempSollDuese, iTmpDs)));
			}
			// Mit diesen Anweisungen wird der Strom der Heizung gemessen
			// Frage ist ob man die beiden Heizung so schalten kann das zyklisch für die Messung nur der Heizblock oder die Düse an sind
			float SensorSpannung = 0;
			int SensorwertHeizung = 0;
			SensorwertHeizung = analogRead(pStromHeizung);
			SensorSpannung = (SensorwertHeizung / 1024.0) * 5000;
			fAmpDuese = ((SensorSpannung - Nullpunkt) / VpA2);
		}
		if (fReferenzwertdicke > 0 && bHeizung && bExtruder && bKuehlung)		// Messung der Dicke des Filaments wird sich zeigen ob man 2 braucht
		{
			faWertSensor1 = (analogRead(pDickesensor1) * 0.0049);
			faWertSensor2 = (analogRead(pDickesensor2) * 0.0049);
		}
		if (bExtruder)			// Messung des Motorstrom 
		{
			float SensorSpannung = 0;
			int SensorwertMotor = 0;
			SensorwertMotor = analogRead(pStromMotor);
			SensorSpannung = (SensorwertMotor / 1024.0) * 5000;
			fAmpMotor = ((SensorSpannung - Nullpunkt) / VpA1);
		}
		timer_2.reset();
	}
}

void LadenEEprom(int x)			// x = Rezeptnummer
{
	for (int i = 1; i <= 5; i++)
	{
		Rezept[x][i] = EEPROM.read(x * 20 - 20 + i);
	}
}

void SpeichernEEprom(int x)		// x = Rezeptnummer
{
	for (int i = 1; i <= 5; i++)
	{
		EEPROM.update(x * 20 - 20 + i, Rezept[x][i]);
	}
}

void getTemp()    // Temperaturen ablesen
{
	fTmpBlck = tmpBlck.readTempC();
	fTmpDs = tmpDs.readTempC();
	iTmpBlck = (int)(fTmpBlck + .5);  // float nach int umwandeln wegen Nextion
	iTmpDs = (int)(fTmpDs + .5);
}

void nanoWrite1()
{
	Wire.beginTransmission(2);			// Übertragung an Device #2
	Wire.write(iVExtruder);				// Sollwert Drehzahl Extrdr an NANO senden
	Wire.write(bExtruder);				// Antriebsfreigabe an NANO senden
	Wire.endTransmission();
}
void nanoWrite2()						// Datenübertragung an Uno zur Steuerung der Zug- und Aufwickeleinheit !! Weitere Variablen kommen noch dazu !!
{
	Wire.beginTransmission(3);			// Übertragung an Device #3
	Wire.write(x1);						// Sollwert Vorschub an UNO senden
	Wire.write(x2);
	Wire.write(x3);
	Wire.write(x4);
	Wire.write(iZugmotor);				// Antriebsfreigabe an UNO senden
	Wire.endTransmission();
	x1 = 0;
	x2 = 0;
	x3 = 0;
	x4 = 0;
}

void SwitchCaseAnzeige()			  // Daten zur aktiven Seite senden
{
	switch (iNexpage)
	{
	case 0:							          // Anzeigen übertragen auf Page 0  Home
		n0.setValue(iTmpDs);
		n1.setValue(iTmpBlck);
		n2.setValue(iVExtruder);
		n30.setValue(ivZugmotor);
		break;
	case 1:							          // Anzeigen übertragen auf Page 1  Setup Geschwindigkeit Temperatur
		n6.setValue(iVExtruder);
		n7.setValue(iTmpDs);
		n8.setValue(iTmpBlck);
		break;
	case 2:							          // Anzeigen übertragen auf Page 2  Setup Zugmotor
		//dtostrf(MeinWert, 0, 1, NextionWert);
		//nexSerial.print(faWertSensor1);
		//nexSerial.print(faWertSensor2);
		//nexSerial.print(fReferenzwertdicke);
		break;
	case 3:							          // Anzeigen übertragen auf Page 3  Datensatz
		break;
	case 4:							          // Anzeigen übertragen auf Page 4  Stromanzeige und Referenzieren
		dtostrf(fAmpMotor, 3, 1, NextionWert);
		t40.setText(NextionWert);
		dtostrf(fAmpDuese, 3, 1, NextionWert);
		t41.setText(NextionWert);
		break;
	case 5:							          // Anzeigen übertragen auf Page 5  Osziloskop Strom Temp usw
		//s0.addValue(0, iTmpDs);
		//s0.addValue(1, iTmpBlck);
		//i = fAmpMotor * 10;
		//s0.addValue(2, i);

		break;
	case 6:							// Datensätze anzeigen.
		va8.setValue(iEEMaterialart);
		va9.setValue(iEEMaterialfarbe);
		n12.setValue(i);
		n13.setValue(iEEVExtruder);
		n14.setValue(iEETempSollDuese);
		n15.setValue(iEETempSollHeizblock);
		break;
	case 7:

		break;
	}

}

//  Nextion Buttonprogramme
void b0PushCallback(void *ptr)		//  Seitenwechsel nach Page 1 Geschwindigkeit und Temperatureinstellung
{
	iNexpage = 1;
}

void b1PushCallback(void *ptr)		// Seitenwechsel nach Page 3 Datensatz ---UNDER CONSTRUTION---
{
	iNexpage = 3;
}

void b2PushCallback(void *ptr)		// Seitenwechsel nach Page 0 Home
{
	iNexpage = 0;
}

void b3PushCallback(void *ptr)		// Seitenwechsel nach Page 2 Aufwicklung Setup ---UNDER CONSTRUTION---
{
	iNexpage = 2;
}

void b4PushCallback(void *ptr)		// Seitenwechsel nach Page 4 Stromanzeige
{
	iNexpage = 4;
}

void b5PushCallback(void *ptr)		// Seitenwechsel nach Page 1  Geschwindigkeit und Temperatureinstellung
{
	iNexpage = 1;
}

void b6PushCallback(void *ptr)		// Übernahme Referenzwert Motorstrom
{

}

void b7PushCallback(void *ptr)		// Übernahme Referenzwert Strom Heizung
{

}

void b8PushCallback(void *ptr)		// Frei für andere Funktionen
{

}

void b9PushCallback(void *ptr)		// Seitenwechsel nach Page 5 Osziloskopanzeige Ströme Temperaturen
{
	iNexpage = 5;
}

void b10PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	iNexpage = 4;
}

void b11PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	iNexpage = 1;
}

void b12PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	iNexpage = 0;
}

void b13PushCallback(void *ptr)		// Da die Datenübertragung der Werte mit den Slidern nicht funktioniert hat, wurde der ENTER Button Umweg genutzt
{
	uint32_t number = 0;
	n3.getValue(&number);			// Abfrage des Wertes Soll Extrudergeschwindigkeit
	iVExtruder = number;			// Übertragen von Temp in Variable
	nanoWrite1();					// Freigabe und Drehzahl an NANO senden
	delay(5);

	n4.getValue(&number);			// Abfrage des Wertes Soll Düsentemperatur
	iTempSollDuese = number;		// Übertragen von Temp in Variable
	delay(5);
	n5.getValue(&number);			// Abfrage des Wertes Soll Heizblocktemperatur
	iTempSollHeizblock = number;	// Übertragen von Temp in Variable
}

void b14PushCallback(void *ptr)		// Button zum Speichern eines Datensatzes Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM 
{
	uint32_t number = 0;			// Hier wird der Wert der Variable für die Materialart abgerufen
	va6.getValue(&number);
	iMaterialart = number;
	delay(2);
	va7.getValue(&number);
	iMaterialfarbe = number;
	
	for (int i = 1; i <= 20; i++)
	{
		if (Rezept[i][1] == 0)
		{
			Rezept[i][1] = iMaterialart;
			Rezept[i][2] = iMaterialfarbe;
			Rezept[i][3] = iTempSollHeizblock;
			Rezept[i][4] = iTempSollDuese;
			Rezept[i][5] = iVExtruder;
			SpeichernEEprom(i);
			break;
		}
	}
  t10.setText("Speichern angeschlossen");
}

void b15PushCallback(void *ptr)		// Button zum Laden eines Datensatzes 
{
	iMaterialart = iEEMaterialart;
	iMaterialfarbe = iEEMaterialfarbe;
	iTempSollHeizblock = iEETempSollHeizblock;
	iTempSollDuese = iEETempSollDuese;
	iVExtruder = iEEVExtruder;
	va3.setValue(iVExtruder);
  delay(2);
	va4.setValue(iTempSollDuese);
	delay(2);
	va5.setValue(iTempSollHeizblock);
	delay(2);
	va6.setValue(iMaterialart);
	delay(2);
	va7.setValue(iMaterialfarbe);
  delay(2);
  t11.setText("Laden erfolgreich");
}

void b16PushCallback(void *ptr)		// Anwahl des Datensates +1
{
	uint32_t number = 0;
	n12.getValue(&number);
	number++;
	if (number > 20) number = 1;
	i = number;
  delay(2);
	n12.setValue(number);
	
	iEEMaterialart = Rezept[i][1];
	iEEMaterialfarbe = Rezept[i][2];
	iEETempSollHeizblock = Rezept[i][3];
	iEETempSollDuese = Rezept[i][4];
	iEEVExtruder = Rezept[i][5];

	// Daten werden in Case Funktion auf Nextion geschrieben
}

void b17PushCallback(void *ptr)		// Anwahl des Datensates -1
{
	uint32_t number = 0;
	n12.getValue(&number);
	number--;
	if (number < 1) number = 20;
	i = number;
  delay(2);
	n12.setValue(number);

	iEEMaterialart = Rezept[i][1];
	iEEMaterialfarbe = Rezept[i][2];
	iEETempSollHeizblock = Rezept[i][3];
	iEETempSollDuese = Rezept[i][4];
	iEEVExtruder = Rezept[i][5];
	// Daten werden in Case Funktion auf Nextion geschrieben
}

void b18PushCallback(void *ptr)		// Seitenwechsel nach Page 3
{
	iNexpage = 3;
}

void b19PushCallback(void *ptr)		// Seitenwechsel nach Page 6
{
	iNexpage = 6;
}

void b20PushCallback(void *ptr)		// Seitenwechsel nach Page 7
{
	iNexpage = 7;
}

void b21PushCallback(void *ptr)		// Drehzahl des Zugmotors um +1 mm/min erhöhen
{

}

void b22PushCallback(void *ptr)		// Drehzahl des Zugmotors um -1 mm/min verringern
{

}

void b23PushCallback(void *ptr)		// Referenzwerte Dicke Filament speichern
{

//	fReferenzwertdicke = (((analogRead(pDickesensor1) + analogRead(pDickesensor2)) / 2) * 0.0049);	// Referenzdicke aus Durchschnitt beider Sensoren
//	dtostrf(fReferenzwertdicke, 5, 3, NextionWert);
//	t54.setText(NextionWert);

}

void b24PushCallback(void *ptr)		// Seitenwechsel nach Page 2
{
	iNexpage = 2;
}

void b25PushCallback(void *ptr)		// Vorschub Zugmotor um +10mm
{

}

void b26PushCallback(void *ptr)		// Vorschub Zugmotor um -10mm
{

}

void b27PushCallback(void *ptr)		// Drehzahl des Zugmotors um +1 mm/min erhöhen
{	
	uint32_t number = 0;
	n16.getValue(&number);
	ivZugmotor = number;
	if (number > 765)
	{
		x1 = number - 765;
		number = number - x1;
	}
	if (number > 510)
	{
		x2 = number - 510;
		number = number - x2;
	}
	if (number > 255)
	{
		x3 = number - 255;
		number = number - x3;
	}
	if (number < 255)
	{
		x4 = number;
	}
	nanoWrite2();
}

void bt0PushCallback(void *ptr)   //  Ein/Aus Extruderschnecke
{
	uint32_t number = 0;
	bt0.getValue(&number);
	bExtruder = number;
	if (bExtruder == 1){
    iZugmotor = 1;
	}
 if (bExtruder == 0){
    iZugmotor = 0;
 }
	nanoWrite1();   // Freigabe und Drehzahl an NANO senden
	delay(2);
	nanoWrite2();

}

void bt1PushCallback(void *ptr)    //  Ein/Aus Heizung
{
	uint32_t number = 0;
	bt1.getValue(&number);
	if (number == 0)
	{
		bHeizung = 0;
		digitalWrite(pFreigabeHeizung, LOW);
	}
	else if (number == 1)
	{
		bHeizung = 1;
		digitalWrite(pFreigabeHeizung, HIGH);
	}
}

void bt2PushCallback(void *ptr)   //  Ein/Aus Kühlung
{
	uint32_t number = 0;
	bt2.getValue(&number);
	if (number == 0)
	{
		bKuehlung = 0;
		digitalWrite(pKuehlung, LOW);
	}
	else if (number == 1)
	{
		bKuehlung = 1;
		digitalWrite(pKuehlung, HIGH);
	}
}

void n0PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h0 ein
{
	iNexpage = 1;
}

void n1PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h1 ein
{
	iNexpage = 1;
}

void n2PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{
	iNexpage = 1;
}
void n30PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{
  iNexpage = 2;
}
