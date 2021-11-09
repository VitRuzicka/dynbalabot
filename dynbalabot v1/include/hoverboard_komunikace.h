#pragma once
#include <Arduino.h>
#define HOVER_SERIAL_BAUD   115200      // [-] Baud rate for HoverSerial (used to communicate with the hoverboard)


void inicializujHB();
void Send(int16_t, int16_t);
void Receive();