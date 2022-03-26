#pragma once


#define MAX_SPEED 700
#define MAX_ACCEL 500
#define NORM_SPEED 50

#define VYJETI 70 //vyjeti z koncaku o xy kroků
#define CELKKROKU 1000
#define PRACPOLOHA 170
extern bool homed;

void nastav_krokace();
void runKrokace();
void homujKrokace();
void moveKrokac(int);
void movePracovniPoloha();