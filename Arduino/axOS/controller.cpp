#include <Arduino.h>
#include "controller.h"
#include "filter.h"

Controller::Controller(float y_h, float y_0, float t_h) {
    Controller::y_0 = y_0;
    Controller::y_h = y_h;
    Controller::t_h = t_h;
}

void Controller::begin(float fsr_init, float fsr_T, float fsr_low_th, float fsr_high_th) {
    Controller::t_0 = millis();
    Controller::x_prev = fsr_init;
    Controller::t_prev = millis() - fsr_T;
    Controller::heel_off = millis();
    Controller::fsr.begin(fsr_init, fsr_T, 0, 1, fsr_low_th, fsr_high_th);
    stage = 0;
}

int Controller::solve(size_t fsr_raw) {
    /*
    * Assumes Axo state is [FSR]
    * 
    * We want to detect a RISE here...since that is heel-off
    */
    Controller::fsr.read(fsr_raw, millis() - Controller::t_prev);
    Controller::t_prev = millis();
    if (Controller::fsr.get_threshold_x() > Controller::x_prev) {
        Controller::stage = 1;
        Controller::heel_off = millis();
    } 
    
    if (Controller::stage == 1) {
        if (millis() - Controller::heel_off < Controller::t_h) {
            return Controller::y_h;
        } else {
            Controller::stage = 0;
            return Controller::y_0;
        }
    } else {
        return Controller::y_0;
    }
}