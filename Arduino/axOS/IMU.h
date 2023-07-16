#ifndef IMU_H
#define IMU_H
#include "Adafruit_BNO08x_RVC.h"


class IMU {

    private:
        Adafruit_BNO08x_RVC rvc;
        bool polled;
        float yaw;
        float pitch;
        float roll;
    public:
        IMU();
        bool begin(Stream *port);
        void poll();
        bool finished();
        void reset();
        float get_yaw();
        float get_pitch();
        float get_roll();
};
#endif
