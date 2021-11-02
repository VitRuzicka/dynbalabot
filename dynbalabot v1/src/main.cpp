
#include<Arduino.h>
#include "ppm.h"

void setup() {
Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(18), cteni_signalu, FALLING); //ve skutecnosti se jedna o vzestupnou hranu protoze pin je v PULLUP rezimu
}
void loop() {
zpracovani_signalu(); //nutno zavolat pred pouzitim signalu z promenne pro "update udaju"

Serial.print(clip(ch[1], 1500, 0));Serial.print("\t");
Serial.print(ch[2]);Serial.print("\t");
Serial.print(ch[3]);Serial.print("\t");
Serial.print(ch[4]);Serial.print("\t");
Serial.print(ch[5]);Serial.print("\t");
Serial.print(ch[6]);Serial.print("\n");

delay(100);
}