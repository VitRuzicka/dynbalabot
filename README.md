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

- pins for Stepper sriver boards
- pins for endstops
- free IO
- premade switching circuit to replace hoverboard's mainboard button
- CAN
- mutliple voltage levels indicated by leds
just a proof of concept really...

## features of pcbv2
-same as v1 + error fixes, smaller footprint, and I used brain while designing it - this time.... 


so far, the whole project has about 650 lines of code, and it still doesn't work :D
EDIT: as of 29.11.21 we are at about 1100 and it works :D


**Table of Contents**

[TOCM]

[TOC]
