#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {

private:
    int low;
    int high;
    HardwareSerial *port;
public:
    Motor(HardwareSerial *port, int low_lim, int high_lim, size_t MOTOR_BAUD);
    bool begin();
    void move(int pos);
};

#endif
