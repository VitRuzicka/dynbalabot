#include "Wire.h" //definovano zde jelikoz je pro main i pro mpu_magic
#pragma once
extern volatile bool PID;
extern volatile float soucasnyUhel, predUhel, error, predErr, soucetErr;


void onTime();
void dmpDataReady();
void konfiguruj_gyro();
void nactiGyro();