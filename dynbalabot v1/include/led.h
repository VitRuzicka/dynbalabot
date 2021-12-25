
#include <Arduino.h>

#define SIG1 13
#define SIG2 2
extern bool stav1;
extern bool stav2;
void zelena(uint8_t);
void zluta(uint8_t);
void konfigurujAdresovatelne();
void heartbeat();