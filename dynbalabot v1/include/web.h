//importovat spolecne promenne a funkce
#include <SPIFFS.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include "mpu.h"
#include "prijimac.h"
extern float VCC;
extern bool telemetrie;
extern float Kp;
extern float Kd;
extern float Ki;
void odesliTelemetrii(int);
void odesliRychlouTelemetrii(float, float, int);
void setupWS();
void WSloop();