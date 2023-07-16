#ifndef AXO_CONSTANTS_H
#define AXO_CONSTANTS_H

#include <Arduino.h>


// General constants
constexpr size_t MONITOR_BAUD   = 115200;
constexpr size_t NUM_KEYS       = 15;
constexpr size_t MAX_KEY_SIZE   = 50;


//IMU constants
#define IMU_SHIN_L              Serial5
#define IMU_SHIN_R              Serial4

// ESC and motor constants
#define ESC_L                   Serial1
#define ESC_R                   Serial2
constexpr size_t ESC_BAUD       = 9600;
constexpr int MOTOR_LOW         = -300;
constexpr int MOTOR_HIGH        = 400;
constexpr size_t MOTOR_BAUD     = 9600;


// Control constants
constexpr int FSR_HIGH_THRESH   = 650;
constexpr int FSR_LOW_THRESH    = 500;
constexpr int Y_H               = 350;
constexpr int Y_0               = 0;
constexpr size_t T_H            = 100;

// Pins
constexpr uint FSR_L            = A0;
constexpr uint FSR_R            = 0; 
constexpr short LOADCELL        = 0;
constexpr short PIEZO           = 6;
constexpr short ESTOP           = 23;




#endif
