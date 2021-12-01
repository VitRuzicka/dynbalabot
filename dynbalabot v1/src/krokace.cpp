#include "prevodovka.h"
#include <AccelStepper.h>
#define ENDPIN1 34
#define ENDPIN2 35 

AccelStepper stepper1(1, 32,33);
AccelStepper stepper2(1, 25,25); 

void nastav_krokace(){
    stepper1.setMaxSpeed(MAX_SPEED);
    stepper1.setAcceleration(MAX_ACCEL);
    stepper2.setMaxSpeed(MAX_SPEED);
    stepper2.setAcceleration(MAX_ACCEL);
    stepper1.setSpeed(NORM_SPEED);
    stepper2.setSpeed(NORM_SPEED);
}
