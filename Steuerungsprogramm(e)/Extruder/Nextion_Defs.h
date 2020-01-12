/*
 *  Name:       Nextion_Defs.h
 *  Version:    v0.11
 *  Created:    21.11.2019  20:32
 *  Author:     Joachim Rüber & Jörg Knaebel
 *
 */



#ifndef NEXTION_DEFS_H
#define NEXTION_DEFS_H

#define pFreigabeHeizung 30     // Versorungsspannung für ExtruderMotortreiber 
#define pHeizungHeizblock 2    // Heizleistung beträgt 200W bei 24V mit 2 P_Mosfet
#define pHeizungDuese 3        // Heizleistung 100W bei 24V 1P_Mosfet
#define pKuehlung 33            // alle Kühlgebläse der Kühlstrecke
#define pReferenz A0            // 5V Referenz Fraglich ob notwendig
#define pStromMotor A1          // Stromsensor ACS712 5A 
#define pStromHeizung A2        // Stromsensor ACS712 20A 
#define pDickesensor1 A3	      // Analogeingang Dickesensor 1
//#define pDickesensor2 A4	      // Analogeingang Dickesensor 2
#define pMaterialFuelstand A4   // Füllstandssensor des Vorratsbehälters

#define CS_PIN_BLCK 34      // CHIP_SELECT Temperaturfühler Block;
#define CS_PIN_DS 35        // CHIP_SELECT Temperaturfühler Düse;


 //  Definition der Anzeigeelemente des Nextion Displays
NexPage page0 = NexPage(0, 0, "page0");			//  Seite Uebersicht
NexPage page1 = NexPage(1, 0, "page1");			//  Setupseite Geschwindigkeit und Temperatur
NexPage page2 = NexPage(2, 0, "page2");			//  Setupseite Dickefilament
NexPage page3 = NexPage(3, 0, "page3");			//  Datensatz speichern
NexPage page4 = NexPage(4, 0, "page4");			//  Anzeige Ströme
NexPage page5 = NexPage(5, 0, "page5");			//	Grafische Anzeige Temperatur Strom usw
NexPage page6 = NexPage(6, 0, "page6");			//	Datensatz laden
NexPage page7 = NexPage(7, 0, "page7");			//	Aufwicklung

//  Button und Anzeigedefinitionen der Page 0
NexButton b0 = NexButton(0, 16, "b0");			//  Seitenwechsel nach Page 1
NexButton b1 = NexButton(0, 17, "b1");			//  Seitenwechsel nach Page 3 Datensatz
NexNumber n3 = NexNumber(0, 18, "n3");			//  Seitenwechsel nach Page 1 und Anzeige Geschwindigkeit Extruder
NexNumber n4 = NexNumber(0, 2, "n4");			//  Seitenwechsel nach Page 1 und Anzeige Temperatur Düse
NexNumber n5 = NexNumber(0, 4, "n5");			//  Seitenwechsel nach Page 1 und Anzeige Temperatur Heizblock
NexNumber n6 = NexNumber(0, 20, "n6");			//  Seitenwechsel nach Page 1 und Anzeige Geschwindigkeit Zugmotor
NexDSButton bt1 = NexDSButton(0, 13, "bt1");	//  Heizung ein/aus
NexDSButton bt0 = NexDSButton(0, 12, "bt0");	//  Extrudermotor ein/aus
NexDSButton bt2 = NexDSButton(0, 14, "bt2");	//  Kühlung ein/aus

//  Button und Anzeigedefinitionen der Page 1
NexButton b10 = NexButton(1, 12, "b10");		//  Seitenwechsel nach Page 0
NexButton b11 = NexButton(1, 14, "b11");		//  Seitenwechsel nach Page 2
NexButton b12 = NexButton(1, 13, "b12");		//  Seitenwechsel nach Page 4
NexButton b13 = NexButton(1, 15, "b13");		//  Button zur Wertübernahme 
NexNumber n13 = NexNumber(1, 2, "n13");			//  Soll/Ist Geschwindigkeit Extruder Eingabe
NexNumber n14 = NexNumber(1, 3, "n14");			//  Temperatur Düse Eingabe
NexNumber n15 = NexNumber(1, 4, "n15");			//  Temperatur Heizblock Eingabe
NexNumber n16 = NexNumber(1, 16, "n16");		//  Soll/Istwert Zugmotor
NexNumber n17 = NexNumber(1, 10, "n17");		//  Anzeige Temperatur Düse
NexNumber n18 = NexNumber(1, 11, "n18");		//  Anzeige Temperatur Heizblock

