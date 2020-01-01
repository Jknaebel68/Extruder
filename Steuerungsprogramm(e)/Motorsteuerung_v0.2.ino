#include<Wire.h>

#define pMotortakt 7
#define pFreigabe 6
#define pRichtung 5
#define pAlarm 1


int iVExtruder = 0;
byte bExtruder = 0;
long premillis = 0;
int iRichtung = 0;
int iRueklaufzeit = 1000;        // Dauer wie lange der Extruder Rückwärts läuft wenn ein Alarm des Servo ansteht
unsigned int uiPause = 2100;   // Delay von 2100 Mikrosekunden -> 1%

void setup() {
  pinMode (pMotortakt, OUTPUT);
  pinMode (pFreigabe, OUTPUT);
  pinMode (pRichtung, OUTPUT);
  pinMode (pAlarm, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pAlarm), Alarm, FALLING);

  digitalWrite(pFreigabe, HIGH);
  digitalWrite(pRichtung, LOW);
  Wire.begin(2);
  Wire.onReceive(get_Motor_Parameter);
}

void loop() {

  if (bExtruder == 1) {
    digitalWrite(pFreigabe, LOW);
    digitalWrite(pMotortakt, HIGH);
    delayMicroseconds(2);
    digitalWrite(pMotortakt, LOW);
    delayMicroseconds(uiPause);
  }
  else {
    digitalWrite(pFreigabe, HIGH);
  }
  if (iRichtung == 1 && bExtruder == 1 && ((millis() - premillis)>= iRueklaufzeit))
  {
   digitalWrite(pRichtung, LOW);
   premillis = 0;
   iRichtung = 0;
  }

}

void get_Motor_Parameter(int howMany) {
  iVExtruder = Wire.read();
  bExtruder = Wire.read();
  uiPause = 2100 / iVExtruder;
}

void Alarm(){
  iRichtung = 1;
  digitalWrite(pRichtung, HIGH);
  premillis = millis();
 }
