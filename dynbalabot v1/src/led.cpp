#include "led.h"
bool stav1 = false;
bool stav2 = false;

void zelena(uint8_t stav){ //tri stavy 0,1 a 2 (toggle)
pinMode(SIG1, OUTPUT);
if(stav == 0){
    digitalWrite(SIG1, 0);
}
else if(stav == 1){
    digitalWrite(SIG1, 1);
}
else{
    digitalWrite(SIG1, stav1);
    stav1 = !stav1;
}
}

void zluta(uint8_t stav){
pinMode(SIG2, OUTPUT);
if(stav == 0){
    digitalWrite(SIG2, 0);
}
else if(stav == 1){
    digitalWrite(SIG2, 1);
}
else{
    digitalWrite(SIG2, stav2);
    stav2 = !stav2;
}
}