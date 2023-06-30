#include <Arduino.h>
#include "motor.h"

Motor::Motor(HardwareSerial *port, int low_lim, int high_lim) {
    Serial.begin(9600);
    Motor::port = port;
    Motor::low = low_lim;
    Motor::high = high_lim;
}

void Motor::begin() {
    port->begin(9600);
    delay(100);
    Motor::port->printf("[L%d]\n", Motor::low);
    delay(100);
    Motor::port->printf("[H%d]\n", Motor::high);
}

void Motor::move(int pos) {
    if (pos >= Motor::low && pos <= Motor::high) {
        Motor::port->printf("[M%d]\n", pos);
        Serial.printf("[M%d]\n", pos);
        delay(10);
    }
}

