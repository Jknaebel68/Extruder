/*
    Name:     Extrudersteuerung.ino
    Version:  v0.3
    Created:	10.11.2019 17:50
    Author:   Joachim Rüber &	Jörg Knaebel

*/

//  Eingebundene Libarys
#include "Nextion.h"
#include "Nextion_Defs.h"

//  Definitionen für Variablen
int inexpage = 0;					//  Merker welche Seite auf dem Display angezeigt wird, damit nur Daten der aufgerufenen Seite übertragen werden
boolean bExtruder = false;			//  Merker Extrudermotor false = aus true = ein
boolean bHeizung = false;			//  Merker Heizung false = aus true = ein
boolean bKuehlung = false;			//  Merker Kühlung false = aus true = ein
int ivExtruder = 0;					//  Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iTempSollHeizblock = 0;			//  Soll Temperatur Heizblock
int iTempSollDuese = 0;				//  Soll Temperatur Düse
int iTempISTHeizblock = 0;		//  Ist Temperatur Heizblock
int iTempISTDuese = 0;			//  Ist Temperatur Düse
long lAMotor = 0;					//  Iststrom Motor Extruder über ACS712 gemessen Berechnung nicht als float Nextion setzt das komma deshalb Wert mal 10 als long
long lAmpHeizblock = 0;				//  Iststrom Heizung Heizblock über ACS712 gemessen
long lAmpDuese = 0;					//  Iststrom Heizung Düse über ACS712 gemessen
byte NexT[3] = { 255, 255, 255 };

// Zeitsteuerung
long lpremillis = 0;
long lpremillis1 = 0;
int iVerz100ms = 100;
int iVerz0_5sec = 500;
int iVerz15min = 15;
int iZaehler1 = 0;
int iZaehler2 = 0;
int iZaehler3 = 0;

boolean bTaktsec = false;
boolean bTaktmin = false;


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
  &bt0,
  &bt1,
  &bt2,
  &n0,
  &n1,
  &n2,
  &n3,
  &n4,
  &n5,
  &h0,
  &h1,
  &h2,
  &page0,
  &page1,
  &page2,
  &page3,
  &page4,
  &page5,
  NULL      //  String terminated
};          //  End of touch event list




//  Definitionen für E/A Ebene
// Digital Input
int dIMaterialFuelstand = 20;	// Füllstandssensor des Vorratsbehälters

// Digital Output
int dOMotorExtruder = 30;		// Versorungsspannung für ExtruderMotortreiber 
int dOHeizungHeizblock = 31;	// Heizleistung beträgt 200W bei 24V mit 2 P_Mosfet
int dOHeizungDuese = 32;		// Heizleistung 100W bei 24V 1P_Mosfet
int dOKuehlung = 33;			// alle Kühlgebläse der Kühlstrecke

// Analog Input
int aIReferenz = A0;			// 5V Referenz Fraglich ob notwendig
int aIStromMotor = A1;			// Stromsensor ACS712 5A 
int aIStromHeizung = A2;		// Stromsensor ACS712 30A 



//  The setup() function runs once each time the micro-controller starts
void setup()
{
  //Serial.begin(9600);
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
  /*------------------------------------
	  Platzhalter für weitere Number anzeigen mit Touchfunktion
  ------------------------------------*/
  h0.attachPop(h0PopCallback, &h0);				//  Geschwindigkeit Motor Extruder 0-100%
  h1.attachPop(h1PopCallback, &h1);				//  Temperatur Heizblock 0-230°c
  h2.attachPop(h2PopCallback, &h2);				//  Temperatur Düse 0-250°C
  /*------------------------------------
	  PinModeeingaben
  ------------------------------------*/


  /*------------------------------------
	Vordefinieren von Eingängen und Ausgängen
------------------------------------*/
  Serial.println("Setup done");
}

//  Add the main program code into the continuous loop() function
void loop()
{
  nexLoop(nex_listen_list);

  // Taktsignal das nur ein Zyklus aktiv
  if ((millis() - lpremillis) >= iVerz0_5sec)
  {
	  bTaktsec = true;
	  lpremillis = millis(); 
	  SwitchCaseAnzeige();			// Hier werden die Werte auf dem Display aktualisiert dies geschieht in einem Zyklus
  }
  else
  {
	  bTaktsec = false;
  }


  // Anzeigesteuerung damit nur Daten zur angezeigten Seite gesendet werden

}


void SwitchCaseAnzeige()
{
	switch (inexpage)
	{
	case 0:
		// Anzeigen übertragen auf Page 0  Home
		n0.setValue(iTempISTDuese);
		n1.setValue(iTempISTHeizblock);
		n2.setValue(ivExtruder);
		break;
	case 1:
		// Anzeigen übertragen auf Page 1  Setup Geschwindigkeit Temperatur
		n6.setValue(ivExtruder);
		n7.setValue(iTempISTDuese);
		n8.setValue(iTempISTHeizblock);
		break;
	case 2:
		// Anzeigen übertragen auf Page 2  Setup Aufwicklung
		break;
	case 3:
		// Anzeigen übertragen auf Page 3  Datensatz
		break;
	case 4:
		// Anzeigen übertragen auf Page 4  Stromanzeige und Referenzieren 
		// Es gibt in der Libary keine .h für die Float anzeige, deshalb dies Methode 
		nexSerial.print("x0.val=\"");
		nexSerial.print(lAMotor);
		nexSerial.write('"');
		nexSerial.write(NexT, 3);
		nexSerial.print("x1.val=\"");
		nexSerial.print(lAmpDuese);
		nexSerial.write('"');
		nexSerial.write(NexT, 3);
		nexSerial.print("x2.val=\"");
		nexSerial.print(lAmpHeizblock);
		nexSerial.write('"');
		nexSerial.write(NexT, 3);
		break;
	case 5:
		// Anzeigen übertragen auf Page 5  Osziloskop Strom Temp usw
		s0.addValue(0, iTempISTHeizblock);
		break;


	}

}

