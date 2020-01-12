/*
	Name:     Extrudersteuerung.ino
	Version:  v0.11
	Created:	12.01.2020  20:12
	Author:   Joachim Rüber &	Jörg Knaebel

*/

//  Eingebundene Libarys
#include "Nextion.h"
#include "Nextion_Defs.h"
#include <MAX6675.h>
#include <SimpleTimer.h>
#include <PID_v1.h>
#include <Wire.h>
#include <EEPROM.h>
//#include <FastPID.h>

// Nextion
int iNexpage = 0;				      //  Merker welche Seite auf dem Display angezeigt wird, damit nur Daten der aufgerufenen Seite übertragen werden
byte bExtruder = 0;				      //  Merker Extrudermotor false = aus true = ein
byte bHeizung = 0;				      //  Merker Heizung false = aus true = ein
byte bKuehlung = 0;				      //  Merker Kühlung false = aus true = ein
int iMaterialart = 0;			      //  Auswahl Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM !! nur mal vorläufig !!
int iMaterialfarbe = 0;				  //  Auswahl Materialfarbe 1=klar 2= weiß 3= gelb 4= schwarz 5= blau 6= grün 7= rot !! nur mal vorläufig !!
int iVExtruder = 1;				      //  Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
double iTempSollHeizblock = 0;			  //  Soll Temperatur Heizblock
double iTempSollDuese = 0;			      //  Soll Temperatur Düse
float fAmpMotor = 0.0;			      //  Iststrom Motor Extruder über ACS712 gemessen Berechnung nicht als float Nextion setzt das komma deshalb Wert mal 10 als long
float fAmpHeizung = 0.0;			  //  Iststrom Heizung Heizblock über ACS712 gemessen
char NextionWert[15];			      //  Hilfsvariable um Daten aus Variablen in Textfelder anzuzeigen
char *cError_text[] = { "Übertemperatur Block", "Übertemperatur Düse", "Antrieb Extr. blockiert", "Antrieb Aufw. blockiert", "Fuellstand gering"};
int iDatensatzNum = 0;
int MaterialFuellstand = 0;
int HilfsVarFehler = 0;				  // Hilfsvariable damit Fehler nur einmal ans Nextion gesendet werden
int iUmrechnungAnzeige = 0;

// Strommessung Motor
int VpA1 = 185;					    // Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
int Nullpunkt = 2507;			    // Spannung in mV bei dem keine Stromstärke vorhanden ist
int VpA2 = 100;					    // Millivolt pro Ampere (100 für 20A Modul und 66 für 30A Modul)
//float frefAMotor, frefHeizungDuese, frefHeizungBlock;		// Referenzwerte der Ströme

// Temperaturmessung
float fTmpBlck, fTmpDs;			    // gemessene float - Temperaturen
double iTmpBlck, iTmpDs;			// umgewandelte int - Temperaturen

// PID-Regler Block
double iWertR_Blck = 0;
double aggKp_Blck=4, aggKi_Blck=0.2, aggKd_Blck=1;
double consKp_Blck=0.05, consKi_Blck=4, consKd_Blck=0.25;
PID myPID_Blck(&iTmpBlck, &iWertR_Blck, &iTempSollHeizblock, consKp_Blck, consKi_Blck, consKd_Blck, DIRECT);
// float Kp_Blck = 0.1, Ki_Blck = 0.5, Kd_Blck = 0, Hz_Blck = 10;
// int output_bits_Blck = 8;
// bool output_signed_Blck = false;

// PID-Regler Düse
double iWertR_DS = 0;
double aggKp_Ds=4, aggKi_Ds=0.2, aggKd_Ds=1;
double consKp_Ds=0.007, consKi_Ds=3, consKd_Ds=0.5;
PID myPID_Ds(&iTmpDs, &iWertR_DS, &iTempSollDuese, consKp_Ds, consKi_Ds, consKd_Ds, DIRECT);
// float Kp_Ds = 0.1, Ki_Ds = 0.5, Kd_Ds = 0, Hz_Ds = 10;
// int output_bits_Ds = 8;
// bool output_signed_Ds = false;


