#include "Wire.h" //definovano zde jelikoz je pro main i pro mpu_magic
#pragma once
#define M_PI 3.141459
extern uint8_t devStatus;  
extern volatile bool PID;
extern volatile float soucasnyUhel, predUhel, error, predErr, soucetErr;

extern uint pulzy;
extern float ypr[]; 

void onTime();
void dmpDataReady();
void konfiguruj_gyro();
void nactiGyro();