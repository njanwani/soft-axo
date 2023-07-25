#include <Arduino.h>
#include "IMU.h"
#include "Adafruit_BNO08x_RVC.h"


IMU::IMU() {
    IMU::polled = false;
    IMU::yaw = 0;
    IMU::roll = 0;
    IMU::pitch = 0;
}

bool IMU::begin(Stream *port) {
    ((HardwareSerial *)port)->begin(115200); // BAUD rate specified by BNO085 datasheet
    // Serial5.begin(115200);
    if (!(IMU::rvc.begin(port))) {
        Serial.println("IMU: Could not find BNO085. Check wiring?");
        return false;
    }
    return true;
}

void IMU::poll() {
    if (IMU::polled) return;
    BNO08x_RVC_Data heading;
    if ((IMU::rvc.read(&heading))) {
        IMU::polled = true;
        IMU::yaw = (float)heading.yaw;
        IMU::pitch = (float)heading.pitch;
        IMU::roll = (float)heading.roll;
    }
}

void IMU::reset() {
    IMU::polled = false;
}

bool IMU::finished() {
    return IMU::polled;
}

float IMU::get_yaw() {
    return IMU::yaw;
}

float IMU::get_pitch() {
    return IMU::pitch;
}

float IMU::get_roll() {
    return IMU::roll;
}