// EEprom
int Rezept[20][6];              // 20 Datensätze mit je 6 Feldern
int iEEMaterialart = 0;         //  Datensatz aus EEPRom Auswahl Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM !! nur mal vorläufig !!
int iEEMaterialfarbe = 0;       //  Datensatz aus EEPRom Auswahl Materialfarbe 1=klar 2= weiß 3= gelb 4= schwarz 5= blau 6= grün 7= rot !! nur mal vorläufig !!
int iEEVExtruder = 0;           //  Datensatz aus EEPRom Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iEETempSollHeizblock = 0;   //  Datensatz aus EEPRom Soll Temperatur Heizblock
int iEETempSollDuese = 0;       //  Datensatz aus EEPRom Soll Temperatur Düse
int iEEVZugmotor = 0;			//  Datensatz aus EEPRom Soll Geschwindigkeit Zugmotor
int i = 0;                      //  Hilfsvariable

// Filamentdicke und Zugmotor
uint16_t ivZugmotor = 1;			  // in mm/min wird umgerechnet in 
int iZugmotor = 0;					   // wird 1 bei Heizung auf Temperatur und Extruder ein
float faWertSensor1 = 0;
float faWertSensor2 = 0;
float fReferenzwertdicke = 0;
uint8_t x[4];


//  Listen to buttons, sliders, etc.
NexTouch *nex_listen_list[] =
{
	&b0, 
	&b1,
	&b10,
	&b11,
	&b12,
	&b13,
	&b20,
	&b21,
	&b22,
	&b30,
	&b31,
	&b32,
	&b40,
	&b41,
	&b42,
	&b43,
	&b50,
	&b60,
	&b61,
	&b62,
	&b63,
	&b70,
	&b210,
	&bt0,
	&bt1,
	&bt2,
	&n3,
	&n4,
	&n5,
	&n6,
	&n17,
	&n18,
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
// FastPID myPID_Blck(Kp_Blck, Ki_Blck, Kd_Blck, Hz_Blck, output_bits_Blck, output_signed_Blck);
// FastPID myPID_Ds(Kp_Ds, Ki_Ds, Kd_Ds, Hz_Ds, output_bits_Ds, output_signed_Ds);

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
	pinMode(pFreigabeHeizung, OUTPUT);				// Wird ab der Hauptplatine V2 nicht mehr benötigt
	pinMode(pHeizungDuese, OUTPUT);
	pinMode(pHeizungHeizblock, OUTPUT);
	pinMode(pKuehlung, OUTPUT);

	digitalWrite(pFreigabeHeizung, LOW);
	analogWrite(pHeizungDuese, 255);
	analogWrite(pHeizungHeizblock, 255);
	digitalWrite(pKuehlung, LOW);

	timer_1.setInterval(1000);    // timer_1 auf 1s gesetzt
	timer_2.setInterval(100);     // timer_2 auf 100ms gesetzt

	myPID_Blck.SetMode(AUTOMATIC);
	myPID_Ds.SetMode(AUTOMATIC);
	// I2C begin
	Wire.begin();

	Serial.begin(9600);
	nexSerial.begin(9600);

	nexInit();
	b0.attachPush(b0PushCallback, &b0);     //  Button press
	b1.attachPush(b1PushCallback, &b1);
	b10.attachPush(b10PushCallback, &b10);
	b11.attachPush(b11PushCallback, &b11);
	b12.attachPush(b12PushCallback, &b12);
	b13.attachPush(b13PushCallback, &b13);
	b20.attachPush(b20PushCallback, &b20);
	b21.attachPush(b21PushCallback, &b21);
	b22.attachPush(b22PushCallback, &b22);
	b30.attachPush(b30PushCallback, &b30);
	b31.attachPush(b31PushCallback, &b31);
	b32.attachPush(b32PushCallback, &b32);
	b40.attachPush(b40PushCallback, &b40);
	b41.attachPush(b41PushCallback, &b41);
	b42.attachPush(b42PushCallback, &b42);
	b43.attachPush(b43PushCallback, &b43);
	b50.attachPush(b50PushCallback, &b50);
	b60.attachPush(b60PushCallback, &b60);
	b61.attachPush(b61PushCallback, &b61);
	b62.attachPush(b62PushCallback, &b62);
	b63.attachPush(b63PushCallback, &b63);
	b70.attachPush(b70PushCallback, &b70);
	//b210.attachPush(b210PushCallback, &b210);					// Mit der Entereingabe der Tastatur sollen die Daten übertragen werden -- Under Construction --

	/*------------------------------------
	  Platzhalter für weitere Buttons
	  ------------------------------------*/
	bt0.attachPush(bt0PushCallback, &bt0);
	bt1.attachPush(bt1PushCallback, &bt1);
	bt2.attachPush(bt2PushCallback, &bt2);

	/*------------------------------------
	  Platzhalter für weitere Dual State Buttons
	  ------------------------------------*/

	n3.attachPush(n3PushCallback, &n3);
	n4.attachPush(n4PushCallback, &n4);
	n5.attachPush(n5PushCallback, &n5);
    n6.attachPush(n6PushCallback, &n6);

	/*------------------------------------
	  Platzhalter für weitere Number anzeigen mit Touchfunktion
	  ------------------------------------*/

	//Serial.println("Setup done");

	page0.show();				// Nach einem Neustart wird immer Page 0 auf dem Nextion angezeigt ( Homescreen )
}

