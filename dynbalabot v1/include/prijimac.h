#pragma once

#include "SBUS.h"
extern uint16_t channels[16];
extern bool failSafe;
extern bool lostFrame;

uint clip(int, int,int);