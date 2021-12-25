#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <math.h>
#include "mpu.h"

uint pulzy;

MPU6050 mpu;

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


// preruseni kazdych 5ms   //preruseni kvuli PID
void IRAM_ATTR onTime() {
    PID = true;
    //pulzy++;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================
void nactiGyro(){
    // pokud není DMP připravené, nebudeme dělat nic
if (!dmpReady) return;

// zde provádíme náš kód, cyklus ověřuje, zda nemá DMP připravena nějaká data
//while (!mpuInterrupt && fifoCount < packetSize) {
    //
//}

// resetujeme proměnnou informující o přerušení vyvolané z DMP a získáme INT_STATUS byte
mpuInterrupt = false;
mpuIntStatus = mpu.getIntStatus();

// získáme velikost FIFO zásobníku
fifoCount = mpu.getFIFOCount();

// zjistíme, zda nedošlo k přetečení zásobníku
// pokud k němu dojde, je třeba optimalizovat kód v cyklu výše,
// případně přerušit provádění mezi delšími výpočty, je-li to třeba
if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    // vyčistíme zásobník
    mpu.resetFIFO();
    Serial.println(F("FIFO overflow!"));

// pokud je vše v pořádku, zpracujeme data z DMP
} else if (mpuIntStatus & 0x02) {
    // čekání na správnou délku dat, mělo by čekat velmi krátce
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
    
    soucasnyUhel = ypr[2] * 180/M_PI; //uhel na ose pitch, lze vymenit za roll
    //mpu.resetFIFO();
}
}



void konfiguruj_gyro(){
    // inicializujeme MPU-6050
Serial.println(F("Initializing I2C devices..."));
mpu.initialize();
#ifdef DIAG
// ověříme připojení k MPU-6050
Serial.println(F("Testing device connections..."));
Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

// incializujeme DMP
Serial.println(F("Initializing DMP..."));
#endif
devStatus = mpu.dmpInitialize();

// ujist// ujistíme se, že funguje
if (devStatus == 0) {
    // zapneme DMP
#ifdef DIAG
    Serial.println(F("Enabling DMP..."));
#endif
    mpu.setDMPEnabled(true);

#ifdef DIAG
    // externí přerušení Arduina nabindujeme na funkci dmpDataReady
    Serial.println(F("Enabling interrupt detection on pin 18..."));
   // digitalPinToInterrupt(intPin); //možná bude třeba aktivovat
#endif
    attachInterrupt(intPin, dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

#ifdef DIAG
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
#endif
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

}