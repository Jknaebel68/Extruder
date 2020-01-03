/*
    Name:     Extrudersteuerung.ino
    Version:  v0.2
    Created:	05.11.2019 20:02
    Author:   Joachim Rüber &	Jörg Knaebel

*/

//  Eingebundene Libarys
#include <Nextion.h>
#include "Nextion_Defs.h"

//  Definitionen für Variablen
int inexpage = 0;            //  Merker welche Seite auf dem Display angezeigt wird
boolean bExtruder = false;  //  Merker Extrudermotor false = aus true = ein
boolean bHeizung = false;   //  Merker Heizung false = aus true = ein
boolean bKuehlung = false;  //  Merker Kühlung false = aus true = ein
int ivExtruder = 0;         //  Soll Geschwindigkeit des Extruders von der max Geschwindigkeit 100% = 1Hz Extruderschnecke 19Hz Extrudermotor
int iTempHeizblock = 0;     //  Soll Temperatur Heizblock
int iTempDuese = 0;         //  Soll Temperatur Düse
int iTempISTHeizblock = 0;  //  Ist Temperatur Heizblock
int iTempISTDuese = 0;      //  Ist Temperatur Düse
float fAMotor = 0.0;        //  Iststrom Motor Extruder über ACS712 gemessen
float fAmpHeizblock = 0.0;  //  Iststrom Heizung Heizblock über ACS712 gemessen
float fAmpDuese = 0.0;      //  Iststrom Heizung Düse über ACS712 gemessen




//  Listen to buttons, sliders, etc.
NexTouch *nex_listen_list[] =
{
  &b0,    //    Button added
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
  &page0,   //  Page added as a touch event
  &page1,  
  &page2,  
  &page3,  
  &page4,  
  &page5,  
  NULL      //  String terminated
};          //  End of touch event list




//  Definitionen für E/A Ebene





//  The setup() function runs once each time the micro-controller starts
void setup()
{
  nexSerial.begin(115200);
  dbSerial.begin(9600);
  nexInit();
  b0.attachPush(b0PushCallback, &b0);     //  Button press
  b1.attachPush(b1PushCallback, &b1);         
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
  h0.attachPop(h0PopCallback, &h0);				//  Geschwindigkeit fuer das Wickeln vorgeben
  h1.attachPop(h1PopCallback, &h1);				//  Anzahl der Windungen eingeben
  h2.attachPop(h1PopCallback, &h2);
}

//  Add the main program code into the continuous loop() function
void loop()
{
  nexLoop(nex_listen_list);

}





//  Nextion Buttonprogramme
void b0PushCallback(void *ptr)    //  Seitenwechsel nach Page 1 Geschwind
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

void bt0PushCallback(void *ptr)   //  Ein/Aus Extruderschnecke
{
  bExtruder = !bExtruder;
}

void bt1PushCallback(void *ptr)    //  Ein/Aus Heizung
{
  bHeizung = !bHeizung;
}

void bt2PushCallback(void *ptr)   //  Ein/Aus Kühlung
{
  bKuehlung = !bKuehlung;
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

  int numberx = 0;
  char temp[10] = { 0 };
  uint32_t number = 0;
  h0.getValue(&number);
  dbSerial.print(number);
  utoa(number, temp, 10);
  numberx = number;
}

void h1PopCallback(void *ptr)     //  Temperatureinstellung Heizblock 0--230°C
{

}

void h2PopCallback(void *ptr)     //  Temperatureinstellung Düse 0--250°C
{

}
