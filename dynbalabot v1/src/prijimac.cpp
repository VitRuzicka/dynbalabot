
#include<Arduino.h>
#include"prijimac.h"
uint16_t channels[16];
bool failSafe;
bool lostFrame;

int clip(int vstup, int max, int min){
  if(vstup > max){
    return max;
  }
  else if(vstup < min){
    return min;
  }
  else{
    return vstup;
  }

}
