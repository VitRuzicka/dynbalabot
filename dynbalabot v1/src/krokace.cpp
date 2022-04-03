#include "prevodovka.h"
#include "config.h"
#include <AccelStepper.h>
AccelStepper stepper1(1, 32,33);
AccelStepper stepper2(1, 25,26); 

bool homed = false;

void nastav_krokace(){
    pinMode(ENDPIN1, INPUT);
    pinMode(ENDPIN2, INPUT);
    stepper1.setMaxSpeed(MAX_SPEED);
    stepper1.setAcceleration(MAX_ACCEL);
    stepper2.setMaxSpeed(MAX_SPEED);
    stepper2.setAcceleration(MAX_ACCEL);
    stepper1.setSpeed(NORM_SPEED);
    stepper2.setSpeed(NORM_SPEED);
}
void runKrokace(){
   // stepper1.runSpeed();
   // stepper2.runSpeed();
   if(!digitalRead(0)){
       //movePracovniPoloha();
   }
  
}
void homujKrokace(){
    if(!homed){ //je treba zahomovat

    if(digitalRead(ENDPIN1) ){ //motory jsou na koncovych spinacich
       stepper1.move(1 * VYJETI);
       while ( digitalRead(ENDPIN1)){
            stepper1.run();
        }
        stepper1.stop();

    } 
    if (digitalRead(ENDPIN2)){
       stepper2.move(1 * VYJETI);
       while ( digitalRead(ENDPIN2)){
            stepper2.run();
        } 
        stepper2.stop();
    }
    
    //motory jsou kdesi mimo koncaky
    stepper1.move(-1 * CELKKROKU); //couvani na koncaky
    stepper2.move(-1 * CELKKROKU);
    while(homed == false){ //dokud neni zahomovano
        if(stepper1.currentPosition() != stepper1.targetPosition() && !digitalRead(ENDPIN1)){ //homuj stepper1 dokud nenarazis na koncak nebo limit osy
            stepper1.run();
        }else{stepper1.stop(); stepper1.setCurrentPosition(0);} //zastav a vynuluj aktualni pozici

        if(stepper2.currentPosition() != stepper2.targetPosition() && !digitalRead(ENDPIN2)){ //homuj stepper2 dokud nenarazis na koncak nebo limit osy
            stepper2.run();
        }else{stepper2.stop(); stepper2.setCurrentPosition(0);} //zastav a vynuluj aktualni pozici

        if(stepper1.currentPosition() == 0 &&  stepper2.currentPosition() == 0){homed=true;}
           
        
    }
    


    }else{     //jiz je zahomovano
    //nedelej nic
    }
}
void movePracovniPoloha(){
    if(homed){
        stepper1.moveTo(PRACPOLOHA);
        stepper2.moveTo(PRACPOLOHA);
        if(stepper1.currentPosition() != stepper1.targetPosition()){
            stepper1.run();
            
        }

       if(stepper2.currentPosition() != stepper2.targetPosition()){
            stepper2.run();
            
        }
    }else{
        homujKrokace();
    }
}