void loop()
{
	nexLoop(nex_listen_list);

	// SwitchCaseAnzeige() aufrufen
	if (timer_1.isReady())		// 1 Sekunde
	{
		SwitchCaseAnzeige();
		timer_1.reset();
		MaterialFuellstand = map(analogRead(pMaterialFuelstand), 0, 1023, 0, 255);
		// Fehleranzeige Materialfüllstand ist gering
		if (MaterialFuellstand <= 50 && HilfsVarFehler == 0)
		{
			HilfsVarFehler = 1;
			t100.setText(cError_text[4]);
			t100.Set_background_color_bco(63978);
		}
		else if (MaterialFuellstand >= 220 && HilfsVarFehler == 1)
		{
			t100.setText("");
			t100.Set_background_color_bco(65535);
			HilfsVarFehler = 0;
		}
   	}

	// PID Heizungsregelungen aufufen
	if (timer_2.isReady())		// 100ms
	{
		getTemp();
		if (bHeizung && iTempSollHeizblock >= 50)
    	{
      		if (iTmpBlck < (iTempSollHeizblock * 0.75))
      		{
			myPID_Blck.SetTunings(aggKp_Blck, aggKi_Blck, aggKd_Blck);
        	// analogWrite(pHeizungHeizblock, 0);
        	// myPID_Blck.clear();
      		}
      		else
      		{
			myPID_Blck.SetTunings(consKp_Blck, consKi_Blck, consKd_Blck);	  
        	// analogWrite(pHeizungHeizblock, myPID_Blck.step(iTempSollHeizblock, iTmpBlck));
      		}
		myPID_Blck.Compute();
		analogWrite(pHeizungHeizblock, 255 - iWertR_Blck);
		}
		if (bHeizung && iTempSollDuese >= 50)
    	{
			if (iTmpDs < (iTempSollDuese * 0.75))		
      		{
			myPID_Ds.SetTunings(aggKp_Ds, aggKi_Ds, aggKd_Ds);
        	// analogWrite(pHeizungDuese, 0);
        	// myPID_Ds.clear();
      		}
      		else
      		{
			myPID_Ds.SetTunings(consKp_Ds, consKi_Ds, consKd_Ds);
        	// analogWrite(pHeizungDuese, myPID_Ds.step(iTempSollDuese, iTmpDs));
      		}
		myPID_Ds.Compute();
		analogWrite(pHeizungDuese, 255 - iWertR_DS);
		}	  
		// Messung des Heizungstroms
		float SensorSpannung = 0;
		int SensorwertHeizung = 0;
		SensorwertHeizung = analogRead(pStromHeizung);
		SensorSpannung = (SensorwertHeizung / 1024.0) * 5000;
		fAmpHeizung = ((SensorSpannung - Nullpunkt) / VpA2);
		
		
		if (fReferenzwertdicke > 0 && bHeizung && bExtruder && bKuehlung)		// Messung der Dicke des Filaments wird sich zeigen ob man 2 braucht
		{
			faWertSensor1 = (analogRead(pDickesensor1) * 0.0049);
			//faWertSensor2 = (analogRead(pDickesensor2) * 0.0049);
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
	for (int i = 1; i <= 6; i++)
	{
		Rezept[x][i] = EEPROM.read(x * 20 - 20 + i);
	}
}

void SpeichernEEprom(int x)		// x = Rezeptnummer
{ 
	for (int i = 1; i <= 6; i++)
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
  Wire.beginTransmission(3);      		// Übertragung an Device #3
  Wire.write(x, 4);           			// Sollwert Vorschub an UNO senden
  Wire.write(iZugmotor);        		// Antriebsfreigabe an UNO senden
  Wire.endTransmission();
  for (int i = 0; i < 4; i++)
  {
    x[i] = 0;
  }
}

void SwitchCaseAnzeige()			  	// Daten zur aktiven Seite senden
{
	switch (iNexpage)
	{
	case 0:							    // Anzeigen übertragen auf Page 0  Home
		n4.setValue(iTmpDs);
		n5.setValue(iTmpBlck);
		n3.setValue(iVExtruder);
		n6.setValue(ivZugmotor);
		break;
	case 1:							    // Anzeigen übertragen auf Page 1  Setup Geschwindigkeit Temperatur
		n17.setValue(iTmpDs);
		n18.setValue(iTmpBlck);
		break;
	case 2:							    // Anzeigen übertragen auf Page 2  Setup Zugmotor
		//dtostrf(faWertSensor1, 0, 1, NextionWert);
		//t26.setText(NextionWert);
		//dtostrf(faWertSensor2, 0, 1, NextionWert);
		//t27.setText(NextionWert);
		//dtostrf(fReferenzwertdicke, 0, 1, NextionWert);
		//t28.setText(NextionWert);
		break;
	case 3:							    // Anzeigen übertragen auf Page 3  Datensatz
		break;
	case 4:							    // Anzeigen übertragen auf Page 4  Stromanzeige und Referenzieren
		dtostrf(fAmpMotor, 3, 1, NextionWert);
		t41.setText(NextionWert);
		dtostrf(fAmpHeizung, 3, 1, NextionWert);
		t42.setText(NextionWert);
		break;
	case 5:							    // Anzeigen übertragen auf Page 5  Osziloskop Strom Temp usw
		iUmrechnungAnzeige = fAmpMotor * 10;  // Mal 10 da der Strom des Motor für die Anzeige zu klein ist
		s0.addValue(0, iUmrechnungAnzeige);
		iUmrechnungAnzeige = fAmpHeizung;
		s0.addValue(1, iUmrechnungAnzeige);
		s0.addValue(2, (iTmpBlck/2));			// geteilt durch 2 da der Graph dann Temperaturen über 200°C nicht mehr darstellen kann
		s0.addValue(3, (iTmpDs/2));
		break;
	case 6:								// Datensätze anzeigen.
		va8.setValue(iEEMaterialart);
		va9.setValue(iEEMaterialfarbe);
		n63.setValue(iEEVExtruder);
		n64.setValue(iEETempSollDuese);
		n65.setValue(iEETempSollHeizblock);
		n66.setValue(iEEVZugmotor);
		break;
	case 7:										// Seite Parameter Aufwicklung

		break;
	}

}

//  Nextion Buttonprogramme
void b0PushCallback(void *ptr)		//  Seitenwechsel nach Page 1 Geschwindigkeit und Temperatureinstellung
{
	iNexpage = 1;
}

void b1PushCallback(void *ptr)		// Seitenwechsel nach Page 3 Datensatz 
{
	iNexpage = 3;
}

void b10PushCallback(void *ptr)		// Seitenwechsel nach Page 0 Home
{
	iNexpage = 0;
}

void b11PushCallback(void *ptr)		// Seitenwechsel nach Page 2 Aufwicklung Setup 
{
	iNexpage = 2;
}

void b12PushCallback(void *ptr)		// Seitenwechsel nach Page 4 Stromanzeige
{
	iNexpage = 4;
}

void b13PushCallback(void *ptr)		// Da die Datenübertragung der Werte
{	
	uint32_t number = 0;
	n13.getValue(&number);			// Abfrage des Wertes Soll Extrudergeschwindigkeit
	iVExtruder = number;			// Übertragen von Temp in Variable
	nanoWrite1();					// Freigabe und Drehzahl an NANO 
	delay(5);
	n14.getValue(&number);			// Abfrage des Wertes Soll Düsentemperatur
	iTempSollDuese = number;		// Übertragen von Temp in Variable
	delay(5);
	n15.getValue(&number);			// Abfrage des Wertes Soll Heizblocktemperatur
	iTempSollHeizblock = number;	// Übertragen von Temp in Variable
	delay(5);
	n16.getValue(&number);
	ivZugmotor = number;
	if (number > 765)
	{
		x[0] = number - 765;
		number = number - x[0];
	}
	if (number > 510)
	{
		x[1] = number - 510;
		number = number - x[1];
	}
	if (number > 255)
	{
		x[2] = number - 255;
		number = number - x[2];
	}
	if (number <= 255)
	{
		x[3] = number;
	}
	nanoWrite2();
}

void b20PushCallback(void *ptr)		// Seitenwechsel nach Page 1  Geschwindigkeit und Temperatureinstellung
{
	iNexpage = 1;
}

void b21PushCallback(void *ptr)		// Seitenwechsel nach Page 5 Osziloskopanzeige Ströme Temperaturen
{
	iNexpage = 7;
}

void b22PushCallback(void *ptr)		// Referenzwerte Dicke Filament speichern
{

	//	fReferenzwertdicke = (((analogRead(pDickesensor1) + analogRead(pDickesensor2)) / 2) * 0.0049);	// Referenzdicke aus Durchschnitt beider Sensoren
	//	dtostrf(fReferenzwertdicke, 5, 3, NextionWert);
	//	t54.setText(NextionWert);

}

void b30PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	iNexpage = 0;
}

void b31PushCallback(void *ptr)		// Button zum Speichern eines Datensatzes Materialart 1= PP 2 = APET 3 = CPET 4 = PETG 5 = PLA 6 = ABS 7 = ASA 8 = POM 
{
	uint32_t number = 0;			// Hier wird der Wert der Variable für die Materialart abgerufen
	va6.getValue(&number);			// Materialart von Nextion lesen
	iMaterialart = number;
	va7.getValue(&number);			// Materialfarbe von Nextion lesen
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
			Rezept[i][6] = ivZugmotor;
			SpeichernEEprom(i);
			break;
		}
	}
	t39.setText("Speichern angeschlossen");
}