//  Button und Anzeigedefinitionen der Page 2 Zugeinheit Parameter
NexButton b20 = NexButton(2, 2, "b20");			//  Seitenwechsel nach Page 1
NexButton b21 = NexButton(2, 3, "b21");			//  Seitenwechsel nach Page 7 Aufwicklung
NexButton b22 = NexButton(2, 7, "b22");			//  Referenzwert der Dickmessung eines Referenzstücks Filament übernehmen
NexText t26 = NexText(2, 8, "t26");				//  Anzeige Dicksensor 1
NexText t27 = NexText(2, 9, "t27");				//  Anzeige Dicksensor 2
NexText t28 = NexText(2, 10, "t28");			//  Anzeige Referenzwert
NexVariable va10 = NexVariable(2, 18, "va10");	//  Merker Referenzdicke auf Nextion fraglich

//  Button und Anzeigedefinitionen der Page 3 Datensatz speicher
NexButton b30 = NexButton(3, 2, "b30");			    //  Seitenwechsel nach Page 0
NexButton b31 = NexButton(3, 20, "b31");		    //  speichern des Datensatzes
NexButton b32 = NexButton(3, 22, "b32");		    //  Seitenwechsel nach Page 6 Datensatz laden
NexVariable va6 = NexVariable(3, 22, "va6");		// Anzeige Materialart
NexVariable va7 = NexVariable(3, 33, "va7");		// Anzeige Materialfarbe
NexNumber n33 = NexNumber(3, 3, "n33");			    //  Anzeige Geschwindigkeit Extruder Eingabe
NexNumber n34 = NexNumber(3, 8, "n34");			    //  Anzeige Temperatur Düse
NexNumber n35 = NexNumber(3, 9, "n35");			    //  Anzeige Temperatur Heizblock
NexNumber n36 = NexNumber(3, 36, "n36");		    //  Anzeige Geschwindigkeit Zugmotor
NexText t39 = NexText(3, 34, "t39");                //  Textanzeige Speichern erfolgreich

//  Button und Anzeigedefinitionen der Page 4 Anzeige Heizstrom und Motorstrom
NexButton b40 = NexButton(4, 3, "b40");			    //  Seitenwechsel nach Page 1
NexButton b41 = NexButton(4, 7, "b41");				//  Seitenwechsel nach Page 5 Graph
NexButton b42 = NexButton(4, 4, "b42");			    //  Setbutton für Stromrefwert Motorstrom
NexButton b43 = NexButton(4, 5, "b43");			    //  Setbutton für Stromrefwert Düse
NexText t41 = NexText(4, 8, "t41");					//  Anzeige Motorstrom
NexText t42 = NexText(4, 9, "t42");					//  Anzeige Strom Heizung


//  Button und Anzeigedefinitionen der Page 5 Anzeige Heizstrom und Motorstrom Grafisch
NexButton b50 = NexButton(5, 1, "b50");			    //  Seitenwechsel nach Page 1
NexWaveform s0 = NexWaveform(5, 2, "s0");		    //  Oszianzeige

//  Button und Anzeigedefinitionen der Page 6 Datensatz laden
NexButton b60 = NexButton(6, 11, "b60");		    //  Seitenwechsel nach Page 3
NexButton b61 = NexButton(6, 2, "b61");			    //  laden eines Datensatzes
NexButton b62 = NexButton(6, 14, "b62");		    //  Datensatz anwahl +1
NexButton b63 = NexButton(6, 16, "b63");		    //  Datensatz anwahl -1
NexVariable va8 = NexVariable(6, 17, "va8");		// Variable Materialart
NexVariable va9 = NexVariable(6, 18, "va9");		// Variable Materialfarbe
NexText t68 = NexText(6, 10, "t68");				// Anzeige Materialart
NexText t69 = NexText(6, 13, "t69");				// Anzeige Materialfarbe
NexNumber n60 = NexNumber(6, 15, "n60");		    //  Anzeige Datensatznummer
NexNumber n63 = NexNumber(6, 3, "n63");			    //  Anzeige Geschwindigkeit Extruder Eingabe
NexNumber n64 = NexNumber(6, 8, "n64");			    //  Anzeige Temperatur Düse
NexNumber n65 = NexNumber(6, 9, "n65");			    //  Anzeige Temperatur Heizblock
NexNumber n66 = NexNumber(6, 21, "n66");			//  Anzeige Geschwindigkeit Zugmotor
NexText t67 = NexText(6, 19, "t67");                //  Textanzeige Laden erfolgreich

//  Button und Anzeigedefinitionen der Page 7 Aufwicklung
NexButton b70 = NexButton(7, 1, "b70");			    //  Seitenwechsel nach Page 2 Zugeinheit

// Fehleranzeige
NexText t100 = NexText(0, 25, "t100");

// Tastatur Enter
NexButton b210 = NexButton(0, 4, "b210");

#endif

