#pragma once

#include "SBUS.h"
extern uint16_t channels[16];
extern bool failSafe;
extern bool lostFrame;

int clip(int, int,int);