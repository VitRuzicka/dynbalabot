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
#include "prijimac.h"
#include "ota_sluzba.h"
#include "mpu.h"
#include "hoverboard_komunikace.h"
#include "prevodovka.h"

const char* ssid = "Net";
const char* password = "ruzicka123456789";

volatile bool PID;
#define Kp  17  // 14 pouze pro P.....18 pro spojení s D
#define Kd  -0.6 //                   -0.5 pro spojení s P
#define Ki  0  //nemá efekt na výsledek :(
#define runTime  0.01 //5ms = 200hz PIDloop , upraveno na 100Hz
float offsetUhel = -1.9; //upravit dle instalace čidla - ve st. - kalibrováno pomoci prearm tlačítka
float cilovyUhel  = 0.0;     // tento úhel bude měněn ovladačem (setpoint)
#define maxHodnota 500 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
#define DEADBAND 2   // určuje jak tenká je hranice (bod kdy systém zůstane v klidu) =====  Hystereze
#define deadMot 90  //kompenzace deadbandu motorů
volatile int vystup;
volatile float soucasnyUhel, predUhel=0, error, predErr=0, soucetErr=0;


SBUS prijimac(Serial1);



hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned long predchoziCas = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 100;  


void setup() {

  Wire.begin();  // připojíme se na I2C sběrnici kvůli MPU
  
  //void inicializujHB();
  Serial2.begin(115200);
  Serial.begin(115200);
  prijimac.begin(18,19, true);;
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
  ArduinoOTA.begin();
  Serial.println("Pripraveno");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  nastav_krokace();
  
//////////////////   preruseni pro PID  smycku    ///////////////////



    // Configure Prescaler to 80, as our timer runs @ 80Mhz
    // Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second  80000 = 1000/sec
    timer = timerBegin(0, 8000, true);   //rozdělí vteřinu na 10 000 tiků             
    timerAttachInterrupt(timer, &onTime, true);    
    timerAlarmWrite(timer, runTime*10000, true); //50 tiků = 5ms viz define     
    timerAlarmEnable(timer);          //kazdych 5ms se promenna PID zmeni na true
//////////////////////////////////////////////////////////////////////
    konfiguruj_gyro();
   

         
}

void loop() {
  
nactiGyro(); 
ArduinoOTA.handle();

//ovladej motory - pracuj s promennou vystup

if (PID) {
    PID = false;
    error = (soucasnyUhel - offsetUhel) - cilovyUhel ; //radek 92 v mpu_magic.cpp
    soucetErr = soucetErr + error;  
    soucetErr = constrain(soucetErr, -maxHodnota, maxHodnota);
    //calculate output from P, I and D values
    vystup = Kp*(error) + Ki*(soucetErr)*runTime - Kd*(soucasnyUhel-predUhel)/runTime;
    predUhel = soucasnyUhel;

    if(failSafe || channels[4] < 500)
    {
      Send(0,0);
    }
    else{
      //Send(0,clip(channels[2]-230, 400, -400));  //pro testování plynu přes ovladač
      //Serial.print(clip(channels[2]-230, 300, -300));Serial.println("");
      if(vystup > DEADBAND){
        vystup += deadMot;

      }
      else if(vystup < -DEADBAND){
        vystup -= deadMot;
      }
      Send(0, clip(vystup, maxHodnota, -maxHodnota));
      cilovyUhel = map(channels[1], 200, 1800, 5, -5);

    }
    }


if(prijimac.read(&channels[0], &failSafe, &lostFrame)){
  if(channels[5] > 500){
    offsetUhel = ypr[2] * 180/M_PI;
  }

  
}





unsigned long soucasnyCas = millis();
//debug serial info:
  if (soucasnyCas- predchoziCas >= interval) {
    // save the last time you blinked the LED
    predchoziCas = soucasnyCas;
    //Serial.print("pulzy: ");
    //Serial.println((1000/interval)*pulzy);
    //pulzy = 0;
    Receive();
    //Serial.println(channels[1]);
  /*  
Serial.print("Roll: ");
Serial.print(ypr[2] * 180/M_PI);
Serial.print(" PID: ");
Serial.println(vystup);



  Serial.print("roll 0: ");
  Serial.print(channels[0]);
  Serial.print("\t");
  Serial.print("pitch 1: ");
  Serial.print(channels[1]);
  Serial.print("\t");
  Serial.print("throttle: ");
  Serial.print(clip(channels[2]-230, 1000, 0));
  Serial.print("\t");
  Serial.print("ARM: ");
  Serial.println(channels[4]);
  Serial.print("\t");
  Serial.print(failSafe);

  
  Serial.print("ypr\t");
  Serial.print(ypr[0] * 180/M_PI);
  Serial.print("\t");
  Serial.print(ypr[1] * 180/M_PI);
  Serial.print("\t");
  Serial.println(ypr[2] * 180/M_PI);
 
 */
  
  }
}
