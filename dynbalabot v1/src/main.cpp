/* TODO:
-vyčistit a zjednodušit kód :DDD kdo to sem napsal
-použití pouze pitch a roll
-zprovoznit ovladani a posouvani setpointu (řízení)
-přidat komunikaci s hoveboardem
-pid

*/

// Knihovna I2Cdev využívá knihovnu Wire
#include <Arduino.h>
#include <math.h>
#include "ppm.h"
#include "ota_sluzba.h"
#include "mpu.h"
#include "hoverboard_komunikace.h"
#include "prevodovka.h"

const char* ssid = "Net";
const char* password = "ruzicka123456789";

volatile bool PID;
#define Kp  0
#define Kd  0.00
#define Ki  0
#define sampleTime  0.005 //5ms = 200hz PIDloop
#define cilovyUhel 0 //upravit dle instalace čidla - ve st.
#define maxHodnota 300 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
volatile int vystup;
volatile float soucasnyUhel, predUhel=0, error, predErr=0, soucetErr=0;




hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned long predchoziCas = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 10;  


void setup() {

  Wire.begin();  // připojíme se na I2C sběrnici kvůli MPU

  void inicializujHB();
  Serial.begin(115200);
  delay(100);
  Serial.println("Bootuju");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Nelze se pripojit! Rebootuju...");
    delay(3000);
    //ESP.restart(); //esp se nebude restartovat bez wifi
    break; 
  }

  configureOTA();
  ArduinoOTA.setHostname("dynbalabot");
  ArduinoOTA.begin();
  Serial.println("Pripraveno");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  nastav_krokace();
  
//////////////////   preruseni pro PID  smycku    ///////////////////



    // Configure Prescaler to 80, as our timer runs @ 80Mhz
    // Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second  80000 = 1000/sec
    timer = timerBegin(0, 8000, true);                
    timerAttachInterrupt(timer, &onTime, true);    
    timerAlarmWrite(timer, sampleTime*10000, true); //5ms viz define     
    timerAlarmEnable(timer);          //kazdych 5ms se promenna PID zmeni na true
//////////////////////////////////////////////////////////////////////
    konfiguruj_gyro();
    attachInterrupt(digitalPinToInterrupt(18), cteni_signalu, FALLING); //zapojeni preruseni pro PPM signal


         
}

void loop() {
nactiGyro();
ArduinoOTA.handle();
//ovladej motory - pracuj s promennou vystup
/*
if (PID) {
    PID = false;
    error = soucasnyUhel - cilovyUhel; //radek 92 v mpu_magic.cpp
    soucetErr = soucetErr + error;  
    soucetErr = constrain(soucetErr, -maxHodnota, maxHodnota);
    //calculate output from P, I and D values
    vystup = Kp*(error) + Ki*(soucetErr)*sampleTime - Kd*(soucasnyUhel-predUhel)/sampleTime;
    predUhel = soucasnyUhel;
    }










Send(0,0);
*/

zpracovani_signalu(); //nutno zavolat pred pouzitim signalu z promenne pro "update udaju"


unsigned long soucasnyCas = millis();
//debug serial info:
  if (soucasnyCas- predchoziCas >= interval) {
    // save the last time you blinked the LED
    predchoziCas = soucasnyCas;
    Serial.print("pulzy: ");
    Serial.println((1000/interval)*pulzy);
    pulzy = 0;

  Serial.print(clip(ch[1], 1500, 0));Serial.print("\t");
  Serial.print(ch[2]);Serial.print("\t");
  Serial.print(clip(ch[3], 1000, 0));Serial.print("\t");
  Serial.print(ch[4]);Serial.print("\t");
  Serial.print(ch[5]);Serial.print("\t");
  Serial.print(ch[6]);Serial.print("\n");

  Serial.print("ypr\t");
  Serial.print(ypr[0] * 180/M_PI);
  Serial.print("\t");
  Serial.print(ypr[1] * 180/M_PI);
  Serial.print("\t");
  Serial.println(ypr[2] * 180/M_PI);

  //posílání stavu do hoverbaordu
  Send(0, clip(ch[3], 1000, 0));

  }
}
