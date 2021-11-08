# Dynbalabot
- is an open source dual legged balancing robot
- based on Ascento and SK8O
- is my final year's project

## platform

- As the brains the esp32-wroom module is being used , soldered on custom PCB as well as:
- CAN controller - used for future communication with knee gearboxes
- two stepper motor drivers - used for driving stepers used in gearboxes
- IMU - for motion sensing (I'm using DMP of MPU6050 to apply some filtering)
- port for communication with hoverboard motherboard 
- port for Frsky receiver (currently only PPM)

## features
- fast CPU
- interrupts for PPM decoding, IMU reading and PID loop control
- pins for endstops
- free IO
- premade switching circuit to replace hoverboard's mainboard button
- OTA (working on it...)


so far, the whole project has about 650 lines of code, and it still doesn't work :D



**Table of Contents**

[TOCM]

[TOC]
