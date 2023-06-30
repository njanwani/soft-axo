#ifndef AXO_CONSTANTS_H
#define AXO_CONSTANTS_H

#include <Arduino.h>

// ESC
namespace esc {
    #define ESC_L                       Serial8
    #define ESC_R                       Serial7
    constexpr float ESC_BAUD =          9600;
}

constexpr int BAUD                      = 115200;

namespace control {
    constexpr int FSR_HIGH_THRESH       = 650;
    constexpr int FSR_LOW_THRESH        = 500;
    constexpr float ALPHA_STEP          = 0;
    constexpr int MOTOR_LOW             = -300;
    constexpr int MOTOR_HIGH            = 400;

    constexpr int MIN_STEP_TIME         = 500;
}

namespace pin {

    // sensors
    constexpr uint FSR_L                = 0;
    constexpr uint FSR_R                = 0; 
    constexpr short LOADCELL            = 0;

    // Piezzo's and e-stop
    constexpr short PIEZO               = 6;
    constexpr short ESTOP               = 23;
}


namespace timer {
    constexpr unsigned int IMU{10}; // ms
    constexpr unsigned int FSR{10}; // ms (TODO good rate??)
    constexpr unsigned int LOADCELL{10}; // ms (TODO good rate??)
    constexpr unsigned int HEARTBEAT{2000}; // ms

    constexpr unsigned int MOTOR{1}; // ms (TODO: faster??)

    constexpr unsigned int SERIAL_TIMEOUT{500}; // ms
}

#endif
