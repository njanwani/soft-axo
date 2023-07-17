/*
   axOS.ino
   This file is currently a testing file for Axo operation.

   TODO:
      - add in "is_online/warning system that will shut down Axo for fatal errors"
      - test controller class
      - test IMU class
   Amber Lab 2023.
*/

//#include "motor.h"
#include "filter.h"
#include "logger.h"
//#include "controller.h"
#include "IMU.h"
//#include "bluetooth.h"
#include "constants.h"

bool on = false;
float t;

//Motor motor_R(&ESC_L, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
//Motor motor_L(&ESC_R, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
Logger logger(NUM_KEYS, MAX_KEY_SIZE, BUILTIN_SDCARD, MONITOR_BAUD);
//Controller bb_L(Y_H, Y_0, T_H);
//Controller bb_R(Y_H, Y_0, T_H);
IMU shin_R = IMU();
IMU shin_L = IMU();
Filter rate = Filter();
//Bluetooth ble(&Serial3, false, 33, true);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //  motor_L.begin();
  shin_L.begin(&IMU_SHIN_L);
  shin_R.begin(&IMU_SHIN_R);
  logger.begin("data.csv");
  rate.begin(10, 50, 0, 1, 0, 1);
  Serial.begin(9600);
  Serial.println("Beginning axOS!");
  logger.add_key("Time");
  logger.add_key("IMU_SHIN_L_YAW");
  logger.add_key("IMU_SHIN_L_PITCH");
  logger.add_key("IMU_SHIN_L_ROLL");
  logger.add_key("IMU_SHIN_R_YAW");
  logger.add_key("IMU_SHIN_R_PITCH");
  logger.add_key("IMU_SHIN_R_ROLL");
  //  logger.add_key("IMU_FOOT_L_YAW");
  //  logger.add_key("IMU_FOOT_L_PITCH");
  //  logger.add_key("IMU_FOOT_L_ROLL");
  //  logger.add_key("IMU_FOOT_R_YAW");
  //  logger.add_key("IMU_FOOT_R_PITCH");
  //  logger.add_key("IMU_FOOT_R_ROLL");
  Serial.println("PRINTING LOGS:");
  logger.print_headers();
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  t = (float)millis();
}

void loop() {
  //  Serial.println("Success?");
  //  delay(1000);
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'q') {
      Serial.println("Shutting down...");
      logger.shutdown();
      Serial.println("Power off when ready");
      while (1) {}
    }
  }
  while (!(shin_L.finished() && shin_R.finished()) || t - (float)millis() > 15) {
    shin_L.poll();
    shin_R.poll();
    delayMicroseconds(500);
  }
  logger.log("Time", (float)millis() / 1000);
  logger.log("IMU_SHIN_L_YAW", shin_L.get_yaw());
  logger.log("IMU_SHIN_L_PITCH", shin_L.get_pitch());
  logger.log("IMU_SHIN_L_ROLL", shin_L.get_roll());
  logger.log("IMU_SHIN_R_YAW", shin_R.get_yaw());
  logger.log("IMU_SHIN_R_PITCH", shin_R.get_pitch());
  logger.log("IMU_SHIN_R_ROLL", shin_R.get_roll());
  logger.write_to_SD();
  shin_L.reset();
  shin_R.reset();
  rate.read((float)millis() - t, 1);
  if ((float)millis() - t > 20) {
    Serial.printf("HERE ==> %f\n", (float)millis() - t);
  }
  t = (float)millis();
  Serial.printf("AXO running at %.1f Hz\n", 1000 / rate.get_x());
}