//  Nextion Buttonprogramme
void b0PushCallback(void *ptr)    //  Seitenwechsel nach Page 1 Geschwindigkeit und Temperatureinstellung
{
	inexpage = 1;
}

void b1PushCallback(void *ptr)		// Seitenwechsel nach Page 3 Datensatz ---UNDER CONSTRUTION---
{
	inexpage = 3;
}

void b2PushCallback(void *ptr)		// Seitenwechsel nach Page 0 Home
{
	inexpage = 0;
}

void b3PushCallback(void *ptr)		// Seitenwechsel nach Page 2 Aufwicklung Setup ---UNDER CONSTRUTION---
{
	inexpage = 2;
}

void b4PushCallback(void *ptr)		// Seitenwechsel nach Page 4 Stromanzeige
{
	inexpage = 4;
}

void b5PushCallback(void *ptr)		// Seitenwechsel nach Page 1  Geschwindigkeit und Temperatureinstellung
{
	inexpage = 1;
}

void b6PushCallback(void *ptr)		// Übernahme Referenzwert Motorstrom
{
	
}

void b7PushCallback(void *ptr)		// Übernahme Referenzwert Strom Heizung Düse
{

}

void b8PushCallback(void *ptr)		// Übernahme Referenzwert Strom Heizung Heizblock
{

}

void b9PushCallback(void *ptr)		// Seitenwechsel nach Page 5 Osziloskopanzeige Ströme Temperaturen
{
	inexpage = 5;
}

void b10PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	inexpage = 4;
}

void b11PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	inexpage = 1;
}

void b12PushCallback(void *ptr)		// Seitenwechsel nach Page
{
	inexpage = 0;
}

void b13PushCallback(void *ptr)		// Da die Datenübertragung der Werte mit den Slidern nicht funktioniert hat, wurde der ENTER Button Umweg genutzt
{
	uint32_t number = 0;
	n3.getValue(&number);			// Abfrage des Wertes Soll Extrudergeschwindigkeit
	ivExtruder = number;			// Übertragen von Temp in Variable
	delay(5);					
	n4.getValue(&number);			// Abfrage des Wertes Soll Düsentemperatur
	iTempSollDuese = number;		// Übertragen von Temp in Variable
	delay(5);
	n5.getValue(&number);			// Abfrage des Wertes Soll Heizblocktemperatur
	iTempSollHeizblock = number;	// Übertragen von Temp in Variable

	//Serial.println("Werte übernommen");	// Anzeige zum Debuggen
	//Serial.println(ivExtruder);
	//Serial.println(iTempSollDuese);
	//Serial.println(iTempSollHeizblock);
}

void bt0PushCallback(void *ptr)   //  Ein/Aus Extruderschnecke
{
	uint32_t number = 0;
	bt0.getValue(&number);
	if (number == 0)
	{
		bExtruder = false;
	}
	else if (number == 1)
	{
		bExtruder = true;
	}

 /* if (bExtruder)
  {
	  Serial.println("Extruder ein");
  }
  else
  {
	  Serial.println("Extruder aus");
  }*/
}

void bt1PushCallback(void *ptr)    //  Ein/Aus Heizung
{
	uint32_t number = 0;
	bt1.getValue(&number);
	if (number == 0)
	{
		bHeizung = false;
	}
	else if (number == 1)
	{
		bHeizung = true;
	}
	/*if (bHeizung)
	{
		Serial.println("Heizung ein");
	}
	else
	{
		Serial.println("Heizung aus");
	}*/
}

  void bt2PushCallback(void *ptr)   //  Ein/Aus Kühlung
  {
	  uint32_t number = 0;
	  bt2.getValue(&number);
	  if (number == 0)
	  {
		  bKuehlung = false;
	  }
	  else if (number == 1)
	  {
		  bKuehlung = true;
	  }
	  if (bKuehlung)
	  {
		  Serial.println("Kühlung ein");
	  }
	  else
	  {
		  Serial.println("Kühlung aus");
	  }
  }

void n0PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h0 ein
{

}

void n1PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h1 ein
{

}

void n2PushCallback(void *ptr)    //  Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{

}

void h0PopCallback(void *ptr)     //  Geschwindigkeitseinstellung Extruderschnecke 0--100%
{


}

void h1PopCallback(void *ptr)     //  Temperatureinstellung Heizung Düse 0 -- 250°C
{

}

void h2PopCallback(void *ptr)     //  Temperatureinstellung Heizung Heizblock 0--230°C
{

}
