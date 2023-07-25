/*
   axOS.ino
   This file is currently a testing file for Axo operation.

   TODO:
      - add in "is_online/warning system that will shut down Axo for fatal errors"
      - test controller class
      - test IMU class
   Amber Lab 2023.
*/

#include "motor.h"
#include "filter.h"
#include "logger.h"
//#include "controller.h"
#include "IMU.h"
#include "bluetooth.h"
#include "tones.h"
#include "constants.h"

bool on = false;
float t;
bool up = false;

Motor motor_R(&ESC_L, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
Motor motor_L(&ESC_R, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
Logger logger(NUM_KEYS, MAX_KEY_SIZE, BUILTIN_SDCARD, MONITOR_BAUD);
//Controller bb_L(Y_H, Y_0, T_H);
//Controller bb_R(Y_H, Y_0, T_H);
IMU shin_L = IMU();
IMU shin_R = IMU();
IMU foot_L = IMU();
IMU foot_R = IMU();
Filter rate = Filter();
Bluetooth ble(&Serial7, false, 30, true);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  //  motor_L.begin();
  Serial.begin(9600);
  Serial.println("Beginning axOS!");
  bool began = true;
  began = began && motor_L.begin()
          && motor_R.begin()
          && shin_L.begin(&IMU_SHIN_L)
          && shin_R.begin(&IMU_SHIN_R)
          && foot_L.begin(&IMU_FOOT_L)
          && foot_R.begin(&IMU_FOOT_R)
          && logger.begin("data.csv")
          && rate.begin(10, 50, 0, 1, 0, 1)
          && ble.begin(MONITOR_BAUD);
  if (!began) {
    while (1) {
      axo_error(PIEZO, 15, 5);
      delay(100);
      axo_error(PIEZO, 15, 5);
      delay(5000);
    }
  }
  logger.add_key("Time");
  logger.add_key("IMU_SHIN_L_YAW");
  logger.add_key("IMU_SHIN_L_PITCH");
  logger.add_key("IMU_SHIN_L_ROLL");
  logger.add_key("IMU_SHIN_R_YAW");
  logger.add_key("IMU_SHIN_R_PITCH");
  logger.add_key("IMU_SHIN_R_ROLL");
  logger.add_key("FSR_HEEL_L_RAW");
  logger.add_key("FSR_HEEL_R_RAW");
  logger.add_key("IMU_FOOT_L_YAW");
  logger.add_key("IMU_FOOT_L_PITCH");
  logger.add_key("IMU_FOOT_L_ROLL");
  logger.add_key("IMU_FOOT_R_YAW");
  logger.add_key("IMU_FOOT_R_PITCH");
  logger.add_key("IMU_FOOT_R_ROLL");
  Serial.println("PRINTING LOGS:");
  logger.print_headers();
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  armed(PIEZO, 150, 1);
  pinMode(5, OUTPUT);
  t = (float)millis();
}

void log_data() {
  logger.log("Time", (float)millis() / 1000);
  logger.log("IMU_SHIN_L_YAW", shin_L.get_yaw());
  logger.log("IMU_SHIN_L_PITCH", shin_L.get_pitch());
  logger.log("IMU_SHIN_L_ROLL", shin_L.get_roll());
  logger.log("IMU_SHIN_R_YAW", shin_R.get_yaw());
  logger.log("IMU_SHIN_R_PITCH", shin_R.get_pitch());
  logger.log("IMU_SHIN_R_ROLL", shin_R.get_roll());
  logger.log("IMU_FOOT_L_YAW", shin_L.get_yaw());
  logger.log("IMU_FOOT_L_PITCH", shin_L.get_pitch());
  logger.log("IMU_FOOT_L_ROLL", shin_L.get_roll());
  logger.log("IMU_FOOT_R_YAW", shin_R.get_yaw());
  logger.log("IMU_FOOT_R_PITCH", shin_R.get_pitch());
  logger.log("IMU_FOOT_R_ROLL", shin_R.get_roll());
  logger.log("FSR_HEEL_L_RAW", analogRead(A9));
  logger.log("FSR_HEEL_R_RAW", analogRead(A8));
  logger.write_to_SD();
}

void shutdown_axo() {
  ble.write("Shutting down...\n", 17);
  Serial.println("Shutting down...");
  logger.shutdown();
  ble.write("Power off when ready\n", 21);
  Serial.println("Power off when ready");
  end_axo(PIEZO, 250, 1);
  while (1) {}
}

void ui(char c) {
  if (c == 'q') {
    shutdown_axo();
  } else if (c == 'r') {
    char buf[100];
    int num = snprintf(buf, 100, "AXO running at %.1f Hz\n", 1000 / rate.get_x());
    ble.write(buf, num);
  } 
  else if (c == 'm') {
    if (!up) {
      motor_R.move(100);
      up = true;
    } else {
      motor_R.move(0);
      up = false;
    }
  }
}

void loop() {
  // User Interfacing...
  if (ble.available()) {
    char c = ble.read();
    ui(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    ui(c);
  }

//  // IMU polling...
//  while (!(shin_L.finished() && shin_R.finished() && foot_L.finished() && foot_R.finished()) || t - (float)millis() > 15) {
//    shin_L.poll();
//    shin_R.poll();
//    foot_L.poll();
//    foot_R.poll();
//  }

  // Logging
  log_data();

  // Reset
  shin_L.reset();
  shin_R.reset();
  foot_L.reset();
  foot_R.reset();
  rate.read((float)millis() - t, 1);
  t = (float)millis();
}
