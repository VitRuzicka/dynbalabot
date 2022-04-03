#include "led.h"
#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    1
#define BRIGHTNESS  50
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


bool stav1 = false;
bool stav2 = false;

void konfigurujAdresovatelne(){
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
}
void heartbeat(){
    leds[0] = CHSV(random8(),255,255);
    FastLED.show();
}
void nastavBarvu(uint8_t redC=0, uint8_t greenC=0, uint8_t blueC=0){
    leds[0].red = redC;
    leds[0].green = greenC;
    leds[0].blue = blueC;
    FastLED.show();
    
}
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