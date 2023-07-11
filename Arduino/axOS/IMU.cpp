#include <Arduino.h>
#include "IMU.h"
#include "Adafruit_BNO08x_RVC.h"


IMU::IMU() {
    IMU::online = false;
    IMU::yaw = 0;
    IMU::roll = 0;
    IMU::pitch = 0;
}

bool IMU::begin(Stream *port) {
    ((HardwareSerial *)port)->begin(115200); // BAUD rate specified by BNO085 datasheet
    if (!IMU::rvc.begin(port)) {
        return false;
    }
    return true;
}

void IMU::read() {
    BNO08x_RVC_Data heading;
    if (rvc.read(&heading)) {
        IMU::online = true;
        IMU::yaw = heading.yaw;
        IMU::pitch = heading.pitch;
        IMU::roll = heading.roll;
    } else {
        IMU::online = false;
    }
}

bool IMU::is_online() {
    return IMU::online;
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

