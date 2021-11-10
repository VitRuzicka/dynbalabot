/*
*TODO:
*-detekce failsafe (dva poslední kanály nikdy nemohou být 500 +-)
*
*
*/
#include<Arduino.h>
#include"prijimac.h"
uint16_t channels[16];
bool failSafe;
bool lostFrame;

uint clip(int vstup, int max, int min){
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
