#include<Wire.h>
#define pMotortakt 11
#define pFreigabe 12

int iVExtruder = 0;
bool bExtruder = false;
unsigned int uiPause = 2100;   // Delay von 2100 Mikrosekunden -> 1%

void setup() {
  pinMode (pMotortakt, OUTPUT);
  pinMode (pFreigabe, OUTPUT);
  Wire.begin(2);
  Wire.onReceive(get_Motor_Parameter);
}

void loop() {

  if (bExtruder = true) {
    digitalWrite(pFreigabe, LOW);
    digitalWrite(pMotortakt, HIGH);
    delayMicroseconds(2);
    digitalWrite(pMotortakt, LOW);
    delayMicroseconds(uiPause);
  }
  else {
    digitalWrite(pFreigabe, HIGH);
  }

}

void get_Motor_Parameter(int howMany) {
  iVExtruder = Wire.read();
  bExtruder = Wire.read();
  uiPause = 2100 / iVExtruder;
}
