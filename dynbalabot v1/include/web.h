//importovat spolecne promenne a funkce
#include <SPIFFS.h>
#include <Arduino.h>
extern float Kp;
extern float Kd;
extern float Ki;
void setupWS();
void WSloop();