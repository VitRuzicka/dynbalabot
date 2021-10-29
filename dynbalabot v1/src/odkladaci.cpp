/* TODO:
-vyčistit a zjednodušit kód :DDD kdo to sem napsal
-použití pouze pitch a roll
-zprovoznit ovladani a posouvani setpointu (řízení)
-přidat komunikaci s hoveboardem
-pid

*/
/*
// Knihovna I2Cdev využívá knihovnu Wire
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <AccelStepper.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <math.h>


AccelStepper stepper1(1, 2,13);
AccelStepper stepper2(1, 32,33); 

// Výchozí I2C adresa MPU-6050 je 0x68, ale pokud máte jinou, zde ji můžete uvést
// AD0 low = 0x68
// AD0 high = 0x69
MPU6050 mpu;
#define M_PI 3.141459
#define intPin 23 //cislo pinu pres ktery je pripojen interrupt

bool dmpReady = false;  // když je DPM připraveno, obsahuje true
uint8_t mpuIntStatus;   // stav externího přerušení z DPM
uint8_t devStatus;      // stav poslední operace
uint16_t packetSize;    // velikost paketu z DPM
uint16_t fifoCount;     // počet bytů ve FIFO zásobníku
uint8_t fifoBuffer[64]; // FIFO zásobník

// proměnné důležité pro výpočet
Quaternion q;           // [w, x, y, z]         kvaternion
VectorFloat gravity;    // [x, y, z]            vektor setrvačnosti
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll úhly

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // true pokud DMP vyvolalo přerušení


void ICACHE_RAM_ATTR dmpDataReady() {
mpuInterrupt = true;
}

volatile bool PID;
#define Kp  40
#define Kd  0.05
#define Ki  40
#define sampleTime  0.005 //5ms = 200hz PIDloop
#define cilovyUhel 0 //upravit dle instalace čidla - ve st.
#define maxHodnota 300 // +- hodnota, která se saturuje po výstupu z PID -- upravit podle hoverboard vstupu
volatile int vystup;
volatile float soucasnyUhel, predUhel=0, error, predErr=0, soucetErr=0;
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

const char* ssid = "Net";
const char* password = "ruzicka123456789";
// preruseni kazdych 5ms
void IRAM_ATTR onTime() {
    PID = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void nactiGyro(){
    // pokud není DMP připravené, nebudeme dělat nic
if (!dmpReady) return;

// zde provádíme náš kód, cyklus ověřuje, zda nemá DMP připravena nějaká data
while (!mpuInterrupt && fifoCount < packetSize) {
    //
}

// resetujeme proměnnou informující o přerušení vyvolané z DMP a získáme INT_STATUS byte
mpuInterrupt = false;
mpuIntStatus = mpu.getIntStatus();

// získáme velikost FIFO zásobníku
fifoCount = mpu.getFIFOCount();

// zjistíme, zda nedošlo k přetečené zásobníku
// pokud k němu dojde, je třeba optimalizovat kód v cyklu výše,
// případně přerušit provádění mezi delšími výpočty, je-li to třeba
if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // vyčistíme zásobník
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

// pokud je vše v pořádku, zpracujeme data z DMP
} else if (mpuIntStatus & 0x02) {
    // čekání na správnou délku dat
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // přečteme paket ze zásobníku
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // pokud je v zásobníku víc než jeden paket, tímto zajistíme, že bude přečten v dalším cyklu
    fifoCount -= packetSize;

    // naplnění proměnných s vypočítanými hodnotami
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    //Výpis YAW/PITCH/ROLL
    Serial.print("ypr\t");
    Serial.print(ypr[0] * 180/M_PI);
    Serial.print("\t");
    Serial.print(ypr[1] * 180/M_PI);
    Serial.print("\t");
    Serial.println(ypr[2] * 180/M_PI);
    soucasnyUhel = ypr[1] * 180/M_PI; //uhel na ose pitch, lze vymenit za roll
}
}
void configureOTA(){
ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
}

void setup() {
// připojíme se na I2C sběrnici
Wire.begin();

// inicializujeme UART
Serial.begin(115200);
Serial.println("Bootuju");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Nelze se pripojit! Rebootuju...");
    delay(5000);
    ESP.restart();
  }
ArduinoOTA.setHostname("dynbalabot");
configureOTA();
ArduinoOTA.begin();

  Serial.println("Pripraveno");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

    stepper1.setMaxSpeed(1000);
    stepper1.setAcceleration(200);
    stepper2.setMaxSpeed(1000);
    stepper2.setAcceleration(200);
    stepper1.setSpeed(500);
    stepper2.setSpeed(500);

// inicializujeme MPU-6050
Serial.println(F("Initializing I2C devices..."));
mpu.initialize();

// ověříme připojení k MPU-6050
Serial.println(F("Testing device connections..."));
Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

// incializujeme DMP
Serial.println(F("Initializing DMP..."));
devStatus = mpu.dmpInitialize();

// ujistíme se, že funguje
if (devStatus == 0) {
    // zapneme DMP
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    // externí přerušení Arduina nabindujeme na funkci dmpDataReady
    Serial.println(F("Enabling interrupt detection on pin 23..."));
   // digitalPinToInterrupt(intPin); //možná bude třeba aktivovat
    attachInterrupt(intPin, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // zjistíme si, jak velké pakety DMP vrací
    packetSize = mpu.dmpGetFIFOPacketSize();
} else {
    // Když dojde k chybě, tak:
    // 1 = selhala úvodní komunikace s DMP
    // 2 = selhala aktualizace nastavení DMP
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
}

    // Configure Prescaler to 80, as our timer runs @ 80Mhz
    // Giving an output of 80,000,000 / 80 = 1,000,000 ticks / second  80000 = 1000/sec
    timer = timerBegin(0, 8000, true);                
    timerAttachInterrupt(timer, &onTime, true);    
    
  // Fire Interrupt every 1s (1 million ticks)  pulvterina by byla  500 000 
    timerAlarmWrite(timer, 50, true); //5 jako 5ms     
    timerAlarmEnable(timer);
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

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
   //tady se deje kod v pripade preruseni
    }

stepper1.runSpeed();
stepper2.runSpeed();
}
*/
