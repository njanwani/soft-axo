#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <SD.h>
#include "filter.h"

class Controller {

private:
    Filter fsr;
    float y_h;
    float y_0;
    float t_h;
    float t_0;
    float t_prev;
    int x_prev;
    int stage;
    float heel_off;
public:
    Controller(float y_h, float y_0, float t_h);
    void begin(float fsr_init, float fsr_T, float fsr_low_th, float fsr_high_th);
    int solve(size_t fsr_raw);
};

#endif
