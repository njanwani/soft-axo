#include <Arduino.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

#define BUFSIZE                        128   // Size of the read buffer for incoming data
#define VERBOSE_MODE                   false  // If set to 'true' enables debug output

#define BLUEFRUIT_SWUART_RXD_PIN       9    // Required for software serial!
#define BLUEFRUIT_SWUART_TXD_PIN       10   // Required for software serial!
#define BLUEFRUIT_UART_CTS_PIN         11   // Required for software serial!
#define BLUEFRUIT_UART_RTS_PIN         -1   // Optional, set to -1 if unused
#define BLUEFRUIT_HWSERIAL_NAME      Serial7
#define BLUEFRUIT_UART_MODE_PIN        30    // Set to -1 if unused
#define FACTORYRESET_ENABLE         1

Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, 4);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

void setup(void)
{
  pinMode(BLUEFRUIT_UART_MODE_PIN, OUTPUT);
  digitalWrite(BLUEFRUIT_UART_MODE_PIN, HIGH);
  delay(1000);
  
  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit AXO"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("ERROR: Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring."));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ) {
      error(F("ERROR: Couldn't factory reset."));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
  ble.setMode(BLUEFRUIT_MODE_DATA);

  /* Change the device name to make it easier to find */
  Serial.println(F("Setting device name to 'AXO something': "));
  ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO-LEFT" ));
  ble.sendCommandCheckOK(F( "AT+BleHIDEn=On" ));
  ble.sendCommandCheckOK(F( "AT+BleKeyboardEn=On"  ));

  if (! ble.sendCommandCheckOK(F( "AT+GAPDEVNAME=AXO" )) ) {
    error(F("ERROR: Could not set device name."));
  }

  /* Add or remove service requires a reset */
  Serial.println(F("Performing a SW reset."));
  if (! ble.reset() ) {
    error(F("ERROR: Couldn't reset."));
  }

  Serial.println("Success! Starting...");
  digitalWrite(BLUEFRUIT_UART_MODE_PIN, LOW);
  delay(1000);
}

void loop(void) {
   char ret;
   if (Serial.available()) {
     ret = Serial.read();
     ble.write(ret);
   }

   if (ble.available()) {
    Serial.write(ble.read());
   }
}
