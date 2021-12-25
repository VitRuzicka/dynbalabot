/* TODO:
-vyčistit a zjednodušit kód :DDD kdo to sem napsal
-použití pouze pitch a roll
-zprovoznit ovladani a posouvani setpointu (řízení)

*/
/////zde se aktivují příslušné funkční celky
#define OTAupload  //musí být aktivována wifi při použití OTA
#define WIFI
//#define DIAG
#define WS
#define CAN


#include <Arduino.h>
#include <math.h>
#include "prijimac.h"
#include "mpu.h"
#include "hoverboard_komunikace.h"
#include "prevodovka.h"
#include "web.h"
#include "led.h"
#include "canbus.h"

#ifdef OTAupload
#include "ota_sluzba.h"
#endif


#define IOPIN1 27
#define IOPIN2 14
#define IOPIN3 12
#define VCCPIN 39

const char* ssid = "Net";
const char* password = "ruzicka123456789";


float Kp = 17;  // 14 pouze pro P.....18 pro spojení s D
float Kd = 0.6; //                   -0.5 pro spojení s P
float Ki = 0;  //nemá efekt na výsledek :(
#define runTime  0.08   //puvodne 5ms = 200hz PIDloop , upraveno na 100Hz ; IMU bezi na 120Hz
float offsetUhel = -1.9;   //upravit dle instalace čidla - ve st. - kalibrováno pomoci prearm tlačítka
float cilovyUhel  = 0.0;     // tento úhel bude měněn ovladačem (setpoint)
#define maxHodnota 500 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
#define DEADBAND 2   // určuje jak tenká je hranice (bod kdy systém zůstane v klidu) =====  Hystereze
#define deadMot 90  //kompenzace deadbandu motorů
volatile bool PID;
volatile int vystup;
volatile float soucasnyUhel, predUhel=0, error, predErr=0, soucetErr=0;


SBUS prijimac(Serial1);



hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

unsigned long predCasWeb = 0, predCasTel = 0, looptime=0;        // promenne k debugovaci smycce
const long intervalWeb = 500;     //interval odesilani bezne telemetrie a zaroven vypis diagnostiky
const long intervalTel = 40;      //interval odesilani rychle telemetrie
float VCC = 0.0;

void setup() {
  zelena(1);
  konfigurujAdresovatelne();
  heartbeat();


  Wire.begin();  // připojíme se na I2C sběrnici kvůli MPU
  inicializujHB();
  Serial.begin(115200);
  prijimac.begin(18,19, true);;
  delay(100);
#ifdef DIAG
  Serial.println("Bootuju");
#endif
#ifdef WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {  //pridat error ledku
    Serial.println("Nelze se pripojit! Rebootuju...");
    zelena(0);
    delay(3000);
    //ESP.restart(); //esp se nebude restartovat bez wifi
    break; 

  }
  Serial.println("Pripraveno");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
#ifdef OTAupload
  configureOTA();
  ArduinoOTA.begin();
#endif
#ifdef WS
 if( !SPIFFS.begin()){
    Serial.println("Nelze pripojit SPIFFS"); //pridat error LEDKU
    zluta(1);
    while(1);
  }
  setupWS();
#endif
  nastav_krokace();
    #ifdef CAN
  CANkonfigurace();
  #endif
//////////////////   preruseni pro PID  smycku    ///////////////////



   //80mhz, delitel 8000 tzn to udela 10000tiku za sekundu
    timer = timerBegin(0, 8000, true);   //rozdělí vteřinu na 10 000 tiků             
    timerAttachInterrupt(timer, &onTime, true);    
    timerAlarmWrite(timer, runTime*10000, true); //50 tiků = 5ms viz define     
    timerAlarmEnable(timer);          //kazdych 5ms se promenna PID zmeni na true
//////////////////////////////////////////////////////////////////////
    konfiguruj_gyro();
   

         
}

void loop() {
 WiFi.status() == WL_CONNECTED ? zelena(1) : zelena(0); //indikace pripojeni
 nactiGyro(); 
#ifdef OTAupload
ArduinoOTA.handle();
#endif
#ifdef WS
void WSloop();
#endif
/*#ifdef CAN
  CANloop();
  #endif*/
if (PID) { //spousteno podle runTime
    PID = false;
    error = (soucasnyUhel - offsetUhel) - cilovyUhel ; //radek 92 v mpu_magic.cpp
    soucetErr = soucetErr + error;  
    soucetErr = constrain(soucetErr, -maxHodnota, maxHodnota);
    //calculate output from P, I and D values
    vystup = Kp*(error) + Ki*(soucetErr)*runTime + Kd*(soucasnyUhel-predUhel)/runTime;
    predUhel = soucasnyUhel;

    if(failSafe || channels[4] < 500)
    {
      stopMot(); //nouzové zastavení
    }
    else{
      //Send(0,clip(channels[2]-230, 400, -400));  //pro testování plynu přes ovladač
      //Serial.print(clip(channels[2]-230, 300, -300));Serial.println("");
      
      Send(0, constrain(kompenzaceDEADBAND(vystup, DEADBAND, deadMot), -maxHodnota, maxHodnota)); //tento prikaz posila vystup PID na motory
      //cilovyUhel = map(channels[1], 200, 1800, 5, -5); //rizeni uhlu z ovladace...nutno zmenit za lateralni pohyb

    }
    }


if(prijimac.read(&channels[0], &failSafe, &lostFrame)){
  //kalibrace uhlu kolmého k zemi (aby robot stal rovne)
  if(channels[5] > 500){
    offsetUhel = ypr[2] * 180/M_PI;
  }

  
}





unsigned long soucasnyCas = millis();

//Serial.print("looptime: ");
//Serial.println(soucasnyCas - looptime);
  looptime = soucasnyCas;
  
  if (soucasnyCas- predCasWeb >= intervalWeb) {
    predCasWeb = soucasnyCas;
    Receive(); //zkusit presunout do nejrychlejsi casti kodu
    VCC = analogRead(VCCPIN)*(33.0/4096)+1;
    odesliTelemetrii();
    heartbeat();


#ifdef CAN
  
   CANping();  //pouze testovaci pro odesilaci desku
#endif



 #ifdef DIAG  
  Serial.print("Uhel IMU: ");
  Serial.print(ypr[3] * 180/M_PI);
  Serial.print(" PID: ");
  Serial.print(" OUTPUT: ");
  Serial.println(vystup);
  Serial.print(" roll: ");
  Serial.print(channels[0]);
  Serial.print("\t");
  Serial.print("pitch: ");
  Serial.print(channels[1]);
  Serial.print("\t");
  Serial.print("throttle: ");
  Serial.print(clip(channels[2]-230, 1000, 0));
  Serial.print("\t");
  Serial.print("ARM: ");
  Serial.println(channels[4]);
  Serial.print("\t");
  Serial.print("FS: ");
  Serial.print(failSafe);
  Serial.print("\t");
  Serial.print("Napeti: ");
  Serial.println(VCC);
 
#endif
  
  }

  if (soucasnyCas- predCasTel >= intervalTel) {
    predCasTel = soucasnyCas;
    odesliRychlouTelemetrii(soucasnyUhel, error);
  }
 
}
