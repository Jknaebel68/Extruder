/*
 *  Name:       Nextion_Defs.h
 *  Version:    v0.10
 *  Created:    21.11.2019  20:32
 *  Author:     Joachim Rüber & Jörg Knaebel
 *
 */



#ifndef NEXTION_DEFS_H
#define NEXTION_DEFS_H

#define pMaterialFuelstand 34   // Füllstandssensor des Vorratsbehälters
#define pFreigabeHeizung 30     // Versorungsspannung für ExtruderMotortreiber 
#define pHeizungHeizblock 31    // Heizleistung beträgt 200W bei 24V mit 2 P_Mosfet
#define pHeizungDuese 32        // Heizleistung 100W bei 24V 1P_Mosfet
#define pKuehlung 33            // alle Kühlgebläse der Kühlstrecke
#define pReferenz A0            // 5V Referenz Fraglich ob notwendig
#define pStromMotor A1          // Stromsensor ACS712 5A 
#define pStromHeizung A2        // Stromsensor ACS712 20A 
#define pDickesensor1 A3	      // Analogeingang Dickesensor 1
#define pDickesensor2 A4	      // Analogeingang Dickesensor 2

#define CS_PIN_BLCK 34      // CHIP_SELECT Temperaturfühler Block;
#define CS_PIN_DS 35        // CHIP_SELECT Temperaturfühler Düse;


 //  Definition der Anzeigeelemente des Nextion Displays
NexPage page0 = NexPage(0, 0, "page0");			//  Seite Uebersicht
NexPage page1 = NexPage(1, 0, "page1");			//  Setupseite Geschwindigkeit und Temperatur
NexPage page2 = NexPage(2, 0, "page2");			//  Setupseite Zugeinheit Dickefilament
NexPage page3 = NexPage(3, 0, "page3");			//  Datensatz speichern
NexPage page4 = NexPage(4, 0, "page4");			//  Anzeige Ströme
NexPage page5 = NexPage(5, 0, "page5");			//	Grafische Anzeige Temperatur Strom usw
NexPage page6 = NexPage(6, 0, "page6");			//	Datensatz laden
NexPage page7 = NexPage(7, 0, "page7");			//	Aufwicklung

//  Button und Anzeigedefinitionen der Page 0
NexButton b0 = NexButton(0, 16, "b0");			    //  Seitenwechsel nach Page 1
NexButton b1 = NexButton(0, 17, "b1");			    //  Seitenwechsel nach Page 3 Datensatz
NexNumber n0 = NexNumber(0, 2, "n0");			      //  Seitenwechsel nach Page 1 und Anzeige Temperatur Düse
NexNumber n1 = NexNumber(0, 4, "n1");			      //  Seitenwechsel nach Page 1 und Anzeige Temperatur Heizblock
NexNumber n2 = NexNumber(0, 18, "n2");			    //  Seitenwechsel nach Page 1 und Anzeige Geschwindigkeit Extruder
NexNumber n30 = NexNumber(0, 23, "n30");         //  Seitenwechsel nach Page 2 und Anzeige Geschwindigkeit Zugmotor
NexDSButton bt1 = NexDSButton(0, 10, "bt1");    //  Heizung ein/aus
NexDSButton bt0 = NexDSButton(0, 9, "bt0");		  //  Extrudermotor ein/aus
NexDSButton bt2 = NexDSButton(0, 11, "bt2");	  //  Kühlung ein/aus

//  Button und Anzeigedefinitionen der Page 1
NexButton b2 = NexButton(1, 13, "b2");			    //  Seitenwechsel nach Page 0
NexButton b3 = NexButton(1, 18, "b3");			    //  Seitenwechsel nach Page 2
NexButton b4 = NexButton(1, 17, "b4");			    //  Seitenwechsel nach Page 4
NexButton b13 = NexButton(1, 22, "b13");		    //  Button zur Wertübernahme ---Testweise----
NexNumber n3 = NexNumber(1, 2, "n3");			      //  Geschwindigkeit Extruder Eingabe
NexNumber n4 = NexNumber(1, 3, "n4");			      //  Temperatur Heizblock Eingabe
NexNumber n5 = NexNumber(1, 4, "n5");			      //  Temperatur Düse Eingabe
NexNumber n6 = NexNumber(1, 10, "n6");			    //  Anzeige Geschwindigkeit Extruder
NexNumber n7 = NexNumber(1, 11, "n7");			    //  Anzeige Temperatur Düse
NexNumber n8 = NexNumber(1, 12, "n8");			    //  Anzeige Temperatur Heizblock
NexSlider h0 = NexSlider(1, 18, "h0");			    //  Einstellung Extrudergeschwindigkeit
NexSlider h1 = NexSlider(1, 19, "h1");			    //  Einstellung Temperatur Düse
NexSlider h2 = NexSlider(1, 20, "h2");			    //  Einstellung Temperatur Heizblock
NexVariable va3 = NexVariable(3, 19, "va3");	  //	Variable Geschwindigkeit Extruder
NexVariable va4 = NexVariable(3, 20, "va4");	  //  Variable SollTemperatur Düse
NexVariable va5 = NexVariable(3, 21, "va5");	  //  Variable SollTemperatur Heizblock

