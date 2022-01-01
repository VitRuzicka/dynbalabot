
extern uint8_t devStatus;  
extern volatile bool PID;
extern volatile float soucasnyUhel, predUhel, error, predErr, soucetErr;


void onTime();
void dmpDataReady();
void konfiguruj_gyro();
void nactiGyro();