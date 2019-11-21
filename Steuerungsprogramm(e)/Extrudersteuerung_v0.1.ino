/*
    Name:       Extrudersteuerung.ino
    Created:	24.10.2019 20:56:14
    Author:     Joachim Rüber
				Jörg Knaebel

	Beschreibung:
	
*/

// Eingebundene Libarys
//


// Definition der Anzeigeelemente des Nextion Displays

#include <NexWaveform.h>
#include <NexVariable.h>
#include <NexTouch.h>
#include <Nextion.h>
#include <NexText.h>
#include <NexSlider.h>
#include <NexProgressBar.h>
#include <NexPage.h>
#include <NexNumber.h>
#include <NexDualStateButton.h>
#include <NexConfig.h>
#include <NexCheckbox.h>
#include <NexButton.h>



NexPage page0 = NexPage(0, 0, "page0");      //Seite Uebersicht
NexPage page1 = NexPage(0, 0, "page1");		// Setupseite Geschwindigkeit und Temperatur
NexPage page2 = NexPage(0, 0, "page2");		// 
NexPage page3 = NexPage(0, 0, "page3");
NexPage page4 = NexPage(0, 0, "page4");
NexPage page5 = NexPage(0, 0, "page5");

// Button und Anzeigedefinitionen der Page 0
NexButton b0 = NexButton(0, 16, "b0");		// Seitenwechsel nach Page 1
NexButton b1 = NexButton(0, 17, "b1");		// Seitenwechsel nach Page 3 Datensatz
NexNumber n0 = NexNumber(0, 2, "n0");		// Seitenwechsel nach Page 1 und Anzeige Geschwindigkeit Extruder
NexNumber n1 = NexNumber(0, 4, "n1");		// Seitenwechsel nach Page 1 und Anzeige Temperatur Heizblock
NexNumber n2 = NexNumber(0, 18, "n2");		// Seitenwechsel nach Page 1 und Anzeige Temperatur Düse
NexDSButton bt0 = NexDSButton(0, 9, "bt0");		// Extrudermotor ein/aus
NexDSButton bt1 = NexDSButton(0, 10, "bt1");		// Heizung ein/aus
NexDSButton bt2 = NexDSButton(0, 11, "bt2");		// Kühlung ein/aus

// Button und Anzeigedefinitionen der Page 1

NexButton b2 = NexButton(1, 17, "b2");		// Seitenwechsel nach Page 0
NexButton b3 = NexButton(1, 21, "b3");		// Seitenwechsel nach Page 2 
NexButton b4 = NexButton(1, 22, "b4");		// Seitenwechsel nach Page 4
NexNumber n3 = NexNumber(1, 2, "n3");		// Geschwindigkeit Extruder Eingabe
NexNumber n4 = NexNumber(1, 3, "n4");		// Temperatur Heizblock Eingabe
NexNumber n5 = NexNumber(1, 4, "n5");		// Temperatur Düse Eingabe
NexNumber n6 = NexNumber(1, 2, "n6");		// Anzeige Geschwindigkeit Extruder
NexNumber n7 = NexNumber(1, 3, "n7");		// Anzeige Temperatur Heizblock
NexNumber n8 = NexNumber(1, 4, "n8");		// Anzeige Temperatur Düse
NexSlider h0 = NexSlider(1, 18, "h0");		// Einstellung Extrudergeschwindigkeit
NexSlider h1 = NexSlider(1, 19, "h1");		// Einstellung Temperatur Heizblock
NexSlider h2 = NexSlider(1, 20, "h2");		// Einstellung Temperatur Düse

// Button und Anzeigedefinitionen der Page 2 Datensatz

// Button und Anzeigedefinitionen der Page 3 Aufwicklungsparameter

// Button und Anzeigedefinitionen der Page 4 Anzeige Heizstrom und Motorstrom

NexButton b5 = NexButton(4, 7, "b5");		// Seitenwechsel nach Page 1
NexButton b6 = NexButton(4, 8, "b6");		// Setbutton für Stromrefwert Motorstrom
NexButton b7 = NexButton(4, 9, "b7");		// Setbutton für Stromrefwert Heizblock
NexButton b8 = NexButton(4, 10, "b8");		// Setbutton für Stromrefwert Düse
NexButton b9 = NexButton(4, 12, "b9");		// Seitenwechsel nach Page 5 Graph

// Button und Anzeigedefinitionen der Page 5 Anzeige Heizstrom und Motorstrom Grafisch

NexButton b10 = NexButton(5, 1, "b10");		// Seitenwechsel nach Page 1
NexWaveform s0 = NexWaveform(5, 2, "s0");	// Oszianzeige



