/*
Tests reading the FSR through a voltage divider. No libraries needed, but
FSRs may need to be readjusted every once in a while. Should work with Serial
Plotter as well.
h
Lorenzo Shaikewitz, 4/17/2022
*/
#include <Metro.h>
//#include </Users/neiljanwani/Documents/soft-axo/Arduino/lib/filter/filter.h>
#include "filter.h"
#include "DFRobot_HX711.h"

// use Serial for printing to the monitor and Serial1 for wireless with Wixel.
#define SerialOut Serial
// #define SerialOut Serial1

// Constants
const short FSRPIN{A13};
const int SAMPLERATE{100};
Metro FSRtimer = Metro(SAMPLERATE); // read the FSR at SAMPLERATE Hz
Filter myfilter = mFilter(0, 0.01, 0, 1000, 20, 30);

void setup() {
    // may work better without this
    pinMode(FSRPIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    if (FSRtimer.check()) {
        unsigned int bro = analogRead(FSRPIN);
        myfilter.read(bro, 0.01);
        SerialOut.print(myfilter.get_x());
        SerialOut.print(' ');
        SerialOut.println(bro);
        if (myfilter.get_threshold_x() < 20) {
          digitalWrite(LED_BUILTIN, HIGH);
        } else {
          digitalWrite(LED_BUILTIN, LOW);
        }
    }
}
