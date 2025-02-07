/*
Main code to run the ankle exoskeleton. Sensor gathering/printing, low-level
motor control, and other secondary functions are abstracted into the Axo class.
This file focuses on the high-level control scheme.

CONTROL: BANG BANG AT PUSHOFF

Lorenzo Shaikewitz, 4/19/2022
*/
#include "Axo.h"
#include "constants.h"
// #include "curves.h"
#include <Metro.h>

Axo axo;

Metro controlTimer{2}; // 500 Hz (2 ms)
unsigned long startTime{};
unsigned long startTimeFSR{};
unsigned long stepTime{2000};
bool fsrHigh{};
float backPulseTime{500};

float fsrAccum{};
float stepTimeAccum{1500};

float triggerPercent{0.3};
float newTriggerPercent{0.3};
float motorOnPercent{0.35};
float newMotorOnPercent{0.35};

void setup() {
    // start up sensors
    axo.begin();
    SerialOut.println("MODE: BANG BANG AT PUSHOFF");
    axo.setBlueLED(HIGH);

    // wait for startup command
    SerialOut.println("Waiting for startup command...");
    while (1) {
        if (SerialOut.available()) {
            if (SerialOut.read() == 's') {
                SerialOut.println("Startup received!");
                axo.resetEstop();
                break;
            }
        }
        // else if (axo.resetEstop()) {
        //     SerialOut.println("Startup received!");
        //     delay(500);
        //     axo.resetEstop();
        //     break;
        // }
        delay(100);
    }
    axo.printKey();
    axo.setBlueLED(LOW);
    axo.setGreenLED(HIGH);

    // start motors
    axo.beginMotors();

    // reset control timer
    controlTimer.reset();
    startTime = millis();
}


void loop() {
    // angle control updating
    if (controlTimer.check()) {
        // compute the time elapsed
        unsigned long currentTime = millis();
        unsigned long dt = currentTime - startTime;
        unsigned long dtFSR = currentTime - startTimeFSR;

        // execute control
        bangBangAtPushoff(axo, dt, stepTimeAccum);

        // check the FSR for a step
        int fsr = axo.getFSR();
        fsrAccum = control::ALPHA_FSR*fsr + (1.0 - control::ALPHA_FSR)*fsrAccum;
        if (fsr > control::FSR_HIGH_THRESH) {
            // want to record only the first heel strike
            if (!fsrHigh) {
                fsrHigh = true;
                stepTime = dt;
                stepTimeAccum = control::ALPHA_STEP*stepTime + (1.0 - control::ALPHA_STEP)*stepTimeAccum;

                startTime = currentTime;
                triggerPercent = newTriggerPercent;
                motorOnPercent = newMotorOnPercent;
                #ifndef SUPPRESS_LOG
                unsigned long t{millis() - axo.startTime()};
                SerialOut.printf("\nLOG,%d | Step Recorded! %u", t, dt);
                #endif
            }
        } else if (fsr < control::FSR_LOW_THRESH) {
            fsrHigh = false;
        }
    }

    axo.spin();

    if (SerialOut.available()) {
        char c = SerialOut.read();

        switch (c) {
            case 'x': {
                // stop!
                #ifndef SUPPRESS_LOG
                unsigned long t{millis() - axo.startTime()};
                SerialOut.printf("\nLOG,%d | Received x. Terminating Axo...",t);
                #endif
                axo.stopMotors();
                delay(1000);
                axo.detachMotors();
                axo.setBlueLED(HIGH);
                axo.setGreenLED(LOW);

                // pause the loop
                while (1);
                break;
            }

            case 'a': {
                // expect a float percentage
                axo.stopMotors();
                newTriggerPercent = SerialOut.parseFloat();
                if (newTriggerPercent > 1.0)
                    newTriggerPercent = 1.0;
                if (newTriggerPercent < 0.0)
                    newTriggerPercent = 0.0;
                #ifndef SUPPRESS_LOG
                SerialOut.printf("LOG,Received a. Setting trigger percent to %f\n", newTriggerPercent);
                #endif
                break;
            }

            case 't': {
                // expect a float percentage
                axo.stopMotors();
                newMotorOnPercent = SerialOut.parseFloat();
                if (newMotorOnPercent > 1.0)
                    newMotorOnPercent = 1.0;
                if (newMotorOnPercent < 0.0)
                    newMotorOnPercent = 0.0;
                #ifndef SUPPRESS_LOG
                SerialOut.printf("LOG,Received a. Setting run time to %d\n", newMotorOnPercent);
                #endif
                break;
            }

            default: {
                #ifndef SUPPRESS_LOG
                unsigned long t{millis() - axo.startTime()};
                SerialOut.printf("\nLOG,%d | Received %c. No action taken.",t,c);
                #endif
                break;
            }
        }
    }
}



static constexpr int maxAngle{650};
static constexpr int zeroAngle{0};

void bangBangAtPushoff(Axo& axo, unsigned long deltat, unsigned long predStepTime) {
    float a{ triggerPercent*predStepTime };                 // pulse starts
    float b{ a + motorOnPercent*predStepTime };             // pulse ends
    float c{ min(predStepTime, b + backPulseTime)};         // buffer time for backwards pulse
    // SerialOut.printf("dt: %.4f, a: %.4f, b: %.4f, c: %.4f", deltat, a, b, c);

    // Follow a piecewise curve
    // BEFORE *TRIGGERPERCENT*
    if (deltat < a) {
        // initially no motion
        axo.stopMotors();
    } else if ((deltat >= a) && (deltat <= b)) {
        // spike at pushoff
        axo.setAngle(maxAngle);
    } else if ((deltat > b) && (deltat <= c)) {
        // brief backwards pulse
        axo.setAngle(zeroAngle);
    } else if (deltat > c) {
        // return to no motion
        axo.stopMotors();
    }
}