NexTouch *nex_listen_list[] =
{
  &b0,  // Button added
  &b1,  // Button added
  &b2,  // Button added
  &b3,  // Button added
  &b4,  // Button added
  &b5,  // Button added
  &b6,  // Button added
  &b7,  // Button added
  &b8,  // Button added
  &b9,  // Button added
  &b10,  // Button added
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
  &page0,  // Page added as a touch event
  &page1,  // Page added as a touch event
  &page2,  // Page added as a touch event
  &page3,  // Page added as a touch event
  &page4,  // Page added as a touch event
  &page5,  // Page added as a touch event
	NULL  // String terminated
};  // End of touch event list


// Definitionen für Variablen
	
int inexpage = 0;				// Merker welche Seite auf dem Display angezeigt wird
boolean bExtruder = false;		// Merker Extrudermotor false = aus true = ein
boolean bHeizung = false;		// Merker Heizung false = aus true = ein
boolean bKuehlung = false;		// Merker Kühlung false = aus true = ein
int ivExtruder = 0;				// Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iTempHeizblock = 0;			// Soll Temperatur Heizblock
int iTempDuese = 0;				// Soll Temperatur Düse
int iTempISTHeizblock = 0;		// Ist Temperatur Heizblock
int iTempISTDuese = 0;			// Ist Temperatur Düse
float fAMotor = 0.0;			// Iststrom Motor Extruder über ACS712 gemessen
float fAmpHeizblock = 0.0;		// Iststrom Heizung Heizblock über ACS712 gemessen
float fAmpDuese = 0.0;			// Iststrom Heizung Düse über ACS712 gemessen

// Definitionen für E/A Ebene




// Nextion Buttonprogramme
void b0PushCallback(void *ptr)		// Seitenwechsel nach Page 1 Geschwind
{
	inexpage = 1;
}
void b1PushCallback(void *ptr)
{

}
void b2PushCallback(void *ptr)
{

}
void b3PushCallback(void *ptr)
{

}
void b4PushCallback(void *ptr)
{

}
void b5PushCallback(void *ptr)
{

}
void b6PushCallback(void *ptr)
{

}
void b7PushCallback(void *ptr)
{

}
void b8PushCallback(void *ptr)
{

}
void b9PushCallback(void *ptr)
{

}
void b10PushCallback(void *ptr)
{

}
void bt0PushCallback(void *ptr)		// Ein/Aus Extruderschnecke
{
	bExtruder = !bExtruder;
}
void bt1PushCallback(void *ptr)		// Ein/Aus Heizung
{
	bHeizung = !bHeizung;
}
void bt2PushCallback(void *ptr)		// Ein/Aus Kühlung
{
	bKuehlung = !bKuehlung;
}
void n0PushCallback(void *ptr)		//Betätigung des grünen Solltempfeldes blendet den Schieberegler h0 ein
{

}
void n1PushCallback(void *ptr)		// Betätigung des grünen Solltempfeldes blendet den Schieberegler h1 ein
{

}
void n2PushCallback(void *ptr)		// Betätigung des grünen Solltempfeldes blendet den Schieberegler h2 ein
{

}
void h0PopCallback(void *ptr)	//Geschwindigkeitseinstellung Extruderschnecke 0--100%
{

		int numberx = 0;
		char temp[10] = { 0 };
		uint32_t number = 0;
		h0.getValue(&number);
		dbSerialPrint(number);
		utoa(number, temp, 10);
		numberx = number;
}
void h1PopCallback(void *ptr)	// Temperatureinstellung Heizblock 0--230°C
{

}
void h2PopCallback(void *ptr)	// Temperatureinstellung Düse 0--250°C
{

}
// The setup() function runs once each time the micro-controller starts
void setup()
{
	nexSerial.begin(115200);
	dbSerialBegin(9600);
	nexInit();
	b0.attachPush(b0PushCallback, &b0);            // Button press
	b1.attachPush(b1PushCallback, &b1);           // Button press
	b2.attachPush(b2PushCallback, &b2);
	b3.attachPush(b2PushCallback, &b3);
	b4.attachPush(b2PushCallback, &b4);
	b5.attachPush(b2PushCallback, &b5);
	b6.attachPush(b2PushCallback, &b6);
	b7.attachPush(b2PushCallback, &b7);
	b8.attachPush(b2PushCallback, &b8);
	b9.attachPush(b2PushCallback, &b9);
	b10.attachPush(b2PushCallback, &b10);

	bt0.attachPush(bt0PushCallback, &bt0);
	bt1.attachPush(bt0PushCallback, &bt1);
	bt2.attachPush(bt0PushCallback, &bt2);

	n0.attachPush(n0PushCallback, &n0);
	n1.attachPush(n0PushCallback, &n1);
	n1.attachPush(n0PushCallback, &n1);

	h0.attachPop(h0PopCallback, &h0);				// Geschwindigkeit f�r das Wickeln vorgeben
	h1.attachPop(h1PopCallback, &h1);				// Anzahl der Windungen eingeben
	h2.attachPop(h1PopCallback, &h2);


}

// Add the main program code into the continuous loop() function
void loop()
{
	nexLoop(nex_listen_list);

}