void b32PushCallback(void *ptr)		// Seitenwechsel nach Page 6
{
	iNexpage = 6;
}

void b40PushCallback(void *ptr)		// Seitenwechsel nach Page 1
{
	iNexpage = 1;
}

void b41PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	iNexpage = 5;
}

void b42PushCallback(void *ptr)		// Referenz Strom Motor
{
	
}

void b43PushCallback(void *ptr)		// Referenz Strom Heizung
{
	
}

void b50PushCallback(void *ptr)		// Seitenwechsel nach Page 1
{
	iNexpage = 4;
}

void b60PushCallback(void *ptr)		// Seitenwechsel nach Page 3
{
	iNexpage = 3;
}

void b61PushCallback(void *ptr)		// Button zum Laden eines Datensatzes 
{
	iMaterialart = iEEMaterialart;
	iMaterialfarbe = iEEMaterialfarbe;
	iTempSollHeizblock = iEETempSollHeizblock;
	iTempSollDuese = iEETempSollDuese;
	iVExtruder = iEEVExtruder;
	ivZugmotor = iEEVZugmotor;
	n13.setValue(iVExtruder);
	n14.setValue(iTempSollDuese);
	n15.setValue(iTempSollHeizblock);
	n16.setValue(ivZugmotor);
	va6.setValue(iMaterialart);
	va7.setValue(iMaterialfarbe);
	t67.setText("Laden erfolgreich");
}

