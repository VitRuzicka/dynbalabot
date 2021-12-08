# Dynbalabot
- an open source dual legged balancing robot
- based on Ascento and SK8O
- my final year's project

## platform

- As the brains the esp32-wroom module is being used , soldered on custom PCB as well as:
- CAN controller - used for future communication with knee gearboxes
- two stepper motor drivers - used for driving steppers used in gearboxes until I switch to CAN
- IMU - for motion sensing (I'm using DMP of MPU6050 to apply some filtering)
- UART port for communication with hoverboard motherboard 
- port for Frsky receiver (currently using SBUS)

## features of pcbv1

- pins for Stepper drivers
- pins for endstops
- free IO
- switching circuit to replace hoverboard's mainboard button
- CAN
- mutliple voltage levels indicated by leds
just a proof of concept really...

## features of pcbv2

-same as v1 + error fixes, smaller footprint, and I used brain while designing it - this time.... though the ADC pin for measuring input voltage cannot be used because it is being used by WIFI :(
- more LEDs and thats what matters


## features of dynbalaStepper

My goal was to create a PCB, that drives stepper motors to act like a spring that returns to default position after some time. The purpose of this is that stepper motors will be located in the gearboxes, which are backdrivable hence driving robot off a step will create huge shock wave from wheels to motors and to dampen it, the custom closed loop stepper control is utilized. I could have used a premade board from BTT but:
- it is expensive $$$$
- it does not utilise CAN bus, (older versions do, but they are not sold anymore)
- they dont have endstop input, which will be used in gearboxes
- hey, its flex creating my own PCBs :D
- 
**Table of Contents**

[TOCM]

[TOC]
