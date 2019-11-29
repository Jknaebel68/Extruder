/*
 *  Name:       Nextion_Defs.h
 *  Version:    v0.3
 *  Created:    21.11.2019  20:32
 *  Author:     Joachim Rüber & Jörg Knaebel
 * 
 */



#ifndef NEXTION_DEFS_H
#define NEXTION_DEFS_H

#define pMaterialFuelstand 34  // Füllstandssensor des Vorratsbehälters
#define pMotorExtruder 30   // Versorungsspannung für ExtruderMotortreiber 
#define pHeizungHeizblock 31  // Heizleistung beträgt 200W bei 24V mit 2 P_Mosfet
#define pHeizungDuese 32    // Heizleistung 100W bei 24V 1P_Mosfet
#define pKuehlung 33      // alle Kühlgebläse der Kühlstrecke
#define pReferenz A0      // 5V Referenz Fraglich ob notwendig
#define pStromMotor A1      // Stromsensor ACS712 5A 
#define pStromHeizung A2    // Stromsensor ACS712 30A 

#define CS_PIN_BLCK 35      // CHIP_SELECT Temperaturfühler Block
#define CS_PIN_DS 36        // CHIP_SELECT Temperaturfühler Düse


 //  Definition der Anzeigeelemente des Nextion Displays
NexPage page0 = NexPage(0, 0, "page0");			//  Seite Uebersicht
NexPage page1 = NexPage(1, 0, "page1");			//  Setupseite Geschwindigkeit und Temperatur
NexPage page2 = NexPage(2, 0, "page2");			//  Setupseite Aufwicklung
NexPage page3 = NexPage(3, 0, "page3");			//  Datensatz speichern
NexPage page4 = NexPage(4, 0, "page4");			//  Anzeige Ströme
NexPage page5 = NexPage(5, 0, "page5");			//	Grafische Anzeige Temperatur Strom usw

//  Button und Anzeigedefinitionen der Page 0
NexButton b0 = NexButton(0, 16, "b0");			//  Seitenwechsel nach Page 1
NexButton b1 = NexButton(0, 17, "b1");			//  Seitenwechsel nach Page 3 Datensatz
NexNumber n0 = NexNumber(0, 2, "n0");			//  Seitenwechsel nach Page 1 und Anzeige Temperatur Düse
NexNumber n1 = NexNumber(0, 4, "n1");			//  Seitenwechsel nach Page 1 und Anzeige Temperatur Heizblock
NexNumber n2 = NexNumber(0, 18, "n2");			//  Seitenwechsel nach Page 1 und Anzeige Geschwindigkeit Extruder
NexDSButton bt0 = NexDSButton(0, 9, "bt0");		//  Extrudermotor ein/aus
NexDSButton bt1 = NexDSButton(0, 10, "bt1");	//  Heizung ein/aus
NexDSButton bt2 = NexDSButton(0, 11, "bt2");	//  Kühlung ein/aus

//  Button und Anzeigedefinitionen der Page 1
NexButton b2 = NexButton(1, 13, "b2");			//  Seitenwechsel nach Page 0
NexButton b3 = NexButton(1, 18, "b3");			//  Seitenwechsel nach Page 2
NexButton b4 = NexButton(1, 17, "b4");			//  Seitenwechsel nach Page 4
NexButton b13 = NexButton(1, 22, "b13");		//  Button zur Wertübernahme ---Testweise----
NexNumber n3 = NexNumber(1, 2, "n3");			//  Geschwindigkeit Extruder Eingabe
NexNumber n4 = NexNumber(1, 3, "n4");			//  Temperatur Heizblock Eingabe
NexNumber n5 = NexNumber(1, 4, "n5");			//  Temperatur Düse Eingabe
NexNumber n6 = NexNumber(1, 10, "n6");			//  Anzeige Geschwindigkeit Extruder
NexNumber n7 = NexNumber(1, 11, "n7");			//  Anzeige Temperatur Düse
NexNumber n8 = NexNumber(1, 12, "n8");			//  Anzeige Temperatur Heizblock
NexSlider h0 = NexSlider(1, 18, "h0");			//  Einstellung Extrudergeschwindigkeit
NexSlider h1 = NexSlider(1, 19, "h1");			//  Einstellung Temperatur Düse
NexSlider h2 = NexSlider(1, 20, "h2");			//  Einstellung Temperatur Heizblock


//  Button und Anzeigedefinitionen der Page 2 Aufwicklungsparameter
NexButton b11 = NexButton(2, 2, "b11");			//  Seitenwechsel nach Page 1

//  Button und Anzeigedefinitionen der Page 3 Datensatz
NexButton b12 = NexButton(3, 2, "b12");			//  Seitenwechsel nach Page 0
NexButton b14 = NexButton(3, 20, "b14");			//  speichern des Datensatzes
NexButton b15 = NexButton(3, 21, "b15");			//  laden eines Datensatzes
NexVariable va6 = NexVariable(3, 22, "va6");        // Anzeige Materialart
NexNumber n9 = NexNumber(3, 3, "n9");			//  Anzeige Geschwindigkeit Extruder Eingabe
NexNumber n10 = NexNumber(3, 8, "n10");			//  Anzeige Temperatur Düse
NexNumber n11 = NexNumber(3, 9, "n11");			//  Anzeige Temperatur Heizblock

//  Button und Anzeigedefinitionen der Page 4 Anzeige Heizstrom und Motorstrom
NexButton b5 = NexButton(4, 7, "b5");			//  Seitenwechsel nach Page 1
NexButton b6 = NexButton(4, 8, "b6");			//  Setbutton für Stromrefwert Motorstrom
NexButton b7 = NexButton(4, 9, "b7");			//  Setbutton für Stromrefwert Düse
NexButton b8 = NexButton(4, 10, "b8");			//  Setbutton für Stromrefwert Heizblock
NexButton b9 = NexButton(4, 12, "b9");			//  Seitenwechsel nach Page 5 Graph


//  Button und Anzeigedefinitionen der Page 5 Anzeige Heizstrom und Motorstrom Grafisch
NexButton b10 = NexButton(5, 1, "b10");			//  Seitenwechsel nach Page 1
NexWaveform s0 = NexWaveform(5, 2, "s0");		//  Oszianzeige
#endif