void b62PushCallback(void *ptr)		// Anwahl des Datensates +1
{
	uint32_t number = 0;
	n60.getValue(&number);
	number++;
	if (number > 20) number = 1;
	i = number;
	n60.setValue(number);

	iEEMaterialart = Rezept[i][1];
	iEEMaterialfarbe = Rezept[i][2];
	iEETempSollHeizblock = Rezept[i][3];
	iEETempSollDuese = Rezept[i][4];
	iEEVExtruder = Rezept[i][5];
	iEEVZugmotor = Rezept[i][6];

	// Daten werden in Case Funktion auf Nextion geschrieben
}

void b63PushCallback(void *ptr)		// Anwahl des Datensates -1
{
	uint32_t number = 0;
	n60.getValue(&number);
	number--;
	if (number < 1) number = 20;
	i = number;
	n60.setValue(number);

	iEEMaterialart = Rezept[i][1];
	iEEMaterialfarbe = Rezept[i][2];
	iEETempSollHeizblock = Rezept[i][3];
	iEETempSollDuese = Rezept[i][4];
	iEEVExtruder = Rezept[i][5];
	iEEVZugmotor = Rezept[i][6];
	// Daten werden in Case Funktion auf Nextion geschrieben
}

void b70PushCallback(void *ptr)		// Seitenwechsel nach Page 3
{
	iNexpage = 2;
}

void bt0PushCallback(void *ptr)   //  Ein/Aus Extruderschnecke
{
	uint32_t number = 0;
	bt0.getValue(&number);
	bExtruder = number;
	if (bExtruder == 1)
	{
    iZugmotor = 1;
	}
	if (bExtruder == 0)
	{
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
		analogWrite(pHeizungDuese, 255);
		analogWrite(pHeizungHeizblock, 255);
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

void n3PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h0 ein
{
	iNexpage = 1;
}

void n4PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h1 ein
{
	iNexpage = 1;
}

void n5PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{
	iNexpage = 1;
}
void n6PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{
  iNexpage = 1;
}