//  Button und Anzeigedefinitionen der Page 2 Zugeinheit Parameter
NexButton b11 = NexButton(2, 2, "b11");			    //  Seitenwechsel nach Page 1
NexButton b20 = NexButton(2, 3, "b20");			    //  Seitenwechsel nach Page 7 Aufwicklung
NexButton b27 = NexButton(2, 19, "b27");			//  Geschwindigkeit Zugmotor übernehmen
NexButton b23 = NexButton(2, 11, "b23");			//  Referenzwert der Dickmessung eines Referenzstücks Filament übernehmen
NexNumber n16 = NexNumber(2, 15, "n16");			//	Drehzahl Zugmotor in mm/min
NexText t52 = NexText(2, 14, "t52");				//  Anzeige Dicksensor 1
NexText t53 = NexText(2, 15, "t53");				//  Anzeige Dicksensor 2
NexText t54 = NexText(2, 16, "t54");				//  Anzeige Referenzwert
NexVariable va10 = NexVariable(2, 18, "va10");

//  Button und Anzeigedefinitionen der Page 3 Datensatz speicher
NexButton b12 = NexButton(3, 2, "b12");			    //  Seitenwechsel nach Page 0
NexButton b14 = NexButton(3, 20, "b14");		    //  speichern des Datensatzes
NexVariable va6 = NexVariable(3, 22, "va6");		// Anzeige Materialart
NexVariable va7 = NexVariable(3, 33, "va7");		// Anzeige Materialfarbe
NexNumber n9 = NexNumber(3, 3, "n9");			    //  Anzeige Geschwindigkeit Extruder Eingabe
NexNumber n10 = NexNumber(3, 8, "n10");			    //  Anzeige Temperatur Düse
NexNumber n11 = NexNumber(3, 9, "n11");			    //  Anzeige Temperatur Heizblock
NexButton b19 = NexButton(3, 22, "b19");		    //  Seitenwechsel nach Page 6 Datensatz laden
NexText t10 = NexText(3, 34, "t10");                //  Textanzeige Speichern erfolgreich

//  Button und Anzeigedefinitionen der Page 4 Anzeige Heizstrom und Motorstrom
NexButton b5 = NexButton(4, 4, "b5");			    //  Seitenwechsel nach Page 1
NexButton b6 = NexButton(4, 8, "b6");			    //  Setbutton für Stromrefwert Motorstrom
NexButton b7 = NexButton(4, 9, "b7");			    //  Setbutton für Stromrefwert Düse
NexButton b8 = NexButton(4, 10, "b8");			    //  Setbutton für Stromrefwert Heizblock
NexButton b9 = NexButton(4, 12, "b9");			    //  Seitenwechsel nach Page 5 Graph
NexText t40 = NexText(4, 10, "t40");				//  Anzeige Motorstrom
NexText t41 = NexText(4, 11, "t41");				//  Anzeige Strom Heizung


//  Button und Anzeigedefinitionen der Page 5 Anzeige Heizstrom und Motorstrom Grafisch
NexButton b10 = NexButton(5, 1, "b10");			    //  Seitenwechsel nach Page 1
NexWaveform s0 = NexWaveform(5, 2, "s0");		    //  Oszianzeige

//  Button und Anzeigedefinitionen der Page 6 Datensatz laden
NexButton b15 = NexButton(6, 2, "b15");			    //  laden eines Datensatzes
NexButton b16 = NexButton(6, 14, "b16");		    //  Datensatz anwahl +1
NexButton b17 = NexButton(6, 16, "b17");		    //  Datensatz anwahl -1
NexButton b18 = NexButton(6, 11, "b18");		    //  Seitenwechsel nach Page 3
NexVariable va8 = NexVariable(6, 17, "va8");    // Variable Materialart
NexVariable va9 = NexVariable(6, 18, "va9");    // Variable Materialfarbe
NexText t8 = NexText(6, 10, "t8");				      // Anzeige Materialart
NexText t9 = NexText(6, 13, "t9");				      // Anzeige Materialfarbe
NexNumber n12 = NexNumber(6, 15, "n12");		    //  Anzeige Datensatznummer
NexNumber n13 = NexNumber(6, 3, "n13");			    //  Anzeige Geschwindigkeit Extruder Eingabe
NexNumber n14 = NexNumber(6, 8, "n14");			    //  Anzeige Temperatur Düse
NexNumber n15 = NexNumber(6, 9, "n15");			    //  Anzeige Temperatur Heizblock
NexText t11 = NexText(6, 19, "t11");                //  Textanzeige Laden erfolgreich

//  Button und Anzeigedefinitionen der Page 7 Aufwicklung
NexButton b24 = NexButton(7, 1, "b24");			    //  Seitenwechsel nach Page 2 Zugeinheit

#endif
