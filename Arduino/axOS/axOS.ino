/*
 * axOS.ino
 * This file is currently a testing file for Axo operation.
 * 
 * TODO:
 *    - add in "is_online/warning system that will shut down Axo for fatal errors"
 *    - test controller class
 *    - test IMU class
 * Amber Lab 2023.
*/

#include "motor.h"
#include "filter.h"
#include "logger.h"
#include "controller.h"
#include "IMU.h"
#include "bluetooth.h"
#include "constants.h"

bool on = false;

Motor motor_R(&ESC_L, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
Motor motor_L(&ESC_R, MOTOR_LOW, MOTOR_HIGH, MOTOR_BAUD);
Logger logger(NUM_KEYS, MAX_KEY_SIZE, BUILTIN_SDCARD, MONITOR_BAUD);
Controller bb_L(Y_H, Y_0, T_H);
Controller bb_R(Y_H, Y_0, T_H);
IMU shin_L();
Bluetooth ble(&Serial3, false, 33, true);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  motor_L.begin();
  logger.begin("data.csv");
  Serial.begin(MONITOR_BAUD);
  Serial.println("Beginning axOS!");
  logger.add_key("Time");
  logger.add_key("Motor_L");
  logger.add_key("Motor_R");
  logger.add_key("FSR_L");
  logger.add_key("FSR_R");
  logger.add_key("IMU_foot_L");
  logger.add_key("IMU_foot_R");
  logger.add_key("IMU_shin_L");
  logger.add_key("IMU_shin_R");
  Serial.println("PRINTING LOGS:");
  logger.print_headers();
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  size_t fsr_L = analogRead(FSR_L);
  size_t fsr_R = analogRead(FSR_R);
  
  int cmd_L = bb_L.solve(fsr_L);
  int cmd_R = bb_R.solve(fsr_R);

  motor_L.move(cmd_L);
  motor_R.move(cmd_R);
}
