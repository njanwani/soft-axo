/*
 * axOS.ino
 * This file is currently a testing file for Axo operation.
 * 
 * Amber Lab 2023.
*/

#include "motor.h"
#include "filter.h"
#include "logger.h"
#include "constants.h"

bool on = false;

Motor motor_L = Motor(&Serial5, -300, 400);
Filter FSR_L = Filter(0, 0.005, 0, 1, 500, 600); // check these values????
Logger logger = Logger(5, 50);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  motor_L.begin();
  logger.begin();
  Serial.begin(9600);
  delay(100);
  Serial.println("Beginning axOS!");
  logger.add_key("Time");
  logger.add_key("Motor_L");
  logger.add_key("Motor_R");
  logger.add_key("FSR_L");
  logger.add_key("FSR_R");
  Serial.println("PRINTING LOGS:");
  logger.print_headers();
  Serial.println();
  logger.write_to_SD();
  Serial.println();
  logger.log("FSR_R", 1);
  Serial.println("PRINTING LOGS:");
  logger.print_headers();
  Serial.println();
  logger.write_to_SD();
  Serial.println();
  delay(2000);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  if (on) {
    motor_L.move(100);
    on = false;
  } else {
    motor_L.move(0);
    on = true;
  }
  delay(1000);
}
