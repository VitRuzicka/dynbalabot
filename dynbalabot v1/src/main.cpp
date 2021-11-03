/* TODO:
-vyčistit a zjednodušit kód :DDD kdo to sem napsal
-použití pouze pitch a roll
-zprovoznit ovladani a posouvani setpointu (řízení)
-přidat komunikaci s hoveboardem
-pid

*/

// Knihovna I2Cdev využívá knihovnu Wire
#include <Arduino.h>
#include <AccelStepper.h>
#include <math.h>
#include "ppm.h"
#include "ota_sluzba.h"
#include "mpu.h"
#include "hoverboard_komunikace.h"


const char* ssid = "Net";
const char* password = "ruzicka123456789";

volatile bool PID;
#define Kp  40
#define Kd  0.05
#define Ki  40
#define sampleTime  0.005 //5ms = 200hz PIDloop
#define cilovyUhel 0 //upravit dle instalace čidla - ve st.
#define maxHodnota 300 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
volatile int vystup;
volatile float soucasnyUhel, predUhel=0, error, predErr=0, soucetErr=0;



AccelStepper stepper1(1, 2,13);
AccelStepper stepper2(1, 32,33); 

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;




void setup() {

Wire.begin();  // připojíme se na I2C sběrnici kvůli MPU

HoverSerial.begin(HOVER_SERIAL_BAUD);  //komunikace s hoverboardem
Serial.begin(115200);
//pinMode(2, INPUT_PULLUP); //PPM PIN
attachInterrupt(digitalPinToInterrupt(18), cteni_signalu, FALLING); //ve skutecnosti se jedna o vzestupnou hranu protoze pin je ppm pin v PULLUP rezimu

// inicializujeme UART
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
konfiguruj_gyro();
  Serial.println("Pripraveno");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

    stepper1.setMaxSpeed(1000);
    stepper1.setAcceleration(200);
    stepper2.setMaxSpeed(1000);
    stepper2.setAcceleration(200);
    stepper1.setSpeed(500);
    stepper2.setSpeed(500);

//////////////////   preruseni pro PID  smycku    ///////////////////



    // Configure Prescaler to 80, as our timer runs @ 80Mhz
    // Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second  80000 = 1000/sec
    timer = timerBegin(0, 8000, true);                
    timerAttachInterrupt(timer, &onTime, true);    
    
  // Fire Interrupt every 1s (1 million ticks)  pulvterina by byla  500 000 
    timerAlarmWrite(timer, 50, true); //50 jako 5ms     
    timerAlarmEnable(timer);          //kazdych 5ms se promenna PID zmeni na true
//////////////////////////////////////////////////////////////////////
}

void loop() {
nactiGyro();
ArduinoOTA.handle();
//ovladej motory - pracuj s promennou vystup
if (PID) {
    PID = false;
    error = soucasnyUhel - cilovyUhel;
    soucetErr = soucetErr + error;  
    soucetErr = constrain(soucetErr, -maxHodnota, maxHodnota);
    //calculate output from P, I and D values
    vystup = Kp*(error) + Ki*(soucetErr)*sampleTime - Kd*(soucasnyUhel-predUhel)/sampleTime;
    predUhel = soucasnyUhel;
    }
/*
stepper1.runSpeed();
stepper2.runSpeed();


zpracovani_signalu(); //nutno zavolat pred pouzitim signalu z promenne pro "update udaju"

Serial.print(clip(ch[1], 1500, 0));Serial.print("\t");
Serial.print(ch[2]);Serial.print("\t");
Serial.print(ch[3]);Serial.print("\t");
Serial.print(ch[4]);Serial.print("\t");
Serial.print(ch[5]);Serial.print("\t");
Serial.print(ch[6]);Serial.print("\n");


Send(SMER, RYCHLOST);
*/
}
